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



//IMPORTANT NOTE: most cases of "UTIL_WeaponTimeBase" have been replaced with "gpGlobals->time".
//"UTIL_WeaponTimeBase" seems to return only 0 here, and that is not helpful for some things.

#pragma once

#include "extdll.h"
#include "egon.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "basemonster.h"

#include "nodes.h"
#include "effects.h"
#include "customentity.h"
#include "gamerules.h"


EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode);
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud);




//MODDD - several things moved to the new egon.h for commonly including client/serverside
// (less redundancy in ev_hldm.cpp)

#define EGON_SWITCH_NARROW_TIME			0.75			// Time it takes to switch fire modes
#define EGON_SWITCH_WIDE_TIME			1.5


LINK_ENTITY_TO_CLASS( weapon_egon, CEgon );






//MODDD - IMPORTANT NOTICE.  All mentions of "m_fireMode" have been replaced with "m_fInAttack", as "m_fireMode" is not well synchronized between
//			the server and client.  "m_fInAttack" was never used in here before, so it may be back-replaced for whatever reason.


//MODDD
void CEgon::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_EGON", FALSE, SUIT_NEXT_IN_30MIN);
}

void CEgon::Spawn( )
{
	Precache( );
	m_iId = WEAPON_EGON;
	SET_MODEL(ENT(pev), "models/w_egon.mdl");

	m_iClip = -1;
	m_iDefaultAmmo = EGON_DEFAULT_GIVE;

	//MODDD - set default firemode
	//m_fInAttack = FIRE_WIDE;
	//MODDD - default is now "m_fInAttack" instead.
	m_fInAttack = FIRE_NARROW;

	FallInit();// get ready to fall down.
}


void CEgon::Precache( void )
{
	PRECACHE_MODEL("models/w_egon.mdl");
	PRECACHE_MODEL("models/v_egon.mdl");
	PRECACHE_MODEL("models/p_egon.mdl");

	PRECACHE_MODEL("models/w_9mmclip.mdl");
	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND( EGON_SOUND_OFF );
	PRECACHE_SOUND( EGON_SOUND_RUN );
	PRECACHE_SOUND( EGON_SOUND_STARTUP );

	PRECACHE_SOUND ("weapons/357_cock1.wav");

	precacheGunPickupSound();


	PRECACHE_MODEL( EGON_BEAM_SPRITE );
	PRECACHE_MODEL( EGON_FLARE_SPRITE );


	m_usEgonFire = PRECACHE_EVENT ( 1, "events/egon_fire.sc" );
	m_usEgonStop = PRECACHE_EVENT ( 1, "events/egon_stop.sc" );
}


BOOL CEgon::Deploy( void )
{
	timeSinceDeployed = 0;
	currentTime = gpGlobals->time;

	m_deployed = FALSE;
	m_fireState = FIRE_OFF;
	return DefaultDeploy( "models/v_egon.mdl", "models/p_egon.mdl", EGON_DRAW, "egon", 0, 0, (16.0/30.0), -1 );
}

int CEgon::AddToPlayer( CBasePlayer *pPlayer )
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



void CEgon::Holster( int skiplocal /* = 0 */ )
{
	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

	//SendWeaponAnim( EGON_HOLSTER );

	//MODDD - at holster, force to the default position.  Firemode is adjusted at fire time, not needed here.
	m_flReleaseThrow = -4;
	m_flStartThrow = -1;


    EndAttack();


	DefaultHolster(EGON_HOLSTER, skiplocal, 0, (16.0f/30.0f) );

}

int CEgon::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "uranium";
	p->iMaxAmmo1 = URANIUM_MAX_CARRY;


	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;

	//p->pszAmmo2 = "uranium";
	//p->iMaxAmmo2 = URANIUM_MAX_CARRY;


	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	//MODDD - RPG moved out.
	//p->iPosition = 2;
	p->iPosition = 1;

	p->iId = m_iId = WEAPON_EGON;
	p->iFlags = 0;
	p->iWeight = EGON_WEIGHT;

	return 1;
}

#define EGON_PULSE_INTERVAL			0.1
#define EGON_DISCHARGE_INTERVAL		0.1

float CEgon::GetPulseInterval( void )
{
	//MODDD
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0){
		return EGON_PULSE_INTERVAL;
	}else{
		return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom);
	}
}

float CEgon::GetDischargeInterval( void )
{
	//MODDD
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0){
		return EGON_DISCHARGE_INTERVAL;
	}else{
		return EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom);
	}
}

BOOL CEgon::HasAmmo( void )
{

	
	if ( m_pPlayer->ammo_uranium <= 0 )
		return FALSE;
		
	/*
	if ( PlayerPrimaryAmmoCount() <= 0 )
	{
		//PlayEmptySound( );
		//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
		return FALSE;
	}
	*/



	return TRUE;
}

void CEgon::UseAmmo( int count )
{
	//MODDD - could the "infinite ammo cheat" be useful here?
	//So yea, only use ammo if the cheat is off.  Other weapon-related behavior should happen still.
	if(EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo) == 0){
		if (PlayerPrimaryAmmoCount() >= count) {
			//m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] -= count;
			ChangePlayerPrimaryAmmoCount(-count);
		}
		else {
			//m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 0;
			SetPlayerPrimaryAmmoCount(0);
		}
	}

}






//MODDD - undone the "UTIL_WeaponTimeBase  --->  gpGlobals->time"   replacements seen before throughout this method.
//Apparently, this causes a glitch where the egon fire sound does not re-loop when playing for more than several seconds.
//Perhaps the client does just fine with "UTIL_WeaponTimeBase" or something?
void CEgon::Attack( void )
{
	// don't fire underwater
	if ( m_pPlayer->pev->waterlevel == 3 )
	{
		
		if ( m_fireState != FIRE_OFF || m_pBeam )
		{
			EndAttack();

			switch ( m_fInAttack )
			{
			case FIRE_NARROW:
				m_flReleaseThrow = -2;
			break;
			case FIRE_WIDE:
				m_flReleaseThrow = -1;
			break;
			}


		}
		else
		{
			PlayEmptySound( );
		}
		return;
	}

	UTIL_MakeVectors( m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle );
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc	 = m_pPlayer->GetGunPosition( );

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	switch( m_fireState )
	{
		case FIRE_OFF:
		{
			
			//easyForcePrintLine("IS THIS A JOKE??? %df", m_pPlayer->ammo_uranium);
			if ( !HasAmmo() )
			{
				//m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.25;
				m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;
				PlayEmptySound( );


				switch ( m_fInAttack )
				{
				case FIRE_NARROW:
					m_flReleaseThrow = -2;
				break;
				case FIRE_WIDE:
					m_flReleaseThrow = -1;
				break;
				}

				return;
			}

			m_flAmmoUseTime = gpGlobals->time;// start using ammo ASAP.

			PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usEgonFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, m_fireState, m_fInAttack, 1, 0 );
						
			m_shakeTime = 0;

			m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
			pev->fuser1	= UTIL_WeaponTimeBase() + 2;

			pev->dmgtime = gpGlobals->time + GetPulseInterval();
			m_fireState = FIRE_CHARGE;
			stopBlockLooping();
		}
		break;

		case FIRE_CHARGE:
		{
			Fire( vecSrc, vecAiming );
			//MODDD - the volume sent out to alert monsters is greater for "wide" fire.
			//m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME;


			if(m_fInAttack == FIRE_NARROW){
				m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME + 100;

			}else if(m_fInAttack == FIRE_WIDE){
				m_pPlayer->m_iWeaponVolume = EGON_PRIMARY_VOLUME + 275;

			}
		
			if ( pev->fuser1 <= UTIL_WeaponTimeBase() )
			{
				PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usEgonFire, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, m_fireState, m_fInAttack, 0, 0 );
				pev->fuser1 = 1000;
			}

			if ( !HasAmmo() )
			{
				EndAttack();
				//ran out cus' you be out of ammo?  Freeze until the player lets go of the mouse.
				
				//CHECK!!!!!

				//if(m_flReleaseThrow == 1){
					
				//}
					switch ( m_fInAttack )
					{
					case FIRE_NARROW:
						m_flReleaseThrow = -2;
					break;
					case FIRE_WIDE:
						m_flReleaseThrow = -1;
					break;
					}

				//MODDD - this means, flag for starting the idle following a stop.
				//m_flReleaseThrow = -666;
				//m_flReleaseThrow = -668;

				//m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.0;
				m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;

				//???
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
				stopBlockLooping();
			}

		}
		break;
	}
}














//MODDD - new vars
CEgon::CEgon(){


	//VARS:
	//---------------------------------------------------------------------------------------------------------------------
	//m_flReleaseThrow ->  "animationSequence".  Which animation to perform next?
	//-3 = firing PRIMARY (narrow now).   Resets to -1 when released.
	//-2 = idle (wait for release; not in effect, so same effect as -1).
	//-1 = idle (wait to be triggered by mouse press at all)
	//0 = ALTFIREON.  turn the top lever to the bottom.
	//1 = ALTFIRECYCLE - attacking.  Firing the "wide" beam (as opposed to the NOW default "narrow"; tradeoff for delay).
	//2 = ALTFIREOFF.  turn the lever at the bottom back to the top.       After that, resumes at "idle", or -1.
	//---------------------------------------------------------------------------------------------------------------------
	//m_flStartThrow -> "currentDelay".  When do I "advance" this animation, if applicable?
	//-1 - non applicable.  Used for idling and firing (not timed).
	//anything else - starts as on offset of "gpGlobals->time" (the current time), and is compared against the current
	//                time to see when to change "m_flReleaseThrow" typically.

	//These existing vars with now irrelevant names are used instead of new ones since they're synced through the DLL's, which are
	//close to impossible to mod.  Just working with what I have (and that, well, works).


	
	canStartSecondary = TRUE;
	secondarySwitched = FALSE;
	holdingPrimary = FALSE;
	holdingSecondary = TRUE;

	queueAnim = -1;


	oldTime = 0;
	currentTime = 0;

	timeDelta = 0;

	lastSentAnim = -1;
	


	timeSinceDeployed = 0;

	oldTime = -1;
	currentTime = -1;

	timeDelta = 0;

	legalHoldSecondary = FALSE;
	startedSecondaryHoldAttempt = FALSE;
	holdingSecondaryCurrent = 0;
	animationTime = 0;


	holdingSecondaryTarget0 = 1.533f - 0.92;
	holdingSecondaryTarget1 = 1.533f;
	holdingSecondaryTarget2 = 0.71f;


	toggledYet = FALSE;

	altFireOn = FALSE;

	animationSequence = -1;
	//for "idle".





	//m_chargeReady = -1;  //corresponds to animationSequence, better for sync with the client.


	//Timer.
	m_flStartThrow = -1;

	//animation index.
	m_flReleaseThrow = -4;
	//-4: true idle (idle locked).
	//-3: true idle.  Fire anytime.
	//-2: locked idle, primary.  Freezes to any idle anim.
	//-1: locked idle, secondary.  Freezes to secondary firing anim.
	//0: secondary: turn the switch on.
	//1: firing, primary.
	//2: firing, secondary.
	//3: turning off (secondary)

	effectsExist = FALSE;

	//m_flReleaseThrow = FALSE;
}//END OF CEgon constructor



// Save/restore for serverside only!
#ifndef CLIENT_DLL
TYPEDESCRIPTION	CEgon::m_SaveData[] =
{
	//	DEFINE_FIELD( CEgon, m_pBeam, FIELD_CLASSPTR ),
	//	DEFINE_FIELD( CEgon, m_pNoise, FIELD_CLASSPTR ),
	//	DEFINE_FIELD( CEgon, m_pSprite, FIELD_CLASSPTR ),
	DEFINE_FIELD(CEgon, m_shootTime, FIELD_TIME),
	DEFINE_FIELD(CEgon, m_fireState, FIELD_INTEGER),
	DEFINE_FIELD(CEgon, m_fireMode, FIELD_INTEGER),
	DEFINE_FIELD(CEgon, m_shakeTime, FIELD_TIME),
	DEFINE_FIELD(CEgon, m_flAmmoUseTime, FIELD_TIME),
};
// custom implementations instead, see below.
//IMPLEMENT_SAVERESTORE( CEgon, CBasePlayerWeapon );

int CEgon::Save(CSave& save){
	if (!CBasePlayerWeapon::Save(save))
		return 0;
	return save.WriteFields("CEgon", this, m_SaveData, ARRAYSIZE(m_SaveData));
}
int CEgon::Restore(CRestore& restore){


	if (!CBasePlayerWeapon::Restore(restore))
		return 0;

	//if(m_pPlayer != NULL){
		//m_pPlayer->TabulateAmmo();
	//}

	int result = restore.ReadFields("CEgon", this, m_SaveData, ARRAYSIZE(m_SaveData));
	m_flReleaseThrow = -4;
	return result;
}
#endif




void CEgon::PrimaryAttack( void )
{


}



void CEgon::PrimaryNotHeld( void ){


}


void CEgon::NeitherHeld( void ){




}




void CEgon::BothHeld( void ){


	/*
	easyPrintLine("BOTH HELD??????");
	//pretend that everything is released.  The user should never hold both buttons down, so just give up trying to process that.
	*/

	/*
	PrimaryNotHeld();
	SecondaryNotHeld();
	NeitherHeld();

	*/

	
	
	//PrimaryNotHeld();
	//SecondaryAttack();
	

}

void CEgon::SecondaryNotHeld( void ){
	


}



void CEgon::SecondaryAttack( void )
{

}//END OF SecondaryAttack



void CEgon::ItemPostFrame(){


	/*
#ifdef CLIENT_DLL
	easyPrintLine("YOU EG %.2f", m_flReleaseThrow);
#else
	easyPrintLine("YOU EG %.2f", m_flReleaseThrow);
#endif
	*/

	BOOL sendFidgetOnOff = TRUE;

	BOOL holdingPrimary = m_pPlayer->pev->button & IN_ATTACK;
	BOOL holdingSecondary = m_pPlayer->pev->button & IN_ATTACK2;
	
	if(holdingPrimary && holdingSecondary){
		//m_flTimeWeaponIdle = -1;  ???
		easyForcePrintLine("IM HOLDING BOTH??? %.2f", gpGlobals->time);

		if(m_flReleaseThrow >= 0){
			switch ( m_fInAttack )
				{
				case FIRE_NARROW:
					m_flReleaseThrow = -2;
				break;
				case FIRE_WIDE:
					m_flReleaseThrow = -1;
				break;
			}

			//sendFidgetOnOff = FALSE;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
		}
		holdingPrimary = FALSE;
		holdingSecondary = FALSE;

		//WeaponIdle();
		//return;   //block!
	}




	
	if(holdingSecondary){
		if(m_flReleaseThrow == -4 || m_flReleaseThrow == -3){
			//ready to begin.
			m_flReleaseThrow = 0;
			m_flStartThrow = gpGlobals->time;
			SendWeaponAnimBypass( EGON_ALTFIREON, 1 );
		}else if(m_flReleaseThrow == 2){
			//fire!
			m_fInAttack = FIRE_WIDE;
			Attack();
		
		//CBasePlayerWeapon::ItemPostFrame();
		//return;
		}
	
	
	}else if(holdingPrimary){

	
	if(m_flReleaseThrow == -4 || m_flReleaseThrow == -3){
		//may fire.

		m_flReleaseThrow = 1;
		m_fInAttack = FIRE_NARROW;
		Attack();
	}else if(m_flReleaseThrow == 1){
		m_fInAttack = FIRE_NARROW;
		Attack();
	}

	}

	//MODDD - primary fire is now narrow instead of wide, since it has no delay animation (tradeoff).
	

	//easyForcePrintLine("IM GONNA no %.2f %.2f", m_flTimeWeaponIdle, UTIL_WeaponTimeBase());


	if(m_flReleaseThrow == 0){

		if(holdingSecondary == FALSE){
			//Forget or reverse anim?
			
			//NOTE: slightly different interpretation.
			float timePassed = gpGlobals->time - m_flStartThrow;
			
			//holdingSecondaryTarget0 = 1.533f - 0.92;
			//holdingSecondaryTarget1 = 1.533f;

			if(timePassed < holdingSecondaryTarget0){
				//jump back to idling.
				m_flReleaseThrow = -4;
				m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
				//m_pPlayer->forceNoWeaponLoop = TRUE;
			}else{
				//too close to the end, skip to the switch back up.
				m_flReleaseThrow = 3;
				m_flStartThrow = gpGlobals->time + holdingSecondaryTarget2;
				SendWeaponAnimBypass( EGON_ALTFIREOFF, 1 );
			}


		}else{
			//no issue.
			//... holdingSecondaryTarget1

			
			float timePassed = gpGlobals->time - m_flStartThrow;

			if(timePassed > holdingSecondaryTarget1 ){
				m_flReleaseThrow = 2;
			}
		}

	}else if(m_flReleaseThrow == -2){
		
		if(sendFidgetOnOff){
			SendWeaponAnimBypass( EGON_FIDGET1, 1 );
		}

		//?
	}else if(m_flReleaseThrow == -1){
		SendWeaponAnimBypass( EGON_ALTFIREOFF, 1 );
		
	}else if(m_flReleaseThrow == 1){

		//?
	}else if(m_flReleaseThrow == 3){
		//... holdingSecondaryTarget2
		if(gpGlobals->time > m_flStartThrow ){
			m_flReleaseThrow = -4;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
		}
	}


	
	
	if ( m_fireState != FIRE_OFF && (!holdingPrimary && !holdingSecondary) ){
		//easyPrintLine("END O ATTACK!");
		EndAttack();

		//this is a cease-fire by release.  How to act?

		switch ( m_fInAttack )
		{
		case FIRE_NARROW:
			m_flReleaseThrow = -4;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
		break;
		case FIRE_WIDE:
			m_flReleaseThrow = 3;
			m_flStartThrow = gpGlobals->time + holdingSecondaryTarget2;
			SendWeaponAnimBypass( EGON_ALTFIREOFF, 1 );
		break;
		}

	}




	
	CBasePlayerWeapon::ItemPostFrame();

}


//MODDD - method added.  Taps into the event-method called (virtually) every frame.
void CEgon::ItemPreFrame( void )
{

	CBasePlayerWeapon::ItemPreFrame();
	
}


void CEgon::Fire( const Vector &vecOrigSrc, const Vector &vecDir )
{
	

	Vector vecDest = vecOrigSrc + vecDir * 2048;
	edict_t		*pentIgnore;
	TraceResult tr;

	pentIgnore = m_pPlayer->edict();
	Vector tmpSrc = vecOrigSrc + gpGlobals->v_up * -8 + gpGlobals->v_right * 3;

	// ALERT( at_console, "." );
	
	UTIL_TraceLine( vecOrigSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr );


	///////tr.vecEndPos();


	if (tr.fAllSolid)
		return;

#ifndef CLIENT_DLL
	CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

	if (pEntity == NULL)
		return;

	if ( IsMultiplayer() )
	{
		if ( m_pSprite && pEntity->pev->takedamage )
		{
			m_pSprite->pev->effects &= ~EF_NODRAW;
		}
		else if ( m_pSprite )
		{
			//MODDD - undone, seems unnecessary.  Original left.
			m_pSprite->pev->effects |= EF_NODRAW;
			//m_pSprite->pev->effects &= ~EF_NODRAW;
		}
	}


#endif

	float timedist;

	switch ( m_fInAttack )
	{
	case FIRE_NARROW:
#ifndef CLIENT_DLL
		if ( pev->dmgtime < gpGlobals->time )
		{
			// Narrow mode only does damage to the entity it hits
			ClearMultiDamage();
			if (pEntity->pev->takedamage)
			{
				//MODDD - added args.  Egon does not make the "bodyhit" sound. See above.
				//        ...it has since been changed how this works. The outside source no longer tells TraceAttack to play the bodyhit sound, but TraceAttack tells the caller whether
				//        to play the bodyhit and it's up to the caller to respond to that if it wants to. So by default, no bodyhit sound will play.
				pEntity->TraceAttack( m_pPlayer->pev, gSkillData.plrDmgEgonNarrow, vecDir, &tr, DMG_ENERGYBEAM, 0, TRUE );
			}
			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);

			if ( IsMultiplayer() )
			{
				// multiplayer uses 1 ammo every 1/10th second
				if ( gpGlobals->time >= m_flAmmoUseTime )
				{
					UseAmmo( 1 );
					m_flAmmoUseTime = gpGlobals->time + 0.1;
				}
			}
			else
			{
				// single player, use 3 ammo/second
				//MODDD - NOTE.  Above is an as-is comment, this is really 6 per second.
				if ( gpGlobals->time >= m_flAmmoUseTime )
				{
					UseAmmo( 1 );
					m_flAmmoUseTime = gpGlobals->time + 0.166;
				}
			}

			pev->dmgtime = gpGlobals->time + GetPulseInterval();
		}
#endif
		timedist = ( pev->dmgtime - gpGlobals->time ) / GetPulseInterval();
		break;
	
	case FIRE_WIDE:
#ifndef CLIENT_DLL
		if ( pev->dmgtime < gpGlobals->time )
		{
			// wide mode does damage to the ent, and radius damage
			ClearMultiDamage();
			if (pEntity->pev->takedamage)
			{
				//MODDD - added args.  Egon does not make the "bodyhit" sound. See above.
				pEntity->TraceAttack( m_pPlayer->pev, gSkillData.plrDmgEgonWide, vecDir, &tr, DMG_ENERGYBEAM | DMG_ALWAYSGIB, 0, TRUE);
			}
			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);

			if ( IsMultiplayer() )
			{
				// radius damage a little more potent in multiplayer.
				::RadiusDamage( tr.vecEndPos, pev, m_pPlayer->pev, gSkillData.plrDmgEgonWide/4, 128, CLASS_NONE, DMG_ENERGYBEAM | DMG_BLAST | DMG_ALWAYSGIB );
			}

			if ( !m_pPlayer->IsAlive() )
				return;

			if ( IsMultiplayer() )
			{
				//multiplayer uses 5 ammo/second
				if ( gpGlobals->time >= m_flAmmoUseTime )
				{
					UseAmmo( 1 );
					m_flAmmoUseTime = gpGlobals->time + 0.2;
				}
			}
			else
			{
				// Wide mode uses 10 charges per second in single player
				if ( gpGlobals->time >= m_flAmmoUseTime )
				{
					UseAmmo( 1 );
					m_flAmmoUseTime = gpGlobals->time + 0.1;
				}
			}

			pev->dmgtime = gpGlobals->time + GetDischargeInterval();
			if ( m_shakeTime < gpGlobals->time )
			{
				UTIL_ScreenShake( tr.vecEndPos, 5.0, 150.0, 0.75, 250.0 );
				m_shakeTime = gpGlobals->time + 1.5;
			}
		}
#endif
		timedist = ( pev->dmgtime - gpGlobals->time ) / GetDischargeInterval();
		break;
	}

	if ( timedist < 0 )
		timedist = 0;
	else if ( timedist > 1 )
		timedist = 1;
	timedist = 1-timedist;

	UpdateEffect( tmpSrc, tr.vecEndPos, timedist );
	
}



//MODDD - why was this method commented out?  Could've been me though.
void CEgon::UpdateEffect( const Vector &startPoint, const Vector &endPoint, float timeBlend )
{

	
	
	
#ifndef CLIENT_DLL
	float egonEffectsModeVar = EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode);
	float egonHitCloudVar = EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud);


	//if ( !m_pBeam )
	if(!effectsExist)
	{
		CreateEffect();
	}

	if(m_pBeam){
		m_pBeam->SetStartPos( endPoint );
		m_pBeam->SetBrightness( 255 - (timeBlend*180) );
		m_pBeam->SetWidth( 40 - (timeBlend*20) );


		if(egonEffectsModeVar == 3){
			//pre-release colors. this is blue always (and only exists for secondary fire).
			m_pBeam->SetColor( 60 + (25*timeBlend), 120 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
		}else{
			//beam colors work as usual.
			if ( m_fInAttack == FIRE_WIDE )
				//purple?
				m_pBeam->SetColor( 30 + (25*timeBlend), 30 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
			else
				//blue?
				m_pBeam->SetColor( 60 + (25*timeBlend), 120 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );

		}


	}

	if(m_pNoise){
		if(egonEffectsModeVar == 3){
			//pre-release colors. Only present for wide fire. and blue.?
			//To make less transparent multiply each by 1.7. including the multiples of variables. OR just the whole thing in parenthesis each argument.
			//m_pNoise->SetColor( 60 + (25*timeBlend), 120 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
			//...no, don't make it blue-er? I think?
			
			//m_pNoise->SetColor( 30 + (25*timeBlend), 30 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );

			//...just keep it constant to what it started as. huh.
		
		}else{
			//normal... maybe.
			m_pNoise->SetColor( 30 + (25*timeBlend), 30 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
		}
	
		m_pNoise->SetStartPos( endPoint );
	
	}


	/*
	
		if ( m_fInAttack == FIRE_WIDE )
		{
			m_pNoise->SetColor( 50, 50, 255 );
			m_pNoise->SetNoise( 8 );
		}
		else
		{
			m_pNoise->SetColor( 80, 120, 255 );
			m_pNoise->SetNoise( 2 );
		}

	*/





	if(egonEffectsModeVar == 4){
		//CHRISTMAS MODE!  Makes what beam is what for egonEffectsMode #2 easier to spot.
		if(m_pBeam){
			m_pBeam->SetColor( 255,0 ,0 );
		}
		if(m_pNoise){
			m_pNoise->SetColor( 0, 255, 0 );
		}
	}


	if(m_pSprite){
		UTIL_SetOrigin( m_pSprite->pev, endPoint );
		m_pSprite->pev->frame += 8 * gpGlobals->frametime;
		if ( m_pSprite->pev->frame > m_pSprite->Frames() )
			m_pSprite->pev->frame = 0;

	}

	
	


#endif
	
}

void CEgon::CreateEffect( void )
{
	//is "CBaseMonster::Instance" even legal here?  not sure  if the attachment qualifies as a "entity".

	//also, ENT?
	//CBaseEntity* attachment = CBaseMonster::Instance(  (INDEXENT(m_pPlayer->entindex() + 0x1000)));


	//edict_t* attachment = (  (INDEXENT(entindex()  )));

	//easyPrintLine("WHAT THE att %d", (attachment == NULL) );


#ifndef CLIENT_DLL


	//INDEXENT???
	
	float egonEffectsModeVar = EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonEffectsMode);
	float egonHitCloudVar = EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(egonHitCloud);

	
	//ripped from rpg right before creating the rocket at the tip of the weapon.
	Vector vecSrc = pev->origin; //m_pPlayer->GetGunPosition( ) + -gpGlobals->v_forward * 100 + gpGlobals->v_right * 8 + gpGlobals->v_up * -8;
		
	DestroyEffect();

	effectsExist = TRUE;


	//NOT if 3!
	if(egonEffectsModeVar >= 1 && egonEffectsModeVar != 3){


		
		m_pBeam = CBeam::BeamCreate( EGON_BEAM_SPRITE, 40 );
		m_pBeam->PointEntInit( vecSrc, m_pPlayer->entindex()  );
		//MODDD - commented out.  Causes the beam not to sufficiently shrink when nearing a surface you're firing at, causing the effect to clip backwards into the player & gun.
		//m_pBeam->SetFlags( BEAM_FSINE );
		m_pBeam->SetEndAttachment( 1 );
		m_pBeam->pev->spawnflags |= SF_BEAM_TEMPORARY;	// Flag these to be destroyed on save/restore or level transition
		//if(testVar == 0 || testVar == 2 ){
		//	m_pBeam->pev->flags |= FL_SKIPLOCALHOST;
		////...does this need to stay commented out?
		//}
		m_pBeam->pev->owner = m_pPlayer->edict();



		if ( m_fInAttack == FIRE_WIDE )
		{
			m_pBeam->SetScrollRate( 50 );
			m_pBeam->SetNoise( 20 );
		}
		else
		{
			m_pBeam->SetScrollRate( 110 );
			m_pBeam->SetNoise( 5 );
		}
		
	}

	if(egonEffectsModeVar >= 2 && egonEffectsModeVar != 3){

		m_pNoise = CBeam::BeamCreate( EGON_BEAM_SPRITE, 55 );
		//new?
		//m_pNoise->SetFlags( BEAM_FSINE );

		m_pNoise->PointEntInit( vecSrc, m_pPlayer->entindex() );
		m_pNoise->SetScrollRate( 25 );
		m_pNoise->SetBrightness( 100 );
		m_pNoise->SetEndAttachment( 1 );
		m_pNoise->pev->spawnflags |= SF_BEAM_TEMPORARY;
		//if(testVar == 0 || testVar == 1){
		//	m_pNoise->pev->flags |= FL_SKIPLOCALHOST;
		//}
		m_pNoise->pev->owner = m_pPlayer->edict();


		if ( m_fInAttack == FIRE_WIDE )
		{
			m_pNoise->SetColor( 50, 50, 255 );
			m_pNoise->SetNoise( 8 );
		}
		else
		{
			m_pNoise->SetColor( 80, 120, 255 );
			m_pNoise->SetNoise( 2 );
		}

		//m_pNoise->Point
	}



	

	//choice 3, pre-release, has special rules.
	//for primary fire, the two clientside lasers only: spiral and the thicker straight one.
	//For secondary fire, the clientside thicker straight and m_pBeam and m_pNoise (but of narrow fire).
	if(egonEffectsModeVar == 3){

		
		//noise beam gets some different behavior.

		//For wide only.
		
		if ( m_fInAttack == FIRE_WIDE ){


			
			m_pBeam = CBeam::BeamCreate( EGON_BEAM_SPRITE, 40 );
			m_pBeam->PointEntInit( vecSrc, m_pPlayer->entindex()  );
			//MODDD - commented out.  Causes the beam not to sufficiently shrink when nearing a surface you're firing at, causing the effect to clip backwards into the player & gun.
			//m_pBeam->SetFlags( BEAM_FSINE );
			m_pBeam->SetEndAttachment( 1 );
			m_pBeam->pev->spawnflags |= SF_BEAM_TEMPORARY;	// Flag these to be destroyed on save/restore or level transition
			//if(testVar == 0 || testVar == 2 ){
			//	m_pBeam->pev->flags |= FL_SKIPLOCALHOST;
			//}
			m_pBeam->pev->owner = m_pPlayer->edict();



			//if ( m_fInAttack == FIRE_WIDE )
			//{
			//	m_pBeam->SetScrollRate( 50 );
			//	m_pBeam->SetNoise( 20 );
			//}
			//else
			//{
			//// want narrow mode's features.
				m_pBeam->SetScrollRate( 110 );
				m_pBeam->SetNoise( 5 );
			//}








			m_pNoise = CBeam::BeamCreate( EGON_BEAM_SPRITE, 55 );
			//new?
			//m_pNoise->SetFlags( BEAM_FSINE );

			m_pNoise->PointEntInit( vecSrc, m_pPlayer->entindex() );
			m_pNoise->SetScrollRate( 25 );
			m_pNoise->SetBrightness( 100 );
			m_pNoise->SetEndAttachment( 1 );
			m_pNoise->pev->spawnflags |= SF_BEAM_TEMPORARY;
			//if(testVar == 0 || testVar == 1){
			//	m_pNoise->pev->flags |= FL_SKIPLOCALHOST;
			//}
			m_pNoise->pev->owner = m_pPlayer->edict();


			//if ( m_fInAttack == FIRE_WIDE )
			//{
			//	m_pNoise->SetColor( 50, 50, 255 );
			//	m_pNoise->SetNoise( 8 );
			//}
			//else
			//{
			//// want narrow's features.
				m_pNoise->SetColor( 80, 120, 255 );
				m_pNoise->SetNoise( 2 );
			//}

		}
		
		//m_pNoise->Point
	}





	
	if(egonHitCloudVar == 1){

		m_pSprite = CSprite::SpriteCreate( EGON_FLARE_SPRITE, pev->origin, FALSE );
		m_pSprite->pev->scale = 1.0;
		m_pSprite->SetTransparency( kRenderGlow, 255, 255, 255, 255, kRenderFxNoDissipation );
		m_pSprite->pev->spawnflags |= SF_SPRITE_TEMPORARY;
		//m_pSprite->pev->flags |= FL_SKIPLOCALHOST;
		m_pSprite->pev->owner = m_pPlayer->edict();

	}

	
	//MODDD - note.  Colors may not matter here if they are updated constantly by "updateEffect".
	
	
	/*
	m_pNoise->SetColor( 50, 50, 255 );
	m_pNoise->SetColor( 80, 120, 255 );
	m_pBeam->SetColor( 30 + (25*timeBlend), 30 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
	*/

	//as seen in "updateEffect" for the main beam.  
	/*
	if ( m_fInAttack == FIRE_WIDE )
		m_pBeam->SetColor( 30 + (25*timeBlend), 30 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
	else
		m_pBeam->SetColor( 60 + (25*timeBlend), 120 + (30*timeBlend), 64 + 80*fabs(sin(gpGlobals->time*10)) );
	*/


#endif
	
}


void CEgon::DestroyEffect( void )
{

#ifndef CLIENT_DLL

	effectsExist = FALSE;

	if ( m_pBeam )
	{
		UTIL_Remove( m_pBeam );
		m_pBeam = NULL;
	}
	if ( m_pNoise )
	{
		UTIL_Remove( m_pNoise );
		m_pNoise = NULL;
	}
	if ( m_pSprite )
	{
		if ( m_fInAttack == FIRE_WIDE )
			m_pSprite->Expand( 10, 500 );
		else
			UTIL_Remove( m_pSprite );
		m_pSprite = NULL;
	}
#endif

}



void CEgon::WeaponIdle( void )
{
	



	ResetEmptySound( );


	//used to be this?  odd how it isn't using "UTIL_WeaponTimeBase()" like the other references to time here?
	//if ( m_flTimeWeaponIdle > gpGlobals->time )
	//	return;


	if(m_flReleaseThrow <= -1){
		//only allow influence from here for idle animations.

	}else{
		return;
	}

	
	
	if(m_flReleaseThrow == -2){
		//NO, the point is to release, so this is OK.
		m_flReleaseThrow = -4;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + randomIdleAnimationDelay();
		//return;
		//TODO: give a random delay here!!!
	}


	if(m_flReleaseThrow == -4){
		//force anim
		SendWeaponAnimBypass( EGON_FIDGET1, 1 );
	}
	
	if(m_flReleaseThrow == -1){
		m_flReleaseThrow = 3;
		m_flStartThrow = gpGlobals->time + holdingSecondaryTarget2;
		SendWeaponAnimBypass( EGON_ALTFIREOFF, 1 );
		return;
	}

	
	//easyPrintLine("????????............. %.2f %.2f %.2f", m_flTimeWeaponIdle, UTIL_WeaponTimeBase(), gpGlobals->time);

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	
	if(m_flReleaseThrow == -4){
		//End.
		m_flReleaseThrow = -3;
		return;
	}



	//easyPrintLine("EGON IDLE CALLED %.2f", gpGlobals->time);

	//m_flStartThrow = 0;

	









	/*
	//MOVE + MOD!!!
	if ( m_fireState != FIRE_OFF ){
		//easyPrintLine("END O ATTACK!");
		EndAttack();

	}
	*/


	int iAnim;

	//debug sound.
	//EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);

	//MODDD
	if(lastSentAnim != EGON_FIDGET1){
		float flRand = RANDOM_FLOAT(0,1);

		if ( flRand <= 0.5 )
		{
			iAnim = EGON_IDLE1;
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (59.8 / 30.0) + randomIdleAnimationDelay();
		}
		else 
		{
			iAnim = EGON_FIDGET1;
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 3;
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 4.3;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (60.0 / 30.0) + randomIdleAnimationDelay();
		
		}
	}else{
		iAnim = EGON_IDLE1;
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (59.8 / 30.0) + randomIdleAnimationDelay();
		//don't fidget twice in a row.
	}

	
	

	
	forceBlockLooping();
	SendWeaponAnim( -1, 1, m_fireState );


	lastSentAnim = iAnim;
	SendWeaponAnimBypass( iAnim, 1 );
	//m_deployed = TRUE;
	//???
}



void CEgon::EndAttack( void )
{
	bool bMakeNoise = false;
		
	if ( m_fireState != FIRE_OFF ) //Checking the button just in case!.
		 bMakeNoise = true;

	//MODDD - 
	//if(m_fireState == FIRE_OFF){
	//	return; //already off?
	//}

	PLAYBACK_EVENT_FULL( FEV_GLOBAL | FEV_RELIABLE, m_pPlayer->edict(), m_usEgonStop, 0, (float *)&m_pPlayer->pev->origin, (float *)&m_pPlayer->pev->angles, 0.0, 0.0, bMakeNoise, 0, 0, 0 );

	//MODDD - this part should use "UTIL_WeaponBase()", even if it is 0...
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;

	//easyPrintLine("TEH ee %.2f", UTIL_WeaponTimeBase());

	//m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
	//m_flNextSecondaryAttack = gpGlobals->time + 0.5;
	

	//m_flReleaseThrow = -668;
	

	
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;
	//m_flNextPrimaryAttack = m_flNextSecondaryAttack = gpGlobals->time + 0.5;

	m_fireState = FIRE_OFF;

	DestroyEffect();

}



class CEgonAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_chainammo.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_chainammo.mdl");
		precacheAmmoPickupSound();
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		if (pOther->GiveAmmo( AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY ) != -1)
		{
			//MODDD - filtered.
			playAmmoPickupSound();

			//MODDD
			//UPDATE: apparently, the Gauss's ammo is the universal, and the egon's ammo is just the
			//mp5 chain ammo (seems really unfitting, I think it was a placeholder).
			//Still, leaving the expected sound.
			if(pOther->IsPlayer()){
				CBasePlayer* pPlayer = (CBasePlayer*)pOther;
				pPlayer->SetSuitUpdate("!HEV_EGONPOWER", FALSE, SUIT_NEXT_IN_20MIN);
			}


			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_egonclip, CEgonAmmo );
