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
/*

===== generic grenade.cpp ========================================================

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "soundent.h"
#include "decals.h"


//===================grenade


//MODDD
#define ROCKET_TRAIL 2
extern unsigned short g_sTrail;
extern unsigned short g_sTrailRA;

extern float global_cl_explosion;
extern float global_explosionDebrisSoundVolume;
extern float global_cheat_touchNeverExplodes;

extern float global_trailTypeTest;


LINK_ENTITY_TO_CLASS( grenade, CGrenade );

// Grenades flagged with this will be triggered when the owner calls detonateSatchelCharges
#define SF_DETONATE		0x0001

//MODDD - extern
EASY_CVAR_EXTERN(handGrenadesUseOldBounceSound)



	
GENERATE_TRACEATTACK_IMPLEMENTATION(CGrenade)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CGrenade)
{
	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}














//
// Grenade Explode
//
//MODDD - method changed, parameters removed.  These aren't even used at all.
/*
void CGrenade::Explode( Vector vecSrc, Vector vecAim )
{
	TraceResult tr;
	UTIL_TraceLine ( pev->origin, pev->origin + Vector ( 0, 0, -32 ),  ignore_monsters, ENT(pev), & tr);

	Explode( &tr, DMG_BLAST );
}
*/
void CGrenade::Explode()
{
	TraceResult tr;
	UTIL_TraceLine ( pev->origin, pev->origin + Vector ( 0, 0, -32 ),  ignore_monsters, ENT(pev), & tr);

	//CGrenade::   ???
	Explode( &tr, DMG_BLAST, 0, 1 );
}



extern unsigned short g_quakeExplosionEffect;

extern unsigned short model_explosivestuff;

// UNDONE: temporary scorching for PreAlpha - find a less sleazy permenant solution.




void CGrenade::Explode( TraceResult *pTrace, int bitsDamageType ){
	CGrenade::Explode(pTrace, bitsDamageType, 0, 1);
}
void CGrenade::Explode( TraceResult *pTrace, int bitsDamageType, int bitsDamageTypeMod ){
	CGrenade::Explode(pTrace, bitsDamageType, bitsDamageTypeMod, 1);
}


//MODDD TODO - any other logic need the used-to-be modified origin (now "explosionOrigin" to be separate)? Then make explosionOrigin an instance var and read that after this... and trust its been set,
//or defaults to pev->origin if not.

void CGrenade::Explode( TraceResult *pTrace, int bitsDamageType, int bitsDamageTypeMod, float shrapMod )
{
	float		flRndSound;// sound randomizer

	pev->model = iStringNull;//invisible
	pev->solid = SOLID_NOT;// intangible

	pev->takedamage = DAMAGE_NO;

	//MODDD - new
	Vector explosionEffectStart = pev->origin;

	//also retail's origin.
	Vector explosionOrigin = pev->origin; //by default.

	// Pull out of the wall a bit
	if ( pTrace->flFraction != 1.0 )
	{

		//MODDD - let's not change our own origin, just record this.
		//pev->origin = ...
		explosionOrigin = pTrace->vecEndPos + (pTrace->vecPlaneNormal * (pev->dmg - 24) * 0.6);;

		//MODDD - actually used for placing the quake explosion effect, if it is called for instead.
		explosionEffectStart = pTrace->vecEndPos + (pTrace->vecPlaneNormal * 5);

	}

	//is this change from pev->origin to explosionOrigin ok?
	int iContents = UTIL_PointContents ( explosionOrigin );
	short spriteChosen;
	if (iContents != CONTENTS_WATER)
	{
		spriteChosen = g_sModelIndexFireball;
	}
	else
	{
		spriteChosen = g_sModelIndexWExplosion;
	}

	
	//MODDD - condensed that a bit. Also sending the "explosionOrigin in place of our pev->origin.
	UTIL_Explosion(pev, explosionOrigin, spriteChosen, (pev->dmg - 50) * 0.60, 15, TE_EXPLFLAG_NONE, explosionEffectStart, shrapMod );
	



	/*
	//MODDD - call just like breakmodel.
#define FTENT_SMOKETRAIL		0x00000010
#define	FTENT_SLOWGRAVITY		0x00000008
	int cFlag = BREAK_METAL | FTENT_SMOKETRAIL; //|  FTENT_SLOWGRAVITY;

	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, (float *)&pev->origin );
		WRITE_BYTE( TE_BREAKMODEL);

		// position
		WRITE_COORD( explosionEffectStart.x );
		WRITE_COORD( explosionEffectStart.y );
		WRITE_COORD( explosionEffectStart.z );

		// size
		WRITE_COORD( 1);
		WRITE_COORD( 1);
		WRITE_COORD( 1);

		// velocity
		WRITE_COORD( 0 ); 
		WRITE_COORD( 0 );
		WRITE_COORD( 0 );

		// randomization
		WRITE_BYTE( 24 ); 

		// Model
		WRITE_SHORT( model_explosivestuff );	//model id#

		// # of shards
		WRITE_BYTE( 26 );

		// duration
		WRITE_BYTE( 25 );// 2.5 seconds

		// flags
		WRITE_BYTE( cFlag );
	MESSAGE_END();
	*/

	/*
	MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_LAVASPLASH);
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
		MESSAGE_END();
	*/

	CSoundEnt::InsertSound ( bits_SOUND_COMBAT, explosionOrigin, NORMAL_EXPLOSION_VOLUME, 3.0 );
	entvars_t *pevOwner;
	if ( pev->owner )
		pevOwner = VARS( pev->owner );
	else
		pevOwner = NULL;

	pev->owner = NULL; // can't traceline attack owner if this is set

	//MODDD - sending explosionOrigin instead of defaulting to pev->origin.
	//RadiusDamageAutoRadius ( explosionOrigin, pev, pevOwner, pev->dmg, CLASS_NONE, bitsDamageType, bitsDamageTypeMod );
	RadiusDamage( explosionOrigin, pev, pevOwner, pev->dmg, pev->dmg * 2.5, CLASS_NONE, bitsDamageType, bitsDamageTypeMod );
	
	
	


	if ( RANDOM_FLOAT( 0 , 1 ) < 0.5 )
	{
		UTIL_DecalTrace( pTrace, DECAL_SCORCH1 );
	}
	else
	{
		UTIL_DecalTrace( pTrace, DECAL_SCORCH2 );
	}

	flRndSound = RANDOM_FLOAT( 0 , 1 );


	
	//randDebrisSound = 1;
	//easyPrintLine("DEBRIS SOUND: %d", randDebrisSound);

	if(global_explosionDebrisSoundVolume > 0){
		int randDebrisSound = RANDOM_LONG(0, 2);

		float debrisVolumeChoice = clamp(global_explosionDebrisSoundVolume, 0, 1);

		switch ( randDebrisSound )
		{
			//NOTE: volume, the argument after the string-path, used to be 0.55.  Now 0.78.
			case 0:	EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "weapons/debris1.wav", debrisVolumeChoice, ATTN_NORM, 0, 84, FALSE);	break;
			case 1:	EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "weapons/debris2.wav", debrisVolumeChoice, ATTN_NORM, 0, 84, FALSE);	break;
			case 2:	EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "weapons/debris3.wav", debrisVolumeChoice, ATTN_NORM, 0, 84, FALSE);	break;
		}
	}
	

	pev->effects |= EF_NODRAW;
	SetThink( &CGrenade::Smoke );
	pev->velocity = g_vecZero;
	pev->nextthink = gpGlobals->time + 0.3;

	//MODDD - only generate sparks if allowed.
	if(UTIL_getExplosionsHaveSparks() ){
		if (iContents != CONTENTS_WATER)
		{
			int sparkCount = RANDOM_LONG(0,3);
			for ( int i = 0; i < sparkCount; i++ )
				Create( "spark_shower", explosionOrigin, pTrace->vecPlaneNormal, NULL );
		}
	}
	
	
	
}


void CGrenade::Smoke( void )
{
	//MODDD - smoke removed.  "return" terminates this method early.
	if(global_cl_explosion == 1){
		//does not smoke.
		UTIL_Remove( this );
		return;
	}

	if (UTIL_PointContents ( pev->origin ) == CONTENTS_WATER)
	{
		UTIL_Bubbles( pev->origin - Vector( 64, 64, 64 ), pev->origin + Vector( 64, 64, 64 ), 100 );
	}
	else
	{
	/*
		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
			WRITE_BYTE( TE_SMOKE );
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_SHORT( g_sModelIndexSmoke );
			WRITE_BYTE( (pev->dmg - 50) * 0.80 ); // scale * 10
			WRITE_BYTE( 12  ); // framerate
		MESSAGE_END();
		*/
	}
	UTIL_Remove( this );
}

GENERATE_KILLED_IMPLEMENTATION(CGrenade)
{
	Detonate( );
}

BOOL CGrenade::isOrganic(void){
	//typically not.
	return FALSE;
}

//I do not.  Player weapons don't.
BOOL CGrenade::usesSoundSentenceSave(void){
	return FALSE;
}


// Timed grenade, this think is called when time runs out.
void CGrenade::DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	SetThink( &CGrenade::Detonate );
	pev->nextthink = gpGlobals->time;
}

void CGrenade::PreDetonate( void )
{
	CSoundEnt::InsertSound ( bits_SOUND_DANGER, pev->origin, 400, 0.3 );

	SetThink( &CGrenade::Detonate );
	pev->nextthink = gpGlobals->time + 1;
}


void CGrenade::Detonate( void )
{
	TraceResult tr;
	Vector		vecSpot;// trace starts here!

	vecSpot = pev->origin + Vector ( 0 , 0 , 8 );
	UTIL_TraceLine ( vecSpot, vecSpot + Vector ( 0, 0, -40 ),  ignore_monsters, ENT(pev), & tr);

	Explode( &tr, DMG_BLAST );
}


//
// Contact grenade, explode when it touches something
// 
void CGrenade::ExplodeTouch( CBaseEntity *pOther )
{
	if(global_cheat_touchNeverExplodes != 1){

		TraceResult tr;
		Vector		vecSpot;// trace starts here!

		pev->enemy = pOther->edict();

		//MODDD - just put the explosion exactly at the impact point for now (or maybe just less of an extreme later)
		//REVERTED, not really the issue.
		vecSpot = pev->origin - pev->velocity.Normalize() * 32;
		UTIL_TraceLine( vecSpot, vecSpot + pev->velocity.Normalize() * 64, ignore_monsters, ENT(pev), &tr );
		
		//attempted improvement anyhow.
		//vecSpot = pev->origin + pev->velocity.Normalize() * -10;
		//Vector vecSpotEnd = pev->origin + pev->velocity.Normalize() * 10;
		//UTIL_TraceLine( vecSpot, vecSpotEnd, ignore_monsters, ENT(pev), &tr );
		

		Explode( &tr, DMG_BLAST );
		
		
		//easyPrintLine("global_cheat_touchNeverExplodes what??");

	}//END OF CVar check...
	
}


void CGrenade::DangerSoundThink( void )
{
	if (!IsInWorld())
	{
		UTIL_Remove( this );
		return;
	}

	CSoundEnt::InsertSound ( bits_SOUND_DANGER, pev->origin + pev->velocity * 0.5, pev->velocity.Length( ), 0.2 );
	pev->nextthink = gpGlobals->time + 0.2;

	if (pev->waterlevel != 0)
	{
		pev->velocity = pev->velocity * 0.5;
	}
}


void CGrenade::BounceTouch( CBaseEntity *pOther )
{
	// don't hit the guy that launched this grenade
	if ( pOther->edict() == pev->owner )
		return;

	// only do damage if we're moving fairly fast
	if (m_flNextAttack < gpGlobals->time && pev->velocity.Length() > 100)
	{
		entvars_t *pevOwner = VARS( pev->owner );
		if (pevOwner)
		{
			TraceResult tr = UTIL_GetGlobalTrace( );
			ClearMultiDamage( );
			pOther->TraceAttack(pevOwner, 1, gpGlobals->v_forward, &tr, DMG_CLUB ); 
			ApplyMultiDamage( pev, pevOwner);
		}
		m_flNextAttack = gpGlobals->time + 1.0; // debounce
	}

	Vector vecTestVelocity;
	// pev->avelocity = Vector (300, 300, 300);

	// this is my heuristic for modulating the grenade velocity because grenades dropped purely vertical
	// or thrown very far tend to slow down too quickly for me to always catch just by testing velocity. 
	// trimming the Z velocity a bit seems to help quite a bit.
	vecTestVelocity = pev->velocity; 
	vecTestVelocity.z *= 0.45;

	if ( !m_fRegisteredSound && vecTestVelocity.Length() <= 60 )
	{
		//ALERT( at_console, "Grenade Registered!: %f\n", vecTestVelocity.Length() );

		// grenade is moving really slow. It's probably very close to where it will ultimately stop moving. 
		// go ahead and emit the danger sound.
		
		// register a radius louder than the explosion, so we make sure everyone gets out of the way
		CSoundEnt::InsertSound ( bits_SOUND_DANGER, pev->origin, pev->dmg / 0.4, 0.3 );
		m_fRegisteredSound = TRUE;
	}

	if (pev->flags & FL_ONGROUND)
	{
		// add a bit of static friction
		pev->velocity = pev->velocity * 0.8;

		pev->sequence = RANDOM_LONG( 1, 1 );
	}
	else
	{
		// play bounce sound
		BounceSound();
	}
	pev->framerate = pev->velocity.Length() / 200.0;
	if (pev->framerate > 1.0)
		pev->framerate = 1;
	else if (pev->framerate < 0.5)
		pev->framerate = 0;

}



void CGrenade::SlideTouch( CBaseEntity *pOther )
{
	// don't hit the guy that launched this grenade
	if ( pOther->edict() == pev->owner )
		return;

	// pev->avelocity = Vector (300, 300, 300);

	if (pev->flags & FL_ONGROUND)
	{
		// add a bit of static friction
		pev->velocity = pev->velocity * 0.95;

		if (pev->velocity.x != 0 || pev->velocity.y != 0)
		{
			// maintain sliding sound
		}
	}
	else
	{
		BounceSound();
	}
}

void CGrenade :: BounceSound( void )
{
	//MODDD - refer to CVar.
	if(EASY_CVAR_GET(handGrenadesUseOldBounceSound) != 1){
		switch ( RANDOM_LONG( 0, 2 ) )
		{
		case 0:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit1.wav", 0.25, ATTN_NORM);	break;
		case 1:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit2.wav", 0.25, ATTN_NORM);	break;
		case 2:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/grenade_hit3.wav", 0.25, ATTN_NORM);	break;
		}
	}else{
		//Using the old sound if the CVar is 1.  copied from the sachel.
		switch ( RANDOM_LONG( 0, 2 ) )
		{
		case 0:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce1.wav", 1, ATTN_NORM);	break;
		case 1:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce2.wav", 1, ATTN_NORM);	break;
		case 2:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "weapons/g_bounce3.wav", 1, ATTN_NORM);	break;
		}
	}

}

void CGrenade :: TumbleThink( void )
{
	if (!IsInWorld())
	{
		UTIL_Remove( this );
		return;
	}

	StudioFrameAdvance( );
	pev->nextthink = gpGlobals->time + 0.1;

	if (pev->dmgtime - 1 < gpGlobals->time)
	{
		CSoundEnt::InsertSound ( bits_SOUND_DANGER, pev->origin + pev->velocity * (pev->dmgtime - gpGlobals->time), 400, 0.1 );
	}

	if (pev->dmgtime <= gpGlobals->time)
	{
		SetThink( &CGrenade::Detonate );
	}
	if (pev->waterlevel != 0)
	{
		pev->velocity = pev->velocity * 0.5;
		pev->framerate = 0.2;
	}
}

/*
//MODDD - added.
//Actually, not necessary.
void CGrenade:: Activate( void ){
	
	CBaseMonster::Activate();
	//Just a bridge for the heirarchy.  Other grenades may implement "Activate".
}
*/

void CGrenade:: Spawn( void )
{
	pev->movetype = MOVETYPE_BOUNCE;
	pev->classname = MAKE_STRING( "grenade" );
	
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "models/grenade.mdl");
	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));

	pev->dmg = 100;
	m_fRegisteredSound = FALSE;
}


CGrenade *CGrenade::ShootContact( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity )
{


	//MODDD NOTE - call "Spawn" on something NULL.. this works.   W H A T
	//             oh, GetClassPtr calls CREATE_ENTITY (engine method) to put an entity in the game. So it just isn't linked to some class besides CBaseEntity.
	CGrenade *pGrenade = GetClassPtr( (CGrenade *)NULL );
	pGrenade->Spawn();
	// contact grenades arc lower
	pGrenade->pev->gravity = 0.5;// lower gravity since grenade is aerodynamic and engine doesn't know it.
	UTIL_SetOrigin( pGrenade->pev, vecStart );
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = UTIL_VecToAngles (pGrenade->pev->velocity);
	pGrenade->pev->owner = ENT(pevOwner);
	
	// make monsters afaid of it while in the air
	pGrenade->SetThink( &CGrenade::DangerSoundThink );
	pGrenade->pev->nextthink = gpGlobals->time;
	
	// Tumble in air
	pGrenade->pev->avelocity.x = RANDOM_FLOAT ( -100, -500 );
	
	// Explode on contact
	pGrenade->SetTouch( &CGrenade::ExplodeTouch );

	pGrenade->pev->dmg = gSkillData.plrDmgM203Grenade;





	//MODDD - ?
	//For a reference.
	//PLAYBACK_EVENT_FULL( flags, m_pPlayer->edict(), fUseAutoAim ? m_usFireGlock1 : m_usFireGlock2, 0.0, (float *)&g_vecZero, (float *)&g_vecZero, vecDir.x, vecDir.y, m_fInAttack, 0, ( m_iClip == 0 ) ? 1 : 0, 0 );
	


	if(global_trailTypeTest > -1){
		//This was just for a test.  Enable (along with some other things in place), and this should make mp5 grenades fly with a trail of grey dots.
		PLAYBACK_EVENT_FULL (FEV_GLOBAL, pGrenade->edict(), g_sTrail, 0.0, 
		(float *)&pGrenade->pev->origin, (float *)&pGrenade->pev->angles, 0.7, 0.0, pGrenade->entindex(), (int)global_trailTypeTest, 0, 0);
	}else if(global_trailTypeTest == -2){
		PLAYBACK_EVENT_FULL (FEV_GLOBAL, pGrenade->edict(), g_sTrailRA, 0.0, 
		(float *)&pGrenade->pev->origin, (float *)&pGrenade->pev->angles, 0.7, 0.0, pGrenade->entindex(), 0, 0, 0);
	
	}



	return pGrenade;
}


CGrenade * CGrenade:: ShootTimed( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time )
{
	CGrenade *pGrenade = GetClassPtr( (CGrenade *)NULL );
	pGrenade->Spawn();
	UTIL_SetOrigin( pGrenade->pev, vecStart );
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = UTIL_VecToAngles(pGrenade->pev->velocity);
	pGrenade->pev->owner = ENT(pevOwner);
	
	pGrenade->SetTouch( &CGrenade::BounceTouch );	// Bounce if touched
	
	// Take one second off of the desired detonation time and set the think to PreDetonate. PreDetonate
	// will insert a DANGER sound into the world sound list and delay detonation for one second so that 
	// the grenade explodes after the exact amount of time specified in the call to ShootTimed(). 

	pGrenade->pev->dmgtime = gpGlobals->time + time;
	pGrenade->SetThink( &CGrenade::TumbleThink );
	pGrenade->pev->nextthink = gpGlobals->time + 0.1;
	if (time < 0.1)
	{
		pGrenade->pev->nextthink = gpGlobals->time;
		pGrenade->pev->velocity = Vector( 0, 0, 0 );
	}
		
	pGrenade->pev->sequence = RANDOM_LONG( 3, 6 );
	pGrenade->pev->framerate = 1.0;

	// Tumble through the air
	// pGrenade->pev->avelocity.x = -400;

	pGrenade->pev->gravity = 0.5;
	pGrenade->pev->friction = 0.8;

	SET_MODEL(ENT(pGrenade->pev), "models/w_grenade.mdl");
	pGrenade->pev->dmg = 100;

	return pGrenade;
}


CGrenade * CGrenade :: ShootSatchelCharge( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity )
{
	CGrenade *pGrenade = GetClassPtr( (CGrenade *)NULL );
	pGrenade->pev->movetype = MOVETYPE_BOUNCE;
	pGrenade->pev->classname = MAKE_STRING( "grenade" );
	
	pGrenade->pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pGrenade->pev), "models/grenade.mdl");	// Change this to satchel charge model

	UTIL_SetSize(pGrenade->pev, Vector( 0, 0, 0), Vector(0, 0, 0));

	pGrenade->pev->dmg = 200;
	UTIL_SetOrigin( pGrenade->pev, vecStart );
	pGrenade->pev->velocity = vecVelocity;
	pGrenade->pev->angles = g_vecZero;
	pGrenade->pev->owner = ENT(pevOwner);
	
	// Detonate in "time" seconds
	pGrenade->SetThink( &CBaseEntity::SUB_DoNothing );
	pGrenade->SetUse( &CGrenade::DetonateUse );
	pGrenade->SetTouch( &CGrenade::SlideTouch );
	pGrenade->pev->spawnflags = SF_DETONATE;

	pGrenade->pev->friction = 0.9;

	return pGrenade;
}



void CGrenade :: UseSatchelCharges( entvars_t *pevOwner, SATCHELCODE code )
{
	edict_t *pentFind;
	edict_t *pentOwner;

	if ( !pevOwner )
		return;

	CBaseEntity	*pOwner = CBaseEntity::Instance( pevOwner );

	pentOwner = pOwner->edict();

	pentFind = FIND_ENTITY_BY_CLASSNAME( NULL, "grenade" );
	while ( !FNullEnt( pentFind ) )
	{
		CBaseEntity *pEnt = Instance( pentFind );
		if ( pEnt )
		{
			if ( FBitSet( pEnt->pev->spawnflags, SF_DETONATE ) && pEnt->pev->owner == pentOwner )
			{
				if ( code == SATCHEL_DETONATE )
					pEnt->Use( pOwner, pOwner, USE_ON, 0 );
				else	// SATCHEL_RELEASE
					pEnt->pev->owner = NULL;
			}
		}
		pentFind = FIND_ENTITY_BY_CLASSNAME( pentFind, "grenade" );
	}
}



float CGrenade::massInfluence(void){
	return 0.18f;
}



//======================end grenade

