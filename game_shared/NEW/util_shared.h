
#ifndef UTIL_SHARED_H
#define UTIL_SHARED_H

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// preliminary includes
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


typedef int BOOL;

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif




#ifdef CLIENT_DLL
// should "#pragma once" be used a lot more often?


//necessary?
//#include "cvardef.h"

//Just need this to give the client access to BOOL, the FALSE / TRUE constants, and some file-related things like MAX_PATH.
#include "windows.h"

//#include "WinDef.h"
/*
//typedef int BOOL;
#define FALSE 0
#define TRUE (!FALSE)
*/

/*
//MODDD - NOTE. This is from extdll.h, usually serverside only. Is going everywhere safe?
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prevent tons of unused windows definitions
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#include "windows.h"
#else // _WIN32
#define FALSE 0
#define TRUE (!FALSE)
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef int BOOL;
#define MAX_PATH PATH_MAX
#include <limits.h>
#include <stdarg.h>

//MODDD - moved to util_shared.cpp, mostly.
//#ifndef min
//#define min(a,b)  (((a) < (b)) ? (a) : (b))
//#endif
//#ifndef max
//#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define _vsnprintf(a,b,c,d) vsnprintf(a,b,c,d)
//#endif

#endif //_WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/


#else
//SERVER
#include "cdll_dll.h"


#endif
//SHARED
#include "util_preprocessor.h"
#include "util_printout.h"


EASY_CVAR_EXTERN(hiddenMemPrintout)


//MODDD - from clientside's cl_util.h.
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))

// NOTE - supplied by base libaries too now in modern VS, but doesn't seem to hurt here.
#define fabs(x)	   ((x) > 0 ? (x) : 0 - (x))



#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_HULL_MAX		Vector( 16,  16,  36)
#define VEC_HUMAN_HULL_MIN	Vector( -16, -16, 0 )
#define VEC_HUMAN_HULL_MAX	Vector( 16, 16, 72 )
#define VEC_HUMAN_HULL_DUCK	Vector( 16, 16, 36 )

#define VEC_VIEW			Vector( 0, 0, 28 )

#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )
#define VEC_DUCK_HULL_MAX	Vector( 16,  16,  18)
#define VEC_DUCK_VIEW		Vector( 0, 0, 12 )


// defaults for clientinfo messages
#define DEFAULT_VIEWHEIGHT	28
#define VEC_DUCK_VIEW_Z 12



#define DMG_GENERIC			0			// generic damage was done
#define DMG_CRUSH			(1 << 0)	// crushed by falling or moving object
#define DMG_BULLET			(1 << 1)	// shot
#define DMG_SLASH			(1 << 2)	// cut, clawed, stabbed
#define DMG_BURN			(1 << 3)	// heat burned
#define DMG_FREEZE			(1 << 4)	// frozen
#define DMG_FALL			(1 << 5)	// fell too far
#define DMG_BLAST			(1 << 6)	// explosive blast damage
#define DMG_CLUB			(1 << 7)	// crowbar, punch, headbutt
#define DMG_SHOCK			(1 << 8)	// electric shock
#define DMG_SONIC			(1 << 9)	// sound pulse shockwave
#define DMG_ENERGYBEAM		(1 << 10)	// laser or other high energy beam 

//MODDD - note: Why the Hell is  1 << 11   missing?

#define DMG_NEVERGIB		(1 << 12)	// with this bit OR'd in, no damage type will be able to gib victims upon death
#define DMG_ALWAYSGIB		(1 << 13)	// with this bit OR'd in, any damage type can be made to gib victims upon death.
#define DMG_DROWN			(1 << 14)	// Drowning

//...what?  Nothing uses this constant.
//#define DMG_FIRSTTIMEBASED  DMG_DROWN

// time-based damage
//NOTE:  I believe this is just a way of saying, 1's for all timed damage (included), 0's for all else.
//       Any actual continual time damage is indicated by the 2nd damage bit (bitsDamageTypeMod) having DMG_TIMEDEFFECT or DMG_TIMEDEFFECTIGNORE.
//       This just shows the timed damages of the first bitmask as initial strikes.
//       Any continual damage still uses the 2nd bitmask for the aforementioned choices regardless.

//#define DMG_TIMEBASED		(~(0x3fff))	// mask for time-based damage
// Going with the version from clientside files to support TF damage types... I guess?
#define DMG_TIMEBASED		(~(0xff003fff))	// mask for time-based damage

#define DMG_PARALYZE		(1 << 15)	// slows affected creature down
#define DMG_NERVEGAS		(1 << 16)	// nerve toxins, very bad
#define DMG_POISON			(1 << 17)	// blood poisioning
#define DMG_RADIATION		(1 << 18)	// radiation exposure
#define DMG_DROWNRECOVER	(1 << 19)	// drowning recovery
#define DMG_ACID			(1 << 20)	// toxic chemicals or acid burns
#define DMG_SLOWBURN		(1 << 21)	// in an oven
#define DMG_SLOWFREEZE		(1 << 22)	// in a subzero freezer
#define DMG_MORTAR			(1 << 23)	// Hit by air raid (done to distinguish grenade from mortar)



//TF ADDITIONS
// Don't even know why this was ever anywhere, but ok.  From clientside files (health.h & eventscripts.h)
#define DMG_IGNITE			(1 << 24)	// Players hit by this begin to burn
#define DMG_RADIUS_MAX		(1 << 25)	// Radius damage with this flag doesn't decrease over distance
#define DMG_RADIUS_QUAKE	(1 << 26)	// Radius damage is done like Quake. 1/2 damage at 1/2 radius.
#define DMG_IGNOREARMOR		(1 << 27)	// Damage ignores target's armor
#define DMG_AIMED			(1 << 28)   // Does Hit location damage
#define DMG_WALLPIERCING	(1 << 29)	// Blast Damages ents through walls

#define DMG_CALTROP				(1<<30)
#define DMG_HALLUC				(1<<31)

// TF Healing Additions for TakeHealth
#define DMG_IGNORE_MAXHEALTH	DMG_IGNITE
// TF Redefines since we never use the originals
#define DMG_NAIL				DMG_SLASH
#define DMG_NOT_SELF			DMG_FREEZE

#define DMG_TRANQ				DMG_MORTAR
#define DMG_CONCUSS				DMG_SONIC





//MODDD - new.  Careful not to overflow this integer, these powers of 2 are getting high!
//Would have started with "24", but 24 - 31 may be placeholders for Team Fortress damage types, 
//according to health.h.
//NOTICE: beyond "31" in 1 << 31  is not valid.  Using a 2nd bitmask when referring to these values...

//#define DMG_TIMEDEFFECT			(1 << 32)	// timed damage that must be differentiated from "generic".
//#define DMG_BLEEDING			(1 << 33)   // bleeding, usually inflicted from strong melee attacks.  Medkits cure it.

#define DMG_TIMEDEFFECT			(1 << 0)	// timed damage that must be differentiated from "generic".  This is non-initial strike damage.
//Do not confuse with the mask "DMG_TIMEBASED", which is unique and not inclusive of any other damage types.
#define DMG_BLEEDING			(1 << 1)   // bleeding, usually inflicted from strong melee attacks.  Medkits cure it.
#define DMG_TIMEDEFFECTIGNORE	(1 << 2)   //same as TIMEDEFFECT, but made to ignore armor (regardless of the cvar).
#define DMG_BARNACLEBITE		(1 << 3)   //not timed.  Just sent by the barnacle's execution bite to NPCs to mark not to ignore (if they would).
#define DMG_GAUSS				(1 << 4)   //coming from the player's gauss weapon. Some things (apache) are now immune to it.  ...or not? that got canceled.
#define DMG_HITBOX_EQUAL		(1 << 5)   //signal that this type of damage can't be increased by hitting particular hitboxes.
										   //It is still completely up to any given monster's TraceAttack / TakeDamage to implement this
										   //(check for the presence of DMG_HITBOX_EQUAL in bitsDamageTypeMod and deny enhancing damage
										   // per headshots, etc. accordingly).
										   //For instance, for lightning attacks, different amounts of damage for body, leg, arm, or headshots don't make sense.
										   //It's possible damage from NPC's just shouldn't even do this kind of damage anyways.


//Which types of damage in the new mask are secondary?
//When other types are added, add them like   (DMG_BLEEDING | NEW | ALSONEW | ...)
#define DMG_TIMEBASEDMOD		(DMG_BLEEDING)



// these are the damage types that are allowed to gib corpses
#define DMG_GIB_CORPSE		( DMG_CRUSH | DMG_FALL | DMG_BLAST | DMG_SONIC | DMG_CLUB )

// these are the damage types that have client hud art
//MODDD - needed edit for bleeding damage!

#define DMG_SHOWNHUD		(DMG_POISON | DMG_ACID | DMG_FREEZE | DMG_SLOWFREEZE | DMG_DROWN | DMG_BURN | DMG_SLOWBURN | DMG_NERVEGAS | DMG_RADIATION | DMG_SHOCK)

//NOTICE: "DMG_TIMEDEFFECT" is just for non-initial strike damage, and is not what triggers the signs.  It is too inspecific.
//It is not a mask, but a way to tell apart timed damage from generic damage when needed (the "ignore armor" cvar).
#define DMG_SHOWNHUDMOD		(DMG_BLEEDING);


//MODDD - new bitmasks for "curables".  That is, conditions that can be cured by some item.
//The significance of this list is playing on hard difficulty with the "timedDamageEndlessOnHard" 
//CVar on.  This way, only CURABLES (by some item) last forever, and non-curables don't (that
//would be far too cruel).
//Perhaps "DMG_ACID" should appear ingame and be cured by the antidote or something? unsure.
//UNUSED, this mechanism wasn't needed actually.  Or wasn't particularly helpful; would have led to 
//taking the "long" way.
#define DMG_CURABLE			(DMG_NERVEGAS | DMG_POISON | DMG_RADIATION)
#define DMG_CURABLEMOD		(DMG_BLEEDING)

//MODDD - any damages
#define DMG_ARMORBLOCKEXCEPTION		(0) //empty.
//Actually, doesn't involve "bleeding".  That is the initial strike.  It will leave "DMG_TIMEDEFFECTIGNORE", so it works.
#define DMG_ARMORBLOCKEXCEPTIONMOD	(DMG_TIMEDEFFECTIGNORE)




// NOTE: tweak these values based on gameplay feedback:
//(MODDD - that comment is not mine.)


//MODDD - changed back to a previous setup that makes more sense, since antidotes are present.
//See player.cpp for that one.  Search "REVERTED TO OLD TIMED DAMAGE".
/*
#define PARALYZE_DURATION	2		// number of 2 second intervals to take damage
#define PARALYZE_DAMAGE		1.0		// damage to take each 2 second interval

#define NERVEGAS_DURATION	2
#define NERVEGAS_DAMAGE		5.0

#define POISON_DURATION		5
#define POISON_DAMAGE		2.0

#define RADIATION_DURATION	2
#define RADIATION_DAMAGE	1.0

#define ACID_DURATION		2
#define ACID_DAMAGE			5.0

#define SLOWBURN_DURATION	2
#define SLOWBURN_DAMAGE		1.0

#define SLOWFREEZE_DURATION	2
#define SLOWFREEZE_DAMAGE	1.0
*/






/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// extern things. CONTENT HERE.
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



//#include "activity.h"
//#include "enginecallback.h"
class CBaseEntity;


extern int giAmmoIndex;
extern BOOL globalflag_muteDeploySound;



// Moved here from in_defs.h.  Nothing else, file removed too.
// up / down
#define PITCH 0
// left / right
#define YAW 1
// fall over
#define ROLL 2 




// Not sure why DLLEXPORT was re-defined in so many files:
//  This was defined the same in the following files (and removed from since):
//  cl_dll/cdll_int.cpp
//  cl_dll/demo.cpp
//  cl_dll/entity.cpp
//  cl_dll/GameStudioModelRenderer.cpp
//  cl_dll/in_defs.h
//  cl_dll/tri.cpp
// These files appear to rely on that same definition anyway:
//  cl_dll/input.cpp
//  cl_dll/inputw32.cpp
//  cl_dll/in_camera.cpp
//  cl_dll/view.cpp

// The version used in those files has been moved here (renamed to DLLEXPORT_2 from DLLEXPORT, same for anything referring to this one).
#define DLLEXPORT_2 __declspec( dllexport )

// HOWEVER, engine/eiface.h had this instead:
#ifdef _WIN32
#define DLLEXPORT __stdcall
#else
#define DLLEXPORT
#endif

//...I have no clue why this is the only different one.
// A few other places require that definition instead.  They'll use the old name.

// There are also a few other similarly named ones, like EXPORT.  And _DLLEXPORT of cl_dll/hud_iface.c?  Weird.
// Moved here though to keep them together:
#define _DLLEXPORT __declspec( dllexport )

// There is also 'define EXPORT' in dlls/cbase.h, but leaving it since this is one of the earliest files in most compiles
// and it shouldn't need to include 'util_shared.h' to reach here first.  It is shared client/serverside anyway.



// These are player-specific but still need to be included here to reach the HUD logic clientside.
////////////////////////////////////////////////////////////////////////////
// How many seconds worth of air does the air tank have?  Coordinate with the GUI (clientside, ammo.cpp)
#define PLAYER_AIRTANK_TIME_MAX 120
// How much longjump charge can the player hold at one time?
#define PLAYER_LONGJUMPCHARGE_MAX 100
// How much charge does one 'longjumpcharge' grant?
#define PLAYER_LONGJUMP_PICKUPADD 25
////////////////////////////////////////////////////////////////////////////

#define DEFAULT_SPARK_BALLS 6


//GERMAN GIBS
//~as seen here:
//m_iGermanGibModelIndex =PRECACHE_MODEL ("models/germanygibs.mdl");
//pGib->Spawn( "models/germangibs.mdl" );
//~there is a slight variation in name: germanygibs vs. germangibs.  If this is just a mistake from Valve, we'll just go with either.
//UPDATE: We're sticking to the name "g_hgibs.mdl" to be more consistent with other naming.
#define GERMAN_GIB_PATH "models/g_hgibs.mdl"





//MODDD - CRITICAL.  ARRAYSIZE macro now won't happen if already defined, such as by Windows.
// If overriding those turns out to be necssary, forget these 'ifndef' checks.
#ifndef ARRAYSIZE
	// Was in ev_hldm.cpp... strangely enough.
#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))
#endif


// From dlls/util.h, may as well be shared since SND_CHANGE_PITCH is needed in cl_dll/ev_hldm.h.
#define SND_SPAWNING		(1<<8)		// duplicated in protocol.h we're spawing, used in some cases for ambients 
#define SND_STOP			(1<<5)		// duplicated in protocol.h stop sound
#define SND_CHANGE_VOL		(1<<6)		// duplicated in protocol.h change sound vol
#define SND_CHANGE_PITCH	(1<<7)		// duplicated in protocol.h change sound pitch





// Originally in cl_dll/hl/ev_hldm.h and dlls/weapons.h.
// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, // glock
	BULLET_PLAYER_MP5, // mp5
	BULLET_PLAYER_357, // python
	BULLET_PLAYER_BUCKSHOT, // shotgun
	BULLET_PLAYER_CROWBAR, // crowbar swipe

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,
} Bullet;














extern int MaxAmmoCarry(int iszName);
//MODDD NEW - prototype so that other places can call this method too.
extern void AddAmmoNameToAmmoRegistry(const char* szAmmoname);


//MODDD - have this.
extern const char* FClassname(CBaseEntity* derp);

extern BOOL stringStartsWith(const char* source, const char* startswith);


extern BOOL checkMatch(const char* src1, char* src2);
extern BOOL checkMatch(const char* src1, char* src2, int size);
extern BOOL checkMatchIgnoreCase(const char* src1, char* src2);
extern BOOL checkMatchIgnoreCase(const char* src1, char* src2, int size);


extern const char* tryIntToString(int arg_src);
extern int tryStringToInt(const char* arg_src);
extern float tryStringToFloat(const char* arg_src);
extern const char* tryFloatToString(float arg_src);
extern void tryFloatToStringBuffer(char* dest, float arg_src);



extern void lowercase(char* src);
extern void lowercase(char* src, int size);






extern float roundToNearest(float num);


extern void UTIL_substring(char* dest, const char* src, int startIndex, int endIndex);

extern int UTIL_findCharFirstPos(const char* search, char toFind);

extern void UTIL_appendTo(char* dest, const char* add, int appendStartLoc);

extern void appendTo(char* dest, const char* add, int* refIndex);
extern void appendToAndTerminate(char* dest, const char* add, int* refIndex);
extern void appendTo(char* dest, const char* add, int* refIndex, char endCharacter);
extern void appendToAndTerminate(char* dest, const char* add, int* refIndex, char endCharacter);
extern void appendTo(char* dest, const int numb, int* refIndex);
extern void appendToAndTerminate(char* dest, const int numb, int* refIndex);
extern void appendTo(char* dest, const int numb, int* refIndex, char endCharacter);
extern void appendToAndTerminate(char* dest, const int numb, int* refIndex, char endCharacter);
extern void appendTo(char* dest, const char add, int* refIndex);
extern void appendToAndTerminate(char* dest, const char add, int* refIndex);
extern void strncpyTerminate(char* dest, const char* send, int arg_length);


extern void copyString(const char* src, char* dest);
extern void copyString(const char* src, char* dest, int size);

extern void UTIL_appendToEnd(char* dest, const char* add);


extern int lengthOfString(const char* src);
extern int lengthOfString(const char* src, int storeSize);
extern BOOL isStringEmpty(const char* arg_src);
extern BOOL stringEndsWith(const char* arg_src, const char* arg_endsWith);
extern BOOL stringEndsWithIgnoreCase(const char* arg_src, const char* arg_endsWith);







extern BOOL checkSubFileExistence(const char* arg_subdir);





extern void determineHiddenMemPath(void);
extern BOOL checkValveSubFileExistence(const char* arg_subdir);

extern BOOL checkSubDirectoryExistence(const char* arg_subdir);
extern void loadHiddenCVars(void);
extern void saveHiddenCVars(void);



extern void convertIntToBinary(char* buffer, unsigned int arg, unsigned int binaryDigits);


// Common method for determing whether the current game is multiplayer or not.
// Behaves like 'bIsMultiplayer' (cl_dll/hl/hl_weapons.cpp) if clientside, and
// behaves like 'IsMultiplayer' of gamerules for serverside.
extern BOOL IsMultiplayer(void);


extern void UTIL_Sparks(const Vector& position);
//MODDD - additional versions.
//extern void			UTIL_Sparks( const Vector &position, int arg_ballsToSpawn );
//extern void UTIL_Sparks2( const Vector &position );
//extern void UTIL_Sparks2( const Vector &position, int arg_ballsToSpawn );
extern void UTIL_Sparks2(const Vector& position, int arg_ballsToSpawn, float arg_extraSparkMulti);




/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //END OF UTIL_SHARED_H