//=========================================================
// Controller bouncy ball attack
//=========================================================

//Originally in controller.cpp, split into its own file for re-use in other files. Good generic following (crudely) sphere of doom.

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
	void MovetoTarget( Vector vecTarget );
	void Crawl( void );

	float massInfluence(void);

	float nextNormalThinkTime;
	int m_iTrail;
	int m_flNextAttack;
	Vector m_vecIdeal;
	EHANDLE m_hOwner;


};





