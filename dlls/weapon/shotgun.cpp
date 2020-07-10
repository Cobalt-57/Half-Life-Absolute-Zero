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
#include "shotgun.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"


EASY_CVAR_EXTERN(cheat_infiniteclip)
EASY_CVAR_EXTERN(cheat_infiniteammo)
EASY_CVAR_EXTERN(cheat_minimumfiredelay)

EASY_CVAR_EXTERN(playerWeaponSpreadMode)

EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)




LINK_ENTITY_TO_CLASS( weapon_shotgun, CShotgun );


CShotgun::CShotgun(){

	//NEW VAR.  If non-zero, we are cheating.  Better for syncing.
	m_chargeReady = 0;

}


// Save/restore for serverside only!
#ifndef CLIENT_DLL
TYPEDESCRIPTION	CShotgun::m_SaveData[] =
{
	DEFINE_FIELD(CShotgun, m_flNextReload, FIELD_TIME),
	DEFINE_FIELD(CShotgun, m_fInSpecialReload, FIELD_INTEGER),
	DEFINE_FIELD(CShotgun, m_flNextReload, FIELD_TIME),
	// DEFINE_FIELD( CShotgun, m_iShell, FIELD_INTEGER ),
	DEFINE_FIELD(CShotgun, m_flPumpTime, FIELD_TIME),
};
IMPLEMENT_SAVERESTORE(CShotgun, CBasePlayerWeapon);
#endif


void CShotgun::Spawn( )
{
	Precache( );
	m_iId = WEAPON_SHOTGUN;
	SET_MODEL(ENT(pev), "models/w_shotgun.mdl");

	m_iDefaultAmmo = SHOTGUN_DEFAULT_GIVE;

	FallInit();// get ready to fall
}


void CShotgun::Precache( void )
{
	PRECACHE_MODEL("models/v_shotgun.mdl");
	PRECACHE_MODEL("models/w_shotgun.mdl");
	PRECACHE_MODEL("models/p_shotgun.mdl");

	m_iShell = PRECACHE_MODEL ("models/shotgunshell.mdl");// shotgun shell

	PRECACHE_SOUND("items/9mmclip1.wav");              

	PRECACHE_SOUND ("weapons/dbarrel1.wav");//shotgun
	PRECACHE_SOUND ("weapons/sbarrel1.wav");//shotgun

	PRECACHE_SOUND ("weapons/reload1.wav");	// shotgun reload
	PRECACHE_SOUND ("weapons/reload3.wav");	// shotgun reload

//	PRECACHE_SOUND ("weapons/sshell1.wav");	// shotgun reload - played on client
//	PRECACHE_SOUND ("weapons/sshell3.wav");	// shotgun reload - played on client
	
	PRECACHE_SOUND ("weapons/357_cock1.wav"); // gun empty sound
	PRECACHE_SOUND ("weapons/scock1.wav");	// cock gun

	precacheGunPickupSound();

	m_usSingleFire = PRECACHE_EVENT( 1, "events/shotgun1.sc" );
	m_usDoubleFire = PRECACHE_EVENT( 1, "events/shotgun2.sc" );
}


//MODDD
void CShotgun::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_SHOTGUN", FALSE, SUIT_NEXT_IN_30MIN);
}

int CShotgun::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}


int CShotgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "buckshot";
	p->iMaxAmmo1 = BUCKSHOT_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = SHOTGUN_MAX_CLIP;
	p->iSlot = 2;
	p->iPosition = 1;
	p->iFlags = 0;
	p->iId = m_iId = WEAPON_SHOTGUN;
	p->iWeight = SHOTGUN_WEIGHT;

	return 1;
}




//MODDD - new.
void CShotgun::Holster( int skiplocal /* = 0 */ )
{ 

	this->m_fireState &= ~128;  //nope.

	//Ha!  Got you.
	m_fInSpecialReload = 0;

	//CBasePlayerWeapon::Holster(skiplocal);
	
	DefaultHolster(SHOTGUN_HOLSTER, skiplocal, 0, (11.0f/30.0f));
}



#ifndef CLIENT_DLL
EASY_CVAR_EXTERN(soundSentenceSave)
#endif

BOOL CShotgun::Deploy( )
{
	this->m_fireState &= ~128;  //nope.
	
	//MODDD TODO: CHANGE SOUNDS!!!
	//if ( flRndSound <= 0.5 )




	//Just try to play at all only if using soundSentenceSave. This stupid meme isn't worth a "could not precache" notice or "missing file" problem.
#ifndef CLIENT_DLL
	if(EASY_CVAR_GET(soundSentenceSave) == 1){
		if(!globalflag_muteDeploySound){
			EMIT_SOUND_FILTERED(ENT(m_pPlayer->pev), CHAN_WEAPON, "meme/i_got_a_shotgun.wav", 1, ATTN_NORM, 0, 100, TRUE);
		}
	}
#endif
	



	//MODDD - offermore specific deploy anim time after skipping other params.
	return DefaultDeploy( "models/v_shotgun.mdl", "models/p_shotgun.mdl", SHOTGUN_DRAW, "shotgun", 0, 0, 13.0/24.0 );
} 




#define SHOTGUN_BIT1 1
#define SHOTGUN_BIT2 2
#define SHOTGUN_BIT3 4
#define SHOTGUN_BIT4 8
#define SHOTGUN_BIT5 16
#define SHOTGUN_BIT6 32
#define SHOTGUN_BIT7 64



void CShotgun::ItemPreFrame(){
	CBasePlayerWeapon::ItemPreFrame();

	if(EASY_CVAR_GET(cheat_infiniteclip) == 1){
		//cheating.
		m_chargeReady |= SHOTGUN_BIT1;
	}else{
		if(m_chargeReady & SHOTGUN_BIT1){
			m_chargeReady &= ~SHOTGUN_BIT1;
		}
	}

	if(EASY_CVAR_GET(cheat_infiniteammo) == 1){
		//cheating.
		m_chargeReady |= SHOTGUN_BIT2;
	}else{
		if(m_chargeReady & SHOTGUN_BIT2){
			m_chargeReady &= ~SHOTGUN_BIT2;
		}
	}

	if(EASY_CVAR_GET(cheat_minimumfiredelay) == 1){
		//cheating.
		m_chargeReady |= SHOTGUN_BIT3;
	}else{
		if(m_chargeReady & SHOTGUN_BIT3){
			m_chargeReady &= ~SHOTGUN_BIT3;
		}
	}

}


void CShotgun::ItemPostFrame( void )
{
	
	if(this->m_flNextPrimaryAttack <= UTIL_WeaponTimeBase() ){
		//HACK - turn off reverse now.
		//this->m_fireState &= ~128;  //nope.
	}

	
	if ( m_flPumpTime && m_flPumpTime < gpGlobals->time )
	{
		// play pumping sound
		EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0,0x1f));
		m_flPumpTime = 0;

		if(m_iClip == 1){
			pev->iuser1 |= SHOTGUN_BIT5; //signify we've put one bullet in.
		}else if(m_iClip > 1){
			pev->iuser1 |= SHOTGUN_BIT6; //two bullets availabe at the time of pump, two bullets ready for double-fire.
		}
	}


	CBasePlayerWeapon::ItemPostFrame();
	
	if(pev->iuser1 & SHOTGUN_BIT7){
		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("I SAW THE WHOLE THING");
		pev->iuser1 &= ~SHOTGUN_BIT7;
		//SendWeaponAnimServerOnlyReverse( SHOTGUN_START_RELOAD );
		//CBasePlayerWeapon::SendWeaponAnimServerOnlyReverse(SHOTGUN_START_RELOAD, 1, 0);

		SendWeaponAnimServerOnlyReverse( (int)SHOTGUN_START_RELOAD, 0 );

		//m_fireState |= 128;
	}
}



void CShotgun::PrimaryAttack()
{


	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.15;
		return;
	}

	if (m_iClip <= 0)
	{
		Reload( );
		if (m_iClip == 0)
			PlayEmptySound( );
		return;
	}


	if(reloadBlockFireCheck()){
		easyForcePrintLine("I GOT BLAWKED!!! time:%.2f", gpGlobals->time);
		return;	
	};

	
	pev->iuser1 &= ~(SHOTGUN_BIT5 | SHOTGUN_BIT6);
	



	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	//MODDD - cheat check.
	if( !(m_chargeReady & SHOTGUN_BIT1) ){
		m_iClip--;
	}

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif


	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	Vector vecDir;


	
	//if (  EASY_CVAR_GET(playerWeaponSpreadMode)!=1 && (EASY_CVAR_GET(playerWeaponSpreadMode)==2 || IsMultiplayer() )  )
	if(EASY_CVAR_GET(playerWeaponSpreadMode)!=2 && (EASY_CVAR_GET(playerWeaponSpreadMode)==1 || !IsMultiplayer()) )
	{
		// regular old, untouched spread. 
		vecDir = m_pPlayer->FireBulletsPlayer( 6, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}
	else
	{
		vecDir = m_pPlayer->FireBulletsPlayer( 4, vecSrc, vecAiming, VECTOR_CONE_DM_SHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}

	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SHOTGUN: PRIMARY FIRE AT %.2f", gpGlobals->time);
	

	//is this necessary here?
	m_fireState &= ~128;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usSingleFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );
	//this is single fire.
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (20.0 / 20.0) + randomIdleAnimationDelay();
	
	
	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);


	//MODDD - WRONG.   if we were out of ammo, execution wouldn't have even reached this point to begin with!
	// this just causes the last visible reload pump of the shotgun (before auto-reloading) to make no noise.
	//if (m_iClip != 0){
		m_flPumpTime = gpGlobals->time + 0.5;
	//}


	//MODDD
	if(!(m_chargeReady & SHOTGUN_BIT3)){

		//MODDD - now enough for the full anim, or almost. Real anim time is 1.00 seconds
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.90;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.90;

		//m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.75;
		//m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.75;

	}else{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
	}


	
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("OK, next attack relative time:%.2f", m_flNextPrimaryAttack);

	/*
	if (m_iClip != 0)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 5.0;
	else
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.75;
	*/

	m_fInSpecialReload = 0;
}


void CShotgun::SecondaryAttack( void )
{
	


	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound( );
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.15;
		return;
	}

	if (m_iClip <= 1)
	{
		Reload( );
		PlayEmptySound( );
		return;
	}


	if(reloadBlockFireCheck()){
		return;
	}

	
	
	pev->iuser1 &= ~(SHOTGUN_BIT5 | SHOTGUN_BIT6);


	m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = NORMAL_GUN_FLASH;

	if( !(m_chargeReady & SHOTGUN_BIT1) ){
		m_iClip -= 2;
	}


	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	m_pPlayer->pev->effects = (int)(m_pPlayer->pev->effects) | EF_MUZZLEFLASH;

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

	Vector vecSrc	 = m_pPlayer->GetGunPosition( );
	Vector vecAiming = m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );

	Vector vecDir;
	

	if(EASY_CVAR_GET(playerWeaponSpreadMode)!=2 && (EASY_CVAR_GET(playerWeaponSpreadMode)==1 || !IsMultiplayer()) )
	{
		//easyForcePrintLine("FLAG S-SINGLEPLAYER");
		// untouched default single player
		vecDir = m_pPlayer->FireBulletsPlayer( 12, vecSrc, vecAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}
	else
	{
		//easyForcePrintLine("FLAG S-MULTIPLAYER");
		// tuned for deathmatch
		vecDir = m_pPlayer->FireBulletsPlayer( 8, vecSrc, vecAiming, VECTOR_CONE_DM_DOUBLESHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed );
	}

	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("SHOTGUN: SECONDARY FIRE AT %.2f", gpGlobals->time);


	
	//is this necessary here?
	m_fireState &= ~128;

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usDoubleFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, 0, 0, 0, 0 );
	//MODDD - secondary fire idle delay.
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (21.0 / 13.0) + randomIdleAnimationDelay();

	if (!m_iClip && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
		// HEV suit - indicate out of ammo condition
		m_pPlayer->SetSuitUpdate("!HEV_AMO0", FALSE, 0);

	//MODDD - WRONG.   if we were out of ammo, execution wouldn't have even reached this point to begin with!
	// this just causes the last visible reload pump of the shotgun (before auto-reloading) to make no noise.
	//if (m_iClip != 0){
		m_flPumpTime = gpGlobals->time + 0.95;

		if(m_iClip == 1){
			pev->iuser1 |= SHOTGUN_BIT5; //signify we've put one bullet in.
		}else{
			pev->iuser1 |= SHOTGUN_BIT6; //two bullets availabe at the time of pump, two bullets ready for double-fire.
		}
	//}


	//MODDD
	if( !(m_chargeReady & SHOTGUN_BIT3) ){
		
		//MODDD - enough for full anim. Or almost. Real time is ~1.615 seconds.
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.5;  //nah default is good here.
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.5;

		//m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.5;
		//m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.5;
	}else{
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
	}
	
	//MODDD
	if( m_chargeReady & SHOTGUN_BIT2){
		if ( pszAmmo1() && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 8 )
		{
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 8;
		}
	}

	if( m_chargeReady & SHOTGUN_BIT1){
		if ( m_iClip <= 8 )
		{
			m_iClip = 8;
		}
	}
	






	/*
	if (m_iClip != 0)
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 6.0;
	else
		m_flTimeWeaponIdle = 1.5;
	*/

	m_fInSpecialReload = 0;

}


void CShotgun::Reload( void )
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0 || m_iClip == SHOTGUN_MAX_CLIP)
		return;

	// don't reload until recoil is done
	if (m_flNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;

	// check to see if we're ready to reload
	if (m_fInSpecialReload == 0)
	{
		//MODDD - important!  was just  "SendWeaponAnimServerOnly".
		SendWeaponAnimServerOnly( SHOTGUN_START_RELOAD );
		//
		m_fInSpecialReload = 1;
		//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.6;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.75; //this is the animation length.
		

		//m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.6; //why 1?
		//m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.6;
		
		return;
	}
	else if (m_fInSpecialReload == 1)
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;

		m_fInSpecialReload = 2;


	}else if (m_fInSpecialReload == 2)
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;
		// was waiting for gun to move to side
		m_fInSpecialReload = 3;

		if (RANDOM_LONG(0,1))
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/reload1.wav", 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0,0x1f));
		else
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/reload3.wav", 1, ATTN_NORM, 0, 85 + RANDOM_LONG(0,0x1f));
		
		//MODDD - important!  was just  "SendWeaponAnimServerOnly".

		
		SendWeaponAnimServerOnly( SHOTGUN_RELOAD );


		if(pev->iuser1 & SHOTGUN_BIT4){

		}
		
		m_flNextReload = UTIL_WeaponTimeBase() + 0.5;

		
		//MODDD - Actually, beware.  Altering the "m_flTimeWeaponIdle" to be different will affect "m_flNextReload" as well, since
		//        "m_flNextReload" is only checked if "m_flTimeWeaponIdle" is not applied (not waiting for the delay to pass).
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (10.0 / 16.0);
		
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
		

	}
	else   //m_fInSpecialReload == 3
	{
		if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;
		// Add them to the clip
		m_iClip += 1;
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= 1;
		m_fInSpecialReload = 2;
	}
}



BOOL CShotgun::reloadBlockFireCheck(){

	
	if(TRUE){
		if (m_fInSpecialReload > 0){
			pev->iuser1 |= SHOTGUN_BIT4;  //queue a pump next time.

			if (m_flTimeWeaponIdle <  UTIL_WeaponTimeBase() ){
				reloadSemi();
			}
			//this->m_fireState

			return TRUE;
		}else if(m_fInSpecialReload == 1){
			//initial delay? can't interrupt. being > 0 already catches this though. oh well.
			return TRUE;
		}
	}else{
		
		if (m_fInSpecialReload == 1){ //&& m_flTimeWeaponIdle > UTIL_WeaponTimeBase() ){
			//initial reload delay? block it.
			return TRUE;
		}
	}

	return FALSE; //not stopped earlier? Not blocked.

}


void CShotgun::reloadFinishPump(){
	
	//no need to do this again. Flag is only set if ending early.
	pev->iuser1 &= (~SHOTGUN_BIT4);

	// reload debounce has timed out
	//MODDD - ALSO, BYPASS NOW

	BOOL usePumpAnim = FALSE;

	if( (pev->iuser1 & (SHOTGUN_BIT5 | SHOTGUN_BIT6 )) == (SHOTGUN_BIT5 | SHOTGUN_BIT6) ){
		//Both pump bits? No possible need for a pump.

	}else if( pev->iuser1 & (SHOTGUN_BIT5 ) ){
		//only for 1-bullet? See if we have more than 1 bullet to make a 2nd pump worthwhile (enable doublefire on a whim).
		if(m_iClip > 1){
			usePumpAnim = TRUE;
		}
	}else if( pev->iuser1 & (SHOTGUN_BIT6) ){
		//only for 2-bullets? inclusive of the first, deny.

	}else{ //neither.
		//If we have neither pump bit, we certainl need to pump.
		usePumpAnim = TRUE;
	}
				

	if(usePumpAnim == TRUE){

		SendWeaponAnimServerOnly( SHOTGUN_PUMP );

		//MODDD - no delay needed for this? neat.
		// play cocking sound
		EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0,0x1f));

	
		if(m_iClip == 1){
			pev->iuser1 |= SHOTGUN_BIT5; //signify we've put one bullet in.
		}else if(m_iClip > 1){
			pev->iuser1 |= SHOTGUN_BIT6; //two bullets availabe at the time of pump, two bullets ready for double-fire.
		}



		m_fInSpecialReload = 0;
				
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (11.0 / 13.0) + randomIdleAnimationDelay();


		if(TRUE){
			//m_flPumpTime = gpGlobals->time + 0.5;
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.75;
		}

	}else{
		//this->m_fireState |= 128;  //this means, play backwards.  Method call does this now.
		//SendWeaponAnimServerOnlyReverse( SHOTGUN_START_RELOAD );

		

		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(viewModelPrintouts)==1)easyForcePrintLine("ILL hahaha");
		pev->iuser1 |= SHOTGUN_BIT7;


		m_fInSpecialReload = 0;
				
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (11.0 / 13.0) + randomIdleAnimationDelay();


		if(TRUE){
			//m_flPumpTime = gpGlobals->time + 0.5;
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.75;
			//MODDD NOTE - shouldn't primary and secondary be affected the same??

		}




	}


}


BOOL CShotgun::reloadSemi(){
	
	if (m_iClip == 0 && m_fInSpecialReload == 0 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
	{
		Reload( );
	}
	else if (m_fInSpecialReload != 0)
	{
		if (m_iClip != 8 && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]  )
		{
			Reload( );

			if( (pev->iuser1 & SHOTGUN_BIT4) ){
				//force the pump!
				reloadFinishPump();
			}

		}
		else
		{

			reloadFinishPump();

		}
	}else{
		return FALSE;
	}

	return TRUE;
}


void CShotgun::WeaponIdle( void )
{
	ResetEmptySound( );

	m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );



	//MODDD - now plays outside the idle ever so slightly so that holding down doesn't hide this sound.
	/*
	if ( m_flPumpTime && m_flPumpTime < gpGlobals->time )
	{
		// play pumping sound
		EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + RANDOM_LONG(0,0x1f));
		m_flPumpTime = 0;
	}
	*/



	if (m_flTimeWeaponIdle <  UTIL_WeaponTimeBase() )
	{
		if(reloadSemi()){
			//if handled, nothing to do.
		}
		else
		{
			//MODDD - times changed for new (restored?) model.
			int iAnim;
			float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
			if (flRand <= 0.8)
			{
				iAnim = SHOTGUN_IDLE_DEEP;
				//was 60 / 20...
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (58.0/12.0) + randomIdleAnimationDelay();// * RANDOM_LONG(2, 5);
			}
			else if (flRand <= 0.95)
			{
				iAnim = SHOTGUN_IDLE;  //sm_idle
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (20.0/9.0) + randomIdleAnimationDelay();
			}
			else
			{
				iAnim = SHOTGUN_IDLE4;  //idle2
				//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (60.0/20.0);
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (58.0/15.0) + randomIdleAnimationDelay();
			}

			


			SendWeaponAnimServerOnly( iAnim );
		}
	}
}



class CShotgunAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_shotbox.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_shotbox.mdl");
		precacheAmmoPickupSound();
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if (pOther->GiveAmmo( AMMO_BUCKSHOTBOX_GIVE, "buckshot", BUCKSHOT_MAX_CARRY ) != -1)
		{
			playAmmoPickupSound();

			//MODDD
			if(pOther->IsPlayer()){
				CBasePlayer* pPlayer = (CBasePlayer*)pOther;
				pPlayer->SetSuitUpdate("!HEV_BUCKSHOT", FALSE, SUIT_NEXT_IN_20MIN);
			}

			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_buckshot, CShotgunAmmo );


