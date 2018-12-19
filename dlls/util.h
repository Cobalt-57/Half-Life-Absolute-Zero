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
//
// Misc utility code
//




//MODDD - WHY NOT???
#ifndef UTIL_H
#define UTIL_H

#include "util_shared.h"  //includes util_printout.h
#include "client_message.h" //for access to gmsg IDs and the LinkUserMessages method.

#include "activity.h"
#include "enginecallback.h"

//MODDD - needed to have "CGlobalState" available to prototypes here.
#include "saverestore.h"


//#include "custom_debug.h"


extern globalvars_t				*gpGlobals;




extern unsigned short g_sTrailEngineChoice;
extern unsigned short g_sImitation7;
extern unsigned short g_sTrail;
extern unsigned short g_sTrailRA;
extern unsigned short g_sCustomBalls;
extern unsigned short g_sCustomBallsPowerup;
extern unsigned short g_quakeExplosionEffect;
extern unsigned short g_decalGunshotCustomEvent;
extern unsigned short g_sFreakyLight;
extern unsigned short g_sFriendlyVomit;




extern int giPrecacheGrunt;








//MODDD - referred to in here, yes.
extern float global_soundAttenuationAll;
extern float global_soundAttenuationStuka;
extern float global_soundVolumeAll;
extern float global_soundVolumeStuka;

//Why wasn't this externed everywhere before? I'm asking you, past me. I'm not insane, I swear.
extern int global_useSentenceSave;



extern float previousFrameTime;
extern BOOL gamePaused;

extern BOOL loadedGame;




// Use this instead of ALLOC_STRING on constant strings
#define STRING(offset)		(const char *)(gpGlobals->pStringBase + (int)offset)
#define MAKE_STRING(str)	((int)str - (int)STRING(0))







inline edict_t *FIND_ENTITY_BY_CLASSNAME(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "classname", pszName);
}	

inline edict_t *FIND_ENTITY_BY_TARGETNAME(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "targetname", pszName);
}	

// for doing a reverse lookup. Say you have a door, and want to find its button.
inline edict_t *FIND_ENTITY_BY_TARGET(edict_t *entStart, const char *pszName) 
{
	return FIND_ENTITY_BY_STRING(entStart, "target", pszName);
}	

// Keeps clutter down a bit, when writing key-value pairs
#define WRITEKEY_INT(pf, szKeyName, iKeyValue) ENGINE_FPRINTF(pf, "\"%s\" \"%d\"\n", szKeyName, iKeyValue)
#define WRITEKEY_FLOAT(pf, szKeyName, flKeyValue)								\
		ENGINE_FPRINTF(pf, "\"%s\" \"%f\"\n", szKeyName, flKeyValue)
#define WRITEKEY_STRING(pf, szKeyName, szKeyValue)								\
		ENGINE_FPRINTF(pf, "\"%s\" \"%s\"\n", szKeyName, szKeyValue)
#define WRITEKEY_VECTOR(pf, szKeyName, flX, flY, flZ)							\
		ENGINE_FPRINTF(pf, "\"%s\" \"%f %f %f\"\n", szKeyName, flX, flY, flZ)

// Keeps clutter down a bit, when using a float as a bit-vector
#define SetBits(flBitVector, bits)		((flBitVector) = (int)(flBitVector) | (bits))
#define ClearBits(flBitVector, bits)	((flBitVector) = (int)(flBitVector) & ~(bits))
#define FBitSet(flBitVector, bit)		((int)(flBitVector) & (bit))

// Makes these more explicit, and easier to find
#define FILE_GLOBAL static
#define DLL_GLOBAL

// Until we figure out why "const" gives the compiler problems, we'll just have to use
// this bogus "empty" define to mark things as constant.
#define CONSTANT

// In case this ever changes
#define M_PI			3.14159265358979323846







//
// How did I ever live without ASSERT?
//
#ifdef	DEBUG
void DBG_AssertFunction(BOOL fExpr, const char* szExpr, const char* szFile, int szLine, const char* szMessage);
#define ASSERT(f)		DBG_AssertFunction(f, #f, __FILE__, __LINE__, NULL)
#define ASSERTSZ(f, sz)	DBG_AssertFunction(f, #f, __FILE__, __LINE__, sz)
#else	// !DEBUG
#define ASSERT(f)
#define ASSERTSZ(f, sz)
#endif	// !DEBUG


extern DLL_GLOBAL const Vector g_vecZero;

//
// Constants that were used only by QC (maybe not used at all now)
//
// Un-comment only as needed
//
#define LANGUAGE_ENGLISH				0
#define LANGUAGE_GERMAN					1
#define LANGUAGE_FRENCH					2
#define LANGUAGE_BRITISH				3

//MODDD - NOTICE: left for reasons of compatability paranoia.  If only the script depends on this, it can be canned though.
//                See globals.cpp for more info.
extern DLL_GLOBAL int			g_Language;

#define AMBIENT_SOUND_STATIC			0	// medium radius attenuation
#define AMBIENT_SOUND_EVERYWHERE		1
#define AMBIENT_SOUND_SMALLRADIUS		2
#define AMBIENT_SOUND_MEDIUMRADIUS		4
#define AMBIENT_SOUND_LARGERADIUS		8
#define AMBIENT_SOUND_START_SILENT		16
#define AMBIENT_SOUND_NOT_LOOPING		32

#define SPEAKER_START_SILENT			1	// wait for trigger 'on' to start announcements


//SOUND FLAGS HERE!
#define SND_SPAWNING		(1<<8)		// duplicated in protocol.h we're spawing, used in some cases for ambients 
#define SND_STOP			(1<<5)		// duplicated in protocol.h stop sound
#define SND_CHANGE_VOL		(1<<6)		// duplicated in protocol.h change sound vol
#define SND_CHANGE_PITCH	(1<<7)		// duplicated in protocol.h change sound pitch

#define	LFO_SQUARE			1
#define LFO_TRIANGLE		2
#define LFO_RANDOM			3

// func_rotating
#define SF_BRUSH_ROTATE_Y_AXIS		0
#define SF_BRUSH_ROTATE_INSTANT		1
#define SF_BRUSH_ROTATE_BACKWARDS	2
#define SF_BRUSH_ROTATE_Z_AXIS		4
#define SF_BRUSH_ROTATE_X_AXIS		8
#define SF_PENDULUM_AUTO_RETURN		16
#define	SF_PENDULUM_PASSABLE		32


//MODDD - NEW
//momentary_rot_button
#define	SF_MOMENTARY_ROTATING_REQUIREMASTERTRIGGER		32



#define SF_BRUSH_ROTATE_SMALLRADIUS	128
#define SF_BRUSH_ROTATE_MEDIUMRADIUS 256
#define SF_BRUSH_ROTATE_LARGERADIUS 512

#define PUSH_BLOCK_ONLY_X	1
#define PUSH_BLOCK_ONLY_Y	2

#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_HULL_MAX		Vector( 16,  16,  36)
#define VEC_HUMAN_HULL_MIN	Vector( -16, -16, 0 )
#define VEC_HUMAN_HULL_MAX	Vector( 16, 16, 72 )
#define VEC_HUMAN_HULL_DUCK	Vector( 16, 16, 36 )

#define VEC_VIEW			Vector( 0, 0, 28 )

#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )
#define VEC_DUCK_HULL_MAX	Vector( 16,  16,  18)
#define VEC_DUCK_VIEW		Vector( 0, 0, 12 )

#define SVC_TEMPENTITY		23
#define SVC_INTERMISSION	30
#define SVC_CDTRACK			32
#define SVC_WEAPONANIM		35
#define SVC_ROOMTYPE		37
#define	SVC_DIRECTOR		51



// triggers
#define	SF_TRIGGER_ALLOWMONSTERS	1// monsters allowed to fire this trigger
#define	SF_TRIGGER_NOCLIENTS		2// players not allowed to fire this trigger
#define SF_TRIGGER_PUSHABLES		4// only pushables can fire this trigger

// func breakable
#define SF_BREAK_TRIGGER_ONLY	1// may only be broken by trigger
#define	SF_BREAK_TOUCH			2// can be 'crashed through' by running player (plate glass)
#define SF_BREAK_PRESSURE		4// can be broken by a player standing on it
#define SF_BREAK_CROWBAR		256// instant break if hit with crowbar

// func_pushable (it's also func_breakable, so don't collide with those flags)
#define SF_PUSH_BREAKABLE		128

#define SF_LIGHT_START_OFF		1

#define SPAWNFLAG_NOMESSAGE	1
#define SPAWNFLAG_NOTOUCH	1
#define SPAWNFLAG_DROIDONLY	4

#define SPAWNFLAG_USEONLY	1		// can't be touched, must be used (buttons)

#define TELE_PLAYER_ONLY	1
#define TELE_SILENT			2

#define SF_TRIG_PUSH_ONCE		1


// Sound Utilities

// sentence groups
//#define CBSENTENCENAME_MAX 16
//MODDD - upped!
#define CBSENTENCENAME_MAX 45

//MODDD - testing, ignoring that limit notice (there is no sound.h, ust hoping that isn't pre-compiled somehow, headers usually aren't...)
//SHIT SHIT SHIT.  Nope.  What is with the devs and having these un-alterable limits...

#define CVOXFILESENTENCEMAX		1536		// max number of sentences in game. NOTE: this must match
											// CVOXFILESENTENCEMAX in engine\sound.h!!!
//#define CVOXFILESENTENCEMAX		4000












//!!!!!!!!!!!!!!
//entity-related typedefs and constant methods like ENT have been moved to "enginecallback.h", also serverside.


//-----------



// Testing strings for nullity
#define iStringNull 0
inline BOOL FStringNull(int iString)			{ return iString == iStringNull; }





#define cchMapNameMost 32

// Dot products for view cone checking
#define VIEW_FIELD_FULL		(float)-1.0 // +-180 degrees
#define	VIEW_FIELD_WIDE		(float)-0.7 // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
#define	VIEW_FIELD_NARROW	(float)0.7 // +-45 degrees, more narrow check used to set up ranged attacks
#define	VIEW_FIELD_ULTRA_NARROW	(float)0.9 // +-25 degrees, more narrow check used to set up ranged attacks






// All monsters need this data
#define		DONT_BLEED			-1
#define		BLOOD_COLOR_RED		(BYTE)70 //(BYTE)247
#define		BLOOD_COLOR_YELLOW	(BYTE)195
#define		BLOOD_COLOR_GREEN	BLOOD_COLOR_YELLOW
#define		BLOOD_COLOR_BLACK	(BYTE)0 //black like oil


typedef enum 
{

	MONSTERSTATE_NONE = 0,
	MONSTERSTATE_IDLE,
	MONSTERSTATE_COMBAT,
	MONSTERSTATE_ALERT,
	MONSTERSTATE_HUNT,
	MONSTERSTATE_PRONE,
	MONSTERSTATE_SCRIPT,
	MONSTERSTATE_PLAYDEAD,
	MONSTERSTATE_DEAD

} MONSTERSTATE;



// Things that toggle (buttons/triggers/doors) need this
typedef enum
	{
	TS_AT_TOP,
	TS_AT_BOTTOM,
	TS_GOING_UP,
	TS_GOING_DOWN
	} TOGGLE_STATE;

extern const char* TOGGLE_STATE_STR[];
const char* TOGGLE_STATE_STR_Safe(int argIndex);


// Misc useful
inline BOOL FStrEq(const char*sz1, const char*sz2)
	{ return (strcmp(sz1, sz2) == 0); }
inline BOOL FClassnameIs(edict_t* pent, const char* szClassname)
	{ return FStrEq(STRING(VARS(pent)->classname), szClassname); }
inline BOOL FClassnameIs(entvars_t* pev, const char* szClassname)
	{ return FStrEq(STRING(pev->classname), szClassname); }








class CBaseEntity;

// Misc. Prototypes
extern void			UTIL_SetSize			(entvars_t* pev, const Vector &vecMin, const Vector &vecMax);

//MODDD - new
extern void UTIL_SetSizeAlt( entvars_t* pev, const Vector &vecMin, const Vector &vecMax);


extern float		UTIL_VecToYaw			(const Vector &vec);


//MODDD - new
extern float UTIL_VecToYawRadians( const Vector &vecAng );
extern Vector UTIL_VecGetForward2D( const Vector &vecAng );
extern Vector UTIL_VecGetForward( const Vector &vecAng );
extern Vector UTIL_velocityToAngles( const Vector &vecVel);
extern Vector		UTIL_YawToVec			(const float &yaw);


extern Vector		UTIL_VecToAngles		(const Vector &vec);
extern float		UTIL_AngleMod			(float a);
extern float		UTIL_AngleDiff			( float destAngle, float srcAngle );

extern CBaseEntity	*UTIL_FindEntityInSphere(CBaseEntity *pStartEntity, const Vector &vecCenter, float flRadius);
extern CBaseEntity	*UTIL_FindEntityByString(CBaseEntity *pStartEntity, const char *szKeyword, const char *szValue );
extern CBaseEntity	*UTIL_FindEntityByClassname(CBaseEntity *pStartEntity, const char *szName );
extern CBaseEntity	*UTIL_FindEntityByTargetname(CBaseEntity *pStartEntity, const char *szName );
extern CBaseEntity	*UTIL_FindEntityGeneric(const char *szName, Vector &vecSrc, float flRadius );

// returns a CBaseEntity pointer to a player by index.  Only returns if the player is spawned and connected
// otherwise returns NULL
// Index is 1 based
extern CBaseEntity	*UTIL_PlayerByIndex( int playerIndex );

#define UTIL_EntitiesInPVS(pent)			(*g_engfuncs.pfnEntitiesInPVS)(pent)
extern void			UTIL_MakeVectors		(const Vector &vecAngles);

// Pass in an array of pointers and an array size, it fills the array and returns the number inserted
extern int			UTIL_MonstersInSphere( CBaseEntity **pList, int listMax, const Vector &center, float radius );
extern int			UTIL_EntitiesInBox( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask );

//MODD - new version
extern int			UTIL_NonDeadEntitiesInBox( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask );

//no longer necessary.
//extern int			UTIL_EntitiesInBoxAlsoBarnacles( CBaseEntity **pList, int listMax, const Vector &mins, const Vector &maxs, int flagMask );


inline void UTIL_MakeVectorsPrivate( const Vector &vecAngles, float *p_vForward, float *p_vRight, float *p_vUp )
{
	g_engfuncs.pfnAngleVectors( vecAngles, p_vForward, p_vRight, p_vUp );
}

extern void			UTIL_MakeAimVectors		( const Vector &vecAngles ); // like MakeVectors, but assumes pitch isn't inverted
extern void			UTIL_MakeInvVectors		( const Vector &vec, globalvars_t *pgv );

extern void			UTIL_SetOrigin			( entvars_t* pev, const Vector &vecOrigin );
//MODDD - Spirit of HL had this...  why doesn't it have both though anyways?  (Missing the "entvars_t*" version that comes with Half-Life as-is, like above)
extern void UTIL_SetOrigin		( CBaseEntity* pEntity, const Vector &vecOrigin );




extern void			UTIL_ParticleEffect		( const Vector &vecOrigin, const Vector &vecDirection, ULONG ulColor, ULONG ulCount );
extern void			UTIL_ScreenShake		( const Vector &center, float amplitude, float frequency, float duration, float radius );
extern void			UTIL_ScreenShakeAll		( const Vector &center, float amplitude, float frequency, float duration );
extern void			UTIL_ShowMessage		( const char *pString, CBaseEntity *pPlayer );
extern void			UTIL_ShowMessageAll		( const char *pString );
extern void			UTIL_ScreenFadeAll		( const Vector &color, float fadeTime, float holdTime, int alpha, int flags );
extern void			UTIL_ScreenFade			( CBaseEntity *pEntity, const Vector &color, float fadeTime, float fadeHold, int alpha, int flags );

typedef enum { ignore_monsters=1, dont_ignore_monsters=0, missile=2 } IGNORE_MONSTERS;
typedef enum { ignore_glass=1, dont_ignore_glass=0 } IGNORE_GLASS;
extern void			UTIL_TraceLine			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, edict_t *pentIgnore, TraceResult *ptr);
extern void			UTIL_TraceLine			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t *pentIgnore, TraceResult *ptr);
typedef enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 };
extern void			UTIL_TraceHull			(const Vector &vecStart, const Vector &vecEnd, IGNORE_MONSTERS igmon, int hullNumber, edict_t *pentIgnore, TraceResult *ptr);
extern TraceResult	UTIL_GetGlobalTrace		(void);
extern void			UTIL_TraceModel			(const Vector &vecStart, const Vector &vecEnd, int hullNumber, edict_t *pentModel, TraceResult *ptr);

//MODDD
extern void UTIL_fromToBlood(CBaseEntity* arg_entSrc, CBaseEntity* arg_entDest, const float& arg_fltDamage);
extern void UTIL_fromToBlood(CBaseEntity* arg_entSrc, CBaseEntity* arg_entDest, const float& arg_fltDamage, const float &arg_fltdistanceHint);
extern void UTIL_fromToBlood(CBaseEntity* arg_entSrc, CBaseEntity* arg_entDest, const float& arg_fltDamage, const float &arg_fltdistanceHint, Vector* arg_suggestedTraceHullVecEndPos, Vector* arg_suggestedTraceHullStart, Vector* arg_suggestedTraceHullEnd);

extern Vector		UTIL_GetAimVector		(edict_t* pent, float flSpeed);
extern int			UTIL_PointContents		(const Vector &vec);

extern int			UTIL_IsMasterTriggered	(string_t sMaster, CBaseEntity *pActivator);
extern void			UTIL_BloodStream( const Vector &origin, const Vector &direction, int color, int amount );
extern void			UTIL_BloodDrips( const Vector &origin, const Vector &direction, int color, int amount );
extern Vector		UTIL_RandomBloodVector( void );
extern BOOL			UTIL_ShouldShowBlood( int bloodColor );
extern void			UTIL_BloodDecalTrace( TraceResult *pTrace, int bloodColor );
extern void			UTIL_DecalTrace( TraceResult *pTrace, int decalNumber );
extern void			UTIL_PlayerDecalTrace( TraceResult *pTrace, int playernum, int decalNumber, BOOL bIsCustom );
extern void			UTIL_GunshotDecalTrace( TraceResult *pTrace, int decalNumber );

//MODDD
extern void			UTIL_GunshotDecalTraceForceDefault( TraceResult *pTrace, int decalNumber );

extern void			UTIL_Sparks( const Vector &position );
//MODDD - additional versions.
//extern void			UTIL_Sparks( const Vector &position, int arg_ballsToSpawn );
//extern void UTIL_Sparks2( const Vector &position );
//extern void UTIL_Sparks2( const Vector &position, int arg_ballsToSpawn );
extern void UTIL_Sparks2( const Vector &position, int arg_ballsToSpawn, float arg_extraSparkMulti );





//MODDD - new
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern Vector		UTIL_GetProjectileVelocityExtra(Vector& playerVelocity, float velocityMode);
extern void			UTIL_Explosion(const Vector &location, short sprite, float size, int framerate, int flag);
extern void			UTIL_Explosion(const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag);
extern void			UTIL_Explosion(const Vector &location, short sprite, float size, int framerate, int flag, const Vector& altLocation);

extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, short sprite, float size, int framerate, int flag);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag, float shrapMod);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, short sprite, float size, int framerate, int flag, const Vector& altLocation);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, short sprite, float size, int framerate, int flag, const Vector& altLocation, float shrapMode);

extern void			UTIL_Explosion(const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag, const Vector& altLocation);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag, const Vector& altLocation);
extern void			UTIL_Explosion(entvars_t* pev, const Vector &location, float offsetx, float offsety, float offsetz, short sprite, float size, int framerate, int flag, const Vector& altLocation, float shrapMod);

extern void			UTIL_Smoke(const Vector& location, short sprite, float scale, int framerate);
extern void			UTIL_Smoke(const Vector& location, float offsetx, float offsety, float offsetz, short sprite, float scale, int framerate);
extern BOOL			UTIL_getExplosionsHaveSparks();



/*
extern void UTIL_PlaySound(entvars_t* pev, int channel, const char *pszName, float volume, float attenuation );
extern void UTIL_PlaySound(edict_t* pev, int channel, const char *pszName, float volume, float attenuation );
extern void UTIL_PlaySound(entvars_t* pev, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch );
extern void UTIL_PlaySound(edict_t* pev, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch );
*/






// NOTE: use EMIT_SOUND_DYN to set the pitch of a sound. Pitch of 100
// is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
// down to 1 is a lower pitch.   150 to 70 is the realistic range.
// EMIT_SOUND_DYN with pitch != 100 should be used sparingly, as it's not quite as
// fast as EMIT_SOUND (the pitchshift mixer is not native coded).



//MODDD HEADERS: see all four.
void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation);
void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation, BOOL useSoundSentenceSave);
void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int flags, int pitch);
void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int flags, int pitch, BOOL useSoundSentenceSave);

extern void UTIL_PlaySound(entvars_t* entity, int channel, const char *pszName, float volume, float attenuation );
extern void UTIL_PlaySound(entvars_t* entity, int channel, const char *pszName, float volume, float attenuation, BOOL useSoundSentenceSave );
extern void UTIL_PlaySound(edict_t* entity, int channel, const char *pszName, float volume, float attenuation );
extern void UTIL_PlaySound(edict_t* entity, int channel, const char *pszName, float volume, float attenuation, BOOL useSoundSentenceSave  );
extern void UTIL_PlaySound(entvars_t* entity, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch );
extern void UTIL_PlaySound(entvars_t* entity, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch, BOOL useSoundSentenceSave );
extern void UTIL_PlaySound(edict_t* entity, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch );
extern void UTIL_PlaySound(edict_t* entity, int channel, const char *pszName, float volume, float attenuation, int flags, int pitch, BOOL useSoundSentenceSave );




void EMIT_SOUND_DYN(edict_t *entity, int channel, const char *sample, float volume, float attenuation,
						   int flags, int pitch);




//UTIL_PlaySound(entity, channel, sample, volume, attenuation, flags, pitch);

//NOTE: involve "global_soundAttenuationAll" instead of ATTN_NORM
#define EMIT_SOUND_ARRAY_FILTERED( chan, array ) \
	EMIT_SOUND_FILTERED ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], global_soundVolumeAll, global_soundAttenuationAll, 0, RANDOM_LONG(95,105) ); 


#define EMIT_SOUND_ARRAY_STUKA_FILTERED( chan, array ) \
	EMIT_SOUND_FILTERED ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], global_soundVolumeStuka, global_soundAttenuationStuka, 0, RANDOM_LONG(m_voicePitch - 5,m_voicePitch + 5) ); 



//OLD WAY:
	//EMIT_SOUND_FILTERED ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, ATTN_NORM, 0, RANDOM_LONG(95,105) ); 


//#define RANDOM_SOUND_ARRAY( array ) (array) [ RANDOM_LONG(0,ARRAYSIZE( (array) )-1) ]



/*
//MODDDSOUND
void EMIT_SOUND_DYN_REAL(edict_t *entity, int channel, const char *sample, float volume, float attenuation,
						   int flags, int pitch);
*/


/*
inline void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation)
{
	EMIT_SOUND_DYN_FILTERED(entity, channel, sample, volume, attenuation, 0, PITCH_NORM);
}
//MODDD - version that accepts "useSoundSentenceSave"
inline void EMIT_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, float volume, float attenuation, BOOL useSoundSentenceSave)
{
	EMIT_SOUND_DYN_FILTERED(entity, channel, sample, volume, attenuation, 0, PITCH_NORM, useSoundSentenceSave);
}
*/


//WARNING - bypasses soundsentencesave filter. Careful. EMIT_SOUND_FILTERED takes this just fine anyways now.
inline void EMIT_SOUND(edict_t *entity, int channel, const char *sample, float volume, float attenuation)
{
	EMIT_SOUND_DYN(entity, channel, sample, volume, attenuation, 0, PITCH_NORM);
}
inline void STOP_SOUND(edict_t *entity, int channel, const char *sample)
{
	EMIT_SOUND_DYN(entity, channel, sample, 0, 0, SND_STOP, PITCH_NORM);
}



//MODDD - filtered version, so that "STOP" can apply to the sentence-trick too.
//        And yes it works, it goes through _FILTERED like the rest of the soundsentencesave system.
//        After all even a "STOP" is just an order through the exact same sound playing system.
inline void STOP_SOUND_FILTERED(edict_t *entity, int channel, const char *sample)
{
	EMIT_SOUND_FILTERED(entity, channel, sample, 0, 0, SND_STOP, PITCH_NORM);
}
//And me too for specifying whether to use the soundSentenceSave in the call instead of leaving it up to context (the setting on the entity)
inline void STOP_SOUND_FILTERED(edict_t *entity, int channel, const char *sample, BOOL useSoundSentenceSave)
{
	EMIT_SOUND_FILTERED(entity, channel, sample, 0, 0, SND_STOP, PITCH_NORM, useSoundSentenceSave);
}







void EMIT_SOUND_SUIT(edict_t *entity, const char *sample);
void STOP_SOUND_SUIT(edict_t *entity, const char *sample);
void EMIT_GROUPID_SUIT(edict_t *entity, int isentenceg);
void EMIT_GROUPNAME_SUIT(edict_t *entity, const char *groupname);



#define PRECACHE_SOUND_ARRAY( a ) \
	{ for (int i = 0; i < ARRAYSIZE( a ); i++ ) PRECACHE_SOUND((char *) a [i]); }
//#define PRECACHE_SOUND_ARRAY UTIL_PRECACHESOUND_ARRAY
//#define PRECACHE_SOUND_ARRAY UTIL_PRECACHESOUND_ARRAY


//No need to worry, this "method?" is almost completely unused.
#define EMIT_SOUND_ARRAY_DYN( chan, array ) \
	EMIT_SOUND_DYN ( ENT(pev), chan , array [ RANDOM_LONG(0,ARRAYSIZE( array )-1) ], 1.0, ATTN_NORM, 0, RANDOM_LONG(95,105) ); 

//This is used a lot more - much more flexible.
#define RANDOM_SOUND_ARRAY( array ) (array) [ RANDOM_LONG(0,ARRAYSIZE( (array) )-1) ]





//MODDD - several new versions added here too.
extern void UTIL_EmitAmbientSound_Filtered( entvars_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation );
extern void UTIL_EmitAmbientSound_Filtered( entvars_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, BOOL useSoundSentenceSave );
extern void UTIL_EmitAmbientSound_Filtered( entvars_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch );
extern void UTIL_EmitAmbientSound_Filtered( entvars_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch, BOOL useSoundSentenceSave );
extern void UTIL_EmitAmbientSound_Filtered( edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch );
extern void UTIL_EmitAmbientSound_Filtered( edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch, BOOL useSoundSentenceSave );
extern void UTIL_EmitAmbientSound( edict_t *entity, const Vector &vecOrigin, const char *samp, float vol, float attenuation, int fFlags, int pitch );

extern void UTIL_PRECACHESOUND(char* path);
extern void UTIL_PRECACHESOUND(char* path, BOOL dontSkipSave);


//any other places these need externing at?
extern void UTIL_PRECACHESOUND_ARRAY(const char* a[], int aSize);
extern void UTIL_PRECACHESOUND_ARRAY(const char* a[], int aSize, BOOL dontSkipSave);











extern void UTIL_TE_ShowLine(const Vector& vec1, const Vector& vec2);
extern void UTIL_TE_ShowLine(float x1, float y1, float z1, float x2, float y2, float z2);

extern void UTIL_drawRect(const Vector& vec1, const Vector& vec2);
extern void UTIL_drawRect(float x1, float y1, float z1, float x2, float y2, float z2);

extern void UTIL_drawBox(const Vector& vec1, const Vector& vec2);
extern void UTIL_drawBox(float x1, float y1, float z1, float x2, float y2, float z2);





extern void UTIL_drawLineFrame(const Vector& vec1, const Vector& vec2, int r, int g, int b);
extern void UTIL_drawLineFrame(const Vector& vec1, const Vector& vec2, int width, int r, int g, int b);
extern void UTIL_drawLineFrame(float x1, float y1, float z1, float x2, float y2, float z2, int width, int r, int g, int b);

extern void UTIL_drawPointFrame(const Vector& vecPoint, int width, int r, int g, int b);
extern void UTIL_drawPointFrame(float point_x, float point_y, float point_z, int width, int r, int g, int b);


extern void UTIL_drawRectFrame(const Vector& vec1, const Vector& vec2, int width, int r, int g, int b);
extern void UTIL_drawRectFrame(float x1, float y1, float z1, float x2, float y2, float z2, int width, int r, int g, int b);

extern void UTIL_drawBoxFrame(const Vector& vec1, const Vector& vec2, int width, int r, int g, int b);
extern void UTIL_drawBoxFrame(float x1, float y1, float z1, float x2, float y2, float z2, int width, int r, int g, int b);

extern void UTIL_drawLineFrameBoxAround(const Vector& vec1, int width, int boxSize, int r, int g, int b);
extern void UTIL_drawLineFrameBoxAround(float x1, float y1, float z1, int width, int boxSize, int r, int g, int b);

extern void UTIL_drawLineFrameBoxAround2(const Vector& vec1, int width, int boxSize, int r, int g, int b);
extern void UTIL_drawLineFrameBoxAround2(float x1, float y1, float z1, int width, int boxSize, int r, int g, int b);

extern void UTIL_drawLineFrameBoxAround3(const Vector& vec1, int width, int boxSize, int r, int g, int b);
extern void UTIL_drawLineFrameBoxAround3(float x1, float y1, float z1, int width, int boxSize, int r, int g, int b);





extern void UTIL_TE_BeamPoints(const Vector& vec1, const Vector& vec2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);
extern void UTIL_TE_BeamPoints(float x1, float y1, float z1, float x2, float y2, float z2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);

extern void UTIL_TE_BeamPoints_Rect(const Vector& vec1, const Vector& vec2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);
extern void UTIL_TE_BeamPoints_Rect(float x1, float y1, float z1, float x2, float y2, float z2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);

extern void UTIL_TE_BeamPoints_Box(const Vector& vec1, const Vector& vec2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);
extern void UTIL_TE_BeamPoints_Box(float x1, float y1, float z1, float x2, float y2, float z2, int frameStart, int frameRate, int life, int width, int noise, int r, int g, int b, int brightness, int speed);

















extern Vector UTIL_rotateShift(const Vector& src, const Vector& forward );
extern Vector UTIL_rotateShift(const Vector& src, const float forwardX, const float forwardY, const float forwardZ );
extern Vector UTIL_rotateShift(const float srcX, const float srcY, const float srcZ, const Vector& forward);
extern Vector UTIL_rotateShift(const float srcX, const float srcY, const float srcZ, const float forwardX, const float forwardY, const float forwardZ );





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






extern void			UTIL_Ricochet( const Vector &position, float scale );
extern void			UTIL_StringToVector( float *pVector, const char *pString );
extern void			UTIL_StringToIntArray( int *pVector, int count, const char *pString );


//simple version for just one number.
extern float clamp(float argTest, float argMin, float argMax);

//MODDD - version that skips normalization offered.
extern Vector		UTIL_ClampVectorToBox( const Vector &input, const Vector &clampSize );
extern Vector		UTIL_ClampVectorToBoxNonNormalized( const Vector &input, const Vector &clampSize );
extern float		UTIL_Approach( float target, float value, float speed );
extern float		UTIL_ApproachAngle( float target, float value, float speed );
extern float		UTIL_AngleDistance( float next, float cur );




//extern void UTIL_playFleshHitSound(entvars_t* pev);
//extern void UTIL_playFleshHitSound(edict_t* pev);


extern Vector UTIL_Intersect( Vector vecSrc, Vector vecDst, Vector vecMove, float flSpeed );

extern Vector UTIL_projectionComponent(const Vector& u, const Vector& n);
extern Vector UTIL_projectionComponentPreserveMag(const Vector& u, const Vector& n);







	
extern void easyPrintLineHgrunt(const char *format, ...);
extern void easyPrintLinePanthereye(const char *format, ...);
extern void easyPrintLineSquadmonster(const char *format, ...);
extern void easyPrintLineHassault(const char *format, ...);
extern void easyPrintLineGargantua(const char *format, ...);
extern void easyprintlineBarnacle(const char *format, ...);
extern void easyPrintLineHoundeye(const char *format, ...);



////////////////////////////////////////////////////////////////////

//TEMP!
extern void easyPrintLineGroup1(const char *format, ...);
extern void easyPrintLineGroup2(const char *format, ...);
extern void easyPrintLineGroup3(const char *format, ...);
extern void easyPrintLineGroup4(const char *format, ...);


/*
#define easyPrintLineGroup1(...) DUMMY
#define easyPrintLineGroup2(...) DUMMY
#define easyPrintLineGroup3(...) DUMMY
#define easyPrintLineGroup4(...) DUMMY
*/





extern void			UTIL_Remove( CBaseEntity *pEntity );




//MODDD
extern BOOL			UTIL_IsDeadEntity( CBaseEntity* ent);
//MODDD
extern BOOL			UTIL_IsAliveEntity( CBaseEntity* ent);

//MODDD
extern BOOL			UTIL_IsValidEntity( CBaseEntity* ent);

extern BOOL			UTIL_IsValidEntity( edict_t *pent );
extern BOOL			UTIL_TeamsMatch( const char *pTeamName1, const char *pTeamName2 );

// Use for ease-in, ease-out style interpolation (accel/decel)
extern float		UTIL_SplineFraction( float value, float scale );

// Search for water transition along a vertical line
extern float		UTIL_WaterLevel( const Vector &position, float minz, float maxz );
extern void			UTIL_Bubbles( Vector mins, Vector maxs, int count );
extern void			UTIL_BubbleTrail( Vector from, Vector to, int count );

// allows precacheing of other entities
extern void			UTIL_PrecacheOther( const char *szClassname );











class CBasePlayerItem;
class CBasePlayer;
extern BOOL UTIL_GetNextBestWeapon( CBasePlayer *pPlayer, CBasePlayerItem *pCurrentWeapon );




// prints a message to the HUD say (chat)
extern void			UTIL_SayText( const char *pText, CBaseEntity *pEntity );
extern void			UTIL_SayTextAll( const char *pText, CBaseEntity *pEntity );


typedef struct hudtextparms_s
{
	float		x;
	float		y;
	int			effect;
	byte		r1, g1, b1, a1;
	byte		r2, g2, b2, a2;
	float		fadeinTime;
	float		fadeoutTime;
	float		holdTime;
	float		fxTime;
	int			channel;
} hudtextparms_t;

// prints as transparent 'title' to the HUD
extern void			UTIL_HudMessageAll( const hudtextparms_t &textparms, const char *pMessage );
extern void			UTIL_HudMessage( CBaseEntity *pEntity, const hudtextparms_t &textparms, const char *pMessage );

// for handy use with ClientPrint params
extern char *UTIL_dtos1( int d );
extern char *UTIL_dtos2( int d );
extern char *UTIL_dtos3( int d );
extern char *UTIL_dtos4( int d );

// Writes message to console with timestamp and FragLog header.
extern void			UTIL_LogPrintf( char *fmt, ... );

// Sorta like FInViewCone, but for nonmonsters. 
extern float UTIL_DotPoints ( const Vector &vecSrc, const Vector &vecCheck, const Vector &vecDir );

extern void UTIL_StripToken( const char *pKey, char *pDest );// for redundant keynames

// Misc functions
extern void SetMovedir(entvars_t* pev);
extern Vector VecBModelOrigin( entvars_t* pevBModel );
extern int BuildChangeList( LEVELLIST *pLevelList, int maxList );








extern char gszallsentencenames[CVOXFILESENTENCEMAX][CBSENTENCENAME_MAX];
extern int gcallsentences;

int USENTENCEG_Pick(int isentenceg, char *szfound);
int USENTENCEG_PickSequential(int isentenceg, char *szfound, int ipick, int freset);
void USENTENCEG_InitLRU(unsigned char *plru, int count);

void SENTENCEG_Init();
void SENTENCEG_Stop(edict_t *entity, int isentenceg, int ipick);
int SENTENCEG_PlayRndI(edict_t *entity, int isentenceg, float volume, float attenuation, int flags, int pitch);
int SENTENCEG_PlayRndSz(edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch);
//MODDD - new
int SENTENCEG_PlayRndSz_Ambient(edict_t *entity, Vector vOrigin, const char *szgroupname, float volume, float attenuation, int flags, int pitch);

int SENTENCEG_PlaySequentialSz(edict_t *entity, const char *szrootname, float volume, float attenuation, int flags, int pitch, int ipick, int freset);
int SENTENCEG_GetIndex(const char *szrootname);
int SENTENCEG_Lookup(const char *sample, char *sentencenum);

void TEXTURETYPE_Init();
char TEXTURETYPE_Find(char *name);
float TEXTURETYPE_PlaySound(TraceResult *ptr,  Vector vecSrc, Vector vecEnd, int iBulletType);







#define PLAYBACK_EVENT( flags, who, index ) PLAYBACK_EVENT_FULL( flags, who, index, 0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );
#define PLAYBACK_EVENT_DELAY( flags, who, index, delay ) PLAYBACK_EVENT_FULL( flags, who, index, delay, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, 0, 0, 0, 0 );

#define GROUP_OP_AND	0
#define GROUP_OP_NAND	1

extern int g_groupmask;
extern int g_groupop;

class UTIL_GroupTrace
{
public:
	UTIL_GroupTrace( int groupmask, int op );
	~UTIL_GroupTrace( void );

private:
	int m_oldgroupmask, m_oldgroupop;
};

void UTIL_SetGroupTrace( int groupmask, int op );
void UTIL_UnsetGroupTrace( void );

int UTIL_SharedRandomLong( unsigned int seed, int low, int high );
float UTIL_SharedRandomFloat( unsigned int seed, float low, float high );

float UTIL_WeaponTimeBase( void );

//MODDDMIRROR
Vector UTIL_MirrorVector( Vector angles );
Vector UTIL_MirrorPos ( Vector endpos );

extern void UTIL_generateFreakyLight( const Vector& arg_origin);











//PrintQueue related:

extern Vector UTIL_getFloor(const Vector &vecStart, const float& distDown, IGNORE_MONSTERS igmon, edict_t *pentIgnore );
extern BOOL isErrorVector(const Vector& vec);















#ifndef CLIENT_DLL

#define PRINTQUEUE_NAMESTRINGSIZE 16
#define PRINTQUEUE_STRINGS 5
#define PRINTQUEUE_STRINGSIZE 30
//#define PRINTQUEUE_NUMBEROF 4     No, varries per setup.

// "+ 2" for boundary room (commans, stoppers, etc.)
#define PRINTQUEUE_TOTALEXPECTED PRINTQUEUE_STRINGS * (PRINTQUEUE_STRINGSIZE + 5)

class PrintQueue{
public:
	int latestPlace;
	char contents[PRINTQUEUE_STRINGS][PRINTQUEUE_STRINGSIZE];
	char displayName[PRINTQUEUE_NAMESTRINGSIZE];

	PrintQueue(const char* arg_name){
		//startest index to add.
		latestPlace = 0;
		int nameLength = lengthOfString(arg_name, PRINTQUEUE_NAMESTRINGSIZE);
		strncpyTerminate(&displayName[0], arg_name, nameLength);
	}

	inline void easyPrint(char *format, ...){
		va_list argptr;
		va_start(argptr, format);
		char* tempResult = UTIL_VarArgsVA(format, argptr );;
		va_end(argptr);
	}

	
	
	void sendToPrintQueue(const char* src, ...){
		if(latestPlace < 5){
			va_list argptr;
			va_start(argptr, src);
			const char* tempResult = UTIL_VarArgsVA(src, argptr );
			//strncpyTerminate(&contents[latestPlace][0], &UTIL_VarArgsVA(src, argptr )[0], lengthOfString(src, PRINTQUEUE_STRINGSIZE) );
			strncpyTerminate(&contents[latestPlace][0], tempResult, lengthOfString(tempResult, PRINTQUEUE_STRINGSIZE) );
			va_end(argptr);
			latestPlace++;
		}else{
			//PROBLEM! TOO MANY IN QUEUE!
		}
	}
	
	









	void receivePrintQueue(char* dest, int* positionOverhead){
		//next place to start writing characters to.
		int* recentIndex = positionOverhead;
		appendTo(dest, displayName, recentIndex, ':');
		if(latestPlace == 0){
			//just print <n>.
			appendToAndTerminate(dest, "<n>", recentIndex, '|');
		}else{
			for(int i = 0; i < latestPlace; i++){
				appendTo(dest, (i+1), recentIndex, ':');
				//appendTo(dest, '*', recentIndex);

				if(i == latestPlace - 1){
					appendTo(dest, &contents[i][0], recentIndex);
				}else{
					appendTo(dest, &contents[i][0], recentIndex, ',');
				}
			}//END OF for(...)
			appendTo(dest, '|', recentIndex);
		}//END OF else OF if(latestPlace == 0)
		//no, already occurring.
		//*positionOverhead += recentIndex;
	}//END OF sendPrintQueue
	void clearPrintQueue(){
		//reset.  Overwrite or ignore others.
		latestPlace = 0;
	}


};

//alternate way now.
extern void PRINTQUEUE_STUKA_SEND(PrintQueue& toPrint, const char* src, ...);


#endif //END




extern void precacheAll(void);

extern BOOL UTIL_IsFacing( entvars_t *pevTest, const Vector &vecLookAtTest );
extern BOOL UTIL_IsFacing( entvars_t *pevTest, const Vector &vecLookAtTest, const float& arg_tolerance );
extern BOOL UTIL_IsFacingAway( entvars_t *pevTest, const Vector &vecLookAtTest );
extern BOOL UTIL_IsFacingAway( entvars_t *pevTest, const Vector &vecLookAtTest, const float& arg_tolerance );





extern float randomInvert(const float& arg_flt);
extern float randomAbsoluteValue(const float& arg_fltMin, const float& arg_fltMax);
extern int randomValueInt(const int& arg_min, const int& arg_max);
extern float randomValue(const float& arg_fltMin, const float& arg_fltMax);

extern void UTIL_deriveColorFromMonsterHealth(const float& curHealth, const float& maxHealth, int& r, int& g, int& b);

extern void attemptSendBulletSound(const Vector& bulletHitLoc, entvars_t* pevShooter);

//extern CBaseEntity *FindEntityForwardOLDVERSION( CBaseEntity *pMe );
extern CBaseEntity *FindEntityForward( CBasePlayer *pMe );





extern float timeDelayFilter(float arg_delay);
extern Vector getRotatedVectorAboutZAxis(const Vector& arg_vec, const float& arg_deg);

extern void UTIL_ServerMassCVarReset(entvars_t* pev);








extern void EASY_CVAR_PRINTIF(float geh, const char *szFmt, ... );
extern void EASY_CVAR_PRINTIF_VECTOR(float geh, const char *szFmt, ... );

extern BOOL getGermanModelsAllowed(void);


extern BOOL verifyModelExists(char* path);




extern int attemptInterpretSpawnFlag(const char* pszSpawnFlags);


extern int UTIL_BloodColorRedFilter(BOOL robotReplacementModelExists);


///////////////////////////////////////////////////////////////////////////
//MODDD - thanks to Spirit of Half-Life 1.9!
///////////////////////////////////////////////////////////////////////////

//LRC- the values used for the new "global states" mechanism.
typedef enum
{
	STATE_OFF = 0,	// disabled, inactive, invisible, closed, or stateless. Or non-alert monster.
	STATE_TURN_ON,  // door opening, env_fade fading in, etc.
	STATE_ON,		// enabled, active, visisble, or open. Or alert monster.
	STATE_TURN_OFF, // door closing, monster dying (?).
	STATE_IN_USE,	// player is in control (train/tank/barney/scientist).
					// In_Use isn't very useful, I'll probably remove it.
} STATE;

extern char* GetStringForState( STATE state );

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////




Vector projectionOntoPlane(Vector arg_vectOnto, Vector arg_planeNormal);
extern void matrices_setup(int arg_cols, int arg_rows);
extern void matrices_sendRow(float arg_1, float arg_2, float arg_c);
extern void matrices_getResults(float* arg_result_t, float* arg_result_s);
extern void matrices_printOut();
extern void matrices_rowReduce();
extern void matrices_pivotizeColumn(int rowNumber, int columnNumber);
extern void matrices_divideRow(int rowNumb, float divideBy);
extern void matrices_swapRows(int row1, int row2);


extern CBaseEntity* UTIL_CreateNamedEntity(const char* arg_entityName);


extern BOOL entityHidden(CBaseEntity* test);
extern BOOL entityHidden(edict_t* test);

extern void UTIL_playOrganicGibSound(entvars_t* pevSoundSource);
extern void UTIL_playMetalGibSound(entvars_t* pevSoundSource);









extern void updateCVarRefs(entvars_t *pev);

extern void turnWorldLightsOn();
extern void turnWorldLightsOff();




extern void OnBeforeChangeLevelTransition(void);
extern void OnMapLoadPreStart(void);
extern void OnMapLoadStart(void);
extern void OnMapLoadEnd(void);

extern void ResetDynamicStaticIDs(void);
extern void SaveDynamicIDs(CGlobalState* argGS);
extern void RestoreDynamicIDs(CGlobalState* argGS);










#define GIB_DUMMY_ID 0
#define GIB_HUMAN_ID 1
#define GIB_ALIEN_ID 2
#define GIB_GERMAN_ID 3

#define GIB_EXTRAMETAL_1_ID 4
#define GIB_EXTRAMETAL_2_ID 5
#define GIB_EXTRAMETAL_3_ID 6
#define GIB_EXTRAMETAL_4_ID 7
#define GIB_EXTRAMETAL_5_ID 8
#define GIB_EXTRAMETAL_6_ID GIB_GERMAN_ID


typedef struct GibInfo_s{
	const char* modelPath;
	int bodyMin;
	int bodyMax;
	int bloodColor;

} GibInfo_t;


extern GibInfo_t aryGibInfo[];









#endif //END OF #ifdef UTIL_H
