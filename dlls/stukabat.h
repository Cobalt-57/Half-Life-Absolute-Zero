

//=========================================================
// STUKABAT (file modeled after controller.cpp, split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"effects.h"
#include	"schedule.h"
#include	"weapons.h"
#include	"squadmonster.h"

//MODDD
#include "flyingmonster.h"
#include	"animation.h"
#include	"soundent.h"
#include	"defaultai.h"


//=========================================================
// Monster's Anim Events Go Here
//=========================================================






#define PRINTQUEUE_DECLARATION(QUEUENAME) PrintQueue QUEUENAME;
#define PRINTQUEUE_NAME(QUEUENAME, DISPLAY) QUEUENAME(PrintQueue(DISPLAY))
#define PRINTQUEUE_CLEAR(QUEUENAME) QUEUENAME.clearPrintQueue();
#define PRINTQUEUE_PRINTOUT(QUEUENAME) QUEUENAME.receivePrintQueue(finalDest, &finalDestLocation);

//this->stukaPrint.tookDamage.sendToPrintQueue("TOOK DMG");



class StukaPrintQueueManager{
public:
	PRINTQUEUE_DECLARATION(pathFinding)
	PRINTQUEUE_DECLARATION(getSchedule)
	PRINTQUEUE_DECLARATION(setActivity)
	PRINTQUEUE_DECLARATION(enemyInfo)
	PRINTQUEUE_DECLARATION(general)
	PRINTQUEUE_DECLARATION(tookDamage)
	PRINTQUEUE_DECLARATION(eatRelated)
	PRINTQUEUE_DECLARATION(soundRelated)
	PRINTQUEUE_DECLARATION(moveRelated)
	
	char displayName[PRINTQUEUE_NAMESTRINGSIZE];

	StukaPrintQueueManager(const char* arg_name):
		//pathFinding(PrintQueue("pthfnd") ),
		//getSchedule(PrintQueue("getsch") ),
		//setActivity(PrintQueue("setact") )
		PRINTQUEUE_NAME(pathFinding, "PTHFND"),
		PRINTQUEUE_NAME(getSchedule, "GETSCH"),
		PRINTQUEUE_NAME(setActivity, "SETACT"),
		PRINTQUEUE_NAME(enemyInfo, "ENEINF"),
		PRINTQUEUE_NAME(general, "GENERL"),
		PRINTQUEUE_NAME(tookDamage, "TOOKDMG"),
		PRINTQUEUE_NAME(eatRelated, "EATREL"),
		PRINTQUEUE_NAME(soundRelated, "SNDREL"),
		PRINTQUEUE_NAME(moveRelated, "MOVREL")
		
	{

		int nameLength = lengthOfString(arg_name, PRINTQUEUE_NAMESTRINGSIZE);
		strncpyTerminate(&displayName[0], arg_name, nameLength);

		//not much else?
	}

	void clearAll(){
		PRINTQUEUE_CLEAR(pathFinding);
		PRINTQUEUE_CLEAR(getSchedule);
		PRINTQUEUE_CLEAR(setActivity);
		PRINTQUEUE_CLEAR(enemyInfo);
		PRINTQUEUE_CLEAR(general);
		PRINTQUEUE_CLEAR(tookDamage);
		PRINTQUEUE_CLEAR(eatRelated);
		PRINTQUEUE_CLEAR(soundRelated);
		PRINTQUEUE_CLEAR(moveRelated);
	}
	

	#define PRINTQUEUE_STUKA_NUMBEROF 8
	//No, varries per setup.

	void printOutAll(){
		int finalDestLocation = 0;
		const int max = PRINTQUEUE_TOTALEXPECTED * PRINTQUEUE_STUKA_NUMBEROF + ((PRINTQUEUE_NAMESTRINGSIZE+1) * (PRINTQUEUE_STUKA_NUMBEROF + 1) ) ;
		char finalDest[max];

		appendTo(finalDest, displayName, &finalDestLocation, ':');
		
		PRINTQUEUE_PRINTOUT(pathFinding)
		PRINTQUEUE_PRINTOUT(getSchedule)
		PRINTQUEUE_PRINTOUT(setActivity)
		PRINTQUEUE_PRINTOUT(enemyInfo)
		PRINTQUEUE_PRINTOUT(general)
		PRINTQUEUE_PRINTOUT(tookDamage)
		PRINTQUEUE_PRINTOUT(eatRelated)
		PRINTQUEUE_PRINTOUT(soundRelated)
		PRINTQUEUE_PRINTOUT(moveRelated)

		//pathFinding.receivePrintQueue(finalDest, &finalDestLocation);
		//getSchedule.receivePrintQueue(finalDest, &finalDestLocation);
		//setActivity.receivePrintQueue(finalDest, &finalDestLocation);
		finalDest[finalDestLocation] = '\0';

		easyPrintLine(finalDest);
		//cout << "~USED: " << finalDestLocation << " AVAILABLE: " << max << endl;
	}

};



















class CStukaBat: public CSquadMonster
{
public:
	


	


	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
	






	
	StukaPrintQueueManager stukaPrint;

	CStukaBat(void);
	
	float getMeleeAnimScaler(void);
	float getAttackDelay(void);
	void checkStartSnap();

	void Activate(void);
	void DefaultSpawnNotice(void);
	void ForceSpawnFlag(int arg_spawnFlag);


	BOOL wakeUp;
	BOOL eating;
	float eatingAnticipatedEnd;
	BOOL turnThatOff;


	void MakeIdealYaw( Vector vecTarget );
	virtual float ChangeYaw ( int speed );
		
	void ForceMakeIdealYaw( Vector vecTarget );
	virtual float ForceChangeYaw ( int speed );



	void CallForHelp( char *szClassname, float flDist, EHANDLE hEnemy, Vector &vecLocation );

	int  IRelationship( CBaseEntity *pTarget );
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
	//MODDD
	void KeyValue( KeyValueData *pkvd );
	void HandleAnimEvent( MonsterEvent_t *pEvent );

	void RunAI( void );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	
	BOOL getHasPathFindingModA();
	BOOL getHasPathFindingMod();

	BOOL tempCheckTraceLineBlock;


	float blockSetActivity;
	Activity lastSetActivitySetting;
	BOOL lastSetActivityforceReset;

	float moveFlyNoInterrupt;

	int iPoisonSprite;


	//NOTE: confirm that the original "setObjectCollisionBox" method (default for monsters) is inadequate before using this override.
	/*
	void SetObjectCollisionBox( void )
	{
		pev->absmin = pev->origin + Vector( -80, -80, 0 );
		pev->absmax = pev->origin + Vector( 80, 80, 214 );
	}

	void SetObjectCollisionBox( void )
	{
		if(pev->deadflag != DEAD_NO){
			pev->absmin = pev->origin + Vector(-64, -64, 0);
			pev->absmax = pev->origin + Vector(64, 64, 50);
		}else{

			CBaseMonster::SetObjectCollisionBox();

		}
	}

	*/
	void SetObjectCollisionBox( void )
	{
		if(pev->deadflag != DEAD_NO){
			pev->absmin = pev->origin + Vector(-32, -32, 0 );
			pev->absmax = pev->origin + Vector(32, 32, 42);
		}else{
			pev->absmin = pev->origin + Vector(-28, -28, 0 );
			pev->absmax = pev->origin + Vector(28, 28, 54);
			//CBaseMonster::SetObjectCollisionBox();

		}
	}




	Schedule_t* GetSchedule ( void );
	Schedule_t* GetScheduleOfType ( int Type );
	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );
	CUSTOM_SCHEDULES;

	void Stop( void );
	
	void Move ( float flInterval );

	int  CheckLocalMove ( const Vector &vecStart, const Vector &vecEnd, CBaseEntity *pTarget, float *pflDist );
	
	void MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval );

	
	virtual BOOL allowedToSetActivity(void);
	virtual void SetActivity ( Activity NewActivity );
	virtual void SetActivity ( Activity NewActivity, BOOL forceReset );

	BOOL ShouldAdvanceRoute( float flWaypointDist, float flInterval );
	int LookupFloat( );
	
	int  ISoundMask( void );
	
	void RadiusDamageNoFriendly(entvars_t* pevInflictor, entvars_t*	pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType );
	
	
	void safeSetMoveFlyNoInterrupt(float timer);
	void safeSetBlockSetActivity(float timer);
					


	//???
	//float m_flShootTime;
	//float m_flShootEnd;


	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( void );



	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pAttackSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pDeathSounds[];



	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE


	void setAnimation(char* animationName);
	void setAnimation(char* animationName, BOOL forceException);
	void setAnimation(char* animationName, BOOL forceException, BOOL forceLoopsProperty);
	void setAnimation(char* animationName, BOOL forceException, BOOL forceLoopsProperty, int extraLogic);
		
	
	void tryDetachFromCeiling(void);


	GENERATE_KILLED_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE

	virtual BOOL isProvokable(void);
	virtual BOOL isProvoked(void);
	virtual void heardBulletHit(entvars_t* pevShooter);

	//DO NOT FORGET "EXPORT"!
	void EXPORT customTouch(CBaseEntity *pOther);
	

	float MoveYawDegreeTolerance(void);
	

	BOOL usesAdvancedAnimSystem(void);

	//void SetActivity ( Activity NewActivity );

	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);


	Vector m_vecEstVelocity;

	Vector m_velocity;
	int m_fInCombat;

	int tempThing;

	//MODDD - extra.
	void MonsterThink();

	Activity GetStoppedActivity( void );
	void SetTurnActivity();
	void SetTurnActivityCustom();


	BOOL landBrake;

	void updateMoveAnim();
	void getPathToEnemyCustom();
	void abortAttack();

	void checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist);
	void checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture);
	
	void checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist);
	void checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture);
	
	void checkFloor(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval);



	EHANDLE* getEnemy();

	void ReportAIState(void);

	Schedule_t* GetStumpedWaitSchedule(void);


	int getLoopingDeathSequence(void);


	
	BOOL violentDeathAllowed(void);
	BOOL violentDeathDamageRequirement(void);
	BOOL violentDeathClear(void);
	int violentDeathPriority(void);

	Activity getIdleActivity(void);







	Vector scentLocationMem;

	Activity recentActivity;
	int attackIndex;

	float attackEffectDelay;
	float attackAgainDelay;
	float maxDiveTime;

	float timeToIdle;

	int m_iSpawnLoc;
	
	BOOL combatCloseEnough;

	BOOL onGround;
	BOOL queueToggleGround;
	BOOL snappedToCeiling;
	BOOL queueToggleSnappedToCeiling;
	BOOL queueAbortAttack;
	
	int queueActionIndex;
	
	BOOL seekingFoodOnGround;
	float lastVelocityChange;

	BOOL rotationAllowed;
	BOOL dontResetActivity;

	float suicideAttackCooldown;

	float lastEnemey2DDistance;

	
	float HearingSensitivity( );

	//int movementHint;

	int chargeIndex;
	int m_voicePitch;

};





TYPEDESCRIPTION	CStukaBat::m_SaveData[] = 
{
	//example:
	//DEFINE_ARRAY( CController, m_pBall, FIELD_CLASSPTR, 2 ),
	DEFINE_FIELD(CStukaBat, tempThing, FIELD_INTEGER),
	DEFINE_FIELD(CStukaBat, blockSetActivity, FIELD_TIME),

	DEFINE_FIELD(CStukaBat, eating, FIELD_BOOLEAN),
	DEFINE_FIELD(CStukaBat, eatingAnticipatedEnd, FIELD_TIME),

	DEFINE_FIELD(CStukaBat, suicideAttackCooldown, FIELD_TIME),
	



	DEFINE_FIELD(CStukaBat, onGround, FIELD_BOOLEAN),
	DEFINE_FIELD(CStukaBat, snappedToCeiling, FIELD_BOOLEAN),
	
	DEFINE_FIELD(CStukaBat, queueToggleGround, FIELD_BOOLEAN),
	DEFINE_FIELD(CStukaBat, queueToggleSnappedToCeiling, FIELD_BOOLEAN),
	
	DEFINE_FIELD(CStukaBat, rotationAllowed, FIELD_BOOLEAN),
	
	DEFINE_FIELD(CStukaBat, queueAbortAttack, FIELD_BOOLEAN),
	DEFINE_FIELD(CStukaBat, chargeIndex, FIELD_INTEGER),
	
	DEFINE_FIELD( CStukaBat, m_voicePitch, FIELD_INTEGER),


};
//IMPLEMENT_SAVERESTORE( CStukaBat, CSquadMonster );
//NOTE: copied here for customization.
int CStukaBat::Save( CSave &save )
{
	if ( !CSquadMonster::Save(save) )
		return 0;
	return save.WriteFields( "CStukaBat", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}
int CStukaBat::Restore( CRestore &restore )
{
	lastVelocityChange = gpGlobals->time;
	//assuming this is okay.

	if ( !CSquadMonster::Restore(restore) )
		return 0;
	return restore.ReadFields( "CStukaBat", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}






const char *CStukaBat::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CStukaBat::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};

const char *CStukaBat::pAttackSounds[] = 
{
	"stuka/stuka_attack1.wav",
	"stuka/stuka_attack2.wav",
	"stuka/stuka_attack3.wav"
};

const char *CStukaBat::pIdleSounds[] = 
{
	"stuka/stuka_idle1.wav",
	"stuka/stuka_idle2.wav"
};

const char *CStukaBat::pAlertSounds[] = 
{
	"stuka/stuka_alert1.wav",
	"stuka/stuka_alert2.wav"
};

const char *CStukaBat::pPainSounds[] = 
{
	"stuka/stuka_pain1.wav",
	"stuka/stuka_pain2.wav",
	"stuka/stuka_pain3.wav"
};

const char *CStukaBat::pDeathSounds[] = 
{
	"stuka/stuka_death1.wav",
	"stuka/stuka_death2.wav"
};







enum
{
	TASK_SOUND_ATTACK = LAST_COMMON_TASK + 1,
	TASK_STUKABAT_LAND_PRE,
	TASK_STUKABAT_LAND,
	TASK_GET_PATH_TO_LANDING,
	TASK_GET_PATH_TO_BESTSCENT_FOOT,
	TASK_ACTION,
	TASK_STUKA_WAIT_FOR_ANIM,

};

//slStukaBatAnimWait

enum
{
	SCHED_STUKABAT_FINDEAT = LAST_COMMON_SCHEDULE + 1,
	SCHED_STUKABAT_ATTEMPTLAND,
	SCHED_STUKABAT_CRAWLTOFOOD,
	SCHED_STUKABAT_EAT,

	SCHED_STUKABAT_IDLE_HOVER,
	SCHED_STUKABAT_IDLE_GROUND,

	//TASK_WAIT_INDEFINITE !!!!!!!!!!!!!!!!!!!!!!
	SCHED_STUKABAT_ANIMWAIT,


	//SCHED_STUKABAT_HOVER_AIR
	//??
	
	//TASK_STUKA_EAT



	//others come here alone, like
	//SCHED_SCRATCH_ASS
	//...

};


//On adding a new schedule, make sure to see this too in stukabat.cpp (search exactly):
//NOTE: DEFINE_CUSTOM_SCHEDULES






//=========================================================
// AI Schedules Specific to this monster
//=========================================================
//NOTE: as always for Stukabat, based off of the controller.  Likely needs new methods.

// Chase enemy schedule
Task_t tlStukaBatChaseEnemy[] = 
{
	
	
	{ TASK_GET_PATH_TO_ENEMY,	(float)128		},

	//{TASK_STRAFE_PATH, (float)128 },

	//is that really wise alone..?
	//{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	
	
	//{ TASK_WAIT,	(float)1		},
	{ TASK_ACTION,	(float)1		},
	
	//CHECK. Is this okay??
	{ TASK_CHECK_STUMPED, (float)1		},


	//TASK_STRAFE_PATH

};

Schedule_t slStukaBatChaseEnemy[] =
{
	{ 
		tlStukaBatChaseEnemy,
		ARRAYSIZE ( tlStukaBatChaseEnemy ),

		/*
		bits_COND_NEW_ENEMY | bits_COND_TASK_FAILED
		//MODDD
		| bits_COND_CAN_RANGE_ATTACK1 | bits_COND_CAN_RANGE_ATTACK2 //???  | bits_COND_SMELL_FOOD
		//| bits_COND_SEE_DISLIKE | bits_COND_SEE_ENEMY | bits_COND_SEE_HATE | bits_COND_SEE_NEMESIS | bits_COND_HEAR_SOUND | bits_COND_SMELL
		*/
		0,

		bits_SOUND_MEAT|bits_SOUND_CARCASS,
		"StukaBatChaseEnemy"
	},
};






//unnecessary
/*
Task_t	tlStukaIdleHang[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)5		},// repick IDLESTAND every five seconds. gives us a chance to pick an active idle, fidget, etc.
};


Schedule_t	slStukaIdleHang[] =
{
	{ 
		tlStukaIdleHang,
		ARRAYSIZE ( tlStukaIdleHang ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED		|
		//MODDD - this is the new part!  Otherwise, a plain copy of "slIdleStand"
		//NO YOU IDIOT!  IT WAS HERE ALL ALONG, LOOK ABOVE JEEZ
		//~an affectionate note to myself.
		//~on a side-note, BLOOD FOR THE BLOOD GOD
		bits_COND_HEAR_SOUND,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"IdleStand"
	},
};
*/







Task_t	tlStukaIdleGround[] =
{
	//{ TASK_STOP_MOVING,			0				},
	//???
	{ TASK_SET_ACTIVITY,		(float)ACT_CROUCHIDLE},
	{ TASK_WAIT,				(float)2		},// repick IDLESTAND every five seconds. gives us a chance to pick an active idle, fidget, etc.
};

Schedule_t	slStukaIdleGround[] =
{
	{ 
		tlStukaIdleGround,
		ARRAYSIZE ( tlStukaIdleGround ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		//bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"STUIdleGround"
	},
};






Task_t	tlStukaIdleHover[] =
{
	//{ TASK_STOP_MOVING,			0				},
	//???
	{ TASK_SET_ACTIVITY,		(float)ACT_HOVER },
	{ TASK_WAIT,				(float)2		},// repick IDLESTAND every five seconds. gives us a chance to pick an active idle, fidget, etc.
};

Schedule_t	slStukaIdleHover[] =
{
	{ 
		tlStukaIdleHover,
		ARRAYSIZE ( tlStukaIdleHover ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		//bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"STUIdleHover"
	},
};










//UNUSED. And beware of "MOVE_TO_TARGET" range, that is different from the ENEMY.
//m_hTarget may or may not be set at all or match the enemy, chances are it will not even be used here.

Task_t	tlStukaBatStrafe[] =
{
	{ TASK_WAIT,					(float)0.2					},
	//{ TASK_GET_PATH_TO_ENEMY,		(float)128					},
	
	{ TASK_MOVE_TO_TARGET_RANGE,	(float)128					},
	

	//{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_WAIT,					(float)0.3					},
};

Schedule_t	slStukaBatStrafe[] =
{
	{ 
		tlStukaBatStrafe,
		ARRAYSIZE ( tlStukaBatStrafe ), 
		bits_COND_NEW_ENEMY,
		0,
		"StukaBatStrafe"
	},
};


Task_t	tlStukaBatTakeCover[] =
{
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_WAIT,					(float)1					},
};

Schedule_t	slStukaBatTakeCover[] =
{
	{ 
		tlStukaBatTakeCover,
		ARRAYSIZE ( tlStukaBatTakeCover ), 
		bits_COND_NEW_ENEMY,
		0,
		"StukaBatTakeCover"
	},
};


//removed "ACT_IDLE", seems more distracting.
Task_t tlStukaBatFail[] =
{
	{ TASK_STOP_MOVING,			0				},

	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)1		},
	//{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slStukaBatFail[] =
{
	{
		tlStukaBatFail,
		ARRAYSIZE ( tlStukaBatFail ),
		/*
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND,
		*/
		0,
		0,
		"StukaBatFail"
	},
};




// secondary range attack
Task_t	tlStukaBatRangeAttack2[] =
{
	//{ TASK_STOP_MOVING,			0				},
	//{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_RANGE_ATTACK2,		(float)0		},
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
};

Schedule_t	slStukaBatRangeAttack2[] =
{
	{
		tlStukaBatRangeAttack2,
		ARRAYSIZE ( tlStukaBatRangeAttack2 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"StukaBatRangeAttack2"
	},
};





//{ TASK_GET_PATH_TO_ENEMY,	(float)128		},
//{ TASK_WAIT,	(float)1		},



Task_t tlStukaBatFindEat[] =
{
	//{ TASK_STOP_MOVING,				(float)0				},
	
	{ TASK_GET_PATH_TO_BESTSCENT,	(float)0				},
	//{ TASK_WAIT,	(float)1		}
	{ TASK_WAIT_FOR_MOVEMENT, (float) 0},

};


Schedule_t slStukaBatFindEat[] =
{
	{
		tlStukaBatFindEat,
		ARRAYSIZE( tlStukaBatFindEat ),
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_NEW_ENEMY	,
		
		// even though HEAR_SOUND/SMELL FOOD doesn't break this schedule, we need this mask
		// here or the monster won't detect these sounds at ALL while running this schedule.
		bits_SOUND_MEAT			|
		bits_SOUND_CARCASS,
		"slSBFindEat"
	}
};







Task_t tlStukaBatAttemptLand[] =
{
	//{ TASK_STOP_MOVING,				(float)0				},
	
	{ TASK_GET_PATH_TO_LANDING,	(float)0				},
	{ TASK_WAIT_FOR_MOVEMENT, (float) 0},
	{ TASK_STUKABAT_LAND_PRE, (float) 0},
	{ TASK_STUKABAT_LAND, (float) 0},
	//{ TASK_WAIT,	(float)1		}


};


Schedule_t slStukaBatAttemptLand[] =
{
	{
		tlStukaBatAttemptLand,
		ARRAYSIZE( tlStukaBatAttemptLand ),
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_NEW_ENEMY,
		
		/*
		// even though HEAR_SOUND/SMELL FOOD doesn't break this schedule, we need this mask
		// here or the monster won't detect these sounds at ALL while running this schedule.
		bits_SOUND_MEAT			|
		bits_SOUND_CARCASS,
		*/
		0,
		"slSBAttemptLand"
	}
};






Task_t tlStukaBatCrawlToFood[] =
{
	//{ TASK_STOP_MOVING,				(float)0				},
	
	{ TASK_GET_PATH_TO_BESTSCENT_FOOT,	(float)0				},
	//{ TASK_WAIT,	(float)1		}
	{ TASK_WAIT_FOR_MOVEMENT, (float) 0},


};


Schedule_t slStukaBatCrawlToFood[] =
{
	{
		tlStukaBatCrawlToFood,
		ARRAYSIZE( tlStukaBatCrawlToFood ),
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_NEW_ENEMY,
		
		
		// even though HEAR_SOUND/SMELL FOOD doesn't break this schedule, we need this mask
		// here or the monster won't detect these sounds at ALL while running this schedule.
		bits_SOUND_MEAT			|
		bits_SOUND_CARCASS,
		
		//0,
		"slSBCrawlToFood"
	}
};




Task_t tlStukaBatEat[] =
{
	{ TASK_STOP_MOVING,				(float)0				},
	
	{ TASK_SET_ACTIVITY_FORCE,			(float)ACT_EAT},
	{ TASK_FACE_IDEAL,				(float)0				},
	{ TASK_WAIT,	(float)2		},
	{ TASK_EAT,	(float)50				},
	{ TASK_WAIT,	(float)8		},
	//If in the middle of one eating cycle, go ahead and let it finish.
	{ TASK_WAIT_FOR_SEQUENCEFINISH, (float) 0 },
	//Just send a signal to pick a different activity, this should work out.
	{ TASK_SET_ACTIVITY, (float)ACT_RESET },

	//{TASK_SET_ACTIVITY_FORCE, (float)ACT_CROUCHIDLE},


};


Schedule_t slStukaBatEat[] =
{
	{
		tlStukaBatEat,
		ARRAYSIZE( tlStukaBatEat ),
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_NEW_ENEMY |
		
		/*
		// even though HEAR_SOUND/SMELL FOOD doesn't break this schedule, we need this mask
		// here or the monster won't detect these sounds at ALL while running this schedule.
		bits_SOUND_MEAT			|
		bits_SOUND_CARCASS,
		*/
		bits_COND_SEE_DISLIKE		|
		bits_COND_SEE_HATE      |
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		//bits_COND_SMELL_FOOD	|
		//bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER,

		"StukaBatEat"
	}
};







/*
			//no, wrap animation-changes into a schedule!
			//check if blockSetActivity is done?
			if(blockSetActivity == -1){
				TaskComplete();
				return;
			}
			*/
			
Task_t tlStukaBatAnimWait[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_STUKA_WAIT_FOR_ANIM,	(float)0		},


};

Schedule_t slStukaBatAnimWait[] =
{
	{
		tlStukaBatAnimWait,
		ARRAYSIZE( tlStukaBatAnimWait ),
		0,
		0,
		"StukaBatWaitForAnim"
	}
};




Task_t	tlStukaPathfindStumped[] =
{

	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	//TASK_FACE_IDEAL ?
	{ TASK_FACE_PREV_LKP,			(float)0	},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },  ???
	{ TASK_WAIT,				(float)10		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slStukaPathfindStumped[] =
{
	{
		tlStukaPathfindStumped,
		ARRAYSIZE ( tlStukaPathfindStumped ),

		bits_COND_CAN_ATTACK |
		bits_COND_SEE_ENEMY |
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND	|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		//bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|
		bits_SOUND_BAIT,

		"StukaPathfindStumped"
	},
};

