
#include "kingPin.h"


extern float global_animationKilledBoundsRemoval;
extern float global_thoroughHitBoxUpdates;

extern float global_noFlinchOnHard;













//MODDD - ripped right from the controller.
//=========================================================
// Controller bouncy ball attack
//=========================================================
class CKingpinBall : public CBaseMonster
{
	//MODDD - why no public?
public:
	CKingpinBall();

	void Spawn( void );
	void Precache( void );
	void EXPORT HuntThink( void );
	void EXPORT DieThink( void );
	void EXPORT BounceTouch( CBaseEntity *pOther );
	void MovetoTarget( Vector vecTarget );
	void Crawl( void );
	int m_iTrail;
	int m_flNextAttack;
	Vector m_vecIdeal;
	EHANDLE m_hOwner;
};
LINK_ENTITY_TO_CLASS( kingpin_ball, CKingpinBall );



CKingpinBall::CKingpinBall(){

}

void CKingpinBall :: Spawn( void )
{
	Precache( );
	// motor
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	SET_MODEL(ENT(pev), "sprites/xspark4.spr");
	pev->rendermode = kRenderTransAdd;
	pev->rendercolor.x = 255;
	pev->rendercolor.y = 255;
	pev->rendercolor.z = 255;
	pev->renderamt = 255;
	pev->scale = 2.0;

	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));
	UTIL_SetOrigin( pev, pev->origin );

	SetThink( &CKingpinBall::HuntThink );
	SetTouch( &CKingpinBall::BounceTouch );

	m_vecIdeal = Vector( 0, 0, 0 );

	pev->nextthink = gpGlobals->time + 0.1;

	m_hOwner = Instance( pev->owner );
	pev->dmgtime = gpGlobals->time;
}


extern int global_useSentenceSave;
void CKingpinBall :: Precache( void )
{

	PRECACHE_MODEL("sprites/xspark1.spr");
	
	global_useSentenceSave = TRUE;
	PRECACHE_SOUND("weapons/electro4.wav", TRUE);//don't skip. This is precached by the player gauss, just keep it.
	PRECACHE_SOUND("debris/zap4.wav");
	global_useSentenceSave = FALSE;

}


void CKingpinBall :: HuntThink( void  )
{
	pev->nextthink = gpGlobals->time + 0.1;

	pev->renderamt -= 5;

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_ELIGHT );
		WRITE_SHORT( entindex( ) );		// entity, attachment
		WRITE_COORD( pev->origin.x );		// origin
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z );
		WRITE_COORD( pev->renderamt / 16 );	// radius
		WRITE_BYTE( 255 );	// R
		WRITE_BYTE( 255 );	// G
		WRITE_BYTE( 255 );	// B
		WRITE_BYTE( 2 );	// life * 10
		WRITE_COORD( 0 ); // decay
	MESSAGE_END();

	// check world boundaries
	if (gpGlobals->time - pev->dmgtime > 5 || pev->renderamt < 64 || m_hEnemy == NULL || m_hOwner == NULL || pev->origin.x < -4096 || pev->origin.x > 4096 || pev->origin.y < -4096 || pev->origin.y > 4096 || pev->origin.z < -4096 || pev->origin.z > 4096)
	{
		SetTouch( NULL );
		UTIL_Remove( this );
		return;
	}

	MovetoTarget( m_hEnemy->Center( ) );

	if ((m_hEnemy->Center() - pev->origin).Length() < 64)
	{
		TraceResult tr;

		UTIL_TraceLine( pev->origin, m_hEnemy->Center(), dont_ignore_monsters, ENT(pev), &tr );

		CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
		if (pEntity != NULL && pEntity->pev->takedamage)
		{
			ClearMultiDamage( );
			pEntity->TraceAttack( m_hOwner->pev, gSkillData.controllerDmgZap, pev->velocity, &tr, DMG_SHOCK );
			ApplyMultiDamage( pev, m_hOwner->pev );
		}

		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_BEAMENTPOINT );
			WRITE_SHORT( entindex() );
			WRITE_COORD( tr.vecEndPos.x );
			WRITE_COORD( tr.vecEndPos.y );
			WRITE_COORD( tr.vecEndPos.z );
			WRITE_SHORT( g_sModelIndexLaser );
			WRITE_BYTE( 0 ); // frame start
			WRITE_BYTE( 10 ); // framerate
			WRITE_BYTE( 3 ); // life
			WRITE_BYTE( 20 );  // width
			WRITE_BYTE( 0 );   // noise
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 255 );   // r, g, b
			WRITE_BYTE( 255 );	// brightness
			WRITE_BYTE( 10 );		// speed
		MESSAGE_END();


		UTIL_EmitAmbientSound_Filtered( ENT(pev), tr.vecEndPos, "weapons/electro4.wav", 0.5, ATTN_NORM, 0, RANDOM_LONG( 140, 160 ), FALSE );

		m_flNextAttack = gpGlobals->time + 3.0;

		SetThink( &CKingpinBall::DieThink );
		pev->nextthink = gpGlobals->time + 0.3;
	}

	// Crawl( );
}


void CKingpinBall :: DieThink( void  )
{
	UTIL_Remove( this );
}


void CKingpinBall :: MovetoTarget( Vector vecTarget )
{
	// accelerate
	float flSpeed = m_vecIdeal.Length();
	if (flSpeed == 0)
	{
		m_vecIdeal = pev->velocity;
		flSpeed = m_vecIdeal.Length();
	}

	//MODDD - has speed 600 instead of 400.
	if (flSpeed > 600)
	{
		m_vecIdeal = m_vecIdeal.Normalize( ) * 600;
	}
	m_vecIdeal = m_vecIdeal + (vecTarget - pev->origin).Normalize() * 100;
	pev->velocity = m_vecIdeal;
}



void CKingpinBall :: Crawl( void  )
{

	Vector vecAim = Vector( RANDOM_FLOAT( -1, 1 ), RANDOM_FLOAT( -1, 1 ), RANDOM_FLOAT( -1, 1 ) ).Normalize( );
	Vector vecPnt = pev->origin + pev->velocity * 0.3 + vecAim * 64;

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_BEAMENTPOINT );
		WRITE_SHORT( entindex() );
		WRITE_COORD( vecPnt.x);
		WRITE_COORD( vecPnt.y);
		WRITE_COORD( vecPnt.z);
		WRITE_SHORT( g_sModelIndexLaser );
		WRITE_BYTE( 0 ); // frame start
		WRITE_BYTE( 10 ); // framerate
		WRITE_BYTE( 3 ); // life
		WRITE_BYTE( 20 );  // width
		WRITE_BYTE( 0 );   // noise
		WRITE_BYTE( 255 );   // r, g, b
		WRITE_BYTE( 255 );   // r, g, b
		WRITE_BYTE( 255 );   // r, g, b
		WRITE_BYTE( 255 );	// brightness
		WRITE_BYTE( 10 );		// speed
	MESSAGE_END();
}


void CKingpinBall::BounceTouch( CBaseEntity *pOther )
{
	Vector vecDir = m_vecIdeal.Normalize( );

	TraceResult tr = UTIL_GetGlobalTrace( );

	float n = -DotProduct(tr.vecPlaneNormal, vecDir);

	vecDir = 2.0 * tr.vecPlaneNormal * n + vecDir;

	m_vecIdeal = vecDir * m_vecIdeal.Length();
}




























///////////////////////////////////////////////////////////////////////////////////




const char* CKingPin::pDeathSounds[] = 
{
	"kingpin/kingpin_death.wav",
};
const char* CKingPin::pAlertSounds[] = 
{
	"kingpin/kingpin_alert.wav",
};

const char* CKingPin::pIdleSounds[] = 
{
	"kingpin/kingpin_idle.wav",
};
const char* CKingPin::pPainSounds[] = 
{
	"kingpin/kingpin_pain.wav",
};
const char* CKingPin::pAttackSounds[] = 
{
	"kingpin/kingpin_attack.wav",
};
const char* CKingPin::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};
const char* CKingPin::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};






/*
TYPEDESCRIPTION	CKingPin::m_SaveData[] = 
{
	
};

//IMPLEMENT_SAVERESTORE( CKingPin, CBaseMonster );
int CKingPin::Save( CSave &save )
{
	if ( !CBaseMonster::Save(save) )
		return 0;
	int iWriteFieldsResult = save.WriteFields( "CKingPin", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iWriteFieldsResult;
}
int CKingPin::Restore( CRestore &restore )
{
	if ( !CBaseMonster::Restore(restore) )
		return 0;
	int iReadFieldsResult = restore.ReadFields( "CKingPin", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iReadFieldsResult;
}
*/




#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_kingpin, CKingPin );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( kingpin, CKingPin );
	
	#if EXTRA_NAMES == 2
		LINK_ENTITY_TO_CLASS( king, CKingPin );
	#endif
	
#endif



CKingPin::CKingPin(void){
	
	powerUpNearbyMonstersCooldown = -1;
	forceEnemyOnPoweredUpMonstersCooldown = -1;
	forceEnemyOnPoweredUpMonstersHardCooldown = -1;
	forgetRecentInflictingMonsterCooldown = -1;
	recentInflictingMonster = NULL;

}//END OF CKingPin constructor




//schedule details here......


Task_t	tlKingpinRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	//{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_KINGPIN_PSIONIC_CHARGE, (float)0},
	{ TASK_KINGPIN_PSIONIC_LAUNCH, (float)0},
};

Schedule_t	slKingpinRangeAttack1[] =
{
	{ 
		tlKingpinRangeAttack1,
		ARRAYSIZE ( tlKingpinRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Kingpin Range Attack1"
	},
};








/*
Task_t	tlKingPinXXX[] =
{
	{ TASK_KINGPIN_XXX,			0				},
	{ TASK_KINGPIN_YYY,			0				},
	{ TASK_KINGPIN_ZZZ,			0				},
};

Schedule_t	slKingPinXXX[] =
{
	{
		tlKingPinXXX,
		ARRAYSIZE ( tlKingPinXXX ),
		bits_COND_XXX | bits_COND_YYY | bits_COND_ZZZ,
		bits_SOUND_XXX | bits_SOUND_YYY | bits_SOUND_ZZZ,
		"kingPinXXX"
	},
};

//repeat for tl / sl KingPinYYY and tl / sl KingPinZZZ.


*/



DEFINE_CUSTOM_SCHEDULES( CKingPin )
{
	slKingpinRangeAttack1,

};
IMPLEMENT_CUSTOM_SCHEDULES( CKingPin, CBaseMonster );



	

//MODDD - sound calls dummied out until they actually exist.
void CKingPin::DeathSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1.0, ATTN_IDLE, 0, pitch );
}
void CKingPin::AlertSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAlertSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CKingPin::IdleSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random idle sound
	//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CKingPin::PainSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	if (RANDOM_LONG(0,5) < 2){
		//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1.0, ATTN_NORM, 0, pitch );
	}
}
void CKingPin::AttackSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random attack sound
	//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAttackSounds), 1.0, ATTN_NORM, 0, pitch );
}



extern int global_useSentenceSave;
void CKingPin::Precache( void )
{
	PRECACHE_MODEL("models/kingpin.mdl");

	global_useSentenceSave = TRUE;
	
	//PRECACHE_SOUND("kingpin/kingpin_XXX.wav");
	
	//NOTICE - attempting to precace files that don't exist crashes the game.
	/*
	PRECACHE_SOUND_ARRAY(pDeathSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	*/
	PRECACHE_SOUND_ARRAY(pAttackHitSounds);
	PRECACHE_SOUND_ARRAY(pAttackMissSounds);



	//for now...
	PRECACHE_SOUND("houndeye/he_blast1.wav");
	PRECACHE_SOUND("houndeye/he_blast2.wav");
	PRECACHE_SOUND("houndeye/he_blast3.wav");

	

	//For the KingpinBall.
	/////////////////////////////////////////////////////////
	PRECACHE_MODEL("sprites/xspark1.spr");
	PRECACHE_MODEL("sprites/xspark4.spr");
	
	global_useSentenceSave = TRUE;
	PRECACHE_SOUND("weapons/electro4.wav", TRUE);//don't skip. This is precached by the player gauss, just keep it.
	PRECACHE_SOUND("debris/zap4.wav");
	global_useSentenceSave = FALSE;
	/////////////////////////////////////////////////////////




	global_useSentenceSave = FALSE;
}//END OF Precache()





void CKingPin::Spawn( void )
{
	Precache( );
	
	//model seems kinda off center. may be ok? unsure.
	SET_MODEL(ENT(pev), "models/kingpin.mdl");
	UTIL_SetSize(pev, Vector(-36, -36, 0), Vector(36, 36, 72));

	pev->classname = MAKE_STRING("monster_kingpin");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->effects		= 0;
	pev->health			= gSkillData.kingpinHealth;
	pev->view_ofs		= Vector ( 0, 0, 20 );// position of the eyes relative to monster's origin.
	pev->yaw_speed		= 5;//!!! should we put this in the monster's changeanim function since turn rates may vary with state/anim?
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	MonsterInit();





	//SetTouch(&CKingPin::CustomTouch );
	//SetTouch( NULL );

}//END OF Spawn();


//based off of GetSchedule for CBaseMonster in schedule.cpp.
Schedule_t* CKingPin::GetSchedule ( void )
{
	//MODDD - safety.
	if(iAmDead){
		return GetScheduleOfType( SCHED_DIE );
	}
	SCHEDULE_TYPE baitSched = getHeardBaitSoundSchedule();

	if(baitSched != SCHED_NONE){
		return GetScheduleOfType ( baitSched );
	}

	switch	( m_MonsterState )
	{
	case MONSTERSTATE_PRONE:
		{
			return GetScheduleOfType( SCHED_BARNACLE_VICTIM_GRAB );
			break;
		}
	case MONSTERSTATE_NONE:
		{
			ALERT ( at_aiconsole, "MONSTERSTATE IS NONE!\n" );
			break;
		}
	case MONSTERSTATE_IDLE:
		{
			if ( HasConditions ( bits_COND_HEAR_SOUND ) )
			{
				return GetScheduleOfType( SCHED_ALERT_FACE );
			}
			else if ( FRouteClear() )
			{
				// no valid route!
				return GetScheduleOfType( SCHED_IDLE_STAND );
			}
			else
			{
				// valid route. Get moving
				return GetScheduleOfType( SCHED_IDLE_WALK );
			}
			break;
		}
	case MONSTERSTATE_ALERT:
		{
			if ( HasConditions( bits_COND_ENEMY_DEAD ) && LookupActivity( ACT_VICTORY_DANCE ) != ACTIVITY_NOT_AVAILABLE )
			{
				return GetScheduleOfType ( SCHED_VICTORY_DANCE );
			}

			if ( HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE) )
			{
				if ( fabs( FlYawDiff() ) < (1.0 - m_flFieldOfView) * 60 ) // roughly in the correct direction
				{
					return GetScheduleOfType( SCHED_TAKE_COVER_FROM_ORIGIN );
				}
				else
				{
					return GetScheduleOfType( SCHED_ALERT_SMALL_FLINCH );
				}
			}

			else if ( HasConditions ( bits_COND_HEAR_SOUND ) )
			{
				return GetScheduleOfType( SCHED_ALERT_FACE );
			}
			else
			{
				return GetScheduleOfType( SCHED_ALERT_STAND );
			}
			break;
		}
	case MONSTERSTATE_COMBAT:
		{

			//Check. Are there agrunts nearby to power up? If none are close enough to power up but aren't much further, route to them and
			//then power themn up.
			//NO - it is determined the powerup & focus on enemy will be passive instead. Don't happen just because this monster has
			//nothing else to do or require an active interaction like an anim.
			/*
			CBaseMonster* closestPowerupableMonster = findClosestPowerupableMonster();

			float distanceToClosest = (closestPowerupableMonster->pev->origin - this->pev->origin).Length();

			if(closestPowerupableMonster){
				//Assessment. How far away is this monster?
				if(distanceToClosest < POWERUP_APPLY_RANGE){
					//start the powerup schedule now!
					return GetScheduleOfType(SCHED_KINGPIN_POWERUP);
				}else if(distanceToClosest < POWERUP_SEARCH_RANGE){
					//get closer to this monster, then power up!
					this->m_hTargetEnt = closestPowerupableMonster;
					return GetScheduleOfType(SCHED_KINGPIN_MOVE_TO_POWERUP);
				}

			}//END OF possible schedule change (power-up) based on how close a powerup-able monster is

			*/


			if ( HasConditions( bits_COND_ENEMY_DEAD ) )
			{
				// clear the current (dead) enemy and try to find another.
				m_hEnemy = NULL;

				if ( GetEnemy() )
				{
					ClearConditions( bits_COND_ENEMY_DEAD );
					return GetSchedule();
				}
				else
				{
					SetState( MONSTERSTATE_ALERT );
					return GetSchedule();
				}
			}

			if ( HasConditions(bits_COND_NEW_ENEMY) )
			{
				return GetScheduleOfType ( SCHED_WAKE_ANGRY );
			}
			//MODDD - other condition.  If "noFlinchOnHard" is on and the skill is hard, don't flinch from getting hit.
			else if (HasConditions(bits_COND_LIGHT_DAMAGE) && !HasMemory( bits_MEMORY_FLINCHED) && !(global_noFlinchOnHard==1 && g_iSkillLevel==SKILL_HARD)  )
			{
				return GetScheduleOfType( SCHED_SMALL_FLINCH );
			}
			else if ( !HasConditions(bits_COND_SEE_ENEMY) )
			{
				// we can't see the enemy
				if ( !HasConditions(bits_COND_ENEMY_OCCLUDED) )
				{
					// enemy is unseen, but not occluded!
					// turn to face enemy
					return GetScheduleOfType( SCHED_COMBAT_FACE );
				}
				else
				{
					// chase!
					//easyPrintLine("ducks??");
					return GetScheduleOfType( SCHED_CHASE_ENEMY );
				}
			}
			else  
			{

				//easyPrintLine("I say, really now? %d %d", HasConditions(bits_COND_CAN_RANGE_ATTACK1), HasConditions(bits_COND_CAN_RANGE_ATTACK2) );

				// we can see the enemy
				if ( HasConditions(bits_COND_CAN_RANGE_ATTACK1) )
				{
					return GetScheduleOfType( SCHED_RANGE_ATTACK1 );
				}
				if ( HasConditions(bits_COND_CAN_RANGE_ATTACK2) )
				{
					return GetScheduleOfType( SCHED_RANGE_ATTACK2 );
				}
				if ( HasConditions(bits_COND_CAN_MELEE_ATTACK1) )
				{
					return GetScheduleOfType( SCHED_MELEE_ATTACK1 );
				}
				if ( HasConditions(bits_COND_CAN_MELEE_ATTACK2) )
				{
					return GetScheduleOfType( SCHED_MELEE_ATTACK2 );
				}
				//MODDD - NOTE - is that intentional?  range1 & melee1,  and not say,  melee1 & melee2???
				if ( !HasConditions(bits_COND_CAN_RANGE_ATTACK1 | bits_COND_CAN_MELEE_ATTACK1) )
				{
					// if we can see enemy but can't use either attack type, we must need to get closer to enemy
					return GetScheduleOfType( SCHED_CHASE_ENEMY );
				}
				else if ( !FacingIdeal() )
				{
					//turn
					return GetScheduleOfType( SCHED_COMBAT_FACE );
				}
				else
				{
					ALERT ( at_aiconsole, "No suitable combat schedule!\n" );
				}
			}
			break;
		}
	case MONSTERSTATE_DEAD:
		{
			return GetScheduleOfType( SCHED_DIE );
			break;
		}
	case MONSTERSTATE_SCRIPT:
		{
			//
			//ASSERT( m_pCine != NULL );

			if(m_pCine == NULL){
				easyPrintLine("WARNING: m_pCine IS NULL!");
			}

			if ( !m_pCine )
			{
				ALERT( at_aiconsole, "Script failed for %s\n", STRING(pev->classname) );
				CineCleanup();
				return GetScheduleOfType( SCHED_IDLE_STAND );
			}

			return GetScheduleOfType( SCHED_AISCRIPT );
		}
	default:
		{
			ALERT ( at_aiconsole, "Invalid State for GetSchedule!\n" );
			break;
		}
	}

	return &slError[ 0 ];
}//END OF GetSchedule()


Schedule_t* CKingPin::GetScheduleOfType( int Type){
	
	switch(Type){
		case SCHED_KINGPIN_POWERUP:
			
			//do this animation.
			this->SetSequenceByIndex(KINGPIN_POWERUP);
			//no schedules yet??
		break;
		case SCHED_KINGPIN_MOVE_TO_POWERUP:
			//... what?
		break;
		case SCHED_RANGE_ATTACK1:
			return slKingpinRangeAttack1;
		break;
	}//END OF switch(Type)
	
	return CBaseMonster::GetScheduleOfType(Type);
}//END OF GetScheduleOfType(...)



void CKingPin::StartTask( Task_t *pTask ){


	switch( pTask->iTask ){
		
		case TASK_KINGPIN_PSIONIC_CHARGE:
		{
			
			if(m_hEnemy == NULL){
				//stop?
				TaskFail();
				return;
			}


			//set animation..?
			this->SetSequenceByIndex(KINGPIN_PSIONIC_CHARGE, 0.8f);


		break;
		}
		case TASK_KINGPIN_PSIONIC_LAUNCH:
		{

			if(m_hEnemy == NULL){
				//stop?
				TaskFail();
				return;
			}

			m_IdealActivity = ACT_RANGE_ATTACK1;
			signalActivityUpdate = TRUE;

		break;
		}
		default:
			CBaseMonster::StartTask( pTask );
		break;
	}//END OF switch(...)

}//END OF StartTask(...)

void CKingPin::RunTask( Task_t *pTask ){
	
	//EASY_CVAR_PRINTIF_PRE(templatePrintout, easyPrintLine("RunTask: sched:%s task:%d", this->m_pSchedule->pName, pTask->iTask) );
	
	switch( pTask->iTask ){
		case TASK_KINGPIN_PSIONIC_CHARGE:

			//ripped houndeye.
			float life;
			life = ((255 - pev->frame) / (pev->framerate * m_flFrameRate));
			if (life < 0.1) life = 0.1;


			MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, pev->origin );
				WRITE_BYTE(  TE_IMPLOSION);
				WRITE_COORD( pev->origin.x);
				WRITE_COORD( pev->origin.y);
				WRITE_COORD( pev->origin.z + 16);
				WRITE_BYTE( 50 * life + 100);  // radius
				WRITE_BYTE( pev->frame / 25.0 ); // count
				WRITE_BYTE( life * 10 ); // life
			MESSAGE_END();
		
			if(this->m_fSequenceFinished){
				TaskComplete();
			}
			

		break;
		case TASK_KINGPIN_PSIONIC_LAUNCH:

			if(this->m_fSequenceFinished){
				TaskComplete();
			}

		break;
		default:
			CBaseMonster::RunTask(pTask);
		break;
	}//END OF switch(...)

}//END OF RunTask(...)



BOOL CKingPin::CheckMeleeAttack1( float flDot, float flDist ){
	

	if ( flDist <= 72 && flDot >= 0.75  ) //&& FBitSet ( m_hEnemy->pev->flags, FL_ONGROUND ) )
	{
		return TRUE;
	}
	return FALSE;



}//END OF CheckMeleeAttack1

BOOL CKingPin::CheckMeleeAttack2( float flDot, float flDist ){
	return FALSE;
}//END OF CheckMeleeAttack2


BOOL CKingPin::CheckRangeAttack1( float flDot, float flDist ){
	

	
	if ( flDist > 72 && flDist <= 2048 && flDot >= 0.5 )
	{
		return TRUE;
	}
	
	return FALSE;



}//END OF CheckRangeAttack1

BOOL CKingPin::CheckRangeAttack2( float flDot, float flDist ){
	return FALSE;
}//END OF CheckRangeAttack2



void CKingPin::CustomTouch( CBaseEntity *pOther ){

}



void CKingPin::MonsterThink( void ){


	if(m_MonsterState == MONSTERSTATE_COMBAT){
		//Periodically power up monstesr and make them target my enemy.

		if(powerUpNearbyMonstersCooldown == -1 || gpGlobals->time >= powerUpNearbyMonstersCooldown){
			easyForcePrintLine("KINGPIN: I licked THE COUCH");
			this->powerUpMonsters();
			powerUpNearbyMonstersCooldown = gpGlobals->time + 8;
		}

		if( 
			//that is, if a monster recently damaged me, or our long-time cooldown is up....
			(  recentInflictingMonster != NULL || (m_hEnemy != NULL && (forceEnemyOnPoweredUpMonstersCooldown == -1 || gpGlobals->time >= forceEnemyOnPoweredUpMonstersCooldown))  ) &&

			//And the hard cooldown (to avoid spamming the logic) passes:
			(forceEnemyOnPoweredUpMonstersHardCooldown == -1 || gpGlobals->time >= forceEnemyOnPoweredUpMonstersHardCooldown) )
		{

			CBaseEntity* enemyToForce;
			BOOL forceEnemyPassive;

			if(recentInflictingMonster != NULL){
				enemyToForce = recentInflictingMonster;
				forceEnemyPassive = FALSE;
			}else{
				enemyToForce = m_hEnemy;
				forceEnemyPassive = TRUE;
			}


			this->forceEnemyOnPoweredUpMonsters(enemyToForce, forceEnemyPassive);

			forceEnemyOnPoweredUpMonstersCooldown = gpGlobals->time + 15;
			forceEnemyOnPoweredUpMonstersHardCooldown = gpGlobals->time + 2.5;
		}


		if(forgetRecentInflictingMonsterCooldown != -1 && gpGlobals->time >= forgetRecentInflictingMonsterCooldown){
			recentInflictingMonster = NULL;
			forgetRecentInflictingMonsterCooldown = -1;
		}



	}//END OF combat state check



	


	CBaseMonster::MonsterThink();
}//END OF MonsterThink(...)




int	CKingPin::Classify( void ){
	return CLASS_ALIEN_MONSTER;
}
BOOL CKingPin::isOrganic(void){
	return TRUE;
}

int CKingPin::IRelationship( CBaseEntity *pTarget ){

	return CBaseMonster::IRelationship(pTarget);
}//END OF IRelationship(...)


void CKingPin::ReportAIState(void){
	//call the parent, and add on to that.
	CBaseMonster::ReportAIState();
	//print anything special with easyForcePrintLine
}//END OF ReportAIState()






GENERATE_TRACEATTACK_IMPLEMENTATION(CKingPin){

	easyForcePrintLine("kingpin ID%d: I WAS JUST HIT AT HITBOX:%d", this->monsterID, ptr->iHitgroup);



	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CKingPin){


	//Alert poweredupmonsters of this occurence in MonsterThink by the "recentlyDamage" feature!
	if(this->pev->deadflag == DEAD_NO && pevInflictor != NULL){
		CBaseEntity* entityTest = CBaseEntity::Instance(pevInflictor);
		//easyForcePrintLine("WELL???? %d %d", (entityTest != NULL), (CBaseEntity::Instance(pevAttacker) != NULL) );
		if(entityTest != NULL){
			CBaseMonster* monsterTest = entityTest->GetMonsterPointer();
			if(monsterTest != NULL && IRelationship(monsterTest) >= R_DL){
				//Require disliking the attacking monster in the first place in case this was just friendly fire.
				recentInflictingMonster = monsterTest;
				forgetRecentInflictingMonsterCooldown = gpGlobals->time + 4;
			}
		}
	}


	

	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}

GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION(CKingPin){
	
	
	return GENERATE_DEADTAKEDAMAGE_PARENT_CALL(CBaseMonster);
}


//Parameters: integer named fGibSpawnsDecal
GENERATE_GIBMONSTER_IMPLEMENTATION(CKingPin)
{


	GENERATE_GIBMONSTER_PARENT_CALL(CBaseMonster);
}





//if dead and a poweredup monster's directedEnemyIssuer is THIS, forget its directedEnemy. !!!
GENERATE_KILLED_IMPLEMENTATION(CKingPin){

	deTargetMyCommandedMonsters();


	BOOL firstCall = FALSE;
	if(pev->deadflag == DEAD_NO){
		//keep this in mind...
		firstCall = TRUE;
	}

	//MODDD - is still doing here ok?
	GENERATE_KILLED_PARENT_CALL(CBaseMonster);

	//if you have the "FL_KILLME" flag, it means this is about to get deleted (gibbed). No point in doing any of this then.
	if(firstCall && !(pev->flags & FL_KILLME) ){
		cheapKilled();
	}//END OF firstCall check
	
	

	

}//END OF Killed



void CKingPin::SetYawSpeed( void ){
	int ys;

	ys = 200;
	pev->yaw_speed = ys;
	return;
}//END OF SetYawSpeed(...)





BOOL CKingPin::getMonsterBlockIdleAutoUpdate(void){
	return FALSE;
}
BOOL CKingPin::forceIdleFrameReset(void){
	return FALSE;
}
BOOL CKingPin::usesAdvancedAnimSystem(void){
	return TRUE;
}

void CKingPin::SetActivity( Activity NewActivity ){
	CBaseMonster::SetActivity(NewActivity);
}



int CKingPin::LookupActivityHard(int activity){
	int i = 0;
	m_flFramerateSuggestion = 1;
	m_iForceLoops = -1;
	pev->framerate = 1;
	//any animation events in progress?  Clear it.
	resetEventQueue();

	//Within an ACTIVITY, pick an animation like this (with whatever logic / random check first):
	//    this->animEventQueuePush(10.0f / 30.0f, 3);  //Sets event #3 to happen at 1/3 of a second
	//    return LookupSequence("die_backwards");      //will play animation die_backwards

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		case ACT_IDLE:
			//return CBaseAnimating::LookupActivity(activity);
			return KINGPIN_IDLE;
		break;
		case ACT_MELEE_ATTACK1:
			//pev->flags &= ~EF_NOINTERP;
			//pev->effects |= EF_NOINTERP;
			m_flFramerateSuggestion = 6;
			this->animEventQueuePush(30.0f/22.0f, 0);
			this->m_iForceLoops = 0;
			//pev->framerate = 6;
			return KINGPIN_SCYTHE;
		break;
		case ACT_RANGE_ATTACK1:
			m_flFramerateSuggestion = 3;
			this->animEventQueuePush(60.0f/22.0f, 1);
			return KINGPIN_PSIONIC_LAUNCH;
		break;
		case ACT_DIESIMPLE:
			return KINGPIN_DIE;
		break;
	}//END OF switch(...)
	
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}//END OF LookupActivityHard(...)


int CKingPin::tryActivitySubstitute(int activity){
	int i = 0;

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		case ACT_IDLE:
			//return CBaseAnimating::LookupActivity(activity);
			return KINGPIN_IDLE;
		break;
		case ACT_MELEE_ATTACK1:
			return KINGPIN_SCYTHE;
		break;
		case ACT_RANGE_ATTACK1:
			return KINGPIN_PSIONIC_LAUNCH;
		break;
		case ACT_DIESIMPLE:
			return KINGPIN_DIE;
		break;
	}//END OF switch(...)


	//not handled by above? We're not using the script to determine animation then. Rely on the model's anim for this activity if there is one.
	return CBaseAnimating::LookupActivity(activity);
}//END OF tryActivitySubstitute(...)

//Handles custom events sent from "LookupActivityHard", which sends events as timed delays along with picking an animation in script.
//So this handles script-provided events, not model ones.
void CKingPin::HandleEventQueueEvent(int arg_eventID){

	switch(arg_eventID){
	case 0:
	{
		//scythe attack.
		//TODO - custom damage for the kingpin's attacks?
		CBaseEntity *pHurt = CheckTraceHullAttack( 74, gSkillData.zombieDmgBothSlash, DMG_SLASH, DMG_BLEEDING );
		if ( pHurt )
		{
			if ( pHurt->pev->flags & (FL_MONSTER|FL_CLIENT) )
			{
				pHurt->pev->punchangle.z = -18;
				pHurt->pev->punchangle.x = 5;
				//pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_forward * 40 + gpGlobals->v_up * 23 - gpGlobals->v_right * 15;
			}
			// Play a random attack hit sound
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
		}
		else // Play a random attack miss sound
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );

		if (RANDOM_LONG(0,1))
			AttackSound();

	break;
	}
	case 1:
	{
		//psionic launch (actually launch a ball)

		
		if(m_hEnemy == NULL){
			//stop?
			TaskFail();
			return;
		}

			
		//m_IdealActivity = ACT_RANGE_ATTACK1;

		//create the ball
		Vector vecStart, angleGun;
		Vector vecForward;



		//GetAttachment( 0, vecStart, angleGun );
			
		::UTIL_MakeVectorsPrivate(pev->angles, vecForward, NULL, NULL);

		vecStart = pev->origin + vecForward * 18 + Vector(0, 0, 60);

		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_ELIGHT );
			WRITE_SHORT( entindex( ) + 0x1000 );		// entity, attachment
			WRITE_COORD( 0 );		// origin
			WRITE_COORD( 0 );
			WRITE_COORD( 0 );
			WRITE_COORD( 32 );	// radius
			WRITE_BYTE( 0 );	// R
			WRITE_BYTE( 62 );	// G
			WRITE_BYTE( 255 );	// B
			WRITE_BYTE( 10 );	// life * 10
			WRITE_COORD( 32 ); // decay
		MESSAGE_END();

		CBaseMonster *pBall = (CBaseMonster*)Create( "kingpin_ball", vecStart, pev->angles, edict() );

		pBall->pev->velocity = Vector( vecForward.x * 100, vecForward.y * 100, 0 );
		pBall->m_hEnemy = m_hEnemy;


		this->playPsionicLaunchSound();
		//this->SetSequenceByIndex(KINGPIN_PSIONIC_LAUNCH);




	break;
	}
	}//END OF switch(...)


}//END OF HandleEventQueueEvent(...)


//This handles events built into the model, not custom hard-coded ones (above).
void CKingPin::HandleAnimEvent(MonsterEvent_t *pEvent ){
	switch( pEvent->event ){
	case 0:
	{

	break;
	}

	default:
		CBaseMonster::HandleAnimEvent( pEvent );
	break;
	}//END OF switch(...)
}






CBaseMonster* CKingPin::findClosestPowerupableMonster(void){

	CBaseEntity* pEntityScan = NULL;
	CBaseMonster* testMon = NULL;
	float thisDistance;
	CBaseMonster* bestChoiceYet = NULL;

	const float flMaxSearchDist = POWERUP_SEARCH_RANGE;

	float leastDistanceYet = flMaxSearchDist; //furthest I go

	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, flMaxSearchDist )) != NULL)
	{
		if(pEntityScan->pev == this->pev){
			//is it me? skip it.
			continue;
		}

		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){

		if(testMon != NULL){
			//EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? classname:%s df:%d, isgi:%d, nl:%d", FClassname(testMon), testMon->pev->deadflag, testMon->isSizeGiant(), FClassnameIs(testMon->pev, "monster_leech") ) );
		}else{
			////EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? beep NO NOT A MONSTER: classname:%s", FClassname(pEntityScan) ) );
		}
		
		if(testMon != NULL && (testMon->pev->deadflag == DEAD_NO) && (::FClassnameIs(testMon->pev, "monster_alien_grunt") ) ){
			thisDistance = (testMon->pev->origin - pev->origin).Length();
			if(thisDistance < leastDistanceYet){
				bestChoiceYet = testMon;
				leastDistanceYet = thisDistance;
				
			}//END OF minimum distance yet
		}//END OF entity scan null check

	}//END OF while loop through all entities
	return bestChoiceYet;

}//END OF getNearestDeadBody



void CKingPin::powerUpMonsters(void){

	
	CBaseEntity* pEntityScan = NULL;
	CBaseMonster* testMon = NULL;
	float thisDistance;
	CBaseMonster* bestChoiceYet = NULL;

	const float flMaxSearchDist = POWERUP_APPLY_RANGE;


	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, flMaxSearchDist )) != NULL)
	{
		if(pEntityScan->pev == this->pev){
			//is it me? skip it.
			continue;
		}

		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){

		if(testMon != NULL){
			//EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? classname:%s df:%d, isgi:%d, nl:%d", FClassname(testMon), testMon->pev->deadflag, testMon->isSizeGiant(), FClassnameIs(testMon->pev, "monster_leech") ) );
		}else{
			////EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? beep NO NOT A MONSTER: classname:%s", FClassname(pEntityScan) ) );
		}


		/*
		if(testMon != NULL){
			easyForcePrintLine("HELLO friendFACE %s", FClassname(testMon));
		
			if(FClassnameIs(testMon->pev, "monster_alien_grunt")){
				easyForcePrintLine("AM I oh???? ", testMon->monsterID);
			}
		}*/
		

		if(testMon != NULL && (testMon->pev->deadflag == DEAD_NO) && (::FClassnameIs(testMon->pev, "monster_alien_grunt") ) ){
			//thisDistance = (testMon->pev->origin - pev->origin).Length();
			//if(thisDistance < POWERUP_APPLY_RANGE){
			//no duh.

			easyForcePrintLine("ILL EAT POOP %d", testMon->monsterID);
			//This can reset the powerup duration for monsters that are already powered up.
			testMon->setPoweredUpOn(this, 12);


			//}//END OF minimum distance yet



		}//END OF entity scan null check

	}//END OF while loop through all entities



}//END OF powerUpMonsters





void CKingPin::removeFromPoweredUpCommandList(CBaseMonster* argToRemove){

	//yadda yadda. dropped feature.
	

}


//When a Kingpin dies, it needs to tell monsters it gave an attack order to forget it.
void CKingPin::deTargetMyCommandedMonsters(void){

	
	CBaseEntity* pEntityScan = NULL;
	CBaseMonster* testMon = NULL;
	float thisDistance;
	CBaseMonster* bestChoiceYet = NULL;

	const float flMaxSearchDist = POWERUP_APPLY_RANGE*2;


	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, flMaxSearchDist )) != NULL)
	{
		if(pEntityScan->pev == this->pev){
			//is it me? skip it.
			continue;
		}

		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){

		if(testMon != NULL){
			//EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? classname:%s df:%d, isgi:%d, nl:%d", FClassname(testMon), testMon->pev->deadflag, testMon->isSizeGiant(), FClassnameIs(testMon->pev, "monster_leech") ) );
		}else{
			////EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? beep NO NOT A MONSTER: classname:%s", FClassname(pEntityScan) ) );
		}
		
		
		if(testMon != NULL && (testMon->pev->deadflag == DEAD_NO) && (::FClassnameIs(testMon->pev, "monster_alien_grunt") ) ){
			testMon->forgetForcedEnemy(this, FALSE);
		}//END OF entity scan null check

	}//END OF while loop through all entities


}//END OF deTargetMyCommandedMonsters


void CKingPin::forceEnemyOnPoweredUpMonsters(CBaseEntity* monsterToForce, BOOL argPassive){
	
	CBaseEntity* pEntityScan = NULL;
	CBaseMonster* testMon = NULL;
	float thisDistance;
	CBaseMonster* bestChoiceYet = NULL;

	const float flMaxSearchDist = POWERUP_APPLY_RANGE*2;
	
	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, flMaxSearchDist )) != NULL)
	{
		if(pEntityScan->pev == this->pev){
			//is it me? skip it.
			continue;
		}

		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){

		if(testMon != NULL){
			//EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? classname:%s df:%d, isgi:%d, nl:%d", FClassname(testMon), testMon->pev->deadflag, testMon->isSizeGiant(), FClassnameIs(testMon->pev, "monster_leech") ) );
		}else{
			////EASY_CVAR_PRINTIF_PRE(friendlyPrintout, easyPrintLine("Friendly: is corpse ok? beep NO NOT A MONSTER: classname:%s", FClassname(pEntityScan) ) );
		}
		
		
		if(testMon != NULL && (testMon->pev->deadflag == DEAD_NO) && (::FClassnameIs(testMon->pev, "monster_alien_grunt") ) ){
			testMon->forceNewEnemy(this, monsterToForce, FALSE);
		}//END OF entity scan null check

	}//END OF while loop through all entities


}//END OF forceEnemyOnPoweredUpMonsters




void CKingPin::playPsionicLaunchSound(){
	switch(RANDOM_LONG(0, 2)){
	case 0:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_WEAPON, "houndeye/he_blast1.wav", global_soundVolumeAll, global_soundAttenuationAll*0.5, 0, 80 + RANDOM_LONG(0, 5));
	break;
	case 1:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_WEAPON, "houndeye/he_blast2.wav", global_soundVolumeAll, global_soundAttenuationAll*0.5, 0, 80 + RANDOM_LONG(0, 5));
	break;
	case 2:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_WEAPON, "houndeye/he_blast3.wav", global_soundVolumeAll, global_soundAttenuationAll*0.5, 0, 80 + RANDOM_LONG(0, 5));
	break;
	}
}//END OF playPsionicLaunchSound()






