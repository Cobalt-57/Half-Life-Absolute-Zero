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

#pragma once

#include "extdll.h"
#include "gauss.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"

#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "shake.h"
#include "gamerules.h"

#include "util_debugdraw.h"


// Bunch of stuff moved to gauss.h for including things needed by here and ev_hldm.


//NOTES ON VARS:
//m_fireState   -  the mode that the animation uses for detecting "phase" (stage towards showing the full spin anim, as opposed to the pre-delay and showing the start spin anim).
//m_flStartThrow - delay required for the charge animation to start playing
//m_flReleaseThrow  -  alternate time recording since charging.  "Charging" for a very short amount of time
//     registers as a "tap" (primary fire) instead, since (quake style) one mouse button is used for both primary
//     (tap) and secondary (hold down) attacks.


EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)

EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)

EASY_CVAR_EXTERN(gauss_mode)  // ?
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_reflectdealsdamage)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)

EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP)




LINK_ENTITY_TO_CLASS( weapon_gauss, CGauss );



CGauss::CGauss(void){

	this->ohMySon = -666.6667;
	ohDearSonny = -1;

}//END OF CGauss constructor




// Save/restore for serverside only!
#ifndef CLIENT_DLL
TYPEDESCRIPTION	CGauss::m_SaveData[] =
{
	DEFINE_FIELD(CGauss, m_fInAttack, FIELD_INTEGER),
	//	DEFINE_FIELD( CGauss, m_flStartCharge, FIELD_TIME ),
	//	DEFINE_FIELD( CGauss, m_flPlayAftershock, FIELD_TIME ),
	//	DEFINE_FIELD( CGauss, m_flNextAmmoBurn, FIELD_TIME ),
	DEFINE_FIELD(CGauss, m_fPrimaryFire, FIELD_BOOLEAN),
};
IMPLEMENT_SAVERESTORE(CGauss, CBasePlayerWeapon);
#endif




float CGauss::GetFullChargeTime( void )
{

	if(IsMultiplayer())
	{
		//MODDD - TIME TIMES THEY ARE A CHANGIN'
		//return 1.5;
		return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_MP) * EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP) ;
	}

	//return 4;
	return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeMaxAmmo_SP) * EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP) ;
}

#ifdef CLIENT_DLL
extern int g_irunninggausspred;
#endif

//MODDD
void CGauss::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_GAUSS", FALSE, SUIT_NEXT_IN_30MIN , 4.3f);
}

void CGauss::Spawn( )
{
	Precache( );
	m_iId = WEAPON_GAUSS;
	SET_MODEL(ENT(pev), "models/w_gauss.mdl");

	m_iDefaultAmmo = GAUSS_DEFAULT_GIVE;

	FallInit();// get ready to fall down.
}


void CGauss::Precache( void )
{
	PRECACHE_MODEL("models/w_gauss.mdl");
	PRECACHE_MODEL("models/v_gauss.mdl");
	PRECACHE_MODEL("models/p_gauss.mdl");

	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("weapons/gauss2.wav");
	PRECACHE_SOUND("weapons/electro4.wav");
	PRECACHE_SOUND("weapons/electro5.wav");
	PRECACHE_SOUND("weapons/electro6.wav");
	PRECACHE_SOUND("ambience/pulsemachine.wav");

	precacheGunPickupSound();
	
	m_iGlow = PRECACHE_MODEL( "sprites/hotglow.spr" );
	m_iBalls = PRECACHE_MODEL( "sprites/hotglow.spr" );
	m_iBeam = PRECACHE_MODEL( "sprites/smoke.spr" );

	m_usGaussFire = PRECACHE_EVENT( 1, "events/gauss.sc" );
	m_usGaussSpin = PRECACHE_EVENT( 1, "events/gaussspin.sc" );
}

int CGauss::AddToPlayer( CBasePlayer *pPlayer )
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

int CGauss::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "uranium";
	p->iMaxAmmo1 = URANIUM_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	//MODDD - now 0. RPG moved out. It used to take islot 3, iPosition 0.
	//p->iPosition = 1;
	p->iPosition = 0;

	p->iId = m_iId = WEAPON_GAUSS;
	p->iFlags = 0;
	p->iWeight = GAUSS_WEIGHT;

	return 1;
}

BOOL CGauss::Deploy( )
{
	m_pPlayer->m_flPlayAftershock = 0.0;
	return DefaultDeploy( "models/v_gauss.mdl", "models/p_gauss.mdl", GAUSS_DRAW, "gauss", 0, 0, (36.0f + 1.0f)/(64.0f), -1 );
}

void CGauss::Holster( int skiplocal /* = 0 */ )
{

	//does this just stop sounds?
	PLAYBACK_EVENT_FULL( FEV_RELIABLE | FEV_GLOBAL, m_pPlayer->edict(), m_usGaussFire, 0.01, (float *)&m_pPlayer->pev->origin, (float *)&m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1 );
	
	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	//SendWeaponAnim( GAUSS_HOLSTER );

	m_fInAttack = 0;
	//MODDD - new.
	m_fireState = 0;


	
	DefaultHolster(GAUSS_HOLSTER, skiplocal, 0, (31.0f + 1.0f)/(60.0f) );

}



#ifdef CLIENT_DLL

#else
	extern BOOL CanAttack( float attack_time, float curtime, BOOL isPredicted );	
#endif

void CGauss::PrimaryAttack()
{



#ifdef CLIENT_DLL
	BOOL secondaryHeld =  ((m_pPlayer->pev->button & IN_ATTACK2 ) && (m_flNextSecondaryAttack <= 0.0));
	BOOL primaryHeld = 	((m_pPlayer->pev->button & IN_ATTACK) && (m_flNextPrimaryAttack <= 0.0));

#else
	BOOL secondaryHeld = ((m_pPlayer->pev->button & IN_ATTACK2) && CanAttack( m_flNextSecondaryAttack, gpGlobals->time, UseDecrement() ));
	BOOL primaryHeld = ((m_pPlayer->pev->button & IN_ATTACK) && CanAttack( m_flNextPrimaryAttack, gpGlobals->time, UseDecrement() ));
	
#endif
	

	/*
#ifdef CLIENT_DLL
	easyForcePrintLine("I AM CLIENT con: %d %d || %.2f %.2f", primaryHeld, secondaryHeld, m_flNextSecondaryAttack, gpGlobals->time );
#else
	easyForcePrintLine("I AM SERVER %d %d || %.2f %.2f", primaryHeld, secondaryHeld, m_flNextSecondaryAttack, gpGlobals->time);
#endif
	*/

	if(secondaryHeld){
		m_flTimeWeaponIdle = -1;
		WeaponIdle();
		return;   //block!
	}


	

	//MODDD
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly) == 1){
		chargeWork();
		//chargeGauss();
		//postChargeAnimCheck();

		//just charge on hold.
		
		//m_pPlayer->m_flNextAttack = gpGlobals->time + 0.2;
		return;
	}//END OF gauss quake style check
	
	

	//similarly, this script has been moved to its own method to be more easily called.
	attemptFirePrimary();
	
}



void CGauss::attemptFirePrimary(){
	
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		PlayEmptySound( );
		m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.15;
		return;
	}

	if (PlayerPrimaryAmmoCount() < 2 )
	{
		PlayEmptySound( );
		//easyForcePrintLine("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
		//m_pPlayer->m_flNextAttack = gpGlobals->time + 0.5;
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
		return;
	}

	stopBlockLooping();

	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	m_fPrimaryFire = TRUE;

	//MODDD - only reduce ammo if cheats are off.
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip) == 0 && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo) == 0){
		ChangePlayerPrimaryAmmoCount(-2);
	}

	StartFire();
	m_fInAttack = 0;
	//MODDD - new.
	m_fireState = 0;

	//MODDDD - do not determine the idle here.
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;


	//MODDD
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0){
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_betweenattackdelay) ;
	}else{
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom);
	}
}



void CGauss::ItemPreFrame(){
	
	CBasePlayerWeapon::ItemPreFrame();


	//MODDD - OBSOLETE: this no longer needs to be done.
	//MODDD - done better to sync b/w the server and client for this cheat.
	/*
	if((EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 1) ){
		//m_fInAttack |= 4;
		m_fireState = 1;
	}else{
		//can't include this bit anymores.
		
		
		//if(m_fInAttack == 6){
		//	m_fInAttack = 0;
		//}
		
		//Now using a different var instead.
		m_fireState = 0;

	}
	*/
}

void CGauss::ItemPostFrame(){

	/*
	easyForcePrintLine("WHATS YO GAAAAME ia:%d fs:%d starthro:%.2f sc:%.2f sca:%.2f scp:%.2f t:%.2f",
		m_fInAttack,
		m_fireState,
		m_flStartThrow,
		m_pPlayer->m_flStartCharge,
		m_pPlayer->m_flStartChargeAnim,
		m_pPlayer->m_flStartChargePreSuperDuper,
		gpGlobals->time);
		*/
	CBasePlayerWeapon::ItemPostFrame();

}

void CGauss::SecondaryAttack()
{
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly) == 1){
		
		//chargeGauss();
		//postChargeAnimCheck();

		//just charge on hold.
		return;
	}//END OF gauss quake style check
	
	chargeWork();


}//END OF getNextAmmoBurnDelay()





void CGauss::chargeWork(){
	
	
	stopBlockLooping();
	while(TRUE){
		
		//m_fInAttack = 1;
		//break;


	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		if ( m_fInAttack != 0 )
		{
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", 1.0, ATTN_NORM, 0, 80 + RANDOM_LONG(0,0x3f));
			SendWeaponAnim( GAUSS_IDLE );
			m_fInAttack = 0;
			m_fireState = 0;
		}
		else
		{
			PlayEmptySound( );
		}

		m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
		break;
	}

	//easyForcePrintLine("AAAAAAAAAAAAAAA %d", m_fInAttack);

	if ( m_fInAttack == 0 )
	{
		ohDearSonny = 0;

		//////easyForcePrintLine("WHAT? WHAT? %d", PlayerPrimaryAmmoCount());
		if (PlayerPrimaryAmmoCount() <= 0 )
		{
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

			break;
		}



		m_fPrimaryFire = FALSE;



		//ChangePlayerPrimaryAmmoCount(-1);// take one ammo just to start the spin
		//m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();
		//NOT YET, FREEMAN!

		//m_pPlayer->m_flNextAmmoBurn = gpGlobals->time;

		m_fInAttack = 1;


		//MODDD
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
		
		
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + EASY_CVAR_GET(gauss_secondarychargemindelay);   //TODO!!!! todo
		
		
		m_flTimeWeaponIdle = 0;

		

		m_flStartThrow = gpGlobals->time + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeworkdelay);
		///??   yes... bizarrely, startThrow uses fixed time, not relative to 0.

		

		m_pPlayer->m_flStartChargePreSuperDuper = gpGlobals->time;  //just record me.

	}
	else if (m_fInAttack == 1)
	{
		if(ohDearSonny != m_fInAttack){
			ohDearSonny = m_fInAttack;

		}





		//stopBlockLooping();
		
		//this->attemptFirePrimary();
		//return;

		//easyForcePrintLine("I REACHED HERE???!!!! comp: starthro:%.2f t:%.2f", m_flStartThrow, gpGlobals->time);

		if( m_flStartThrow < gpGlobals->time ){
				
			//OH SHIT SON   I MOVED YOU.
			/*
			m_pPlayer->m_flStartCharge = gpGlobals->time;
			m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();
			*/
				
			//move on and collect charge?
			m_fInAttack = 2;
				
			//MOVED DOWN
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay);   //TODO!!!! todo
		}



	}
	else if (m_fInAttack == 2)
	{
		
		if(ohDearSonny != m_fInAttack){
			ohDearSonny = m_fInAttack;
			
			//easyForcePrintLine("OH SHIT I DID THE SHIT DO YOU SEE THIS SHIT DAMN");
			

			if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly) != 1){
				//only allow when primary fire is disallowed.
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay);   //TODO!!!! todo
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();
			}else{
				//is that okay?
				this->pev->fuser1 = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargemindelay);
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + getAmmoChargeInterval();   // + 0.1
			}

			


			ChangePlayerPrimaryAmmoCount(-1);// take one ammo just to start the spin
				


			easyForcePrintLine("OH dear MAN WORK CHARGIN STARTIN");
			m_pPlayer->m_flStartCharge = gpGlobals->time;
			m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();
		}


		BOOL moveToNextPhase = FALSE;

		if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly) != 1){
			moveToNextPhase = (m_flTimeWeaponIdle <= UTIL_WeaponTimeBase());
		}else{
			moveToNextPhase = (pev->fuser1 <= UTIL_WeaponTimeBase());
		}

		//easyForcePrintLine("????????????????? %.2f %.2f", m_flTimeWeaponIdle, UTIL_WeaponTimeBase() );
		
		if(moveToNextPhase){
			m_fInAttack = 3;
		}
		



	}else if (m_fInAttack == 3)
	{
		if(ohDearSonny != m_fInAttack){
			ohDearSonny = m_fInAttack;

		}


		/*
#ifdef CLIENT_DLL
	easyForcePrintLine("IM aaa %.2f %.2f %.2f", m_pPlayer->m_flNextAmmoBurn, gpGlobals->time, UTIL_WeaponTimeBase());
#else
	easyForcePrintLine("IM bbb %.2f %.2f %.2f", m_pPlayer->m_flNextAmmoBurn, gpGlobals->time, UTIL_WeaponTimeBase());
#endif
	*/
		
		// during the charging process, eat one bit of ammo every once in a while
		//if ( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_pPlayer->m_flNextAmmoBurn != 1000 )
		if ( UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_pPlayer->m_flNextAmmoBurn != 1000 )
		{
			ChangePlayerPrimaryAmmoCount(-1);
			m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + getAmmoChargeInterval();   // + 0.1
			
			easyPrintLine("gauss: dropped ammo by 1, left: %d", PlayerPrimaryAmmoCount());
		}

		if (PlayerPrimaryAmmoCount() <= 0 )
		{
			// out of ammo! force the gun to fire
			StartFireDecision();  //is that okay?
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1;
			break;
		}
		
		if ( UTIL_WeaponTimeBase() >= m_pPlayer->m_flAmmoStartCharge )
		{
			// don't eat any more ammo after gun is fully charged.
			m_pPlayer->m_flNextAmmoBurn = 1000;
		}

		//
		/*
		int pitch = ( gpGlobals->time - m_pPlayer->m_flStartCharge ) * ( 150 / GetFullChargeTime() ) + 100;
		if ( pitch > 250 ) 
			 pitch = 250;
			 */
		//
		
		// ALERT( at_console, "%d %d %d\n", m_fInAttack, m_iSoundState, pitch );

		if (m_iSoundState == 0) {
			ALERT(at_console, "sound state %d\n", m_iSoundState);
		}

		//
		//PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usGaussSpin, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, pitch, 0, ( m_iSoundState == SND_CHANGE_PITCH ) ? 1 : 0, 0 );
		//


		//
		/*
		m_iSoundState = SND_CHANGE_PITCH; // hack for going through level transitions

		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;
		*/
		//

		
		// m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
		if ( m_pPlayer->m_flStartCharge < gpGlobals->time - 10 )
		{
			// Player charged up too long. Zap him.
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", 1.0, ATTN_NORM, 0, 80 + RANDOM_LONG(0,0x3f));
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM,   "weapons/electro6.wav", 1.0, ATTN_NORM, 0, 75 + RANDOM_LONG(0,0x3f));
			
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;
				
#ifndef CLIENT_DLL
			m_pPlayer->TakeDamage( VARS(eoNullEntity), VARS(eoNullEntity), 50, DMG_SHOCK, DMG_GAUSS );
			UTIL_ScreenFade( m_pPlayer, Vector(255,128,0), 2, 0.5, 128, FFADE_IN );
#endif
			SendWeaponAnim( GAUSS_IDLE );
			
			// Player may have been killed and this weapon dropped, don't execute any more code after this!
			break;
		}

	}

	
	break;
	}//END OF loop


	postChargeAnimCheck();
}//END OF chargeWork()




void CGauss::postChargeAnimCheck(){

	if(m_fInAttack <= 0){
		//can't be.  Anim not allowed; the attack was most likely canceled in this case.
		return;
	}
	
	//easyForcePrintLine("DERP? %df", m_fireState);

	if(m_fireState == 0){

		ohMySon = 0;


		//send delay!

		m_flReleaseThrow = gpGlobals->time + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeanimdelay);
		m_fireState = 1;

	}else if(m_fireState == 1){

		if(ohMySon != m_fireState){
			ohMySon = m_fireState;


		}
		
			//stopBlockLooping();

		//return;
		if(m_flReleaseThrow < gpGlobals->time){

			m_fireState = 2;

		
		}

	}else if(m_fireState == 2){
		
		if(ohMySon != m_fireState){
			ohMySon = m_fireState;

			
			//NO SON NO.
			//m_pPlayer->m_flStartCharge = gpGlobals->time;
			//m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();


			
			//////easyForcePrintLine("DO I HAPPEN??? %.2f ", gpGlobals->time);

			//m_pPlayer->m_flStartCharge = gpGlobals->time;
			//starting when? starting now.

			// spin up
			m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;
		
			SendWeaponAnimBypass( GAUSS_SPINUP );
			PLAYBACK_EVENT_FULL(  FEV_RELIABLE, m_pPlayer->edict(), m_usGaussSpin, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 110, 0, 0, 0 );

			m_iSoundState = SND_CHANGE_PITCH;

			//time is 30 seconds.
			//target = time + 0.5  
			//target = 30.5

			//time is 30.4 seconds.  pass? no.  time is under
			

			
			//only allow when primary fire is disallowed.
			//MODDD NO, moved to the logic (chargeWork) instead.
			//ChangePlayerPrimaryAmmoCount(-1);// take one ammo just to start the spin
			//m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

			
			//NOTICE - 
			//ChangePlayerPrimaryAmmoCount(-1);// take one ammo just to start the spin
			//m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();


			this->m_flReleaseThrow = gpGlobals->time + 0.5;


			//???!!!

		}//end of ODAMN



		//????????
			stopBlockLooping();
			//easyForcePrintLine("???? " + 


	
		if(ohMySon != m_fireState){
			ohMySon = m_fireState;
		}


		if (this->m_flReleaseThrow < gpGlobals->time)
		{
			m_pPlayer->m_flStartChargeAnim = gpGlobals->time;
			SendWeaponAnimBypass( GAUSS_SPIN );
			m_fireState = 3;
		}

	}else{
		
			stopBlockLooping();

		int pitch = ( gpGlobals->time - m_pPlayer->m_flStartChargeAnim ) * ( 150 / GetFullChargeTime() ) + 100;
		if ( pitch > 250 ) 
			 pitch = 250;
		

		/*
#ifdef CLIENT_DLL
		easyForcePrintLine("PITCHC?? %.2f %.2f  %d", m_pPlayer->m_flStartCharge, gpGlobals->time, pitch );
#else
		easyForcePrintLine("PITCHS %.2f %.2f  %d", m_pPlayer->m_flStartCharge, gpGlobals->time, pitch );
#endif
		*/

		// ALERT( at_console, "%d %d %d\n", m_fInAttack, m_iSoundState, pitch );

		if ( m_iSoundState == 0 )
			ALERT( at_console, "sound state %d\n", m_iSoundState );

		PLAYBACK_EVENT_FULL( FEV_RELIABLE, m_pPlayer->edict(), m_usGaussSpin, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, pitch, 0, ( m_iSoundState == SND_CHANGE_PITCH ) ? 1 : 0, 0 );

		m_iSoundState = SND_CHANGE_PITCH; // hack for going through level transitions

		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;
		
	}


}//END OF postChargeAnimCheck()






void CGauss::StartFireDecision( void ){

	//see which to use... primary or secondary?

	

	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryonly) == 1){
		float timePassed = gpGlobals->time - m_pPlayer->m_flStartChargePreSuperDuper;
		//easyForcePrintLine("TimePassed!! : %.2f : %.2f", timePassed, EASY_CVAR_GET(gauss_secondarychargetimereq)  );

		easyForcePrintLine("AW no MAN %.2f", timePassed);
		if(m_fInAttack > 1 && timePassed > EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarychargetimereq) ){
			//proceed with defaults: secondary attack.
			m_fPrimaryFire = FALSE;
		}else{
			//primary attack!  As seen below, forces the "20" damage.
			m_fPrimaryFire = TRUE;

		}

	}//END OF gaussquake check
	else{
		//not primary fire.  In retail, a call thru "Decision" here is always secondary.  Primary calls straight without this decision.
		m_fPrimaryFire = FALSE;
	}


	if(m_fPrimaryFire){
		attemptFirePrimary();
	}else{
		StartFire();
	}




}//END OF StartFireDecision()


//=========================================================
// StartFire- since all of this code has to run and then 
// call Fire(), it was easier at this point to rip it out 
// of weaponidle() and make its own function then to try to
// merge this into Fire(), which has some identical variable names 
//=========================================================
void CGauss::StartFire( void )
{
	float flDamage;
	
	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc = m_pPlayer->GetGunPosition( ); // + gpGlobals->v_up * -8 + gpGlobals->v_right * 8;
	


	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0){
		if ( gpGlobals->time - m_pPlayer->m_flStartCharge > GetFullChargeTime() )
		{
			flDamage = 200;
		}
		else
		{
			flDamage = 200 * (( gpGlobals->time - m_pPlayer->m_flStartCharge) / GetFullChargeTime() );
		}
	}else{
		//secondary always does full damage with the minimumfiredelay cheat.
		flDamage = 200;
	}



	if ( m_fPrimaryFire )
	{
		// fixed damage on primary attack
#ifdef CLIENT_DLL
		flDamage = 20;
#else 
		flDamage = gSkillData.plrDmgGauss;
#endif
	}

	//MODDD - used to be != 3, although "3" was never used... so using "4", the now never-used  one.   Still weird.?
	if (m_fInAttack != 4)
	{
		//ALERT ( at_console, "Time:%f Damage:%f\n", UTIL_WeaponTimeBase() - m_pPlayer->m_flStartCharge, flDamage );

#ifndef CLIENT_DLL
		float flZVel = m_pPlayer->pev->velocity.z;


		if ( !m_fPrimaryFire )
		{
			//MODDD - a cheat may disable the recoil force.
			if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil) == 0){
				m_pPlayer->pev->velocity = m_pPlayer->pev->velocity - gpGlobals->v_forward * flDamage * 5;
			}
		}
		//MODDD - the 2nd condition is extra, from a new CVar.  Could allow z-force.
		if ( !IsMultiplayer() && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP) == 0)

		{
			// in deathmatch, gauss can pop you up into the air. Not in single play.
			m_pPlayer->pev->velocity.z = flZVel;
		}
#endif
		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
	}

	// time until aftershock 'static discharge' sound
	m_pPlayer->m_flPlayAftershock = gpGlobals->time + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0.3, 0.8 );

	Fire( vecSrc, vecAiming, flDamage );



	
	/*
	if ( PlayerPrimaryAmmoCount() < 2 )
	{
		PlayEmptySound( );
		m_pPlayer->m_flNextAttack = gpGlobals->time + 0.5;
		return;
	}
	
	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + EASY_CVAR_GET(gauss_betweenattackdelay);
	*/
	


}

void CGauss::Fire( Vector vecOrigSrc, Vector vecDir, float flDamage )
{
	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;

	Vector vecSrc = vecOrigSrc;
	Vector vecDest = vecSrc + vecDir * 8192;
	edict_t		*pentIgnore;
	TraceResult tr, beam_tr;
	float flMaxFrac = 1.0;
	int nTotal = 0;
	int fHasPunched = 0;
	int fFirstBeam = 1;
	int nMaxHits = 10;

	pentIgnore = ENT( m_pPlayer->pev );

#ifdef CLIENT_DLL
	if ( m_fPrimaryFire == FALSE )
		 g_irunninggausspred = TRUE;
#endif
	
	// The main firing event is sent unreliably so it won't be delayed.
	PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usGaussFire, 0.0, (float *)&m_pPlayer->pev->origin, (float *)&m_pPlayer->pev->angles, flDamage, 0.0, 0, 0, m_fPrimaryFire ? 1 : 0, 0 );

	// This reliable event is used to stop the spinning sound
	// It's delayed by a fraction of second to make sure it is delayed by 1 frame on the client
	// It's sent reliably anyway, which could lead to other delays

	PLAYBACK_EVENT_FULL( FEV_NOTHOST | FEV_RELIABLE, m_pPlayer->edict(), m_usGaussFire, 0.01, (float *)&m_pPlayer->pev->origin, (float *)&m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1 );



	//MODDD - handle idle time here.
	if(m_fPrimaryFire){
		
		//It seems only "fire2" is ever used, so using that one's length instead.
		//...our modded model's "fire2" now takes 30/30 seconds, so maintaining.
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (31.0/30.0);
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (31.0/30.0) + randomIdleAnimationDelay();;


		/////////////////////////////////////////////////////////////////////////////////////////////
		//MODDD - NOTE. is m_flNextAttack redundant with m_flNextPrimary/Secondary Attack?  Who knows.
		// also on primary, assume this is already handled.  So far at least.   Secondary needs this though.
		//m_pPlayer->m_flNextAttack = m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.7;

	}else{
		//No need to set the idle time for secondary fire, as that is charging (release has to set the idle time)
		// it is fine here.  Fire is called when released, so it works here too.
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (41.0/30.0);
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (31.0/30.0) + randomIdleAnimationDelay();;

		m_pPlayer->m_flNextAttack = m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.7;
	}

	
	/*ALERT( at_console, "%f %f %f\n%f %f %f\n", 
		vecSrc.x, vecSrc.y, vecSrc.z, 
		vecDest.x, vecDest.y, vecDest.z );*/
	

//	ALERT( at_console, "%f %f\n", tr.flFraction, flMaxFrac );






//NOTE: on any changes, sync me with a similar looking area in the client-side event in ev_hldm.cpp for gauss firing (EV_FireGauss)!
#ifndef CLIENT_DLL
	//DebugLine_ClearAll();
	int reflecto = 0;
	int loops = 0;
	int thingsHurt = 0;

	while (flDamage > 10 && nMaxHits > 0)
	{
		//if (fFirstBeam) {
		//	DebugLine_Setup(0, vecSrc, vecDest, 255, 0, 0);
		//}


		nMaxHits--;

		// ALERT( at_console, "." );
		UTIL_TraceLine(vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr);


		if (tr.fAllSolid) {
			break;
		}


		if ( fFirstBeam )
		{
			m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
			fFirstBeam = 0;
	
			nTotal += 26;
		}

		// MODDD - added check for flFraction like FireBulletsPlayer does.  I assume that had a point.
		if (tr.flFraction >= 1.0) {
			break;
		}
		
		CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

		if (pEntity == NULL) {
			break;
		}


		//MODDD - no need for this check, the world knows not to take damage.
		// Methods with TraceAttack methods will reject damage just like ones called by FirePlayerBullets do,
		// which doesn't even do a ThingHit->pev->takedamage check itself anyway
		//if (pEntity->pev->takedamage)
		{
			BOOL useBulletHitSound = TRUE;
			ClearMultiDamage();

			//MODDD - NEW.
			// This is an "AI Sound", or not a real one audible to the player, but one that checks for monsters nearby (distance) and alerts them if they are in hearing range.
			// TODO - egon can get this too, probably.
			attemptSendBulletSound(tr.vecEndPos, m_pPlayer->pev);

			const char* theName = pEntity->getClassname();

			pEntity->TraceAttack( m_pPlayer->pev, flDamage, vecDir, &tr, DMG_BULLET, DMG_GAUSS, TRUE, &useBulletHitSound);
			thingsHurt++;  // probably

			//MODDD - Play a texture-hit sound, it is a bullet after all.
			// And just force a bullet type to MP5 here, point is it's not the crowbar
			// NOTICE - just use FirePlayerBullets at this point for better support, whether to do texture-sounds or decals
			// server or clientside is a little more comlpex than just tacking it on here.
			// Entities can make noise this way though, just not the world under default CVars.
			if (useBulletHitSound) {
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecDest, BULLET_PLAYER_MP5);
				DecalGunshot(&tr, BULLET_PLAYER_MP5);
			}

			/*
			if (FClassnameIs(pEntity->pev, "player")) {
				DebugLine_Setup(vecSrc, vecDest, 255, 0, 255);

				DebugLine_SetupPoint(vecSrc, 255, 255, 255);
			}
			else {

				DebugLine_Setup(vecSrc, vecDest, 0, 255, 0);
			}
			*/

			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);
			//easyPrintLine("GAUSSLASER: HIT an enemy?? %d", nMaxHits);
		}


		//MODDD NOTE
		//Notice that the default (barely ever touched) "ReflectGauss" requires the thing hit (pEntity) to not be capable of taking damage (not an enemy or breakable).
		//If this is the case (incapable of taking dmg), it is reflectable and a check is done to see whether to reflect off the thing hit (enough degrees) or attempt
		//a punch thru (secondary gauss fire can go some distance through walls).
		//OTHERWISE, capable of taking DMG, this assumes that the beam hit a monster for "pEntity", and dealt damage to it.  Don't try to reflect, just try to go right
		//through.  Notice that the pEntity just hit is set to be the "ignore" entity as to not hit the same one again.  Setting the vecSrc (source) to be the hit
		//location (of the enemy struck) a slight ways forwards for the next line-trace at the next run-thru of the loop (where it may hit another enemy or hit a surface
		//as expected) is also a good sign of this idea.
		//...Can block the reflection this early, preventing the "punch through" (go through a wall), or allow that much.

		if ( pEntity->ReflectGauss() )
		{
			float n;

			//MODDD - easy there!  Don't set pentIgnore to NULL so soon, do it if the surface hit warrants a reflection
			// (hit coming at enough of an angle).  Otherwise, the punch-through check that runs instead will
			// find the player at the start of the trace and go 'Oh look, something to punch through.  DAMGE.'
			//pentIgnore = NULL;

			n = -DotProduct(tr.vecPlaneNormal, vecDir);



			BOOL reflectCheckPossible = FALSE;
			if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primaryreflects)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondaryreflects)!=0)   ){
				reflectCheckPossible = TRUE;
			}
			//MODDD - involving "reflectCheckPossible"
			if (reflectCheckPossible && n < 0.5) // 60 degrees
			{
				reflecto++;

				//MODDD - should be safe now
				pentIgnore = NULL;

				// ALERT( at_console, "reflect %f\n", n );
				// reflect
				Vector r;
			
				r = 2.0 * tr.vecPlaneNormal * n + vecDir;
				flMaxFrac = flMaxFrac - tr.flFraction;
				vecDir = r;
				vecSrc = tr.vecEndPos + vecDir * 8;
				vecDest = vecSrc + vecDir * 8192;
				


				// explode a bit
				//MODDD NOTE - why was this "m_pPlayer->RadiusDamage"? No need to be specific to the player in the call,
				//             we already send our own PEV and the player's PEV as arguments.
				// No difference in just "RadiusDamage( ... )" alone.
				//m_pPlayer->RadiusDamage(...)
				// Also, Damange no longer depends on the reflection angle, was 'flDamage * n'.
				// 
				RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, flDamage * 0.45, flDamage * 1.6, CLASS_NONE, DMG_BLAST, DMG_GAUSS );

				nTotal += 34;
				
				// lose energy
				if (n == 0) n = 0.1;
				flDamage = flDamage * (1 - n);

				//easyPrintLine("GAUSSLASER: reflect?? %d", nMaxHits);

			}
			else
			{
				const char* theIgnoreName;
				if (pentIgnore) {
					theIgnoreName = STRING(pentIgnore->v.classname);
				}
				else {
					theIgnoreName = "NULL";
				}

				//easyPrintLine("GAUSSLASER: last hit?? %d", nMaxHits);
				nTotal += 13;

				// limit it to one hole punch
				if (fHasPunched)
					break;
				fHasPunched = 1;

				//MODDD - punching through is possible depending on CVars now, which control whether primary, secondary, neither, or both can punch through.
				// try punching through wall if secondary attack (primary is incapable of breaking through)
				//if ( !m_fPrimaryFire )
				BOOL punchAttempt = FALSE;
				if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypunchthrough)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypunchthrough)!=0)   ){
					punchAttempt = TRUE;
				}
				if(punchAttempt == TRUE){
					UTIL_TraceLine( tr.vecEndPos + vecDir * 8, vecDest, dont_ignore_monsters, pentIgnore, &beam_tr);
					if (!beam_tr.fAllSolid)
					{
						// trace backwards to find exit point
						UTIL_TraceLine( beam_tr.vecEndPos, tr.vecEndPos, dont_ignore_monsters, pentIgnore, &beam_tr);

						n = (beam_tr.vecEndPos - tr.vecEndPos).Length( );

						if (n < flDamage)
						{
							if (n == 0) n = 1;
							flDamage -= n;

							// ALERT( at_console, "punch %f\n", n );
							nTotal += 21;


							//MODDD - redone.
							/*
							// exit blast damage
							//m_pPlayer->RadiusDamage( beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage, CLASS_NONE, DMG_BLAST );
							float damage_radius;
							

							if ( IsMultiplayer() )
							{
								damage_radius = flDamage * 1.75;  // Old code == 2.5
							}
							else
							{
								damage_radius = flDamage * 2.5;
							}

							::RadiusDamage( beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage, damage_radius, CLASS_NONE, DMG_BLAST, DMG_GAUSS );
							*/
							::RadiusDamage(beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage * 0.40, flDamage * 1.7, CLASS_NONE, DMG_BLAST, DMG_GAUSS);



							CSoundEnt::InsertSound ( bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3.0 );

							nTotal += 53;

							vecSrc = beam_tr.vecEndPos + vecDir;
						}
						else {
							//MODDD - NEW.
							// So couldn't punch through something in the way.  Why do the loop again?
							// Nothing about vecSrc nor vecDest changed, so the same trace is being done again.
							// Except this time, with a NULL pentIgnore.  Redoing the trace from the player's weapon
							// to where the player is looking is likely to register a hit on the player from occuring 
							// within the player's bounds without being told to ignore that.
							flDamage = 0;
						}
					}
					else
					{
						 //ALERT( at_console, "blocked %f\n", n );
						flDamage = 0;
					}
				}
				else
				{
					//ALERT( at_console, "blocked solid\n" );
					
					flDamage = 0;
				}

				//MODDD - now it is safe to turn this off.
				pentIgnore = NULL;
			}
		}
		else
		{
			
			// This is at penetrating an enemy and preparing to make the beam go through.  It is possible to block this by CVars.
			BOOL canPierce = FALSE;
			if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_primarypierces)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_secondarypierces)!=0)   ){
				canPierce = TRUE;
			}

			if(!canPierce){
				break;
			}
			
			//easyPrintLine("GAUSSLASER: pierce enemy?? %d", nMaxHits);
			vecSrc = tr.vecEndPos + vecDir;
			pentIgnore = ENT( pEntity->pev );
		}

		loops++;
	}
#endif
	// ALERT( at_console, "%d bytes\n", nTotal );
}




//Depending on multiplayer or singleplayer, return the appropriate CVar for this.
float CGauss::getAmmoChargeInterval(void){
	
	if(IsMultiplayer())
	{
		return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_MP);
	}
	else
	{
		return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gauss_chargeInterval_SP);
	}
}//END OF getAmmoChargeInterval






void CGauss::WeaponIdle( void )
{


		//StartFireDecision();   //is that okay?
		///return;

	ResetEmptySound( );

	// play aftershock static discharge
	if ( m_pPlayer->m_flPlayAftershock && m_pPlayer->m_flPlayAftershock < gpGlobals->time )
	{
		switch (RANDOM_LONG(0,3))
		{
		case 0:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 1:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro5.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 2:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro6.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 3:	break; // no sound
		}
		m_pPlayer->m_flPlayAftershock = 0.0;
	}

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	
	//easyForcePrintLine("AAACK!!!!!!");
	if (m_fInAttack != 0)
	{
		easyPrintLine("MY STATZ: mia: %d mf: %d",
			m_fInAttack,
			m_fireState
		);


		StartFireDecision();   //is that okay?
		m_fInAttack = 0;
		m_fireState = 0;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;
	}
	else
	{
		int iAnim;
		float flRand = RANDOM_FLOAT(0, 1);
		if (flRand <= 0.5)
		{
			iAnim = GAUSS_IDLE;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		}
		else if (flRand <= 0.75)
		{
			iAnim = GAUSS_IDLE2;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		}
		else
		{
			iAnim = GAUSS_FIDGET;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (71.0f + 1.0f)/(30.0f);
		}

		return;      //no.     apparently.
		SendWeaponAnim( iAnim );
		
	}
}






class CGaussAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_gaussammo.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_gaussammo.mdl");
		precacheAmmoPickupSound();
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if (pOther->GiveAmmo( AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY ) != -1)
		{
			playAmmoPickupSound();

			//MODDD - Could not find any sentences or clips involving Gauss ammo, specifically (uranium).
			//Perhaps this is okay for both the Gauss and the Egon?
			//UPDATE: apparently, the Gauss's ammo is the universal, and the egon's ammo is just the
			//mp5 chain ammo (seems really unfitting, I think it was a placeholder).
			if(pOther->IsPlayer()){
				CBasePlayer* pPlayer = (CBasePlayer*)pOther;
				pPlayer->SetSuitUpdate("!HEV_EGONPOWER", FALSE, SUIT_NEXT_IN_20MIN);
			}


			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_gaussclip, CGaussAmmo );
