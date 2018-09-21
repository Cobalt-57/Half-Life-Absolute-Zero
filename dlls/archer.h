
//=========================================================
// ARCHER (archer) (split into .h & .cpp for prototype & implementation details accordingly)
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


#ifndef ARCHER_H
#define ARCHER_H


class CArcher : public CBaseMonster{
public:
	
	CUSTOM_SCHEDULES;
	
	//save info
	//////////////////////////////////////////////////////////////////////////////////
	virtual int		Save( CSave &save ); 
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];
	//////////////////////////////////////////////////////////////////////////////////



	
};//END OF class CArcher


#endif //END OF #ifdef ARCHER_H
