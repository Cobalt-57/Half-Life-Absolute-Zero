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



//MODDDD TODO - 
/*
  1. FIRING UNDERWATER IS STILL POSSIBLE (since retail).  Should it stay this way?


  2. PROBLEM:
  If for some reason the RPG's m_cActiveRockets, as seen in "m_pLauncher->m_cActiveRockets--",
  isn't called even when the flying rocket is removed, the game will think there is still a rocket
  en route and deny letting the player reload while smart fire (laser pointer) is on.  It feels silly.

  This seems more likely to happen if firing rockets underwater and watching them for a long time when they go off.
  No idea what happens differnetly to cause not turning m_cActiveRockets down by 1 like it should.

  A more secure version of this would have an array of up to say, 10 missiles at a time
  (for safety even though 4 should be plenty), deny firing missiles if whatever max is exceeded.
  for keeping EDICTs of all missiles ever launched.
  When one goes null, that spot is opened for another rocket to take when it fires.
  The idea is, this guarantees being able to check whether or not there are any rockets still
  traveling.  If all slots are NULL, there is indeed no rocket fired left.
  
*/







#if !defined( OEM_BUILD )

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"




//extern:
extern unsigned short g_sTrailRA;
extern float global_cl_rockettrail;
extern float global_rocketSkipIgnite;


enum rpg_e {
	RPG_IDLE = 0,
	RPG_FIDGET,     // NEW COMMENT: loaded fidget.
	RPG_RELOAD,		// to reload
	RPG_FIRE2,		// to empty
	RPG_HOLSTER1,	// loaded
	RPG_DRAW1,		// loaded
	RPG_HOLSTER2,	// unloaded
	RPG_DRAW_UL,	// unloaded
	RPG_IDLE_UL,	// unloaded idle


	//MODDD NOTICE - this anim just does not exist, no anim of this index in the model.
	//               Should we make a special one.?
	//RPG_FIDGET_UL,	// unloaded fidget
};

LINK_ENTITY_TO_CLASS( weapon_rpg, CRpg );




#ifndef CLIENT_DLL

LINK_ENTITY_TO_CLASS( laser_spot, CLaserSpot );

//=========================================================
//=========================================================
CLaserSpot *CLaserSpot::CreateSpot( void )
{
	CLaserSpot *pSpot = GetClassPtr( (CLaserSpot *)NULL );
	pSpot->Spawn();

	pSpot->pev->classname = MAKE_STRING("laser_spot");

	return pSpot;
}

//=========================================================
//=========================================================
void CLaserSpot::Spawn( void )
{
	Precache( );
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_NOT;

	pev->rendermode = kRenderGlow;
	pev->renderfx = kRenderFxNoDissipation;
	pev->renderamt = 255;

	SET_MODEL(ENT(pev), "sprites/laserdot.spr");
	UTIL_SetOrigin( pev, pev->origin );
};

//=========================================================
// Suspend- make the laser sight invisible. 
//=========================================================
void CLaserSpot::Suspend( float flSuspendTime )
{
	pev->effects |= EF_NODRAW;
	
	SetThink( &CLaserSpot::Revive );
	pev->nextthink = gpGlobals->time + flSuspendTime;
}

//=========================================================
// Revive - bring a suspended laser sight back.
//=========================================================
void CLaserSpot::Revive( void )
{
	pev->effects &= ~EF_NODRAW;

	SetThink( NULL );
}

void CLaserSpot::Precache( void )
{
	PRECACHE_MODEL("sprites/laserdot.spr");
};

LINK_ENTITY_TO_CLASS( rpg_rocket, CRpgRocket );




CRpgRocket::CRpgRocket(void){
	ignited = FALSE;
	alreadyDeleted = FALSE;
}//END OF CRpgRocket constructor


//=========================================================
//=========================================================
CRpgRocket *CRpgRocket::CreateRpgRocket( Vector vecOrigin, Vector vecAngles, CBaseEntity *pOwner, CRpg *pLauncher )
{
	CRpgRocket *pRocket = GetClassPtr( (CRpgRocket *)NULL );

	UTIL_SetOrigin( pRocket->pev, vecOrigin );
	pRocket->pev->angles = vecAngles;
	pRocket->Spawn();
	pRocket->SetTouch( &CRpgRocket::RocketTouch );
	pRocket->m_pLauncher = pLauncher;// remember what RPG fired me. 
	pRocket->m_pLauncher->m_cActiveRockets++;// register this missile as active for the launcher
	pRocket->pev->owner = pOwner->edict();

	return pRocket;
}

//=========================================================
//=========================================================
void CRpgRocket :: Spawn( void )
{
	ignited = FALSE;

	Precache( );
	// motor
	pev->movetype = MOVETYPE_BOUNCE;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "models/rpgrocket.mdl");
	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin( pev, pev->origin );

	pev->classname = MAKE_STRING("rpg_rocket");

	SetThink( &CRpgRocket::IgniteThink );
	SetTouch( &CGrenade::ExplodeTouch );


	if(global_rocketSkipIgnite != 1){

		pev->angles.x -= 30;
		UTIL_MakeVectors( pev->angles );
		pev->angles.x = -(pev->angles.x + 30);

		//MODDD - come again later.
		//easyPrintLine("????????? %d", pev->owner == NULL);
		//pev->velocity = gpGlobals->v_forward * 250;
		//if(m_pLauncher && m_pLauncher->m_pPlayer){
			//pev->velocity = gpGlobals->v_forward * 250 + UTIL_GetProjectileVelocityExtra(m_pLauncher->m_pPlayer->pev->velocity, m_pLauncher->m_pPlayer->projectilesInheritPlayerVelocityMem);
			//easyPrintLine("STUFF?? %.2f, %.2f, %.2f    %.2f", m_pLauncher->m_pPlayer->pev->velocity.x, m_pLauncher->m_pPlayer->pev->velocity.y, m_pLauncher->m_pPlayer->pev->velocity.z, m_pLauncher->m_pPlayer->projectilesInheritPlayerVelocityMem);
		//}else{
			//no choice, ignore that.
			pev->velocity = gpGlobals->v_forward * 250;
		//}

		pev->gravity = 0.5;

		pev->nextthink = gpGlobals->time + 0.4;

	}else{
		//easyForcePrintLine("WHAT ARE HIS VECTORS   %.2f %.2f %.2f", -pev->angles.x, pev->angles.y, pev->angles.z);

		//pev->angles = Vector(-pev->angles.x, pev->angles.y, pev->angles.z);
		pev->angles.x = -pev->angles.x;

		UTIL_MakeVectors( pev->angles );
		//a little slower of a start, because this is focused towards the target already.
		pev->velocity = gpGlobals->v_forward * 180;
		pev->nextthink = gpGlobals->time;
	}

	

	pev->dmg = gSkillData.plrDmgRPG;
}

//=========================================================
//=========================================================
void CRpgRocket :: RocketTouch ( CBaseEntity *pOther )
{
	if( m_pLauncher ){
		// my launcher is still around, tell it I'm dead.
		m_pLauncher->m_cActiveRockets--;
		alreadyDeleted = TRUE;
	}else{
		easyForcePrintLine("CRpgRocket RED ALERT A: ROCKET LAUNCHER MISSING?! WHAT");
	}

	STOP_SOUND( edict(), CHAN_VOICE, "weapons/rocket1.wav" );
	ExplodeTouch( pOther );
}

void CRpgRocket::onDelete(){
	//Should this be deleted, at least formally, remove a rocket.

	if(!alreadyDeleted){
		easyForcePrintLine("CRpgRocket AHA! I GOT YOU");
		if( m_pLauncher ){
			// my launcher is still around, tell it I'm dead.
			m_pLauncher->m_cActiveRockets--;
		}else{
			easyForcePrintLine("CRpgRocket RED ALERT B: ROCKET LAUNCHER MISSING?! WHAT");
		}
	}
}


//=========================================================
//=========================================================
void CRpgRocket :: Precache( void )
{
	PRECACHE_MODEL("models/rpgrocket.mdl");
	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
	PRECACHE_SOUND ("weapons/rocket1.wav");
}


void CRpgRocket :: IgniteThink( void  )
{
	// pev->movetype = MOVETYPE_TOSS;

	pev->movetype = MOVETYPE_FLY;
	
	if(global_cl_rockettrail == 0){
		pev->effects |= EF_LIGHT;
	}
	ignited = TRUE;

	// make rocket sound
	EMIT_SOUND( ENT(pev), CHAN_VOICE, "weapons/rocket1.wav", 1, 0.5 );


	if(global_cl_rockettrail == 0){


		//MODDD - can we make the end of the trail disappear right at the moment of impact so it doesn't teleport to the explosion's inevitable offset away from the hit surface?
		//ACTUALLY... it may be better to make it so the explosion just happens at an offset of the rocket's hit place and the rocket itself does not teleport to that offset at all.
		//done.
		// rocket trail
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );

			WRITE_BYTE( TE_BEAMFOLLOW );
			WRITE_SHORT(entindex());	// entity
			WRITE_SHORT(m_iTrail );	// model
			WRITE_BYTE( 40 ); // life
			WRITE_BYTE( 5 );  // width
			WRITE_BYTE( 224 );   // r, g, b
			WRITE_BYTE( 224 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 255 );	// brightness

		MESSAGE_END();  // move PHS/PVS data sending into here (SEND_ALL, SEND_PVS, SEND_PHS)

	}else if(global_cl_rockettrail == 1){

		//g_sTrailRA
		PLAYBACK_EVENT_FULL (FEV_GLOBAL, this->edict(), g_sTrailRA, 0.0, 
		(float *)&this->pev->origin, (float *)&this->pev->angles, 0.0, 0.0, this->entindex(), 0, 0, 0);
	
	}



	m_flIgniteTime = gpGlobals->time;

	// set to follow laser spot
	SetThink( &CRpgRocket::FollowThink );
	pev->nextthink = gpGlobals->time + 0.1;
}


void CRpgRocket :: FollowThink( void  )
{
	CBaseEntity *pOther = NULL;
	Vector vecTarget;
	Vector vecDir;
	float flDist, flMax, flDot;
	TraceResult tr;

	UTIL_MakeAimVectors( pev->angles );

	vecTarget = gpGlobals->v_forward;
	flMax = 4096;
	
	// Examine all entities within a reasonable radius
	while ((pOther = UTIL_FindEntityByClassname( pOther, "laser_spot" )) != NULL)
	{
		UTIL_TraceLine ( pev->origin, pOther->pev->origin, dont_ignore_monsters, ENT(pev), &tr );
		// ALERT( at_console, "%f\n", tr.flFraction );
		if (tr.flFraction >= 0.90)
		{
			vecDir = pOther->pev->origin - pev->origin;
			flDist = vecDir.Length( );
			vecDir = vecDir.Normalize( );
			flDot = DotProduct( gpGlobals->v_forward, vecDir );
			if ((flDot > 0) && (flDist * (1 - flDot) < flMax))
			{
				flMax = flDist * (1 - flDot);
				vecTarget = vecDir;
			}
		}
	}

	pev->angles = UTIL_VecToAngles( vecTarget );

	// this acceleration and turning math is totally wrong, but it seems to respond well so don't change it.
	float flSpeed = pev->velocity.Length();
	if (gpGlobals->time - m_flIgniteTime < 1.0)
	{
		pev->velocity = pev->velocity * 0.2 + vecTarget * (flSpeed * 0.8 + 400);
		if (pev->waterlevel == 3)
		{
			// go slow underwater
			if (pev->velocity.Length() > 300)
			{
				pev->velocity = pev->velocity.Normalize() * 300;
			}
			UTIL_BubbleTrail( pev->origin - pev->velocity * 0.1, pev->origin, 4 );
		} 
		else 
		{
			if (pev->velocity.Length() > 2000)
			{
				pev->velocity = pev->velocity.Normalize() * 2000;
			}
		}
	}
	else
	{
		//MODDD - NOTE: the alpha trail does not use this light, so that is not a good indication.  Use the var "ignited" instead.
		//if (pev->effects & EF_LIGHT)
		if(ignited)
		{
			if(global_cl_rockettrail == 0){
				pev->effects = 0;
			}
			ignited = FALSE;
			STOP_SOUND( ENT(pev), CHAN_VOICE, "weapons/rocket1.wav" );
		}
		pev->velocity = pev->velocity * 0.2 + vecTarget * flSpeed * 0.798;
		if (pev->waterlevel == 0 && pev->velocity.Length() < 1500)
		{
			Detonate( );
		}
	}
	// ALERT( at_console, "%.0f\n", flSpeed );

	pev->nextthink = gpGlobals->time + 0.1;
}
#endif



void CRpg::Reload( void )
{
	int iResult;

	if ( m_iClip == 1 )
	{
		// don't bother with any of this if don't need to reload.
		return;
	}

	if ( m_pPlayer->ammo_rockets <= 0 )
		return;

	// because the RPG waits to autoreload when no missiles are active while  the LTD is on, the
	// weapons code is constantly calling into this function, but is often denied because 
	// a) missiles are in flight, but the LTD is on
	// or
	// b) player is totally out of ammo and has nothing to switch to, and should be allowed to
	//    shine the designator around
	//
	// Set the next attack time into the future so that WeaponIdle will get called more often
	// than reload, allowing the RPG LTD to be updated
	
	m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;

	if ( m_cActiveRockets && m_fSpotActive )
	{
		// no reloading when there are active missiles tracking the designator.
		// ward off future autoreload attempts by setting next attack time into the future for a bit. 
		return;
	}

#ifndef CLIENT_DLL
	if ( m_pSpot && m_fSpotActive )
	{
		m_pSpot->Suspend( 2.1 );
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 2.1;
	}
#endif

	if ( m_iClip == 0 )
		//iResult = DefaultReload( RPG_MAX_CLIP, RPG_RELOAD, 2 );
		iResult = DefaultReload( RPG_MAX_CLIP, RPG_RELOAD, (61.0/30.0) );
	

	//MODDD - Not necessary.  "DefaultReload" now gives the correct idle time (length of anim)
	//if ( iResult )
	//	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
	
}

//MODDD
void CRpg::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_RPG", FALSE, SUIT_NEXT_IN_30MIN);
}

void CRpg::Spawn( )
{
	Precache( );
	m_iId = WEAPON_RPG;

	SET_MODEL(ENT(pev), "models/w_rpg.mdl");
	m_fSpotActive = 1;

#ifdef CLIENT_DLL
	if ( bIsMultiplayer() )
#else
	if ( g_pGameRules->IsMultiplayer() )
#endif
	{
		// more default ammo in multiplay. 
		m_iDefaultAmmo = RPG_DEFAULT_GIVE * 2;
	}
	else
	{
		m_iDefaultAmmo = RPG_DEFAULT_GIVE;
	}

	FallInit();// get ready to fall down.
}


void CRpg::Precache( void )
{
	PRECACHE_MODEL("models/w_rpg.mdl");
	PRECACHE_MODEL("models/v_rpg.mdl");
	PRECACHE_MODEL("models/p_rpg.mdl");

	PRECACHE_SOUND("items/9mmclip1.wav");

	UTIL_PrecacheOther( "laser_spot" );
	UTIL_PrecacheOther( "rpg_rocket" );

	PRECACHE_SOUND("weapons/rocketfire1.wav");
	PRECACHE_SOUND("weapons/glauncher.wav"); // alternative fire sound

	precacheGunPickupSound();

	m_usRpg = PRECACHE_EVENT ( 1, "events/rpg.sc" );
}


int CRpg::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "rockets";
	p->iMaxAmmo1 = ROCKET_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = RPG_MAX_CLIP;

	//MODDD - used to be...
	/*
	p->iSlot = 3;
	p->iPosition = 0;
	*/
	p->iSlot = 2;
	p->iPosition = 3;

	p->iId = m_iId = WEAPON_RPG;
	p->iFlags = 0;
	p->iWeight = RPG_WEIGHT;

	return 1;
}

int CRpg::AddToPlayer( CBasePlayer *pPlayer )
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

BOOL CRpg::Deploy( )
{

	if ( m_iClip == 0 )
	{
		return DefaultDeploy( "models/v_rpg.mdl", "models/p_rpg.mdl", RPG_DRAW_UL, "rpg", 0, 0, (16.0/30.0), -1 );
	}

	return DefaultDeploy( "models/v_rpg.mdl", "models/p_rpg.mdl", RPG_DRAW1, "rpg", 0, 0, (16.0/30.0), -1 );
}



//MODDD - verify.  Is  CanHolster even obeyed?
BOOL CRpg::CanHolster( void )
{
	if ( m_fSpotActive && m_cActiveRockets )
	{
		// can't put away while guiding a missile.
		return FALSE;
	}

	return TRUE;
}

void CRpg::Holster( int skiplocal /* = 0 */ )
{
	int holsterAnimToSend;
	m_fInReload = FALSE;// cancel any reload in progress.

	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	//SendWeaponAnim( RPG_HOLSTER1 );


#ifndef CLIENT_DLL
	if (m_pSpot)
	{
		m_pSpot->Killed( NULL, GIB_NEVER );
		m_pSpot = NULL;
	}
#endif


	//MODDD - also going to involve whether the RPG is loaded while holstered (HOLSTER1) or not (HOLSTER2).
	//
	if(m_iClip){
		holsterAnimToSend = RPG_HOLSTER1;
	}else{
		holsterAnimToSend = RPG_HOLSTER2;
	}
	
	DefaultHolster(holsterAnimToSend, skiplocal, 0, (16.0f/30.0f));


}//END OF Holster



void CRpg::PrimaryAttack()
{
	if ( m_iClip )
	{
		m_pPlayer->m_iWeaponVolume = LOUD_GUN_VOLUME;
		m_pPlayer->m_iWeaponFlash = BRIGHT_GUN_FLASH;

#ifndef CLIENT_DLL
		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		Vector vecSrc = m_pPlayer->GetGunPosition( ) + gpGlobals->v_forward * 16 + gpGlobals->v_right * 8 + gpGlobals->v_up * -8;
		
		CRpgRocket *pRocket = CRpgRocket::CreateRpgRocket( vecSrc, m_pPlayer->pev->v_angle, m_pPlayer, this );

		UTIL_MakeVectors( m_pPlayer->pev->v_angle );// RpgRocket::Create stomps on globals, so remake.
		pRocket->pev->velocity = pRocket->pev->velocity + gpGlobals->v_forward * DotProduct( m_pPlayer->pev->velocity, gpGlobals->v_forward );
#endif

		// firing RPG no longer turns on the designator. ALT fire is a toggle switch for the LTD.
		// Ken signed up for this as a global change (sjb)

		int flags;
#if defined( CLIENT_WEAPONS )
	flags = FEV_NOTHOST;
#else
	flags = 0;
#endif

		PLAYBACK_EVENT( flags, m_pPlayer->edict(), m_usRpg );

		//MODDD
		if(m_pPlayer->cheat_infiniteclipMem == 0){
			m_iClip--; 
		}
				
		if(m_pPlayer->cheat_minimumfiredelayMem == 0){
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 1.5;
		}else{
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
		}
		
		//MODDD
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (25.0/30.0) + randomIdleAnimationDelay();
	}
	else
	{

		if(m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] <= 0){
			//only do the empty click if we can't reload.
			PlayEmptySound();
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;  //MODDD - this is all it takes to block sound spam. Come on man.
		}
		
	}
	UpdateSpot( );
}


void CRpg::SecondaryAttack()
{
	m_fSpotActive = ! m_fSpotActive;

#ifndef CLIENT_DLL
	if (!m_fSpotActive && m_pSpot)
	{
		m_pSpot->Killed( NULL, GIB_NORMAL );
		m_pSpot = NULL;
	}
#endif

	m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.2;
}


void CRpg::WeaponIdle( void )
{
	UpdateSpot( );

	ResetEmptySound( );

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	//MODDD - requirement for idle anim removed.
	//if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType])
	{
		int iAnim;
		float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
		if (flRand <= 0.75 || m_fSpotActive)
		{
			if ( m_iClip == 0 )
				iAnim = RPG_IDLE_UL;
			else
				iAnim = RPG_IDLE;

			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0 / 15.0 + randomIdleAnimationDelay();
		}
		else
		{

			/*
			//MODDD - a RPG_FIDGET_UL sequence (that number) was never provided in the model, nor for ours as of now. Why was it called anyways?
			if ( m_iClip == 0 )
				iAnim = RPG_FIDGET_UL;
			else
				iAnim = RPG_FIDGET;
			*/

			if ( m_iClip == 0 )
				iAnim = RPG_FIDGET;
			else
				iAnim = RPG_FIDGET;    //Or pick RPG_IDLE instead if unloaded?




			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0 / 15.0 + randomIdleAnimationDelay();// + 3.0;
		}

		SendWeaponAnim( iAnim );
	}
	//else
	//{
	//	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1;
	//}
}



void CRpg::UpdateSpot( void )
{

#ifndef CLIENT_DLL
	if (m_fSpotActive)
	{
		if (!m_pSpot)
		{
			m_pSpot = CLaserSpot::CreateSpot();
		}

		UTIL_MakeVectors( m_pPlayer->pev->v_angle );
		Vector vecSrc = m_pPlayer->GetGunPosition( );;
		Vector vecAiming = gpGlobals->v_forward;

		TraceResult tr;
		UTIL_TraceLine ( vecSrc, vecSrc + vecAiming * 8192, dont_ignore_monsters, ENT(m_pPlayer->pev), &tr );
		
		UTIL_SetOrigin( m_pSpot->pev, tr.vecEndPos );
	}
#endif

}


class CRpgAmmo : public CBasePlayerAmmo
{
	void Spawn( void )
	{ 
		Precache( );
		SET_MODEL(ENT(pev), "models/w_rpgammo.mdl");
		CBasePlayerAmmo::Spawn( );
	}
	void Precache( void )
	{
		PRECACHE_MODEL ("models/w_rpgammo.mdl");
		precacheAmmoPickupSound();
	}
	BOOL AddAmmo( CBaseEntity *pOther ) 
	{ 
		int iGive;

#ifdef CLIENT_DLL
	if ( bIsMultiplayer() )
#else
	if ( g_pGameRules->IsMultiplayer() )
#endif
		{
			// hand out more ammo per rocket in multiplayer.
			iGive = AMMO_RPGCLIP_GIVE * 2;
		}
		else
		{
			iGive = AMMO_RPGCLIP_GIVE;
		}

		if (pOther->GiveAmmo( iGive, "rockets", ROCKET_MAX_CARRY ) != -1)
		{
			playAmmoPickupSound();

			//MODDD
			if(pOther->IsPlayer()){
				CBasePlayer* pPlayer = (CBasePlayer*)pOther;
				pPlayer->SetSuitUpdate("!HEV_RPGAMMO", FALSE, SUIT_NEXT_IN_20MIN);
			}

			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS( ammo_rpgclip, CRpgAmmo );

#endif