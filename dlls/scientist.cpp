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
//=========================================================
// human scientist (passive lab worker)
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"talkmonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"scripted.h"
#include	"animation.h"
#include	"soundent.h"

//MODDD - added, so that the player's "setSuitUpdate" may be called directly (edicts are too general
//and don't have that, nor are they safely moddable to my knowledge, very DLL intensive in transfers).
#include "player.h"



//TODO... god even more.
//Make it so taking damage from TIMED_DAMAGE or TIMED_DAMAGE_MOD (right bitmask) doesn't unfollow. doesn't make sense to, no direct enemy to scare them to cause this damage.







//NOTICE - are these unused?

/*
SC_NOGO scientist/dontgothere

SC_MONST0 scientist/seeheadcrab
SC_MONST1 scientist/importantspecies

SC_HEAR0 scientist/ihearsomething
SC_HEAR1 scientist/didyouhear
SC_HEAR2 scientist/whatissound


*/







//SCIENTIST: say a line from fear's or NOOO when player dies while follo





//MODDD
extern float global_wildHeads;

extern float global_germanCensorship;

extern float global_scientistHealNPCDebug;
extern float global_scientistHealNPC;
extern float global_thatWasntPunch;

extern float global_scientistHealNPCFract;
extern float global_scientistHealCooldown;

extern float global_monsterSpawnPrintout;
extern BOOL globalPSEUDO_iCanHazMemez;

//was this model found in the client's folder too?
extern BOOL globalPSEUDO_germanModel_scientistFound;

extern float global_scientistBravery;





extern void scientistHeadFilter( CBaseMonster& somePerson, int arg_numberOfModelBodyParts, int* trueBody);




#define		NUM_SCIENTIST_HEADS		3


//MODDD - there is a rather suble problem with this setup.
//Old head enumeration:
//enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_LUTHER = 2, HEAD_SLICK = 3}
//new head enumeration:
//enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_SLICK = 2 };

//And, here is the retail model's head:
//HEAD_GLASSES
//HEAD_EINSTEIN
//HEAD_LUTHER
//HEAD_SLICK
//See the issue?  If you want to remove anything BUT the last one, simply cutting the last one won't be completely effective.

//So, better idea:  handle the offset (if not using the alpha model that has 3 head models).  Otherwise, this is not necessary:
//So if there are any immediate issues, try adjusting this first.
#define headOffsetFix 0
//NOTE: the alpa model can still treat "HEAD_SLICK" as the egon head if it sticks to being "2".  What is in a name, after all?

#if headOffsetFix == 0
	enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_SLICK = 2 };
	int scientistHeadsModelRef[] = {0, 1, 2};
#else
	enum { HEAD_GLASSES = 0, HEAD_EINSTEIN = 1, HEAD_SLICK = 3 };
	int scientistHeadsModelRef[] = {0, 1, 3};
#endif

enum
{
	SCHED_HIDE = LAST_TALKMONSTER_SCHEDULE + 1,
	SCHED_FEAR,
	SCHED_PANIC,
	SCHED_STARTLE,
	SCHED_TARGET_CHASE_SCARED,
	SCHED_TARGET_FACE_SCARED,
	SCHED_SCIENTIST_ANGRY_CHASE_ENEMY,
	SCHED_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED,
};


	
enum
{
	TASK_SAY_HEAL = LAST_TALKMONSTER_TASK + 1,
	TASK_HEAL,
	TASK_SAY_FEAR,
	TASK_RUN_PATH_SCARED,
	TASK_SCREAM,
	TASK_RANDOM_SCREAM,
	TASK_MOVE_TO_TARGET_RANGE_SCARED,
	TASK_SCIENTIST_FIGHT_OR_FLIGHT,
	TASK_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED
};

//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		SCIENTIST_AE_HEAL		( 1 )
#define		SCIENTIST_AE_NEEDLEON	( 2 )
#define		SCIENTIST_AE_NEEDLEOFF	( 3 )



extern float global_pissedNPCs;

//=======================================================
// Scientist
//=======================================================

class CScientist : public CTalkMonster
{
public:

	static const char* pDeathSounds[];

	float aggro;

	float aggroCooldown;
	float playFearAnimCooldown;

	Vector aggroOrigin;


	CScientist(void);
	int getMadSentencesMax(void);
	int getMadInterSentencesMax(void);

	
	static const char*		madInterSentences[];
	static int				madInterSentencesMax;


	void forgetHealNPC(void);

    //MODDD
    BOOL healNPCChosen;

	//MODDD
	void StartFollowing(CBaseEntity *pLeader);


	//MODDD
	void Activate( void );
	//void playPissed();

	void Spawn( void );
	void Precache( void );

	void SetYawSpeed( void );
	int  Classify ( void );
	BOOL isOrganic(void){return !CanUseGermanModel();}

	void HandleAnimEvent( MonsterEvent_t *pEvent );
	void RunTask( Task_t *pTask );
	void StartTask( Task_t *pTask );
	int	ObjectCaps( void ) { return CTalkMonster :: ObjectCaps() | FCAP_IMPULSE_USE; }

	//MODDD
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	//MODDD
	static int numberOfModelBodyParts;
	int trueBody;
	float healNPCCheckDelay;


	virtual int FriendNumber( int arrayNumber );
	void SetActivity ( Activity newActivity );
	Activity GetStoppedActivity( void );
	int ISoundMask( void );
	void DeclineFollowing( void );

	float	CoverRadius( void ) { return 1200; }		// Need more room for cover because scientists want to get far away!
	
	//MODDD MAJOR - DEBUG. NO.
	//BOOL	DisregardEnemy( CBaseEntity *pEnemy ) { return !pEnemy->IsAlive() || (gpGlobals->time - m_fearTime) > 15; }
	BOOL	DisregardEnemy( CBaseEntity *pEnemy ) { return FALSE; }

	BOOL	CanHeal( void );
	//MODDD
	BOOL	CanHeal(CBaseMonster* arg_monsterTry);

	void	Heal( void );
	void	Scream( void );

	//MODDD
	void ScheduleChange(void);

	// Override these to set behavior
	Schedule_t *GetScheduleOfType ( int Type );
	Schedule_t *GetSchedule ( void );
	MONSTERSTATE GetIdealState ( void );


	//MODDD - new.
	void SetObjectCollisionBox( void )
	{
		if(pev->deadflag != DEAD_NO){
			pev->absmin = pev->origin + Vector(-65, -65, 0);
			pev->absmax = pev->origin + Vector(65, 65, 72);
		}else{

			CBaseMonster::SetObjectCollisionBox();

		}
	}

	//yes?
	void MonsterThink(void);


	//MODDD
	void setModel(void);
	void setModel(const char* m);
	BOOL getGermanModelRequirement(void);
	const char* getGermanModel(void);
	const char* getNormalModel(void);


	void DeathSound( void );
	void PainSound( void );

	void tempMethod(void);


	void TalkInit( void );

	GENERATE_KILLED_PROTOTYPE

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
	
	void PostRestore(void);

	CUSTOM_SCHEDULES;


	
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	
	void HandleEventQueueEvent(int arg_eventID);
	BOOL usesAdvancedAnimSystem(void);
	int tryActivitySubstitute(int activity);
	int LookupActivityHard(int activity);

	
	void DeclineFollowingProvoked(CBaseEntity* pCaller);
	void SayProvoked(void);
	void SaySuspicious(void);
	void SayLeaderDied(void);
	void SayNearPassive(void);
	void SayNearCautious(void);

	void ReportAIState(void);

	BOOL violentDeathAllowed(void);
	BOOL violentDeathClear(void);

	void initiateAss(void);
	void myAssHungers(void);

	
//MODDD - new data stuff.
	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];



private:
	float m_painTime;
	float m_healTime;
	float m_fearTime;
	float explodeDelay;
};


const char* CScientist::pDeathSounds[] = 
{
	"scientist/sci_die1.wav",
	"scientist/sci_die2.wav",
	"scientist/sci_die3.wav",
	"scientist/sci_die4.wav",
};




int CScientist::numberOfModelBodyParts = -1;

#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_scientist, CScientist );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( scientist, CScientist );

	//no extras.

#endif

TYPEDESCRIPTION	CScientist::m_SaveData[] =
{
	DEFINE_FIELD( CScientist, m_painTime, FIELD_TIME ),
	DEFINE_FIELD( CScientist, m_healTime, FIELD_TIME ),
	DEFINE_FIELD( CScientist, m_fearTime, FIELD_TIME ),
	//MODDD
	DEFINE_FIELD( CScientist, trueBody, FIELD_INTEGER ),



};

IMPLEMENT_SAVERESTORE( CScientist, CTalkMonster );

void CScientist::PostRestore(){
	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);
	
}


//MODDD - new data stuff.	
const char *CScientist::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char *CScientist::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};





const char* CScientist::madInterSentences[] = {
	"!SC_POKE0",
	"!SC_POKE1",
	"!SC_POKE2",
	"!SC_POKE3",
	"!SC_POKE4",
	"!SC_POKE5",
	"!SC_POKEQ0",
	"!SC_POKEQ1",
	////"!SC_POKE8",
	"!SC_POKE9",
	"!SC_POKE10",
	//"!SC_POKEQ2",
	"!SC_POKE12",
	//"!SC_POKEQ3",
	"!SC_POKE14",
	"!SC_POKE15",
	"!SC_POKE16",
	"!SC_POKE17",
	"!SC_POKE18",
	"!SC_POKE19",
	"!SC_POKE20",
	"!SC_POKE21",
	"!SC_POKE22",
	"!SC_POKE23",
	"!SC_POKE24",
	"!SC_POKE25",
	"!SC_POKE26",
	"!SC_POKE27",
	"!SC_POKE28",
	"!SC_POKE29",
	"!SC_POKE30",
	"!SC_POKE31",
	"!SC_POKE32",
	"!SC_POKE33",
	"!SC_POKE34",
	"!SC_POKE35",
	"!SC_POKE36"


};
//int CScientist::madInterSentencesMax = 37 - 1;






int CScientist::getMadInterSentencesMax(void){
	if(globalPSEUDO_iCanHazMemez == TRUE){
		return 34;
	}else{
		return 33;
	}
}
int CScientist::getMadSentencesMax(void){
	if(globalPSEUDO_iCanHazMemez == TRUE){
		return 37;
	}else{
		return 36;
	}
}




// Chase enemy schedule
Task_t tlAngryScientistChaseEnemyFailed[] = 
{
	{ TASK_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED, (float)0 },
	//no, just allow to repick a schedule.
	//{ TASK_SET_SCHEDULE,	(float)???	}
};

Schedule_t slAngryScientistChaseEnemyFailed[] =
{
	{ 
		tlAngryScientistChaseEnemyFailed,
		ARRAYSIZE ( tlAngryScientistChaseEnemyFailed ),
		0,
		0,
		"Chase Enemy ASf"
	},
};


// Chase enemy schedule
Task_t tlAngryScientistChaseEnemy[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED	},
	//{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	//{ TASK_RUN_PATH,			(float)0		},
	//{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	//Now modeled after the SmartFollow in schedule.cpp.
	{ TASK_MOVE_TO_ENEMY_RANGE,(float)0		}
};

Schedule_t slAngryScientistChaseEnemy[] =
{
	{ 
		tlAngryScientistChaseEnemy,
		ARRAYSIZE ( tlAngryScientistChaseEnemy ),
		bits_COND_NEW_ENEMY			|
		//MODDD - added, the bullsquid counts this.  Why doesn't everything?
		bits_COND_ENEMY_DEAD |

		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_TASK_FAILED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER|bits_SOUND_COMBAT,
		"Chase Enemy AS"
	},
};



Task_t	tlScientistPunch[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_MELEE_ATTACK2,		(float)0		},
};

Schedule_t	slScientistPunch[] =
{
	{ 
		tlScientistPunch,
		ARRAYSIZE ( tlScientistPunch ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		//bits_COND_LIGHT_DAMAGE		|
		//bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED,
		0,
		"scientist punch"
	},
};





//=========================================================
// AI Schedules Specific to this monster
//=========================================================
Task_t	tlFollow[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CANT_FOLLOW },	// If you fail, bail out of follow
	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_FOLLOW_SUCCESSFUL, (float)0		},
//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};

Schedule_t	slFollow[] =
{
	{
		tlFollow,
		ARRAYSIZE ( tlFollow ),
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"Follow"
	},
};

Task_t	tlFollowScared[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_TARGET_CHASE },// If you fail, follow normally
	{ TASK_MOVE_TO_TARGET_RANGE_SCARED,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_FOLLOW_SUCCESSFUL, (float)0		},
//	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE_SCARED },
};

Schedule_t	slFollowScared[] =
{
	{
		tlFollowScared,
		ARRAYSIZE ( tlFollowScared ),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE,
		bits_SOUND_DANGER,
		"FollowScared"
	},
};

Task_t	tlFaceTargetScared[] =
{
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_CROUCHIDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE_SCARED },
};

Schedule_t	slFaceTargetScared[] =
{
	{
		tlFaceTargetScared,
		ARRAYSIZE ( tlFaceTargetScared ),
		bits_COND_HEAR_SOUND |
		bits_COND_NEW_ENEMY,
		bits_SOUND_DANGER,
		"FaceTargetScared"
	},
};

//MODDD - stopFollowing schedule moved to talkMonster


Task_t	tlHeal[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_TARGET_CHASE },	// If you fail, catch up with that guy! (change this to put syringe away and then chase)
	{ TASK_MOVE_TO_TARGET_RANGE,(float)50		},	// Move within 60 of target ent (client)
	{ TASK_FOLLOW_SUCCESSFUL, (float)0		},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_SAY_HEAL,			(float)0		},
	{ TASK_PLAY_SEQUENCE_FACE_TARGET,		(float)ACT_ARM	},			// Whip out the needle
	{ TASK_HEAL,				(float)0	},	// Put it in the player
	{ TASK_PLAY_SEQUENCE_FACE_TARGET,		(float)ACT_DISARM	},			// Put away the needle
};

Schedule_t	slHeal[] =
{
	{
		tlHeal,
		ARRAYSIZE ( tlHeal ),
		0,	// Don't interrupt or he'll end up running around with a needle all the time
		0,
		"Heal"
	},
};


Task_t	tlFaceTarget[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_TARGET,			(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_CHASE },
};

Schedule_t	slFaceTarget[] =
{
	{
		tlFaceTarget,
		ARRAYSIZE ( tlFaceTarget ),
		bits_COND_CLIENT_PUSH |
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT |
		bits_SOUND_DANGER,
		"FaceTarget"
	},
};


Task_t	tlSciPanic[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_FACE_ENEMY,			(float)0		},
	//MODDD - intervention.
	//{ TASK_SCREAM,				(float)0		},
	{ TASK_SCIENTIST_FIGHT_OR_FLIGHT,  (float)1  },    //NOTICE - this "1" means he is more likely to attack the monster, since Panic is picked when we're cornered (no cover, no choice)
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,		(float)ACT_EXCITED	},	// This is really fear-stricken excitement
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE	},
};

Schedule_t	slSciPanic[] =
{
	{
		tlSciPanic,
		ARRAYSIZE ( tlSciPanic ),
		0,
		0,
		"SciPanic"
	},
};


Task_t	tlIdleSciStand[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		}, // repick IDLESTAND every two seconds.
	{ TASK_TLK_HEADRESET,		(float)0		}, // reset head position
};

Schedule_t	slIdleSciStand[] =
{
	{
		tlIdleSciStand,
		ARRAYSIZE ( tlIdleSciStand ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL			|
		bits_COND_CLIENT_PUSH	|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		//bits_SOUND_PLAYER		|
		//bits_SOUND_WORLD		|
		bits_SOUND_DANGER		|
		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE,
		"IdleSciStand"

	},
};


Task_t	tlScientistCover[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH_SCARED,			(float)0					},
	{ TASK_TURN_LEFT,				(float)179					},
	{ TASK_SET_SCHEDULE,			(float)SCHED_HIDE			},
};

Schedule_t	slScientistCover[] =
{
	{
		tlScientistCover,
		ARRAYSIZE ( tlScientistCover ),
		bits_COND_NEW_ENEMY,
		0,
		"ScientistCover"
	},
};



Task_t	tlScientistHide[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_PLAY_SEQUENCE,			(float)ACT_CROUCH			},
	{ TASK_SET_ACTIVITY,			(float)ACT_CROUCHIDLE		},	// FIXME: This looks lame
	{ TASK_WAIT_RANDOM,				(float)10.0					},
};

Schedule_t	slScientistHide[] =
{
	{
		tlScientistHide,
		ARRAYSIZE ( tlScientistHide ),
		bits_COND_NEW_ENEMY |
		bits_COND_HEAR_SOUND |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE,
		bits_SOUND_DANGER,
		"ScientistHide"
	},
};


Task_t	tlScientistStartle[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_PANIC },		// If you fail, just panic!
	{ TASK_RANDOM_SCREAM,			(float)0.3 },				// Scream 30% of the time
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCH			},
	{ TASK_RANDOM_SCREAM,			(float)0.1 },				// Scream again 10% of the time
	{ TASK_PLAY_SEQUENCE_FACE_ENEMY,			(float)ACT_CROUCHIDLE		},
	{ TASK_WAIT_RANDOM,				(float)1.0					},
};

Schedule_t	slScientistStartle[] =
{
	{
		tlScientistStartle,
		ARRAYSIZE ( tlScientistStartle ),
		bits_COND_NEW_ENEMY |
		bits_COND_SEE_ENEMY |
		bits_COND_SEE_HATE |
		bits_COND_SEE_FEAR |
		bits_COND_SEE_DISLIKE |
		//MODDD - new
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE,
		0,
		"ScientistStartle"
	},
};



Task_t	tlFear[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_FACE_ENEMY,				(float)0					},
	
	{ TASK_SAY_FEAR,				(float)0					},

	//MODDD - new.
	{ TASK_SCIENTIST_FIGHT_OR_FLIGHT,  (float)0   },

//MODDD: used to be commented out, no longer is.
	{ TASK_PLAY_SEQUENCE,			(float)ACT_FEAR_DISPLAY		},
};

Schedule_t	slFear[] =
{
	{
		tlFear,
		ARRAYSIZE ( tlFear ),
		//MODDD - added some interrupt conditions.
		//        Got it, you love gawking at the thing you're afraid of, but I think bullets and/or claws ought to make you hurry that up a bit.
		bits_COND_NEW_ENEMY |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_DANGER,
		"Fear"
	},
};


DEFINE_CUSTOM_SCHEDULES( CScientist )
{
	slFollow,
	slFaceTarget,
	slIdleSciStand,
	slFear,
	slScientistCover,
	slScientistHide,
	slScientistStartle,
	slHeal,
	//slStopFollowing,    Belongs to all of TalkMonster now!
	slSciPanic,
	slFollowScared,
	slFaceTargetScared,
	slAngryScientistChaseEnemyFailed,
	slAngryScientistChaseEnemy,
	slScientistPunch,
};


IMPLEMENT_CUSTOM_SCHEDULES( CScientist, CTalkMonster );








	
void CScientist::DeclineFollowingProvoked(CBaseEntity* pCaller){
	//Barney won't say anything, he's too busy shooting you.
	//...or will he? MUHAHAHA.
	
	if(global_pissedNPCs != 1 || !globalPSEUDO_iCanHazMemez){
		PlaySentence( "SC_SCREAM_TRU", 4, VOL_NORM, ATTN_NORM );  //OH NO YOU FOUND ME.
	}else{
		PlaySentence( "BA_POKE_D", 8, VOL_NORM, ATTN_NORM );
	}

	if(this->m_pSchedule == slScientistCover ){
		//no reaction, already running.

	}else if(aggro <= 0){
		//try the fight or flight. No random chance of going agro if there is valid cover though.
		ChangeSchedule(GetScheduleOfType(SCHED_FIGHT_OR_FLIGHT));

	}else{  //aggro above > 0? for now, no effect, they are already angry.

	}

}
void CScientist::SayProvoked(void){
	
	if(global_pissedNPCs != 1 || !globalPSEUDO_iCanHazMemez){

		switch(RANDOM_LONG(0, 4)){
			case 0:
				EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch());
			break;
			case 1:
				PlaySentence( "!SC_FEAR3", 6, VOL_NORM, ATTN_NORM );
			break;
			case 2:
				PlaySentence( "!SC_PLFEAR3", 6, VOL_NORM, ATTN_NORM );
			break;
			case 3:
				PlaySentence( "!SCI_EXTRAPROVOKED", 6, VOL_NORM, ATTN_NORM );
			break;
			case 4:
				PlaySentence( "SC_SCREAM_TRU", 6, VOL_NORM, ATTN_NORM );
			break;
		}//END OF decision

	}else{
		PlaySentence( "BA_POKE_D", 8, VOL_NORM, ATTN_NORM );
	}

	
	if(this->m_pSchedule == slScientistCover ){
		//no reaction, already running.

	}else if(aggro <= 0){
		//try the fight or flight. No random chance of going agro if there is valid cover though.

		//UH OH. IS THIS LINE OFFENDING? Let's say this in a different place perhaps.
		//ChangeSchedule(GetScheduleOfType(SCHED_FIGHT_OR_FLIGHT));

	}else{  //aggro above > 0? for now, no effect, they are already angry.

	}

}
void CScientist::SaySuspicious(void){
	if(global_pissedNPCs != 1 || !globalPSEUDO_iCanHazMemez){
	
		switch(RANDOM_LONG(0, 7)){
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			PlaySentence( "SC_SCREAM", 4, VOL_NORM, ATTN_NORM ); //actually ends up tying to the sci_fear sounds.
			break;
			case 6:
				PlaySentence( "!SC_PLFEAR3", 4, VOL_NORM, ATTN_NORM ); //scientist/noplease
			break;
			case 7:
				PlaySentence( "!SC_FEAR0", 4, VOL_NORM, ATTN_NORM ); //nooo
			break;
		}//END OF switch

	}else{
		PlaySentence( "BA_POKE_C", 6, VOL_NORM, ATTN_NORM );
	}
}
void CScientist::SayLeaderDied(void){

	switch(RANDOM_LONG(0, 7)){
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			PlaySentence( "SC_SCREAM", 4, VOL_NORM, ATTN_NORM );
		break;
		case 5:
			PlaySentence( "!SC_PLFEAR3", 4, VOL_NORM, ATTN_NORM ); //scientist/noplease
		break;
		case 6:
			PlaySentence( "!SC_PLFEAR4", 4, VOL_NORM, ATTN_NORM ); //getoutofhere
		break;
		case 7:
			PlaySentence( "!SC_FEAR0", 4, VOL_NORM, ATTN_NORM ); //nooo
		break;
	}//END OF switch
	

}//END OF SayLeaderDied




//Say a sentence to express interest in something, like stopping to stare at a chumtoad.
void CScientist::SayNearPassive(void){


	
	switch(RANDOM_LONG(0, 17)){
	case 0:
		PlaySentenceSingular( "SC_QUESTION0", 4, VOL_NORM, ATTN_NORM );
	break;
	case 1:
		PlaySentenceSingular( "SC_QUESTION2", 4, VOL_NORM, ATTN_NORM );
	break;
	case 2:
		PlaySentenceSingular( "SC_QUESTION7", 4, VOL_NORM, ATTN_NORM );
	break;
	case 3:
		PlaySentenceSingular( "SC_QUESTION8", 4, VOL_NORM, ATTN_NORM );
	break;
	case 4:
		PlaySentenceSingular( "SC_QUESTION10", 4, VOL_NORM, ATTN_NORM );
	break;
	case 5:
		PlaySentenceSingular( "SC_QUESTION11", 4, VOL_NORM, ATTN_NORM );
	break;
	case 6:
		PlaySentenceSingular( "SC_QUESTION16", 4, VOL_NORM, ATTN_NORM );
	break;
	case 7:
		PlaySentenceSingular( "SC_QUESTION17", 4, VOL_NORM, ATTN_NORM );
	break;
	case 8:
		PlaySentenceSingular( "SC_QUESTION18", 4, VOL_NORM, ATTN_NORM );
	break;
	case 9:
		PlaySentenceSingular( "SC_QUESTION22", 4, VOL_NORM, ATTN_NORM );
	break;
	case 10:
		PlaySentenceSingular( "SC_IDLE3", 4, VOL_NORM, ATTN_NORM );
	break;
	case 11:
		PlaySentenceSingular( "SC_IDLE4", 4, VOL_NORM, ATTN_NORM );
	break;
	case 12:
		PlaySentenceSingular( "SC_IDLE5", 4, VOL_NORM, ATTN_NORM );
	break;
	case 13:
		PlaySentenceSingular( "SC_IDLE11", 4, VOL_NORM, ATTN_NORM );
	break;
	case 14:
		PlaySentenceSingular( "SC_IDLE13", 4, VOL_NORM, ATTN_NORM );
	break;
	case 15:
		PlaySentenceSingular( "SC_MONST1", 4, VOL_NORM, ATTN_NORM );
	break;
	case 16:
		PlaySentenceSingular( "SC_SMELL2", 4, VOL_NORM, ATTN_NORM );
	break;
	case 17:
		PlaySentenceSingular( "SC_SMELL3", 4, VOL_NORM, ATTN_NORM );
	break;
	default:

	break;
	}//END OF switch

	
}//END OF SayNearPassive


void CScientist::SayNearCautious(void){
	


	switch(RANDOM_LONG(0, 23)){
	case 0:
		PlaySentenceSingular( "SC_HEAR0", 4, VOL_NORM, ATTN_NORM );
	break;
	case 1:
		PlaySentenceSingular( "SC_HEAR1", 4, VOL_NORM, ATTN_NORM );
	break;
	case 2:
		PlaySentenceSingular( "SC_HEAR2", 4, VOL_NORM, ATTN_NORM );
	break;
	case 3:
		PlaySentenceSingular( "SC_FEAR0", 4, VOL_NORM, ATTN_NORM );
	break;
	case 4:
		PlaySentenceSingular( "SC_FEAR1", 4, VOL_NORM, ATTN_NORM );
	break;
	case 5:
		PlaySentenceSingular( "SC_FEAR2", 4, VOL_NORM, ATTN_NORM );
	break;
	case 6:
		PlaySentenceSingular( "SC_FEAR4", 4, VOL_NORM, ATTN_NORM );
	break;
	case 7:
		PlaySentenceSingular( "SC_FEAR5", 4, VOL_NORM, ATTN_NORM );
	break;
	case 8:
		PlaySentenceSingular( "SC_FEAR6", 4, VOL_NORM, ATTN_NORM );
	break;
	case 9:
		PlaySentenceSingular( "SC_FEAR7", 4, VOL_NORM, ATTN_NORM );
	break;
	case 10:
		PlaySentenceSingular( "SC_FEAR8", 4, VOL_NORM, ATTN_NORM );
	break;
	case 11:
		PlaySentenceSingular( "SC_FEAR9", 4, VOL_NORM, ATTN_NORM );
	break;
	case 12:
		PlaySentenceSingular( "SC_FEAR10", 4, VOL_NORM, ATTN_NORM );
	break;
	case 13:
		PlaySentenceSingular( "SC_FEAR11", 4, VOL_NORM, ATTN_NORM );
	break;
	case 14:
		PlaySentenceSingular( "SC_FEAR12", 4, VOL_NORM, ATTN_NORM );
	break;
	case 15:
		PlaySentenceSingular( "SC_SCREAM1", 4, VOL_NORM, ATTN_NORM );
	break;
	case 16:
		PlaySentenceSingular( "SC_SCREAM3", 4, VOL_NORM, ATTN_NORM );
	break;
	case 17:
		PlaySentenceSingular( "SC_SCREAM4", 4, VOL_NORM, ATTN_NORM );
	break;
	case 18:
		PlaySentenceSingular( "SC_SCREAM5", 4, VOL_NORM, ATTN_NORM );
	break;
	case 19:
		PlaySentenceSingular( "SC_SCREAM10", 4, VOL_NORM, ATTN_NORM );
	break;
	case 20:
		PlaySentenceSingular( "SC_SCREAM11", 4, VOL_NORM, ATTN_NORM );
	break;
	case 21:
		PlaySentenceSingular( "SC_QUESTION4", 4, VOL_NORM, ATTN_NORM );
	break;
	case 22:
		PlaySentenceSingular( "SC_QUESTION5", 4, VOL_NORM, ATTN_NORM );
	break;
	case 23:
		PlaySentenceSingular( "SC_IDLE13", 4, VOL_NORM, ATTN_NORM );
	break;
	default:

	break;
	}//END OF switch

}//END OF SayNearCautious
	
















void CScientist::DeclineFollowing( void )
{
	//MODDD
	if(global_pissedNPCs < 1){
		Talk( 10 );
		m_hTalkTarget = m_hEnemy;
		PlaySentence( "SC_POK", 2, VOL_NORM, ATTN_NORM );

	}else{

		playPissed();
	}


}

/*
void CScientist::playPissed(){

	PlaySentence( "SC_POKE", RANDOM_FLOAT(2.8, 3.2), VOL_NORM, ATTN_IDLE );

}
*/


void CScientist :: Scream( void )
{
	if ( FOkToSpeak() )
	{
		Talk( 10 );
		m_hTalkTarget = m_hEnemy;
		PlaySentence( "SC_SCREAM", RANDOM_FLOAT(3, 6), VOL_NORM, ATTN_NORM );
	}
}


Activity CScientist::GetStoppedActivity( void )
{
	//MODDD - if following, don't do this.
	if(!m_hTargetEnt){
		if ( m_hEnemy != NULL )
			return ACT_EXCITED;

	}
	return CTalkMonster::GetStoppedActivity();
}


void CScientist :: StartTask( Task_t *pTask )
{
	float dist;
	BOOL decidedToFight;
	BOOL decidedToRun;

	//"m_iScheduleIndex" is actually what task we picked. Fuckin' names man.
	//easyForcePrintLine("EACH DAY I CRY SOME MORE StartTask sched:%s: task:%d index:%d", getScheduleName(), pTask->iTask, m_iScheduleIndex);

	/*
	if(m_hTargetEnt == NULL){
			TaskFail();
			return;
		}
		*/
	//Also I meant to ask about the chumtoad spawning, trying to avoid being able to spawn on walls where it slowly falls through the world. If there's a location where it looks clearly open but won't spawn, see what lines turn red.

	//easyPrintLine("I WILL EAT YOUR FLESH!!!!! %s:: %d", m_pSchedule->pName, pTask->iTask);

	switch( pTask->iTask )
	{


	case TASK_PLAY_SEQUENCE_FACE_ENEMY:
	case TASK_PLAY_SEQUENCE_FACE_TARGET:
	case TASK_PLAY_SEQUENCE:
		easyPrintLine("WHAT THE FUUUUUCK %.2f", pTask->flData);
		if(pTask->flData == ACT_EXCITED || pTask->flData == ACT_CROUCH || pTask->flData == ACT_CROUCHIDLE){
			if(gpGlobals->time > playFearAnimCooldown ){
				//allowed.
				easyPrintLine("I ALLOWED YOU!!! pfac:%.2f ct:%.2f", playFearAnimCooldown, gpGlobals->time);
				playFearAnimCooldown = gpGlobals->time + 14;
			}else{
				//don't try to play this anim, it may get spammed and make the scientist even more useless than it already is.
				easyPrintLine("I BLOCKED YOU!!! pfac:%.2f ct:%.2f", playFearAnimCooldown, gpGlobals->time);
				TaskComplete();
				break;
			}
		}

		//otherwise base behavior is good.
		CTalkMonster::StartTask(pTask);
	break;
	case TASK_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED:
		//just a notice to turn aggro off, if we can't even make it to our enemy.
		aggro = 0;
		TaskComplete();
		return;
	break;
	case TASK_CANT_FOLLOW:

		//MODDD - WARNING: sensitive script.  Let's be careful about this...
		if(m_hTargetEnt == NULL){
			TaskFail();
			return;
		}

		if(healNPCChosen){
			//just give up and re-check later...
			forgetHealNPC();
			healNPCCheckDelay = gpGlobals->time + 6;
			TaskFail();
			return;
		}else{
			if(m_hTargetEnt != NULL){
				CTalkMonster::StartTask( pTask );
			}
		}
		
		if(m_hTargetEnt == NULL){
			TaskFail();
			return;
		}


		break;

	case TASK_SAY_HEAL:

		//MODDD
		if(m_hTargetEnt == NULL){
			TaskFail();
			return;
		}

//		if ( FOkToSpeak() )
		Talk( 2 );
		m_hTalkTarget = m_hTargetEnt;
		PlaySentence( "SC_HEAL", 2, VOL_NORM, ATTN_IDLE );

		TaskComplete();
		break;

		//MODDD
	case TASK_SCIENTIST_FIGHT_OR_FLIGHT:
		

		//if 0, this isn't the "panic".  This replaces the scream for panic, so only scream for panic.
		if(pTask->flData == 1){
			Scream();
		}

		if(global_scientistBravery > 0){

			decidedToFight = FALSE;
			decidedToRun = FALSE;

			//in panic-mode, we can't wuss out.
			if(aggro >= 0||aggro==-0.5 || pTask->flData == 1){

				aggro = 0;


				//elevation differences?  Nope, none of that.
				if(m_hEnemy != NULL && (abs(m_hEnemy->pev->origin.z - pev->origin.z ) < 44) ) {
					dist = (m_hEnemy->pev->origin - this->pev->origin).Length();

					if(pTask->flData == 1){
						//much easier to tick off now.
						dist *= 0.5;
					}
					if(aggro==-0.5){
						//tighten it instead..
						dist *= 1.7;
					}
					//...
					Vector tempEnBoundDelta = (m_hEnemy->pev->absmax - m_hEnemy->pev->absmin);
					float tempEnSize = tempEnBoundDelta.x * tempEnBoundDelta.y * tempEnBoundDelta.z;

					//tempEnSize   scale:   under 200... not so important.
					
					float fightOddsInfluence = 1.05;
					float fearFactor = 0;

					//size of agrunt: 73728.

					if(tempEnSize < 16000){  //headcrab size: 13824
						//no change.
						
					}else if(tempEnSize <= 348160){  //size of agrunt: about 348160
						//less likely to fight, the more out of our comfort zone this is.

						fearFactor = ((tempEnSize - 16000) / (348160-16000));
						dist *= (1 + fearFactor);

						fightOddsInfluence = (1 - fearFactor + 0.05f);

					}else{
						//OH GOD ITS HUGE
						dist *= 7;
						fightOddsInfluence = 0.01f;
					}


					
					//easyPrintLine("WHATS THE SIZE   %.2f %.2f %.2f %.2f", tempEnSize, dist, fearFactor, fightOddsInfluence);
					//easyPrintLine("kk");

					if(dist < 85){
						//fight!
						decidedToFight = TRUE;
					}else if(dist < 130){
						if(aggro==-0.5){
							decidedToFight = (RANDOM_FLOAT(0, 1) < 0.38 * fightOddsInfluence);
						}else{
							decidedToFight = (RANDOM_FLOAT(0, 1) < 0.7 * fightOddsInfluence);
						}

						if(!decidedToFight && pTask->flData == 0){
							//"pTask->flData" being 1 means running is not an option... sit and drool I guess.
							decidedToRun = TRUE;
						}
					}

				}

			}else if(aggro == -1){
				decidedToRun = TRUE;
				//FLEE!
			}

			//easyPrintLine("MY CHOICE?  aggro:%.2f pTask->flData:%.2f ::: decidedToFight:%d decidedToRun:%d", aggro, pTask->flData, decidedToFight, decidedToRun);

			if(decidedToFight == TRUE){
				aggro = 1;
				aggroOrigin = pev->origin;
				aggroCooldown = gpGlobals->time + 2;
				ChangeSchedule(this->GetScheduleOfType(SCHED_SCIENTIST_ANGRY_CHASE_ENEMY) );
				break;
			}else if(decidedToRun == TRUE){
				aggro = 0;
				ChangeSchedule(slScientistCover);
				break;
			}
		}


		aggro = 0;

		TaskComplete();

		break;
	case TASK_SCREAM:
		Scream();
		TaskComplete();
		break;

	case TASK_RANDOM_SCREAM:
		if ( RANDOM_FLOAT( 0, 1 ) < pTask->flData )
			Scream();
		TaskComplete();
		break;

	case TASK_SAY_FEAR:
		if ( FOkToSpeak() )
		{
			Talk( 2 );
			m_hTalkTarget = m_hEnemy;
			if ( m_hEnemy->IsPlayer() )
				PlaySentence( "SC_PLFEAR", 5, VOL_NORM, ATTN_NORM );
			else
				PlaySentence( "SC_FEAR", 5, VOL_NORM, ATTN_NORM );
		}
		TaskComplete();
		break;

	case TASK_HEAL:
		m_IdealActivity = ACT_MELEE_ATTACK1;
		break;

	case TASK_RUN_PATH_SCARED:
		m_movementActivity = ACT_RUN_SCARED;
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
		{
			//MODDD
			if(m_hTargetEnt == NULL){
				TaskFail();
				return;
			}

			if ( (m_hTargetEnt->pev->origin - pev->origin).Length() < 1 )
				TaskComplete();
			else
			{
				m_vecMoveGoal = m_hTargetEnt->pev->origin;
				if ( !MoveToTarget( ACT_WALK_SCARED, 0.5 ) )
					TaskFail();
			}
		}
		break;

	default:
		CTalkMonster::StartTask( pTask );
		break;
	}
}

void CScientist :: RunTask( Task_t *pTask )
{

	//MODDD
	/*
	if(m_hTargetEnt == NULL){
		TaskFail();
		return;
	}else{
					
		easyPrintLine("STUUUUFFFFFF %d   %s::%d", monsterID, m_hTargetEnt->getClassname(), m_hTargetEnt->MyMonsterPointer()->monsterID );
	}
	*/

	switch ( pTask->iTask )
	{
	case TASK_RUN_PATH_SCARED:
		if ( MovementIsComplete() )
			TaskComplete();
		if ( RANDOM_LONG(0,31) < 8 )
			Scream();
		break;

	case TASK_MOVE_TO_TARGET_RANGE_SCARED:
		{
			if ( RANDOM_LONG(0,63)< 8 )
				Scream();

			if ( m_hEnemy == NULL )
			{
				TaskFail();
			}
			else
			{
				//MODDD
				if(m_hTargetEnt == NULL){
					TaskFail();
					return;
				}


				float distance;

				distance = ( m_vecMoveGoal - pev->origin ).Length2D();
				// Re-evaluate when you think your finished, or the target has moved too far
				if ( (distance < pTask->flData) || (m_vecMoveGoal - m_hTargetEnt->pev->origin).Length() > pTask->flData * 0.5 )
				{
					m_vecMoveGoal = m_hTargetEnt->pev->origin;
					distance = ( m_vecMoveGoal - pev->origin ).Length2D();
					FRefreshRoute();
				}

				// Set the appropriate activity based on an overlapping range
				// overlap the range to prevent oscillation
				if ( distance < pTask->flData )
				{
					TaskComplete();
					RouteClear();		// Stop moving
				}
				else if ( distance < 190 && m_movementActivity != ACT_WALK_SCARED )
					m_movementActivity = ACT_WALK_SCARED;
				else if ( distance >= 270 && m_movementActivity != ACT_RUN_SCARED )
					m_movementActivity = ACT_RUN_SCARED;
			}
		}
		break;

	case TASK_HEAL:
		if ( m_fSequenceFinished )
		{
			TaskComplete();
		}
		else
		{
			//MODDD - IMPORTANT NOW!
			if(m_hTargetEnt == NULL){
				return;
			}

			if ( TargetDistance() > 90 )
				TaskComplete();
			pev->ideal_yaw = UTIL_VecToYaw( m_hTargetEnt->pev->origin - pev->origin );
			ChangeYaw( pev->yaw_speed );
		}
		break;
	default:
		CTalkMonster::RunTask( pTask );
		break;
	}
}

//=========================================================
// Classify - indicates this monster's place in the
// relationship table.
//=========================================================
int	CScientist :: Classify ( void )
{
	return	CLASS_HUMAN_PASSIVE;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CScientist :: SetYawSpeed ( void )
{
	int ys;

	ys = 90;

	switch ( m_Activity )
	{
	case ACT_IDLE:
		ys = 120;
		break;
	case ACT_WALK:
		ys = 180;
		break;
	case ACT_RUN:
		ys = 150;
		break;
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 120;
		break;
	}

	pev->yaw_speed = ys;
}

//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CScientist :: HandleAnimEvent( MonsterEvent_t *pEvent )
{

	//float const;

	//MODDD - we want the total number of heads in the model, even including ones we skip (only matters for the headfix: have all 3 in mind for the shift)
	//if(this->numberOfModelBodyParts == 2){
		/*
		#if headOffsetFix == 0
			#define NUM_SCIENTIST_HEADS_MODEL NUM_SCIENTIST_HEADS
		#else
			#define NUM_SCIENTIST_HEADS_MODEL NUM_SCIENTIST_HEADS+1
		#endif
		*/
	//}else{
	//
	//}

	//MODDD - COUNTREMOVAL.  Count is not consistent across machines.  Scrapped.
	//const int NUM_SCIENTIST_HEADS_MODEL = this->numberOfModelBodyParts+1;
	const int NUM_SCIENTIST_HEADS_MODEL = 3;


	int oldBody;

	switch( pEvent->event )
	{
	case SCIENTIST_AE_HEAL:		// Heal my target (if within range)
		{
		Heal();

		//re-pick a new TargetEnt.
		forgetHealNPC();

		break;
		}
	case SCIENTIST_AE_NEEDLEON:
		{
		oldBody = pev->body;
		//easyPrintLine("OLD BODY1 %d %d", oldBody, pev->body);
		if(NUM_SCIENTIST_HEADS_MODEL > 1){
			pev->body = (oldBody % (NUM_SCIENTIST_HEADS_MODEL) ) + (NUM_SCIENTIST_HEADS_MODEL) * 1;
		}else{
			//no others, just 0.
			pev->body = 0;
		}

		//easyPrintLine("NEW BODY1 %d", pev->body);
		break;
		}
	case SCIENTIST_AE_NEEDLEOFF:
		{
		oldBody = pev->body;
		//easyPrintLine("OLD BODY2 %d %d", oldBody, pev->body);
		if(NUM_SCIENTIST_HEADS_MODEL > 1){
			pev->body = (oldBody % (NUM_SCIENTIST_HEADS_MODEL) ) + (NUM_SCIENTIST_HEADS_MODEL) * 0;
		}else{
			pev->body = 0;
		}

		//easyPrintLine("NEW BODY2 %d", pev->body);
		break;
		}
	default:
		CTalkMonster::HandleAnimEvent( pEvent );
	}
}

//MODDD - new top-level method for adjusting scientist heads to factor out the possible extra head value of 2 or 3 (depending on what to expect of the model:
//has submodels (heads) 0, 1, 2, 3,  then skip #2.   If it only has 0, 1, 2,   do them linearly, nothing special (but, upon trying head #3, set to something else)
void scientistHeadFilter( CBaseMonster& somePerson, int arg_numberOfModelBodyParts, int* trueBody){

	if ( *trueBody == 0 ){
		*trueBody = RANDOM_LONG(1, 3);
	}

	if(*trueBody > 3){
		//so what do we do?  randomize it, or just force it something?
		//Forcing 0 for now.
		somePerson.pev->body = 0;
		easyForcePrintLine("SCIENTIST: BAD HEAD VALUE? body:%d truebody:%d", somePerson.pev->body, *trueBody);
	}else{
		
		somePerson.pev->body = *trueBody-1;
	}

	//WHY??!!!
	//somePerson.pev->skin = 0;


}




BOOL CScientist::getGermanModelRequirement(void){
	return globalPSEUDO_germanModel_scientistFound;
}
const char* CScientist::getGermanModel(void){
	return "models/g_scientist.mdl";
}
const char* CScientist::getNormalModel(void){
	return "models/scientist.mdl";
}
//MODDD - note that "CSittingScientist" inherits from CScientist, so this also carries over to there if left unspecified for it.
void CScientist::setModel(void){
	CScientist::setModel(NULL);
}
void CScientist::setModel(const char* m){

	//easyPrintLine("NO!!!!!!!!! PLEASE!!!!!!!!!!!!!!!!!!!!!!!!!! %d", (int)global_scientistModel);
	//easyPrintLine("Huh?? %.2f", CVAR_GET_FLOAT("scientistModel"));

	//let this handle model management.
	CTalkMonster::setModel(m);

	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);
	if(numberOfModelBodyParts==-1)numberOfModelBodyParts = getNumberOfBodyParts();
	//easyPrintLine("BOOT: %d", numberOfModelBodyParts);


	//It is actually a bad idea to depend on saved things in here like "trueBody". It might not have loaded yet and so isn't reliable, it would be better to hook this at the end of Restore.
	//scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);

}



//MODDD - new.
void CScientist :: Activate( void ){

	/*
	easyPrintLine("I AMA %d %d", pev->body, trueBody);
	//!setModelCustom();
	*/
	////TalkInit();

	CTalkMonster::Activate();
}

CScientist::CScientist(void){

	//givenModelBody = -1;

	explodeDelay = -1;

	aggroOrigin = Vector(0,0,0); //???

	aggroCooldown = -1;
	aggro = 0;

	playFearAnimCooldown = -1;

	trueBody = -1;

	healNPCChosen = FALSE;
	healNPCCheckDelay = -1;

	//On a restore, if never seen before, this is loaded as "0" instead.  Beware of that.

	
	madInterSentencesLocation = madInterSentences;
	//madInterSentencesMaxLocation = &madInterSentencesMax;

	//are parent constructors called automatically in CScientist?
	//CTalkMonster::CTalkMonster();
}

//=========================================================
// Spawn
//=========================================================
void CScientist :: Spawn( void )
{

	
	

	Precache( );


	//easyPrintLine("I AMS %d %d", pev->body, trueBody);
	if(spawnedDynamically && firstSpawnCall){
		pev->body = -1;
		//signal a randomization if spawned by the player.
	}else{
		//leave "pev->body" to whatever hte map made it...?

	}
	//offset by one.  "Random" is now 0 instead.
	trueBody = pev->body + 1;


	CTalkMonster::Spawn();


	//MODDD - all model calls replaced with a call to "setModelCustom" to let "global_scientistModel" act as a selector.
	setModel(); //"models/scientist.mdl"  //argument unused for monsters with German versions. 
	//!setModelCustom();
	
	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);

	//HERE instead.
	////TalkInit();



	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->classname = MAKE_STRING("monster_scientist");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor = BloodColorRedFilter();
	pev->health			= gSkillData.scientistHealth;
	pev->view_ofs		= Vector ( 0, 0, 50 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so scientists will notice player and say hello
	m_MonsterState		= MONSTERSTATE_NONE;

//	m_flDistTooFar		= 256.0;

	m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE;

	// White hands
	//pev->skin = 0;



	//scientistHeadFilter(*this, numberOfModelBodyParts);

	//pev->body = 0;
	//pev->modelindex = 137;
	//easyPrintLine("WHAT IS SCIENTIST MODEL %d", pev->modelindex );
	//scientist: 95
	//barney: 137

	MonsterInit();
	
	//FL_CLIENT
		//pev->flags |= FL_CLIENT;

	


	
	pev->skin = 0; //default.

	//if( (pev->spawnflags & SF_MONSTER_TALKMONSTER_BLOODY) && global_germanCensorship != 1 && global_scientistModel < 2){
	if( (pev->spawnflags & SF_MONSTER_TALKMONSTER_BLOODY) && global_germanCensorship != 1){
		pev->skin = 1;
		
		if(global_monsterSpawnPrintout == 1){
			easyPrintLine("SCIHEAD: BLOODY CORPSE FLAG UNDERSTOOD!!!");
		}

		//if this spawn flag is set, start with the bloody skin.
	}

	if(global_monsterSpawnPrintout == 1){
		easyPrintLine("SCIHEAD: FINAL BODY: %d SKIN: %d", pev->body, pev->skin);
		easyPrintLine("SCIHEAD: COUNTPOST: %d", getNumberOfBodyParts( ) );
	}



	SetUse( &CTalkMonster::FollowerUse );


}

extern int global_useSentenceSave;
//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CScientist :: Precache( void )
{

	PRECACHE_MODEL("models/scientist.mdl");
	//PRECACHE_MODEL("models/scientist_pre_e3.mdl");
	//PRECACHE_MODEL("models/scientist_e3.mdl");


	global_useSentenceSave = TRUE;
	PRECACHE_SOUND("scientist/sci_pain1.wav");
	PRECACHE_SOUND("scientist/sci_pain2.wav");
	PRECACHE_SOUND("scientist/sci_pain3.wav");
	PRECACHE_SOUND("scientist/sci_pain4.wav");
	PRECACHE_SOUND("scientist/sci_pain5.wav");



	PRECACHE_SOUND_ARRAY(pDeathSounds);


	
	//MODDD - can now play "Strike" sounds on hitting something with the kick.  NEW DATA STUFF
	PRECACHE_SOUND("zombie/claw_strike1.wav");
	PRECACHE_SOUND("zombie/claw_strike2.wav");
	PRECACHE_SOUND("zombie/claw_strike3.wav");
	PRECACHE_SOUND("zombie/claw_miss1.wav");
	PRECACHE_SOUND("zombie/claw_miss2.wav");


	global_useSentenceSave = FALSE;
	// every new scientist must call this, otherwise
	// when a level is loaded, nobody will talk (time is reset to 0)

	TalkInit();
	//Just do TalkInit() in spawn.
	//Wait... why were we doing it that way instead of in Preacache like how the Barney does it?
	//Just do it in both places then.



	//MODDD - note that "CSittingScientist" calls this same method.

	CTalkMonster::Precache();
}

// Init talk data
void CScientist :: TalkInit()
{

	//easyPrintLine("TALKINIT S %d %d", pev->body, trueBody);


	//pev->renderfx |= 128;
	//why 128?  wasn't it 64 for "ISNPC"?   May be redundant with "monsterinit" already doing this, though.
	pev->renderfx |= ISNPC;

	CTalkMonster::TalkInit();

	//MODDD - ...evidently, this method is called BEFORE "Activate".  So, this check must be done here too.

	//setModelCustom();  //!!!???
	//scientistHeadFilter(*this, numberOfModelBodyParts);


	//MODDD
	madSentencesMax = 1;
	madSentences[0] = "SC_POKE";

	//Static now!
	//madInterSentencesMax = 1;
	//madInterSentences[0] = "SC_POKEQ";


	// scientist will try to talk to friends in this order:

	m_szFriends[0] = "monster_scientist";
	m_szFriends[1] = "monster_sitting_scientist";
	m_szFriends[2] = "monster_barney";


	// scientists speach group names (group names are in sentences.txt)

	m_szGrp[TLK_ANSWER]  =	"SC_ANSWER";
	m_szGrp[TLK_QUESTION] =	"SC_QUESTION";
	m_szGrp[TLK_IDLE] =		"SC_IDLE";
	m_szGrp[TLK_STARE] =	"SC_STARE";
	m_szGrp[TLK_USE] =		"SC_OK";
	m_szGrp[TLK_UNUSE] =	"SC_WAIT";
	m_szGrp[TLK_STOP] =		"SC_STOP";
	m_szGrp[TLK_NOSHOOT] =	"SC_SCARED";
	m_szGrp[TLK_HELLO] =	"SC_HELLO";

	m_szGrp[TLK_PLHURT1] =	"!SC_CUREA";
	m_szGrp[TLK_PLHURT2] =	"!SC_CUREB";
	m_szGrp[TLK_PLHURT3] =	"!SC_CUREC";

	m_szGrp[TLK_PHELLO] =	"SC_PHELLO";
	m_szGrp[TLK_PIDLE] =	"SC_PIDLE";
	m_szGrp[TLK_PQUESTION] = "SC_PQUEST";
	m_szGrp[TLK_SMELL] =	"SC_SMELL";

	m_szGrp[TLK_WOUND] =	"SC_WOUND";
	m_szGrp[TLK_MORTAL] =	"SC_MORTAL";

	// get voice for head
	//switch (pev->body % 3)
	//should this use "% 2" instead, as "% 3", leftover from when there were 4 heads, may not be necessary?


	if(numberOfModelBodyParts == 2){

#if headOffsetFix == 0
	switch (pev->body % 3)
	{
	default:
	case HEAD_GLASSES:	m_voicePitch = 100; break;	//glasses
		//used to be 105, we want universal pitch now.
	case HEAD_EINSTEIN: m_voicePitch = 100; break;	//einstein
	//MODDD - removed.
	//case HEAD_LUTHER:	m_voicePitch = 95;  break;	//luther
	case HEAD_SLICK:	m_voicePitch = 100;  break; //egon.  (or Luther in progress).
	}
#else
	switch (pev->body % (NUM_SCIENTIST_HEADS+1)) //as "3" was "4 - 1", or "NUM_SCIENTIST_HEADS - 1".
		//...ACTUALLY, NO.   I am convinced that it should NOT be one less than NUM_..., because the range
		//of modulus is already  0 to 2nd # - 1.  So, range of  # % 4 is  0 - 3, and # % 3 --> 0 - 2 (desirable).
		//Making it "plus 1" to accept "3".
	{
	default:
	case HEAD_GLASSES:	m_voicePitch = 100; break;	//glasses, head#0
		//used to be 105, we want universal pitch now.
	case HEAD_EINSTEIN: m_voicePitch = 100; break;	//einstein, head#1

	case HEAD_SLICK:	m_voicePitch = 100;  break; //slick, head#3   (2 skipped, but the range of  # % 2 is 0, 1, 2.  Only model references need the authentic number.
	}


#endif

	}else if(numberOfModelBodyParts == 3){

		switch (pev->body % 4)
		{
		default:
			case 0:	m_voicePitch = 105; break;	//glasses
			case 1: m_voicePitch = 100; break;	//einstein
			case 2:	m_voicePitch = 95;  break;	//luther
			case 3:	m_voicePitch = 100;  break; //slick
		}


	}else{
		//otherwise, just work like alpha, pick from one of the voices based on what it would've been (#3 = 0).
		//also note that "trueBody" is always offset by 1.
		switch ((trueBody-1) % 3)
		{
		default:
			case 0:	m_voicePitch = 105; break;
			case 1: m_voicePitch = 100; break;
			case 2:	m_voicePitch = 100;  break;
		}
	}




}



GENERATE_TRACEATTACK_IMPLEMENTATION(CScientist)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CTalkMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CScientist)
{
	BOOL alreadyRunningAway = FALSE;
	int tkdDmgRes;


	if(this->m_MonsterState == MONSTERSTATE_SCRIPT && (m_pCine && !m_pCine->CanInterrupt()) ){
		//Exception. Don't try to tap into our schedules.
		return GENERATE_TAKEDAMAGE_PARENT_CALL(CTalkMonster);
	}


	if(pevInflictor != NULL   ){
		CBaseEntity* attackerEnt = CBaseEntity::Instance(pevInflictor);

		int rel = IRelationship(attackerEnt);
		if(rel > R_NO || rel==R_FR){
			m_fearTime = gpGlobals->time;  //something meant to hit me. 
		}




		//IS THIS LINE OFFENDING?  search for the other example.
		/*
		if(aggro != -1 && aggro < 1 && (m_hEnemy == NULL || m_hEnemy != attackerEnt) ){
			//took damage from something other what I expected to run from and I'm not feeling very brave? Hurry up and run away!
			aggro = -1;
			TaskFail();
			ChangeSchedule(slScientistCover);//  ???
			//WARNING - if StopFollowing() below is called, it will ignore ths ScientistCover call. Butu that may be ok, below
			//is called only if the player (FL_CLIENT on the inflictor) attacked this scientist.

			alreadyRunningAway = TRUE;
		}
		*/

	}


	


	if(!alreadyRunningAway){
		if(aggro > 0 && (flDamage >= 6 || RANDOM_LONG(0, 2) < 2) ){
			//2/3 chance of getting scared fast.  If damage is significant, it is definite.
			
			/*
			//DEBUG - not this time.
			aggro = -1;
			TaskFail();
			ChangeSchedule(slScientistCover);//  ???
			*/

		}
	}

	//MODDD - the base talkmonster now handles this with a little tolerance.
	/*
	if ( pevInflictor && pevInflictor->flags & FL_CLIENT )
	{
		Remember( bits_MEMORY_PROVOKED );
		StopFollowing( TRUE );
	}
	*/



	// make sure friends talk about it if player hurts scientist...
	tkdDmgRes = GENERATE_TAKEDAMAGE_PARENT_CALL(CTalkMonster);

	//MODDD HACKY - don't turn around to see what hit you ever, you're too busy running.
	pev->ideal_yaw = this->pev->angles.y;

	return tkdDmgRes;
}


//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
//=========================================================
int CScientist :: ISoundMask ( void )
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_DANGER	|
			bits_SOUND_PLAYER;
}

//=========================================================
// PainSound
//=========================================================
void CScientist :: PainSound ( void )
{
	if (gpGlobals->time < m_painTime )
		return;

	m_painTime = gpGlobals->time + RANDOM_FLOAT(0.5, 0.75);

	switch (RANDOM_LONG(0,4))
	{
	case 0: EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain1.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 1: EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain2.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 2: EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain3.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 3: EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain4.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	case 4: EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "scientist/sci_pain5.wav", 1, ATTN_NORM, 0, GetVoicePitch()); break;
	}
}


void CScientist::tempMethod(void){
	
	//force the pain sound to be played?
	//m_painTime = -1;
	DeathSound();

}//END OF tempMethod

//=========================================================
// DeathSound
//=========================================================
void CScientist :: DeathSound ( void )
{

	//sci_die1


	//PainSound();
	
	if(explodeDelay == -1){
		EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1.0, ATTN_NORM, 0, GetVoicePitch() );
	}


}


GENERATE_KILLED_IMPLEMENTATION(CScientist)
{
	//test();
	//easyPrintLine("YEAHHHH %d", numberOfModelBodyParts);

	if(explodeDelay != -2){
		//not doing that? ok, normal behavior.
		SetUse( NULL );
		GENERATE_KILLED_PARENT_CALL(CTalkMonster);
	}else{
		//The glitch. Skip to calling the base monster, interrupting other talkers reciting lines isn't ok.
		StopTalking();
		SetUse( NULL );
		GENERATE_KILLED_PARENT_CALL(CBaseMonster);
	}
}


void CScientist :: SetActivity ( Activity newActivity )
{
	int	iSequence;

	float framerateChoice = 1;
	
	//Note that "ACT_MELEE_ATTACK1" is when it is applying the heal.  It may be very short, about negligible, but oh well.
	if(newActivity == ACT_ARM || newActivity == ACT_DISARM || newActivity == ACT_MELEE_ATTACK1){
		if(healNPCChosen == FALSE){
			//the heal anim for the player gets a slight boost.
			framerateChoice = 1.4f;
		}else{
			//For other NPC's, we need to be faster.  Barney's and scientists can't take much damage and need timed damage cured ASAP.
			framerateChoice = 1.8f;
		}
	}else{

	}

	//....why?! The base monster's SetActivity will work with this fine.
	//iSequence = LookupActivity ( newActivity );
	

	
	//if(monsterID==2)easyForcePrintLine("IT IS I WHO NUTS TO THAT ID:%d befr act:%d seq:%d fr:%.2f", monsterID, newActivity, iSequence, pev->frame);
	
	CTalkMonster::SetActivity( newActivity );


	//if(monsterID==2)easyForcePrintLine("IT IS I WHO NUTS TO THAT ID:%d aftr act:%d seq:%d fr:%.2f", monsterID, newActivity, pev->sequence, pev->frame);


	// Set to the desired anim, or default anim if the desired is not present
	//if ( iSequence == ACTIVITY_NOT_AVAILABLE ){
	//MODDD NOTE - actually, if the sequence is left 0, that means the activity get failed.
	//...it's handled fine the way it is, forget this.
	/*
	if(pev->sequence == 0){
		newActivity = ACT_IDLE;
		framerateChoice = 1;
	}
	*/


	if(pev->framerate != framerateChoice){
		pev->framerate = framerateChoice;
	}

}




void CScientist::ReportAIState(void){

	//call the parent, and add on to that.
	CTalkMonster::ReportAIState();

	easyPrintLine("SCIENTIST: Body: %d Truebody: %d Skin: %d aggro: %d", pev->body, trueBody, pev->skin, this->aggro);


}//END OF ReportAIState



Schedule_t* CScientist :: GetScheduleOfType ( int Type )
{
	Schedule_t *psched;


	//easyForcePrintLine("WHATS GOOD IM SCIENTIST:%d AND I PICKED SCHED TYPE %d", monsterID, Type);



	//MODDD - is it safe to have this here no matter what?
	//   Note that we have to check to see if the enemy is dead to drop them as an enemy or else the scientist will still be scared of their corpse. Yep, gotta love computers.
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
			
			//MODDD - shouldn't you clear the conditions here too??
			ClearConditions( bits_COND_ENEMY_DEAD );
			
			//if not already in the ALERT state or tyring to get there, go ahead and be alert & reget your schedule.
			if(this->m_MonsterState != MONSTERSTATE_ALERT || this->m_IdealMonsterState != MONSTERSTATE_ALERT){
				SetState( MONSTERSTATE_ALERT );
				return GetSchedule();
			}
		}
	}






	switch( Type )
	{

	case SCHED_SCIENTIST_ANGRY_CHASE_ENEMY:	
	//also, in case we pick this schedule from the TASK_FIND_COVER_FROM_ENEMY_OR_FIGHT in schedule.cpp, make sure to boost your aggro so the rest of the AI works right.
	case SCHED_CHASE_ENEMY:
		//MODDD - why was this for MELEE_ATTACK_2 as well? no, this leads into that when we get close enough..

		if(aggro < 1)aggro = 1; //clearly we are mad at the enemy to have called this method.
		return slAngryScientistChaseEnemy;
	break;
	//case SCHED_RANGE_ATTACK1:
	//case SCHED_RANGE_ATTACK2:
	//case SCHED_MELEE_ATTACK1:
	case SCHED_MELEE_ATTACK2:
		return slScientistPunch;
	break;
	case SCHED_SCIENTIST_ANGRY_CHASE_ENEMY_FAILED:
		return slAngryScientistChaseEnemyFailed;
	break;
		//same as below..
	case SCHED_TAKE_COVER_FROM_ORIGIN:
		aggro = 0;
		return CTalkMonster::GetScheduleOfType(SCHED_TAKE_COVER_FROM_ORIGIN);
	break;
		//MODDD - intercept this to add something...
	case SCHED_TAKE_COVER_FROM_BEST_SOUND:
		aggro = 0;
		return CTalkMonster::GetScheduleOfType(SCHED_TAKE_COVER_FROM_BEST_SOUND);
	break;


	// Hook these to make a looping schedule
	case SCHED_TARGET_FACE:
		// call base class default so that scientist will talk
		// when 'used'

		psched = CTalkMonster::GetScheduleOfType(Type);
		if (psched == slIdleStand){
			return slFaceTarget;	// override this for different target face behavior
		}else{
			//otherwise, the TalkMonster told us to talk.
			return psched;
		}

	case SCHED_TARGET_CHASE:
		return slFollow;

	case SCHED_CANT_FOLLOW:
		//return slStopFollowing;
		//MODDD - we're going to do this instead.  CTalkMonster has the common method.
		return CTalkMonster::GetScheduleOfType(Type);
	case SCHED_PANIC:
		return slSciPanic;

	case SCHED_TARGET_CHASE_SCARED:
		return slFollowScared;

	case SCHED_TARGET_FACE_SCARED:
		return slFaceTargetScared;

	case SCHED_IDLE_STAND:

		// call base class default so that scientist will talk
		// when standing during idle
		psched = CTalkMonster::GetScheduleOfType(Type);

		if (psched == slIdleStand)
			return slIdleSciStand; //substitution!
		else
			return psched;

	case SCHED_HIDE:
		{
		aggro = 0;
		return slScientistHide;
		break;
		}

	case SCHED_STARTLE:
		{
		aggro = 0;
		return slScientistStartle;
		break;
		}

	case SCHED_FEAR:
		{
		aggro = 0;
		return slFear;
		break;
		}
	}

	return CTalkMonster::GetScheduleOfType( Type );
}



void CScientist::ScheduleChange(void){
	forgetHealNPC();

	CTalkMonster::ScheduleChange();
}


Schedule_t *CScientist :: GetSchedule ( void )
{
	//MODDD - new block. If the one I was following recently died, get scared.
	if(leaderRecentlyDied){
		leaderRecentlyDied = FALSE;
		SayLeaderDied();
        StopFollowing( FALSE, FALSE );  //no generic unuse sentence.

		//enter a panic state.
		return GetScheduleOfType(SCHED_PANIC);
	}


	//return CBaseMonster::GetSchedule();

	//MODDD - is this okay?   This says that, on schedule failure, forget healing.
	forgetHealNPC();

	while(true){

		if(aggro > 0){

			//if ( HasConditions( bits_COND_HEAR_SOUND ) )

			if(HasConditions(bits_COND_HEAR_SOUND) || HasConditions(bits_COND_SEE_ENEMY) || HasConditions(bits_COND_NEW_ENEMY) ){

				if(m_hEnemy == NULL || (m_hEnemy->pev->origin - this->pev->origin).Length() > 310 ){
					//too far away? no enemy?  screw it.   "break" skips the rest.
					aggro = 0;
					break;
				}else{
					//reset aggro timer.
					aggroCooldown = gpGlobals->time + 2;
				}
			}

			if(gpGlobals->time >= aggroCooldown){
				//not so aggressive now.  Continue with a usual schedule below all this.
				aggro = 0;
			}else{
				//I CAME HERE TO DO RESEARCH AND COLLECT BLOOD SAMPLES... AND IM ALL OUTTA RESEARCH.
				return CBaseMonster::GetSchedule();
			}
		}

		break;
	}//END OF while(true)...  just a procedural loop to be interrupted as needed.


	CBaseEntity *pEntityScan = NULL;

	// so we don't keep calling through the EHANDLE stuff
	CBaseEntity *pEnemy = m_hEnemy;

	if ( HasConditions( bits_COND_HEAR_SOUND ) )
	{
		CSound *pSound;
		pSound = PBestSound();

		ASSERT( pSound != NULL );
		if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
			return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
	}



	float thisDistance;
	float leastDistanceYet;
	CBaseMonster* testMon;
	CTalkMonster* thisNameSucks;
	CTalkMonster* bestChoiceYet;


	switch( m_MonsterState )
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:

		//Scream if the party is going hard.
		if(global_thatWasntPunch){
			if ( RANDOM_FLOAT( 0, 1 ) < 0.4 ){
			//PlaySentence( "SC_SCREAM_TRU", 2, VOL_NORM, ATTN_NORM );
			SENTENCEG_PlayRndSz( edict(), "SC_SCREAM_TRU", VOL_NORM, ATTN_IDLE, 0, GetVoicePitch() );
			}
		}
		
		if ( pEnemy )
		{
			if ( HasConditions( bits_COND_SEE_ENEMY ) )
				m_fearTime = gpGlobals->time;
			else if ( DisregardEnemy( pEnemy ) )		// After 15 seconds of being hidden, return to alert
			{
				m_hEnemy = NULL;
				pEnemy = NULL;
			}
		}

		if ( HasConditions(bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE))
		{
			// flinch if hurt
			return GetScheduleOfType( SCHED_SMALL_FLINCH );
		}

		// Cower when you hear something scary
		if ( HasConditions( bits_COND_HEAR_SOUND ) )
		{
			CSound *pSound;
			pSound = PBestSound();

			ASSERT( pSound != NULL );
			if ( pSound )
			{
				if ( pSound->m_iType & (bits_SOUND_DANGER | bits_SOUND_COMBAT) )
				{
					if ( gpGlobals->time - m_fearTime > 3 )	// Only cower every 3 seconds or so
					{
						m_fearTime = gpGlobals->time;		// Update last fear
						return GetScheduleOfType( SCHED_STARTLE );	// This will just duck for a second
					}
				}
			}
		}


		//TASK_FACE_TARGET
		if(healNPCCheckDelay < gpGlobals->time && m_hTargetEnt == NULL && global_scientistHealNPC != 0 && m_healTime <= gpGlobals->time){
			//before even doing any checks, require the heal timer to not be in place (delay between healing again, a minute as of writing)
			leastDistanceYet = 999999;  //large, so that the first distance at all is the "best".

			//check for allied NPCs to heal if not following.
			pEntityScan = NULL;

			//does UTIL_MonstersInSphere work?
			while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, 800 )) != NULL)
			{
				testMon = pEntityScan->MyMonsterPointer();
				//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){
				if(testMon != NULL && testMon->pev != this->pev && UTIL_IsAliveEntity(testMon) && testMon->isTalkMonster() ){
					thisDistance = (testMon->pev->origin - pev->origin).Length();
					
					thisNameSucks = static_cast<CTalkMonster*>(testMon);
					
					//only allow one scientist to try to reach this NPC.  That is, this NPC's own "scientistTryingToHealMe" is null, that is.
					if(thisNameSucks != NULL && thisNameSucks->scientistTryingToHealMeEHANDLE == NULL && thisDistance < leastDistanceYet && CanHeal(testMon)){
						//healTargetNPC = testMon;
						m_hTargetEnt = testMon;
						bestChoiceYet = thisNameSucks;
						healNPCChosen = TRUE;
						leastDistanceYet = thisDistance;
						//break;
					}

				}

			}

			if(healNPCChosen){
				//NOT THE thisNameSucks!
				//thisNameSucks->scientistTryingToHealMe = this;
				bestChoiceYet->scientistTryingToHealMe = this;
				bestChoiceYet->scientistTryingToHealMeEHANDLE = this;
			}


		}//END OF healTargetNPC check


		


		//!!



		// Behavior for following the player... OR tracking down an NPC to heal them.
		if ( m_hTargetEnt != NULL && (IsFollowing() || healNPCChosen == TRUE) )
		{
			if(global_scientistHealNPCDebug == 1){
				/*
				if(m_hTargetEnt == NULL){
					easyPrintLine("SCI: TARGET ENT: NULL");
				}else{
					easyPrintLine("SCI: TARGET ENT: %s", STRING(m_hTargetEnt->pev->classname) );
				}
				*/
			}


			if ( !m_hTargetEnt->IsAlive() )
			{
				// UNDONE: Comment about the recently dead player here?
				//MODDD - your wish is my command, anonymous dev!

				if(healNPCChosen == FALSE){
				    //this means we were following the player.
					SayLeaderDied();
                    StopFollowing( FALSE, FALSE );  //no generic unuse sentence.

					//enter a panic state.
					return GetScheduleOfType(SCHED_PANIC);
                }else{

                    forgetHealNPC();
                }

				break;
			}

			int relationship = R_NO;

			// Nothing scary, just me and the player
			if ( pEnemy != NULL )
				relationship = IRelationship( pEnemy );

			// UNDONE: Model fear properly, fix R_FR and add multiple levels of fear
			if ( relationship != R_DL && relationship != R_HT )
			{
				// If I'm already close enough to my target
				//MODDD - changed to a little higher than 128 to avoid a possible staring glitch, maybe.
				if ( TargetDistance() <= 140 )
				{
					if ( CanHeal() ){	//Heal opportunistically
						return slHeal;
					}else if(healNPCChosen){
						//can't heal, try to look for another or give up.
						forgetHealNPC();
					}

					if(healNPCChosen == FALSE){
					    //player only.
                        if ( HasConditions( bits_COND_CLIENT_PUSH ) )	// Player wants me to move
                            return GetScheduleOfType( SCHED_MOVE_AWAY_FOLLOW );
					}
				}
				return GetScheduleOfType( SCHED_TARGET_FACE );	// Just face and follow.
			}
			else	// UNDONE: When afraid, scientist won't move out of your way.  Keep This?  If not, write move away scared
			{
				//bits_COND_LIGHT_DAMAGE
				//if ( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
				//	return slScientistCover;		// Take Cover

				easyPrintLine("OH MAI GAWD %d", HasConditions( bits_COND_NEW_ENEMY ));

				if ( HasConditions( bits_COND_NEW_ENEMY ) ) // I just saw something new and scary, react
					return GetScheduleOfType( SCHED_FEAR );					// React to something scary
				return GetScheduleOfType( SCHED_TARGET_FACE_SCARED );	// face and follow, but I'm scared!


			}
		}//END OF IsFollowing OR tracking down NPC to heal




		if ( HasConditions( bits_COND_CLIENT_PUSH ) )	// Player wants me to move
			return GetScheduleOfType( SCHED_MOVE_AWAY );



		// try to say something about smells
		TrySmellTalk();
		break;
	case MONSTERSTATE_COMBAT:
		if ( HasConditions( bits_COND_NEW_ENEMY ) )
			return slFear;					// Point and scream!
		if ( HasConditions( bits_COND_SEE_ENEMY ) )
			return slScientistCover;		// Take Cover

		if ( HasConditions( bits_COND_HEAR_SOUND ) )
			return slTakeCoverFromBestSound;	// Cower and panic from the scary sound!

		return slScientistCover;			// Run & Cower
		break;
	}

	return CTalkMonster::GetSchedule();
}

MONSTERSTATE CScientist :: GetIdealState ( void )
{


	if(aggro > 0){
		//AGGRESSIVE!
		return MONSTERSTATE_COMBAT;
	}

	switch ( m_MonsterState )
	{
	case MONSTERSTATE_ALERT:
	case MONSTERSTATE_IDLE:
		if ( HasConditions( bits_COND_NEW_ENEMY ) )
		{
			if ( IsFollowing() )
			{
				int relationship = IRelationship( m_hEnemy );
				if ( relationship != R_FR || relationship != R_HT && !HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
				{
					// Don't go to combat if you're following the player
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}
				StopFollowing( TRUE );
			}
		}
		else if ( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
		{
			// Stop following if you take damage
			if ( IsFollowing() )
				StopFollowing( TRUE );
		}
		break;

	case MONSTERSTATE_COMBAT:
		{
			CBaseEntity *pEnemy = m_hEnemy;
			if ( pEnemy != NULL )
			{
				if ( DisregardEnemy( pEnemy ) )		// After 15 seconds of being hidden, return to alert
				{
					// Strip enemy when going to alert
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					m_hEnemy = NULL;
					return m_IdealMonsterState;
				}
				// Follow if only scared a little
				if ( m_hTargetEnt != NULL )
				{
					m_IdealMonsterState = MONSTERSTATE_ALERT;
					return m_IdealMonsterState;
				}

				if ( HasConditions ( bits_COND_SEE_ENEMY ) )
				{
					m_fearTime = gpGlobals->time;
					m_IdealMonsterState = MONSTERSTATE_COMBAT;
					return m_IdealMonsterState;
				}

			}
		}
		break;
	}

	return CTalkMonster::GetIdealState();
}



BOOL CScientist::CanHeal( CBaseMonster* arg_monsterTry ){

	if(arg_monsterTry == NULL){
		return FALSE;
	}

	//NPCs can have up to 70% of their health to be eligible for healing, since they're weaker.
	if ( (m_healTime > gpGlobals->time) || (arg_monsterTry == NULL) || (arg_monsterTry->pev->health > (arg_monsterTry->pev->max_health * global_scientistHealNPCFract) ) && ( !(arg_monsterTry->m_bitsDamageType & DMG_TIMEBASED || arg_monsterTry->m_bitsDamageTypeMod & DMG_TIMEBASEDMOD))  )
		return FALSE;

	return TRUE;
}

BOOL CScientist::CanHeal( void )
{
	if(m_hTargetEnt == NULL){
		return FALSE;
	}

	//HA!  THIS WOULD ACTUALLY BE really INTUITIVE.  CAN'T HAVE nice things.  FIND ME IN THE ALPS.
	//CBaseEntity* entityAttempt = CBaseEntity::Instance(m_hTargetEnt);
	CBaseEntity* entityAttempt = CBaseEntity::Instance(m_hTargetEnt->pev);

	//CBaseEntity* entityAttempt = GetClassPtr((CBaseEntity *)pev);
		


	CBaseMonster* monsterAttempt = NULL;
	if(entityAttempt != NULL){
		monsterAttempt = entityAttempt->MyMonsterPointer();
	}


	if(monsterAttempt == NULL){
		//can only heal a "Monster".
		return FALSE;
	}

	//OLD HEAL SCRIPT.
	//if ( (m_healTime > gpGlobals->time) || (m_hTargetEnt == NULL) || (m_hTargetEnt->pev->health > (m_hTargetEnt->pev->max_health * 0.5)) )
	//	return FALSE;


	
	float percentage = 0.5;
	if(healNPCChosen){
		percentage = global_scientistHealNPCFract;
	}


	if ( (m_healTime > gpGlobals->time) || (m_hTargetEnt == NULL) || (m_hTargetEnt->pev->health > (m_hTargetEnt->pev->max_health * percentage) ) && ( !(monsterAttempt->m_bitsDamageType & DMG_TIMEBASED || monsterAttempt->m_bitsDamageTypeMod & DMG_TIMEBASEDMOD))  )
		return FALSE;


	return TRUE;
}

void CScientist::Heal( void )
{
	if ( !CanHeal() )
		return;


	
	if(m_hTargetEnt == NULL){
		return;
	}

	Vector target = m_hTargetEnt->pev->origin - pev->origin;
	if ( target.Length() > 100 )
		return;



	CBaseEntity* entityAttempt = CBaseEntity::Instance(m_hTargetEnt->pev);
	CBaseMonster* monsterAttempt = NULL;
	if(entityAttempt != NULL){
		monsterAttempt = entityAttempt->MyMonsterPointer();
	}
	if(monsterAttempt == NULL){
		//can only heal a "Monster".
		return;
	}
	BOOL timedDamageFlag = FALSE;
	if(monsterAttempt->m_bitsDamageType & DMG_TIMEBASED || monsterAttempt->m_bitsDamageTypeMod & DMG_TIMEBASEDMOD){
		timedDamageFlag = TRUE;

		
		if(monsterAttempt->IsPlayer()){
			//send an HEV update.
			CBasePlayer* penis = static_cast<CBasePlayer*>(monsterAttempt);
			penis->SetSuitUpdate("!HEV_HEAL_GNC", FALSE, SUIT_REPEAT_OK);
		}
	}
	monsterAttempt->attemptResetTimedDamage(TRUE);
	
	float percentage = 0.5;
	if(healNPCChosen){
		percentage = global_scientistHealNPCFract;
	}
	//MODDD - added this check.  Now, healing does not add health if over 50% health (before, could abuse the timed-damage-heal-trigger by taking timed damage over and over again to get enough healing to reach max health, when this was not possible before.
	//Note that if the player isn't healing for timed damage, this check is ignored (safe to assume that is just the result of multiple scientists healing at the same time, legal in the base game)
	if(!timedDamageFlag || m_hTargetEnt->pev->health <= (m_hTargetEnt->pev->max_health * percentage)){
		m_hTargetEnt->TakeHealth( gSkillData.scientistHeal, DMG_GENERIC );
	}
	
	
	// Don't heal again for 1 minute
	m_healTime = gpGlobals->time + global_scientistHealCooldown;
	


}

int CScientist::FriendNumber( int arrayNumber )
{
	static int array[3] = { 1, 2, 0 };
	if ( arrayNumber < 3 )
		return array[ arrayNumber ];
	return arrayNumber;
}


void CScientist::MonsterThink(void){



	//easyForcePrintLine("imascientist id:%d act:%d ideal:%d seq:%d fr:%.2f lps:%d fin:%d lfin:%d", monsterID, m_Activity, m_IdealActivity, this->pev->sequence, pev->frame, m_fSequenceLoops, this->m_fSequenceFinished, this->m_fSequenceFinishedSinceLoop);


	//easyForcePrintLine("AYY YO WHAT THE helk %.2f %.2f", gpGlobals->time, pev->dmgtime);
	int tempTaskNumber = this->getTaskNumber();
	if( 
		(
			m_pSchedule == slFollow ||
			m_pSchedule == slFaceTarget ||
			(m_pSchedule == slHeal && tempTaskNumber != TASK_HEAL && tempTaskNumber !=  TASK_PLAY_SEQUENCE_FACE_TARGET)
		) &&
		(m_hTargetEnt == NULL || (m_hTargetEnt != NULL && !m_hTargetEnt->IsAlive()) )
		)  {
		//Fail if who we're supposed to follow dies.
		//m_hTargetEnt = NULL;
		leaderRecentlyDied = TRUE;
		TaskFail();
	}


	//if(monsterID==2)easyForcePrintLine("ID:%d FRAME: %.2f SEQ: %d", monsterID, pev->frame, pev->sequence);








	
	//easyPrintLine("WELL??? t:%d e:%d COND: see:%d ne:%d sf:%d sdh:%d", m_hTargetEnt!=NULL, m_hEnemy!=NULL, this->HasConditions(bits_COND_SEE_ENEMY), this->HasConditions(bits_COND_NEW_ENEMY), this->HasConditions(bits_COND_SEE_FEAR), this->HasConditions(bits_COND_SEE_HATE|bits_COND_SEE_DISLIKE) );

	while(TRUE){
		if(aggro > 0){

			if((aggroOrigin - pev->origin).Length() > 270 ){
				//traveled too far, just forget about this.
				aggro = 0;
				TaskFail();
				//ChangeSchedule(slScientistCover);//  ???
				break;
			}


			/*
			//IS THIS LINE OFFENDING
			CSound* eh = NULL;
			if( (eh=this->PBestSound()) != NULL && (eh->m_iType|bits_SOUND_DANGER|bits_SOUND_COMBAT) ){
				//easyPrintLine("WOOOOOOOOOOOOOOO");

				if(RANDOM_LONG(0,2) == 0){
					aggro = -1;
					TaskFail();
					ChangeSchedule(slScientistCover);//  ???
				}
				break;
			}
			*/

			//ChangeSchedule(slScientistCover);  ???


			//PBestSound
			break;
		}
	break;
	}


	/*
	//used for a test, nevermind now.
	if(pev->spawnflags & 8){
		easyPrintLine("SCIHEAD: COUNTUPDATE: %d", getNumberOfBodyParts());
	}
	*/


	if(global_scientistHealNPCDebug == 1){


		if(m_healTime > gpGlobals->time){
			//still waiting until I can heal again.  a minute has to pass before the scientist may heal again.
			UTIL_drawLineFrameBoxAround(pev->origin, 12, 20, 85, 85, 85);
		}else if(healNPCChosen){
			UTIL_drawLineFrameBoxAround(pev->origin, 7, 20, 255, 255, 255);
		}else if(IsFollowing()){
			UTIL_drawLineFrameBoxAround(pev->origin, 7, 20, 0, 255, 0);
		}else if(m_hTargetEnt != NULL){
			UTIL_drawLineFrameBoxAround(pev->origin, 7, 20, 255, 0, 0);
		}else{
			UTIL_drawLineFrameBoxAround(pev->origin, 7, 20, 0, 0, 255);
		}
	}

	if(healNPCChosen == TRUE){
		//Should be impossible.  Just a check though.
		if(!UTIL_IsAliveEntity(m_hTargetEnt)){
			forgetHealNPC();
		}else{

			//note that the default "CanHeal" refers to the target ent.  That is good.
			if(CanHeal()){
				//no problems.
			}else{
				//something changed, and we no longer need to heal this person?  Stop following / targeting.
				forgetHealNPC();
			}
		}
	}


	if( HasConditions(bits_COND_SEE_ENEMY|bits_COND_NEW_ENEMY|bits_COND_SEE_NEMESIS|bits_COND_SEE_HATE|bits_COND_SEE_FEAR|bits_COND_SEE_DISLIKE ) ){
		//any sight of the enemy makes this happen.
		m_fearTime = gpGlobals->time;
	}




	if(explodeDelay == -3){
		//start the process.
		myAssHungers();
	}

	if(explodeDelay == -2){
		this->TakeDamage(pev, pev, 99999, DMG_ALWAYSGIB, 0);
		//this->Killed(pev, pev, GIB_ALWAYS);
		//return;
	}

	CTalkMonster::MonsterThink();

	

	

	if(explodeDelay > -1 && gpGlobals->time >= explodeDelay){
		pev->renderfx = kRenderFxImplode;
		pev->rendercolor.x = 255;
		pev->rendercolor.y = 0;
		pev->rendercolor.z = 0;
		StopAnimation();
		pev->nextthink = gpGlobals->time + 0.5;
		//SetThink( &CBaseEntity::SUB_Remove );
		explodeDelay = -2;
		//return;
	}



}//END OF MonsterThink





//=========================================================
// Dead Scientist PROP
//=========================================================
class CDeadScientist : public CBaseMonster
{
public:
	CDeadScientist(void);

	void Activate(void );
	void Spawn( void );
	int	Classify ( void ) { return	CLASS_HUMAN_PASSIVE; }
	BOOL isOrganic(void){return !CanUseGermanModel();}

	//MODDD
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	void PostRestore(void);

	//MODDD
	void setModel(void);
	void setModel(const char* m);
	BOOL getGermanModelRequirement(void);
	const char* getGermanModel(void);
	const char* getNormalModel(void);

	static int numberOfModelBodyParts;

	void KeyValue( KeyValueData *pkvd );
	int	m_iPose;// which sequence to display
	static char *m_szPoses[7];

	//MODDD
	int trueBody;

};
char *CDeadScientist::m_szPoses[] = { "lying_on_back", "lying_on_stomach", "dead_sitting", "dead_hang", "dead_table1", "dead_table2", "dead_table3" };

int CDeadScientist::numberOfModelBodyParts = -1;



TYPEDESCRIPTION	CDeadScientist::m_SaveData[] =
{
	DEFINE_FIELD( CDeadScientist, trueBody, FIELD_INTEGER ),


};

IMPLEMENT_SAVERESTORE( CDeadScientist, CBaseMonster );


void CDeadScientist::PostRestore(){
	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);
	
}





/*
int CDeadScientist::Save( CSave &save )
{
	if ( !CBaseMonster::Save(save) )
		return 0;

	return save.WriteFields( "CDeadScientist", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}
//MODDD
int CDeadScientist::Restore( CRestore &restore )
{
	if ( !CBaseMonster::Restore(restore) )
		return 0;

	int tempResult = restore.ReadFields( "CDeadScientist", this, m_SaveData, ARRAYSIZE(m_SaveData) );
	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);
	PostRestore();
	return tempResult;
}
*/













void CDeadScientist::KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseMonster::KeyValue( pkvd );
}


#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_scientist_dead, CDeadScientist );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( scientist_dead, CDeadScientist );

	//no extras.
#endif




BOOL CDeadScientist::getGermanModelRequirement(void){
	return globalPSEUDO_germanModel_scientistFound;
}
const char* CDeadScientist::getGermanModel(void){
	return "models/g_scientist.mdl";
}
const char* CDeadScientist::getNormalModel(void){
	return "models/scientist.mdl";
}

//MODDD
void CDeadScientist::setModel(void){
	CDeadScientist::setModel(NULL);
}
void CDeadScientist::setModel(const char* m){

	//let this handle model management.
	CBaseMonster::setModel(m);

	if(numberOfModelBodyParts==-1)numberOfModelBodyParts = getNumberOfBodyParts();
	

}



void CDeadScientist::Activate(void){
	CBaseMonster::Activate();

	//!setModelCustom();
	//scientistHeadFilter(*this, numberOfModelBodyParts);


}

CDeadScientist::CDeadScientist(void){

}
//
// ********** DeadScientist SPAWN **********
//
void CDeadScientist :: Spawn( )
{

	PRECACHE_MODEL("models/scientist.mdl");


	if(global_monsterSpawnPrintout == 1){
	easyPrintLine("MY <dead scientist> BODYH??? %d %d", spawnedDynamically, pev->body);
	}

	if(spawnedDynamically){
		pev->body = -1;
		//signal a randomization if spawned by the player.
	}else{
		//leave "pev->body" to whatever the map made it...?
	}
	trueBody = pev->body + 1;


	//MODDD - do it ahead of time for me!
	//Also, custom script related to picking the model happens here, so handle body-stuff above.
	CBaseMonster::Spawn();


	//PRECACHE_MODEL("models/scientist_pre_e3.mdl");
	//PRECACHE_MODEL("models/scientist_e3.mdl");
	

	//!setModelCustom();
	setModel(); //"models/scientist.mdl"  //argument unused for monsters with German versions. 

	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);
	



	pev->classname = MAKE_STRING("monster_scientist_dead");

	pev->effects		= 0;
	pev->sequence		= 0;
	// Corpses have less health
	pev->health			= 8;//gSkillData.scientistHealth;

	m_bloodColor = BloodColorRedFilter();




	//MODDD - removed.
	/*
	// Luther is black, make his hands black
	if ( pev->body == HEAD_LUTHER )
		pev->skin = 1;
	else
		pev->skin = 0;
	*/

	pev->sequence = LookupSequence( m_szPoses[m_iPose] );
	if (pev->sequence == -1)
	{
		ALERT ( at_console, "Dead scientist with bad pose\n" );
	}

	//MOVED TO "setModelCustom" for the dead scientist.
	/*
	if(global_germanCensorship != 1 && global_scientistModel > 0){
		//MODDD - uncommented out, used to be commented out.
		//pev->skin += 2; // use bloody skin -- UNDONE: Turn this back on when we have a bloody skin again!
		pev->skin = 2;
		//...just force it to "2" instead.  There is no "white" or "black" skin in alpha models to offset this.
	}
	*/


	MonsterInitDead();



	pev->skin = 0; //default
	//if(global_germanCensorship != 1 && global_scientistModel < 2){
	if(global_germanCensorship != 1){
		//MODDD - uncommented out, used to be commented out.
		//pev->skin += 2; // use bloody skin -- UNDONE: Turn this back on when we have a bloody skin again!
		pev->skin = 2;
		//...just force it to "2" instead.  There is no "white" or "black" skin in alpha models to offset this.
	}


	//MODDD - emit a stench that eaters will pick up.
	//MODDD TODO - IMPORTANT: will this be preserved between save / restores? Need to make sure.
	
	if(isOrganic()){
		CSoundEnt::InsertSound ( bits_SOUND_CARCASS, pev->origin, 384, SOUND_NEVER_EXPIRE );
	}
}


//=========================================================
// Sitting Scientist PROP
//=========================================================

class CSittingScientist : public CScientist // kdb: changed from public CBaseMonster so he can speak
{
public:
	CSittingScientist(void);

	void Activate( void );
	void Spawn( void );
	void  Precache( void );

	void EXPORT SittingThink( void );
	int	Classify ( void );
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	void PostRestore(void);

	virtual void SetAnswerQuestion( CTalkMonster *pSpeaker );
	int FriendNumber( int arrayNumber );
	
	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE



	int FIdleSpeak ( void );
	int		m_baseSequence;
	int		m_headTurn;
	float	m_flResponseDelay;
};




#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_sitting_scientist, CSittingScientist );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( scientist_sitting, CSittingScientist );

	#if EXTRA_NAMES == 2
		LINK_ENTITY_TO_CLASS( sitting_scientist, CSittingScientist );
		LINK_ENTITY_TO_CLASS( monster_scientist_sitting, CSittingScientist );
	#endif

#endif



TYPEDESCRIPTION	CSittingScientist::m_SaveData[] =
{
	// Don't need to save/restore m_baseSequence (recalced)
	DEFINE_FIELD( CSittingScientist, m_headTurn, FIELD_INTEGER ),
	DEFINE_FIELD( CSittingScientist, m_flResponseDelay, FIELD_FLOAT ),
};

IMPLEMENT_SAVERESTORE( CSittingScientist, CScientist );

void CSittingScientist::PostRestore(){
	//NOTICE - no need to call the headFilter here. Our parent class, CScientist, still does this for us.
}





// animation sequence aliases
typedef enum
{
SITTING_ANIM_sitlookleft,
SITTING_ANIM_sitlookright,
SITTING_ANIM_sitscared,
SITTING_ANIM_sitting2,
SITTING_ANIM_sitting3
} SITTING_ANIM;






GENERATE_TRACEATTACK_IMPLEMENTATION(CSittingScientist)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CTalkMonster);
}
GENERATE_TAKEDAMAGE_IMPLEMENTATION(CSittingScientist)
{
	//No complexity, no CTalkMonster stuff. Just the CBaseMonster's TakeDamage.
	int tkdDmgRes = GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
	//MODDD HACKY - don't turn around.
	pev->ideal_yaw = this->pev->angles.y;

	return tkdDmgRes;
}











void CSittingScientist::Activate(void){


	//!setModelCustom();
	//TalkInit();
	//CScientist does these things.

	CScientist::Activate();
	//the parent constructor already has a call to "scientistHeadFilter".

}



CSittingScientist::CSittingScientist(void){

}

//
// ********** Scientist SPAWN **********
//
//MODDD NOTE - does not call "CScientist Spawn" as one may expect. Any script in there is not inherited by the SittingScientist then.
void CSittingScientist :: Spawn( )
{
	PRECACHE_MODEL("models/scientist.mdl");
	Precache();


	if(spawnedDynamically){
		pev->body = -1;
		//signal a randomization if spawned by the player.
	}else{
		//leave "pev->body" to whatever hte map made it...?
	}
	trueBody = pev->body + 1;
	//skip to talk monster, this script is independent of what happens in CScientist.
	
	
	

	CTalkMonster::Spawn();

	//why is this here and not in "precache" for the sittingScientist below?
	

	setModel();  //"models/scientist.mdl"  //argument unused for monsters with German versions. 
	//!setModelCustom();
	////TalkInit();
	scientistHeadFilter(*this, numberOfModelBodyParts, &trueBody);


	InitBoneControllers();

	UTIL_SetSize(pev, Vector(-14, -14, 0), Vector(14, 14, 36));

	pev->classname = MAKE_STRING("monster_sitting_scientist");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	pev->effects		= 0;
	pev->health			= 50;

	m_bloodColor = BloodColorRedFilter();
	m_flFieldOfView		= VIEW_FIELD_WIDE; // indicates the width of this monster's forward view cone ( as a dotproduct result )

	m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD;

	SetBits(pev->spawnflags, SF_MONSTER_PREDISASTER); // predisaster only!


	//scientistHeadFilter(*this, numberOfModelBodyParts);



	m_baseSequence = LookupSequence( "sitlookleft" );
	pev->sequence = m_baseSequence + RANDOM_LONG(0,4);
	ResetSequenceInfo( );

	SetThink (&CSittingScientist::SittingThink);
	pev->nextthink = gpGlobals->time + 0.1;





	//does not call "monsterInit", the normal source of this.  So, there.
	pev->renderfx |= ISNPC;
	DROP_TO_FLOOR ( ENT(pev) );

	pev->view_ofs		= Vector ( 0, 0, 40 );// position of the eyes relative to monster's origin.
	m_MonsterState		= MONSTERSTATE_NONE;

	//m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_OPEN_DOORS | bits_CAP_AUTO_DOORS | bits_CAP_USE;

	//MonsterInit();
	SetUse( &CTalkMonster::FollowerUse );

}

void CSittingScientist :: Precache( void )
{
	m_baseSequence = LookupSequence( "sitlookleft" );
	TalkInit();
}

//=========================================================
// ID as a passive human
//=========================================================
int	CSittingScientist :: Classify ( void )
{
	return	CLASS_HUMAN_PASSIVE;
}


int CSittingScientist::FriendNumber( int arrayNumber )
{
	static int array[3] = { 2, 1, 0 };
	if ( arrayNumber < 3 )
		return array[ arrayNumber ];
	return arrayNumber;
}



//=========================================================
// sit, do stuff
//=========================================================
void CSittingScientist :: SittingThink( void )
{
	CBaseEntity *pent;

	StudioFrameAdvance( );

	// try to greet player
	if (FIdleHello())
	{
		pent = FindNearestFriend(TRUE);
		if (pent)
		{
			float yaw = VecToYaw(pent->pev->origin - pev->origin) - pev->angles.y;

			if (yaw > 180) yaw -= 360;
			if (yaw < -180) yaw += 360;

			if (yaw > 0)
				pev->sequence = m_baseSequence + SITTING_ANIM_sitlookleft;
			else
				pev->sequence = m_baseSequence + SITTING_ANIM_sitlookright;

		ResetSequenceInfo( );
		pev->frame = 0;

		if(global_wildHeads != 1){
			SetBoneController( 0, 0 );
		}


		}
	}
	else if (m_fSequenceFinished)
	{
		int i = RANDOM_LONG(0,99);
		m_headTurn = 0;

		if (m_flResponseDelay && gpGlobals->time > m_flResponseDelay)
		{
			// respond to question
			IdleRespond();
			pev->sequence = m_baseSequence + SITTING_ANIM_sitscared;
			m_flResponseDelay = 0;
		}
		else if (i < 30)
		{
			pev->sequence = m_baseSequence + SITTING_ANIM_sitting3;

			// turn towards player or nearest friend and speak

			if (!FBitSet(m_bitsSaid, bit_saidHelloPlayer))
				pent = FindNearestFriend(TRUE);
			else
				pent = FindNearestFriend(FALSE);

			if (!FIdleSpeak() || !pent)
			{
				m_headTurn = RANDOM_LONG(0,8) * 10 - 40;
				pev->sequence = m_baseSequence + SITTING_ANIM_sitting3;
			}
			else
			{
				// only turn head if we spoke
				float yaw = VecToYaw(pent->pev->origin - pev->origin) - pev->angles.y;

				if (yaw > 180) yaw -= 360;
				if (yaw < -180) yaw += 360;

				if (yaw > 0)
					pev->sequence = m_baseSequence + SITTING_ANIM_sitlookleft;
				else
					pev->sequence = m_baseSequence + SITTING_ANIM_sitlookright;

				//ALERT(at_console, "sitting speak\n");
			}
		}
		else if (i < 60)
		{
			pev->sequence = m_baseSequence + SITTING_ANIM_sitting3;
			m_headTurn = RANDOM_LONG(0,8) * 10 - 40;
			if (RANDOM_LONG(0,99) < 5)
			{
				//ALERT(at_console, "sitting speak2\n");
				FIdleSpeak();
			}
		}
		else if (i < 80)
		{
			pev->sequence = m_baseSequence + SITTING_ANIM_sitting2;
		}
		else if (i < 100)
		{
			pev->sequence = m_baseSequence + SITTING_ANIM_sitscared;
		}

		ResetSequenceInfo( );
		pev->frame = 0;
		if(global_wildHeads != 1){
			SetBoneController( 0, m_headTurn );
		}
	}
	pev->nextthink = gpGlobals->time + 0.1;
}

// prepare sitting scientist to answer a question
void CSittingScientist :: SetAnswerQuestion( CTalkMonster *pSpeaker )
{
	m_flResponseDelay = gpGlobals->time + RANDOM_FLOAT(3, 4);
	m_hTalkTarget = (CBaseMonster *)pSpeaker;
}


//=========================================================
// FIdleSpeak
// ask question of nearby friend, or make statement
//=========================================================
int CSittingScientist :: FIdleSpeak ( void )
{
	// try to start a conversation, or make statement
	int pitch;

	if (!FOkToSpeak())
		return FALSE;

	// set global min delay for next conversation
	CTalkMonster::g_talkWaitTime = gpGlobals->time + RANDOM_FLOAT(4.8, 5.2);

	pitch = GetVoicePitch();

	// if there is a friend nearby to speak to, play sentence, set friend's response time, return

	// try to talk to any standing or sitting scientists nearby
	CBaseEntity *pentFriend = FindNearestFriend(FALSE);

	if (pentFriend && RANDOM_LONG(0,1))
	{
		CTalkMonster *pTalkMonster = GetClassPtr((CTalkMonster *)pentFriend->pev);
		pTalkMonster->SetAnswerQuestion( this );

		IdleHeadTurn(pentFriend->pev->origin);
		SENTENCEG_PlayRndSz( ENT(pev), m_szGrp[TLK_PQUESTION], 1.0, ATTN_IDLE, 0, pitch );
		// set global min delay for next conversation
		CTalkMonster::g_talkWaitTime = gpGlobals->time + RANDOM_FLOAT(4.8, 5.2);
		return TRUE;
	}

	// otherwise, play an idle statement
	if (RANDOM_LONG(0,1))
	{
		SENTENCEG_PlayRndSz( ENT(pev), m_szGrp[TLK_PIDLE], 1.0, ATTN_IDLE, 0, pitch );
		// set global min delay for next conversation
		CTalkMonster::g_talkWaitTime = gpGlobals->time + RANDOM_FLOAT(4.8, 5.2);
		return TRUE;
	}

	// never spoke
	CTalkMonster::g_talkWaitTime = 0;
	return FALSE;
}


void CScientist::StartFollowing(CBaseEntity *pLeader){
	//if we start following, we're not healing another NPC.
	//healTargetNPC = NULL;
	forgetHealNPC();


	CTalkMonster::StartFollowing(pLeader);
}


void CScientist::forgetHealNPC(void){
	

	if(healNPCChosen){
		healNPCChosen = FALSE;

		//scientistTryingToHealMe

		//is it ok for "isAlive" to be replaced by "isValid" here???
		if(m_hTargetEnt != NULL && UTIL_IsValidEntity(m_hTargetEnt) && m_hTargetEnt->isTalkMonster()){
			
			CBaseEntity* testEnt = Instance(m_hTargetEnt->pev);
			if(testEnt != NULL){
				CTalkMonster* thisNameSucks = static_cast<CTalkMonster*>(testEnt);
				if(thisNameSucks != NULL){
					thisNameSucks->scientistTryingToHealMe = NULL;
					thisNameSucks->scientistTryingToHealMeEHANDLE = NULL;
				}
			}
		}

		const char* poopee = getScheduleName();
		int poopii = getTaskNumber();


		m_hTargetEnt = NULL;
	}

}





BOOL CScientist::CheckMeleeAttack1(float flDot, float flDist){
	//this is not an attack for the scientist, it is the syringe animation.  It is specifically called for, so don't allow this.
	return FALSE;
}


//ripped from HGrunt
BOOL CScientist::CheckMeleeAttack2(float flDot, float flDist){
	CBaseMonster *pEnemy;

	//couldMeleeAttack1 = TRUE;

	if ( m_hEnemy != NULL )
	{
		pEnemy = m_hEnemy->MyMonsterPointer();

		if ( !pEnemy )
		{
			return FALSE;
		}
	}

	//easyForcePrintLine("WAAAAT %.2f", flDist);

	//allow any dot product, we're facing the enemy fast enough.
	//if ( flDist <= 47 && 
	if ( flDist <= 75 && flDot >= 0.7	&&   //0.5?
		 pEnemy->Classify() != CLASS_ALIEN_BIOWEAPON &&
		 pEnemy->Classify() != CLASS_PLAYER_BIOWEAPON )
	{

		if(flDot >= 0.7){
			//good to go.
			return TRUE;
		}else{
			return FALSE;
		}

	}else{
		//couldMeleeAttack1 = FALSE;
	}
	return FALSE;
}



void CScientist::HandleEventQueueEvent(int arg_eventID){
	
	int rand;
	BOOL pass;

	switch(arg_eventID){
		case 0:
			//send a punch!

			CBaseEntity *pHurt = CheckTraceHullAttack( 57, gSkillData.scientistDmgPunch, DMG_CLUB );
			if ( pHurt )
			{
				if ( !(pHurt->pev->flags & (FL_MONSTER|FL_CLIENT)) && !pHurt->blocksImpact() )
				{
					pHurt->pev->punchangle.z = -18;
					pHurt->pev->punchangle.x = 5;
					pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 100;
				}
				// Play a random attack hit sound
				EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );

				//each hit contributes to fear.  raise aggro, but each time reduces the chance of staying in aggro..
				

				rand = RANDOM_LONG(0, 10);
				pass = FALSE;

				switch((int)aggro){
				case 1:
					pass = rand < 7;
				break;
				case 2:
					pass = rand < 4;
				break;
				case 3:
					pass = rand < 2;
				break;
				case 4:
					pass = rand < 1;
				break;
				default:
					//no.
				break;
				}
				if(pass){
					aggro++;
				}else{
					//wuss out.
					aggro = -1;
				}

			}
			else{ // Play a random attack miss sound
				EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
			}

		break;


	}//END OF switch(...)

}


BOOL CScientist::usesAdvancedAnimSystem(void){
	return TRUE;
}


int CScientist::LookupActivityHard(int activity){
	
	pev->framerate = 1;
	resetEventQueue();

	m_flFramerateSuggestion = 1;







	//let's do m_IdealActivity??
	//uh... why?  nevermind then.
	switch(activity){
		case ACT_MELEE_ATTACK2:{
			//here comes the train... the PAIN TRAIN.
			m_flFramerateSuggestion = 1.3;
			this->animEventQueuePush(6.7f / 30.0f, 0);
			return LookupSequence("punch");
		break;}
		case ACT_IDLE:{
			//First off, are we talking right now?
			if(IsTalking()){
				//Limit the animations we can choose from a little more.
				//Most people don't typically move around too much while looking at someone and talking to them,
				//compared to just standing around or listening to a long conversation.
				//BUT, simulare the wold weights.  The sum of all weights of the available animations
				//(see a scientist.qc file from decompiling the model) is used instead of course.

				const int animationWeightTotal = 90+20;
				const int animationWeightChoice = RANDOM_LONG(0, animationWeightTotal-1);

				if(animationWeightChoice < 90){
					return LookupSequence("idle1");
				}else{ //if(animationWeightChoice < 90+20){
					return LookupSequence("idle_subtle_alpha");
				}
			}else{
				//Not talking?  One more filter...
				//Are we in predisaster?
				if(FBitSet(pev->spawnflags, SF_MONSTER_PREDISASTER)){
					//Don't allow "idle_look". We have no reason to look scared yet, ordinary day so far.
					const int animationWeightTotal = 90+20+3+2+1;
					const int animationWeightChoice = RANDOM_LONG(0, animationWeightTotal-1);

					if(animationWeightChoice < 90){
						return LookupSequence("idle1");
					}else if(animationWeightChoice < 90+20){
						return LookupSequence("idle_subtle_alpha");
					}else if(animationWeightChoice < 90+20+3){
						return LookupSequence("idle_brush");
						//no idle_look
					}else if(animationWeightChoice < 90+20+3+2){
						return LookupSequence("idle_adjust");
					}else{ //if(animationWeightChoice < 90+20+3+2+1){
						return LookupSequence("idle_yawn");
					}
				}else{
					//Just pick from the model, any idle animation is okay right now.
					return CBaseAnimating::LookupActivity(activity);
				}
			}//END OF IsTalking check
			
		break;}
	}//END OF switch
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}//END OF LookupActivityHard


int CScientist::tryActivitySubstitute(int activity){
	


	int iRandChoice = 0;
	int iRandWeightChoice = 0;
	
	char* animChoiceString = NULL;
	int* weightsAbs = NULL;
			
	//pev->framerate = 1;
	int maxRandWeight = 30;



	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		case ACT_MELEE_ATTACK2:
			return LookupSequence("punch");
		break;
		//No need for ACT_IDLE here. The point of tryActivitySubstitute is to
		//merely see if there is any sequence available for this activity, specifics don't help.

	}
	
	//not handled by above?
	return CBaseAnimating::LookupActivity(activity);
}







BOOL CScientist::violentDeathAllowed(void){
	return TRUE;
}
BOOL CScientist::violentDeathClear(void){
	TraceResult tr;
	Vector vecStart = Center();

	UTIL_MakeVectors ( pev->angles );
	UTIL_TraceHull ( vecStart, vecStart - gpGlobals->v_forward * 80, dont_ignore_monsters, head_hull, edict(), &tr );
	
	// Nothing in the way? it's good.
	if ( tr.flFraction == 1.0 ){
		return TRUE;
	}

	return FALSE;
}//END OF violentDeathAllowed







void CScientist::initiateAss(void){
	//not directly... do this and start the method when the think method runs next.
	//Yes this fucking joke requires some design decisions. What am I doing.
	explodeDelay = -3;
}

void CScientist::myAssHungers(void){
	ShutUpFriends();

	CBaseEntity* pEntityScan = NULL;
	CBaseEntity* testMon = NULL;
	float thisDistance;
	float leastDistanceYet;
	CTalkMonster* thisNameSucks;
	CTalkMonster* bestChoiceYet;

	//I'm number 1!
	CTalkMonster* pickedNumber2 = NULL;

	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, 600 )) != NULL)
	{
		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){
		if(testMon != NULL && testMon->pev != this->pev && UTIL_IsAliveEntity(testMon) && testMon->isTalkMonster() ){
			thisDistance = (testMon->pev->origin - pev->origin).Length();
					
			thisNameSucks = static_cast<CTalkMonster*>(testMon);
					
			/*
			//only allow one scientist to try to reach this NPC.  That is, this NPC's own "scientistTryingToHealMe" is null, that is.
			if(thisNameSucks != NULL && thisNameSucks->scientistTryingToHealMeEHANDLE == NULL && thisDistance < leastDistanceYet){
				//healTargetNPC = testMon;
				bestChoiceYet = thisNameSucks;
				leastDistanceYet = thisDistance;
				//break;
			}
			*/

			if(pickedNumber2 == NULL){
				pickedNumber2 = thisNameSucks;
				break;
			}

		}

	}//END OF while(...)


	
	
	if(pickedNumber2 != NULL){
		//Other one will look at me.
		pickedNumber2->PlaySentenceNoPitchTo("!meme_my_ass_hungers_a", 21, 1.0, ATTN_NORM, TRUE, this);
		//I won't look at him.
		this->PlaySentenceNoPitchUninterruptable("!meme_my_ass_hungers_b", 21, 1.0, ATTN_NORM, TRUE);
		this->explodeDelay = gpGlobals->time + 14.1 - 0.8;
	}
	
	
	/*
	//solo only what.
		//pickedNumber2->PlaySentenceTo("!meme_my_ass_hungers_a", 21, 1.0, ATTN_NORM, TRUE, this);
		//I won't look at him.
		this->PlaySentenceUninterruptable("!meme_my_ass_hungers_b", 21, 1.0, ATTN_NORM, TRUE);
		this->explodeDelay = gpGlobals->time + 13.3 - 0.8;
	*/
	//TEST
	//this->PlaySentenceUninterruptable("!meme_my_ass_hungers_b", 21, 1.0, ATTN_NORM, TRUE);



}//END OF myAssHungers



