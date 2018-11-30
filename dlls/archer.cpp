
#include "archer.h"

#include "schedule.h"
#include "activity.h"
#include "animation.h"

#include "defaultai.h"
#include "soundent.h"
#include "game.h"

#include "custom_debug.h"

#include "squidspit.h"
#include "weapons.h"

/*
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
*/



//TODO MAJOR - is it a problem if the death sequence replies on loading a save when floating to the top is blocked by something?
//             probably not, and this is kinda easily fixable if so.



EASY_CVAR_EXTERN(noFlinchOnHard)
EASY_CVAR_EXTERN(animationFramerateMulti)

EASY_CVAR_EXTERN(drawDebugPathfinding)
EASY_CVAR_EXTERN(drawDebugPathfinding2)


EASY_CVAR_EXTERN(STUrepelMulti)
EASY_CVAR_EXTERN(STUcheckDistV)
EASY_CVAR_EXTERN(STUcheckDistH)
EASY_CVAR_EXTERN(STUcheckDistD)
EASY_CVAR_EXTERN(STUSpeedMulti)



/*
//TODO. spawn balls of death as ranaged attack.
//TODO. Start fall cylcer at death, use animation for hitting the ground on touching the ground (possible?).

//what. is this default behavior?
void CController::Stop( void ) 
{ 
	m_IdealActivity = GetStoppedActivity(); 
}
*/



//TODO - archer needs to be able to do a check to see if the enemy is past the waterlevel.
//       If so, it can try to move straight up, towards the enemy or a blend. Just route to the surface where there is empty
//       space so it makes sense to rise above the water there.  Don't goto a spot if some platform or dock is in the way.
//       Then it can do a ranged sea-land attack however many times and submerge, even be interrupted to submerge sooner
//       or more urgently on taking damage?

//       CheckLocalMove needs to see if the destination is towards a different waterlevel than the source.
//       (or really just not underwater at all, archers should never move anywhere outside of water)
//       But maybe make it a little less sensitive when going above the water slightly to do a sea-land attack.

//       Finally when itself and the enemy are both underwater, it really acts like an underwater bullsquid.
//       Use ranged projectiles if possible, and follow the enemy otherwise. Can melee when close enough.

//       QUESTION: there are sequences sink, dead_float, die1 and die2.  Does one of the "die"s lead to floating to the top (die2?) and the other sinking (die1)?

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//sequences in the model. Some sequences have the same display name and so should just be referenced by order
//(numbered index).
enum archer_sequence{  //key: frames, FPS
	SEQ_ARCHER_IDLE1,
	SEQ_ARCHER_IDLE2,
	SEQ_ARCHER_IDLE3,
	SEQ_ARCHER_BURST_SMALL,
	SEQ_ARCHER_SWIM,
	SEQ_ARCHER_SWIM_FAST,
	SEQ_ARCHER_BITE,
	SEQ_ARCHER_FLINCH1,
	SEQ_ARCHER_FLINCH2,
	SEQ_ARCHER_DIE1,
	SEQ_ARCHER_DIE2,
	SEQ_ARCHER_SHOOT,
	SEQ_ARCHER_SURFACE,
	SEQ_ARCHER_SINK,
	SEQ_ARCHER_DEAD_FLOAT,
	SEQ_ARCHER_180_LEFT,
	SEQ_ARCHER_180_RIGHT

};


//custom schedules
enum{
	SCHED_ARCHER_RANGE_ATTACK = LAST_COMMON_SCHEDULE + 1,


};

//custom tasks
enum{
	TASK_ARCHER_XXX = LAST_COMMON_TASK + 1,
	

};







//Original archer sound methods for reference. Just zombie sounds though, still placeholders.
/*

void CBloater :: PainSound( void )
{
#if 0	
	int pitch = 95 + RANDOM_LONG(0,9);

	switch (RANDOM_LONG(0,5))
	{
	case 0: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_pain1.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 1:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_pain2.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	default:
		break;
	}
#endif
}

void CBloater :: AlertSound( void )
{
#if 0
	int pitch = 95 + RANDOM_LONG(0,9);

	switch (RANDOM_LONG(0,2))
	{
	case 0: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_alert10.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 1:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_alert20.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 2:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_alert30.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	}
#endif
}

void CBloater :: IdleSound( void )
{
#if 0
	int pitch = 95 + RANDOM_LONG(0,9);

	switch (RANDOM_LONG(0,2))
	{
	case 0: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_idle1.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 1:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_idle2.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 2:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_idle3.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	}
#endif
}

void CBloater :: AttackSnd( void )
{
#if 0
	int pitch = 95 + RANDOM_LONG(0,9);

	switch (RANDOM_LONG(0,1))
	{
	case 0: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_attack1.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	case 1:
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "zombie/zo_attack2.wav", 1.0, ATTN_NORM, 0, pitch);
		break;
	}
#endif
}

*/



const char* CArcher::pDeathSounds[] = 
{
	"archer/archer_death.wav",
};
const char* CArcher::pAlertSounds[] = 
{
	"archer/archer_alert.wav",
};

const char* CArcher::pIdleSounds[] = 
{
	"archer/archer_idle.wav",
};
const char* CArcher::pPainSounds[] = 
{
	"archer/archer_pain.wav",
};
const char* CArcher::pAttackSounds[] = 
{
	"archer/archer_attack.wav",
};
const char* CArcher::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};
const char* CArcher::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};






/*
TYPEDESCRIPTION	CArcher::m_SaveData[] = 
{
	DEFINE_FIELD( CArcher, ????, FIELD_TIME ),
};

//IMPLEMENT_SAVERESTORE( CArcher, CFlyingMonster );
int CArcher::Save( CSave &save )
{
	if ( !CFlyingMonster::Save(save) )
		return 0;
	int iWriteFieldsResult = save.WriteFields( "CArcher", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iWriteFieldsResult;
}
int CArcher::Restore( CRestore &restore )
{
	if ( !CFlyingMonster::Restore(restore) )
		return 0;
	int iReadFieldsResult = restore.ReadFields( "CArcher", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iReadFieldsResult;
}
*/




#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_archer, CArcher );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( archer, CArcher );
	
	#if EXTRA_NAMES == 2
		//none?
	#endif
	
#endif



CArcher::CArcher(void){

	shootCooldown = 0;

	m_flightSpeed = 0;
	tempCheckTraceLineBlock = FALSE;
	m_velocity = Vector(0,0,0);

	lastVelocityChange = -1;
	
	hitGroundDead = FALSE;

}//END OF CArcher constructor




//Thank you bullsquid, you know what is up dog.
Task_t	tlArcherRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	
	//No, letting the end of TASK_RANGE_ATTACK1 handle this on our end instead.
	//We need the activity shift to idle to be instant.
	//{ TASK_SET_ACTIVITY,		(float)ACT_IDLE	},

};
Schedule_t	slArcherRangeAttack1[] =
{
	{ 
		tlArcherRangeAttack1,
		ARRAYSIZE ( tlArcherRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		//bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED,   //er, wat?
		0,
		"Archer Range Attack1"
	},
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






DEFINE_CUSTOM_SCHEDULES( CArcher )
{
	slArcherRangeAttack1,
	//slArcherYYY,
	//slArcherZZZ,

};
IMPLEMENT_CUSTOM_SCHEDULES( CArcher, CFlyingMonster );

	









	
void CArcher::DeathSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1.0, ATTN_IDLE, 0, pitch );
}
void CArcher::AlertSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAlertSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CArcher::IdleSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random idle sound
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CArcher::PainSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	if (RANDOM_LONG(0,5) < 2){
		EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1.0, ATTN_NORM, 0, pitch );
	}
}
void CArcher::AttackSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random attack sound
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAttackSounds), 1.0, ATTN_NORM, 0, pitch );
}















extern int global_useSentenceSave;
void CArcher::Precache( void )
{
	PRECACHE_MODEL("models/archer.mdl");

	
	//sprite precache left to the SquidSpit file (separate).
	CSquidSpit::precacheStatic();

	global_useSentenceSave = TRUE;
	
	
	//NOTICE - attempting to precace files that don't exist crashes the game.
	/*
	//PRECACHE_SOUND("archer/archer_XXX.wav");
	PRECACHE_SOUND_ARRAY(pDeathSounds);
	PRECACHE_SOUND_ARRAY(pAlertSounds);
	PRECACHE_SOUND_ARRAY(pIdleSounds);
	PRECACHE_SOUND_ARRAY(pPainSounds);
	PRECACHE_SOUND_ARRAY(pAttackSounds);
	PRECACHE_SOUND_ARRAY(pAttackHitSounds);
	PRECACHE_SOUND_ARRAY(pAttackMissSounds);
	*/



	global_useSentenceSave = FALSE;
}//END OF Precache()



void CArcher::Spawn( void )
{
	Precache( );


	setModel("models/archer.mdl");
	//UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );
	UTIL_SetSize( pev, Vector( -9, -9, 2 ), Vector( 9, 9, 48 ));

	pev->classname = MAKE_STRING("monster_archer");

	//I am underwater.
	pev->flags |= FL_SWIM;



	
	pev->solid			= SOLID_BBOX;  //not SOLID_SLIDEBOX
	pev->movetype		= MOVETYPE_FLY;


	pev->solid			= SOLID_SLIDEBOX;  //SOLID_TRIGGER?  Difference?
	//pev->movetype		= MOVETYPE_BOUNCEMISSILE;
	


	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->effects		= 0;
	//NOTE - you have to make this exist over in skill.h and handle some other setup (gamerules.cpp, and the CVar in game.cpp)!
	//example: skilldata_t member "scientistHealth" and CVar "sk_scientist_health".
	pev->health			= gSkillData.archerHealth;
	pev->view_ofs		= VEC_VIEW;/// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	pev->yaw_speed		= 100;//bound to change often from "SetYawSpeed". Likely meaningless here but a default can't hurt.

	MonsterInit();


	m_flightSpeed = 400;

	SetTouch(&CArcher::CustomTouch );
	//SetTouch( NULL );


}//END OF Spawn();





























	



int CArcher :: CheckLocalMove ( const Vector &vecStart, const Vector &vecEnd, CBaseEntity *pTarget, float *pflDist )
{
	int iReturn;

	/*
	// UNDONE: need to check more than the endpoint
	if (FBitSet(pev->flags, FL_SWIM) && (UTIL_PointContents(vecEnd) != CONTENTS_WATER))
	{
		// ALERT(at_aiconsole, "can't swim out of water\n");
		return FALSE;
	}
	*/




	
	//Now wait a moment.  We can't move to a point that's out of the water can we? Deny if so.
	int conPosition = UTIL_PointContents(vecEnd);
	if( conPosition != CONTENTS_WATER){
		//Water only!
		return LOCALMOVE_INVALID_DONT_TRIANGULATE;
	}




	


	/*
	//Vector goalDir = vecEnd - vecStart;

	//Gets a yaw (0 - 360 degrees).
	//To radians:
	//float goal_pitch = UTIL_VecToYaw(vecEnd - vecStart) * (CONST_PI/180.0f);

	Vector goal_direction = (vecEnd - vecStart).Normalize();
	Vector goal_direction_adjacent = CrossProduct(goal_direction, Vector(0, 0, 1) ).Normalize();

	
	UTIL_MakeVectors( pev->angles );
		

	
	TraceResult trTopLeft;
	TraceResult trTopRight;
	TraceResult trBottomLeft;
	TraceResult trBottomRight;
	
	Vector vecOff;
	float boundXSize = abs(pev->mins.x);
	float boundYSize = abs(pev->mins.y);
	float boundZSize = abs(pev->maxs.z/2);

	//Vector vecCenter = Vector(pev->origin.x, pev->origin.y, pev->origin.z + (pev->maxs.z - pev->mins.z)/2.0);
	Vector vecCenterRel = Vector(0, 0, (pev->maxs.z - pev->mins.z)/2.0);
	
	Vector whut = pev->origin;
	Vector vecStartAlt = vecStart + vecCenterRel + goal_direction * boundXSize*1.3;
	Vector vecEndAlt = vecEnd + vecCenterRel + -goal_direction * boundXSize*1.3;
	


	DebugLine_SetupPoint(7, vecStartAlt, 255, 255, 255);

	DebugLine_SetupPoint(8, vecEndAlt, 255, 0, 0);

	DebugLine_ClearAll();

	vecOff = -goal_direction_adjacent * boundYSize*1.3 + gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trTopLeft );
	DebugLine_Setup(0, vecStartAlt+vecOff, vecEndAlt+vecOff, trTopLeft.flFraction);

	vecOff = goal_direction_adjacent * boundYSize*1.3 + gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trTopRight );
	DebugLine_Setup(1, vecStartAlt+vecOff, vecEndAlt+vecOff, trTopRight.flFraction);

	vecOff = -goal_direction_adjacent * boundYSize*1.3 + -gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trBottomLeft );
	DebugLine_Setup(4, vecStartAlt+vecOff, vecEndAlt+vecOff, trBottomLeft.flFraction);

	vecOff = goal_direction_adjacent * boundYSize*1.3 + -gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trBottomRight );
	DebugLine_Setup(3, vecStartAlt+vecOff, vecEndAlt+vecOff, trBottomRight.flFraction);

	float minFraction;
	trTopLeft.flFraction<trTopRight.flFraction?minFraction=trTopLeft.flFraction:minFraction=trTopRight.flFraction;
	minFraction<trBottomLeft.flFraction?minFraction=minFraction:minFraction=trBottomLeft.flFraction;
	minFraction<trBottomRight.flFraction?minFraction=minFraction:minFraction=trBottomRight.flFraction;
	//minFraction<trCenter.flFraction?minFraction=minFraction:minFraction=trCenter.flFraction;
	BOOL tracesSolid;
	BOOL tracesStartSolid;
	tracesSolid = (trTopLeft.fAllSolid != 0 || trTopRight.fAllSolid != 0 || trBottomLeft.fAllSolid != 0 || trBottomRight.fAllSolid != 0); //|| trCenter.fAllSolid != 0);
	tracesStartSolid = (trTopLeft.fStartSolid != 0 || trTopRight.fStartSolid != 0 || trBottomLeft.fStartSolid != 0 || trBottomRight.fStartSolid != 0); //|| trCenter.fStartSolid != 0);

	
	

	
	if ( (tracesSolid == FALSE && tracesStartSolid == FALSE && minFraction >= 1.0)  ) //|| EASY_CVAR_GET(testVar) == 2)
	//if ( tr.fAllSolid == 0 && tr.fStartSolid == 0 && tr.flFraction >= 1.0)
	{
		//if(minFractionStore != NULL){ *minFractionStore = minFraction; }  //on success, the caller wants to know the minimum fraction seen, if a place to put it is provided.
		//return TRUE;
		//no, fall through.
	}else{
		//no go.
		return LOCALMOVE_INVALID_DONT_TRIANGULATE;
	}

	//return FALSE;



	if (pflDist)
	{
		*pflDist = minFraction * (vecEnd - vecStart).Length();
	}

	iReturn = LOCALMOVE_VALID;
	

	
	//if(tracesStartSolid || minFraction < 1.0)

	*/



	


	
	TraceResult tr;


	//UTIL_TraceHull( vecStart + Vector( 0, 0, 32 ), vecEnd + Vector( 0, 0, 32 ), dont_ignore_monsters, large_hull, edict(), &tr );
	UTIL_TraceHull( vecStart + Vector( 0, 0, 4), vecEnd + Vector( 0, 0, 4), dont_ignore_monsters, point_hull, edict(), &tr );
	
	// ALERT( at_console, "%.0f %.0f %.0f : ", vecStart.x, vecStart.y, vecStart.z );
	// ALERT( at_console, "%.0f %.0f %.0f\n", vecEnd.x, vecEnd.y, vecEnd.z );

	if (pflDist)
	{
		*pflDist = ( (tr.vecEndPos - Vector( 0, 0, 32 )) - vecStart ).Length();// get the distance.
	}
	


	// ALERT( at_console, "check %d %d %f\n", tr.fStartSolid, tr.fAllSolid, tr.flFraction );
	if (tr.fStartSolid || tr.flFraction < 1.0)
	{
		if ( pTarget && pTarget->edict() == gpGlobals->trace_ent ){
			iReturn = LOCALMOVE_VALID;
		}else{
			iReturn = LOCALMOVE_INVALID;
		}
	}else{
		iReturn = LOCALMOVE_VALID;
	}
	



	

	

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



	return iReturn;
}



//Copied from flyingmonster.cpp. Our sequence may not properly convey the m_flGroundSpeed.
void CArcher::Move( float flInterval )
{
	if ( pev->movetype == MOVETYPE_FLY )
		m_flGroundSpeed = m_flightSpeed;
	
	//CFlyingMonster::Move( flInterval );
	CBaseMonster::Move(flInterval);
}


BOOL CArcher::ShouldAdvanceRoute( float flWaypointDist )
{
	// Get true 3D distance to the goal so we actually reach the correct height
	if ( m_Route[ m_iRouteIndex ].iType & bits_MF_IS_GOAL )
		flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Length();

	if ( flWaypointDist <= 64 + (m_flGroundSpeed * gpGlobals->frametime) )
		return TRUE;

	return FALSE;
}





void CArcher::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
{

	/*
	if ( m_IdealActivity != m_movementActivity )
		m_IdealActivity = m_movementActivity;

	// ALERT( at_console, "move %.4f %.4f %.4f : %f\n", vecDir.x, vecDir.y, vecDir.z, flInterval );

	// float flTotal = m_flGroundSpeed * pev->framerate * flInterval;
	// UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, flTotal, MOVE_STRAFE );

	//m_velocity = m_velocity * 0.8 + m_flGroundSpeed * vecDir * 0.2;
	
	//UTIL_MoveToOrigin ( ENT(pev), pev->origin + m_velocity, m_velocity.Length() * flInterval, MOVE_STRAFE );
	m_flGroundSpeed = 124;
	UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, (m_flGroundSpeed * flInterval), MOVE_STRAFE );
	
	


	
	*/


	

	/*
	Vector vecSuggestedDir = (m_Route[m_iRouteIndex].vecLocation - pev->origin).Normalize();
	//float velMag = flStep * global_STUSpeedMulti;
	float velMag = m_flGroundSpeed * global_STUSpeedMulti;

	CFlyingMonster::MoveExecute(pTargetEnt, vecDir, flInterval);
	*/
    //checkFloor(vecSuggestedDir, velMag, flInterval);


	if ( m_IdealActivity != m_movementActivity )
	{
		m_IdealActivity = m_movementActivity;
		m_flGroundSpeed = m_flightSpeed = 200;
	}
	
	//m_flGroundSpeed = m_flightSpeed = 200;

	//m_flGroundSpeed = m_flightSpeed = 10;
	//TEST - just force it?
	//m_flGroundSpeed = m_flightSpeed = 200;
	//this->SetSequenceByIndex(SEQ_ARCHER_TURN_LEFT, 1);

	//m_flGroundSpeed = 200;

	float flTotal = 0;
	float flStepTimefactored = m_flGroundSpeed * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval;
	float flStep = m_flGroundSpeed * 1 * 1;
	


	float velMag = flStep * global_STUSpeedMulti;

	float timeAdjust = (pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval);
	float distOneFrame = velMag * pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval;
	
	Vector dest = m_Route[ m_iRouteIndex ].vecLocation;
	Vector vectBetween = (dest - pev->origin);
	float distBetween = vectBetween.Length();
	Vector dirTowardsDest = vectBetween.Normalize();
	Vector _velocity;

	if(distOneFrame <= distBetween){
		_velocity = dirTowardsDest * velMag;
	}else{
		_velocity = dirTowardsDest * distBetween/timeAdjust;
	}

	//UTIL_printLineVector("MOVEOUT", velMag);
	//easyPrintLineGroup2("HELP %.8ff %.8f", velMag, flInterval);

	//UTIL_drawLineFrame(pev->origin, dest, 64, 255, 0, 0);

	
	m_velocity = m_velocity * 0.8 + _velocity * 0.2;

	//m_velocity = m_velocity * 0.8 + m_flGroundSpeed * vecDir * 0.2;

	Vector flatVelocity = Vector(_velocity.x, _velocity.y, 0);
	Vector vertVelocity = Vector(0, 0, _velocity.z);

	//pev->velocity = _velocity;
	pev->velocity = m_velocity;

	Vector vecSuggestedDir = (m_Route[m_iRouteIndex].vecLocation - pev->origin).Normalize();

	checkFloor(vecSuggestedDir, velMag, flInterval);


	lastVelocityChange = gpGlobals->time;




}//END OF MoveExecute



/*

void CArcher::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
{
	//m_flGroundSpeed = 25;
	if ( pev->movetype == MOVETYPE_FLY )
	{
		if ( gpGlobals->time - m_stopTime > 1.0 )
		{
			if ( m_IdealActivity != m_movementActivity )
			{
				m_IdealActivity = m_movementActivity;
				m_flGroundSpeed = m_flightSpeed = 200;
			}
		}
		//Vector vecMove = pev->origin + (( vecDir + (m_vecTravel * m_momentum) ).Normalize() * (m_flGroundSpeed * flInterval));
		Vector vecMove = m_Route[ m_iRouteIndex ].vecLocation;

		if ( m_IdealActivity != m_movementActivity )
		{
			m_flightSpeed = UTIL_Approach( 100, m_flightSpeed, 75 * gpGlobals->frametime );
			if ( m_flightSpeed < 100 )
				m_stopTime = gpGlobals->time;
		}
		else
			m_flightSpeed = UTIL_Approach( 20, m_flightSpeed, 300 * gpGlobals->frametime );
		
		if ( CheckLocalMove ( pev->origin, vecMove, pTargetEnt, NULL ) )
		{
			m_vecTravel = (vecMove - pev->origin);
			m_vecTravel = m_vecTravel.Normalize();
			
			//UTIL_MoveToOrigin(ENT(pev), vecMove, (m_flGroundSpeed * flInterval), MOVE_STRAFE);
			UTIL_MoveToOrigin ( ENT(pev), m_Route[ m_iRouteIndex ].vecLocation, (m_flGroundSpeed * flInterval), MOVE_NORMAL );

			//pev->origin = pev->origin + (vecMove - pev->origin).Normalize()*(m_flGroundSpeed * flInterval);
		}
		else
		{
			m_IdealActivity = GetStoppedActivity();
			m_stopTime = gpGlobals->time;
			m_vecTravel = g_vecZero;
		}
	}
	else
		CBaseMonster::MoveExecute( pTargetEnt, vecDir, flInterval );
}

*/



//If the model won't tell us, we have to make one.
void CArcher::SetEyePosition(void){
	//CFlyingMonster::SetEyePosition();
	pev->view_ofs = Vector(0, 0, 42 - 8);//VEC_VIEW;
}//END OF SetEyePosition




//based off of GetSchedule for CFlyingMonster in schedule.cpp.
Schedule_t* CArcher::GetSchedule ( void )
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
					//return GetScheduleOfType( SCHED_CHASE_ENEMY_STOP_SIGHT );
					GetScheduleOfType(SCHED_CHASE_ENEMY);
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


Schedule_t* CArcher::GetScheduleOfType( int Type){
	
	switch(Type){

		case SCHED_CHASE_ENEMY:{
			//HOLD UP.  Does it really make sense to try this?

			if(m_hEnemy == NULL || m_hEnemy->pev->waterlevel == 3){
				//our enemy disappeared (???) or is in the water? ok. proceed as usual.
				//If they disappeared this will fail pretty fast. How'd it get called anyways?
				return slChaseEnemySmart;
			}else{
				//Enemy isn't in the water? Wait for them to come back. Can interrupt by being able to attack too.
				return slWaitForEnemyToEnterWater;
			}

		break;}
		case SCHED_CHASE_ENEMY_FAILED:{
			//Repeat from what schedule.cpp.  I'm not calling the parent method just for this.
			if(m_hEnemy != NULL){
				//this->m_vecEnemyLKP = m_hEnemy->pev->origin;
				setEnemyLKP(m_hEnemy->pev->origin);
			}

			if(m_hEnemy != NULL && m_hEnemy->pev->waterlevel == 3){
				//enemy is in the water and you failed?  Typical pathfind fail I guess.
				return &slFail[ 0 ];
			}else{
				//Enemy isn't in the water?  No wonder we can't get to them.
				//Just stick to staring with continual checks for the enemy being in the water or not.
				//That is be a little more reactive while waiting than just staring into space.
				return &slWaitForEnemyToEnterWater[ 0 ];
			}

		break;}
		


		case SCHED_DIE:
			//return flierDeathSchedule();
			return slDieWaterFloat;
		break;
		case SCHED_RANGE_ATTACK1:

			if(m_hEnemy == NULL || m_hEnemy->pev->waterlevel == 3){
				//Our enemy disappeared (will fail soon?) or is still in the water? Typical attack, nothing special.
				return slArcherRangeAttack1;
			}else{
				//Enemy isn't in the water?  Let's see if emerging at the surface of the water to do an attack is possible.
				//return slWaitForEnemyToEnterWater;
				//...
				//TODO. change this later.
				return slArcherRangeAttack1;
			}

		break;



	}//END OF switch(Type)
	
	return CFlyingMonster::GetScheduleOfType(Type);
}//END OF GetScheduleOfType


void CArcher::ScheduleChange(){

	Schedule_t* endedSchedule = m_pSchedule;

	//Use this to do something when a schedule ends that must happen at the time of interruption.
	//Depending on what schedule that ended typically, whether it was interrupted by some condition or naturally
	//ran out of tasks.
	
	
	
	
	CFlyingMonster::ScheduleChange(); //Call the parent.

}//END OF ScheduleChange


Schedule_t* CArcher::GetStumpedWaitSchedule(){
	return CFlyingMonster::GetStumpedWaitSchedule();
}//END OF GetStumpedWaitSchedule




void CArcher::StartTask( Task_t *pTask ){


	switch( pTask->iTask ){

		case TASK_DIE:
			//just do what the parent does.
			CFlyingMonster::StartTask(pTask);
		break;

		case TASK_RANGE_ATTACK1:
			shootCooldown = gpGlobals->time + RANDOM_LONG(3.5, 6.2);
			CFlyingMonster::StartTask(pTask);
		break;
		case TASK_STOP_MOVING:
			this->m_velocity = Vector(0, 0, 0);
			pev->velocity = Vector(0,0,0);
			CFlyingMonster::StartTask(pTask);

		break;

		case TASK_WATER_DEAD_FLOAT:
			//pev->skin = EYE_BASE;
			//SetSequenceByName( "bellyup" );
			SetSequenceByIndex(SEQ_ARCHER_DEAD_FLOAT);
			CFlyingMonster::StartTask(pTask);
		break;


		default:
			CFlyingMonster::StartTask( pTask );
		break;
	}//END OF switch

}//END OF StartTask

void CArcher::RunTask( Task_t *pTask ){
	
	//EASY_CVAR_PRINTIF_PRE(templatePrintout, easyPrintLine("RunTask: sched:%s task:%d", this->m_pSchedule->pName, pTask->iTask) );
	
	switch( pTask->iTask ){
		case TASK_RANGE_ATTACK1:{

			
			MakeIdealYaw ( m_vecEnemyLKP );
			ChangeYaw ( pev->yaw_speed );

			if ( m_fSequenceFinished )
			{
				//m_Activity = ACT_IDLE;
				//m_IdealActivity?
				SetActivity(ACT_HOVER);
				TaskComplete();
			}
		break;}

		/*
		case TASK_DIE_LOOP:{
			if(hitGroundDead){
				TaskComplete();
			}
		break;}
		*/

	



		default:
			CFlyingMonster::RunTask(pTask);
		break;
	}//END OF switch

}//END OF RunTask



BOOL CArcher::CheckMeleeAttack1( float flDot, float flDist ){
	if ( flDist <= 64 && flDot >= 0.7 && m_hEnemy != NULL )
	{
		return TRUE;
	}

	return FALSE;
}
BOOL CArcher::CheckMeleeAttack2( float flDot, float flDist ){
	return FALSE;
}
BOOL CArcher::CheckRangeAttack1( float flDot, float flDist ){

	//DEBUG - why you no work!!!??

	if(gpGlobals->time >= shootCooldown){
		//past cooldown? allowed.
	}else{
		//no.
		return FALSE;
	}


	//if ( flDot > 0.5 && flDist > 256 && flDist <= 2048 )
	if ( flDot > 0.5 && flDist <= 1024 )
	{
		//easyForcePrintLine("YAY?!");
		return TRUE;
	}
	//easyForcePrintLine("NAY?!");
	return FALSE;
}
BOOL CArcher::CheckRangeAttack2( float flDot, float flDist ){
	return FALSE;
}



void CArcher::CustomTouch( CBaseEntity *pOther ){
	int x = 46;
	if(pOther == NULL){
		return; //??????
	}
}


void CArcher::KilledFallingTouch( CBaseEntity *pOther ){
	
	//easyForcePrintLine("OH no IM a person friend %s", pOther!=NULL?pOther->getClassname():"WTF");

	int x = 46;
	//does this even work? uhh..

	//CHEAP FIX:

	if(pOther == NULL){
		return; //??????
	}


	hitGroundDead = TRUE;

	//groundTouchCheckDuration = gpGlobals->time + 4;
	
}











void CArcher::MonsterThink(){

	


	//easyForcePrintLine("IM GONNA %d %d", m_Activity, m_IdealActivity);
	//easyForcePrintLine("MY EYES: %.2f %.2f %.2f", pev->view_ofs.x,pev->view_ofs.y,pev->view_ofs.z);

	/*
	if(pev->deadflag == DEAD_NO && lastVelocityChange != -1 && (gpGlobals->time - lastVelocityChange) > 0.24  ){
		//no edits to velocity?  Start slowing down a lot.
		m_velocity = m_velocity * 0.15;
		pev->velocity = m_velocity;
		lastVelocityChange = gpGlobals->time;
	}else{

	}
	*/


	CFlyingMonster::MonsterThink();
}//END OF MonsterThink

// PrescheduleThink - this function runs after conditions are collected and before scheduling code is run.
//NOTE - PrescheduleThink is called by RunAI of monsterstate.cpp, which is called from MonsterThink in the parent CFlyingMonster class (monsters.cpp).
//The "MonsterThink" below still occurs earlier than PrescheduleThink
void CArcher::PrescheduleThink (){



	CFlyingMonster::PrescheduleThink();
}//END OF PrescheduleThink










int	CArcher::Classify(){
	return CLASS_ALIEN_MONSTER;
}
BOOL CArcher::isOrganic(){
	return TRUE;
}

int CArcher::IRelationship( CBaseEntity *pTarget ){

	return CFlyingMonster::IRelationship(pTarget);
}//END OF IRelationship


void CArcher::ReportAIState(){
	//call the parent, and add on to that.
	CFlyingMonster::ReportAIState();
	//print anything special with easyForcePrintLine
}//END OF ReportAIState()





GENERATE_TRACEATTACK_IMPLEMENTATION(CArcher)
{



	GENERATE_TRACEATTACK_PARENT_CALL(CFlyingMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CArcher)
{
	
	//CFlyingMonster already calls PainSound.
	//PainSound();


	return GENERATE_TAKEDAMAGE_PARENT_CALL(CFlyingMonster);
}



//NOTE - called by CFlyingMonster's TakeDamage method. If that isn't called, DeadTakeDamage won't get called naturally.
GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION(CArcher)
{



	return GENERATE_DEADTAKEDAMAGE_PARENT_CALL(CFlyingMonster);
}//END OF DeadTakeDamage




//Parameters: integer named fGibSpawnsDecal
// The parent method calls GIBMONSTERGIB, then GIBMONSTERSOUND and GIBMONSTEREND, passing along to the latter two whether GIBMONSTERGIB
// actually "gibbed" or not (it must return whether it did or not).
// This default behavior is ok, change pieces at a time instead in other methods. GIBMONSTERGIB is the most likely one that needs customization,
// such as only spawning two alien gibs for the ChumToad. The others are good as defaults.
GENERATE_GIBMONSTER_IMPLEMENTATION(CArcher)
{
	GENERATE_GIBMONSTER_PARENT_CALL(CFlyingMonster);
}



//Parameters: BOOL fGibSpawnsDecal
//Returns: BOOL. Did this monster spawn gibs and is safe to stop drawing?
// If this monster has a special way of spawning gibs or checking whether to spawn gibs, handle that here and remove the parent call.
// The parent GIBMONSTERGIB method is supposed to handle generic cases like determining whether to spawn human or alien gibs based on
// Classify(), or robot gibs under german censorship. This implementation can be specific to just this monster instead.
//NOTICE - do NOT do something special here AND call the parent method through GENERATE_GIBMONSTERGIB_PARENT_CALL.
//Anything done here is meant to completely replace how the parent method gibs a monster in general. None of it is required.
GENERATE_GIBMONSTERGIB_IMPLEMENTATION(CArcher)
{
	

	//BOOM.

	int iContents = UTIL_PointContents ( pev->origin );
	short spriteChosen;
	if (iContents != CONTENTS_WATER)
	{
		spriteChosen = g_sModelIndexFireball;
	}
	else
	{
		spriteChosen = g_sModelIndexWExplosion;
	}
	UTIL_Explosion(pev, pev->origin + Vector(0, 0, 8), spriteChosen, (160 - 50) * 0.60, 15, TE_EXPLFLAG_NOSOUND | TE_EXPLFLAG_NOPARTICLES, pev->origin + Vector(0, 0, 16), 1 );
	


//void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType )
//void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )



	RadiusDamage(pev->origin, pev, pev, gSkillData.bullsquidDmgSpit, 160.0f,
		CLASS_MACHINE | CLASS_ALIEN_MILITARY | CLASS_ALIEN_PASSIVE | CLASS_ALIEN_MONSTER | CLASS_ALIEN_PREY | CLASS_ALIEN_PREDATOR | CLASS_INSECT | CLASS_BARNACLE,
		DMG_POISON);


	//Calling the parent method is still okay in this case. Spawning gib pieces / censorship checks should still take place.
	return GENERATE_GIBMONSTERGIB_PARENT_CALL(CFlyingMonster);
}

//The other related methods, GIBMONSTERSOUND and GIBMONSTEREND, are well suited to the majority of cases.




GENERATE_KILLED_IMPLEMENTATION(CArcher)
{


	

	/*

	BOOL firstCall = FALSE;
	if(pev->deadflag == DEAD_NO){
		//keep this in mind...
		firstCall = TRUE;

		//Only reset the velocity if this is the first Killed call (since we stop following).
		//Any further resets will look like gravity suddenly stops with each shot (Killed call again).
		pev->velocity = Vector(0, 0, 0);
		m_velocity = Vector(0, 0, 0);
	}

	//MODDD - is still doing here ok?
	GENERATE_KILLED_PARENT_CALL(CFlyingMonster);

	//HACK. guarantee we fall to the ground, even if killed while in the DEAD_DYING state.
	//...which forces MOVETYPE_STEP, and is not very good.

	pev->movetype = MOVETYPE_TOSS;
	*/




	//Copy of how the ichy does it to want to float to the top. Or part of it.
	
	//Is calling direct parent CFlyingMonster instead of CBaseMonster ok?
	GENERATE_KILLED_PARENT_CALL(CFlyingMonster);
	pev->velocity = Vector( 0, 0, 0 );

	//MODDD NOTE
	//Forcing the movetype to MOVETYPE_STEP in CFlyingMonster's Killed above can be bad for the floating logic.
	//Or flyers in general ever really, MOVETYPE_TOSS falls just fine. no idea why
	pev->movetype = MOVETYPE_FLY;







	/*
	//if you have the "FL_KILLME" flag, it means this is about to get deleted (gibbed). No point in doing any of this then.
	if(firstCall && !(pev->flags & FL_KILLME) ){
		cheapKilledFlier();
	}//END OF firstCall check
	*/



}//END OF Killed







void CArcher::SetYawSpeed( void ){
	int ys;
	
	ys = 120;
	//ys = 200;

	//Switch on current activity m_Activity to determine yaw speed and set it?
	
	switch ( m_Activity )
	{
	case ACT_IDLE:
		//ys = 100;
	break;
	}


	pev->yaw_speed = ys;
}//END OF SetYawSpeed





BOOL CArcher::getMonsterBlockIdleAutoUpdate(void){
	return FALSE;
}
BOOL CArcher::forceIdleFrameReset(void){
	return FALSE;
}
BOOL CArcher::usesAdvancedAnimSystem(void){
	return TRUE;
}

void CArcher::SetActivity(Activity NewActivity ){

	//Little check. If this is to change IDLE to HOVER or HOVER to IDLE, don't reset the animation.
	//it's fine the way it is, the are one and the same.

	if((NewActivity == ACT_IDLE || NewActivity == ACT_HOVER) &&
		(m_Activity == ACT_IDLE || m_Activity == ACT_HOVER)){

		m_Activity = ACT_HOVER;
		m_IdealActivity = ACT_HOVER;
		return;
	}else{
		//proceed normally.
		CFlyingMonster::SetActivity(NewActivity);
	}

}//END OF SetActivity




//IMPORTANT. To get the animation from the model the usual way, you must use "CBaseAnimating::LookupActivity(activity);" to do so,
//           do NOT forget the "CBaseAnimating::" in front and do NOT use any other base class along the way, like CSquadMonster or CFlyingMonster.
//           Need to call CBaseAnimating's primitive version because CFlyingMonster's LookupActivity may now call LookupActivityHard or tryActivitySubstitute
//           on its own, which would trigger an infinite loop of calls (stack overflow):
//           * This class calling LookupActivityHard, persay, falling back to calling...
//           * Monster's LookupActivity, deciding to call the...
//           * Child class's LookupActiivtyHard for a possible substitution, falling back to calling...
//           * Monster's LookupActivity, deciding to call the...
//           * Child class's LookupActivityHard for a possible substitution, falling back to...
//           Repeat the last two ad infinitum. Crash.

int CArcher::tryActivitySubstitute(int activity){
	int i = 0;

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		//Let whoever know we have these anims.
		case ACT_IDLE:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_WALK:
		case ACT_RUN:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_MELEE_ATTACK1:
			return SEQ_ARCHER_BITE;
		break;
		case ACT_RANGE_ATTACK1:
			return SEQ_ARCHER_SHOOT;
		break;
		//Do these break anything?
		case ACT_FLY:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_HOVER:
			return SEQ_ARCHER_IDLE1;
		break;
	}//END OF switch


	//not handled by above? Rely on the model's anim for this activity if there is one.
	return CBaseAnimating::LookupActivity(activity);
}//END OF tryActivitySubstitute



int CArcher::LookupActivityHard(int activity){
	int i = 0;
	m_flFramerateSuggestion = 1;
	pev->framerate = 1;
	m_iForceLoops = -1;  //signal to leave looping up to the model.
	//any animation events in progress?  Clear it.
	resetEventQueue();

	//Within an ACTIVITY, pick an animation like this (with whatever logic / random check first):
	//    this->animEventQueuePush(10.0f / 30.0f, 3);  //Sets event #3 to happen at 1/3 of a second
	//    return LookupSequence("die_backwards");      //will play animation die_backwards


	
	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		case ACT_IDLE:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_WALK:
		case ACT_RUN:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_TURN_LEFT:
		case ACT_TURN_RIGHT:
			m_iForceLoops = TRUE;
			//otherwise what is returned is fine.
		break;
		case ACT_MELEE_ATTACK1:
			animEventQueuePush(45.0f, 1);
			return SEQ_ARCHER_BITE;
		break;
		case ACT_RANGE_ATTACK1:
			
			animEventQueuePush(10.8f / 18.0f, 0);
			m_iForceLoops = FALSE;
			return SEQ_ARCHER_SHOOT;
		break;
		//Do these break anything?
		case ACT_FLY:
			return SEQ_ARCHER_IDLE1;
		break;
		case ACT_HOVER:
			return SEQ_ARCHER_IDLE1;
		break;
	}//END OF switch
	
	
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}//END OF LookupActivityHard



//Handles custom events sent from "LookupActivityHard", which sends events as timed delays along with picking an animation in script.
//So this handles script-provided events, not model ones.
void CArcher::HandleEventQueueEvent(int arg_eventID){

	switch(arg_eventID){
	case 0:
	{

		//break;

		//fire a bullsquid projectile? not psychic like the kingpin.
		Vector	vecSpitOffset;
		Vector	vecSpitDir;

		UTIL_MakeVectors ( pev->angles );

		vecSpitOffset = ( pev->origin + gpGlobals->v_right * 0 + gpGlobals->v_forward * 18 + gpGlobals->v_up * 22 );		
		
		vecSpitDir = ( ( m_vecEnemyLKP +  ((m_hEnemy!=NULL)?(m_hEnemy->EyeOffset()):(Vector(0,0,5)))   ) - vecSpitOffset ).Normalize();
		
		vecSpitDir.x += RANDOM_FLOAT( -0.05, 0.05 );
		vecSpitDir.y += RANDOM_FLOAT( -0.05, 0.05 );
		vecSpitDir.z += RANDOM_FLOAT( -0.05, 0 );

		// do stuff for this event.
		AttackSound();

		CSquidSpit::Shoot( this, vecSpitOffset, vecSpitDir, 900 );

	break;
	}
	case 1:
	{

		//melee bite?
		CBaseEntity *pHurt = CheckTraceHullAttack( 70, gSkillData.bullsquidDmgBite, DMG_SLASH, DMG_BLEEDING );
		if ( pHurt )
		{
			if ( pHurt->pev->flags & (FL_MONSTER|FL_CLIENT) )
			{
				pHurt->pev->punchangle.x = 5;

				//zombie's both arm slash velocity pull.
				//pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_forward * -100;
				
				pHurt->pev->velocity = pHurt->pev->velocity - gpGlobals->v_forward * 13;
				pHurt->pev->velocity = pHurt->pev->velocity + gpGlobals->v_up * 4;
				//MODDD TODO - Should there be a random velocity applied since this is underwater?
			}
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackHitSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackHitSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );
		}
		else
			EMIT_SOUND_FILTERED ( ENT(pev), CHAN_WEAPON, pAttackMissSounds[ RANDOM_LONG(0,ARRAYSIZE(pAttackMissSounds)-1) ], 1.0, ATTN_NORM, 0, 100 + RANDOM_LONG(-5,5) );


		/*
		//MODDD TODO - do this later when we have the sound?
		if (RANDOM_LONG(0,1))
			AttackSound();
		*/

	break;
	}
	}//END OF switch


}//END OF HandleEventQueueEvent


//This handles events built into the model, not custom hard-coded ones (above).
void CArcher::HandleAnimEvent(MonsterEvent_t *pEvent ){
	switch( pEvent->event ){
	/*
	case BLOATER_AE_ATTACK_MELEE1:
	{
		// do stuff for this event.
		AttackSnd();
	}
	*/
	case 0:
	{

	break;
	}

	default:
		CFlyingMonster::HandleAnimEvent( pEvent );
	break;
	}//END OF switch
}
























//inline
void CArcher::checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist){
	checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecStart, vecRelativeEnd, moveDist, TRUE);
}
//Vector& const vecRelstar, ???   Vector& const reactionMove,
//inline
void CArcher::checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture){
	

	//WELL WHAT THE whatIN what IS MOVIN YA.
	//return;

	TraceResult tr;

	//    * moveDist ??
	Vector vecRelativeEndScale = vecRelativeEnd * moveDist;

	if(!tempCheckTraceLineBlock){

		//Vector vecEnd = vecStart + Vector(0, 0, 38);
		UTIL_TraceLine(vecStart, vecStart + vecRelativeEndScale, ignore_monsters, ENT(pev), &tr);
		if(tr.flFraction < 1.0){
			//hit something!

			//Get projection
			// = sugdir - proj. of sugdir onto the normal vector.

			//does this work?
			float dist = tr.flFraction * (float)moveDist;
			float toMove = moveDist - dist;
			//pev->origin = pev->origin + -toMove*vecRelativeEnd;
		
			float timeAdjust = (pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval);
			
			Vector vecMoveParallel = UTIL_projectionComponent(vecSuggestedDir, tr.vecPlaneNormal).Normalize() * (travelMag * 1);
			//Vector vecMoveParallel = Vector(0,0,0);

			if(timeAdjust == 0){
				//easyPrintLineGroup2("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return;
			}else{
				//...
			}

			Vector vecMoveRepel = (tr.vecPlaneNormal*toMove*global_STUrepelMulti)/1;
			
			//pev->origin = pev->origin + vecMoveParallel;
			////UTIL_MoveToOrigin ( ENT(pev), pev->origin + -toMove*vecRelativeEnd + vecMoveParallel , travelMag, MOVE_STRAFE );
		
			//Vector vecTotalAdjust = vecMoveParallel + vecMoveRepel;
			Vector vecTotalAdjust = vecMoveParallel*timeAdjust + vecMoveRepel;


			//???    + -(toMove*1)*vecRelativeEnd
			//pev->velocity = pev->velocity  + ((vecMoveParallel + vecMoveRepel)/timeAdjust);
			
			//MODDD NOTICE - We have a big problem here.
			/*
			UTIL_MoveToOrigin is nice because it only moves the origin of a given entity (this one) up to so far
			until it collides with anything, other monsters or map geometry, if anything is in the way. 
			A direct pev->origin set does not offer this at all.
			Problem is, UTIL_MoveToOrigin can also hang on the same corner we are, so it won't move the stuka
			at all past a corner it is caught on because it is "blocked" by that same corner.
			Way to get around: Move one coord at a time, all of the X-ways, then Y-ways, then Z-ways.
			
			//TODO - this still isn't perfect. It would be better to let the direction we're repelling
			//from play a role in whether the X or Y gets to run first for instance, but generally doing x, y, z
			//individually at all is still better than not.
			
			*/
			//JUST SPLIT IT UP!
			Vector vecTotalAdjustX = Vector(vecTotalAdjust.x, 0, 0);
			Vector vecTotalAdjustY = Vector(0, vecTotalAdjust.y, 0);
			Vector vecTotalAdjustZ = Vector(0, 0, vecTotalAdjust.z);

			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustX , vecTotalAdjustX.Length(), MOVE_STRAFE );
			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustY , vecTotalAdjustY.Length(), MOVE_STRAFE );
			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustZ , vecTotalAdjustZ.Length(), MOVE_STRAFE );




			//pev->origin = pev->origin + tr.vecPlaneNormal*toMove*global_repelMulti;
			//easyPrintLineGroup2("MOOO %s: SPEED: %.2f", STRING(tr.pHit->v.classname), travelMag );
			//EASY_CVAR_PRINTIF_PRE(stukaPrintout, UTIL_printLineVector("VECCCC", tr.vecPlaneNormal ) );

			if(canBlockFuture){
				tempCheckTraceLineBlock = TRUE;
			}
			//MODDAHHH 0.91, 5, 0.44, 4.56
			//easyPrintLineGroup2("MODDAHHH %.2f, %d, %.2f, %.2f ", tr.flFraction, moveDist, toMove, (tr.vecEndPos - vecStart).Length());

		}//END OF if(tr.flFraction < 1.0)
	}//END OF if(!tempCheckTraceLineBlock)
	
	if(global_drawDebugPathfinding2 == 1){
		UTIL_drawLineFrame(vecStart, vecStart + vecRelativeEndScale, 16, 0, 255, 0);
	}

}




inline
void CArcher::checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist){
	checkTraceLineTest(vecSuggestedDir, travelMag, flInterval, vecStart, vecRelativeEnd, moveDist, TRUE);
}
//Vector& const vecRelstar, ???   Vector& const reactionMove,
inline
void CArcher::checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture){
	
	
	//WELL WHAT THE whatIN what IS MOVIN YA.
	//return;

	//tempCheckTraceLineBlock = FALSE;

	TraceResult tr;

	//    * moveDist ??
	Vector vecRelativeEndScale = vecRelativeEnd * moveDist;

	if(!tempCheckTraceLineBlock){

		//Vector vecEnd = vecStart + Vector(0, 0, 38);
		UTIL_TraceLine(vecStart, vecStart + vecRelativeEndScale, ignore_monsters, ENT(pev), &tr);
		if(tr.flFraction < 1.0){
			//hit something!

			//Get projection
			// = sugdir - proj. of sugdir onto the normal vector.

			//does this work?
			float dist = tr.flFraction * (float)moveDist;
			float toMove = moveDist - dist;
			//pev->origin = pev->origin + -toMove*vecRelativeEnd;
		
			float timeAdjust = (pev->framerate * EASY_CVAR_GET(animationFramerateMulti) * flInterval);
			
			Vector vecMoveParallel = UTIL_projectionComponent(vecSuggestedDir, tr.vecPlaneNormal).Normalize() * (travelMag * 1);
			//Vector vecMoveParallel = Vector(0,0,0);

			if(timeAdjust == 0){
				//easyPrintLineGroup2("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				return;
			}else{
				//...
			}

			Vector vecMoveRepel = (tr.vecPlaneNormal*toMove*global_STUrepelMulti)/1;
			
			//pev->origin = pev->origin + vecMoveParallel;
			////UTIL_MoveToOrigin ( ENT(pev), pev->origin + -toMove*vecRelativeEnd + vecMoveParallel , travelMag, MOVE_STRAFE );
		
			//Vector vecTotalAdjust = vecMoveParallel + vecMoveRepel;
			Vector vecTotalAdjust = vecMoveParallel*timeAdjust + vecMoveRepel;


			//???    + -(toMove*1)*vecRelativeEnd
			//pev->velocity = pev->velocity  + ((vecMoveParallel + vecMoveRepel)/timeAdjust);
			
			//UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjust , vecTotalAdjust.Length(), MOVE_STRAFE );
			//pev->origin = pev->origin + vecTotalAdjust;

			//MODDD NOTICE - We have a big problem here.
			/*
			UTIL_MoveToOrigin is nice because it only moves the origin of a given entity (this one) up to so far
			until it collides with anything, other monsters or map geometry, if anything is in the way. 
			A direct pev->origin set does not offer this at all.
			Problem is, UTIL_MoveToOrigin can also hang on the same corner we are, so it won't move the stuka
			at all past a corner it is caught on because it is "blocked" by that same corner.
			Way to get around: Move one coord at a time, all of the X-ways, then Y-ways, then Z-ways.
			
			//TODO - this still isn't perfect. It would be better to let the direction we're repelling
			//from play a role in whether the X or Y gets to run first for instance, but generally doing x, y, z
			//individually at all is still better than not.
			
			*/
			//JUST SPLIT IT UP!
			Vector vecTotalAdjustX = Vector(vecTotalAdjust.x, 0, 0);
			Vector vecTotalAdjustY = Vector(0, vecTotalAdjust.y, 0);
			Vector vecTotalAdjustZ = Vector(0, 0, vecTotalAdjust.z);

			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustX , vecTotalAdjustX.Length(), MOVE_STRAFE );
			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustY , vecTotalAdjustY.Length(), MOVE_STRAFE );
			UTIL_MoveToOrigin ( ENT(pev), pev->origin + vecTotalAdjustZ , vecTotalAdjustZ.Length(), MOVE_STRAFE );




			//easyForcePrintLine("BUT YOU MOVE????? %.2f ", vecTotalAdjust.Length());
			::UTIL_drawLineFrame(pev->origin, pev->origin + vecTotalAdjust,40, 255, 0, 0);

			//pev->origin = pev->origin + tr.vecPlaneNormal*toMove*global_repelMulti;
			//easyPrintLineGroup2("MOOO %s: SPEED: %.2f", STRING(tr.pHit->v.classname), travelMag );
			//EASY_CVAR_PRINTIF_PRE(stukaPrintout, UTIL_printLineVector("VECCCC", tr.vecPlaneNormal ) );

			if(canBlockFuture){
				tempCheckTraceLineBlock = TRUE;
			}
			//MODDAHHH 0.91, 5, 0.44, 4.56
			//easyPrintLineGroup2("MODDAHHH %.2f, %d, %.2f, %.2f ", tr.flFraction, moveDist, toMove, (tr.vecEndPos - vecStart).Length());

		}//END OF if(tr.flFraction < 1.0)
	}//END OF if(!tempCheckTraceLineBlock)
	
	if(global_drawDebugPathfinding2 == 1){
		UTIL_drawLineFrame(vecStart, vecStart + vecRelativeEndScale, 16, 0, 255, 0);
	}

}









void CArcher::checkFloor(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval){

	//return;
	/*
	if(turnThatOff){
		//we're not doing the checks in this case.
		return;
	}
	*/
	//UTIL_drawBoxFrame(pev->absmin, pev->absmax, 16, 0, 0, 255);
	if(global_drawDebugPathfinding2 == 1){
		UTIL_drawBoxFrame(pev->origin + pev->mins, pev->origin + pev->maxs, 16, 0, 0, 255);
	}
	
	int maxX = pev->maxs.x;
	int maxY = pev->maxs.y;
	int maxZ = pev->maxs.z;
	
	int minX = pev->mins.x;
	int minY = pev->mins.y;
	int minZ = pev->mins.z;
	//     Min      Max
	//z = bottom / top
	//x = left / right
	//y = back / forward


	float boundMultiple = 0.7f;

	Vector vecTopRightForward = pev->origin + pev->maxs*boundMultiple;
	
	Vector vecTopLeftForward = pev->origin + Vector(minX, maxY, maxZ)*boundMultiple;
	Vector vecTopRightBackward = pev->origin + Vector(maxX, minY, maxZ)*boundMultiple;
	Vector vecTopLeftBackward = pev->origin + Vector(minX, minY, maxZ)*boundMultiple;

	Vector vecBottomLeftBackward = pev->origin + pev->mins*boundMultiple;
	
	Vector vecBottomLeftForward = pev->origin + Vector(minX, maxY, minZ)*boundMultiple;
	Vector vecBottomRightBackward = pev->origin + Vector(maxX, minY, minZ)*boundMultiple;
	Vector vecBottomRightForward = pev->origin + Vector(maxX, maxY, minZ)*boundMultiple;
	
	//const float root2 = 1.41421356;
	//const float root3 = ?;
	const float root2rec = 0.70710678;
	const float root3rec = 0.57735027;

	/*
	int checkDist = 18;
	int checkDistV = 32;
	
	int checkDistD = 38;
	*/

	int checkDist = global_STUcheckDistH;
	int checkDistV = global_STUcheckDistV;
	
	int checkDistD = global_STUcheckDistD;


	//float Vector push;
	
	if(vecSuggestedDir.x > 0.8){
		//checkCollisionLeft(vecTopLeftForward, 2);
		tempCheckTraceLineBlock = FALSE;
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightForward, Vector(1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightBackward, Vector(1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightForward, Vector(1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(1, 0, 0), checkDist);
		
	}else if (vecSuggestedDir.x < -0.8){
		
		tempCheckTraceLineBlock = FALSE;
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftForward, Vector(-1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftBackward, Vector(-1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftForward, Vector(-1, 0, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftBackward, Vector(-1, 0, 0), checkDist);
		
	}

	if(vecSuggestedDir.y > 0.8){
		//checkCollisionLeft(vecTopLeftForward, 2);
		tempCheckTraceLineBlock = FALSE;
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftForward, Vector(0, 1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightForward, Vector(0, 1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftForward, Vector(0, 1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightForward, Vector(0, 1, 0), checkDist);
		
	}else if (vecSuggestedDir.y < -0.8){

		tempCheckTraceLineBlock = FALSE;
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftBackward, Vector(0, -1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightBackward, Vector(0, -1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftBackward, Vector(0, -1, 0), checkDist);
		checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(0, -1, 0), checkDist);
	}



	BOOL onGround = FALSE;
	if(!onGround){

		if(vecSuggestedDir.z > 0){
			//checkCollisionLeft(vecTopLeftForward, 2);

			if(vecSuggestedDir.z > 0.3){
				tempCheckTraceLineBlock = FALSE;
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftForward, Vector(0, 0, 1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightForward, Vector(0, 0, 1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftBackward, Vector(0, 0, 1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightBackward, Vector(0, 0, 1), checkDistV);
			}
			

			BOOL topLeftForwardCheck = FALSE;
			BOOL topLeftBackwardCheck = FALSE;
			BOOL topRightForwardCheck = FALSE;
			BOOL topRightBackwardCheck = FALSE;

			if(vecSuggestedDir.x > 0.5){
				//do the right ones.
				topRightForwardCheck = TRUE;
				topRightBackwardCheck = TRUE;
			}else if(vecSuggestedDir.x < 0.5){
				topLeftForwardCheck = TRUE;
				topLeftBackwardCheck = TRUE;
			}

			if(vecSuggestedDir.y > 0.5){
				//do the forward ones.
				topRightForwardCheck = TRUE;
				topLeftForwardCheck = TRUE;
			}else if(vecSuggestedDir.y < 0.5){
				topRightBackwardCheck = TRUE;
				topLeftBackwardCheck = TRUE;
			}
			

			//TEST - ENABLE ALL
			topRightForwardCheck = TRUE;
			topLeftForwardCheck = TRUE;
			topRightBackwardCheck = TRUE;
			topLeftBackwardCheck = TRUE;


			
			tempCheckTraceLineBlock = FALSE; //is that okay?
			if(topRightForwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightForward, Vector(root3rec, root3rec, -root3rec), checkDistD);
			if(topLeftForwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightBackward, Vector(root3rec, -root3rec, -root3rec), checkDistD);
			if(topRightBackwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftForward, Vector(-root3rec, root3rec, -root3rec), checkDistD);
			if(topLeftBackwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftBackward, Vector(-root3rec, -root3rec, -root3rec), checkDistD);
			


			//easyForcePrintLine("AWWWWW SHIT %.2f %.2f", vecSuggestedDir.x, vecSuggestedDir.y);


			/*
			if(vecSuggestedDir.x > 0){
				if(vecSuggestedDir.y > 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightForward, Vector(root3rec, root3rec, -root3rec), checkDistD);
				}else if(vecSuggestedDir.y < 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopRightBackward, Vector(root3rec, -root3rec, -root3rec), checkDistD);
				}
			}else if(vecSuggestedDir.x < 0){
				if(vecSuggestedDir.y > 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftForward, Vector(-root3rec, root3rec, -root3rec), checkDistD);
				}else if(vecSuggestedDir.y < 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecTopLeftBackward, Vector(-root3rec, -root3rec, -root3rec), checkDistD);
				}
			}
			*/






			//just try bottom checks at least, even with no Z direction. Diagonals can be important.
		}else if (vecSuggestedDir.z <= 0){
		
			if(vecSuggestedDir.z < -0.3){
				tempCheckTraceLineBlock = FALSE;
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftForward, Vector(0, 0, -1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightForward, Vector(0, 0, -1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftBackward, Vector(0, 0, -1), checkDistV);
				checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(0, 0, -1), checkDistV);
			}

			

			
			BOOL bottomLeftForwardCheck = FALSE;
			BOOL bottomLeftBackwardCheck = FALSE;
			BOOL bottomRightForwardCheck = FALSE;
			BOOL bottomRightBackwardCheck = FALSE;

			if(vecSuggestedDir.x > 0.5){
				//do the right ones.
				bottomRightForwardCheck = TRUE;
				bottomRightBackwardCheck = TRUE;
			}else if(vecSuggestedDir.x < 0.5){
				bottomLeftForwardCheck = TRUE;
				bottomLeftBackwardCheck = TRUE;
			}

			if(vecSuggestedDir.y > 0.5){
				//do the forward ones.
				bottomRightForwardCheck = TRUE;
				bottomLeftForwardCheck = TRUE;
			}else if(vecSuggestedDir.y < 0.5){
				bottomRightBackwardCheck = TRUE;
				bottomLeftBackwardCheck = TRUE;
			}
			

			//TEST - ENABLE ALL
			bottomRightForwardCheck = TRUE;
			bottomLeftForwardCheck = TRUE;
			bottomRightBackwardCheck = TRUE;
			bottomLeftBackwardCheck = TRUE;

			tempCheckTraceLineBlock = FALSE; //is that okay?
			if(bottomRightForwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightForward, Vector(root3rec, root3rec, -root3rec), checkDistD);
			if(bottomLeftForwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(root3rec, -root3rec, -root3rec), checkDistD);
			if(bottomRightBackwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftForward, Vector(-root3rec, root3rec, -root3rec), checkDistD);
			if(bottomLeftBackwardCheck)checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftBackward, Vector(-root3rec, -root3rec, -root3rec), checkDistD);
			



			/*
			if(vecSuggestedDir.x > 0){
				if(vecSuggestedDir.y > 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightForward, Vector(root3rec, root3rec, -root3rec), checkDistD);
				}else if(vecSuggestedDir.y < 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(root3rec, -root3rec, -root3rec), checkDistD);
				}
			}else if(vecSuggestedDir.x < 0){
				if(vecSuggestedDir.y > 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftForward, Vector(-root3rec, root3rec, -root3rec), checkDistD);
				}else if(vecSuggestedDir.y < 0){
					checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecBottomLeftBackward, Vector(-root3rec, -root3rec, -root3rec), checkDistD);
				}
			}
			*/

			
			//checkTraceLineTest(vecSuggestedDir, travelMag, flInterval, vecBottomRightBackward, Vector(root3rec, -root3rec, -root3rec), checkDistD, FALSE);
			


		}

	}//END OF if(!onGround)


}//END OF checkFloor


//TEST: what happens if already touching the ground before death? Test!!
void CArcher::OnKilledSetTouch(void){
	SetTouch(&CArcher::KilledFallingTouch);
}

int CArcher::getLoopingDeathSequence(void){
	//TODO - this is underwater, this may work a little differently. gravity? etc?
	return SEQ_ARCHER_SINK;
}

//Everything should just aim at my head.
Vector CArcher::BodyTarget(const Vector &posSrc){
	return Vector(pev->origin.x, pev->origin.y, pev->origin.z + pev->maxs.z - 8);
}
Vector CArcher::BodyTargetMod(const Vector &posSrc){
	return Vector(pev->origin.x, pev->origin.y, pev->origin.z + pev->maxs.z - 8);
}


void CArcher::onDeathAnimationEnd(){
	

	//wait. Do we want to float like the ichy does?
	//It does nothing special.  Just keep it to what the parent does.
	CFlyingMonster::onDeathAnimationEnd();

}//END OF onDeathAnimationEnd


//I can goto the surface of water to do ranged attacks too. I have to know if I can by seeing where the enemy is.
//And be able to track the enemy above water (from itself being underwater) to even begin to determine that.
BOOL CArcher::SeeThroughWaterLine(void){
	return TRUE;
}//END OF SeeThroughWaterLine

BOOL CArcher::noncombat_Look_ignores_PVS_check(void){
	return TRUE;
}//END OF ignores_PVS_check




//MODDD - mimick how the ichy does this.
void CArcher::BecomeDead( void )
{
	pev->takedamage = DAMAGE_YES;// don't let autoaim aim at corpses.

	// give the corpse half of the monster's original maximum health. 
	pev->health = pev->max_health / 2;
	pev->max_health = 5; // max_health now becomes a counter for how many blood decals the corpse can place.
}//END OF BecomeDead


//TODO - implement the probe?  Ensure the monster stays underwater like the ichy does.



