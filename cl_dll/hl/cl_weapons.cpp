// NEW FILE.  Equivalent of dlls/weapons.cpp for mostly clones not dummied
// Copy of weapons.cpp's includes to start.
// See hl_weapons.cpp for other includes clientside may need

#include "hl_weapons.h"

// includes copied from weapons.cpp
#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "soundent.h"
#include "decals.h"
#include "gamerules.h"
#include "pickupwalker.h"

#include "../hud_iface.h"
#include "../com_weapons.h"


EASY_CVAR_EXTERN(firstPersonIdleDelayMin)
EASY_CVAR_EXTERN(firstPersonIdleDelayMax)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)
EASY_CVAR_EXTERN(viewModelSyncFixPrintouts)
EASY_CVAR_EXTERN(cl_holster)
EASY_CVAR_EXTERN(wpn_glocksilencer)


//MODDD - ????      This file seems to need re-implementations of several (including newly added) CBasePlayerWeapon methods from weapons.h
//(besides weapons.cpp ...    parallel implementations for the same exact prototypes?  Very strange.)
CBasePlayerWeapon::CBasePlayerWeapon() {
	//Starts as 0, not a garbage value, for ALL weapons.
	m_chargeReady = 0;

	//bitmask.
	//1 = primary held and ready to fire.
	//2 = secondary held and ready to fire.
	buttonFiltered = 0;

	bothFireButtonsMode = 2;
}


//MODDD - undummied.
int CBasePlayerWeapon::PrimaryAmmoIndex(void)
{
	return getPrimaryAmmoType();
}
int CBasePlayerWeapon::SecondaryAmmoIndex(void)
{
	return getSecondaryAmmoType();
}
//MODDD - more clones.   yay.
int CBasePlayerWeapon::PlayerPrimaryAmmoCount() {
	int myPrimaryAmmoType = getPrimaryAmmoType();
	if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
		return m_pPlayer->m_rgAmmo[myPrimaryAmmoType];
	}
	else {
		return -1;  //what
	}
}
int CBasePlayerWeapon::PlayerSecondaryAmmoCount() {
	int mySecondaryAmmoType = getSecondaryAmmoType();
	if (IS_AMMOTYPE_VALID(mySecondaryAmmoType)) {
		return m_pPlayer->m_rgAmmo[mySecondaryAmmoType];
	}
	else {
		return -1;  //what
	}
}

// WARNING - these don't check to see whether removing ammo goes below 0 or
// adding ammo goes over the max allowed in reserve!
void CBasePlayerWeapon::ChangePlayerPrimaryAmmoCount(int changeBy) {
	int myPrimaryAmmoType = getPrimaryAmmoType();
	if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
		m_pPlayer->m_rgAmmo[myPrimaryAmmoType] += changeBy;
	}
	else {
		//what
	}
}
void CBasePlayerWeapon::ChangePlayerSecondaryAmmoCount(int changeBy) {
	int mySecondaryAmmoType = getSecondaryAmmoType();
	if (IS_AMMOTYPE_VALID(mySecondaryAmmoType)) {
		m_pPlayer->m_rgAmmo[mySecondaryAmmoType] += changeBy;
	}
	else {
		//what
	}
}


void CBasePlayerWeapon::SetPlayerPrimaryAmmoCount(int newVal) {
	int myPrimaryAmmoType = getPrimaryAmmoType();
	if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
		m_pPlayer->m_rgAmmo[myPrimaryAmmoType] = newVal;
	}
	else {
		//what
	}
}
void CBasePlayerWeapon::SetPlayerSecondaryAmmoCount(int newVal) {
	int mySecondaryAmmoType = getSecondaryAmmoType();
	if (IS_AMMOTYPE_VALID(mySecondaryAmmoType)) {
		m_pPlayer->m_rgAmmo[mySecondaryAmmoType] = newVal;
	}
	else {
		//what
	}
}







//MODDD - should a weapon NOT have any deploy methods, this default will also undo the player silencer render effect.  Just for safety.
//...not using this 128 for renderfx anymore, at least not for this.
BOOL CBasePlayerWeapon::Deploy() {

	//m_pPlayer->pev->renderfx &= ~128;
	return CBasePlayerItem::Deploy();
}
/*
=====================
CBasePlayerWeapon :: DefaultReload
=====================
*/
BOOL CBasePlayerWeapon::DefaultReload(int iClipSize, int iAnim, float fDelay, int body)
{
	int myPrimaryAmmoType = getPrimaryAmmoType();
	//MODDD - check for AmmoType
	if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
		// valid ammo.
	}
	else {
		// oh no
		return FALSE;
	}


	if (m_pPlayer->cheat_infiniteammoMem != 1) {
		if (m_pPlayer->m_rgAmmo[myPrimaryAmmoType] <= 0)
			return FALSE;

		int j = min(iClipSize - m_iClip, m_pPlayer->m_rgAmmo[myPrimaryAmmoType]);
		//MODDD - is this edit ok?  To help with things that mess with the max-count like the glock with old reload logic.
		//if (j == 0)
		if (j <= 0)
			return FALSE;
	}
	else {
		if (m_pPlayer->m_rgAmmo[myPrimaryAmmoType] <= 0)
			m_pPlayer->m_rgAmmo[myPrimaryAmmoType] = 1;

		int j = min(iClipSize - m_iClip, m_pPlayer->m_rgAmmo[myPrimaryAmmoType]);
		if (j <= 0)
			m_pPlayer->m_rgAmmo[myPrimaryAmmoType] = 1;

		return TRUE;
	}


	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + fDelay;

	//MODDD - seems to make sense?  undo if this is more problematic.
	if (pev->body != body) {
		pev->body = body;
	}

	//!!UNDONE -- reload sound goes here !!!     <--- MODDD - not my message.
	SendWeaponAnim(iAnim, UseDecrement(), body);

	m_fInReload = TRUE;
	reloadBlocker = FALSE;

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
	return TRUE;
}

/*
=====================
CBasePlayerWeapon :: CanDeploy
=====================
*/
BOOL CBasePlayerWeapon::CanDeploy(void)
{
	//easyPrintLine("MESSAGE4");
	BOOL bHasAmmo = 0;

	if (!pszAmmo1())
	{
		// this weapon doesn't use ammo, can always deploy.
		return TRUE;
	}

	if (pszAmmo1())
	{
		int myPrimaryAmmoType = getPrimaryAmmoType();
		if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
			// valid ammo.
			bHasAmmo |= (m_pPlayer->m_rgAmmo[myPrimaryAmmoType] != 0);
		}
		else {
			// oh no
		}
	}
	if (pszAmmo2())
	{
		int mySecondaryAmmoType = getSecondaryAmmoType();
		if (IS_AMMOTYPE_VALID(mySecondaryAmmoType)) {
			// valid ammo.
			bHasAmmo |= (m_pPlayer->m_rgAmmo[mySecondaryAmmoType] != 0);
		}
		else {
			// oh no
		}
	}
	if (m_iClip > 0)
	{
		bHasAmmo |= 1;
	}
	if (!bHasAmmo)
	{
		return FALSE;
	}

	return TRUE;
}

/*
=====================
CBasePlayerWeapon :: DefaultDeploy

=====================
*/
//MODDD - additions since.
//BOOL CBasePlayerWeapon :: DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal, int body )

//NOTE: "fireDelayTime" defaults to 0.5, but can also be set to -1 to be forced to "deployAnimTime" instead.
BOOL CBasePlayerWeapon::DefaultDeploy(char* szViewModel, char* szWeaponModel, int iAnim, char* szAnimExt, int skiplocal /* = 0 */, int body, float deployAnimTime, float fireDelayTime)
{
	//easyPrintLine("MESSAGE5");

	if (!CanDeploy())
		return FALSE;

	m_chargeReady &= ~128;

	int x = 45;

	// NOTICE - checking 'gEngfuncs.GetViewModel()->curstate.modelindex' to see when the model changed this soon is pointless, it won't.
	// Seems this CL_LoadModel makes a call that takes a little to pick up the new model.
	// Until then, be sure no sequences meant for while the new model is up play.  That would be bad.

	//gEngfuncs.CL_LoadModel(szViewModel, &m_pPlayer->pev->viewmodel);
	// Why not use this then?
	LoadVModel(szViewModel, m_pPlayer);


	//MODDD - seems to make sense?  undo if this is more problematic.
	if (pev->body != body) {
		pev->body = body;
	}



	// !!!!!!!!!!!!
	// don't play the deploy anim until we're certain the model actually updated
	
	/*
	blockUntilModelChange = TRUE;
	oldModel = gEngfuncs.GetViewModel()->curstate.modelindex;
	queuedBlockedModelAnim = iAnim;
	*/

	forgetBlockUntilModelChangeTime = gpGlobals->time + 0.3;

	//resistTime = gpGlobals->time + 0.01;

	seqPlayDelay = gpGlobals->time + 0.01;
	seqPlay = iAnim;

	//MODDD - TESTING! Disabled
	//SendWeaponAnim(iAnim, skiplocal, body);





	//MODDD - so uh.     yeah.     you don't want to forget this.                thanks.
	if (fireDelayTime == -1) {
		//make match the "deployAnimTime":
		fireDelayTime = deployAnimTime;
	}


	g_irunninggausspred = false;
	//m_pPlayer->m_flNextAttack = 0.5;
	m_pPlayer->m_flNextAttack = fireDelayTime;

	//m_flTimeWeaponIdle = 1.0;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + deployAnimTime; //used to be "1.0", now depends on optional parameter (defaults to "1.0");

	return TRUE;
}

//Not much difference between this and the server's one (weapons.cpp). Is that ok?
void CBasePlayerWeapon::DefaultHolster(int iAnim, int skiplocal /* = 0 */, int body, float holsterAnimTime)
{
	//Do the same things a Holster() call would do since we're effecitvely replacing that.
	m_fInReload = FALSE; // cancel any reload in progress.
	reloadBlocker = FALSE;
	g_irunninggausspred = false;


	//HACK - make this a little longer to stop the client from thinking it is done the moment this ends.
	//       The notice to deploy the next weapon may come a little late.
	// try with and without + 1 maybe?
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + holsterAnimTime + 1;

	//Let this handle the time it takes to change anims instead, not transmitted to the client by default.
	m_pPlayer->m_fCustomHolsterWaitTime = gpGlobals->time + holsterAnimTime;

	//Don't want to risk setting idle animations while holstering, set this to an impossible value.
	m_pPlayer->m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + holsterAnimTime + 5;

	m_chargeReady |= 128;


	resistTime = gpGlobals->time + 0.03;

	SendWeaponAnim(iAnim);


}//END OF DefaultHolster



/*
=====================
CBasePlayerWeapon :: PlayEmptySound

=====================
*/
BOOL CBasePlayerWeapon::PlayEmptySound(void)
{
	//easyPrintLine("MESSAGE6");
	if (m_iPlayEmptySound)
	{
		HUD_PlaySound("weapons/357_cock1.wav", 0.8);
		m_iPlayEmptySound = 0;
		return 0;
	}
	return 0;
}

/*
=====================
CBasePlayerWeapon :: ResetEmptySound

=====================
*/
void CBasePlayerWeapon::ResetEmptySound(void)
{
	m_iPlayEmptySound = 1;
}

/*
=====================
CBasePlayerWeapon::Holster

Put away weapon
=====================
*/
void CBasePlayerWeapon::Holster(int skiplocal /* = 0 */)
{
	//easyPrintLine("MESSAGE8");
	m_fInReload = FALSE; // cancel any reload in progress.
	reloadBlocker = FALSE;
	g_irunninggausspred = false;

	//TODO - not quite, switch to our designated holder anim (fetch by a virtual method, could even be this same method that calls  CBaseWeapon's).
	//Or have a "DefaultHolster" like "DefaultDeploy" exist to be called most of the time in any given weapon's own Holster implementation.
	//ADDRESSED - each weapon will now implement the Holster method and call "DefaultHolster" to provide its own anim index and anim duration. 
	//            By default blanking out the viewmodel and instnatly changing is ok.
	m_pPlayer->pev->viewmodel = 0;


	//For safety, set this to instantly change the weapon.
	m_pPlayer->m_flNextAttack = 0;

	m_pPlayer->m_fCustomHolsterWaitTime = gpGlobals->time;

	//Don't want to risk setting idle animations while holstering, set this to an impossible value. ?
	m_pPlayer->m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5;
}

/*
=====================
CBasePlayerWeapon::SendWeaponAnim

Animate weapon model
=====================
*/
void CBasePlayerWeapon::SendWeaponAnim(int iAnim, int skiplocal, int body)
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnim %d", iAnim);

	//MODDD - seems to make sense?  undo if this is more problematic.
	/*
	if(pev->body != body){
		pev->body = body;
	}
	*/

	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim(iAnim, body, 2);
}


void CBasePlayerWeapon::SendWeaponAnimReverse(int iAnim, int skiplocal, int body)
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnimReverse %d", iAnim);

	//MODDD - seems to make sense?  undo if this is more problematic.
	/*
	if(pev->body != body){
		pev->body = body;
	}
	*/

	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim(iAnim, body, 2);
}



void CBasePlayerWeapon::SendWeaponAnimBypass(int iAnim, int body)
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnimBypass %d", iAnim);

	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim(iAnim, body, 2);
}

void CBasePlayerWeapon::SendWeaponAnimBypassReverse(int iAnim, int body)
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnimBypassReverse %d", iAnim);


	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;

	HUD_SendWeaponAnim(iAnim, body, 2);
}



void CBasePlayerWeapon::SendWeaponAnimClientOnly(int iAnim, int body)
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnimClientOnly %d", iAnim);


	m_pPlayer->pev->weaponanim = iAnim;

	HUD_SendWeaponAnim(iAnim, body, 2);
}

//MODDD
void CBasePlayerWeapon::SendWeaponAnimClientOnlyReverse(int iAnim, int body)
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("SendWeaponAnimClientOnlyReverse %d", iAnim);

	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;

	HUD_SendWeaponAnim(iAnim, body, 2);
}


//MODDD
void CBasePlayerWeapon::SendWeaponAnimServerOnly(int iAnim, int body)
{
	//client? no.
}

//MODDD
void CBasePlayerWeapon::SendWeaponAnimServerOnlyReverse(int iAnim, int body)
{
	//no.
}


// DUMMIED!  Only used by serverside dlls/weapons.cpp's SendWeaponAnim methods,
// clientside calls HUD_SendWeaponAnim instead
void CBasePlayerWeapon::SendWeaponAnimMessageFromServer(int iAnim, int body) {

}










///////////////////////////////////////////////

//MODDD - all new, clones.
void CBasePlayerWeapon::setchargeReady(int arg) {
	int mem = m_chargeReady & 64;
	m_chargeReady = arg;
	if (mem) {
		m_chargeReady |= 64;
	}
}
int CBasePlayerWeapon::getchargeReady(void) {
	return m_chargeReady & ~64;
}
void CBasePlayerWeapon::forceBlockLooping(void) {
	m_chargeReady |= 64;
}
void CBasePlayerWeapon::stopBlockLooping(void) {
	m_chargeReady &= ~64;
}

//................................................
/*
void CGlock::setExtraBulletFalse(void){
	m_chargeReady &= ~32;
}
void CGlock::setExtraBulletTrue(void){
	m_chargeReady |= 32;
}
BOOL CGlock::getExtraBullet(void){
	return (m_chargeReady & 32) != 0;
}
*/

///////////////////////////////////////////////


//MODDD - also new.
// WE'RE ONLY INCLUDED CLIENTSIDE DAMMIT.
// Check dlls/weapons.cpp for the serverside equivalent.  Only server-exclusive file of weapons-specific files, funny enough.
float CBasePlayerWeapon::randomIdleAnimationDelay(void) {
	if (EASY_CVAR_GET(firstPersonIdleDelayMin) > 0 && EASY_CVAR_GET(firstPersonIdleDelayMax) > 0) {
		//let's go.
		float rand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, EASY_CVAR_GET(firstPersonIdleDelayMin), EASY_CVAR_GET(firstPersonIdleDelayMax));
		//easyPrintLine("OK client client %.2f", rand);
		return rand;
	}
	else {
		return 0;
	}
}//END OF randomIdleAnimationDelay



//MODDD - new, now called.
void CBasePlayerWeapon::ItemPostFrameThink() {

	/*
	//MODDD - should be a good place to check for deplying the next weapon.
	if(m_pActiveItem && m_bHolstering && gpGlobals->time >= m_flNextAttack){
		//done holstering? complete the switch to the picked weapon. Deploy that one.

		m_bHolstering = FALSE;
		setActiveItem(m_pQueuedActiveItem);
		m_pQueuedActiveItem = FALSE;
	}
	*/

	//Surprisingly no, don't do this, this never even happens nor should it. Deploy should only be called by the server
	//and its results are echoed to the client.




	// HOLSTER - SWAPPO DISABLE.
	// or both disable.
	// MODDD - MAJOR TODO.
	// Supporting this client-side would be great and all, but there's a major problem.
	// SelectLastItem is never getting called clientside, so interrupting a LastItem call through
	// a normal number selection (4 twice, click, etc.) interrupts that clientside and interrupts
	// the deploy animation soon after by repeating itself.  Client-server synch issues are weird.
	// Does it make sense to let "lastinv" work clientside, like interpret console input right there?
	// maybe if it were a client command that send the same thing to the server....
	
	//easyForcePrintLine("HOLS:%d HOLSTIM:%.2f NEXTATTA:%.2f TIME:%.2f",m_pPlayer->m_bHolstering, m_pPlayer->m_fCustomHolsterWaitTime, m_pPlayer->m_flNextAttack, gpGlobals->time);
	//MODDD - should be a good place to check for deplying the next weapon.
	if (m_pPlayer->m_bHolstering && gpGlobals->time >= m_pPlayer->m_fCustomHolsterWaitTime) { //m_pPlayer->m_flNextAttack <= 0.0){
		//done holstering? complete the switch to the picked weapon. Deploy that one.

		//RESIST
		resistTime = gpGlobals->time + 0.01;

		m_pPlayer->m_bHolstering = FALSE;
		m_chargeReady &= ~128;
		//blockUntilModelChange = FALSE;       pointless.

		if (m_pPlayer->m_pQueuedActiveItem != NULL) {
			m_pPlayer->m_pQueuedActiveItem->m_chargeReady &= ~128;
			m_pPlayer->setActiveItem(m_pPlayer->m_pQueuedActiveItem);
			m_pPlayer->m_pQueuedActiveItem = NULL;
		}

		m_pPlayer->m_fCustomHolsterWaitTime = -1;
	}
	



	//easyForcePrintLine("AFTR HOLS:%d HOLSTIM:%.2f NEXTATTA:%.2f TIME:%.2f",m_pPlayer->m_bHolstering, m_pPlayer->m_fCustomHolsterWaitTime, m_pPlayer->m_flNextAttack, gpGlobals->time);


	CBasePlayerItem::ItemPostFrameThink();
}//END OF ItemPostFrameThink


//MODDD - added.  Doubt it will work
void CBasePlayerWeapon::ItemPreFrame() {
	easyPrintLine("ItemPreFrame CLIENT called???");
	//Evidently, not.  I guess the DLL doesn't.
}


/*
=====================
CBasePlayerWeapon::ItemPostFrame

Handles weapon firing, reloading, etc.
=====================
*/
//MODDD NOTICE - only this seems to get called clientside. Any other item think variants, at least in as-is script (still?),
//               any other item think methods never get called.
void CBasePlayerWeapon::ItemPostFrame()
{
	//little much even for you.
	//if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("ItemPostFrame: b:%d npa:%.2f", m_pPlayer->pev->button & IN_ATTACK, m_flNextPrimaryAttack);

	//easyForcePrintLine("I EAT things %.2f %d %.2f", gpGlobals->time, m_pPlayer->pev->button & IN_ATTACK, m_flNextPrimaryAttack);



	//Surprisingly no, don't do this, this never even happens nor should it. Deploy should only be called by the server
	//and its results are echoed to the client.

	//easyForcePrintLine("HOLS:%d HOLSTIM:%.2f NEXTATTA:%.2f TIME:%.2f",m_pPlayer->m_bHolstering, m_pPlayer->m_fCustomHolsterWaitTime, m_pPlayer->m_flNextAttack, gpGlobals->time);
	//MODDD - should be a good place to check for deplying the next weapon.


	// HOLSTER - SWAPPO DISABLE.
	
	/*
	if (m_pPlayer->m_bHolstering) { //m_pPlayer->m_flNextAttack <= 0.0){
		// done holstering? complete the switch to the picked weapon. Deploy that one.

		m_pPlayer->m_bHolstering = FALSE;
		m_chargeReady &= ~128;
		//blockUntilModelChange = FALSE;       pointless.   
		m_pPlayer->m_pQueuedActiveItem->m_chargeReady &= ~128;
		m_pPlayer->setActiveItem(m_pPlayer->m_pQueuedActiveItem);
		m_pPlayer->m_pQueuedActiveItem = NULL;
		return;
	}
	*/
	





	BOOL secondaryHeld = ((m_pPlayer->pev->button & IN_ATTACK2) && m_flNextSecondaryAttack <= 0.0);
	BOOL primaryHeld = ((m_pPlayer->pev->button & IN_ATTACK) && m_flNextPrimaryAttack <= 0.0);


	//NOTICE - the player would usually check "m_flNextAttack" and not even call ItemPostFrame in the first place if it weren't 0.
	//         So the check must be done here since player-specific script is all in player.cpp, a serverside-only file and most
	//         player think logic is not clientside at all, besides a little for working with weapons like here.
	//         It was this wa in the as-is script but just pointing this out.
	//         ...some things in here don't require m_flNextPrimaryAttack to be 0 regardless? odd, but not much difference if any.
	//         notice that ItemPostFrame even clientside just checks if m_pPlayer->flNextAttack is aboe 0.
	//         So any "...m_flNextAttack <= 0.0" checks in here are very redundant. Oh well.





	//MODDD - the 0.0 here is the same 0'd current time to check for given by UTIL_WeaponTimeBase() for being clientside.
	// So it checks out

	if ((m_fInReload) && (m_pPlayer->m_flNextAttack <= 0.0))
	{
		//MODDD - NOTE.  Oh dear, a note left as-is.  Or don't fix what ain't broken?
		// I don't know what ingame issue this is referring to.


#if 0 // FIXME, need ammo on client to make this work right
		// complete the reload. 
		int j = min(iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);

		// Add them to the clip
		m_iClip += j;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= j;
#else	
		//MODDD - ok.  I'll just paste a bunch of stuff from dlls/weapons.cpp here then, looks like other
		// aspects of client ammo have been fixed anyway.
		//m_iClip += 10;
		////////////////////////////////////////////////////////////////////////////////////////////////////

		// complete the reload.
		int myPrimaryAmmoType = getPrimaryAmmoType();
		if (IS_AMMOTYPE_VALID(myPrimaryAmmoType)) {
			int j = min(iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[myPrimaryAmmoType]);

			// Add them to the clip
			m_iClip += j;

			//MODDD - cheat intervention, if available.  With this cheat, reloading does not consume total ammo.
			if (EASY_CVAR_GET(cheat_infiniteammo) == 0) {
				m_pPlayer->m_rgAmmo[myPrimaryAmmoType] -= j;
			}
			else {
				if (m_pPlayer->m_rgAmmo[myPrimaryAmmoType] == 0) {
					m_pPlayer->m_rgAmmo[myPrimaryAmmoType] = 1;
				}

			}

			m_pPlayer->TabulateAmmo();

			//MODDD - new event!
			this->OnReloadApply();
		}
		m_fInReload = FALSE;
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
		//MODDD - old leftovers as a bare minimum before the weapons.cpp restoration above.
		/*
		this->OnReloadApply();

		m_fInReload = FALSE;
		*/

	}//END OF that reload stuff

	//BOOL canDoNormalPressBehavior = TRUE;
	BOOL canCallBoth = TRUE;
	BOOL canCallPrimaryNot = TRUE;
	BOOL canCallPrimary = TRUE;
	BOOL canCallSecondaryNot = TRUE;
	BOOL canCallSecondary = TRUE;
	BOOL canCallNeither = TRUE;


	if (canCallNeither && !secondaryHeld && !primaryHeld) {
		NeitherHeld();
	}

	if (canCallBoth && secondaryHeld && primaryHeld) {
		BothHeld();
		//canCallBoth = TRUE;
		canCallPrimaryNot = FALSE;
		canCallPrimary = FALSE;
		canCallSecondaryNot = FALSE;
		canCallSecondary = FALSE;
		canCallNeither = FALSE;

		primaryHeld = FALSE;
	}

	//easyPrintLine("NOT HELDD!!! %d %d %.2f", canCallPrimaryNot, primaryHeld, gpGlobals->time);
	if (canCallPrimaryNot && !primaryHeld) {
		PrimaryNotHeld();
	}
	if (canCallSecondaryNot && !secondaryHeld) {
		SecondaryNotHeld();
	}


	//MODDD
	//if ((m_pPlayer->pev->button & IN_ATTACK2) && (m_flNextSecondaryAttack <= 0.0))
	if (canCallSecondary && secondaryHeld)
	{
		if (pszAmmo2() && !m_pPlayer->m_rgAmmo[SecondaryAmmoIndex()])
		{
			m_fFireOnEmpty = TRUE;
		}

		SecondaryAttack();


		//MODDD NOTE - ?????
		// Removal.  See notes in this same place in dlls/weapons.cpp.
		//m_pPlayer->pev->button &= ~IN_ATTACK2;
	}
	//MODDD
	//else if ((m_pPlayer->pev->button & IN_ATTACK) && (m_flNextPrimaryAttack <= 0.0))
	else if (canCallPrimary && primaryHeld)
	{
		if ((m_iClip == 0 && pszAmmo1()) || (iMaxClip() == -1 && !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()]))
		{
			m_fFireOnEmpty = TRUE;
		}

		if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("Postframe PrimaryAttack!");
		PrimaryAttack();
		if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts) == 1)easyForcePrintLine("My new npa: %.2f", m_flNextPrimaryAttack);

	}
	else if (m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload)
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if (!(m_pPlayer->pev->button & (IN_ATTACK | IN_ATTACK2)))
	{
		// no fire buttons down

		m_fFireOnEmpty = FALSE;

		// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
		if (m_iClip == 0 && !(iFlags() & ITEM_FLAG_NOAUTORELOAD) && m_flNextPrimaryAttack < 0.0)
		{
			Reload();
			return;
		}

		WeaponIdle();
		return;
	}

	// catch all
	if (ShouldWeaponIdle())
	{
		WeaponIdle();
	}
}




/*
=====================
CBasePlayerWeapon::PrintState

For debugging, print out state variables to log file
=====================
*/
void CBasePlayerWeapon::PrintState(void)
{
	COM_Log("c:\\hl.log", "%.4f ", gpGlobals->time);
	COM_Log("c:\\hl.log", "%.4f ", m_pPlayer->m_flNextAttack);
	COM_Log("c:\\hl.log", "%.4f ", m_flNextPrimaryAttack);
	COM_Log("c:\\hl.log", "%.4f ", m_flTimeWeaponIdle - gpGlobals->time);
	COM_Log("c:\\hl.log", "%i ", m_iClip);
}




