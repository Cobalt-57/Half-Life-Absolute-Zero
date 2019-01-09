
#include "kingpin_ball.h"
#include "weapons.h"



extern BOOL map_anyAirNodes;

EASY_CVAR_EXTERN(pathfindNodeToleranceMulti)
EASY_CVAR_EXTERN(drawDebugPathfinding)


LINK_ENTITY_TO_CLASS( kingpin_ball, CKingpinBall );

CKingpinBall::CKingpinBall(void){
	
	pathfind_RouteRetryTime = -1;
	pathfind_onRoute = FALSE;
	pathfind_consecutiveFails = 0;
}


void CKingpinBall :: Spawn( void ){

	CControllerHeadBall::Spawn();



	
	//MODDD - in any case do these instead.
	//SetThink( &CKingpinBall::HuntThink );
	SetTouch( &CKingpinBall::BounceTouch );


	ClearConditions(bits_COND_TASK_FAILED);

	pathfind_onRoute = FALSE;

	if(map_anyAirNodes){
		//Use a different type of think to mimic 
		
		if(m_hEnemy != NULL){
			//this once.
			m_vecEnemyLKP = m_hEnemy->pev->origin;
		}

		startSmartFollow();

	}else{

		//nothing out of the ordinary.

	}



}//END OF Spawn

void CKingpinBall :: Precache( void ){

	CControllerHeadBall::Precache();


}//END OF Precache


//MODDD - little less reactive to following th enemy.
void CKingpinBall :: MovetoTarget( Vector vecTarget ){

	float distToEnemy = 0;

	if(m_hEnemy != NULL){
		//I'm a crazy ball. I know where the enemy is.
		m_vecEnemyLKP = m_hEnemy->pev->origin;
	}
	distToEnemy = (m_vecEnemyLKP - pev->origin).Length();


	//This gets called routely so treat it as a Think() method.


	

	if(pathfind_onRoute){
		const BOOL movementCompleted = MovementIsComplete();
		const BOOL taskFailed = HasConditions(bits_COND_TASK_FAILED);

		if(taskFailed || movementCompleted){

			//need to re-route?
			pathfind_onRoute = FALSE;
			//pathfind_RouteRetryTime = gpGlobals->time + 2.0f;
		
			//try again this second?
			startSmartFollow();
		}

	}//END OF pathfind_onRoute


	if(pathfind_onRoute){
		//Did our enemy move too far from our goal?
		const float enemyDistanceToGoal = (m_vecEnemyLKP - m_vecMoveGoal).Length();


		//2 miles away.
		//enemy moved 0.3 miles away from the goal from an ealier point.

		if(distToEnemy * 0.2 <= enemyDistanceToGoal){
			//If the distance between my target and my recent pathfind goal (point) exceeds 20% of the toal distance between me and the target, reroute.
			startSmartFollow();
		}

	}


	if(pathfind_RouteRetryTime != -1 && gpGlobals->time >= pathfind_RouteRetryTime){
		pathfind_RouteRetryTime = -1;
		
		//see if I can resume?
		startSmartFollow();

		//pathfind_RouteRetryTime = gpGlobals->time + 2.0f;
	}






	if(pathfind_onRoute){
		//move to the next route piece.
		pathfind_move(distToEnemy);

	}else{
		float maxSpeed = 800;
		BOOL approachingTarget;
		if(distToEnemy <= 600){
			approachingTarget = TRUE;
		}else{
			approachingTarget = FALSE;
		}
		
		if(approachingTarget){
			maxSpeed = 400; //instead, if going right towards the enemy.
		}

		// accelerate
		velocityCheck(maxSpeed);


		//change is here, times 400 at the end was the old way.
		m_vecIdeal = m_vecIdeal + (vecTarget - pev->origin).Normalize() * 320;
		pev->velocity = m_vecIdeal;
	}


}//END OF MovetoTarget



void CKingpinBall::pathfind_move(const float& arg_distToEnemy){
	
	//go to the next place in the route instead.
	const Vector vecTarget = m_Route[ m_iRouteIndex ].vecLocation;

	//m_vecIdeal = m_vecIdeal + (vecTarget - pev->origin).Normalize() * 320;
	//pev->velocity = m_vecIdeal;
	float maxSpeed = 800;
	BOOL approachingTarget;


	//if(!MovementIsComplete()){

		drawPathConstant = TRUE; //force me.

	    CBaseMonster::Move();
		//???
		//DrawRoute( pev, m_Route, m_iRouteIndex, 0, 0, 176 );
	//}
	
	velocityCheck(420);

	if(m_Route[ m_iRouteIndex ].iType & bits_MF_IS_GOAL || arg_distToEnemy <= 600){
		approachingTarget = TRUE;
	}else{
		approachingTarget = FALSE;
	}
	
	if(approachingTarget){
		maxSpeed = 340; //instead, if going right towards the enemy.
	}


	// accelerate
	velocityCheck(maxSpeed);



	if(approachingTarget){
		//if this end point is a "goal", our enemy is there.  Move less accurately to give them a fair chance of evading me.
		//(Hollywood logic)
		m_vecIdeal = m_vecIdeal * 1.0 + (vecTarget - pev->origin).Normalize() * 230;
		pev->velocity = m_vecIdeal;
	}else{
		m_vecIdeal = m_vecIdeal * 0.3 + (vecTarget - pev->origin).Normalize() * 380;
		pev->velocity = m_vecIdeal;
	}






}//END OF pathfind_think





float CKingpinBall::nearZapDamage(void){
	//TODO - custom damage for me?
	return gSkillData.controllerDmgZap * 0.4;
}



void CKingpinBall::startSmartFollow(void){


	
	if(m_hEnemy == NULL){
		//what??
		//TaskFail();
		return;
	}



	//if ( (m_hEnemy->pev->origin - pev->origin).Length() < 1 )
	
	/*
	if ( (m_vecEnemyLKP - pev->origin).Length() < 1 ){
		//aren't we there then?

	}else
	*/
	{
		BOOL test = FRefreshRouteChaseEnemySmart();

		
		if(!test){
			//fail? huh.
			
			pathfind_RouteRetryTime = gpGlobals->time + 1.0f;
			pathfind_onRoute = FALSE;

		}else{
			ClearConditions(bits_COND_TASK_FAILED);
			pathfind_onRoute = TRUE;
			pathfind_RouteRetryTime = -1;  //no need for retries.
		}
	}


}//END OF startSmartFollow



int CKingpinBall::getHullIndexForNodes(void){
	return NODE_POINT_HULL;
}


//MODDD - clone of controller_head_ball's.  Can modify beyond if needed.
void CKingpinBall::BounceTouch( CBaseEntity *pOther )
{
	Vector vecDir = m_vecIdeal.Normalize( );

	TraceResult tr = UTIL_GetGlobalTrace( );

	float n = -DotProduct(tr.vecPlaneNormal, vecDir);

	//vecDir = 2.0 * tr.vecPlaneNormal * n + vecDir;
	vecDir = 0.2 * tr.vecPlaneNormal * n + vecDir;

	m_vecIdeal = vecDir * m_vecIdeal.Length();
}





void CKingpinBall::MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval ){

	//Copy of flymonster.cpp's MoveExecute script if anything is salvagable.
	//Mainly we don't want any direction from the Move method call to interfere with out movement that's going fine.
	//leaving this completely blank may be fine.

	/*
	if ( isMovetypeFlying() )
	{
		if ( gpGlobals->time - m_stopTime > 1.0 )
		{
			if ( m_IdealActivity != m_movementActivity )
			{
				m_IdealActivity = m_movementActivity;
				m_flGroundSpeed = m_flightSpeed = 200;
			}
		}
		Vector vecMove = pev->origin + (( vecDir + (m_vecTravel * m_momentum) ).Normalize() * (m_flGroundSpeed * flInterval));

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
			UTIL_MoveToOrigin(ENT(pev), vecMove, (m_flGroundSpeed * flInterval), MOVE_STRAFE);
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
	*/
}


BOOL CKingpinBall::ShouldAdvanceRoute( float flWaypointDist, float flInterval ){

	const float velocityLength = pev->velocity.Length();
	const float anticipatedMovement = velocityLength * flInterval * 1.6f;

	const float moveTolerance = max(anticipatedMovement * EASY_CVAR_GET(pathfindNodeToleranceMulti), 20.0f);

	
	if(flWaypointDist <= moveTolerance )
	{
		// ALERT( at_console, "cut %f\n", flWaypointDist );
		return TRUE;
	}
	return FALSE;


	//const float moveDistTest = rawMoveSpeedPerSec * EASY_CVAR_GET(pathfindNodeToleranceMulti);  //defaults to 1 for no effect.
	//const float moveDistTol = max(moveDistTest, 8);  //must be at least 8.



}//END OF ShouldAdvanceRoute


float CKingpinBall::MoveYawDegreeTolerance(void){
	//I can turn anywhere. I am just a ball.
	return -1;

	//return 360;
}

void CKingpinBall::Stop(void){
	//I do nothing!

}


void CKingpinBall::SetActivity(Activity NewActivity){
	//I am only a ball!

}




int CKingpinBall::CheckLocalMove( const Vector& vecStart, const Vector& vecEnd, CBaseEntity* pTarget, float* pflDist ){
	int iReturn;


	TraceResult tr;
	Vector vecStartTrace = vecStart + Vector( 0, 0, 0 );

	//UTIL_TraceHull( vecStart + Vector( 0, 0, 32 ), vecEnd + Vector( 0, 0, 32 ), dont_ignore_monsters, large_hull, edict(), &tr );

	//MODDD - large_hull is probably the safest for flyers in general.  point_hull if using the bounce system. or maybe head_hull?
	UTIL_TraceHull( vecStartTrace, vecEnd + Vector( 0, 0, 0), dont_ignore_monsters, point_hull, edict(), &tr );
	
	// ALERT( at_console, "%.0f %.0f %.0f : ", vecStart.x, vecStart.y, vecStart.z );
	// ALERT( at_console, "%.0f %.0f %.0f\n", vecEnd.x, vecEnd.y, vecEnd.z );

	if (pflDist)
	{
		*pflDist = ( (tr.vecEndPos ) - vecStartTrace ).Length();// get the distance.
	}
	

	if(tr.fStartSolid){
		//what??
		return LOCALMOVE_VALID;
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


//How much opacity do I lose per think cycle (0.1 seconds)?  Slow it down to last longer.
float CKingpinBall::getFadeOutAmount(void){
	return 2.6;
}


