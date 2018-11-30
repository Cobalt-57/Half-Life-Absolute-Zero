
//=========================================================
// PANTHER EYE (panthereye) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"activity.h"
#include	"animation.h"

#include "defaultai.h"
#include "soundent.h"
#include	"game.h"


class CPantherEye : public CBaseMonster
{
public:
	

	BOOL testLeapNoBlock(void);

	int  IRelationship( CBaseEntity *pTarget );
	void panthereye_findCoverFromEnemy(void);

	virtual int		Restore( CRestore &restore );
	virtual int		Save( CSave &save );
	static TYPEDESCRIPTION m_SaveData[];
	
	float faceLookTime;
	float leapAttackCooldown;

	BOOL isPissable;
	BOOL isCornered;

	CPantherEye(void);

	float runawayTime;
	float pissedOffTime;
	float timeTillSneakAgain;
	float findCoverTaskDataMem;


	float m_iTargetRanderamt;
	int sneakMode;
	//-1 = not sneaking.
	//0 = sneaking.


	Vector debugDrawVect;


	void EXPORT LeapTouch ( CBaseEntity *pOther );
		

	Schedule_t* ChaseOrCircle(void);
	
	BOOL hasSeeEnemyFix(void);

	BOOL forceIdleFrameReset(void);
	BOOL usesAdvancedAnimSystem(void);
	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);



	

	void Spawn( void );
	void Precache( void );
	
	void SetEyePosition(void);


	void SetYawSpeed( void );
	int  Classify ( void );
	
	void HandleEventQueueEvent(int arg_eventID);
	void HandleAnimEvent( MonsterEvent_t *pEvent );

	int IgnoreConditions ( void );

	float newPathDelay;



	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	

	BOOL needsMovementBoundFix(void);
	void MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval );
	

	// No range attacks
	BOOL CheckRangeAttack1 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckRangeAttack2 ( float flDot, float flDist ) { return FALSE; }
	

	BOOL violentDeathAllowed(void);
	BOOL violentDeathClear(void);












	float bugAnimBlockedTime;
	BOOL waitingForNewPath;

	//UNUSED, I guess.
	float chaseMode;
	//-1: not yet chasing anything.
	//0: trying to get into circle range...
	//1: circling around, ready to pick random position.
	//2: go to random position within circle edge
	//3: jump at target.
	//4: charge directly at foe.  (leads to attack melee1).


	float stareTime;

    float maxWaitPVSTime;

	float m_flNextFlinch;

	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( void );



	static const char *pAttackSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pLeapAttackHitSounds[];
	static const char *pDeathSounds[];


	

	
	//MODDD
	GENERATE_TAKEDAMAGE_PROTOTYPE
	GENERATE_TRACEATTACK_PROTOTYPE
	
	void MonsterThink ( void );
	void SetActivity ( Activity NewActivity );


	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );

	//originally 784.
	float CoverRadius( void ) { return 1200; }
		
	
	
	CUSTOM_SCHEDULES;


};




#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_panthereye, CPantherEye );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( panthereye, CPantherEye );
	
	#if EXTRA_NAMES == 2
		LINK_ENTITY_TO_CLASS( panther, CPantherEye );
	#endif
	
#endif


const char *CPantherEye::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CPantherEye::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *CPantherEye::pAttackSounds[] = 
{
	"panthereye/pa_attack1.wav"
};

const char *CPantherEye::pIdleSounds[] = 
{
	"panthereye/pa_idle1.wav",
	"panthereye/pa_idle2.wav",
	"panthereye/pa_idle3.wav",
	"panthereye/pa_idle4.wav",

};

const char *CPantherEye::pAlertSounds[] = 
{
	"zombie/zo_alert10.wav",
	"zombie/zo_alert20.wav",
	"zombie/zo_alert30.wav",
};

const char *CPantherEye::pPainSounds[] = 
{
	"zombie/zo_pain1.wav",
	"zombie/zo_pain2.wav",
};

const char *CPantherEye::pLeapAttackHitSounds[] = 
{
	//for now, clone of zombie strike sounds.
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CPantherEye::pDeathSounds[] = 
{
	"panthereye/pa_death1.wav",
};






TYPEDESCRIPTION	CPantherEye::m_SaveData[] = 
{
	//DEFINE_FIELD( CBaseEntity,m_pfnThink , FIELD_FUNCTION),
	DEFINE_FIELD(CPantherEye, sneakMode, FIELD_INTEGER)
//	*m_pfnThink
};
//IMPLEMENT_SAVERESTORE( CPantherEye, CBaseMonster );
int CPantherEye::Save( CSave &save )
{
	if ( !CBaseMonster::Save(save) )
		return 0;
	return save.WriteFields( "CPantherEye", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}
int CPantherEye::Restore( CRestore &restore )
{
	if ( !CBaseMonster::Restore(restore) )
		return 0;
	return restore.ReadFields( "CPantherEye", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}















//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_PANTHEREYE_GENERIC_FAIL = LAST_COMMON_SCHEDULE + 1,
	SCHED_PANTHEREYE_CHASE_ENEMY,
	SCHED_PANTHEREYE_SNEAK_TO_LOCATION,
	SCHED_PANTHEREYE_COVER_FAIL,
	//follow?
	

};

//=========================================================
// monster-specific tasks
//=========================================================
enum 
{
	TASK_PANTHEREYE_PLAYANIM_GETBUG = LAST_COMMON_TASK + 1,
	TASK_PANTHEREYE_PLAYANIM_CROUCHTOJUMP,
	TASK_PANTHEREYE_SNEAK_WAIT,
	TASK_PANTHER_NORMALMOVE,
	TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY,
	TASK_PANTHEREYE_COVER_FAIL_WAIT,


};








/*
Task_t	tlGruntTakeCoverFromBestSound[] =
{
	{ TASK_SET_FAIL_SCHEDULE,			(float)SCHED_COWER			},// duck and cover if cannot move from explosion
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_BEST_SOUND,	(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slGruntTakeCoverFromBestSound[] =
{
	{ 
		tlGruntTakeCoverFromBestSound,
		ARRAYSIZE ( tlGruntTakeCoverFromBestSound ), 
		0,
		0,
		"GruntTakeCoverFromBestSound"
	},
};
*/





















//isn't "panthereyeSneakToLocation" based off of this?  Just simpler.  ah well.
Task_t	tlPanthereyeTakeCoverFromEnemy[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_COVER_FAIL	},
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
//	{ TASK_TURN_LEFT,				(float)179					},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_WAIT,					(float)1					},
};

Schedule_t	slPanthereyeTakeCoverFromEnemy[] =
{
	{ 
		tlPanthereyeTakeCoverFromEnemy,
		ARRAYSIZE ( tlPanthereyeTakeCoverFromEnemy ), 
		bits_COND_NEW_ENEMY,
		0,
		"PanthereyeTakeCoverFromEnemy"
	},
};




//=========================================================
// move away from where you're currently standing. 
//=========================================================
Task_t	tlPanthereyeTakeCoverFromOrigin[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_COVER_FAIL	},
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_ORIGIN,		(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slPanthereyeTakeCoverFromOrigin[] =
{
	{ 
		tlPanthereyeTakeCoverFromOrigin,
		ARRAYSIZE ( tlPanthereyeTakeCoverFromOrigin ), 
		bits_COND_NEW_ENEMY,
		0,
		"PanthereyeTakeCoverFromOrigin"
	},
};
//=========================================================
// hide from the loudest sound source
//=========================================================
Task_t	tlPanthereyeTakeCoverFromBestSound[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_COVER_FAIL	},
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_BEST_SOUND,	(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slPanthereyeTakeCoverFromBestSound[] =
{
	{ 
		tlPanthereyeTakeCoverFromBestSound,
		ARRAYSIZE ( tlPanthereyeTakeCoverFromBestSound ), 
		bits_COND_NEW_ENEMY,
		0,
		"PanthereyeTakeCoverFromBestSound"
	},
};









//slPanthereyeFlee



//MODDD NOTE - funny, the "TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY" has no call for "TaskCompletion", so anything after "TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY" never happens.
//Perhaps this rule of interruption is ok?
//MODDD NOTE UPDATE - NO. Look at the "panthereye_findCoverFromEnemy" method. It calls "TaskComplete" if it determines a path. So what comes after
//                    TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY still runs and the schedule finishes normally.
Task_t	tlPanthereyeSneakToLocation[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_COVER_FAIL	},
	{ TASK_STOP_MOVING,			0				},
	//{ TASK_WAIT,					(float)0.2					},
	{ TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_PANTHEREYE_SNEAK_WAIT,	(float)0					},
	
};

Schedule_t	slPanthereyeSneakToLocation[] =
{
	{
		tlPanthereyeSneakToLocation,
		ARRAYSIZE ( tlPanthereyeSneakToLocation ),
		//TODO: much more!  sound_  stuff too.
		bits_COND_ENEMY_DEAD |
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2,

		bits_SOUND_DANGER,
		"Panther_sneakToLocation"
	},
};



//Go straight to sneakwait. Has its uses.
Task_t	tlPanthereyeSneakWait[] =
{
	{ TASK_UPDATE_LKP,	(float)0					},
	{ TASK_PANTHEREYE_SNEAK_WAIT,	(float)0					},
	
};

Schedule_t	slPanthereyeSneakWait[] =
{
	{
		tlPanthereyeSneakWait,
		ARRAYSIZE ( tlPanthereyeSneakWait ),
		//TODO: much more!  sound_  stuff too.
		bits_COND_ENEMY_DEAD |
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2,

		bits_SOUND_DANGER,
		"Panther_sneakWait"
	},
};




Task_t	tlPanthereyeJumpAtEnemy[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_GENERIC_FAIL	},
	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	//{ TASK_WAIT,				(float)0.4		},
	{ TASK_FACE_IDEAL,			(float)25		},   //set before going to this sched...
	{ TASK_PANTHEREYE_PLAYANIM_CROUCHTOJUMP,			(float)0		},  //includes wait for anim finish

};

Schedule_t	slPanthereyeJumpAtEnemy[] =
{
	{
		tlPanthereyeJumpAtEnemy,
		ARRAYSIZE ( tlPanthereyeJumpAtEnemy ),
		//TODO: much more!  sound_  stuff too.
		0,
		0,
		"Panther_JumpAtEnemy"
	},
};



// Chase enemy schedule
Task_t tlPanthereyeChaseEnemy[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_GENERIC_FAIL	},
	//{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	//{ TASK_RUN_PATH,			(float)0		},
	//{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	
	//is this ok?
	{TASK_MOVE_TO_ENEMY_RANGE, (float)0					},
	{TASK_CHECK_STUMPED, (float)0						},





};

Schedule_t slPanthereyeChaseEnemy[] =
{
	{ 
		tlPanthereyeChaseEnemy,
		ARRAYSIZE ( tlPanthereyeChaseEnemy ),
		bits_COND_NEW_ENEMY			|
		//MODDD - added, the bullsquid counts this.  Why doesn't everything?
		bits_COND_ENEMY_DEAD |

		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Panthereye Chase Enemy"
	},
};




// Chase enemy schedule
Task_t tlPanthereyeGetIntoCirclingRange[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_GENERIC_FAIL	},
	{ TASK_PANTHER_NORMALMOVE,	(float)260		},
	{ TASK_RUN_PATH,			(float)0		},
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
};

Schedule_t slPanthereyeGetIntoCirclingRange[] =
{
	{ 
		tlPanthereyeGetIntoCirclingRange,
		ARRAYSIZE ( tlPanthereyeGetIntoCirclingRange ),
		bits_COND_NEW_ENEMY			|
		//MODDD - added, the bullsquid counts this.  Why doesn't everything?
		bits_COND_ENEMY_DEAD |

		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Panthereye Getintocircl"
	},
};














Task_t	tlPanthereyeGetBug[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_GENERIC_FAIL	},
	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	//{ TASK_WAIT,				(float)0.4		},
	{ TASK_FACE_IDEAL,			(float)24		},   //set before going to this sched...
	{ TASK_PANTHEREYE_PLAYANIM_GETBUG,			(float)0		},  //includes wait for anim finish

};

Schedule_t	slPanthereyeGetBug[] =
{
	{
		tlPanthereyeGetBug,
		ARRAYSIZE ( tlPanthereyeGetBug ),
		//TODO: much more!  sound_  stuff too.
		bits_COND_CAN_ATTACK,
		0,
		"Panther_getbug"
	},
};








Task_t	tlPantherEyeCoverFail[] =
{
	{ TASK_PANTHEREYE_COVER_FAIL_WAIT,			0				},
};

Schedule_t	slPantherEyeCoverFail[] =
{
	{
		tlPantherEyeCoverFail,
		ARRAYSIZE ( tlPantherEyeCoverFail ),
		bits_COND_CAN_ATTACK,
		0,
		"Panther_genFail"
	},
};




Task_t	tlPantherEyeGenericFail[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_PANTHEREYE_GENERIC_FAIL	},
	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)0.2		},
	{ TASK_WAIT_PVS,			(float)0		},
	{ TASK_UPDATE_LKP, (float)0		},
};

Schedule_t	slPantherEyeGenericFail[] =
{
	{
		tlPantherEyeGenericFail,
		ARRAYSIZE ( tlPantherEyeGenericFail ),
		0,
		0,
		"Panther_genFail"
	},
};














/*


// Chase enemy schedule
Task_t tlHAssault_follow[] = 
{
	//{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CHASE_ENEMY_FAILED	},
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_HASSAULT_GENERIC_FAIL	},
	{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	{ TASK_RUN_PATH,			(float)0		},
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	{ TASK_FACE_ENEMY,	(float)0		},
};

//NOTICE: Based off of "slChaseEnemy" from defaultai.cpp.  Customize as needed.  Ditto for above.
Schedule_t slHAssault_follow[] =
{
	{ 
		tlHAssault_follow,
		ARRAYSIZE ( tlHAssault_follow ),
		bits_COND_NEW_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"HA Chase Enemy"
	},
};
*/





/*
Task_t	tlHAssault_fire[] =
{
	
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	{ TASK_HASSAULT_CHECK_FIRE, (float)0	},
	{ TASK_FACE_ENEMY,			(float)0		},
	{TASK_SET_ACTIVITY,  (float)ACT_RANGE_ATTACK1},
	
	{ TASK_RANGE_ATTACK1,				(float)0		},
	
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
};




Schedule_t	slHAssault_fire[] =
{
	{ 
		tlHAssault_fire,
		ARRAYSIZE ( tlHAssault_fire ), 

		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND		|
		//bits_COND_GRUNT_NOFIRE		|
		bits_COND_SPECIAL1	| 
		bits_COND_NO_AMMO_LOADED,
		

		bits_SOUND_DANGER,


		"slHAssault_fire"
	},
};

*/






DEFINE_CUSTOM_SCHEDULES( CPantherEye )
{
	//slPanthereye_follow,
	//slHAssaultGenericFail,
	slPantherEyeGenericFail,
	slPanthereyeGetBug,
	slPanthereyeJumpAtEnemy,

	slPanthereyeChaseEnemy,

	slPanthereyeGetIntoCirclingRange,
	slPanthereyeSneakWait,
	slPanthereyeSneakToLocation,
	//slPanthereyeFlee,
	slPanthereyeTakeCoverFromOrigin,
	slPanthereyeTakeCoverFromBestSound,
	slPanthereyeTakeCoverFromEnemy,
	slPantherEyeCoverFail

};

IMPLEMENT_CUSTOM_SCHEDULES( CPantherEye, CBaseMonster );

