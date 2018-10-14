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
// Default behaviors.
//=========================================================
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"defaultai.h"
#include	"soundent.h"
#include	"nodes.h"
#include	"scripted.h"



EASY_CVAR_EXTERN(pathfindStumpedMode)
EASY_CVAR_EXTERN(pathfindStumpedWaitTime)
EASY_CVAR_EXTERN(pathfindStumpedForgetEnemy)

Task_t tlWaitForSequence[] =
{
	{ TASK_WAIT_FOR_SEQUENCEFINISH,	(float)0				},
};
Schedule_t	slWaitForSequence[] =
{
	{ 
		tlWaitForSequence,
		ARRAYSIZE ( tlWaitForSequence ), 
		//BITS_COND... (fill-inable)
		0,
		
		//bis_sound_... (fill-inable)
		0,
		"WaitForSequence"
	},
};






//MODDD - new schedules for everything.
//also, "slInvestigateSound" is interesting...

//=========================================================
// InvestigateSound - sends a monster to the location of the
// sound that was just heard, to check things out. 
//=========================================================
Task_t tlAnimationSmartAndStop[] =
{
	{ TASK_STOP_MOVING,				(float)0				},
	{ TASK_SET_SEQUENCE_BY_NAME,	(float)0				},
};
Schedule_t	slAnimationSmartAndStop[] =
{
	{ 
		tlAnimationSmartAndStop,
		ARRAYSIZE ( tlAnimationSmartAndStop ), 
		//BITS_COND... (fill-inable)
		0,
		
		//bis_sound_... (fill-inable)
		0,
		"AnimationSmartAndStop"
	},
};




Task_t tlAnimationSmart[] =
{
	//{ TASK_STOP_MOVING,				(float)0				},
	{ TASK_SET_SEQUENCE_BY_NAME,	(float)0				},
};
Schedule_t	slAnimationSmart[] =
{
	{ 
		tlAnimationSmart,
		ARRAYSIZE ( tlAnimationSmart ), 
		//BITS_COND... (fill-inable)
		0,
		
		//bis_sound_... (fill-inable)
		0,
		"AnimationSmart"
	},
};


//////////////////////////////////////////////////////////////////////








/*
			MakeIdealYaw( m_vecEnemyLKP );

			ChangeYaw( pev->yaw_speed );

			//easyForcePrintLine("TASK_FACE_ENEMY: %s%d WHAT? yawdif:%.2f yaw_spd:%.2f", this->getClassname(), this->monsterID,    FlYawDiff(), pev->yaw_speed);

			if ( FacingIdeal() )
			{
				TaskComplete();
			}
*/


//=========================================================
// Stumped - a clone of Fail that is interrupted by seeing the enemy. Also a longer wait time in general.
//=========================================================
Task_t	tlPathfindStumped[] =
{

	{ TASK_STOP_MOVING,			0				},
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	//TASK_FACE_IDEAL ?
	
	//MODDD - Is this a good idea or not?!
	//{ TASK_FACE_PREV_LKP,			(float)0	},
	
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT_STUMPED,				(float)0		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slPathfindStumped[] =
{
	{
		tlPathfindStumped,
		ARRAYSIZE ( tlPathfindStumped ),

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

		"PathfindStumped"
	},
};


//=========================================================
// Fail
//=========================================================
Task_t	tlFail[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slFail[] =
{
	{
		tlFail,
		ARRAYSIZE ( tlFail ),
		bits_COND_CAN_ATTACK,
		0,
		"Fail"
	},
};

//=========================================================
//	Idle Schedules
//=========================================================
Task_t	tlIdleStand1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)5		},// repick IDLESTAND every five seconds. gives us a chance to pick an active idle, fidget, etc.
};

Schedule_t	slIdleStand[] =
{
	{ 
		tlIdleStand1,
		ARRAYSIZE ( tlIdleStand1 ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_SEE_FEAR		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE		|
		//MODDD - new
		bits_SOUND_BAIT,
		"IdleStand"
	},
};

Schedule_t	slIdleTrigger[] =
{
	{ 
		tlIdleStand1,
		ARRAYSIZE ( tlIdleStand1 ), 
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE,
		0,
		"Idle Trigger"
	},
};


Task_t	tlIdleWalk1[] =
{
	{ TASK_WALK_PATH,			(float)9999 },
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0	},
};

Schedule_t	slIdleWalk[] =
{
	{ 
		tlIdleWalk1,
		ARRAYSIZE ( tlIdleWalk1 ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND	|
		bits_COND_SMELL_FOOD	|
		bits_COND_SMELL			|
		bits_COND_PROVOKED,

		bits_SOUND_COMBAT		|// sound flags

		bits_SOUND_MEAT			|// scents
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE		|
		//MODDD - new
		bits_SOUND_BAIT,
		"Idle Walk"
	},
};

//=========================================================
// Ambush - monster stands in place and waits for a new 
// enemy, or chance to attack an existing enemy.
//=========================================================
Task_t	tlAmbush[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT_INDEFINITE,		(float)0		},
};

Schedule_t	slAmbush[] =
{
	{ 
		tlAmbush,
		ARRAYSIZE ( tlAmbush ), 
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED,

		0,
		"Ambush"
	},
};

//=========================================================
// ActiveIdle schedule - !!!BUGBUG - if this schedule doesn't
// complete on its own, the monster's HintNode will not be 
// cleared, and the rest of the monster's group will avoid
// that node because they think the group member that was 
// previously interrupted is still using that node to active
// idle.
///=========================================================
Task_t tlActiveIdle[] =
{
	{ TASK_FIND_HINTNODE,			(float)0	},
	{ TASK_GET_PATH_TO_HINTNODE,	(float)0	},
	{ TASK_STORE_LASTPOSITION,		(float)0	},
	{ TASK_WALK_PATH,				(float)0	},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0	},
	{ TASK_FACE_HINTNODE,			(float)0	},
	{ TASK_PLAY_ACTIVE_IDLE,		(float)0	},
	{ TASK_GET_PATH_TO_LASTPOSITION,(float)0	},
	{ TASK_WALK_PATH,				(float)0	},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0	},
	{ TASK_CLEAR_LASTPOSITION,		(float)0	},
	{ TASK_CLEAR_HINTNODE,			(float)0	},
};

Schedule_t slActiveIdle[] =
{
	{
		tlActiveIdle,
		ARRAYSIZE( tlActiveIdle ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED		|
		bits_COND_HEAR_SOUND,

		bits_SOUND_COMBAT		|
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|
		//MODDD - new
		bits_SOUND_BAIT,
		"Active Idle"
	}
};

//=========================================================
//	Wake Schedules
//=========================================================
Task_t tlWakeAngry1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_SOUND_WAKE,			(float)0	},
	{ TASK_FACE_IDEAL,			(float)0	},
};

Schedule_t slWakeAngry[] =
{
	{
		tlWakeAngry1,
		ARRAYSIZE ( tlWakeAngry1 ),
		0,
		0,
		"Wake Angry"
	}
};

//=========================================================
// AlertFace Schedules
//=========================================================
Task_t	tlAlertFace1[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	{ TASK_FACE_IDEAL,				(float)0		},
};

//MODDD MAJOR - ignores damage conditions to guarantee at least looking someplace.
Schedule_t	slAlertFace[] =
{
	{ 
		tlAlertFace1,
		ARRAYSIZE ( tlAlertFace1 ),
		bits_COND_NEW_ENEMY		|
		//MODDD - why wasn't SEE_ENEMY here??
		bits_COND_SEE_ENEMY |

		bits_COND_SEE_FEAR		|
		//bits_COND_LIGHT_DAMAGE	|
		//bits_COND_HEAVY_DAMAGE	|
		bits_COND_PROVOKED |
		bits_COND_SCHEDULE_DONE,   //NEW. To let the monsterstate change immediaetly at the end of finishing this schedule.
		//0,
		//MODDD - now interruptible by bait?
		bits_SOUND_BAIT,
		"Alert Face"
	},
};




//=========================================================
// CombatFace Schedule
//=========================================================
Task_t	tlCombatFaceSound[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	{ TASK_FACE_BEST_SOUND,	(float)0		},
	{ TASK_CHECK_STUMPED,			(float)0		},
};

Schedule_t	slCombatFaceSound[] =
{
	{ 
		tlCombatFaceSound,
		ARRAYSIZE ( tlCombatFaceSound ), 
		bits_COND_CAN_ATTACK			|
		bits_COND_NEW_ENEMY				|
		bits_COND_ENEMY_DEAD,
		0,
		"CombatFace Sound"
	},
};














//=========================================================
// AlertSmallFlinch Schedule - shot, but didn't see attacker,
// flinch then face
//=========================================================
Task_t	tlAlertSmallFlinch[] =
{
	{ TASK_STOP_MOVING,				0						},
	{ TASK_REMEMBER,				(float)bits_MEMORY_FLINCHED },
	{ TASK_SMALL_FLINCH,			(float)0				},
	{ TASK_SET_SCHEDULE,			(float)SCHED_ALERT_FACE	},
};

Schedule_t	slAlertSmallFlinch[] =
{
	{ 
		tlAlertSmallFlinch,
		ARRAYSIZE ( tlAlertSmallFlinch ),
		0,
		0,
		"Alert Small Flinch"
	},
};

//=========================================================
// AlertIdle Schedules
//=========================================================
Task_t	tlAlertStand1[] =
{
	{ TASK_STOP_MOVING,			0						 },
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE			 },
	{ TASK_WAIT,				(float)20				 },
	{ TASK_SUGGEST_STATE,		(float)MONSTERSTATE_IDLE },
};

Schedule_t	slAlertStand[] =
{
	{ 
		tlAlertStand1,
		ARRAYSIZE ( tlAlertStand1 ), 
		bits_COND_NEW_ENEMY				|
		bits_COND_SEE_ENEMY				|
		bits_COND_SEE_FEAR				|
		bits_COND_LIGHT_DAMAGE			|
		bits_COND_HEAVY_DAMAGE			|
		bits_COND_PROVOKED				|
		bits_COND_SMELL					|
		bits_COND_SMELL_FOOD			|
		bits_COND_HEAR_SOUND,

		bits_SOUND_COMBAT		|// sound flags
		bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER		|

		bits_SOUND_MEAT			|// scent flags
		bits_SOUND_CARCASS		|
		bits_SOUND_GARBAGE		|
		//MODDD - new
		bits_SOUND_BAIT,
		"Alert Stand"
	},
};

//=========================================================
// InvestigateSound - sends a monster to the location of the
// sound that was just heard, to check things out. 
//=========================================================
Task_t tlInvestigateSound[] =
{
	{ TASK_STOP_MOVING,				(float)0				},
	{ TASK_STORE_LASTPOSITION,		(float)0				},
	{ TASK_GET_PATH_TO_BESTSOUND,	(float)0				},
	{ TASK_FACE_IDEAL,				(float)0				},
	{ TASK_WALK_PATH,				(float)0				},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0				},
	{ TASK_PLAY_SEQUENCE,			(float)ACT_IDLE			},
	{ TASK_WAIT,					(float)10				},
	{ TASK_GET_PATH_TO_LASTPOSITION,(float)0				},
	{ TASK_WALK_PATH,				(float)0				},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0				},
	{ TASK_CLEAR_LASTPOSITION,		(float)0				},
};

Schedule_t	slInvestigateSound[] =
{
	{ 
		tlInvestigateSound,
		ARRAYSIZE ( tlInvestigateSound ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_SEE_FEAR			|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"InvestigateSound"
	},
};



/*
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CANT_FOLLOW },	// If you fail, bail out of follow

	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
*/



/*
Task_t tlChaseEnemy1[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CHASE_ENEMY_FAILED	},
	{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	{ TASK_RUN_PATH,			(float)0		},
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
};
*/



//MODDD - new. NOTICE: this schedule expects m_vecMoveGoal has been set before being called!
Task_t tlInvestigateSoundBait[] =
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CANT_FOLLOW_BAIT },	// mODDD TODO If you fail, don't care about bait for a bit?
	{ TASK_STOP_MOVING,				(float)0				},
	{ TASK_STORE_LASTPOSITION,		(float)0				},
	{ TASK_GET_PATH_TO_BESTSOUND,	(float)0				},
	{ TASK_FACE_IDEAL,				(float)0				},
	//{ TASK_WALK_PATH,				(float)0				},

	{ TASK_RUN_PATH,				(float)0				},
	//MODDD TODO - require sight too, at least until closer?
	{ TASK_MOVE_TO_POINT_RANGE,		(float)150				},
	
	//look at the bait.
	{ TASK_FACE_POINT,				(float)0				},

	//{ TASK_WAIT_FOR_MOVEMENT,		(float)0				},

	{ TASK_PLAY_SEQUENCE,			(float)ACT_IDLE			},
	/*
	{ TASK_WAIT,					(float)10				},
	{ TASK_GET_PATH_TO_LASTPOSITION,(float)0				},
	{ TASK_WALK_PATH,				(float)0				},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0				},
	{ TASK_CLEAR_LASTPOSITION,		(float)0				},
	*/
};

Schedule_t	slInvestigateSoundBait[] =
{
	{ 
		tlInvestigateSoundBait,
		ARRAYSIZE ( tlInvestigateSoundBait ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_SEE_FEAR			|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_COMBAT		|// sound flags
		//bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER,
		"InvestigateSoundBait"
	},
};


//clone of tlFail.
Task_t	tlCantFollowBait[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT,				(float)2		},
	{ TASK_WAIT_PVS,			(float)0		},
};

Schedule_t	slCantFollowBait[] =
{
	{
		tlCantFollowBait,
		ARRAYSIZE ( tlCantFollowBait ),
		bits_COND_CAN_ATTACK,
		0,
		"slCantFollowBait"
	},
};



//=========================================================
// CombatIdle Schedule
//=========================================================
Task_t	tlCombatStand1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE },
	{ TASK_WAIT_INDEFINITE,		(float)0		},
};

Schedule_t	slCombatStand[] =
{
	{ 
		tlCombatStand1,
		ARRAYSIZE ( tlCombatStand1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_CAN_ATTACK, 
		0,
		"Combat Stand"
	},
};


//NOTICE - it is possible to get a monster pursuing the player to stare at a wall and keep calling this "Combat Face" schedule 
//         if the player is not occluded but the monster is still satisfied just looking at the "last known position".
//         So, little intervention... TASK_CHECK_STUMPED

//=========================================================
// CombatFace Schedule
//=========================================================
Task_t	tlCombatFace1[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	{ TASK_FACE_ENEMY,				(float)0		},
	{ TASK_CHECK_STUMPED,			(float)0		},
};

Schedule_t	slCombatFace[] =
{
	{ 
		tlCombatFace1,
		ARRAYSIZE ( tlCombatFace1 ), 
		bits_COND_CAN_ATTACK			|
		bits_COND_NEW_ENEMY				|
		bits_COND_ENEMY_DEAD,
		0,
		"CombatFace Stumpable"
	},
};




//=========================================================
// CombatFace Schedule
//=========================================================
Task_t	tlCombatFaceNoStump[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	{ TASK_FACE_ENEMY,				(float)0		},
	//{ TASK_CHECK_STUMPED,			(float)0		},
};

Schedule_t	slCombatFaceNoStump[] =
{
	{ 
		tlCombatFaceNoStump,
		ARRAYSIZE ( tlCombatFaceNoStump ), 
		bits_COND_CAN_ATTACK			|
		bits_COND_NEW_ENEMY				|
		bits_COND_ENEMY_DEAD,
		0,
		"CombatFace NOSTUMP"
	},
};







//=========================================================
// Standoff schedule. Used in combat when a monster is 
// hiding in cover or the enemy has moved out of sight. 
// Should we look around in this schedule?
//=========================================================
Task_t	tlStandoff[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_SET_ACTIVITY,			(float)ACT_IDLE				},
	{ TASK_WAIT_FACE_ENEMY,			(float)2					},
};

Schedule_t slStandoff[] = 
{
	{
		tlStandoff,
		ARRAYSIZE ( tlStandoff ),
		bits_COND_CAN_RANGE_ATTACK1		|
		bits_COND_CAN_RANGE_ATTACK2		|
		bits_COND_ENEMY_DEAD			|
		bits_COND_NEW_ENEMY				|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Standoff"
	}
};

//=========================================================
// Arm weapon (draw gun)
//=========================================================
Task_t	tlArmWeapon[] =
{
	{ TASK_STOP_MOVING,		0				},
	{ TASK_PLAY_SEQUENCE,	(float) ACT_ARM }
};

Schedule_t slArmWeapon[] = 
{
	{
		tlArmWeapon,
		ARRAYSIZE ( tlArmWeapon ),
		0,
		0,
		"Arm Weapon"
	}
};

//=========================================================
// reload schedule
//=========================================================
Task_t	tlReload[] =
{
	{ TASK_STOP_MOVING,			0					},
	{ TASK_PLAY_SEQUENCE,		float(ACT_RELOAD)	},
};

Schedule_t slReload[] = 
{
	{
		tlReload,
		ARRAYSIZE ( tlReload ),
		bits_COND_HEAVY_DAMAGE,
		0,
		"Reload"
	}
};

//=========================================================
//	Attack Schedules
//=========================================================

// primary range attack
Task_t	tlRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
};

Schedule_t	slRangeAttack1[] =
{
	{ 
		tlRangeAttack1,
		ARRAYSIZE ( tlRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Range Attack1"
	},
};

// secondary range attack
Task_t	tlRangeAttack2[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_RANGE_ATTACK2,		(float)0		},
};

Schedule_t	slRangeAttack2[] =
{
	{ 
		tlRangeAttack2,
		ARRAYSIZE ( tlRangeAttack2 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Range Attack2"
	},
};


//MODDD - MAJOR. Melee attacks are by default, no longer interruptable by damage conditions,
//as getting interrupted usually doesn't make sense. You have to expect to get hit at close range.

// primary melee attack
Task_t	tlPrimaryMeleeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_MELEE_ATTACK1,		(float)0		},
};

Schedule_t	slPrimaryMeleeAttack[] =
{
	{ 
		tlPrimaryMeleeAttack1,
		ARRAYSIZE ( tlPrimaryMeleeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		//bits_COND_LIGHT_DAMAGE		|
		//bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED,
		0,
		"Primary Melee Attack"
	},
};

// secondary melee attack
Task_t	tlSecondaryMeleeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_MELEE_ATTACK2,		(float)0		},
};

Schedule_t	slSecondaryMeleeAttack[] =
{
	{ 
		tlSecondaryMeleeAttack1,
		ARRAYSIZE ( tlSecondaryMeleeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		//bits_COND_LIGHT_DAMAGE		|
		//bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED,
		0,
		"Secondary Melee Attack"
	},
};

// special attack1
Task_t	tlSpecialAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_SPECIAL_ATTACK1,		(float)0		},
};

Schedule_t	slSpecialAttack1[] =
{
	{ 
		tlSpecialAttack1,
		ARRAYSIZE ( tlSpecialAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Special Attack1"
	},
};

// special attack2
Task_t	tlSpecialAttack2[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_ENEMY,			(float)0		},
	{ TASK_SPECIAL_ATTACK2,		(float)0		},
};

Schedule_t	slSpecialAttack2[] =
{
	{ 
		tlSpecialAttack2,
		ARRAYSIZE ( tlSpecialAttack2 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_LIGHT_DAMAGE		|
		bits_COND_HEAVY_DAMAGE		|
		bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED	|
		bits_COND_HEAR_SOUND,
		
		bits_SOUND_DANGER,
		"Special Attack2"
	},
};

// Chase enemy schedule
Task_t tlChaseEnemy1[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CHASE_ENEMY_FAILED	},
	{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	{ TASK_RUN_PATH,			(float)0		},
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
};

Schedule_t slChaseEnemy[] =
{
	{ 
		tlChaseEnemy1,
		ARRAYSIZE ( tlChaseEnemy1 ),
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
		"Chase Enemy"
	},
};


/*
Task_t	tlBaFollow[] =
{

	//MODDD - the Barney should use his BA_STOP lines.
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CANT_FOLLOW },	// If you fail, bail out of follow

	{ TASK_MOVE_TO_TARGET_RANGE,(float)128		},	// Move within 128 of target ent (client)
	{ TASK_SET_SCHEDULE,		(float)SCHED_TARGET_FACE },
};

Schedule_t	slBaFollow[] =
{
	{
		tlBaFollow,
		ARRAYSIZE ( tlBaFollow ),
		bits_COND_NEW_ENEMY		|
		bits_COND_LIGHT_DAMAGE	|
		bits_COND_HEAVY_DAMAGE	|
		bits_COND_HEAR_SOUND |
		bits_COND_PROVOKED,
		bits_SOUND_DANGER,
		"Follow"
	},
};
*/



#ifdef CHASE_ENEMY_SMART

#else

#endif

//MODDD - Chase enemy schedule, SMART.
Task_t tlChaseEnemySmart[] = 
{
	{ TASK_SET_FAIL_SCHEDULE,	(float)SCHED_CHASE_ENEMY_FAILED	},
	//{ TASK_GET_PATH_TO_ENEMY,	(float)0		},
	//{ TASK_RUN_PATH,			(float)0		},
	{ TASK_MOVE_TO_ENEMY_RANGE,(float)0		},
	{ TASK_CHECK_STUMPED,(float)0			},
};

Schedule_t slChaseEnemySmart[] =
{
	{ 
		tlChaseEnemySmart,
		ARRAYSIZE ( tlChaseEnemySmart ),
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
		"Chase Enemy Smart"
	},
};






/*

case TASK_MOVE_TO_TARGET_RANGE:
		{
			float distance;

			if ( m_hTargetEnt == NULL ){
				easyPrintLine("I HAVE FAILED THE EMPRAH.  ID: %d", this->monsterID);
				TaskFail();
			}else
			{
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
				else if ( distance < 190 && m_movementActivity != ACT_WALK )
					m_movementActivity = ACT_WALK;
				else if ( distance >= 270 && m_movementActivity != ACT_RUN )
					m_movementActivity = ACT_RUN;
			}

			break;
		}
*/






// Chase enemy failure schedule
Task_t	tlChaseEnemyFailed[] =
{
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

Schedule_t	slChaseEnemyFailed[] =
{
	{ 
		tlChaseEnemyFailed,
		ARRAYSIZE ( tlChaseEnemyFailed ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_CAN_RANGE_ATTACK1	|
		bits_COND_CAN_MELEE_ATTACK1	|
		bits_COND_CAN_RANGE_ATTACK2	|
		bits_COND_CAN_MELEE_ATTACK2	|
		bits_COND_HEAR_SOUND,

		bits_SOUND_DANGER,
		"tlChaseEnemyFailed"
	},
};


//=========================================================
// small flinch, played when minor damage is taken.
//=========================================================
Task_t tlSmallFlinch[] =
{
	{ TASK_REMEMBER,			(float)bits_MEMORY_FLINCHED },
	{ TASK_STOP_MOVING,			0	},
	{ TASK_SMALL_FLINCH,		0	},
};

Schedule_t slSmallFlinch[] =
{
	{
		tlSmallFlinch,
		ARRAYSIZE ( tlSmallFlinch ),
		0,
		0,
		"Small Flinch"
	},
};

//=========================================================
// Die!
//=========================================================
Task_t tlDie1[] =
{
	{ TASK_STOP_MOVING,			0				 },
	{ TASK_SOUND_DIE,		(float)0			 },
	{ TASK_DIE,				(float)0			 },
};

Schedule_t slDie[] =
{
	{
		tlDie1,
		ARRAYSIZE( tlDie1 ),
		0,
		0,
		"Die"
	},
};

//=========================================================
// Victory Dance
//=========================================================
Task_t tlVictoryDance[] =
{
	{ TASK_STOP_MOVING,			0							},
	{ TASK_PLAY_SEQUENCE,		(float)ACT_VICTORY_DANCE	},
	{ TASK_WAIT,				(float)0					},
};

Schedule_t slVictoryDance[] =
{
	{
		tlVictoryDance,
		ARRAYSIZE( tlVictoryDance ),
		0,
		0,
		"Victory Dance"
	},
};

//=========================================================
// BarnacleVictimGrab - barnacle tongue just hit the monster,
// so play a hit animation, then play a cycling pull animation
// as the creature is hoisting the monster.
//=========================================================
Task_t	tlBarnacleVictimGrab[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_PLAY_SEQUENCE,	(float)ACT_BARNACLE_HIT	 },
	{ TASK_SET_ACTIVITY,	(float)ACT_BARNACLE_PULL },
	{ TASK_WAIT_INDEFINITE,	(float)0				 },// just cycle barnacle pull anim while barnacle hoists. 
};

Schedule_t slBarnacleVictimGrab[] =
{
	{
		tlBarnacleVictimGrab,
		ARRAYSIZE ( tlBarnacleVictimGrab ),
		0,
		0,
		"Barnacle Victim"
	}
};

//=========================================================
// BarnacleVictimChomp - barnacle has pulled the prey to its
// mouth. Victim should play the BARNCLE_CHOMP animation 
// once, then loop the BARNACLE_CHEW animation indefinitely
//=========================================================
Task_t	tlBarnacleVictimChomp[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_PLAY_SEQUENCE,	(float)ACT_BARNACLE_CHOMP },
	{ TASK_SET_ACTIVITY,	(float)ACT_BARNACLE_CHEW  },
	{ TASK_WAIT_INDEFINITE,	(float)0				  },// just cycle barnacle pull anim while barnacle hoists. 
};

Schedule_t slBarnacleVictimChomp[] =
{
	{
		tlBarnacleVictimChomp,
		ARRAYSIZE ( tlBarnacleVictimChomp ),
		0,
		0,
		"Barnacle Chomp"
	}
};


//	Universal Error Schedule
Task_t	tlError[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_WAIT_INDEFINITE,				(float)0 },
};

Schedule_t	slError[] =
{
	{ 
		tlError,
		ARRAYSIZE ( tlError ), 
		0,
		0,
		"Error"
	},
};

Task_t tlScriptedWalk[] = 
{
	{ TASK_WALK_TO_TARGET,		(float)TARGET_MOVE_SCRIPTED },
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	{ TASK_PLANT_ON_SCRIPT,		(float)0		},
	{ TASK_FACE_SCRIPT,			(float)0		},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_ENABLE_SCRIPT,		(float)0		},
	{ TASK_WAIT_FOR_SCRIPT,		(float)0		},
	{ TASK_PLAY_SCRIPT,			(float)0		},
};

Schedule_t slWalkToScript[] =
{
	{ 
		tlScriptedWalk,
		ARRAYSIZE ( tlScriptedWalk ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"WalkToScript"
	},
};


Task_t tlScriptedRun[] = 
{
	{ TASK_RUN_TO_TARGET,		(float)TARGET_MOVE_SCRIPTED },
	{ TASK_WAIT_FOR_MOVEMENT,	(float)0		},
	{ TASK_PLANT_ON_SCRIPT,		(float)0		},
	{ TASK_FACE_SCRIPT,			(float)0		},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_ENABLE_SCRIPT,		(float)0		},
	{ TASK_WAIT_FOR_SCRIPT,		(float)0		},
	{ TASK_PLAY_SCRIPT,			(float)0		},
};

Schedule_t slRunToScript[] =
{
	{ 
		tlScriptedRun,
		ARRAYSIZE ( tlScriptedRun ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"RunToScript"
	},
};

Task_t tlScriptedWait[] = 
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_WAIT_FOR_SCRIPT,		(float)0		},
	{ TASK_PLAY_SCRIPT,			(float)0		},
};

Schedule_t slWaitScript[] =
{
	{ 
		tlScriptedWait,
		ARRAYSIZE ( tlScriptedWait ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"WaitForScript"
	},
};

Task_t tlScriptedFace[] = 
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_SCRIPT,			(float)0		},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_WAIT_FOR_SCRIPT,		(float)0		},
	{ TASK_PLAY_SCRIPT,			(float)0		},
};

Schedule_t slFaceScript[] =
{
	{ 
		tlScriptedFace,
		ARRAYSIZE ( tlScriptedFace ),
		SCRIPT_BREAK_CONDITIONS,
		0,
		"FaceScript"
	},
};

//=========================================================
// Cower - this is what is usually done when attempts
// to escape danger fail.
//=========================================================
Task_t	tlCower[] =
{
	{ TASK_STOP_MOVING,			0					},
	{ TASK_PLAY_SEQUENCE,		(float)ACT_COWER	},
};

Schedule_t	slCower[] =
{
	{
		tlCower,
		ARRAYSIZE ( tlCower ),
		0,
		0,
		"Cower"
	},
};

//=========================================================
// move away from where you're currently standing. 
//=========================================================
Task_t	tlTakeCoverFromOrigin[] =
{
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_ORIGIN,		(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slTakeCoverFromOrigin[] =
{
	{ 
		tlTakeCoverFromOrigin,
		ARRAYSIZE ( tlTakeCoverFromOrigin ), 
		bits_COND_NEW_ENEMY,
		0,
		"TakeCoverFromOrigin"
	},
};





//MODDD - NEW!!!
//TODO: make more interruptable.
//=========================================================
// move away from where you're currently standing. But just walk to get there, not in a hurry. Just bored.
//=========================================================
Task_t	tlWalkAwayFromOrigin[] =
{
	{ TASK_STOP_MOVING,					(float)0					},
	//{ TASK_WAIT,						(float)2					},
	{ TASK_FIND_COVER_FROM_ORIGIN,		(float)0					},
	{ TASK_WALK_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	//{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	//{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slWalkAwayFromOrigin[] =
{
	{ 
		tlWalkAwayFromOrigin,
		ARRAYSIZE ( tlWalkAwayFromOrigin ), 
		bits_COND_NEW_ENEMY |
        bits_COND_SEE_ENEMY |
        bits_COND_SEE_DISLIKE |
        bits_COND_SEE_HATE |
        bits_COND_SEE_NEMESIS | 
        bits_COND_SEE_FEAR |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT		|// sound flags
		//bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER,
		"WalkAwayFromOrigin"
	},
};









Task_t	tlRandomWander[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_RANDOMWANDER_CHECKSEEKSHORT,			0		},
	{ TASK_FACE_IDEAL,				(float)0				},
	{ TASK_WALK_PATH,				(float)0				},
	{ TASK_RANDOMWANDER_TEST,			0			},
	{TASK_WAIT_FOR_MOVEMENT, 0},
	//{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	//{ TASK_WAIT_PVS,				0				},
};
Schedule_t	slRandomWander[] =
{
	{
		tlRandomWander,
		ARRAYSIZE ( tlRandomWander ),
		bits_COND_NEW_ENEMY |
        bits_COND_SEE_ENEMY |
        bits_COND_SEE_HATE |
        bits_COND_SEE_FEAR |
		bits_COND_LIGHT_DAMAGE |
		bits_COND_HEAVY_DAMAGE |
		bits_COND_HEAR_SOUND,
		bits_SOUND_COMBAT		|// sound flags
		//bits_SOUND_WORLD		|
		bits_SOUND_PLAYER		|
		bits_SOUND_DANGER,
		"slRandomWander"
	},
};





Task_t	tlRandomWanderUninterruptable[] =
{
	{ TASK_STOP_MOVING,				0				},
	{ TASK_RANDOMWANDER_CHECKSEEKSHORT,			0		},
	{ TASK_FACE_IDEAL,				(float)0				},
	{ TASK_WALK_PATH,				(float)0				},
	{ TASK_RANDOMWANDER_TEST,			0			},
	{TASK_WAIT_FOR_MOVEMENT, 0},
	//{ TASK_SET_ACTIVITY,			(float)ACT_IDLE	},
	//{ TASK_WAIT_PVS,				0				},
};
Schedule_t	slRandomWanderUninterruptable[] =
{
	{
		tlRandomWanderUninterruptable,
		ARRAYSIZE ( tlRandomWanderUninterruptable ),
		0,
		0,
		"slRandomWanderUninterruptable"
	},
};












//=========================================================
// hide from the loudest sound source
//=========================================================
Task_t	tlTakeCoverFromBestSound[] =
{
	{ TASK_STOP_MOVING,					(float)0					},
	{ TASK_FIND_COVER_FROM_BEST_SOUND,	(float)0					},
	{ TASK_RUN_PATH,					(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,			(float)0					},
	{ TASK_REMEMBER,					(float)bits_MEMORY_INCOVER	},
	{ TASK_TURN_LEFT,					(float)179					},
};

Schedule_t	slTakeCoverFromBestSound[] =
{
	{ 
		tlTakeCoverFromBestSound,
		ARRAYSIZE ( tlTakeCoverFromBestSound ), 
		bits_COND_NEW_ENEMY,
		0,
		"TakeCoverFromBestSound"
	},
};

//=========================================================
// Take cover from enemy! Tries lateral cover before node 
// cover! 
//=========================================================
Task_t	tlTakeCoverFromEnemy[] =
{
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

Schedule_t	slTakeCoverFromEnemy[] =
{
	{ 
		tlTakeCoverFromEnemy,
		ARRAYSIZE ( tlTakeCoverFromEnemy ), 
		bits_COND_NEW_ENEMY,
		0,
		"tlTakeCoverFromEnemy"
	},
};




//MODDD - same as above, but chase the enemy if unable to take cover.
Task_t	tlTakeCoverFromEnemyOrChase[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_FIND_COVER_FROM_ENEMY_OR_CHASE,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
//	{ TASK_TURN_LEFT,				(float)179					},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_WAIT,					(float)1					},
};

Schedule_t	slTakeCoverFromEnemyOrChase[] =
{
	{ 
		tlTakeCoverFromEnemyOrChase,
		ARRAYSIZE ( tlTakeCoverFromEnemyOrChase ), 
		bits_COND_NEW_ENEMY,
		0,
		"tlTakeCoverFromEnemyOrChase"
	},
};






Task_t	tlFightOrFlight[] =
{
	{ TASK_STOP_MOVING,				(float)0					},
	{ TASK_WAIT,					(float)0.2					},
	{ TASK_FIND_COVER_FROM_ENEMY_OR_FIGHT,	(float)0					},
	{ TASK_RUN_PATH,				(float)0					},
	{ TASK_WAIT_FOR_MOVEMENT,		(float)0					},
	{ TASK_REMEMBER,				(float)bits_MEMORY_INCOVER	},
//	{ TASK_TURN_LEFT,				(float)179					},
	{ TASK_FACE_ENEMY,				(float)0					},
	{ TASK_WAIT,					(float)1					},
};

Schedule_t	slFightOrFlight[] =
{
	{ 
		tlFightOrFlight,
		ARRAYSIZE ( tlFightOrFlight ), 
		bits_COND_NEW_ENEMY,
		0,
		"slFightOrFlight"
	},
};





Schedule_t *CBaseMonster::m_scheduleList[] = 
{
	slIdleStand,
	slIdleTrigger,
	slIdleWalk,
	slAmbush,
	slActiveIdle,
	slWakeAngry,
	slAlertFace,
	slAlertSmallFlinch,
	slAlertStand,
	slInvestigateSound,
	slCombatStand,
	slCombatFace,
	slCombatFaceNoStump,
	slCombatFaceSound,
	slStandoff,
	slArmWeapon,
	slReload,
	slRangeAttack1,
	slRangeAttack2,
	slPrimaryMeleeAttack,
	slSecondaryMeleeAttack,
	slSpecialAttack1,
	slSpecialAttack2,
	slChaseEnemy,
	slChaseEnemySmart,
	slChaseEnemyFailed,
	slSmallFlinch,
	slDie,
	slVictoryDance,
	slBarnacleVictimGrab,
	slBarnacleVictimChomp,
	slError,
	slWalkToScript,
	slRunToScript,
	slWaitScript,
	slFaceScript,
	slCower,
	slTakeCoverFromOrigin,
	slTakeCoverFromBestSound,
	slTakeCoverFromEnemy,
	slFail,

	//MODDD - additions
	slWaitForSequence,
	slAnimationSmart,
	slAnimationSmartAndStop,
	slInvestigateSoundBait,
	slCantFollowBait,
	slWalkAwayFromOrigin,
	slRandomWander,
	slRandomWanderUninterruptable,
	slTakeCoverFromEnemyOrChase,
	slPathfindStumped,
	slFightOrFlight,

};

Schedule_t *CBaseMonster::ScheduleFromName( const char *pName )
{
	return ScheduleInList( pName, m_scheduleList, ARRAYSIZE(m_scheduleList) );
}


Schedule_t *CBaseMonster :: ScheduleInList( const char *pName, Schedule_t **pList, int listCount )
{
	int i;
	
	if ( !pName )
	{
		ALERT( at_console, "%s set to unnamed schedule!\n", STRING(pev->classname) );
		return NULL;
	}


	for ( i = 0; i < listCount; i++ )
	{
		if ( !pList[i]->pName )
		{
			ALERT( at_console, "Unnamed schedule!\n" );
			continue;
		}
		if ( stricmp( pName, pList[i]->pName ) == 0 )
			return pList[i];
	}
	return NULL;
}

//=========================================================
// GetScheduleOfType - returns a pointer to one of the 
// monster's available schedules of the indicated type.
//=========================================================
Schedule_t* CBaseMonster :: GetScheduleOfType ( int Type ) 
{



	if(Type == SCHED_RANGE_ATTACK2){
		int x = 666;
	}

//	ALERT ( at_console, "Sched Type:%d\n", Type );
	switch	( Type )
	{
	// This is the schedule for scripted sequences AND scripted AI
	case SCHED_AISCRIPT:
		{
			ASSERT( m_pCine != NULL );
			if ( !m_pCine )
			{
				ALERT( at_aiconsole, "Script failed for %s\n", STRING(pev->classname) );
				CineCleanup();
				return GetScheduleOfType( SCHED_IDLE_STAND );
			}
//			else
//				ALERT( at_aiconsole, "Starting script %s for %s\n", STRING( m_pCine->m_iszPlay ), STRING(pev->classname) );

			//YOU ARE 1
			//easyForcePrintLine("YOUR PUNK ASS SAYZ %d ::: %d", monsterID, m_pCine->m_fMoveTo);
			//return GetScheduleOfType( SCHED_IDLE_STAND );

			switch ( m_pCine->m_fMoveTo )
			{
				case 0: 
				case 4: 
					return slWaitScript;
				case 1: 
					return slWalkToScript;
				case 2: 
					return slRunToScript;
				case 5:
					return slFaceScript;
			}
			break;
		}
	case SCHED_IDLE_STAND:
		{
			if ( RANDOM_LONG(0,14) == 0 && FCanActiveIdle() )
			{
				return &slActiveIdle[ 0 ];
			}

			return &slIdleStand[ 0 ];
		}
	case SCHED_IDLE_WALK:
		{
			return &slIdleWalk[ 0 ];
		}
	case SCHED_WAIT_TRIGGER:
		{
			return &slIdleTrigger[ 0 ];
		}
	case SCHED_WAKE_ANGRY:
		{
			return &slWakeAngry[ 0 ];
		}
	case SCHED_ALERT_FACE:
		{
			return &slAlertFace[ 0 ];
		}
	case SCHED_ALERT_STAND:
		{
			return &slAlertStand[ 0 ];
		}
	case SCHED_COMBAT_STAND:
		{
			return &slCombatStand[ 0 ];
		}
	case SCHED_COMBAT_FACE:
		{
			//MODDD - CRITICAL TEST
			return &slCombatFace[ 0 ];
			//return &slCombatFaceSound[0];
		}
	case SCHED_CHASE_ENEMY:
		{
			//MODDD - see what happens.
			//return &slChaseEnemy[ 0 ];
			return &slChaseEnemySmart[0];
		}
	case SCHED_CHASE_ENEMY_SMART:
		{
			return &slChaseEnemySmart[ 0 ];
		}
	case SCHED_CHASE_ENEMY_FAILED:
		{
			//MODDD NOTE - nothing special? damn.
			//ACTUALLY YES, force an update on the enemy LKP just to unclog things maybe.

			if(m_hEnemy != NULL){
				this->m_vecEnemyLKP = m_hEnemy->pev->origin;
			}

			return &slFail[ 0 ];
		}
	case SCHED_SMALL_FLINCH:
		{
			return &slSmallFlinch[ 0 ];
		}
	case SCHED_ALERT_SMALL_FLINCH:
		{
			return &slAlertSmallFlinch[ 0 ];
		}
	case SCHED_RELOAD:
		{
			return &slReload[ 0 ];
		}
	case SCHED_ARM_WEAPON:
		{
			return &slArmWeapon[ 0 ];
		}
	case SCHED_STANDOFF:
		{
			return &slStandoff[ 0 ];
		}
	case SCHED_RANGE_ATTACK1:
		{
			return &slRangeAttack1[ 0 ];
		}
	case SCHED_RANGE_ATTACK2:
		{
			return &slRangeAttack2[ 0 ];
		}
	case SCHED_MELEE_ATTACK1:
		{
			return &slPrimaryMeleeAttack[ 0 ];
		}
	case SCHED_MELEE_ATTACK2:
		{
			return &slSecondaryMeleeAttack[ 0 ];
		}
	case SCHED_SPECIAL_ATTACK1:
		{
			return &slSpecialAttack1[ 0 ];
		}
	case SCHED_SPECIAL_ATTACK2:
		{
			return &slSpecialAttack2[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_BEST_SOUND:
		{
			return &slTakeCoverFromBestSound[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_ENEMY:
		{
			return &slTakeCoverFromEnemy[ 0 ];
		}
	case SCHED_COWER:
		{
			return &slCower[ 0 ];
		}
	case SCHED_AMBUSH:
		{
			return &slAmbush[ 0 ];
		}
	case SCHED_BARNACLE_VICTIM_GRAB:
		{
			return &slBarnacleVictimGrab[ 0 ];
		}
	case SCHED_BARNACLE_VICTIM_CHOMP:
		{
			return &slBarnacleVictimChomp[ 0 ];
		}
	case SCHED_INVESTIGATE_SOUND:
		{
			return &slInvestigateSound[ 0 ];
		}
	//MODDD - new
	case SCHED_INVESTIGATE_SOUND_BAIT:
	{
		return &slInvestigateSoundBait[ 0 ];
	}
	case SCHED_CANT_FOLLOW_BAIT:
	{
		easyForcePrintLine("YOU STUPID guy");
		return &slCantFollowBait[ 0];
	}

	case SCHED_DIE:
		{
			return &slDie[ 0 ];
		}
	case SCHED_TAKE_COVER_FROM_ORIGIN:
		{
			return &slTakeCoverFromOrigin[ 0 ];
		}
		//MODDD - NEW
	case SCHED_WALK_AWAY_FROM_ORIGIN:
		{
			return &slWalkAwayFromOrigin[0];
		}
	case SCHED_RANDOMWANDER:
		return &slRandomWander[0];
	break;
	case SCHED_RANDOMWANDER_UNINTERRUPTABLE:
		return &slRandomWanderUninterruptable[0];
	break;
	case SCHED_TAKE_COVER_FROM_ENEMY_OR_CHASE:
		return &slTakeCoverFromEnemyOrChase[0];
	break;
	case SCHED_FIGHT_OR_FLIGHT:
		return &slFightOrFlight[0];
	break;
	case SCHED_VICTORY_DANCE:
		{
			return &slVictoryDance[ 0 ];
		}
	case SCHED_PATHFIND_STUMPED:
		return &slPathfindStumped[0];
	break;
	case SCHED_FAIL:
		{
			return slFail;
		}
	default:
		{
			ALERT ( at_console, "GetScheduleOfType()\nNo CASE for Schedule Type %d!\n", Type );

			return &slIdleStand[ 0 ];
			break;
		}
	}

	return NULL;
}


void CBaseMonster::ScheduleChange(void){

	Schedule_t* endedSchedule = m_pSchedule;

	if(endedSchedule == GetStumpedWaitSchedule() ){

		if(EASY_CVAR_GET(pathfindStumpedMode) == 2){
			//reget this to keep it up to date!
			if(m_hEnemy != NULL){
				m_vecEnemyLKP = m_hEnemy->pev->origin;
			}
		}
		
		if(EASY_CVAR_GET(pathfindStumpedForgetEnemy) == 1){
			//after going to this LKP, we are unable to re-query the enemy's position out of nowhere.
			//TODO, distant future... pick a random location throughout the map to wander too up to so much of a distance in a search?
			m_hEnemy = NULL;
		}
	}

	
}//END OF ScheduleChange


Schedule_t* CBaseMonster::GetStumpedWaitSchedule(){
	//default.
	return slPathfindStumped;
}//END OF GetStumpedWaitSchedule

