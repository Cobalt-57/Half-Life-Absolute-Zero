/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// Ammo.cpp
//
// implementation of CHudAmmo class
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include "pm_shared.h"

#include <string.h>
#include <stdio.h>

#include "ammohistory.h"
#include "vgui_TeamFortressViewport.h"


//MODDD - new stuff
DECLARE_MESSAGE(m_Ammo, AntidoteP)
DECLARE_MESSAGE(m_Ammo, AdrenalineP)
DECLARE_MESSAGE(m_Ammo, RadiationP)
DECLARE_MESSAGE(m_Ammo, UpdTankTime)
DECLARE_MESSAGE(m_Ammo, UpdLJCharge)
DECLARE_MESSAGE(m_Ammo, ClearWpn)
//DECLARE_MESSAGE(m_Ammo, UpdateCam)

//that is, UpdateAlphaCrosshair
DECLARE_MESSAGE(m_Ammo, UpdACH)
DECLARE_MESSAGE(m_Ammo, UpdFrz )


DECLARE_MESSAGE( m_Ammo, HasGlockSil )



//global2PSEUDO_grabbedByBarancle






//MODDD - YO
extern int global2PSEUDO_playerHasGlockSilencer;
extern float global2_hud_version;
extern float global2_canShowWeaponSelectAtDeath;





extern float global2_useAlphaCrosshair;
extern float global2_allowAlphaCrosshairWithoutGuns;
extern float global2_alphaCrosshairBlockedOnFrozen;





EASY_CVAR_EXTERN(hud_drawammobar)
EASY_CVAR_EXTERN(hud_weaponselecthideslower)
EASY_CVAR_EXTERN(weaponSelectSoundPlayOnMousewheel)


EASY_CVAR_EXTERN(drawViewModel)
EASY_CVAR_EXTERN(drawHUD)



//MODDD - gpActiveSel and gpLastSel moved to the WeaponsResource (ammohistory.h) class for outside access.
//Also, the WeaponsResource instance, gWR, has been moved to the CHudAmmo class

client_sprite_t *GetSpriteList(client_sprite_t *pList, const char *psz, int iRes, int iCount);

int g_weaponselect = 0;



void WeaponsResource :: LoadAllWeaponSprites( void )
{
	for (int i = 0; i < MAX_WEAPONS; i++)
	{
		if ( rgWeapons[i].iId ){
			//easyForcePrintLine("OH YEAH!!! %d", rgWeapons[i].iId);
			LoadWeaponSprites( &rgWeapons[i] );
		}
	}
}

int WeaponsResource :: CountAmmo( int iId ) 
{ 
	if ( iId < 0 )
		return 0;

	return riAmmo[iId];
}

int WeaponsResource :: HasAmmo( WEAPON *p )
{
	if ( !p )
		return FALSE;

	// weapons with no max ammo can always be selected
	if ( p->iMax1 == -1 )
		return TRUE;

	return (p->iAmmoType == -1) || p->iClip > 0 || CountAmmo(p->iAmmoType) 
		|| CountAmmo(p->iAmmo2Type) || ( p->iFlags & WEAPON_FLAGS_SELECTONEMPTY );
}


void WeaponsResource :: LoadWeaponSprites( WEAPON *pWeapon )
{
	//easyPrintLine("HELP LoadWeaponSprites!!! %s", pWeapon->szName);
	int i, iRes;

	if (ScreenWidth < 640)
		iRes = 320;
	else
		iRes = 640;

	char sz[128];

	if ( !pWeapon )
		return;

	memset( &pWeapon->rcActive, 0, sizeof(wrect_t) );
	memset( &pWeapon->rcInactive, 0, sizeof(wrect_t) );
	memset( &pWeapon->rcAmmo, 0, sizeof(wrect_t) );
	memset( &pWeapon->rcAmmo2, 0, sizeof(wrect_t) );
	pWeapon->hInactive = 0;
	pWeapon->hActive = 0;
	pWeapon->hAmmo = 0;
	pWeapon->hAmmo2 = 0;

	sprintf(sz, "sprites/%s.txt", pWeapon->szName);
	client_sprite_t *pList = SPR_GetList(sz, &i);

	if (!pList)
		return;

	client_sprite_t *p;
	
	p = GetSpriteList( pList, "crosshair", iRes, i );
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hCrosshair = SPR_Load(sz);
		pWeapon->rcCrosshair = p->rc;
	}
	else
		pWeapon->hCrosshair = NULL;

	p = GetSpriteList(pList, "autoaim", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hAutoaim = SPR_Load(sz);
		pWeapon->rcAutoaim = p->rc;
	}
	else
		pWeapon->hAutoaim = 0;

	p = GetSpriteList( pList, "zoom", iRes, i );
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hZoomedCrosshair = SPR_Load(sz);
		pWeapon->rcZoomedCrosshair = p->rc;
	}
	else
	{
		pWeapon->hZoomedCrosshair = pWeapon->hCrosshair; //default to non-zoomed crosshair
		pWeapon->rcZoomedCrosshair = pWeapon->rcCrosshair;
	}

	p = GetSpriteList(pList, "zoom_autoaim", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hZoomedAutoaim = SPR_Load(sz);
		pWeapon->rcZoomedAutoaim = p->rc;
	}
	else
	{
		pWeapon->hZoomedAutoaim = pWeapon->hZoomedCrosshair;  //default to zoomed crosshair
		pWeapon->rcZoomedAutoaim = pWeapon->rcZoomedCrosshair;
	}

	p = GetSpriteList(pList, "weapon", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		//easyPrintLine("HELP what is the sprinte A?? %s", sz);
		pWeapon->hInactive = SPR_Load(sz);
		pWeapon->rcInactive = p->rc;

		gHR.iHistoryGap = max( gHR.iHistoryGap, pWeapon->rcActive.bottom - pWeapon->rcActive.top );
	}
	else
		pWeapon->hInactive = 0;

	p = GetSpriteList(pList, "weapon_s", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		//easyPrintLine("HELP what is the sprinte B?? %s", sz);
		pWeapon->hActive = SPR_Load(sz);
		pWeapon->rcActive = p->rc;
	}
	else
		pWeapon->hActive = 0;

	p = GetSpriteList(pList, "ammo", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hAmmo = SPR_Load(sz);
		pWeapon->rcAmmo = p->rc;

		gHR.iHistoryGap = max( gHR.iHistoryGap, pWeapon->rcActive.bottom - pWeapon->rcActive.top );
	}
	else
		pWeapon->hAmmo = 0;

	p = GetSpriteList(pList, "ammo2", iRes, i);
	if (p)
	{
		sprintf(sz, "sprites/%s.spr", p->szSprite);
		pWeapon->hAmmo2 = SPR_Load(sz);
		pWeapon->rcAmmo2 = p->rc;

		gHR.iHistoryGap = max( gHR.iHistoryGap, pWeapon->rcActive.bottom - pWeapon->rcActive.top );
	}
	else
		pWeapon->hAmmo2 = 0;

}

// Returns the first weapon for a given slot.
WEAPON *WeaponsResource :: GetFirstPos( int iSlot )
{
	WEAPON *pret = NULL;

	for (int i = 0; i < MAX_WEAPON_POSITIONS; i++)
	{
		if ( rgSlots[iSlot][i] && HasAmmo( rgSlots[iSlot][i] ) )
		{
			pret = rgSlots[iSlot][i];
			break;
		}
	}

	return pret;
}


WEAPON* WeaponsResource :: GetNextActivePos( int iSlot, int iSlotPos )
{
	if ( iSlotPos >= MAX_WEAPON_POSITIONS || iSlot >= MAX_WEAPON_SLOTS )
		return NULL;

	//MODDD - what? Just making referecnes to gWR more specific to the gHUD.m_Ammo.gWR WeaponsResource instance. Which... should be this one, right? Why not just drop  gWR entirely? oh well.
	WEAPON *p = gHUD.m_Ammo.gWR.rgSlots[ iSlot ][ iSlotPos+1 ];
	
	if ( !p || !gHUD.m_Ammo.gWR.HasAmmo(p) )
		return GetNextActivePos( iSlot, iSlotPos + 1 );

	return p;
}


int giBucketHeight, giBucketWidth, giABHeight, giABWidth; // Ammo Bar width and height

SpriteHandle_t ghsprBuckets;					// Sprite for top row of weapons menu

DECLARE_MESSAGE(m_Ammo, CurWeapon );	// Current weapon and clip
DECLARE_MESSAGE(m_Ammo, WeaponList);	// new weapon type
DECLARE_MESSAGE(m_Ammo, AmmoX);			// update known ammo type's count
DECLARE_MESSAGE(m_Ammo, AmmoPickup);	// flashes an ammo pickup record
DECLARE_MESSAGE(m_Ammo, WeapPickup);    // flashes a weapon pickup record
DECLARE_MESSAGE(m_Ammo, HideWeapon);	// hides the weapon, ammo, and crosshair displays temporarily
DECLARE_MESSAGE(m_Ammo, ItemPickup);

//MODDD CUSTOM MESSAGE



DECLARE_COMMAND(m_Ammo, Slot1);
DECLARE_COMMAND(m_Ammo, Slot2);
DECLARE_COMMAND(m_Ammo, Slot3);
DECLARE_COMMAND(m_Ammo, Slot4);
DECLARE_COMMAND(m_Ammo, Slot5);
DECLARE_COMMAND(m_Ammo, Slot6);
DECLARE_COMMAND(m_Ammo, Slot7);
DECLARE_COMMAND(m_Ammo, Slot8);
DECLARE_COMMAND(m_Ammo, Slot9);
DECLARE_COMMAND(m_Ammo, Slot10);
DECLARE_COMMAND(m_Ammo, Close);
DECLARE_COMMAND(m_Ammo, NextWeapon);
DECLARE_COMMAND(m_Ammo, PrevWeapon);

// width of ammo fonts
#define AMMO_SMALL_WIDTH 10
#define AMMO_LARGE_WIDTH 20

#define HISTORY_DRAW_TIME	"5"

int CHudAmmo::Init(void)
{
	gHUD.AddHudElem(this);

	HOOK_MESSAGE(CurWeapon);
	HOOK_MESSAGE(WeaponList);
	HOOK_MESSAGE(AmmoPickup);
	HOOK_MESSAGE(WeapPickup);
	HOOK_MESSAGE(ItemPickup);
	HOOK_MESSAGE(HideWeapon);
	HOOK_MESSAGE(AmmoX);

	//MODDD - CUSTOM MESSAGE
	HOOK_MESSAGE(AntidoteP);
	HOOK_MESSAGE(AdrenalineP);
	HOOK_MESSAGE(RadiationP);
	HOOK_MESSAGE(UpdTankTime);
	HOOK_MESSAGE(UpdLJCharge);
	HOOK_MESSAGE(ClearWpn);
	HOOK_MESSAGE(UpdACH);
	HOOK_MESSAGE(UpdFrz);



	
	HOOK_MESSAGE(HasGlockSil);

	
	

	HOOK_COMMAND("slot1", Slot1);
	HOOK_COMMAND("slot2", Slot2);
	HOOK_COMMAND("slot3", Slot3);
	HOOK_COMMAND("slot4", Slot4);
	HOOK_COMMAND("slot5", Slot5);
	HOOK_COMMAND("slot6", Slot6);
	HOOK_COMMAND("slot7", Slot7);
	HOOK_COMMAND("slot8", Slot8);
	HOOK_COMMAND("slot9", Slot9);
	HOOK_COMMAND("slot10", Slot10);
	HOOK_COMMAND("cancelselect", Close);
	HOOK_COMMAND("invnext", NextWeapon);
	HOOK_COMMAND("invprev", PrevWeapon);

	Reset();

	CVAR_CREATE( "hud_drawhistory_time", HISTORY_DRAW_TIME, 0 );
	CVAR_CREATE( "hud_fastswitch", "0", FCVAR_ARCHIVE );		// controls whether or not weapons can be selected in one keypress

	m_iFlags |= HUD_ACTIVE; //!!!

	gWR.Init();
	gHR.Init();

	return 1;
};

void CHudAmmo::Reset(void)
{
	m_fFade = 0;
	m_iFlags |= HUD_ACTIVE; //!!!

	gWR.gpActiveSel = NULL;

	gWR.gpLastSel = NULL; //MODDD - new, safety.
	

	gHUD.m_iHideHUDDisplay = 0;

	gWR.Reset();
	gHR.Reset();

	//	VidInit();

}
















int CHudAmmo::VidInit(void)
{
	// Load sprites for buckets (top row of weapon menu)

	//may as well see if this needs re-loading.
	if(gHUD.alphaCrossHairIndex == -1){
		gHUD.alphaCrossHairIndex = gHUD.GetSpriteIndex( "alphacrosshair" );
		if(gHUD.alphaCrossHairIndex == -1){
			easyPrint("ALPHACROSSHAIR NOT LOADED B\n");
		}else{
			//easyPrint("ALPHACROSSHAIR LOADED\n");
		}
	}


	//MODDD - replaced.  ACTUALLY, remove that too later, now completely unused either way.
	//m_HUD_bucket0 = gHUD.GetSpriteIndex( "bucket1" );
	m_HUD_weapons_categorybackground = gHUD.GetSpriteIndex( "weapons_categorybackground");

	
	m_HUD_selection = gHUD.GetSpriteIndex( "selection" );

	//MODDD - initialize new index
	m_HUD_slash = gHUD.GetSpriteIndex("number_slash");
	m_antidoteindex = gHUD.GetSpriteIndex("antidote");
	m_adrenalineindex = gHUD.GetSpriteIndex("adrenaline");
	m_radiationindex = gHUD.GetSpriteIndex("radiation");


	m_longjump_empty = gHUD.GetSpriteIndex("longjump_empty");
	m_longjump_full = gHUD.GetSpriteIndex("longjump_full");
	m_airtank_empty = gHUD.GetSpriteIndex("airtank_empty");
	m_airtank_full = gHUD.GetSpriteIndex("airtank_full");
		



	//And the alphacrosshair

	/*
	alphaCrossHair = &gHUD.GetSprite(gHUD.GetSpriteIndex( "alphacrosshair" )) ;
	alphaCrossHairRect = &gHUD.GetSpriteRect(gHUD.GetSpriteIndex( "alphacrosshair" ));
	*/
	

	ghsprBuckets = gHUD.GetSprite(m_HUD_weapons_categorybackground);
	giBucketWidth = gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).right - gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).left;
	giBucketHeight = gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).bottom - gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).top;

	gHR.iHistoryGap = max( gHR.iHistoryGap, gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).bottom - gHUD.GetSpriteRect(m_HUD_weapons_categorybackground).top);

	// If we've already loaded weapons, let's get new sprites
	gWR.LoadAllWeaponSprites();

	if (ScreenWidth >= 640)
	{
		giABWidth = 20;
		giABHeight = 4;
	}
	else
	{
		giABWidth = 10;
		giABHeight = 2;
	}

	return 1;
}









//
// Think:
//  Used for selection of weapon menu item.
//
void CHudAmmo::Think(void)
{


	//MODDD - still okay to do weapon selection during death, IF the option is on.
	//if ( gHUD.m_fPlayerDead )
	if(global2_canShowWeaponSelectAtDeath == 0 && gHUD.m_fPlayerDead)
		return;

	if ( gHUD.m_iWeaponBits != gWR.iOldWeaponBits )
	{
		gWR.iOldWeaponBits = gHUD.m_iWeaponBits;

		for (int i = MAX_WEAPONS-1; i > 0; i-- )
		{
			WEAPON *p = gWR.GetWeapon(i);

			if ( p )
			{
				if ( gHUD.m_iWeaponBits & ( 1 << p->iId ) )
					gWR.PickupWeapon( p );
				else
					gWR.DropWeapon( p );
			}
		}
	}

	if (!gWR.gpActiveSel)
		return;


	// has the player selected one?
	if (gHUD.m_iKeyBits & IN_ATTACK)
	{
		//MODDD - player must also not be dead to make a selection (in case weapons menu is still enabled)
		//MODDD - new condition...?
		//if(!gHUD.m_fPlayerDead ){
			if (gWR.gpActiveSel != (WEAPON *)1)
			{
				ServerCmd(gWR.gpActiveSel->szName);
				g_weaponselect = gWR.gpActiveSel->iId;
			}
		//}


		PlaySound("common/wpn_select.wav", 1);

		//Misunderstood something, nevermind this.
		/*
		if(CVAR_GET_FLOAT("deployingWeaponPlaysOtherSound") == 1){
			PlaySound("items/gunpickup4.wav", 1);
		}else{
			PlaySound("common/wpn_select.wav", 1);
		}
		*/


		gHUD.m_iKeyBits &= ~IN_ATTACK;

		gWR.gpLastSel = gWR.gpActiveSel;
		gWR.gpActiveSel = NULL;


	}

}

//
// Helper function to return a Ammo pointer from id
//



SpriteHandle_t* WeaponsResource :: GetAmmoPicFromWeapon( int iAmmoId, wrect_t& rect )
{
	for ( int i = 0; i < MAX_WEAPONS; i++ )
	{
		if ( rgWeapons[i].iAmmoType == iAmmoId )
		{
			rect = rgWeapons[i].rcAmmo;
			return &rgWeapons[i].hAmmo;
		}
		else if ( rgWeapons[i].iAmmo2Type == iAmmoId )
		{
			rect = rgWeapons[i].rcAmmo2;
			return &rgWeapons[i].hAmmo2;
		}
	}

	return NULL;
}


// Menu Selection Code

void WeaponsResource :: SelectSlot( int iSlot, int fAdvance, int iDirection )
{



	if ( gHUD.m_Menu.m_fMenuDisplayed && (fAdvance == FALSE) && (iDirection == 1) )	
	{ // menu is overriding slot use commands
		gHUD.m_Menu.SelectMenuItem( iSlot + 1 );  // slots are one off the key numbers
		return;
	}

	if ( iSlot > MAX_WEAPON_SLOTS )
		return;

	//MODDD
	//if ( gHUD.m_fPlayerDead || gHUD.m_iHideHUDDisplay & ( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
	if ( (global2_canShowWeaponSelectAtDeath == 0 && gHUD.m_fPlayerDead)  || gHUD.m_iHideHUDDisplay & ( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
		return;

	if (!(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT)) ))
		return;

	if ( ! ( gHUD.m_iWeaponBits & ~(1<<(WEAPON_SUIT)) ))
		return;

	WEAPON *p = NULL;
	bool fastSwitch = CVAR_GET_FLOAT( "hud_fastswitch" ) != 0;

	if ( (gpActiveSel == NULL) || (gpActiveSel == (WEAPON *)1) || (iSlot != gpActiveSel->iSlot) )
	{
		PlaySound( "common/wpn_hudon.wav", 1 );
		p = GetFirstPos( iSlot );

		if ( p && fastSwitch ) // check for fast weapon switch mode
		{
			// if fast weapon switch is on, then weapons can be selected in a single keypress
			// but only if there is only one item in the bucket
			WEAPON *p2 = GetNextActivePos( p->iSlot, p->iSlotPos );
			if ( !p2 )
			{	// only one active item in bucket, so change directly to weapon
				ServerCmd( p->szName );
				g_weaponselect = p->iId;
				return;
			}
		}
	}
	else
	{
		gHUD.playWeaponSelectMoveSound();
		if ( gpActiveSel )
			p = GetNextActivePos( gpActiveSel->iSlot, gpActiveSel->iSlotPos );
		if ( !p )
			p = GetFirstPos( iSlot );
	}

	
	if ( !p )  // no selection found
	{
		// just display the weapon list, unless fastswitch is on just ignore it
		if ( !fastSwitch )
			gpActiveSel = (WEAPON *)1;
		else
			gpActiveSel = NULL;
	}
	else 
		gpActiveSel = p;
}

//------------------------------------------------------------------------
// Message Handlers
//------------------------------------------------------------------------

//
// AmmoX  -- Update the count of a known type of ammo
// 
int CHudAmmo::MsgFunc_AmmoX(const char *pszName, int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	int iIndex = READ_BYTE();
	int iCount = READ_BYTE();

	gWR.SetAmmo( iIndex, abs(iCount) );

	return 1;
}

int CHudAmmo::MsgFunc_AmmoPickup( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int iIndex = READ_BYTE();
	int iCount = READ_BYTE();

	// Add ammo to the history
	gHR.AddToHistory( HISTSLOT_AMMO, iIndex, abs(iCount) );

	return 1;
}

int CHudAmmo::MsgFunc_WeapPickup( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	int iIndex = READ_BYTE();

	// Add the weapon to the history
	gHR.AddToHistory( HISTSLOT_WEAP, iIndex );

	return 1;
}

int CHudAmmo::MsgFunc_ItemPickup( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	const char *szName = READ_STRING();

	// Add the weapon to the history
	gHR.AddToHistory( HISTSLOT_ITEM, szName );

	return 1;
}

int CHudAmmo::MsgFunc_HideWeapon( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	
	//surprisingly, this seems not too important in regards to the crosshair.  Scripted stuff won't be using this I don't think.
	//easyPrintLine("!!!!!!!!!!HIDE WEAPON IS HERE!!!!   PAY ATTNETION !!!??!!!!!");

	gHUD.m_iHideHUDDisplay = READ_BYTE();

	if (gEngfuncs.IsSpectateOnly())
		return 1;

	if ( gHUD.m_iHideHUDDisplay & ( HIDEHUD_WEAPONS | HIDEHUD_ALL ) )
	{
		static wrect_t nullrc;
		gWR.gpActiveSel = NULL;
		SetCrosshairFiltered( 0, nullrc, 0, 0, 0 );
	}
	else
	{
		//if ( m_pWeapon )
		//	SetCrosshairFiltered( m_pWeapon->hCrosshair, m_pWeapon->rcCrosshair, 255, 255, 255 );

		//MODDD TODO - is this replacement a good idea? Verify!
		updateCrosshair();


	}

	return 1;
}

//MODDD - constructor added.
CHudAmmo::CHudAmmo(){
	//easyPrint("CONSTRUCTA\n");

	//no, that's part of the gHUD (CHUD).
	//alphaCrossHairIndex = -1;
	
	m_antidotes = 0;
	m_adrenalines = 0;
	m_radiations = 0;
	//safe default, since it might be possible to refer to this var before picking up any antidotes.
	m_airTankAirTime = 0;
	m_longJumpCharge = -1;



}




bool checkEqualStrings(char* input1, char* input2, char limit){
	int i = 0;
	bool match = FALSE;
	while(TRUE){
		

		if(input1[i] != input2[i]){
			break;
		}
		if(input1[i] == '\0' && input2[i] == '\0'){
			match = TRUE;
			break;
		}


		if(i > limit){
			match = TRUE;
			break;
		}

		i += 1;
	}
	return match;
}









	
int CHudAmmo::MsgFunc_ClearWpn(const char *pszName, int iSize, void *pbuf ){

	//???
	//BEGIN_READ( pbuf, iSize );
	//int x = READ_SHORT();


	//Forcing this reset!
	//easyPrintLine("*********WEAP RESET!!!!!!!!!!***********");
	m_pWeapon = NULL;
	updateCrosshair();

	return 1;
}



// 
//  CurWeapon: Update hud state with the current weapon and clip count. Ammo
//  counts are updated with AmmoX. Server assures that the Weapon ammo type 
//  numbers match a real ammo type.
//
int CHudAmmo::MsgFunc_CurWeapon(const char *pszName, int iSize, void *pbuf )
{
	
	
	//const char *test = "rrrr";
	
	static wrect_t nullrc;
	int fOnTarget = FALSE;

	BEGIN_READ( pbuf, iSize );

	int iState = READ_BYTE();
	int iId = READ_CHAR();
	int iClip = READ_CHAR();

	// detect if we're also on target
	if ( iState > 1 )
	{
		fOnTarget = TRUE;
	}

	//save to instance-var for seeing this later.
	recentOnTarget = fOnTarget;



	if ( iId < 1 )
	{
		SetCrosshairFiltered(0, nullrc, 0, 0, 0);
		return 0;
	}

	if ( g_iUser1 != OBS_IN_EYE )
	{
		// Is player dead???
		if ((iId == -1) && (iClip == -1))
		{
			//MODDD - this is not a decent check to see if the player is dead or not.
			//At least, not since making a fairly influential change in player.cpp.
			//gHUD.m_fPlayerDead = TRUE;
			gWR.gpActiveSel = NULL;
			return 1;
		}
		//gHUD.m_fPlayerDead = FALSE;
	}

	WEAPON *pWeapon = gWR.GetWeapon( iId );
	
	

	//gHUD.testVar->string = pWeapon->szName;

	//Nah, support for this CVar cut for now.
	/*
	easyPrintLine("NO WEAPON? %d", (pWeapon == NULL) );

	if(!pWeapon && CVAR_GET_FLOAT("allowCrosshairWithoutGuns") == 0){
		SetCrosshairFiltered(NULL, m_pWeapon->rcAutoaim, 255, 255, 255);
	}
	*/

	if ( !pWeapon )
		return 0;

	if ( iClip < -1 )
		pWeapon->iClip = abs(iClip);
	else
		pWeapon->iClip = iClip;

	//if(pWeapon->szName == "weapon_rpg"){



	if ( iState == 0 )	// we're not the current weapon, so update no more
		return 1;

	m_pWeapon = pWeapon;


	//MODDD - force the alphacrosshair instead.
	//alphacrosshair
	

	//Use "updateCrosshair" instead, which takes some CVars into account.
	/*
	if ( gHUD.m_iFOV >= 90 )
	{ // normal crosshairs
		if (fOnTarget && m_pWeapon->hAutoaim)
			SetCrosshairFiltered(m_pWeapon->hAutoaim, m_pWeapon->rcAutoaim, 255, 255, 255);
		else
			SetCrosshairFiltered(m_pWeapon->hCrosshair, m_pWeapon->rcCrosshair, 255, 255, 255);
	}
	else
	{ // zoomed crosshairs
		if (fOnTarget && m_pWeapon->hZoomedAutoaim)
			SetCrosshairFiltered(m_pWeapon->hZoomedAutoaim, m_pWeapon->rcZoomedAutoaim, 255, 255, 255);
		else
			SetCrosshairFiltered(m_pWeapon->hZoomedCrosshair, m_pWeapon->rcZoomedCrosshair, 255, 255, 255);

	}
	*/
	updateCrosshair();



		
	//ALPHA CROSSHAIR SCRIPT MOVED!

		
	
	//MODDD- this is forcing the alpha crosshair.
	m_fFade = 200.0f; //!!!
	m_iFlags |= HUD_ACTIVE;
	
	return 1;
}

//
// WeaponList -- Tells the hud about a new weapon type.
//
int CHudAmmo::MsgFunc_WeaponList(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	
	WEAPON Weapon;

	strcpy( Weapon.szName, READ_STRING() );
	Weapon.iAmmoType = (int)READ_CHAR();	
	
	Weapon.iMax1 = READ_BYTE();
	if (Weapon.iMax1 == 255)
		Weapon.iMax1 = -1;

	Weapon.iAmmo2Type = READ_CHAR();
	Weapon.iMax2 = READ_BYTE();
	if (Weapon.iMax2 == 255)
		Weapon.iMax2 = -1;

	Weapon.iSlot = READ_CHAR();
	Weapon.iSlotPos = READ_CHAR();
	Weapon.iId = READ_CHAR();
	Weapon.iFlags = READ_BYTE();
	Weapon.iClip = 0;

	gWR.AddWeapon( &Weapon );

	return 1;

}




//MODDD - needless to say, the next 3 methods are new.
int CHudAmmo:: MsgFunc_AntidoteP(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	m_antidotes = x;

	/*if (x != m_iBat)
	{
		m_fFade = FADE_TIME;
		
	}
	m_iBat = 100;
	gHUD.m_Health.m_iHealth = 100;
	*/

	return 1;
}



int CHudAmmo:: MsgFunc_AdrenalineP(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	m_adrenalines = x;

	return 1;
}


int CHudAmmo:: MsgFunc_RadiationP(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	m_radiations = x;

	return 1;
}

int CHudAmmo:: MsgFunc_UpdTankTime(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	m_airTankAirTime = x;

	return 1;
}

int CHudAmmo:: MsgFunc_UpdLJCharge(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	float xmod = ((float)x) / 100;

	//easyPrint("CONSTRUCTA %.3f\n", xmod);

	m_longJumpCharge = xmod;

	return 1;
}



/*
int CHudAmmo::MsgFunc_UpdateCam(const char *pszName, int iSize, void *pbuf){

	
	//gHUD.m_iLastCameraMode;

	if(gHUD.CVar_cameraModeMem != CL_IsThirdPerson()){

		if(CL_IsThirdPerson()){
			gHUD.CVar_cameraMode->value = 1;
		}else{
			gHUD.CVar_cameraMode->value = 0;
		}

		gHUD.CVar_cameraModeMem = CL_IsThirdPerson();
	}
	//gHUD.CVar_cameraMode->value = 1;


	return 1;
}
*/





void CHudAmmo::updateCrosshair(void){
	static wrect_t nullrc;
	int allowCrosshairUpdate = FALSE;


	//easyPrintLine("My Weapon?? %s flag:%d  count:%d clip:%d",  m_pWeapon!=NULL?m_pWeapon->szName:"NULL", m_pWeapon!=NULL?m_pWeapon->iFlags:-1, m_pWeapon!=NULL?m_pWeapon->iCount:-1, m_pWeapon!=NULL?m_pWeapon->iClip:-1);


	//MODDDSUPER:

	if(m_pWeapon  ){
		allowCrosshairUpdate = TRUE;

		//gets just a little annoying.
		//easyPrintLine("CROSSHAIRUPDATE: GUN NAME: %s", m_pWeapon->szName);

	}else{

		if(global2_useAlphaCrosshair == TRUE && global2_allowAlphaCrosshairWithoutGuns == TRUE ){
			allowCrosshairUpdate = TRUE;
		}
	}

	
	if(global2_alphaCrosshairBlockedOnFrozen==TRUE && gHUD.frozenMem == TRUE){
		allowCrosshairUpdate = FALSE;
	}

	if(EASY_CVAR_GET(drawHUD) != 0 && EASY_CVAR_GET(drawHUD) != 1){
		allowCrosshairUpdate = FALSE;
	}
	



	//easyPrintLine("WHAT UPDDDDDDDEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE %d %d %d %d", allowCrosshairUpdate, m_pWeapon==NULL, useAlphaCrosshairVal, allowAlphaCrosshairWithoutGunsVal);

	if(allowCrosshairUpdate){

		if(m_pWeapon){

			if ( gHUD.m_iFOV >= 90 )
			{ // normal crosshairs
				if (recentOnTarget && m_pWeapon->hAutoaim)
					SetCrosshairFiltered(m_pWeapon->hAutoaim, m_pWeapon->rcAutoaim, 255, 255, 255);
				else
					SetCrosshairFiltered(m_pWeapon->hCrosshair, m_pWeapon->rcCrosshair, 255, 255, 255);
			}
			else
			{ // zoomed crosshairs
				if (recentOnTarget && m_pWeapon->hZoomedAutoaim)
					SetCrosshairFiltered(m_pWeapon->hZoomedAutoaim, m_pWeapon->rcZoomedAutoaim, 255, 255, 255);
				else
					SetCrosshairFiltered(m_pWeapon->hZoomedCrosshair, m_pWeapon->rcZoomedCrosshair, 255, 255, 255);
			}

		}else{
			//send bogus info.  It will be overidden by detecting "useAlphaCrosshair".
			SetCrosshairFiltered(0, nullrc, 0, 0, 0);

		}


	}else{
		//easyPrintLine("HOW COULD YOU NOT?!");
		SetCrosshairFiltered(0, nullrc, 0, 0, 0, TRUE);
	}


	
	gHUD.useAlphaCrosshairMem = global2_useAlphaCrosshair;
	gHUD.allowAlphaCrosshairWithoutGunsMem = global2_allowAlphaCrosshairWithoutGuns;


}







int CHudAmmo::MsgFunc_UpdACH(const char *pszName, int iSize, void *pbuf){
	
	updateCrosshair();

	return 1;
}


int CHudAmmo::MsgFunc_UpdFrz(const char *pszName, int iSize, void *pbuf){
	


	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();
	gHUD.frozenMem = x;

	//okay???
	updateCrosshair();

	return 1;
}





//JUKEBOX!   Just send a "stop" request.
int CHudAmmo::MsgFunc_HasGlockSil(const char *pszName, int iSize, void *pbuf){
	
	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	global2PSEUDO_playerHasGlockSilencer = x;

	return 1;
}











//------------------------------------------------------------------------
// Command Handlers
//------------------------------------------------------------------------
// Slot button pressed
void CHudAmmo::SlotInput( int iSlot )
{

	
	if(!gHUD.m_fPlayerDead){
		//normal behaviour.
		if ( gViewPort && gViewPort->SlotInput( iSlot ) ){
			return;
		}

		gWR.SelectSlot(iSlot, FALSE, 1);
	}else{
		if(global2_canShowWeaponSelectAtDeath == 1 ){
			if ( gViewPort && gViewPort->SlotInput( iSlot ) ){
				return;
			}
			gWR.SelectSlot(iSlot, FALSE, 1);
		}else{
			if ( gViewPort && gViewPort->SlotInput( iSlot ) ){
				return;
			}
			//no select attempt available.
		}
		

	}
	

	//gWR.SelectSlot(iSlot, FALSE, 1);

	/*
	if(global2_canShowWeaponSelectAtDeath == 1 || !gHUD.m_fPlayerDead){
		// Let the Viewport use it first, for menus
		if ( gViewPort && gViewPort->SlotInput( iSlot ) ){
			return;
		}

		gWR.SelectSlot(iSlot, FALSE, 1);
	}
	*/

}

void CHudAmmo::UserCmd_Slot1(void)
{
	SlotInput( 0 );
}

void CHudAmmo::UserCmd_Slot2(void)
{
	SlotInput( 1 );
}

void CHudAmmo::UserCmd_Slot3(void)
{
	SlotInput( 2 );
}

void CHudAmmo::UserCmd_Slot4(void)
{
	SlotInput( 3 );
}

void CHudAmmo::UserCmd_Slot5(void)
{
	SlotInput( 4 );
}

void CHudAmmo::UserCmd_Slot6(void)
{
	SlotInput( 5 );
}

void CHudAmmo::UserCmd_Slot7(void)
{
	SlotInput( 6 );
}

void CHudAmmo::UserCmd_Slot8(void)
{
	SlotInput( 7 );
}

void CHudAmmo::UserCmd_Slot9(void)
{
	SlotInput( 8 );
}

void CHudAmmo::UserCmd_Slot10(void)
{
	SlotInput( 9 );
}

void CHudAmmo::UserCmd_Close(void)
{
	if (gWR.gpActiveSel)
	{
		gWR.gpLastSel = gWR.gpActiveSel;
		gWR.gpActiveSel = NULL;
		PlaySound("common/wpn_hudoff.wav", 1);
	}
	else
		ClientCmd("escape");
}


// Selects the next item in the weapon menu
void CHudAmmo::UserCmd_NextWeapon(void)
{
	


	//if ( gHUD.m_fPlayerDead || (gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL)) )
	if ( (global2_canShowWeaponSelectAtDeath == 0 && gHUD.m_fPlayerDead) || (gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL)) )
		return;

	if ( !gWR.gpActiveSel || gWR.gpActiveSel == (WEAPON*)1 )
		gWR.gpActiveSel = m_pWeapon;

	int pos = 0;
	int slot = 0;
	if ( gWR.gpActiveSel )
	{
		pos = gWR.gpActiveSel->iSlotPos + 1;
		slot = gWR.gpActiveSel->iSlot;
	}




	for ( int loop = 0; loop <= 1; loop++ )
	{
		for ( ; slot < MAX_WEAPON_SLOTS; slot++ )
		{
			for ( ; pos < MAX_WEAPON_POSITIONS; pos++ )
			{
				WEAPON *wsp = gWR.GetWeaponSlot( slot, pos );

				if ( wsp && gWR.HasAmmo(wsp) )
				{
					gWR.gpActiveSel = wsp;

					//MODDD - if this CVar is on, play the weapon-select sound on mousewheeling through.
					if(global2_weaponSelectSoundPlayOnMousewheel == 1){
						gHUD.playWeaponSelectMoveSound();
						
					}
					return;
				}
			}

			pos = 0;
		}

		slot = 0;  // start looking from the first slot again
	}


	gWR.gpActiveSel = NULL;

	
}

// Selects the previous item in the menu
void CHudAmmo::UserCmd_PrevWeapon(void)
{
	/*
	if(global2_canShowWeaponSelectAtDeath == 1 || !gHUD.m_fPlayerDead){

	}else{
		return;
	}
	*/

	//if ( gHUD.m_fPlayerDead || (gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL)) )
	if ( (global2_canShowWeaponSelectAtDeath == 0 && gHUD.m_fPlayerDead) || (gHUD.m_iHideHUDDisplay & (HIDEHUD_WEAPONS | HIDEHUD_ALL)) )
		return;

	if ( !gWR.gpActiveSel || gWR.gpActiveSel == (WEAPON*)1 )
		gWR.gpActiveSel = m_pWeapon;

	int pos = MAX_WEAPON_POSITIONS-1;
	int slot = MAX_WEAPON_SLOTS-1;
	if ( gWR.gpActiveSel )
	{
		pos = gWR.gpActiveSel->iSlotPos - 1;
		slot = gWR.gpActiveSel->iSlot;
	}
	
	for ( int loop = 0; loop <= 1; loop++ )
	{
		for ( ; slot >= 0; slot-- )
		{
			for ( ; pos >= 0; pos-- )
			{
				WEAPON *wsp = gWR.GetWeaponSlot( slot, pos );

				if ( wsp && gWR.HasAmmo(wsp) )
				{
					gWR.gpActiveSel = wsp;

					//MODDD - if this CVar is on, play the weapon-select sound on mousewheeling through.
					if(global2_weaponSelectSoundPlayOnMousewheel == 1){
						gHUD.playWeaponSelectMoveSound();
					}

					return;
				}
			}

			pos = MAX_WEAPON_POSITIONS-1;
		}
		
		slot = MAX_WEAPON_SLOTS-1;
	}

	gWR.gpActiveSel = NULL;
	
}







//-------------------------------------------------------------------------
// Drawing code
//-------------------------------------------------------------------------



int CHudAmmo::Draw(float flTime)
{



	//easyPrintLine("DERP??? hide: %d viewf: %d inter: %d lastcam: %d", gHUD.m_iHideHUDDisplay, gHUD.viewFlags, gHUD.m_iIntermission, gHUD.m_iLastCameraMode);


	int a, x, y, r, g, b;
	int AmmoWidth;




	if (!(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT)) ))
		return 1;




	if ( (gHUD.m_iHideHUDDisplay & ( HIDEHUD_WEAPONS | HIDEHUD_ALL )) )
		return 1;

	// Draw Weapon Menu
	DrawWList(flTime);

	//Seems to be a preference if these two should go below or above the "if(!gHUD.canDrawBottomStats)" block.
	if (!(m_iFlags & HUD_ACTIVE))
		return 0;







	
	bool forceShowZero = FALSE;

	//if dead, and cannot show the weapon select screen, don't bother drawing ammo.
	//Can depend on "global2_hud_version" too, I think the E3 (yellow) may just never draw the ammo at death or something.
	if(gHUD.m_fPlayerDead){
		
		if(global2_canShowWeaponSelectAtDeath == 1){
			//If the weapon select screen can be shown, just show 0 for ammo.
			forceShowZero = TRUE;
		}else{
			//otherwise, don't draw anything here.
			return 0;
		}
		
	}



	

	//if the weapon select is on, don't draw anything else... except the side-GUI (# of antidotes? adrenaline syringes? radiation syringes?)
	//if(!gHUD.canDrawBottomStats){
	if(  gHUD.canDrawSidebar() ){
		
		//each is the same size.
		int inventoryIconWidth = gHUD.GetSpriteRect(m_antidoteindex).right - gHUD.GetSpriteRect(m_antidoteindex).left;
		int inventoryIconHeight = gHUD.GetSpriteRect(m_antidoteindex).bottom - gHUD.GetSpriteRect(m_antidoteindex).top;

		gHUD.getGenericGUIColor(r, g, b);


		//AIR TANK
		//////////////////////////////////////////////////////////////////////
		//m_airTankAirTime

		x = ScreenWidth - inventoryIconWidth - 20;
		y = ScreenHeight - (inventoryIconHeight*6+(8*5)) - 73;

		SPR_Set(gHUD.GetSprite(m_airtank_empty), r, g, b );
		SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(m_airtank_empty));
		


		SpriteHandle_t spritehandle = gHUD.GetSprite( m_airtank_full );
		wrect_t* spriterect = &gHUD.GetSpriteRect( m_airtank_full );


		gHUD.drawPartialFromBottom(spritehandle, spriterect, ( (float)m_airTankAirTime / (float)PLAYER_AIRTANK_TIME_MAX ), x + 11, y + 3, r, g, b);
		

		//////////////////////////////////////////////////////////////////////


		//LONG JUMP CHARGE
		//////////////////////////////////////////////////////////////////////
		
		//easyPrint("HELP MEee %d\n", m_longJumpCharge);
		if(m_longJumpCharge >= 0){
			
			x = ScreenWidth - inventoryIconWidth - 20;
			y = ScreenHeight - (inventoryIconHeight*5+(8*4)) - 73;

			SPR_Set(gHUD.GetSprite(m_longjump_empty), r, g, b );
			SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(m_longjump_empty));
			

			spritehandle = gHUD.GetSprite( m_longjump_full );
			spriterect = &gHUD.GetSpriteRect( m_longjump_full );

			gHUD.drawPartialFromBottom(spritehandle, spriterect, ( (float)m_longJumpCharge / (float)PLAYER_LONGJUMPCHARGE_MAX ), x + 11, y + 4, r, g, b);
			
		}

		//////////////////////////////////////////////////////////////////////




		//RADIATION
		//////////////////////////////////////////////////////////////////////
		x = ScreenWidth - inventoryIconWidth - 20;
		y = ScreenHeight - (inventoryIconHeight*4+(8*3)) - 73;

		SPR_Set(gHUD.GetSprite(m_radiationindex), r, g, b );
		SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(m_radiationindex));
		
		x += 4;
		y += 38;

		//Draw the radiation number (adjust these coords if needed)
		gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, m_radiations, r, g, b, 2);
		//////////////////////////////////////////////////////////////////////


		//ANTIDOTE
		//////////////////////////////////////////////////////////////////////
		x = ScreenWidth - inventoryIconWidth - 20;
		y = ScreenHeight - (inventoryIconHeight*3+(8*2)) - 73;

		SPR_Set(gHUD.GetSprite(m_antidoteindex), r, g, b );
		SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(m_antidoteindex));
		
		x += 4;
		y += 38;

		//Draw the antidote number (adjust these coords if needed)
		gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, m_antidotes, r, g, b, 2);
		//////////////////////////////////////////////////////////////////////


		//ADRENALINE
		//////////////////////////////////////////////////////////////////////
		x = ScreenWidth - inventoryIconWidth - 20;
		y = ScreenHeight - (inventoryIconHeight*2+(8*1)) - 73;

		SPR_Set(gHUD.GetSprite(m_adrenalineindex), r, g, b );
		SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(m_adrenalineindex));
		
		x += 4;
		y += 38;

		//Draw the adrenaline number (adjust these coords if needed)
		gHUD.DrawHudNumber(x, y, DHN_DRAWZERO, m_adrenalines, r, g, b, 2);
		//////////////////////////////////////////////////////////////////////


		

	}














	if (!m_pWeapon)
		return 0;
	///////////////////////////////////////////////////////////////



	int primaryAmmoClip;
	int primaryAmmoTotal;
	int secondaryAmmoTotal;



	if(!forceShowZero){
		//NOTE: the later seen "pw" is just shorthand for "m_pWeapon".
		primaryAmmoClip = m_pWeapon->iClip;

		/*
		if(m_pWeapon->iAmmoType > 0){
			primaryAmmoTotal = gWR.CountAmmo(m_pWeapon->iAmmoType);
		}else{
			primaryAmmoTotal = 0;
		}
		*/
		primaryAmmoTotal = gWR.CountAmmo(m_pWeapon->iAmmoType);

		//It is safe to assume that "secondaryAmmoTotal" is referred to only if the secondary ammo type is above 0.
		if(m_pWeapon->iAmmo2Type > 0){
			secondaryAmmoTotal = gWR.CountAmmo(m_pWeapon->iAmmo2Type);
		}else{
			secondaryAmmoTotal = 0;
		}
	}else{
		//force all 0's.
		primaryAmmoClip = 0;
		primaryAmmoTotal = 0;
		secondaryAmmoTotal = 0;
	}










	//easyForcePrintLine("WHO AREREEEEE YOU %d :: %.2f", gHUD.canDrawBottomStats, global2_hud_weaponselecthideslower);

	if(!gHUD.canDrawBottomStats && global2_hud_weaponselecthideslower == 1){
		//only block the bottom GUI (health, armor) if this CVar says to.
		return 1;
	}



	// Draw ammo pickup history
	//MODDD - removed.
	/*
	gHR.DrawAmmoHistory( flTime );
	*/


	


	WEAPON *pw = m_pWeapon; // shorthand
	
	//I will assume this is what cuts off the crowbar's attempt at drawing ammo.
	// SPR_Draw Ammo
	if ((pw->iAmmoType < 0) && (pw->iAmmo2Type < 0))
		return 0;









	int iFlags = DHN_DRAWZERO; // draw 0 values

	//m_HUD_number_0   is just the plain font, not the boxed or tiny one.
	AmmoWidth = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).right - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).left;

	a = (int) max( MIN_ALPHA, m_fFade );

	if (m_fFade > 0)
		m_fFade -= (gHUD.m_flTimeDelta * 20);

	//MODDD - CHANGE COLOR - use green instead.
	//UnpackRGB(r,g,b, RGB_YELLOWISH);
	gHUD.getGenericGUIColor(r, g, b);

	//MODDD - not used.
	//ScaleColors(r, g, b, a );

	//MODDD - swap primary & secondary ammo Y positions.
	// Does this weapon have a clip?
	//y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight/2;




	bool isRPG = FALSE;
	

	//MODDD - this is how we check to see if the currently equipped weapon is the rpg:
	if(checkEqualStrings(m_pWeapon->szName, "weapon_rpg", 10)){
		isRPG = TRUE;
	}

	if(global2_hud_version == 0){


		int iNumberHeight = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).bottom - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).top;
		//y = (ScreenHeight - gHUD.m_iFontHeight*2.75);
		y = (ScreenHeight - (iNumberHeight*2.5) );
	
		
		//gHUD.testVar->string = m_pWeapon->szName;

		//if(!forceShowZero){
		//MODDD - if "isRPG", do something else...
			if(!isRPG){
			// Does weapon have any ammo at all?
				if (m_pWeapon->iAmmoType > 0){
					int iIconWidth = m_pWeapon->rcAmmo.right - m_pWeapon->rcAmmo.left;
		
					if (primaryAmmoClip >= 0){
						// room for the number and the '|' and the current ammo
						//MODDD - also, not supplying the new arguemnt (fondID of 0, 1, or 2) means it will be 0: plain font.
						//x = ScreenWidth - (8 * AmmoWidth) - iIconWidth;
						x = ScreenWidth - (8 * AmmoWidth) - 1;
						x = gHUD.DrawHudNumber(x, y, iFlags | DHN_3DIGITS, primaryAmmoClip, r, g, b, 0, 1);

						wrect_t rc;
						rc.top = 0;
						rc.left = 0;
						rc.right = AmmoWidth;
						rc.bottom = 100;

						//MODDD - "iBarWidth" will now be the width of the slash instead.
						//int iBarWidth =  AmmoWidth/10;
						int iBarWidth = gHUD.GetSpriteRect(m_HUD_slash).right - gHUD.GetSpriteRect(m_HUD_slash).left;

						//MODDD - removed - no need for extra space as an equidistant character.
						//x += AmmoWidth/2;


						//MODDD
						//UnpackRGB(r,g,b, RGB_YELLOWISH);

						//MODDD - draw a slash instead.
						/*
						// draw the | bar
						FillRGBA(x, y, iBarWidth, gHUD.m_iFontHeight, r, g, b, a);
						*/
						
						
						//SPR_Set(gHUD.GetSprite(m_HUD_slash), r, g, b );
						////// NYYYYY   SPR_Set(GetSprite(m_HUD_number_0 + k), r, g, b );
						gHUD.drawAdditiveFilter( gHUD.GetSprite(m_HUD_slash), r, g, b, 0, x, y, &gHUD.GetSpriteRect(m_HUD_slash), 1);
						////////////////////////////////////////////////////////////////////////////


						//MODDD - removed - no need for extra space as an equidistant character.
						//x += iBarWidth + AmmoWidth/2;
						x += iBarWidth;

						// GL Seems to need this
						//ScaleColors(r, g, b, a );
						x = gHUD.DrawHudNumber(x, y, iFlags | DHN_3DIGITS, primaryAmmoTotal, r, g, b, 0, 1);		


					}
					else
					{
						// SPR_Draw a bullets only line
						//MODDD - assuming this is used for the uranium weapons only (gauss / tau cannon & gluon) 
						//
			
						//x = ScreenWidth - 4 * AmmoWidth - iIconWidth;

						//MODDD - Anyways, those kinds of ammo should be drawn at the bottom of the GUI instead (where the secondary ammo (MP5 grenades) is now)
						x = ScreenWidth - (5 * AmmoWidth);
						//y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight/2;
						y = (ScreenHeight - (iNumberHeight*1.5) );
					
						//NOTICE: JUst confirming.  Does the crowbar need a check so as to not even attempt to draw a number here?
						//No issues so far, just pointing that out.
						x = gHUD.DrawHudNumber(x, y, iFlags | DHN_3DIGITS, primaryAmmoTotal, r, g, b, 0, 1);
			
					}

					//MODDD - icon removed.
					/*
					// Draw the ammo Icon
					int iOffset = (m_pWeapon->rcAmmo.bottom - m_pWeapon->rcAmmo.top)/8;
					SPR_Set(m_pWeapon->hAmmo, r, g, b);
					SPR_DrawAdditive(0, x, y - iOffset, &m_pWeapon->rcAmmo);
					*/
				}

			}//END OF if(!isRPG)
			else{
				//For the RPG, just add the clip and remaining ammo.  Draw that as a single number.
				//The effect is that reloading makes no difference on the single number shown.
				//Firing removes one, however (and thus, to reload OR fire, it must be at least 1).
				x = ScreenWidth - (5 * AmmoWidth) - 1;
				y = (ScreenHeight - (iNumberHeight*1.5) ) + 5;
				x = gHUD.DrawHudNumber(x, y, iFlags | DHN_3DIGITS, primaryAmmoClip + primaryAmmoTotal, r, g, b, 0, 1);
			}

			// Does weapon have seconday ammo?
			//(this is not contained by the "!isRPG" check? odd.)
			if(pw->iAmmo2Type > 0){
				int iIconWidth = m_pWeapon->rcAmmo2.right - m_pWeapon->rcAmmo2.left;

				//MODDD
				//y -= gHUD.m_iFontHeight + gHUD.m_iFontHeight/4;
				y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight/2 + 5;

				//MODDD - changed to draw under the primary ammo's slash.
				//x = ScreenWidth - 4 * AmmoWidth - iIconWidth;
				x = ScreenWidth - (5 * AmmoWidth) - 1;
				x = gHUD.DrawHudNumber(x, y, iFlags|DHN_3DIGITS, secondaryAmmoTotal, r, g, b, 0, 1);

				//MODDD - icon removed.
				/*
				// Draw the ammo Icon
				SPR_Set(m_pWeapon->hAmmo2, r, g, b);
				int iOffset = (m_pWeapon->rcAmmo2.bottom - m_pWeapon->rcAmmo2.top)/8;
				SPR_DrawAdditive(0, x, y - iOffset, &m_pWeapon->rcAmmo2);
				*/

			}//END OF if(pw->iAmmo2Type > 0)
		
		/*
		}//END OF if(forceShowZero)
		else{
			//just draw 0.
			x = ScreenWidth - (8 * AmmoWidth) - 1;
			x = gHUD.DrawHudNumber(x, y, iFlags | DHN_3DIGITS, 0, r, g, b);
		}
		*/

	}//END OF if(global2_hud_version == 0)
	else{

		//what is "pw->iAmmoType" again?

		//used again.
		ScaleColors(r, g, b, a );


		//if(!forceShowZero){

			if(!isRPG){

			//TOTAL
			x = ScreenWidth - (30 + 24*3) + 13 + 8 -1 + 2 + 1;
			y = ScreenHeight - 45;
			//NOTICE: JUst confirming.  Does the crowbar need a check so as to not even attempt to draw a number here?
						//No issues so far, just pointing that out.
			gHUD.DrawHudNumber(x, y, DHN_DRAWZERO|DHN_3DIGITS|DHN_DRAWPLACE|DHN_EMPTYDIGITSUNFADED , primaryAmmoTotal, r, g, b, 4);
			

			if(primaryAmmoClip >= 0){
				//Hard-coded bar graphic.
				x = ScreenWidth - 90;
				y = ScreenHeight - 48;
				int iHeight = 33;
				int iWidth = 3;
				FillRGBA(x, y, iWidth, iHeight, r, g, b, a);

				//CLIP
				x = ScreenWidth - 114 - (24 * 3) + 5 + 21 + 1;
				y = ScreenHeight - 45;
				gHUD.DrawHudNumber(x, y, DHN_DRAWZERO|DHN_3DIGITS|DHN_DRAWPLACE|DHN_EMPTYDIGITSUNFADED , primaryAmmoClip, r, g, b, 4);
			}

		
			if (pw->iAmmo2Type > 0 && secondaryAmmoTotal > 0) {
				//SECONDARY
				x = ScreenWidth - (30 + 24*3 + 0) + 13 + 8 + 2;
				y = ScreenHeight - 59 - 33 + 20 - 4 - 10;
				gHUD.DrawHudNumber(x, y, DHN_DRAWZERO|DHN_2DIGITS|DHN_DRAWPLACE|DHN_EMPTYDIGITSUNFADED , secondaryAmmoTotal, r, g, b, 4);
			}

		}else{

			x = ScreenWidth - (30 + 24*3) + 13 + 8 -1 + 2 + 1;
			y = ScreenHeight - 45;
			gHUD.DrawHudNumber(x, y, DHN_DRAWZERO|DHN_3DIGITS|DHN_DRAWPLACE|DHN_EMPTYDIGITSUNFADED, primaryAmmoClip + primaryAmmoTotal, r, g, b, 4);


		}
			//
		/*
		}//END OF if(!forceShowZero)
		else{
			//just draw 0.

		}
		*/


	}//END OF else OF if(global2_hud_version == 0)  [effectively: if(global2_hud_version == 1) ]



	return 1;
}


//
// Draws the ammo bar on the hud
//
int DrawBar(int x, int y, int width, int height, float f)
{
	int r, g, b;

	if (f < 0)
		f = 0;
	if (f > 1)
		f = 1;

	if (f)
	{
		int w = f * width;

		// Always show at least one pixel if we have ammo.
		if (w <= 0)
			w = 1;
		UnpackRGB(r, g, b, RGB_GREENISH);
		FillRGBA(x, y, w, height, r, g, b, 255);
		x += w;
		width -= w;
	}

	UnpackRGB(r, g, b, RGB_YELLOWISH);

	FillRGBA(x, y, width, height, r, g, b, 128);

	return (x + width);
}



void DrawAmmoBar(WEAPON *p, int x, int y, int width, int height)
{
	//MODDD - removed.
	
	if(global2_hud_drawammobar == 1){
		//proceed to draw.
	}else{
		//No.
		return;
	}

	if ( !p )
		return;
	
	if (p->iAmmoType != -1)
	{
		//MODDD - what.
		if (!gHUD.m_Ammo.gWR.CountAmmo(p->iAmmoType))
			return;

		float f = (float)gHUD.m_Ammo.gWR.CountAmmo(p->iAmmoType)/(float)p->iMax1;
		
		x = DrawBar(x, y, width, height, f);


		// Do we have secondary ammo too?

		if (p->iAmmo2Type != -1)
		{
			f = (float)gHUD.m_Ammo.gWR.CountAmmo(p->iAmmo2Type)/(float)p->iMax2;

			x += 5; //!!!

			DrawBar(x, y, width, height, f);
		}
	}
	
}




void setWeaponMenuColor(int &r, int &g, int &b){
	//UnpackRGB(r,g,b, RGB_YELLOWISH);
	gHUD.getGenericGUIColor(r, g, b);
}

void setWeaponMenuColorOutOfAmmo(int &r, int &g, int &b){
	gHUD.getGenericEmptyColor(r, g, b);
}


//
// Draw Weapon Menu
//
int CHudAmmo::DrawWList(float flTime)
{
	int r,g,b,x,y,a,i;

	//MODDD - added.  Force the selected weapon null if the player is dead and cannot see the weapon menu.
	if(gHUD.m_fPlayerDead && global2_canShowWeaponSelectAtDeath == 0){
		gWR.gpLastSel = gWR.gpActiveSel;
		gWR.gpActiveSel = NULL;
		//can't select a weapon in this state.
	}



	if ( !gWR.gpActiveSel ){
		gHUD.canDrawBottomStats = TRUE;
		return 0;
	}else{
		gHUD.canDrawBottomStats = FALSE;
	}

	

	int iActiveSlot;

	if ( gWR.gpActiveSel == (WEAPON *)1 )
		iActiveSlot = -1;	// current slot has no weapons
	else 
		iActiveSlot = gWR.gpActiveSel->iSlot;

	//MODDD - 
	//x = 55; //!!!
	//y = 10; //!!!
	x = 28;
	y = 4;


	// Ensure that there are available choices in the active slot
	if ( iActiveSlot > 0 )
	{
		if ( !gWR.GetFirstPos( iActiveSlot ) )
		{
			gWR.gpActiveSel = (WEAPON *)1;
			iActiveSlot = -1;
		}
	}
		
	// Draw top line
	for ( i = 0; i < MAX_WEAPON_SLOTS; i++ )
	{
		int iWidth;

		setWeaponMenuColor(r, g, b);
	
		if ( iActiveSlot == i ){
			//a = 255;
			//MODDD
			a = 100;
		}else{
			//a = 192;
			//MODDD
			a = 100;
		}

		ScaleColors(r, g, b, 255);


		//MODDD No, background, then number separately.
		//SPR_Set(gHUD.GetSprite(m_HUD_bucket0 + i), r, g, b );


		
		SPR_Set(gHUD.GetSprite(m_HUD_weapons_categorybackground), r, g, b );
		


		// make active slot wide enough to accomodate gun pictures
		if ( i == iActiveSlot )
		{
			WEAPON *p = gWR.GetFirstPos(iActiveSlot);
			if ( p )
				iWidth = p->rcActive.right - p->rcActive.left;
			else
				iWidth = giBucketWidth;
		}
		else
			iWidth = giBucketWidth;




		//hm, try this here instead?
		FillRGBA( x, y, giBucketWidth, giBucketHeight, r, g, b, a );

		

		//MODDD  see above
		//SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(m_HUD_weapons_categorybackground));
		

		//SPR_Set(gHUD.GetSprite(number_1tiny), r, g, b );
		//SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(number_1tiny));
		


		a = 224;
		gHUD.DrawHudNumber(x + 6, y + 6, 0, i+1, r, g, b, 2);


		

		//x += iWidth + 5;
		x += iWidth + 4;
	}


	a = 128; //!!!
	//MODDD - new.
	//x = 10;
	x = 28;

	// Draw all of the buckets
	for (i = 0; i < MAX_WEAPON_SLOTS; i++)
	{
		
		//MODDD
		//y = giBucketHeight + 10;

		y = giBucketHeight + 8;

		// If this is the active slot, draw the bigger pictures,
		// otherwise just draw boxes
		if ( i == iActiveSlot )
		{
			WEAPON *p = gWR.GetFirstPos( i );
			int iWidth = giBucketWidth;
			if ( p )
				iWidth = p->rcActive.right - p->rcActive.left;

			for ( int iPos = 0; iPos < MAX_WEAPON_POSITIONS; iPos++ )
			{
				p = gWR.GetWeaponSlot( i, iPos );

				if ( !p || !p->iId )
					continue;

				setWeaponMenuColor(r, g, b);
			
				// if active, then we must have ammo.


				int fallToDefault = 1;
				//easyPrintLine("YOU ARE AMAZING %d %s", iPos, p->szName);
				if(strcmp(p->szName, "weapon_9mmhandgun") == 0){

					if(global2PSEUDO_playerHasGlockSilencer == 1){
						fallToDefault = 0;
						
						
						if ( gWR.gpActiveSel == p ){


							//MODDD - Broken trans?
							gHUD.attemptDrawBrokenTrans(x, y, p->rcActive.right - p->rcActive.left, p->rcActive.bottom - p->rcActive.top);

							SPR_Set( gHUD.GetSprite(gHUD.m_glockSilencerWpnIcoActive), r, g, b );
							SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(gHUD.m_glockSilencerWpnIcoActive) );

							SPR_Set(gHUD.GetSprite(m_HUD_selection), r, g, b );
							SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(m_HUD_selection));
							
						}
						else{
							// Draw Weapon if Red if no ammo
							if ( gWR.HasAmmo(p) )
								ScaleColors(r, g, b, 192);
							else
							{
								//UnpackRGB(r,g,b, RGB_REDISH);
								setWeaponMenuColorOutOfAmmo(r, g, b);
								ScaleColors(r, g, b, 128);
							}
							//MODDD - Broken trans?
							gHUD.attemptDrawBrokenTrans(x, y, p->rcInactive.right - p->rcInactive.left, p->rcInactive.bottom - p->rcInactive.top);

							SPR_Set( gHUD.GetSprite(gHUD.m_glockSilencerWpnIcoInactive), r, g, b );
							SPR_DrawAdditive( 0, x, y, &gHUD.GetSpriteRect(gHUD.m_glockSilencerWpnIcoInactive) );
						}



					}else{

						fallToDefault = 1;


					}//END OF else OF if(global2PSEUDO_playerHasGlockSilencer == 1)





				}


				if(fallToDefault){
					
					if ( gWR.gpActiveSel == p )
					{


						//MODDD - Broken trans?
						gHUD.attemptDrawBrokenTrans(x, y, p->rcActive.right - p->rcActive.left, p->rcActive.bottom - p->rcActive.top);

						SPR_Set(p->hActive, r, g, b );
						SPR_DrawAdditive(0, x, y, &p->rcActive);

						SPR_Set(gHUD.GetSprite(m_HUD_selection), r, g, b );
						SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(m_HUD_selection));
					
					
					}
					else
					{

						// Draw Weapon if Red if no ammo

						if ( gWR.HasAmmo(p) )
							ScaleColors(r, g, b, 192);
						else
						{
							//UnpackRGB(r,g,b, RGB_REDISH);
							setWeaponMenuColorOutOfAmmo(r, g, b);
							ScaleColors(r, g, b, 128);
						}
					
						//MODDD - Broken trans?
						gHUD.attemptDrawBrokenTrans(x, y, p->rcInactive.right - p->rcInactive.left, p->rcInactive.bottom - p->rcInactive.top);

						SPR_Set( p->hInactive, r, g, b );
						SPR_DrawAdditive( 0, x, y, &p->rcInactive );

					}




				}//END OF special glock check.



				// Draw Ammo Bar

				DrawAmmoBar(p, x + giABWidth/2, y, giABWidth, giABHeight);
				
				//MODDD
				//y += p->rcActive.bottom - p->rcActive.top + 5; ???
				y += p->rcActive.bottom - p->rcActive.top + 4;
			}
			//MODDD - was + 5.
			x += iWidth + 4;

		}
		else
		{
			// Draw Row of weapons.

			//UnpackRGB(r,g,b, RGB_YELLOWISH);
			setWeaponMenuColor(r, g, b);

			for ( int iPos = 0; iPos < MAX_WEAPON_POSITIONS; iPos++ )
			{
				WEAPON *p = gWR.GetWeaponSlot( i, iPos );
				
				
				if ( !p || !p->iId )
					continue;


				setWeaponMenuColor(r, g, b);

				if ( gWR.HasAmmo(p) )
				{
					//UnpackRGB(r,g,b, RGB_YELLOWISH);
					setWeaponMenuColor(r, g, b);
					//a = 128;
					//MODDD - now 100.
					a = 100;
				}
				else
				{
					//UnpackRGB(r,g,b, RGB_REDISH);
					setWeaponMenuColorOutOfAmmo(r, g, b);
					a = 96;
				}

				//MODDD - Broken trans?
				gHUD.attemptDrawBrokenTrans(x, y-1, giBucketWidth, giBucketHeight);
		
				FillRGBA( x, y, giBucketWidth, giBucketHeight, r, g, b, a );

				

				//MODDD
				//y += giBucketHeight + 5;
				y += giBucketHeight + 4;
			}

			//MODDD
			//x += giBucketWidth + 5;
			x += giBucketWidth + 4;
		}
	}	

	return 1;

}


/* =================================
	GetSpriteList

Finds and returns the matching 
sprite name 'psz' and resolution 'iRes'
in the given sprite list 'pList'
iCount is the number of items in the pList
================================= */
client_sprite_t *GetSpriteList(client_sprite_t *pList, const char *psz, int iRes, int iCount)
{
	if (!pList)
		return NULL;

	int i = iCount;
	client_sprite_t *p = pList;

	while(i--)
	{
		if ((!strcmp(psz, p->szName)) && (p->iRes == iRes))
			return p;
		p++;
	}

	return NULL;
}
