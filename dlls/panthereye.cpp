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
// Panthereye - By Osiris / OsirisGodoftheDead / THE_YETI
//=========================================================


#include "panthereye.h"

#include "custom_debug.h"


extern DLL_GLOBAL int		g_iSkillLevel;


#define newPathDelayDuration 0.5;

extern float global_noFlinchOnHard;
extern float global_panthereyeHasCloakingAbility;
extern float global_panthereyeJumpDotTol;

extern float global_panthereyePrintout;
EASY_CVAR_EXTERN(animationFramerateMulti)

					






BOOL CPantherEye::testLeapNoBlock(void){

	//jump!... or try to.
	TraceResult tr;


	if(m_hEnemy != NULL){
		Vector vTestStart = pev->origin + Vector(0, 0, 10);
		Vector vTestEnd = m_hEnemy->pev->origin + Vector(0, 0, 10);
		UTIL_TraceHull( vTestStart, vTestEnd, dont_ignore_monsters, head_hull, ENT( m_hEnemy->pev ), &tr );
				
		DebugLine_ClearAll();
		DebugLine_Setup(0, vTestStart, vTestEnd, tr.flFraction);

		//don't do the "!tr.fAllSolid" check.  ?
		if((tr.flFraction >= 1.0 || tr.pHit == m_hEnemy.Get() )  ){
			//did not hit anything on the way or hit the enemy? it is OK.
			//proceed
		}else{
			//stop!
			return FALSE;
		}
	}else{
		//go ahead regardless.?
	}

	return TRUE;
}//END OF testLeapNoBlock






int CPantherEye::IRelationship( CBaseEntity *pTarget )
{
	/*
	if(pTarget->isProvokable() && !pTarget->isProvoked() ){
		//I have no reason to pick a fight with this unprovoked, neutral enemy.
		return R_NO;
	}
	*/

	if(!pTarget->isForceHated(this) && pTarget->Classify() == CLASS_ALIEN_PREY){
		//don't hate, just "Dislike" instead.
		return R_DL;
	}



	return CBaseMonster::IRelationship( pTarget );
}








void CPantherEye::panthereye_findCoverFromEnemy( void ){
	
	entvars_t *pevCover;

	if ( m_hEnemy == NULL )
	{
		// Find cover from self if no enemy available
		pevCover = pev;
//				TaskFail();
//				return;
	}
	else
		pevCover = m_hEnemy->pev;
			
	EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("YOU nAYYYYYYYYYYYYYYYYYYY") );
	if ( FindLateralCover( pevCover->origin, pevCover->view_ofs ) )
	{
		// try lateral first
		m_flMoveWaitFinished = gpGlobals->time + findCoverTaskDataMem;
		TaskComplete();
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("YOU coward2"));
	}
	else if ( FindCover( pevCover->origin, pevCover->view_ofs, 0, CoverRadius() ) )
	{
		// then try for plain ole cover
		m_flMoveWaitFinished = gpGlobals->time + findCoverTaskDataMem;
		TaskComplete();
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("YOU coward3"));
	}
	else
	{
				
//let's uh... let's not.
		/*
		// no coverwhatsoever.
				
		//MODDD - how do we handle failing to find cover?
				
		if( m_hEnemy != NULL && UTIL_IsFacing(m_hEnemy->pev, pev->origin, 0.2) && (m_hEnemy->pev->origin - pev->origin).Length() < 240 ){
			//go aggro!
			pissedOffTime = gpGlobals->time + 5;
			ChangeSchedule(GetSchedule());
			//timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(7, 16);
		}

		if( m_hEnemy != NULL && UTIL_IsFacing(m_hEnemy->pev, pev->origin, 0.2) && (m_hEnemy->pev->origin - pev->origin).Length() < 240 ){
			//go aggro!
			pissedOffTime = gpGlobals->time + 5;
			ChangeSchedule(GetSchedule());
			return;
			//timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(7, 16);
		}else{
			//try sneaking again sooner?  Be easier to piss off since couldn't update cover...  Being cornered makes most things easier to piss off.
			isPissable = TRUE;
			timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(4, 8);
		}
		*/
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("YOU 4"));
		RouteClear();
		TaskFail();

	}

}




//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
//Note that predators attack other predators on sight.  Panthers will attack other panthers.  Change class to "CLASS_ALIEN_MONSTER" to still make it hostile to non-aliens (to still be hostile to "Prey", may need to make a new one)
int	CPantherEye :: Classify ( void )
{
	return	CLASS_ALIEN_PREDATOR;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CPantherEye :: SetYawSpeed ( void )
{
	int ys;

	//nimble, turn fast.  (default is 120?)
	ys = 200;

#if 0
	switch ( m_Activity )
	{
	}
#endif

	pev->yaw_speed = ys;
}




GENERATE_TRACEATTACK_IMPLEMENTATION(CPantherEye)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CPantherEye)
{

	// HACK HACK -- until we fix this.
	if ( IsAlive() )
		PainSound();
	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}







//I will.
BOOL CPantherEye::forceIdleFrameReset(void){
	return TRUE;
}


BOOL CPantherEye::usesAdvancedAnimSystem(void){
	return TRUE;
}




int CPantherEye::LookupActivityHard(int activity){
	
	int i = 0;


	int iRandChoice = 0;
	int iRandWeightChoice = 0;
	
	char* animChoiceString = NULL;
	int* weightsAbs = NULL;
			
	//pev->framerate = 1;
	int maxRandWeight = 30;



	

	m_flFramerateSuggestion = 1;
	m_flFrameRate = 1;
	pev->framerate = 1;
	//is this safe?

	/*
	m_flFramerateSuggestion = -1;
	//pev->frame = 6;
	return LookupSequence("get_bug");
	*/

	//any animation events in progress?  Clear it.
	resetEventQueue();

	//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("AHH %d", m_fSequenceFinished));

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){

		case ACT_IDLE:

			iRandChoice = -1;
			if(sneakMode == -1){
				
				maxRandWeight = 10;
				weightsAbs = new int[2];
				weightsAbs[0] = 7;
				weightsAbs[1] = 10;

			
				//1 to highest possible (sum of all weights).
				iRandWeightChoice = RANDOM_LONG(1, maxRandWeight);
			
				//What range did we strike?

				for(i = 0; i < 2; i++){
					if(iRandWeightChoice <= weightsAbs[i]){
						iRandChoice = i;
						break;
					}
				}
				delete[] weightsAbs;

				//"get_bug" may make more sense when there is ACTUALLY a cockroach nearby.
			
				//iRandChoice = 2;
				switch(iRandChoice){
					case 0:
						m_flFramerateSuggestion = 1.1f;
						animChoiceString = "itch";
					break;
					case 1:
						m_flFramerateSuggestion = 1;
						animChoiceString = "shakes";
					break;
				}


			}else{
				//sneakery...

				
				maxRandWeight = 10;
				weightsAbs = new int[2];
				weightsAbs[0] = 7;
				weightsAbs[1] = 10;

			
				//1 to highest possible (sum of all weights).
				iRandWeightChoice = RANDOM_LONG(1, maxRandWeight);
			
				//What range did we strike?

				for(i = 0; i < 2; i++){
					if(iRandWeightChoice <= weightsAbs[i]){
						iRandChoice = i;
						break;
					}
				}
				delete[] weightsAbs;

				//"get_bug" may make more sense when there is ACTUALLY a cockroach nearby.
			
				//iRandChoice = 2;
				switch(iRandChoice){
					case 0:
						animChoiceString = "subtle_motion";
					break;
					case 1:
						animChoiceString = "idle_figit";
					break;
				}
			}//END OF else OF sneak check


			
			//animChoiceString = "subtle_motion";

			if(animChoiceString != NULL){
				EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHER: getIdleAnim: %s", animChoiceString));
				return LookupSequence(animChoiceString);
			}else{
				EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHER: getIdleAnim FAILED!!!"));
			}


		break;
		case ACT_MELEE_ATTACK1:



			if(g_iSkillLevel == SKILL_EASY){
				m_flFramerateSuggestion = 1.22;
			}else if(g_iSkillLevel == SKILL_MEDIUM){
				m_flFramerateSuggestion = 1.3;
			}else if(g_iSkillLevel == SKILL_HARD){
				m_flFramerateSuggestion = 1.45;
			}


			//randomize...
			iRandChoice = RANDOM_LONG(0, 2);
			switch(iRandChoice){
				case 0:
					
					//rise, hard-left hit, slight right hit.
					//ALSO, artificial anim event: the 2nd hand hit does not come with a model event.
					
					//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PRIMARY START!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %.2f", gpGlobals->time));
					//m_flFramerateSuggestion = 0.2f;
					animEventQueuePush(12.0f/15.0f, 0);

					return LookupSequence("attack_primary");
				break;
				case 1:
					//slightly longer right-swipe.
					return LookupSequence("attack_main_claw");
				break;
				case 2:
					//slightly faster left-swipe.
					return LookupSequence("attack_simple_claw");
				break;
			}
		break;
		
		case ACT_MELEE_ATTACK2:
			m_flFramerateSuggestion = 0.95f;

			//jump with the delay.
			//animEventQueuePush(10.35f/(15.0f), 2 );
			
			return LookupSequence("crouch_to_jump");
		break;
		case ACT_SMALL_FLINCH:
			//NOTE: animation "flinch" seems to be a clone of "flinch_light".
			m_flFramerateSuggestion = 1.4f;
			return LookupSequence("flinch_light");
		break;
		case ACT_BIG_FLINCH:
			m_flFramerateSuggestion = 2.5f;
			return LookupSequence("flinch_hard");
		break;
		case ACT_DIEVIOLENT:
			return LookupSequence("death_simple");
		break;
		case ACT_TURN_LEFT:
			m_flFramerateSuggestion = 2.3f;
			return LookupSequence("turn_left");
		break;
		case ACT_TURN_RIGHT:
			m_flFramerateSuggestion = 2.3f;
			return LookupSequence("turn_right");
		break;
		case ACT_RUN:
			//base method does fine, we just want to... 'intervene', so to speak.

			if(g_iSkillLevel == SKILL_EASY){
				m_flFramerateSuggestion = 1.08;
			}else if(g_iSkillLevel == SKILL_MEDIUM){
				m_flFramerateSuggestion = 1.14;
			}else if(g_iSkillLevel == SKILL_HARD){
				m_flFramerateSuggestion = 1.22;
			}
			return CBaseAnimating::LookupActivity(activity);
		break;



	}
	
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}


int CPantherEye::tryActivitySubstitute(int activity){
	
	int i = 0;
	int iRandChoice = 0;
	int iRandWeightChoice = 0;

	char* animChoiceString = NULL;
	int* weightsAbs = NULL;
			
	//pev->framerate = 1;
	int maxRandWeight = 30;

	//any animation events in progress?  Clear it.
	//MODDD - this is tryActivitySubstitute, often used to test if an activitiy is OK to call, not to do it in particular.
	//Any actual effects just from just this check could be unwelcome.
	//resetEventQueue();

	//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("AHH %d", m_fSequenceFinished));

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){

		case ACT_IDLE:
			iRandChoice = -1;
			
			//each anim's chance of happening (not equal).
			//int weights[] = {25, 3, 1, 1};
			weightsAbs = new int[4];
			weightsAbs[0] = 25;
			weightsAbs[1] = 28;
			weightsAbs[2] = 29;
			weightsAbs[3] = 30;

			//1 to highest possible (sum of all weights).
			iRandWeightChoice = RANDOM_LONG(1, maxRandWeight);
			//What range did we strike?
			for(i = 0; i < 4; i++){
				if(iRandWeightChoice <= weightsAbs[i]){
					iRandChoice = i;
					break;
				}
			}
			delete[] weightsAbs;

			//"get_bug" may make more sense when there is ACTUALLY a cockroach nearby.
			//iRandChoice = 2;
			switch(iRandChoice){
				case 0:
					animChoiceString = "subtle_motion";
				break;
				case 1:
					animChoiceString = "idle_figit";
				break;
				case 2:
					animChoiceString = "itch";
				break;
				case 3:
					animChoiceString = "shakes";
				break;
			}
			return LookupSequence(animChoiceString);

		break;
		case ACT_MELEE_ATTACK1:
			//randomize...
			iRandChoice = RANDOM_LONG(0, 2);
			switch(iRandChoice){
				case 0:
					return LookupSequence("attack_primary");
				break;
				case 1:
					//slightly longer right-swipe.
					return LookupSequence("attack_main_claw");
				break;
				case 2:
					//slightly faster left-swipe.
					return LookupSequence("attack_simple_claw");
				break;
			}
		break;
		case ACT_MELEE_ATTACK2:
			return LookupSequence("crouch_to_jump");
		break;
		case ACT_SMALL_FLINCH:
			//NOTE: animation "flinch" seems to be a clone of "flinch_light".
			return LookupSequence("flinch_light");
		break;
		case ACT_BIG_FLINCH:
			return LookupSequence("flinch_hard");
		break;
		case ACT_DIEVIOLENT:
			return LookupSequence("death_simple");
		break;
		case ACT_TURN_LEFT:
			//gets it right, but add framerate:
			//...other version of lookup.  HARD
			return LookupSequence("turn_left");
		break;
		case ACT_TURN_RIGHT:
			return LookupSequence("turn_right");
		break;


	}
	
	//not handled by above?
	return CBaseAnimating::LookupActivity(activity);
}


void CPantherEye :: PainSound( void )
{
//	int pitch = 95 + RANDOM_LONG(0,9);

//	if (RANDOM_LONG(0,5) < 2)
//		EMIT_SOUND_FILTERED ( ENT(pev), CHAN_VOICE, pPainSounds[ RANDOM_LONG(0,ARRAYSIZE(pPainSounds)-1) ], 1.0, ATTN_NORM, 0, pitch );
}

void CPantherEye :: AlertSound( void )
{
//	int pitch = 95 + RANDOM_LONG(0,9);

//	EMIT_SOUND_FILTERED ( ENT(pev), CHAN_VOICE, pAlertSounds[ RANDOM_LONG(0,ARRAYSIZE(pAlertSounds)-1) ], 1.0, ATTN_NORM, 0, pitch );
}

void CPantherEye :: IdleSound( void )
{
	int pitch = 95 + RANDOM_LONG(0,9);

	// Play a random idle sound
	EMIT_SOUND_FILTERED ( ENT(pev), CHAN_VOICE, pIdleSounds[ RANDOM_LONG(0,ARRAYSIZE(pIdleSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
}

void CPantherEye :: AttackSound( void )
{
	// Play a random attack sound
	EMIT_SOUND_FILTERED ( ENT(pev), CHAN_VOICE, pAttackSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
}





void CPantherEye::HandleEventQueueEvent(int arg_eventID){
	

	switch(arg_eventID){
		case 0:
			{
			// do stuff for this event.
	//		ALERT( at_console, "Slash right!\n" );

				//half-damage, the second swipe is a less pronounced claw swipe.
			CBaseEntity *pHurt = CheckTraceHullAttack( 84*1.2, gSkillData.panthereyeDmgClaw/2, DMG_BLEEDING );
			if ( pHurt )
			{
				if ( pHurt->pev->flags & (FL_MONSTER|FL_CLIENT) )
				{
					pHurt->pev->punchangle.z = -18;
					pHurt->pev->punchangle.x = 5;
					pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 100;
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
			//
			UTIL_MakeVectors(pev->angles);

			//....
			Vector vecForwardFrom = pev->origin + gpGlobals->v_forward * 40;
						
			Vector vecSearchStart = vecForwardFrom;
						
			//Vector vecSearchStart = UTIL_getFloor(vecForwardFrom, 300, ignore_monsters, ENT(pev));

			//if(isErrorVector(vecSearchStart)){
			//	//no floor? just use the area...
			//	vecSearchStart = vecForwardFrom - (0, 0, 5);
			//}
						
						
			debugDrawVect = vecForwardFrom;


			CBaseEntity* pEntityScan = NULL;
			while ( (pEntityScan = UTIL_FindEntityInSphere( pEntityScan, vecSearchStart, 40 ) ) != NULL)
			{

				//MySquadMonsterPointer ????
				CBaseMonster* testMon = pEntityScan->MyMonsterPointer();

				//could check for "classify" being "insect" too, if there were more insects?
				if(testMon != NULL && FClassnameIs(testMon->pev, "monster_cockroach")){
					//only one.
					testMon->TakeDamage( pev, pev, testMon->pev->health, DMG_CRUSH );
					break;
				};


			}

			bugAnimBlockedTime = gpGlobals->time + RANDOM_LONG(6, 15);

		break;
			}
		case 2:
			{

			BOOL isJumpOkay = testLeapNoBlock();
			if(!isJumpOkay){
				TaskFail();
				//force a new animation to be fetched.
				m_Activity = ACT_RESET;
				this->SetActivity(ACT_IDLE);
				return;
			}



			ClearBits( pev->flags, FL_ONGROUND );

			UTIL_SetOrigin (pev, pev->origin + Vector ( 0 , 0 , 1) );// take him off ground so engine doesn't instantly reset onground 
			UTIL_MakeVectors ( pev->angles );

			Vector vecJumpDir;
			if (m_hEnemy != NULL)
			{
				float gravity = g_psv_gravity->value;
				if (gravity <= 1)
					gravity = 1;

				// How fast does the headcrab need to travel to reach that height given gravity?
				float height = (m_hEnemy->pev->origin.z + m_hEnemy->pev->view_ofs.z - pev->origin.z);

				float speedExtraMult = 1;
				//was a minimum of 15.
				if (height < 11)
					height = 11;

				if(height > 40){
					//CRUDE. Take how much height was above 40 and let it add to speedExtraMult.
					speedExtraMult = 1 + (height - 40) / 40;
					height = 40;
				}

				//was 2 * gravity * height.
				float speed = sqrt( 1.7 * gravity * height * speedExtraMult);
				float time = speed / gravity;

				// Scale the sideways velocity to get there at the right time
				//???
				//vecJumpDir = (m_hEnemy->pev->origin + m_hEnemy->pev->view_ofs - pev->origin);

				//must jump at least "240" far...
				vecJumpDir = max( (m_hEnemy->pev->origin + m_hEnemy->pev->view_ofs - pev->origin).Length() - 80, 320 ) * gpGlobals->v_forward;
				
				vecJumpDir = vecJumpDir * ( 1.0 / time );

				// Speed to offset gravity at the desired height
				vecJumpDir.z = speed;

				// Don't jump too far/fast
				float distance = vecJumpDir.Length();
							
				//vecJumpDir = (distance -50) * vecJumpDir.Normalize();

				//I'm not entirely sure what this is doing...
				//OH, I see, forcing a dist of 650 exactly, factoring out the old value.  It's how vectors work.
				if (distance > 950)
				{
					vecJumpDir = vecJumpDir * ( 950.0 / distance );
				}

			}
			else
			{
				// jump hop, don't care where
				vecJumpDir = Vector( gpGlobals->v_forward.x, gpGlobals->v_forward.y, gpGlobals->v_up.z ) * 350;
			}

			int iSound = RANDOM_LONG(0,2);
			if ( iSound != 0 )
				EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, pAttackSounds[iSound], 1, ATTN_IDLE, 0, 100 );

			pev->velocity = vecJumpDir;

			SetTouch( &CPantherEye::LeapTouch );
		break;
		}
	}//END OF switch(arg_eventID)

}



//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CPantherEye :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("BATTA-BING, BATTA-BOOM %d", pEvent->event));

	switch( pEvent->event )
	{
		case 1:
		{
			// do stuff for this event.
	//		ALERT( at_console, "Slash right!\n" );
			CBaseEntity *pHurt = CheckTraceHullAttack( 84*1.2, gSkillData.panthereyeDmgClaw, DMG_BLEEDING );
			if ( pHurt )
			{
				if ( pHurt->pev->flags & (FL_MONSTER|FL_CLIENT) )
				{
					pHurt->pev->punchangle.z = -18;
					pHurt->pev->punchangle.x = 5;
					pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_right * 100;
				}
				// Play a random attack hit sound
				EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
			}
			else // Play a random attack miss sound
				EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );

			if (RANDOM_LONG(0,1))
				AttackSound();
		}
		break;

		default:
			CBaseMonster::HandleAnimEvent( pEvent );
			break;
	}
}

CPantherEye::CPantherEye(void){

	stareTime = 0;

	maxWaitPVSTime = -1;
	chaseMode = -1;
	m_flFramerateSuggestion = 1;
	bugAnimBlockedTime = -1;
	sneakMode = -1;
	newPathDelay = -1;

	
	runawayTime = -1;
	pissedOffTime = -1;

	timeTillSneakAgain = -1;
	isPissable = FALSE;

	isCornered = FALSE;

	waitingForNewPath = FALSE;

	findCoverTaskDataMem = -1;

}






//=========================================================
// Spawn
//=========================================================
void CPantherEye::Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/panthereye.mdl");
	//UTIL_SetSize( pev, Vector(-30, -30, 0), Vector(30, 30, 40) );
	UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );



	pev->classname = MAKE_STRING("monster_panthereye");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->health		= gSkillData.panthereyeHealth;

	//MODDD - NOTE: can "view_ofs" just come from a model?  Verify by printout maybe?


	//pev->view_ofs		= VEC_VIEW;// position of the eyes relative to monster's origin.
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;
	m_afCapability		= bits_CAP_DOORS_GROUP;

	MonsterInit();

	//start out not sneaking.
	sneakMode = -1;
	
}



void CPantherEye::SetEyePosition(void){
	pev->view_ofs = VEC_VIEW;
}//END OF SetEyePosition


extern int global_useSentenceSave;
//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================
void CPantherEye :: Precache()
{
	int i;

	PRECACHE_MODEL("models/panthereye.mdl");

	global_useSentenceSave = TRUE;
	for ( i = 0; i < ARRAYSIZE( pAttackHitSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackHitSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackMissSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackMissSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAttackSounds ); i++ )
		PRECACHE_SOUND((char *)pAttackSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pIdleSounds ); i++ )
		PRECACHE_SOUND((char *)pIdleSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pAlertSounds ); i++ )
		PRECACHE_SOUND((char *)pAlertSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pPainSounds ); i++ )
		PRECACHE_SOUND((char *)pPainSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pLeapAttackHitSounds ); i++ )
		PRECACHE_SOUND((char *)pLeapAttackHitSounds[i]);

	for ( i = 0; i < ARRAYSIZE( pDeathSounds ); i++ )
		PRECACHE_SOUND((char *)pDeathSounds[i]);


	global_useSentenceSave = FALSE;
}	

//=========================================================
// AI Schedules Specific to this monster
//=========================================================




void CPantherEye :: DeathSound ( void )
{
	int pitch = randomValueInt(96, 102);

	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1, ATTN_IDLE, 0, pitch );
		
}



BOOL CPantherEye::CheckMeleeAttack1 ( float flDot, float flDist )
{
	/*
	if(m_hEnemy != NULL){
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("SO::: %d, %.2f %.2f %d", !HasConditions( bits_COND_ENEMY_OCCLUDED ), flDist, flDot, m_hEnemy ->Classify() ));
	}else{
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("NO ENEMY"));
	}
	*/
	if ( !HasConditions( bits_COND_ENEMY_OCCLUDED ) && flDist <= 90 	&& 
	 m_hEnemy != NULL &&
	 m_hEnemy ->Classify() != CLASS_ALIEN_BIOWEAPON &&
	 m_hEnemy ->Classify() != CLASS_PLAYER_BIOWEAPON   )
	{

		//also need to be looking closely enough.
		if(flDot >= 0.7){
			return TRUE;
		}

	}

	return FALSE;
}





BOOL CPantherEye::CheckMeleeAttack2 ( float flDot, float flDist )
{
	/*
	if(m_hEnemy != NULL){
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("SO::: %d, %.2f %.2f %d", !HasConditions( bits_COND_ENEMY_OCCLUDED ), flDist, flDot, m_hEnemy ->Classify() ));
	}else{
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("NO ENEMY"));
	}
	*/


	float enemyFloorZ;
	
	

	if(m_hEnemy != NULL){

		if(m_hEnemy->IsPlayer()){
			//If the enemy is a player, their origin is the center. adjust accordingly.
			enemyFloorZ = m_hEnemy->pev->origin.z - 30;
		}else{
			//origin is already at the floor.
			enemyFloorZ = m_hEnemy->pev->origin.z;
		}

		//easyForcePrintLine("eeee %.2f", abs(this->pev->origin.z - enemyFloorZ));
	}


	if ( m_hEnemy != NULL &&
		!HasConditions( bits_COND_ENEMY_OCCLUDED ) && flDist >= 270 && flDist <= 410 &&
		abs(this->pev->origin.z - enemyFloorZ) < 30 &&     //not too much veritcal difference allowed, this is for a long floor-wise leap.
	 m_hEnemy ->Classify() != CLASS_ALIEN_BIOWEAPON &&
	 m_hEnemy ->Classify() != CLASS_PLAYER_BIOWEAPON   )
	{

		//lastly, is the path unobstructed?
		
		BOOL isJumpOkay = testLeapNoBlock();
		if(!isJumpOkay)return FALSE;

		//also need to be looking closely enough.
		if(flDot >= 0.91){
			return TRUE;
		}

	}

	return FALSE;
}









int CPantherEye::IgnoreConditions ( void )
{
	int iIgnore = CBaseMonster::IgnoreConditions();

	return iIgnore;
	
}


BOOL CPantherEye::hasSeeEnemyFix(void){
	return FALSE;
}



//Based off of headcrab's "LeapTouch".  Not a method from CBaseMonster.
void CPantherEye::LeapTouch ( CBaseEntity *pOther )
{
	if ( !pOther->pev->takedamage )
	{
		return;
	}

	//int classifyVal = pOther->Classify();
	int relationshipVal = IRelationship( pOther );
	if ( pOther != m_hEnemy && (relationshipVal == R_NO || relationshipVal == R_AL) )
	{
		//can't hurt neutrals or ally's by accident with this.
		return;
	}

	// Don't hit if back on ground
	if ( !FBitSet( pev->flags, FL_ONGROUND ) )
	{
		//generic attack sound?
		EMIT_SOUND_FILTERED( edict(), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pLeapAttackHitSounds), 1, ATTN_IDLE, 0, 100 );
		
		//Increase damage beyond default?  Probably not.?
		pOther->TakeDamage( pev, pev, gSkillData.panthereyeDmgClaw, DMG_SLASH );
	}

	SetTouch( NULL );
}





void CPantherEye::MonsterThink ( void )
{

	if(!deadSetActivityBlock && !iAmDead && pev->deadflag == DEAD_NO){
		//ok?
		//return;

		if(newPathDelay != -1){
			if(newPathDelay <= gpGlobals->time){
				//-1 is a symbol that means, no longer waiting.
				newPathDelay = -1;
			}
		}

		//RIPPED FROM hassassin.

		//m_Activity == ACT_RUN || m_Activity == ACT_WALK 
		if (global_panthereyeHasCloakingAbility <= 0 || m_hEnemy == NULL || pev->deadflag != DEAD_NO || !(pev->flags & FL_ONGROUND) || sneakMode == -1 || (m_hEnemy->pev->origin - pev->origin).Length() < 170 )
			m_iTargetRanderamt = 255;
		else{
			//m_iTargetRanderamt = 20;
			m_iTargetRanderamt = min(global_panthereyeHasCloakingAbility, 1) * 255;
		}
		if (pev->renderamt > m_iTargetRanderamt)
		{
			pev->renderamt = max( pev->renderamt - 50, m_iTargetRanderamt );
			pev->rendermode = kRenderTransTexture;
		}
		else if (pev->renderamt < m_iTargetRanderamt)
		{
			pev->renderamt = min( pev->renderamt + 50, m_iTargetRanderamt );
			if (pev->renderamt == 255)
				pev->rendermode = kRenderNormal;
		}

	
		EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHERREPORT: SCHED: %s TASK: %d SEE-EN: %d CAN_ME: %d CAPABLE: %d PISSA: %d CORN: %d", tryGetScheduleName(), tryGetTaskID(), HasConditions(bits_COND_SEE_ENEMY), HasConditions(bits_COND_CAN_MELEE_ATTACK1), m_afCapability & bits_CAP_MELEE_ATTACK1, isPissable, isCornered  ));
		
		//MODDD - TODO: how about another CVar to check that we're not in the middle of real combat (getting distracted by bugs while getting shot it is a bit odd).  While chasing alone, having not been fired at and/or damaged isn't as bad though.
		if (pev->deadflag == DEAD_NO && m_IdealActivity == ACT_IDLE && m_pSchedule != slPanthereyeGetBug && bugAnimBlockedTime <= gpGlobals->time ) //do this stuff while alive
		{
			//does this get every roach on the map or anything?    Just one "roach" if I understand right?   Can that really work?
			//(I'm thinking check for ents in a radius every-so-often, maybe half-a-second?).
		
			CBaseEntity* pEntityScan = NULL;
			while ( (pEntityScan = UTIL_FindEntityInSphere( pEntityScan, pev->origin, 68 ) ) != NULL)
			{

				//MySquadMonsterPointer ????
				CBaseMonster* testMon = pEntityScan->MyMonsterPointer();

				//could check for "classify" being "insect" too, if there were more insects?
				if(testMon != NULL && FClassnameIs(testMon->pev, "monster_cockroach")){
					MakeIdealYaw(testMon->pev->origin);
					ChangeSchedule(slPanthereyeGetBug);

				};

			}
		}
	}//END OF dead check


	CBaseMonster::MonsterThink();
}



void CPantherEye::SetActivity ( Activity NewActivity )
{

	/*
	if(pev->deadflag != DEAD_NO &&
	*/

	if(deadSetActivityBlock &&
		(NewActivity != ACT_DIESIMPLE &&
		NewActivity != ACT_DIEBACKWARD && 
		NewActivity != ACT_DIEFORWARD &&
		NewActivity != ACT_DIEVIOLENT 
		)
		){
		//////easyForcePrintLine("OOOOOOOOOOOOOOOOOOOOOO freakin HOW %d ::: %d %d %d", monsterID, NewActivity, pev->deadflag, m_MonsterState);
		return;
	}
	

	EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHER: SetAct: %d", NewActivity));

	int	iSequence;

	//iSequence = LookupActivity ( NewActivity );
	iSequence = LookupActivityHard ( NewActivity );

	//only difference?  no need to have this copy over here then..

	// Set to the desired anim, or default anim if the desired is not present
	if ( iSequence > ACTIVITY_NOT_AVAILABLE )
	{
		//MODDD - added "forceReset"... NO, REVERTED.
		//if ( forceReset || (pev->sequence != iSequence || !m_fSequenceLoops) )
		if (  pev->sequence != iSequence || !m_fSequenceLoops )
		{
			// don't reset frame between walk and run
			if ( !(m_Activity == ACT_WALK || m_Activity == ACT_RUN) || !(NewActivity == ACT_WALK || NewActivity == ACT_RUN))
				resetFrame();
		}

		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)

		//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("NewActivity:%d : forceReset:%d",  NewActivity, forceReset ));
		ResetSequenceInfo( );

		//MODDD
		pev->framerate = m_flFramerateSuggestion;
		//should this always be reset?
		//m_flFramerateSuggestion = 1;

		SetYawSpeed();
	}
	else
	{
		// Not available try to get default anim
		ALERT ( at_aiconsole, "%s has no sequence for act:%d\n", STRING(pev->classname), NewActivity );
		
		pev->sequence		= 0;	// Set to the reset anim (if it's there)
	}

	m_Activity = NewActivity; // Go ahead and set this so it doesn't keep trying when the anim is not present
	
	// In case someone calls this with something other than the ideal activity
	m_IdealActivity = m_Activity;


	//CBaseMonster::SetActivity(NewActivity);
}


//based off of GetSchedule for CBaseMonster in schedule.cpp.
Schedule_t *CPantherEye::GetSchedule ( void )
{

	if(!iAmDead && deadSetActivityBlock){
		//////easyForcePrintLine("WHAT THE HELL IS WRONG WITH YOOOUUUUUUUUU %d : %s %d %d   ::: %d %d", monsterID, m_pSchedule!=NULL?m_pSchedule->pName:"NULL", pev->deadflag, m_MonsterState, monsterID, iAmDead, iAmDead, deadSetActivityBlock);
	}

	if(iAmDead || deadSetActivityBlock){
		return GetScheduleOfType( SCHED_DIE );
	}
	if(!iAmDead && deadSetActivityBlock){
		//////easyForcePrintLine("IM GONNA eat YOUR WHOLE box of raisins");
	}


	//MODDD - experimental. Auto get the best sound then.
	SCHEDULE_TYPE baitSched = getHeardBaitSoundSchedule(); //getHeardBaitSoundSchedule(pSound);

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
					//return GetScheduleOfType( SCHED_COMBAT_FACE );

					//better handling.
					return GetScheduleOfType( SCHED_PANTHEREYE_CHASE_ENEMY );

				}
				else
				{
					// chase!
					///EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("ducks??"));
					return GetScheduleOfType( SCHED_PANTHEREYE_CHASE_ENEMY );
				}
			}
			else  
			{
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
				if ( !HasConditions(bits_COND_CAN_RANGE_ATTACK1 | bits_COND_CAN_MELEE_ATTACK1) )
				{
					// if we can see enemy but can't use either attack type, we must need to get closer to enemy
					//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("ducks2"));
					return GetScheduleOfType( SCHED_PANTHEREYE_CHASE_ENEMY );
				}
				else if ( !FacingIdeal() )
				{
					//turn
					//return GetScheduleOfType( SCHED_COMBAT_FACE );

					//better handling.
					return GetScheduleOfType( SCHED_PANTHEREYE_CHASE_ENEMY );


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
				EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("WARNING: m_pCine IS NULL!"));
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

	EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHER: I AM A COMPLETE DISGRACE"));
	return &slError[ 0 ];
}



Schedule_t* CPantherEye::ChaseOrCircle(void){
	
	TraceResult rt;
	Vector enemyFloor = UTIL_getFloor(m_hEnemy->pev->origin, 800, ignore_monsters, ENT(m_hEnemy));

	float verticalDist = enemyFloor.z - pev->origin.z;
	if(isErrorVector(enemyFloor) || (verticalDist > 18) ){
		//we're chasing as usual.
		return slPanthereyeChaseEnemy;
	}else{
		//conditions good for circling.
		return slPanthereyeGetIntoCirclingRange;
	}

}

Schedule_t* CPantherEye::GetScheduleOfType( int Type){

	
	EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("PANTHER: GET SCHEDULE OF TYPE: %d", Type));

	
	if( (iAmDead || deadSetActivityBlock) && Type != SCHED_DIE){
		//////easyForcePrintLine("I WILL eat YOUR WHOLE box of rasinssss %d: %d %d ::: %d", monsterID, iAmDead, deadSetActivityBlock, Type);
	}



	switch(Type){
		/*
		case SCHED_COMBAT_FACE:

		break;
		*/
		case SCHED_PANTHEREYE_COVER_FAIL:
			return slPantherEyeCoverFail;
		break;
		case SCHED_PANTHEREYE_GENERIC_FAIL:
			return slPantherEyeGenericFail;
		break;

		case SCHED_PANTHEREYE_CHASE_ENEMY:

			EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("the TIME dmg: %d | %.2f %.2f",  HasConditions(bits_COND_LIGHT_DAMAGE), (timeDelayFilter(pissedOffTime) ), ( timeDelayFilter(runawayTime) ))); 
			if(pissedOffTime != -1){
				
				if(pissedOffTime <= gpGlobals->time){
					//end!
					
				}else{
					//still going.
					//return slPanthereyeGetIntoCirclingRange;
					//NO.
					return slPanthereyeChaseEnemy;
				}
				
			}
			
			if(runawayTime != -1 ){
				if(runawayTime <= gpGlobals->time){
					//end!
					
				}else{
					return GetScheduleOfType(SCHED_TAKE_COVER_FROM_ENEMY);
				}
			}

			//nothing from above?  We're sneaking around.
			return slPanthereyeSneakToLocation;
		break;

		//Don't stare into space for 15 seconds. Just sneakwait for being more reactive.
		case SCHED_PATHFIND_STUMPED:
			return &slPanthereyeSneakWait[0];
		break;


		case SCHED_MELEE_ATTACK2:
			//the MELEE_ATTACK_1 sched is okay. #2 needs to use a custom schedule to
			//(be able to) turn while prepping the jump.
			return &slPanthereyeJumpAtEnemy[0];
		break;
		case SCHED_FAIL:
		case SCHED_CHASE_ENEMY_FAILED:
			//what is usually called on failure.  Can call something else if preferred.
			return &slPantherEyeGenericFail[ 0 ];
		break;
	}

	return CBaseMonster::GetScheduleOfType(Type);
}



void CPantherEye::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
{
//	float flYaw = UTIL_VecToYaw ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin );// build a yaw that points to the goal.
//	WALK_MOVE( ENT(pev), flYaw, m_flGroundSpeed * flInterval, WALKMOVE_NORMAL );


	if(this->m_pSchedule == slPanthereyeJumpAtEnemy){
		//dont do it
		return;
	}


	if ( m_IdealActivity != m_movementActivity )
		m_IdealActivity = m_movementActivity;

	float flTotal = m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval;
	float flStep;
	while (flTotal > 0.001)
	{
		// don't walk more than 16 units or stairs stop working
		flStep = min( 16.0, flTotal );


		UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, flStep, MOVE_NORMAL );
		flTotal -= flStep;
	}
	// ALERT( at_console, "dist %f\n", m_flGroundSpeed * pev->framerate * flInterval );


}//END OF MoveExecute







void CPantherEye::StartTask ( Task_t *pTask ){

	//new task?  If we were waiting, no more.
	waitingForNewPath = FALSE;

	//any new task resets this.
	isPissable = FALSE;
	isCornered = FALSE;

	switch ( pTask->iTask ){
		//Copied from schedule.cpp.
		case TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY:
		{

			if(newPathDelay == -1){

				findCoverTaskDataMem = pTask->flData;

				panthereye_findCoverFromEnemy();
				newPathDelay = gpGlobals->time + newPathDelayDuration;
				waitingForNewPath = FALSE;
			}else{
				waitingForNewPath = TRUE;
			}

			break;
		}
		

		case TASK_PANTHEREYE_COVER_FAIL_WAIT:
		case TASK_PANTHEREYE_SNEAK_WAIT:
		{

			EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("CAN YOU EVEN JUST THIS ONCE???? %d", pTask->iTask)); 
			if(pTask->iTask == TASK_PANTHEREYE_COVER_FAIL_WAIT){
				//cover failed?  We're "cornered"...  go crazy aggro on damage.
				isCornered = TRUE;
			}

			timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(5, 11);

			if( m_hEnemy != NULL &&
				UTIL_IsFacing(m_hEnemy->pev, pev->origin, 0.3) && (
				    (isCornered && (m_hEnemy->pev->origin - pev->origin).Length() < 350*20) ||
				    (stareTime > 0 && stareTime > 2.6 && (m_hEnemy->pev->origin - pev->origin).Length() < 350*20)   //(gpGlobals->timie - stareTime > 2.6)
				)
					
			){
				//go aggro!
				pissedOffTime = gpGlobals->time + 5;
				ChangeSchedule(GetSchedule());
				return;
				//timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(7, 16);
			}else{
				//try sneaking again sooner?  Be easier to piss off since couldn't update cover...  Being cornered makes most things easier to piss off.
				isPissable = TRUE;
				timeTillSneakAgain = gpGlobals->time + RANDOM_LONG(4, 8);
			}


			//Can turn while waiting.
			if ( !FacingIdeal() ){
				SetTurnActivity();
				ChangeYaw( pev->yaw_speed );
			}

		break;
		}
		case TASK_PANTHER_NORMALMOVE:
		{

			
			if(m_hTargetEnt == NULL){
				//oh no.
				TaskFail();
				return;
			}
			if ( (m_hTargetEnt->pev->origin - pev->origin).Length() < 1 )
				TaskComplete();
			else
			{
				m_vecMoveGoal = m_hTargetEnt->pev->origin;
				if ( !MoveToTarget( ACT_WALK, 2 ) )
					TaskFail();
			}
			break;
		}


		case TASK_PANTHEREYE_PLAYANIM_GETBUG:

			this->setAnimationSmart("get_bug", 2.3f);
			resetEventQueue();
			animEventQueuePush(36.0f/15.0f, 1);
			
		break;
		case TASK_PANTHEREYE_PLAYANIM_CROUCHTOJUMP:
			
			//desired frame / framerate (factor in pev->framerate if other than "1" by multiplying default framerate by pev->framerate BEFORE division)
			
			//this->setAnimationSmart("crouch_to_jump", 0.95f);  //m_flFramerateSuggestion and pev->framerate become "2".
			
			//resetEventQueue();
			
			
			animEventQueuePush(10.35f/(15.0f), 2 );
			//animEventQueuePush(10.35f/(15.0f), 2 );
				
			
			
			this->SetSequenceByName("crouch_to_jump", 0.95f);
			
					
			
		break;
		case TASK_WAIT_PVS:
			maxWaitPVSTime = gpGlobals->time + 2;
			CBaseMonster::StartTask(pTask);
		break;
		case TASK_GET_PATH_TO_ENEMY:
		{
			if(newPathDelay == -1){
				CBaseMonster::StartTask(pTask);
				newPathDelay = gpGlobals->time + newPathDelayDuration;
				//got it right here.?
				waitingForNewPath = FALSE;
				
			}else{
				waitingForNewPath = TRUE;
			}

			//why copy?  Just rely on the parent case.
			/*
			if ( BuildRoute ( m_hEnemy->pev->origin, bits_MF_TO_ENEMY, m_hEnemy ) )
			{
				m_iTaskStatus = TASKSTATUS_COMPLETE;
			}
			else
			{
				ALERT ( at_aiconsole, "GetPathToEnemy failed!!\n" );
				TaskFail();
			}
			break;
			*/

			break;
		}
		case TASK_WAIT_FOR_MOVEMENT:

			//actually, nothing unusual.  Let the normal "Move" method do its thing.
			
			CBaseMonster::StartTask(pTask);

		break;
		case TASK_FACE_ENEMY:

			//wtf?
			//spend this much time at most trying to face the enemy
			faceLookTime = gpGlobals->time + 0.8f;
			CBaseMonster::StartTask(pTask);

		break;

		default:
			CBaseMonster::StartTask(pTask);
		break;
	}
}



void CPantherEye::RunTask ( Task_t *pTask ){
	easyForcePrintLine("HEY YOOOO %s %d", getScheduleName(), pTask->iTask);
	
	BOOL isEnemyLookingAtMe = FALSE;




	//NOTE: should only do this logic check if alive of course.
	if(pev->deadflag == DEAD_NO && !deadSetActivityBlock && !iAmDead){

		if(pissedOffTime == -1 && runawayTime == -1){
			//nothing all that strong, and an enemy about?  Sneak time!
			sneakMode = 0;
		}else{
			sneakMode = -1;
		}


		if(m_hEnemy != NULL){


			float distanceFromEnemy = (m_hEnemy->pev->origin - pev->origin).Length();

			BOOL pissOff = FALSE;
			BOOL pissOffHard = FALSE;
			BOOL tryRunAway = FALSE;

			//how far does the player have to be to look at me to piss me off?
			float lookAtMePissRange = 115 * 20;

			//how close does the player have to be, regardless of anything, to piss me off?
			float distancePissRange = 70 * 20;

			//if hurt (took damage), how far do I have to be to go on the attack instead of flee?
			float fightOrFlightRange = 400 * 20;


			if(isCornered){
				lookAtMePissRange = 340 * 20;
				distancePissRange = 230 * 20;
				//nowhere to go?  I'm coming for you.
				fightOrFlightRange = 1600 * 20;
			}else if(isPissable){
				//just more anxious than usual.
				lookAtMePissRange = 200 * 20;
				distancePissRange = 80 * 20;
				fightOrFlightRange = 500 * 20;
			}


			float extraSensitiveness =  1 + (1 - (pev->health / pev->max_health) ) * 0.6 ;

			lookAtMePissRange *= extraSensitiveness;
			distancePissRange *= distancePissRange;
			fightOrFlightRange *= fightOrFlightRange;
		

			if(HasConditions(bits_COND_SEE_ENEMY) ){


			}

			if(!HasConditions(bits_COND_ENEMY_OCCLUDED)  ){
				
				if(UTIL_IsFacing(m_hEnemy->pev, pev->origin, 0.3)){
					//if they face me anytime, face them.

					stareTime += gpGlobals->frametime;


					//they are.
					isEnemyLookingAtMe = TRUE;

					int tsknbr = getTaskNumber();
					EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("RUN RUN %s %d %d", m_pSchedule->pName, tsknbr, isCornered));
					if(  (isCornered || (m_pSchedule == slPanthereyeSneakToLocation || m_pSchedule == slPantherEyeCoverFail) && (tsknbr == TASK_PANTHEREYE_SNEAK_WAIT || tsknbr == TASK_PANTHEREYE_COVER_FAIL_WAIT) ) ){
						//likely waiting.
						MakeIdealYaw(m_hEnemy->pev->origin);

						//instant yaw change. is that a good idea?
						//ChangeYaw(99);
					}

				}else{
					stareTime = 0;
				}


				if(pissedOffTime == -1){

					//if the enemy is facing me, not behind a wall, clearly looking at me...
					if(UTIL_IsFacing(m_hEnemy->pev, pev->origin, 0.2)){
						//a lower distance can make it attack.
						if(distanceFromEnemy < lookAtMePissRange){
							pissOff = TRUE;
						}
					}else{
						//this close?  Just attack already.
						//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("WELL DDDD %d", UTIL_IsFacingAway(m_hEnemy->pev, pev->origin, 0.3) )); 
						if(UTIL_IsFacingAway(m_hEnemy->pev, pev->origin, global_panthereyeJumpDotTol) ){
							//can jump...?
							if(distanceFromEnemy < 360 * 20){

								if(distanceFromEnemy < 130 * 20){
									//too close, just go as normal.
									pissOff = TRUE;
								}else{

									if(m_pSchedule != slPanthereyeJumpAtEnemy){
										//b/w 130 and 320?  JUMP
										RouteClear();
										ChangeSchedule(slPanthereyeJumpAtEnemy);
									}
								}

							}

						}else{

							if(distanceFromEnemy < distancePissRange){
								pissOff = TRUE;
							}

						}

					}

				}//END OF if(pissedOffTime == -1)
				else{
					if(!HasConditions(bits_COND_ENEMY_OCCLUDED) && HasConditions(bits_COND_SEE_ENEMY)){

						if(distanceFromEnemy < 300 * 20){
							//if mad, gets mad easier!
							pissOff = TRUE;
						}
					}


				}

			}else{
				//if occluded, reset stare time.
				stareTime = 0;
			}
			//EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("WHAT DA %d", HasConditions(bits_COND_LIGHT_DAMAGE)));
			//If attacked recently, be a bit more easy to attack, or flee.  "fight or flight"
			
			if(!pissOff && HasConditions(bits_COND_LIGHT_DAMAGE) || HasConditions(bits_COND_HEAVY_DAMAGE) ){
				if(distanceFromEnemy < fightOrFlightRange){
					pissOffHard = TRUE;
				}else{
					tryRunAway = TRUE;
				}
			}

			if(tryRunAway){
			
				BOOL startRunningAway = FALSE;
				if(runawayTime == -1){
					//initiate!
					startRunningAway = TRUE;
				}
				runawayTime = gpGlobals->time + 7;

				if(startRunningAway && pissedOffTime == -1){
					EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("!!!!! IM A STUPID <thing>"));
					ChangeSchedule(GetSchedule());
				}

			}

			if(pissOffHard || pissOff){
				//longer!
			
				BOOL startRunningAway = FALSE;
				if(pissedOffTime == -1){
					//initiate!
					startRunningAway = TRUE;
				}
			
				if(pissOffHard){
				
					//isCornered

				
					if(pissedOffTime < gpGlobals->time + 8){
						pissedOffTime = gpGlobals->time + 8;
					}

				}else{
					//make it so.  Check so that it doesn't override "pissOffHard" pushing past 2 seconds.
					if(pissedOffTime < gpGlobals->time + 3){
						pissedOffTime = gpGlobals->time + 3;
					}
				}

				runawayTime = -1;

				if(startRunningAway){
					EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("!!!!! IM A RAGING <thing>"));
					ChangeSchedule(GetSchedule());
				}

			}
		
		
		}//END OF if(m_hEnemy != NULL)
		else{
			//no enemy?  Reset all timers.
			runawayTime = -1;
			pissedOffTime = -1;
			stareTime = 0;
		}


		if(runawayTime != -1 ){

			if(runawayTime <= gpGlobals->time){
				//end!
				runawayTime = -1;

				if(m_pSchedule == slTakeCoverFromBestSound){
					//TaskFail();
					EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("!!!!! IM NOT A STUPID <thing>"));
					ChangeSchedule(GetSchedule());
					return;
				}

			}else{
				//still going.
				/*
				if(m_pSchedule != slTakeCoverFromBestSound){
					ChangeSchedule(slTakeCoverFromBestSound);
					return;
				}
				*/
			}
		}



		if(pissedOffTime != -1){
		
			if(pissedOffTime <= gpGlobals->time){
				//end!
				pissedOffTime = -1;

			
				//too crass of an interruption, try something else...?

				//was CIRCLIN
				if(m_pSchedule == slPanthereyeChaseEnemy){
					//TaskFail();
					EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("!!!!! IM NOT A RAGING <thing>"));
					ChangeSchedule(GetSchedule());
					return;
				}
			

			}else{
				//still going.

				/*
				if(m_pSchedule != slPanthereyeGetIntoCirclingRange){
					ChangeSchedule(slPanthereyeGetIntoCirclingRange);
					return;
				}
				*/

			}
		
		}


	}//END OF if(pev->deadflag == DEAD_NO && !deadSetActivityBlock && !iAmDead)
	else{

		//////easyForcePrintLine(".....???????? %d : %d   %d %d %d", monsterID, pTask->iTask, pev->deadflag, deadSetActivityBlock, iAmDead);
	}

	switch ( pTask->iTask ){
	case TASK_PANTHEREYE_FIND_COVER_FROM_ENEMY:
	{
		if(waitingForNewPath && newPathDelay == -1){
			panthereye_findCoverFromEnemy();
			newPathDelay = gpGlobals->time + newPathDelayDuration;
			waitingForNewPath = FALSE;
		}

		break;
	}
		
	case TASK_FACE_ENEMY:
		if(HasConditions(bits_COND_SEE_ENEMY)){
			m_vecEnemyLKP = m_hEnemy->pev->origin;
		}else{

			if(!HasConditions(bits_COND_ENEMY_OCCLUDED)){
				//if not occluded, try to see anyways?
				m_vecEnemyLKP = m_hEnemy->pev->origin;
			}else{
				//can't see the enemy?  Can't face them.
				TaskFail();
				return;
			}
		}

		if(faceLookTime <= gpGlobals->time){
			//nope.
			TaskFail();
			return;
		}

		MakeIdealYaw( m_vecEnemyLKP );

		//DEBUG!!!
		//UTIL_drawLineFrame(pev->origin, m_vecEnemyLKP, 9, 255, 0, 0);

		ChangeYaw( pev->yaw_speed );

		if ( FacingIdeal() )
		{
			TaskComplete();
		}
	break;

	case TASK_PANTHEREYE_COVER_FAIL_WAIT:
	case TASK_PANTHEREYE_SNEAK_WAIT:
		{
			EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("YOU ARE NOT SERIOUS."));

			if(timeTillSneakAgain <= gpGlobals->time || (!isCornered && isEnemyLookingAtMe) ){
				timeTillSneakAgain = -1;
				ChangeSchedule(GetSchedule());
				return;
			}
		break;
		}
		
		//copied from schedule.cpp & modified.
	case TASK_PANTHER_NORMALMOVE:
		{
			float distance;
			float targetDiscrepency;


			//is that okay?
			m_hTargetEnt = m_hEnemy;

			if ( m_hTargetEnt == NULL ){
				TaskFail();
				return;
			}else
			{

				distance = ( m_vecMoveGoal - pev->origin ).Length2D();
				targetDiscrepency = (m_vecMoveGoal - m_hTargetEnt->pev->origin).Length();

				
				m_vecMoveGoal = m_hTargetEnt->pev->origin;
				FRefreshRoute();

				if ( (distance < 230) || targetDiscrepency > pTask->flData * 0.5 )
				{
					m_vecMoveGoal = m_hTargetEnt->pev->origin;
					distance = ( m_vecMoveGoal - pev->origin ).Length2D();

					//MODDD - NOTE: is this really necessary??
					//FRefreshRoute();
				}
				//keep goin'

			}

			m_movementActivity = ACT_RUN;

			break;
		}
		case TASK_FACE_IDEAL:
			if(pTask->flData == 24){
				//flag: turn faster than usual.
				pev->yaw_speed = 500;
			}else if(pTask->flData == 25){
				pev->yaw_speed = 20;
			}
			CBaseMonster::RunTask(pTask);
		break;

		case TASK_PANTHEREYE_PLAYANIM_GETBUG:

			//this->SetSequenceByName("get_bug");

			if(this->m_fSequenceFinished){
				//end.
				TaskComplete();
				return;
			}

		break;
		case TASK_PANTHEREYE_PLAYANIM_CROUCHTOJUMP:

			if(m_hEnemy == NULL){
				//reset anim info?  Make sure other failure is considered too,.
				TaskFail();
				break;
			}
			
			//before the first event check...
			if(animEventQueueTime[0] != -1 && (animEventQueueTime[0] + animEventQueueStartTime > gpGlobals->time) ){
				//just the start of the anim, can still turn...

				MakeIdealYaw(m_hEnemy->pev->origin);

				ChangeYaw(42);
			}

			EASY_CVAR_PRINTIF_PRE(panthereyePrintout, easyPrintLine("WHAT THE : %.2f %d", pev->frame, m_fSequenceFinished));
			//TODO: an "onGround" check may be nice.
			if(this->m_fSequenceFinished){
				TaskComplete();
				//no leap-touch, if still here.
				SetTouch( NULL );
				return;
			}

		break;
		case TASK_WAIT_PVS:
			if(gpGlobals->time >= maxWaitPVSTime){
				TaskFail();
				return;
			}
			CBaseMonster::RunTask(pTask);
		break;

		case TASK_GET_PATH_TO_ENEMY:
			
			if(waitingForNewPath && newPathDelay == -1){
				//if we're waiting to try a path, go ahead (like start would).
				CBaseMonster::StartTask(pTask);
				newPathDelay = gpGlobals->time + newPathDelayDuration;
				waitingForNewPath = FALSE;
			}

		break;

		default:
			CBaseMonster::RunTask(pTask);
		break;
	}
}




















