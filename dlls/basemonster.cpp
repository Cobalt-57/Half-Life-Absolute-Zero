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
/*

===== basemonster.cpp (formerly monsters.cpp) ========================================================

  Monster-related utility code

*/

//MODDD TODO MAJOR - in BestVisibleEnemy, could distance be a scalable priotity alongside relationship, instead of relationship being
//                   100% overpowering?
//                   for instance, a monster 3 miles away that's a nemesis that is sighted would draw more attention
//                   than a big hulking agrunt that's 2 feet away slashing at me that's "hated" would.
//                   It's a bizarre picture.  At least in theory that's how it would work out, but see if it's such a problem it needs fixing.


#include "extdll.h"

#include "basemonster.h"  //MODDD - is this addition necessary?
#include "util.h"	
#include "nodes.h"
#include "util_model.h"
#include "saverestore.h"
#include "weapons.h"
#include "scripted.h"
#include "squadmonster.h"
#include "decals.h"
#include "soundent.h"
#include "gamerules.h"
//MODDD - never included before.?
#include "defaultai.h"

#include "util_debugdraw.h"
//MODDD - why not?
#include "game.h"
	

//MODDD - unused, now factors in the monster's current expected distance to cover this frame (groundspeed & frame time) 
//        for a better tolerance instead.  Still a hard minimum of 8 to pass just in case it would otherwise be too small.
//#define MONSTER_CUT_CORNER_DIST		8 // 8 means the monster's bounding box is contained without the box of the node in WC



//#define USE_MOVEMENT_BOUND_FIX
#define USE_MOVEMENT_BOUND_FIX_ALT



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//extern DLL_GLOBAL	BOOL	g_fDrawLines;
extern DLL_GLOBAL	short	g_sModelIndexLaser;// holds the index for the laser beam
extern DLL_GLOBAL	short	g_sModelIndexLaserDot;// holds the index for the laser beam dot

extern CGraph WorldGraph;// the world node graph

//MODDD - added
EASY_CVAR_EXTERN(germanCensorship)
EASY_CVAR_EXTERN(seeMonsterHealth)
EASY_CVAR_EXTERN(applyLKPPathFixToAll)
EASY_CVAR_EXTERN(crazyMonsterPrintouts)
EASY_CVAR_EXTERN(monsterSpawnPrintout)
EASY_CVAR_EXTERN(timedDamageAffectsMonsters)

EASY_CVAR_EXTERN(STUextraTriangH)
EASY_CVAR_EXTERN(STUextraTriangV)

EASY_CVAR_EXTERN(timedDamageEndlessOnHard)
extern float globalPSEUDO_canApplyGermanCensorship;
EASY_CVAR_EXTERN(allowGermanModels)
EASY_CVAR_EXTERN(cineAllowSequenceOverwrite)
EASY_CVAR_EXTERN(pathfindPrintout)
EASY_CVAR_EXTERN(pathfindFidgetFailTime)
EASY_CVAR_EXTERN(pathfindTopRampFixDistance)
EASY_CVAR_EXTERN(pathfindTopRampFixDraw)
EASY_CVAR_EXTERN(pathfindLooseMapNodes)
EASY_CVAR_EXTERN(pathfindRampFix)
EASY_CVAR_EXTERN(pathfindNodeToleranceMulti)
EASY_CVAR_EXTERN(pathfindSnapToNode)
EASY_CVAR_EXTERN(animationFramerateMulti)
EASY_CVAR_EXTERN(peaceOut)
EASY_CVAR_EXTERN(monsterAIForceFindDistance)
EASY_CVAR_EXTERN(movementIsCompletePrintout)
EASY_CVAR_EXTERN(animationPrintouts)
EASY_CVAR_EXTERN(pathfindEdgeCheck)
EASY_CVAR_EXTERN(thoroughHitBoxUpdates)
EASY_CVAR_EXTERN(animationKilledBoundsRemoval)
EASY_CVAR_EXTERN(drawDebugEnemyLKP)
EASY_CVAR_EXTERN(pathfindLargeBoundFix)
EASY_CVAR_EXTERN(flyerKilledFallingLoop)
EASY_CVAR_EXTERN(barnacleGrabNoInterpolation)

//EASY_CVAR_EXTERN(testVar)




// Global Savedata for monster
// UNDONE: Save schedule data?  Can this be done?  We may
// lose our enemy pointer or other data (goal ent, target, etc)
// that make the current schedule invalid, perhaps it's best
// to just pick a new one when we start up again.
TYPEDESCRIPTION	CBaseMonster::m_SaveData[] = 
{
	DEFINE_FIELD( CBaseMonster, m_hEnemy, FIELD_EHANDLE ),
	DEFINE_FIELD( CBaseMonster, m_hTargetEnt, FIELD_EHANDLE ),
	DEFINE_ARRAY( CBaseMonster, m_hOldEnemy, FIELD_EHANDLE, MAX_OLD_ENEMIES ),
	DEFINE_ARRAY( CBaseMonster, m_vecOldEnemy, FIELD_POSITION_VECTOR, MAX_OLD_ENEMIES ),

	//MODDD - new
	DEFINE_FIELD( CBaseMonster, m_intOldEnemyNextIndex, FIELD_INTEGER),


	DEFINE_FIELD( CBaseMonster, m_flFieldOfView, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flWaitFinished, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_flMoveWaitFinished, FIELD_TIME ),

	DEFINE_FIELD( CBaseMonster, m_Activity, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_IdealActivity, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_LastHitGroup, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_MonsterState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_IdealMonsterState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iTaskStatus, FIELD_INTEGER ),

	//Schedule_t			*m_pSchedule;

	DEFINE_FIELD( CBaseMonster, m_iScheduleIndex, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afConditions, FIELD_INTEGER ),
	
	//MODDD - new
	DEFINE_FIELD( CBaseMonster, m_afConditionsFrame, FIELD_INTEGER ),
	
	//WayPoint_t			m_Route[ ROUTE_SIZE ];
//	DEFINE_FIELD( CBaseMonster, m_movementGoal, FIELD_INTEGER ),
//	DEFINE_FIELD( CBaseMonster, m_iRouteIndex, FIELD_INTEGER ),
//	DEFINE_FIELD( CBaseMonster, m_moveWaitTime, FIELD_FLOAT ),

	DEFINE_FIELD( CBaseMonster, m_vecMoveGoal, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_movementActivity, FIELD_INTEGER ),

	//		int				m_iAudibleList; // first index of a linked list of sounds that the monster can hear.
//	DEFINE_FIELD( CBaseMonster, m_afSoundTypes, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_vecLastPosition, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_iHintNode, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afMemory, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iMaxHealth, FIELD_INTEGER ),

	DEFINE_FIELD( CBaseMonster, m_vecEnemyLKP, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CBaseMonster, m_cAmmoLoaded, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_afCapability, FIELD_INTEGER ),

	//MODDD - removed.
	//DEFINE_FIELD( CBaseMonster, m_flNextAttack, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_bitsDamageType, FIELD_INTEGER ),
	DEFINE_ARRAY( CBaseMonster, m_rgbTimeBasedDamage, FIELD_CHARACTER, CDMG_TIMEBASED ),
	DEFINE_FIELD( CBaseMonster, m_bloodColor, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_failSchedule, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, hardSetFailSchedule, FIELD_BOOLEAN ),  //MODDD - saved now too.
	DEFINE_FIELD( CBaseMonster, scheduleSurvivesStateChange, FIELD_BOOLEAN ),  //MODDD - saved now too.

	
	

	DEFINE_FIELD( CBaseMonster, m_flHungryTime, FIELD_TIME ),
	DEFINE_FIELD( CBaseMonster, m_flDistTooFar, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_flDistLook, FIELD_FLOAT ),
	DEFINE_FIELD( CBaseMonster, m_iTriggerCondition, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_iszTriggerTarget, FIELD_STRING ),

	DEFINE_FIELD( CBaseMonster, m_HackedGunPos, FIELD_VECTOR ),

	DEFINE_FIELD( CBaseMonster, m_scriptState, FIELD_INTEGER ),
	DEFINE_FIELD( CBaseMonster, m_pCine, FIELD_CLASSPTR ),

	//MODDD - is this a good idea?
	DEFINE_FIELD( CBaseMonster, monsterID, FIELD_INTEGER ),

};






//MODDD - moved from method iRelationship to instance scope. Accessible from other places now.
//   And why did ALIENMONSTER and ALIENPREY have a R_NO with MACHINE? Machines (like sentries, part of security or human military) certainly hate them.
//static int iEnemy[14][14] =
int CBaseMonster::iEnemy[14][14] =
{			 //   NONE	 MACH	 PLYR	 HPASS	 HMIL	 AMIL	 APASS	 AMONST	APREY	 APRED	 INSECT	PLRALY	PBWPN	ABWPN
/*NONE*/		{ R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO,	R_NO,	R_NO	},
/*MACHINE*/		{ R_NO	,R_NO	,R_DL	,R_DL	,R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_DL,	R_DL,	R_DL	},
/*PLAYER*/		{ R_NO	,R_DL	,R_NO	,R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_NO,	R_DL,	R_DL	},
/*HUMANPASSIVE*/{ R_NO	,R_NO	,R_AL	,R_AL	,R_HT	,R_FR	,R_NO	,R_HT	,R_DL	,R_FR	,R_NO	,R_AL,	R_NO,	R_NO	},
/*HUMANMILITAR*/{ R_NO	,R_NO	,R_HT	,R_DL	,R_NO	,R_HT	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_HT,	R_NO,	R_NO	},
/*ALIENMILITAR*/{ R_NO	,R_DL	,R_HT	,R_DL	,R_HT	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_DL,	R_NO,	R_NO	},
/*ALIENPASSIVE*/{ R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO,	R_NO,	R_NO	},
/*ALIENMONSTER*/{ R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_NO	,R_DL,	R_NO,	R_NO	},
/*ALIENPREY   */{ R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_NO	,R_NO	,R_NO	,R_FR	,R_NO	,R_DL,	R_NO,	R_NO	},
/*ALIENPREDATO*/{ R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_NO	,R_NO	,R_HT	,R_DL	,R_NO	,R_DL,	R_NO,	R_NO	},
/*INSECT*/		{ R_FR	,R_FR	,R_FR	,R_FR	,R_FR	,R_NO	,R_FR	,R_FR	,R_FR	,R_FR	,R_NO	,R_FR,	R_NO,	R_NO	},
/*PLAYERALLY*/	{ R_NO	,R_DL	,R_AL	,R_AL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_NO,	R_NO,	R_NO	},
/*PBIOWEAPON*/	{ R_NO	,R_NO	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_DL	,R_NO	,R_DL,	R_NO,	R_DL	},
/*ABIOWEAPON*/	{ R_NO	,R_NO	,R_DL	,R_DL	,R_DL	,R_AL	,R_NO	,R_NO	,R_DL	,R_NO	,R_NO	,R_DL,	R_DL,	R_NO	}
};





const char* CBaseMonster::pStandardAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};

const char* CBaseMonster::pStandardAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};




int CBaseMonster::monsterIDLatest = 0;


//MODDD - ?
//WARNING: "endVec" is unused, this still gets the enemy's BodyTargetMod manually (ignores the player's randomness of BodyTarget).
BOOL CBaseMonster::NoFriendlyFireImp(const Vector& startVec, const Vector& endVec){
	
	//easyForcePrintLine("NoFriendlyFireImp %s:%d start: %.2f %.2f %.2f", getClassname(), monsterID, m_vecEnemyLKP.x, m_vecEnemyLKP.y, m_vecEnemyLKP.z);
	//Vector shootOrigin = pev->origin + Vector( 0, 0, 55 );
	TraceResult tr;

	CBaseEntity *pEnemy = m_hEnemy;

	//Vector shootTarget = ( (pEnemy->BodyTargetMod( startVec ) - pEnemy->pev->origin) + m_vecEnemyLKP );
	Vector shootTarget = pEnemy->BodyTargetMod( startVec);
	
	
	UTIL_TraceLine( startVec, shootTarget, dont_ignore_monsters, ignore_glass, ENT(pev), &tr );
	//UTIL_TraceLine( startVec, shootTarget, ignore_monsters, ignore_glass, ENT(pev), &tr );
	
	//Vector vecSrc = GetGunPosition();
	//UTIL_TraceLine( vecSrc, m_hEnemy->BodyTargetMod(vecSrc), ignore_monsters, ignore_glass, ENT(pev), &tr);


	//m_checkAttackTime = gpGlobals->time + 1;

	if(tr.pHit == NULL){
		//wait, why is this FALSE? isn't it ok if there is nothing on the other side?
		//it says "NoFriendlyFireImp" you dip!
		//easyForcePrintLine("NoFriendlyFireImp: Flag 1");
		return TRUE;
	}
	CBaseEntity* tempTarget = CBaseEntity::Instance(tr.pHit);


	int reltemp = R_NO;  //default when there isn't a hit.
	
	if(tempTarget != NULL){
		reltemp = IRelationship(tempTarget);
	}

	//easyForcePrintLine("NoFriendlyFireImp: Flag 2: %.2f %d %d %d %d",  tr.flFraction, tempTarget==NULL, (tempTarget == pEnemy), reltemp  > R_NO, reltemp == R_FR );
	//tempTarget being NULL really shouldn't be possible, but this doesn't hurt.


	BOOL isInanimate = FALSE;
	if(tr.pHit != NULL){
		//CBaseEntity* test = CBaseEntity::Instance(tr.pHit->v.class);
		if( !strcmp( STRING(tr.pHit->v.classname), "func_breakable")){
			isInanimate = TRUE;  //ok to break me.
		}
	}

	//easyForcePrintLine("NoFriendlyFireImp: Hit: %s Relationship: %d isInanimate:%d", tr.pHit!=NULL?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL", reltemp, isInanimate);

	if ( (!tr.fStartSolid && tr.flFraction == 1.0) || tempTarget==NULL || (tempTarget == pEnemy) || (reltemp  > R_NO || reltemp == R_FR  ) || isInanimate   ){
		//line of fire not interrupted, whatever interrupted it was the intended enemy, or whatever interrutped it is hated to some extent (Dislike to Nemesis) or feared? We can fire.
		return TRUE;
	}else{
		if(tempTarget != NULL){
			//easyForcePrintLine("NoFriendlyFireImp: FAIL! colliding ent: %s", tempTarget->getClassname());
		}else{
			//easyForcePrintLine("NoFriendlyFireImp: FAIL! colliding ent: %s", "NULL!");
		}
		//line of fire interrupted by a non-enemy with relationship <= R_NO and != R_FR? This is a friendly or non-hostile, block this attack.
		return FALSE;
	}
	//m_checkAttackTime = gpGlobals->time + 1.5;
}//END OF NoFriendlyFireImp


//MODDD - new
CBaseMonster::CBaseMonster(){
	

	timeOfDeath_activity = ACT_RESET;
	timeOfDeath_sequence = 0;

	lastDamageReceived = 0;
	
	forgetSmallFlinchTime = -1;
	forgetBigFlinchTime = -1;

	floatSinkSpeed = WATER_DEAD_SINKSPEED_INITIAL;
	oldWaterLevel = -1;


	hardSetFailSchedule = FALSE;
	scheduleSurvivesStateChange = FALSE;

	disableEnemyAutoNode = FALSE;

	m_fNewScheduleThisFrame = FALSE;

	forceNoDrop = FALSE;
	debugFailColor = FALSE;

	debugVectorMode = -1;
	debugVectorsSet = FALSE;

	deadSetActivityBlock = FALSE;

	blockDamage = FALSE;
	buddhaMode = FALSE;
	blockTimedDamage = FALSE;

	forceFlyInterpretation = FALSE;

	drawPathConstant = FALSE;
	drawFieldOfVisionConstant = FALSE;

	monsterID = -1;

	targetIsDeadException = FALSE;

	signalActivityUpdate = FALSE;

	barnacleLocked = FALSE;
	canSetAnim = TRUE;

	//MODDD TODO - we don't yet restore the queued state if the monster loses the PRONE state. Should we? this may not be very important.
	queuedMonsterState = MONSTERSTATE_NONE;
	//by default, can auto set animations from activity changes.

	canRangedAttack1 = FALSE;
	canRangedAttack2 = FALSE;


	//no by default.
	canDrawDebugSurface = FALSE;

	//default.
	m_flFramerateSuggestion = 1;

	
	fApplyTempVelocity = FALSE;
	//velocityApplyTemp;

}

//All monsters use this feature by default. Just not necessarily all entities.
BOOL CBaseMonster::usesSoundSentenceSave(void){
	return TRUE;
}



/*
//done in cbase.h (CBaseEntity) instead.
BOOL CBaseMonster::isBarnacleVictimException(){
	//by default, no monster is an "exception" to what the relationship says (mostly non-aliens).
	return FALSE;
}
*/


BOOL CBaseMonster::isSizeGiant(void){
	//most monsters are not drastically larger than the player (the Gargantua is).
	return FALSE;
}

//By default, all monsters are organic. This is easier since most fall under that category.
//Just make sentries / other robotics exceptions (return FALSE to this)
BOOL CBaseMonster::isOrganic(){
	return TRUE;
}

BOOL CBaseMonster::isOrganicLogic(){
	//This means, if this monster is organic.. of course it's OrganicLogic.
	//But if it isn't organic, and this monster is using a german model replacement, we're going to treat it like it was organic.
	//Specific monsters can also just make this method return TRUE or FALSE constantly for consistent behavior for
	//dropping carcass scents at death.
	//This includes an ordinary "isOrganic" check so checking both isn't usually necessary.
	return (isOrganic() || (GermanModelOrganicLogic() && getGermanModel()!=NULL ));
}




float CBaseMonster::getBarnaclePulledTopOffset(void){
	//by default, no offset, the default offset of -8 works.  This, if non-zero, is added to that one.
	return 0;
}
float CBaseMonster::getBarnacleForwardOffset(void){
	return 0;
}
//NOTICE: "1", 100%, is the default.
float CBaseMonster::getBarnacleAnimationFactor(void){
	return 1;
}




//MODDD - just a utility method.
//It is ripped & slightly adjusted from the houndeye's "setActivity" method portion that forces an idle animation.
//...WAIT.  Is that what "SetSequenceByName" ?.

void CBaseMonster::setAnimation(char* animationName){
	setAnimation(animationName, FALSE, -1, 0);
}

void CBaseMonster::setAnimation(char* animationName, BOOL forceException){
	setAnimation(animationName, forceException, -1, 0);
}

void CBaseMonster::setAnimation(char* animationName, BOOL forceException, BOOL forceLoopsProperty){
	setAnimation(animationName, forceException, forceLoopsProperty, 0);
}

void CBaseMonster::setAnimation(char* animationName, BOOL forceException, BOOL forceLoopsProperty, int extraLogic){

	
	int iSequence = LookupSequence( animationName );
	
	//easyPrintLine("OH YOU despicable bastard FROM HELL extralogic: %d", extraLogic);
	//easyPrintLine("OH YOU despicable bastard FROM HELL %s", animationName);

	if ( forceException || (canSetAnim == TRUE) && iSequence > ACTIVITY_NOT_AVAILABLE )
	{

		BOOL pass = FALSE;
		if(extraLogic == 2){
			//can only set animation if it isn't already the desired one, or if it is finished.
			if(iSequence != pev->sequence || m_fSequenceFinished){
				pass = TRUE;
			}

		}else if(extraLogic == 1){
			//can pass if we're interrupting something, but NOT if we already have the right sequence here.
			
			//ONLY allow to change if not already correct.
			if(iSequence != pev->sequence){
				pass = TRUE;
			}

		}else if(extraLogic == 3){

			if(iSequence != pev->sequence || m_fSequenceFinished){
				pass = TRUE;
			}

		}else if(extraLogic == 4){

			if(m_fSequenceFinished){
				//only.
				pass = TRUE;
			}

		}else if(extraLogic == 5){
			//INTERRUPT ANYTHING
			pass = TRUE;

		}else{
			pass = TRUE;
		}

		if(!pass){
			return;
		}

		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)
		pev->frame			= 0;		// FIX: frame counter shouldn't be reset when its the same activity as before
		//???
		//pev->framerate = 1.0f;
		if(extraLogic <= 2 && extraLogic != 1){
			//easyPrintLine("@@@@@ SEQUENCE INFO RESET %s %d", animationName, extraLogic);
			ResetSequenceInfo();
		}else{
			//easyPrintLine("@@@@@ SEQUENCE INFO SAFE %s %d", animationName, extraLogic);
			ResetSequenceInfoSafe();
		}
		usingCustomSequence = TRUE;
		SetYawSpeed();

		if(forceLoopsProperty != -1){
			m_fSequenceLoops = forceLoopsProperty;
		}
	}
	//canSetAnim == FALSE;
	
}








//extern Schedule_t* slAnimationSmartAndStop;
//extern Schedule_t* slAnimationSmart;



void CBaseMonster::setAnimationSmart(const char* arg_animName){
	setAnimationSmart(arg_animName, -999);
}


//modeled moreso after "setSequenceByName".
void CBaseMonster::setAnimationSmart(const char* arg_animName, float arg_frameRate){

	int iSequence;

	iSequence = LookupSequence ( arg_animName );

	setAnimationSmart(iSequence, arg_frameRate);
	
}

void CBaseMonster::setAnimationSmart(int arg_animIndex, float arg_frameRate){
	
	int iSequence = arg_animIndex;

	// Set to the desired anim, or default anim if the desired is not present
	if ( iSequence > ACTIVITY_NOT_AVAILABLE )
	{
		if(m_flFramerateSuggestion != -999){
			m_flFramerateSuggestion = arg_frameRate;
		}else{
			m_flFramerateSuggestion = 1;
		}

		if ( pev->sequence != iSequence || !m_fSequenceLoops )
		{
			resetFrame();
		}

		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)
		ResetSequenceInfoSafe();
		usingCustomSequence = TRUE;
		SetYawSpeed();
	}
	else
	{
		// Not available try to get default anim
		pev->sequence		= 0;	// Set to the reset anim (if it's there)
		usingCustomSequence = FALSE;
	}
	//pev->framerate = arg_frameRate;

	//NO, ditched.
	//wait for a delay, or until m_fSequenceFinished?
	//this->ChangeSchedule(slAnimationSmart);
}


void CBaseMonster::setAnimationSmartAndStop(const char* arg_animName){
	setAnimationSmartAndStop(arg_animName, -999);
}
//modeled moreso after "setSequenceByName".
void CBaseMonster::setAnimationSmartAndStop(const char* arg_animName, float arg_frameRate){

	int iSequence;

	iSequence = LookupSequence ( arg_animName );

	setAnimationSmartAndStop(iSequence, arg_frameRate);
	
}


//modeled moreso after "setSequenceByName"... also stops MOVEMENT, that is (if it wasn't already?)
void CBaseMonster::setAnimationSmartAndStop(int arg_animIndex, float arg_frameRate){

	int iSequence = arg_animIndex;

	// Set to the desired anim, or default anim if the desired is not present
	if ( iSequence > ACTIVITY_NOT_AVAILABLE )
	{
		if(m_flFramerateSuggestion != -999){
			m_flFramerateSuggestion = arg_frameRate;
		}else{
			m_flFramerateSuggestion = 1;
		}


		if ( pev->sequence != iSequence || !m_fSequenceLoops )
		{
			resetFrame();
		}
		
		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)
		ResetSequenceInfo();
		usingCustomSequence = TRUE;
		SetYawSpeed();
	}
	else
	{
		// Not available try to get default anim
		pev->sequence		= 0;	// Set to the reset anim (if it's there)
		usingCustomSequence = FALSE;
	}
	//pev->framerate = arg_frameRate;
	//wait for a delay, or until m_fSequenceFinished?
	//this->ChangeSchedule(slAnimationSmartAndStop);
	
}











//IMPLEMENT_SAVERESTORE( CBaseMonster, CBaseToggle );
int CBaseMonster::Save( CSave &save )
{
	if ( !CBaseToggle::Save(save) )
		return 0;
	return save.WriteFields( "CBaseMonster", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}

int CBaseMonster::Restore( CRestore &restore )
{
	if ( !CBaseToggle::Restore(restore) )
		return 0;
	int status = restore.ReadFields( "CBaseMonster", this, m_SaveData, ARRAYSIZE(m_SaveData) );
	
	// We don't save/restore routes yet
	RouteClear();

	// We don't save/restore schedules yet
	m_pSchedule = NULL;
	m_iTaskStatus = TASKSTATUS_NEW;
	
	// Reset animation
	m_Activity = ACT_RESET;

	// If we don't have an enemy, clear conditions like see enemy, etc.
	if ( m_hEnemy == NULL ){
		clearAllConditions();
	}

	PostRestore();

	return status;
}


void CBaseMonster::PostRestore(){
	//easyForcePrintLine("PostRestore: CBaseMonster: %s", getClassname(), monsterID);
}


//=========================================================
// Eat - makes a monster full for a little while.
//=========================================================
void CBaseMonster :: Eat ( float flFullDuration )
{
	m_flHungryTime = gpGlobals->time + flFullDuration;
}

//=========================================================
// FShouldEat - returns true if a monster is hungry.
//=========================================================
BOOL CBaseMonster :: FShouldEat ( void )
{
	if ( m_flHungryTime > gpGlobals->time )
	{
		return FALSE;
	}

	return TRUE;
}

//=========================================================
// BarnacleVictimBitten - called
// by Barnacle victims when the barnacle pulls their head
// into its mouth
//=========================================================
void CBaseMonster :: BarnacleVictimBitten ( entvars_t *pevBarnacle )
{
	Schedule_t	*pNewSchedule;

	pNewSchedule = GetScheduleOfType( SCHED_BARNACLE_VICTIM_CHOMP );

	if ( pNewSchedule )
	{
		ChangeSchedule( pNewSchedule );
	}

}

//=========================================================
// BarnacleVictimReleased - called by barnacle victims when
// the host barnacle is killed.
//=========================================================
void CBaseMonster :: BarnacleVictimReleased ( void )
{
	m_IdealMonsterState = MONSTERSTATE_IDLE;

	pev->velocity = g_vecZero;

	//MODDD NOTE - the old movetype is step? quite an assumption, careful.
	//  Would resetting the origin be safe? turn off a onGround flag? etc.
	//this bit is new though.
	UTIL_SetOrigin(pev, pev->origin);

	pev->movetype = MOVETYPE_STEP;

	
	//MODDD - turn interpolation back on when released
	if(EASY_CVAR_GET(barnacleGrabNoInterpolation) == 1){
		pev->effects &= ~EF_NOINTERP;
	}

}


void CBaseMonster::testMethod(void){
	easyPrintLine("test message goes here.");
}






//TODO: put something in the bis_SOUND_BAIT sound that records who / what made the noise, and who / what is allowed to be interested in it instead?
//This method assumes it is a chumtoad croak. Only CLASS_ALIEN_MONSTER and CLASS_ALIEN_PREDATOR investigate that.
//Also, must not be in a COMBAT state. IDLE and ACTIVE are ok.
BOOL CBaseMonster::interestedInBait(int arg_classID){

	BOOL test = (arg_classID == CLASS_ALIEN_MILITARY || arg_classID == CLASS_ALIEN_MONSTER || arg_classID ==CLASS_ALIEN_PREDATOR) && (m_MonsterState == MONSTERSTATE_IDLE || m_MonsterState == MONSTERSTATE_ALERT);
	//easyForcePrintLine("SO??? %d : %d --- %d", arg_classID, this->Classify(), test);
	return test;
}


SCHEDULE_TYPE CBaseMonster::getHeardBaitSoundSchedule(CSound* pSound){
	

	//Yes, this condition is copy pasted. Preserve it to the other getHeardBaitSoundSchedule that does not take a sound!
	if( interestedInBait(this->Classify() )  ){
		_getHeardBaitSoundSchedule(pSound);
	}
	
	return SCHED_NONE;

}//END OF getHeardBaitSoundSchedule(...)

SCHEDULE_TYPE CBaseMonster::_getHeardBaitSoundSchedule(CSound* pSound){
	if ( pSound && (pSound->m_iType & bits_SOUND_BAIT) )
	{
		//MODDD TODO - shouldn't the INVESTIGATE_SOUND_BAIT schedule better handle that?
		//if we can directly see the bait and are less than X units away (distance), we'll just look at it instead.
		if( (pSound->m_vecOrigin - EarPosition()).Length() < 200 && FInViewCone( &pSound->m_vecOrigin ) && this->CheckLocalMove(this->pev->origin + Vector(0, 0, 4), pSound->m_vecOrigin+ Vector(0, 0, 4), NULL, NULL ) ){
			//look at it instead.
			easyForcePrintLine("%s:ID%d LOOKIN AT THE BAIT!", this->getClassname(), this->monsterID);
			//return GetScheduleOfType( SCHED_ALERT_FACE);
			return SCHED_ALERT_FACE;
		}	
		easyForcePrintLine("%s:ID%d BAIT NOISE, FOLLOW!", this->getClassname(), this->monsterID);
		//return GetScheduleOfType( SCHED_INVESTIGATE_SOUND_BAIT );
		return SCHED_INVESTIGATE_SOUND_BAIT;
	}
	return SCHED_NONE;
}//END OF getHeardBaitSoundSchedule(...)



	

SCHEDULE_TYPE CBaseMonster::getHeardBaitSoundSchedule(){
	
	//MODDD - crude addition.
	//If the monsterstate is IDLE or ALERT (not focused like in COMBAT), we need to investigate the BAIT sound.
	
	//Yes, this condition is copy pasted. Preserve it to the other getHeardBaitSoundSchedule that takes a sound!
	if( interestedInBait(this->Classify()  ) ){
		
		
	    //We need to check out bait sounds!
		if ( HasConditions ( bits_COND_HEAR_SOUND ))
		{
			CSound *pSound;
			pSound = PBestSound();

			ASSERT( pSound != NULL );

			//don't bother if we don't even have the sound.
			if(pSound == NULL) //goto skipSoundInvestigate;
			return SCHED_NONE;
			
			return CBaseMonster::_getHeardBaitSoundSchedule(pSound);

		}
		
	}//END OF m_MonsterState IDLE or ALERT check.


	//skipSoundInvestigate:
	return SCHED_NONE;
}//END OF getHeardBaitSoundSchedule(...)


//=========================================================
// Listen - monsters dig through the active sound list for
// any sounds that may interest them. (smells, too!)
//=========================================================
void CBaseMonster :: Listen ( void )
{
	int	iSound;
	int	iMySounds;
	float hearingSensitivity;
	CSound	*pCurrentSound;



	m_iAudibleList = SOUNDLIST_EMPTY; 
	ClearConditions(bits_COND_HEAR_SOUND | bits_COND_SMELL | bits_COND_SMELL_FOOD);
	m_afSoundTypes = 0;

	iMySounds = ISoundMask();

	if ( m_pSchedule )
	{
		//!!!WATCH THIS SPOT IF YOU ARE HAVING SOUND RELATED BUGS!
		// Make sure your schedule AND personal sound masks agree!
		
		//TEST MAJOR
		iMySounds &= m_pSchedule->iSoundMask;
	}
	

	//bits_SOUND_COMBAT
	if(FClassnameIs(pev, "monster_stukabat")){
		//different.
		//stukabats ALWAYS hear combat.
		
		iMySounds |= bits_SOUND_PLAYER;
		iMySounds |= bits_SOUND_COMBAT;
		iMySounds |= bits_SOUND_MEAT;
		iMySounds |= bits_SOUND_CARCASS;
		
	}

	//1, 4
	//2^0, 2^2

	iSound = CSoundEnt::ActiveList();

	// UNDONE: Clear these here?
	ClearConditions( bits_COND_HEAR_SOUND | bits_COND_SMELL_FOOD | bits_COND_SMELL );
	hearingSensitivity = HearingSensitivity( );

	while ( iSound != SOUNDLIST_EMPTY )
	{
		pCurrentSound = CSoundEnt::SoundPointerForIndex( iSound );
		

		//easyForcePrintLine("%s:ID%d CAN I HEAR THAT? %d", this->getClassnameShort(), this->monsterID, pCurrentSound->m_iVolume);


		//this->testMethod();

		//was this here before?  I forget.
		//if ( ( g_pSoundEnt->m_SoundPool[ iSound ].m_iType & iMySounds ) && ( g_pSoundEnt->m_SoundPool[ iSound ].m_vecOrigin - EarPosition()).Length () <= g_pSoundEnt->m_SoundPool[ iSound ].m_iVolume * hearingSensitivity ) 

		//MY NAME IS monster_stukabat::: 0 5 1     29 5
		//easyPrintLine("MY NAME IS %s::: %d %d %d     %d %d", STRING(this->pev->classname), pCurrentSound==NULL, (pCurrentSound->m_iType & iMySounds), ((pCurrentSound->m_vecOrigin - EarPosition() ).Length() <= pCurrentSound->m_iVolume * hearingSensitivity), iMySounds, pCurrentSound->m_iType ) ;
		if ( pCurrentSound	&& 
			 ( pCurrentSound->m_iType & iMySounds )	&& 
			 ( ( pCurrentSound->m_vecOrigin - EarPosition() ).Length() <= pCurrentSound->m_iVolume * hearingSensitivity ) //&&
			//MODDD - new. It is possible this sound has its own other requirement, presumably set by whatever made it. Such as, chumtoad croaks only being heard by non ALIEN_PREY.
			//((pCurrentSound->canListenHandle == NULL) || (pCurrentSound->canListenHandle(this) == TRUE) )

		)
		{
				//easyForcePrintLine("%s:ID%d I HEARD IT.", this->getClassnameShort(), this->monsterID);




 			// the monster cares about this sound, and it's close enough to hear.
			//g_pSoundEnt->m_SoundPool[ iSound ].m_iNextAudible = m_iAudibleList;
			pCurrentSound->m_iNextAudible = m_iAudibleList;
			
			
			if ( pCurrentSound->FIsSound() )
			{
				// this is an audible sound.
				
				//easyForcePrintLine("%s:ID%d I SO HEARD IT IT.", this->getClassnameShort(), this->monsterID);
				SetConditions( bits_COND_HEAR_SOUND );
			}
			else
			{
				// if not a sound, must be a smell - determine if it's just a scent, or if it's a food scent
//				if ( g_pSoundEnt->m_SoundPool[ iSound ].m_iType & ( bits_SOUND_MEAT | bits_SOUND_CARCASS ) )
				if ( pCurrentSound->m_iType & ( bits_SOUND_MEAT | bits_SOUND_CARCASS ) )
				{

					if(FClassnameIs(pev, "monster_stukabat")){
						//easyPrintLine("ahhhhhhhhhhhhhhhhhhhhhh");
					}	

					// the detected scent is a food item, so set both conditions.
					// !!!BUGBUG - maybe a virtual function to determine whether or not the scent is food?
					SetConditions( bits_COND_SMELL_FOOD );
					SetConditions( bits_COND_SMELL );
				}
				else
				{
					// just a normal scent. 
					SetConditions( bits_COND_SMELL );
				}
			}

//			m_afSoundTypes |= g_pSoundEnt->m_SoundPool[ iSound ].m_iType;
			m_afSoundTypes |= pCurrentSound->m_iType;

			m_iAudibleList = iSound;
		}

//		iSound = g_pSoundEnt->m_SoundPool[ iSound ].m_iNext;
		iSound = pCurrentSound->m_iNext;
	}


	
	if(FClassnameIs(pev, "monster_stukabat")){
		//easyPrintLine("HOLY der MOTHERderER %d %d",  HasConditions( bits_COND_HEAR_SOUND ),  HasConditions( bits_COND_SMELL_FOOD ) );
	}	

	//SetConditions(bits_COND_SMELL_FOOD);

	//easyPrintLine("MY NAME IS %s::: AND I HEAR %d", STRING(this->pev->classname), HasConditions( bits_COND_HEAR_SOUND )) ;
		

}

//=========================================================
// FLSoundVolume - subtracts the volume of the given sound
// from the distance the sound source is from the caller, 
// and returns that value, which is considered to be the 'local' 
// volume of the sound. 
//=========================================================
float CBaseMonster :: FLSoundVolume ( CSound *pSound )
{
	return ( pSound->m_iVolume - ( ( pSound->m_vecOrigin - pev->origin ).Length() ) );
}

//=========================================================
// FValidateHintType - tells use whether or not the monster cares
// about the type of Hint Node given
//=========================================================
BOOL CBaseMonster :: FValidateHintType ( short sHint )
{
	return FALSE;
}

//=========================================================
// Look - Base class monster function to find enemies or 
// food by sight. iDistance is distance ( in units ) that the 
// monster can see.
//
// Sets the sight bits of the m_afConditions mask to indicate
// which types of entities were sighted.
// Function also sets the Looker's m_pLink 
// to the head of a link list that contains all visible ents.
// (linked via each ent's m_pLink field)
//
//=========================================================
//MODDD - why was a distance (the parameter here) an int?  why not a decimal?
void CBaseMonster :: Look ( float flDistance )
{
	int iSighted = 0;

	// DON'T let visibility information from last frame sit around!
	ClearConditions(bits_COND_SEE_HATE | bits_COND_SEE_DISLIKE | bits_COND_SEE_ENEMY | bits_COND_SEE_FEAR | bits_COND_SEE_NEMESIS | bits_COND_SEE_CLIENT);


	
	if(EASY_CVAR_GET(peaceOut) == 1){
		//like, no way, man. Let's just smoke a fat blunt and sit on the couch all day.
		return;
	}

	/*
	int chuckTesta = 6666;
	int chuckTestb = 6667;
	const char* MYCLASSNAMEDAMN = getClassname();
	const char* MYCLASSNAMEDAMNALT = STRING(pev->classname);
	BOOL doesThatThingEqualMyThing = strcmp(MYCLASSNAMEDAMN, "monster_barney");

	BOOL thingResult = FClassnameIs(pev, "monster_barney");
	if(FClassnameIs(pev, "monster_barney") == TRUE ){
		int x = 666;
		chuckTesta = 1337;
	}

	if(FClassnameIs(pev, "monster_barney") == TRUE ){
		int x = 666;
	}

	if(thingResult){
		int x = 666;
		chuckTestb = 1338;
	}




	BOOL test2 = FStrEq(STRING(pev->classname), "monster_barney");
	BOOL test3 = strcmp(STRING(pev->classname), "monster_barney");


	BOOL whatTheHay = FClassnameIs(pev, "monster_barney");
	*/



	m_pLink = NULL;

	CBaseEntity	*pSightEnt = NULL;// the current visible entity that we're dealing with



	// See no evil if prisoner is set
	if ( !FBitSet( pev->spawnflags, SF_MONSTER_PRISONER ) )
	{
		CBaseEntity *pList[100];

		Vector delta = Vector( flDistance, flDistance, flDistance );

		// Find only monsters/clients in box, NOT limited to PVS
		int count = UTIL_EntitiesInBox( pList, 100, pev->origin - delta, pev->origin + delta, FL_CLIENT|FL_MONSTER );
		for ( int i = 0; i < count; i++ )
		{
			pSightEnt = pList[i];
			// !!!temporarily only considering other monsters and clients, don't see prisoners
			

			//Good for doing breakpoints with.
			/*
			const char* sightedClassname = "derp";
			BOOL sightedEntStillAliveByAI = FALSE;
			if(pSightEnt != NULL){
				sightedClassname = pSightEnt->getClassname();
				sightedEntStillAliveByAI = IsAlive_FromAI(this);
			}

			
			const char* enemyClassname = "DERR";
			const char* myClassname = getClassname();
			if(m_hEnemy != NULL){
				enemyClassname = m_hEnemy->getClassname();
			}else{

			}
			int yyy = 666;


			int x = 666;

			BOOL isThatThingAlive = pSightEnt->IsAlive_FromAI(this);
			
			int x2 = 666;
			*/

			if ( pSightEnt != this												&& 
				 !FBitSet( pSightEnt->pev->spawnflags, SF_MONSTER_PRISONER )	&& 
				 
				 //MODDD - exluding this for now.  Still think it's alive for a tiny bit during the death animation.
				 //pSightEnt->pev->health > 0 &&
				 
				 //MODDD - new conditions. Must be alive... wait, this really wasn't here before?! Without a check like this, the conditions can still be flagged
				 //and cause the AI to act as though there is some hostile monster forwards, such as stopping a monster fooled by a chumtoad from wandering off
				 //the millisecond it gets that schedule because the chumtoad playing dead is in front of it.
				 pSightEnt->IsAlive_FromAI(this)

				 )
			{

				
				//if(FClassnameIs(pev, "monster_chumtoad"))easyForcePrintLine("DOES IT PASS??? %s %d %d %d %d", pSightEnt->getClassnameShort(), (IRelationship( pSightEnt ) != R_NO), FInViewCone( pSightEnt ), (!FBitSet( pSightEnt->pev->flags, FL_NOTARGET )), FVisible( pSightEnt )   );

				// the looker will want to consider this entity
				// don't check anything else about an entity that can't be seen, or an entity that you don't care about.
				
				//OKAY.  You are not too bright are ya.
				/*
				int daRelation = IRelationship(pSightEnt);
				BOOL isInDaViewcone = FInViewCone(pSightEnt);
				BOOL isVisaba = FVisible(pSightEnt);
				BOOL isDaFlagSet = FBitSet(pSightEnt->pev->flags, FL_NOTARGET);
				int xxx = 666;
				*/
				
				
				if ( IRelationship( pSightEnt ) != R_NO && FInViewCone( pSightEnt ) && !FBitSet( pSightEnt->pev->flags, FL_NOTARGET ) && FVisible( pSightEnt ) )
				{
					if(EASY_CVAR_GET(crazyMonsterPrintouts)){
						easyPrintLine("FLAGGER 64 1");
					}
					if ( pSightEnt->IsPlayer() )
					{
						if ( pev->spawnflags & SF_MONSTER_WAIT_TILL_SEEN )
						{
							CBaseMonster *pClient;

							pClient = pSightEnt->MyMonsterPointer();
							// don't link this client in the list if the monster is wait till seen and the player isn't facing the monster
							if ( pSightEnt && !pClient->FInViewCone( this ) )
							{
								// we're not in the player's view cone. 
								continue;
							}
							else
							{
								// player sees us, become normal now.
								pev->spawnflags &= ~SF_MONSTER_WAIT_TILL_SEEN;
							}
						}

						// if we see a client, remember that (mostly for scripted AI)
						iSighted |= bits_COND_SEE_CLIENT;
					}
					if(EASY_CVAR_GET(crazyMonsterPrintouts)){
						easyPrintLine("FLAGGER 64 2");
					}
					pSightEnt->m_pLink = m_pLink;
					m_pLink = pSightEnt;
					
					if(EASY_CVAR_GET(crazyMonsterPrintouts)){
						easyPrintLine("FLAGGER 64 3::? (%d) %s %s", (pSightEnt == m_hEnemy), FClassname(pSightEnt), FClassname(m_hEnemy)  );
					}


					if ( pSightEnt == m_hEnemy )
					{
						// we know this ent is visible, so if it also happens to be our enemy, store that now.
						iSighted |= bits_COND_SEE_ENEMY;
					}

					//if(FClassnameIs(pev, "monster_chumtoad"))easyForcePrintLine("THE ENEMEH???");

					// don't add the Enemy's relationship to the conditions. We only want to worry about conditions when
					// we see monsters other than the Enemy.

					//MODDD TODO MAJOR - would it be a good idea to piggyback off of the bits_COND_SEE_NEMESIS for 
					//interrupting schedules if bait is sighted (R_BA)?
					//or should that be it's own bits_COND_SEE_BAIT, would need its own bit and need to be added to several default (and monster-specific schedules).
					switch ( IRelationship ( pSightEnt ) )
					{
					
					//MODDD - trying out treating R_BA as NEMESIS.  See if that works out.
					case	R_BA:
						iSighted |= bits_COND_SEE_NEMESIS;		
					break;

					case	R_NM:
						iSighted |= bits_COND_SEE_NEMESIS;		
						break;
					case	R_HT:		
						iSighted |= bits_COND_SEE_HATE;		
						break;
					case	R_DL:
						iSighted |= bits_COND_SEE_DISLIKE;
						break;
					case	R_FR:
						iSighted |= bits_COND_SEE_FEAR;
						break;
					case    R_AL:
						//nothing special about allies apparently.
						break;
					default:
						ALERT ( at_aiconsole, "%s can't assess %s\n", STRING(pev->classname), STRING(pSightEnt->pev->classname ) );
						break;
					}






				}
			}
		}
	}
	
	SetConditions( iSighted );
}

//=========================================================
// ISoundMask - returns a bit mask indicating which types
// of sounds this monster regards. In the base class implementation,
// monsters care about all sounds, but no scents.
// MODDD NOTE - and not the one fired by the anticipated landing spot of
//              grenades for knowing to flee from that, bits_SOUND_DANGER,
//              apparently.
//              Not that dumb aliens are supposed to react to that anyways
//              but include it as needed for smarter things.
//=========================================================
int CBaseMonster :: ISoundMask ( void )
{
	return	bits_SOUND_WORLD	|
			bits_SOUND_COMBAT	|
			bits_SOUND_PLAYER	|
			//MODDD - new
			bits_SOUND_BAIT;
}

//=========================================================
// PBestSound - returns a pointer to the sound the monster 
// should react to. Right now responds only to nearest sound.
//=========================================================
CSound* CBaseMonster :: PBestSound ( void )
{	
	int iThisSound; 
	int iBestSound = -1;
	float flBestDist = 8192;// so first nearby sound will become best so far.
	float flDist;
	CSound *pSound;

	iThisSound = m_iAudibleList; 

	if ( iThisSound == SOUNDLIST_EMPTY )
	{
		//MODDD - Enough already, stop spamming this...
		//ALERT ( at_aiconsole, "ERROR! monster %s has no audible sounds!\n", STRING(pev->classname) );
#if _DEBUG
		//ALERT( at_error, "NULL Return from PBestSound\n" );
#endif
		return NULL;
	}

	while ( iThisSound != SOUNDLIST_EMPTY )
	{
		pSound = CSoundEnt::SoundPointerForIndex( iThisSound );

		if ( pSound && pSound->FIsSound() )
		{
			flDist = ( pSound->m_vecOrigin - EarPosition()).Length();

			if ( flDist < flBestDist )
			{
				iBestSound = iThisSound;
				flBestDist = flDist;
			}
		}

		iThisSound = pSound->m_iNextAudible;
	}
	if ( iBestSound >= 0 )
	{
		pSound = CSoundEnt::SoundPointerForIndex( iBestSound );
		return pSound;
	}
#if _DEBUG
	ALERT( at_error, "NULL Return from PBestSound\n" );
#endif
	return NULL;
}

//=========================================================
// PBestScent - returns a pointer to the scent the monster 
// should react to. Right now responds only to nearest scent
//=========================================================
CSound* CBaseMonster :: PBestScent ( void )
{	
	int iThisScent; 
	int iBestScent = -1;
	float flBestDist = 8192;// so first nearby smell will become best so far.
	float flDist;
	CSound *pSound;

	iThisScent = m_iAudibleList;// smells are in the sound list.

	if ( iThisScent == SOUNDLIST_EMPTY )
	{
		ALERT ( at_aiconsole, "ERROR! PBestScent() has empty soundlist!\n" );
#if _DEBUG
		ALERT( at_error, "NULL Return from PBestSound\n" );
#endif
		return NULL;
	}

	while ( iThisScent != SOUNDLIST_EMPTY )
	{
		pSound = CSoundEnt::SoundPointerForIndex( iThisScent );

		if ( pSound->FIsScent() )
		{
			flDist = ( pSound->m_vecOrigin - pev->origin ).Length();

			if ( flDist < flBestDist )
			{
				iBestScent = iThisScent;
				flBestDist = flDist;
			}
		}

		iThisScent = pSound->m_iNextAudible;
	}
	if ( iBestScent >= 0 )
	{
		pSound = CSoundEnt::SoundPointerForIndex( iBestScent );

		return pSound;
	}
#if _DEBUG
	ALERT( at_error, "NULL Return from PBestScent\n" );
#endif
	return NULL;
}







//MODDD - by default, most monsters will do the usual "takeDamage" script's reaction to investigating the source of damage.
//Note that even this method getting called requires "EASY_CVAR_GET(bulletholeAlertRange)" to be above 0 (the distance the bullet-hit sound triggers enemies by calling this)
void CBaseMonster::heardBulletHit(entvars_t* pevShooter){

	//I heard it.
	entvars_t* pevListener = pev;
	CBaseMonster* monListener = this;


	// react to the sound
	if ( (pevListener->flags & FL_MONSTER) && !FNullEnt(pevShooter) )
	{
		if ( pevShooter->flags & (FL_MONSTER | FL_CLIENT) )
		{
			// only if the attacker was a monster or client!
			
			// enemy's last known position is somewhere down the vector that the attack came from.
			if (pevShooter)
			{
				if (monListener->m_hEnemy == NULL || pevShooter == monListener->m_hEnemy->pev || !monListener->HasConditions(bits_COND_SEE_ENEMY))
				{
					monListener->setEnemyLKP(pevShooter->origin);
				}
			}
			else
			{
				monListener->setEnemyLKP(monListener->pev->origin + ( g_vecAttackDir * 64 )); 
			}

			//want to face your LKP, yes?
			monListener->MakeIdealYaw( monListener->m_vecEnemyLKP );

			/*
			if ( flDamage > 0 )
			{
				SetConditions(bits_COND_LIGHT_DAMAGE);
			}

			if ( flDamage >= 20 )
			{
				SetConditions(bits_COND_HEAVY_DAMAGE);
			}
			*/

			//This makes most NPCs investigate their "m_vecEnemyLKP"'s, I suppose. Maybe flinch in surprise.
			monListener->SetConditions(bits_COND_LIGHT_DAMAGE);

		}
	}


}//END OF heardBulletHit(...)



void CBaseMonster::wanderAway(const Vector& toWalkAwayFrom){

	//you are going to walk somewhere away from the given location.
	//NOTICE - whoever called me already did the MONSTERSTATE checks to see we aren't telling something preoccupied to suddenly change its mind in
	//whatever it's doing.  Only those that are IDLE or ALERT.

	/*
	//STOP task script from schedule.cpp for reference
	case TASK_STOP_MOVING:
		{
			if ( m_IdealActivity == m_movementActivity )
			{
				m_IdealActivity = GetStoppedActivity();
			}
	*/

	//TODO: call "taskFail" and createa a delay until this monster wanders away. Break the delay on doing absolutely anything.
	//Peraps changing to a "waitForWanderDelay" schedule, interruptible by practically anything, would work?


	easyForcePrintLine("YOU, %s:%d, GOT TOLD TO WANDER AWAY SO YOU BETTER LISTEN", this->getClassname(), monsterID);
	//if(this->m_movementActivity == ACT_IDLE){
		//m_movementActivity

		//TODO: specify walking away from "toWalkAwayFrom" ?

		this->ChangeSchedule(  this->GetScheduleOfType(SCHED_WALK_AWAY_FROM_ORIGIN)  );
	//}


}//END OF wanderAway()







float CBaseMonster::paralyzeDuration = 0;
float CBaseMonster::nervegasDuration = 0;
float CBaseMonster::poisonDuration = 0;
float CBaseMonster::radiationDuration = 0;
float CBaseMonster::acidDuration = 0;
float CBaseMonster::slowburnDuration = 0;
float CBaseMonster::slowfreezeDuration = 0;
float CBaseMonster::bleedingDuration = 0;




//Easy way to convert new damage types without adjusting existing constants.
//That may be okay, but I'm not taking risks just yet.
int CBaseMonster::convert_itbd_to_damage(int i){
	
	
	if(i <= 7){
		//The old way works fine for existing damage types.

		//DAMNIT PAST ME, comments.
		//These damage types are represented per value of i:
		//0 : DMG_PARALYZE		(1 << 15)	// slows affected creature down
		//1 : DMG_NERVEGAS		(1 << 16)	// nerve toxins, very bad
		//2 : DMG_POISON			(1 << 17)	// blood poisioning
		//3 : DMG_RADIATION		(1 << 18)	// radiation exposure
		//4 : DMG_DROWNRECOVER	(1 << 19)	// drowning recovery
		//5 : DMG_ACID			(1 << 20)	// toxic chemicals or acid burns
		//6 : DMG_SLOWBURN		(1 << 21)	// in an oven
		//7 : DMG_SLOWFREEZE		(1 << 22)	// in a subzero freezer

		return (DMG_PARALYZE << i);
	}else{
		// start at 8 = DMG_BLEEDING.
		//return (DMG_PARALYZE << (i - 8));
		// ...actually no, unless all future timed damages are placed so they occur
		// one-after-the-other like for 'i <= 7' above.
		// Only one yet so hard to say.
		switch(i){
		case 8:
			return DMG_BLEEDING;
		break;
		}//END OF switch(i)
	}

	easyPrintLine("ERROR: failed to convert timed damage itbd #%d to damage type!", i);
	//throw error! problem!
	return -1;
}//END OF convert_itbd_to_damage



//MODDD - checkpoint.
void CBaseMonster::CheckTimeBasedDamage(void) 
{
	int i;
	BYTE bDuration = 0;

	static float gtbdPrev = 0.0;

	//no timed damage for 
	if( pev->health <= 0 || pev->deadflag != DEAD_NO){
		return;
	}

	// only check for time based damage approx. every 2 seconds
	if (abs(gpGlobals->time - m_tbdPrev) < 2.0)
		return;

	//not usually here.
	
	//MODDD - check other too!
	//if (!(m_bitsDamageType & DMG_TIMEBASED))
	if (!(m_bitsDamageType & DMG_TIMEBASED) && !(m_bitsDamageTypeMod & (DMG_TIMEBASEDMOD))  )
		return;

	m_tbdPrev = gpGlobals->time;
	
	//m_tbdPrev = gpGlobals->time;

	for (i = 0; i < CDMG_TIMEBASED; i++)
	{

		
		int* m_bitsDamageTypeRef = 0;
		if(i <= 7){
			//use the old bitmask.
			m_bitsDamageTypeRef = &m_bitsDamageType;
		}else{
			//use the new bitmask.
			m_bitsDamageTypeRef = &m_bitsDamageTypeMod;

			//easyPrintLine("WELLLLL?! %d :: %d", m_bitsDamageTypeMod, convert_itbd_to_damage(i));
			//continue;
		}
		

		if(convert_itbd_to_damage(i) == -1){
			easyPrintLine("CRITICAL ERROR: MONSTER TIMED DAMAGE BOOBOO: %d", i);
			return;
		}


		// make sure bit is set for damage type
		//if (m_bitsDamageType & (DMG_PARALYZE << i))
		if((*m_bitsDamageTypeRef) & (convert_itbd_to_damage(i))  )
		{

			
			int damageType = 0;

			//
			//if(timedDamageIgnoresArmorMem == 1){
			//	//timed damage ignores armor.
			//	damageType = DMG_FALL;
			//}else{
			//	//timed damage hits armor first.
			//	damageType = DMG_GENERIC;
			//}
			//

			//no, can't just do that.

			//instead, a damgeType being "DMG_TIMEDEFFECT" means,
			//it is just generic, but use this for telling whether to
			//apply the armor-bypass or not without the sideeffect of what-
			//ever happens to DMG_FALL.
			damageType = DMG_TIMEDEFFECT;


			switch (i)
			{
				//NOTE: should these ignore armor or not?  I feel like they kind of should.
				//If so, make the damage type (DMG_GENERIC) become "DMG_FALL" instead (known to ignore armor).
			case itbd_Paralyze:
				// UNDONE - flag movement as half-speed
				bDuration = paralyzeDuration;
				//MODDD - see above.
				//bDuration = PARALYZE_DURATION;
				break;
			case itbd_NerveGas:
				//MODDD - comment undone.
				TakeDamage(pev, pev, NERVEGAS_DAMAGE, 0, damageType);	
				
				bDuration = nervegasDuration;
				//MODDD - see above.
				//bDuration = NERVEGAS_DURATION;
				break;
			case itbd_Poison:
				TakeDamage(pev, pev, POISON_DAMAGE, 0, damageType | DMG_TIMEDEFFECTIGNORE);
				
				bDuration = poisonDuration;
				
				//MODDD - see above.
				//bDuration = POISON_DURATION;
				break;
			case itbd_Radiation:
				//MODDD - comment on "TakeDamage" undone.
				TakeDamage(pev, pev, RADIATION_DAMAGE, 0, damageType | DMG_TIMEDEFFECTIGNORE);
				
				bDuration = radiationDuration;
				//MODDD - see above.
				//bDuration = RADIATION_DURATION;
				break;
			case itbd_DrownRecover:
				//
				//// NOTE: this hack is actually used to RESTORE health
				//// after the player has been drowning and finally takes a breath
				//if (m_idrowndmg > m_idrownrestored)
				//{
				//	int idif = min(m_idrowndmg - m_idrownrestored, 10);

				//	TakeHealth(idif, DMG_GENERIC);
				//	m_idrownrestored += idif;
				//}
				//bDuration = 4;	// get up to 5*10 = 50 points back
				//
				break;
			case itbd_Acid:
				//MODDD - comment undone.
				TakeDamage(pev, pev, ACID_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = acidDuration;
				//bDuration = ACID_DURATION;
				break;
			case itbd_SlowBurn:
				//MODDD - comment undone.
				TakeDamage(pev, pev, SLOWBURN_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = slowburnDuration;
				//bDuration = SLOWBURN_DURATION;
				break;
			case itbd_SlowFreeze:
				//MODDD - comment undone.
				TakeDamage(pev, pev, SLOWFREEZE_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = slowfreezeDuration;
				//bDuration = SLOWFREEZE_DURATION;
				break;
				//MODDD - new.
			case itbd_Bleeding:
				//this will always ignore the armor (hence DMG_TIMEDEFFECT).
				TakeDamage(pev, pev, BLEEDING_DAMAGE, 0, DMG_TIMEDEFFECTIGNORE);
				
				bDuration = bleedingDuration;
				break;

			default:
				bDuration = 0;
			}




			//
			//MODDD - diagnositic.
			//if(i == itbd_Radiation){
			//		easyPrintLine("timebaseddamage for rad? %d", m_rgbTimeBasedDamage[i]);
			//}
			//


			if (m_rgbTimeBasedDamage[i])
			{
				// use up an antitoxin on poison or nervegas after a few seconds of damage
				//MODDD - instead of referring to constants like "NERVEGASDURATION", it is referring to the
				//variable "nervegasDuration", which is set according to difficulty.  Same for poison.
				

				
				//if(CVAR_skillMem == 3 && timedDamageEndlessOnHardMem == 1){
				if(g_iSkillLevel == 3 && EASY_CVAR_GET(timedDamageEndlessOnHard) == 1){
					//Hard mode is on, and "timedDamageEndlessOnHard" is on...
					//Do NOT decrement non-curable durations.
					//However, still decrement only ONCE on curables to satisfy the one-second-passing rule for canisters to work.
					if(!m_rgbTimeBasedFirstFrame[i] &&
						(i == itbd_NerveGas || i == itbd_Poison || i ==  itbd_Radiation || i == itbd_Bleeding) ){
						//DO NOTHING.  Only the appropriate cure can fix it.
					}else if(m_rgbTimeBasedDamage[i] != 0){
						m_rgbTimeBasedDamage[i]--;
						m_rgbTimeBasedFirstFrame[i] = FALSE;
					}

				}else{
					//work as normal.  Decrement durations.

					if(m_rgbTimeBasedDamage[i] != 0){
						m_rgbTimeBasedDamage[i]--;
					}
				}

				// decrement damage duration, detect when done.
				//MODDD - change to how that works.
				//if (!m_rgbTimeBasedDamage[i] || --m_rgbTimeBasedDamage[i] == 0)
				if(!m_rgbTimeBasedDamage[i])
				{
					m_rgbTimeBasedDamage[i] = 0;
					//MODDD
					m_rgbTimeBasedFirstFrame[i] = TRUE;
					//MODDD
					// if we're done, clear damage bits
					//m_bitsDamageType &= ~(DMG_PARALYZE << i);	
					(*m_bitsDamageTypeRef) &= ~(convert_itbd_to_damage(i));
				}

			}
			else{
				// first time taking this damage type - init damage duration
				//MODDD - probably a bit redundant, but ah well.
				m_rgbTimeBasedFirstFrame[i] = TRUE;
				m_rgbTimeBasedDamage[i] = bDuration;
			}
		}
	}
}




BOOL CBaseMonster::forceIdleFrameReset(void){
	return FALSE;
}

void CBaseMonster::onNewRouteNode(void){
	//nothing by default
}


//CANNED!
//MODDD - an injection that occurs before "MontserThink" for all monsters.
void CBaseMonster::MonsterThinkPreMOD(void){
	
	//return;
	//TODO: cancel my schedule / task on death?  is that possible?



	/*
	BOOL canCheck = TRUE;

	Task_t* tempTask = NULL;
	if( (tempTask = this->GetTask()) != NULL){
		if( (tempTask->iTask) == TASK_DIE){
			canCheck = FALSE;
		}
	}
	
	if(canCheck){

		if(m_hEnemy != NULL && !UTIL_IsAliveEntity(m_hEnemy)){

		
			//SetConditions ( bits_COND_ENEMY_DEAD );
			//ClearConditions( bits_COND_SEE_ENEMY | bits_COND_ENEMY_OCCLUDED );

			m_hEnemy = NULL;
			//assume the current schedule involves the monster not being null, for safety.
			TaskFail();
			canCheck = FALSE;
			//return;
		}
	
		if(canCheck && m_hTargetEnt != NULL && (!targetIsDeadException && !UTIL_IsAliveEntity(m_hTargetEnt)) ){
			m_hTargetEnt = NULL;
			TaskFail();
			//return;
		}

	}
	*/

	/*

	if(EASY_CVAR_GET(seeMonsterHealth) == 1 && pev->max_health >= 1){
		//topCenter
		Vector topCenter = Vector(this->pev->origin.x + (pev->maxs.x + pev->mins.x)/2, this->pev->origin.y + (pev->maxs.y + pev->mins.y)/2, this->pev->origin.z + (pev->maxs.z) - 4 );
		int r;
		int g;
		int b;
		UTIL_deriveColorFromMonsterHealth(pev->health, pev->max_health, r, g, b);

		UTIL_drawLineFrameBoxAround(topCenter, 6, 16, r, g, b);
	}

	*/


	//MonsterThink();

	

}//END OF MonsterThinkPreMOD

BOOL CBaseMonster::usesAdvancedAnimSystem(void){
	return FALSE;
}









//=========================================================
// Monster Think - calls out to core AI functions and handles this
// monster's specific animation events
//=========================================================
void CBaseMonster :: MonsterThink ( void )
{


	///pev->renderfx |= NOMUZZLEFLASH;
	pev->nextthink = gpGlobals->time + 0.1;// keep monster thinking.
	

	


	if(forgetSmallFlinchTime != -1 && gpGlobals->time >= forgetSmallFlinchTime){
		forgetSmallFlinchTime = -1;
		this->Forget(bits_MEMORY_FLINCHED);
	}
	if(forgetBigFlinchTime != -1 && gpGlobals->time >= forgetBigFlinchTime){
		forgetBigFlinchTime = -1;
		//this->Forget(bits_MEMORY_BIG_FLINCHED);
	}



	//TODO - test frame. is 255 or 256 the last??
	/*
	pev->frame = EASY_CVAR_GET(testVar);
	pev->framerate = 0;

	return;
	*/

	//easyForcePrintLine("FRAMEA:%.2f seq:%d loop:%d fin:%d", this->pev->frame, pev->sequence, m_fSequenceLoops, m_fSequenceFinished);

	

	
	/*
	if(EASY_CVAR_GET(testVar) == 1){
		//BLOCKER
		return;
	}

	if(EASY_CVAR_GET(testVar) == 2){
		this->m_fSequenceLoops = FALSE;
	}
	*/
	
	/*
	//pev->renderfx |= STOPINTR;

	
	pev->sequence = EASY_CVAR_GET(testVar);
	pev->framerate = 1;

	return;
	*/

	


	if(this->drawFieldOfVisionConstant == TRUE){
		DrawFieldOfVision();
	}
	

	if(fApplyTempVelocity){
		fApplyTempVelocity = FALSE;
		pev->velocity = velocityApplyTemp;
	}



	int clrR = 0;
	int clrG = 0;
	int clrB = 0;


	if(EASY_CVAR_GET(pathfindTopRampFixDraw)==1 && debugVectorsSet){
		
		if(debugVectorMode == 0){

			if(debugVectorPrePass){
				clrR=0;clrG=255;clrB=0;
				if(debugFailColor){clrR=255;clrG=0;clrB=0;}
				UTIL_drawLineFrame(debugVector1, debugVector2, 12, clrR, clrG, clrB);
			}else{
				clrR=255;clrG=0;clrB=0;
				if(debugFailColor){clrR=255;clrG=0;clrB=0;}
				UTIL_drawLineFrame(debugVector1, debugVector2, 12, clrR, clrG, clrB);
			}
			clrR=0;clrG=255;clrB=0;
			if(debugFailColor){clrR=255;clrG=0;clrB=0;}
			UTIL_drawLineFrame(debugVector2, debugVector3, 12, clrR, clrG, clrB);
		

		}else if(debugVectorMode == 1){
			
			clrR=0;clrG=255;clrB=0;
			if(debugFailColor){clrR=255;clrG=0;clrB=0;}
			UTIL_drawLineFrame(debugVector1, debugVector2, 12, clrR, clrG, clrB);
			
			clrR=0;clrG=0;clrB=255;
			if(debugFailColor){clrR=255;clrG=0;clrB=0;}
			UTIL_drawLineFrame(debugVector2, debugVector3, 12, clrR, clrG, clrB);
			
			clrR=0;clrG=255;clrB=0;
			if(debugFailColor){clrR=255;clrG=0;clrB=0;}
			UTIL_drawLineFrame(debugVector3, debugVector4, 12, clrR, clrG, clrB);


		}//END OF debugVectorMode checks
		else if(debugVectorMode == 2){
			
			clrR=0;clrG=255;clrB=0;
			if(debugFailColor){clrR=255;clrG=0;clrB=0;}
			UTIL_drawLineFrame(debugVector1, debugVector2, 12, clrR, clrG, clrB);

		}

	}//END OF draw debug vector check (path finding ramp fix)
	

	

	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info A? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);
	RunAI();


	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info B frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);
	

	//hey WELL.
	if(EASY_CVAR_GET(timedDamageAffectsMonsters) == 1){
		CheckTimeBasedDamage();
	}
	
	//float flInterval = 0;
	float flInterval = StudioFrameAdvance( ); // animate
	//MODDD MAJOR MAJOR MAJOR NOTE - is using the same interval from animation across the rest of logic bad?
	//Should gpGlobals->frametime be used instead?  This has seemed reliable 99% of the time in any case.
	//Seems changing the animation way too often (every think frame) could reset the anim time, forcing an interval of 0 which
	//breaks the tolerance checks for advancing path nodes.  But for that... don't set animations that often then.


	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info C? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);


// start or end a fidget
// This needs a better home -- switching animations over time should be encapsulated on a per-activity basis
// perhaps MaintainActivity() or a ShiftAnimationOverTime() or something.



	//easyPrintLine("HEY THERE undesirable person %d", m_fSequenceFinished);

	//Is that last part OKAY?
	//if ( m_MonsterState != MONSTERSTATE_SCRIPT && m_MonsterState != MONSTERSTATE_DEAD && m_Activity == ACT_IDLE && m_fSequenceFinished )
	
	if(monsterID == 6){
		int x = 45;
	}

	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info IDLE RESET check?: custo:%d autoblock:%d stateForbid:%d idle?%d seqfin?%d - frame:%.2f done:%d",
			getClassname(), monsterID,
			usingCustomSequence, getMonsterBlockIdleAutoUpdate(), (m_MonsterState == MONSTERSTATE_SCRIPT||m_MonsterState==MONSTERSTATE_DEAD), (m_Activity==ACT_IDLE), m_fSequenceFinished,
		pev->frame, m_fSequenceFinished);

	//If looping and using a custom sequence (set by some "setSequenceBy..." method or similar, as opposed to selected by a new activity),
	//do NOT force a new animation! We mean to keep the current animation.
	//ALSO IMPORTANT: This will fuck with tasks based on waiting for for the sequence to be complete, which would be seen the next frame. Fuck that shit.
	if (
		//!(m_pSchedule != NULL && getTaskNumber() ==  
		
		//!(m_fSequenceLoops && usingCustomSequence) && 
		!IsMoving() && !usingCustomSequence &&
		!getMonsterBlockIdleAutoUpdate() && m_MonsterState != MONSTERSTATE_SCRIPT && m_MonsterState != MONSTERSTATE_DEAD && ( (m_Activity == ACT_IDLE && m_fSequenceFinished) ) ) 
	{
		int iSequence;
		
		

		//easyPrintLine("YOU THINK THIS IS A great GAME %d", m_fSequenceFinished);
		
		if ( m_fSequenceLoops )
		{
			// animation does loop, which means we're playing subtle idle. Might need to 
			// fidget.
			if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info IDLE RESET #1? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);

				if(usesAdvancedAnimSystem()){
					iSequence = LookupActivityHard ( m_Activity );
				}else{
					iSequence = LookupActivity ( m_Activity );
				}
			
		}
		else
		{
			if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info IDLE RESET #2? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);

			// animation that just ended doesn't loop! That means we just finished a fidget
			// and should return to our heaviest weighted idle (the subtle one)

			if(usesAdvancedAnimSystem()){
				iSequence = LookupActivityHard ( m_Activity );
			}else{
				iSequence = LookupActivityHeaviest ( m_Activity );
			}

		}
		if ( iSequence != ACTIVITY_NOT_AVAILABLE )
		{
			int oldSequence = pev->sequence;
			//BOOL resetFrameYet = FALSE;

			
			//easyPrintLine("ANIMATION CHANGE!!!! B");
			pev->sequence = iSequence;	// Set to new anim (if it's there)
			ResetSequenceInfo( );

			//MODDD IMPORTANT. Go ahead and let the system know this sequence finshed at least once, sometimes that matters. Even a replacement different idle sequence.
			m_fSequenceFinishedSinceLoop = TRUE;

			
			//MODDD - why wasn't this here before?!  Why do we assume the new sequence will reset the frame? We sure don't know that?
			//        Even without forceIdleFrameReset.
			if(forceIdleFrameReset() || !(iSequence == oldSequence && this->m_fSequenceLoops) ){
				//if this is the same sequence and it loops, no need to reset.
				resetFrame();
			}
		}
	}

	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info D? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);
	DispatchAnimEvents( flInterval );
	if(EASY_CVAR_GET(animationPrintouts) == 1 && monsterID >= -1)easyForcePrintLine("%s:%d Anim info E? frame:%.2f done:%d", getClassname(), monsterID, pev->frame, m_fSequenceFinished);

	if ( !MovementIsComplete() )
	{
		//easyForcePrintLine("GET good sonny A");
		Move( flInterval );
		//MODDD - flInterval may be reset on the same frame the animation is changed.  If that is ok, keep using Move(flInterval) like retail does.
		//Move(gpGlobals->frametime);

	}
#if _DEBUG	
	else 
	{
		//easyForcePrintLine("GET good sonny B");
		if ( !TaskIsRunning() && !TaskIsComplete() )
			ALERT( at_error, "Schedule stalled!!\n" );
	}
#endif
	

	/*
	//MODDD - are you insane
		pev->renderamt = 255;
		pev->rendermode = kRenderTransTexture;
		pev->renderfx = kRenderFxGlowShell;
	*/



	
	
	if(EASY_CVAR_GET(peaceOut) == 1){

		if(m_hEnemy != NULL){
			//all is forgiven.
			m_hEnemy = NULL;
			if(this->m_MonsterState == MONSTERSTATE_COMBAT || this->m_IdealMonsterState == MONSTERSTATE_COMBAT){
				this->m_MonsterState = MONSTERSTATE_IDLE;
				this->m_IdealMonsterState = MONSTERSTATE_IDLE;
			}
			TaskFail();
		}
	}

	
	if(EASY_CVAR_GET(drawDebugEnemyLKP)){
		//::DebugLine_Setup(2, m_vecEnemyLKP + Vector(0, 0, 8), m_vecEnemyLKP + Vector(0, 0, -8), 0, 0, 255);
		//no... just draw it this frame only.
		::UTIL_drawPointFrame(m_vecEnemyLKP + Vector(0, 0, 8), DEBUG_LINE_WIDTH, 255, 255, 0);
	}
	
	//easyForcePrintLine("FRAMEB:%.2f seq:%d loop:%d fin:%d", this->pev->frame, pev->sequence, m_fSequenceLoops, m_fSequenceFinished);

}//END OF monsterThink








//MODDD
//by default, don't use alternate models.  Use one if specified in a child class.
BOOL CBaseMonster::getGermanModelRequirement(void){
	return FALSE;
}
const char* CBaseMonster::getGermanModel(void){
	return NULL;
}
const char* CBaseMonster::getNormalModel(void){
	return NULL;
}


//MODDD
void CBaseMonster::setModel(void){
	CBaseMonster::setModel(NULL);
}
void CBaseMonster::setModel(const char* m){
	//too broad to do much here.

	//...actually, if unspecified, a little default behavior:
	const char* germanModelPath = getGermanModel();
	BOOL hasGermanModel = (germanModelPath != NULL);




	//if there is a german model at all (not everything may ever involve german model script)
	if(hasGermanModel){

		const char* normalModelPath = getNormalModel();
		//This should NEVER be null, as the german & normal model paths should be set at the same time.

		//if german censorship is off


		//NOTICE: not sure what to do if  "getGermanModelRequirement()"  fails.  Crash?  Invisible model?    For now, just relying on retail's version.
		if(EASY_CVAR_GET(germanCensorship) == 0 || EASY_CVAR_GET(allowGermanModels) != 1 || globalPSEUDO_canApplyGermanCensorship == 0 || getGermanModelRequirement() == FALSE){
			//but we're using the german model...
			//if(usingGermanModel){
				SET_MODEL(ENT(pev), normalModelPath);
			//}
			//if german censorship is on
		}else if(EASY_CVAR_GET(germanCensorship) == 1 && EASY_CVAR_GET(allowGermanModels) == 1 && globalPSEUDO_canApplyGermanCensorship == 1){
			//but we're not using the german model (and have one)
			//if(hasGermanModel && !usingGermanModel){
			//if(hasGermanModel){   //REDUNDANT.
				SET_MODEL(ENT(pev), germanModelPath);
			//}
			//}
		}else{
			//FREAKY INBETWEEN STATE.  HELP!!!
			easyPrintLine("!!!ERROR 93r|\\|\\4|\\|.  %s:%d", this->getClassname(), this->monsterID);
		}

	}else{

		if(m != NULL && m[0] != '\0'){
			//Non-empty string? Not using the GermanModel system? Just go with what we were sent.
			CBaseEntity::setModel(m);
		}
	}

}

BOOL CBaseMonster::getMonsterBlockIdleAutoUpdate(){
	//by default, monsters have no problems looping the idle anim / looking for a new one automatically when it finishes.
	//Override this method in child monster classes (children of CBaseMonster) to change this per child.
	return FALSE;
}//END OF getMonsterBlockIdleAutoUpdate(...)


//=========================================================
// CBaseMonster - USE - will make a monster angry at whomever
// activated it.
//=========================================================
void CBaseMonster :: MonsterUse ( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	m_IdealMonsterState = MONSTERSTATE_ALERT;
}

//=========================================================
// Ignore conditions - before a set of conditions is allowed
// to interrupt a monster's schedule, this function removes
// conditions that we have flagged to interrupt the current
// schedule, but may not want to interrupt the schedule every
// time. (Pain, for instance)
//=========================================================
int CBaseMonster :: IgnoreConditions ( void )
{
	int iIgnoreConditions = 0;

	if ( !FShouldEat() )
	{
		// not hungry? Ignore food smell.
		iIgnoreConditions |= bits_COND_SMELL_FOOD;
	}

	if ( m_MonsterState == MONSTERSTATE_SCRIPT && m_pCine )
		iIgnoreConditions |= m_pCine->IgnoreConditions();

	return iIgnoreConditions;
}

//=========================================================
// 	RouteClear - zeroes out the monster's route array and goal
//=========================================================
void CBaseMonster :: RouteClear ( void )
{
	disableEnemyAutoNode = FALSE;

	//easyForcePrintLine("OH no ROUTECLEAR");

	RouteNew();
	m_movementGoal = MOVEGOAL_NONE;
	m_movementActivity = ACT_IDLE;
	Forget( bits_MEMORY_MOVE_FAILED );
}

//=========================================================
// Route New - clears out a route to be changed, but keeps
//				goal intact.
//=========================================================
void CBaseMonster :: RouteNew ( void )
{
	m_Route[ 0 ].iType		= 0;
	m_iRouteIndex			= 0;
}

//=========================================================
// FRouteClear - returns TRUE is the Route is cleared out
// ( invalid )
//=========================================================
BOOL CBaseMonster :: FRouteClear ( void )
{
	//Note that this is not clearing the route and returning success of clearing the route. This is a check to see if the route is clear right now
	//without affecting it at all.
	//disableEnemyAutoNode = FALSE;

	if ( m_Route[ m_iRouteIndex ].iType == 0 || m_movementGoal == MOVEGOAL_NONE )
		return TRUE;

	return FALSE;
}







//MODDD - new. Clone of FRefreshRoute that better incorporates the method calls done in "CHASE_ENEMY"'s schedule.
BOOL CBaseMonster::FRefreshRouteChaseEnemySmart(void){


	//WHUT
	//return FRefreshRoute();
	



	BOOL returnCode;

	
	/*	
	if(m_hEnemy == NULL){
		//what's the point?
		//...but we can still follow the existing LKP? I don't know.
		return FALSE;
	}
	*/

	
	//case TASK_GET_PATH_TO_ENEMY:
	CBaseEntity *pEnemy = m_hEnemy;


	disableEnemyAutoNode = TRUE;
	//probably unnecessary. no harm.
	m_movementActivity = ACT_RUN; //is that safe to assume?
	m_moveWaitTime = 2; //same?

	//HACKER SACKS!!!!!    was MOVEGOAL_ENEMY;
	//m_movementGoal = MOVEGOAL_LOCATION;
	//int iMoveFlaggg = bits_MF_TO_LOCATION;
	
	m_movementGoal = MOVEGOAL_ENEMY;
	int iMoveFlaggg = bits_MF_TO_ENEMY;




	//was this... missing?
	RouteNew();



	//Mainly just to keep track of where the enemy is since last calling for a new route.
	//As in, if the enemy moves too far from the last place a path found a route TO, we need to re-route to keep the destination accurate.
	


	/*
	m_vecMoveGoal = m_hEnemy->pev->origin;

	//MODDD - new?
	//m_vecEnemyLKP = m_hEnemy->pev->origin;
	setEnemyLKP(m_hEnemy->pev->origin);
	*/

	m_vecMoveGoal = m_vecEnemyLKP;

	
	//MODDD - used to have "pEnemy->pev->origin" as the first argument for BuildRoute and BuildNearestRoute. Now just pipes along the set "m_vecMoveGoal" above, no problem.
	if ( BuildRoute ( m_vecMoveGoal, iMoveFlaggg, pEnemy ) )
	{
		//TaskComplete();
		returnCode = TRUE;
	}
	//else if (BuildNearestRoute( m_vecMoveGoal, pEnemy->pev->view_ofs, 0, (pEnemy->pev->origin - pev->origin).Length() ))
	else if (BuildNearestRoute( m_vecMoveGoal, pEnemy->pev->view_ofs, 0, (m_vecEnemyLKP - pev->origin).Length(), iMoveFlaggg, pEnemy ))
	{
		//TaskComplete();
		returnCode = TRUE;
	}
	else
	{
		// no way to get there =(
		//ALERT ( at_aiconsole, "GetPathToEnemy failed!!\n" );
		//TaskFail();
		returnCode = FALSE;
	}

	return returnCode;
}//END OF FRefreshRouteChaseEnemySmart


//=========================================================
// FRefreshRoute - after calculating a path to the monster's
// target, this function copies as many waypoints as possible
// from that path to the monster's Route array
//=========================================================
BOOL CBaseMonster :: FRefreshRoute ( void )
{
	CBaseEntity	*pPathCorner;
	int		i;
	BOOL		returnCode;

	RouteNew();

	returnCode = FALSE;

	switch( m_movementGoal )
	{
		case MOVEGOAL_PATHCORNER:
			{
				// monster is on a path_corner loop
				pPathCorner = m_pGoalEnt;
				i = 0;

				while ( pPathCorner && i < ROUTE_SIZE )
				{
					m_Route[ i ].iType = bits_MF_TO_PATHCORNER;
					m_Route[ i ].vecLocation = pPathCorner->pev->origin;

					pPathCorner = pPathCorner->GetNextTarget();

					// Last path_corner in list?
					if ( !pPathCorner )
						m_Route[i].iType |= bits_MF_IS_GOAL;
					
					i++;
				}
			}
			returnCode = TRUE;
			break;

		case MOVEGOAL_ENEMY:

			/*
			if(m_hEnemy == FALSE){
				//what's the point?
				//...but we can still follow the existing LKP? I don't know.
				return FALSE;
			}
			*/

			////m_vecEnemyLKP = m_hEnemy->pev->origin; //!!!
			//setEnemyLKP(m_hEnemy->pev->origin);
			returnCode = BuildRoute( m_vecEnemyLKP, bits_MF_TO_ENEMY, m_hEnemy );


			//MODDD - why wasn't there a BuildNearestRoute attempt if above failed?
			if (returnCode == FALSE)
			{
				returnCode = BuildNearestRoute( m_vecEnemyLKP, m_hEnemy->pev->view_ofs, 0, (m_vecEnemyLKP - pev->origin).Length(), bits_MF_TO_ENEMY, m_hEnemy );
			}

			//MODDD - CHECK. Is automatically setting "m_vecMoveGoal" to the goal of this path ok? m_vecMoveGoal is usually an input.
			if(returnCode){
				m_vecMoveGoal = m_vecEnemyLKP;
			}

			break;

		case MOVEGOAL_LOCATION:
			returnCode = BuildRoute( m_vecMoveGoal, bits_MF_TO_LOCATION, NULL );

			//MODDD - can we do a nearest instead?
			if(returnCode == FALSE){
				returnCode = BuildNearestRoute(m_vecMoveGoal, pev->view_ofs, 0, (m_vecMoveGoal - pev->origin).Length(), bits_MF_TO_LOCATION, NULL );
			}

			break;

		case MOVEGOAL_TARGETENT:
			if (m_hTargetEnt != NULL)
			{
				returnCode = BuildRoute( m_hTargetEnt->pev->origin, bits_MF_TO_TARGETENT, m_hTargetEnt );

				if(!returnCode){
					//is this okay?
					returnCode = BuildNearestRoute( m_hTargetEnt->pev->origin, m_hTargetEnt->pev->view_ofs, 0, (m_hTargetEnt->pev->origin - this->pev->origin).Length() + 500, bits_MF_TO_TARGETENT, m_hTargetEnt  );
				}

			}else{
				returnCode = FALSE;
			}

			if(returnCode){
				//MODDD - CHECK. Is automatically setting "m_vecMoveGoal" to the goal of this path ok? m_vecMoveGoal is usually an input.
				if(returnCode){
					m_vecMoveGoal = m_hTargetEnt->pev->origin;
				}
			}

			break;

		case MOVEGOAL_NODE:
			returnCode = FGetNodeRoute( m_vecMoveGoal );
//			if ( returnCode )
//				RouteSimplify( NULL );
			break;
	}

	return returnCode;
}


BOOL CBaseMonster::MoveToEnemy( Activity movementAct, float waitTime )
{
	m_movementActivity = movementAct;
	m_moveWaitTime = waitTime;
	
	m_movementGoal = MOVEGOAL_ENEMY;
	return FRefreshRoute();
}


BOOL CBaseMonster::MoveToLocation( Activity movementAct, float waitTime, const Vector &goal )
{
	m_movementActivity = movementAct;
	m_moveWaitTime = waitTime;
	
	m_movementGoal = MOVEGOAL_LOCATION;
	m_vecMoveGoal = goal;
	return FRefreshRoute();
}


BOOL CBaseMonster::MoveToTarget( Activity movementAct, float waitTime )
{
	m_movementActivity = movementAct;
	m_moveWaitTime = waitTime;
	
	m_movementGoal = MOVEGOAL_TARGETENT;
	return FRefreshRoute();
}


BOOL CBaseMonster::MoveToNode( Activity movementAct, float waitTime, const Vector &goal )
{
	m_movementActivity = movementAct;
	m_moveWaitTime = waitTime;

	m_movementGoal = MOVEGOAL_NODE;
	m_vecMoveGoal = goal;
	return FRefreshRoute();
}






void CBaseMonster::DrawRoute( entvars_t *pev, WayPoint_t *m_Route, int m_iRouteIndex, int r, int g, int b )
{
	int		i;

	if ( m_Route[m_iRouteIndex].iType == 0 )
	{
		ALERT( at_aiconsole, "Can't draw route!\n" );
		return;
	}

//	UTIL_ParticleEffect ( m_Route[ m_iRouteIndex ].vecLocation, g_vecZero, 255, 25 );

	MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
		WRITE_BYTE( TE_BEAMPOINTS);
		WRITE_COORD( pev->origin.x );
		WRITE_COORD( pev->origin.y );
		WRITE_COORD( pev->origin.z );
		WRITE_COORD( m_Route[ m_iRouteIndex ].vecLocation.x );
		WRITE_COORD( m_Route[ m_iRouteIndex ].vecLocation.y );
		WRITE_COORD( m_Route[ m_iRouteIndex ].vecLocation.z );

		WRITE_SHORT( g_sModelIndexLaser );
		WRITE_BYTE( 0 ); // frame start
		WRITE_BYTE( 10 ); // framerate
		WRITE_BYTE( 1 ); // life
		WRITE_BYTE( 16 );  // width
		WRITE_BYTE( 0 );   // noise
		WRITE_BYTE( r );   // r, g, b
		WRITE_BYTE( g );   // r, g, b
		WRITE_BYTE( b );   // r, g, b
		WRITE_BYTE( 255 );	// brightness
		WRITE_BYTE( 10 );		// speed
	MESSAGE_END();

	for ( i = m_iRouteIndex ; i < ROUTE_SIZE - 1; i++ )
	{
		if ( (m_Route[ i ].iType & bits_MF_IS_GOAL) || (m_Route[ i+1 ].iType == 0) )
			break;

		
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_BEAMPOINTS );
			WRITE_COORD( m_Route[ i ].vecLocation.x );
			WRITE_COORD( m_Route[ i ].vecLocation.y );
			WRITE_COORD( m_Route[ i ].vecLocation.z );
			WRITE_COORD( m_Route[ i + 1 ].vecLocation.x );
			WRITE_COORD( m_Route[ i + 1 ].vecLocation.y );
			WRITE_COORD( m_Route[ i + 1 ].vecLocation.z );
			WRITE_SHORT( g_sModelIndexLaser );
			WRITE_BYTE( 0 ); // frame start
			WRITE_BYTE( 10 ); // framerate
			WRITE_BYTE( 1 ); // life
			WRITE_BYTE( 8 );  // width
			WRITE_BYTE( 0 );   // noise
			WRITE_BYTE( r );   // r, g, b
			WRITE_BYTE( g );   // r, g, b
			WRITE_BYTE( b );   // r, g, b
			WRITE_BYTE( 255 );	// brightness
			WRITE_BYTE( 10 );		// speed
		MESSAGE_END();

//		UTIL_ParticleEffect ( m_Route[ i ].vecLocation, g_vecZero, 255, 25 );
	}
}//END OF DrawRoute(...)




int ShouldSimplify( int routeType )
{
	routeType &= ~bits_MF_IS_GOAL;

	if ( (routeType == bits_MF_TO_PATHCORNER) || (routeType & bits_MF_DONT_SIMPLIFY) )
		return FALSE;
	return TRUE;
}

//=========================================================
// RouteSimplify
//
// Attempts to make the route more direct by cutting out
// unnecessary nodes & cutting corners.
//
//=========================================================



void CBaseMonster :: RouteSimplify( CBaseEntity *pTargetEnt )
{
	// BUGBUG: this doesn't work 100% yet
	int		i, count, outCount;
	Vector		vecStart;
	WayPoint_t	outRoute[ ROUTE_SIZE * 2 ];	// Any points except the ends can turn into 2 points in the simplified route

	count = 0;

	for ( i = m_iRouteIndex; i < ROUTE_SIZE; i++ )
	{
		if ( !m_Route[i].iType )
			break;
		else
			count++;
		if ( m_Route[i].iType & bits_MF_IS_GOAL )
			break;
	}
	// Can't simplify a direct route!
	if ( count < 2 )
	{
//		DrawRoute( pev, m_Route, m_iRouteIndex, 0, 0, 255 );
		return;
	}

	outCount = 0;
	vecStart = pev->origin;
	for ( i = 0; i < count-1; i++ )
	{
		// Don't eliminate path_corners
		if ( !ShouldSimplify( m_Route[m_iRouteIndex+i].iType ) )
		{
			outRoute[outCount] = m_Route[ m_iRouteIndex + i ];
			outCount++;
		}
		else if ( CheckLocalMove ( vecStart, m_Route[m_iRouteIndex+i+1].vecLocation, pTargetEnt, NULL ) == LOCALMOVE_VALID )
		{
			// Skip vert
			continue;
		}
		else
		{
			Vector vecTest, vecSplit;

			// Halfway between this and next
			vecTest = (m_Route[m_iRouteIndex+i+1].vecLocation + m_Route[m_iRouteIndex+i].vecLocation) * 0.5;

			// Halfway between this and previous
			vecSplit = (m_Route[m_iRouteIndex+i].vecLocation + vecStart) * 0.5;

			int iType = (m_Route[m_iRouteIndex+i].iType | bits_MF_TO_DETOUR) & ~bits_MF_NOT_TO_MASK;
			if ( CheckLocalMove ( vecStart, vecTest, pTargetEnt, NULL ) == LOCALMOVE_VALID )
			{
				outRoute[outCount].iType = iType;
				outRoute[outCount].vecLocation = vecTest;
			}
			else if ( CheckLocalMove ( vecSplit, vecTest, pTargetEnt, NULL ) == LOCALMOVE_VALID )
			{
				outRoute[outCount].iType = iType;
				outRoute[outCount].vecLocation = vecSplit;
				outRoute[outCount+1].iType = iType;
				outRoute[outCount+1].vecLocation = vecTest;
				outCount++; // Adding an extra point
			}
			else
			{
				outRoute[outCount] = m_Route[ m_iRouteIndex + i ];
			}
		}
		// Get last point
		vecStart = outRoute[ outCount ].vecLocation;
		outCount++;
	}
	ASSERT( i < count );
	outRoute[outCount] = m_Route[ m_iRouteIndex + i ];
	outCount++;
	
	// Terminate
	outRoute[outCount].iType = 0;
	ASSERT( outCount < (ROUTE_SIZE*2) );

// Copy the simplified route, disable for testing
	m_iRouteIndex = 0;
	for ( i = 0; i < ROUTE_SIZE && i < outCount; i++ )
	{
		m_Route[i] = outRoute[i];
	}

	// Terminate route
	if ( i < ROUTE_SIZE )
		m_Route[i].iType = 0;

// Debug, test movement code
#if 0
//	if ( CVAR_GET_FLOAT( "simplify" ) != 0 )
		DrawRoute( pev, outRoute, 0, 255, 0, 0 );
//	else
		DrawRoute( pev, m_Route, m_iRouteIndex, 0, 255, 0 );
#endif
}

//=========================================================
// FBecomeProne - tries to send a monster into PRONE state.
// right now only used when a barnacle snatches someone, so 
// may have some special case stuff for that.
//=========================================================
BOOL CBaseMonster :: FBecomeProne ( void )
{
	if ( FBitSet ( pev->flags, FL_ONGROUND ) )
	{
		//MODDD NOTE - minus to remove something from a bitmask? Well you see something new everyday.
		// This is okay IF the bit is set. OTherwise subtacting would make the whole thing negative and weird.
		// weird to do it this way when  "pev->flags &= ~FL_ONGROUND" would've done it.
		pev->flags -= FL_ONGROUND;
	}

	m_IdealMonsterState = MONSTERSTATE_PRONE;

	
	
	//MODDD - turn off interpolation when bitten.
	if(EASY_CVAR_GET(barnacleGrabNoInterpolation) == 1){
		pev->effects |= EF_NOINTERP;
	}
	


	return TRUE;
}

//=========================================================
// CheckRangeAttack1
//=========================================================
BOOL CBaseMonster :: CheckRangeAttack1 ( float flDot, float flDist )
{
	if ( flDist > 64 && flDist <= 784 && flDot >= 0.5 )
	{
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckRangeAttack2
//=========================================================
BOOL CBaseMonster :: CheckRangeAttack2 ( float flDot, float flDist )
{
	if ( flDist > 64 && flDist <= 512 && flDot >= 0.5 )
	{
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckMeleeAttack1
//=========================================================
BOOL CBaseMonster :: CheckMeleeAttack1 ( float flDot, float flDist )
{
	// Decent fix to keep folks from kicking/punching hornets and snarks is to check the onground flag(sjb)

	//MODDD NOTE - isn't the above comment from the as-is script outdated, even before any modding? hornets are meant to be untargetable by Classify()
	//             preventing alien bioweapons from being targetable, I think. Worst case scenrio some extra "projectileIgnore()" check per enemy
	//             to see if it is worth targeting like this would be good. As for snarks I don't see the problem with trying to melee them?
	//             Natural reflex to swat off annoying fast-moving "bugs".

	if ( flDist <= 64 && flDot >= 0.7 && m_hEnemy != NULL && FBitSet ( m_hEnemy->pev->flags, FL_ONGROUND ) )
	{
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckMeleeAttack2
//=========================================================
BOOL CBaseMonster :: CheckMeleeAttack2 ( float flDot, float flDist )
{
	if ( flDist <= 64 && flDot >= 0.7 )
	{
		return TRUE;
	}
	return FALSE;
}

//=========================================================
// CheckAttacks - sets all of the bits for attacks that the
// monster is capable of carrying out on the passed entity.
//=========================================================
void CBaseMonster :: CheckAttacks ( CBaseEntity *pTarget, float flDist )
{
	Vector2D	vec2LOS;
	float	flDot;

	UTIL_MakeVectors ( pev->angles );

	vec2LOS = ( pTarget->pev->origin - pev->origin ).Make2D();
	vec2LOS = vec2LOS.Normalize();

	flDot = DotProduct (vec2LOS , gpGlobals->v_forward.Make2D() );

	// we know the enemy is in front now. We'll find which attacks the monster is capable of by
	// checking for corresponding Activities in the model file, then do the simple checks to validate
	// those attack types.
	
	// Clear all attack conditions
	ClearConditions( bits_COND_CAN_RANGE_ATTACK1 | bits_COND_CAN_RANGE_ATTACK2 | bits_COND_CAN_MELEE_ATTACK1 |bits_COND_CAN_MELEE_ATTACK2 );
	canRangedAttack1 = FALSE;
	canRangedAttack2 = FALSE;

	//couldRangedAttack1 = FALSE;


	if ( m_afCapability & bits_CAP_RANGE_ATTACK1 )
	{
		if ( CheckRangeAttack1 ( flDot, flDist ) ){
			SetConditions( bits_COND_CAN_RANGE_ATTACK1 );
			canRangedAttack1 = TRUE;
		}

		/*
		//MODDD - new feature.  Could I range attack, IF I were facing the target?
		//NOTICE: removed.  Deemed most appropriate to work with within "CheckRangeAttack1", just include this when everything BUT the dot-check passes,
		//then check for the dot-check for total passing (returning TRUE as usual).
		if ( CouldRangeAttack1 ( flDot, flDist ) ){
			
			couldRangedAttack1 = TRUE;
		}
		*/

	}
	if ( m_afCapability & bits_CAP_RANGE_ATTACK2 )
	{
		if ( CheckRangeAttack2 ( flDot, flDist ) ){
			SetConditions( bits_COND_CAN_RANGE_ATTACK2 );
			canRangedAttack2 = TRUE;
		}
	}
	if ( m_afCapability & bits_CAP_MELEE_ATTACK1 )
	{

		if ( CheckMeleeAttack1 ( flDot, flDist ) ){
			SetConditions( bits_COND_CAN_MELEE_ATTACK1 );

		}
	}
	if ( m_afCapability & bits_CAP_MELEE_ATTACK2 )
	{
		if ( CheckMeleeAttack2 ( flDot, flDist ) ){
			SetConditions( bits_COND_CAN_MELEE_ATTACK2 );

		}
	}

	//easyPrintLine("I CHECKED EM!!!!!! %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1) );

}

//=========================================================
// CanCheckAttacks - prequalifies a monster to do more fine
// checking of potential attacks. 
//=========================================================
BOOL CBaseMonster :: FCanCheckAttacks ( void )
{
	if ( HasConditions(bits_COND_SEE_ENEMY) && !HasConditions( bits_COND_ENEMY_TOOFAR ) )
	{
		return TRUE;
	}

	return FALSE;
}

//=========================================================
// CheckEnemy - part of the Condition collection process,
// gets and stores data and conditions pertaining to a monster's
// enemy. Returns TRUE if Enemy LKP was updated.
//=========================================================
int CBaseMonster :: CheckEnemy ( CBaseEntity *pEnemy )
{
	float flDistToEnemy;
	int	iUpdatedLKP;// set this to TRUE if you update the EnemyLKP in this function.

	iUpdatedLKP = FALSE;
	ClearConditions ( bits_COND_ENEMY_FACING_ME );
	
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
	easyPrintLine("CanAttack1? %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
	if ( !FVisible( pEnemy ) )
	{
		ASSERT(!HasConditions(bits_COND_SEE_ENEMY));
		SetConditions( bits_COND_ENEMY_OCCLUDED );
	}
	else{
		ClearConditions( bits_COND_ENEMY_OCCLUDED );
	}
	
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
	easyPrintLine("CanAttack2? %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
	
	
	//FLAG 666

	//easyPrintLine("HEY WHO IS THAT ENEMY!? %s:: %d %d", pEnemy->getClassname(), pEnemy->IsAlive(), pEnemy->pev->deadflag);
	//m_hEnemy!=NULL?easyPrintLine("HEY WHO IS MY ENEMY!? %s:: %d %d", m_hEnemy->getClassname(), m_hEnemy->IsAlive(), m_hEnemy->pev->deadflag):easyPrintLine("NOOEEE");
	
		//IsAlive_FromAI takes "this" mosnter as a parameter. It already knows what itself is.

	if(FClassnameIs(pev, "monster_barney")){
		int x = 666;
	}

	//Why does this wait to count an enemy as dead, but
	//bits_COND_SEE_ENEMY sure happens instantly the moment of death?
	if ( !pEnemy->IsAlive_FromAI(this) )
	{
		SetConditions ( bits_COND_ENEMY_DEAD );
		ClearConditions( bits_COND_SEE_ENEMY | bits_COND_ENEMY_OCCLUDED );
		return FALSE;
	}

	Vector vecEnemyPos = pEnemy->pev->origin;
	// distance to enemy's origin
	flDistToEnemy = ( vecEnemyPos - pev->origin ).Length();
	vecEnemyPos.z += pEnemy->pev->size.z * 0.5;

	// distance to enemy's head
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
		easyPrintLine("CanAttack3? %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}

	float flDistToEnemy2 = (vecEnemyPos - pev->origin).Length();
	if (flDistToEnemy2 < flDistToEnemy)
		flDistToEnemy = flDistToEnemy2;
	else
	{
		// distance to enemy's feet
		vecEnemyPos.z -= pEnemy->pev->size.z;
		flDistToEnemy2 = (vecEnemyPos - pev->origin).Length();
		if (flDistToEnemy2 < flDistToEnemy)
			flDistToEnemy = flDistToEnemy2;
	}
	
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
		easyPrintLine("CanAttack4? %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}

	if ( HasConditions( bits_COND_SEE_ENEMY ) )
	{
		CBaseMonster *pEnemyMonster;

		iUpdatedLKP = TRUE;
		
		//m_vecEnemyLKP = pEnemy->pev->origin;
		setEnemyLKP(pEnemy->pev->origin);

		pEnemyMonster = pEnemy->MyMonsterPointer();

		if ( pEnemyMonster )
		{
			if ( pEnemyMonster->FInViewCone ( this ) )
			{
				SetConditions ( bits_COND_ENEMY_FACING_ME );
			}
			else
				ClearConditions( bits_COND_ENEMY_FACING_ME );
		}

		if (pEnemy->pev->velocity != Vector( 0, 0, 0))
		{
			// trail the enemy a bit
			//m_vecEnemyLKP = ...
			setEnemyLKP(m_vecEnemyLKP - pEnemy->pev->velocity * RANDOM_FLOAT( -0.05, 0 ));
		}
		else
		{
			// UNDONE: use pev->oldorigin?
		}
	}
	

	//MODDD - POINT OF FRUSTRATION
	//MODDD - alteration.
	//else if ( !HasConditions(bits_COND_ENEMY_OCCLUDED|bits_COND_SEE_ENEMY) && ( flDistToEnemy <= 256 ) )
	else if ( !HasConditions(bits_COND_ENEMY_OCCLUDED|bits_COND_SEE_ENEMY) && ( flDistToEnemy <= EASY_CVAR_GET(monsterAIForceFindDistance) ) )
	{
		// if the enemy is not occluded, and unseen, that means it is behind or beside the monster.
		// if the enemy is near enough the monster, we go ahead and let the monster know where the
		// enemy is. 
		
		iUpdatedLKP = TRUE;
		//m_vecEnemyLKP = pEnemy->pev->origin;
		setEnemyLKP(pEnemy->pev->origin);
		
	}
	
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
	easyPrintLine("CanAttack5? %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}
	if ( flDistToEnemy >= m_flDistTooFar )
	{
		// enemy is very far away from monster
		SetConditions( bits_COND_ENEMY_TOOFAR );
	}
	else
		ClearConditions( bits_COND_ENEMY_TOOFAR );

	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
	easyPrintLine("Can I Check Attacks? %d", FCanCheckAttacks());
	}


	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1)easyForcePrintLine("ALRIGHT HOTSHOT %d %d", HasConditions(bits_COND_SEE_ENEMY), !HasConditions( bits_COND_ENEMY_TOOFAR ) );
	
	if ( FCanCheckAttacks() )	
	{
		CheckAttacks ( m_hEnemy, flDistToEnemy );

		//MODDD - test?

		//if(monsterID==1){ easyForcePrintLine("WOO %d", HasConditions(bits_COND_CAN_RANGE_ATTACK2));  }


	}else{
		//MODDD MAJOR - if unable to check attacks, assume they would have failed anyways. Don't keep memory of attack conditions that now survive schedule changes.
		ClearConditions( bits_COND_CAN_RANGE_ATTACK1 | bits_COND_CAN_RANGE_ATTACK2 | bits_COND_CAN_MELEE_ATTACK1 |bits_COND_CAN_MELEE_ATTACK2 );
	}

	//the smart pathfinding method does not use this, this is not very smart.
	if ( !disableEnemyAutoNode && m_movementGoal == MOVEGOAL_ENEMY )
	{
		for ( int i = m_iRouteIndex; i < ROUTE_SIZE; i++ )
		{
			//MODDD
			//if ( m_Route[ i ].iType == (bits_MF_IS_GOAL|bits_MF_TO_ENEMY) )
			//This means, the two bits MUST be within iType. Not just one or the other.
			if ( (m_Route[ i ].iType & (bits_MF_IS_GOAL|bits_MF_TO_ENEMY)) == (bits_MF_IS_GOAL|bits_MF_TO_ENEMY) )
			{
				// UNDONE: Should we allow monsters to override this distance (80?)
				if ( (m_Route[ i ].vecLocation - m_vecEnemyLKP).Length() > 80 )
				{
					// Refresh
					FRefreshRoute();
					return iUpdatedLKP;
				}
			}
		}
	}
	
	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
	easyPrintLine("CanAttack6 %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1));
	}


	return iUpdatedLKP;
}

















//Remove any empty spaces from the stack and push anything to the right to the left to fill the space.
//ex: say we have these enemis in m_hOldEnemey:
//	[0]		[1]		[2]		[3]
//	alive	dead	alive	alive
// the [1], being dead, should be removed and replaced by [2]. Then [2] gets replaced by 3.
//Or,
//	[0]		[1]		[2]		[3]
//	[0]		[2]		[3]		N/A
//	alive	alive	alive	N/A
//(also reducing m_intOldEnemeyNextIndex)
void CBaseMonster::refreshStack() {

	//minus 1, because m_intOldEnemyNextIndex is ACTUALLY the place to put a new enemy. It itself, is not actually the most recently added enemy. But one index less, is.
	//This is also why it must be above 0 to actually have anything (index of 0 - 1 = -1, meaning empty. index of 2 - 1 = 1, meaning the most recent addition is at #1).

	for(int i = m_intOldEnemyNextIndex - 1; i >= 0; i--){

		//IsAlive_FromAI takes "this" mosnter as a parameter. It already knows what itself is.
		if (m_hOldEnemy[i] != NULL && m_hOldEnemy[i]->IsAlive_FromAI(this))
		{
			//this is okay.
		}
		else {
			m_hOldEnemy[i] = NULL;
			//do any entries above me need to be pushed down?
			//such as, say there are three enemies, m_intOldEnemyNextIndex is 3, and indexes [0], [1], and [2] are used of m_hOldEnemy.
			//If [1] is dead and removed, [1] is set to NULL.  Shouldn't [2] move to [1]? And if there were a [3], that should replace the blank space of [2].

			for (int i2 = i + 1; i2 < m_intOldEnemyNextIndex; i2++) {
				m_hOldEnemy[i2 - 1] = m_hOldEnemy[i2];
				m_vecOldEnemy[i2 - 1] = m_vecOldEnemy[i2];
			}
			//clearly, one less enemy to remember.
			m_intOldEnemyNextIndex--;
		}
	}//END OF for(...)

}



 //=========================================================
 // PushEnemy - remember the last few enemies, always remember the player
 //=========================================================
void CBaseMonster::PushEnemy( CBaseEntity *pEnemy, Vector &vecLastKnownPos )
{
	int i;

	if (pEnemy == NULL)
		return;


	//MODDD - NEW. Is it ok to deny if the currently suggested addition is the current enemy (m_hEnemy) already? It is possible this never happens.
	if(pEnemy == m_hEnemy){
		//no need to say this every time now.
		//easyForcePrintLine("%s:ID%d !!!PushEnemy: denied adding the current enemy to the stack! %s", this->getClassnameShort(), this->monsterID, m_hEnemy->getClassnameShort());
		return;
	}


	// UNDONE: blah, this is bad, we should use a stack but I'm too lazy to code one.
	//MODDD - HERES JOHNNY BITCH. I'll give you a stack! Or the idea at least (pick the most recent additon, not necessarily #0 just because it's an ok option)
	/*
	for (i = 0; i < MAX_OLD_ENEMIES; i++)
	{
	if (m_hOldEnemy[i] == pEnemy)
	return;
	if (m_hOldEnemy[i] == NULL) // someone died, reuse their slot
	break;
	}
	if (i >= MAX_OLD_ENEMIES)
	return;
	*/



	refreshStack();



	//a stack just needs to see the most recent position. Defaults to 0 of course.
	//It is actualy a sign of the next "target" index to fill. being 0 means index "0" is ready to fill. Being index 4 (MAX_OLD_ENEMIES at the time of writing) exactly means it is full.
	if (m_intOldEnemyNextIndex < MAX_OLD_ENEMIES) {
		

		
		//first a check. Is this enemy already in the list?
		for(i = 0; i < m_intOldEnemyNextIndex; i++){
			if(m_hOldEnemy[i] == pEnemy){
				//Already in the list. Do not add.
				return;
			}
		}


		m_hOldEnemy[m_intOldEnemyNextIndex] = pEnemy;
		m_vecOldEnemy[m_intOldEnemyNextIndex] = vecLastKnownPos;

		m_intOldEnemyNextIndex++;
		return;
	}else{
		//no room. Deny adding to my memory.
		return;
	}

}

//=========================================================
// PopEnemy - try remembering the last few enemies
//=========================================================
BOOL CBaseMonster::PopEnemy()
{
	// UNDONE: blah, this is bad, we should use a stack but I'm too lazy to code one.
	//MODDD - HERES JOHNNY BITCH. I'll give you a stack! Or the idea at least (pick the most recent additon, not necessarily #0 just because it's an ok option)
	/*
	for (int i = MAX_OLD_ENEMIES - 1; i >= 0; i--)
	{
	if (m_hOldEnemy[i] != NULL)
	{
	if (m_hOldEnemy[i]->IsAlive( )) // cheat and know when they die
	{
	m_hEnemy = m_hOldEnemy[i];
	m_vecEnemyLKP = m_vecOldEnemy[i];
	// ALERT( at_console, "remembering\n");
	return TRUE;
	}
	else
	{
	m_hOldEnemy[i] = NULL;
	}
	}
	}
	*/


	refreshStack();

	if (m_intOldEnemyNextIndex > 0) {

		m_hEnemy = m_hOldEnemy[m_intOldEnemyNextIndex - 1];

		//m_vecEnemyLKP = m_vecOldEnemy[m_intOldEnemyNextIndex];
		setEnemyLKP(m_vecOldEnemy[m_intOldEnemyNextIndex]);

		m_intOldEnemyNextIndex--;
		return TRUE;
	}

	return FALSE;
}







void CBaseMonster::DrawFieldOfVision(){
	
	//1: +-0 deg.
	//0: +-90 deg.
	//-1: +-180 deg.

	//turn FOV into a number of degrees... or radians:

	//pev->angles.x

	float degShift = m_flFieldOfView*-90 + 90;


	float angForwardRad = (pev->angles.y) * (CONST_DEG_TO_RAD_CONV);

	float fovForwardExtentX = cos(angForwardRad);
	float fovForwardExtentY = sin(angForwardRad);

	Vector vecFovExtentForward = Vector(fovForwardExtentX, fovForwardExtentY, 0) * 500;


	//get the X and Y (top-down perspective) of looking this many degrees away from what way we are facing groundwise. pev->angles.x:
	float angLeftRad = (pev->angles.y - degShift) * (CONST_DEG_TO_RAD_CONV);
	float angRightRad = (pev->angles.y + degShift) * (CONST_DEG_TO_RAD_CONV);

	float fovExtentLeftX = cos(angLeftRad);
	float fovExtentLeftY = sin(angLeftRad);

	Vector vecFovExtentLeft = Vector(fovExtentLeftX, fovExtentLeftY, 0) * 500;
	
	float fovExtentRightX = cos(angRightRad);
	float fovExtentRightY = sin(angRightRad);
	
	Vector vecFovExtentRight = Vector(fovExtentRightX, fovExtentRightY, 0) * 500;
	
	
	//UTIL_MakeVectors ( pev->angles );
	//::UTIL_drawLineFrame(pev->origin, pev->origin + giveZ(gpGlobals->v_forward.Make2D().Normalize(), this->EyePosition().z ) * 500, 12, 0, 0, 255);


	Vector eyePos = pev->origin + pev->view_ofs;
	//pev->view_ofs ?
	::UTIL_drawLineFrame(eyePos, eyePos + vecFovExtentForward, 12, 0, 0, 255);

	::UTIL_drawLineFrame(eyePos, eyePos + vecFovExtentLeft, 12, 0, 255, 0);
	::UTIL_drawLineFrame(eyePos, eyePos + vecFovExtentRight, 12, 0, 255, 0);

}//END OF DrawFieldOfVision




/*
//MODDD - new paramter: forceReset
void CBaseMonster :: SetActivity ( Activity NewActivity )
{
	CBaseMonster ::SetActivity(NewActivity, FALSE);
}
*/


//MODDD - new
BOOL CBaseMonster::allowedToSetActivity(void){
	return TRUE;
}

//MODDD - new
int CBaseMonster::tryGetTaskID(void){

	//NOTICE: made "GetTask" check for "m_pSchedule" being null.  It's good over there now.
	/*
	if(m_pSchedule == NULL){
		//Note that "GetTask" does not check for m_pSchedule being Null, despite going thru it.  Possible
		//source of crashes...?
		return -3;
	}
	*/

	Task_t* taskAttempt = GetTask();
	if(taskAttempt == NULL){
		return -2;
	}else{
		return taskAttempt->iTask;
	}
}

const char* CBaseMonster::tryGetScheduleName(void){
	if(m_pSchedule == NULL){
		return "NULL!";
	}else{
		return m_pSchedule->pName;
	}
}



//=========================================================
// SetActivity 
//=========================================================
//MODDD- VIRTUAL
void CBaseMonster :: SetActivity ( Activity NewActivity )
{


	if(monsterID == 14){
		int x = 666;
	}

	
	//MODDD TODO - on any loosely linked clones of SetActivity, they should also set "signalActivityUpdate" to false.
	//This method getting called is supposed to be enough to satisfy the signal, like to force resetting to itself after a possible
	//raw anim change.
	signalActivityUpdate = FALSE;

	
	if(this->allowedToSetActivity() == TRUE){

	}else{
		//NO.
		return;
	}


	int iSequence;

	//nah...
	//easyPrintLine("SMOOTH AS DER %d %d %d", HasConditions(bits_COND_CAN_MELEE_ATTACK1), m_Activity, m_IdealActivity );

	//MODDD - possible insertion.
	if(usesAdvancedAnimSystem()){
		iSequence = LookupActivityHard ( NewActivity );
	}else{
		iSequence = LookupActivity ( NewActivity );
	}

	
	

	//easyForcePrintLine("SET ACTIVITY: %d I DID THIS: %d", NewActivity, iSequence);


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
			
		//easyPrintLine("ANIMATION CHANGE!!!! C %d");
		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)

		//easyPrintLine("$ASS$ %d : %d",  NewActivity, forceReset );
		

		ResetSequenceInfo( );
		//pev->frame = 0;
		//pev->framerate = 2;
		m_fSequenceFinished = FALSE;


		usingCustomSequence = FALSE;  //MODDD - automatically picked.
		SetYawSpeed();
	}
	else
	{
		// Not available try to get default anim
		ALERT ( at_aiconsole, "%s has no sequence for act:%d\n", STRING(pev->classname), NewActivity );
		
		pev->sequence		= 0;	// Set to the reset anim (if it's there)
		usingCustomSequence = FALSE;  //MODDD - automatically picked.
	}

	m_Activity = NewActivity; // Go ahead and set this so it doesn't keep trying when the anim is not present
	
	// In case someone calls this with something other than the ideal activity
	m_IdealActivity = m_Activity;


}







//Notice that the "ForceLoops" variants below are completely different from m_iForceLoops. 
//If m_iForceLoops is anything but -1, it still loses to the provided forceLoops parameter, but
//any animations after will still be affected by m_iForceLoops if non-negative instead.

void CBaseMonster::SetSequenceByIndex(int iSequence)
{
	SetSequenceByIndex(iSequence, FALSE);
}
void CBaseMonster::SetSequenceByName(char* szSequence)
{
	SetSequenceByName(szSequence, FALSE);
}


void CBaseMonster::SetSequenceByIndex(int iSequence, float flFramerateMulti)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByIndex(iSequence, FALSE);
}
void CBaseMonster::SetSequenceByName(char* szSequence, float flFramerateMulti)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByName(szSequence, FALSE);
}

//MODDD - if we have a hardcoded number (index; offset from 0, the first sequence in the model), just use that.
void CBaseMonster::SetSequenceByIndex(int iSequence, BOOL safeReset)
{
	// Set to the desired anim, or default anim if the desired is not present
	if ( iSequence > ACTIVITY_NOT_AVAILABLE )
	{
		if ( pev->sequence != iSequence || !m_fSequenceLoops )
		{
			resetFrame();
		}

		pev->sequence		= iSequence;	// Set to the reset anim (if it's there)
		
		//easyForcePrintLine("SetSequenceByIndex: ses:%d fs:%.2f", iSequence, m_flFramerateSuggestion);
		if(!safeReset){
			ResetSequenceInfo( );
		}else{
			ResetSequenceInfoSafe();
		}
		usingCustomSequence = TRUE;
		SetYawSpeed();
	}
	else
	{
		// Not available try to get default anim
		//MODDD - why is %f the 2nd arg here?  isn't that for floats, not strings?  Disabling this line.
		//ALERT ( at_aiconsole, "%s has no sequence named:%f\n", STRING(pev->classname), szSequence );
		pev->sequence		= 0;	// Set to the reset anim (if it's there)
		usingCustomSequence = FALSE;
	}
}//END OF SetSequenceByNumber()
void CBaseMonster::SetSequenceByName(char* szSequence, BOOL safeReset)
{
	int iSequence;
	iSequence = LookupSequence ( szSequence );
	SetSequenceByIndex(iSequence, safeReset);
}

void CBaseMonster::SetSequenceByIndex(int iSequence, float flFramerateMulti, BOOL safeReset)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByIndex(iSequence, safeReset);
}
void CBaseMonster::SetSequenceByName(char* szSequence, float flFramerateMulti, BOOL safeReset)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByName(szSequence, safeReset);
}







//MODDD - call the above methods, but force the "m_fSequenceLoops" flag to a provided value (true / false).
void CBaseMonster::SetSequenceByIndexForceLoops(int iSequence, BOOL forceLoops)
{
	SetSequenceByIndex(iSequence, FALSE);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByNameForceLoops(char* szSequence, BOOL forceLoops)
{
	SetSequenceByName(szSequence, FALSE);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByIndexForceLoops(int iSequence, float flFramerateMulti, BOOL forceLoops)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByIndex(iSequence, FALSE);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByNameForceLoops(char* szSequence, float flFramerateMulti, BOOL forceLoops)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByName(szSequence, FALSE);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByIndexForceLoops(int iSequence, BOOL safeReset, BOOL forceLoops)
{
	SetSequenceByIndex(iSequence, safeReset);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByNameForceLoops(char* szSequence, BOOL safeReset, BOOL forceLoops)
{
	SetSequenceByName(szSequence, safeReset);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByIndexForceLoops(int iSequence, float flFramerateMulti, BOOL safeReset, BOOL forceLoops)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByIndex(iSequence, safeReset);
	m_fSequenceLoops = forceLoops;
}

void CBaseMonster::SetSequenceByNameForceLoops(char* szSequence, float flFramerateMulti, BOOL safeReset, BOOL forceLoops)
{
	m_flFramerateSuggestion = flFramerateMulti;
	SetSequenceByName(szSequence, safeReset);
	m_fSequenceLoops = forceLoops;
}










//MODDD - new method.  Return if an alternative to the incremental process of "WALK_MOVE" should be used instead.
BOOL CBaseMonster::getHasPathFindingModA(){
	return FALSE; //by default, false.
}
BOOL CBaseMonster::getHasPathFindingMod(){
	return FALSE; //by default, false.
}



//=========================================================
// CheckLocalMove - returns TRUE if the caller can walk a 
// straight line from its current origin to the given 
// location. If so, don't use the node graph!
//
// if a valid pointer to a int is passed, the function
// will fill that int with the distance that the check 
// reached before hitting something. THIS ONLY HAPPENS
// IF THE LOCAL MOVE CHECK FAILS!
//
// !!!PERFORMANCE - should we try to load balance this?
// DON"T USE SETORIGIN! 
//=========================================================
//MODDD - SHUN!    was 16
#define LOCAL_STEP_SIZE	16
#define LOCAL_STEP_SIZE_MOD 10

//MODDD - ref
EASY_CVAR_EXTERN(drawDebugPathfinding)



static const float stepChoiceArray[] = {LOCAL_STEP_SIZE, LOCAL_STEP_SIZE_MOD};







int CBaseMonster::CheckLocalMoveHull(const Vector &vecStart, const Vector &vecEnd, CBaseEntity *pTarget, float *pflDist  )
{

	//MODDD - todo at some point. Is that really a good idea... always TRUE here??
	return LOCALMOVE_VALID;


	
	TraceResult tr;

	UTIL_TraceHull( vecStart + Vector( 0, 0, 32), vecEnd + Vector( 0, 0, 32), dont_ignore_monsters, large_hull, edict(), &tr );
	//UTIL_TraceHull( vecStart + Vector( 0, 0, 32), vecEnd + Vector( 0, 0, 32), dont_ignore_monsters, head_hull, edict(), &tr );

	// ALERT( at_console, "%.0f %.0f %.0f : ", vecStart.x, vecStart.y, vecStart.z );
	// ALERT( at_console, "%.0f %.0f %.0f\n", vecEnd.x, vecEnd.y, vecEnd.z );

	if (pflDist)
	{
		*pflDist = ( (tr.vecEndPos - Vector( 0, 0, 32 )) - vecStart ).Length();// get the distance.
	}

	// ALERT( at_console, "check %d %d %f\n", tr.fStartSolid, tr.fAllSolid, tr.flFraction );
	if (tr.fStartSolid || tr.flFraction < 1.0)
	{
		if ( pTarget && pTarget->edict() == gpGlobals->trace_ent )
			return LOCALMOVE_VALID;
		return LOCALMOVE_INVALID;
	}

	return LOCALMOVE_VALID;


}




//MODDD MASSREVERT- CheckLocalMove 

//=========================================================
// CheckLocalMove - returns TRUE if the caller can walk a 
// straight line from its current origin to the given 
// location. If so, don't use the node graph!
//
// if a valid pointer to a int is passed, the function
// will fill that int with the distance that the check 
// reached before hitting something. THIS ONLY HAPPENS
// IF THE LOCAL MOVE CHECK FAILS!
//
// !!!PERFORMANCE - should we try to load balance this?
// DON"T USE SETORIGIN! 
//=========================================================
#define LOCAL_STEP_SIZE	16
int CBaseMonster :: CheckLocalMove ( const Vector &vecStart, const Vector &vecEnd, CBaseEntity *pTarget, float *pflDist )
{
	Vector	vecStartPos;// record monster's position before trying the move
	float flYaw;
	float flDist;
	float flStep, stepSize;
	int	iReturn;
#if defined(USE_MOVEMENT_BOUND_FIX) || defined(USE_MOVEMENT_BOUND_FIX_ALT)
	Vector oldMins;
	Vector oldMaxs;
#endif

	vecStartPos = pev->origin;
	
	
	flYaw = UTIL_VecToYaw ( vecEnd - vecStart );// build a yaw that points to the goal.
	flDist = ( vecEnd - vecStart ).Length2D();// get the distance.
	iReturn = LOCALMOVE_VALID;// assume everything will be ok.

	// move the monster to the start of the local move that's to be checked.
	UTIL_SetOrigin( pev, vecStart );// !!!BUGBUG - won't this fire triggers? - nope, SetOrigin doesn't fire

	if ( !(pev->flags & (FL_FLY|FL_SWIM)) )
	{
		DROP_TO_FLOOR( ENT( pev ) );//make sure monster is on the floor!
	}

	//pev->origin.z = vecStartPos.z;//!!!HACKHACK

//	pev->origin = vecStart;

/*
	if ( flDist > 1024 )
	{
		// !!!PERFORMANCE - this operation may be too CPU intensive to try checks this large.
		// We don't lose much here, because a distance this great is very likely
		// to have something in the way.

		// since we've actually moved the monster during the check, undo the move.
		pev->origin = vecStartPos;
		return FALSE;
	}
*/

	


	
#if defined(USE_MOVEMENT_BOUND_FIX)
	if(needsMovementBoundFix()){
		//Plays better with the engine to do checks with these bounds against ramps. No clue why.
		//UTIL_SetSize( pev, Vector(-30, -30, 0), Vector(30, 30, 40) );

		//record the old bounds for getting them back.
		oldMins = pev->mins;
		oldMaxs = pev->maxs;

		UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );
	}
#endif

	




	// this loop takes single steps to the goal.
	for ( flStep = 0 ; flStep < flDist ; flStep += LOCAL_STEP_SIZE )
	{
		Vector oldOrigin;
		stepSize = LOCAL_STEP_SIZE;

		if ( (flStep + LOCAL_STEP_SIZE) >= (flDist-1) )
			stepSize = (flDist - flStep) - 1;
		
//		UTIL_ParticleEffect ( pev->origin, g_vecZero, 255, 25 );


		
		oldOrigin = pev->origin;
		if ( !WALK_MOVE( ENT(pev), flYaw, stepSize, WALKMOVE_CHECKONLY ) )
		{// can't take the next step, fail!






#ifdef USE_MOVEMENT_BOUND_FIX_ALT
			if( EASY_CVAR_GET(pathfindLargeBoundFix) == 1 && needsMovementBoundFix() ){
				CBaseEntity* whut = CBaseEntity::Instance(gpGlobals->trace_ent);

				if(whut != NULL){
					/*
					easyForcePrintLine("%s:%d MBFclm: %s:%d isW:%d", getClassname(), monsterID, whut->getClassname(), 
						(whut->GetMonsterPointer() != NULL)?whut->GetMonsterPointer()->monsterID:-1
						, whut->IsWorld());
					*/
				}

				if(whut == NULL || whut->IsWorld()){
					//WAIT. You get one more chance.
					oldMins = pev->mins;
					oldMaxs = pev->maxs;
					UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );
					pev->origin = oldOrigin;


					if(!WALK_MOVE( ENT(pev), flYaw, stepSize, WALKMOVE_CHECKONLY )){
						//still fail? it is ok to run the script below that accepts failure.
						UTIL_SetSize(pev, oldMins, oldMaxs);
						//WAIT. Allow below to run in this case or not?  I don't know man!!
						//But slowdowns can happen sometimes from this.  Why??
						//continue;
					}else{
						//success? no, allowed to keep going.
						UTIL_SetSize(pev, oldMins, oldMaxs);
						continue;
					}
					UTIL_SetSize(pev, oldMins, oldMaxs);
				}
			}

#endif




			if ( pflDist != NULL )
			{
				*pflDist = flStep;
			}

			const char* DEBUGSHIT = (gpGlobals->trace_ent!=NULL)?CBaseEntity::Instance(gpGlobals->trace_ent)->getClassname():"WHUT";

			if ( pTarget && pTarget->edict() == gpGlobals->trace_ent )
			{
				// if this step hits target ent, the move is legal.
				iReturn = LOCALMOVE_VALID;
				break;
			}
			else
			{
				// If we're going toward an entity, and we're almost getting there, it's OK.
//				if ( pTarget && fabs( flDist - iStep ) < LOCAL_STEP_SIZE )
//					fReturn = TRUE;
//				else
				iReturn = LOCALMOVE_INVALID;
				break;
			}

		}
	}

	
#if defined(USE_MOVEMENT_BOUND_FIX)
	if(needsMovementBoundFix()){
		//undo the bound change.
		UTIL_SetSize( pev, oldMins, oldMaxs );
	}
#endif

	if ( iReturn == LOCALMOVE_VALID && 	!(pev->flags & (FL_FLY|FL_SWIM) ) && (!pTarget || (pTarget->pev->flags & FL_ONGROUND)) )
	{
		// The monster can move to a spot UNDER the target, but not to it. Don't try to triangulate, go directly to the node graph.
		// UNDONE: Magic # 64 -- this used to be pev->size.z but that won't work for small creatures like the headcrab
		if ( fabs(vecEnd.z - pev->origin.z) > 64 )
		{
			iReturn = LOCALMOVE_INVALID_DONT_TRIANGULATE;
		}
	}
	/*
	// uncommenting this block will draw a line representing the nearest legal move.
	WRITE_BYTE(MSG_BROADCAST, SVC_TEMPENTITY);
	WRITE_BYTE(MSG_BROADCAST, TE_SHOWLINE);
	WRITE_COORD(MSG_BROADCAST, pev->origin.x);
	WRITE_COORD(MSG_BROADCAST, pev->origin.y);
	WRITE_COORD(MSG_BROADCAST, pev->origin.z);
	WRITE_COORD(MSG_BROADCAST, vecStart.x);
	WRITE_COORD(MSG_BROADCAST, vecStart.y);
	WRITE_COORD(MSG_BROADCAST, vecStart.z);
	*/



	if( EASY_CVAR_GET(drawDebugPathfinding) == 1){
		switch(iReturn){
			case LOCALMOVE_INVALID:
				//ORANGE
				//DrawRoute( pev, m_Route, m_iRouteIndex, 239, 165, 16 );
				DrawRoute( pev, m_Route, m_iRouteIndex, 48, 33, 4 );
			break;
			case LOCALMOVE_INVALID_DONT_TRIANGULATE:
				//RED
				//DrawRoute( pev, m_Route, m_iRouteIndex, 234, 23, 23 );
				DrawRoute( pev, m_Route, m_iRouteIndex, 47, 5, 5 );
			break;
			case LOCALMOVE_VALID:
				//GREEN
				//DrawRoute( pev, m_Route, m_iRouteIndex, 97, 239, 97 );
				DrawRoute( pev, m_Route, m_iRouteIndex, 20, 48, 20 );
			break;
		}
	}





	// since we've actually moved the monster during the check, undo the move.
	UTIL_SetOrigin( pev, vecStartPos );

	return iReturn;
}

float CBaseMonster :: OpenDoorAndWait( entvars_t *pevDoor )
{
	float flTravelTime = 0;

	//ALERT(at_aiconsole, "A door. ");
	CBaseEntity *pcbeDoor = CBaseEntity::Instance(pevDoor);
	if (pcbeDoor && !pcbeDoor->IsLockedByMaster())
	{
		//ALERT(at_aiconsole, "unlocked! ");
		pcbeDoor->Use(this, this, USE_ON, 0.0);
		//ALERT(at_aiconsole, "pevDoor->nextthink = %d ms\n", (int)(1000*pevDoor->nextthink));
		//ALERT(at_aiconsole, "pevDoor->ltime = %d ms\n", (int)(1000*pevDoor->ltime));
		//ALERT(at_aiconsole, "pev-> nextthink = %d ms\n", (int)(1000*pev->nextthink));
		//ALERT(at_aiconsole, "pev->ltime = %d ms\n", (int)(1000*pev->ltime));
		flTravelTime = pevDoor->nextthink - pevDoor->ltime;
		//ALERT(at_aiconsole, "Waiting %d ms\n", (int)(1000*flTravelTime));
		if ( pcbeDoor->pev->targetname )
		{
			edict_t *pentTarget = NULL;
			for (;;)
			{
				pentTarget = FIND_ENTITY_BY_TARGETNAME( pentTarget, STRING(pcbeDoor->pev->targetname));

				if ( VARS( pentTarget ) != pcbeDoor->pev )
				{
					if (FNullEnt(pentTarget))
						break;

					if ( FClassnameIs ( pentTarget, STRING(pcbeDoor->pev->classname) ) )
					{
						CBaseEntity *pDoor = Instance(pentTarget);
						if ( pDoor )
							pDoor->Use(this, this, USE_ON, 0.0);
					}
				}
			}
		}
	}

	return gpGlobals->time + flTravelTime;
}


//=========================================================
// AdvanceRoute - poorly named function that advances the 
// m_iRouteIndex. If it goes beyond ROUTE_SIZE, the route 
// is refreshed. 
//=========================================================
void CBaseMonster :: AdvanceRoute ( float distance, float flInterval )
{

	if ( m_iRouteIndex == ROUTE_SIZE - 1 )
	{
		// time to refresh the route.
		if ( !FRefreshRoute() )
		{
			ALERT ( at_aiconsole, "Can't Refresh Route!!\n" );
		}
	}
	else
	{
		if ( ! (m_Route[ m_iRouteIndex ].iType & bits_MF_IS_GOAL) )
		{
			// If we've just passed a path_corner, advance m_pGoalEnt
			
			
			//if ( (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK) == bits_MF_TO_PATHCORNER )
			if ( (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK) & bits_MF_TO_PATHCORNER )
				m_pGoalEnt = m_pGoalEnt->GetNextTarget();

			// IF both waypoints are nodes, then check for a link for a door and operate it.
			//
			if (  (m_Route[m_iRouteIndex].iType   & bits_MF_TO_NODE) == bits_MF_TO_NODE
			   && (m_Route[m_iRouteIndex+1].iType & bits_MF_TO_NODE) == bits_MF_TO_NODE)
			{
				//ALERT(at_aiconsole, "SVD: Two nodes. ");

				int iSrcNode  = WorldGraph.FindNearestNode(m_Route[m_iRouteIndex].vecLocation, this );
				int iDestNode = WorldGraph.FindNearestNode(m_Route[m_iRouteIndex+1].vecLocation, this );

				int iLink;
				WorldGraph.HashSearch(iSrcNode, iDestNode, iLink);


				if ( iLink >= 0 && WorldGraph.m_pLinkPool[iLink].m_pLinkEnt != NULL )
				{
					//ALERT(at_aiconsole, "A link. ");
					if ( WorldGraph.HandleLinkEnt ( iSrcNode, WorldGraph.m_pLinkPool[iLink].m_pLinkEnt, m_afCapability, CGraph::NODEGRAPH_DYNAMIC ) )
					{
						//ALERT(at_aiconsole, "usable.");
						entvars_t *pevDoor = WorldGraph.m_pLinkPool[iLink].m_pLinkEnt;
						if (pevDoor)
						{
							m_flMoveWaitFinished = OpenDoorAndWait( pevDoor );
//							ALERT( at_aiconsole, "Wating for door %.2f\n", m_flMoveWaitFinished-gpGlobals->time );
						}
					}
				}
				//ALERT(at_aiconsole, "\n");
			}
			//MODDD - let monsters know that this is going to a new node (HGrunt strafe is forced to stop).
			onNewRouteNode();

			m_iRouteIndex++;
		}
		else	// At goal!!!
		{
			//MODDD NOTE - hold on. This extra check before calling "MovementComplete" is retail behavior at least.
			//Factoring in framerate, interval and "pathfindNodeToleranceMulti" for passing are new.
			//But why is this check here to begin with? We can only get here (AdvanceRoute) from "ShouldAdvanceRoute" passing which already does the
			//distance check towards the next node or goal.  So it's redundant: how could this check fail if ShouldAdvanceRoute said we should advance?
			//The checks above for not being towards the goal node / point don't even do any extra checks, they just bump up the node as told.
			//I got nothing.

			/*
			const float rawMoveSpeedPerSec = (m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval );
			const float moveDistTest = rawMoveSpeedPerSec * EASY_CVAR_GET(pathfindNodeToleranceMulti);  //defaults to 1 for no effect.
			const float moveDistTol = max(moveDistTest, 8);  //must be at least 8.

			if(EASY_CVAR_GET(pathfindPrintout)==1)easyForcePrintLine("MovementComplete Call 456: dist:%.2f spd:%.2f req:%.2f", distance, rawMoveSpeedPerSec, moveDistTol );
			//BETTER FIX!
			//if ( distance < m_flGroundSpeed * 0.2  ) // FIX
			if( distance < moveDistTol)
			{
				MovementComplete();
			}
			*/
			
			//MODDD - replaced with ShouldAdvanceRoute call since the logic's really the same.  
			//...this exact check HAD to pass to even make it to this point. Just skip it and continue already.
			//   We wanted to advance, we called this method BECAUSE we were close enough, just go. Say you're done. Thank you.
			//if(ShouldAdvanceRoute(distance, flInterval)){
				MovementComplete();
			//}


		}
	}
}


int CBaseMonster :: RouteClassify( int iMoveFlag )
{
	int movementGoal;

	movementGoal = MOVEGOAL_NONE;

	if ( iMoveFlag & bits_MF_TO_TARGETENT )
		movementGoal = MOVEGOAL_TARGETENT;
	else if ( iMoveFlag & bits_MF_TO_ENEMY )
		movementGoal = MOVEGOAL_ENEMY;
	else if ( iMoveFlag & bits_MF_TO_PATHCORNER )
		movementGoal = MOVEGOAL_PATHCORNER;
	else if ( iMoveFlag & bits_MF_TO_NODE )
		movementGoal = MOVEGOAL_NODE;
	else if ( iMoveFlag & bits_MF_TO_LOCATION )
		movementGoal = MOVEGOAL_LOCATION;

	return movementGoal;
}

//=========================================================
// BuildRoute
//=========================================================


BOOL CBaseMonster :: BuildRoute ( const Vector &vecGoal, int iMoveFlag, CBaseEntity *pTarget )
{
	float flDist;
	Vector	vecApex;
	int	iLocalMove;


	RouteNew();

	m_movementGoal = RouteClassify( iMoveFlag );

// so we don't end up with no moveflags
	m_Route[ 0 ].vecLocation = vecGoal;
	m_Route[ 0 ].iType = iMoveFlag | bits_MF_IS_GOAL;




	if(monsterID == 6){
		int arrrr = 45;
	}

	
// check simple local move
	iLocalMove = CheckLocalMove( pev->origin, vecGoal, pTarget, &flDist );

	if ( iLocalMove == LOCALMOVE_VALID )
	{
		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d BuildRoute: I GOT SATISIFED 1", getClassnameShort(), monsterID) );
		// monster can walk straight there!
		return TRUE;
	}
// try to triangulate around any obstacles.
	else if ( iLocalMove != LOCALMOVE_INVALID_DONT_TRIANGULATE && FTriangulate( pev->origin, vecGoal, flDist, pTarget, &vecApex ) )
	{
		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d BuildRoute: I GOT SATISIFED 2", getClassnameShort(), monsterID) );
		// there is a slightly more complicated path that allows the monster to reach vecGoal
		m_Route[ 0 ].vecLocation = vecApex;
		m_Route[ 0 ].iType = (iMoveFlag | bits_MF_TO_DETOUR);

		m_Route[ 1 ].vecLocation = vecGoal;
		m_Route[ 1 ].iType = iMoveFlag | bits_MF_IS_GOAL;

			
			//WRITE_BYTE(MSG_BROADCAST, SVC_TEMPENTITY);
			//WRITE_BYTE(MSG_BROADCAST, TE_SHOWLINE);
			//WRITE_COORD(MSG_BROADCAST, vecApex.x );
			//WRITE_COORD(MSG_BROADCAST, vecApex.y );
			//WRITE_COORD(MSG_BROADCAST, vecApex.z );
			//WRITE_COORD(MSG_BROADCAST, vecApex.x );
			//WRITE_COORD(MSG_BROADCAST, vecApex.y );
			//WRITE_COORD(MSG_BROADCAST, vecApex.z + 128 );
			

		RouteSimplify( pTarget );
		return TRUE;
	}

// last ditch, try nodes
	if ( FGetNodeRoute( vecGoal ) )
	{
//		ALERT ( at_console, "Can get there on nodes\n" );
		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d BuildRoute: I GOT SATISIFED 3", getClassnameShort(), monsterID) );
		m_vecMoveGoal = vecGoal;
		RouteSimplify( pTarget );
		return TRUE;
	}


	



	Vector vecDir = (vecGoal - pev->origin).Normalize();


	
	//TODO IN THE PATH FINDING:
	//elevation check?  See if the goal (next node; m_Route[m_iRouteIndex].vecLocation) position is above my current position, or below.
	// If the goal is  on the same level... can't really do the fix.
	// If the goal is below me, it's the top-ramp fix.  (currently in)
	// If the goal is above me, it's the bottom-ramp fix (not done)



	
	//initial attempt.
	
	/*

	//
	BOOL localMovePass = (CheckLocalMove ( pev->origin, vecGoal, pTarget, &flDist ) == LOCALMOVE_VALID);
	//BOOL localMovePass = (CheckLocalMove ( pev->origin, pev->origin + vecDir * flWaypointDist, pTargetEnt, &flDist ) == LOCALMOVE_VALID);
	//BOOL localMovePass = (CheckLocalMove ( pev->origin, pev->origin + vecDir * flCheckDist, pTargetEnt, &flDist ) == LOCALMOVE_VALID);
	EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE FIRST PASS? %d", getClassnameShort(), monsterID, localMovePass) );
	*/
			//debugVectorMode = 0;
			//	debugVector1 = pev->origin;
			//	debugVector2 = pev->origin + vecDir * flCheckDist;
			//	debugVector3 = m_Route[ m_iRouteIndex ].vecLocation;
				//debugVectorsSet = TRUE;
		



	debugVectorsSet = FALSE;
	debugFailColor = FALSE;


	if(EASY_CVAR_GET(pathfindRampFix) == 1){
	//if( !localMovePass ){
		//still a shot...?
		
		
		
		Vector rampTopPoint1;
		Vector rampTopPoint2;

		BOOL localMovePass = FALSE;




		int rampFixAttempt = 0; 
		//-1 = to go down (I'm higher than the goal, have to go down)
		//0 = none (too flat, no ramp fix possible)
		//1 = to go up (I'm lower than the goal, have to go up)

		if(pev->origin.z > vecGoal.z + 20 ){
			//above. 
			rampFixAttempt = 1;
		}else if(pev->origin.z < vecGoal.z + -20 ){
			//below.
			rampFixAttempt = -1;
		}

		if(rampFixAttempt != 0){
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: TRYING RAMPFIX...", getClassnameShort(), monsterID) );
	
			
			//first, check the point where the first localMove check failed. Is this a ramp? Check the slope.
			
			debugVectorMode = 1;

			BOOL hasTowardsRampNode = FALSE;



			Vector vecDirFlatUnnormal = Vector(vecDir.x, vecDir.y, 0);
			//Vector initFailPoint = pev->origin + vecDir * flDist;
			Vector vecMyOrigin = pev->origin;
			//Vector vecNextNode = m_Route[m_iRouteIndex].vecLocation;
			Vector vecNextNode = vecGoal;

			/*
			TraceResult trPathFind;
	        edict_t* pentIgnore = ENT( this->pev );
			UTIL_TraceLine(vecMyOrigin, pev->origin + vecDir * flCheckDist, dont_ignore_monsters, pentIgnore, &trPathFind);
			//CBaseEntity* pEntityHit;
			*/

			//


			//

			TraceResult trRampBeginAttempt;
	        edict_t* pentIgnore;

			Vector vecTraceStart;
			Vector vecTraceEnd;


			if(rampFixAttempt == 1){
				//travel down the ramp.
				//(go from the goal to MY origin to collide with the ramp first)
				vecTraceStart = Vector(vecNextNode.x, vecNextNode.y, vecNextNode.z + 4);
				vecTraceEnd = Vector(vecMyOrigin.x, vecMyOrigin.y, vecNextNode.z + 4);
				
				if(pTarget != NULL){
					pentIgnore = ENT( pTarget->pev );
				}else{
					pentIgnore = ENT( this->pev );
				}
				
			}else{
				//travel up the ramp
				vecTraceStart = Vector(vecMyOrigin.x, vecMyOrigin.y, vecMyOrigin.z + 4);
				//vecTraceEnd = Vector(vecGoal.x, vecGoal.y, vecMyOrigin.z + 4);
				vecTraceEnd = Vector(vecNextNode.x, vecNextNode.y, vecMyOrigin.z + 4);
				
				pentIgnore = ENT( this->pev );
			}

			
			/*
			debugVector1 = vecRampLowPoint;
			debugVector2 = rampTopPoint1;
			debugVector3 = rampTopPoint2;
			debugVector4 = vecGoalOrigin;
			debugVectorsSet = TRUE;
			*/

			/*
			debugVector1 = vecTraceStart;
			debugVector2 = vecTraceEnd;
			debugVectorsSet = TRUE;
			debugFailColor = FALSE;
			debugVectorMode = 2;
			*/

				

			//UTIL_TraceLine(vecTraceStart, vecTraceEnd, dont_ignore_monsters, pentIgnore, &trRampBeginAttempt);
				UTIL_TraceLine(vecTraceStart, vecTraceEnd, ignore_monsters, pentIgnore, &trRampBeginAttempt);
				


			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d BOTTOM TO UP: GOALPOS? (%.2f %.2f %.2f) SLD?:%d FRAC:%.2f N:(%.2f %.2f %.2f)", getClassnameShort(), this->monsterID,
				vecGoal.x, vecGoal.y, vecGoal.z
				, !trRampBeginAttempt.fAllSolid, trRampBeginAttempt.flFraction,
				trRampBeginAttempt.vecPlaneNormal.x, trRampBeginAttempt.vecPlaneNormal.y, trRampBeginAttempt.vecPlaneNormal.z ) );


			//MODDD TODO - is it possible for a ramp to have a vecPlaneNormal.z that is negative and still a typical ramp (as opposed to an incline down from the ceiling)?

			//also, if the fracition is 1.0, that means nothing was hit.  That is important this time.
			//if (!trPathFind.fAllSolid && trPathFind.flFraction < 1.0 && ((pEntityHit = CBaseEntity::Instance(trPathFind.pHit)) != NULL) ){
			if (!trRampBeginAttempt.fAllSolid && trRampBeginAttempt.flFraction < 1.0 && (trRampBeginAttempt.vecPlaneNormal.z >= 0.92) ){

				Vector distVect = (vecGoal - vecMyOrigin);
				Vector distVectFlat = Vector(distVect.x, distVect.y, 0);
				



				BOOL checkLocalMovePreRampTest;

				
				Vector toBaseRamp;
				Vector vecRampLowPoint;
				if(rampFixAttempt == 1){
					//travel down the ramp
					toBaseRamp = -vecDirFlatUnnormal * distVectFlat.Length() * trRampBeginAttempt.flFraction;
					vecRampLowPoint = vecNextNode + Vector(0, 0, 12) + toBaseRamp + vecDirFlatUnnormal * 6;
				}else{
					//travel up the ramp.
					toBaseRamp = vecDirFlatUnnormal * distVectFlat.Length() * trRampBeginAttempt.flFraction;
					vecRampLowPoint = vecMyOrigin + Vector(0, 0, 12) + toBaseRamp + -vecDirFlatUnnormal * 6;
				}

				
				/*

				Vector vecRampLowPoint = vecMyOrigin + Vector(0, 0, 12) + toBaseRamp + -vecDirFlatUnnormal * 6;

				if(toBaseRamp.Length() > 20){
					hasTowardsRampNode = TRUE;
				}else{
					hasTowardsRampNode = FALSE;
					//change. "I" am the ramp low point.
					vecRampLowPoint = pev->origin;
				}


				if(!hasTowardsRampNode){
					checkLocalMovePreRampTest = TRUE;
				}else{
					//checkLocalMovePreRampTest = CheckLocalMoveHull(pev->origin + Vector(0, 0, 12), vecRampLowPoint, pTarget, &flDist);
					//HACK
					checkLocalMovePreRampTest = TRUE;
				}
				*/


				checkLocalMovePreRampTest = TRUE;

				//if close enough, skip this check. otherwise, require it to be safe.
				if(checkLocalMovePreRampTest ){
					//this passed? continue with the ramp check.
					Vector vecRampPlaneNormal = trRampBeginAttempt.vecPlaneNormal;
					Vector vecGoalOrigin;
					if(rampFixAttempt == 1){
						//travel down.
						vecGoalOrigin = vecMyOrigin;
					}else{
						//travel up.
						//vecGoalOrigin = m_Route[m_iRouteIndex].vecLocation;  ??
						vecGoalOrigin = vecGoal;
					}

					float dotTest = -DotProduct(vecRampPlaneNormal, vecDir);
					//if(dotTest < 0.5){
						//skipping the check for now.
						Vector dirRawStartToGoal = ( vecGoalOrigin - vecRampLowPoint ).Normalize();
						
						//First, get a direction from the goal to the current point, flat-ways.
						Vector dirFlatGoalToStart = projectionOntoPlane( dirRawStartToGoal, Vector(0, 0, 1) ).Normalize();
						Vector dirUpRamp = projectionOntoPlane(dirRawStartToGoal, vecRampPlaneNormal).Normalize();

						//columns by rows. NOT including the constant column.
						Vector deltaOrigin = vecGoalOrigin + -vecRampLowPoint;
						float par_t;

						Vector n = Vector(0, 0, 1); //flat plane at top of ramp
						Vector la = dirUpRamp;

						float dot1 = DotProduct(n, deltaOrigin);
						float dot2 = DotProduct(n, la);
						if (dot2 != 0) {
							par_t = dot1 / dot2;
							
							Vector vecIntersection = Vector(
								dirUpRamp.x * par_t + vecRampLowPoint.x,
								dirUpRamp.y * par_t + vecRampLowPoint.y,
								dirUpRamp.z * par_t + vecRampLowPoint.z
							);
								
							//rampTopPoint1 = vecIntersection + Vector(0, 0, 6) + -dirUpRamp * 3;
							//rampTopPoint2 = vecIntersection + Vector(0, 0, 6) + -dirFlatGoalToStart * 3;
							rampTopPoint1 = vecIntersection + Vector(0, 0, 12); //+ -dirUpRamp * 20;
							rampTopPoint2 = vecIntersection + Vector(0, 0, 12);

							BOOL passCheck1 = (CheckLocalMoveHull (vecRampLowPoint, rampTopPoint1, pTarget, &flDist) == LOCALMOVE_VALID);
							BOOL passCheck2 = FALSE; //default.
							//if(passCheck1){
								passCheck2 = (CheckLocalMoveHull (rampTopPoint2, vecGoalOrigin, pTarget, &flDist) == LOCALMOVE_VALID);
							//}
								
							if(rampFixAttempt == 1){
								EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d BOTTOM TO UP: PASS? %d %d", getClassnameShort(), this->monsterID, passCheck1, passCheck2 ) );
							}else{
								EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d TOP TO DOWN: PASS? %d %d", getClassnameShort(), this->monsterID, passCheck1, passCheck2) );
							}

							if(passCheck1 && passCheck2){
								//we can go "up" the ramp.
								localMovePass = TRUE;
							}
						}
						else {
							EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d HORRIBLE FAILURE YOU CANNOT DIVIDE BY 0 MORTAL", getClassnameShort(), this->monsterID ) );
						}

						if(localMovePass){
							int currentNodeIndex = 0;
								

							//hasTowardsRampNode
							if( (vecGoalOrigin - vecMyOrigin ).Length2D() > 60 ){
								m_Route[ currentNodeIndex ].vecLocation = vecRampLowPoint;
								m_Route[ currentNodeIndex ].iType = (iMoveFlag | bits_MF_RAMPFIX | bits_MF_TO_DETOUR);
								currentNodeIndex++;
							}

							m_Route[ currentNodeIndex ].vecLocation = rampTopPoint1;
							m_Route[ currentNodeIndex ].iType = (iMoveFlag | bits_MF_RAMPFIX | bits_MF_TO_DETOUR);
							currentNodeIndex++;

							m_Route[ currentNodeIndex ].vecLocation = vecGoal;
							m_Route[ currentNodeIndex ].iType = iMoveFlag | bits_MF_RAMPFIX | bits_MF_IS_GOAL;
							currentNodeIndex++;

							m_vecMoveGoal = vecGoal;
							//RouteSimplify( pTarget );

							debugVector1 = vecRampLowPoint;
							debugVector2 = rampTopPoint1;
							debugVector3 = rampTopPoint2;
							debugVector4 = vecGoalOrigin;
							debugVectorsSet = TRUE;
							return TRUE;
						}//END OF if(localMovePass)
						else{
							debugVector1 = vecRampLowPoint;
							debugVector2 = rampTopPoint1;
							debugVector3 = rampTopPoint2;
							debugVector4 = vecGoalOrigin;
							debugVectorsSet = TRUE;
							debugFailColor = TRUE;
						}

					//}//END OF dot check
				}//END OF pre move test (from current place to the bottom of the ramp, if not immediately there)
			}//END OF trace-hit-something check

		}else{
			
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: NO RAMP TEST.", getClassnameShort(), monsterID) );

		}

	}//END OF first  if( !localMovePass)  check


	
	EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d BuildRoute: TOTAL FAIL!", getClassnameShort(), monsterID) );

	// b0rk
	return FALSE;
}








//=========================================================
// InsertWaypoint - Rebuilds the existing route so that the
// supplied vector and moveflags are the first waypoint in
// the route, and fills the rest of the route with as much
// of the pre-existing route as possible
//=========================================================
void CBaseMonster :: InsertWaypoint ( Vector vecLocation, int afMoveFlags )
{
	int		i, type;

	
	// we have to save some Index and Type information from the real
	// path_corner or node waypoint that the monster was trying to reach. This makes sure that data necessary 
	// to refresh the original path exists even in the new waypoints that don't correspond directy to a path_corner
	// or node. 
	type = afMoveFlags | (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK);

	for ( i = ROUTE_SIZE-1; i > 0; i-- )
		m_Route[i] = m_Route[i-1];

	m_Route[ m_iRouteIndex ].vecLocation = vecLocation;
	m_Route[ m_iRouteIndex ].iType = type;
}



//=========================================================
// FTriangulate - tries to overcome local obstacles by 
// triangulating a path around them.
//
// iApexDist is how far the obstruction that we are trying
// to triangulate around is from the monster.
//=========================================================


//MODDD DREVERT - major

//=========================================================
// FTriangulate - tries to overcome local obstacles by 
// triangulating a path around them.
//
// iApexDist is how far the obstruction that we are trying
// to triangulate around is from the monster.
//=========================================================
BOOL CBaseMonster :: FTriangulate ( const Vector &vecStart , const Vector &vecEnd, float flDist, CBaseEntity *pTargetEnt, Vector *pApex )
{
	Vector		vecDir;
	Vector		vecForward;
	Vector		vecLeft;// the spot we'll try to triangulate to on the left
	Vector		vecRight;// the spot we'll try to triangulate to on the right
	Vector		vecTop;// the spot we'll try to triangulate to on the top
	Vector		vecBottom;// the spot we'll try to triangulate to on the bottom
	Vector		vecFarSide;// the spot that we'll move to after hitting the triangulated point, before moving on to our normal goal.
	int		i;
	float	sizeX, sizeZ;

	// If the hull width is less than 24, use 24 because CheckLocalMove uses a min of
	// 24.
	sizeX = pev->size.x;
	if (sizeX < 24.0)
		sizeX = 24.0;
	else if (sizeX > 48.0)
		sizeX = 48.0;
	sizeZ = pev->size.z;
	//if (sizeZ < 24.0)
	//	sizeZ = 24.0;

	vecForward = ( vecEnd - vecStart ).Normalize();

	Vector vecDirUp(0,0,1);
	vecDir = CrossProduct ( vecForward, vecDirUp);

	// start checking right about where the object is, picking two equidistant starting points, one on
	// the left, one on the right. As we progress through the loop, we'll push these away from the obstacle, 
	// hoping to find a way around on either side. pev->size.x is added to the ApexDist in order to help select
	// an apex point that insures that the monster is sufficiently past the obstacle before trying to turn back
	// onto its original course.

	vecLeft = pev->origin + ( vecForward * ( flDist + sizeX ) ) - vecDir * ( sizeX * 3 );
	vecRight = pev->origin + ( vecForward * ( flDist + sizeX ) ) + vecDir * ( sizeX * 3 );
	if (isMovetypeFlying())
	{
		vecTop = pev->origin + (vecForward * flDist) + (vecDirUp * sizeZ * 3);
		vecBottom = pev->origin + (vecForward * flDist) - (vecDirUp *  sizeZ * 3);
	}

	vecFarSide = m_Route[ m_iRouteIndex ].vecLocation;
	
	vecDir = vecDir * sizeX * 2;
	if (pev->movetype == isMovetypeFlying() )
		vecDirUp = vecDirUp * sizeZ * 2;

	for ( i = 0 ; i < 8; i++ )
	{
// Debug, Draw the triangulation
#if 0
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_SHOWLINE);
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_COORD( vecRight.x );
			WRITE_COORD( vecRight.y );
			WRITE_COORD( vecRight.z );
		MESSAGE_END();

		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_SHOWLINE );
			WRITE_COORD( pev->origin.x );
			WRITE_COORD( pev->origin.y );
			WRITE_COORD( pev->origin.z );
			WRITE_COORD( vecLeft.x );
			WRITE_COORD( vecLeft.y );
			WRITE_COORD( vecLeft.z );
		MESSAGE_END();
#endif

#if 0
		if (isMovetypeFlying())
		{
			MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
				WRITE_BYTE( TE_SHOWLINE );
				WRITE_COORD( pev->origin.x );
				WRITE_COORD( pev->origin.y );
				WRITE_COORD( pev->origin.z );
				WRITE_COORD( vecTop.x );
				WRITE_COORD( vecTop.y );
				WRITE_COORD( vecTop.z );
			MESSAGE_END();

			MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
				WRITE_BYTE( TE_SHOWLINE );
				WRITE_COORD( pev->origin.x );
				WRITE_COORD( pev->origin.y );
				WRITE_COORD( pev->origin.z );
				WRITE_COORD( vecBottom.x );
				WRITE_COORD( vecBottom.y );
				WRITE_COORD( vecBottom.z );
			MESSAGE_END();
		}
#endif

		if ( CheckLocalMove( pev->origin, vecRight, pTargetEnt, NULL ) == LOCALMOVE_VALID )
		{
			if ( CheckLocalMove ( vecRight, vecFarSide, pTargetEnt, NULL ) == LOCALMOVE_VALID )
			{
				if ( pApex )
				{
					*pApex = vecRight;
				}

				return TRUE;
			}
		}
		if ( CheckLocalMove( pev->origin, vecLeft, pTargetEnt, NULL ) == LOCALMOVE_VALID )
		{
			if ( CheckLocalMove ( vecLeft, vecFarSide, pTargetEnt, NULL ) == LOCALMOVE_VALID )
			{
				if ( pApex )
				{
					*pApex = vecLeft;
				}

				return TRUE;
			}
		}

		if (isMovetypeFlying())
		{
			if ( CheckLocalMove( pev->origin, vecTop, pTargetEnt, NULL ) == LOCALMOVE_VALID)
			{
				if ( CheckLocalMove ( vecTop, vecFarSide, pTargetEnt, NULL ) == LOCALMOVE_VALID )
				{
					if ( pApex )
					{
						*pApex = vecTop;
						//ALERT(at_aiconsole, "triangulate over\n");
					}

					return TRUE;
				}
			}
#if 1
			if ( CheckLocalMove( pev->origin, vecBottom, pTargetEnt, NULL ) == LOCALMOVE_VALID )
			{
				if ( CheckLocalMove ( vecBottom, vecFarSide, pTargetEnt, NULL ) == LOCALMOVE_VALID )
				{
					if ( pApex )
					{
						*pApex = vecBottom;
						//ALERT(at_aiconsole, "triangulate under\n");
					}

					return TRUE;
				}
			}
#endif
		}

		vecRight = vecRight + vecDir;
		vecLeft = vecLeft - vecDir;
		if (isMovetypeFlying())
		{
			vecTop = vecTop + vecDirUp;
			vecBottom = vecBottom - vecDirUp;
		}
	}

	return FALSE;
}










//=========================================================
// Move - take a single step towards the next ROUTE location
//=========================================================
#define DIST_TO_CHECK	200



void CBaseMonster :: Move ( float flInterval ) 
{
	

	if(drawPathConstant){
		DrawRoute( pev, m_Route, m_iRouteIndex, 0, 0, 176 );
	}



	float	flWaypointDist;
	float	flCheckDist;
	float	flDist;// how far the lookahead check got before hitting an object.
	Vector		vecDir;
	Vector		vecApex;
	CBaseEntity	*pTargetEnt;

	// Don't move if no valid route
	if ( FRouteClear() )
	{
		// If we still have a movement goal, then this is probably a route truncated by SimplifyRoute()
		// so refresh it.
		if ( m_movementGoal == MOVEGOAL_NONE || !FRefreshRoute() )
		{
			ALERT( at_aiconsole, "Tried to move with no route!\n" );
			TaskFail();
			return;
		}
	}
	
	if ( m_flMoveWaitFinished > gpGlobals->time )
		return;

// Debug, test movement code
#if 0
//	if ( CVAR_GET_FLOAT("stopmove" ) != 0 )
	{
		if ( m_movementGoal == MOVEGOAL_ENEMY )
			RouteSimplify( m_hEnemy );
		else
			RouteSimplify( m_hTargetEnt );
		FRefreshRoute();
		return;
	}
#else
// Debug, draw the route
//	DrawRoute( pev, m_Route, m_iRouteIndex, 0, 200, 0 );
#endif

	// if the monster is moving directly towards an entity (enemy for instance), we'll set this pointer
	// to that entity for the CheckLocalMove and Triangulate functions.
	pTargetEnt = NULL;

	// local move to waypoint.
	vecDir = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Normalize();

	//MODDD - HOLD ON!  Why is this always "Length2D()"?  Even for flyers? 
	//        When would flyers call for Length2D instead of typical Length (includes Z axis)?
	//flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Length2D();

	if( !isMovetypeFlying()){
		//not a flyer? Default behavior.
		flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Length2D();
	}else{
		//Otherwise, use 3D distance instead.  If there's ever a time the Length2D is still preferred... eh. why?
		flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Length();
	}



	
	MakeIdealYaw ( m_Route[ m_iRouteIndex ].vecLocation );
	ChangeYaw ( pev->yaw_speed );

	if(m_movementGoal == MOVEGOAL_NONE){
		//MODDD - YEAH
		//no more! Cheap trick to interrupt the movement method if ChangeYaw decides to clear the route.
		return; 
	}



	// if the waypoint is closer than CheckDist, CheckDist is the dist to waypoint
	if ( flWaypointDist < DIST_TO_CHECK )
	{
		flCheckDist = flWaypointDist;
	}
	else
	{
		flCheckDist = DIST_TO_CHECK;
	}
	
	//MODDD - bit checks instead now.
	//if ( (m_Route[ m_iRouteIndex ].iType & (~bits_MF_NOT_TO_MASK)) == bits_MF_TO_ENEMY )
	if ( (m_Route[ m_iRouteIndex ].iType & (~bits_MF_NOT_TO_MASK)) & bits_MF_TO_ENEMY )
	{
		// only on a PURE move to enemy ( i.e., ONLY MF_TO_ENEMY set, not MF_TO_ENEMY and DETOUR )
		pTargetEnt = m_hEnemy;
	}
	//else if ( (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK) == bits_MF_TO_TARGETENT )
	else if ( (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK) & bits_MF_TO_TARGETENT )
	{
		pTargetEnt = m_hTargetEnt;
	}


	if(EASY_CVAR_GET(crazyMonsterPrintouts) == 1){
		if(pTargetEnt == NULL){
			easyPrintLine("I AM OBNOXIOUS: NULL : %d", m_Route[ m_iRouteIndex ].iType);
		}else{
			easyPrintLine("I AM OBNOXIOUS: %s %d", STRING(pTargetEnt->pev->classname), m_Route[ m_iRouteIndex ].iType );
		}
	}

	if( EASY_CVAR_GET(applyLKPPathFixToAll) == 1 || hasSeeEnemyFix() && !(m_Route[ m_iRouteIndex].iType & bits_MF_TO_ENEMY) && HasConditions(bits_COND_SEE_ENEMY) ){
		//this is a fix to make the enemy re-route in case of seeing the enemy while taking a path not necessarily to the enemy (such as, on its way to a last known location).
		this->MovementComplete();
		return;
	}



	// !!!BUGBUG - CheckDist should be derived from ground speed.
	// If this fails, it should be because of some dynamic entity blocking this guy.
	// We've already checked this path, so we should wait and time out if the entity doesn't move
	flDist = 0;



	//BOOL localMovePass = (CheckLocalMove ( pev->origin, pev->origin + vecDir * flWaypointDist, pTargetEnt, &flDist ) == LOCALMOVE_VALID);
	
	BOOL localMovePass;


	//If using a RAMPFIX or NODE type of node, use "CheckLocalMoveHull" instead. It's a bit less strict.

	int useHullCheckMask = bits_MF_RAMPFIX;
	if(EASY_CVAR_GET(pathfindLooseMapNodes) == 1){
		useHullCheckMask |= bits_MF_TO_NODE;
	}
	


	if( (m_Route[ m_iRouteIndex ].iType & ~bits_MF_NOT_TO_MASK) & (useHullCheckMask) ){
		//for now...
		localMovePass = (CheckLocalMoveHull ( pev->origin, pev->origin + vecDir * flCheckDist, pTargetEnt, &flDist ) == LOCALMOVE_VALID);
		//localMovePass = TRUE;
		if(!localMovePass){
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d Move: CheckLocalMoveHull Failed!!!", getClassnameShort(), monsterID) );
		}
	}else{
		localMovePass = (CheckLocalMove ( pev->origin, pev->origin + vecDir * flCheckDist, pTargetEnt, &flDist ) == LOCALMOVE_VALID);

		if(localMovePass){
			//if it passed, likely didn't bother writing anything to flDist. Just go ahead and assume it was full, that is what passing means.
			flDist = flCheckDist;
		}

		if(drawPathConstant){
			//Show the result of the recent localMove?
			::DebugLine_Setup(0, pev->origin, pev->origin + vecDir * flCheckDist, (flDist / flCheckDist) );
		}

		if(!localMovePass){
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:ID%d Move: CheckLocalMove Failed!", getClassnameShort(), monsterID) ) ;
		}

	}
	
	


	//careful now!
	//localMovePass = TRUE;




	//MODDD
	//if ( CheckLocalMove ( pev->origin, pev->origin + vecDir * flCheckDist, pTargetEnt, &flDist ) != LOCALMOVE_VALID )
	if( !localMovePass )
	{
		CBaseEntity *pBlocker;

		// Can't move, stop
		Stop();
		// Blocking entity is in global trace_ent

		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE A", getClassnameShort(), monsterID) );
		pBlocker = CBaseEntity::Instance( gpGlobals->trace_ent );

		//easyForcePrintLine("I AM THE BLOCKER AND I SUCK RAISINS %s", pBlocker->getClassname());

		if (pBlocker)
		{
			DispatchBlocked( edict(), pBlocker->edict() );
		}
		
		if ( pBlocker && m_moveWaitTime > 0 && pBlocker->IsMoving() && !pBlocker->IsPlayer() && (gpGlobals->time-m_flMoveWaitFinished) > 3.0 )
		{
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE B1", getClassnameShort(), monsterID) );
			// Can we still move toward our target?
			if ( flDist < m_flGroundSpeed )
			{
				// No, Wait for a second
				EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: B11 FAIL", getClassnameShort(), monsterID) );
				m_flMoveWaitFinished = gpGlobals->time + m_moveWaitTime;
				return;
			}
			// Ok, still enough room to take a step
		}
		else 
		{
			EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE B2", getClassnameShort(), monsterID) );
			// try to triangulate around whatever is in the way.
			if ( FTriangulate( pev->origin, m_Route[ m_iRouteIndex ].vecLocation, flDist, pTargetEnt, &vecApex ) )
			{
				EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE C1", getClassnameShort(), monsterID) );
				InsertWaypoint( vecApex, bits_MF_TO_DETOUR );
				RouteSimplify( pTargetEnt );
			}
			else
			{
				EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE C2", getClassnameShort(), monsterID) );
//				ALERT ( at_aiconsole, "Couldn't Triangulate\n" );
				Stop();
				// Only do this once until your route is cleared
				if ( m_moveWaitTime > 0 && !(m_afMemory & bits_MEMORY_MOVE_FAILED) )
				{
					EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE D1", getClassnameShort(), monsterID) );
					FRefreshRoute();
					if ( FRouteClear() )
					{
						EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE E1 FAIL", getClassnameShort(), monsterID) );
						TaskFail();
					}
					else
					{	
						
						EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE E2", getClassnameShort(), monsterID) );
						// Don't get stuck
						
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//MODDD OF GREAT EMPHASIS
// Note that the  < #   part can be whatever other number, bigger to allow this to admit failure more easily in case of repeating a move.
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if ( (gpGlobals->time - m_flMoveWaitFinished) < EASY_CVAR_GET(pathfindFidgetFailTime) )
							Remember( bits_MEMORY_MOVE_FAILED );

						m_flMoveWaitFinished = gpGlobals->time + 0.1;
					}
				}
				else
				{





					if(EASY_CVAR_GET(pathfindEdgeCheck) == 1){

						//Before admitting failure, do a check. Are we close enough to the goal to let this count as success?
					
						//m_iRouteIndex
						WayPoint_t* waypointGoalRef = NULL;

						/*
						if(m_Route[ 0 ].iType & bits_MF_IS_GOAL){
							waypointGoalRef = &m_Route[ 0 ];
						}else if(m_Route[ 1 ].iType & bits_MF_IS_GOAL){
							waypointGoalRef = &m_Route[ 1 ];
							//is our current destination waypoint not 0 or 1 but the goal? 0 and 1 have already been tried of course.
						}else if(m_iRouteIndex != 0 && m_iRouteIndex != 1 && (m_Route[ m_iRouteIndex ].iType & bits_MF_IS_GOAL)){
							waypointGoalRef = &m_Route[ m_iRouteIndex ];
						}
						*/

						waypointGoalRef = GetGoalNode();

						////////////////////////////////////////////////
						if( ( waypointGoalRef != NULL && waypointGoalRef->iType & bits_MF_IS_GOAL) )
						{

							float distanceee = (waypointGoalRef->vecLocation - pev->origin ).Length();
							
							//NOTICE - this allowed distance is very floaty. It is the expected distance to move in a frame times a number to go a bit further.
							//...no, our movement speed is unimportant. Use a constant distance instead, possibly factor in this monster's own size later.
							// ... < (m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval * 5)

							float maxDist = (pev->size.y/2.0f) + 50.0f;
							EASY_CVAR_PRINTIF_PRE(pathfindPrintout,easyForcePrintLine("PathfindEdgeCheck: DISTANCE TO GOAL: %.2f MAX ALLOWED: %.2f", distanceee, maxDist));
							if(distanceee <= maxDist){

								//Is there a straight line from me to the goal?
								TraceResult trTemp;
								UTIL_TraceLine ( pev->origin + Vector(0, 0, 5), waypointGoalRef->vecLocation + Vector(0, 0, 5), dont_ignore_monsters, dont_ignore_glass,  ENT(pev), &trTemp );

								DebugLine_ClearAll();
								if( trTemp.flFraction==1 || trTemp.pHit == NULL || (m_hEnemy != NULL && trTemp.pHit == m_hEnemy.Get()) ){
									//if nothing was hit or I happened to hit my enemy with this trace, pass.
									
									MovementComplete();
									return;
								}else{
									EASY_CVAR_PRINTIF_PRE(pathfindPrintout,easyForcePrintLine("PathfindEdgeCheck: TRACE FAILED?! classname:%s : fract:%.2f", STRING(trTemp.pHit->v.classname), trTemp.flFraction));
									DebugLine_Setup(0, pev->origin+Vector(0, 0, 5), waypointGoalRef->vecLocation + Vector(0, 0, 5), trTemp.flFraction);
								}

							}
						}
						////////////////////////////////////////////////


					}




					EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE D2", getClassnameShort(), monsterID) );
					TaskFail();
					ALERT( at_aiconsole, "%s Failed to move (%d)!\n", STRING(pev->classname), HasMemory( bits_MEMORY_MOVE_FAILED ) );
					//ALERT( at_aiconsole, "%f, %f, %f\n", pev->origin.z, (pev->origin + (vecDir * flCheckDist)).z, m_Route[m_iRouteIndex].vecLocation.z );



				}
				return;
			}
		}
	}


	BOOL skipMoveExecute = FALSE;
	
	EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE X", getClassnameShort(), monsterID) );
	// close enough to the target, now advance to the next target. This is done before actually reaching
	// the target so that we get a nice natural turn while moving.

	///!!!BUGBUG- magic number
	if ( ShouldAdvanceRoute( flWaypointDist, flInterval ) )
	{

		/*
		//MODDD - no need. Notice the script below, "flCheckDist < distExpectedToCover". This will end up moving the monster
		//        the right amount of distance to cover the point anyways.
		if(EASY_CVAR_GET(pathfindSnapToNode) == 1){
			//MODDD - if it returns true, snap to that position. Don't move this frame, this is good enough.
			pev->origin = m_Route[ m_iRouteIndex ].vecLocation;
			skipMoveExecute = TRUE;
		}
		*/

		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: ROUTE Z", getClassnameShort(), monsterID) );
		AdvanceRoute( flWaypointDist, flInterval );
	}else{

	}

	// Might be waiting for a door
	if ( m_flMoveWaitFinished > gpGlobals->time )
	{
		Stop();
		return;
	}

	
	const float distExpectedToCover = m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * 1;

	//flCheckDist: 6    (just to waypoint w/ a cap of 200)
	//flCheckDist: 70
	//flCheckDist * 0.1: 7

	// UNDONE: this is a hack to quit moving farther than it has looked ahead.
	//if (flCheckDist < m_flGroundSpeed * flInterval)
	if(flCheckDist < distExpectedToCover * flInterval)
	{
		//flInterval = flCheckDist / m_flGroundSpeed;
		if(distExpectedToCover > 0){
			//no dividing by 0!
			flInterval = flCheckDist / distExpectedToCover;
		}else{
			flInterval = 0;
		}
		// ALERT( at_console, "%.02f\n", flInterval );
	}
	
	//MODDD - if we've already been snapped to the waypoint (allowed, was close enough to, and did), no need to move this frame. The logic won't move us towards the next waypoint yet,
	//        no need to go further beyond.
	//MODDD - actually see above, the hack on flInterval is enough to make the monster move only far enough to reach the node exactly this frame without passing it.
	//if(!skipMoveExecute){





	BOOL facingNextNode = TRUE;

	float facingTolerance = MoveYawDegreeTolerance();

	if(facingTolerance > 0){
		//One more thing. Are we actually facing where we want to go enough?
		//Vector vecDirToCurrentNode = (pev->origin - m_Route[ m_iRouteIndex ].vecLocation).Normalize();
		//...actually our ideal yaw already has to be towards this node. Just use that.
		
		//pev->ideal_yaw

		if(FacingIdeal(facingTolerance)){
			//good.
		}else{
			//not enough.
			facingNextNode = FALSE;
		}

	}else{
		//skip it, just pass (default).
	}


	BOOL movementCanAutoTurn = getMovementCanAutoTurn();

	//if movementCanAutoTurn is off, force a movement.
	if(!movementCanAutoTurn || facingNextNode){
		MoveExecute( pTargetEnt, vecDir, flInterval );
	}else{
		//try to face it?
		if(m_IdealActivity == ACT_RUN || m_IdealActivity == ACT_WALK || m_IdealActivity == ACT_HOVER || m_IdealActivity == ACT_FLY){
			Stop();
		}
		
		SetTurnActivity();
		
		if(m_IdealActivity != ACT_IDLE && m_IdealActivity != ACT_TURN_LEFT && m_IdealActivity != ACT_TURN_RIGHT){
			//force the ideal activity to IDLE for now.  No need to be frozen in any other animation and turn.
			//...is this a good idea or does it make things fidget for a frame?
			m_IdealActivity = ACT_IDLE;
		}
		

	}//END OF else OF facingNextNode



	//}

	if ( MovementIsComplete() )
	{
		EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyPrintLine("%s:ID%d Pathfinding: MOVEMENT IS COMPLETE??", getClassnameShort(), monsterID) );
		Stop();
		RouteClear();
	}
}




//MODDD - now accepts the time eslapsed during this frame (flInterval) to see how far we could potentially move this frame. This is a better measure as to whether we are close enough
//        to the node than whether we are just less than 8 units of distance away. If this monster were too fast for whatever reason, this monster would go back / forth forever
//        without ever satisfying the " < 8" distance requirement. Base it off of the potential distance traveled in this frame and it can't fail.
BOOL CBaseMonster:: ShouldAdvanceRoute( float flWaypointDist, float flInterval )
{
	const float rawMoveSpeedPerSec = (m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval );
	const float moveDistTest = rawMoveSpeedPerSec * EASY_CVAR_GET(pathfindNodeToleranceMulti);  //defaults to 1 for no effect.
	const float moveDistTol = max(moveDistTest, 8);  //must be at least 8.

	EASY_CVAR_PRINTIF_PRE(pathfindPrintout, easyForcePrintLine("%s:%d SHOULD I ADVANCE KIND SIR? d:%.2f rm:%.2f tol:%.2f pass:%d", FClassname(this), monsterID, (flWaypointDist), rawMoveSpeedPerSec, moveDistTol, (flWaypointDist <= moveDistTol) ) );
	//if ( flWaypointDist <= MONSTER_CUT_CORNER_DIST )
	

	if(flWaypointDist <= moveDistTol )
	{
		// ALERT( at_console, "cut %f\n", flWaypointDist );
		return TRUE;
	}
	return FALSE;
}


void CBaseMonster::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
{
#if defined(USE_MOVEMENT_BOUND_FIX) || defined(USE_MOVEMENT_BOUND_FIX_ALT)
	Vector oldMins;
	Vector oldMaxs;
	float flYaw;
		
#endif
//	float flYaw = UTIL_VecToYaw ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin );// build a yaw that points to the goal.
//	WALK_MOVE( ENT(pev), flYaw, m_flGroundSpeed * flInterval, WALKMOVE_NORMAL );
	if ( m_IdealActivity != m_movementActivity )
		m_IdealActivity = m_movementActivity;

	
#ifdef USE_MOVEMENT_BOUND_FIX
	if(needsMovementBoundFix()){
		oldMins = pev->mins;
		oldMaxs = pev->maxs;
		UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );
	}
#endif




#ifdef USE_MOVEMENT_BOUND_FIX_ALT
	flYaw = UTIL_VecToYaw ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin );
#endif



	float flTotal = m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval;
	float flStep;
	while (flTotal > 0.001)
	{
		// don't walk more than 16 units or stairs stop working
		flStep = min( 16.0, flTotal );



#ifndef USE_MOVEMENT_BOUND_FIX_ALT
		//Normal way!
		UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, flStep, MOVE_NORMAL );

#else

		if( !(EASY_CVAR_GET(pathfindLargeBoundFix) == 1 && needsMovementBoundFix() ) ){
			//Normal way!
			UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, flStep, MOVE_NORMAL );
		}else{
		//ALTERNATE.
		/////////////////////////////////////////////////////////////////////////////////////
		//MODDD - MAJOR. is that okay?
		//pTarget->edict() == gpGlobals->trace_ent
		if ( !WALK_MOVE( ENT(pev), flYaw, flStep, WALKMOVE_NORMAL ) ){

			/*
			if ( pflDist != NULL )
			{
				*pflDist = flStep;
			}
			if ( pTarget && pTarget->edict() == gpGlobals->trace_ent )
			{
				// if this step hits target ent, the move is legal.
				iReturn = LOCALMOVE_VALID;
				break;
			}
			else
			{
				// If we're going toward an entity, and we're almost getting there, it's OK.
//				if ( pTarget && fabs( flDist - iStep ) < LOCAL_STEP_SIZE )
//					fReturn = TRUE;
//				else
				iReturn = LOCALMOVE_INVALID;
				break;
			}
			*/

			//if(gpGlobals->trace_ent != NULL){
				CBaseEntity* whut = CBaseEntity::Instance(gpGlobals->trace_ent);

				if(whut != NULL){
					easyForcePrintLine("%s:%d MBFme: %s:%d isW:%d", getClassname(), monsterID, whut->getClassname(), 
						(whut->GetMonsterPointer() != NULL)?whut->GetMonsterPointer()->monsterID:-1
						, whut->IsWorld());
				}

				//These "AdvanceRoute" calls. Isn't that a tad drastic of an assumption?

				if(whut == NULL || whut->IsWorld()){
					//it is ok... maybe?  Check to see if the reason this was hit was because it was a ramp?
					//
					//resize me.
					oldMins = pev->mins;
					oldMaxs = pev->maxs;
					UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );

					if(!WALK_MOVE( ENT(pev), flYaw, flStep, WALKMOVE_NORMAL )){
						//still no? that's the last shot you get.
						AdvanceRoute(flStep, flInterval);
						break;
					}

					UTIL_SetSize( pev, oldMins, oldMaxs );

				}else{
					//not the world? entity? etc.? assume it is not okay.
					AdvanceRoute(flStep, flInterval);
					break;
				}
			//}


		}//END OF if(!WALK_MOVE( ... ) )

		}//END OF else OF pathfindLargeBoundFix check



		/////////////////////////////////////////////////////////////////////////////////////

#endif

		




		flTotal -= flStep;
	}
	// ALERT( at_console, "dist %f\n", m_flGroundSpeed * pev->framerate * flInterval );

	
#ifdef USE_MOVEMENT_BOUND_FIX
	if(needsMovementBoundFix()){
		//and revert.
		UTIL_SetSize( pev, oldMins, oldMaxs );
	}
#endif



}


//=========================================================
// MonsterInit - after a monster is spawned, it needs to 
// be dropped into the world, checked for mobility problems,
// and put on the proper path, if any. This function does
// all of those things after the monster spawns. Any
// initialization that should take place for all monsters
// goes here.
//=========================================================
void CBaseMonster :: MonsterInit ( void )
{

	//MODDD - extra check.
	const char* classname_test = STRING(pev->classname);
	if (classname_test == NULL || strlen(classname_test)==0) {
		easyForcePrintLine("WARNING! Monster without a classname spawned?");
	}

	
	//MODDD - "&& EASY_CVAR_GET(forceAllowMonsterSpawning) != 1" CVar removed.
	// Redundant with "mp_allowmonsters".
	// Also, added an exception to this rule for certain monsters.
	// The new 'pickup walker' monsters should not be forbidden by this setting.


	//if(CVAR_GET_FLOAT("pooptest") == 0){
	if (!g_pGameRules->FAllowMonsters() && !this->bypassAllowMonstersSpawnCheck() ){

		easyForcePrintLine("WARNING! Request to spawn \"%s\" denied, \'mp_allowmonsters\' is off.", this->getClassname());

		//MODDD - "FL_KILLME" flag-add replaced with UTIL_Remove call, does a bit more.
		// Calls "onDelete" that way too.
		//pev->flags |= FL_KILLME;		// Post this because some monster code modifies class data after calling this function

		UTIL_Remove(this);

		//MODDD - commented out as-is.
//		REMOVE_ENTITY(ENT(pev));
		return;
	}

	const char* classnameCheat = STRING(pev->classname);

	if(monsterID == -1){
		//MODDD - unique ID just for me!
		monsterID = monsterIDLatest;
		monsterIDLatest++;
	}

	// Set fields common to all monsters
	pev->effects		= 0;
	pev->takedamage		= DAMAGE_AIM;
	pev->ideal_yaw		= pev->angles.y;

	//MODDD NOTE - this line assumes the monster's health was set during Spawn before MonsterInit was called. Just make sure of that.
	//    Also monsters that skip MonsterInit may want to do this too.
	pev->max_health		= pev->health;

	pev->deadflag		= DEAD_NO;
	m_IdealMonsterState	= MONSTERSTATE_IDLE;// Assume monster will be idle, until proven otherwise

	m_IdealActivity = ACT_IDLE;

	SetBits (pev->flags, FL_MONSTER);
	if ( pev->spawnflags & SF_MONSTER_HITMONSTERCLIP )
		pev->flags |= FL_MONSTERCLIP;
	
	ClearSchedule();
	RouteClear();
	InitBoneControllers( ); // FIX: should be done in Spawn

	m_iHintNode			= NO_NODE;

	m_afMemory			= MEMORY_CLEAR;

	m_hEnemy			= NULL;

	//MODDD - these are now received from overridable methods instead so that child classes can change them.
	//        What else would the point of them being instance variables be?
	//        that and they really should just be per class as methods like this instead of per individual monster (instance).
	//        Really, when is say, one Kingpin going to have different DistTooFar or DistLook than any other Kingpin?
	m_flDistTooFar		= getDistTooFar();
	m_flDistLook		= getDistLook();

	//MODDD - flag for mirror recognition.
	pev->renderfx |= ISNPC;

	if(EASY_CVAR_GET(monsterSpawnPrintout) == 1){
		easyPrintLine("I SPAWNED::: %s flags:%d", STRING(pev->classname), pev->spawnflags);
	}

	// set eye position
	SetEyePosition();



	SetThink( &CBaseMonster::MonsterInitThink );

	//!!!
	//SetThink ( &CBaseMonster::CallMonsterThink );

	pev->nextthink = gpGlobals->time + 0.1;
	SetUse ( &CBaseMonster::MonsterUse );

}


//=========================================================
// MonsterInitThink - Calls StartMonster. Startmonster is 
// virtual, but this function cannot be 
//=========================================================
void CBaseMonster :: MonsterInitThink ( void )
{
	StartMonster();
}



//=========================================================
// StartMonster - final bit of initization before a monster 
// is turned over to the AI. 
//=========================================================
void CBaseMonster :: StartMonster ( void )
{
	// update capabilities
	if ( LookupActivityFiltered ( ACT_RANGE_ATTACK1 ) != ACTIVITY_NOT_AVAILABLE )
	{
		m_afCapability |= bits_CAP_RANGE_ATTACK1;
	}
	if ( LookupActivityFiltered ( ACT_RANGE_ATTACK2 ) != ACTIVITY_NOT_AVAILABLE )
	{
		m_afCapability |= bits_CAP_RANGE_ATTACK2;
	}
	if ( LookupActivityFiltered ( ACT_MELEE_ATTACK1 ) != ACTIVITY_NOT_AVAILABLE )
	{
		m_afCapability |= bits_CAP_MELEE_ATTACK1;
	}
	if ( LookupActivityFiltered ( ACT_MELEE_ATTACK2 ) != ACTIVITY_NOT_AVAILABLE )
	{
		m_afCapability |= bits_CAP_MELEE_ATTACK2;
	}

	// Raise monster off the floor one unit, then drop to floor
	


	//MODDD NOTE - Oddly enough? missing "SF_MONSTER_FALL_TO_GROUND" causes the monster to fall to the ground. Interdasting.
	if ( !isMovetypeFlying() && !FBitSet( pev->spawnflags, SF_MONSTER_FALL_TO_GROUND ) )
	{

		if(EASY_CVAR_GET(crazyMonsterPrintouts))easyForcePrintLine("YOU amazing piece of work");

		pev->origin.z += 1;
		DROP_TO_FLOOR ( ENT(pev) );
		// Try to move the monster to make sure it's not stuck in a brush.
		if ( !skipSpawnStuckCheck() && !WALK_MOVE ( ENT(pev), 0, 0, WALKMOVE_NORMAL ) )
		{
			ALERT(at_error, "Monster %s stuck in wall--level design error", STRING(pev->classname));
			pev->effects = EF_BRIGHTFIELD;
		}
	}
	else 
	{
		pev->flags &= ~FL_ONGROUND;
	}
	
	if ( !FStringNull(pev->target) )// this monster has a target
	{
		// Find the monster's initial target entity, stash it
		m_pGoalEnt = CBaseEntity::Instance( FIND_ENTITY_BY_TARGETNAME ( NULL, STRING( pev->target ) ) );

		if ( !m_pGoalEnt )
		{
			ALERT(at_error, "ReadyMonster()--%s couldn't find target %s", STRING(pev->classname), STRING(pev->target));
		}
		else
		{
			// Monster will start turning towards his destination
			MakeIdealYaw ( m_pGoalEnt->pev->origin );

			// JAY: How important is this error message?  Big Momma doesn't obey this rule, so I took it out.
#if 0
			// At this point, we expect only a path_corner as initial goal
			if (!FClassnameIs( m_pGoalEnt->pev, "path_corner"))
			{
				ALERT(at_warning, "ReadyMonster--monster's initial goal '%s' is not a path_corner", STRING(pev->target));
			}
#endif

			// set the monster up to walk a path corner path. 
			// !!!BUGBUG - this is a minor bit of a hack.
			// JAYJAY
			m_movementGoal = MOVEGOAL_PATHCORNER;
			

			//MODDD - is this appropriate for swimmers too?
			if(isMovetypeFlying())
				m_movementActivity = ACT_FLY;
			else
				m_movementActivity = ACT_WALK;

			if ( !FRefreshRoute() )
			{
				ALERT ( at_aiconsole, "Can't Create Route!\n" );
			}
			SetState( MONSTERSTATE_IDLE );
			ChangeSchedule( GetScheduleOfType( SCHED_IDLE_WALK ) );
		}
	}
	

	//SetState ( m_IdealMonsterState );
	//SetActivity ( m_IdealActivity );

	// Delay drop to floor to make sure each door in the level has had its chance to spawn
	// Spread think times so that they don't all happen at the same time (Carmack)
	SetThink ( &CBaseMonster::CallMonsterThink );
	pev->nextthink += RANDOM_FLOAT(0.1, 0.4); // spread think times.
	
	if ( !FStringNull(pev->targetname) )// wait until triggered
	{
		SetState( MONSTERSTATE_IDLE );
		// UNDONE: Some scripted sequence monsters don't have an idle?
		SetActivity( ACT_IDLE );
		ChangeSchedule( GetScheduleOfType( SCHED_WAIT_TRIGGER ) );
	}
}



void CBaseMonster :: MovementComplete( void ) 
{ 
	if(EASY_CVAR_GET(movementIsCompletePrintout)==1)easyForcePrintLine("MovementComplete: %s:%d WHO CALLED THIS", getClassname(), monsterID);

	switch( m_iTaskStatus )
	{
	case TASKSTATUS_NEW:
	case TASKSTATUS_RUNNING:
		m_iTaskStatus = TASKSTATUS_RUNNING_TASK;
		break;

	case TASKSTATUS_RUNNING_MOVEMENT:
		TaskComplete();
		break;
	
	case TASKSTATUS_RUNNING_TASK:
		ALERT( at_error, "Movement completed twice!\n" );

		//MODDD - new. This now also counts as TaskComplete. The Stuka needs it this way to be stumpable.  
		//...yes that seems funny, make more specific if other things are messed up as a result.
		//TaskComplete();
		break;

	case TASKSTATUS_COMPLETE:		
		break;
	}
	m_movementGoal = MOVEGOAL_NONE;
}



void CBaseMonster::TaskFail(void)
{
	//easyForcePrintLine("I PHAIL %.2f sched:%s task:%d", gpGlobals->time, getScheduleName(), getTaskNumber());
	SetConditions(bits_COND_TASK_FAILED);

	
	if(monsterID == 1){
		int x = 4;
	}

	//if(FClassnameIs(this->pev, "monster_scientist")){
	if(monsterID == 4){
		//break point!
		const char* imLazy = (m_pSchedule!=NULL)?m_pSchedule->pName:"NULL!";
		int x = 7;
	}

}

int CBaseMonster::TaskIsRunning( void )
{
	if ( m_iTaskStatus != TASKSTATUS_COMPLETE && 
		 m_iTaskStatus != TASKSTATUS_RUNNING_MOVEMENT )
		 return 1;

	return 0;
}








//MODDD - major note here. Monsters that are feared, R_FR, are belo R_NO. It is easy to mistake R_FR for "Friend", since it's negative (-1) towards R_AL for "Ally" (-2).
//        But as-is "BestVisibleEnemy" does not allowed a feared target to be marked designated as enemy, so any monster schedule depending on having a "Feared" enemy
//        will never see it.

//BestVisibleEnemy - HEAVILY depends on relationships. Can pick a monster hated more, even if it is further away.

//=========================================================
// IRelationship - returns an integer that describes the 
// relationship between two types of monster.
//=========================================================
int CBaseMonster::IRelationship ( CBaseEntity *pTarget )
{

	//NOTICE - Bioweapon's relationship with AMONST changed from R_DL to R_NO so that snarks don't attack alien monsters that otherwise are uninsterested in them.
	//Besides, is there really all that big of a difference between ALIEN_MILITARY and ALIEN_MONSTER or even ALIEN_PREDATOR?
	
	//MODDD - there is a new feature: "isForceHated".
	//        If true, we must hate this monster.
	//        May consider a "getRelationship()" later for forcing to hate.




	//CHUMTOAD NEVER CALLS ME.  R_DEFAULT DOESNT WORK RETURNED FROM A CHILD CLASS, ITS SUPPOSED TO FIGURE THAT OUT ON ITS OWN
	//  OR REDIRECT TO EITHER forceRelationshipWith OR  call the table here.
	//Heck just call the parent CBaseMonster::Irelationship instead of returning R_DEFAULT, nothing else knows what to do with that.

	int forcedRelationshipTest = pTarget->forcedRelationshipWith(this);
	
	if(forcedRelationshipTest != R_DEFAULT){
		//The other monster (pTarget) is forcing this monster to have an attitude towards it other than what the table may suggest.
		return forcedRelationshipTest;
	}else{}  //return below as usual.


	/*
	int thisThing = Classify();
	int thatThing = pTarget->Classify();
	*/

	int generalRelationship = iEnemy[ Classify() ][ pTarget->Classify() ];
	

	/*
	if(FClassnameIs(this->pev, "monster_scientist")){
		easyForcePrintLine("myid %d: MY RELATIONSHIP WITH THIS DUDE %s IS %d", monsterID, pTarget->getClassname(), generalRelationship);
	}
	*/

	return generalRelationship;
}

//=========================================================
// FindCover - tries to find a nearby node that will hide
// the caller from its enemy. 
//
// If supplied, search will return a node at least as far
// away as MinDist, but no farther than MaxDist. 
// if MaxDist isn't supplied, it defaults to a reasonable 
// value
//=========================================================
// UNDONE: Should this find the nearest node?

//float CGraph::PathLength( int iStart, int iDest, int iHull, int afCapMask )

BOOL CBaseMonster :: FindCover ( Vector vecThreat, Vector vecViewOffset, float flMinDist, float flMaxDist )
{
	int i;
	int iMyHullIndex;
	int iMyNode;
	int iThreatNode;
	float flDist;
	Vector	vecLookersOffset;
	TraceResult tr;

	if ( !flMaxDist )
	{
		// user didn't supply a MaxDist, so work up a crazy one.
		flMaxDist = 784;
	}

	if ( flMinDist > 0.5 * flMaxDist)
	{
#if _DEBUG
		ALERT ( at_console, "FindCover MinDist (%.0f) too close to MaxDist (%.0f)\n", flMinDist, flMaxDist );
#endif
		flMinDist = 0.5 * flMaxDist;
	}

	if ( !WorldGraph.m_fGraphPresent || !WorldGraph.m_fGraphPointersSet )
	{
		ALERT ( at_aiconsole, "Graph not ready for findcover!\n" );
		return FALSE;
	}

	iMyNode = WorldGraph.FindNearestNode( pev->origin, this );
	iThreatNode = WorldGraph.FindNearestNode ( vecThreat, this );
	iMyHullIndex = WorldGraph.HullIndex( this );

	if ( iMyNode == NO_NODE )
	{
		ALERT ( at_aiconsole, "FindCover() - %s has no nearest node!\n", STRING(pev->classname));
		return FALSE;
	}
	if ( iThreatNode == NO_NODE )
	{
		// ALERT ( at_aiconsole, "FindCover() - Threat has no nearest node!\n" );
		iThreatNode = iMyNode;
		// return FALSE;
	}

	vecLookersOffset = vecThreat + vecViewOffset;// calculate location of enemy's eyes

	// we'll do a rough sample to find nodes that are relatively nearby
	for ( i = 0 ; i < WorldGraph.m_cNodes ; i++ )
	{
		int nodeNumber = (i + WorldGraph.m_iLastCoverSearch) % WorldGraph.m_cNodes;

		CNode &node = WorldGraph.Node( nodeNumber );
		WorldGraph.m_iLastCoverSearch = nodeNumber + 1; // next monster that searches for cover node will start where we left off here.

		// could use an optimization here!!
		flDist = ( pev->origin - node.m_vecOrigin ).Length();

		// DON'T do the trace check on a node that is farther away than a node that we've already found to 
		// provide cover! Also make sure the node is within the mins/maxs of the search.
		if ( flDist >= flMinDist && flDist < flMaxDist )
		{
			UTIL_TraceLine ( node.m_vecOrigin + vecViewOffset, vecLookersOffset, ignore_monsters, ignore_glass,  ENT(pev), &tr );
			//return TRUE; //WARNING - SUPER DUPER HACKY SACKS

			// if this node will block the threat's line of sight to me...
			if ( tr.flFraction != 1.0 )
			{
				// ..and is also closer to me than the threat, or the same distance from myself and the threat the node is good.
				if ( ( iMyNode == iThreatNode ) || WorldGraph.PathLength( iMyNode, nodeNumber, iMyHullIndex, m_afCapability ) <= WorldGraph.PathLength( iThreatNode, nodeNumber, iMyHullIndex, m_afCapability ) )
				{

					
					if ( FValidateCover ( node.m_vecOrigin ) && MoveToLocation( ACT_RUN, 0, node.m_vecOrigin ) )
					{
						
						//The debug line thingy.
						/*
						MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
							WRITE_BYTE( TE_SHOWLINE);
							
							WRITE_COORD( node.m_vecOrigin.x );
							WRITE_COORD( node.m_vecOrigin.y );
							WRITE_COORD( node.m_vecOrigin.z );

							WRITE_COORD( vecLookersOffset.x );
							WRITE_COORD( vecLookersOffset.y );
							WRITE_COORD( vecLookersOffset.z );
						MESSAGE_END();
						*/

						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
}


//=========================================================
// BuildNearestRoute - tries to build a route as close to the target
// as possible, even if there isn't a path to the final point.
//
// If supplied, search will return a node at least as far
// away as MinDist from vecThreat, but no farther than MaxDist. 
// if MaxDist isn't supplied, it defaults to a reasonable 
// value
//=========================================================
//MODDD - now supports optional moveflags and target entity just like BuildRoute does to send to it if provided.
BOOL CBaseMonster :: BuildNearestRoute ( Vector vecThreat, Vector vecViewOffset, float flMinDist, float flMaxDist ){
	//default moveflag is bits_MF_TO_LOCATION.  That was always sent to BuildRoute in BuildNearestRoute as of retail.
	return BuildNearestRoute(vecThreat, vecViewOffset, flMinDist, flMaxDist, bits_MF_TO_LOCATION, NULL);
}

BOOL CBaseMonster :: BuildNearestRoute ( Vector vecThreat, Vector vecViewOffset, float flMinDist, float flMaxDist, int iMoveFlag, CBaseEntity* pTarget )
{
	int i;
	int iMyHullIndex;
	int iMyNode;
	float flDist;
	Vector	vecLookersOffset;
	TraceResult tr;

	//HACKY HACKY
	//flMaxDist = 9999;

	if ( !flMaxDist )
	{
		// user didn't supply a MaxDist, so work up a crazy one.
		flMaxDist = 784;
	}

	if ( flMinDist > 0.5 * flMaxDist)
	{
#if _DEBUG
		ALERT ( at_console, "FindCover MinDist (%.0f) too close to MaxDist (%.0f)\n", flMinDist, flMaxDist );
#endif
		flMinDist = 0.5 * flMaxDist;
	}

	if ( !WorldGraph.m_fGraphPresent || !WorldGraph.m_fGraphPointersSet )
	{
		ALERT ( at_aiconsole, "Graph not ready for BuildNearestRoute!\n" );
		return FALSE;
	}

	iMyNode = WorldGraph.FindNearestNode( pev->origin, this );
	iMyHullIndex = WorldGraph.HullIndex( this );

	//MODDD - CHEAT CHEAT CHEAT!
	//iMyHullIndex = NODE_POINT_HULL;

	if ( iMyNode == NO_NODE )
	{
		if(EASY_CVAR_GET(pathfindPrintout)==1)easyForcePrintLine("Path %s:%d Here ye here ye, I could not find a nearby node! I suck!", getClassname(), monsterID);
			
		
		ALERT ( at_aiconsole, "BuildNearestRoute() - %s has no nearest node!\n", STRING(pev->classname));
		return FALSE;
	}

	vecLookersOffset = vecThreat + vecViewOffset;// calculate location of enemy's eyes

	// we'll do a rough sample to find nodes that are relatively nearby
	for ( i = 0 ; i < WorldGraph.m_cNodes ; i++ )
	{
		int nodeNumber = (i + WorldGraph.m_iLastCoverSearch) % WorldGraph.m_cNodes;

		CNode &node = WorldGraph.Node( nodeNumber );
		WorldGraph.m_iLastCoverSearch = nodeNumber + 1; // next monster that searches for cover node will start where we left off here.

		// can I get there?
		if (WorldGraph.NextNodeInRoute( iMyNode, nodeNumber, iMyHullIndex, 0 ) != iMyNode)
		{
			flDist = ( vecThreat - node.m_vecOrigin ).Length();

			// is it close?
			if ( flDist > flMinDist && flDist < flMaxDist)
			{
				// can I see where I want to be from there?
				UTIL_TraceLine( node.m_vecOrigin + pev->view_ofs, vecLookersOffset, ignore_monsters, edict(), &tr );

				if (tr.flFraction == 1.0)
				{
					// try to actually get there
					//MODDD - involve the now parameterized "iMoveFlag" and "pTarget".
					//if ( BuildRoute ( node.m_vecOrigin, bits_MF_TO_LOCATION, NULL ) )
					if ( BuildRoute ( node.m_vecOrigin, iMoveFlag, pTarget ) )
					{
						flMaxDist = flDist;
						m_vecMoveGoal = node.m_vecOrigin;
						return TRUE; // UNDONE: keep looking for something closer!
					}
				}
			}
		}
	}

	return FALSE;
}//END OF BuildNearestRoute



//=========================================================
// BestVisibleEnemy - this functions searches the link
// list whose head is the caller's m_pLink field, and returns
// a pointer to the enemy entity in that list that is nearest the 
// caller.
//
// !!!UNDONE - currently, this only returns the closest enemy.
// we'll want to consider distance, relationship, attack types, back turned, etc.
//=========================================================
CBaseEntity *CBaseMonster :: BestVisibleEnemy ( void )
{
	CBaseEntity	*pReturn;
	CBaseEntity	*pNextEnt;

	//MODDD - ...why did these used to be integers?  any length is a decimal.
	float		flNearest;
	float		flDist;

	int		iBestRelationship;

	//MODDD - new. Also keep track of all feared enemeis separately.
	CBaseEntity* pFearReturn;
	float flFearNearest;

	float flNormalNearest;  //Of all enemies from Dislike to Nemesis, what is the closest distance found? Not necessarily the distance of the most despised / picked enemy.

	flFearNearest = 8192;
	pFearReturn = NULL;



	flNearest = 8192;// so first visible entity will become the closest.
	pNextEnt = m_pLink;
	pReturn = NULL;

	flNormalNearest = 8192;


	//MODDD - MAJOR! Monsters with "R_FR" for "fear" can be marked enemies too, but AI should want to run away from feared creatures instead of confront them unless cornered.
	iBestRelationship = R_NO;
	//iBestRelationship = R_FR;

	//Note that "R_NO" relationship'd enemies can't be checked. They are not added to the m_pLink list that's populated by the "Look" method called before this BestVisibleEnemy one.

	//ACTUALLY slightly different take. If a feared monster is the closest, it gets priority no matter what. Otherwise the usual system goes: "no" up to "nemesis", and then
	//the closest of those at the same highest hatred.

	//ACTUALLY another change: keep track of all feared enemies and normal ones (Dislike to Nemesis) separately.
	//If the closest of all feared enemies is closer than closest of all normal enemies (not necessarily just the most despised enemy, who may be further away than that),
	//then we make our enemy the feared enemy instead.


	//R_DL 200
	//R_NE 600
	//R_FR 400


	while ( pNextEnt != NULL )
	{

		/*
		if(FClassnameIs(pev, "monster_scientist")){
			easyForcePrintLine("HERE WE GO %s", pNextEnt->getClassname() );
		}
		*/
		
		//IsAlive_FromAI takes "this" mosnter as a parameter. It already knows what itself is.
		if ( pNextEnt->IsAlive_FromAI(this) )
		{

			const int relationshipWithNextEnt = IRelationship( pNextEnt);

			if(relationshipWithNextEnt == R_FR){
				
				flDist = ( pNextEnt->pev->origin - pev->origin ).Length();
				//If I fear this one and he's the closest, he's the best enemy yet.
				if ( flDist <= flFearNearest )
				{
					flFearNearest = flDist;
					pFearReturn = pNextEnt;
				}


			}else if ( relationshipWithNextEnt > iBestRelationship )
			{
				// this entity is disliked MORE than the entity that we 
				// currently think is the best visible enemy. No need to do 
				// a distance check, just get mad at this one for now.


				iBestRelationship = relationshipWithNextEnt;
				flNearest = ( pNextEnt->pev->origin - pev->origin ).Length();
				pReturn = pNextEnt;

				if(flNearest < flNormalNearest){
					flNormalNearest = flNearest; //just keeping track of the closest "Dislike - Nemesis" range enemy.
				}
				
			}
			else if ( relationshipWithNextEnt == iBestRelationship )
			{
				// this entity is disliked just as much as the entity that
				// we currently think is the best visible enemy, so we only
				// get mad at it if it is closer.
				flDist = ( pNextEnt->pev->origin - pev->origin ).Length();
				
				if ( flDist <= flNearest )
				{
					flNearest = flDist;
					iBestRelationship = relationshipWithNextEnt;
					pReturn = pNextEnt;
				}
				if(flDist < flNormalNearest){
					flNormalNearest = flDist; //just keeping track of the closest "Dislike - Nemesis" range enemy.
				}

			}
		}//END OF this monster alive (as far as the AI cares) check

		pNextEnt = pNextEnt->m_pLink;
	}//END OF while loop through all seen monsters list "m_pLink".



	//Now, make a decision. 
	if(pReturn == NULL && pFearReturn == NULL){
		//both null? Nothing to do.
		return NULL;
	}else if(pReturn != NULL && pFearReturn == NULL){
		//pReturn isn't null but the other is? return this.
		return pReturn;
	}else if(pReturn == NULL && pFearReturn != NULL){
		//vice versa. the not-null one.
		return pFearReturn;
	}else{
		//both are not null? Whichever one is closer, do that. pReturn, the hostile, is a tie-breaker for equal distance.

		if(flNormalNearest <= flFearNearest){
			return pReturn;
		}else{
			return pFearReturn;
		}
	}

	//should not be reached.
	return pReturn;
}//END OF BestVisibleEnemy


//=========================================================
// MakeIdealYaw - gets a yaw value for the caller that would
// face the supplied vector. Value is stuffed into the monster's
// ideal_yaw
//=========================================================
void CBaseMonster :: MakeIdealYaw( Vector vecTarget )
{
	
	

	//MODDD - this kind of immitation "Strafe" is too crude and causes issues with the AI sometimes, like fiddling-around near the end point of a path..
	// strafing monster needs to face 90 degrees away from its goal
	if ( m_movementActivity == ACT_STRAFE_LEFT )
	{
		Vector	vecProjection;
		vecProjection.x = -vecTarget.y;
		vecProjection.y = vecTarget.x;


		//UTIL_drawLineFrame(pev->origin, (vecProjection + Vector(0, 0, vecTarget.z + 10) ), 7, 0, 255, 124 );
		pev->ideal_yaw = UTIL_VecToYaw( vecProjection - pev->origin );
	}
	else if ( m_movementActivity == ACT_STRAFE_RIGHT )
	{
		Vector	vecProjection;
		vecProjection.x = vecTarget.y;
		vecProjection.y = vecTarget.x;

		
		//UTIL_drawLineFrame(pev->origin, (vecProjection + Vector(0, 0, vecTarget.z + 10) ), 7, 255, 0, 124 );
		pev->ideal_yaw = UTIL_VecToYaw( vecProjection - pev->origin );
	}
	else
	{
		//vecProjection.x = vecTarget.x;
		//vecProjection.y = vecTarget.y;


		//UTIL_drawLineFrame(pev->origin, (vecProjection + Vector(0, 0, vecTarget.z + 10) ), 7, 255, 255, 255 );
		pev->ideal_yaw = UTIL_VecToYaw ( vecTarget - pev->origin );
	}


}

//=========================================================
// FlYawDiff - returns the difference ( in degrees ) between
// monster's current yaw and ideal_yaw
//
// Positive result is left turn, negative is right turn
//=========================================================
float CBaseMonster::FlYawDiff ( void )
{
	float flCurrentYaw;

	flCurrentYaw = UTIL_AngleMod( pev->angles.y );

	if ( flCurrentYaw == pev->ideal_yaw )
	{
		return 0;
	}


	return UTIL_AngleDiff( pev->ideal_yaw, flCurrentYaw );
}


//=========================================================
// Changeyaw - turns a monster towards its ideal_yaw
//=========================================================
float CBaseMonster::ChangeYaw ( int yawSpeed )
{
	float	ideal, current, move, speed;

	current = UTIL_AngleMod( pev->angles.y );
	ideal = pev->ideal_yaw;
	if (current != ideal)
	{
		speed = (float)yawSpeed * gpGlobals->frametime * 10;
		move = ideal - current;

		if (ideal > current)
		{
			if (move >= 180)
				move = move - 360;
		}
		else
		{
			if (move <= -180)
				move = move + 360;
		}

		if (move > 0)
		{// turning to the monster's left
			if (move > speed)
				move = speed;
		}
		else
		{// turning to the monster's right
			if (move < -speed)
				move = -speed;
		}
		
		pev->angles.y = UTIL_AngleMod (current + move);

		// turn head in desired direction only if they have a turnable head
		if (m_afCapability & bits_CAP_TURN_HEAD)
		{
			float yaw = pev->ideal_yaw - pev->angles.y;
			if (yaw > 180) yaw -= 360;
			if (yaw < -180) yaw += 360;
			// yaw *= 0.8;
			SetBoneController( 0, yaw );
		}
	}
	else
		move = 0;

	return move;
}

//=========================================================
// VecToYaw - turns a directional vector into a yaw value
// that points down that vector.
//=========================================================
float CBaseMonster::VecToYaw ( Vector vecDir )
{
	if (vecDir.x == 0 && vecDir.y == 0 && vecDir.z == 0)
		return pev->angles.y;

	return UTIL_VecToYaw( vecDir );

}




//=========================================================
// SetEyePosition
//
// queries the monster's model for $eyeposition and copies
// that vector to the monster's view_ofs
//
//=========================================================
void CBaseMonster :: SetEyePosition ( void )
{
	Vector  vecEyePosition;
	void *pmodel = GET_MODEL_PTR( ENT(pev) );

	GetEyePosition( pmodel, vecEyePosition );

	pev->view_ofs = vecEyePosition;

	if ( pev->view_ofs == g_vecZero )
	{
		ALERT ( at_aiconsole, "%s has no view_ofs!\n", STRING ( pev->classname ) );
	}
}

void CBaseMonster :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	switch( pEvent->event )
	{
	case SCRIPT_EVENT_DEAD:
		if ( m_MonsterState == MONSTERSTATE_SCRIPT )
		{
			pev->deadflag = DEAD_DYING;
			// Kill me now! (and fade out when CineCleanup() is called)
#if _DEBUG
			ALERT( at_aiconsole, "Death event: %s\n", STRING(pev->classname) );
#endif
			pev->health = 0;
		}
#if _DEBUG
		else
			ALERT( at_aiconsole, "INVALID death event:%s\n", STRING(pev->classname) );
#endif
		break;
	case SCRIPT_EVENT_NOT_DEAD:
		if ( m_MonsterState == MONSTERSTATE_SCRIPT )
		{
			pev->deadflag = DEAD_NO;
			// This is for life/death sequences where the player can determine whether a character is dead or alive after the script 
			pev->health = pev->max_health;
		}
		break;

	case SCRIPT_EVENT_SOUND:			// Play a named wave file
		EMIT_SOUND( edict(), CHAN_BODY, pEvent->options, 1.0, ATTN_IDLE );
		break;

	case SCRIPT_EVENT_SOUND_VOICE:
		EMIT_SOUND( edict(), CHAN_VOICE, pEvent->options, 1.0, ATTN_IDLE );
		break;

	case SCRIPT_EVENT_SENTENCE_RND1:		// Play a named sentence group 33% of the time
		if (RANDOM_LONG(0,2) == 0)
			break;
		// fall through...
	case SCRIPT_EVENT_SENTENCE:			// Play a named sentence group
		SENTENCEG_PlayRndSz( edict(), pEvent->options, 1.0, ATTN_IDLE, 0, 100 );
		break;

	case SCRIPT_EVENT_FIREEVENT:		// Fire a trigger
		FireTargets( pEvent->options, this, this, USE_TOGGLE, 0 );
		break;

	case SCRIPT_EVENT_NOINTERRUPT:		// Can't be interrupted from now on
		if ( m_pCine )
			m_pCine->AllowInterrupt( FALSE );
		break;

	case SCRIPT_EVENT_CANINTERRUPT:		// OK to interrupt now
		if ( m_pCine )
			m_pCine->AllowInterrupt( TRUE );
		break;

#if 0
	case SCRIPT_EVENT_INAIR:			// Don't DROP_TO_FLOOR()
	case SCRIPT_EVENT_ENDANIMATION:		// Set ending animation sequence to
		break;
#endif

	case MONSTER_EVENT_BODYDROP_HEAVY:
		if ( pev->flags & FL_ONGROUND )
		{
			//easyForcePrintLine("BODYDROP_HEAVY %d", this->soundSentenceSavePreference);
			if ( RANDOM_LONG( 0, 1 ) == 0 )
			{
				EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "common/bodydrop3.wav", 1, ATTN_NORM, 0, 90, FALSE );
			}
			else
			{
				EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "common/bodydrop4.wav", 1, ATTN_NORM, 0, 90, FALSE );
			}
		}
		break;

	case MONSTER_EVENT_BODYDROP_LIGHT:
		if ( pev->flags & FL_ONGROUND )
		{
			//easyForcePrintLine("BODYDROP_LIGHT %d", this->soundSentenceSavePreference);
			if ( RANDOM_LONG( 0, 1 ) == 0 )
			{
				EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "common/bodydrop3.wav", 1, ATTN_NORM, FALSE );
			}
			else
			{
				EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "common/bodydrop4.wav", 1, ATTN_NORM, FALSE );
			}
		}
		break;

	case MONSTER_EVENT_SWISHSOUND:
		{
			// NO MONSTER may use this anim event unless that monster's precache precaches this sound!!! this one uses the soundsentencesave.
			EMIT_SOUND_FILTERED( ENT(pev), CHAN_BODY, "zombie/claw_miss2.wav", 1, ATTN_NORM, TRUE );
			break;
		}

	default:
		ALERT( at_aiconsole, "Unhandled animation event %d for %s\n", pEvent->event, STRING(pev->classname) );
		break;

	}
}


// Combat









//MODDD - GetGunPositionAI is a new different version of GetGunPosition for use by AI calls. Things like 
//        This version should be less precise to the literal point of the gun on the model, which may varry with what sequence or frame is
//        picked. The attachment position used by some monsters can change often enough to let the player hide behind cover just right to
//        make the AI oscillate between "my gun shifted a little to the right... obscurred by cover" and "my gun shifted a little to the 
//        left... start firing!" and back and forth.
//        By comparing a more absolute position on the monster to the enemy's position intead, just a constant offset from the monster's origin,
//        this should not happen. Actual bullets can continue to come from the more accurate "GetGunPosition" below.

//        Note that this only applies to monsters that get the offset from attachment logic in getGunPosition, not an absolute offset.
//        In those cases, GetGunPositionAI's default behavior of just calling GetGunPosition works perfectly fine. Never that much precision used there.
	
Vector CBaseMonster :: GetGunPosition( )
{

	UTIL_MakeVectors(pev->angles);

	// Vector vecSrc = pev->origin + gpGlobals->v_forward * 10;
	//vecSrc.z = pevShooter->absmin.z + pevShooter->size.z * 0.7;
	//vecSrc.z = pev->origin.z + (pev->view_ofs.z - 4);
	Vector vecSrc = pev->origin 
					+ gpGlobals->v_forward * m_HackedGunPos.y 
					+ gpGlobals->v_right * m_HackedGunPos.x 
					+ gpGlobals->v_up * m_HackedGunPos.z;

	return vecSrc;
}
Vector CBaseMonster::GetGunPositionAI(){
	//return GetGunPosition();
	//For safety, this will be a clone of GetGunPosition, not a redirect. This means we need to rely on m_HackedGunPos like a Monster would have before,
	//not possibly redirect to a particular monster's own overly precise GetGunPosition.
	UTIL_MakeVectors(pev->angles);
	Vector vecSrc = pev->origin 
					+ gpGlobals->v_forward * m_HackedGunPos.y 
					+ gpGlobals->v_right * m_HackedGunPos.x 
					+ gpGlobals->v_up * m_HackedGunPos.z;
	return vecSrc;
}



//MODDD - this is commonly used to aim the torso of a monster pitch-wise to look at its enemy.  The model must support this,
//        and this is assuming blend #0 handles that.
void CBaseMonster::lookAtEnemy_pitch(void){
	Vector vecShootDir;
	Vector angDir;
	UTIL_MakeVectors(pev->angles);
	vecShootDir = ShootAtEnemyMod( GetGunPosition() );
	angDir = UTIL_VecToAngles( vecShootDir );
	
	// make angles +-180
	if (angDir.x > 180){
		angDir.x = angDir.x - 360;
	}

	SetBlending( 0, angDir.x );
}//END OF lookAtEnemy_pitch





//=========================================================
// NODE GRAPH
//=========================================================





//=========================================================
// FGetNodeRoute - tries to build an entire node path from
// the callers origin to the passed vector. If this is 
// possible, ROUTE_SIZE waypoints will be copied into the
// callers m_Route. TRUE is returned if the operation 
// succeeds (path is valid) or FALSE if failed (no path 
// exists )
//=========================================================
BOOL CBaseMonster :: FGetNodeRoute ( Vector vecDest )
{

	//return FALSE;

	int iPath[ MAX_PATH_SIZE ];
	int iSrcNode, iDestNode;
	int iResult;
	int i;
	int iNumToCopy;

	iSrcNode = WorldGraph.FindNearestNode ( pev->origin, this );
	iDestNode = WorldGraph.FindNearestNode ( vecDest, this );

	if ( iSrcNode == -1 )
	{
		// no node nearest self
//		ALERT ( at_aiconsole, "FGetNodeRoute: No valid node near self!\n" );
		return FALSE;
	}
	else if ( iDestNode == -1 )
	{
		// no node nearest target
//		ALERT ( at_aiconsole, "FGetNodeRoute: No valid node near target!\n" );
		return FALSE;
	}

	// valid src and dest nodes were found, so it's safe to proceed with
	// find shortest path
	int iNodeHull = WorldGraph.HullIndex( this ); // make this a monster virtual function
	
	//MODDD - CHEAT CHEAT CHEAT!
	//iNodeHull = NODE_POINT_HULL;

	iResult = WorldGraph.FindShortestPath ( iPath, iSrcNode, iDestNode, iNodeHull, m_afCapability );


	if(iResult==2 && iPath[0] == iPath[1]){
		//MODDD TODO - 
		//That is, if the # of nodes returned is 2 and they are exactly equal, it means we returned our own position.
		//---Should we set some flag to do something about this...? Ranged AI can just try another nearby node also close to the enemy
		//to see if that is a point with a clear shot?

		//go ahead and try the straight-shot.
		CBaseEntity* goalEnt = NULL;
		if(m_hEnemy != NULL){
			//goalEnt = (CBaseEntity *)GET_PRIVATE( m_hEnemy.Get( ) );
			goalEnt = m_hEnemy.GetEntity();
		}

		if(this->CheckLocalMove(pev->origin, m_vecMoveGoal, goalEnt, NULL)){
			//it is okay apparently.
			int x = 45;
		}else{
			//No!
			return FALSE;
		}

	}


	if ( !iResult )
	{
#if 1
		ALERT ( at_aiconsole, "No Path from %d to %d!\n", iSrcNode, iDestNode );
		return FALSE;
#else
		BOOL bRoutingSave = WorldGraph.m_fRoutingComplete;
		WorldGraph.m_fRoutingComplete = FALSE;
		iResult = WorldGraph.FindShortestPath(iPath, iSrcNode, iDestNode, iNodeHull, m_afCapability);
		WorldGraph.m_fRoutingComplete = bRoutingSave;
		if ( !iResult )
		{
			ALERT ( at_aiconsole, "No Path from %d to %d!\n", iSrcNode, iDestNode );
			return FALSE;
		}
		else
		{
			ALERT ( at_aiconsole, "Routing is inconsistent!" );
		}
#endif
	}

	// there's a valid path within iPath now, so now we will fill the route array
	// up with as many of the waypoints as it will hold.
	
	// don't copy ROUTE_SIZE entries if the path returned is shorter
	// than ROUTE_SIZE!!!
	if ( iResult < ROUTE_SIZE )
	{
		iNumToCopy = iResult;
	}
	else
	{
		iNumToCopy = ROUTE_SIZE;
	}
	
	for ( i = 0 ; i < iNumToCopy; i++ )
	{
		m_Route[ i ].vecLocation = WorldGraph.m_pNodes[ iPath[ i ] ].m_vecOrigin;
		m_Route[ i ].iType = bits_MF_TO_NODE;
	}
	
	if ( iNumToCopy < ROUTE_SIZE )
	{
		m_Route[ iNumToCopy ].vecLocation = vecDest;
		//why not both?
		//m_Route[ iNumToCopy ].iType |= bits_MF_IS_GOAL;
		m_Route[ iNumToCopy ].iType |= (bits_MF_TO_NODE | bits_MF_IS_GOAL);

	}

	return TRUE;
}

//=========================================================
// FindHintNode
//=========================================================
int CBaseMonster :: FindHintNode ( void )
{
	int i;
	TraceResult tr;

	if ( !WorldGraph.m_fGraphPresent )
	{
		ALERT ( at_aiconsole, "find_hintnode: graph not ready!\n" );
		return NO_NODE;
	}

	if ( WorldGraph.m_iLastActiveIdleSearch >= WorldGraph.m_cNodes )
	{
		WorldGraph.m_iLastActiveIdleSearch = 0;
	}

	for ( i = 0; i < WorldGraph.m_cNodes ; i++ )
	{
		int nodeNumber = (i + WorldGraph.m_iLastActiveIdleSearch) % WorldGraph.m_cNodes;
		CNode &node = WorldGraph.Node( nodeNumber );

		if ( node.m_sHintType )
		{
			// this node has a hint. Take it if it is visible, the monster likes it, and the monster has an animation to match the hint's activity.
			if ( FValidateHintType ( node.m_sHintType ) )
			{
				if ( !node.m_sHintActivity || LookupActivity ( node.m_sHintActivity ) != ACTIVITY_NOT_AVAILABLE )
				{
					UTIL_TraceLine ( pev->origin + pev->view_ofs, node.m_vecOrigin + pev->view_ofs, ignore_monsters, ENT(pev), &tr );

					if ( tr.flFraction == 1.0 )
					{
						WorldGraph.m_iLastActiveIdleSearch = nodeNumber + 1; // next monster that searches for hint nodes will start where we left off.
						return nodeNumber;// take it!
					}
				}
			}
		}
	}

	WorldGraph.m_iLastActiveIdleSearch = 0;// start at the top of the list for the next search.

	return NO_NODE;
}
			


void CBaseMonster::reportNetName(void){


	const char* netnameBetter;
	if(FStringNull(pev->netname) || ((netnameBetter = STRING(pev->netname)) == NULL)  ){
		easyForcePrintLine("%s:%d MY NETNAME IS %s", this->getClassname(), this->monsterID, "__NULL__");
	}else{
		easyForcePrintLine("%s:%d MY NETNAME IS %s", this->getClassname(), this->monsterID, netnameBetter);
	}

}



char* getActivityName(Activity arg_act){
	int i = 0;
	int actAsNumber = (int)arg_act;

	while ( activity_map[i].type != 0 )
	{
		if ( activity_map[i].type == actAsNumber )
		{
			//ALERT( level, "Activity %s, ", activity_map[i].name );
			return activity_map[i].name;
			break;
		}
		i++;
	}
	return "NONE?";
}//END OF getActivityName




void CBaseMonster::ReportAIState( void )
{
	float currentYaw = UTIL_AngleMod( pev->angles.y );

	//this->pev->movetype = MOVETYPE_TOSS;
	//ClearBits( pev->flags, FL_ONGROUND );

	//m_pfnThink m_pfnTouch
	ALERT_TYPE level = at_console;

	//mirrors the states listed in util.h's MONSTERSTATE enum.
	static const char *pStateNames[] = { "None", "Idle", "Combat", "Alert", "Hunt", "Prone", "Script", "PlayDead", "Dead" };

	

	easyForcePrintLine("%s ID:%d", getClassname(), monsterID);

	int i = 0;
	

	easyForcePrintLine("ACTS: Current:%s  Ideal:%s  Movement:%s", getActivityName(m_Activity), getActivityName(m_IdealActivity), getActivityName(m_movementActivity));
		
	easyForcePrintLine("STATES: Current: %s  Ideal: %s", pStateNames[this->m_MonsterState], pStateNames[this->m_IdealMonsterState]);





	if ( m_pSchedule )
	{
		easyForcePrint("Schedule: %s || FailSchedType:%d\n", getScheduleName(), m_failSchedule);
		Task_t *pTask = GetTask();
		if ( pTask ){
			easyForcePrint("Task#: %d (schedindex:%d) ", pTask->iTask, m_iScheduleIndex );
		}else{
			easyForcePrint("Task#: NULL?");
		}

	}
	else{
		easyForcePrint("Schedule: NULL?!");
	}
	easyForcePrint("\n");


	if ( IsMoving() )
	{
		easyForcePrint("Moving: Yes." );
		if ( m_flMoveWaitFinished > gpGlobals->time ){
			easyForcePrint(" !!! Stopped for %.2f. ", m_flMoveWaitFinished - gpGlobals->time );
		}else if ( m_IdealActivity == GetStoppedActivity() ){
			easyForcePrint(" !!! In stopped anim. " );
		}
	}else{
		easyForcePrint("Moving: No.");
	}
	easyForcePrint("\n");

	
	
	CSquadMonster *pSquadMonster = MySquadMonsterPointer();

	if ( pSquadMonster )
	{
		easyForcePrint("SQUAD:");
		if ( !pSquadMonster->InSquad() )
		{
			easyForcePrint("not " );
		}

		easyForcePrint("In Squad, " );

		if ( !pSquadMonster->IsLeader() )
		{
			easyForcePrint("not " );
		}

		easyForcePrint("Leader." );
		easyForcePrint("\n");
	}

	char enemyInfoString[127];
	enemyInfoString[0] = '\0';
	char targetInfoString[127];
	targetInfoString[0] = '\0';

	CBaseMonster* tempMonsterPointer;

	tempMonsterPointer = NULL;
	if(m_hTargetEnt != NULL ){
		if((tempMonsterPointer = m_hTargetEnt->MyMonsterPointer()) != NULL){
			sprintf(targetInfoString, "%s:%d", m_hTargetEnt->getClassname(), tempMonsterPointer->monsterID);
		}else{
			sprintf(targetInfoString, "%s:%s", m_hTargetEnt->getClassname(), "NOT A MONSTER");
		}
	}else{
		sprintf(targetInfoString, "%s", "EMPTY");
	}

	tempMonsterPointer = NULL;
	if(m_hEnemy != NULL ){
		if((tempMonsterPointer = m_hEnemy->MyMonsterPointer()) != NULL){
			sprintf(enemyInfoString, "%s:%d", m_hEnemy->getClassname(), tempMonsterPointer->monsterID);
		}else{
			sprintf(enemyInfoString, "%s:%s", m_hEnemy->getClassname(), "NOT A MONSTER");
		}
	}else{
		sprintf(enemyInfoString, "%s", "EMPTY");
	}
	easyForcePrintLine("Enemy: %s   TargetEnt: %s", enemyInfoString, targetInfoString);
	

	/*
	//MODDD TODO - No... make this its own printout callable by the client (console) later.
	easyForcePrint("Old Enemey Queue: ");

	if(m_intOldEnemyNextIndex > 0){
	
		for(int i = 0; i < m_intOldEnemyNextIndex; i++){
			
			//IsAlive_FromAI takes "this" mosnter as a parameter. It already knows what itself is.
			if (m_hOldEnemy[i] != NULL && m_hOldEnemy[i]->IsAlive_FromAI(this))
			{
				//this is okay.
				if(i!=0)easyForcePrint(", ");
				easyForcePrint("#%d:%s:ID%d", i, m_hOldEnemy[i]->getClassnameShort(), m_hOldEnemy[i]->MyMonsterPointer()->monsterID);
			}
			else {
				//null or dead. Say so.
				if(i!=0)easyForcePrint(", ");
				easyForcePrint("#%d:%s", i, "EMPTY");	
			}
		}//END OF for(...)
		
		easyForcePrintLine("");
	}else{
		
		easyForcePrintLine("EMPTY");
	}
	*/
	
	easyForcePrintLine("Health: %.2f / %.2f", pev->health, pev->max_health );
	easyForcePrintLine("Sequence ID:%d Frame:%.2f Framerate:%.2f spawnflag:%d deadflag:%d loops:%d sequencefinished:%d ThinkACTIVE:%d nextThink:%.2f currentTime:%.2f targetname:%s target:%s globalname:%s", pev->sequence, pev->frame, pev->framerate, pev->spawnflags, pev->deadflag, this->m_fSequenceLoops, this->m_fSequenceFinished, (m_pfnThink!=NULL), pev->nextthink, gpGlobals->time,  STRING(pev->targetname), STRING(pev->target), STRING(pev->globalname ) );
	easyForcePrintLine("Yaw:%.2f Ideal:%.2f yawspd:%.2f", currentYaw, pev->ideal_yaw, pev->yaw_speed);


	if(m_pCine == NULL){
		easyForcePrintLine("CINE: none");
	}else{
		easyForcePrintLine("CINE: %s:%d  sf:%d ob:%d targetname:%s target:%s iszE:%s globalname: %s", m_pCine->getClassname(), m_pCine->monsterID, m_pCine->pev->spawnflags, m_pCine->ObjectCaps(),  STRING(m_pCine->pev->targetname), STRING(m_pCine->pev->target),  STRING( m_pCine->m_iszEntity ), STRING(m_pCine->pev->globalname ) );
	}
	

	easyForcePrint("Capability: ");
	printLineIntAsBinary((unsigned int)m_afCapability, 32u);


	//

	//easyForcePrintLine("isOrganic:%d", isOrganic());

	easyForcePrintLine("EXTRA: flags:%d effects:%d renderfx:%d rendermode:%d renderamt:%.2f gamestate:%d solid:%d waterlevel:%d gravity:%.2f movetype:%d", pev->flags, pev->effects, pev->renderfx, pev->rendermode, pev->renderamt, pev->gamestate, pev->solid, pev->waterlevel, pev->gravity, pev->movetype);
	easyForcePrintLine("Eyepos: (%.2f,%.2f,%.2f)", pev->view_ofs.x, pev->view_ofs.y, pev->view_ofs.z);
	easyForcePrint("BOUNDS: mins: (%.2f,%.2f,%.2f)", pev->mins.x, pev->mins.y, pev->mins.z);
	easyForcePrintLine("maxs: (%.2f,%.2f,%.2f)", pev->maxs.x, pev->maxs.y, pev->maxs.z);


	easyForcePrintLine("-------------------------------------------------------------");

}

//=========================================================
// KeyValue
//
// !!! netname entvar field is used in squadmonster for groupname!!!
//=========================================================
void CBaseMonster :: KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "TriggerTarget"))
	{
		m_iszTriggerTarget = ALLOC_STRING( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "TriggerCondition") )
	{
		m_iTriggerCondition = atoi( pkvd->szValue );
		pkvd->fHandled = TRUE;
	}
	else
	{
		CBaseToggle::KeyValue( pkvd );
	}
}

//=========================================================
// FCheckAITrigger - checks the monster's AI Trigger Conditions,
// if there is a condition, then checks to see if condition is 
// met. If yes, the monster's TriggerTarget is fired.
//
// Returns TRUE if the target is fired.
//=========================================================
BOOL CBaseMonster :: FCheckAITrigger ( void )
{
	BOOL fFireTarget;

	if ( m_iTriggerCondition == AITRIGGER_NONE )
	{
		// no conditions, so this trigger is never fired.
		return FALSE; 
	}

	fFireTarget = FALSE;

	switch ( m_iTriggerCondition )
	{
	case AITRIGGER_SEEPLAYER_ANGRY_AT_PLAYER:
		if ( m_hEnemy != NULL && m_hEnemy->IsPlayer() && HasConditions ( bits_COND_SEE_ENEMY ) )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_SEEPLAYER_UNCONDITIONAL:
		if ( HasConditions ( bits_COND_SEE_CLIENT ) )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_SEEPLAYER_NOT_IN_COMBAT:
		if ( HasConditions ( bits_COND_SEE_CLIENT ) && 
			 m_MonsterState != MONSTERSTATE_COMBAT	&& 
			 m_MonsterState != MONSTERSTATE_PRONE	&& 
			 m_MonsterState != MONSTERSTATE_SCRIPT)
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_TAKEDAMAGE:
		if ( HasConditions( bits_COND_LIGHT_DAMAGE | bits_COND_HEAVY_DAMAGE ) )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_DEATH:
		if ( pev->deadflag != DEAD_NO )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_HALFHEALTH:
		if ( IsAlive() && pev->health <= ( pev->max_health / 2 ) )
		{
			fFireTarget = TRUE;
		}
		break;
/*

  // !!!UNDONE - no persistant game state that allows us to track these two. 

	case AITRIGGER_SQUADMEMBERDIE:
		break;
	case AITRIGGER_SQUADLEADERDIE:
		break;
*/
	case AITRIGGER_HEARWORLD:
		if ( HasAllConditions(bits_COND_HEAR_SOUND | bits_SOUND_WORLD) )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_HEARPLAYER:
		if ( HasAllConditions(bits_COND_HEAR_SOUND | bits_SOUND_PLAYER) )
		{
			fFireTarget = TRUE;
		}
		break;
	case AITRIGGER_HEARCOMBAT:
		if ( HasAllConditions(bits_COND_HEAR_SOUND | bits_SOUND_COMBAT) )
		{
			fFireTarget = TRUE;
		}
		break;
	}

	if ( fFireTarget )
	{
		// fire the target, then set the trigger conditions to NONE so we don't fire again
		ALERT ( at_aiconsole, "AI Trigger Fire Target\n" );
		FireTargets( STRING( m_iszTriggerTarget ), this, this, USE_TOGGLE, 0 );
		m_iTriggerCondition = AITRIGGER_NONE;
		return TRUE;
	}

	return FALSE;
}

//=========================================================	
// CanPlaySequence - determines whether or not the monster
// can play the scripted sequence or AI sequence that is 
// trying to possess it. If DisregardState is set, the monster
// will be sucked into the script no matter what state it is
// in. ONLY Scripted AI ents should allow this.
//=========================================================	
int CBaseMonster :: CanPlaySequence( BOOL fDisregardMonsterState, int interruptLevel )
{
	//MODDD - it is possible for "EASY_CVAR_GET(cineAllowSequenceOverwrite)" to block "m_pCine" already being occupied to stop this from having influence.
	if ( (m_pCine && EASY_CVAR_GET(cineAllowSequenceOverwrite) != 1) || !IsAlive() || m_MonsterState == MONSTERSTATE_PRONE )
	{
		// monster is already running a scripted sequence or dead!
		return FALSE;
	}
	
	if ( fDisregardMonsterState )
	{
		// ok to go, no matter what the monster state. (scripted AI)
		return TRUE;
	}

	if ( m_MonsterState == MONSTERSTATE_NONE || m_MonsterState == MONSTERSTATE_IDLE || m_IdealMonsterState == MONSTERSTATE_IDLE )
	{
		// ok to go, but only in these states
		return TRUE;
	}
	
	if ( m_MonsterState == MONSTERSTATE_ALERT && interruptLevel >= SS_INTERRUPT_BY_NAME )
		return TRUE;

	// unknown situation
	return FALSE;
}


//=========================================================
// FindLateralCover - attempts to locate a spot in the world
// directly to the left or right of the caller that will
// conceal them from view of pSightEnt
//=========================================================
#define COVER_CHECKS	5// how many checks are made
#define COVER_DELTA		48// distance between checks

BOOL CBaseMonster :: FindLateralCover ( const Vector &vecThreat, const Vector &vecViewOffset )
{
	TraceResult	tr;
	Vector	vecBestOnLeft;
	Vector	vecBestOnRight;
	Vector	vecLeftTest;
	Vector	vecRightTest;
	Vector	vecStepRight;
	int	i;

	UTIL_MakeVectors ( pev->angles );
	vecStepRight = gpGlobals->v_right * COVER_DELTA;
	vecStepRight.z = 0; 
	
	vecLeftTest = vecRightTest = pev->origin;

	for ( i = 0 ; i < COVER_CHECKS ; i++ )
	{
		vecLeftTest = vecLeftTest - vecStepRight;
		vecRightTest = vecRightTest + vecStepRight;

		// it's faster to check the SightEnt's visibility to the potential spot than to check the local move, so we do that first.
		UTIL_TraceLine( vecThreat + vecViewOffset, vecLeftTest + pev->view_ofs, ignore_monsters, ignore_glass, ENT(pev)/*pentIgnore*/, &tr);
		
		if (tr.flFraction != 1.0)
		{
			if ( FValidateCover ( vecLeftTest ) && CheckLocalMove( pev->origin, vecLeftTest, NULL, NULL ) == LOCALMOVE_VALID )
			{
				if ( MoveToLocation( ACT_RUN, 0, vecLeftTest ) )
				{
					return TRUE;
				}
			}
		}
		
		// it's faster to check the SightEnt's visibility to the potential spot than to check the local move, so we do that first.
		UTIL_TraceLine(vecThreat + vecViewOffset, vecRightTest + pev->view_ofs, ignore_monsters, ignore_glass, ENT(pev)/*pentIgnore*/, &tr);
		
		if ( tr.flFraction != 1.0 )
		{
			if (  FValidateCover ( vecRightTest ) && CheckLocalMove( pev->origin, vecRightTest, NULL, NULL ) == LOCALMOVE_VALID )
			{
				if ( MoveToLocation( ACT_RUN, 0, vecRightTest ) )
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


Vector CBaseMonster :: ShootAtEnemy( const Vector &shootOrigin )
{
	CBaseEntity *pEnemy = m_hEnemy;

	if ( pEnemy )
	{
		//MODDD NOTE ........ what is this formula?
		//    I'm guessing that the BodyTarget includes the enemy's pev->origin, but we subtract it out so we can substitute it with m_vecEnemyLKP.
		//    So why not make a separate BodyTarget method that never added pev->origin in the first place? Who knows.
		return ( (pEnemy->BodyTarget( shootOrigin ) - pEnemy->pev->origin) + m_vecEnemyLKP - shootOrigin ).Normalize();
	}
	else
		return gpGlobals->v_forward;
	//MODDD NOTICE - isn't trusting "gpGlobals->v_forward" kinda dangerous? This assumes we recently called MakeVectors and not privately for v_forward to be relevant
	//               to this monster.
}

//Similar to above, but uses BodyTargetMod instead of BodyTarget. BodyTargetMod returns the true center of the player so that things that use this result to determine
//aim pitch don't appear to jitter around up and down (with changes in the slightly randomly returned BodyCenter) just to get an aim pitch.
Vector CBaseMonster :: ShootAtEnemyMod( const Vector &shootOrigin )
{
	CBaseEntity *pEnemy = m_hEnemy;

	if ( pEnemy )
	{
		//MODDD NOTE ........ what is this formula?
		//    I'm guessing that the BodyTarget includes the enemy's pev->origin, but we subtract it out so we can substitute it with m_vecEnemyLKP.
		//    So why not make a separate BodyTarget method that never added pev->origin in the first place? Who knows.
		return ( (pEnemy->BodyTargetMod( shootOrigin ) - pEnemy->pev->origin) + m_vecEnemyLKP - shootOrigin ).Normalize();
	}
	else
		return gpGlobals->v_forward;
	//MODDD NOTICE - isn't trusting "gpGlobals->v_forward" kinda dangerous? This assumes we recently called MakeVectors and not privately for v_forward to be relevant
	//               to this monster.
}









//=========================================================
// FacingIdeal - tells us if a monster is facing its ideal
// yaw. Created this function because many spots in the 
// code were checking the yawdiff against this magic
// number. Nicer to have it in one place if we're gonna
// be stuck with it.
//=========================================================
BOOL CBaseMonster :: FacingIdeal( void )
{
	
	/*
	float flCurrentYaw = UTIL_AngleMod( pev->angles.y );

	//if ( flCurrentYaw == pev->ideal_yaw )
	easyForcePrintLine("monsterID:%d WHAT?? diff:%.2f cur:%.2f ideal:%.2f", monsterID, FlYawDiff(), flCurrentYaw, pev->ideal_yaw);
	*/




	if ( fabs( FlYawDiff() ) <= 0.006 )//!!!BUGBUG - no magic numbers!!!
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CBaseMonster :: FacingIdeal( float argDegreeTolerance )
{
	if ( fabs( FlYawDiff() ) <= argDegreeTolerance )//Finally, that not-so-magic number.
	{
		return TRUE;
	}

	return FALSE;
}



//=========================================================
// FCanActiveIdle
//=========================================================
BOOL CBaseMonster :: FCanActiveIdle ( void )
{
	/*
	if ( m_MonsterState == MONSTERSTATE_IDLE && m_IdealMonsterState == MONSTERSTATE_IDLE && !IsMoving() )
	{
		return TRUE;
	}
	*/
	return FALSE;
}


void CBaseMonster::PlaySentence( const char *pszSentence, float duration, float volume, float attenuation )
{
	if ( pszSentence && IsAlive() )
	{
		if ( pszSentence[0] == '!' )
			EMIT_SOUND_DYN( edict(), CHAN_VOICE, pszSentence, volume, attenuation, 0, PITCH_NORM );
		else
			SENTENCEG_PlayRndSz( edict(), pszSentence, volume, attenuation, 0, PITCH_NORM );
	}
}


void CBaseMonster::PlayScriptedSentence( const char *pszSentence, float duration, float volume, float attenuation, BOOL bConcurrent, CBaseEntity *pListener )
{ 
	PlaySentence( pszSentence, duration, volume, attenuation );
}


void CBaseMonster::SentenceStop( void )
{
	EMIT_SOUND( edict(), CHAN_VOICE, "common/null.wav", 1.0, ATTN_IDLE );
}


void CBaseMonster::CorpseFallThink( void )
{
	if ( pev->flags & FL_ONGROUND )
	{
		SetThink ( NULL );

		SetSequenceBox( );
		UTIL_SetOrigin( pev, pev->origin );// link into world.
	}
	else
		pev->nextthink = gpGlobals->time + 0.1;
}

// Call after animation/pose is set up
void CBaseMonster :: MonsterInitDead( void )
{
	InitBoneControllers();

	pev->solid			= SOLID_BBOX;
	pev->movetype		= MOVETYPE_TOSS;// so he'll fall to ground

	//MDODD - always play death-anim at least forward (and not too slow)?
	if(m_flFramerateSuggestion <= 0.2){
		m_flFramerateSuggestion = 0.2;
	}

	resetFrame();
	ResetSequenceInfo( );
	pev->framerate = 0;
	
	// Copy health
	pev->max_health		= pev->health;
	pev->deadflag		= DEAD_DEAD;
	
	UTIL_SetSize(pev, g_vecZero, g_vecZero );
	UTIL_SetOrigin( pev, pev->origin );


	//MODDD - flag for mirror recognition.
	pev->renderfx |= ISNPC;

	// Setup health counters, etc.
	BecomeDead();
	SetThink( &CBaseMonster::CorpseFallThink );
	pev->nextthink = gpGlobals->time + 0.5;
}

//MODDD - more of a placeholder, see "NoFriendlyFireImp" for more complexity.
// As to how this was never at least minimally stubbed as-is, no clue.
BOOL CBaseMonster::NoFriendlyFire(void) {
	// Usually for squadmonsters.  Otherwise assume we can fire.
	return TRUE;
}





//=========================================================
// BBoxIsFlat - check to see if the monster's bounding box
// is lying flat on a surface (traces from all four corners
// are same length.)
//=========================================================
BOOL CBaseMonster :: BBoxFlat ( void )
{
	TraceResult	tr;
	Vector		vecPoint;
	float	flXSize, flYSize;
	float	flLength;
	float	flLength2;

	flXSize = pev->size.x / 2;
	flYSize = pev->size.y / 2;

	vecPoint.x = pev->origin.x + flXSize;
	vecPoint.y = pev->origin.y + flYSize;
	vecPoint.z = pev->origin.z;

	UTIL_TraceLine ( vecPoint, vecPoint - Vector ( 0, 0, 100 ), ignore_monsters, ENT(pev), &tr );
	flLength = (vecPoint - tr.vecEndPos).Length();

	vecPoint.x = pev->origin.x - flXSize;
	vecPoint.y = pev->origin.y - flYSize;

	UTIL_TraceLine ( vecPoint, vecPoint - Vector ( 0, 0, 100 ), ignore_monsters, ENT(pev), &tr );
	flLength2 = (vecPoint - tr.vecEndPos).Length();
	if ( flLength2 > flLength )
	{
		return FALSE;
	}
	flLength = flLength2;

	vecPoint.x = pev->origin.x - flXSize;
	vecPoint.y = pev->origin.y + flYSize;
	UTIL_TraceLine ( vecPoint, vecPoint - Vector ( 0, 0, 100 ), ignore_monsters, ENT(pev), &tr );
	flLength2 = (vecPoint - tr.vecEndPos).Length();
	if ( flLength2 > flLength )
	{
		return FALSE;
	}
	flLength = flLength2;

	vecPoint.x = pev->origin.x + flXSize;
	vecPoint.y = pev->origin.y - flYSize;
	UTIL_TraceLine ( vecPoint, vecPoint - Vector ( 0, 0, 100 ), ignore_monsters, ENT(pev), &tr );
	flLength2 = (vecPoint - tr.vecEndPos).Length();
	if ( flLength2 > flLength )
	{
		return FALSE;
	}
	flLength = flLength2;

	return TRUE;
}



//MODDD - new version.  Can accept whether this should ignore the _SEE conditions and schedule interruptable by NEW_ENEMY checks.
//        RUNTASK logic wanting to search for new enemies in the middle of a task shouldn't be ignored simply because those requirements aren't met.
BOOL CBaseMonster::GetEnemy(void){
	return GetEnemy(FALSE);
}

//=========================================================
// Get Enemy - tries to find the best suitable enemy for the monster.
//=========================================================
BOOL CBaseMonster :: GetEnemy (BOOL arg_forceWork )
{
	CBaseEntity *pNewEnemy;


	if(EASY_CVAR_GET(peaceOut) == 1){
		//like, no way, man. Let's just smoke a fat blunt and sit on the couch all day.
		return FALSE;
	}



	//MODDD - if dead, can't do this.
	if(!UTIL_IsAliveEntity(this)){
		return FALSE;// monster has no enemy
	}
	

	//MODDD - major. Adding FEAR as a condition here now.

	//if ( HasConditions(bits_COND_SEE_HATE | bits_COND_SEE_DISLIKE | bits_COND_SEE_NEMESIS) )
	if (arg_forceWork || HasConditions(bits_COND_SEE_FEAR | bits_COND_SEE_HATE | bits_COND_SEE_DISLIKE | bits_COND_SEE_NEMESIS) )
	{
		pNewEnemy = BestVisibleEnemy();

		if ( pNewEnemy != m_hEnemy && pNewEnemy != NULL)
		{
			// DO NOT mess with the monster's m_hEnemy pointer unless the schedule the monster is currently running will be interrupted
			// by COND_NEW_ENEMY. This will eliminate the problem of monsters getting a new enemy while they are in a schedule that doesn't care,
			// and then not realizing it by the time they get to a schedule that does. I don't feel this is a good permanent fix. 

			if ( m_pSchedule ){
				//easyPrintLine("WHAT THE schedname %s", m_pSchedule->pName);
				//easyPrintLine("OH NOI %d", m_pSchedule->iInterruptMask & bits_COND_NEW_ENEMY );

				
				//MODDD - sometimes, such as when the chumtoad is playing dead, we need to be able to pick up on new enemies (even in case the original one gets forgotten).
				//        We need a schedule that is not interruptible by bits_COND_NEW_ENEMY, and yet can still take new enemies during its run.
				//        Override "getForceAllowNewEnemy" to work for the playdead schedule.

				//MODDD - POINT OF FRUSTRATION HERE
				//if ( m_pSchedule->iInterruptMask & bits_COND_NEW_ENEMY )
				if ( m_pSchedule->iInterruptMask & bits_COND_NEW_ENEMY || arg_forceWork || getForceAllowNewEnemy(pNewEnemy) )
				{
					PushEnemy( m_hEnemy, m_vecEnemyLKP );
					SetConditions(bits_COND_NEW_ENEMY);
					m_hEnemy = pNewEnemy;
					
					//m_vecEnemyLKP = m_hEnemy->pev->origin;
					setEnemyLKP(m_hEnemy->pev->origin);

				}
				// if the new enemy has an owner, take that one as well
				if (pNewEnemy->pev->owner != NULL)
				{
					CBaseEntity *pOwner = GetMonsterPointer( pNewEnemy->pev->owner );
					if ( pOwner && (pOwner->pev->flags & FL_MONSTER) && IRelationship( pOwner ) != R_NO )
						PushEnemy( pOwner, m_vecEnemyLKP );
				}
			}
		}
	}

	// remember old enemies
	if (m_hEnemy == NULL && PopEnemy( ))
	{
		if ( m_pSchedule )
		{
			if ( m_pSchedule->iInterruptMask & bits_COND_NEW_ENEMY )
			{
				SetConditions(bits_COND_NEW_ENEMY);
				//MODDD - hold on. Shouldn't we also rip the old enemy LKP from the stack too?
				//        ACTUALLY The PopEnemy() call above automatically does that. It did even change m_hEnemy to the next enemy remembered in line.

			}
		}
	}

	if ( m_hEnemy != NULL )
	{
		// monster has an enemy.
		//MODDD NOTE - returning trues just because m_hEnemy ends up "not null" isn't very informative.
		//Perhaps we should return TRUE only if a new enemy was actually picked, or at least an attempt was made to pick the bestVisibleEnemy regarldess
		//of it matching our old enemy or not?  Say we just... did something?
		return TRUE;
	}

	return FALSE;// monster has no enemy
}


//=========================================================
// DropItem - dead monster drops named item 
//=========================================================
CBaseEntity* CBaseMonster :: DropItem ( char *pszItemName, const Vector &vecPos, const Vector &vecAng )
{
	if ( !pszItemName )
	{
		ALERT ( at_console, "DropItem() - No item name!\n" );
		return NULL;
	}

	CBaseEntity *pItem = CBaseEntity::Create( pszItemName, vecPos, vecAng, edict() );

	if ( pItem )
	{
		// do we want this behavior to be default?! (sjb)
		pItem->pev->velocity = pev->velocity;
		pItem->pev->avelocity = Vector ( 0, RANDOM_FLOAT( 0, 100 ), 0 );
		return pItem;
	}
	else
	{
		ALERT ( at_console, "DropItem() - Didn't create!\n" );
		return FALSE;
	}

}


BOOL CBaseMonster :: ShouldFadeOnDeath( void )
{
	// if flagged to fade out or I have an owner (I came from a monster spawner)
	if ( (pev->spawnflags & SF_MONSTER_FADECORPSE) || !FNullEnt( pev->owner ) )
		return TRUE;

	return FALSE;
}



void CBaseMonster::setPhysicalHitboxForDeath(void){
	//just stuff schedule.cpp used to do at death (and still does really, just more modular this way)

	//MODDD NOTE - it seems this is not always quite right, unsure.
	if ( !BBoxFlat() )
	{
		// a bit of a hack. If a corpses' bbox is positioned such that being left solid so that it can be attacked will
		// block the player on a slope or stairs, the corpse is made nonsolid. 
//					pev->solid = SOLID_NOT;
		easyPrintLine("setPhysicalHitboxForDeath: A");
		UTIL_SetSize ( pev, Vector ( -4, -4, 0 ), Vector ( 4, 4, 0.2 ) );
	}
	else
	{
		// !!!HACKHACK - put monster in a thin, wide bounding box until we fix the solid type/bounding volume problem
		//MODDD - thinner height-wise. used to be "1" tall from the mins.z (and above)
		easyPrintLine("setPhysicalHitboxForDeath: B");
		UTIL_SetSize ( pev, Vector ( pev->mins.x, pev->mins.y, pev->mins.z ), Vector ( pev->maxs.x, pev->maxs.y, pev->mins.z + 0.2 ) );
		//UTIL_SetSize ( pev, Vector ( pev->mins.x, pev->mins.y, 0), Vector ( pev->maxs.x, pev->maxs.y, 0 ) );
	}

}





//All ripped from the start of TASK_DIE in schedule.cpp.
void CBaseMonster::DeathAnimationStart(){
	
	
	RouteClear();	
			
	m_IdealActivity = GetDeathActivity();
	signalActivityUpdate = TRUE;

	pev->deadflag = DEAD_DYING;

	deadSetActivityBlock = TRUE;
	
	//ensure the death activity we pick (or have picked) gets to run.
	signalActivityUpdate = TRUE;


	//easyPrintLine("ARE YOU SOME KIND OF insecure person??? %.2f %d", EASY_CVAR_GET(thoroughHitBoxUpdates), pev->deadflag );
	//MODDD
	if(EASY_CVAR_GET(thoroughHitBoxUpdates) == 1){
		//update the collision box now,
		this->SetObjectCollisionBox();
	}

	if(EASY_CVAR_GET(animationKilledBoundsRemoval) == 1){
		setPhysicalHitboxForDeath();
	}

}//END OF DeathAnimationStart


//MODDD TODO - would it be possible to use "SetSequenceBox" to set the bounds
//             in a way that guarantees the corpse is hittable?
void CBaseMonster::DeathAnimationEnd(){
	
	//do we need to force the frame to 255 in here?
	pev->frame = 255;
	this->m_fSequenceFinished = TRUE;  //forcing too?

	pev->deadflag = DEAD_DEAD;
	
	//MODDD - IMPORTANT NOTICE:  "SetThink" delinking MonsterThink has been re-delegated to "onDeathAnimationEnd".  The default case for all monsters does this, but some may override the method to omit it and still "think" on death, usually for planning a revival.
	//SetThink ( NULL );

	StopAnimation();

	//easyPrintLine("DEAD: boxFlat?   %d", (BBoxFlat()));

	if(EASY_CVAR_GET(animationKilledBoundsRemoval) == 2){
		setPhysicalHitboxForDeath();
	}

	//MODDD - bound-altering script for death moved to "setPhyiscalHitboxForDeath" for better through by a CVar.
				
	if ( ShouldFadeOnDeath() )
	{
		// this monster was created by a monstermaker... fade the corpse out.
		SUB_StartFadeOut();
	}
	else
	{
		// body is gonna be around for a while, so have it stink for a bit.
		// German robots still will if they are supposed to behave like normal.
		if(isOrganicLogic() ){
			CSoundEnt::InsertSound ( bits_SOUND_CARCASS, pev->origin, 384, 30 );
		}

	}

	//Something calling us may need to know not to try resetting the activity.
	signalActivityUpdate = FALSE;
	
	//an event.  But ONLY if not fading out.  At least in the default case,
	//it is up to other monsters whether they care about that, like auto-detonators (floaters).
	onDeathAnimationEnd();
}//END OF DeathAnimationEnd

void CBaseMonster::onDeathAnimationEnd(){
	//...nothing in the broadest case.
	//except that.
	if(!this->ShouldFadeOnDeath()){
		//Don't do it if I'm already fading out.  Let other monsters decide if they want to do something else
		//instead or depend on this at all.
		SetThink ( NULL );
	}
}

//MODDD - new.
void CBaseMonster::Activate( void ){

	CBaseToggle::Activate();


	//const char* nameCheat = STRING(pev->classname);

	/*
	if( monsterID == -1){
		monsterID = monsterIDLatest;
		monsterIDLatest++;
	}
	*/
	
	//setModelCustom();



	//MODDD - hold up. If below does effectively nothing with what's commented out (time of writing, er... typing), why bother?
	// Stop early.
	return;


	if(m_pCine == NULL && !FStringNull(pev->targetname) ){

		edict_t		*pEdicttt;
		CBaseEntity *pEntityyy;
		pEdicttt = g_engfuncs.pfnPEntityOfEntIndex( 1 );
		if ( pEdicttt ){
			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdicttt++ ){
				if ( pEdicttt->free )	// Not in use
				continue;
		
				pEntityyy = CBaseEntity::Instance(pEdicttt);
				if ( !pEntityyy )
					continue;

				if(pEntityyy->pev == this->pev){
					//same ent, move along..
					continue;
				}

				/*
				//NEVERMIND...  the m_pCine  on startup looking for its entity and verifying its own "wasAttached" as usuall still works just fine for either case (forgetting either the host ent or m_pCine that is).
				CBaseMonster* tempmon = NULL;
				if(  (tempmon = pEntityyy->GetMonsterPointer()) != NULL){

					if(FClassnameIs(pEntityyy->pev,"scripted_sequence") || FClassnameIs(pEntityyy->pev,"aiscripted_sequence")){ //|| FClassnameIs(pEntityyy->pev,"scripted_sentence")   ){
						CCineMonster* cineMon = (CCineMonster*) pEntityyy;
						if( cineMon->wasAttached >= 2 && FStrEq( STRING(cineMon->m_iszEntity), STRING(pev->targetname)) ){
							//YEAS, we have a match.
							m_pCine = cineMon;

							//pTarget->SetState(MONSTERSTATE_SCRIPT);
							m_MonsterState = MONSTERSTATE_SCRIPT;
							m_IdealMonsterState = MONSTERSTATE_SCRIPT;

							if(m_pCine->m_fMoveTo > 0){
								//pTarget->SetState(MONSTERSTATE_SCRIPT);
								m_hTargetEnt = m_pCine;    //I WILL DESTROY YOUR VERY WILL TO LIIIIIIIIIIIIIIVE
							}

						}
					}
				}
				*/
			}//END OF loop
		}//END OF pEdict check (first)

	}//END OF verify I may want a cine object.

}

void CBaseMonster::Spawn( ){

	//careful, not everything calls the parent spawn method, even if it should.
	//MonsterInit is a better place that's often called by most monster Spawn scripts. Beware of those that don't even do that.

	CBaseToggle::Spawn();

	//setModelCustom();
}




//MODDD - use "usesAdvancedAnimSystem()" to determine whether to use LookupActivityHard for this or just the ordinary LookupActivity.
int CBaseMonster::LookupActivityFiltered(int NewActivity){


	if(usesAdvancedAnimSystem()){

		//
		//return LookupActivityHard ( NewActivity );
		return tryActivitySubstitute ( NewActivity );
		

	}else{
		return LookupActivity ( NewActivity );
	}
}//END OF LookupActivityFiltered



//Name is kind of odd.  ACTUALLY gets the (or an) animation (by integer?) related to this activity.
int CBaseMonster :: LookupActivity(int activity )
{
	if( !usesAdvancedAnimSystem()){
		//not using the advanced anim? Nothing special.
	}else{
		int animationTry = tryActivitySubstitute(activity);
		
		//Nevermind this check, return regardless. We assume tryActivitySubstitute already tried using the base model (CBaseAnimating::LookupActivity) as a last resort, so no need to try that again.
		//if(animationTry != ACTIVITY_NOT_AVAILABLE){
			//EASY_CVAR_PRINTIF_PRE(hassaultPrintout, easyPrintLine( "PANTHEREYE: LOOKUP ACT %d : %d", activity, animationTry));
			return animationTry;
		//}
	}
	return CBaseAnimating::LookupActivity(activity);
}
int CBaseMonster :: LookupActivityHeaviest(int activity )
{
	if( !usesAdvancedAnimSystem()){
		//not using the advanced anim? Nothing special.
	}else{
		int animationTry = tryActivitySubstitute(activity);
		
		//if(animationTry != ACTIVITY_NOT_AVAILABLE){
			//EASY_CVAR_PRINTIF_PRE(hassaultPrintout, easyPrintLine( "PANTHEREYE: LOOKUP ACTHEAV %d : %d", activity, animationTry));
			return animationTry;
		//}
	}
	return CBaseAnimating::LookupActivityHeaviest(activity);
}


//NOTICE - the methods "LookupActivityHard" and "tryActivitySubstitute" are meant to be implemented by other monsters that choose to hardcode
//         sequence choices for activities (LookupActivityHard) or indications of having activities but not adjusting themselves just from
//         such a check (tryActivitySubstitute). Their base implementations in CBaseAnimating for everything else, no different from LookupActivity,
//         are just fine.
//         Monsters no longer need to implement "LookupActivity" or "LookupActivityHeaviest", the as-is retail activity-sequence methods, since
//         the new version above for CBaseMonster already does the "usesAdvancedAnimSystem()" check. That must still be done to take advantage of
//         LookupActivityHard and tryActivitySubstitute implementations / sequence injections.







//MODDD - new method for determining whether to register a case of damage as worthy of LIGHT_DAMAGE or HEAVY_DAMAGE for the AI.
//        Can result in interrupting the current schedule.
//        This is expected to get called from CBaseMonster's TakeDamage method in combat.cpp. This method may be customized per monster,
//        should start with a copy of this method without calling the parent method, not much here.
void CBaseMonster::OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod){

	//MODDD - intervention. Timed damage might not affect the AI since it could get needlessly distracting.
	if(bitsDamageTypeMod & (DMG_TIMEDEFFECT|DMG_TIMEDEFFECTIGNORE) ){
		//If this is continual timed damage, don't register as any damage condition. Not worth possibly interrupting the AI.
		return;
	}

	//default case from CBaseMonster's TakeDamage.
	//Also count being in a non-combat state to force looking in that direction.  But maybe at least 0 damage should be a requirement too, even in cases where the minimum damage for LIGHT is above 0?
	if (m_MonsterState == MONSTERSTATE_IDLE || m_MonsterState == MONSTERSTATE_ALERT || flDamage > 0 )
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
	if(gpGlobals->time >= forgetBigFlinchTime && (flDamage >=  pev->max_health * 0.55 || flDamage >= 30) )
	{
		SetConditions(bits_COND_HEAVY_DAMAGE);
		forgetSmallFlinchTime = gpGlobals->time + DEFAULT_FORGET_SMALL_FLINCH_TIME;
		forgetBigFlinchTime = gpGlobals->time + DEFAULT_FORGET_BIG_FLINCH_TIME;
	}


/*
	if(EASY_CVAR_GET(testVar) == 10){
		//any damage causes me now.
		SetConditions(bits_COND_HEAVY_DAMAGE);
	}
*/

	//easyForcePrintLine("%s:%d OnTkDmgSetCond raw:%.2f fract:%.2f", getClassname(), monsterID, flDamage, (flDamage / pev->max_health));



}//END OF OnTakeDamageSetConditions





void CBaseMonster::ForgetEnemy(void) {

	m_afMemory &= ~(bits_MEMORY_SUSPICIOUS | bits_MEMORY_PROVOKED);

	if (m_hEnemy != NULL) {
		m_hEnemy = NULL;
		m_hTargetEnt = NULL;
		if (this->m_MonsterState == MONSTERSTATE_COMBAT) {
			// doesn't make sense to be in COMBAT without an enemy.
			this->m_MonsterState = MONSTERSTATE_ALERT;
		}
		TaskFail();
	}

}//END OF ForgetEnemy

//by default, does nothing. Used for the Kingpin to let a monster remove itself from the Kingpin's command list of entities it powered up and can order to attack its enemy.
void CBaseMonster::removeFromPoweredUpCommandList(CBaseMonster* argToRemove){

}


void CBaseMonster::forceNewEnemy(CBaseEntity* argIssuing, CBaseEntity* argNewEnemy, BOOL argPassive){
	//Things that expect to have this called should implement this better. When to change schedules, etc.

	if(m_hEnemy != argNewEnemy){
		m_hEnemy = argNewEnemy;
		SetState(MONSTERSTATE_COMBAT);
		if(!argPassive){
			//ChangeSchedule(GetSchedule());
			TaskFail();
		}
	}

}//END OF forceNewEnemy

//dummied by default
void CBaseMonster::setPoweredUpOff(void){}
void CBaseMonster::setPoweredUpOn(CBaseMonster* argPoweredUpCauseEnt, float argHowLong ){}
// Not to be confused with "ForgetEnemy" further above.
// This is for forgetting an enemy assigned by some other entity.
void CBaseMonster::forgetForcedEnemy(CBaseMonster* argIssuing, BOOL argPassive){};




//Go revert islave to its backup for "riseFromTheGrave", this is not so great for it anymore.
//Maybe it's still ok? verify...
void CBaseMonster::startReanimation(){
	int i;
	
	pev->deadflag = DEAD_NO;

	//!!!
	deadSetActivityBlock = FALSE;
	

	//before spawn or init script may interfere.
	int oldSeq = pev->sequence;

	//no recollection of that.
	m_hEnemy = NULL;

	//And clear the list of old enemies.
	//or m_intOldEnemyNextIndex - 1 ?
	for(i = 0; i < MAX_OLD_ENEMIES; i++){
		m_hOldEnemy[i] = NULL;
	}

				
				
	/*
	//!!!

	UTIL_SetSize(pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX);

	//pev->classname = MAKE_STRING("monster_alien_slave");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_STEP;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->effects		= 0;

	//The outside has to figure this out??
	//pev->health			= gSkillData.slaveHealth;
	pev->health			= pev->max_health;

	pev->view_ofs		= Vector ( 0, 0, 64 );// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE; // NOTE: we need a wide field of view so npc will notice player and say hello
	m_MonsterState		= MONSTERSTATE_NONE;
	m_afCapability		= bits_CAP_HEAR | bits_CAP_TURN_HEAD | bits_CAP_RANGE_ATTACK2 | bits_CAP_DOORS_GROUP;

	
	//as to not cause this mismatch to change our desired revive animation.
	
	
	*/

	//This should set the monster's health to "pev->max_health".
	//And assume it calls "MonsterInit" again if it ever did before.
	Spawn();








	//Most of MonsterInit's script here just to be safe... Nah, assume Spawn calls it if it makes sense to.
	
	/*
	// Set fields common to all monsters
	pev->effects		= 0;
	pev->takedamage		= DAMAGE_AIM;
	pev->ideal_yaw		= pev->angles.y;
	//pev->max_health		= pev->health;    //Better already know that...
	pev->deadflag		= DEAD_NO;
	m_IdealMonsterState	= MONSTERSTATE_IDLE;// Assume monster will be idle, until proven otherwise
	//!!!
	m_MonsterState = MONSTERSTATE_IDLE;


	m_IdealActivity = ACT_IDLE;
	//!!! No sequence changing, force the activity to this now.
	m_Activity = ACT_IDLE;

	SetBits (pev->flags, FL_MONSTER);
	if ( pev->spawnflags & SF_MONSTER_HITMONSTERCLIP )
		pev->flags |= FL_MONSTERCLIP;
	
	ClearSchedule();
	RouteClear();
	InitBoneControllers( ); // FIX: should be done in Spawn

	m_iHintNode			= NO_NODE;

	m_afMemory			= MEMORY_CLEAR;

	m_hEnemy			= NULL;

	m_flDistTooFar		= 1024.0;
	m_flDistLook		= 2048.0;

	//MODDD - flag for mirror recognition.
	pev->renderfx |= ISNPC;


	//if(EASY_CVAR_GET(monsterSpawnPrintout) == 1){
	//	easyPrintLine("I SPAWNED::: %s flags:%d", STRING(pev->classname), pev->spawnflags);
	//}

	//m_MonsterState = ::MONSTERSTATE_IDLE;
	//m_IdealMonsterState = ::MONSTERSTATE_IDLE;

	// set eye position
	//SetEyePosition();
	*/





	EndOfRevive(oldSeq);
	







}//END OF startReanimation





//Overridable method. What to do when this monster is at the end of revival from startReanimation above.
//Override to let a monster determine how to revive the monster on its own, like pick a different animation.
//Default behavior is to play the existing animation over again.
void CBaseMonster::EndOfRevive(int preReviveSequence){

	

	m_IdealMonsterState	= MONSTERSTATE_ALERT;// Assume monster will be alert, having come back from the dead and all.
	m_MonsterState = MONSTERSTATE_ALERT; //!!!

	m_IdealActivity = ACT_IDLE;
	m_Activity = ACT_IDLE; //!!! No sequence changing, force the activity to this now.

	pev->sequence = -1; //force reset.
	SetSequenceByIndex(preReviveSequence, -1, FALSE);

	ChangeSchedule(slWaitForReviveSequence );
}//END OF EndOfRevive


//Overridable. Whether a monster wants to turn to face a node up to so many degrees (such as +- 45 degrees before going back to walking or running).
// as this is plus or minus already, negative values make no sense. Negative 1 allows all angles as well.
//Monsters that can strafe like hgrunts or flyers may want to override this to depend on some other factor, like being mid-strafe or flight or not.
float CBaseMonster::MoveYawDegreeTolerance(){
	//return -1;
	return 35;
}


//Shortcut to UTIL_BloodColorRedFilter that lets a monster provide its own german model requirement CVar automatically.
int CBaseMonster::BloodColorRedFilter(){
	return UTIL_BloodColorRedFilter(getGermanModelRequirement()==1);
}

int CBaseMonster::CanUseGermanModel(){
	//If we can use our german model and found ours
	return (getGermanModelsAllowed() && getGermanModelRequirement()==1 );
}


//Try to find the earliest node that is marked GOAL. Sometimes this gets shifted around from 0
WayPoint_t* CBaseMonster::GetGoalNode(){
	for(int i = 0; i <= m_iRouteIndex; i++){
		if(m_Route[i].iType & bits_MF_IS_GOAL){
			//it is this.
			return &m_Route[i];
		}
	}
	//didn't find a node with bits_MF_IS_GOAL set from 0 to m_iRouteIndex?
	return NULL;
}//END OF getGoalNode

void CBaseMonster::ReportGeneric(){
	CBaseEntity::ReportGeneric();

	//Also call our own special report method: ReportAI.
	ReportAIState();

}//END OF ReportGeneric()






//MODDD - off for most.  Some new NPCs may use this (or old could be made to).
BOOL CBaseMonster::hasSeeEnemyFix(void){
	return FALSE;
}

//Can this monster accept a new enemy, regardless of whether or not the current schedule is interruptible by "cond_NEW_ENEMY" ?
BOOL CBaseMonster::getForceAllowNewEnemy(CBaseEntity* pOther){
	return FALSE;
}


void CBaseMonster::tempMethod(void){
	//by default nothing. Use me for testing with client calls
	//(point at this monster ingame, crosshairs, and type some command that calls this one's tempMethod)
}//END OF tempMethod




//Monsters with bigger bounds may need them to be temporarily reduced to play better with the pathfiding.
//Side effects not well understood yet, see if this is really worth it.
//By default off, enable per monsters as needed.
BOOL CBaseMonster::needsMovementBoundFix(){

	return FALSE;
}//END OF needsMovementBoundFix


void CBaseMonster::cheapKilled(void){
	this->m_IdealMonsterState = MONSTERSTATE_DEAD;
	//MODDD - major HACK - pathetic stand-in death anim until there is a proper death anim.
	this->pev->gravity = 0;
	pev->movetype		= MOVETYPE_NONE;
	this->pev->origin = pev->origin + Vector(0, 0, 13.4);   //so it isn't poking through the ground. Yes this will look really weird.
	this->pev->angles = Vector(0, 0, 90);
	this->pev->framerate = 0;
	this->m_flFramerateSuggestion = 0;
	this->m_flFrameRate = 0;

	//this->pev->frame = 255;
	//this->m_fSequenceFinished = TRUE;


	DeathAnimationStart();
	DeathAnimationEnd();
}//END OF cheapKilled


//Version of cheapKilled for flying monsters. They should still drop to the ground as expected, not akwardly freeze in mid-air.  Or maybe they should? dunno.
void CBaseMonster::cheapKilledFlyer(void){
	this->m_IdealMonsterState = MONSTERSTATE_DEAD;
	//MODDD - major HACK - pathetic stand-in death anim until there is a proper death anim.
	//this->pev->gravity = 0;

	//Not working out so great anymore.  okay then..
	//pev->movetype		= MOVETYPE_STEP;

	//this->pev->origin = pev->origin + Vector(0, 0, 13.4);   //so it isn't poking through the ground. Yes this will look really weird.
	
	//Copied from Flyer's killed script
	ClearBits( pev->flags, FL_ONGROUND );
	//pev->angles.z = 0;
	//pev->angles.x = 0;

	this->pev->angles = Vector(0, 0, 90);
	this->pev->framerate = 0;
	this->m_flFramerateSuggestion = 0;
	this->m_flFrameRate = 0;

	//this->pev->frame = 255;
	//this->m_fSequenceFinished = TRUE;


	DeathAnimationStart();
	DeathAnimationEnd();
}//END OF cheapKilledFlyer


// When killed, how do we handle the "Touch" callback method from the engine?
// Default says to set it to NULL. Override this for different behavior.
// i.e., flyers / hover-ers can tell this to interrupt a falling cycler animation on colliding with anything (the ground?).
// In that case, default behavior of turning touch off would leave you scratching your head as to why it ignores everything after killed.
// HOWEVER the new slDieFallLoop schedule include setting Touch to KilledFinishTouch which works for all monsters.  This method (OnKilledSetTouch) may no longer be necessary.
void CBaseMonster::OnKilledSetTouch(void){
	SetTouch(NULL);
}

// I need to finish the looping death animation with the "OnKilledSetTouch" one.
void CBaseMonster::KilledFinishTouch( CBaseEntity *pOther ){
	if(pOther == NULL){
		return; //??????
	}
	
	//hitGroundDead = TRUE;
	
	// old was "slDieLoop".
	// Do we even need the schedule + task check?  Touch anything, complete my task.
	//if(m_pSchedule == slDieFallLoop && this->getTaskNumber() == TASK_DIE_LOOP){
		TaskComplete();
		SetTouch(NULL); //don't need to do this again.
	//}

}//END OF KilledFinishTouch



// Makes the most sense for flyers to have looping falling animations at deaths in air.
// By default, -1 means none.
// This is called for by the 
int CBaseMonster::getLoopingDeathSequence(void){
	return -1;
}


// Depending on the value of CVar flyerKilledFallingLoop, pick the schedule that uses the looping animation or don't.
Schedule_t* CBaseMonster::flyerDeathSchedule(void){
	
	
	if(EASY_CVAR_GET(flyerKilledFallingLoop) == 1){

		//First a check. If we are very close or virtually on the ground, just skip to the typical die method to do the "hitting the ground" animation.

		TraceResult tr;
		Vector vecStart = pev->origin + Vector(0, 0, pev->mins.z);

		UTIL_TraceLine(vecStart, vecStart + Vector(0, 0, -6), dont_ignore_monsters, this->edict(), &tr);

		if(tr.fStartSolid || tr.fAllSolid || tr.flFraction < 1.0){
			//go to "slDie" below.
		}else{
			//clear? send this.
			return slDieFallLoop;
		}

	}//END OF flyerKilledFallingLoop check



	//default
	return slDie;
}

// Can this monster automatically turn to face a node better in ::Move above?
// Turn off for things that may play with direction faced (like strafing hgrunts while they are expected to strafe)
BOOL CBaseMonster::getMovementCanAutoTurn(void){
	//without a reason not to, defaults to yes all the time.
	return TRUE;
}//END OF getMovementCanAutoTurn



// If there is an enemy, set the m_vecEnemyLKP to the enemy's current position
void CBaseMonster::updateEnemyLKP(void){
	if(m_hEnemy != NULL){
		m_vecEnemyLKP = m_hEnemy->pev->origin;
		investigatingAltLKP = FALSE; //this is the real deal.
	}
}
void CBaseMonster::setEnemyLKP(const Vector& argNewVector){
	m_vecEnemyLKP = argNewVector;
	investigatingAltLKP = FALSE; //this is the real deal.
}//END OF setEnemyLKP

// Push this new vector to m_vecEnemyLKP.
// Save the old enemy LKP to another var to back up to when done with that.
void CBaseMonster::setEnemyLKP_Investigate(const Vector& argToInvestigate){
	m_vecEnemyLKP_Real = m_vecEnemyLKP;
	m_vecEnemyLKP = argToInvestigate;
	investigatingAltLKP = TRUE;
}







//MODDD
// Moved to the base monster for usefullness.  Probably needs to be overridden per monster.
// More of a special utility per monster, bound not to really be salvagable on its own here.
CBaseEntity* CBaseMonster::getNearestDeadBody(const Vector& arg_searchOrigin, const float arg_maxDist){

	CBaseEntity* pEntityScan = NULL;
	CBaseMonster* testMon = NULL;
	float thisDistance;
	CBaseEntity* bestChoiceYet = NULL;
	float leastDistanceYet = arg_maxDist; //furthest I go to a dead body.

	//does UTIL_MonstersInSphere work?
	while ((pEntityScan = UTIL_FindEntityInSphere( pEntityScan, arg_searchOrigin, 800 )) != NULL)
	{
		if(pEntityScan->pev == this->pev){
			//is it me? skip it.
			continue;
		}

		testMon = pEntityScan->MyMonsterPointer();
		//if(testMon != NULL && testMon->pev != this->pev && ( FClassnameIs(testMon->pev, "monster_scientist") || FClassnameIs(testMon->pev, "monster_barney")  ) ){

		//MODDD TODO - should this be able to be fooled by things playing dead?
		//             Use "testMon->IsAlive_FromAI()" to be fool-able instead of straight deadflag checks.  But the player one (FL_CLIENT check first) is still fine.
		

		//Don't try to eat leeches, too tiny. Nothing else this small leaves a corpse.
		if(testMon != NULL && (testMon->pev->deadflag == DEAD_DEAD || ( (testMon->pev->flags & (FL_CLIENT)) && testMon->pev->deadflag == DEAD_RESPAWNABLE && !(testMon->pev->effects &EF_NODRAW) ) ) && testMon->isSizeGiant() == FALSE && testMon->isOrganicLogic() && !(::FClassnameIs(testMon->pev, "monster_leech") ) ){
			thisDistance = (testMon->pev->origin - arg_searchOrigin).Length();
			
			if(thisDistance < leastDistanceYet){
				//WAIT, one more check. Look nearby for players.

				//UTIL_TraceLine ( node.m_vecOrigin + vecViewOffset, vecLookersOffset, ignore_monsters, ignore_glass,  ENT(pev), &tr );
				
				bestChoiceYet = testMon;
				leastDistanceYet = thisDistance;
				
			}//END OF minimum distance yet
		}//END OF entity scan null check

	}//END OF while loop through all entities in an area to see which are corpses.
	return bestChoiceYet;

}//END OF getNearestDeadBody

// Whether to do the usual "Look" method for checking for new or existing enemies.
// Needed by archers to be able to call "Look" despite the player never going underwater (causes the PVS check to pass at least once to start combat).
BOOL CBaseMonster::noncombat_Look_ignores_PVS_check(void){
	return FALSE;
}//END OF noncombat_Look_ignores_PVS_check

// Implement for monsters that should spawn regardless of the "mp_allowmonsters" CVar.
// Pickup walkers are a clear example since they're needed for some weapons intended
// by the map.
BOOL CBaseMonster::bypassAllowMonstersSpawnCheck(void){
	return FALSE;
}//END OF bypassAllowMonstersSpawnCheck



// It is up to an individual monster with a violent death sequence, regardless of whether it's mapped to activity ACT_DIEVIOLENT or not, to
// override this and say "TRUE", along with providing its own rule for a clear distance check.  That is, if whatevre distance forwards/backwards
// (typically backwards) is safe.  But it varries from monster to monster's sequence, just have to make it so the sequence can't clip through anything
// and look really weird.
// It is also OK if the sequence for violent death is not tied to ACT_DIEVIOLENT in the model. Then pick it for ACT_DIEVIOLENT in the custom animation system instead.
BOOL CBaseMonster::violentDeathAllowed(void){
	return FALSE;
}//END OF hasViolentDeathSequence

// Default case should work fine for most monsters.
// Only allow a violent death animation if the last hit solidly did this much damage.
// Could do checks on m_LastHitGroup too (see method GetDeathActivity of combat.cpp)
BOOL CBaseMonster::violentDeathDamageRequirement(void){
	return (lastDamageReceived >= 20);
}

// This method MUST be overridden to do line traces forwards / backwards to see if there is enough space in whatever direction to play the animation
// to avoid clipping through things.  If this is unnecessary, leave this as it is.  But "hasViolentDeathSequence" must  be overridden to return TRUE;
// above regardless.
BOOL CBaseMonster::violentDeathClear(void){
	return TRUE;
}//END OF violentDeathClear

//MODDD - the priority that ACT_DIEVIOLENT gets compared to other activities.  That is, even if the conditions for a violent death act are met or could
//        be met if checked, what activities should get picked first anyways if their criteria matches at the same time?  Only applies if "violentDeathAllowed" is TRUE.
//Modes:
//1: top. violent death happens if the conditions are met period above all other death activities.
//2: mid. violent death happens before (priority over) any directional death checks (forwards and backwards), but after (loses to) the model hitbox check ones
//   (ACT_DIE_HEADSHOT and ACT_DIE_GUTSHOT)
//3: low. violent death happens only during the last hit being on the front of the enemy to compete with ACT_DIEBACKWARD, which it takes precedence over if so.
//   Otherwise, all other death acts get priority.
int CBaseMonster::violentDeathPriority(void){
	return 3;
}//END OF violentDeathPriority


// a utility for checking to see if behind a monster by up to "argDistance" is completely unobstructed and is of the same ground level as this entity.
// That way a violent death anim can't work when there's a cliff backwards. Does not know to fall as the model's movement is just part of the animation.
BOOL CBaseMonster::violentDeathClear_BackwardsCheck(float argDistance){
	TraceResult tr;
	Vector vecStart;
	Vector vecEnd;
	Vector vecGroundEnd;

	UTIL_MakeVectors ( pev->angles );
	
	vecStart = Center();
	vecEnd = vecStart - gpGlobals->v_forward * argDistance;
	vecGroundEnd = (pev->origin + Vector(0, 0, pev->mins.z) ) - gpGlobals->v_forward * argDistance;

	
	
	UTIL_TraceHull ( vecStart, vecEnd, dont_ignore_monsters, head_hull, edict(), &tr );
	
	







	// Nothing in the way? it's good.
	if ( tr.flFraction == 1.0 ){
		//return TRUE;
		//not yet, there's another hoop to go through.
	}else{
		return FALSE;
	}

	//Pretty good distance. would that make us fall either? Is there ground at this place "vecEnd"?
	//UTIL_TraceLine or UTIL_TraceLine?
	UTIL_TraceLine ( vecGroundEnd, vecGroundEnd + Vector(0, 0, -4), dont_ignore_monsters, edict(), &tr );
	float zDelta = abs(tr.vecEndPos.z - pev->origin.z);
	if(!tr.fStartSolid && !tr.fAllSolid && tr.flFraction < 1.0 && zDelta < 3){
		//it is good!
		return TRUE;
	}
	
	return FALSE;
}//END OF violentDeathClear_BackwardsCheck


void CBaseMonster::lookAtEnemyLKP(void){
					
	MakeIdealYaw ( m_vecEnemyLKP );
	ChangeYaw ( pev->yaw_speed );
}

//Is this montser able to predict re-picking the same activity in the next frame?
//If so, and the conditions for it are met, this can be set to do so.
//BUT not all monsters play nicely with this. Some fidget for a moment on the same animation for a frame and stop
//if it does not get picked.
void CBaseMonster::predictActRepeat(int arg_bits_cond){
	if(HasConditions(arg_bits_cond)){
		m_Activity = ACT_RESET;
	}
}

//Am I able to use the "predictActRepeat" check above at all? Defaults to TRUE, set differently when problematic.
BOOL CBaseMonster::canPredictActRepeat(void){
	return TRUE;
}//END OF canPredictActRepeat





//MODDD - came from hgrunt.cpp, used to be named Kick. Now callable by other monsters too.
//        This is probably some old version of checking for entities in front before it was standardized to
//        CheckTraceHullAttack.  But whatever, this could even indirectly call that too instead.

CBaseEntity* CBaseMonster:: HumanKick( void ){
	return HumanKick(70);  //default.
}

CBaseEntity* CBaseMonster:: HumanKick(float argCheckDistance ){
	TraceResult tr;

	UTIL_MakeVectors( pev->angles );
	Vector vecStart = pev->origin;
	vecStart.z += pev->size.z * 0.5;
	Vector vecEnd = vecStart + (gpGlobals->v_forward * argCheckDistance);

	UTIL_TraceHull( vecStart, vecEnd, dont_ignore_monsters, head_hull, ENT(pev), &tr );

	if ( tr.pHit )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( tr.pHit );

		return pEntity;
	}

	return NULL;
}



void CBaseMonster::precacheStandardMeleeAttackMissSounds(void){
	PRECACHE_SOUND_ARRAY(pStandardAttackMissSounds);
}
void CBaseMonster::precacheStandardMeleeAttackHitSounds(void){
	PRECACHE_SOUND_ARRAY(pStandardAttackHitSounds);
}

void CBaseMonster::playStandardMeleeAttackMissSound(void){
	EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pStandardAttackMissSounds), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
}
void CBaseMonster::playStandardMeleeAttackHitSound(void){
	EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, RANDOM_SOUND_ARRAY(pStandardAttackHitSounds), 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
}

BOOL CBaseMonster::traceResultObstructionValidForAttack(const TraceResult& arg_tr){
	//Given a TraceResult that has already been written to by some trace method,
	//did anything block the trace and, if so, was it something I can live with attacking?
	//IMPORTANT - does not include fStartSolid and fAllSolid checks, handle those separately.

	if(arg_tr.flFraction < 1.0f && arg_tr.pHit != NULL){
		//what is blocking me?
		//CBaseEntity* tempEnt = CBaseEntity::Instance(trTemp.pHit);

		const char* daName = STRING(arg_tr.pHit->v.classname);

		if(m_hEnemy != NULL && arg_tr.pHit == m_hEnemy.Get()){
			//has an enemy, matches it? this is acceptable.
		}else{
			//anything else blocking? wait... maybe I hate it anyways.
			CBaseEntity* tempEnt = CBaseEntity::Instance(arg_tr.pHit);
			if(this->IRelationship(tempEnt) > R_NO){
				//horray, I hate it anyways.
			}else{
				//let's not.
				return FALSE;
			}
		}
	}//END OF fraction and thing-hit checks.


	return TRUE;
}//END OF traceResultObstructionValidForAttack


//default, override if necessary.
float CBaseMonster::getDistTooFar(void){
	return 1024.0;
}
//default, override if necessary.
float CBaseMonster::getDistLook(void){
	return 2048.0;
}


