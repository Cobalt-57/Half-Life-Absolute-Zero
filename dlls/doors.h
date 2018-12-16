/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/

//MODDD - CBaseDoor class moved to here, other door classes could be moved here too.

#ifndef DOORS_H
#define DOORS_H


#include "basetoggle.h"


// doors
//MODDD - this spawnflag is... completely unused. 0? How can that be included or excluded by anything?
//        The constant isn't used at least. Just weird.
//#define SF_DOOR_ROTATE_Y			0

#define	SF_DOOR_START_OPEN			1
#define SF_DOOR_ROTATE_BACKWARDS	2
//                                  4 was unused.  But it might still be some other higher entity flag, who knows.
#define SF_DOOR_PASSABLE			8
#define SF_DOOR_ONEWAY				16
#define	SF_DOOR_NO_AUTO_RETURN		32
#define SF_DOOR_ROTATE_Z			64
#define SF_DOOR_ROTATE_X			128
#define SF_DOOR_USE_ONLY			256	// door must be opened by player's use button.
#define SF_DOOR_NOMONSTERS			512	// Monster can't open

//IMPORTANT NOTE - Bits #10 to 12 (powers of 2 1024, 2048 and 4096) are seemingly randomly given to func_door_rotation's
//                 across several recovered maps. It may be best to avoid using these as bits 13 to 30 were tested and 
//                 appear to be completely unused.
//                 This was not tested for plain func_door's or any other type of entity.
//                 It is unknown whether there are similar cases of garbage flags completely unreferenced by script in
//                 retail / the original SDK.  Accidentally using those to mean something could mean random entities
//                 take their effects without screening each one.


//MODDD - new
//#define SF_DOOR_HEAL				1024
#define SF_DOOR_HEAL				(1 << 13)  //or 8192.

#define SF_DOOR_SILENT				0x80000000






class CBaseDoor : public CBaseToggle
{
public:
	
	CBaseDoor(void);


	void Spawn( void );
	void Precache( void );
	virtual void KeyValue( KeyValueData *pkvd );


	virtual BOOL IsWorldAffiliated(void);

	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual void Blocked( CBaseEntity *pOther );


	virtual int	ObjectCaps( void ) 
	{ 
		if (pev->spawnflags & SF_ITEM_USE_ONLY)
			return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION) | FCAP_IMPULSE_USE;
		else
			return (CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION);
	};
	virtual int	Save( CSave &save );
	virtual int	Restore( CRestore &restore );

	static	TYPEDESCRIPTION m_SaveData[];

	virtual void SetToggleState( int state );

	// used to selectivly override defaults
	void EXPORT DoorTouch( CBaseEntity *pOther );

	//MODDD - new
	virtual void AngularMove( Vector vecDestAngle, float flSpeed );

	// local functions
	int DoorActivate( );
	void EXPORT DoorGoUp( void );
	void EXPORT DoorGoDown( void );
	void EXPORT DoorHitTop( void );
	void EXPORT DoorHitBottom( void );

	//MODDD - event methods for doing something in particular on hitting the top or bottom, so long as the base door's DoorHitTop and DoorHitBottom are used.
	//The event (EXPORT'd) methods above call these instead so that child classes just have to override the "On" versions instead.
	virtual void OnDoorGoUp(void);
	virtual void OnDoorHitTop(void);
	virtual void OnDoorGoDown(void);
	virtual void OnDoorHitBottom(void);
	
	BYTE	m_bHealthValue;// some doors are medi-kit doors, they give players health
	
	BYTE	m_bMoveSnd;			// sound a door makes while moving
	BYTE	m_bStopSnd;			// sound a door makes when it stops

	locksound_t m_ls;			// door lock sounds
	
	BYTE	m_bLockedSound;		// ordinals from entity selection
	BYTE	m_bLockedSentence;	
	BYTE	m_bUnlockedSound;	
	BYTE	m_bUnlockedSentence;
};







#endif		//DOORS_H
