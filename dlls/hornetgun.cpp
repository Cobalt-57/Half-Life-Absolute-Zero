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
#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "hornet.h"
#include "gamerules.h"


enum hgun_e {
	HGUN_IDLE1 = 0,
	HGUN_FIDGETSWAY,
	HGUN_FIDGETSHAKE,
	HGUN_DOWN,
	HGUN_UP,
	HGUN_SHOOT
};

enum firemode_e
{
	FIREMODE_TRACK = 0,
	FIREMODE_FAST
};


LINK_ENTITY_TO_CLASS( weapon_hornetgun, CHgun );



//EXTERN
EASY_CVAR_EXTERN(fastHornetsInheritsPlayerVelocity);




BOOL CHgun::IsUseable( void )
{
	return TRUE;
}

void CHgun::Spawn( )
{
	Precache( );
	m_iId = WEAPON_HORNETGUN;
	SET_MODEL(ENT(pev), "models/w_hgun.mdl");

	m_iDefaultAmmo = HIVEHAND_DEFAULT_GIVE;
	m_iFirePhase = 0;

	FallInit();// get ready to fall down.
}


void CHgun::Precache( void )
{
	//Lots of sounds precached in the hornet's precache, including this weapon's firing sounds.
	//Not that this makes a difference, precached either way with UTIL_PrecacheOther("hornet") below.
	precacheGunPickupSound();

	PRECACHE_MODEL("models/v_hgun.mdl");
	PRECACHE_MODEL("models/w_hgun.mdl");
	PRECACHE_MODEL("models/p_hgun.mdl");

	m_usHornetFire = PRECACHE_EVENT ( 1, "events/firehornet.sc" );

	UTIL_PrecacheOther("hornet");
}


//MODDD
void CHgun::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_HORNET", FALSE, SUIT_NEXT_IN_30MIN);
}

int CHgun::AddToPlayer( CBasePlayer *pPlayer )
{
	if ( CBasePlayerWeapon::AddToPlayer( pPlayer ) )
	{

#ifndef CLIENT_DLL
		if ( g_pGameRules->IsMultiplayer() )
		{
			// in multiplayer, all hivehands come full. 
			pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] = HORNET_MAX_CARRY;
		}
#endif

		MESSAGE_BEGIN( MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev );
			WRITE_BYTE( m_iId );
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

int CHgun::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Hornets";
	p->iMaxAmmo1 = HORNET_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;
	//MODDD - rpg moved out.
	//p->iPosition = 3;
	p->iPosition = 2;

	p->iId = m_iId = WEAPON_HORNETGUN;
	p->iFlags = ITEM_FLAG_NOAUTOSWITCHEMPTY | ITEM_FLAG_NOAUTORELOAD;
	p->iWeight = HORNETGUN_WEIGHT;

	return 1;
}


BOOL CHgun::Deploy( )
{
	//MODDD - get a differnet deploy time.
	return DefaultDeploy( "models/v_hgun.mdl", "models/p_hgun.mdl", HGUN_UP, "hive", 0, 0, 30.0/30.0, 30.0/30.0 );
}

void CHgun::Holster( int skiplocal /* = 0 */ )
{
	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	//SendWeaponAnim( HGUN_DOWN );

	//!!!HACKHACK - can't select hornetgun if it's empty! no way to get ammo for it, either.
	if ( !m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] )
	{
		m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] = 1;
	}

	//WARNING - this is retail's, not sure if what we use will have a different anim time.
	DefaultHolster(HGUN_DOWN, skiplocal, 0, (19.0f/16.0f) );

}


void CHgun::PrimaryAttack()
{

	Reload( );

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		return;
	}

#ifndef CLIENT_DLL
	UTIL_MakeVectors( m_pPlayer->pev->v_angle );

	CBaseEntity *pHornet = CBaseEntity::Create( "hornet", m_pPlayer->GetGunPosition( ) + gpGlobals->v_forward * 16 + gpGlobals->v_right * 8 + gpGlobals->v_up * -12, m_pPlayer->pev->v_angle, m_pPlayer->edict() );
	
	//MODDD - no, nevermind.  Too slow to be decent, hornets move on their own, making this seem really wonky.
	pHornet->pev->velocity = gpGlobals->v_forward * 300;

	//Hm. ?
	//pHornet->vecFlightDirTrue = gpGlobals->v_forward * 300;

	//pHornet->pev->velocity = gpGlobals->v_forward * 300 + UTIL_GetProjectileVelocityExtra(m_pPlayer->pev->velocity, m_pPlayer->someotherhornetstuffhere);



	m_flRechargeTime = gpGlobals->time + 0.5;
#endif
	
	//MODDD
	if(m_pPlayer->cheat_infiniteclipMem == 0 && m_pPlayer->cheat_infiniteammoMem == 0){
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
	}
	m_pPlayer->m_iWeaponVolume = QUIET_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usHornetFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, FIREMODE_TRACK, 0, 0, 0 );
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (11.0/24.0) + randomIdleAnimationDelay();
	

	// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );


	//MODDD - again, no idea what the extra three lines are for, just going
	//with that.
	if(m_pPlayer->cheat_minimumfiredelayMem == 0){
		m_flNextPrimaryAttack = m_flNextPrimaryAttack + 0.25;

		if (m_flNextPrimaryAttack < UTIL_WeaponTimeBase() )
		{
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;
		}
	}else{
		//little extra, because the primary attack seems to fail if it is too low; hornets hit each other and fall.
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem + 0.03f;
	}


	

	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}



void CHgun::SecondaryAttack( void )
{

	if(m_pPlayer->cheat_infiniteammoMem == 1){
		if ( pszAmmo1() && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < 8 )
		{
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = 8;
		}
	}



	Reload();

	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0)
	{
		return;
	}

	//Wouldn't be a bad idea to completely predict these, since they fly so fast...
#ifndef CLIENT_DLL
	CBaseEntity *pHornet;
	Vector vecSrc;

	UTIL_MakeVectors( m_pPlayer->pev->v_angle );

	vecSrc = m_pPlayer->GetGunPosition( ) + gpGlobals->v_forward * 16 + gpGlobals->v_right * 8 + gpGlobals->v_up * -12;

	m_iFirePhase++;
	switch ( m_iFirePhase )
	{
	case 1:
		vecSrc = vecSrc + gpGlobals->v_up * 8;
		break;
	case 2:
		vecSrc = vecSrc + gpGlobals->v_up * 8;
		vecSrc = vecSrc + gpGlobals->v_right * 8;
		break;
	case 3:
		vecSrc = vecSrc + gpGlobals->v_right * 8;
		break;
	case 4:
		vecSrc = vecSrc + gpGlobals->v_up * -8;
		vecSrc = vecSrc + gpGlobals->v_right * 8;
		break;
	case 5:
		vecSrc = vecSrc + gpGlobals->v_up * -8;
		break;
	case 6:
		vecSrc = vecSrc + gpGlobals->v_up * -8;
		vecSrc = vecSrc + gpGlobals->v_right * -8;
		break;
	case 7:
		vecSrc = vecSrc + gpGlobals->v_right * -8;
		break;
	case 8:
		vecSrc = vecSrc + gpGlobals->v_up * 8;
		vecSrc = vecSrc + gpGlobals->v_right * -8;
		m_iFirePhase = 0;
		break;
	}

	pHornet = CBaseEntity::Create( "hornet", vecSrc, m_pPlayer->pev->v_angle, m_pPlayer->edict() );
	
	//MODDD
	//pHornet->pev->velocity = gpGlobals->v_forward * 1200;
	pHornet->pev->velocity = gpGlobals->v_forward * 1200 + UTIL_GetProjectileVelocityExtra(m_pPlayer->pev->velocity, EASY_CVAR_GET(fastHornetsInheritsPlayerVelocity) );

	pHornet->pev->angles = UTIL_VecToAngles( pHornet->pev->velocity );

	pHornet->SetThink( &CHornet::StartDart );

	m_flRechargeTime = gpGlobals->time + 0.5;
#endif

	int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

	PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), m_usHornetFire, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, FIREMODE_FAST, 0, 0, 0 );


	//MODDD
	if(m_pPlayer->cheat_infiniteclipMem == 0 && m_pPlayer->cheat_infiniteammoMem == 0){
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
	}

	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (11.0/24.0) + randomIdleAnimationDelay();


	m_pPlayer->m_iWeaponVolume = NORMAL_GUN_VOLUME;
	m_pPlayer->m_iWeaponFlash = DIM_GUN_FLASH;

		// player "shoot" animation
	m_pPlayer->SetAnimation( PLAYER_ATTACK1 );


	//MODDD 
	if(m_pPlayer->cheat_minimumfiredelayMem == 0){
		m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.1;
	}else{
		m_flNextPrimaryAttack = m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
	}
	

	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
}


void CHgun::Reload( void )
{
	if (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] >= HORNET_MAX_CARRY)
		return;

	while (m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] < HORNET_MAX_CARRY && m_flRechargeTime < gpGlobals->time)
	{
		m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]++;
		m_flRechargeTime += 0.5;
	}
}


void CHgun::WeaponIdle( void )
{
	Reload( );

	//MODDD
	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
	//if (m_flTimeWeaponIdle > gpGlobals->time)
		return;

	int iAnim;
	float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
	if (flRand <= 0.75)
	{
		iAnim = HGUN_IDLE1;
		//MODDD
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 30.0 / 16 * (2);
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 30.0 / 16 * (1) + randomIdleAnimationDelay();
		//m_flTimeWeaponIdle = gpGlobals->time + 30.0 / 16 * (1);
	}
	else if (flRand <= 0.875)
	{
		iAnim = HGUN_FIDGETSWAY;
		//MODDD
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 40.0 / 16.0 + randomIdleAnimationDelay();
		//m_flTimeWeaponIdle = gpGlobals->time + 40.0 / 16.0;
	}
	else
	{
		iAnim = HGUN_FIDGETSHAKE;
		//MODDD
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 35.0 / 16.0 + randomIdleAnimationDelay();
		//m_flTimeWeaponIdle = gpGlobals->time + 35.0 / 16.0;
	}
	SendWeaponAnim( iAnim );
}

#endif