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



//MODDD TODO - the melee attack seems to be completely unused, even as of retail there wasn't any case of checking for an area in front to do a
//             melee attack (CheckTraceHullAttack, TakeDamage, punchangle, velocity shove).
//             So the melee attack anim went completely unused and wouldn't do anything (physically) even if it did play.
//             Check to see if it has any events in HandleAnimEvent if played at all, and if not can create our own hardcoded ones the usual way.

//MODDD TODO - FUTURE. Perhaps the HAssassin should do a check for  nearby undetonated grenades, player or hgrunt / hassassin-thrown, 
//                     and make an effort not to jump into their blast radius? Looks silly when that happens.

//MODDD - TODO. Notice that SCHED_INVESTIGATE_SOUND goes unused in most places. Could it be part of more human / intelligent alien NPC GetSchedule's?



#if !defined( OEM_BUILD ) && !defined( HLDEMO_BUILD )

//=========================================================
// hassassin - Human assassin, fast and stealthy
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"squadmonster.h"
#include	"weapons.h"
#include	"soundent.h"
#include	"game.h"

extern DLL_GLOBAL int  g_iSkillLevel;

//=========================================================
// monster-specific schedule types
//=========================================================
enum
{
	SCHED_ASSASSIN_EXPOSED = LAST_COMMON_SCHEDULE + 1,// cover was blown.
	SCHED_ASSASSIN_JUMP,	// fly through the air
	SCHED_ASSASSIN_JUMP_ATTACK,	// fly through the air and shoot
	SCHED_ASSASSIN_JUMP_LAND, // hit and run away
	SCHED_ASSASSIN_TAKE_COVER_RELOAD,   //MODDD - new
	SCHED_ASSASSIN_RELOAD,
	SCHED_ASSASSIN_RANGE_ATTACK1_CANRELOAD,
	SCHED_ASSASSIN_RANGE_ATTACK2_CANRELOAD,  //special, to allow reloading in case the grenade can't work.
};

//=========================================================
// monster-specific tasks
//=========================================================

enum
{
	TASK_ASSASSIN_FALL_TO_GROUND = LAST_COMMON_TASK + 1, // falling and waiting to hit ground
};


//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define		ASSASSIN_AE_SHOOT1	1
#define		ASSASSIN_AE_TOSS1	2
#define		ASSASSIN_AE_JUMP	3


#define bits_MEMORY_BADJUMP		(bits_MEMORY_CUSTOM1)


//#define HASSASSIN_CROSSBOW_RELOAD_APPLY_DELAY 1.1
EASY_CVAR_EXTERN(hassassinCrossbowReloadSoundDelay)

#define HASSASSIN_CROSSBOW_RELOAD_ANIM "reload"




extern float global_thatWasntPunch;


EASY_CVAR_EXTERN(hassassinCrossbowDebug);







class CHAssassin : public CBaseMonster
{
public:
	CHAssassin();
	
	void MonsterThink(void);
	int IRelationship ( CBaseEntity *pTarget );
	


	void EXPORT HAssassinTouch ( CBaseEntity *pOther );
	
	void Spawn( void );
	void Precache( void );
	void SetYawSpeed ( void );
	int  Classify ( void );
	int  ISoundMask ( void);
	void Shoot( void );

	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	GENERATE_KILLED_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE
	
	BOOL attemptDropWeapon(void);

	BOOL canResetBlend0(void);
	BOOL onResetBlend0(void);




	void HandleAnimEvent( MonsterEvent_t *pEvent );
	Schedule_t* GetSchedule ( void );
	Schedule_t* GetScheduleOfType ( int Type );
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );	// jump
	// BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );	// shoot
	BOOL CheckRangeAttack2 ( float flDot, float flDist );	// throw grenade
	void StartTask ( Task_t *pTask );
	void RunAI( void );
	void RunTask ( Task_t *pTask );
	void DeathSound ( void );
	void IdleSound ( void );
	
	Vector GetGunPosition(void);
	Vector GetGunPositionAI(void);
	
	CUSTOM_SCHEDULES;

	int	Save( CSave &save ); 
	int Restore( CRestore &restore );
	static TYPEDESCRIPTION m_SaveData[];


	
	BOOL usesAdvancedAnimSystem(void);
	int tryActivitySubstitute(int activity);
	int LookupActivityHard(int activity);
	
	void HandleEventQueueEvent(int arg_eventID);


	float m_flLastShot;
	float m_flDiviation;

	float m_flNextJump;
	Vector m_vecJumpVelocity;

	float m_flNextGrenadeCheck;
	Vector	m_vecTossVelocity;
	BOOL	m_fThrowGrenade;

	int		m_iTargetRanderamt;

	int		m_iFrustration;

	int		m_iShell;

	//NEW
	float reloadApplyTime;
	int shootEventOffset;
	BOOL droppedWeapon;
	float groundTouchCheckDuration;


};










//=========================================================
// AI Schedules Specific to this monster
//=========================================================

//=========================================================
// Fail Schedule
//=========================================================
Task_t	tlAssassinFail[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT_FACE_ENEMY,		(float)2		},
	// { TASK_WAIT_PVS,			(float)0		},
	{ TASK_SET_SCHEDULE,		(float)SCHED_CHASE_ENEMY },
};

Schedule_t	slAssassinFail[] =
{
	{
		tlAssassinFail,
		ARRAYSIZE ( tlAssassinFail ),
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_PROVOKED			|
		bits_COND_CAN_RANGE_ATTACK1 |
		bits_COND_CAN_RANGE_ATTACK2 |
		bits_COND_CAN_MELEE_ATTACK1 |
		bits_COND_HEAR_SOUND,
	
		bits_SOUND_DANGER |
		bits_SOUND_PLAYER,
		"AssassinFail"
	},
};


//=========================================================
// Enemy exposed Agrunt's cover
//=========================================================
Task_t	tlAssassinExposed[] =
{
	{ TASK_STOP_MOVING,			(float)0							},
	
	//Set activity to looking ready to fire?
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE_ANGRY },



	//MODDD - Woa  hey, at least face the enemy first! How are you firing that crossbow, even pistol before sideways?
	{ TASK_FACE_ENEMY,			(float)0							},

	//Special task. This will FAIL the schedule if we are unable to make a ranged attack.
	{ TASK_CHECK_RANGED_ATTACK_1,(float)0							},

	{ TASK_RANGE_ATTACK1,		(float)0							},


	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_ASSASSIN_JUMP			},

	{ TASK_SET_SCHEDULE,		(float)SCHED_TAKE_COVER_FROM_ENEMY	},
};

Schedule_t slAssassinExposed[] =
{
	{
		tlAssassinExposed,
		ARRAYSIZE ( tlAssassinExposed ),
		bits_COND_CAN_MELEE_ATTACK1,
		0,
		"AssassinExposed",
	},
};


//=========================================================
// Take cover from enemy! Tries lateral cover before node 
// cover! 
//=========================================================
Task_t	tlAssassinTakeCoverFromEnemy[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_ASSASSIN_RANGE_ATTACK1_CANRELOAD	},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	{ TASK_FACE_ENEMY,				(float)0					},
};

Schedule_t	slAssassinTakeCoverFromEnemy[] =
{
	{ 
		tlAssassinTakeCoverFromEnemy,
		ARRAYSIZE ( tlAssassinTakeCoverFromEnemy ), 
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_MELEE_ATTACK1		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinTakeCoverFromEnemy"
	},
};


//=========================================================
// Take cover from enemy! Tries lateral cover before node 
// cover! 
//=========================================================
Task_t	tlAssassinTakeCoverFromEnemy2[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	
	//We need a little time to turn now, don't wait.
	//{ TASK_WAIT,					(float)0.2					},

	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_RANGE_ATTACK1,			(float)0					},
	//in case this "takeCover" fails, our only option may be to just start reloading.
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_ASSASSIN_RANGE_ATTACK2_CANRELOAD	},
	{ TASK_FIND_FAR_NODE_COVER_FROM_ENEMY,	(float)24			},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	{ TASK_FACE_ENEMY,				(float)0					},
};

Schedule_t	slAssassinTakeCoverFromEnemy2[] =
{
	{ 
		tlAssassinTakeCoverFromEnemy2,
		ARRAYSIZE ( tlAssassinTakeCoverFromEnemy2 ), 
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_MELEE_ATTACK2		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinTakeCoverFromEnemy2"
	},
};


//=========================================================
// hide from the loudest sound source
//=========================================================
Task_t	tlAssassinTakeCoverFromBestSound[] =
{
	{ TASK_SET_FAIL_SCHEDULE,			(float)SCHED_MELEE_ATTACK1	},
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_BEST_SOUND,	(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slAssassinTakeCoverFromBestSound[] =
{
	{ 
		tlAssassinTakeCoverFromBestSound,
		ARRAYSIZE ( tlAssassinTakeCoverFromBestSound ), 
		bits_COND_NEW_ENEMY,
		0,
		"AssassinTakeCoverFromBestSound"
	},
};





//=========================================================
// AlertIdle Schedules
//=========================================================
Task_t	tlAssassinHide[] =
{
	{ TASK_STOP_MOVING,			0						 },
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE			 },
	{ TASK_WAIT,				(float)5				 },
	{ TASK_SET_SCHEDULE,		(float)SCHED_CHASE_ENEMY },
};

Schedule_t	slAssassinHide[] =
{
	{ 
		tlAssassinHide,
		ARRAYSIZE ( tlAssassinHide ), 
		bits_COND_NEW_ENEMY				|
		bits_COND_SEE_ENEMY				|
		bits_COND_SEE_FEAR				|
		bits_COND_LIGHT_DAMAGE			|
		bits_COND_HEAVY_DAMAGE			|
		bits_COND_PROVOKED		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinHide"
	},
};



//=========================================================
// HUNT Schedules
//=========================================================
Task_t tlAssassinHunt[] = 
{
	//{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	//{ TASK_RUN_PATH,			(float)0		},

	//smart versio.
	
	{TASK_MOVE_TO_ENEMY_RANGE, (float)0					},
	{TASK_CHECK_STUMPED, (float)0						},

	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
};

Schedule_t slAssassinHunt[] =
{
	{ 
		tlAssassinHunt,
		ARRAYSIZE ( tlAssassinHunt ),
		bits_COND_NEW_ENEMY			|
		// bits_COND_SEE_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinHunt"
	},
};


//=========================================================
// Jumping Schedules
//=========================================================
Task_t	tlAssassinJump[] =
{
	{ TASK_STOP_MOVING,			(float)0		},
	{ TASK_PLAY_SEQUENCE,		(float)ACT_HOP	},
	{ TASK_SET_SCHEDULE,		(float)SCHED_ASSASSIN_JUMP_ATTACK },
};

Schedule_t	slAssassinJump[] =
{
	{ 
		tlAssassinJump,
		ARRAYSIZE ( tlAssassinJump ), 
		0, 
		0, 
		"AssassinJump"
	},
};


//MODDD TODO - would it be good to give the hassassin some sort of ariel kick attack or whatever this was meant to be...?
//=========================================================
// repel 
//=========================================================
Task_t	tlAssassinJumpAttack[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_ASSASSIN_JUMP_LAND	},
	// { TASK_SET_ACTIVITY,		(float)ACT_FLY	},
	{ TASK_ASSASSIN_FALL_TO_GROUND, (float)0		},
};


Schedule_t	slAssassinJumpAttack[] =
{
	{ 
		tlAssassinJumpAttack,
		ARRAYSIZE ( tlAssassinJumpAttack ), 
		0, 
		0,
		"AssassinJumpAttack"
	},
};


//=========================================================
// repel 
//=========================================================
Task_t	tlAssassinJumpLand[] =
{
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_ASSASSIN_EXPOSED	},
	// { TASK_SET_FAIL_SCHEDULE,		(float)SCHED_MELEE_ATTACK1	},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE				},
	{ TASK_REMEMBER,				(float)bits_MEMORY_BADJUMP	},
	{ TASK_FIND_NODE_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_FORGET,					(float)bits_MEMORY_BADJUMP	},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_RANGE_ATTACK1	},
};

Schedule_t	slAssassinJumpLand[] =
{
	{ 
		tlAssassinJumpLand,
		ARRAYSIZE ( tlAssassinJumpLand ), 
		0, 
		0,
		"AssassinJumpLand"
	},
};





Task_t	tlAssassinTakeCoverReload[] =
{
	{ TASK_SET_ACTIVITY,		    (float)ACT_COMBAT_IDLE			    },
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_RANGE_ATTACK1	},
	{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	{ TASK_FACE_ENEMY,				(float)0					},
	//reload!
	{ TASK_RELOAD,					(float)0					},
};

Schedule_t	slAssassinTakeCoverReload[] =
{
	{ 
		tlAssassinTakeCoverReload,
		ARRAYSIZE ( tlAssassinTakeCoverReload ), 
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_MELEE_ATTACK1		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinTakeCoverReload"
	},
};



Task_t	tlAssassinReload[] =
{
	{ TASK_SET_ACTIVITY,		    (float)ACT_COMBAT_IDLE			    },
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	//{ TASK_SET_FAIL_SCHEDULE,		(float)SCHED_RANGE_ATTACK1	},
	//{ TASK_FIND_COVER_FROM_ENEMY,	(float)0					},
	//{ TASK_RUN_PATH,				(float)0					},
	//{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	//{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
	//{ TASK_FACE_ENEMY,				(float)0					},
	//reload!
	{ TASK_RELOAD,					(float)0					},
};

Schedule_t	slAssassinReload[] =
{
	{ 
		tlAssassinReload,
		ARRAYSIZE ( tlAssassinReload ), 
		bits_COND_NEW_ENEMY |
		bits_COND_CAN_MELEE_ATTACK1		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"AssassinReload"
	},
};






DEFINE_CUSTOM_SCHEDULES( CHAssassin )
{
	slAssassinFail,
	slAssassinExposed,
	slAssassinTakeCoverFromEnemy,
	slAssassinTakeCoverFromEnemy2,
	slAssassinTakeCoverFromBestSound,
	slAssassinHide,
	slAssassinHunt,
	slAssassinJump,
	slAssassinJumpAttack,
	slAssassinJumpLand,
	slAssassinTakeCoverReload, //MODDD - new
	slAssassinReload, //MODDD - new
};

IMPLEMENT_CUSTOM_SCHEDULES( CHAssassin, CBaseMonster );






















#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_human_assassin, CHAssassin );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( hassassin, CHAssassin );

	#if EXTRA_NAMES == 2
		LINK_ENTITY_TO_CLASS( human_assassin, CHAssassin );
		LINK_ENTITY_TO_CLASS( monster_hassassin, CHAssassin );
	#endif
	
	//no extras.

#endif



TYPEDESCRIPTION	CHAssassin::m_SaveData[] = 
{
	DEFINE_FIELD( CHAssassin, m_flLastShot, FIELD_TIME ),
	DEFINE_FIELD( CHAssassin, m_flDiviation, FIELD_FLOAT ),

	DEFINE_FIELD( CHAssassin, m_flNextJump, FIELD_TIME ),
	DEFINE_FIELD( CHAssassin, m_vecJumpVelocity, FIELD_VECTOR ),

	DEFINE_FIELD( CHAssassin, m_flNextGrenadeCheck, FIELD_TIME ),
	DEFINE_FIELD( CHAssassin, m_vecTossVelocity, FIELD_VECTOR ),
	DEFINE_FIELD( CHAssassin, m_fThrowGrenade, FIELD_BOOLEAN ),

	DEFINE_FIELD( CHAssassin, m_iTargetRanderamt, FIELD_INTEGER ),
	DEFINE_FIELD( CHAssassin, m_iFrustration, FIELD_INTEGER ),
	DEFINE_FIELD( CHAssassin, droppedWeapon, FIELD_BOOLEAN ),   //MODDD - new. Have I dropped my weapon or not yet? Probably will only
	//                                                            be relevant for in the middle of a death animation. So that gibbing
	//                                                            after dropping a crossbow doesn't make another crossbow show up.

	
};

IMPLEMENT_SAVERESTORE( CHAssassin, CBaseMonster );




//SetTouch(&CChumToad::ChumToadTouch );




//MODDD - although more expensive, it may be best to move this in the MonsterThink method instead,
//or at least work on several seconds after recently touching anything to be safe then.
void CHAssassin::HAssassinTouch( CBaseEntity* pOther ){
	
	//easyForcePrintLine("OH no IM a person friend %s", pOther!=NULL?pOther->getClassname():"WTF");

	//CHEAP FIX:

	if(pOther == NULL){
		return; //??????
	}

	groundTouchCheckDuration = gpGlobals->time + 4;
	

}//END OF HAssassinTouch




void CHAssassin::MonsterThink(){




	//If we touched something a little while ago, check below for the ground for landing.
	if(groundTouchCheckDuration != -1){

		

		if(gpGlobals->time <= groundTouchCheckDuration){
			if(pev->movetype == MOVETYPE_TOSS){
				//MODDD TODO - a better check would be just to see if we've landed on anything that counts as a ground. Do a linetrace downwards and pass if it looks to be the case?
				TraceResult tr;
				UTIL_TraceLine( pev->origin, pev->origin + Vector(0, 0, -7), dont_ignore_monsters, dont_ignore_glass, ENT(pev), &tr);
		
				//if(FClassnameIs(pOther->pev, "worldspawn")){
				//if(tr.pHit != NULL && FClassnameIs(tr.pHit, "worldspawn")){
				if(tr.flFraction < 1.0){
					//hit something? land.
					if(m_pSchedule == slAssassinJump || m_pSchedule == slAssassinJumpAttack){
						//jumping schedules need to reset.
						TaskFail();
						pev->movetype = MOVETYPE_STEP;
						ChangeSchedule ( GetScheduleOfType(SCHED_ASSASSIN_JUMP_LAND) );
					}
				}
			}//END OF MOVETYPE_TOSS check
		}else{
			groundTouchCheckDuration = -1;
		}

	}


	//easyForcePrintLine("HASSASSIN ANIM: ID:%d seq:%d fr:%.2f", monsterID, pev->sequence, pev->frame);

	//easyForcePrintLine("AMMO??? %d", m_cAmmoLoaded);

	if(global_thatWasntPunch == 1 && this->m_fSequenceFinished){

			switch(RANDOM_LONG(0, 20)){
			case 0:
				this->SetSequenceByName("idle2");
			break;
			case 1:
				this->SetSequenceByName("idle2");
			break;
			case 2:
				this->SetSequenceByName("grenadethrow");
			break;
			case 3:
				this->SetSequenceByName("grenadethrow");
			break;
			case 4:
				this->SetSequenceByName("grenadethrow");
			break;
			case 5:
				this->SetSequenceByName("grenadethrow");
			break;
			case 6:
				this->SetSequenceByName("grenadethrow");
			break;
			case 7:
				this->SetSequenceByName("grenadethrow");
			break;
			case 8:
				this->SetSequenceByName("kick");
			break;
			case 9:
				this->SetSequenceByName("kick");
			break;
			case 10:
				this->SetSequenceByName("kick");
			break;
			case 11:
				this->SetSequenceByName("kickshort");
			break;
			case 12:
				this->SetSequenceByName("jump");
			break;
			case 13:
				this->SetSequenceByName("jump");
			break;
			case 14:
				this->SetSequenceByName("fly_up");
			break;
			case 15:
				this->SetSequenceByName("fly_up");
			break;
			case 16:
				this->SetSequenceByName("fly_down");
			break;
			case 17:
				this->SetSequenceByName("fly_down");
			break;
			case 18:
				this->SetSequenceByName("land_from_jump");
			break;
			case 19:
				this->SetSequenceByName("land_from_jump");
			break;
			case 20:
				this->SetSequenceByName("land_from_jump");
			break;
			}

	}


	CBaseMonster::MonsterThink();
}


int CHAssassin::IRelationship ( CBaseEntity *pTarget )
{
	if(global_thatWasntPunch == 1){
		//I just don't give a damn man
		return R_NO;
	}

	return CBaseMonster::IRelationship(pTarget);

}



//=========================================================
// DieSound
//=========================================================
void CHAssassin :: DeathSound ( void )
{
}

//=========================================================
// IdleSound
//=========================================================
void CHAssassin :: IdleSound ( void )
{
}

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. 
//=========================================================
int CHAssassin :: ISoundMask ( void) 
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_DANGER	|
			bits_SOUND_PLAYER	|
			//MODDD - new
			bits_SOUND_BAIT;
}


//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CHAssassin :: Classify ( void )
{
	return	CLASS_HUMAN_MILITARY;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CHAssassin :: SetYawSpeed ( void )
{
	int ys;

	switch ( m_Activity )
	{
	case ACT_TURN_LEFT:
	case ACT_TURN_RIGHT:
		ys = 360;
		break;
	default:			
		ys = 360;
		break;
	}


	ys *= 1.67;  //to compensate for having to turn to evade the player now.

	pev->yaw_speed = ys;
}


//=========================================================
// Shoot
//=========================================================
void CHAssassin :: Shoot ( void )
{
	if (m_hEnemy == NULL)
	{
		return;
	}

	Vector vecShootOrigin = GetGunPosition();
	
	if (m_flLastShot + 16 < gpGlobals->time)
	{
		m_flDiviation = 0.07;
	}
	else
	{
		m_flDiviation -= 0.03;
		if (m_flDiviation < 0.01)
			m_flDiviation = 0.01;
	}
	m_flLastShot = gpGlobals->time;
	

	UTIL_MakeVectors ( pev->angles );
	
	Vector vecShootDir = ShootAtEnemyMod( vecShootOrigin ) + Vector( RANDOM_FLOAT(-m_flDiviation, m_flDiviation), RANDOM_FLOAT(-m_flDiviation, m_flDiviation), RANDOM_FLOAT(-m_flDiviation, m_flDiviation) );
	Vector anglesAim = UTIL_VecToAngles( vecShootDir );



	/*
	Vector	vecShellVelocity = gpGlobals->v_right * RANDOM_FLOAT(40,90) + gpGlobals->v_up * RANDOM_FLOAT(75,200) + gpGlobals->v_forward * RANDOM_FLOAT(-40, 40);
	EjectBrass ( pev->origin + gpGlobals->v_up * 32 + gpGlobals->v_forward * 12, vecShellVelocity, pev->angles.y, m_iShell, TE_BOUNCE_SHELL); 
	FireBullets(1, vecShootOrigin, vecShootDir, Vector( m_flDiviation, m_flDiviation, m_flDiviation ), 2048, BULLET_MONSTER_9MM ); // shoot +-8 degrees

	switch(RANDOM_LONG(0,1))
	{
	case 0:
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/pl_gun1.wav", RANDOM_FLOAT(0.6, 0.8), ATTN_NORM);
		break;
	case 1:
		EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/pl_gun2.wav", RANDOM_FLOAT(0.6, 0.8), ATTN_NORM);
		break;
	}

	pev->effects |= EF_MUZZLEFLASH;
	*/


	
	//TODO: different fire sounds? don't know if we ever want this.
	switch(RANDOM_LONG(0,0)){
	    case 0:
	        EMIT_SOUND(ENT(pev), CHAN_WEAPON, "weapons/xbow_fire1.wav", RANDOM_FLOAT(0.78, 0.94), ATTN_NORM);
	    break;
	}


	CCrossbowBolt *pBolt = CCrossbowBolt::BoltCreate();
	pBolt->pev->origin = vecShootOrigin;
	pBolt->pev->angles = anglesAim;
	pBolt->pev->owner = this->edict();

	if (this->pev->waterlevel == 3)
	{
		pBolt->pev->velocity = vecShootDir * BOLT_WATER_VELOCITY;
		pBolt->pev->speed = BOLT_WATER_VELOCITY;
	}
	else
	{
		pBolt->pev->velocity = vecShootDir * BOLT_AIR_VELOCITY;
		pBolt->pev->speed = BOLT_AIR_VELOCITY;
	}
	pBolt->pev->avelocity.z = 10;



	//already have it: anglesAim
	//Vector angDir = UTIL_VecToAngles( vecShootDir );
	SetBlending( 0, anglesAim.x );

	m_cAmmoLoaded--;

	
	/*
	//we have to stop!
	//tlAssassinTakeCoverReload
	this->m_fSequenceLoops = FALSE;  //hacky hacky!
	TaskFail();
	ChangeSchedule(slAssassinTakeCoverReload);
	this->m_fSequenceLoops = FALSE;  //hacky hacky!
	*/

}


//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//
// Returns number of events handled, 0 if none.
//=========================================================
void CHAssassin :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	//easyForcePrintLine("WHAT THE heck IS THiS stuff event:%d evoff:%d seq:%d fr:%.2f", pEvent->event, shootEventOffset, pev->sequence, pev->frame);

	if(global_thatWasntPunch == 1){
		//Best not to.
		return;
	}

	//c2a3d

	//Not using the glock anymore. And wasn't it a silenced glock anyways? Whatever.
	pev->renderfx |= NOMUZZLEFLASH;

	switch( pEvent->event )
	{
	case ASSASSIN_AE_SHOOT1:
		{

			//Not necessary anymore, remove this var entirely later.
			//if(shootEventOffset == 1){
				//only the 2nd one is ok.
				Shoot( );
			//}
			//shootEventOffset += 1;

			break;
		}
	case ASSASSIN_AE_TOSS1:
		{
			this->m_fSequenceLoops = FALSE;

			UTIL_MakeVectors( pev->angles );
			CGrenade::ShootTimed( pev, pev->origin + gpGlobals->v_forward * 34 + Vector (0, 0, 32), m_vecTossVelocity, 2.0 );

			m_flNextGrenadeCheck = gpGlobals->time + 6;// wait six seconds before even looking again to see if a grenade can be thrown.
			m_fThrowGrenade = FALSE;
			// !!!LATER - when in a group, only try to throw grenade if ordered.
			break;
		}
	case ASSASSIN_AE_JUMP:
		{
			// ALERT( at_console, "jumping");
			UTIL_MakeAimVectors( pev->angles );
			pev->movetype = MOVETYPE_TOSS;
			pev->flags &= ~FL_ONGROUND;
			pev->velocity = m_vecJumpVelocity;
			m_flNextJump = gpGlobals->time + 3.0;
			break;
		}
		//return; ..why was this here?
	default:
		CBaseMonster::HandleAnimEvent( pEvent );
		break;
	}
}

CHAssassin::CHAssassin(){
	reloadApplyTime = -1;
	shootEventOffset = 0;
	groundTouchCheckDuration = -1;
}

//=========================================================
// Spawn
//=========================================================
void CHAssassin :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/hassassin.mdl");
	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	pev->classname = MAKE_STRING("monster_human_assassin");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_RED;
	pev->effects		= 0;
	pev->health			= gSkillData.hassassinHealth;
	m_flFieldOfView		= VIEW_FIELD_WIDE; // indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;
	m_afCapability		= bits_CAP_MELEE_ATTACK1 | bits_CAP_DOORS_GROUP;
	pev->friction		= 1;

	m_HackedGunPos		= Vector( 0, 24, 48 );

	m_iTargetRanderamt	= 20;
	pev->renderamt		= 20;
	pev->rendermode		= kRenderTransTexture;

	MonsterInit();
	

	//come loaded.
	m_cAmmoLoaded = 1;
	droppedWeapon = FALSE;
	SetBodygroup(1, 0);  //holding the crossbow.


	SetTouch( &CHAssassin::HAssassinTouch );

}

extern int global_useSentenceSave;
//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CHAssassin :: Precache()
{
	PRECACHE_MODEL("models/hassassin.mdl");

	global_useSentenceSave = TRUE;
	//PRECACHE_SOUND("weapons/pl_gun1.wav", TRUE);
	//PRECACHE_SOUND("weapons/pl_gun2.wav", TRUE);

	//MODDD - fires using this instead.
	PRECACHE_SOUND("weapons/xbow_fire1.wav");
	PRECACHE_SOUND("weapons/xbow_reload1.wav");


	PRECACHE_SOUND("debris/beamstart1.wav");
	global_useSentenceSave = FALSE;

	m_iShell = PRECACHE_MODEL ("models/shell.mdl");// brass shell
}	
	

//AI SCHEDULES MOVED TO THE TOP OF THIS FILE


//=========================================================
// CheckMeleeAttack1 - jump like crazy if the enemy gets too close. 
//=========================================================
BOOL CHAssassin :: CheckMeleeAttack1 ( float flDot, float flDist )
{

	if(m_hEnemy == NULL){
		return FALSE;
	}

	if ( m_flNextJump < gpGlobals->time && (flDist <= 128 || HasMemory( bits_MEMORY_BADJUMP )) )
	{
		TraceResult	tr;

		Vector vecDest = pev->origin + Vector( RANDOM_FLOAT( -64, 64), RANDOM_FLOAT( -64, 64 ), 160 );

		UTIL_TraceHull( pev->origin + Vector( 0, 0, 36 ), vecDest + Vector( 0, 0, 36 ), dont_ignore_monsters, human_hull, ENT(pev), &tr);

		if ( !tr.fStartSolid && (tr.flFraction >= 1.0 || (tr.pHit != NULL && tr.pHit != m_hEnemy.Get() )) )
		{
			//accetpable.
		}else{
			return FALSE;
		}

		float flGravity = g_psv_gravity->value;

		float time = sqrt( 160 / (0.5 * flGravity));
		float speed = flGravity * time / 160;
		m_vecJumpVelocity = (vecDest - pev->origin) * speed;

		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckRangeAttack1  - drop a cap in their ass
//
//=========================================================
BOOL CHAssassin :: CheckRangeAttack1 ( float flDot, float flDist )
{

	if(m_hEnemy == NULL){
		return FALSE;
	}

	if(m_cAmmoLoaded < 0){
		//you have to reload silly!
		return FALSE;
	}
	
	Vector vecShootOrigin = GetGunPositionAI();

	//NoFriendlyFireImp(vecShootOrigin, m_hEnemy->BodyTargetMod(vecShootOrigin)
	//MODDD - you do too need a dotproduct check!
	//if ( !HasConditions( bits_COND_ENEMY_OCCLUDED ) && flDist > 64 && flDist <= 2048 /* && flDot >= 0.5 */ /* && NoFriendlyFire() */ )
	if ( !HasConditions( bits_COND_ENEMY_OCCLUDED ) && flDist > 64 && flDist <= 2048  && flDot >= 0.5  && NoFriendlyFireImp(vecShootOrigin, m_hEnemy->BodyTargetMod(vecShootOrigin))  )
	{
		TraceResult	tr;

		Vector vecSrc = GetGunPositionAI();

		// verify that a bullet fired from the gun will hit the enemy before the world.
		UTIL_TraceLine( vecSrc, m_hEnemy->BodyTargetMod(vecSrc), dont_ignore_monsters, ENT(pev), &tr);

		if ( !tr.fStartSolid && (tr.flFraction >= 1 || tr.pHit == m_hEnemy->edict()) )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//=========================================================
// CheckRangeAttack2 - toss grenade is enemy gets in the way and is too close. 
//=========================================================
BOOL CHAssassin :: CheckRangeAttack2 ( float flDot, float flDist )
{
	m_fThrowGrenade = FALSE;
	if ( !FBitSet ( m_hEnemy->pev->flags, FL_ONGROUND ) )
	{
		// don't throw grenades at anything that isn't on the ground!
		return FALSE;
	}

	// don't get grenade happy unless the player starts to piss you off
	if ( m_iFrustration <= 2)
		return FALSE;

	if ( m_flNextGrenadeCheck < gpGlobals->time && !HasConditions( bits_COND_ENEMY_OCCLUDED ) && flDist <= 512 /* && flDot >= 0.5 */ /* && NoFriendlyFire() */ )
	{
		Vector vecToss = VecCheckThrow( pev, GetGunPositionAI( ), m_hEnemy->Center(), flDist, 0.5 ); // use dist as speed to get there in 1 second

		if ( vecToss != g_vecZero )
		{
			m_vecTossVelocity = vecToss;

			// throw a hand grenade
			m_fThrowGrenade = TRUE;

			return TRUE;
		}
	}

	return FALSE;
}


//=========================================================
// RunAI
//=========================================================
void CHAssassin :: RunAI( void )
{
	CBaseMonster :: RunAI();

	// always visible if moving
	// always visible is not on hard

	//MODDD - also, this cheat CVar being on means we're definitely visible.
	if (global_thatWasntPunch == 1 || (g_iSkillLevel != SKILL_HARD || m_hEnemy == NULL || pev->deadflag != DEAD_NO || m_Activity == ACT_RUN || m_Activity == ACT_WALK || !(pev->flags & FL_ONGROUND) )  )
		m_iTargetRanderamt = 255;
	else
		m_iTargetRanderamt = 20;

	if (pev->renderamt > m_iTargetRanderamt)
	{
		//
		//easyPrintLine("HA %.2f %.2f", pev->renderamt, m_iTargetRanderamt);

		//VISIBLE, MAKE NOISE TO SIGNAL START OF LOWERING!
		if (pev->renderamt == 255)
		{
			EMIT_SOUND_FILTERED (ENT(pev), CHAN_BODY, "debris/beamstart1.wav", 0.2, ATTN_NORM );
		}

		pev->renderamt = max( pev->renderamt - 50, m_iTargetRanderamt );
		pev->rendermode = kRenderTransTexture;
	}
	else if (pev->renderamt < m_iTargetRanderamt)
	{
		pev->renderamt = min( pev->renderamt + 50, m_iTargetRanderamt );
		if (pev->renderamt == 255)
			pev->rendermode = kRenderNormal;
	}



	/*
	//even ACT_WALK? isn't that a tiny bit obnoxious?
	if (m_Activity == ACT_RUN)  // || m_Activity == ACT_WALK)
	{
		static int iStep = 0;
		iStep = ! iStep;
		if (iStep)
		{
			//MODDD - These are already hard-precached by the player without the soundsentencesave system, regardless of its setting.
			//        May as well take advantage of this.
			switch( RANDOM_LONG( 0, 3 ) )
			{
			case 0:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step1.wav", 0.5, ATTN_NORM, FALSE);	break;
			case 1:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step3.wav", 0.5, ATTN_NORM, FALSE);	break;
			case 2:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step2.wav", 0.5, ATTN_NORM, FALSE);	break;
			case 3:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step4.wav", 0.5, ATTN_NORM, FALSE);	break;
			}
		}
	}
	*/
}


//=========================================================
// StartTask
//=========================================================
void CHAssassin :: StartTask ( Task_t *pTask )
{

	const char* schedName = getScheduleName();
	int taskNumberr = pTask->iTask;
	//easyForcePrintLine("%s:%d StartTask sched:%s task:%d", getClassname(), monsterID, schedName, taskNumberr);

	switch ( pTask->iTask )
	{

	case TASK_RANGE_ATTACK1:
		{
		
			//CBaseMonster :: StartTask ( pTask );

		
			if(m_cAmmoLoaded > 0){
				//MODDD - interesting... why was this sequence ever marked to loop if the hassassin can only fire one shot
				//        before reloading (changing anims)?
				
				if(EASY_CVAR_GET(hassassinCrossbowDebug) != 1){
					m_fSequenceLoops = FALSE;
				}

				shootEventOffset = 0;
				CBaseMonster :: StartTask ( pTask );
			}else{
				//nope? skip this.
				TaskComplete();
			}
		
		break;
		}
	case TASK_RELOAD:
	{

		//this->SetSequenceByName(HASSASSIN_CROSSBOW_RELOAD_ANIM);

		reloadApplyTime = gpGlobals->time + EASY_CVAR_GET(hassassinCrossbowReloadSoundDelay);

		CBaseMonster::StartTask(pTask);

		break;
	}

	case TASK_RANGE_ATTACK2:
		if (!m_fThrowGrenade)
		{
			TaskComplete( );
		}
		else
		{
			CBaseMonster :: StartTask ( pTask );
		}
		break;
	case TASK_ASSASSIN_FALL_TO_GROUND:
		break;
	default:
		CBaseMonster :: StartTask ( pTask );
		break;
	}
}












GENERATE_TRACEATTACK_IMPLEMENTATION(CHAssassin)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CHAssassin)
{
	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}


GENERATE_KILLED_IMPLEMENTATION(CHAssassin){

	//TODO - make this an event during the death animation, or some death animations?
	attemptDropWeapon();
	
	GENERATE_KILLED_PARENT_CALL(CBaseMonster);
}


//=========================================================
// GibMonster - make gun fly through the air.
//=========================================================
GENERATE_GIBMONSTER_IMPLEMENTATION(CHAssassin)
{
	//if ( GetBodygroup( GUN_GROUP ) != GUN_NONE )

	//do we need to check if we even have a crossbow, or always imply it?
	attemptDropWeapon();

	//CBaseMonster::GibMonster(spawnHeadBlock, gibsSpawnDecal);
	GENERATE_GIBMONSTER_PARENT_CALL(CBaseMonster);
}




BOOL CHAssassin ::attemptDropWeapon(void){



	Vector	vecGunPos;
	Vector	vecGunAngles;

	if(!droppedWeapon && (g_iSkillLevel==SKILL_EASY || g_iSkillLevel == SKILL_MEDIUM)  ){
		GetAttachment( 0, vecGunPos, vecGunAngles );

		SetBodygroup(1, 1);  //no longer holding it.

		CBaseEntity *pGun;
		pGun = DropItem( "weapon_crossbow", vecGunPos, vecGunAngles );


		if ( pGun )
		{
			pGun->pev->velocity = Vector (RANDOM_FLOAT(-100,100), RANDOM_FLOAT(-100,100), RANDOM_FLOAT(200,300));
			pGun->pev->avelocity = Vector ( 0, RANDOM_FLOAT( 200, 400 ), 0 );
			droppedWeapon = TRUE;
			return TRUE;  //dropped the weapon.
		}

	}else{


	}



	return FALSE;

}//END OF attemptDropWeapon(...)





//=========================================================
// RunTask 
//=========================================================
void CHAssassin :: RunTask ( Task_t *pTask )
{

	
	//easyForcePrintLine("%s:%d RunTask sched:%s task:%d", getClassname(), monsterID, getScheduleName(), pTask->iTask );


	//easyForcePrintLine("MY stuff %s %d", this->getScheduleName(), pTask->iTask);
	switch ( pTask->iTask )
	{

	case TASK_RANGE_ATTACK1:

		
		if(EASY_CVAR_GET(hassassinCrossbowDebug) != 1){
			if(m_fSequenceFinished == TRUE){
				//done.
				SetActivity(ACT_COMBAT_IDLE);
				//m_IdealActivity = ACT_RESET;
				TaskComplete();
			}
		}else{

			if(m_fSequenceFinished == TRUE){
				//TaskComplete();
				//pev->frame = 0; //??
			}
		}
		
	break;
	case TASK_RANGE_ATTACK2:
		if(m_fSequenceFinished == TRUE){
			//done.
			SetActivity(ACT_COMBAT_IDLE);
			//m_IdealActivity = ACT_RESET;
			TaskComplete();
		}
	break;


		//MODDD - new
	case TASK_RELOAD:
		{
		if(reloadApplyTime != -1 &&  gpGlobals->time >= reloadApplyTime){
			reloadApplyTime = -1;
			m_cAmmoLoaded = 1;  //loaded.
			//notice: not using the sound sentence save. The player precaches this.
			EMIT_SOUND_FILTERED(ENT(this->pev), CHAN_ITEM, "weapons/xbow_reload1.wav", RANDOM_FLOAT(0.85, 0.94), ATTN_NORM, 0, 93 + RANDOM_LONG(0,0xF), FALSE);
			//sound!!
		}
		//are we done?
		/*
		if(m_fSequenceFinished){
			TaskComplete();
		}
		*/

		CBaseMonster::RunTask(pTask);

		break;
		}
	case TASK_ASSASSIN_FALL_TO_GROUND:
		MakeIdealYaw( m_vecEnemyLKP );
		ChangeYaw( pev->yaw_speed );

		if (m_fSequenceFinished)
		{
			if (pev->velocity.z > 0)
			{
				pev->sequence = LookupSequence( "fly_up" );
			}
			else if (HasConditions ( bits_COND_SEE_ENEMY ))
			{
				pev->sequence = LookupSequence( "fly_attack" );
				pev->frame = 0;
			}
			else
			{
				pev->sequence = LookupSequence( "fly_down" );
				pev->frame = 0;
			}
			
			ResetSequenceInfo( );
			SetYawSpeed();
		}
		if (pev->flags & FL_ONGROUND)
		{
			// ALERT( at_console, "on ground\n");
			TaskComplete( );
		}
		break;
	default: 
		CBaseMonster :: RunTask ( pTask );
		break;
	}
}

//=========================================================
// GetSchedule - Decides which type of schedule best suits
// the monster's current state and conditions. Then calls
// monster's member function to get a pointer to a schedule
// of the proper type.
//=========================================================
Schedule_t *CHAssassin :: GetSchedule ( void )
{

	
	//no need for extra bait script, defaults should carry over.


	switch	( m_MonsterState )
	{
	case MONSTERSTATE_IDLE:
	case MONSTERSTATE_ALERT:
		{
			if ( HasConditions ( bits_COND_HEAR_SOUND ))
			{
				CSound *pSound;
				pSound = PBestSound();

				ASSERT( pSound != NULL );
				if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
				{
					return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
				}
				if ( pSound && (pSound->m_iType & bits_SOUND_COMBAT) )
				{
					return GetScheduleOfType( SCHED_INVESTIGATE_SOUND );
				}
			}
		}
		break;

	case MONSTERSTATE_COMBAT:
		{
// dead enemy


			if ( HasConditions( bits_COND_ENEMY_DEAD ) )
			{
				// call base class, all code to handle dead enemies is centralized there.
				return CBaseMonster :: GetSchedule();
			}

			// flying?
			//...unfortunately this is not very reactive. Maybe a touch method that interrupts the schedule on flying and touching the ground would be better.
			//FIXED, see above. There is a gound check done with the var "groundTouchCheckDuration". If anything is touched, ground checks are done every monsterThink frame
			//just in case a wall or the ceiling was touched intead, which should not trigger a landing (a linetrace to below the flying hassassin would find only the air if so).


			if ( pev->movetype == MOVETYPE_TOSS)
			{
				if (pev->flags & FL_ONGROUND)
				{
					// ALERT( at_console, "landed\n");
					// just landed
					pev->movetype = MOVETYPE_STEP;
					return GetScheduleOfType ( SCHED_ASSASSIN_JUMP_LAND );
				}
				else
				{
					// ALERT( at_console, "jump\n");
					// jump or jump/shoot
					if ( m_MonsterState == MONSTERSTATE_COMBAT ){
						return GetScheduleOfType ( SCHED_ASSASSIN_JUMP );
					}else{
						//MODDD NOTE - DERP. This is impossible to reach as we clearly must be in MONSTERSTATE_COMBAT to even be in this case statement at all.
						//This is probably the point.
						return GetScheduleOfType ( SCHED_ASSASSIN_JUMP_ATTACK );
					}
				}
			}






			if ( HasConditions ( bits_COND_HEAR_SOUND ))
			{
				CSound *pSound;
				pSound = PBestSound();

				ASSERT( pSound != NULL );
				if ( pSound && (pSound->m_iType & bits_SOUND_DANGER) )
				{
					return GetScheduleOfType( SCHED_TAKE_COVER_FROM_BEST_SOUND );
				}
			}

			if ( HasConditions ( bits_COND_LIGHT_DAMAGE ) )
			{
				m_iFrustration++;
			}
			if ( HasConditions ( bits_COND_HEAVY_DAMAGE ) )
			{
				m_iFrustration++;
			}



		// jump player!
			if ( HasConditions ( bits_COND_CAN_MELEE_ATTACK1 ) )
			{
				// ALERT( at_console, "melee attack 1\n");
				return GetScheduleOfType ( SCHED_MELEE_ATTACK1 );
			}

		// throw grenade
			if ( HasConditions ( bits_COND_CAN_RANGE_ATTACK2 ) )
			{
				// ALERT( at_console, "range attack 2\n");
				return GetScheduleOfType ( SCHED_RANGE_ATTACK2 );
			}



			if(m_cAmmoLoaded <= 0){
				//can we reload?
				if ( HasConditions( bits_COND_ENEMY_OCCLUDED )) {
					//if occluded or we're in cover, just go ahead and reload.
					return GetScheduleOfType(SCHED_ASSASSIN_RELOAD);
				}else{
					//take cover or try to?
					return GetScheduleOfType(SCHED_ASSASSIN_TAKE_COVER_RELOAD);
				}
			}



		// spotted
			if ( HasConditions ( bits_COND_SEE_ENEMY ) && HasConditions ( bits_COND_ENEMY_FACING_ME ) )
			{
				// ALERT( at_console, "exposed\n");
				m_iFrustration++;
				return GetScheduleOfType ( SCHED_ASSASSIN_EXPOSED );
			}


			








		// can attack
			if ( HasConditions ( bits_COND_CAN_RANGE_ATTACK1 ) )
			{
				// ALERT( at_console, "range attack 1\n");
				m_iFrustration = 0;
				return GetScheduleOfType ( SCHED_RANGE_ATTACK1 );
			}

			if ( HasConditions ( bits_COND_SEE_ENEMY ) )
			{
				// ALERT( at_console, "face\n");
				return GetScheduleOfType ( SCHED_COMBAT_FACE );
			}

		// new enemy
			if ( HasConditions ( bits_COND_NEW_ENEMY ) )
			{
				// ALERT( at_console, "take cover\n");
				return GetScheduleOfType ( SCHED_TAKE_COVER_FROM_ENEMY );
			}

			// ALERT( at_console, "stand\n");


			return GetScheduleOfType ( SCHED_ALERT_STAND );
		}
		break;
	}

	return CBaseMonster :: GetSchedule();
}

//=========================================================
//=========================================================
Schedule_t* CHAssassin :: GetScheduleOfType ( int Type ) 
{
	// ALERT( at_console, "%d\n", m_iFrustration );
	switch	( Type )
	{

		//AssassinTakeCoverFromEnemy2
		//!!!???

	case SCHED_ASSASSIN_RANGE_ATTACK1_CANRELOAD:
		//can attack? go ahead.
		if(m_cAmmoLoaded > 0){
			//MODDD - why was this SCHED_RANGE_ATTACK2 ??? that is for grenades.
			return CBaseMonster :: GetScheduleOfType( SCHED_RANGE_ATTACK1 );
		}else{
			//reload!
			return slAssassinReload;
		}

	break;
	case SCHED_ASSASSIN_RANGE_ATTACK2_CANRELOAD:


		//MODDD - extra check, for m_fThrowGrenade
		if( m_flNextGrenadeCheck < gpGlobals->time && !HasConditions( bits_COND_ENEMY_OCCLUDED ) && m_fThrowGrenade){
			//can throw a grenade probably? just try.
			return CBaseMonster :: GetScheduleOfType( SCHED_RANGE_ATTACK2 );
		}else{
			//too soon since thrown a grenade / enemy is occluded? Just reload.
			if(m_cAmmoLoaded > 0){
				//MODDD - why was this SCHED_RANGE_ATTACK2 ??? that is for grenades.
				return CBaseMonster :: GetScheduleOfType( SCHED_RANGE_ATTACK1 );
			}else{
				//reload!
				return slAssassinReload;
			}
		}

	break;

	case SCHED_RANGE_ATTACK1:

		if(m_cAmmoLoaded > 0){
			//acceptable to attack.
			return CBaseMonster :: GetScheduleOfType( Type );
		}else{
			//you can't attack, so flee!
			return slAssassinTakeCoverFromEnemy2;
		}
	break;
	case SCHED_ASSASSIN_TAKE_COVER_RELOAD:
		return slAssassinTakeCoverReload;
	break;
	case SCHED_ASSASSIN_RELOAD:
		return slAssassinReload;
	break;


	case SCHED_TAKE_COVER_FROM_ENEMY:
		if (pev->health > 30)
			return slAssassinTakeCoverFromEnemy;
		else
			return slAssassinTakeCoverFromEnemy2;
	case SCHED_TAKE_COVER_FROM_BEST_SOUND:
		return slAssassinTakeCoverFromBestSound;
	case SCHED_ASSASSIN_EXPOSED:
		return slAssassinExposed;
	case SCHED_FAIL:
		if (m_MonsterState == MONSTERSTATE_COMBAT)
			return slAssassinFail;
		break;
	case SCHED_ALERT_STAND:
		if (m_MonsterState == MONSTERSTATE_COMBAT)
			return slAssassinHide;
		break;
	case SCHED_CHASE_ENEMY:
		return slAssassinHunt;
	case SCHED_MELEE_ATTACK1:
		if (pev->flags & FL_ONGROUND)
		{
			if (m_flNextJump > gpGlobals->time)
			{
				// can't jump yet, go ahead and fail
				return slAssassinFail;
			}
			else
			{
				return slAssassinJump;
			}
		}
		else
		{
			return slAssassinJumpAttack;
		}
	case SCHED_ASSASSIN_JUMP:
	case SCHED_ASSASSIN_JUMP_ATTACK:
		return slAssassinJumpAttack;
	case SCHED_ASSASSIN_JUMP_LAND:
		return slAssassinJumpLand;
	}

	return CBaseMonster :: GetScheduleOfType( Type );
}








BOOL CHAssassin::canResetBlend0(void){
	return FALSE;
}

BOOL CHAssassin::onResetBlend0(void){
	//add something?
	
	Vector vecDirToEnemy;
	Vector angDir;


	if (HasConditions( bits_COND_SEE_ENEMY))
	{
		vecDirToEnemy = ( ( m_vecEnemyLKP ) - pev->origin );

		//okay?
		vecDirToEnemy = vecDirToEnemy.Normalize();

		angDir = UTIL_VecToAngles( vecDirToEnemy );
		//vecDirToEnemy = vecDirToEnemy.Normalize();
	}
	else
	{
		angDir = pev->angles;
		UTIL_MakeAimVectors( angDir );
		vecDirToEnemy = gpGlobals->v_forward;
	}
	// make angles +-180
	if (angDir.x > 180)
	{
		angDir.x = angDir.x - 360;
	}
	//easyForcePrintLine("YOU GOON %d ::: %.2f", HasConditions( bits_COND_SEE_ENEMY), angDir.x );
	SetBlending( 0, angDir.x );


	return TRUE;
}




Vector CHAssassin::GetGunPosition(void){

	//NOTE - unknown if attachments are a good idea here. Use default behavior instead.
	//return CBaseMonster::GetGunPosition();
	

	Vector vecGunPos;
	Vector vecGunAngles;
	GetAttachment( 0, vecGunPos, vecGunAngles );
	::UTIL_printLineVector("yehhhag", vecGunPos-pev->origin);

	return vecGunPos;
}//END OF GetGunPosition


Vector CHAssassin::GetGunPositionAI(void){
	//NOTE - same as above.
	return CBaseMonster::GetGunPositionAI();


	Vector forward, angle;
	angle = pev->angles;
	
	angle.x = 0; //pitch is not a factor here.
	UTIL_MakeVectorsPrivate( angle, forward, NULL, NULL );

	return pev->origin + Vector( 0, 0, 43 ) + forward * 41;
}//END OF GetGunPositionAI





BOOL CHAssassin::usesAdvancedAnimSystem(void){
	return TRUE;
}


//NOTICE - ACT_COMBAT_IDLE may be fine as it is too.

int CHAssassin::LookupActivityHard(int activity){
	

	pev->framerate = 1;
	resetEventQueue();

	m_iForceLoops = -1;

	m_flFramerateSuggestion = 1;
	switch(activity){
		case ACT_RANGE_ATTACK1:
			
			m_flFramerateSuggestion = 1;
			//m_flFramerateSuggestion = -1;


			if(EASY_CVAR_GET(hassassinCrossbowDebug) != 1){
				m_iForceLoops = 0; //no loopping.
			}

			return LookupSequence("shoot");
		break;
		case ACT_IDLE_ANGRY:
			//MODDD TODO - "Shoot" sequence? inconsistencies with above?  WHAT IS THIS MESS.


			m_flFramerateSuggestion = 0;

			pev->frame = 0;
			this->m_fSequenceFinished = TRUE;  //just to be safe. If this even stays or is necessary at all.


			
			if(EASY_CVAR_GET(hassassinCrossbowDebug) != 1){
				m_iForceLoops = 0; //no loopping.
			}



			return LookupSequence("shoot");
			//return CBaseAnimating::LookupActivity(activity);
		break;
		case ACT_RELOAD:
			//MODDD - placeholder. Update this when we have a real crossbow reload animation!
			return LookupSequence(HASSASSIN_CROSSBOW_RELOAD_ANIM);
		break;

		case ACT_WALK:

			//events.
			this->animEventQueuePush(2.0f / 35.0f, 1);
			this->animEventQueuePush(23.0 / 35.0f, 2);
			this->animEventQueuePush(44.0f / 35.0f, 1);
			this->animEventQueuePush(65.0 / 35.0f, 2);
			return CBaseAnimating::LookupActivity(activity);
		break;
		case ACT_RUN:
			
			this->animEventQueuePush(8.0f / 40.0f, 3);
			this->animEventQueuePush(17.0f / 40.0f, 4);

			return CBaseAnimating::LookupActivity(activity);
		break;
		

	}

	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}




int CHAssassin::tryActivitySubstitute(int activity){

	
	int iRandChoice = 0;
	int iRandWeightChoice = 0;
	
	char* animChoiceString = NULL;
	int* weightsAbs = NULL;
			
	//pev->framerate = 1;
	int maxRandWeight = 30;

	


	switch(activity){
		case ACT_RANGE_ATTACK1:
			return LookupSequence("shoot");
		break;
		case ACT_IDLE_ANGRY:
			return LookupSequence("shoot");
			//return CBaseAnimating::LookupActivity(activity);
		break;
		case ACT_RELOAD:
			//MODDD - placeholder. Update this when we have a real crossbow reload animation!
			return LookupSequence(HASSASSIN_CROSSBOW_RELOAD_ANIM);
		break;

	}//END OF switch


	//not handled by above?  No animations.
	//MODDD TODO - would it be safer for all monsters to call "PARENTCLASS::LookupActivity(activity);" instead of giving up this easily? Verify.
	return CBaseAnimating::LookupActivity(activity);
}







//Handles custom events sent from "LookupActivityHard", which sends events as timed delays along with picking an animation in script.
//So this handles script-provided events, not model ones.
void CHAssassin::HandleEventQueueEvent(int arg_eventID){

	switch(arg_eventID){
	case 0:
	{


	break;
	}
	case 1:
	{
		//right foot, walk
		switch( RANDOM_LONG( 0, 1 ) ){
		case 0:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step1.wav", 0.28, ATTN_NORM, FALSE);	break;
		case 1:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step3.wav", 0.28, ATTN_NORM, FALSE);	break;
		}
	break;
	}
	case 2:
	{
		//left foot, walk
		switch( RANDOM_LONG( 0, 1 ) )
		{
		case 0:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step2.wav", 0.28, ATTN_NORM, FALSE);	break;
		case 1:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step4.wav", 0.28, ATTN_NORM, FALSE);	break;
		}
	break;
	}
	case 3:
	{
		//right foot, run
		switch( RANDOM_LONG( 0, 1 ) ){
		case 0:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step1.wav", 0.5, ATTN_NORM, FALSE);	break;
		case 1:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step3.wav", 0.5, ATTN_NORM, FALSE);	break;
		}
	break;
	}
	case 4:
	{
		//left foot, run
		switch( RANDOM_LONG( 0, 1 ) )
		{
		case 0:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step2.wav", 0.5, ATTN_NORM, FALSE);	break;
		case 1:	EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "player/pl_step4.wav", 0.5, ATTN_NORM, FALSE);	break;
		}
	break;
	}

	

	}//END OF switch


}//END OF HandleEventQueueEvent

















#endif









