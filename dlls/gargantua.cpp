/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef OEM_BUILD

//=========================================================
// Gargantua
//=========================================================
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"nodes.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"customentity.h"
#include	"weapons.h"
#include	"effects.h"
#include	"soundent.h"
#include	"decals.h"
#include	"explode.h"
#include	"func_break.h"

//=========================================================
// Gargantua Monster
//=========================================================
const float GARG_ATTACKDIST = 80.0;

// Garg animation events
#define GARG_AE_SLASH_LEFT			1
//#define GARG_AE_BEAM_ATTACK_RIGHT	2		// No longer used
#define GARG_AE_LEFT_FOOT			3
#define GARG_AE_RIGHT_FOOT			4
#define GARG_AE_STOMP				5
#define GARG_AE_BREATHE				6


// Gargantua is immune to any damage but this
#define GARG_DAMAGE					(DMG_ENERGYBEAM|DMG_CRUSH|DMG_MORTAR|DMG_BLAST)
#define GARG_EYE_SPRITE_NAME		"sprites/gargeye1.spr"
#define GARG_BEAM_SPRITE_NAME		"sprites/xbeam3.spr"
#define GARG_BEAM_SPRITE2			"sprites/xbeam3.spr"
#define GARG_STOMP_SPRITE_NAME		"sprites/gargeye1.spr"
#define GARG_STOMP_BUZZ_SOUND		"weapons/mine_charge.wav"
#define GARG_FLAME_LENGTH			330
#define GARG_GIB_MODEL				"models/metalplategibs.mdl"

#define ATTN_GARG					(ATTN_NORM)

#define STOMP_SPRITE_COUNT			10

int gStompSprite = 0, gGargGibModel = 0;
void SpawnExplosion( Vector center, float randomRange, float time, int magnitude );

class CSmoker;

//MODDD - extern
extern float global_drawCollisionBoundsAtDeath;
extern float global_drawHitBoundsAtDeath;

extern float global_thoroughHitBoxUpdates;
extern float global_thatWasntPunch;
extern float global_gargantuaPrintout;
extern float global_gargantuaCorpseDeath;
extern float global_gargantuaFallSound;
extern float global_gargantuaBleeds;

extern float global_animationKilledBoundsRemoval;
extern float global_gargantuaKilledBoundsAssist;

EASY_CVAR_EXTERN(testVar)


// Spiral Effect
class CSpiral : public CBaseEntity
{
public:
	void Spawn( void );
	void Think( void );
	int ObjectCaps( void ) { return FCAP_DONT_SAVE; }
	static CSpiral *Create( const Vector &origin, float height, float radius, float duration );
};
LINK_ENTITY_TO_CLASS( streak_spiral, CSpiral );


class CStomp : public CBaseEntity
{
public:
	CStomp();
	BOOL usesSoundSentenceSave(void);
	void Spawn( void );
	void Think( void );
	static CStomp *StompCreate( const Vector &origin, const Vector &end, float speed );

private:
// UNDONE: re-use this sprite list instead of creating new ones all the time
//	CSprite		*m_pSprites[ STOMP_SPRITE_COUNT ];
};

LINK_ENTITY_TO_CLASS( garg_stomp, CStomp );
CStomp *CStomp::StompCreate( const Vector &origin, const Vector &end, float speed )
{
	CStomp *pStomp = GetClassPtr( (CStomp *)NULL );
	
	pStomp->pev->origin = origin;
	Vector dir = (end - origin);
	pStomp->pev->scale = dir.Length();
	pStomp->pev->movedir = dir.Normalize();
	pStomp->pev->speed = speed;
	pStomp->Spawn();
	
	return pStomp;
}

CStomp::CStomp(){
	
}
BOOL CStomp::usesSoundSentenceSave(void){
	return TRUE;
}


void CStomp::Spawn( void )
{
	pev->nextthink = gpGlobals->time;
	pev->classname = MAKE_STRING("garg_stomp");
	pev->dmgtime = gpGlobals->time;

	pev->framerate = 30;
	pev->model = MAKE_STRING(GARG_STOMP_SPRITE_NAME);
	pev->rendermode = kRenderTransTexture;
	pev->renderamt = 0;
	EMIT_SOUND_DYN( edict(), CHAN_BODY, GARG_STOMP_BUZZ_SOUND, 1, ATTN_NORM, 0, PITCH_NORM * 0.55);
}


#define	STOMP_INTERVAL		0.025

void CStomp::Think( void )
{
	TraceResult tr;

	pev->nextthink = gpGlobals->time + 0.1;

	// Do damage for this frame
	Vector vecStart = pev->origin;
	vecStart.z += 30;
	Vector vecEnd = vecStart + (pev->movedir * pev->speed * gpGlobals->frametime);

	UTIL_TraceHull( vecStart, vecEnd, dont_ignore_monsters, head_hull, ENT(pev), &tr );
	
	if ( tr.pHit && tr.pHit != pev->owner )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( tr.pHit );
		entvars_t *pevOwner = pev;
		if ( pev->owner )
			pevOwner = VARS(pev->owner);

		if ( pEntity )
			pEntity->TakeDamage( pev, pevOwner, gSkillData.gargantuaDmgStomp, DMG_SONIC );
	}
	
	// Accelerate the effect
	pev->speed = pev->speed + (gpGlobals->frametime) * pev->framerate;
	pev->framerate = pev->framerate + (gpGlobals->frametime) * 1500;
	
	// Move and spawn trails
	while ( gpGlobals->time - pev->dmgtime > STOMP_INTERVAL )
	{
		pev->origin = pev->origin + pev->movedir * pev->speed * STOMP_INTERVAL;
		for ( int i = 0; i < 2; i++ )
		{
			CSprite *pSprite = CSprite::SpriteCreate( GARG_STOMP_SPRITE_NAME, pev->origin, TRUE );
			if ( pSprite )
			{
				UTIL_TraceLine( pev->origin, pev->origin - Vector(0,0,500), ignore_monsters, edict(), &tr );
				pSprite->pev->origin = tr.vecEndPos;
				pSprite->pev->velocity = Vector(RANDOM_FLOAT(-200,200),RANDOM_FLOAT(-200,200),175);
				// pSprite->AnimateAndDie( RANDOM_FLOAT( 8.0, 12.0 ) );
				pSprite->pev->nextthink = gpGlobals->time + 0.3;
				pSprite->SetThink( &CBaseEntity::SUB_Remove );
				pSprite->SetTransparency( kRenderTransAdd, 255, 255, 255, 255, kRenderFxFadeFast );
			}
		}
		pev->dmgtime += STOMP_INTERVAL;
		// Scale has the "life" of this effect
		pev->scale -= STOMP_INTERVAL * pev->speed;
		if ( pev->scale <= 0 )
		{
			// Life has run out
			UTIL_Remove(this);
			STOP_SOUND_FILTERED( edict(), CHAN_BODY, GARG_STOMP_BUZZ_SOUND );
		}

	}
}


void StreakSplash( const Vector &origin, const Vector &direction, int color, int count, int speed, int velocityRange )
{
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, origin );
		WRITE_BYTE( TE_STREAK_SPLASH );
		WRITE_COORD( origin.x );		// origin
		WRITE_COORD( origin.y );
		WRITE_COORD( origin.z );
		WRITE_COORD( direction.x );	// direction
		WRITE_COORD( direction.y );
		WRITE_COORD( direction.z );
		WRITE_BYTE( color );	// Streak color 6
		WRITE_SHORT( count );	// count
		WRITE_SHORT( speed );
		WRITE_SHORT( velocityRange );	// Random velocity modifier
	MESSAGE_END();
}


class CGargantua : public CBaseMonster
{
public:
	//MODDD - new
	CGargantua();
	BOOL gargDeadBoundChangeYet;
	float fallShakeTime;

	void MonsterThink(void);
	int  IRelationship( CBaseEntity *pTarget );
	

	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
	
	BOOL isSizeGiant(void);

	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE



	void HandleAnimEvent( MonsterEvent_t *pEvent );

	BOOL CheckMeleeAttack1( float flDot, float flDist );		// Swipe
	BOOL CheckMeleeAttack2( float flDot, float flDist );		// Flames
	BOOL CheckRangeAttack1( float flDot, float flDist );		// Stomp attack
	

	//MODDD - made the bounds bigger, so that no matter where the death anim falls, it will be included for registering weapons hits.
	//(at the moment, the corpse is not collidable with movable entities though due to being difficult to determine with the current cheap hitbox.)
	//being able to have an offset to "pev->origin" for drawing (or the hitbox at least) would be very nice.
	/*
	void SetObjectCollisionBox( void )
	{
		pev->absmin = pev->origin + Vector( -80, -80, 0 );
		pev->absmax = pev->origin + Vector( 80, 80, 214 );
	}
	*/
	//MODDD - new.
	void SetObjectCollisionBox( void )
	{
		//EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "garg deadflag? %d", pev->deadflag));
		//could it be re-adjusted for "DEAD_DEAD" too?
		if(pev->deadflag != DEAD_NO){
			//if we are dead?
			pev->absmin = pev->origin + Vector( -280, -280, 0 );
			pev->absmax = pev->origin + Vector( 280, 280, 214 );
		}else{
			pev->absmin = pev->origin + Vector( -80, -80, 0 );
			pev->absmax = pev->origin + Vector( 80, 80, 214 );
			//CBaseMonster::SetObjectCollisionBox();

		}
	}



	Schedule_t *GetScheduleOfType( int Type );
	void StartTask( Task_t *pTask );
	void RunTask( Task_t *pTask );

	void PrescheduleThink( void );

	GENERATE_KILLED_PROTOTYPE
	void DeathEffect( void );

	//MODDD - Why unused?
	void DeathSound(void);

	void EyeOff( void );
	void EyeOn( int level );
	void EyeUpdate( void );
	void Leap( void );
	void StompAttack( void );
	void FlameCreate( void );
	void FlameUpdate( void );
	void FlameControls( float angleX, float angleY );
	void FlameDestroy( void );
	inline BOOL FlameIsOn( void ) { return m_pFlame[0] != NULL; }

	void FlameDamage( Vector vecStart, Vector vecEnd, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType );

	//MODDD - new. What causes heavy damage? It takes a lot to do that to me.
	void OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod);

	//MODDD - advanced anim.  Just to change flinch speeds per difficulty.
	BOOL usesAdvancedAnimSystem(void);
	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);

	int getHullIndexForNodes(void);


	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	CUSTOM_SCHEDULES;

private:
	static const char *pAttackHitSounds[];
	static const char *pBeamAttackSounds[];
	static const char *pAttackMissSounds[];
	//static const char *pRicSounds[];
	static const char *pFootSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackSounds[];
	static const char *pStompSounds[];
	static const char *pBreatheSounds[];

	//MODDD - new
	CBaseEntity* GargantuaCheckTraceHullAttack(float flDist, int iDamage, int iDmgType, int iDmgTypeMod);
	CBaseEntity* GargantuaCheckTraceHullAttack(float flDist, int iDamage, int iDmgType);

	CSprite		*m_pEyeGlow;		// Glow around the eyes
	CBeam		*m_pFlame[4];		// Flame beams

	int			m_eyeBrightness;	// Brightness target
	float		m_seeTime;			// Time to attack (when I see the enemy, I set this)
	float		m_flameTime;		// Time of next flame attack
	float		m_painSoundTime;	// Time of next pain sound
	float		m_streakTime;		// streak timer (don't send too many)
	float		m_flameX;			// Flame thrower aim
	float		m_flameY;			
};


#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_gargantua, CGargantua );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( gargantua, CGargantua );
	
	#if EXTRA_NAMES == 2
		LINK_ENTITY_TO_CLASS( alien_gargantua, CGargantua );
		LINK_ENTITY_TO_CLASS( monster_alien_gargantua, CGargantua );
		LINK_ENTITY_TO_CLASS( agargantua, CGargantua );
	#endif

#endif

TYPEDESCRIPTION	CGargantua::m_SaveData[] = 
{
	DEFINE_FIELD( CGargantua, m_pEyeGlow, FIELD_CLASSPTR ),
	DEFINE_FIELD( CGargantua, m_eyeBrightness, FIELD_INTEGER ),
	DEFINE_FIELD( CGargantua, m_seeTime, FIELD_TIME ),
	DEFINE_FIELD( CGargantua, m_flameTime, FIELD_TIME ),
	DEFINE_FIELD( CGargantua, m_streakTime, FIELD_TIME ),
	DEFINE_FIELD( CGargantua, m_painSoundTime, FIELD_TIME ),
	DEFINE_ARRAY( CGargantua, m_pFlame, FIELD_CLASSPTR, 4 ),
	DEFINE_FIELD( CGargantua, m_flameX, FIELD_FLOAT ),
	DEFINE_FIELD( CGargantua, m_flameY, FIELD_FLOAT ),
};

IMPLEMENT_SAVERESTORE( CGargantua, CBaseMonster );

const char *CGargantua::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CGargantua::pBeamAttackSounds[] = 
{
	"garg/gar_flameoff1.wav",
	"garg/gar_flameon1.wav",
	"garg/gar_flamerun1.wav",
};


const char *CGargantua::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};


//now unused.
/*
const char *CGargantua::pRicSounds[] = 
{
#if 0
	"weapons/ric1.wav",
	"weapons/ric2.wav",
	"weapons/ric3.wav",
	"weapons/ric4.wav",
	"weapons/ric5.wav",
#else
	"debris/metal4.wav",
	"debris/metal6.wav",
	"weapons/ric4.wav",
	"weapons/ric5.wav",
#endif
};
*/

const char *CGargantua::pFootSounds[] = 
{
	"garg/gar_step1.wav",
	"garg/gar_step2.wav",
};


const char *CGargantua::pIdleSounds[] = 
{
	"garg/gar_idle1.wav",
	"garg/gar_idle2.wav",
	"garg/gar_idle3.wav",
	"garg/gar_idle4.wav",
	"garg/gar_idle5.wav",
};


const char *CGargantua::pAttackSounds[] = 
{
	"garg/gar_attack1.wav",
	"garg/gar_attack2.wav",
	"garg/gar_attack3.wav",
};

const char *CGargantua::pAlertSounds[] = 
{
	"garg/gar_alert1.wav",
	"garg/gar_alert2.wav",
	"garg/gar_alert3.wav",
};

const char *CGargantua::pPainSounds[] = 
{
	"garg/gar_pain1.wav",
	"garg/gar_pain2.wav",
	"garg/gar_pain3.wav",
};

const char *CGargantua::pStompSounds[] = 
{
	"garg/gar_stomp1.wav",
};

const char *CGargantua::pBreatheSounds[] = 
{
	"garg/gar_breathe1.wav",
	"garg/gar_breathe2.wav",
	"garg/gar_breathe3.wav",
};
//=========================================================
// AI Schedules Specific to this monster
//=========================================================
#if 0
enum
{
	SCHED_ = LAST_COMMON_SCHEDULE + 1,
};
#endif

enum
{
	TASK_SOUND_ATTACK = LAST_COMMON_TASK + 1,
	TASK_FLAME_SWEEP,
};

Task_t	tlGargFlame[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_SOUND_ATTACK,		(float)0		},
	// { TASK_PLAY_SEQUENCE,		(float)ACT_SIGNAL1	},
	{ TASK_SET_ACTIVITY,		(float)ACT_MELEE_ATTACK2 },
	{ TASK_FLAME_SWEEP,			(float)4.5		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE	},
};

Schedule_t	slGargFlame[] =
{
	{ 
		tlGargFlame,
		ARRAYSIZE ( tlGargFlame ),
		//MODDD - interruptable by heavy damage
		bits_COND_HEAVY_DAMAGE,
		0,
		"GargFlame"
	},
};


// primary melee attack
Task_t	tlGargSwipe[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_MELEE_ATTACK1,		(float)0		},
};

Schedule_t	slGargSwipe[] =
{
	{ 
		tlGargSwipe,
		ARRAYSIZE ( tlGargSwipe ), 
		bits_COND_CAN_MELEE_ATTACK2 ||
		//MODDD - interruptable by heavy damage
		bits_COND_HEAVY_DAMAGE,
		0,
		"GargSwipe"
	},
};


DEFINE_CUSTOM_SCHEDULES( CGargantua )
{
	slGargFlame,
	slGargSwipe,
};

IMPLEMENT_CUSTOM_SCHEDULES( CGargantua, CBaseMonster );



void CGargantua::DeathSound(void){

	switch(RANDOM_LONG(0, 1)){
		case 0:
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, "garg/gar_die1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM + RANDOM_LONG(-10,10) );
		break;
		case 1:
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, "garg/gar_die2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM + RANDOM_LONG(-10,10) );
		break;
	}

}


int CGargantua::IRelationship( CBaseEntity *pTarget )
{
	
	if(global_thatWasntPunch == 1){
		return R_NO;
	}

	if ( (pTarget->IsPlayer()) )
		if ( (pev->spawnflags & SF_MONSTER_WAIT_UNTIL_PROVOKED ) && ! (m_afMemory & bits_MEMORY_PROVOKED ))
			return R_NO;
	return CBaseMonster::IRelationship( pTarget );
}


void CGargantua::MonsterThink(void){

	
	if(global_thatWasntPunch == 1 && this->m_fSequenceFinished){

		switch(RANDOM_LONG(0, 45)){

			case 0:
				this->SetSequenceByName("bust");
			break;
			case 1:
				this->SetSequenceByName("bust");
			break;
			case 2:
				this->SetSequenceByName("bust");
			break;
			case 3:
				this->SetSequenceByName("pushcar");
			break;
			case 4:
				this->SetSequenceByName("kickcar");
			break;
			case 5:
				this->SetSequenceByName("kickcar");
			break;
			case 6:
				this->SetSequenceByName("kickcar");
			break;
			case 7:
				this->SetSequenceByName("kickcar");
			break;
			case 8:
				this->SetSequenceByName("rollcar");
			break;
			case 9:
				this->SetSequenceByName("rollcar");
			break;
			case 10:
				this->SetSequenceByName("smash");
			break;
			case 11:
				this->SetSequenceByName("smash");
			break;
			case 12:
				this->SetSequenceByName("smash");
			break;
			case 13:
				this->SetSequenceByName("smash");
			break;
			case 14:
				this->SetSequenceByName("throwbody");
			break;
			case 15:
				this->SetSequenceByName("throwbody");
			break;
			case 16:
				this->SetSequenceByName("bitehead");
			break;
			case 17:
				this->SetSequenceByName("bitehead");
			break;
			case 18:
				this->SetSequenceByName("bitehead");
			break;
			case 19:
				this->SetSequenceByName("Flinchheavy");
			break;
			case 20:
				this->SetSequenceByName("Flinchheavy");
			break;
			case 21:
				this->SetSequenceByName("flinchlight");
			break;
			case 22:
				this->SetSequenceByName("flinchlight");
			break;
			case 23:
				this->SetSequenceByName("flinchlight");
			break;
			case 24:
				this->SetSequenceByName("flinchlight");
			break;
			case 25:
				this->SetSequenceByName("flinchlight");
			break;
			case 26:
				this->SetSequenceByName("flinchlight");
			break;
			case 27:
				this->SetSequenceByName("180right");
			break;
			case 28:
				this->SetSequenceByName("180right");
			break;
			case 29:
				this->SetSequenceByName("180left");
			break;
			case 30:
				this->SetSequenceByName("180left");
			break;
			case 31:
				this->SetSequenceByName("stomp");
			break;
			case 32:
				this->SetSequenceByName("Attack");
			break;
			case 33:
				this->SetSequenceByName("Attack");
			break;
			case 34:
				this->SetSequenceByName("Attack");
			break;
			case 35:
				this->SetSequenceByName("shootflames2");
			break;
			case 36:
				this->SetSequenceByName("shootflames2");
			break;
			case 37:
				this->SetSequenceByName("shootflames2");
			break;
			case 38:
				this->SetSequenceByName("shootflames1");
			break;
			case 39:
				this->SetSequenceByName("shootflames1");
			break;
			case 40:
				this->SetSequenceByName("shootflames1");
			break;
			case 41:
				this->SetSequenceByName("idle1");
			break;
			case 42:
				this->SetSequenceByName("idle1");
			break;
			case 43:
				this->SetSequenceByName("idle1");
			break;
			case 44:
				this->SetSequenceByName("idle2");
			break;
			case 45:
				this->SetSequenceByName("idle4");
			break;


		}

	}


	CBaseMonster::MonsterThink();
}




void CGargantua::EyeOn( int level )
{
	m_eyeBrightness = level;	
}


void CGargantua::EyeOff( void )
{
	m_eyeBrightness = 0;
}


void CGargantua::EyeUpdate( void )
{
	if ( m_pEyeGlow )
	{
		m_pEyeGlow->pev->renderamt = UTIL_Approach( m_eyeBrightness, m_pEyeGlow->pev->renderamt, 26 );
		if ( m_pEyeGlow->pev->renderamt == 0 )
			m_pEyeGlow->pev->effects |= EF_NODRAW;
		else
			m_pEyeGlow->pev->effects &= ~EF_NODRAW;
		UTIL_SetOrigin( m_pEyeGlow->pev, pev->origin );
	}
}


void CGargantua::StompAttack( void )
{
	TraceResult trace;

	UTIL_MakeVectors( pev->angles );
	Vector vecStart = pev->origin + Vector(0,0,60) + 35 * gpGlobals->v_forward;
	Vector vecAim = ShootAtEnemy( vecStart );
	Vector vecEnd = (vecAim * 1024) + vecStart;

	UTIL_TraceLine( vecStart, vecEnd, ignore_monsters, edict(), &trace );
	CStomp::StompCreate( vecStart, trace.vecEndPos, 0 );
	UTIL_ScreenShake( pev->origin, 12.0, 100.0, 2.0, 1000 );
	EMIT_SOUND_FILTERED ( edict(), CHAN_WEAPON, pStompSounds[ RANDOM_LONG(0,ARRAYSIZE(pStompSounds)-1) ], 1.0, ATTN_GARG, 0, PITCH_NORM + RANDOM_LONG(-10,10) );

	UTIL_TraceLine( pev->origin, pev->origin - Vector(0,0,20), ignore_monsters, edict(), &trace );
	if ( trace.flFraction < 1.0 )
		UTIL_DecalTrace( &trace, DECAL_GARGSTOMP1 );
}


void CGargantua :: FlameCreate( void )
{
	int			i;
	Vector		posGun, angleGun;
	TraceResult trace;

	UTIL_MakeVectors( pev->angles );
	
	for ( i = 0; i < 4; i++ )
	{
		if ( i < 2 )
			m_pFlame[i] = CBeam::BeamCreate( GARG_BEAM_SPRITE_NAME, 240 );
		else
			m_pFlame[i] = CBeam::BeamCreate( GARG_BEAM_SPRITE2, 140 );
		if ( m_pFlame[i] )
		{
			int attach = i%2;
			// attachment is 0 based in GetAttachment
			GetAttachment( attach+1, posGun, angleGun );

			Vector vecEnd = (gpGlobals->v_forward * GARG_FLAME_LENGTH) + posGun;
			UTIL_TraceLine( posGun, vecEnd, dont_ignore_monsters, edict(), &trace );

			m_pFlame[i]->PointEntInit( trace.vecEndPos, entindex() );
			if ( i < 2 )
				m_pFlame[i]->SetColor( 255, 130, 90 );
			else
				m_pFlame[i]->SetColor( 0, 120, 255 );
			m_pFlame[i]->SetBrightness( 190 );
			m_pFlame[i]->SetFlags( BEAM_FSHADEIN );
			m_pFlame[i]->SetScrollRate( 20 );
			// attachment is 1 based in SetEndAttachment
			m_pFlame[i]->SetEndAttachment( attach + 2 );
			CSoundEnt::InsertSound( bits_SOUND_COMBAT, posGun, 384, 0.3 );
		}
	}
	EMIT_SOUND_FILTERED ( edict(), CHAN_BODY, pBeamAttackSounds[ 1 ], 1.0, ATTN_NORM, 0, PITCH_NORM );
	EMIT_SOUND_FILTERED ( edict(), CHAN_WEAPON, pBeamAttackSounds[ 2 ], 1.0, ATTN_NORM, 0, PITCH_NORM );
}


void CGargantua :: FlameControls( float angleX, float angleY )
{
	if ( angleY < -180 )
		angleY += 360;
	else if ( angleY > 180 )
		angleY -= 360;

	if ( angleY < -45 )
		angleY = -45;
	else if ( angleY > 45 )
		angleY = 45;

	m_flameX = UTIL_ApproachAngle( angleX, m_flameX, 4 );
	m_flameY = UTIL_ApproachAngle( angleY, m_flameY, 8 );
	SetBoneController( 0, m_flameY );
	SetBoneController( 1, m_flameX );
}


void CGargantua :: FlameUpdate( void )
{
	int				i;
	static float	offset[2] = { 60, -60 };
	TraceResult		trace;
	Vector			vecStart, angleGun;
	BOOL			streaks = FALSE;

	for ( i = 0; i < 2; i++ )
	{
		if ( m_pFlame[i] )
		{
			Vector vecAim = pev->angles;
			vecAim.x += m_flameX;
			vecAim.y += m_flameY;

			UTIL_MakeVectors( vecAim );

			GetAttachment( i+1, vecStart, angleGun );
			Vector vecEnd = vecStart + (gpGlobals->v_forward * GARG_FLAME_LENGTH); //  - offset[i] * gpGlobals->v_right;

			UTIL_TraceLine( vecStart, vecEnd, dont_ignore_monsters, edict(), &trace );

			m_pFlame[i]->SetStartPos( trace.vecEndPos );
			m_pFlame[i+2]->SetStartPos( (vecStart * 0.6) + (trace.vecEndPos * 0.4) );

			if ( trace.flFraction != 1.0 && gpGlobals->time > m_streakTime )
			{
				StreakSplash( trace.vecEndPos, trace.vecPlaneNormal, 6, 20, 50, 400 );
				streaks = TRUE;
				UTIL_DecalTrace( &trace, DECAL_SMALLSCORCH1 + RANDOM_LONG(0,2) );
			}
			// RadiusDamageAutoRadius( trace.vecEndPos, pev, pev, gSkillData.gargantuaDmgFire, CLASS_ALIEN_MONSTER, DMG_BURN );
			FlameDamage( vecStart, trace.vecEndPos, pev, pev, gSkillData.gargantuaDmgFire, CLASS_ALIEN_MONSTER, DMG_BURN );

			MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
				WRITE_BYTE( TE_ELIGHT );
				WRITE_SHORT( entindex( ) + 0x1000 * (i + 2) );		// entity, attachment
				WRITE_COORD( vecStart.x );		// origin
				WRITE_COORD( vecStart.y );
				WRITE_COORD( vecStart.z );
				WRITE_COORD( RANDOM_FLOAT( 32, 48 ) );	// radius
				WRITE_BYTE( 255 );	// R
				WRITE_BYTE( 255 );	// G
				WRITE_BYTE( 255 );	// B
				WRITE_BYTE( 2 );	// life * 10
				WRITE_COORD( 0 ); // decay
			MESSAGE_END();
		}
	}
	if ( streaks )
		m_streakTime = gpGlobals->time;
}



void CGargantua :: FlameDamage( Vector vecStart, Vector vecEnd, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType )
{
	CBaseEntity *pEntity = NULL;
	TraceResult	tr;
	float		flAdjustedDamage;
	Vector		vecSpot;

	Vector vecMid = (vecStart + vecEnd) * 0.5;

	float searchRadius = (vecStart - vecMid).Length();

	Vector vecAim = (vecEnd - vecStart).Normalize( );

	// iterate on all entities in the vicinity.
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, vecMid, searchRadius )) != NULL)
	{
		if ( pEntity->pev->takedamage != DAMAGE_NO )
		{
			// UNDONE: this should check a damage mask, not an ignore
			if ( iClassIgnore != CLASS_NONE && pEntity->Classify() == iClassIgnore )
			{// houndeyes don't hurt other houndeyes with their attack
				continue;
			}
			
			vecSpot = pEntity->BodyTarget( vecMid );
		
			float dist = DotProduct( vecAim, vecSpot - vecMid );
			if (dist > searchRadius)
				dist = searchRadius;
			else if (dist < -searchRadius)
				dist = searchRadius;
			
			Vector vecSrc = vecMid + dist * vecAim;

			UTIL_TraceLine ( vecSrc, vecSpot, dont_ignore_monsters, ENT(pev), &tr );

			if ( tr.flFraction == 1.0 || tr.pHit == pEntity->edict() )
			{// the explosion can 'see' this entity, so hurt them!
				// decrease damage for an ent that's farther from the flame.
				dist = ( vecSrc - tr.vecEndPos ).Length();

				if (dist > 64)
				{
					flAdjustedDamage = flDamage - (dist - 64) * 0.4;
					if (flAdjustedDamage <= 0)
						continue;
				}
				else
				{
					flAdjustedDamage = flDamage;
				}

				// ALERT( at_console, "hit %s\n", STRING( pEntity->pev->classname ) );
				if (tr.flFraction != 1.0)
				{
					ClearMultiDamage( );
					//MODDD - don't do different damage to different hitboxes.
					pEntity->TraceAttack( pevInflictor, flAdjustedDamage, (tr.vecEndPos - vecSrc).Normalize( ), &tr, bitsDamageType, DMG_HITBOX_EQUAL );
					ApplyMultiDamage( pevInflictor, pevAttacker );
				}
				else
				{
					pEntity->TakeDamage ( pevInflictor, pevAttacker, flAdjustedDamage, bitsDamageType, DMG_HITBOX_EQUAL );
				}
			}
		}
	}
}


void CGargantua :: FlameDestroy( void )
{
	int i;

	EMIT_SOUND_FILTERED ( edict(), CHAN_WEAPON, pBeamAttackSounds[ 0 ], 1.0, ATTN_NORM, 0, PITCH_NORM );
	for ( i = 0; i < 4; i++ )
	{
		if ( m_pFlame[i] )
		{
			UTIL_Remove( m_pFlame[i] );
			m_pFlame[i] = NULL;
		}
	}
}


void CGargantua :: PrescheduleThink( void )
{
	if ( !HasConditions( bits_COND_SEE_ENEMY ) )
	{
		m_seeTime = gpGlobals->time + 5;
		EyeOff();
	}
	else
		EyeOn( 200 );
	
	EyeUpdate();
}


//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CGargantua :: Classify ( void )
{
	return	CLASS_ALIEN_MONSTER;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CGargantua :: SetYawSpeed ( void )
{
	int ys;

	switch ( m_Activity )
	{
	case ACT_IDLE:
		ys = 60;
		break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 180;
		break;
	case ACT_WALK:
	case ACT_RUN:
		ys = 60;
		break;

	default:
		ys = 60;
		break;
	}

	pev->yaw_speed = ys;
}


CGargantua::CGargantua(){

	gargDeadBoundChangeYet = FALSE;
	fallShakeTime = -1;
}

//=========================================================
// Spawn
//=========================================================
void CGargantua :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/garg.mdl");
	UTIL_SetSize( pev, Vector( -32, -32, 0 ), Vector( 32, 32, 64 ) );


	pev->classname = MAKE_STRING("monster_gargantua");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->health			= gSkillData.gargantuaHealth;
	//pev->view_ofs		= Vector ( 0, 0, 96 );// taken from mdl file
	m_flFieldOfView		= -0.2;// width of forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	MonsterInit();

	m_pEyeGlow = CSprite::SpriteCreate( GARG_EYE_SPRITE_NAME, pev->origin, FALSE );
	m_pEyeGlow->SetTransparency( kRenderGlow, 255, 255, 255, 0, kRenderFxNoDissipation );
	m_pEyeGlow->SetAttachment( edict(), 1 );
	EyeOff();
	m_seeTime = gpGlobals->time + 5;
	m_flameTime = gpGlobals->time + 2;

}

extern int global_useSentenceSave;
//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CGargantua :: Precache()
{
	int i;

	PRECACHE_MODEL("models/garg.mdl");
	PRECACHE_MODEL( GARG_EYE_SPRITE_NAME );
	PRECACHE_MODEL( GARG_BEAM_SPRITE_NAME );
	PRECACHE_MODEL( GARG_BEAM_SPRITE2 );
	gStompSprite = PRECACHE_MODEL( GARG_STOMP_SPRITE_NAME );
	gGargGibModel = PRECACHE_MODEL( GARG_GIB_MODEL );


	global_useSentenceSave = TRUE;
	PRECACHE_SOUND( GARG_STOMP_BUZZ_SOUND, TRUE ); //precached by the player, can't skip.
	
	PRECACHE_SOUND("debris/metal6.wav");

	for ( i = 0; i < ARRAYSIZE( pAttackHitSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackHitSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pBeamAttackSounds ); i++ )
		PRECACHE_SOUND((char *)pBeamAttackSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackMissSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackMissSounds[i]);

	//for ( i = 0; i < ARRAYSIZE( pRicSounds ); i++ )
	//	PRECACHE_SOUND((char *)pRicSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pFootSounds ); i++ )
		PRECACHE_SOUND((char *)pFootSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pIdleSounds ); i++ )
		PRECACHE_SOUND((char *)pIdleSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAlertSounds ); i++ )
		PRECACHE_SOUND((char *)pAlertSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pPainSounds ); i++ )
		PRECACHE_SOUND((char *)pPainSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pStompSounds ); i++ )
		PRECACHE_SOUND((char *)pStompSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pBreatheSounds ); i++ )
		PRECACHE_SOUND((char *)pBreatheSounds[i]);

	
	PRECACHE_SOUND("garg/gar_die1.wav");
	PRECACHE_SOUND("garg/gar_die2.wav");


	global_useSentenceSave = FALSE;
}	









GENERATE_TRACEATTACK_IMPLEMENTATION(CGargantua){


	ALERT( at_aiconsole, "CGargantua::TraceAttack\n");


	BOOL isAliveVar = IsAlive();

	//MODDD - nah, this method is fine, even if dead.
	/*
	if ( !IsAlive() )
	{
		CBaseMonster::TraceAttack( pevAttacker, flDamage, vecDir, ptr, bitsDamageType );
		return;
	}
	*/



	BOOL painSoundPass = FALSE;
	if(pev->deadflag == DEAD_NO){
		painSoundPass = TRUE;
	}else if(pev->deadflag == DEAD_DYING){

		if( (global_gargantuaCorpseDeath == 2 || global_gargantuaCorpseDeath == 5)){
			painSoundPass = TRUE;
		}

	}else{
		//leave false.
	}


	//pev->deadflag == DEAD_NO
	//MODDD - condition also depends on "gargantuaCorpseDeath" (or being alive, neither dead nor in dying animation).
	// UNDONE: Hit group specific damage?   (comment found here, oh well)
	if ( painSoundPass && (bitsDamageType & (GARG_DAMAGE|DMG_BLAST) )    )
	{
		if ( m_painSoundTime < gpGlobals->time )
		{
			EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, pPainSounds[ RANDOM_LONG(0,ARRAYSIZE(pPainSounds)-1) ], 1.0, ATTN_GARG, 0, PITCH_NORM );
			m_painSoundTime = gpGlobals->time + RANDOM_FLOAT( 2.5, 4 );
		}
	}


	//!!!!!!
	if(pev->deadflag != DEAD_DEAD){
		bitsDamageType &= GARG_DAMAGE;
	}else{
		//if dead, let "DMG_CLUB" pass...
		bitsDamageType &= (GARG_DAMAGE|DMG_CLUB);
	}


	//MODDD - this might look a little weird. Note that, above, bitsDamageType gets all damage types filtered out except for those in GARG_DAMAGE (or also DMG_CLUB if dead so the crowbar is alloowed).
	//        GARG_DAMAGE is made of a list of forms of damage that count against the gargantua.
	//        If not a single damage type from GARG_DAMAGE is present in the provided bitsDamageType, then stripping bitsDamageType of all but GARG_DAMAGE leaves nothing (0).
	//        So the 0 check here doesn't ask, "Did the sender give us a 0 bitsDamageType?". It's saying, "After stripping out all tpes but those in GARG_DAMAGE from bitsDamageType, does
	//        anything remain? If not...". Also this ends up setting the damage dealt to 0 if the gargantua is alive.
	if ( bitsDamageType == 0)
	{
		if ( pev->dmgtime != gpGlobals->time || (RANDOM_LONG(0,100) < 20) )
		{

			float gargantuaBleedsVar = global_gargantuaBleeds;

			
			//MODDD - options for hit effect.
			if(gargantuaBleedsVar == 0){
				//no sound or effect.
				if(useBulletHitSound)*useBulletHitSound=FALSE;
			}else if(gargantuaBleedsVar == 1){
				//UTIL_playFleshHitSound(pev);
				//just don't block.
				DrawAlphaBlood(flDamage, ptr );
			}else if(gargantuaBleedsVar == 2){
				if(useBulletHitSound)*useBulletHitSound=FALSE;
				DrawAlphaBlood(flDamage, ptr );
			}else if(gargantuaBleedsVar == 3){
				if(useBulletHitSound)*useBulletHitSound=FALSE;
				UTIL_Ricochet( ptr->vecEndPos, RANDOM_FLOAT(0.5,1.5) );
			}
			
			//MODDD - this does require being alive, though.
			if(isAliveVar){
				pev->dmgtime = gpGlobals->time;
			}

//			if ( RANDOM_LONG(0,100) < 25 )
//				EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, pRicSounds[ RANDOM_LONG(0,ARRAYSIZE(pRicSounds)-1) ], 1.0, ATTN_NORM, 0, PITCH_NORM );
		}

		if(isAliveVar){
			flDamage = 0;
		}
		
	}

	//send "FALSE" for "useBloodEffect", as the hit effect to use was handled above already.
	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);

}


//definitely.
BOOL CGargantua::isSizeGiant(void){
	return TRUE;
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CGargantua)
{
	ALERT( at_aiconsole, "CGargantua::TakeDamage\n");




	EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "GARG DAMAGE %.2f", pev->health ));
	//easyForcePrintLine("????????????? %d %d", IsAlive(), pev->deadflag);
	if ( IsAlive() )
	{

		
		//usal checks.
		if ( !(bitsDamageType & GARG_DAMAGE) )
			flDamage *= 0.01;
		if ( bitsDamageType & DMG_BLAST )
			SetConditions( bits_COND_LIGHT_DAMAGE );

		

	}else{
		//MODDD - possible intervention.  If the player (FL_CLIENT) uses the crowbar on the corpse (DEAD_DEAD), do at least a little.
		//
		//pevAttacker!=NULL&&FBitSet ( pevAttacker->flags, FL_CLIENT )&&
		//easyForcePrintLine("YOU CRAZY LITTLE ffeee %d", bitsDamageType);
		if (   pev->deadflag == DEAD_DEAD ){

			if(bitsDamageType & DMG_CLUB){
				flDamage = 50;
			}else if(DMG_BLAST & (DMG_BLAST | DMG_MORTAR) ){
				flDamage *= 7;
			}
			//pev->health --;
		}

	}



	/*
	//MODDD - NEW
	else{
		return 0;
	}
	*/

	float valueOf = global_gargantuaCorpseDeath;


	if( (valueOf == 1 || valueOf == 4) && pev->deadflag == DEAD_DYING){
		EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "DAMAGE BLOCKED!"));
		//when in the dying anim and "gargantuaCorpseDeath" is 1 or 4, do not take damage yet.
		return 1;
	}


	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}


void CGargantua::DeathEffect( void )
{

	int i;
	UTIL_MakeVectors(pev->angles);
	Vector deathPos = pev->origin + gpGlobals->v_forward * 100;

	// Create a spiral of streaks
	CSpiral::Create( deathPos, (pev->absmax.z - pev->absmin.z) * 0.6, 125, 1.5 );

	Vector position = pev->origin;
	position.z += 32;
	for ( i = 0; i < 7; i+=2 )
	{
		SpawnExplosion( position, 70, (i * 0.3), 60 + (i*20) );
		position.z += 15;
	}

	CBaseEntity *pSmoker = CBaseEntity::Create( "env_smoker", pev->origin, g_vecZero, NULL );
	pSmoker->pev->health = 1;	// 1 smoke balls
	pSmoker->pev->scale = 46;	// 4.6X normal size
	pSmoker->pev->dmg = 0;		// 0 radial distribution
	pSmoker->pev->nextthink = gpGlobals->time + 2.5;	// Start in 2.5 seconds
}



/*
void CGargantua::Killed( entvars_t *pevAttacker, int iGib )
{
	EyeOff();
	UTIL_Remove( m_pEyeGlow );
	m_pEyeGlow = NULL;
	CBaseMonster::Killed( pevAttacker, GIB_NEVER );
}
*/

GENERATE_KILLED_IMPLEMENTATION(CGargantua)
{
	EyeOff();
	UTIL_Remove( m_pEyeGlow );
	m_pEyeGlow = NULL;



	//MODDD - WHY IS THIS NOT HERE???!!!
	FlameDestroy();



	int gibFlag = 0;

	float valueOf = global_gargantuaCorpseDeath;



	EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "DEAD FLAG: %d", pev->deadflag));

	if(valueOf == 0 || valueOf == 3 || valueOf == 6){
		gibFlag = GIB_NEVER;  //handled by death effect.  Or, at 3, just never gibbable.
	}if(valueOf == 1 || valueOf == 4){

		if(pev->deadflag == DEAD_NO){
			gibFlag = GIB_NEVER;
		}else if(pev->deadflag == DEAD_DYING){
			gibFlag = GIB_NEVER;
		}else if(pev->deadflag == DEAD_DEAD){
			gibFlag = GIB_ALWAYS;
		}

	}else if(valueOf == 2 || valueOf == 5){
		if(pev->deadflag == DEAD_NO){
			gibFlag = GIB_NEVER;
		}else if(pev->deadflag == DEAD_DYING){
			gibFlag = GIB_NORMAL;
		}else if(pev->deadflag == DEAD_DEAD){
			gibFlag = GIB_ALWAYS;
		}

	}

	
	BOOL justDied = (pev->deadflag == DEAD_NO);



	//MODDD 
	//EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "GARG: GIB I CAME WITH: %d", iGib));
	//if(iGib == GIB_NORMAL){

	//if we're getting gibbed (died the "2nd" time), mute the channels.  Nothing to play sound here anymore.
	if(!justDied){
		//stop sound.
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, "common/null.wav", 1, ATTN_NORM);
		EMIT_SOUND(ENT(pev), CHAN_VOICE, "common/null.wav", 1, ATTN_NORM);


		//MODDD - not enough?  Try this for all pain sounds I guess.
		//STOP_SOUND_FILTERED( ENT(pev), CHAN_STATIC, "apache/ap_rotor4.wav" );

		
	}



	if(valueOf == 4 || valueOf == 5){
		if(justDied){
			//about to be?
			EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "HEALTH FORCED TO 30"));
			//pev->health = 30;
		}
	}
	



	if(pev->deadflag == DEAD_NO){
		//The first blow can never gib this creature.  It's too big to be killed in one hit,
		//no matter how many before that, and just disappear into flying gibs.
		CBaseMonster::Killed( pevInflictor, pevAttacker, GIB_NEVER );
	}else{

		if(valueOf == 0 || valueOf == 6){
			//Retail explosion transform effect mid-death anim.
			//Still never gibbable. The death anim ends in gibbing itself so let it finish.
			CBaseMonster::Killed( pevInflictor, pevAttacker, GIB_NEVER );
		}else{
			//Going to leave a corpse.  Let it be gibbable this time.
			//DYING or DEAD?  eh, gibbable. This is possible now.
			GENERATE_KILLED_PARENT_CALL(CBaseMonster);
		}
	}
}//END OF killed

//=========================================================
// CheckMeleeAttack1
// Garg swipe attack
// 
//=========================================================
BOOL CGargantua::CheckMeleeAttack1( float flDot, float flDist )
{
//	ALERT(at_aiconsole, "CheckMelee(%f, %f)\n", flDot, flDist);

	if (flDot >= 0.7)
	{
		if (flDist <= GARG_ATTACKDIST)
			return TRUE;
	}
	return FALSE;
}


// Flame thrower madness!
BOOL CGargantua::CheckMeleeAttack2( float flDot, float flDist )
{
//	ALERT(at_aiconsole, "CheckMelee(%f, %f)\n", flDot, flDist);

	if ( gpGlobals->time > m_flameTime )
	{
		if (flDot >= 0.8 && flDist > GARG_ATTACKDIST)
		{
			if ( flDist <= GARG_FLAME_LENGTH )
				return TRUE;
		}
	}
	return FALSE;
}


//=========================================================
// CheckRangeAttack1
// flDot is the cos of the angle of the cone within which
// the attack can occur.
//=========================================================
//
// Stomp attack
//
//=========================================================
BOOL CGargantua::CheckRangeAttack1( float flDot, float flDist )
{
	if ( gpGlobals->time > m_seeTime )
	{
		if (flDot >= 0.7 && flDist > GARG_ATTACKDIST)
		{
				return TRUE;
		}
	}
	return FALSE;
}




//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CGargantua::HandleAnimEvent(MonsterEvent_t *pEvent)
{


	if(global_thatWasntPunch == 1){
		return;
	}


	switch( pEvent->event )
	{
	case GARG_AE_SLASH_LEFT:
		{
			// HACKHACK!!!
			//MODDD - inflicts bleeding.
			CBaseEntity *pHurt = GargantuaCheckTraceHullAttack( GARG_ATTACKDIST + 10.0, gSkillData.gargantuaDmgSlash, DMG_SLASH, DMG_BLEEDING );
			if (pHurt)
			{
				if ( (pHurt->pev->flags & (FL_MONSTER|FL_CLIENT)) && !pHurt->blocksImpact() )
				{
					pHurt->pev->punchangle.x = -30; // pitch
					pHurt->pev->punchangle.y = -30;	// yaw
					pHurt->pev->punchangle.z = 30;	// roll
					//UTIL_MakeVectors(pev->angles);	// called by CheckTraceHullAttack
					pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 100;
				}
				EMIT_SOUND_FILTERED ( edict(), CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 50 + RANDOM_LONG(0,15) );
			}
			else // Play a random attack miss sound
				EMIT_SOUND_FILTERED ( edict(), CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 50 + RANDOM_LONG(0,15) );

			Vector forward;
			UTIL_MakeVectorsPrivate( pev->angles, forward, NULL, NULL );
		}
		break;

	case GARG_AE_RIGHT_FOOT:
	case GARG_AE_LEFT_FOOT:
		UTIL_ScreenShake( pev->origin, 4.0, 3.0, 1.0, 750 );
		EMIT_SOUND_FILTERED ( edict(), CHAN_BODY, pFootSounds[ RANDOM_LONG(0,ARRAYSIZE(pFootSounds)-1) ], 1.0, ATTN_GARG, 0, PITCH_NORM + RANDOM_LONG(-10,10) );
		break;

	case GARG_AE_STOMP:
		StompAttack();
		m_seeTime = gpGlobals->time + 12;
		break;

	case GARG_AE_BREATHE:
		EMIT_SOUND_FILTERED ( edict(), CHAN_VOICE, pBreatheSounds[ RANDOM_LONG(0,ARRAYSIZE(pBreatheSounds)-1) ], 1.0, ATTN_GARG, 0, PITCH_NORM + RANDOM_LONG(-10,10) );
		break;

	default:
		CBaseMonster::HandleAnimEvent(pEvent);
		break;
	}
}


//=========================================================
// CheckTraceHullAttack - expects a length to trace, amount 
// of damage to do, and damage type. Returns a pointer to
// the damaged entity in case the monster wishes to do
// other stuff to the victim (punchangle, etc)
// Used for many contact-range melee attacks. Bites, claws, etc.

// Overridden for Gargantua because his swing starts lower as
// a percentage of his height (otherwise he swings over the
// players head)
//=========================================================

//MODDD - can also work with the new damage mask in the same way as "CheckTraceHullAttack".
CBaseEntity* CGargantua::GargantuaCheckTraceHullAttack(float flDist, int iDamage, int iDmgType)
{
	return CGargantua::GargantuaCheckTraceHullAttack(flDist, iDamage, iDmgType, 0);

}


CBaseEntity* CGargantua::GargantuaCheckTraceHullAttack(float flDist, int iDamage, int iDmgType, int iDmgTypeMod)
{
	TraceResult tr;

	UTIL_MakeVectors( pev->angles );
	Vector vecStart = pev->origin;
	vecStart.z += 64;
	Vector vecEnd = vecStart + (gpGlobals->v_forward * flDist) - (gpGlobals->v_up * flDist * 0.3);

	UTIL_TraceHull( vecStart, vecEnd, dont_ignore_monsters, head_hull, ENT(pev), &tr );
	
	if ( tr.pHit )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( tr.pHit );

		if ( iDamage > 0 )
		{
			pEntity->TakeDamage( pev, pev, iDamage, iDmgType );
			//MODDD - here too.
			//MODDD - draw blood.
			UTIL_fromToBlood(this, pEntity, (float)iDamage, flDist, &tr.vecEndPos, &vecStart, &vecEnd);

		}

		return pEntity;
	}

	return NULL;
}


Schedule_t *CGargantua::GetScheduleOfType( int Type )
{
	// HACKHACK - turn off the flames if they are on and garg goes scripted / dead
	if ( FlameIsOn() )
		FlameDestroy();

	switch( Type )
	{
		case SCHED_MELEE_ATTACK2:
			return slGargFlame;
		case SCHED_MELEE_ATTACK1:
			return slGargSwipe;
		break;
	}

	return CBaseMonster::GetScheduleOfType( Type );
}


void CGargantua::StartTask( Task_t *pTask )
{

	float valueOf;
	float time = -1;

	switch ( pTask->iTask )
	{
	case TASK_FLAME_SWEEP:
		FlameCreate();
		m_flWaitFinished = gpGlobals->time + pTask->flData;
		m_flameTime = gpGlobals->time + 6;
		m_flameX = 0;
		m_flameY = 0;
		break;

	case TASK_SOUND_ATTACK:
		if ( RANDOM_LONG(0,100) < 30 ){
			EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, pAttackSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackSounds)-1) ], 1.0, ATTN_GARG, 0, PITCH_NORM );
		}
		TaskComplete();
		break;
	
	case TASK_DIE:
		//MODDD - time changed.
		

		//Actually, no.
		valueOf = global_gargantuaCorpseDeath;
		time = -1;
		if(valueOf == 0){
			time = 1.6;
		}else if(valueOf == 6){
			//full anim
			time = (76.0 / 15.0);
		}


		if(time != -1){
			m_flWaitFinished = gpGlobals->time + time;
			DeathEffect();
		}

		if(valueOf > 0){
			//any value but "0", the death anim will make it to the fall thud (if not gibbed before).
			fallShakeTime = gpGlobals->time + (64.0 / 15.0);
		}


		
		//CBaseMonster::StartTask(pTask);
		//NOTE: copy of StartTask's TASK_DIE fragment for the edit.
		RouteClear();
		m_IdealActivity = GetDeathActivity();
		pev->deadflag = DEAD_DYING;
		deadSetActivityBlock = TRUE;
		//easyPrintLine("ARE YOU SOME KIND OF insecure person??? %.2f %d", global_thoroughHitBoxUpdates, pev->deadflag );
		//MODDD
		if(global_thoroughHitBoxUpdates == 1){
			//update the collision box now,
			this->SetObjectCollisionBox();
		}
		if(global_gargantuaKilledBoundsAssist == 0 &&global_animationKilledBoundsRemoval == 1){
			setPhysicalHitboxForDeath();
		}






		// FALL THROUGH... NO THAT IS TERRIBLE.
		break;
	default: 
		CBaseMonster::StartTask( pTask );
		break;
	}
}

//=========================================================
// RunTask
//=========================================================
void CGargantua::RunTask( Task_t *pTask )
{
	float valueOf;
	float valueOf2;

	switch ( pTask->iTask )
	{
	case TASK_DIE:

		//MODDD - This is the transform-effect.  Removed, fall and become a gib-able corpse like the rest.
		


		valueOf = global_gargantuaCorpseDeath;
		valueOf2 = global_gargantuaFallSound;
		
		


		if(fallShakeTime != -1 && valueOf > 0 && gpGlobals->time > fallShakeTime){
			UTIL_ScreenShake( pev->origin, 12.0, 100.0, 2.0, 1000 );
			fallShakeTime = -1;

			if(valueOf2 == 1){
				EMIT_SOUND_FILTERED( edict(), CHAN_BODY, "!gargFallSnd", 1, ATTN_NORM, 0, PITCH_NORM); //* 0.55);
			}else if(valueOf2 == 2){
				EMIT_SOUND_FILTERED( edict(), CHAN_BODY, "debris/metal6.wav", 1, ATTN_NORM, 0, PITCH_NORM);
			}

		}

		if ( (valueOf == 0 || valueOf == 6) && gpGlobals->time > m_flWaitFinished )
		{
			//MODDD - notes
			//  This is for the gargantua's gib explosion at death following the transform stretch effect,
			//  in dramatic video game fashion for something big and scary dying.
		    
			pev->renderfx = kRenderFxExplode;
			pev->rendercolor.x = 255;
			pev->rendercolor.y = 0;
			pev->rendercolor.z = 0;
			StopAnimation();
			pev->nextthink = gpGlobals->time + 0.15;
			SetThink( &CBaseEntity::SUB_Remove );
			int i;
			int parts = MODEL_FRAMES( gGargGibModel );
			for ( i = 0; i < 10; i++ )
			{
				CGib *pGib = GetClassPtr( (CGib *)NULL );

				pGib->Spawn( GARG_GIB_MODEL );
				
				int bodyPart = 0;
				if ( parts > 1 )
					bodyPart = RANDOM_LONG( 0, pev->body-1 );

				pGib->pev->body = bodyPart;
				pGib->m_bloodColor = BLOOD_COLOR_YELLOW;
				pGib->m_material = matNone;
				pGib->pev->origin = pev->origin;
				pGib->pev->velocity = UTIL_RandomBloodVector() * RANDOM_FLOAT( 300, 500 );
				pGib->pev->nextthink = gpGlobals->time + 1.25;
				pGib->SetThink( &CBaseEntity::SUB_FadeOut );
			}
			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
				WRITE_BYTE( TE_BREAKMODEL);

				// position
				WRITE_COORD( pev->origin.x );
				WRITE_COORD( pev->origin.y );
				WRITE_COORD( pev->origin.z );

				// size
				WRITE_COORD( 200 );
				WRITE_COORD( 200 );
				WRITE_COORD( 128 );

				// velocity
				WRITE_COORD( 0 ); 
				WRITE_COORD( 0 );
				WRITE_COORD( 0 );

				// randomization
				WRITE_BYTE( 200 ); 

				// Model
				WRITE_SHORT( gGargGibModel );	//model id#

				// # of shards
				WRITE_BYTE( 50 );

				// duration
				WRITE_BYTE( 20 );// 3.0 seconds

				// flags

				WRITE_BYTE( BREAK_FLESH );
			MESSAGE_END();

			return;
		}
		else{
			//MODDD - notes
			//  This is the rest of the Gargantua's falling anim that is cutoff in retail.
			//  Includes the gargantua falling and potentially crushing whatever is unfortunate enough
			//  to be in front of him when he hits the ground, provided gargantuaKilledBoundsAssist is 1.
			
				//includes the usual way... no.
				//CBaseMonster::RunTask(pTask);
				
			if(!gargDeadBoundChangeYet && global_gargantuaKilledBoundsAssist==1 && pev->frame >= ((60.0/75.0)*255)){
				gargDeadBoundChangeYet = TRUE;



			/*
				float radz = this->pev->angles[1];
				*/
				Vector vecForward = UTIL_VecGetForward2D(pev->angles);

				//UTIL_printLineVector("EDDEDEDE", vecForward);
				//always bias slightly to the (relative) left (see yourself as facing the yaw)...

				Vector vecRight = CrossProduct(vecForward, Vector(0,0,1));

				Vector boundCenter = pev->origin + vecForward * 200 + vecRight * -17;


				//Perhaps we can slighlty adjust for being slightly longer than wide at floor-wise rotations?
				float theYaw = UTIL_VecToYawRadians(pev->angles);

				//apply the warp.  Base: sideways facing right.  Box is:

				//-80, -60, 0
				//80, 60, 0

				//...just go for it!

				//? = 60 + function(angle) * 20
				//OR
				//? = 60 + coordMod(angle)
				//coordMod = function(angle) * 20


				//range: 0.6 - 1.  At most no change... this steadily reduces the total size as it gets more square (remove that nasty corner space on a 45 degree rotated monster)
						

				//that is, M_PI / 4.0f    (90 degrees, in radians).
				//float halfRight = 0.7853981634f;

				//float totalModScalar = ((abs(cos(theYaw)) - 0.70710678f )) ;
				float totalModScalar =  (abs(cos(theYaw)) - abs(sin(theYaw)) )*0.45 + 0.65;



				float xMod = abs(cos(theYaw)) * 44 * totalModScalar;
				float yMod = abs(sin(theYaw)) * 44 * totalModScalar;
						
				float finalX = 49 + xMod;
				float finalY = 49 + yMod;


				//we have an idea of the place to fall now.
				
				CBaseEntity *pList[50];
				int count = UTIL_EntitiesInBox(
					pList,
					10, boundCenter + Vector(-finalX - 8, -finalY - 8, 0),
					boundCenter + Vector(finalX + 8, finalY + 8, 86),
					(FL_CLIENT|FL_MONSTER)
				);
				for(int i = 0; i < count; i++){
					if(pList[i]->pev != this->pev ){
						//Crushed by this guy? Insta-death.
						pList[i]->TakeDamage(this->pev, this->pev, 99999, DMG_ALWAYSGIB, 0);
					}

				}



				UTIL_SetSize(pev,
					(boundCenter + Vector(-finalX, -finalY, 0)) - pev->origin,
					(boundCenter + Vector(finalX, finalY, 86)) - pev->origin
				);

				/*
				//UTIL_drawBox(pev->origin  + Vector(0,0,0), pev->origin +  Vector(40, 40, 40));
				UTIL_drawBox(
					(boundCenter + Vector(-finalX, -finalY, 0)),
					(boundCenter + Vector(finalX, finalY, 86))
				);
				*/
				this->SetObjectCollisionBox();



			}else if ( m_fSequenceFinished && pev->frame >= 255 )
			{
				pev->health = 180;

				//MODDD - FOR DEBUGGING
				if(global_drawCollisionBoundsAtDeath == 1){
					UTIL_drawBox(pev->origin + pev->mins, pev->origin + pev->maxs);
				}
				if(global_drawHitBoundsAtDeath == 1){
					UTIL_drawBox(pev->absmin, pev->absmax);
				}
					

				pev->deadflag = DEAD_DEAD;
				StopAnimation();
				//easyPrintLine("DEAD: boxFlat?   %d", (BBoxFlat()));
				if(global_gargantuaKilledBoundsAssist == 0){
						
					if(global_animationKilledBoundsRemoval == 2){
						setPhysicalHitboxForDeath();
					}
					
				}else{

					
				}


				//MODDD - bound-altering script for death moved to "setPhyiscalHitboxForDeath" for better through by a CVar.
				if ( ShouldFadeOnDeath() ){
					// this monster was created by a monstermaker... fade the corpse out.
					SUB_StartFadeOut();
				}
				else{
					// body is gonna be around for a while, so have it stink for a bit.
					//...no, this is too big and tough for something to just start eating. No need.
					//CSoundEnt::InsertSound ( bits_SOUND_CARCASS, pev->origin, 384, 30 );
				}
				//an event.
				onDeathAnimationEnd();
			}




			//}else{

			//MODDDD - scrapped.
			

			/*
				if ( m_fSequenceFinished && pev->frame >= 255 )
				{
					EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "GARG DEAD!"));
					//set the size differently now.
					
					//MODDD - DEATH COLLISION BOX IDEA SCRAPPED.  Not possible with all the differnet rotations, as the main "hitbox" box itself cannot be rotated between 90 degree increments.
					
					//Vector forward;
					//UTIL_MakeVectorsPrivate( pev->angles, forward, NULL, NULL );
					//
					//
					//


					//UTIL_printLineVector("what is it", forward);

					////UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);
					////UTIL_SetSize(pev, Vector(80, -80, 0), Vector(350, 80, 200));


					//Vector pnt1 = UTIL_rotateShift( Vector(80, -80, 0), forward );
					//Vector pnt2 = UTIL_rotateShift( Vector(350, 80, 200), forward );

					//UTIL_SetSize(pev, pnt1, pnt2);





					//UTIL_drawBox(pev->origin + pev->mins, pev->origin + pev->maxs);
					////EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "ahhh %.2f %.2f %.2f, %.2f %.2f %.2f, %.2f %.2f %.2f", pev->origin.x, pev->origin.y, pev->origin.z, pev->mins.x, pev->mins.y, pev->mins.z, pev->maxs.x, pev->maxs.y, pev->maxs.z ));



					//UTIL_drawLine(pev->origin, pev->origin + Vector(0, 0, 40));
					//





					//UTIL_drawBox(pev->absmin, pev->absmax);



					//
					//pev->absmax = pev->origin + Vector(60, 60, 40);
					//pev->absmin = pev->origin + Vector(40, 40, 0);

					//UTIL_drawBox(pev->absmin, pev->absmax);
					//



				}
			*/




			//}//END OF else(...)
			




		}
		break;

	case TASK_FLAME_SWEEP:
		if ( gpGlobals->time > m_flWaitFinished )
		{
			FlameDestroy();
			TaskComplete();
			FlameControls( 0, 0 );
			SetBoneController( 0, 0 );
			SetBoneController( 1, 0 );
		}
		else
		{
			BOOL cancel = FALSE;

			Vector angles = g_vecZero;

			FlameUpdate();
			CBaseEntity *pEnemy = m_hEnemy;
			if ( pEnemy )
			{
				Vector org = pev->origin;
				org.z += 64;
				Vector dir = pEnemy->BodyTarget(org) - org;
				angles = UTIL_VecToAngles( dir );
				angles.x = -angles.x;
				angles.y -= pev->angles.y;
				if ( dir.Length() > 400 )
					cancel = TRUE;
			}
			if ( fabs(angles.y) > 60 )
				cancel = TRUE;
			
			if ( cancel )
			{
				m_flWaitFinished -= 0.5;
				m_flameTime -= 0.5;
			}
			// FlameControls( angles.x + 2 * sin(gpGlobals->time*8), angles.y + 28 * sin(gpGlobals->time*8.5) );
			FlameControls( angles.x, angles.y );
		}
		break;

	default:
		CBaseMonster::RunTask( pTask );
		break;
	}
}


class CSmoker : public CBaseEntity
{
public:
	void Spawn( void );
	void Think( void );
};

LINK_ENTITY_TO_CLASS( env_smoker, CSmoker );

void CSmoker::Spawn( void )
{
	pev->movetype = MOVETYPE_NONE;
	pev->nextthink = gpGlobals->time;
	pev->solid = SOLID_NOT;
	UTIL_SetSize(pev, g_vecZero, g_vecZero );
	pev->effects |= EF_NODRAW;
	pev->angles = g_vecZero;
}


void CSmoker::Think( void )
{
	// lots of smoke
	MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, pev->origin );
		WRITE_BYTE( TE_SMOKE );
		WRITE_COORD( pev->origin.x + RANDOM_FLOAT( -pev->dmg, pev->dmg ));
		WRITE_COORD( pev->origin.y + RANDOM_FLOAT( -pev->dmg, pev->dmg ));
		WRITE_COORD( pev->origin.z);
		WRITE_SHORT( g_sModelIndexSmoke );
		WRITE_BYTE( RANDOM_LONG(pev->scale, pev->scale * 1.1) );
		WRITE_BYTE( RANDOM_LONG(8,14)  ); // framerate
	MESSAGE_END();

	pev->health--;
	if ( pev->health > 0 )
		pev->nextthink = gpGlobals->time + RANDOM_FLOAT(0.1, 0.2);
	else
		UTIL_Remove( this );
}


void CSpiral::Spawn( void )
{
	pev->movetype = MOVETYPE_NONE;
	pev->nextthink = gpGlobals->time;
	pev->solid = SOLID_NOT;
	UTIL_SetSize(pev, g_vecZero, g_vecZero );
	pev->effects |= EF_NODRAW;
	pev->angles = g_vecZero;
}


CSpiral *CSpiral::Create( const Vector &origin, float height, float radius, float duration )
{
	if ( duration <= 0 )
		return NULL;

	CSpiral *pSpiral = GetClassPtr( (CSpiral *)NULL );
	pSpiral->Spawn();
	pSpiral->pev->dmgtime = pSpiral->pev->nextthink;
	pSpiral->pev->origin = origin;
	pSpiral->pev->scale = radius;
	pSpiral->pev->dmg = height;
	pSpiral->pev->speed = duration;
	pSpiral->pev->health = 0;
	pSpiral->pev->angles = g_vecZero;

	return pSpiral;
}

#define SPIRAL_INTERVAL		0.1 //025

void CSpiral::Think( void )
{
	float time = gpGlobals->time - pev->dmgtime;

	while ( time > SPIRAL_INTERVAL )
	{
		Vector position = pev->origin;
		Vector direction = Vector(0,0,1);
		
		float fraction = 1.0 / pev->speed;

		float radius = (pev->scale * pev->health) * fraction;

		position.z += (pev->health * pev->dmg) * fraction;
		pev->angles.y = (pev->health * 360 * 8) * fraction;
		UTIL_MakeVectors( pev->angles );
		position = position + gpGlobals->v_forward * radius;
		direction = (direction + gpGlobals->v_forward).Normalize();

		StreakSplash( position, Vector(0,0,1), RANDOM_LONG(8,11), 20, RANDOM_LONG(50,150), 400 );

		// Jeez, how many counters should this take ? :)
		pev->dmgtime += SPIRAL_INTERVAL;
		pev->health += SPIRAL_INTERVAL;
		time -= SPIRAL_INTERVAL;
	}

	pev->nextthink = gpGlobals->time;

	if ( pev->health >= pev->speed )
		UTIL_Remove( this );
}


// HACKHACK Cut and pasted from explode.cpp
void SpawnExplosion( Vector center, float randomRange, float time, int magnitude )
{
	KeyValueData	kvd;
	char			buf[128];

	center.x += RANDOM_FLOAT( -randomRange, randomRange );
	center.y += RANDOM_FLOAT( -randomRange, randomRange );

	CBaseEntity *pExplosion = CBaseEntity::Create( "env_explosion", center, g_vecZero, NULL );
	sprintf( buf, "%3d", magnitude );
	kvd.szKeyName = "iMagnitude";
	kvd.szValue = buf;
	pExplosion->KeyValue( &kvd );
	pExplosion->pev->spawnflags |= SF_ENVEXPLOSION_NODAMAGE;

	pExplosion->Spawn();
	pExplosion->SetThink( &CBaseEntity::SUB_CallUseToggle );
	pExplosion->pev->nextthink = gpGlobals->time + time;
}



//MODDD - new method for determining whether to register a case of damage as worthy of LIGHT_DAMAGE or HEAVY_DAMAGE for the AI.
//        Can result in interrupting the current schedule.
//        This is expected to get called from CBaseMonster's TakeDamage method in combat.cpp. This method may be customized per monster,
//        should start with a copy of this method without calling the parent method, not much here.
void CGargantua::OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod){

	//MODDD - intervention. Timed damage might not affect the AI since it could get needlessly distracting.


	

	if(bitsDamageTypeMod & (DMG_TIMEDEFFECT|DMG_TIMEDEFFECTIGNORE) ){
		//If this is continual timed damage, don't register as any damage condition. Not worth possibly interrupting the AI.
		return;
	}

	
	//default case from CBaseMonster's TakeDamage.
	//Also count being in a non-combat state to force looking in that direction.
	if(m_MonsterState == MONSTERSTATE_IDLE || m_MonsterState == MONSTERSTATE_ALERT || flDamage > 20 )
	{
		SetConditions(bits_COND_LIGHT_DAMAGE);

		//MODDD NEW - set a timer to forget a flinch-preventing memory bit.
		forgetSmallFlinchTime = gpGlobals->time + DEFAULT_FORGET_SMALL_FLINCH_TIME;
	}

	//MODDD - HEAVY_DAMAGE was unused before.  For using the BIG_FLINCH activity that is (never got communicated)
	//    Stricter requirement:  this attack took 70% of health away.
	//    The agrunt used to use this so that its only flinch was for heavy damage (above 20 in one attack), but that's easy by overriding this OnTakeDamageSetconditions method now.
	//    Keep it to using light damage for that instead.
	//if ( flDamage >= 20 )


	
	if(gpGlobals->time >= forgetBigFlinchTime && (flDamage >=  pev->max_health * 0.55 || flDamage >= 70) )
	{
		SetConditions(bits_COND_HEAVY_DAMAGE);
		forgetSmallFlinchTime = gpGlobals->time + DEFAULT_FORGET_SMALL_FLINCH_TIME*2.3;
		forgetBigFlinchTime = gpGlobals->time + DEFAULT_FORGET_BIG_FLINCH_TIME*2.2;
	}


	if(EASY_CVAR_GET(testVar) == 10){
		//any damage causes me now.
		SetConditions(bits_COND_HEAVY_DAMAGE);
	}

	easyForcePrintLine("%s:%d OnTkDmgSetCond raw:%.2f fract:%.2f", getClassname(), monsterID, flDamage, (flDamage / pev->max_health));



}//END OF OnTakeDamageSetConditions







BOOL CGargantua::usesAdvancedAnimSystem(void){
	return TRUE;
}




int CGargantua::LookupActivityHard(int activity){
	int i = 0;
	m_flFramerateSuggestion = 1;
	pev->framerate = 1;
	//any animation events in progress?  Clear it.
	resetEventQueue();

	switch(activity){
		case ACT_SMALL_FLINCH:
		case ACT_BIG_FLINCH:			//MODDD NOTE - is this effective?
		case ACT_FLINCH_HEAD:
		case ACT_FLINCH_CHEST:
		case ACT_FLINCH_STOMACH:
		case ACT_FLINCH_LEFTARM:
		case ACT_FLINCH_RIGHTARM:
		case ACT_FLINCH_LEFTLEG:
		case ACT_FLINCH_RIGHTLEG:
			if(g_iSkillLevel == SKILL_EASY){
				m_flFramerateSuggestion = 1.1;
			}else if(g_iSkillLevel == SKILL_MEDIUM){
				m_flFramerateSuggestion = 1.5;
			}else if(g_iSkillLevel == SKILL_HARD){
				m_flFramerateSuggestion = 1.8;
			}
		break;
	}//END OF switch(...)
	
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}//END OF LookupActivityHard(...)


int CGargantua::tryActivitySubstitute(int activity){
	int i = 0;

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		case ACT_RUN:

		break;
	}//END OF switch(...)


	//not handled by above? We're not using the script to determine animation then. Rely on the model's anim for this activity if there is one.
	return CBaseAnimating::LookupActivity(activity);
}//END OF tryActivitySubstitute(...)



int CGargantua::getHullIndexForNodes(void){
    return NODE_LARGE_HULL;  //...ya think?
}





#endif




