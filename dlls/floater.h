
//=========================================================
// FLOATER (floater) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"
#include	"activity.h"
#include	"animation.h"

#include "defaultai.h"
#include "soundent.h"
#include "game.h"

#include "basemonster.h"


#ifndef FLOATER_H
#define FLOATER_H


class CFloater : public CBaseMonster{
public:
	
	CUSTOM_SCHEDULES;
	
	//save info
	//////////////////////////////////////////////////////////////////////////////////
	virtual int		Save( CSave &save ); 
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];
	//////////////////////////////////////////////////////////////////////////////////



	
};//END OF class CFloater



#endif //END OF #ifdef FLOATER_H
