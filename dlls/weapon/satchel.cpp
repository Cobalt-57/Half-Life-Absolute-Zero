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
#include "satchel.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"







//MODDD - why wasn't this serverside anyway??
#ifndef CLIENT_DLL
//=========================================================
// DeactivateSatchels - removes all satchels owned by
// the provided player. Should only be used upon death.
//
// Made this global on purpose.
//=========================================================
void DeactivateSatchels(CBasePlayer* pOwner)
{
	edict_t* pFind;

	pFind = FIND_ENTITY_BY_CLASSNAME(NULL, "monster_satchel");

	while (!FNullEnt(pFind))
	{
		CBaseEntity* pEnt = CBaseEntity::Instance(pFind);
		CSatchelCharge* pSatchel = (CSatchelCharge*)pEnt;

		if (pSatchel)
		{
			if (pSatchel->pev->owner == pOwner->edict())
			{
				pSatchel->Deactivate();
			}
		}

		pFind = FIND_ENTITY_BY_CLASSNAME(pFind, "monster_satchel");
	}
}
#endif //CLIENT_DLL check




//MODDD - why wasn't this always marked serverside-only??
#ifndef CLIENT_DLL

LINK_ENTITY_TO_CLASS( monster_satchel, CSatchelCharge );

//=========================================================
// Deactivate - do whatever it is we do to an orphaned 
// satchel when we don't want it in the world anymore.
//=========================================================
void CSatchelCharge::Deactivate( void )
{
	pev->solid = SOLID_NOT;
	UTIL_Remove( this );
}


void CSatchelCharge :: Spawn( void )
{
	Precache( );
	// motor
	pev->movetype = MOVETYPE_BOUNCE;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "models/w_satchel.mdl");
	//UTIL_SetSize(pev, Vector( -16, -16, -4), Vector(16, 16, 32));	// Old box -- size of headcrab monsters/players get blocked by this
	UTIL_SetSize(pev, Vector( -4, -4, -4), Vector(4, 4, 4));	// Uses point-sized, and can be stepped over
	UTIL_SetOrigin( pev, pev->origin );

	SetTouch( &CSatchelCharge::SatchelSlide );
	SetUse( &CGrenade::DetonateUse );
	SetThink( &CSatchelCharge::SatchelThink );
	pev->nextthink = gpGlobals->time + 0.1;

	pev->gravity = 0.5;
	pev->friction = 0.8;

	pev->dmg = gSkillData.plrDmgSatchel;
	// ResetSequenceInfo( );
	pev->sequence = 1;
}


void CSatchelCharge::SatchelSlide( CBaseEntity *pOther )
{
	entvars_t	*pevOther = pOther->pev;

	// don't hit the guy that launched this grenade
	if ( pOther->edict() == pev->owner )
		return;

	// pev->avelocity = Vector (300, 300, 300);
	pev->gravity = 1;// normal gravity now

	// HACKHACK - On ground isn't always set, so look for ground underneath
	TraceResult tr;
	UTIL_TraceLine( pev->origin, pev->origin - Vector(0,0,10), ignore_monsters, edict(), &tr );

	if ( tr.flFraction < 1.0 )
	{
		// add a bit of static friction
		pev->velocity = pev->velocity * 0.95;
		pev->avelocity = pev->avelocity * 0.9;
		// play sliding sound, volume based on velocity
	}
	if ( !(pev->flags & FL_ONGROUND) && pev->velocity.Length2D() > 10 )
	{
		BounceSound();
	}
	StudioFrameAdvance( );
}


void CSatchelCharge :: SatchelThink( void )
{
	StudioFrameAdvance( );
	pev->nextthink = gpGlobals->time + 0.1;

	if (!IsInWorld())
	{
		UTIL_Remove( this );
		return;
	}

	if (pev->waterlevel == 3)
	{
		pev->movetype = MOVETYPE_FLY;
		pev->velocity = pev->velocity * 0.8;
		pev->avelocity = pev->avelocity * 0.9;
		pev->velocity.z += 8;
	}
	else if (pev->waterlevel == 0)
	{
		pev->movetype = MOVETYPE_BOUNCE;
	}
	else
	{
		pev->velocity.z -= 8;
	}	
}

void CSatchelCharge :: Precache( void )
{
	PRECACHE_MODEL("models/grenade.mdl");
	PRECACHE_SOUND("weapons/g_bounce1.wav");
	PRECACHE_SOUND("weapons/g_bounce2.wav");
	PRECACHE_SOUND("weapons/g_bounce3.wav");
}

void CSatchelCharge :: BounceSound( void )
{
	switch ( RANDOM_LONG( 0, 2 ) )
	{
	case 0:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce1.wav", 1, ATTN_NORM);	break;
	case 1:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce2.wav", 1, ATTN_NORM);	break;
	case 2:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce3.wav", 1, ATTN_NORM);	break;
	}
}


LINK_ENTITY_TO_CLASS( weapon_satchel, CSatchel );



float CSatchelCharge::massInfluence(void){
	return 0.18f;
}
int CSatchelCharge::GetProjectileType(void){
	//do we need a custom type for remote charges like this?
	return PROJECTILE_GRENADE;
}

#endif //CLIENT_DLL check





CSatchel::CSatchel() {

	//NOTE: this used to be a custom var named "weaponRetired", but it does not seem to sync too well b/w the server & client.
	//Now reusing an existing sync'd var.
	m_fInAttack = FALSE;

}


// Save/restore for serverside only!
#ifndef CLIENT_DLL
TYPEDESCRIPTION	CSatchel::m_SaveData[] =
{
	DEFINE_FIELD(CSatchel, m_chargeReady, FIELD_INTEGER),
};
IMPLEMENT_SAVERESTORE(CSatchel, CBasePlayerWeapon);
#endif



//=========================================================
// CALLED THROUGH the newly-touched weapon's instance. The existing player weapon is pOriginal
//=========================================================
int CSatchel::AddDuplicate( CBasePlayerItem *pOriginal )
{
	CSatchel *pSatchel;

	if ( IsMultiplayer() )
	{
		pSatchel = (CSatchel *)pOriginal;

		if ( pSatchel->getchargeReady() != 0 )
		{
			// player has some satchels deployed. Refuse to add more.
			return FALSE;
		}
	}

	return CBasePlayerWeapon::AddDuplicate ( pOriginal );
}

//=========================================================
//=========================================================
int CSatchel::AddToPlayer( CBasePlayer *pPlayer )
{
	int bResult = CBasePlayerItem::AddToPlayer( pPlayer );

	pPlayer->pev->weapons |= (1<<m_iId);
	this->setchargeReady(0);// this satchel charge weapon now forgets that any satchels are deployed by it.

	if ( bResult )
	{
		return AddWeapon( );
	}
	return FALSE;
}



//MODDD
void CSatchel::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_SATCHEL", FALSE, SUIT_NEXT_IN_30MIN);
}

void CSatchel::Spawn( )
{
	Precache( );
	m_iId = WEAPON_SATCHEL;
	SET_MODEL(ENT(pev), "models/w_satchel.mdl");

	m_iDefaultAmmo = SATCHEL_DEFAULT_GIVE;
		
	FallInit();// get ready to fall down.

	m_fInAttack = FALSE;
}


void CSatchel::Precache( void )
{
	PRECACHE_MODEL("models/v_satchel.mdl");
	PRECACHE_MODEL("models/v_satchel_radio.mdl");
	PRECACHE_MODEL("models/w_satchel.mdl");
	PRECACHE_MODEL("models/p_satchel.mdl");
	PRECACHE_MODEL("models/p_satchel_radio.mdl");

	precacheGunPickupSound();
	precacheAmmoPickupSound(); //doubles as ammo.

	UTIL_PrecacheOther( "monster_satchel" );
}


int CSatchel::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Satchel Charge";
	p->iMaxAmmo1 = SATCHEL_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 1;
	p->iFlags = ITEM_FLAG_SELECTONEMPTY | ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;
	p->iId = m_iId = WEAPON_SATCHEL;
	p->iWeight = SATCHEL_WEIGHT;

	return 1;
}

//=========================================================
//=========================================================
BOOL CSatchel::IsUseable( void )
{
	if ( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] > 0 ) 
	{
		// player is carrying some satchels
		return TRUE;
	}

	if ( getchargeReady() != 0 )
	{
		// player isn't carrying any satchels, but has some out
		return TRUE;
	}

	return FALSE;
}

BOOL CSatchel::CanDeploy( void )
{
	if ( m_pPlayer->m_rgAmmo[ PrimaryAmmoIndex() ] > 0 ) 
	{
		// player is carrying some satchels
		return TRUE;
	}

	if ( getchargeReady() != 0 )
	{
		// player isn't carrying any satchels, but has some out
		return TRUE;
	}

	return FALSE;
}

BOOL CSatchel::Deploy( )
{
	m_fInAttack = FALSE;

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;
	
	//no, done below.
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );

	//easyPrintLine("DEPLOYYYYY %d", m_chargeReady);

	
	if ( getchargeReady() ){
		return DefaultDeploy( "models/v_satchel_radio.mdl", "models/p_satchel_radio.mdl", SATCHEL_RADIO_DRAW, "hive", 0, 0, (19.0/30.0), (12.0/30.0)  );
	}else{
		return DefaultDeploy( "models/v_satchel.mdl", "models/p_satchel.mdl", SATCHEL_DRAW, "trip", 0, 0, (61.0/30.0), (24.0/30.0) );
	}
	
	return TRUE;
}


void CSatchel::Holster( int skiplocal /* = 0 */ )
{

	m_fInAttack = FALSE;

	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	


	if ( !m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] && !getchargeReady() )
	{
		m_pPlayer->pev->weapons &= ~(1<<WEAPON_SATCHEL);
		SetThink( &CBasePlayerItem::DestroyItem );
		pev->nextthink = gpGlobals->time + 0.1;
	}else{
		int holsterAnimToSend;
		if ( getchargeReady() )
		{
			holsterAnimToSend = SATCHEL_RADIO_HOLSTER;
		}
		else
		{
			holsterAnimToSend = SATCHEL_DROP;
		}
	
		DefaultHolster(holsterAnimToSend, skiplocal, 0, (16.0f/30.0f));
	}


	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "common/null.wav", 1.0, ATTN_NORM);
}



void CSatchel::PrimaryAttack()
{
	switch (getchargeReady() )
	{
	case 0:
		{
		Throw( );
		}
		break;
	case 1:
		{
		SendWeaponAnim( SATCHEL_RADIO_FIRE );

		edict_t *pPlayer = m_pPlayer->edict( );

		CBaseEntity *pSatchel = NULL;

		while ((pSatchel = UTIL_FindEntityInSphere( pSatchel, m_pPlayer->pev->origin, 4096 )) != NULL)
		{
			if (FClassnameIs( pSatchel->pev, "monster_satchel"))
			{
				if (pSatchel->pev->owner == pPlayer)
				{
					pSatchel->Use( m_pPlayer, m_pPlayer, USE_ON, 0 );
					setchargeReady(2);
				}
			}
		}

		setchargeReady(2);
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;

		//MODDD - match anim time
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (31.0/30.0);
		break;
		}

	case 2:
		// we're reloading, don't allow fire
		{
		}
		break;
	}
}


void CSatchel::SecondaryAttack( void )
{
	if ( getchargeReady() != 2 )
	{
		Throw( );

		//if the cheat is not off:
		if(! (m_pPlayer->cheat_minimumfiredelayMem == 0) ){
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
			m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem + 0.03f;
		}

	}
}


void CSatchel::Throw( void )
{
	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		Vector vecSrc = m_pPlayer->pev->origin;

		Vector vecThrow = gpGlobals->v_forward * 274 + m_pPlayer->pev->velocity;

#ifndef CLIENT_DLL
		CBaseEntity *pSatchel = Create( "monster_satchel", vecSrc, Vector( 0, 0, 0), m_pPlayer->edict() );
		pSatchel->pev->velocity = vecThrow;
		pSatchel->pev->avelocity.y = 400;

		m_pPlayer->pev->viewmodel = MAKE_STRING("models/v_satchel_radio.mdl");
		m_pPlayer->pev->weaponmodel = MAKE_STRING("models/p_satchel_radio.mdl");
#else
		LoadVModel ( "models/v_satchel_radio.mdl", m_pPlayer );
#endif

		SendWeaponAnim( SATCHEL_RADIO_DRAW );
		

		//MODDD - addition.
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (19.0/30.0) + randomIdleAnimationDelay();


		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		setchargeReady(1);
		
		
		//MODDD - cheat check.			
		if(m_pPlayer->cheat_infiniteclipMem == 0 && m_pPlayer->cheat_infiniteammoMem == 0){
			m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType]--;
		}

		//MODDD
		//NOTE: Primary fire isn't affected here since this may be the first charge (holding any longer would make it blow up
		//in the player's face).
		if(m_pPlayer->cheat_minimumfiredelayMem == 0){
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
			m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;
		}else{
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.0;
			//they stick together sometimes, so they get an extra delay.
			m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem + 0.03f;
		}
		

	}
}


void CSatchel::WeaponIdle( void )
{
	int randomAnim;

	//if the weapon is retired but we pick up ammo, re-deploy it.
	/*
	if(m_fInAttack && m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] > 0){
		m_fInAttack = FALSE;
		DefaultDeploy( "models/v_satchel.mdl", "models/p_satchel.mdl", SATCHEL_DRAW, "trip", 0, 0, (99.0/30.0), (24.0/30.0) );
		return;
	}
	*/

	//MODDD - is this okay?
	if(m_pPlayer->pev->viewmodel == iStringNull){
		if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] > 0 ){

			globalflag_muteDeploySound = TRUE;
			Deploy();
			globalflag_muteDeploySound = FALSE;

			return;
		}
	}


	//easyPrintLine("WHAT THE time %.2f, %.2f", m_flTimeWeaponIdle, UTIL_WeaponTimeBase());

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	switch( getchargeReady() )
	{
	case 0:
		randomAnim = RANDOM_LONG(0, 1);

		if(randomAnim == 0){
			SendWeaponAnim( SATCHEL_IDLE1 );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 101.0/20.0 + randomIdleAnimationDelay();
		}else{
			SendWeaponAnim( SATCHEL_FIDGET1 );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 61.0/30.0 + randomIdleAnimationDelay();
		}

		
		// use tripmine animations
		strcpy( m_pPlayer->m_szAnimExtention, "trip" );
		break;
	case 1:
		randomAnim = RANDOM_LONG(0, 1);
		
		//easyPrintLine("RANDOM stuff no %d", randomAnim);
		if(randomAnim == 0){
			SendWeaponAnim( SATCHEL_RADIO_IDLE1 );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 81.0/30.0 + randomIdleAnimationDelay();
		}else{
			
			SendWeaponAnim( SATCHEL_RADIO_FIDGET1 );
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 111.0/30.0 - 0.9f + randomIdleAnimationDelay();
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 111.0/40.0 + randomIdleAnimationDelay();
		}

		// use hivehand animations
		strcpy( m_pPlayer->m_szAnimExtention, "hive" );
		break;
	case 2:
		if ( !m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
		{
			setchargeReady(0);
			RetireWeapon();
			m_fInAttack = TRUE;
			return;
		}

#ifndef CLIENT_DLL
		m_pPlayer->pev->viewmodel = MAKE_STRING("models/v_satchel.mdl");
		m_pPlayer->pev->weaponmodel = MAKE_STRING("models/p_satchel.mdl");
#else
		LoadVModel ( "models/v_satchel.mdl", m_pPlayer );
#endif

		SendWeaponAnim( SATCHEL_DRAW );

		// use tripmine animations
		strcpy( m_pPlayer->m_szAnimExtention, "trip" );

		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.5;
		setchargeReady(0);
		//MODDD - addition.
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 61.0/30.0 + randomIdleAnimationDelay();
		break;
	}

	//MODDD - no, specific to the anim that was chosen's time.
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );// how long till we do this again.
}

