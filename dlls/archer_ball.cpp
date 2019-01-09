
#include "archer_ball.h"
#include "weapons.h"





LINK_ENTITY_TO_CLASS( archer_ball, CArcherBall );

CArcherBall::CArcherBall(void){

}



void CArcherBall :: Spawn( void ){

	CControllerHeadBall::Spawn();

	
	SetTouch( &CArcherBall::BounceTouch );


}//END OF Spawn

void CArcherBall :: Precache( void ){

	CControllerHeadBall::Precache();


}//END OF Precache


//MODDD - little less reactive to following th enemy.
void CArcherBall :: MovetoTarget( Vector vecTarget ){
	// accelerate
	velocityCheck(420);

	//change is here, times 100 at the end was the old way.
	m_vecIdeal = m_vecIdeal + (vecTarget - pev->origin).Normalize() * 85;
	pev->velocity = m_vecIdeal;
}


//MODDD - clone of controller_head_ball's.  Can modify beyond if needed.
void CArcherBall::BounceTouch( CBaseEntity *pOther )
{
	Vector vecDir = m_vecIdeal.Normalize( );

	TraceResult tr = UTIL_GetGlobalTrace( );

	float n = -DotProduct(tr.vecPlaneNormal, vecDir);

	//vecDir = 2.0 * tr.vecPlaneNormal * n + vecDir;
	vecDir = 1.4 * tr.vecPlaneNormal * n + vecDir;

	m_vecIdeal = vecDir * m_vecIdeal.Length();
}



float CArcherBall::nearZapDamage(void){
	//TODO - custom damage for me?
	return gSkillData.controllerDmgZap * 0.66;
}


//How much opacity do I lose per think cycle (0.1 seconds)?  Slow it down to last longer.
float CArcherBall::getFadeOutAmount(void){
	return 4.25;
}


