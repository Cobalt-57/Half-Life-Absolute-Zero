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
#include "extdll.h"
#include "util.h"
#include "cbase.h"
//MODDDD - interestingly enough...
#include "animating.h"
#include "basetoggle.h"
#include "basebutton.h"
#include "doors.h"  //is that okay?
#include "basemonster.h"
#include "weapons.h"
//MODDD
#include "chumtoadweapon.h"
#include "crossbow.h"
#include "crowbar.h"
#include "egon.h"
#include "gauss.h"
#include "glock.h"
#include "handgrenade.h"
#include "hornetgun.h"
#include "mp5.h"
#include "python.h"
#include "rpg.h"
#include "satchel.h"
#include "shotgun.h"
#include "squeak.h"
#include "tripmine.h"


#include "nodes.h"
#include "player.h"
#include "usercmd.h"
#include "entity_state.h"
#include "demo_api.h"
#include "pm_defs.h"
#include "event_api.h"
#include "r_efx.h"

#include "../hud_iface.h"
#include "../com_weapons.h"
#include "../demo.h"



//MODDD - handy?
//EASY_CVAR_EXTERN(testVar)

EASY_CVAR_EXTERN(firstPersonIdleDelayMin)
EASY_CVAR_EXTERN(firstPersonIdleDelayMax)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)
EASY_CVAR_EXTERN(viewModelSyncFixPrintouts)
EASY_CVAR_EXTERN(cl_holster)
EASY_CVAR_EXTERN(wpn_glocksilencer)

extern int flag_apply_m_flTimeWeaponIdle;
extern float stored_m_flTimeWeaponIdle;

extern int flag_apply_m_fJustThrown;
extern int stored_m_fJustThrown;



//MODDD - important point before
//extern globalvars_t *gpGlobals;

extern int g_iUser1;

// Pool of client side entities/entvars_t
static entvars_t	ev[ 32 ];
static int		num_ents = 0;



// The entity we'll use to represent the local client
static CBasePlayer player;

// Local version of game .dll global variables ( time, etc. )
static globalvars_t Globals; 

static CBasePlayerWeapon* g_pWpns[ 32 ];

float g_flApplyVel = 0.0;
int g_irunninggausspred = 0;

vec3_t previousorigin;

// HLDM Weapon placeholder entities.
CGlock g_Glock;
CCrowbar g_Crowbar;
CPython g_Python;
CMP5 g_Mp5;
CCrossbow g_Crossbow;
CShotgun g_Shotgun;
CRpg g_Rpg;
CGauss g_Gauss;
CEgon g_Egon;
CHgun g_HGun;
CHandGrenade g_HandGren;
CSatchel g_Satchel;
CTripmine g_Tripmine;
CSqueak g_Snark;
//MODDD - new
CChumToadWeapon g_ChumToadWeapon;


//clientside only variable.
BOOL reloadBlocker = FALSE;




//MODDD - NOTE - ???
// Don't understand giving g_engfuncs methods here as this is all clientside.
// AlertMessage is pretty generic at least.  Leaving it here as-is.
/*
======================
AlertMessage

Print debug messages to console
======================
*/
void AlertMessage( ALERT_TYPE atype, char *szFmt, ... )
{
	va_list argptr;
	static char	string[1024];
	
	va_start (argptr, szFmt);
	vsprintf (string, szFmt,argptr);
	va_end (argptr);

	gEngfuncs.Con_Printf( "cl:  " );
	gEngfuncs.Con_Printf( string );
}

//Returns if it's multiplayer.
//Mostly used by the client side weapons.
//MODDD - obsolete, "IsMultiplayer" used in util_shared.h replaces this and
// can be called in shared code too, still functional.
//bool bIsMultiplayer ( void )
//{
//	return (gEngfuncs.GetMaxClients() == 1) ? FALSE : TRUE;
//}

//Just loads a v_ model.
void LoadVModel ( char *szViewModel, CBasePlayer *m_pPlayer )
{
	gEngfuncs.CL_LoadModel( szViewModel, &m_pPlayer->pev->viewmodel );
}

/*
=====================
HUD_PrepEntity

Links the raw entity to an entvars_s holder.  If a player is passed in as the owner, then
we set up the m_pPlayer field.
=====================
*/
void HUD_PrepEntity( CBaseEntity *pEntity, CBasePlayer *pWeaponOwner )
{
	memset( &ev[ num_ents ], 0, sizeof( entvars_t ) );
	pEntity->pev = &ev[ num_ents++ ];

	pEntity->Precache();
	pEntity->Spawn();

	// Only proceed with all this, if pWeaponOwner isn't NULL.
	// I sure hope it isn't NULL.

	//MODDD - now, only a check for whether this entity is a Player or not.
	// If not a player, we trust it's a weapon and all this makes sense.
	//if ( pWeaponOwner )
	if (!pEntity->IsPlayer())
	{
		CBasePlayerWeapon* tempWeaponRef = (CBasePlayerWeapon*)pEntity;
		tempWeaponRef->m_pPlayer = pWeaponOwner;
		RegisterWeapon(tempWeaponRef, g_pWpns);
	}//END OF pWeaponOwner check
}//END OF HUD_PrepEntity






/*
=====================
CBaseEntity :: Killed

If weapons code "kills" an entity, just set its effects to EF_NODRAW
=====================
*/
GENERATE_KILLED_IMPLEMENTATION(CBaseEntity)
{
	//easyPrintLine("MESSAGE2");
	pev->effects |= EF_NODRAW;
}


//MODDD - ????      This file seems to need re-implementations of several (including newly added) CBasePlayerWeapon methods from weapons.h
//(besides weapons.cpp ...    parallel implementations for the same exact prototypes?  Very strange.)
CBasePlayerWeapon::CBasePlayerWeapon(){
	//Starts as 0, not a garbage value, for ALL weapons.
	m_chargeReady = 0;
	
	//bitmask.
	//1 = primary held and ready to fire.
	//2 = secondary held and ready to fire.
	buttonFiltered = 0;

	bothFireButtonsMode = 2;
}


//MODDD - should a weapon NOT have any deploy methods, this default will also undo the player silencer render effect.  Just for safety.
//...not using this 128 for renderfx anymore, at least not for this.
BOOL CBasePlayerWeapon :: Deploy(){

	//m_pPlayer->pev->renderfx &= ~128;
	return CBasePlayerItem::Deploy();
}
/*
=====================
CBasePlayerWeapon :: DefaultReload
=====================
*/
BOOL CBasePlayerWeapon :: DefaultReload( int iClipSize, int iAnim, float fDelay, int body )
{
	//easyPrintLine("MESSAGE3");
	
	if(m_pPlayer->cheat_infiniteammoMem != 1){
		if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
			return FALSE;

		int j = min(iClipSize - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);	
		//MODDD - is this edit ok?  To help with things that mess with the max-count like the glock with old reload logic.
		//if (j == 0)
		if (j <= 0)
			return FALSE;
	}else{
		if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 1;

		int j = min(iClipSize - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);
		if (j <= 0)
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 1;

		return TRUE;
	}


	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + fDelay;

	//MODDD - seems to make sense?  undo if this is more problematic.
	if(pev->body != body){
		pev->body = body;
	}

	//!!UNDONE -- reload sound goes here !!!     <--- MODDD - not my message.
	SendWeaponAnim( iAnim, UseDecrement(), body );
	
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
BOOL CBasePlayerWeapon :: CanDeploy( void ) 
{
	//easyPrintLine("MESSAGE4");
	BOOL bHasAmmo = 0;

	if ( !pszAmmo1() )
	{
		// this weapon doesn't use ammo, can always deploy.
		return TRUE;
	}

	if ( pszAmmo1() )
	{
		bHasAmmo |= (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] != 0);
	}
	if ( pszAmmo2() )
	{
		bHasAmmo |= (m_pPlayer->m_rgAmmo[m_iSecondaryAmmoType] != 0);
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
BOOL CBasePlayerWeapon :: DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal /* = 0 */, int body, float deployAnimTime, float fireDelayTime )
{
	//easyPrintLine("MESSAGE5");
	
	if ( !CanDeploy() )
		return FALSE;

	gEngfuncs.CL_LoadModel( szViewModel, &m_pPlayer->pev->viewmodel );
	

	//MODDD - seems to make sense?  undo if this is more problematic.
	if(pev->body != body){
		pev->body = body;
	}
	SendWeaponAnim( iAnim, skiplocal, body );
	
	
	//MODDD - I SWEAR IF YOU GO FORGETTING THIS SHIT LIKE THIS AGAIN I WILL <<<redacted for fucking insanity>>>
	//        Yes that's me talking to myself. Ain't I healthy?
	if(fireDelayTime == -1){
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
void CBasePlayerWeapon::DefaultHolster( int iAnim, int skiplocal /* = 0 */, int body, float holsterAnimTime )
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

	SendWeaponAnim( iAnim );


}//END OF DefaultHolster



/*
=====================
CBasePlayerWeapon :: PlayEmptySound

=====================
*/
BOOL CBasePlayerWeapon :: PlayEmptySound( void )
{
	//easyPrintLine("MESSAGE6");
	if (m_iPlayEmptySound)
	{
		HUD_PlaySound( "weapons/357_cock1.wav", 0.8 );
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
void CBasePlayerWeapon :: ResetEmptySound( void )
{
	m_iPlayEmptySound = 1;
}

/*
=====================
CBasePlayerWeapon::Holster

Put away weapon
=====================
*/
void CBasePlayerWeapon::Holster( int skiplocal /* = 0 */ )
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
void CBasePlayerWeapon::SendWeaponAnim( int iAnim, int skiplocal, int body )
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnim %d", iAnim);
	
	//MODDD - seems to make sense?  undo if this is more problematic.
	/*
	if(pev->body != body){
		pev->body = body;
	}
	*/

	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim( iAnim, body, 0 );
}


void CBasePlayerWeapon::SendWeaponAnimReverse( int iAnim, int skiplocal, int body )
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnimReverse %d", iAnim);
	
	//MODDD - seems to make sense?  undo if this is more problematic.
	/*
	if(pev->body != body){
		pev->body = body;
	}
	*/
	
	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim( iAnim, body, 0 );
}



void CBasePlayerWeapon::SendWeaponAnimBypass( int iAnim, int body )
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnimBypass %d", iAnim);
	
	m_pPlayer->pev->weaponanim = iAnim;
	HUD_SendWeaponAnim( iAnim, body, 2 );
}

void CBasePlayerWeapon::SendWeaponAnimBypassReverse( int iAnim, int body )
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnimBypassReverse %d", iAnim);

	
	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;
	
	HUD_SendWeaponAnim( iAnim, body, 2 );
}



void CBasePlayerWeapon::SendWeaponAnimClientOnly( int iAnim, int body )
{
	this->m_fireState &= ~128;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnimClientOnly %d", iAnim);
	

	m_pPlayer->pev->weaponanim = iAnim;
	
	HUD_SendWeaponAnim( iAnim, body, 2 );
}

//MODDD
void CBasePlayerWeapon::SendWeaponAnimClientOnlyReverse( int iAnim, int body )
{
	this->m_fireState |= 128;
	//gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SendWeaponAnimClientOnlyReverse %d", iAnim);

	iAnim |= 128;

	m_pPlayer->pev->weaponanim = iAnim;

	HUD_SendWeaponAnim( iAnim, body, 2 );
}


//MODDD
void CBasePlayerWeapon::SendWeaponAnimServerOnly( int iAnim, int body )
{
	//client? no.
}

//MODDD
void CBasePlayerWeapon::SendWeaponAnimServerOnlyReverse( int iAnim, int body )
{
	//no.
}



/*
=====================
CBaseEntity::FireBulletsPlayer

Only produces random numbers to match the server ones.
=====================
*/
Vector CBaseEntity::FireBulletsPlayer ( ULONG cShots, Vector vecSrc, Vector vecDirShooting, Vector vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker, int shared_rand )
{
	float x, y, z;

	for ( ULONG iShot = 1; iShot <= cShots; iShot++ )
	{
		if ( pevAttacker == NULL )
		{
			// get circular gaussian spread
			do {
					x = RANDOM_FLOAT(-0.5, 0.5) + RANDOM_FLOAT(-0.5, 0.5);
					y = RANDOM_FLOAT(-0.5, 0.5) + RANDOM_FLOAT(-0.5, 0.5);
					z = x*x+y*y;
			} while (z > 1);
		}
		else
		{
			//Use player's random seed.
			// get circular gaussian spread
			x = UTIL_SharedRandomFloat( shared_rand + iShot, -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 1 + iShot ) , -0.5, 0.5 );
			y = UTIL_SharedRandomFloat( shared_rand + ( 2 + iShot ), -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 3 + iShot ), -0.5, 0.5 );
			z = x * x + y * y;
		}
			
	}

    return Vector ( x * vecSpread.x, y * vecSpread.y, 0.0 );
}


///////////////////////////////////////////////

//MODDD - all new, clones.
void CBasePlayerWeapon::setchargeReady(int arg){
	int mem = m_chargeReady & 64;
	m_chargeReady = arg;
	if(mem){
		m_chargeReady |= 64;
	}
}
int CBasePlayerWeapon::getchargeReady(void){
	return m_chargeReady & ~64;
}
void CBasePlayerWeapon::forceBlockLooping(void){
	m_chargeReady |= 64;
}
void CBasePlayerWeapon::stopBlockLooping(void){
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
float CBasePlayerWeapon::randomIdleAnimationDelay(void){
	if(EASY_CVAR_GET(firstPersonIdleDelayMin) > 0 && EASY_CVAR_GET(firstPersonIdleDelayMax) > 0){
		//let's go.
		float rand = UTIL_SharedRandomFloat(m_pPlayer->random_seed, EASY_CVAR_GET(firstPersonIdleDelayMin), EASY_CVAR_GET(firstPersonIdleDelayMax));
		//easyPrintLine("OK client client %.2f", rand);
		return rand;
	}else{
		return 0;
	}
}//END OF randomIdleAnimationDelay



//MODDD - new, now called.
void CBasePlayerWeapon::ItemPostFrameThink(){

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
	/*
	//easyForcePrintLine("HOLS:%d HOLSTIM:%.2f NEXTATTA:%.2f TIME:%.2f",m_pPlayer->m_bHolstering, m_pPlayer->m_fCustomHolsterWaitTime, m_pPlayer->m_flNextAttack, gpGlobals->time);
	//MODDD - should be a good place to check for deplying the next weapon.
	if (m_pPlayer->m_bHolstering && gpGlobals->time >= m_pPlayer->m_fCustomHolsterWaitTime) { //m_pPlayer->m_flNextAttack <= 0.0){
		//done holstering? complete the switch to the picked weapon. Deploy that one.

		m_pPlayer->m_bHolstering = FALSE;
		m_pPlayer->setActiveItem(m_pPlayer->m_pQueuedActiveItem);
		m_pPlayer->m_pQueuedActiveItem = NULL;
	}
	*/
	


	//easyForcePrintLine("AFTR HOLS:%d HOLSTIM:%.2f NEXTATTA:%.2f TIME:%.2f",m_pPlayer->m_bHolstering, m_pPlayer->m_fCustomHolsterWaitTime, m_pPlayer->m_flNextAttack, gpGlobals->time);
	

	CBasePlayerItem::ItemPostFrameThink();
}//END OF ItemPostFrameThink


//MODDD - added.  Doubt it will work
void CBasePlayerWeapon::ItemPreFrame( ){
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
void CBasePlayerWeapon::ItemPostFrame( )
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

	if ((m_fInReload) && (m_pPlayer->m_flNextAttack <= 0.0))
	{
//MODDD - NOTE.  Oh dear, a note left as-is.  Or don't fix what ain't broken?
// I don't know what ingame issue this is referring to.
#if 0 // FIXME, need ammo on client to make this work right
		// complete the reload. 
		int j = min( iMaxClip() - m_iClip, m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]);	

		// Add them to the clip
		m_iClip += j;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= j;
#else	
		m_iClip += 10;
#endif
		//MODDD - new event!
		this->OnReloadApply();

		m_fInReload = FALSE;
	}

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
		if ( pszAmmo2() && !m_pPlayer->m_rgAmmo[SecondaryAmmoIndex()] )
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
		if ( (m_iClip == 0 && pszAmmo1()) || (iMaxClip() == -1 && !m_pPlayer->m_rgAmmo[PrimaryAmmoIndex()] ) )
		{
			m_fFireOnEmpty = TRUE;
		}

		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("Postframe PrimaryAttack!");
		PrimaryAttack();
		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("My new npa: %.2f", m_flNextPrimaryAttack);

	}
	else if ( m_pPlayer->pev->button & IN_RELOAD && iMaxClip() != WEAPON_NOCLIP && !m_fInReload ) 
	{
		// reload when reload is pressed, or if no buttons are down and weapon is empty.
		Reload();
	}
	else if ( !(m_pPlayer->pev->button & (IN_ATTACK|IN_ATTACK2) ) )
	{
		// no fire buttons down

		m_fFireOnEmpty = FALSE;

		// weapon is useable. Reload if empty and weapon has waited as long as it has to after firing
		if ( m_iClip == 0 && !(iFlags() & ITEM_FLAG_NOAUTORELOAD) && m_flNextPrimaryAttack < 0.0 )
		{
			Reload();
			return;
		}

		WeaponIdle( );
		return;
	}
	
	// catch all
	if ( ShouldWeaponIdle() )
	{
		WeaponIdle();
	}
}


/*
=====================
CBasePlayer::SelectItem

  Switch weapons
=====================
*/
//MODDD - IMPORTANT. Apparently this may as well have been dummied, but was found in the as-is script.
//        This method is never called for the client, any vars set by the server calling SelectItem on
//        its own side need to be sent to the client.
void CBasePlayer::SelectItem(const char *pstr)
{
	//easyPrintLine("MESSAGE11");
	if (!pstr)
		return;

	CBasePlayerItem *pItem = NULL;

	if (!pItem)
		return;

	
	if (pItem == m_pActiveItem)
		return;

	/*
	if (m_pActiveItem)
		m_pActiveItem->Holster( );
	
	m_pLastItem = m_pActiveItem;
	m_pActiveItem = pItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
	}
	*/

	easyForcePrintLine("$$$$ AHA I SHOULD NEVER EVER HAPPEN, REPORT ME IF I DO YOU HEAR?! $$$$");
}//END OF SelectItem


void CBasePlayer::setActiveItem(CBasePlayerItem* argItem){
	
	m_pLastItem = m_pActiveItem;
	m_pActiveItem = argItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}
}


// if allowed by cl_holster, and sets the target weapon to deploy after that finishes.
// Or goes to deploy instantly like retail.
// a filter that calls setActiveItem straight away if holstering is disabled.
void CBasePlayer::setActiveItem_HolsterCheck(CBasePlayerItem* argItem) {

	// ******SCRIPT THIS REPLACES, exact or similar-intent repeated a few places
	/*
	// FIX, this needs to queue them up and delay
	if (m_pActiveItem)
		m_pActiveItem->Holster( );

	m_pLastItem = m_pActiveItem;
	m_pActiveItem = pItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}
	*/
	///////////////////////////////////////////////////////////////////////////

	if (m_pActiveItem) {
		//easyForcePrintLine("OH yeah AM I HOLSTERIN ALREADY %d", m_bHolstering);

		if (!m_bHolstering) {
			// don't holster the currently equipped weapon if already in the middle of holstering.
			m_pActiveItem->Holster();
			m_bHolstering = TRUE;
		}

		// cl_holster
		if (fHolsterAnimsEnabled == TRUE) {
			// using holster anim? Tell the currently equipped item to change to this weapon when that is over.
			m_pQueuedActiveItem = argItem;  //set this later instead, after the holster anim is done.
		}
		else {
			// Not using holster anims? Immediately change weapon.
			setActiveItem(argItem);
			m_bHolstering = FALSE;
		}

	}
	else {
		// just pick it now.
		setActiveItem(argItem);
	}

}









/*
=====================
CBasePlayer::SelectLastItem

=====================
*/
void CBasePlayer::SelectLastItem(void)
{
	//easyPrintLine("MESSAGE12");
	if (!m_pLastItem)
	{
		return;
	}

	if ( m_pActiveItem && !m_pActiveItem->CanHolster() )
	{
		return;
	}

	if (m_pActiveItem)
		m_pActiveItem->Holster( );
	
	CBasePlayerItem *pTemp = m_pActiveItem;
	m_pActiveItem = m_pLastItem;
	m_pLastItem = pTemp;
	m_pActiveItem->Deploy( );
}








//MODDD NOTE.
// And If wanted, remove these from hl_baseentity.cpp and implement them here.
//GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION_DUMMY_CLIENT(CBasePlayer)
//GENERATE_GIBMONSTER_IMPLEMENTATION_DUMMY_CLIENT(CBasePlayer)


/*
=====================
CBasePlayer::Killed

=====================
*/
GENERATE_KILLED_IMPLEMENTATION(CBasePlayer)
{
	// Holster weapon immediately, to allow it to cleanup
	if ( m_pActiveItem )
		 m_pActiveItem->Holster( );
	
	g_irunninggausspred = false;

}

//MODD - NEW.
void CBasePlayer::onDelete(void) {
	// ???  anything we want to do here?
}





/*
=====================
CBasePlayer::Spawn

=====================
*/
void CBasePlayer::Spawn( void )
{
	if (m_pActiveItem)
		m_pActiveItem->Deploy( );

	g_irunninggausspred = false;
}
//MODDD - for the alt version, if ever called, do the same.  Doesn't involve that BOOL.
void CBasePlayer::Spawn( BOOL revived )
{
	if (m_pActiveItem)
		m_pActiveItem->Deploy( );

	g_irunninggausspred = false;
}



void CBasePlayer::Activate( void )
{
	//um, what.

}

/*
=====================
UTIL_TraceLine

Don't actually trace, but act like the trace didn't hit anything.
=====================
*/

//MODDD - why not this too?
void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr )
{
	memset( ptr, 0, sizeof( *ptr ) );
	ptr->flFraction = 1.0;
}
void UTIL_TraceLine( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr )
{
	memset( ptr, 0, sizeof( *ptr ) );
	ptr->flFraction = 1.0;
}
//MODDD - why not this too?
TraceResult	UTIL_GetGlobalTrace(){
	//MODDD - mimicking how other dummied out versions handle it?
	TraceResult tr;
	tr.flFraction		= 1.0;
	return tr;
}

void UTIL_TraceHull( const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr )
{
	memset( ptr, 0, sizeof( *ptr ) );
	ptr->flFraction = 1.0;
}

//MODDDD NOTE - never used. at all. Does this work?
void UTIL_TraceModel( const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr )
{
	memset( ptr, 0, sizeof( *ptr ) );
	ptr->flFraction = 1.0;
}


/*
=====================
UTIL_ParticleBox

For debugging, draw a box around a player made out of particles
=====================
*/
void UTIL_ParticleBox( CBasePlayer *pPlayer, float *mins, float *maxs, float life, unsigned char r, unsigned char g, unsigned char b )
{
	int i;
	vec3_t mmin, mmax;

	for ( i = 0; i < 3; i++ )
	{
		mmin[ i ] = pPlayer->pev->origin[ i ] + mins[ i ];
		mmax[ i ] = pPlayer->pev->origin[ i ] + maxs[ i ];
	}

	gEngfuncs.pEfxAPI->R_ParticleBox( (float *)&mmin, (float *)&mmax, 5.0, 0, 255, 0 );
}

/*
=====================
UTIL_ParticleBoxes

For debugging, draw boxes for other collidable players
=====================
*/
void UTIL_ParticleBoxes( void )
{
	int idx;
	physent_t *pe;
	cl_entity_t *pPlayer;
	vec3_t mins, maxs;
	
	gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	pPlayer = gEngfuncs.GetLocalPlayer();
	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( pPlayer->index - 1 );	

	for ( idx = 1; idx < 100; idx++ )
	{
		pe = gEngfuncs.pEventAPI->EV_GetPhysent( idx );
		if ( !pe )
			break;

		if ( pe->info >= 1 && pe->info <= gEngfuncs.GetMaxClients() )
		{
			mins = pe->origin + pe->mins;
			maxs = pe->origin + pe->maxs;

			gEngfuncs.pEfxAPI->R_ParticleBox( (float *)&mins, (float *)&maxs, 0, 0, 255, 2.0 );
		}
	}

	gEngfuncs.pEventAPI->EV_PopPMStates();
}

/*
=====================
UTIL_ParticleLine

For debugging, draw a line made out of particles
=====================
*/
//MODDD - oh.  pPlayer isn't even used in here?
void UTIL_ParticleLine( CBasePlayer * pPlayer, float *start, float *end, float life, unsigned char r, unsigned char g, unsigned char b )
{
	gEngfuncs.pEfxAPI->R_ParticleLine( start, end, r, g, b, life );
}

/*
=====================
CBasePlayerWeapon::PrintState

For debugging, print out state variables to log file
=====================
*/
void CBasePlayerWeapon::PrintState( void )
{
	COM_Log( "c:\\hl.log", "%.4f ", gpGlobals->time );
	COM_Log( "c:\\hl.log", "%.4f ", m_pPlayer->m_flNextAttack );
	COM_Log( "c:\\hl.log", "%.4f ", m_flNextPrimaryAttack );
	COM_Log( "c:\\hl.log", "%.4f ", m_flTimeWeaponIdle - gpGlobals->time);
	COM_Log( "c:\\hl.log", "%i ", m_iClip );
}

/*
=====================
HUD_InitClientWeapons

Set up weapons, player and functions needed to run weapons code client-side.
=====================
*/


//Note - does nothing if already initialized from an earlier call.
void HUD_InitClientWeapons( void )
{
	static int initialized = 0;
	if ( initialized )
		return;  //right there.

	initialized = 1;


	// Set up pointer ( dummy object )
	gpGlobals = &Globals;

	// Fill in current time ( probably not needed )
	gpGlobals->time = gEngfuncs.GetClientTime();
	


	// Fake functions
	g_engfuncs.pfnPrecacheModel		= stub_PrecacheModel;
	g_engfuncs.pfnPrecacheSound		= stub_PrecacheSound;
	g_engfuncs.pfnPrecacheEvent		= stub_PrecacheEvent;
	g_engfuncs.pfnNameForFunction	= stub_NameForFunction;
	g_engfuncs.pfnSetModel			= stub_SetModel;
	g_engfuncs.pfnSetClientMaxspeed = HUD_SetMaxSpeed;

	// Handled locally
	g_engfuncs.pfnPlaybackEvent		= HUD_PlaybackEvent;
	g_engfuncs.pfnAlertMessage		= AlertMessage;

	// Pass through to engine
	g_engfuncs.pfnPrecacheEvent		= gEngfuncs.pfnPrecacheEvent;
	g_engfuncs.pfnRandomFloat		= gEngfuncs.pfnRandomFloat;
	g_engfuncs.pfnRandomLong		= gEngfuncs.pfnRandomLong;



	// MODDD - place for script similar between client and serverside.
	PrecacheShared();

	// Allocate a slot for the local player
	HUD_PrepEntity( &player		, NULL );

	// Allocate slot(s) for each weapon that we are going to be predicting
	HUD_PrepEntity( &g_Glock	, &player );
	HUD_PrepEntity( &g_Crowbar	, &player );
	HUD_PrepEntity( &g_Python	, &player );
	HUD_PrepEntity( &g_Mp5	, &player );
	HUD_PrepEntity( &g_Crossbow	, &player );
	HUD_PrepEntity( &g_Shotgun	, &player );
	HUD_PrepEntity( &g_Rpg	, &player );
	HUD_PrepEntity( &g_Gauss	, &player );
	HUD_PrepEntity( &g_Egon	, &player );
	HUD_PrepEntity( &g_HGun	, &player );
	HUD_PrepEntity( &g_HandGren	, &player );
	HUD_PrepEntity( &g_Satchel	, &player );
	HUD_PrepEntity( &g_Tripmine	, &player );
	HUD_PrepEntity( &g_Snark	, &player );
	//MODDD - new 
	HUD_PrepEntity( &g_ChumToadWeapon	, &player );


}

/*
=====================
HUD_GetLastOrg

Retruns the last position that we stored for egon beam endpoint.
=====================
*/
void HUD_GetLastOrg( float *org )
{
	int i;
	
	// Return last origin
	for ( i = 0; i < 3; i++ )
	{
		org[i] = previousorigin[i];
	}
}

/*
=====================
HUD_SetLastOrg

Remember our exact predicted origin so we can draw the egon to the right position.
=====================
*/
void HUD_SetLastOrg( void )
{
	int i;
	
	// Offset final origin by view_offset
	for ( i = 0; i < 3; i++ )
	{
		previousorigin[i] = g_finalstate->playerstate.origin[i] + g_finalstate->client.view_ofs[ i ];
	}
}

/*
=====================
HUD_WeaponsPostThink

Run Weapon firing code on client
=====================
*/

void HUD_WeaponsPostThink( local_state_s *from, local_state_s *to, usercmd_t *cmd, double time, unsigned int random_seed )
{

	int i;
	int buttonsChanged;
	CBasePlayerWeapon *pWeapon = NULL;
	CBasePlayerWeapon *pCurrent;
	weapon_data_t nulldata, *pfrom, *pto;
	static int lasthealth;
	// MODDD - new.
	static float nextBlinkLogicFrame = -1;

	memset( &nulldata, 0, sizeof( nulldata ) );

	//MODDD - why can't this be done at the start of HUD_PostRunCMD, which calls this HUD_WeaponsPostThink method anyways?
	//HUD_InitClientWeapons();

	// Get current clock
	//MODDD - same. Why can't HUD_PostRunCMD do this instead?
	//gpGlobals->time = time;

	// Fill in data based on selected weapon
	// FIXME, make this a method in each weapon?  where you pass in an entity_state_t *?
	

	//MODDD - number of frames (skin values) for different stages of eye blinking. Just hardcoding since everything else player-model related is.
	//Default of 0, weapons with blinkable eyes should specify this.
	int numberOfEyeSkins = 0;
	//And in a 1 out of X chance every 0.1 seconds (one frame for NPC think logic), the blink will occur.
	int blinkChancePerFrame = 127;

	switch ( from->client.m_iId )
	{
		case WEAPON_CROWBAR:
			pWeapon = &g_Crowbar;
			break;
		
		case WEAPON_GLOCK:
			pWeapon = &g_Glock;
			break;
		
		case WEAPON_PYTHON:
			pWeapon = &g_Python;
			break;
			
		case WEAPON_MP5:
			pWeapon = &g_Mp5;
			break;

		case WEAPON_CROSSBOW:
			pWeapon = &g_Crossbow;
			break;

		case WEAPON_SHOTGUN:
			pWeapon = &g_Shotgun;
			break;

		case WEAPON_RPG:
			pWeapon = &g_Rpg;
			break;

		case WEAPON_GAUSS:
			pWeapon = &g_Gauss;
			break;

		case WEAPON_EGON:
			pWeapon = &g_Egon;
			break;

		case WEAPON_HORNETGUN:
			pWeapon = &g_HGun;
			break;

		case WEAPON_HANDGRENADE:
			pWeapon = &g_HandGren;
			break;

		case WEAPON_SATCHEL:
			pWeapon = &g_Satchel;
			break;

		case WEAPON_TRIPMINE:
			pWeapon = &g_Tripmine;
			break;

		case WEAPON_SNARK:
			numberOfEyeSkins = 3;
			blinkChancePerFrame = 33;
			pWeapon = &g_Snark;
			break;
		//MODDD - new
		case WEAPON_CHUMTOAD:
			numberOfEyeSkins = 3;
			blinkChancePerFrame = 50;
			pWeapon = &g_ChumToadWeapon;
			break;
			
	}//END OF weapon ID link

	// Store pointer to our destination entity_state_t so we can get our origin, etc. from it
	//  for setting up events on the client
	g_finalstate = to;

	// If we are running events/etc. go ahead and see if we
	//  managed to die between last frame and this one
	// If so, run the appropriate player killed or spawn function
	if ( g_runfuncs )
	{
		if ( to->client.health <= 0 && lasthealth > 0 )
		{
			player.Killed( NULL, 0 );
			
		}
		else if ( to->client.health > 0 && lasthealth <= 0 )
		{
			player.Spawn();
		}

		lasthealth = to->client.health;
	}

	// We are not predicting the current weapon, just bow out here.
	if ( !pWeapon )
		return;


	
	//???
	gEngfuncs.GetViewModel()->curstate.renderfx |= ISVIEWMODEL;
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;


	//100001
	//100000
	
	/*
	if(player.forceNoWeaponLoop == TRUE){
		gEngfuncs.GetViewModel()->curstate.renderfx |= FORCE_NOLOOP;
	}else{
		gEngfuncs.GetViewModel()->curstate.renderfx &= ~FORCE_NOLOOP;
	}
	*/


	//unnecessary, if it was here.  Not used in this setting.
	pWeapon->m_chargeReady &= ~32;

	//MODDD
	//"pWeapon" is the currently equipped weapon here... I think.
	if(pWeapon->m_chargeReady & 64){
		pWeapon->m_chargeReady &= ~64;
		
		gEngfuncs.GetViewModel()->curstate.renderfx |= FORCE_NOLOOP;
	}else{
		//pWeapon->m_chargeReady &= ~64;  //unnecessary?
		gEngfuncs.GetViewModel()->curstate.renderfx &= ~FORCE_NOLOOP;
	}
	
	//easyPrintLine("POOOOOOP %d", player.forceNoWeaponLoop);
	


	//MODDD - moved from below, see the old ItmePostFrame call location there.
	// Don't go firing anything if we have died.
	// Or if we don't have a weapon model deployed
	//NEW LOCATION...


	BOOL letsNotNameThingsLikeThat = FALSE;

	for ( i = 0; i < 32; i++ )
	{
		pCurrent = g_pWpns[ i ];

		if ( !pCurrent )
		{
			continue;
		}

		pfrom = &from->weapondata[ i ];


		pCurrent->m_fInReload			= pfrom->m_fInReload;
		pCurrent->m_fInSpecialReload	= pfrom->m_fInSpecialReload;
//		pCurrent->m_flPumpTime			= pfrom->m_flPumpTime;
		pCurrent->m_iClip				= pfrom->m_iClip;

		if(pCurrent->m_iId == WEAPON_SHOTGUN){
			//easyForcePrintLine("UPDATE A: %.2f -> %.2f", pCurrent->m_flNextPrimaryAttack, pfrom->m_flNextPrimaryAttack);
		}

		//Little filter...
		
		//If the new value would be less than 0 but our next primary attack is noticably above 0, this is too much chance in one frame. Deny the transfer this time.
		if( !(pfrom->m_flNextPrimaryAttack <= 0 && pCurrent->m_flNextPrimaryAttack >= 0.1) ){
			pCurrent->m_flNextPrimaryAttack	= pfrom->m_flNextPrimaryAttack;
		}
		if( !(pfrom->m_flNextSecondaryAttack <= 0 && pCurrent->m_flNextSecondaryAttack >= 0.1) ){
			pCurrent->m_flNextSecondaryAttack = pfrom->m_flNextSecondaryAttack;
		}
		if( !(pfrom->m_flTimeWeaponIdle <= 0 && pCurrent->m_flTimeWeaponIdle >= 0.1) ){
			pCurrent->m_flTimeWeaponIdle = pfrom->m_flTimeWeaponIdle;
		}else{
			if(EASY_CVAR_GET(viewModelSyncFixPrintouts)==1) easyForcePrintLine("*****VIEWMODEL SYNCH FIX APPLIED.");
		}


		pCurrent->pev->fuser1			= pfrom->fuser1;
		pCurrent->m_flStartThrow		= pfrom->fuser2;
		pCurrent->m_flReleaseThrow		= pfrom->fuser3;



		//NOTICE - despite already existing, "fuser4" seems untouched by the DLL.  Do not rely on it!
		//pCurrent->fuser4				= pfrom->fuser4;
		/*
		pCurrent->fuser5				= pfrom->fuser5;
		pCurrent->fuser6				= pfrom->fuser6;
		pCurrent->fuser7				= pfrom->fuser7;
		pCurrent->fuser8				= pfrom->fuser8;
		*/

		pCurrent->m_chargeReady			= pfrom->iuser1;
		pCurrent->m_fInAttack			= pfrom->iuser2;
		pCurrent->m_fireState			= pfrom->iuser3;

		pCurrent->m_iSecondaryAmmoType		= (int)from->client.vuser3[ 2 ];
		pCurrent->m_iPrimaryAmmoType		= (int)from->client.vuser4[ 0 ];
		player.m_rgAmmo[ pCurrent->m_iPrimaryAmmoType ]	= (int)from->client.vuser4[ 1 ];
		player.m_rgAmmo[ pCurrent->m_iSecondaryAmmoType ]	= (int)from->client.vuser4[ 2 ];


		//This stuff used to be above. Isn't this area better?

		if(pWeapon->m_iId == pCurrent->m_iId){
		
			if(pWeapon->m_iId == WEAPON_EGON && pfrom->m_iId == WEAPON_EGON && pCurrent->m_iId == WEAPON_EGON){
				//easyPrintLine("SYNC TEST EGON: %.8f, %.8f, %0.2f", pCurrent->m_flStartThrow, pfrom->fuser2, gpGlobals->time);
				//easyPrintLine("SYNC TEST EGON 2: %df, %d, %0.2f", pCurrent->m_fInAttack, pfrom->iuser2, gpGlobals->time);
			}
			if( (pWeapon->m_iId == WEAPON_GLOCK && pfrom->m_iId == WEAPON_GLOCK || pCurrent->m_iId == WEAPON_GLOCK) ||
				(pWeapon->m_iId == WEAPON_SHOTGUN && pfrom->m_iId == WEAPON_SHOTGUN || pCurrent->m_iId == WEAPON_SHOTGUN)
				){
				//easyPrintLine("SYNC TEST GLOCK: %d, %d, %.2f", pCurrent->m_fireState, pfrom->iuser3, gpGlobals->time);

				//pWeapon == &g_Glock

				//m_fireState
				

				/*
				if(pfrom->iuser3 & 128){
					letsNotNameThingsLikeThat = TRUE;
					//pfrom->iuser3 &= ~128;
					gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
				}
				*/
			}
		}
	}//END OF yet another weapons loop.

	

	// For random weapon events, use this seed to seed random # generator
	player.random_seed = random_seed;

	// Get old buttons from previous state.
	player.m_afButtonLast = from->playerstate.oldbuttons;

	// Which buttsons chave changed
	buttonsChanged = (player.m_afButtonLast ^ cmd->buttons);	// These buttons have changed this frame
	
	// Debounced button codes for pressed/released
	// The changed ones still down are "pressed"
	player.m_afButtonPressed =  buttonsChanged & cmd->buttons;	
	// The ones not down are "released"
	player.m_afButtonReleased = buttonsChanged & (~cmd->buttons);

	// Set player variables that weapons code might check/alter
	// TAGGG - CRITICAL CRITICAL CRITICAL!!!  Kinda important methinks.
	// Although this looks to be for receiving button info from the server to the client?
	// Seems a bit backwards, but all this prediction mumbo jumbo can be weird.
	// input.cpp's own "->button" transfer is probably more forward to understand.
	player.pev->button = cmd->buttons;

	player.pev->velocity = from->client.velocity;
	player.pev->flags = from->client.flags;

	player.pev->deadflag = from->client.deadflag;
	player.pev->waterlevel = from->client.waterlevel;
	player.pev->maxspeed    = from->client.maxspeed;
	player.pev->fov = from->client.fov;

	//IS THAT OKAY???
	//player.pev->weaponanim = from->client.weaponanim;


	//MODDD - little insertion here...
	//LITTLE TIP ABOUT WEAPON ANIMS: send an anim with 128 as part of a bitmask and it will animate backwards!


	//player.pev->weaponanim


	
	/*
	gEngfuncs.GetViewModel()->curstate.renderfx |= ISVIEWMODEL;


	if(from->client.weaponanim & 128){

		if(EASY_CVAR_GET(testVar)==1){
			easyPrintLine("BACKWARDS!!!! %d : %d", from->client.weaponanim & ~128, from->client.weaponanim);
		}

		from->client.weaponanim &= ~128;
		gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	}else{
		if(EASY_CVAR_GET(testVar)==1){
			easyPrintLine("NOOOOOT BACKWARDS!!!! %d", from->client.weaponanim);
		}
		gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;
	}
	*/

	if(pWeapon == &g_Glock){
		//DIE.  DIE HARD.

	}


	player.pev->weaponanim = from->client.weaponanim;

	player.pev->viewmodel = from->client.viewmodel;
	player.m_flNextAttack = from->client.m_flNextAttack;
	player.m_flNextAmmoBurn = from->client.fuser2;
	player.m_flAmmoStartCharge = from->client.fuser3;

	//Stores all our ammo info, so the client side weapons can use them.
	player.ammo_9mm			= (int)from->client.vuser1[0];
	player.ammo_357			= (int)from->client.vuser1[1];
	player.ammo_argrens		= (int)from->client.vuser1[2];
	player.ammo_bolts		= (int)from->client.ammo_nails; //is an int anyways...
	player.ammo_buckshot	= (int)from->client.ammo_shells; 
	player.ammo_uranium		= (int)from->client.ammo_cells;
	player.ammo_hornets		= (int)from->client.vuser2[0];
	player.ammo_rockets		= (int)from->client.ammo_rockets;

	
	// Point to current weapon object
	if ( from->client.m_iId )
	{
		player.m_pActiveItem = g_pWpns[ from->client.m_iId ];
	}


	//easyForcePrintLine("AW snao D %.2f", player.m_flNextAttack);


	if ( player.m_pActiveItem->m_iId == WEAPON_RPG )
	{
		 ( ( CRpg * )player.m_pActiveItem)->m_fSpotActive = (int)from->client.vuser2[ 1 ];
		 ( ( CRpg * )player.m_pActiveItem)->m_cActiveRockets = (int)from->client.vuser2[ 2 ];
	}
	//MODDD - added.
	else if(player.m_pActiveItem->m_iId == WEAPON_PYTHON){
		( ( CPython * )player.m_pActiveItem)->m_fSpotActive = (int)from->client.vuser2[ 1 ];
	}

	//MODDD


	/*
	//it's all in interpretation...  (only things trying to use this will ever get this right)
	if( !(pWeapon->m_fireState & 128)){
		
		gEngfuncs.GetViewModel()->curstate.renderfx |= ISVIEWMODEL;
		gEngfuncs.GetViewModel()->curstate.renderfx &= ~ANIMATEBACKWARDS;

	}else{
		
		gEngfuncs.GetViewModel()->curstate.renderfx |= ISVIEWMODEL;
		gEngfuncs.GetViewModel()->curstate.renderfx |= ANIMATEBACKWARDS;
	}
	*/


	//easyPrintLine("WHAT??? %d %d %d", gEngfuncs.GetViewModel()->curstate.renderfx,  (DONOTDRAWSHADOW | ISPLAYER), ISVIEWMODEL);
	

	//is this okay?
	if(flag_apply_m_flTimeWeaponIdle){
		easyForcePrintLine("FIGHTERS LAUNCHED. newidle: %.2f", stored_m_flTimeWeaponIdle);
		pWeapon->m_flTimeWeaponIdle = stored_m_flTimeWeaponIdle;
		flag_apply_m_flTimeWeaponIdle = FALSE;
	}
	if(flag_apply_m_fJustThrown){
		easyForcePrintLine("FIGHTERS LAUNCHED 2. newthrown: %d", stored_m_fJustThrown);
		//can only work if our weapon is a snark or chumtoad.

		if(pWeapon->m_iId == WEAPON_SNARK){
			CSqueak* tempSqueak = static_cast<CSqueak*>(pWeapon);
			tempSqueak->m_fJustThrown = stored_m_fJustThrown;
		}else if(pWeapon->m_iId == WEAPON_CHUMTOAD){
			CChumToadWeapon* tempToad = static_cast<CChumToadWeapon*>(pWeapon);
			tempToad->m_fJustThrown = stored_m_fJustThrown;
		}
		flag_apply_m_fJustThrown = FALSE;
	}



	//MODDD - OLD LOCATION OF ItemPostFrame() CALLS.  Moved to back here, some issues with the new location.
	if ( ( player.pev->deadflag != ( DEAD_DISCARDBODY + 1 ) ) && 
		 !CL_IsDead() && player.pev->viewmodel && !g_iUser1 )
	{
		//MODDD - just like the player does, call this too.		
		pWeapon->ItemPostFrameThink( );
			


		if ( player.m_flNextAttack <= 0 )
		{
			pWeapon->ItemPostFrame();
		}
	}
	
	// Assume that we are not going to switch weapons
	to->client.m_iId					= from->client.m_iId;

	// Now see if we issued a changeweapon command ( and we're not dead )
	if ( cmd->weaponselect && ( player.pev->deadflag != ( DEAD_DISCARDBODY + 1 ) ) )
	{
		// Switched to a different weapon?
		if ( from->weapondata[ cmd->weaponselect ].m_iId == cmd->weaponselect )
		{

			
			CBasePlayerWeapon *pNew = g_pWpns[ cmd->weaponselect ];

			//MODDD NOTICE - unlike the player's (player.cpp) own script that works similary to this in the SelectItem method, serverside only there,
			//               this does nothing at all if pNew turns out to be NULL. SelectItem will still set the current item to NULL at least,
			//               and set the LastWeapon to what it was.
			//               Is it okay this way? TODO - test this sometime.
			//               NEVERMIND - my understanding is a little off. The player's currently equipped weapon to holster there, is m_pActiveItem (and here)
			//               so it will be ok to change weaps even if it is null.  pNew is what to change to, which must not be null. this is ok, I think.
			
			if ( pNew && ( pNew != pWeapon )
				//MODDD - new condition to mirror the player's change. If queueing up a weapon during the holster anim,
				//        it can be the same as you started with.
				|| (player.m_pQueuedActiveItem != NULL && pNew != player.m_pQueuedActiveItem)
				)
			{

				//MODDD - now anticipates the holster animations better.
				/*
				// Put away old weapon
				if (player.m_pActiveItem)
					player.m_pActiveItem->Holster( );
				
				player.m_pLastItem = player.m_pActiveItem;
				player.m_pActiveItem = pNew;

				// Deploy new weapon
				if (player.m_pActiveItem)
				{
					player.m_pActiveItem->Deploy( );
				}

				// Update weapon id so we can predict things correctly.
				to->client.m_iId = cmd->weaponselect;
				*/



				/*
				if(player.m_pActiveItem){
					if(player.m_bHolstering == FALSE){
						player.m_pActiveItem->Holster();
						player.m_bHolstering = TRUE;
					}
					player.m_pQueuedActiveItem = pNew;  //set this later instead, after the holster anim is done.
				}else{
					//just pick it now.
					player.setActiveItem(pNew);

				}
				
				*/



				/*
				
				if(player.m_pActiveItem){
					if(player.m_bHolstering == FALSE){
						player.m_pActiveItem->Holster();
						player.m_bHolstering = TRUE;
					}
					player.m_pQueuedActiveItem = pNew;  //set this later instead, after the holster anim is done.
				}else{
					//just pick it now.
					player.setActiveItem(pNew);

				}

				*/


				
				if(!player.m_bHolstering){
					//don't holster the currently equipped weapon if already in the middle of holstering.
					player.m_pActiveItem->Holster( );
					player.m_bHolstering = TRUE;
				}


				if(EASY_CVAR_GET(cl_holster) == 1){
					//using holster anim? Tell the currently equipped item to change to this weapon when that is over.
					player.m_pQueuedActiveItem = pNew;  //set this later instead, after the holster anim is done.
				}else{
					//Not using holster anims? Immediately change weapon.
					player.setActiveItem(pNew);
					player.m_bHolstering = FALSE;
					player.m_pQueuedActiveItem = NULL; //just in case.
				}




				//Perhaps doing something with this would be safer.?
				//if(
				//6player.m_fCustomHolsterWaitTime = 45;

				//Is keeping this out safe regardless?
				// Update weapon id so we can predict things correctly.
				to->client.m_iId = cmd->weaponselect;


			}
		}
	}

	// Copy in results of prediction code
	to->client.viewmodel				= player.pev->viewmodel;
	to->client.fov						= player.pev->fov;
	to->client.weaponanim				= player.pev->weaponanim;
	to->client.m_flNextAttack			= player.m_flNextAttack;
	to->client.fuser2					= player.m_flNextAmmoBurn;
	to->client.fuser3					= player.m_flAmmoStartCharge;
	to->client.maxspeed					= player.pev->maxspeed;

	//HL Weapons
	to->client.vuser1[0]				= player.ammo_9mm;
	to->client.vuser1[1]				= player.ammo_357;
	to->client.vuser1[2]				= player.ammo_argrens;

	to->client.ammo_nails				= player.ammo_bolts;
	to->client.ammo_shells				= player.ammo_buckshot;
	to->client.ammo_cells				= player.ammo_uranium;
	to->client.vuser2[0]				= player.ammo_hornets;
	to->client.ammo_rockets				= player.ammo_rockets;

	if ( player.m_pActiveItem->m_iId == WEAPON_RPG )
	{
		 from->client.vuser2[ 1 ] = ( ( CRpg * )player.m_pActiveItem)->m_fSpotActive;
		 from->client.vuser2[ 2 ] = ( ( CRpg * )player.m_pActiveItem)->m_cActiveRockets;
	}
	//MODDD - addition
	else if(player.m_pActiveItem->m_iId == WEAPON_PYTHON){
		from->client.vuser2[ 1 ] = ( ( CPython * )player.m_pActiveItem)->m_fSpotActive;
		//??
	}



	//Apparently, animations sent from serverside are picked up here.

	// Make sure that weapon animation matches what the game .dll is telling us
	//  over the wire ( fixes some animation glitches )


	//easyForcePrintLine("WHAT THE FUCK c:%d f:%d t:%d pw:%d pr:%d", HUD_GetWeaponAnim(), from->client.weaponanim, to->client.weaponanim, pWeapon->m_pPlayer->pev->weaponanim, pWeapon->pev->sequence);
	

	int tempthingy = to->client.weaponanim;
	if (to->client.weaponanim == ANIM_NO_UPDATE){
		int xxx = 4;
	}
	if (to->client.weaponanim == 9) {
		int xxx = 4;
	}

	//MODDD - added check for "254".  That's a special code for, "the serverside anim request got cleared".
	// This probably happened from becoming irrelevant and we don't want to make the client play an
	// irrelevant animation, like putting on/taking off the glock silencer twice.
	if ( g_runfuncs && (to->client.weaponanim != ANIM_NO_UPDATE && HUD_GetWeaponAnim() != to->client.weaponanim ) )
	{
		int body = 2;

		//Pop the model to body 0.
		if ( pWeapon == &g_Tripmine ){
			 body = 0;
		}


		if(to->client.weaponanim & 64){
			//this is only a cheap trick to get the animation to change. Remove it.
			easyForcePrintLine("GET YA haha UP");

			/*
			to->client.weaponanim &= ~64;

			from->client.weaponanim &= ~64;
			pWeapon->pev->weaponanim &= ~64;

			pWeapon->m_pPlayer->pev->weaponanim &= ~64;
			*/

		}


		//Show laser sight/scope combo
		//MODDD - how this is done is slightly different.
		/*
		if ( pWeapon == &g_Python && bIsMultiplayer() )
			 body = 1;
		*/
		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("Client: Received anim from server. Existing anim: %d New: %d", HUD_GetWeaponAnim(), to->client.weaponanim);

		//MODDD
		if ( pWeapon == &g_Glock){
			// Force a fixed anim down to viewmodel
			HUD_SendWeaponAnim( to->client.weaponanim, g_Glock.m_fireState , 2);

		}else if( pWeapon == &g_Python){
			HUD_SendWeaponAnim( to->client.weaponanim, g_Python.m_fInAttack, 2 );
			
		}else if( pWeapon == &g_Shotgun){
			
			HUD_SendWeaponAnim( to->client.weaponanim, body, 2 );
			
		}else{
			// Force a fixed anim down to viewmodel
			HUD_SendWeaponAnim( to->client.weaponanim, body, 1 );
		}

		//put above in "else".
		//// Force a fixed anim down to viewmodel
		//HUD_SendWeaponAnim( to->client.weaponanim, body, 1 );
	}


	for ( i = 0; i < 32; i++ )
	{
		pCurrent = g_pWpns[ i ];

		pto = &to->weapondata[ i ];

		if ( !pCurrent )
		{
			memset( pto, 0, sizeof( weapon_data_t ) );
			continue;
		}
	
		pto->m_fInReload				= pCurrent->m_fInReload;
		pto->m_fInSpecialReload			= pCurrent->m_fInSpecialReload;
//		pto->m_flPumpTime				= pCurrent->m_flPumpTime;
		pto->m_iClip					= pCurrent->m_iClip; 

		if(pCurrent->m_iId == WEAPON_SHOTGUN){
			//easyForcePrintLine("UPDATE B: %.2f -> %.2f", pto->m_flNextPrimaryAttack, pCurrent->m_flNextPrimaryAttack);
		}

		pto->m_flNextPrimaryAttack		= pCurrent->m_flNextPrimaryAttack;
		pto->m_flNextSecondaryAttack	= pCurrent->m_flNextSecondaryAttack;
		pto->m_flTimeWeaponIdle			= pCurrent->m_flTimeWeaponIdle;
		pto->fuser1						= pCurrent->pev->fuser1;
		pto->fuser2						= pCurrent->m_flStartThrow;
		pto->fuser3						= pCurrent->m_flReleaseThrow;

		
		//NOTE: fuser4 seems unreliable, do not use!
		//pto->fuser4						= pCurrent->fuser4;
		/*
		pto->fuser5						= pCurrent->fuser5;
		pto->fuser6						= pCurrent->fuser6;
		pto->fuser7						= pCurrent->fuser7;
		pto->fuser8						= pCurrent->fuser8;
		*/
		
		//if(pCurrent->m_iId == WEAPON_GLOCK && pto->m_iId == WEAPON_GLOCK){

		//}else{
		pto->iuser1						= pCurrent->m_chargeReady;
		//}
		pto->iuser2						= pCurrent->m_fInAttack;
		pto->iuser3						= pCurrent->m_fireState;

		// Decrement weapon counters, server does this at same time ( during post think, after doing everything else )
		pto->m_flNextReload				-= cmd->msec / 1000.0;
		pto->m_fNextAimBonus			-= cmd->msec / 1000.0;
		
		if(pCurrent->m_iId == WEAPON_SHOTGUN){
			//easyForcePrintLine("UPDATE C: %.2f -> %.2f", pto->m_flNextPrimaryAttack, pto->m_flNextPrimaryAttack - cmd->msec / 1000.0 );
		}

		pto->m_flNextPrimaryAttack		-= cmd->msec / 1000.0;
		pto->m_flNextSecondaryAttack	-= cmd->msec / 1000.0;
		pto->m_flTimeWeaponIdle			-= cmd->msec / 1000.0;
		pto->fuser1						-= cmd->msec / 1000.0;

		to->client.vuser3[2]				= pCurrent->m_iSecondaryAmmoType;
		to->client.vuser4[0]				= pCurrent->m_iPrimaryAmmoType;
		to->client.vuser4[1]				= player.m_rgAmmo[ pCurrent->m_iPrimaryAmmoType ];
		to->client.vuser4[2]				= player.m_rgAmmo[ pCurrent->m_iSecondaryAmmoType ];

/*		if ( pto->m_flPumpTime != -9999 )
		{
			pto->m_flPumpTime -= cmd->msec / 1000.0;
			if ( pto->m_flPumpTime < -0.001 )
				pto->m_flPumpTime = -0.001;
		}*/

		if ( pto->m_fNextAimBonus < -1.0 )
		{
			pto->m_fNextAimBonus = -1.0;
		}

		if ( pto->m_flNextPrimaryAttack < -1.0 )
		{
			if(pCurrent->m_iId == WEAPON_SHOTGUN){
				//easyForcePrintLine("HOW THE hey THIS HAPPEN WILLIS.");
			}
			pto->m_flNextPrimaryAttack = -1.0;
		}

		if ( pto->m_flNextSecondaryAttack < -0.001 )
		{
			pto->m_flNextSecondaryAttack = -0.001;
		}

		if ( pto->m_flTimeWeaponIdle < -0.001 )
		{
			pto->m_flTimeWeaponIdle = -0.001;
		}

		if ( pto->m_flNextReload < -0.001 )
		{
			pto->m_flNextReload = -0.001;
		}

		if ( pto->fuser1 < -0.001 )
		{
			pto->fuser1 = -0.001;
		}
	}//END OF that for loop. Wow don't hide from me like that.

	// m_flNextAttack is now part of the weapons, but is part of the player instead
	to->client.m_flNextAttack -= cmd->msec / 1000.0;
	if ( to->client.m_flNextAttack < -0.001 )
	{
		to->client.m_flNextAttack = -0.001;
	}

	to->client.fuser2 -= cmd->msec / 1000.0;
	if ( to->client.fuser2 < -0.001 )
	{
		to->client.fuser2 = -0.001;
	}
	
	to->client.fuser3 -= cmd->msec / 1000.0;
	if ( to->client.fuser3 < -0.001 )
	{
		to->client.fuser3 = -0.001;
	}

	// Store off the last position from the predicted state.
	HUD_SetLastOrg();

	// Wipe it so we can't use it after this frame
	g_finalstate = NULL;



	/*
	if(from->client.m_iId != WEAPON_GLOCK && gEngfuncs.GetViewModel()->curstate.iuser1 == -500){
		//if(from->client.m_iId != WEAPON_SHOTGUN){
		gEngfuncs.GetViewModel()->curstate.iuser1 = 0;
		//}
		//gEngfuncs.GetLocalPlayer()->curstate.iuser1 = 0;
	}
	*/

	/*
	gEngfuncs.GetLocalPlayer()->curstate.body = 50000;
	gEngfuncs.GetViewModel()->curstate.body = -500;
	player.pev->body = 50002;
	pWeapon->m_pPlayer->pev->body = -250;
	*/


	//by default, enable the muzzle flash in case it was tampered with.
	//gEngfuncs.GetViewModel()->curstate.renderfx &= ~NOMUZZLEFLASH;
	//hacky hacky.
	gEngfuncs.GetViewModel()->curstate.renderfx &= ~(1 << 1);


	//MODDD TODO - shouldn't the body always be reset to 0 just in case?
	//gEngfuncs.GetViewModel()->curstate.body = 0;

	
	if(from->client.m_iId == WEAPON_TRIPMINE){
		//...the tripmine still needs this to reset its body to 0 if it has been tampered with (like by the glock-silencer
		//setting the client model's body to "1".  Default is 0.
		gEngfuncs.GetViewModel()->curstate.body = 0;

	}else if(from->client.m_iId == WEAPON_GLOCK){
		//the glock's "body" (submodel) is coordinated with "m_fireState".

		//Check, is this reliable? Should the body be set in other places too to be safe?
		gEngfuncs.GetViewModel()->curstate.body = pWeapon->m_fireState;

		if(pWeapon->m_fireState == 0){
			//silencer off? muzzle flash.
		}else{
			//disable muzzle flash for silencer.
			gEngfuncs.GetViewModel()->curstate.renderfx |= NOMUZZLEFLASH;
			//will this work??
			//pWeapon->m_pPlayer->pev->renderfx |= NOMUZZLEFLASH;
			//player.pev->renderfx |= NOMUZZLEFLASH;
		}

		//gEngfuncs.GetViewModel()->curstate.iuser1 = -500;  //tells the event system to pay attention to the model (no muzzle flash if the silencer is on)

		//easyPrintLine("CLIENT FIRESTATE: %d", pWeapon->m_fireState);
		

		//player.pev->iuser1 = 50000;
		//gEngfuncs.GetLocalPlayer()->curstate.iuser1 = 500 + pWeapon->m_fireState;

	}else if(from->client.m_iId == WEAPON_PYTHON){
		gEngfuncs.GetViewModel()->curstate.body = pWeapon->m_fInAttack;

		
		//easyPrintLine("PRINTER START!!!!!!!!!!!!!!!!!!!!!!!!!!");
		//for(int i = 0; i < 32; i++){
		//	entvars_t& thisEnt = ev[i];
		//	easyPrintLine("PRINTER::: %s", thisEnt.classname);
		//}
		//easyPrintLine("PRINTER END!!!!!!!!!!!!!!!!!!!!!!!!!!");
		

	}else if(from->client.m_iId == WEAPON_EGON){
		
		//?
		//gEngfuncs.GetViewModel()->curstate.frame = 17;
	}

	/*
	gEngfuncs.GetViewModel()->baseline.body = pWeapon->m_fireState;
	gEngfuncs.GetViewModel()->curstate.body = pWeapon->m_fireState;
	*/
	//gEngfuncs.GetViewModel()->baseline.body = gEngfuncs.GetViewModel()->prevstate.body;


	if(numberOfEyeSkins == 0){
		//force a skin of just the default.
		gEngfuncs.GetViewModel()->curstate.skin = 0;

	}else{
		//do blink logic here. Once every 0.1 seconds to resemble the NPC logic.
		if(gpGlobals->time >= nextBlinkLogicFrame){
			nextBlinkLogicFrame = gpGlobals->time + 0.1;

			cl_entity_s* viewModelRef = gEngfuncs.GetViewModel();
			entity_state_t& viewModelState = viewModelRef->curstate;

			if( ( viewModelState.skin == 0 ) && RANDOM_LONG(0, blinkChancePerFrame) == 0)
			{
				viewModelState.skin = max(numberOfEyeSkins - 1, 0);
			}
			else if ( viewModelState.skin > 0 )
			{// already blinking
				viewModelState.skin--;
			}

		}//END OF time check (every 0.1 seconds)

	}//END OF skin count check
	

}//END OF HUD_WeaponsPostThink




BOOL recentDuckVal = FALSE;
int recentDuckTime = 0;

/*
=====================
HUD_PostRunCmd

Client calls this during prediction, after it has moved the player and updated any info changed into to->
time is the current client clock based on prediction
cmd is the command that caused the movement, etc
runfuncs is 1 if this is the first time we've predicted this command.  If so, sounds and effects should play, otherwise, they should
be ignored
=====================
*/
void DLLEXPORT HUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed)
{
	//MODDD - new. Must keep track of the time between method calls to know when it is ok to do an eye-check.
	static float previousTime = -1; //to guarantee the first time always happens.

	g_runfuncs = runfuncs;

	//MODDD - moved to here from HUD_WeaponsPostThink.
	HUD_InitClientWeapons();
	gpGlobals->time = time;


	//MODDD - for now, these are unused.  Who knows if they'd ever be useful.
	recentDuckVal = to->client.bInDuck;
	recentDuckTime = to->client.flDuckTime;


	//////////////////////////////////////////////////////////////////////////
	//MODDD - should we keep doing this??
	// ok, but ONLY for singleplayer!  In multiplayer it breaks viewmodel animations.  They often, randomly break.
	// (EV_ calls don't make it from server to client; nothing shows up at all, no view anims / clientside event if the glitch happens)
	// That's not a frustrating problem to run into at all!
	if (!IsMultiplayer()) {
		if (gpGlobals->time - previousTime == 0) {
			//If no time has passed since the last client-reported time, assume we're paused. Don't try any logic.
			//easyForcePrintLine("HUD_PostRunCmd: paused. time:%.2f", gpGlobals->time);
			return;
		}
		previousTime = gpGlobals->time;
	}
	//////////////////////////////////////////////////////////////////////////


	//player = gEngfuncs.GetLocalPlayer();

	//if(player){
	if (player.m_pActiveItem != 0) {
		player.m_pActiveItem->m_flStartThrow = 2;
	}
	if (player.m_pClientActiveItem != 0) {
		player.m_pClientActiveItem->m_flStartThrow = 2;
	}
	//}




#if defined( CLIENT_WEAPONS )

	//MODDD - way to force some edits for testing.   Maybe.
	////////////////////////////////////////////////////////////////////////////////////////
	/*
	if(from->client.m_iId == WEAPON_GLOCK){
		gEngfuncs.GetViewModel()->curstate.body = pWeapon->m_fireState;
	}
	*/
	/*
	if(!cl_lw){
		easyPrintLine("FLAG4234");
	}else{
		if( !(cl_lw->value) ){
			easyPrintLine("FLAG4235");
		}
	}
	*/
	//gEngfuncs.GetViewModel()->curstate.body = 1;
	////////////////////////////////////////////////////////////////////////////////////////




	if (cl_lw && cl_lw->value)
	{
		HUD_WeaponsPostThink(from, to, cmd, time, random_seed);
	}
	else
#endif
	{
		to->client.fov = g_lastFOV;
	}

	if (g_irunninggausspred == 1)
	{
		Vector forward;
		gEngfuncs.pfnAngleVectors(v_angles, forward, NULL, NULL);
		to->client.velocity = to->client.velocity - forward * g_flApplyVel * 5;
		g_irunninggausspred = false;
	}

	// All games can use FOV state
	g_lastFOV = to->client.fov;
}

