//=========================================================
// Controller bouncy ball attack
//=========================================================

//Originally in controller.cpp, split into its own file for re-use in other files. Good generic following (crudely) sphere of doom.

#ifndef CONTROLLER_HEAD_BALL_H
#define CONTROLLER_HEAD_BALL_H

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"

class CControllerHeadBall : public CBaseMonster
{
	//MODDD - public
public:
	CControllerHeadBall(void);

	void Spawn( void );
	void Precache( void );
	void EXPORT HuntThink( void );
	void EXPORT DieThink( void );
	void EXPORT BounceTouch( CBaseEntity *pOther );

	virtual BOOL usesSegmentedMove(void);

	virtual void MovetoTarget( Vector vecTarget );

	virtual void Crawl( void );

	virtual float massInfluence(void);
	virtual int GetProjectileType(void);

	virtual float nearZapDamage(void);
	virtual float getFadeOutAmount(void);

	void velocityCheck(const float& arg_maxSpeed);

	float nextNormalThinkTime;
	int m_iTrail;
	int m_flNextAttack;
	Vector m_vecIdeal;
	EHANDLE m_hOwner;
};


#endif //CONTROLLER_HEAD_BALL_H