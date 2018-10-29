
#include "floater.h"

#include "schedule.h"
#include "activity.h"
#include "animation.h"

#include "defaultai.h"
#include "soundent.h"
#include "game.h"

#include "custom_debug.h"


/*
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
*/


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




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//sequences in the anim, in the order they appear in the anim. Some anims have the same display name and so should just be referenced by order
//(numbered index), named well after purpose and based on display names for clarity. Safer this way.
enum floater_sequence{  //key: frames, FPS
	SEQ_FLOATER_IDLE1,
	SEQ_FLOATER_TURN_LEFT,
	SEQ_FLOATER_TURN_RIGHT,
	SEQ_FLOATER_SHOOT,
	SEQ_FLOATER_DIE,
	SEQ_FLOATER_FALL_LOOP,
	SEQ_FLOATER_FALL_DIE,
	SEQ_FLOATER_FLINCH,


};


//custom schedules
enum{
	SCHED_FLOATER_RANGE_ATTACK = LAST_COMMON_SCHEDULE + 1,


};

//custom tasks
enum{
	TASK_FLOATER_XXX = LAST_COMMON_TASK + 1,
	

};







//Original floater sound methods for reference. Just zombie sounds though, still placeholders.
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



const char* CFloater::pDeathSounds[] = 
{
	"floater/floater_death.wav",
};
const char* CFloater::pAlertSounds[] = 
{
	"floater/floater_alert.wav",
};

const char* CFloater::pIdleSounds[] = 
{
	"floater/floater_idle.wav",
};
const char* CFloater::pPainSounds[] = 
{
	"floater/floater_pain.wav",
};
const char* CFloater::pAttackSounds[] = 
{
	"floater/floater_attack.wav",
};
const char* CFloater::pAttackHitSounds[] = 
{
	"zombie/claw_strike1.wav",
	"zombie/claw_strike2.wav",
	"zombie/claw_strike3.wav",
};
const char* CFloater::pAttackMissSounds[] = 
{
	"zombie/claw_miss1.wav",
	"zombie/claw_miss2.wav",
};



//...did not come with a melee anim. what.
#define	BLOATER_AE_ATTACK_MELEE1		0x01




/*
TYPEDESCRIPTION	CFloater::m_SaveData[] = 
{
	
};

//IMPLEMENT_SAVERESTORE( CFloater, CFlyingMonster );
int CFloater::Save( CSave &save )
{
	if ( !CFlyingMonster::Save(save) )
		return 0;
	int iWriteFieldsResult = save.WriteFields( "CFloater", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iWriteFieldsResult;
}
int CFloater::Restore( CRestore &restore )
{
	if ( !CFlyingMonster::Restore(restore) )
		return 0;
	int iReadFieldsResult = restore.ReadFields( "CFloater", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return iReadFieldsResult;
}
*/




#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_floater, CFloater );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( floater, CFloater );
	
	#if EXTRA_NAMES == 2
		//NOTICE - example extra name! There may be multiple or no extra names too.
		LINK_ENTITY_TO_CLASS( bloater, CFloater );
		LINK_ENTITY_TO_CLASS( monster_bloater, CFloater );
	#endif
	
#endif



CFloater::CFloater(void){

	m_flightSpeed = 0;
	tempCheckTraceLineBlock = FALSE;
	m_velocity = Vector(0,0,0);

	lastVelocityChange = -1;
	
	hitGroundDead = FALSE;

}//END OF CFloater constructor




//Thank you bullsquid, you know what is up dog.
Task_t	tlFloaterRangeAttack1[] =
{
	{ TASK_STOP_MOVING,			0				},
	{ TASK_FACE_IDEAL,			(float)0		},
	{ TASK_RANGE_ATTACK1,		(float)0		},
	{ TASK_SET_ACTIVITY,		(float)ACT_IDLE	},
};
Schedule_t	slFloaterRangeAttack1[] =
{
	{ 
		tlFloaterRangeAttack1,
		ARRAYSIZE ( tlFloaterRangeAttack1 ), 
		bits_COND_NEW_ENEMY			|
		bits_COND_ENEMY_DEAD		|
		bits_COND_HEAVY_DAMAGE		|
		//bits_COND_ENEMY_OCCLUDED	|
		bits_COND_NO_AMMO_LOADED,   //er, wat?
		0,
		"Floater Range Attack1"
	},
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






DEFINE_CUSTOM_SCHEDULES( CFloater )
{
	slFloaterRangeAttack1,
	//slFloaterYYY,
	//slFloaterZZZ,

};
IMPLEMENT_CUSTOM_SCHEDULES( CFloater, CFlyingMonster );

	









	
void CFloater::DeathSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pDeathSounds), 1.0, ATTN_IDLE, 0, pitch );
}
void CFloater::AlertSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAlertSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CFloater::IdleSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random idle sound
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pIdleSounds), 1.0, ATTN_NORM, 0, pitch );
}
void CFloater::PainSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	if (RANDOM_LONG(0,5) < 2){
		EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pPainSounds), 1.0, ATTN_NORM, 0, pitch );
	}
}
void CFloater::AttackSound( void ){
	int pitch = 95 + RANDOM_LONG(0,9);
	// Play a random attack sound
	EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, RANDOM_SOUND_ARRAY(pAttackSounds), 1.0, ATTN_NORM, 0, pitch );
}















extern int global_useSentenceSave;
void CFloater::Precache( void )
{
	PRECACHE_MODEL("models/floater.mdl");

	global_useSentenceSave = TRUE;
	
	
	//NOTICE - attempting to precace files that don't exist crashes the game.
	/*
	//PRECACHE_SOUND("floater/floater_XXX.wav");
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



void CFloater::Spawn( void )
{
	Precache( );

	setModel("models/floater.mdl");
	//UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );
	UTIL_SetSize( pev, Vector( -8, -8, 0 ), Vector( 8, 8, 28 ));

	pev->classname = MAKE_STRING("monster_floater");

	//SetBits(pev->flags, FL_FLY);
	pev->flags |= FL_FLY;



	
	pev->solid			= SOLID_BBOX;  //not SOLID_SLIDEBOX
	pev->movetype		= MOVETYPE_FLY;


	pev->solid			= SOLID_SLIDEBOX;  //SOLID_TRIGGER?  Difference?
	//pev->movetype		= MOVETYPE_BOUNCEMISSILE;
	


	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->effects		= 0;
	//NOTE - you have to make this exist over in skill.h and handle some other setup (gamerules.cpp, and the CVar in game.cpp)!
	//example: skilldata_t member "scientistHealth" and CVar "sk_scientist_health".
	pev->health			= gSkillData.floaterHealth;
	pev->view_ofs		= VEC_VIEW;/// position of the eyes relative to monster's origin.
	m_flFieldOfView		= VIEW_FIELD_WIDE;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	pev->yaw_speed		= 100;//bound to change often from "SetYawSpeed". Likely meaningless here but a default can't hurt.

	MonsterInit();


	m_flightSpeed = 400;

	SetTouch(&CFloater::CustomTouch );
	//SetTouch( NULL );


}//END OF Spawn();





























	



int CFloater :: CheckLocalMove ( const Vector &vecStart, const Vector &vecEnd, CBaseEntity *pTarget, float *pflDist )
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









	







	/*
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
	
	Vector vecStartAlt = vecStart + vecCenterRel + gpGlobals->v_forward * boundXSize*1.3;
	Vector vecEndAlt = vecEnd + vecCenterRel + -gpGlobals->v_forward * boundXSize*1.3;
	

	DebugLine_ClearAll();

	vecOff = -gpGlobals->v_right * boundYSize*1.3 + gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trTopLeft );
	DebugLine_Setup(0, vecStartAlt+vecOff, vecEndAlt+vecOff, trTopLeft.flFraction);

	vecOff = gpGlobals->v_right * boundYSize*1.3 + gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trTopRight );
	DebugLine_Setup(1, vecStartAlt+vecOff, vecEndAlt+vecOff, trTopRight.flFraction);

	vecOff = -gpGlobals->v_right * boundYSize*1.3 + -gpGlobals->v_up * boundZSize*1.3;
	UTIL_TraceHull( vecStartAlt + vecOff, vecEndAlt + vecOff, dont_ignore_monsters, point_hull, edict(), &trBottomLeft );
	DebugLine_Setup(4, vecStartAlt+vecOff, vecEndAlt+vecOff, trBottomLeft.flFraction);

	vecOff = gpGlobals->v_right * boundYSize*1.3 + -gpGlobals->v_up * boundZSize*1.3;
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

	*/
	

	/*
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
	*/

	
	//if(tracesStartSolid || minFraction < 1.0)





	


	
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
void CFloater::Move( float flInterval )
{
	if ( pev->movetype == MOVETYPE_FLY )
		m_flGroundSpeed = m_flightSpeed;
	CFlyingMonster::Move( flInterval );
}


BOOL CFloater::ShouldAdvanceRoute( float flWaypointDist )
{
	// Get true 3D distance to the goal so we actually reach the correct height
	if ( m_Route[ m_iRouteIndex ].iType & bits_MF_IS_GOAL )
		flWaypointDist = ( m_Route[ m_iRouteIndex ].vecLocation - pev->origin ).Length();

	if ( flWaypointDist <= 64 + (m_flGroundSpeed * gpGlobals->frametime) )
		return TRUE;

	return FALSE;
}





void CFloater::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
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
	m_flGroundSpeed = m_flightSpeed = 10;
	//TEST - just force it?
	//m_flGroundSpeed = m_flightSpeed = 200;
	//this->SetSequenceByIndex(SEQ_FLOATER_TURN_LEFT, 1);

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

void CFloater::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval )
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


void CFloater::SetEyePosition(void){
	CFlyingMonster::SetEyePosition();
	//pev->view_ofs = VEC_VIEW;
}//END OF SetEyePosition




//based off of GetSchedule for CFlyingMonster in schedule.cpp.
Schedule_t* CFloater::GetSchedule ( void )
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
					return GetScheduleOfType( SCHED_CHASE_ENEMY );
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


Schedule_t* CFloater::GetScheduleOfType( int Type){
	
	switch(Type){

		case SCHED_DIE:
			return flierDeathSchedule();
		break;
		case SCHED_RANGE_ATTACK1:
			return slFloaterRangeAttack1;
		break;



	}//END OF switch(Type)
	
	return CFlyingMonster::GetScheduleOfType(Type);
}//END OF GetScheduleOfType


void CFloater::ScheduleChange(){

	Schedule_t* endedSchedule = m_pSchedule;

	//Use this to do something when a schedule ends that must happen at the time of interruption.
	//Depending on what schedule that ended typically, whether it was interrupted by some condition or naturally
	//ran out of tasks.
	
	
	
	
	CFlyingMonster::ScheduleChange(); //Call the parent.

}//END OF ScheduleChange


Schedule_t* CFloater::GetStumpedWaitSchedule(){
	return CFlyingMonster::GetStumpedWaitSchedule();
}//END OF GetStumpedWaitSchedule




void CFloater::StartTask( Task_t *pTask ){


	switch( pTask->iTask ){
		case TASK_FLOATER_XXX:

		break;
		default:
			CFlyingMonster::StartTask( pTask );
		break;
	}//END OF switch

}//END OF StartTask

void CFloater::RunTask( Task_t *pTask ){
	
	//EASY_CVAR_PRINTIF_PRE(templatePrintout, easyPrintLine("RunTask: sched:%s task:%d", this->m_pSchedule->pName, pTask->iTask) );
	
	switch( pTask->iTask ){
		case TASK_FLOATER_XXX:

		break;
		case TASK_DIE_LOOP:{
			if(hitGroundDead){
				TaskComplete();
			}
		break;}
		default:
			CFlyingMonster::RunTask(pTask);
		break;
	}//END OF switch

}//END OF RunTask



BOOL CFloater::CheckMeleeAttack1( float flDot, float flDist ){
	return FALSE;
}
BOOL CFloater::CheckMeleeAttack2( float flDot, float flDist ){
	return FALSE;
}
BOOL CFloater::CheckRangeAttack1( float flDot, float flDist ){

	//DEBUG - why you no work!!!??

	if ( flDot > 0.5 && flDist > 256 && flDist <= 2048 )
	{
		easyForcePrintLine("YAY?!");
		return TRUE;
	}
	easyForcePrintLine("NAY?!");
	return FALSE;
}
BOOL CFloater::CheckRangeAttack2( float flDot, float flDist ){
	return FALSE;
}



void CFloater::CustomTouch( CBaseEntity *pOther ){
	int x = 46;
	if(pOther == NULL){
		return; //??????
	}
}


void CFloater::KilledFallingTouch( CBaseEntity *pOther ){
	
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











void CFloater::MonsterThink(){


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
void CFloater::PrescheduleThink (){



	CFlyingMonster::PrescheduleThink();
}//END OF PrescheduleThink










int	CFloater::Classify(){
	return CLASS_ALIEN_MONSTER;
}
BOOL CFloater::isOrganic(){
	return TRUE;
}

int CFloater::IRelationship( CBaseEntity *pTarget ){

	return CFlyingMonster::IRelationship(pTarget);
}//END OF IRelationship


void CFloater::ReportAIState(){
	//call the parent, and add on to that.
	CFlyingMonster::ReportAIState();
	//print anything special with easyForcePrintLine
}//END OF ReportAIState()





GENERATE_TRACEATTACK_IMPLEMENTATION(CFloater)
{



	GENERATE_TRACEATTACK_PARENT_CALL(CFlyingMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CFloater)
{
	
	//CFlyingMonster already calls PainSound.
	//PainSound();


	return GENERATE_TAKEDAMAGE_PARENT_CALL(CFlyingMonster);
}



//NOTE - called by CFlyingMonster's TakeDamage method. If that isn't called, DeadTakeDamage won't get called naturally.
GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION(CFloater)
{



	return GENERATE_DEADTAKEDAMAGE_PARENT_CALL(CFlyingMonster);
}//END OF DeadTakeDamage




//Parameters: integer named fGibSpawnsDecal
// The parent method calls GIBMONSTERGIB, then GIBMONSTERSOUND and GIBMONSTEREND, passing along to the latter two whether GIBMONSTERGIB
// actually "gibbed" or not (it must return whether it did or not).
// This default behavior is ok, change pieces at a time instead in other methods. GIBMONSTERGIB is the most likely one that needs customization,
// such as only spawning two alien gibs for the ChumToad. The others are good as defaults.
GENERATE_GIBMONSTER_IMPLEMENTATION(CFloater)
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
GENERATE_GIBMONSTERGIB_IMPLEMENTATION(CFloater)
{
	




	return GENERATE_GIBMONSTERGIB_PARENT_CALL(CFlyingMonster);
}

//The other related methods, GIBMONSTERSOUND and GIBMONSTEREND, are well suited to the majority of cases.




GENERATE_KILLED_IMPLEMENTATION(CFloater)
{


	

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



	/*
	//if you have the "FL_KILLME" flag, it means this is about to get deleted (gibbed). No point in doing any of this then.
	if(firstCall && !(pev->flags & FL_KILLME) ){
		cheapKilledFlier();
	}//END OF firstCall check
	*/



}//END OF Killed







void CFloater::SetYawSpeed( void ){
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





BOOL CFloater::getMonsterBlockIdleAutoUpdate(void){
	return FALSE;
}
BOOL CFloater::forceIdleFrameReset(void){
	return FALSE;
}
BOOL CFloater::usesAdvancedAnimSystem(void){
	return TRUE;
}

void CFloater::SetActivity( Activity NewActivity ){
	CFlyingMonster::SetActivity(NewActivity);
}



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

int CFloater::tryActivitySubstitute(int activity){
	int i = 0;

	//no need for default, just falls back to the normal activity lookup.
	switch(activity){
		//Let whoever know we have these anims.
		case ACT_IDLE:
			return SEQ_FLOATER_IDLE1;
		break;
		case ACT_WALK:
		case ACT_RUN:
			return SEQ_FLOATER_IDLE1;
		break;
	}//END OF switch


	//not handled by above? Rely on the model's anim for this activity if there is one.
	return CBaseAnimating::LookupActivity(activity);
}//END OF tryActivitySubstitute



int CFloater::LookupActivityHard(int activity){
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
			return SEQ_FLOATER_IDLE1;
		break;
		case ACT_WALK:
		case ACT_RUN:
			return SEQ_FLOATER_IDLE1;
		break;
		case ACT_TURN_LEFT:
		case ACT_TURN_RIGHT:
			m_iForceLoops = TRUE;
			//otherwise what is returned is fine.
		break;
		case ACT_RANGE_ATTACK1:
			//wait, it has this mapped already. Put it here if this ever changes.

		break;

	}//END OF switch
	
	
	//not handled by above?  try the real deal.
	return CBaseAnimating::LookupActivity(activity);
}//END OF LookupActivityHard



//Handles custom events sent from "LookupActivityHard", which sends events as timed delays along with picking an animation in script.
//So this handles script-provided events, not model ones.
void CFloater::HandleEventQueueEvent(int arg_eventID){

	switch(arg_eventID){
		//fire the ball.
	case 0:
	{


	break;
	}
	case 1:
	{


	break;
	}
	}//END OF switch


}//END OF HandleEventQueueEvent


//This handles events built into the model, not custom hard-coded ones (above).
void CFloater::HandleAnimEvent(MonsterEvent_t *pEvent ){
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
void CFloater::checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist){
	checkTraceLine(vecSuggestedDir, travelMag, flInterval, vecStart, vecRelativeEnd, moveDist, TRUE);
}
//Vector& const vecRelstar, ???   Vector& const reactionMove,
//inline
void CFloater::checkTraceLine(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture){
	

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
void CFloater::checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist){
	checkTraceLineTest(vecSuggestedDir, travelMag, flInterval, vecStart, vecRelativeEnd, moveDist, TRUE);
}
//Vector& const vecRelstar, ???   Vector& const reactionMove,
inline
void CFloater::checkTraceLineTest(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval, const Vector& vecStart, const Vector& vecRelativeEnd, const int& moveDist, const BOOL canBlockFuture){
	
	
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









void CFloater::checkFloor(const Vector& vecSuggestedDir, const float& travelMag, const float& flInterval){

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
void CFloater::OnKilledSetTouch(void){
	SetTouch(&CFloater::KilledFallingTouch);
}

int CFloater::getLoopingDeathSequence(void){
	return SEQ_FLOATER_FALL_LOOP;
}



