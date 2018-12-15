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
/*

===== doors.cpp ========================================================

*/

#include "extdll.h"

#include "doors.h"

#include "util.h"


#include "basetoggle.h"
#include "basebutton.h"

#include "player.h"


#include "healthmodule.h"


//last? what?
//#include "doors.h"


extern void SetMovedir(entvars_t* ev);

#define noiseMoving noise1
#define noiseArrived noise2


TYPEDESCRIPTION	CBaseDoor::m_SaveData[] = 
{
	DEFINE_FIELD( CBaseDoor, m_bHealthValue, FIELD_CHARACTER ),
	DEFINE_FIELD( CBaseDoor, m_bMoveSnd, FIELD_CHARACTER ),
	DEFINE_FIELD( CBaseDoor, m_bStopSnd, FIELD_CHARACTER ),
	
	DEFINE_FIELD( CBaseDoor, m_bLockedSound, FIELD_CHARACTER ),
	DEFINE_FIELD( CBaseDoor, m_bLockedSentence, FIELD_CHARACTER ),
	DEFINE_FIELD( CBaseDoor, m_bUnlockedSound, FIELD_CHARACTER ),	
	DEFINE_FIELD( CBaseDoor, m_bUnlockedSentence, FIELD_CHARACTER ),	

};

IMPLEMENT_SAVERESTORE( CBaseDoor, CBaseToggle );


#define DOOR_SENTENCEWAIT	6
#define DOOR_SOUNDWAIT		3
#define BUTTON_SOUNDWAIT	0.5

// play door or button locked or unlocked sounds. 
// pass in pointer to valid locksound struct. 
// if flocked is true, play 'door is locked' sound,
// otherwise play 'door is unlocked' sound
// NOTE: this routine is shared by doors and buttons

void PlayLockSounds(entvars_t *pev, locksound_t *pls, int flocked, int fbutton)
{
	// LOCKED SOUND
	
	// CONSIDER: consolidate the locksound_t struct (all entries are duplicates for lock/unlock)
	// CONSIDER: and condense this code.
	float flsoundwait;

	if (fbutton)
		flsoundwait = BUTTON_SOUNDWAIT;
	else
		flsoundwait = DOOR_SOUNDWAIT;

	if (flocked)
	{
		int fplaysound = (pls->sLockedSound && gpGlobals->time > pls->flwaitSound);
		int fplaysentence = (pls->sLockedSentence && !pls->bEOFLocked && gpGlobals->time > pls->flwaitSentence);
		float fvol;

		if (fplaysound && fplaysentence)
			fvol = 0.25;
		else
			fvol = 1.0;

		// if there is a locked sound, and we've debounced, play sound
		if (fplaysound)
		{
			// play 'door locked' sound
			EMIT_SOUND(ENT(pev), CHAN_ITEM, (char*)STRING(pls->sLockedSound), fvol, ATTN_NORM);
			pls->flwaitSound = gpGlobals->time + flsoundwait;
		}

		// if there is a sentence, we've not played all in list, and we've debounced, play sound
		if (fplaysentence)
		{
			// play next 'door locked' sentence in group
			int iprev = pls->iLockedSentence;
			
			pls->iLockedSentence = SENTENCEG_PlaySequentialSz(ENT(pev), STRING(pls->sLockedSentence), 
					  0.85, ATTN_NORM, 0, 100, pls->iLockedSentence, FALSE);
			pls->iUnlockedSentence = 0;

			// make sure we don't keep calling last sentence in list
			pls->bEOFLocked = (iprev == pls->iLockedSentence);
		
			pls->flwaitSentence = gpGlobals->time + DOOR_SENTENCEWAIT;
		}
	}
	else
	{
		// UNLOCKED SOUND

		int fplaysound = (pls->sUnlockedSound && gpGlobals->time > pls->flwaitSound);
		int fplaysentence = (pls->sUnlockedSentence && !pls->bEOFUnlocked && gpGlobals->time > pls->flwaitSentence);
		float fvol;

		// if playing both sentence and sound, lower sound volume so we hear sentence
		if (fplaysound && fplaysentence)
			fvol = 0.25;
		else
			fvol = 1.0;

		// play 'door unlocked' sound if set
		if (fplaysound)
		{
			EMIT_SOUND(ENT(pev), CHAN_ITEM, (char*)STRING(pls->sUnlockedSound), fvol, ATTN_NORM);
			pls->flwaitSound = gpGlobals->time + flsoundwait;
		}

		// play next 'door unlocked' sentence in group
		if (fplaysentence)
		{
			int iprev = pls->iUnlockedSentence;
			
			pls->iUnlockedSentence = SENTENCEG_PlaySequentialSz(ENT(pev), STRING(pls->sUnlockedSentence), 
					  0.85, ATTN_NORM, 0, 100, pls->iUnlockedSentence, FALSE);
			pls->iLockedSentence = 0;

			// make sure we don't keep calling last sentence in list
			pls->bEOFUnlocked = (iprev == pls->iUnlockedSentence);
			pls->flwaitSentence = gpGlobals->time + DOOR_SENTENCEWAIT;
		}
	}
}

//
// Cache user-entity-field values until spawn is called.
//

void CBaseDoor::KeyValue( KeyValueData *pkvd )
{

	if (FStrEq(pkvd->szKeyName, "skin"))//skin is used for content type
	{
		pev->skin = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "movesnd"))
	{
		m_bMoveSnd = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "stopsnd"))
	{
		m_bStopSnd = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "healthvalue"))
	{
		m_bHealthValue = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "locked_sound"))
	{
		m_bLockedSound = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "locked_sentence"))
	{
		m_bLockedSentence = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "unlocked_sound"))
	{
		m_bUnlockedSound = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "unlocked_sentence"))
	{
		m_bUnlockedSentence = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "WaveHeight"))
	{
		pev->scale = atof(pkvd->szValue) * (1.0/8.0);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

BOOL CBaseDoor::IsWorldAffiliated(){
	return TRUE;
}

/*QUAKED func_door (0 .5 .8) ? START_OPEN x DOOR_DONT_LINK TOGGLE
if two doors touch, they are assumed to be connected and operate as a unit.

TOGGLE causes the door to wait in both the start and end states for a trigger event.

START_OPEN causes the door to move to its destination when spawned, and operate in reverse.
It is used to temporarily or permanently close off an area when triggered (not usefull for
touch or takedamage doors).

"angle"         determines the opening direction
"targetname"	if set, no touch field will be spawned and a remote button or trigger
				field activates the door.
"health"        if set, door must be shot open
"speed"         movement speed (100 default)
"wait"          wait before returning (3 default, -1 = never return)
"lip"           lip remaining at end of move (8 default)
"dmg"           damage to inflict when blocked (2 default)
"sounds"
0)      no sound
1)      stone
2)      base
3)      stone chain
4)      screechy metal
*/

LINK_ENTITY_TO_CLASS( func_door, CBaseDoor );
//
// func_water - same as a door. 
//
LINK_ENTITY_TO_CLASS( func_water, CBaseDoor );



CBaseDoor::CBaseDoor(void){

}//END OF CBaseDoor constructor


void CBaseDoor::Spawn( )
{
	Precache();
	SetMovedir (pev);

	if ( pev->skin == 0 )
	{//normal door
		if ( FBitSet (pev->spawnflags, SF_DOOR_PASSABLE) )
			pev->solid		= SOLID_NOT;
		else
			pev->solid		= SOLID_BSP;
	}
	else
	{// special contents
		pev->solid		= SOLID_NOT;
		SetBits( pev->spawnflags, SF_DOOR_SILENT );	// water is silent for now
	}

	pev->movetype	= MOVETYPE_PUSH;
	UTIL_SetOrigin(pev, pev->origin);
	SET_MODEL( ENT(pev), STRING(pev->model) );
	
	if (pev->speed == 0)
		pev->speed = 100;
	
	m_vecPosition1	= pev->origin;
	// Subtract 2 from size because the engine expands bboxes by 1 in all directions making the size too big
	m_vecPosition2	= m_vecPosition1 + (pev->movedir * (fabs( pev->movedir.x * (pev->size.x-2) ) + fabs( pev->movedir.y * (pev->size.y-2) ) + fabs( pev->movedir.z * (pev->size.z-2) ) - m_flLip));
	ASSERTSZ(m_vecPosition1 != m_vecPosition2, "door start/end positions are equal");
	if ( FBitSet (pev->spawnflags, SF_DOOR_START_OPEN) )
	{	// swap pos1 and pos2, put door at pos2
		UTIL_SetOrigin(pev, m_vecPosition2);
		m_vecPosition2 = m_vecPosition1;
		m_vecPosition1 = pev->origin;
	}

	m_toggle_state = TS_AT_BOTTOM;
	
	// if the door is flagged for USE button activation only, use NULL touch function
	if ( FBitSet ( pev->spawnflags, SF_DOOR_USE_ONLY ) )
	{
		SetTouch ( NULL );
	}
	else // touchable button
		SetTouch( &CBaseDoor::DoorTouch );
}
 

void CBaseDoor :: SetToggleState( int state )
{
	if ( state == TS_AT_TOP )
		UTIL_SetOrigin( pev, m_vecPosition2 );
	else
		UTIL_SetOrigin( pev, m_vecPosition1 );
}


void CBaseDoor::Precache( void )
{
	char *pszSound;

// set the door's "in-motion" sound
	switch (m_bMoveSnd)
	{
	case	0:
		pev->noiseMoving = ALLOC_STRING("common/null.wav");
		break;
	case	1:
		PRECACHE_SOUND ("doors/doormove1.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove1.wav");
		break;
	case	2:
		PRECACHE_SOUND ("doors/doormove2.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove2.wav");
		break;
	case	3:
		PRECACHE_SOUND ("doors/doormove3.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove3.wav");
		break;
	case	4:
		PRECACHE_SOUND ("doors/doormove4.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove4.wav");
		break;
	case	5:
		PRECACHE_SOUND ("doors/doormove5.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove5.wav");
		break;
	case	6:
		PRECACHE_SOUND ("doors/doormove6.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove6.wav");
		break;
	case	7:
		PRECACHE_SOUND ("doors/doormove7.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove7.wav");
		break;
	case	8:
		PRECACHE_SOUND ("doors/doormove8.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove8.wav");
		break;
	case	9:
		PRECACHE_SOUND ("doors/doormove9.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove9.wav");
		break;
	case	10:
		PRECACHE_SOUND ("doors/doormove10.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove10.wav");
		break;
	default:
		pev->noiseMoving = ALLOC_STRING("common/null.wav");
		break;
	}

// set the door's 'reached destination' stop sound
	switch (m_bStopSnd)
	{
	case	0:
		pev->noiseArrived = ALLOC_STRING("common/null.wav");
		break;
	case	1:
		PRECACHE_SOUND ("doors/doorstop1.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop1.wav");
		break;
	case	2:
		PRECACHE_SOUND ("doors/doorstop2.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop2.wav");
		break;
	case	3:
		PRECACHE_SOUND ("doors/doorstop3.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop3.wav");
		break;
	case	4:
		PRECACHE_SOUND ("doors/doorstop4.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop4.wav");
		break;
	case	5:
		PRECACHE_SOUND ("doors/doorstop5.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop5.wav");
		break;
	case	6:
		PRECACHE_SOUND ("doors/doorstop6.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop6.wav");
		break;
	case	7:
		PRECACHE_SOUND ("doors/doorstop7.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop7.wav");
		break;
	case	8:
		PRECACHE_SOUND ("doors/doorstop8.wav");
		pev->noiseArrived = ALLOC_STRING("doors/doorstop8.wav");
		break;
	default:
		pev->noiseArrived = ALLOC_STRING("common/null.wav");
		break;
	}

	// get door button sounds, for doors which are directly 'touched' to open

	if (m_bLockedSound)
	{
		pszSound = ButtonSound( (int)m_bLockedSound );
		PRECACHE_SOUND(pszSound);
		m_ls.sLockedSound = ALLOC_STRING(pszSound);
	}

	if (m_bUnlockedSound)
	{
		pszSound = ButtonSound( (int)m_bUnlockedSound );
		PRECACHE_SOUND(pszSound);
		m_ls.sUnlockedSound = ALLOC_STRING(pszSound);
	}

	// get sentence group names, for doors which are directly 'touched' to open

	switch (m_bLockedSentence)
	{
		case 1: m_ls.sLockedSentence = ALLOC_STRING("NA"); break; // access denied
		case 2: m_ls.sLockedSentence = ALLOC_STRING("ND"); break; // security lockout
		case 3: m_ls.sLockedSentence = ALLOC_STRING("NF"); break; // blast door
		case 4: m_ls.sLockedSentence = ALLOC_STRING("NFIRE"); break; // fire door
		case 5: m_ls.sLockedSentence = ALLOC_STRING("NCHEM"); break; // chemical door
		case 6: m_ls.sLockedSentence = ALLOC_STRING("NRAD"); break; // radiation door
		case 7: m_ls.sLockedSentence = ALLOC_STRING("NCON"); break; // gen containment
		case 8: m_ls.sLockedSentence = ALLOC_STRING("NH"); break; // maintenance door
		case 9: m_ls.sLockedSentence = ALLOC_STRING("NG"); break; // broken door
		
		default: m_ls.sLockedSentence = 0; break;
	}

	switch (m_bUnlockedSentence)
	{
		case 1: m_ls.sUnlockedSentence = ALLOC_STRING("EA"); break; // access granted
		case 2: m_ls.sUnlockedSentence = ALLOC_STRING("ED"); break; // security door
		case 3: m_ls.sUnlockedSentence = ALLOC_STRING("EF"); break; // blast door
		case 4: m_ls.sUnlockedSentence = ALLOC_STRING("EFIRE"); break; // fire door
		case 5: m_ls.sUnlockedSentence = ALLOC_STRING("ECHEM"); break; // chemical door
		case 6: m_ls.sUnlockedSentence = ALLOC_STRING("ERAD"); break; // radiation door
		case 7: m_ls.sUnlockedSentence = ALLOC_STRING("ECON"); break; // gen containment
		case 8: m_ls.sUnlockedSentence = ALLOC_STRING("EH"); break; // maintenance door
		
		default: m_ls.sUnlockedSentence = 0; break;
	}
}

//
// Doors not tied to anything (e.g. button, another door) can be touched, to make them activate.
//
void CBaseDoor::DoorTouch( CBaseEntity *pOther )
{
	entvars_t*	pevToucher = pOther->pev;
	
	// Ignore touches by anything but players
	if (!FClassnameIs(pevToucher, "player"))
		return;

	// If door has master, and it's not ready to trigger, 
	// play 'locked' sound

	if (m_sMaster && !UTIL_IsMasterTriggered(m_sMaster, pOther))
		PlayLockSounds(pev, &m_ls, TRUE, FALSE);
	
	// If door is somebody's target, then touching does nothing.
	// You have to activate the owner (e.g. button).
	
	if (!FStringNull(pev->targetname))
	{
		// play locked sound
		PlayLockSounds(pev, &m_ls, TRUE, FALSE);
		return; 
	}
	
	m_hActivator = pOther;// remember who activated the door

	if (DoorActivate( ))
		SetTouch( NULL ); // Temporarily disable the touch function, until movement is finished.
}


//
// Used by SUB_UseTargets, when a door is the target of a button.
//
void CBaseDoor::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	m_hActivator = pActivator;
	// if not ready to be used, ignore "use" command.
	if (m_toggle_state == TS_AT_BOTTOM || FBitSet(pev->spawnflags, SF_DOOR_NO_AUTO_RETURN) && m_toggle_state == TS_AT_TOP)
		DoorActivate();
}







void CBaseDoor :: AngularMove( Vector vecDestAngle, float flSpeed )
{
	CBaseToggle::AngularMove(vecDestAngle, flSpeed);
}//END OF AngularMove















//
// Causes the door to "do its thing", i.e. start moving, and cascade activation.
//
int CBaseDoor::DoorActivate( )
{
	if (!UTIL_IsMasterTriggered(m_sMaster, m_hActivator))
		return 0;

	if (FBitSet(pev->spawnflags, SF_DOOR_NO_AUTO_RETURN) && m_toggle_state == TS_AT_TOP)
	{// door should close
		DoorGoDown();
	}
	else
	{// door should open

		if ( m_hActivator != NULL && m_hActivator->IsPlayer() )
		{// give health if player opened the door (medikit)
		// VARS( m_eoActivator )->health += m_bHealthValue;
			//MODDD NOTE - this "m_bHealthValue" thing seems not to be used.
			//             Not that it matters, some other changes are needed to properly mimick CWallHealth.

			m_hActivator->TakeHealth( m_bHealthValue, DMG_GENERIC );

		}

		// play door unlock sounds
		PlayLockSounds(pev, &m_ls, FALSE, FALSE);
		
		DoorGoUp();
	}

	return 1;
}

extern Vector VecBModelOrigin( entvars_t* pevBModel );




//
// Starts the door going to its "up" position (simply ToggleData->vecPosition2).
//
void CBaseDoor::DoorGoUp( void )
{
	entvars_t	*pevActivator;

	// It could be going-down, if blocked.
	ASSERT(m_toggle_state == TS_AT_BOTTOM || m_toggle_state == TS_GOING_DOWN);

	// emit door moving and stop sounds on CHAN_STATIC so that the multicast doesn't
	// filter them out and leave a client stuck with looping door sounds!
	if ( !FBitSet( pev->spawnflags, SF_DOOR_SILENT ) )
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving), 1, ATTN_NORM);

	m_toggle_state = TS_GOING_UP;
	
	SetMoveDone( &CBaseDoor::DoorHitTop );
	if ( FClassnameIs(pev, "func_door_rotating"))		// !!! BUGBUG Triggered doors don't work with this yet
	{
		float	sign = 1.0;

		if ( m_hActivator != NULL )
		{
			pevActivator = m_hActivator->pev;
			
			if ( !FBitSet( pev->spawnflags, SF_DOOR_ONEWAY ) && pev->movedir.y ) 		// Y axis rotation, move away from the player
			{
				Vector vec = pevActivator->origin - pev->origin;
				Vector angles = pevActivator->angles;
				angles.x = 0;
				angles.z = 0;
				UTIL_MakeVectors (angles);
	//			Vector vnext = (pevToucher->origin + (pevToucher->velocity * 10)) - pev->origin;
				UTIL_MakeVectors ( pevActivator->angles );
				Vector vnext = (pevActivator->origin + (gpGlobals->v_forward * 10)) - pev->origin;
				if ( (vec.x*vnext.y - vec.y*vnext.x) < 0 )
					sign = -1.0;
			}
		}
		AngularMove(m_vecAngle2*sign, pev->speed);
	}
	else
		LinearMove(m_vecPosition2, pev->speed);
}


//
// The door has reached the "up" position.  Either go back down, or wait for another activation.
//
void CBaseDoor::DoorHitTop( void )
{
	OnDoorHitTop(); //MODDD - may want to know.

	if ( !FBitSet( pev->spawnflags, SF_DOOR_SILENT ) )
	{
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving) );
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseArrived), 1, ATTN_NORM);
	}

	ASSERT(m_toggle_state == TS_GOING_UP);
	m_toggle_state = TS_AT_TOP;
	
	// toggle-doors don't come down automatically, they wait for refire.
	if (FBitSet(pev->spawnflags, SF_DOOR_NO_AUTO_RETURN))
	{
		// Re-instate touch method, movement is complete
		if ( !FBitSet ( pev->spawnflags, SF_DOOR_USE_ONLY ) )
			SetTouch( &CBaseDoor::DoorTouch );
	}
	else
	{
		// In flWait seconds, DoorGoDown will fire, unless wait is -1, then door stays open
		//MODDD
		if(!(pev->spawnflags & SF_DOOR_HEAL)){
			//normal.
			pev->nextthink = pev->ltime + m_flWait;
			SetThink( &CBaseDoor::DoorGoDown );
		}else{
			//MODDD - nevermind this, the rot door will handle when to close on its own, and involve "m_flWait" if it is reliably set.
			//doorGoDownDelay = gpGlobals->time + m_flWait;
		}

		if ( m_flWait == -1 )
		{
			//MODDD
			if(!(pev->spawnflags & SF_DOOR_HEAL)){
				pev->nextthink = -1;
			}else{
				//huh?
				//doorGoDownDelay = -1;
			}
		}
	}

	// Fire the close target (if startopen is set, then "top" is closed) - netname is the close target
	if ( pev->netname && (pev->spawnflags & SF_DOOR_START_OPEN) )
		FireTargets( STRING(pev->netname), m_hActivator, this, USE_TOGGLE, 0 );

	SUB_UseTargets( m_hActivator, USE_TOGGLE, 0 ); // this isn't finished
}


//
// Starts the door going to its "down" position (simply ToggleData->vecPosition1).
//
void CBaseDoor::DoorGoDown( void )
{
	if ( !FBitSet( pev->spawnflags, SF_DOOR_SILENT ) )
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving), 1, ATTN_NORM);
	
#ifdef DOOR_ASSERT
	ASSERT(m_toggle_state == TS_AT_TOP);
#endif // DOOR_ASSERT
	m_toggle_state = TS_GOING_DOWN;

	SetMoveDone( &CBaseDoor::DoorHitBottom );
	if ( FClassnameIs(pev, "func_door_rotating"))//rotating door
		AngularMove( m_vecAngle1, pev->speed);
	else
		LinearMove( m_vecPosition1, pev->speed);
}

//
// The door has reached the "down" position.  Back to quiescence.
//
void CBaseDoor::DoorHitBottom( void )
{
	OnDoorHitBottom(); //MODDD - may want to know.

	if ( !FBitSet( pev->spawnflags, SF_DOOR_SILENT ) )
	{
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving) );
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseArrived), 1, ATTN_NORM);
	}

	ASSERT(m_toggle_state == TS_GOING_DOWN);
	m_toggle_state = TS_AT_BOTTOM;

	// Re-instate touch method, cycle is complete
	if ( FBitSet ( pev->spawnflags, SF_DOOR_USE_ONLY ) )
	{// use only door
		SetTouch ( NULL );
	}
	else // touchable door
		SetTouch( &CBaseDoor::DoorTouch );

	SUB_UseTargets( m_hActivator, USE_TOGGLE, 0 ); // this isn't finished

	// Fire the close target (if startopen is set, then "top" is closed) - netname is the close target
	if ( pev->netname && !(pev->spawnflags & SF_DOOR_START_OPEN) )
		FireTargets( STRING(pev->netname), m_hActivator, this, USE_TOGGLE, 0 );
}

//no default behavior.
void CBaseDoor::OnDoorHitTop(void){

}
void CBaseDoor::OnDoorHitBottom(void){

}



void CBaseDoor::Blocked( CBaseEntity *pOther )
{
	edict_t	*pentTarget = NULL;
	CBaseDoor	*pDoor		= NULL;


	// Hurt the blocker a little.
	if ( pev->dmg )
		pOther->TakeDamage( pev, pev, pev->dmg, DMG_CRUSH );

	// if a door has a negative wait, it would never come back if blocked,
	// so let it just squash the object to death real fast

	if (m_flWait >= 0)
	{
		if (m_toggle_state == TS_GOING_DOWN)
		{
			DoorGoUp();
		}
		else
		{
			DoorGoDown();
		}
	}

	// Block all door pieces with the same targetname here.
	if ( !FStringNull ( pev->targetname ) )
	{
		for (;;)
		{
			pentTarget = FIND_ENTITY_BY_TARGETNAME(pentTarget, STRING(pev->targetname));

			if ( VARS( pentTarget ) != pev )
			{
				if (FNullEnt(pentTarget))
					break;

				if ( FClassnameIs ( pentTarget, "func_door" ) || FClassnameIs ( pentTarget, "func_door_rotating" ) )
				{
				
					pDoor = GetClassPtr( (CBaseDoor *) VARS(pentTarget) );

					if ( pDoor->m_flWait >= 0)
					{
						if (pDoor->pev->velocity == pev->velocity && pDoor->pev->avelocity == pev->velocity)
						{
							// this is the most hacked, evil, bastardized thing I've ever seen. kjb
							if ( FClassnameIs ( pentTarget, "func_door" ) )
							{// set origin to realign normal doors
								pDoor->pev->origin = pev->origin;
								pDoor->pev->velocity = g_vecZero;// stop!
							}
							else
							{// set angles to realign rotating doors
								pDoor->pev->angles = pev->angles;
								pDoor->pev->avelocity = g_vecZero;
							}
						}

						if ( pDoor->m_toggle_state == TS_GOING_DOWN)
							pDoor->DoorGoUp();
						else
							pDoor->DoorGoDown();
					}
				}
			}
		}
	}
}


/*QUAKED FuncRotDoorSpawn (0 .5 .8) ? START_OPEN REVERSE  
DOOR_DONT_LINK TOGGLE X_AXIS Y_AXIS
if two doors touch, they are assumed to be connected and operate as  
a unit.

TOGGLE causes the door to wait in both the start and end states for  
a trigger event.

START_OPEN causes the door to move to its destination when spawned,  
and operate in reverse.  It is used to temporarily or permanently  
close off an area when triggered (not usefull for touch or  
takedamage doors).

You need to have an origin brush as part of this entity.  The  
center of that brush will be
the point around which it is rotated. It will rotate around the Z  
axis by default.  You can
check either the X_AXIS or Y_AXIS box to change that.

"distance" is how many degrees the door will be rotated.
"speed" determines how fast the door moves; default value is 100.

REVERSE will cause the door to rotate in the opposite direction.

"angle"		determines the opening direction
"targetname" if set, no touch field will be spawned and a remote  
button or trigger field activates the door.
"health"	if set, door must be shot open
"speed"		movement speed (100 default)
"wait"		wait before returning (3 default, -1 = never return)
"dmg"		damage to inflict when blocked (2 default)
"sounds"
0)	no sound
1)	stone
2)	base
3)	stone chain
4)	screechy metal
*/


typedef enum {
  HDP_OPEN = 0,
  HDP_CLOSED = 1
  
} HealDoorPreference;

//MODDD - serious problem. We can't use the same ent for healing / a door as think is needed for both. independend think times.
//        But this door can just choose to spawn an entity with only the HealthModule, and use itself for thinks. that would work fine.

class CRotDoor : public CBaseDoor, I_HealthModule_Parent
{
public:
	float angularMoveDoneTime;
	float doorCloseDelay;
	

	HealthModule healthModuleInstance;  //guaranteed instance.
	HealDoorPreference currentPreference;

	static TYPEDESCRIPTION m_SaveData[];
	virtual int Save( CSave &save );
	virtual int Restore( CRestore &restore );

	CRotDoor(void);
	//BOOL usesSoundSentenceSave(void);
	
	void I_HealthModule_ChargeEmpty(void);
	void I_HealthModule_ChargeRestored(void);
	void I_HealthModule_UseStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void I_HealthModule_UseContinuous(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);

	void onDoorUse(void);  //this is my own simple common method to call for UseStart and UseContinuous above.

	void I_HealthModule_UseEnd(void);
	
	void I_HealthModule_SetThink_UseEnd(void);
	void I_HealthModule_SetThink_ChargeRestored(void);
	void I_HealthModule_SetThink_Custom(void);

	void EXPORT CustomThink(void);
	
	//MODDD - new override in case of something special the health wall door healer needs.
	virtual void AngularMove( Vector vecDestAngle, float flSpeed );

	void OnDoorHitTop(void);
	void OnDoorHitBottom(void);

	void ReportGeneric(void);
	

	
	//Moved to HealthModule. This is completely internal to healing logic.
	//void EXPORT Off(void);

	//Moved to HealthModule. This is completely internal to healing logic.
	//void EXPORT Recharge(void);
	void EXPORT UseEnd(void);
	void EXPORT ChargeRestored(void);
	
	void KeyValue( KeyValueData *pkvd );
	
	//MODDD - new.
	void Activate();
	//void Spawn( );
	void Precache( void );
	virtual void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

	
	virtual int	ObjectCaps( void ){
		if(pev->spawnflags & SF_DOOR_HEAL){
			//add CONTINUOUS_USE, remove FCAP_ACROSS_TRANSITION (although CBaseDoor already does that 100% of the time)
			return (CBaseDoor :: ObjectCaps() | FCAP_CONTINUOUS_USE) & ~FCAP_ACROSS_TRANSITION;
		}else{
			//just the default behavior.
			return (CBaseDoor :: ObjectCaps());
		}
	}//END OF ObjectCaps









	virtual void Spawn( void );
	virtual void SetToggleState( int state );
};

LINK_ENTITY_TO_CLASS( func_door_rotating, CRotDoor );






CRotDoor::CRotDoor(void){

	//only used if a healing rotation door needs them.
	currentPreference = HDP_CLOSED;
	angularMoveDoneTime = -1;
	doorCloseDelay = -1;

}//END OF CRotDoor constructor



//??
//doorCloseDelay
//currentPreference
TYPEDESCRIPTION CRotDoor::m_SaveData[] =
{
	DEFINE_FIELD( CRotDoor, angularMoveDoneTime, FIELD_TIME),
	
    //...
};


//IMPLEMENT_SAVERESTORE( CRotDoor, CBaseDoor );

//MODDD - NEW. Let the healthModuleInstance know to save / restore here, otherwise it has no way of getting called.
//        Only entities are called straight from the engine on saving / loading for this.
//Yes, CBaseToggle is the direct parent of CWallHealth here but CBaseEntity was chosen instead.
//Not sure if CBaseToggle was deliberately skipped, but keeping it that way.
int CRotDoor::Save( CSave &save )
{
	if ( !CBaseDoor::Save(save) )
		return 0;
	int iWriteFieldsResult = save.WriteFields( "CRotDoor", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	//return iWriteFieldsResult;

	if(pev->spawnflags & SF_DOOR_HEAL){
		if(iWriteFieldsResult){
			int iWriteFields_HealthModule_Result = healthModuleInstance.Save(save);
			return iWriteFields_HealthModule_Result;
		}
	}else{
		return iWriteFieldsResult;
	}
}
int CRotDoor::Restore( CRestore &restore )
{
	if ( !CBaseDoor::Restore(restore) )
		return 0;

	
	if(pev->spawnflags & SF_DOOR_HEAL){
		//Establish that I'm the parent entity again.
		healthModuleInstance.setupRestore(static_cast <CBaseEntity*>(this), static_cast <I_HealthModule_Parent*>(this));
	}

	int iReadFieldsResult = restore.ReadFields( "CRotDoor", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	//return iReadFieldsResult;

	if(pev->spawnflags & SF_DOOR_HEAL){
		if(iReadFieldsResult){
			int iReadFields_HealthModule_Result = healthModuleInstance.Restore(restore);
			return iReadFields_HealthModule_Result;
		}
	}else{
		return iReadFieldsResult;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










//MODDD - also new event methods required by the HealthModule instance for callbacks.
void CRotDoor::I_HealthModule_ChargeEmpty(void){
	//pev->frame = 1;
	//turn the door if this happens?
}
void CRotDoor::I_HealthModule_ChargeRestored(void){
	//pev->frame = 0;
}
void CRotDoor::I_HealthModule_UseStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value){
	//CBaseDoor::Use(pActivator, pCaller, useType, value);
	onDoorUse();
}
void CRotDoor::I_HealthModule_UseContinuous(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value){
	//CBaseDoor::Use(pActivator, pCaller, useType, value);
	onDoorUse();
}

void CRotDoor::onDoorUse(void){
	if(m_toggle_state == TS_AT_BOTTOM){
		//Closed? Open me.
		currentPreference = HDP_OPEN;
	}
	if(m_toggle_state == TS_AT_TOP){
		//This resets the close delay.
		doorCloseDelay = gpGlobals->time + m_flWait;
	}
	//}
}



void CRotDoor::I_HealthModule_UseEnd(void){

}

void CRotDoor::I_HealthModule_SetThink_UseEnd(void){
	SetThink( static_cast <void (CBaseEntity::*)(void)>(&CRotDoor::UseEnd) );
}
void CRotDoor::I_HealthModule_SetThink_ChargeRestored(void){
	SetThink( static_cast <void (CBaseEntity::*)(void)>(&CRotDoor::ChargeRestored) );
}

void CRotDoor::UseEnd(void){
	healthModuleInstance.UseEnd();
}

void CRotDoor::ChargeRestored(void){
	healthModuleInstance.ChargeRestored();
}

void CRotDoor::I_HealthModule_SetThink_Custom(void){
	SetThink( static_cast <void (CBaseEntity::*)(void)>(&CRotDoor::CustomThink) );
}

void CRotDoor::CustomThink(void){
	//This think method is only possible if we have the healer spawnflag.
	
	if(angularMoveDoneTime != -1 && gpGlobals->time >= angularMoveDoneTime){
		angularMoveDoneTime = -1;
		AngularMoveDone();
	}

	healthModuleInstance.CustomThink();
	
	//doorGoDownDelay ?
	if(doorCloseDelay != -1 && gpGlobals->time >= doorCloseDelay){
		doorCloseDelay = -1;
		//Open for too long without use? close it.
		currentPreference = HDP_CLOSED;
	}
	
	if(m_toggle_state == TS_AT_BOTTOM){
		//If closed...
		if(currentPreference == HDP_OPEN){
			//closed but want to open?

			// play door unlock sounds
			//MODDD TODO - also play for closing, or a separate sound for closing?
			PlayLockSounds(pev, &m_ls, FALSE, FALSE);

			DoorGoUp();
		}

	}
	if(m_toggle_state == TS_AT_TOP){
		
		if(healthModuleInstance.m_iJuice <= 0){
			//I want to shut.
			doorCloseDelay = -1;
			currentPreference = HDP_CLOSED;
		} 

		if(currentPreference == HDP_CLOSED){
			//stopped at the top but want to close? do so.
			DoorGoDown();
		}
	}


}//END OF CustomThink



void CRotDoor :: AngularMove( Vector vecDestAngle, float flSpeed )
{
	if(!(pev->spawnflags & SF_DOOR_HEAL)){
		//If not a heal door, nothing unusual.
		CBaseDoor::AngularMove(vecDestAngle, flSpeed);
	}else{
		//Otherwise we need to handle this differently.  Can't affect the think method or time at all.
		//Started as a clone of CBaseToggle's AngularMove.
		ASSERTSZ(flSpeed != 0, "AngularMove:  no speed is defined!");
	//	ASSERTSZ(m_pfnCallWhenMoveDone != NULL, "AngularMove: no post-move function defined");
	
		m_vecFinalAngle = vecDestAngle;

		// Already there?
		if (vecDestAngle == pev->angles)
		{
			AngularMoveDone();
			return;
		}
		
		// set destdelta to the vector needed to move
		Vector vecDestDelta = vecDestAngle - pev->angles;
	
		// divide by speed to get time to reach dest
		float flTravelTime = vecDestDelta.Length() / flSpeed;

		// set nextthink to trigger a call to AngularMoveDone when dest is reached
		//MODDD - CHANGE.  Need to use a scheduled time instead.
		//pev->nextthink = pev->ltime + flTravelTime;
		//SetThink( &CBaseToggle::AngularMoveDone );
		angularMoveDoneTime = gpGlobals->time + flTravelTime;


		// scale the destdelta vector by the time spent traveling to get velocity
		pev->avelocity = vecDestDelta / flTravelTime;

	}//END OF spawnflag check
}//END OF AngularMove






void CRotDoor::OnDoorHitTop(void){
	if(currentPreference == HDP_OPEN){
		//I want to stay open for at least this much longer.
		doorCloseDelay = gpGlobals->time + m_flWait;
	}
}
void CRotDoor::OnDoorHitBottom(void){
	//nothing special?
}



void CRotDoor::ReportGeneric(void){

	CBaseDoor::ReportGeneric();

	//HACK: turn my charge back on if out.
	//if(healthModuleInstance.m_iJuice <= 0){
	//	healthModuleInstance.ChargeRestored();
	//}

}//END OF ReportGeneric







void CRotDoor::KeyValue( KeyValueData *pkvd )
{
	/*
	if(!healthModuleInstance.establishedParentYet){
		//incredibly hacky, but KeyValue appears to be the earliest thing called in an entity. And it may be needed to set
		//things in our healthModule.  Not that it even needs to know who the parent is at this point.  Ah well.
		healthModuleInstance.setup(static_cast <CBaseEntity*>(this), static_cast <I_HealthModule_Parent*>(this));
	}
	*/

	//Let my healthModuleInstance have a say first.

	if(pev->spawnflags & SF_DOOR_HEAL){
		healthModuleInstance.KeyValue(pkvd);
		
		if(pkvd->fHandled == FALSE){
			CBaseDoor::KeyValue( pkvd );
		}
	}else{
		CBaseDoor::KeyValue( pkvd );
	}

}


void CRotDoor::Activate(){
	CBaseDoor::Activate();
}


void CRotDoor::Precache()
{
	if(pev->spawnflags & SF_DOOR_HEAL){
		healthModuleInstance.Precache();
	}
	CBaseDoor::Precache();  //likely had something in mind?
}








/*
	- Must be a flag
- When flag is enabled
    * Check if the player doesn't have 100 hp
        * If the player doesn't have 100hp, let the door to open
            *Give the player X amount of health (make it to use the already existing values from the healthcharger in skill.cfg
            *Make sure you can't get any more HP afterwards (I can prevent this by forcing the door to never go back to normal
            *Of course make it play a sound when using (medshot4.wav) and one when you are already full HP (medshotno1.wav)
*/
void CRotDoor::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	//???
	//if ( useType != USE_SET )		// Momentary buttons will pass down a float in here
	//	return;

	//if ( value > 1.0 )
	//	value = 1.0;



	//DEBUG: awlays pass for now.?
	//pev->spawnflags |= SF_DOOR_HEAL;

	if(pev->spawnflags & SF_DOOR_HEAL){
		healthModuleInstance.Use(pActivator, pCaller, useType, value);
		
		//at least do this like the base class does. Just in case?
		m_hActivator = pActivator;

		//on taking, send a signal to do
		//    CBaseDoor::Use(pActivator, pCaller, useType, value);

	}else{

		//do whatever the parent does.
		CBaseDoor::Use(pActivator, pCaller, useType, value);
	}

}//END OF Use
























void CRotDoor::Spawn( void )
{
	
	
	if((pev->spawnflags & SF_DOOR_HEAL) && !healthModuleInstance.establishedParentYet){
		healthModuleInstance.setupSpawn(static_cast <CBaseEntity*>(this), static_cast <I_HealthModule_Parent*>(this));
	}

	Precache( );

	if((pev->spawnflags & SF_DOOR_HEAL)){
		healthModuleInstance.Spawn();

		//also, THIS must be something. Defaulting to 4 seconds if not provided properly from KeyValues.
		if(m_flWait < 0){
			m_flWait = 2.5f;
		}

	}




	// set the axis of rotation
	CBaseToggle::AxisDir( pev );

	// check for clockwise rotation
	if ( FBitSet (pev->spawnflags, SF_DOOR_ROTATE_BACKWARDS) )
		pev->movedir = pev->movedir * -1;
	
	//m_flWait			= 2; who the hell did this? (sjb)
	m_vecAngle1	= pev->angles;
	m_vecAngle2	= pev->angles + pev->movedir * m_flMoveDistance;

	ASSERTSZ(m_vecAngle1 != m_vecAngle2, "rotating door start/end positions are equal");
	
	if ( FBitSet (pev->spawnflags, SF_DOOR_PASSABLE) )
		pev->solid		= SOLID_NOT;
	else
		pev->solid		= SOLID_BSP;

	pev->movetype	= MOVETYPE_PUSH;
	UTIL_SetOrigin(pev, pev->origin);
	SET_MODEL(ENT(pev), STRING(pev->model) );

	if (pev->speed == 0)
		pev->speed = 100;
	
// DOOR_START_OPEN is to allow an entity to be lighted in the closed position
// but spawn in the open position
	if ( FBitSet (pev->spawnflags, SF_DOOR_START_OPEN) )
	{	// swap pos1 and pos2, put door at pos2, invert movement direction
		pev->angles = m_vecAngle2;
		Vector vecSav = m_vecAngle1;
		m_vecAngle2 = m_vecAngle1;
		m_vecAngle1 = vecSav;
		pev->movedir = pev->movedir * -1;
	}

	m_toggle_state = TS_AT_BOTTOM;

	if ( FBitSet ( pev->spawnflags, SF_DOOR_USE_ONLY ) )
	{
		SetTouch ( NULL );
	}
	else // touchable button
		SetTouch( &CBaseDoor::DoorTouch );
}


void CRotDoor :: SetToggleState( int state )
{
	if ( state == TS_AT_TOP )
		pev->angles = m_vecAngle2;
	else
		pev->angles = m_vecAngle1;

	UTIL_SetOrigin( pev, pev->origin );
}








class CMomentaryDoor : public CBaseToggle
{
public:

	CMomentaryDoor(void);


	void	Spawn( void );
	void Precache( void );

	void	KeyValue( KeyValueData *pkvd );

	virtual BOOL IsWorldAffiliated(void);

	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual int	ObjectCaps( void ) { return CBaseToggle :: ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

	//MODDD - is that safe?
	//virtual void Think( void );


	virtual int	Save( CSave &save );
	virtual int	Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	BYTE	m_bMoveSnd;			// sound a door makes while moving
	
	//float stopSoundDelay;  //MODDD
	float previousValue;
	BOOL hasPreviousValue;
	float previousValueDelta;
	

};

LINK_ENTITY_TO_CLASS( momentary_door, CMomentaryDoor );

TYPEDESCRIPTION	CMomentaryDoor::m_SaveData[] = 
{
	DEFINE_FIELD( CMomentaryDoor, m_bMoveSnd, FIELD_CHARACTER ),
};

IMPLEMENT_SAVERESTORE( CMomentaryDoor, CBaseToggle );


//MODDD
CMomentaryDoor::CMomentaryDoor(void){
	//stopSoundDelay = -1;
	previousValue = 0;
	hasPreviousValue = FALSE;
	previousValueDelta = 0;
}//END OF constructor



void CMomentaryDoor::Spawn( void )
{
	SetMovedir (pev);

	pev->solid		= SOLID_BSP;
	pev->movetype	= MOVETYPE_PUSH;

	UTIL_SetOrigin(pev, pev->origin);
	SET_MODEL( ENT(pev), STRING(pev->model) );
	
	if (pev->speed == 0)
		pev->speed = 100;
	if (pev->dmg == 0)
		pev->dmg = 2;
	
	m_vecPosition1	= pev->origin;
	// Subtract 2 from size because the engine expands bboxes by 1 in all directions making the size too big
	m_vecPosition2	= m_vecPosition1 + (pev->movedir * (fabs( pev->movedir.x * (pev->size.x-2) ) + fabs( pev->movedir.y * (pev->size.y-2) ) + fabs( pev->movedir.z * (pev->size.z-2) ) - m_flLip));
	ASSERTSZ(m_vecPosition1 != m_vecPosition2, "door start/end positions are equal");

	if ( FBitSet (pev->spawnflags, SF_DOOR_START_OPEN) )
	{	// swap pos1 and pos2, put door at pos2
		UTIL_SetOrigin(pev, m_vecPosition2);
		m_vecPosition2 = m_vecPosition1;
		m_vecPosition1 = pev->origin;
	}
	SetTouch( NULL );
	
	Precache();
}
	
void CMomentaryDoor::Precache( void )
{

// set the door's "in-motion" sound
	switch (m_bMoveSnd)
	{
	case	0:
		pev->noiseMoving = ALLOC_STRING("common/null.wav");
		break;
	case	1:
		PRECACHE_SOUND ("doors/doormove1.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove1.wav");
		break;
	case	2:
		PRECACHE_SOUND ("doors/doormove2.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove2.wav");
		break;
	case	3:
		PRECACHE_SOUND ("doors/doormove3.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove3.wav");
		break;
	case	4:
		PRECACHE_SOUND ("doors/doormove4.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove4.wav");
		break;
	case	5:
		PRECACHE_SOUND ("doors/doormove5.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove5.wav");
		break;
	case	6:
		PRECACHE_SOUND ("doors/doormove6.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove6.wav");
		break;
	case	7:
		PRECACHE_SOUND ("doors/doormove7.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove7.wav");
		break;
	case	8:
		PRECACHE_SOUND ("doors/doormove8.wav");
		pev->noiseMoving = ALLOC_STRING("doors/doormove8.wav");
		break;
	default:
		pev->noiseMoving = ALLOC_STRING("common/null.wav");
		break;
	}
}

void CMomentaryDoor::KeyValue( KeyValueData *pkvd )
{

	if (FStrEq(pkvd->szKeyName, "movesnd"))
	{
		m_bMoveSnd = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "stopsnd"))
	{
//		m_bStopSnd = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "healthvalue"))
	{
//		m_bHealthValue = atof(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else
		CBaseToggle::KeyValue( pkvd );
}

BOOL CMomentaryDoor::IsWorldAffiliated(){
	return TRUE;
}

void CMomentaryDoor::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if ( useType != USE_SET )		// Momentary buttons will pass down a float in here
		return;

	if ( value > 1.0 )
		value = 1.0;
	Vector move = m_vecPosition1 + (value * (m_vecPosition2 - m_vecPosition1));
	

	Vector delta = move - pev->origin;
	float speed = delta.Length() * 10;

	/*
	const char* activatorClassnameDebug = NULL;
	const char* callerClassnameDebug = NULL;
	
	if(pActivator != NULL){
		activatorClassnameDebug = pActivator->getClassname();
	}
	if(pCaller != NULL){
		callerClassnameDebug = pCaller->getClassname();
	}
	*/

	BOOL canPlaySound = FALSE;
	BOOL canStopSound = FALSE;

	if(hasPreviousValue){
		float valueDelta = value - previousValue;
		if(previousValueDelta * valueDelta < 0){
			//If in 
			canPlaySound = TRUE;
		}
		previousValueDelta = valueDelta;
	}else{
		//No previous value? This is a first then.
		if(speed != 0){
			canPlaySound = TRUE;
		}
	}

	
	if ( speed != 0 )
	{
		//MODDD - new section. Determine whether to make a sound like so.
		/*
		if(previousValue * value <= 0){
			//the signs are different or either is 0 (only previousSpeed could be)?
			//either starting from nothing in either direction or changing directions, play the sound this once.
			canPlaySound = TRUE;
		}
		*/
		
		// This entity only thinks when it moves, so if it's thinking, it's in the process of moving
		// play the sound when it starts moving
		//if ( pev->nextthink < pev->ltime || pev->nextthink == 0 ){
		// <play sound...>

		//stopSoundDelay = gpGlobals->time + 4;

		LinearMove( move, speed );
	}else{
		if(previousValue != 0){
			// was moving before, stopped now? Stop the noise.
			// Seems this isn't getting called since Use isn't called after the door stops.
			// So this doesn't get a chance to be reached.
			canStopSound = TRUE;
		}
	}
	
	hasPreviousValue = TRUE;
	previousValue = value;

	

	//MODDD - HACKER SACKS - if my activator is about to stop thinking, then I'd better shut up.
	//    if(pev->nextthink <= 0 || m_pfnThink==NULL){

	//ANOTHER WAY OF TELLING:   value <= 0 || >= 1 ???
	if(value <= 0 || value >= 1 || (pActivator != NULL && (pActivator->pev->nextthink <= 0)) ){
		//turning the think method off? The noise must cease.
		canStopSound = TRUE;
	}



	
	if(canStopSound){
		easyForcePrintLine("CMomentaryDoor STOPSND %s", STRING(pev->noiseMoving));
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving));
		hasPreviousValue = FALSE;
		previousValueDelta = 0;
	}else if(canPlaySound){
		easyForcePrintLine("CMomentaryDoor PLAYSND %s", STRING(pev->noiseMoving));
		//const char* debugNoiseMoving = STRING(pev->noiseMoving);
		//STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving)); //1, ATTN_NORM);
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving));
		EMIT_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving), 1, ATTN_NORM);
	}

	



}//END OF Use


/*
//MODDD - new... nevermind
void CMomentaryDoor::Think(){
	if(stopSoundDelay != -1 && gpGlobals->time >= stopSoundDelay){
		stopSoundDelay = -1;
		STOP_SOUND(ENT(pev), CHAN_STATIC, (char*)STRING(pev->noiseMoving)); //1, ATTN_NORM);
	}
	CBaseToggle::Think();
}//END OF Think
*/



