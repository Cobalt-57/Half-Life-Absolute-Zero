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
// monsterstate.cpp - base class monster functions for 
// controlling core AI.
//=========================================================

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "nodes.h"
#include "basemonster.h"
#include "animation.h"
#include "saverestore.h"
#include "soundent.h"

extern float global_crazyMonsterPrintouts;

//=========================================================
// SetState
//=========================================================
void CBaseMonster :: SetState ( MONSTERSTATE State )
{
/*
	if ( State != m_MonsterState )
	{
		ALERT ( at_aiconsole, "State Changed to %d\n", State );
	}
*/


	//MODDD - force the state to stay "PRONE" if caught by a barnacle!
	
	if(barnacleLocked == TRUE){
		//easyPrintLine("LOCKED: %d", State);

		switch( State )
		{
		case MONSTERSTATE_PRONE:
			m_MonsterState = State;
			m_IdealMonsterState = State;
		break;
		default:
			queuedMonsterState = State;
		break;
		}

		return;
	}
	
	
	switch( State )
	{
	
	// Drop enemy pointers when going to idle
	case MONSTERSTATE_IDLE:

		if ( m_hEnemy != NULL )
		{
			m_hEnemy = NULL;// not allowed to have an enemy anymore.
			ALERT ( at_aiconsole, "Stripped\n" );
		}
		break;
	}

	m_MonsterState = State;
	m_IdealMonsterState = State;
}

//=========================================================
// RunAI
//=========================================================
void CBaseMonster :: RunAI ( void )
{


	//these conditions are reset each frame for safety now that schedules rememeber conditions after being set.
	//this->m_afConditions &= ~(bits_COND_NEW_ENEMY);
	

	//MODDD TODO - this might still be flimsy? and why don't bits_COND_LIGHT_DAMAGE and HEAVY_DAMAGE (or LIGHT if that's all that's used) get remembered? takedamage calls
	//occur earlier than this perhaps and get overwritten here and so never seen by scheduling logic below?
	
	
	//m_afConditions &= (bits_COND_TASK_FAILED | bits_COND_SCHEDULE_DONE | bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE | bits_COND_SEE_ENEMY | bits_COND_ENEMY_OCCLUDED);
	//m_afConditions = 0;


	//MODDD - new. This mostly copy of the conditions will be maintained throughout this frame.
	m_afConditionsFrame = 0;


	// to test model's eye height
	//UTIL_ParticleEffect ( pev->origin + pev->view_ofs, g_vecZero, 255, 10 );

	// IDLE sound permitted in ALERT state is because monsters were silent in ALERT state. Only play IDLE sound in IDLE state
	// once we have sounds for that state.
	//MODDD - extra condition. Must also have a deadflag of NO.
	if ( pev->deadflag == DEAD_NO &&   ( m_MonsterState == MONSTERSTATE_IDLE || m_MonsterState == MONSTERSTATE_ALERT ) && RANDOM_LONG(0,99) == 0 && !(pev->flags & SF_MONSTER_GAG) )
	{
		IdleSound();
	}


	if ( m_MonsterState != MONSTERSTATE_NONE	&& 
		 m_MonsterState != MONSTERSTATE_PRONE   && 
		 m_MonsterState != MONSTERSTATE_DEAD )// don't bother with this crap if monster is prone. 
	{

		// collect some sensory Condition information.
		// don't let monsters outside of the player's PVS act up, or most of the interesting
		// things will happen before the player gets there!
		// UPDATE: We now let COMBAT state monsters think and act fully outside of player PVS. This allows the player to leave 
		// an area where monsters are fighting, and the fight will continue.
		
		if(global_crazyMonsterPrintouts){
			easyPrintLine("%s: Can I look and listen?! (%d || %d) %.2f", this->getClassname(), !FNullEnt( FIND_CLIENT_IN_PVS( edict() ) ),  ( m_MonsterState == MONSTERSTATE_COMBAT ), m_flDistLook );
		}

		//MODDD - some monsters may need to check for enemies and ignore the PVS check.
		//        The archer is unable to detect a client and look for enemies if the player is past the waterlevel, strangely.
		//        This is not good for monsters meant to rise to the top and do attacks from water to land. The player should not
		//        have to have been in the water to initate this.  It's the point of being able to see through water (another addition)
		//if ( !FNullEnt( FIND_CLIENT_IN_PVS( edict() ) ) || ( m_MonsterState == MONSTERSTATE_COMBAT ) )
		if ( (noncombat_Look_ignores_PVS_check() || !FNullEnt( FIND_CLIENT_IN_PVS( edict() ) )) || ( m_MonsterState == MONSTERSTATE_COMBAT ) )
		{
			Look( m_flDistLook );
			Listen();// check for audible sounds. 

			// now filter conditions.
			ClearConditions( IgnoreConditions() );

			GetEnemy();
		}

		// do these calculations if monster has an enemy.
		if ( m_hEnemy != NULL )
		{
			CheckEnemy( m_hEnemy );
		}

		CheckAmmo();
	}
	



	FCheckAITrigger();
	
	PrescheduleThink();

	if(global_crazyMonsterPrintouts == 1){
		easyPrintLine("SHUUUT1 %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
	
	//     : )
	MaintainSchedule();

	if(global_crazyMonsterPrintouts == 1){
		easyPrintLine("SHUUUT2 %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}

	// if the monster didn't use these conditions during the above call to MaintainSchedule() or CheckAITrigger()
	// we throw them out cause we don't want them sitting around through the lifespan of a schedule
	// that doesn't use them.
	m_afConditions &= ~( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE );



	
	if(global_crazyMonsterPrintouts == 1){
		easyPrintLine("SHUUUT3 %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
	
	if(global_crazyMonsterPrintouts == 1){
		easyPrintLine("IM SO flamin\' amazing %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
}

//=========================================================
// GetIdealState - surveys the Conditions information available
// and finds the best new state for a monster.
//=========================================================
MONSTERSTATE CBaseMonster :: GetIdealState ( void )
{
	int	iConditions;

	iConditions = IScheduleFlags();
	
	// If no schedule conditions, the new ideal state is probably the reason we're in here.
	switch ( m_MonsterState )
	{
	case MONSTERSTATE_IDLE:
		
		/*
		IDLE goes to ALERT upon hearing a sound
		-IDLE goes to ALERT upon being injured
		IDLE goes to ALERT upon seeing food
		-IDLE goes to COMBAT upon sighting an enemy
		IDLE goes to HUNT upon smelling food
		*/
		{
			if ( iConditions & bits_COND_NEW_ENEMY )			
			{
				// new enemy! This means an idle monster has seen someone it dislikes, or 
				// that a monster in combat has found a more suitable target to attack
				m_IdealMonsterState = MONSTERSTATE_COMBAT;
			}
			else if ( iConditions & bits_COND_LIGHT_DAMAGE )
			{
				MakeIdealYaw ( m_vecEnemyLKP );
				m_IdealMonsterState = MONSTERSTATE_ALERT;
			}
			else if ( iConditions & bits_COND_HEAVY_DAMAGE )
			{
				MakeIdealYaw ( m_vecEnemyLKP );
				m_IdealMonsterState = MONSTERSTATE_ALERT;
			}
			else if ( iConditions & bits_COND_HEAR_SOUND )
			{
				CSound *pSound;
				
				pSound = PBestSound();
				ASSERT( pSound != NULL );
				if ( pSound )
				{
					MakeIdealYaw ( pSound->m_vecOrigin );
					if ( pSound->m_iType & (bits_SOUND_COMBAT|bits_SOUND_DANGER) )
						m_IdealMonsterState = MONSTERSTATE_ALERT;
				}
			}
			else if ( iConditions & (bits_COND_SMELL | bits_COND_SMELL_FOOD) )
			{
				m_IdealMonsterState = MONSTERSTATE_ALERT;
			}

			break;
		}
	case MONSTERSTATE_ALERT:
		/*
		ALERT goes to IDLE upon becoming bored
		-ALERT goes to COMBAT upon sighting an enemy
		ALERT goes to HUNT upon hearing a noise
		*/
		{

			/*
			if(FClassnameIs(pev, "monster_scientist")){
				easyForcePrintLine("hey ya. %d %d", iConditions & (bits_COND_NEW_ENEMY), iConditions & (bits_COND_SEE_ENEMY) );
			}
			*/

			if ( iConditions & (bits_COND_NEW_ENEMY|bits_COND_SEE_ENEMY) )			
			{
				// see an enemy we MUST attack
				m_IdealMonsterState = MONSTERSTATE_COMBAT;
			}
			else if ( iConditions & bits_COND_HEAR_SOUND )
			{
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				CSound *pSound = PBestSound();
				ASSERT( pSound != NULL );
				if ( pSound )
					MakeIdealYaw ( pSound->m_vecOrigin );
			}
			break;
		}
	case MONSTERSTATE_COMBAT:
		/*
		COMBAT goes to HUNT upon losing sight of enemy
		COMBAT goes to ALERT upon death of enemy
		*/
		{
			if ( m_hEnemy == NULL )
			{
				m_IdealMonsterState = MONSTERSTATE_ALERT;
				// pev->effects = EF_BRIGHTFIELD;
				ALERT ( at_aiconsole, "***Combat state with no enemy!\n" );
			}
			break;
		}
	case MONSTERSTATE_HUNT:
		/*
		HUNT goes to ALERT upon seeing food
		HUNT goes to ALERT upon being injured
		HUNT goes to IDLE if goal touched
		HUNT goes to COMBAT upon seeing enemy
		*/
		{
			break;
		}
	case MONSTERSTATE_SCRIPT:
		if ( iConditions & (bits_COND_TASK_FAILED|bits_COND_LIGHT_DAMAGE|bits_COND_HEAVY_DAMAGE) )
		{
			ExitScriptedSequence();	// This will set the ideal state
		}
		break;

	case MONSTERSTATE_DEAD:
		m_IdealMonsterState = MONSTERSTATE_DEAD;
		break;
	}

	return m_IdealMonsterState;
}

