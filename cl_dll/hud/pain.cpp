
// NEW FILE.  More central place for pain-related methods, moved from health.cpp

#include "pain.h"
#include "external_lib_include.h"
//#include "STDIO.H"
//#include "STDLIB.H"
//#include "MATH.H"
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>


EASY_CVAR_EXTERN(painFlashSuitless)
EASY_CVAR_EXTERN(painArrowColorMode)
EASY_CVAR_EXTERN(painFlashColorMode)
EASY_CVAR_EXTERN(painFlashDmgMin)
EASY_CVAR_EXTERN(painFlashDmgExMult)
EASY_CVAR_EXTERN(painFlashCumulativeMinDrowning)
EASY_CVAR_EXTERN(painFlashCumulativeMax)
EASY_CVAR_EXTERN(painFlashDrawOpacityMax)
EASY_CVAR_EXTERN(painArrowDrawOpacityMin)
EASY_CVAR_EXTERN(painArrowDrawOpacityMax)
EASY_CVAR_EXTERN(painFlashFadeMult)
EASY_CVAR_EXTERN(painArrowFadeMult)
EASY_CVAR_EXTERN(painFlashDirTolerance)
EASY_CVAR_EXTERN(painArrowCumulativeAppearMin)
EASY_CVAR_EXTERN(painArrowCumulativeDmgJump)
EASY_CVAR_EXTERN(painFlashPrintouts)
EASY_CVAR_EXTERN(painFlashArmorBlock)
EASY_CVAR_EXTERN(painFlashDrownMode)



// MODDD - changed to refer to the whitened version (more flexible w/ different colors)
//#define PAIN_NAME "sprites/%d_pain.spr"
#define PAIN_NAME "sprites/%d_painmod.spr"

// also, it appears that this is ununsed.  Perhaps it stored the damage-icons at some point?  File no longer exist.
//#define DAMAGE_NAME "sprites/%d_dmg.spr"




CHudPain::CHudPain(void) {
	m_painFlashSprite = 0;

}

int CHudPain::Init(void){

	// Always active, although I don't do much.
	// Actually, HUD_ACTIVE set moved to the "FirstAppr" message (when the player first connects to the server,
	// although this also includes after a map transition).
	//m_iFlags |= HUD_ACTIVE;

	m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;
	m_fAttackFrontDamage = m_fAttackRearDamage = m_fAttackRightDamage = m_fAttackLeftDamage = 0;

	// This comes from a message sent by the player, "Yes" or "No".
	playerIsDrowning = FALSE;


	gHUD.AddHudElem(this);
	return 1;
}

int CHudPain::VidInit(void) {

	m_painFlashSprite = 0;

	// Also, note that the name PAIN_NAME has been altered to use a white-version (the red isn't as flexible with different colors)
	//if (!m_painFlashSprite)
	//m_painFlashSprite = LoadSprite(PAIN_NAME);


	return 1;
}


// NOTICE!  Don't draw the painflash in here.  Leave that call up to outside logic to draw
// before or after all other GUI (in hud_redraw.cpp)
int CHudPain::Draw(float fTime) {

	if (gHUD.frozenMem) {
		//Don't while frozen.
		return 0;
	}

	
	if (!m_painFlashSprite)
	m_painFlashSprite = LoadSprite(PAIN_NAME);


	return 1;
}

void CHudPain::Reset(void) {

	// make sure the pain compass is cleared when the player respawns
	m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;
	m_fAttackFrontDamage = m_fAttackRearDamage = m_fAttackRightDamage = m_fAttackLeftDamage = 0;

	//MODDD - new
	cumulativeFade = 0;
	fAttackFrontMem = 0;
	fAttackRearMem = 0;
	fAttackRightMem = 0;
	fAttackLeftMem = 0;

}






// !!! Moved here from health.cpp since it no longer calls this method.
// In this file, it is called to put the pain flash before or after all other GUI.
//-----------------------------------------------------------------------------------------
// This method draws the arrows of varrying transparency to suggest damage direction and intensity.
// Also draws the screen flash.
int CHudPain::DrawPain(float flTime)
{
	if (EASY_CVAR_GET(hideDamage) >= 1) {
		//don't do it.
		return 1;
	}

	if (EASY_CVAR_GET(painFlashSuitless) == 0 && !(gHUD.m_iWeaponBits & (1 << (WEAPON_SUIT))))
		return 1;


	//MODDD - no longer effective for the new cumulative method.
	/*
	if (!(m_fAttackFront || m_fAttackRear || m_fAttackLeft || m_fAttackRight))
		return 1;
	*/

	int r, g, b;
	int x, y, a, shade;

	// TODO:  get the shift value of the health
	a = 255;	// max brightness until then

	//MODDD - changing the multiple on the Delta changes the rate of fade change.
	//float fFade = gHUD.m_flTimeDelta * 2;
	const float fFade = gHUD.m_flTimeDelta * EASY_CVAR_GET(painFlashFadeMult);

	const float fFadeARROW = gHUD.m_flTimeDelta * EASY_CVAR_GET(painArrowFadeMult);



	//MODDD - FINAL PAIN SYSTEM:  a combination of the cumulative fade and fade arrows:

	//So, if the m_fAttack<dir> is above 0 (updated recently), the copy of that direction var for this file (fAttack<dir>Mem) is assigned it.
	//The m_fAttack<dir> is set to 0 so that it does not increment cumulativeFade multiple times for one call.
	//fAttack<dir>Mem is then used by the old fade effect to work with the version of the time-based var that would behave like the old
	//were never changed.

	//ALSO: on drowning, the pain effect may get a minimum above 0 (constant red).
	float cumulativeFadeMinimum = 0;

	if (EASY_CVAR_GET(painFlashDrownMode) == 1 && playerIsDrowning) {
		cumulativeFadeMinimum = EASY_CVAR_GET(painFlashCumulativeMinDrowning);
	}
	//easyForcePrintLine("OH naw %.2f %d %.2f", EASY_CVAR_GET(painFlashDrownMode), playerIsDrowning, EASY_CVAR_GET(painFlashCumulativeMinDrowning));


	int allowArrows = 1;
	int allowFlash = 1;

	//if(gHUD.recentDamageBitmask & DMG_DROWN){
	if (playerIsDrowning) {
		if (EASY_CVAR_GET(painFlashDrownMode) == 2) {
			//allowFlash = 0;

			if (cumulativeFadeDrown > 0.00) {
				//maxAttackFade += 0.2;
				//GetPainColor(r,g,b);
				getPainColorMode((int)EASY_CVAR_GET(painFlashColorMode), r, g, b);

				//Clip cumulativeFade to 1.00, as you can't shade anymore intensely than 255, opaque.
				//(as opaque as the engine will allow, it seems)

				if (EASY_CVAR_GET(painFlashColorMode) > 0) {
					//draw it.
					shade = a * min(cumulativeFadeDrown, 1.00);
					//shade = a * 1;
					ScaleColors(r, g, b, shade);
					FillRGBA(0, 0, ScreenWidth, ScreenHeight, r, g, b, shade);
				}

			}
			//reduce the cumulative fade by "fFade", time.
			if (cumulativeFadeDrown > 0) {
				cumulativeFadeDrown = max(0, cumulativeFadeDrown - fFade);
			}


		}
		else {

		}
	}

	cumulativeFade = max(cumulativeFadeMinimum, cumulativeFade);


	if (m_fAttackFront > 0) {
		const float fadeContributionARROW = m_fAttackFront * EASY_CVAR_GET(painArrowCumulativeDmgJump);
		fAttackFrontMem = fadeContributionARROW;
		const float fadeContribution = EASY_CVAR_GET(painFlashDmgMin) + m_fAttackFront * m_fAttackFrontDamage * (EASY_CVAR_GET(painFlashDmgExMult));
		//const float fadeContributionARROW = ...
		cumulativeFade += fadeContribution;
		if (EASY_CVAR_GET(painFlashPrintouts) == 1)easyForcePrintLine("OW front: raw:%.2f dam:%.2f result:%.2f resultA:%.2f", m_fAttackRight, m_fAttackRightDamage, fadeContribution, fadeContributionARROW);
		m_fAttackFront = 0;
	}
	if (m_fAttackRear > 0) {
		const float fadeContributionARROW = m_fAttackRear * EASY_CVAR_GET(painArrowCumulativeDmgJump);
		fAttackRearMem = fadeContributionARROW;
		const float fadeContribution = EASY_CVAR_GET(painFlashDmgMin) + m_fAttackRear * m_fAttackRearDamage * EASY_CVAR_GET(painFlashDmgExMult);
		cumulativeFade += fadeContribution;
		if (EASY_CVAR_GET(painFlashPrintouts) == 1)easyForcePrintLine("OW rear raw:%.2f dam:%.2f result:%.2f resultA:%.2f", m_fAttackRight, m_fAttackRightDamage, fadeContribution, fadeContributionARROW);
		m_fAttackRear = 0;
	}
	if (m_fAttackLeft > 0) {
		const float fadeContributionARROW = m_fAttackLeft * EASY_CVAR_GET(painArrowCumulativeDmgJump);
		fAttackLeftMem = fadeContributionARROW;
		const float fadeContribution = EASY_CVAR_GET(painFlashDmgMin) + m_fAttackLeft * m_fAttackLeftDamage * (EASY_CVAR_GET(painFlashDmgExMult));
		cumulativeFade += fadeContribution;
		if (EASY_CVAR_GET(painFlashPrintouts) == 1)easyForcePrintLine("OW left raw:%.2f dam:%.2f result:%.2f resultA:%.2f", m_fAttackRight, m_fAttackRightDamage, fadeContribution, fadeContributionARROW);
		m_fAttackLeft = 0;
	}
	if (m_fAttackRight > 0) {
		const float fadeContributionARROW = m_fAttackRight * EASY_CVAR_GET(painArrowCumulativeDmgJump);
		fAttackRightMem = fadeContributionARROW;
		const float fadeContribution = EASY_CVAR_GET(painFlashDmgMin) + m_fAttackRight * m_fAttackRightDamage * (EASY_CVAR_GET(painFlashDmgExMult));
		cumulativeFade += fadeContribution;
		if (EASY_CVAR_GET(painFlashPrintouts) == 1)easyForcePrintLine("OW right dir:%.2f dam:%.2f result:%.2f resultA:%.2f", m_fAttackRight, m_fAttackRightDamage, fadeContribution, fadeContributionARROW);
		m_fAttackRight = 0;
	}

	//cumulativeFade is capped at "1.2", so if lots of damage is taken, it will remain at max intensity for 0.2 seconds at most
	//(if no further damage comes to bump it again)

	//was 1.2 on the right!!!
	cumulativeFade = min(cumulativeFade, EASY_CVAR_GET(painFlashCumulativeMax));
	//!!!!!

	//easyPrintLine("cumula %.2f", cumulativeFade);

	if (cumulativeFade > 0.00) {
		//maxAttackFade += 0.2;
		//GetPainColor(r,g,b);
		getPainColorMode((int)EASY_CVAR_GET(painFlashColorMode), r, g, b);

		//Clip cumulativeFade to 1.00, as you can't shade anymore intensely than 255, opaque.
		//(as opaque as the engine will allow, it seems)

		if (EASY_CVAR_GET(painFlashColorMode) > 0 && allowFlash == 1) {
			//draw it.

			//WAS 1.0 on the right!!
			shade = a * min(cumulativeFade * EASY_CVAR_GET(painFlashDrawOpacityMax), EASY_CVAR_GET(painFlashDrawOpacityMax));
			//!!!!!

			//shade = a * 1;
			ScaleColors(r, g, b, shade);
			FillRGBA(0, 0, ScreenWidth, ScreenHeight, r, g, b, shade);
		}
	}
	//EASY_CVAR_GET(painArrowDrawOpacityMax)

	//reduce the cumulative fade by "fFade", time.
	if (cumulativeFade > cumulativeFadeMinimum) {
		cumulativeFade = max(cumulativeFadeMinimum, cumulativeFade - fFade);
	}

	//MODDD - and below is the original pain system, theoretically unabridged.
	//This time around, the difference is that "fAttack<dir>mem" vars are used instead, see the explanation above.
	// SPR_Draw top

	if (EASY_CVAR_GET(painArrowColorMode) > 0 && allowArrows == 1) {

		if (fAttackFrontMem > EASY_CVAR_GET(painArrowCumulativeAppearMin))
		{
			//GetPainColor(r,g,b);
			getPainColorMode((int)EASY_CVAR_GET(painArrowColorMode), r, g, b);

			shade = a * min(max(fAttackFrontMem * EASY_CVAR_GET(painArrowDrawOpacityMax), EASY_CVAR_GET(painArrowDrawOpacityMin)), EASY_CVAR_GET(painArrowDrawOpacityMax));
			ScaleColors(r, g, b, shade);
			SPR_Set(m_painFlashSprite, r, g, b);

			x = ScreenWidth / 2 - SPR_Width(m_painFlashSprite, 0) / 2;
			y = ScreenHeight / 2 - SPR_Height(m_painFlashSprite, 0) * 3;
			SPR_DrawAdditive(0, x, y, NULL);
			fAttackFrontMem = max(0, fAttackFrontMem - fFadeARROW);
		}
		else
			fAttackFrontMem = 0;

		if (fAttackRightMem > EASY_CVAR_GET(painArrowCumulativeAppearMin))
		{
			//GetPainColor(r,g,b);
			getPainColorMode((int)EASY_CVAR_GET(painArrowColorMode), r, g, b);

			shade = a * min(max(fAttackRightMem * EASY_CVAR_GET(painArrowDrawOpacityMax), EASY_CVAR_GET(painArrowDrawOpacityMin)), EASY_CVAR_GET(painArrowDrawOpacityMax));
			ScaleColors(r, g, b, shade);
			SPR_Set(m_painFlashSprite, r, g, b);

			x = ScreenWidth / 2 + SPR_Width(m_painFlashSprite, 1) * 2;
			y = ScreenHeight / 2 - SPR_Height(m_painFlashSprite, 1) / 2;
			SPR_DrawAdditive(1, x, y, NULL);
			fAttackRightMem = max(0, fAttackRightMem - fFadeARROW);
		}
		else
			fAttackRightMem = 0;

		if (fAttackRearMem > EASY_CVAR_GET(painArrowCumulativeAppearMin))
		{
			//GetPainColor(r,g,b);
			getPainColorMode((int)EASY_CVAR_GET(painArrowColorMode), r, g, b);

			shade = a * min(max(fAttackRearMem * EASY_CVAR_GET(painArrowDrawOpacityMax), EASY_CVAR_GET(painArrowDrawOpacityMin)), EASY_CVAR_GET(painArrowDrawOpacityMax));
			ScaleColors(r, g, b, shade);
			SPR_Set(m_painFlashSprite, r, g, b);

			x = ScreenWidth / 2 - SPR_Width(m_painFlashSprite, 2) / 2;
			y = ScreenHeight / 2 + SPR_Height(m_painFlashSprite, 2) * 2;
			SPR_DrawAdditive(2, x, y, NULL);
			fAttackRearMem = max(0, fAttackRearMem - fFadeARROW);
		}
		else
			fAttackRearMem = 0;

		if (fAttackLeftMem > EASY_CVAR_GET(painArrowCumulativeAppearMin))
		{
			//GetPainColor(r,g,b);
			getPainColorMode((int)EASY_CVAR_GET(painArrowColorMode), r, g, b);

			shade = a * min(max(fAttackLeftMem * EASY_CVAR_GET(painArrowDrawOpacityMax), EASY_CVAR_GET(painArrowDrawOpacityMin)), EASY_CVAR_GET(painArrowDrawOpacityMax));
			ScaleColors(r, g, b, shade);
			SPR_Set(m_painFlashSprite, r, g, b);

			x = ScreenWidth / 2 - SPR_Width(m_painFlashSprite, 3) * 3;
			y = ScreenHeight / 2 - SPR_Height(m_painFlashSprite, 3) / 2;
			SPR_DrawAdditive(3, x, y, NULL);

			fAttackLeftMem = max(0, fAttackLeftMem - fFadeARROW);
		}
		else
			fAttackLeftMem = 0;

	}//END OF if(painFlashColorModeVar != 0)


	//MODDD - OVERVIEW OF NEW CUMULATIVE FADE METHOD:
	//~Meant to mimic the fade of this youtube video:
	//https://www.youtube.com/watch?v=GWqNkOMyKAA
	//For any attack in any of the directions, if it is above 0, "cumulativeFade" is upped by "fadeIncrement".
	//
	return 1;
}//DrawPain




void CHudPain::CalcDamageDirection(vec3_t vecFrom, int damageAmount, int rawDamageAmount)
{
	vec3_t	forward, right, up;
	float side, front;
	vec3_t vecOrigin, vecAngles;

	if (!vecFrom[0] && !vecFrom[1] && !vecFrom[2])
	{
		//easyPrintLine("OH NO I HAVE FAILED");
		m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;
		setUniformDamage(0);
		return;
	}
	int damageBlockedByArmor = rawDamageAmount - damageAmount;
	//this gets reduced by a factor of, "painFlashArmorBlock".

	memcpy(vecOrigin, gHUD.m_vecOrigin, sizeof(vec3_t));
	memcpy(vecAngles, gHUD.m_vecAngles, sizeof(vec3_t));

	VectorSubtract_f(vecFrom, vecOrigin, vecFrom);

	float flDistToTarget = vecFrom.Length();

	vecFrom = vecFrom.Normalize();
	AngleVectors(vecAngles, forward, right, up);

	front = DotProduct(vecFrom, right);
	side = DotProduct(vecFrom, forward);

	const float damageFlashModTotal = damageAmount + damageBlockedByArmor * EASY_CVAR_GET(painFlashArmorBlock);

	if (flDistToTarget <= 50)
	{
		//MODDD - changes to half the intended effect instead, no need to make attacks close-up look too dramatic
		// compared to actual damage.
		//m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 1;
		m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0.5;
		setUniformDamage(damageFlashModTotal);
	}
	else
	{
		const float dirStrictness = 1 - EASY_CVAR_GET(painFlashDirTolerance);

		if (side > 0)
		{
			//MODDD - right-hand side was 0.3!!!
			//if (side > 0.3){
			if (side > dirStrictness) {
				m_fAttackFront = max(m_fAttackFront, side);
				m_fAttackFrontDamage = damageFlashModTotal;
			}
		}
		else
		{
			float f = fabs(side);
			if (f > dirStrictness) {
				m_fAttackRear = max(m_fAttackRear, f);
				m_fAttackRearDamage = damageFlashModTotal;
			}
		}

		if (front > 0)
		{
			if (front > dirStrictness) {
				m_fAttackRight = max(m_fAttackRight, front);
				m_fAttackRightDamage = damageFlashModTotal;
			}
		}
		else
		{
			float f = fabs(front);
			if (f > dirStrictness) {
				m_fAttackLeft = max(m_fAttackLeft, f);
				m_fAttackLeftDamage = damageFlashModTotal;
			}
		}
	}
	//easyPrintLine("EEE CLIENT DAMAGE STAT: %.2f %.2f %.2f %.2f", m_fAttackFront, m_fAttackRear, m_fAttackRight, m_fAttackLeft);
}



void CHudPain::setUniformDamage(float damageAmount) {

	m_fAttackFrontDamage = m_fAttackRearDamage = m_fAttackRightDamage = m_fAttackLeftDamage = damageAmount / 4.0f;

}//END OF setUniformDamage(...)



//Do NOT supply alpha, the 1st is the mode, not r, g, or b (those 3 follow)!
void CHudPain::getPainColorMode(int mode, int& r, int& g, int& b) {
	switch (mode) {
	case 0:
		//nothing.  Plan on not drawing at all.
		break;
	case 1:
		//note that "1" is reserved for saying, fetch modes 2, 3, or 4 based on the current GUI setup (Pre-E3 or E3).
		if (EASY_CVAR_GET(hud_version) < 3) {
			getPainColorMode(4, r, g, b);
		}
		else {
			getPainColorMode(2, r, g, b);
		}
		break;
	case 2:
		//pure bright red.
		r = 255;
		g = 0;
		b = 0;
		break;
	case 3:
		//green.
		gHUD.getGenericGreenColor(r, g, b);
		break;
	case 4:
		gHUD.m_Health.deriveColorFromHealth(r, g, b);
		break;

	}
}


