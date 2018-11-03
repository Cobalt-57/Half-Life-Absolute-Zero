//=========================================================
// CControllerZapBall
//=========================================================

//Originally in controller.cpp, split into its own file for re-use in other files. Good generic dumb-fire sphere of doom.

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"

class CControllerZapBall : public CBaseMonster
{
	//MODDD - why no public?
public:
	CControllerZapBall(void);

	void Spawn( void );
	void Precache( void );
	void EXPORT AnimateThink( void );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );

	EHANDLE m_hOwner;
};

