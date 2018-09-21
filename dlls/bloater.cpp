/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
//=========================================================
// Bloater
//=========================================================

#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include "basemonster.h"
#include	"schedule.h"


//=========================================================
// Monster's Anim Events Go Here
//=========================================================
#define	BLOATER_AE_ATTACK_MELEE1		0x01


class CBloater : public CBaseMonster
{
public:
	CBloater();

	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
	void HandleAnimEvent( MonsterEvent_t *pEvent );

	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSnd( void );

	// No range attacks
	BOOL CheckRangeAttack1 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckRangeAttack2 ( float flDot, float flDist ) { return FALSE; }

	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE



};


#if REMOVE_ORIGINAL_NAMES != 1
	LINK_ENTITY_TO_CLASS( monster_bloater, CBloater );
#endif

#if EXTRA_NAMES > 0
	LINK_ENTITY_TO_CLASS( bloater, CBloater );
	
	//no extras.

#endif

//=========================================================
// Classify - indicates this monster's place in the 
// relationship table.
//=========================================================
int	CBloater :: Classify ( void )
{
	return	CLASS_ALIEN_MONSTER;
}

//=========================================================
// SetYawSpeed - allows each sequence to have a different
// turn rate associated with it.
//=========================================================
void CBloater :: SetYawSpeed ( void )
{
	int ys;

	ys = 120;

#if 0
	switch ( m_Activity )
	{
	}
#endif

	pev->yaw_speed = ys;
}







GENERATE_TRACEATTACK_IMPLEMENTATION(CBloater)
{
	GENERATE_TRACEATTACK_PARENT_CALL(CBaseMonster);
}

GENERATE_TAKEDAMAGE_IMPLEMENTATION(CBloater)
{
	PainSound();
	return GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
}

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


//=========================================================
// HandleAnimEvent - catches the monster-specific messages
// that occur when tagged animation frames are played.
//=========================================================
void CBloater :: HandleAnimEvent( MonsterEvent_t *pEvent )
{
	switch( pEvent->event )
	{
		case BLOATER_AE_ATTACK_MELEE1:
		{
			// do stuff for this event.
			AttackSnd();
		}
		break;

		default:
			CBaseMonster::HandleAnimEvent( pEvent );
			break;
	}
}

CBloater::CBloater(){

}

//=========================================================
// Spawn
//=========================================================
void CBloater :: Spawn()
{
	Precache( );

	SET_MODEL(ENT(pev), "models/floater.mdl");
	UTIL_SetSize( pev, VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX );

	pev->classname = MAKE_STRING("monster_bloater");

	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_FLY;
	pev->spawnflags		|= FL_FLY;
	m_bloodColor		= BLOOD_COLOR_GREEN;
	pev->health			= 40;
	pev->view_ofs		= VEC_VIEW;// position of the eyes relative to monster's origin.
	m_flFieldOfView		= 0.5;// indicates the width of this monster's forward view cone ( as a dotproduct result )
	m_MonsterState		= MONSTERSTATE_NONE;

	MonsterInit();

}

//=========================================================
// Precache - precaches all resources this monster needs
//=========================================================

extern int global_useSentenceSave;
void CBloater :: Precache()
{
	PRECACHE_MODEL("models/floater.mdl");

	global_useSentenceSave = TRUE;

	//NOTE: insert sounds to be precached here, just in case.  Also add to sentences.txt (ask blue24 if unsure what that means), has to do with the "save sentences" method to save on precache space.

	global_useSentenceSave = FALSE;

}	

//=========================================================
// AI Schedules Specific to this monster
//=========================================================

