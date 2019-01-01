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
#include	"extdll.h"
#include	"util.h"
#include	"cbase.h"
#include	"saverestore.h"
#include	"client.h"
#include	"decals.h"
#include	"gamerules.h"
#include	"game.h"

void EntvarsKeyvalue( entvars_t *pev, KeyValueData *pkvd );

extern "C" void PM_Move ( struct playermove_s *ppmove, int server );
extern "C" void PM_Init ( struct playermove_s *ppmove  );
extern "C" char PM_FindTextureType( char *name );

extern Vector VecBModelOrigin( entvars_t* pevBModel );
extern DLL_GLOBAL Vector		g_vecAttackDir;
extern DLL_GLOBAL int			g_iSkillLevel;

//MODDD 
extern float global_cl_explosion;
extern short g_sGaussBallSprite;
extern float global_soundSentenceSave;

EASY_CVAR_EXTERN(weaponPickupPlaysAnyReloadSounds);


static DLL_FUNCTIONS gFunctionTable = 
{
	GameDLLInit,				//pfnGameInit
	DispatchSpawn,				//pfnSpawn
	DispatchThink,				//pfnThink
	DispatchUse,				//pfnUse
	DispatchTouch,				//pfnTouch
	DispatchBlocked,			//pfnBlocked
	DispatchKeyValue,			//pfnKeyValue
	DispatchSave,				//pfnSave
	DispatchRestore,			//pfnRestore
	DispatchObjectCollsionBox,	//pfnAbsBox

	SaveWriteFields,			//pfnSaveWriteFields
	SaveReadFields,				//pfnSaveReadFields

	SaveGlobalState,			//pfnSaveGlobalState
	RestoreGlobalState,			//pfnRestoreGlobalState
	ResetGlobalState,			//pfnResetGlobalState

	ClientConnect,				//pfnClientConnect
	ClientDisconnect,			//pfnClientDisconnect
	ClientKill,					//pfnClientKill
	ClientPutInServer,			//pfnClientPutInServer
	ClientCommand,				//pfnClientCommand
	ClientUserInfoChanged,		//pfnClientUserInfoChanged
	ServerActivate,				//pfnServerActivate
	ServerDeactivate,			//pfnServerDeactivate

	PlayerPreThink,				//pfnPlayerPreThink
	PlayerPostThink,			//pfnPlayerPostThink

	StartFrame,					//pfnStartFrame
	ParmsNewLevel,				//pfnParmsNewLevel
	ParmsChangeLevel,			//pfnParmsChangeLevel

	GetGameDescription,         //pfnGetGameDescription    Returns string describing current .dll game.
	PlayerCustomization,        //pfnPlayerCustomization   Notifies .dll of new customization for player.

	SpectatorConnect,			//pfnSpectatorConnect      Called when spectator joins server
	SpectatorDisconnect,        //pfnSpectatorDisconnect   Called when spectator leaves the server
	SpectatorThink,				//pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)

	Sys_Error,					//pfnSys_Error				Called when engine has encountered an error

	PM_Move,					//pfnPM_Move
	PM_Init,					//pfnPM_Init				Server version of player movement initialization
	PM_FindTextureType,			//pfnPM_FindTextureType
	
	SetupVisibility,			//pfnSetupVisibility        Set up PVS and PAS for networking for this client
	UpdateClientData,			//pfnUpdateClientData       Set up data sent only to specific client
	AddToFullPack,				//pfnAddToFullPack
	CreateBaseline,				//pfnCreateBaseline			Tweak entity baseline for network encoding, allows setup of player baselines, too.
	RegisterEncoders,			//pfnRegisterEncoders		Callbacks for network encoding
	GetWeaponData,				//pfnGetWeaponData
	CmdStart,					//pfnCmdStart
	CmdEnd,						//pfnCmdEnd
	ConnectionlessPacket,		//pfnConnectionlessPacket
	GetHullBounds,				//pfnGetHullBounds
	CreateInstancedBaselines,   //pfnCreateInstancedBaselines
	InconsistentFile,			//pfnInconsistentFile
	AllowLagCompensation,		//pfnAllowLagCompensation
};

static void SetObjectCollisionBox( entvars_t *pev );

#ifndef _WIN32
extern "C" {
#endif
int GetEntityAPI( DLL_FUNCTIONS *pFunctionTable, int interfaceVersion )
{
	if ( !pFunctionTable || interfaceVersion != INTERFACE_VERSION )
	{
		return FALSE;
	}
	
	memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );
	return TRUE;
}

int GetEntityAPI2( DLL_FUNCTIONS *pFunctionTable, int *interfaceVersion )
{
	if ( !pFunctionTable || *interfaceVersion != INTERFACE_VERSION )
	{
		// Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return FALSE;
	}
	
	memcpy( pFunctionTable, &gFunctionTable, sizeof( DLL_FUNCTIONS ) );
	return TRUE;
}

#ifndef _WIN32
}
#endif


int DispatchSpawn( edict_t *pent )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if (pEntity)
	{
		// Initialize these or entities who don't link to the world won't have anything in here
		pEntity->pev->absmin = pEntity->pev->origin - Vector(1,1,1);
		pEntity->pev->absmax = pEntity->pev->origin + Vector(1,1,1);
		//easyPrintLine("SOME stuff SPAWNED %d", pEntity->pev->spawnflags);
		pEntity->Spawn();

		// Try to get the pointer again, in case the spawn function deleted the entity.
		// UNDONE: Spawn() should really return a code to ask that the entity be deleted, but
		// that would touch too much code for me to do that right now.
		pEntity = (CBaseEntity *)GET_PRIVATE(pent);

		if ( pEntity )
		{
			if ( g_pGameRules && !g_pGameRules->IsAllowedToSpawn( pEntity ) )
				return -1;	// return that this entity should be deleted
			if ( pEntity->pev->flags & FL_KILLME )
				return -1;
		}


		// Handle global stuff here
		if ( pEntity && pEntity->pev->globalname ) 
		{
			const globalentity_t *pGlobal = gGlobalState.EntityFromTable( pEntity->pev->globalname );
			if ( pGlobal )
			{
				// Already dead? delete
				if ( pGlobal->state == GLOBAL_DEAD )
					return -1;
				else if ( !FStrEq( STRING(gpGlobals->mapname), pGlobal->levelName ) )
					pEntity->MakeDormant();	// Hasn't been moved to this level yet, wait but stay alive
				// In this level & not dead, continue on as normal
			}
			else
			{
				// Spawned entities default to 'On'
				gGlobalState.EntityAdd( pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON );
//				ALERT( at_console, "Added global entity %s (%s)\n", STRING(pEntity->pev->classname), STRING(pEntity->pev->globalname) );
			}
		}

	}

	return 0;
}

void DispatchKeyValue( edict_t *pentKeyvalue, KeyValueData *pkvd )
{
	if ( !pkvd || !pentKeyvalue )
		return;

	EntvarsKeyvalue( VARS(pentKeyvalue), pkvd );

	// If the key was an entity variable, or there's no class set yet, don't look for the object, it may
	// not exist yet.
	if ( pkvd->fHandled || pkvd->szClassName == NULL )
		return;

	// Get the actualy entity object
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentKeyvalue);

	if ( !pEntity )
		return;

	pEntity->KeyValue( pkvd );
}


// HACKHACK -- this is a hack to keep the node graph entity from "touching" things (like triggers)
// while it builds the graph
BOOL gTouchDisabled = FALSE;
void DispatchTouch( edict_t *pentTouched, edict_t *pentOther )
{
	if ( gTouchDisabled )
		return;

	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentTouched);
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE( pentOther );

	if ( pEntity && pOther && ! ((pEntity->pev->flags | pOther->pev->flags) & FL_KILLME) )
		pEntity->Touch( pOther );
}


void DispatchUse( edict_t *pentUsed, edict_t *pentOther )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pentUsed);
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE(pentOther);

	if (pEntity && !(pEntity->pev->flags & FL_KILLME) )
		pEntity->Use( pOther, pOther, USE_TOGGLE, 0 );
}

void DispatchThink( edict_t *pent )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);
	if (pEntity)
	{
		if ( FBitSet( pEntity->pev->flags, FL_DORMANT ) )
			ALERT( at_error, "Dormant entity %s is thinking!!\n", STRING(pEntity->pev->classname) );
				
		pEntity->Think();
	}
}

void DispatchBlocked( edict_t *pentBlocked, edict_t *pentOther )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE( pentBlocked );
	CBaseEntity *pOther = (CBaseEntity *)GET_PRIVATE( pentOther );

	if (pEntity)
		pEntity->Blocked( pOther );
}

void DispatchSave( edict_t *pent, SAVERESTOREDATA *pSaveData )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);
	
	if ( pEntity && pSaveData )
	{
		ENTITYTABLE *pTable = &pSaveData->pTable[ pSaveData->currentIndex ];

		if ( pTable->pent != pent )
			ALERT( at_error, "ENTITY TABLE OR INDEX IS WRONG!!!!\n" );

		if ( pEntity->ObjectCaps() & FCAP_DONT_SAVE )
			return;

		// These don't use ltime & nextthink as times really, but we'll fudge around it.
		if ( pEntity->pev->movetype == MOVETYPE_PUSH )
		{
			float delta = pEntity->pev->nextthink - pEntity->pev->ltime;
			pEntity->pev->ltime = gpGlobals->time;
			pEntity->pev->nextthink = pEntity->pev->ltime + delta;
		}

		pTable->location = pSaveData->size;		// Remember entity position for file I/O
		pTable->classname = pEntity->pev->classname;	// Remember entity class for respawn

		CSave saveHelper( pSaveData );
		pEntity->Save( saveHelper );

		pTable->size = pSaveData->size - pTable->location;	// Size of entity block is data size written to block
	}
}


// Find the matching global entity.  Spit out an error if the designer made entities of
// different classes with the same global name
CBaseEntity *FindGlobalEntity( string_t classname, string_t globalname )
{
	edict_t *pent = FIND_ENTITY_BY_STRING( NULL, "globalname", STRING(globalname) );
	CBaseEntity *pReturn = CBaseEntity::Instance( pent );
	if ( pReturn )
	{
		if ( !FClassnameIs( pReturn->pev, STRING(classname) ) )
		{
			ALERT( at_console, "Global entity found %s, wrong class %s\n", STRING(globalname), STRING(pReturn->pev->classname) );
			pReturn = NULL;
		}
	}

	return pReturn;
}


int DispatchRestore( edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity )
{
	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);

	if ( pEntity && pSaveData )
	{
		entvars_t tmpVars;
		Vector oldOffset;

		CRestore restoreHelper( pSaveData );
		if ( globalEntity )
		{
			CRestore tmpRestore( pSaveData );
			tmpRestore.PrecacheMode( 0 );
			tmpRestore.ReadEntVars( "ENTVARS", &tmpVars );

			// HACKHACK - reset the save pointers, we're going to restore for real this time
			pSaveData->size = pSaveData->pTable[pSaveData->currentIndex].location;
			pSaveData->pCurrentData = pSaveData->pBaseData + pSaveData->size;
			// -------------------


			const globalentity_t *pGlobal = gGlobalState.EntityFromTable( tmpVars.globalname );
			
			// Don't overlay any instance of the global that isn't the latest
			// pSaveData->szCurrentMapName is the level this entity is coming from
			// pGlobla->levelName is the last level the global entity was active in.
			// If they aren't the same, then this global update is out of date.
			if ( !FStrEq( pSaveData->szCurrentMapName, pGlobal->levelName ) )
				return 0;

			// Compute the new global offset
			oldOffset = pSaveData->vecLandmarkOffset;
			CBaseEntity *pNewEntity = FindGlobalEntity( tmpVars.classname, tmpVars.globalname );
			if ( pNewEntity )
			{
//				ALERT( at_console, "Overlay %s with %s\n", STRING(pNewEntity->pev->classname), STRING(tmpVars.classname) );
				// Tell the restore code we're overlaying a global entity from another level
				restoreHelper.SetGlobalMode( 1 );	// Don't overwrite global fields
				pSaveData->vecLandmarkOffset = (pSaveData->vecLandmarkOffset - pNewEntity->pev->mins) + tmpVars.mins;
				pEntity = pNewEntity;// we're going to restore this data OVER the old entity
				pent = ENT( pEntity->pev );
				// Update the global table to say that the global definition of this entity should come from this level
				gGlobalState.EntityUpdate( pEntity->pev->globalname, gpGlobals->mapname );
			}
			else
			{
				// This entity will be freed automatically by the engine.  If we don't do a restore on a matching entity (below)
				// or call EntityUpdate() to move it to this level, we haven't changed global state at all.
				return 0;
			}

		}

		if ( pEntity->ObjectCaps() & FCAP_MUST_SPAWN )
		{
			pEntity->Restore( restoreHelper );
			pEntity->Spawn();
		}
		else
		{
			pEntity->Restore( restoreHelper );
			pEntity->Precache( );
		}

		// Again, could be deleted, get the pointer again.
		pEntity = (CBaseEntity *)GET_PRIVATE(pent);

#if 0
		if ( pEntity && pEntity->pev->globalname && globalEntity ) 
		{
			ALERT( at_console, "Global %s is %s\n", STRING(pEntity->pev->globalname), STRING(pEntity->pev->model) );
		}
#endif

		// Is this an overriding global entity (coming over the transition), or one restoring in a level
		if ( globalEntity )
		{
//			ALERT( at_console, "After: %f %f %f %s\n", pEntity->pev->origin.x, pEntity->pev->origin.y, pEntity->pev->origin.z, STRING(pEntity->pev->model) );
			pSaveData->vecLandmarkOffset = oldOffset;
			if ( pEntity )
			{
				UTIL_SetOrigin( pEntity->pev, pEntity->pev->origin );
				pEntity->OverrideReset();
			}
		}
		else if ( pEntity && pEntity->pev->globalname ) 
		{
			const globalentity_t *pGlobal = gGlobalState.EntityFromTable( pEntity->pev->globalname );
			if ( pGlobal )
			{
				// Already dead? delete
				if ( pGlobal->state == GLOBAL_DEAD )
					return -1;
				else if ( !FStrEq( STRING(gpGlobals->mapname), pGlobal->levelName ) )
				{
					pEntity->MakeDormant();	// Hasn't been moved to this level yet, wait but stay alive
				}
				// In this level & not dead, continue on as normal
			}
			else
			{
				ALERT( at_error, "Global Entity %s (%s) not in table!!!\n", STRING(pEntity->pev->globalname), STRING(pEntity->pev->classname) );
				// Spawned entities default to 'On'
				gGlobalState.EntityAdd( pEntity->pev->globalname, gpGlobals->mapname, GLOBAL_ON );
			}
		}
	}
	return 0;
}


void DispatchObjectCollsionBox( edict_t *pent )
{

	CBaseEntity *pEntity = (CBaseEntity *)GET_PRIVATE(pent);
	if (pEntity)
	{
		pEntity->SetObjectCollisionBox();
	}
	else
		SetObjectCollisionBox( &pent->v );
}


void SaveWriteFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	CSave saveHelper( pSaveData );
	saveHelper.WriteFields( pname, pBaseData, pFields, fieldCount );
}


void SaveReadFields( SAVERESTOREDATA *pSaveData, const char *pname, void *pBaseData, TYPEDESCRIPTION *pFields, int fieldCount )
{
	CRestore restoreHelper( pSaveData );
	restoreHelper.ReadFields( pname, pBaseData, pFields, fieldCount );
}


edict_t * EHANDLE::Get( void ) 
{ 
	if (m_pent)
	{
		if (m_pent->serialnumber == m_serialnumber) 
			return m_pent; 
		else
			return NULL;
	}
	return NULL; 
};


edict_t * EHANDLE::Set( edict_t *pent ) 
{ 
	m_pent = pent;  
	if (pent) 
		m_serialnumber = m_pent->serialnumber; 
	return pent; 
};


EHANDLE :: operator CBaseEntity *() 
{ 
	return (CBaseEntity *)GET_PRIVATE( Get( ) ); 
};


CBaseEntity * EHANDLE :: operator = (CBaseEntity *pEntity)
{
	if (pEntity)
	{
		m_pent = ENT( pEntity->pev );
		if (m_pent)
			m_serialnumber = m_pent->serialnumber;
	}
	else
	{
		m_pent = NULL;
		m_serialnumber = 0;
	}
	return pEntity;
}

EHANDLE :: operator int ()
{
	return Get() != NULL;
}

CBaseEntity * EHANDLE :: operator -> ()
{
	return (CBaseEntity *)GET_PRIVATE( Get( ) ); 
}

//MODDD - why isn't getting the private data (CBaseEntity pointer) directly in a method call an option?
//        Basically what the "->" operator does but returns the CBaseEntity pointer itself instead of using it
//        to get at the CBaseEntity's own methods.
//        Could... this suppport templates?  like <SomeType T> or however that works?  Whatever, static casting on
//        the user's end works okay.
CBaseEntity* EHANDLE::GetEntity(){
	return (CBaseEntity *)GET_PRIVATE( Get( ) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////










CBaseEntity::CBaseEntity(void){

	usingCustomSequence = FALSE;
	wasAttached = FALSE;
	alreadySaved = FALSE;
	crazyPrintout = FALSE;

	//assume not spawned dynamically (by "give" commands in-game, not at a map / boundary's first load)
	spawnedDynamically = FALSE;


	//THIS IS LOOSELY IMPLEMENTED. It is up to somewhere else calling spawn() on this, where it expects it not to be the first spawn call,
	//to turn this off to FALSE before calling. This isn't to be saved.  Could be if absolutely necessary though.
	firstSpawnCall = TRUE;

	//barnacleVictimException = FALSE; ???

	
	recognizablyDead = FALSE; //golly gee, doubt it.

	firstTimeKilled = TRUE;

}

//OLD: soundSentenceSavePreference
//Whether this entity uses the sound sentence save feature. By default, false.
BOOL CBaseEntity::usesSoundSentenceSave(void){
	return FALSE;
}



//Is this monster a flyer for the purposes of pathfinding or what nodes to use?
//Flyers use a different type of nodes than the typical snapped-to-ground ones.
//Flyers are things that defy gravity and can move anywhere on the Z axis really.  Not just anything that can jump.
//Don't worry about swimming (pev->flags having FL_SWIM I think).  Swimmers are just flyers with a different coat of paint
//and already know to use their own in-water nodes.
BOOL CBaseEntity::isMovetypeFlying(void) const{
	return (pev->movetype == MOVETYPE_FLY || pev->movetype == MOVETYPE_BOUNCEMISSILE);
	//Count MOVETYPE_BOUNCEMISSILE too. It may be used sometimes?
	//exclude TOSS for now, it obeys gravity.
	//MOVETYPE_TOSS
}

BOOL CBaseEntity::isSizeGiant(void){
	//things larger than usual, like gargantuas, apaches, can just say they are.
	//Things expecting roughly human-sized things like barnacles should know to skip these.
	return FALSE;
}
BOOL CBaseEntity::isOrganic(void){
	//entities must individually say they are to count for some things like having edible corpses.
	return FALSE;
}

//Override me if specifying a different hull is necessary.
//For things that don't fit evenly into certain sizes as seen in nodes.cpp's HullIndex method,
//a NODE_HUMAN_HULL may be implied. Even something larger than the largest or smaller than the smallest gets this
//assumption. It is not very good, so just say what an entity prefers here.
int CBaseEntity::getHullIndexForNodes(void) const{
	return NODE_DEFAULT_HULL;
}//END OF getHullIndexForNodes




//MODDD - when an entity is forcibly deleted (currently, only by the player doing entRemove), run this method to see if anything needs to be done right before cleanup (like stopping a looping sound).
void CBaseEntity::onDelete(void){

}



const char* CBaseEntity::getClassname(void){
	const char* test = STRING(pev->classname);
	if(test == NULL){
		return "";
	}else{
		return test;
	}
}
const char* CBaseEntity::getClassnameShort(void){
	char charBuffer[127];
	const char* test = STRING(pev->classname);
	if(test == NULL){
		return "";
	}else{
		//shorten it. If it starts with monster_, remove that.

		if(stringStartsWith(test, "monster_")){
			//cut!
			//::strcpy(&charBuffer[0], test);
			UTIL_substring(&charBuffer[0], test, 8, -1);


			// This part:
			//   STRING(MAKE_STRING( ...  ))
			// may look weird, but it effectively clones the char array to be treated as a single value and sent back, unlike localvar "charBuffer" which loses scope as this method ends.
			// It's just a neat tool.

			return STRING(MAKE_STRING(charBuffer));
		}else{
			//unsafe to assume any part can be cut.
			return test;
		}
	}
}



//MODDD - mostly for testing, really.
void CBaseEntity::Spawn(void){
	//easyPrintLine("WWWWWWW %s", STRING(pev->classname) );
	return;
}

//MODDD
//spawned by user, but without any flags in particular.
void CBaseEntity::DefaultSpawnNotice(void){
	
	//still spawned in-game.
	spawnedDynamically = TRUE;
	//flag not forced though.
	flagForced = FALSE;

}
//spawned by user, force the given flag.
void CBaseEntity::ForceSpawnFlag(int arg_spawnFlag){
	//by default, not doing much (too broad here).  Just update the spawn flag as intended.
	pev->spawnflags = arg_spawnFlag;
	
	//forcedSpawnFlag = arg_spawnFlag;
	spawnedDynamically = TRUE;
	flagForced = TRUE;
}






//MODDDMIRROR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//LRC
// PUSH entities won't have their velocity applied unless they're thinking.
// make them do so for the foreseeable future.
void CBaseEntity :: SetEternalThink( void )
{
	if (pev->movetype == MOVETYPE_PUSH)
	{
		// record m_fPevNextThink as well, because we want to be able to
		// tell when the bloody engine CHANGES IT!
//		pev->nextthink = 1E9;
		pev->nextthink = pev->ltime + 1E6;
		m_fPevNextThink = pev->nextthink;
	}

	CBaseEntity *pChild;
	for (pChild = m_pChildMoveWith; pChild != NULL; pChild = pChild->m_pSiblingMoveWith)
		pChild->SetEternalThink( );
}




//LRC - for getting round the engine's preconceptions.
// MoveWith entities have to be able to think independently of moving.
// This is how we do so.
void CBaseEntity :: SetNextThink( float delay, BOOL correctSpeed )
{
	// now monsters use this method, too.
	if (m_pMoveWith || m_pChildMoveWith || pev->flags & FL_MONSTER)
	{
		// use the Assist system, so that thinking doesn't mess up movement.
		if (pev->movetype == MOVETYPE_PUSH)
			m_fNextThink = pev->ltime + delay;
		else
			m_fNextThink = gpGlobals->time + delay;
		SetEternalThink( );
		//UTIL_MarkForAssist( this, correctSpeed ); ???

//		ALERT(at_console, "SetAssistedThink for %s: %f\n", STRING(pev->targetname), m_fNextThink);
	}
	else
	{
		// set nextthink as normal.
		if (pev->movetype == MOVETYPE_PUSH)
		{
			pev->nextthink = pev->ltime + delay;
		}
		else
		{
			pev->nextthink = gpGlobals->time + delay;
		}

		m_fPevNextThink = m_fNextThink = pev->nextthink;

//		if (pev->classname) ALERT(at_console, "SetNormThink for %s: %f\n", STRING(pev->targetname), m_fNextThink);
	}
}



//LRC
void CBaseEntity :: AbsoluteNextThink( float time, BOOL correctSpeed )
{
	if (m_pMoveWith || m_pChildMoveWith)
	{
		// use the Assist system, so that thinking doesn't mess up movement.
		m_fNextThink = time;
		SetEternalThink( );
		
		//UTIL_MarkForAssist( this, correctSpeed ); ???
	}
	else
	{
		// set nextthink as normal.
		pev->nextthink = time;
		m_fPevNextThink = m_fNextThink = pev->nextthink;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//MODDD notice - CBaseEntity's TakeHealth, TakeDamage and Killed method implementations have been moved to combat.cpp for consistency.
//               CBaseMonster has all three of those methods over there, and both CBaseMonster and CBaseEntity already had their 
//               TraceAttack's over there. More fitting to keep all of these methods in combat.cpp.



CBaseEntity *CBaseEntity::GetNextTarget( void )
{
	if ( FStringNull( pev->target ) )
		return NULL;
	edict_t *pTarget = FIND_ENTITY_BY_TARGETNAME ( NULL, STRING(pev->target) );
	if ( FNullEnt(pTarget) )
		return NULL;

	return Instance( pTarget );
}

// Global Savedata for Delay
//MODDD - uh, this was found to be for CBaseEntity?  Oh well.
TYPEDESCRIPTION	CBaseEntity::m_SaveData[] = 
{
	DEFINE_FIELD( CBaseEntity, m_pGoalEnt, FIELD_CLASSPTR ),

	DEFINE_FIELD( CBaseEntity, m_pfnThink, FIELD_FUNCTION ),		// UNDONE: Build table of these!!!
	DEFINE_FIELD( CBaseEntity, m_pfnTouch, FIELD_FUNCTION ),
	DEFINE_FIELD( CBaseEntity, m_pfnUse, FIELD_FUNCTION ),
	DEFINE_FIELD( CBaseEntity, m_pfnBlocked, FIELD_FUNCTION ),
	
	//MODDD
	DEFINE_FIELD( CBaseEntity, recognizablyDead, FIELD_BOOLEAN ),

	//
};



void CBaseEntity::playAmmoPickupSound(){
	playAmmoPickupSound(NULL);

}

void CBaseEntity::playAmmoPickupSound(entvars_t* sentPev){

	entvars_t* pevToUse = NULL;

	if(sentPev != NULL){
		pevToUse = sentPev;
	}else{
		pevToUse = pev;
	}


	if(EASY_CVAR_GET(weaponPickupPlaysAnyReloadSounds) != 1){
		//normal.
		EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);

	}else{

		switch(g_engfuncs.pfnRandomLong(0,2)){
		case 0:
			EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "weapons/reload1.wav", 1, ATTN_NORM);
		break;
		case 1:
			EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "weapons/reload2.wav", 1, ATTN_NORM);
		break;
		case 2:
			EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "weapons/reload3.wav", 1, ATTN_NORM);
		break;
		}

	}

}

void CBaseEntity::playGunPickupSound(){
	playGunPickupSound(NULL);
}

void CBaseEntity::playGunPickupSound(entvars_t* sentPev){
	
	entvars_t* pevToUse = NULL;

	if(sentPev != NULL){
		pevToUse = sentPev;
	}else{
		pevToUse = pev;
	}

	switch(RANDOM_LONG(0, 3)){
	case 0:
		EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "items/gunpickup1.wav", 1, ATTN_NORM);
	break;
	case 1:
		EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM);
	break;
	case 2:
		EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "items/gunpickup3.wav", 1, ATTN_NORM);
	break;
	case 3:
		EMIT_SOUND(ENT(pevToUse), CHAN_ITEM, "items/gunpickup4.wav", 1, ATTN_NORM);
	break;
	}

}




void CBaseEntity::precacheAmmoPickupSound(){

	if(EASY_CVAR_GET(weaponPickupPlaysAnyReloadSounds) != 1){
		//normal.
		PRECACHE_SOUND("items/9mmclip1.wav", TRUE);
	}else{
		PRECACHE_SOUND("weapons/reload1.wav", TRUE);
		PRECACHE_SOUND("weapons/reload2.wav", TRUE);
		PRECACHE_SOUND("weapons/reload3.wav", TRUE);
	}
}

void CBaseEntity::precacheGunPickupSound(){

	PRECACHE_SOUND("items/gunpickup1.wav", TRUE);
	PRECACHE_SOUND("items/gunpickup2.wav", TRUE);
	PRECACHE_SOUND("items/gunpickup3.wav", TRUE);
	PRECACHE_SOUND("items/gunpickup4.wav", TRUE);
	
}




int CBaseEntity::Save( CSave &save )
{
	if ( save.WriteEntVars( "ENTVARS", pev ) )
		return save.WriteFields( "BASE", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	return 0;
}

int CBaseEntity::Restore( CRestore &restore )
{
	int status;

	status = restore.ReadEntVars( "ENTVARS", pev );
	if ( status )
		status = restore.ReadFields( "BASE", this, m_SaveData, ARRAYSIZE(m_SaveData) );

	//if( !strcmp(STRING(pev->classname), "monster_scientist")){
	//	easyForcePrintLine("WHAAAAUT");
	//}

    if ( pev->modelindex != 0 && !FStringNull(pev->model) )
	{
		Vector mins, maxs;
		mins = pev->mins;	// Set model is about to destroy these
		maxs = pev->maxs;


		PRECACHE_MODEL( (char *)STRING(pev->model) );
		//MODDD - call this new instance method instead, "setModel", which is overridable so that the entity having its model changed can do something that may depend on that.
		//SET_MODEL(ENT(pev), STRING(pev->model));

		//BEWARE OF USING "setModel" TO RELY ON A MONSTER'S OWN CUSTOM SAVED VARIABLES. They won't be loaded in time for this call!
		//Try using them at the end of the monster's Restore method instead, before returning its load call.
		setModel(STRING(pev->model));
		UTIL_SetSize(pev, mins, maxs);	// Reset them
	}

	return status;
}

//Meant to be implemented by classes that need something to happen after loading their parent's information from a saved game.
void CBaseEntity::PostRestore(){
	//easyForcePrintLine("PostRestore: %s", this->getClassname());
}




//Ensure this is always precached.  Player sound so it is, and does not need the soundSentenceSave system.
void CBaseEntity::playMetallicHitSound(int arg_channel, float arg_volume){
	//default to CHAN_ITEM or CHAN_WEAPON if unspecified?  Most enemies use CHAN_WEAPON so probably that. CHAN_ITEM may have just been for coming from the player crowbar.
	switch( RANDOM_LONG(0,1) ){
	case 0:
		EMIT_SOUND_DYN(ENT(pev), arg_channel, "weapons/cbar_hit1.wav", arg_volume, ATTN_NORM, 0, 103 + RANDOM_LONG(0,3)); 
		break;
	case 1:
		EMIT_SOUND_DYN(ENT(pev), arg_channel, "weapons/cbar_hit2.wav", arg_volume, ATTN_NORM, 0, 103 + RANDOM_LONG(0,3)); 
		break;
	}
}//END OF playMetallicHitSound




BOOL CBaseEntity::IsWorld(){
	return FALSE;  //uhhh.... I don't think I'm the world.
}
BOOL CBaseEntity::IsWorldAffiliated(){
	//This is only for entities placed at map creation that help make the map more interactive, like func_brekable, func_button or func_wall (ladders).
	return FALSE;  
}
BOOL CBaseEntity::IsWorldOrAffiliated(){
	//Just a shortcut for being either the world or affiliated with it.
	return (IsWorld() || IsWorldAffiliated());
}


BOOL CBaseEntity::IsBreakable(){
	//This isn't a general adjective. It is for func_breakable's that are able to take damage.
	//This method was already present in func_breakble, but not all entities, making it a little inconvenient.
	return FALSE;
}


// Initialize absmin & absmax to the appropriate box
void SetObjectCollisionBox( entvars_t *pev )
{
	if ( (pev->solid == SOLID_BSP) && 
		 (pev->angles.x || pev->angles.y|| pev->angles.z) )
	{	// expand for rotation
		float		max, v;
		int			i;

		max = 0;
		for (i=0 ; i<3 ; i++)
		{
			v = fabs( ((float *)pev->mins)[i]);
			if (v > max)
				max = v;
			v = fabs( ((float *)pev->maxs)[i]);
			if (v > max)
				max = v;
		}
		for (i=0 ; i<3 ; i++)
		{
			((float *)pev->absmin)[i] = ((float *)pev->origin)[i] - max;
			((float *)pev->absmax)[i] = ((float *)pev->origin)[i] + max;
		}
	}
	else
	{
		pev->absmin = pev->origin + pev->mins;
		pev->absmax = pev->origin + pev->maxs;
	}

	pev->absmin.x -= 1;
	pev->absmin.y -= 1;
	pev->absmin.z -= 1;
	pev->absmax.x += 1;
	pev->absmax.y += 1;
	pev->absmax.z += 1;
}


BOOL CBaseEntity::getIsBarnacleVictimException(void){
	return FALSE;
}


//MODDD
//just return "True" if this is of class "CBreakable" or a subclass of it.  (also known as func_breakable)
BOOL CBaseEntity::isBreakableOrchild(void){
    return FALSE;
}
//MODDD
//Return whether or not this is considered a non-living object that can be destroyed.
//(namely, func_breakable & pushable, including their conditions for breaking.  Some spawnflags prevent breaking)
//~anything NOT of class CBreakable (func_breakable) should just return FALSE (default here)
BOOL CBaseEntity::isDestructibleInanimate(void){
	return FALSE;
}


//MODDD
//Is this (monster) a child of the TalkMonster class?
BOOL CBaseEntity::isTalkMonster(void){
	return FALSE;
}

//MODDD
//Is this a neutral creature that can be "pissed off" and become hostile?
BOOL CBaseEntity::isProvokable(void){
	return FALSE;
}
//MODDD
//Is this creature that IS provokable now able to harm us?  (By default, assume hostiles are hostile, so TRUE)
BOOL CBaseEntity::isProvoked(void){
	return TRUE;
}






void CBaseEntity::SetObjectCollisionBox( void )
{
	::SetObjectCollisionBox( pev );
}

void CBaseEntity::setModel(void){
	CBaseEntity::setModel(NULL);
}
void CBaseEntity::setModel(const char* m){
	SET_MODEL(ENT(pev), m);
}


BOOL CBaseEntity::isForceHated(CBaseEntity *pBy){
	return (forcedRelationshipWith(pBy) > R_NO );
	//return FALSE;
}
		
int CBaseEntity::forcedRelationshipWith(CBaseEntity *pWith){
	//the default, R_DEFAULT, is a signal to just go with the table instead. Otherwise, the called on monster may intervene in the decision.
	return R_DEFAULT;
}


//Server only.
#ifndef CLIENT_DLL
SCHEDULE_TYPE CBaseEntity::getHeardBaitSoundSchedule(CSound* pSound){
	return SCHED_NONE;
}
SCHEDULE_TYPE CBaseEntity::_getHeardBaitSoundSchedule(CSound* pSound){
	return SCHED_NONE;
}
SCHEDULE_TYPE CBaseEntity::getHeardBaitSoundSchedule(){
	return SCHED_NONE;
}
#endif



int	CBaseEntity :: Intersects( CBaseEntity *pOther )
{
	if ( pOther->pev->absmin.x > pev->absmax.x ||
		 pOther->pev->absmin.y > pev->absmax.y ||
		 pOther->pev->absmin.z > pev->absmax.z ||
		 pOther->pev->absmax.x < pev->absmin.x ||
		 pOther->pev->absmax.y < pev->absmin.y ||
		 pOther->pev->absmax.z < pev->absmin.z )
		 return 0;
	return 1;
}

void CBaseEntity :: MakeDormant( void )
{
	SetBits( pev->flags, FL_DORMANT );
	
	// Don't touch
	pev->solid = SOLID_NOT;
	// Don't move
	pev->movetype = MOVETYPE_NONE;
	// Don't draw
	SetBits( pev->effects, EF_NODRAW );
	// Don't think
	pev->nextthink = 0;
	// Relink
	UTIL_SetOrigin( pev, pev->origin );
}

int CBaseEntity :: IsDormant( void )
{
	return FBitSet( pev->flags, FL_DORMANT );
}

BOOL CBaseEntity :: IsInWorld( void )
{
	// position 
	if (pev->origin.x >= 4096) return FALSE;
	if (pev->origin.y >= 4096) return FALSE;
	if (pev->origin.z >= 4096) return FALSE;
	if (pev->origin.x <= -4096) return FALSE;
	if (pev->origin.y <= -4096) return FALSE;
	if (pev->origin.z <= -4096) return FALSE;
	// speed
	if (pev->velocity.x >= 2000) return FALSE;
	if (pev->velocity.y >= 2000) return FALSE;
	if (pev->velocity.z >= 2000) return FALSE;
	if (pev->velocity.x <= -2000) return FALSE;
	if (pev->velocity.y <= -2000) return FALSE;
	if (pev->velocity.z <= -2000) return FALSE;

	return TRUE;
}

int CBaseEntity::ShouldToggle( USE_TYPE useType, BOOL currentState )
{
	if ( useType != USE_TOGGLE && useType != USE_SET )
	{
		if ( (currentState && useType == USE_ON) || (!currentState && useType == USE_OFF) )
			return 0;
	}
	return 1;
}


int	CBaseEntity :: DamageDecal( int bitsDamageType)
{
	return CBaseEntity::DamageDecal(bitsDamageType, 0);
}

int	CBaseEntity :: DamageDecal( int bitsDamageType, int bitsDamageTypeMod )
{
	if ( pev->rendermode == kRenderTransAlpha )
		return -1;

	if ( pev->rendermode != kRenderNormal )
		return DECAL_BPROOF1;

	return DECAL_GUNSHOT1 + RANDOM_LONG(0,4);
}




CBaseEntity * CBaseEntity::CreateManual( const char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner ){
	edict_t	*pent;
	CBaseEntity *pEntity;

	pent = CREATE_NAMED_ENTITY( MAKE_STRING( szName ));
	if ( FNullEnt( pent ) )
	{
		ALERT ( at_console, "NULL Ent in Create!\n" );
		return NULL;
	}
	pEntity = Instance( pent );
	pEntity->pev->owner = pentOwner;
	pEntity->pev->origin = vecOrigin;
	pEntity->pev->angles = vecAngles;

	return pEntity;
}

// NOTE: szName must be a pointer to constant memory, e.g. "monster_class" because the entity
// will keep a pointer to it after this call.
//Also, szName is NOW const (constant)
CBaseEntity * CBaseEntity::Create( const char *szName, const Vector &vecOrigin, const Vector &vecAngles, int setSpawnflags, edict_t *pentOwner ){
	CBaseEntity* pEntity = CBaseEntity::CreateManual(szName, vecOrigin, vecAngles, pentOwner);
	if(!pEntity)return NULL;

	pEntity->ForceSpawnFlag(setSpawnflags);

	DispatchSpawn( pEntity->edict() );
	return pEntity;
}
CBaseEntity * CBaseEntity::Create( const char *szName, const Vector &vecOrigin, const Vector &vecAngles, edict_t *pentOwner ){
	//seems to happen from real-time creation, like on crating gibs, not creatures at map load.
	CBaseEntity* pEntity = CBaseEntity::CreateManual(szName, vecOrigin, vecAngles, pentOwner);
	if(!pEntity)return NULL;

	DispatchSpawn( pEntity->edict() );
	return pEntity;
}




//Whether a monster can see monsters through the water line.
//That is, whether submerged monsters can see monsters above the water, and vice versa (whichever this monster is).
//Not whether other monsters can see this one itself past the waterline necessarily.
//Defaults to FALSE like retail. Special cases need TRUE.
BOOL CBaseEntity::SeeThroughWaterLine(void){
	return FALSE;
}//END OF SeeThroughWaterLine






void CBaseEntity::ReportGeneric(){
	//To be determined further by each entity class and its own specific variables. But this general info is ok.
	//Child classes implementing this method should call their parent methods to reach any other parent specifics available.

	easyForcePrintLine("Classname:%s targetname:%s netname:%s", STRING(pev->classname), pev->targetname!=NULL?STRING(pev->targetname):"_NONE_", pev->netname!=NULL?STRING(pev->netname):"_NONE_");
	easyForcePrintLine("nextthink:%.2f ltime:%.2f currenttime:%.2f", pev->nextthink, pev->ltime, gpGlobals->time);

	



	easyForcePrint("Spawnflags: ");
	printLineIntAsBinary((unsigned int)pev->spawnflags, 32u);

	easyForcePrintLine("Sequence:%d Frame:%.2f Framerate:%.2f", pev->sequence, pev->frame, pev->framerate);
	easyForcePrintLine("Flags:%d renderfx:%d rendermode:%d renderamt:%.2f gamestate:%d solid:%d movetype:%d", pev->flags, pev->renderfx, pev->rendermode, pev->renderamt, pev->gamestate, pev->solid, pev->movetype);
	easyForcePrintLine("ThinkACTIVE:%d curtime:%.2f nextthink:%.2f ltime:%.2f", (m_pfnThink!=NULL), gpGlobals->time, pev->nextthink, pev->ltime);


}//END OF ReportGeneric


Vector CBaseEntity::GetAbsVelocity(){
	return pev->velocity;
}



Vector CBaseEntity::GetAbsOrigin(){
	return pev->origin;
}
void CBaseEntity::SetAbsOrigin(const Vector& arg_newOrigin){
	
	//pev->origin = arg_newOrigin;
	//or?
	UTIL_SetOrigin (pev, arg_newOrigin);

}


Vector CBaseEntity::GetAbsAngles(){
	return pev->angles;
}
void CBaseEntity::SetAbsAngles(const Vector& arg_newAngles){
	
	pev->angles = arg_newAngles;
	//or?
	//UTIL_SetAngles (pev, arg_newOrigin);

}


//Does this entity resist the effects of physical attacks?
//By default no.
BOOL CBaseEntity::blocksImpact(void){
	return FALSE;
}//END OF Impact

//How much of an effect this has on other pushable entities.
//Things smaller than average should reduce this number. This larger should increase it.
//Most things won't touch pushables very often.
//But things that would look silly making a pushable go flying (like arrows or hornets) should make this tiny to not look so strange.
float CBaseEntity::massInfluence(void){
	return 0.7f;
}//END OF massInfluence


