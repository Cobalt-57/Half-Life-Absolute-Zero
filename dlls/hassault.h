
//=========================================================
// HUMAN ASSAULT (hassault) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"weapons.h"


//MODDD - hgrunt, a fair base for this class in some ways, inherits from these, so may as well here too
#include	"squadmonster.h"
#include	"talkmonster.h"

//JUST DO IT!
#include "defaultai.h"
#include "animation.h"


#include	"soundent.h"




//MODDD - making a "CBaseSquadMonster" instead to be more compatibile with squads (particularly as leaders to HGrunts)
class CHAssault : public CSquadMonster
{
public:
	
	float SafeSetBlending ( int iBlender, float flValue );



	float signal1Cooldown;
	float signal2Cooldown;

	float meleeBlockTime;

	BOOL nonStumpableCombatLook;
	int previousAnimationActivity;

	float fireDelay;
	
	//MODDD - addition
	BOOL couldRangedAttack1;
	BOOL couldRangedAttack2;
	BOOL couldMeleeAttack1;
	BOOL couldMeleeAttack2;

	BOOL forceBlockResidual;
	void ReportAIState( void );


	Vector GetGunPosition(void);
	Vector GetGunPositionAI(void);


	int IRelationship ( CBaseEntity *pTarget );
	
	float getBarnacleForwardOffset(void);
	float getBarnaclePulledTopOffset(void);
	Vector debugDrawVect;
	void onDelete(void);

	BOOL usesAdvancedAnimSystem(void);
	int tryActivitySubstitute(int activity);
	int LookupActivityHard(int activity);
	
   

	
	virtual int		Restore( CRestore &restore );
	virtual int		Save( CSave &save );
	static TYPEDESCRIPTION m_SaveData[];


	CHAssault(void);

	float meleeAttackTimeMax;
	float residualFireTime;
	float waittime;
	Schedule_t* recentSchedule;
	Schedule_t* recentRecentSchedule;
	float idleSpinSoundDelay;
	float chainFireSoundDelay;
	BOOL chainFiredRecently;
	float spinuptimeIdleSoundDelay;
	float residualFireTimeBehindCheck;
	//!!!
	Vector forceFireTargetPosition;

	//edict_t* forceFireTargetObject;
	EHANDLE forceFireTargetObject;

	float forceFireGiveUp;

	float alertSoundCooldown;



	int getIdleSpinChannel(void );
	int getSpinUpDownChannel(void );
	int getChainFireChannel(void);

	void resetChainFireSound(void);
	void attemptStopIdleSpinSound(void);
	void attemptStopIdleSpinSound(BOOL forceStop);

	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
	BOOL isOrganic(void){return !CanUseGermanModel();}
	
	BOOL getGermanModelRequirement(void);
	const char* getGermanModel(void);
	const char* getNormalModel(void);


	void PrescheduleThink ( void );

	void HandleEventQueueEvent( int arg_eventID);
	void HandleAnimEvent( MonsterEvent_t *pEvent );
//	int IgnoreConditions ( void );

	float m_flNextFlinch;
	
	BOOL FCanCheckAttacks();
	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( void );
	void Shoot ( void );
	void ShootAtForceFireTarget();

	//MODDD - new.
	void SetObjectCollisionBox( void )
	{
		pev->absmin = pev->origin + Vector(-65, -65, 0);
		pev->absmax = pev->origin + Vector(65, 65, 72);
	}



	static const char *pAttackSounds[];
	//static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];

	void MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval );
	
	GENERATE_KILLED_PROTOTYPE

	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist ){return false;};
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );

	void SetActivity(Activity NewActivity);
	
	void SetTurnActivity(void);

	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE
	

	int m_voicePitch;

	BOOL firing;
	float spinuptime;
	float spinuptimeremain;

	float rageTimer;
	float movementBaseFramerate;

	BOOL soundgiven;
	int		m_iBrassShell;
	void MonsterThink( void );
//	virtual void HAssaultThink( void );
	CBaseEntity* Kick( void );


	//MADE VIRTUAL!!!
	BOOL FValidateCover ( const Vector &vecCoverLocation );
	int ISoundMask ( void );




	

	//MODDD - May be a better approach than "monsterThink".
	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );

	//MODDD - new.
	BOOL spinuptimeSet;
	int SquadRecruit( int searchRadius, int maxMembers );

	void AimAtEnemy(Vector& refVecShootOrigin, Vector& refVecShootDir);
	

	BOOL canResetBlend0(void);
	BOOL onResetBlend0(void);

	CUSTOM_SCHEDULES;

};




TYPEDESCRIPTION	CHAssault::m_SaveData[] = 
{
	DEFINE_FIELD( CBaseEntity,m_pfnThink , FIELD_FUNCTION),
	DEFINE_FIELD( CHAssault, m_voicePitch, FIELD_INTEGER),
	DEFINE_FIELD( CHAssault, spinuptime, FIELD_TIME),
	DEFINE_FIELD( CHAssault, spinuptimeremain, FIELD_TIME),
	
};
//IMPLEMENT_SAVERESTORE( CHAssault, CSquadMonster );    //parent was "CBaseMonster", assuming CSquadMonster is okay though?
int CHAssault::Save( CSave &save )
{
	if ( !CSquadMonster::Save(save) )
		return 0;
	return save.WriteFields( "CHAssault", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}
int CHAssault::Restore( CRestore &restore )
{
	if ( !CSquadMonster::Restore(restore) )
		return 0;
	return restore.ReadFields( "CHAssault", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}



/*
int CHAssault::Save( CSave &save )
{
	if ( !CBaseToggle::Save(save) )
		return 0;
	return save.WriteFields( "CHAssault", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}
*/

const char *CHAssault::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CHAssault::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *CHAssault::pAttackSounds[] = 
{
	"zombie/zo_attack1.wav",
	"zombie/zo_attack2.wav",
};


//"cannot allocate an array of constant size 0" my ass.
/*
const char *CHAssault::pIdleSounds[] = 
{
	//Play a sentence instead. HGrunt does that with gr_idle's 1-3 through sentences HG_IDLE 0-2.
	//"hgrunt/gr_radio1.wav",
	//"hgrunt/gr_radio2.wav",
	//"hgrunt/gr_radio3.wav",
	//"hgrunt/gr_radio4.wav",
	//"hgrunt/gr_radio5.wav",
	//"hgrunt/gr_radio6.wav",
	"hgrunt/gr_idle1.wav",
	"hgrunt/gr_idle2.wav",
	"hgrunt/gr_idle3.wav",
};
*/

const char *CHAssault::pAlertSounds[] = 
{
	"hassault/hw_alert.wav",
};

const char *CHAssault::pPainSounds[] = 
{
	"hgrunt/gr_pain1.wav",
	"hgrunt/gr_pain2.wav",
	"hgrunt/gr_pain3.wav",
	"hgrunt/gr_pain4.wav",
	"hgrunt/gr_pain5.wav",
};



















//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_HASSAULT_SPIN = LAST_COMMON_SCHEDULE + 1,
	SCHED_HASSAULT_FOUND_ENEMY,
	SCHED_HASSAULT_SUPPRESS,
	SCHED_HASSAULT_FIRE,
	SCHED_HASSAULT_FIRE_OVER,
	SCHED_HASSAULT_GENERIC_FAIL,
	SCHED_HASSAULT_RESIDUAL_FIRE,
	SCHED_HASSAULT_FORCEFIRE,
	SCHED_HASSAULT_MELEE1,

};

//=========================================================
// monster-specific tasks
//=========================================================
enum 
{
	TASK_HASSAULT_SPIN = LAST_COMMON_TASK + 1,
	TASK_HASSAULT_SPINDOWN,
	TASK_HASSAULT_FIRE,
	TASK_HASSAULT_CHECK_FIRE,
	TASK_HASSAULT_RESIDUAL_FIRE,
	TASK_HASSAULT_FACE_FORCEFIRE,
	TASK_HASSAULT_FORCEFIRE,
	TASK_HASSAULT_START_SPIN,
	TASK_HASSAULT_WAIT_FOR_SPIN_FINISH

};







Task_t	tlHAssaultFireOver[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_RESIDUAL_FIRE	},
	/*
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)0.2		},
	{ TASK_WAIT_PVS,			(float)0		},
	*/
};

Schedule_t	slHAssaultFireOver[] =
{
	{
		tlHAssaultFireOver,
		ARRAYSIZE ( tlHAssaultFireOver ),
		0,
		0,
		"HAssault_FireOverFail"
	},
};





Task_t	tlHAssaultGenericFail[] =
{
	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)0.2		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slHAssaultGenericFail[] =
{
	{
		tlHAssaultGenericFail,
		ARRAYSIZE ( tlHAssaultGenericFail ),
		bits_COND_CAN_ATTACK,
		0,
		"HAssault_genFail"
	},
};






Task_t	tlHAssault_spin[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	{ TASK_STOP_MOVING,			(float)0		},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_HASSAULT_SPIN,					(float)1.5f},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_FACE_ENEMY,			(float)0		},
	//not yet dummkopf!
	//{ TASK_RANGE_ATTACK1,				(float)0		},
	
	//{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_IDLE_ANGRY  },

	/*
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	
	*/
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
};


//GruntRangeAttack1B

Schedule_t	slHAssault_spin[] =
{
	{ 
		tlHAssault_spin,
		ARRAYSIZE ( tlHAssault_spin ), 

		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		//bits_COND_HEAR_SOUND		|
		//bits_COND_GRUNT_NOFIRE		|
		bits_COND_SPECIAL1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_NO_AMMO_LOADED,
		
		//FOR CRYIN OUT LOUD DONT INTERRUPT THE SPIN.  or maybe it's ok?
		//But at least find a way not to interrupt the startup spin anim perhaps if so?
		//bits_SOUND_DANGER | bits_SOUND_PLAYER | bits_SOUND_WORLD,
		0,

		"tlHAssault_spin"
	},
};


Task_t	tlHAssault_foundEnemy[] =
{
	{ TASK_STOP_MOVING,				0							},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_HASSAULT_START_SPIN,				(float)0					},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_SIGNAL1			},
	{ TASK_HASSAULT_WAIT_FOR_SPIN_FINISH,		(float)0				},
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
};

Schedule_t	slHAssault_foundEnemy[] =
{
	{ 
		tlHAssault_foundEnemy,
		ARRAYSIZE ( tlHAssault_foundEnemy ), 
		
		0,

		0,

		"tlHAssault_FoundEn"
	},
};



Task_t	tlHAssault_suppress[] =
{
	{ TASK_STOP_MOVING,				0							},
	{ TASK_SOUND_WAKE,				(float)0					},   //is that okaY?
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_HASSAULT_START_SPIN,				(float)0					},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_SIGNAL2			},
	{ TASK_HASSAULT_WAIT_FOR_SPIN_FINISH,		(float)0				},
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
};

Schedule_t	slHAssault_suppress[] =
{
	{ 
		tlHAssault_suppress,
		ARRAYSIZE ( tlHAssault_suppress ), 
		
		0,

		0,

		"tlHAssault_Suppress"
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
*/

Task_t tlHAssault_follow[] = 
{
	//NO
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_HASSAULT_GENERIC_FAIL	},
	
	//{ TASK_GET_PATH_TO_ENEMY,	(float)0					},
	//{ TASK_RUN_PATH,			(float)0					},
	//{ TASK_WAIT_FOR_MOVEMENT,	(float)0					},
	{TASK_MOVE_TO_ENEMY_RANGE, (float)0					},
	{ TASK_CHECK_STUMPED,(float)0			},


};




//NOTICE: Based off of "slChaseEnemy" from defaultai.cpp.  Customize as needed.  Ditto for above.
Schedule_t slHAssault_follow[] =
{
	{ 
		tlHAssault_follow,
		ARRAYSIZE ( tlHAssault_follow ),

		/*
		bits_COND_NEW_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|

		bits_COND_SEE_ENEMY |  //MODDD - ok?
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |

		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER | bits_SOUND_PLAYER,
		*/



		bits_COND_NEW_ENEMY			|
		//MODDD - added, the bullsquid counts this.  Why doesn't everything?
		bits_COND_ENEMY_DEAD |

		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER | bits_SOUND_PLAYER | bits_SOUND_WORLD,






		"HA Chase Enemy"
	},
};






Task_t	tlHAssault_fire[] =
{
	
	//{ TASK_FACE_ENEMY,			(float)0		},
	//{ TASK_HASSAULT_SPIN,					(float)1.5f},


	{ TASK_STOP_MOVING, (float)0},
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	
	//these are redundant now.
	//{ TASK_HASSAULT_CHECK_FIRE, (float)0	},
	//{ TASK_FACE_ENEMY,			(float)0		},
	
	
	
	//set activity to ACT_RANGE_ATTACK1 ?
	//{TASK_SET_ACTIVITY,  (float)ACT_RANGE_ATTACK1},
	
	//MODDD - this will now last as long as the hgrunt has a clear firing view of the enemy. It automatically loops,
	//        and relying on the looping behavior instead of going through this schedule again when it's done saves
	//        a frame that would otherwise be wasted.
	{ TASK_RANGE_ATTACK1,				(float)0		},
	
	//{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_IDLE_ANGRY  },

	/*
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	
	*/
	//{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
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
		//bits_COND_HEAR_SOUND		|

		//bits_COND_GRUNT_NOFIRE		|
		bits_COND_SPECIAL1	| 
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_NO_AMMO_LOADED,
		



		
		//bits_SOUND_DANGER,
		0,


		"slHAssault_fire"
	},
};








Task_t	tlHAssault_residualFire[] =
{
	
	//{ TASK_FACE_ENEMY,			(float)0		},
	//{ TASK_HASSAULT_SPIN,					(float)1.5f},

	
	{ TASK_STOP_MOVING, (float)0},
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	{ TASK_HASSAULT_CHECK_FIRE, (float)0	},

	//This probably isn't the same as "LKM", it's the "last place this monster was before conflict".
	//No issues, so... no complaints I guess.
	
	//hm... best not do this.
	//{ TASK_FACE_LASTPOSITION,			(float)0		},




	//MODDD TODO - make this TASK_RANGE_ATTACK1, the endless looping task later.
	//{ TASK_RANGE_ATTACK1,				(float)0		},


	//set activity to ACT_RANGE_ATTACK1 ?
	{TASK_SET_ACTIVITY,  (float)ACT_RANGE_ATTACK1},
	
	{ TASK_HASSAULT_RESIDUAL_FIRE,				(float)0		},
	
	//{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_IDLE_ANGRY  },

	/*
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	
	*/
	//{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
};




Schedule_t	slHAssault_residualFire[] =
{
	{ 
		tlHAssault_residualFire,
		ARRAYSIZE ( tlHAssault_residualFire ), 

		bits_COND_NEW_ENEMY			|

		//MODDD - Surpsrisingly no. Just because we can see an enemy behind cover doesn't mean we can actually hit them, this tricks the system.
		//////bits_COND_SEE_ENEMY			|
		//But being able to do a ranged attack (clear shot) means we can stop using residual fire.
		bits_COND_CAN_RANGE_ATTACK1 |

		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		//bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND		|
		//bits_COND_GRUNT_NOFIRE		|
		bits_COND_SPECIAL1	| 
		//MODDD - yes.
		bits_COND_CAN_MELEE_ATTACK1 |

		bits_COND_NO_AMMO_LOADED,
		//bits_COND_SEE_ENEMY,  //NEW!  ... no, we must be more specific.

		
		bits_SOUND_DANGER,

		"slHAssault_resfire"
	},
};











/*
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	{ TASK_HASSAULT_CHECK_FIRE, (float)0	},
	{ TASK_FACE_ENEMY,			(float)0		},
	//set activity to ACT_RANGE_ATTACK1 ?
	{TASK_SET_ACTIVITY,  (float)ACT_RANGE_ATTACK1},
	
	{ TASK_RANGE_ATTACK1,				(float)0		},
	*/




// MODDD TODO - Make the hgrunt check to see if the player is blocked by something, even while spinning up, and remember that object to then begin
// firing at it when done spinning up.
Task_t	tlHAssault_forceFireAtTarget[] =
{
	
	//{ TASK_FACE_ENEMY,			(float)0		},
	//{ TASK_HASSAULT_SPIN,					(float)1.5f},


	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_FIRE_OVER},
	{ TASK_HASSAULT_CHECK_FIRE, (float)0	},

	//This probably isn't the same as "LKM", it's the "last place this monster was before conflict".
	//No issues, so... no complaints I guess.
	{ TASK_HASSAULT_FACE_FORCEFIRE,			(float)0		},


	//set activity to ACT_RANGE_ATTACK1 ?
	{TASK_SET_ACTIVITY,  (float)ACT_RANGE_ATTACK1},
	
	{ TASK_HASSAULT_FORCEFIRE,				(float)0		},
	
	//{ TASK_PLAY_SEQUENCE_FACE_ENEMY,(float)ACT_IDLE_ANGRY  },

	/*
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_STOP_MOVING,	(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	
	*/
	//{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FIRE	},
	{ TASK_SET_SCHEDULE,			(float)SCHED_HASSAULT_FORCEFIRE	},
};


Schedule_t	slHAssault_forceFireAtTarget[] =
{
	{ 
		tlHAssault_forceFireAtTarget,
		ARRAYSIZE ( tlHAssault_forceFireAtTarget ), 

		//This little addition here is ok, right?
		bits_COND_CAN_RANGE_ATTACK1 |

		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		//bits_COND_ENEMY_OCCLUDED	|
		//bits_COND_HEAR_SOUND		|
		//bits_COND_GRUNT_NOFIRE		|
		bits_COND_SPECIAL1	| 
		bits_COND_NO_AMMO_LOADED,
		//bits_COND_SEE_ENEMY,  //NEW!  ... no, we must be more specific.

		

		//bits_SOUND_DANGER,
		0,


		"slHAssault_forceFi"
	},
};










// primary melee attack
Task_t	tlHAssault_melee1[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_HASSAULT_GENERIC_FAIL},
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_MELEE_ATTACK1,		(float)0		},
};

Schedule_t	slHAssault_melee1[] =
{
	{ 
		tlHAssault_melee1,
		ARRAYSIZE ( tlHAssault_melee1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		//MODDD - also, WHY NOT?
		bits_COND_TASK_FAILED |
		//bits_COND_CAN_RANGE_ATTACK1 |
		bits_COND_ENEMY_OCCLUDED,
		0,
		"hassault MeleeAttack1"
	},
};




DEFINE_CUSTOM_SCHEDULES( CHAssault )
{
	slHAssault_spin,
	slHAssault_foundEnemy,
	slHAssault_suppress,
	slHAssault_fire,
	slHAssault_follow,
	slHAssaultGenericFail,
	slHAssaultFireOver,
	slHAssault_residualFire,
	slHAssault_forceFireAtTarget,
	slHAssault_melee1

};

IMPLEMENT_CUSTOM_SCHEDULES( CHAssault, CSquadMonster );



