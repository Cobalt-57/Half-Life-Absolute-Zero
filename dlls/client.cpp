/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.6
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
// Robin, 4-22-98: Moved set_suicide_frame() here from player.cpp to allow us to 
//				   have one without a hardcoded player.mdl in tf_client.cpp

/*

===== client.cpp ========================================================

  client/server game specific stuff

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "saverestore.h"
#include "player.h"
#include "spectator.h"
#include "client.h"
#include "soundent.h"
#include "gamerules.h"
#include "game.h"
#include "customentity.h"
#include "weapons.h"
#include "weaponinfo.h"
#include "usercmd.h"
#include "netadr.h"

//MODDD
#include "scripted.h"
//MODDD - TESTING! ???
#include "squadmonster.h"
#include "nodes.h"
#include "custom_debug.h"

//MODDD - moved from below to here for neatness.
#include "voice_gamemgr.h"
#include "entity_state.h"

#include "trains.h"

#include "talkmonster.h"




extern DLL_GLOBAL ULONG		g_ulModelIndexPlayer;
extern DLL_GLOBAL BOOL		g_fGameOver;
extern DLL_GLOBAL int		g_iSkillLevel;
extern DLL_GLOBAL ULONG		g_ulFrameCount;

extern void CopyToBodyQue(entvars_t* pev);

extern int g_teamplay;




//No.  For once, something here in client.cpp is to be externed elsewhere instead.
//extern void resetModCVars(CBasePlayer* arg_plyRef, BOOL isEmergency);






float globalPSEUDO_minimumfiredelaymem = -1;
//MODDD
extern float cheat_barnacleEatsEverything;


extern float globalPSEUDO_cameraMode;
extern float globalPSEUDO_aspectratio_determined_fov;


EASY_CVAR_EXTERN_MASS





////////////////////////////////////////////////////////////////////////////////////////////////////////



extern BOOL globalPSEUDO_queueClientSendoff;

BOOL playerCanThink1 = FALSE;
BOOL playerCanThink2 = FALSE;


//MODDD TODO - this should just get copied to methods that need it, may only just be one. Or just put it in each one and make it static? eh.
char queuedSound[127];
BOOL playQueued = FALSE;
float playQueuedTime = -1;




//disabled.
/*
cvar_t* cvar_protoVersion = 0;

BOOL protoVersionMemGiven = FALSE;
char protoVersionMem[255];
*/

//MODDD - cheat CVAR storage.
/*
cvar_t* clientCheat_infiniteclip = 0;
cvar_t* clientCheat_infiniteammo = 0;
cvar_t* clientCheat_minimumfiredelay = 0;
*/


//MODDD - keep track of "sv_cheats".
cvar_t* sv_cheatsRefClient = 0;

//cvar_t* cvar_minimumfiredelaymem = 0;


void debugNodeMode(void){
	EASY_CVAR_SET_DEBUGONLY(drawNodeAll, 230);
	//EASY_CVAR_SET_DEBUGONLY(drawNodeSpecial, -1);
	EASY_CVAR_SET_DEBUGONLY(drawNodeConnections, 2);
	//EASY_CVAR_SET_DEBUGONLY(drawNodeAlternateTime, 0);
}
void debugNodeModeWeak(void){
	EASY_CVAR_SET_DEBUGONLY(drawNodeAll, 500);
	//EASY_CVAR_SET_DEBUGONLY(drawNodeSpecial, -1);
	EASY_CVAR_SET_DEBUGONLY(drawNodeConnections, 1);
	//EASY_CVAR_SET_DEBUGONLY(drawNodeAlternateTime, 0);
}
void debugNodeModeOff(void){
	EASY_CVAR_SET_DEBUGONLY(drawNodeAll, -1);
	EASY_CVAR_SET_DEBUGONLY(drawNodeSpecial, -1);
	EASY_CVAR_SET_DEBUGONLY(drawNodeConnections, 0);
	EASY_CVAR_SET_DEBUGONLY(drawNodeAlternateTime, 0);
}





/*
void DrawSetupPathTrack(CPathTrack* pathTrackTarget, int lineIDOffset){
	
	if(pathTrackTarget->pev->spawnflags & SF_PATH_DISABLED){
		//color it red.
		DebugLine_SetupPoint(lineIDOffset+3, pathTrackTarget->pev->origin, 255, 0, 0);
	}else{
		//color it green.
		DebugLine_SetupPoint(lineIDOffset+3, pathTrackTarget->pev->origin, 0, 255, 0);
	}

	if(pathTrackTarget->m_pprevious != NULL){
		//line to previous is yellow.
		DebugLine_Setup(lineIDOffset+0, pathTrackTarget->pev->origin, pathTrackTarget->m_pprevious->pev->origin, 255, 255, 0);
	}
	if(pathTrackTarget->m_pnext != NULL){
		//line to next is blue.
		DebugLine_Setup(lineIDOffset+1, pathTrackTarget->pev->origin, pathTrackTarget->m_pnext->pev->origin, 0, 0, 255);
	}
	if(pathTrackTarget->m_paltpath != NULL){
		//line to alternate is white. Rarely exists most likely.
		DebugLine_Setup(lineIDOffset+2, pathTrackTarget->pev->origin, pathTrackTarget->m_paltpath->pev->origin, 255, 255, 255);
	}

}//END OF DrawSetupPathTrack
*/






BOOL attemptParseStringToInt(int* toStoreResult, const char* toRead, const char* errorMsg_badInput, const char* errorMsg_noInput){
	
	if(toRead != NULL && !isStringEmpty(toRead)){
		//try parsing it.
		try{
			int numbAttempt = tryStringToInt(toRead);
			*toStoreResult = numbAttempt;
			return TRUE;
		}catch(int){
			easyForcePrintLine(errorMsg_badInput);
			return FALSE;
		}
	}else{
		easyForcePrintLine(errorMsg_noInput);
		return FALSE;
	}//END OF arg check

	return FALSE;  //how could this be reached??
}//END OF attemptParseStringToInt

CPathTrack* getPathTrackWithID(int argID){

	//try to find it.
	CBaseEntity* pEntityScan = NULL;
	while( (pEntityScan = UTIL_FindEntityByClassname(pEntityScan, "path_track")) != NULL){
		CPathTrack* pathTrackRef = static_cast<CPathTrack*>(pEntityScan);
		if(pathTrackRef != NULL && pathTrackRef->PathTrackID == argID){
			//found it!
			return pathTrackRef;
			break;
		}

	}//END OF while loop

	//printout alongside this.
	easyForcePrintLine("path_track of ID %d not found.", argID);
	return NULL;
}//END OF getPathTrackWithID

CFuncTrackChange* getTrackChangeWithID(int argID){
	
	//try to find it.
	CBaseEntity* pEntityScan = NULL;
	while( (pEntityScan = UTIL_FindEntityByClassname(pEntityScan, "func_trackchange")) != NULL){
		CFuncTrackChange* trackChangeRef = static_cast<CFuncTrackChange*>(pEntityScan);
		if(trackChangeRef != NULL && trackChangeRef->FuncTrackChangeID == argID){
			//found it!
			return trackChangeRef;
			break;
		}

	}//END OF while loop

	//printout alongside this.
	easyForcePrintLine("func_trackchange of ID %d not found.", argID);
	return NULL;

}//END OF getTrackChangeWithID














CBaseMonster* getMonsterWithID(int argSeekID){
	//find monster ID of argSeekID.
	edict_t		*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );
	CBaseEntity *pEntity;
	int			count;
	float		distance, delta;
	count = 0;
	if ( !pEdict )
		return NULL;
	for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
	{
		if ( pEdict->free )	// Not in use
			continue;
		if ( !(pEdict->v.flags & (FL_CLIENT|FL_MONSTER)) )	// Not a client/monster ?
			continue;

		pEntity = CBaseEntity::Instance(pEdict);
		if ( !pEntity )
			continue;

		CBaseMonster* tempMonster = pEntity->MyMonsterPointer();
		if(tempMonster == NULL || FClassnameIs(tempMonster->pev, "player")){
			continue;  //not players or non-monsters.
		}

		if(tempMonster->monsterID == argSeekID){
			//go!
			return tempMonster;
			//break;  redundant, "return" already interrupts.
		}

	}//END OF list through all entities.

	//did not find it?
	return NULL;
}











void interpretAsCoords(CBaseEntity* arg_target, const char* arg_targetName){

	easyForcePrintLine("%s\'s Origin: (%.2f, %.2f, %.2f)\n%s\'s Angles: (%.2f, %.2f, %.2f)",
				arg_targetName,
				arg_target->pev->origin.x, arg_target->pev->origin.y, arg_target->pev->origin.z,
				arg_targetName,
				arg_target->pev->angles.x, arg_target->pev->angles.y, arg_target->pev->angles.z);


	/*
	easyForcePrintLine("Your coords:\nMy Origin: (%.2f, %.2f, %.2f)\nMy Angles: (%.2f, %.2f, %.2f)",
				tempplayer->pev->origin.x, tempplayer->pev->origin.y, tempplayer->pev->origin.z,
				tempplayer->pev->angles.x, tempplayer->pev->angles.y, tempplayer->pev->angles.z);

			easyForcePrintLine("%s\'s coords:\nOrigin: (%.2f, %.2f, %.2f)\nAngles: (%.2f, %.2f, %.2f)",
				STRING(pEntityForward->pev->classname),
				pEntityForward->pev->origin.x, pEntityForward->pev->origin.y, pEntityForward->pev->origin.z,
				pEntityForward->pev->angles.x, pEntityForward->pev->angles.y, pEntityForward->pev->angles.z);
	*/
}

void interpretAsHealth(CBaseEntity* arg_target, const char* arg_arg1Ref, const char* arg_targetName ){


	if(isStringEmpty(arg_arg1Ref)){
		
		//if no argument was provided, we're fetching the stats of this entity.
		easyForcePrintLine("%s\'s health:%d %s\'s maxHealth:%d deadflag:%d IsAlive:%d", arg_targetName, (int)arg_target->pev->health, arg_targetName, (int)arg_target->pev->max_health, arg_target->pev->deadflag, arg_target->IsAlive() );
	}else{
		//set the entity's current health to the provided argument, if it is a whole number.
		
		if(checkMatchIgnoreCase(arg_arg1Ref, "full") ){
			//"full" is a keyword for the arg that means, set to max health.
			arg_target->pev->health = arg_target->pev->max_health;
		}else if(checkMatchIgnoreCase(arg_arg1Ref, "dead") || checkMatchIgnoreCase(arg_arg1Ref, "kill")  ){
			//keyword for dead.
			arg_target->pev->health = 0;
		}else{
			try{
				int numbAttempt = tryStringToInt(arg_arg1Ref);
				arg_target->pev->health = (float)numbAttempt;
				easyForcePrintLine("%s\'s health set to %d.", arg_targetName, numbAttempt);
			}catch(int){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
			}
		}


	}


	/*
	CBaseEntity* pEntity = FindEntityForward( tempplayer );
	if ( pEntity )
	{
		if(isStringEmpty(arg1ref)){
			//if no argument was provided, we're fetching the stats of this entity.
			easyForcePrintLine("%s\nhealth: %d\nmaxHealth: %d", STRING(pEntity->pev->classname), pev->health, pev->max_health );
		}else{
			//set the entity's current health to the provided argument, if it is a whole number.
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				pEntity->pev->health = numbAttempt;
				easyForcePrintLine("setHealth successful.");
			}catch(int err){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
			}
		}
	}else{
		easyForcePrintLine("Could not find something to set / get health of!");
	}
	*/

}



//MODDD - new.
void resetPlayerRPGRockets(CBasePlayer* playerRef){
	//CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);   already done.

	if(playerRef){
		//...why is this here?!
		//playerRef->pev->flags &= ~FL_GODMODE;

		for(int i = 0; i < MAX_ITEM_TYPES; i++){
			CBasePlayerItem *pPlayerItem = playerRef->m_rgpPlayerItems[ i ];

			while ( pPlayerItem )
			{
					
				CBasePlayerWeapon* temp = (CBasePlayerWeapon*) pPlayerItem->GetWeaponPtr();
				
				if(temp != NULL && temp->m_iId == WEAPON_RPG){
					CRpg* temp2 = (CRpg *)temp;
					temp2->m_cActiveRockets = 0;
					return;
				}

				pPlayerItem = pPlayerItem->m_pNext;
			}//END OF while
		}//END OF for(...)
	}//END OF if(playerRef)

}//END OF resetPlayerRPGRockets







//The result of "fixcvars" or "unfuckup".
//NOTE: defined here instead of in combat.cpp (usual place for things to be extern'd out
//globally) because client.cpp is better for working with type "CBasePlayer" (more convenient)
void resetModCVars(CBasePlayer* arg_plyRef, BOOL isEmergency){
	
	entvars_t* pev = (arg_plyRef!=NULL)?arg_plyRef->pev:NULL;


	BOOL minimumFireDelayWasOn = FALSE;
	if(global_cheat_minimumfiredelaycustom == 1){
		minimumFireDelayWasOn = TRUE;
	}
	
	
	//"nocheat"
	/*
	CVAR_SET_FLOAT("cheat_infiniteclip", 0 );
	CVAR_SET_FLOAT("cheat_infiniteammo", 0 );
	CVAR_SET_FLOAT("cheat_minimumfiredelay", 0 );
	CVAR_SET_FLOAT("cheat_nogaussrecoil", 0);
	CVAR_SET_FLOAT("gaussRecoilSendsUpInSP", 0);
	//minimumfiredelaycustom
	CVAR_SET_FLOAT("infiniteLongJumpCharge", 0);
	CVAR_SET_FLOAT("cheat_touchNeverExplodes", 0);
	*/

	
	//CVAR_SET_FLOAT("IGNOREminimumfiredelaymem", 0);
	globalPSEUDO_minimumfiredelaymem = 0;

	if(arg_plyRef != NULL){
		resetPlayerRPGRockets( arg_plyRef );
	}

	//if the barnacle cheat is on, turn it of.
	if(cheat_barnacleEatsEverything == 1){
		cheat_barnacleEatsEverything = 2;
		if(arg_plyRef != NULL){
			easyForcePrintLine("CHEAT OFF: Barnacles returned to normal.");
			//This is done so the player can save the cheat.
			arg_plyRef->myRef_barnacleEatsEverything = cheat_barnacleEatsEverything;
		}
	}
		
	CVAR_SET_FLOAT( "hud_logo", 0 );

		
	CVAR_SET_FLOAT("cl_fvox", 1);
	CVAR_SET_FLOAT("cl_explosion", 0);

	CVAR_SET_FLOAT("pissedNPCs", 0);




	UTIL_ServerMassCVarReset(pev);





	if(!isEmergency){
		easyForcePrintLine("***Remember to restart if a precache error has happend during this launch of the game***");
	}
	/*
	if(CVAR_GET_FLOAT("precacheAll") == 1){
		CVAR_SET_FLOAT("precacheAll", 0);
		easyForcePrintLine("***ALSO: precacheAll turned off.  Turn it back on if desired***");
	}
	*/

	if(arg_plyRef!=NULL && ( (arg_plyRef->pev->flags & FL_NOTARGET) || arg_plyRef->m_fNoPlayerSound == TRUE) ) {
		arg_plyRef->turnOffSneaky();

		if(global_autoSneaky != 0){
			EASY_CVAR_SET_DEBUGONLY(autoSneaky, 0);
			easyForcePrintLine("***AI cheats (notarget / nosound) detected and turned off.  autoSneaky reset to 0.***");
		}else{
			easyForcePrintLine("***AI cheats (notarget / nosound)  detected and turned off.***");
		}
	}

	if(global_itemBatteryPrerequisite == 1){
		//if(!isEmergency){
			easyForcePrintLine("***\"itemBatteryPrerequisite\" is on.  Remember to have at least one battery charge before expecting longjump, injectibles (besides adrenaline), and a few other things to work.***");
		//}
	}

	float temppppp = CVAR_GET_FLOAT("soundSentenceSave");

	easyForcePrintLine("*** soundSentenceSave is %d.  Read up about it if unsure of if this is appropriate (1 advised)***", (int)temppppp );

	float temppppp2 = CVAR_GET_FLOAT("precacheAll");

	if(temppppp == 1 && temppppp2 != 1){
		easyForcePrintLine("***~ soundSentenceSave is most effective when paired with \"precacheAll = 1\".***");
	}
	if(temppppp != 1 && temppppp2 == 1){
		easyForcePrintLine("***~ precacheAll is most effective when paired with \"soundSentenceSave = 1\".***");
	}
	easyForcePrintLine("***use \'fixprecache\' to turn both of these on.***");

		
	if(minimumFireDelayWasOn){
		easyForcePrintLine("***Minimum fire turned off, corresponding custom fire rate unaffected.  To control the fire rate of all weapons with \"minimum fire\", adjust \"cheat_minimumfiredelaycustom\"***");
	}

	if(isEmergency){
		easyForcePrintLine("!!! IMPORTANT !!!");
		easyForcePrintLine("CVar \"emergencyFix\" was activated last time.  Fix successfully applied.");
	}

	//reset kills the emergency flag.
	global_emergencyFix = 0;
	

	//if applicable..
	saveHiddenCVars();



}




void partyStart(CBasePlayer* playerRef){

	EASY_CVAR_SET_DEBUGONLY(myStrobe, 1);
	EASY_CVAR_SET_DEBUGONLY(peopleStrobe, 1);
	EASY_CVAR_SET_DEBUGONLY(forceWorldLightOff, 1);
	EASY_CVAR_SET_DEBUGONLY(wildHeads, 1);
	global_forceWorldLightOff = 1;
	turnWorldLightsOff();

	//CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
	if ( playerRef){
		edict_t* tempEd = ENT(playerRef->pev);

		//EMIT_SOUND(tempEd, CHAN_ITEM, "!partysong", 1, ATTN_NORM);


		/*
		int rando = RANDOM_LONG(0, 2);
		if(rando == 0){
			EMIT_SOUND(tempEd, CHAN_STREAM, "!partysong0", 1, ATTN_NORM);
		}else if(rando == 0){
			EMIT_SOUND(tempEd, CHAN_STREAM, "!partysong1", 1, ATTN_NORM);
		}else if(rando == 2){
			EMIT_SOUND(tempEd, CHAN_STREAM, "!partysong2", 1, ATTN_NORM);
		}
		*/

		//Conversion to mp3's:

		char* eh[] = {"cold_as_ice", "how_its_done", "smack_my", };
		int choice = RANDOM_LONG(0, 2);

		char* cmdStringSend = UTIL_VarArgs("mp3 play media/%s.mp3", eh[choice]);
		//CLIENT_COMMAND(tempEd, cmdStringSend);

		submitJukeboxRequest(tempEd, cmdStringSend);

	}
}

void partyOff(CBasePlayer* playerRef){


	EASY_CVAR_SET_DEBUGONLY(myStrobe, 0);
	EASY_CVAR_SET_DEBUGONLY(peopleStrobe, 0);
	EASY_CVAR_SET_DEBUGONLY(forceWorldLightOff, 0);
	EASY_CVAR_SET_DEBUGONLY(wildHeads, 0);
		
		
	EASY_CVAR_SET_DEBUGONLY(myStrobe, 0);
	EASY_CVAR_SET_DEBUGONLY(peopleStrobe, 0);
	EASY_CVAR_SET_DEBUGONLY(fogTest, 0);
	EASY_CVAR_SET_DEBUGONLY(imAllFuckedUp, 0);
	EASY_CVAR_SET_DEBUGONLY(thatWasntGrass, 0)
	EASY_CVAR_SET_DEBUGONLY(thatWasntPunch, 0)

	

	global_forceWorldLightOff = 0;
	turnWorldLightsOn();
		
	//CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
	if ( playerRef){
		edict_t* tempEd = ENT(playerRef->pev);


		//EMIT_SOUND_DYN2(tempEd, CHAN_ITEM, "!partysong", 0, 0, SND_STOP, PITCH_NORM);
		//stop.

		/*
		EMIT_SOUND( tempEd, CHAN_VOICE, "common/null.wav", 1.0, ATTN_IDLE );
		EMIT_SOUND( tempEd, CHAN_ITEM, "common/null.wav", 1.0, ATTN_IDLE );
		EMIT_SOUND( tempEd, CHAN_STREAM, "common/null.wav", 1.0, ATTN_IDLE );
		*/
		//EMIT_SOUND_DYN( tempEd, CHAN_STREAM, SOUND_FLASHLIGHT_ON, 1.0, ATTN_NORM, 0, PITCH_NORM );

		//no, just do this instead.
		submitJukeboxOff(tempEd);
	}

}















/*
 * used by kill command and disconnect command
 * ROBIN: Moved here from player.cpp, to allow multiple player models
 */
void set_suicide_frame(entvars_t* pev)
{       
	if (!FStrEq(STRING(pev->model), "models/player.mdl"))
		return; // allready gibbed

//	pev->frame		= $deatha11;
	pev->solid		= SOLID_NOT;
	pev->movetype	= MOVETYPE_TOSS;
	pev->deadflag	= DEAD_DEAD;
	pev->nextthink	= -1;
}


/*
===========
ClientConnect

called when a player connects to a server
============
*/
BOOL ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]  )
{	
	return g_pGameRules->ClientConnected( pEntity, pszName, pszAddress, szRejectReason );

// a client connecting during an intermission can cause problems
//	if (intermission_running)
//		ExitIntermission ();

}


/*
===========
ClientDisconnect

called when a player disconnects from a server

GLOBALS ASSUMED SET:  g_fGameOver
============
*/
void ClientDisconnect( edict_t *pEntity )
{
	if (g_fGameOver)
		return;

	char text[256];
	sprintf( text, "- %s has left the game\n", STRING(pEntity->v.netname) );
	MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
		WRITE_BYTE( ENTINDEX(pEntity) );
		WRITE_STRING( text );
	MESSAGE_END();

	CSound *pSound;
	pSound = CSoundEnt::SoundPointerForIndex( CSoundEnt::ClientSoundIndex( pEntity ) );
	{
		// since this client isn't around to think anymore, reset their sound. 
		if ( pSound )
		{
			pSound->Reset();
		}
	}

// since the edict doesn't get deleted, fix it so it doesn't interfere.
	pEntity->v.takedamage = DAMAGE_NO;// don't attract autoaim
	pEntity->v.solid = SOLID_NOT;// nonsolid
	UTIL_SetOrigin ( &pEntity->v, pEntity->v.origin );

	g_pGameRules->ClientDisconnected( pEntity );
}


// called by ClientKill and DeadThink
void respawn(entvars_t* pev, BOOL fCopyCorpse)
{
	if (gpGlobals->coop || gpGlobals->deathmatch)
	{
		if ( fCopyCorpse )
		{
			// make a copy of the dead body for appearances sake
			CopyToBodyQue(pev);
		}

		// respawn player
		GetClassPtr( (CBasePlayer *)pev)->Spawn( );
	}
	else
	{       // restart the entire server
		SERVER_COMMAND("reload\n");
	}
}

/*
============
ClientKill

Player entered the suicide command

GLOBALS ASSUMED SET:  g_ulModelIndexPlayer
============
*/
void ClientKill( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;

	CBasePlayer *pl = (CBasePlayer*) CBasePlayer::Instance( pev );

	if ( pl->m_fNextSuicideTime > gpGlobals->time )
		return;  // prevent suiciding too ofter

	pl->m_fNextSuicideTime = gpGlobals->time + 1;  // don't let them suicide for 5 seconds after suiciding

	// have the player kill themself
	pev->health = 0;
	pl->Killed( pev, pev, GIB_NEVER );

//	pev->modelindex = g_ulModelIndexPlayer;
//	pev->frags -= 2;		// extra penalty
//	respawn( pev );
}

/*
===========
ClientPutInServer

called each time a player is spawned
============
*/
void ClientPutInServer( edict_t *pEntity )
{
	CBasePlayer *pPlayer;

	entvars_t *pev = &pEntity->v;

	pPlayer = GetClassPtr((CBasePlayer *)pev);
	pPlayer->SetCustomDecalFrames(-1); // Assume none;

	// Allocate a CBasePlayer for pev, and call spawn
	pPlayer->Spawn() ;

	// Reset interpolation during first frame
	pPlayer->pev->effects |= EF_NOINTERP;
}

//old voice_gamemgr.h include location.
extern CVoiceGameMgr g_VoiceGameMgr;

//// HOST_SAY
// String comes in as
// say blah blah blah
// or as
// blah blah blah
//
void Host_Say( edict_t *pEntity, int teamonly )
{
	CBasePlayer *client;
	int		j;
	char	*p;
	char	text[128];
	char    szTemp[256];
	const char *cpSay = "say";
	const char *cpSayTeam = "say_team";
	const char *pcmd = CMD_ARGV(0);

	// We can get a raw string now, without the "say " prepended
	if ( CMD_ARGC() == 0 )
		return;

	entvars_t *pev = &pEntity->v;
	CBasePlayer* player = GetClassPtr((CBasePlayer *)pev);

	//Not yet.
	if ( player->m_flNextChatTime > gpGlobals->time )
		 return;

	if ( !stricmp( pcmd, cpSay) || !stricmp( pcmd, cpSayTeam ) )
	{
		if ( CMD_ARGC() >= 2 )
		{
			p = (char *)CMD_ARGS();
		}
		else
		{
			// say with a blank message, nothing to do
			return;
		}
	}
	else  // Raw text, need to prepend argv[0]
	{
		if ( CMD_ARGC() >= 2 )
		{
			sprintf( szTemp, "%s %s", ( char * )pcmd, (char *)CMD_ARGS() );
		}
		else
		{
			// Just a one word command, use the first word...sigh
			sprintf( szTemp, "%s", ( char * )pcmd );
		}
		p = szTemp;
	}

// remove quotes if present
	if (*p == '"')
	{
		p++;
		p[strlen(p)-1] = 0;
	}

// make sure the text has content
	char *pc;
	for ( pc = p; pc != NULL && *pc != 0; pc++ )
	{
		if ( isprint( *pc ) && !isspace( *pc ) )
		{
			pc = NULL;	// we've found an alphanumeric character,  so text is valid
			break;
		}
	}
	if ( pc != NULL )
		return;  // no character found, so say nothing

// turn on color set 2  (color on,  no sound)
	if ( teamonly )
		sprintf( text, "%c(TEAM) %s: ", 2, STRING( pEntity->v.netname ) );
	else
		sprintf( text, "%c%s: ", 2, STRING( pEntity->v.netname ) );

	j = sizeof(text) - 2 - strlen(text);  // -2 for /n and null terminator
	if ( (int)strlen(p) > j )
		p[j] = 0;

	strcat( text, p );
	strcat( text, "\n" );


	player->m_flNextChatTime = gpGlobals->time + CHAT_INTERVAL;

	// loop through all players
	// Start with the first player.
	// This may return the world in single player if the client types something between levels or during spawn
	// so check it, or it will infinite loop

	client = NULL;
	while ( ((client = (CBasePlayer*)UTIL_FindEntityByClassname( client, "player" )) != NULL) && (!FNullEnt(client->edict())) ) 
	{
		if ( !client->pev )
			continue;
		
		if ( client->edict() == pEntity )
			continue;

		if ( !(client->IsNetClient()) )	// Not a client ? (should never be true)
			continue;

		// can the receiver hear the sender? or has he muted him?
		if ( g_VoiceGameMgr.PlayerHasBlockedPlayer( client, player ) )
			continue;

		if ( teamonly && g_pGameRules->PlayerRelationship(client, CBaseEntity::Instance(pEntity)) != GR_TEAMMATE )
			continue;

		MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, client->pev );
			WRITE_BYTE( ENTINDEX(pEntity) );
			WRITE_STRING( text );
		MESSAGE_END();

	}

	// print to the sending client
	MESSAGE_BEGIN( MSG_ONE, gmsgSayText, NULL, &pEntity->v );
		WRITE_BYTE( ENTINDEX(pEntity) );
		WRITE_STRING( text );
	MESSAGE_END();

	// echo to server console
	g_engfuncs.pfnServerPrint( text );

	char * temp;
	if ( teamonly )
		temp = "say_team";
	else
		temp = "say";
	
	// team match?
	if ( g_teamplay )
	{
		UTIL_LogPrintf( "\"%s<%i><%s><%s>\" %s \"%s\"\n", 
			STRING( pEntity->v.netname ), 
			GETPLAYERUSERID( pEntity ),
			GETPLAYERAUTHID( pEntity ),
			g_engfuncs.pfnInfoKeyValue( g_engfuncs.pfnGetInfoKeyBuffer( pEntity ), "model" ),
			temp,
			p );
	}
	else
	{
		UTIL_LogPrintf( "\"%s<%i><%s><%i>\" %s \"%s\"\n", 
			STRING( pEntity->v.netname ), 
			GETPLAYERUSERID( pEntity ),
			GETPLAYERAUTHID( pEntity ),
			GETPLAYERUSERID( pEntity ),
			temp,
			p );
	}
}


/*
===========
ClientCommand
called each time a player uses a "cmd" command
============
*/
extern float g_flWeaponCheat;






//MODDD - NOTICE: method now unused.
char* stringRemoveConstProperty(const char *input){
	char* chrReturn;

	int inputLength = -1; //unknown yet.

	int j = 0;
	while(true){

		inputLength += 1;
		if(input[j] == '\0' || input[j] == 0){
		//if(  !((input[j] >= 'a' && input[j] <= 'z') || (input[j] >= 'A' && input[j] <= 'Z'))  ){
		//if(input[j] != 'r'){
			break;
		}

		if(j > 20){
			//emergency break!
			return "ERROR: STRING TOO LONG OR NOT TERMINATED";
		}

		j += 1;
	}

	chrReturn = new char[inputLength];

	for(int i = 0; i < inputLength; i++){
		chrReturn[i] = input[i];
	}
	chrReturn[inputLength] = '\0';

	return chrReturn;
}








// Use CMD_ARGV,  CMD_ARGV, and CMD_ARGC to get pointers the character string command.
void ClientCommand( edict_t *pEntity )
{
	const char *pcmd = CMD_ARGV(0);
	const char *pstr;

	
	char pcmdRefined[128];
	strncpy( &pcmdRefined[0], &pcmd[0], 127 );
	pcmdRefined[127] = '\0';
	lowercase(&pcmdRefined[0]);

	const char* pcmdRefinedRef = pcmdRefined;


	//!!!pcmdRefined

	//easyForcePrintLine("LOOK AT ME|%s", pcmd);


	//MODDD - new
	if(sv_cheatsRefClient == 0){
		sv_cheatsRefClient = CVAR_GET_POINTER( "sv_cheats" );
	}

	//disabled.
	/*
	if(cvar_protoVersion == 0){
		cvar_protoVersion = CVAR_GET_POINTER("protoVersion");
	}
	if(!protoVersionMemGiven && cvar_protoVersion != 0){
		protoVersionMemGiven = true;
		strcpy(protoVersionMem, cvar_protoVersion->string);
	}
	*/


	//MODDD - seems like a decent place for cheat logic (if applicable).


	/*
	if(clientCheat_infiniteclip == 0){
		clientCheat_infiniteclip = CVAR_GET_POINTER("cheat_infiniteclip");
	}
	if(clientCheat_infiniteammo == 0){
		clientCheat_infiniteammo = CVAR_GET_POINTER("cheat_infiniteammo");
	}
	if(clientCheat_minimumfiredelay == 0){
		clientCheat_minimumfiredelay = CVAR_GET_POINTER("cheat_minimumfiredelay");
	}
	

	if(cvar_minimumfiredelaymem == 0){
		cvar_minimumfiredelaymem = CVAR_GET_POINTER("IGNOREminimumfiredelaymem");
	}
	*/



	
	
	//MODDD - update "g_flWeaponCheat" to what sv_cheats is.
	if(sv_cheatsRefClient != 0){
		if(sv_cheatsRefClient->value == 1){
			g_flWeaponCheat = 1;
		}else{
			g_flWeaponCheat = 0;
		}
	}

	



	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
		return;

	entvars_t *pev = &pEntity->v;

	

	if(gmsgRetrieveFOV > 0 && globalPSEUDO_aspectratio_determined_fov == -1){
		//request the client re-send that FOV info.
		MESSAGE_BEGIN( MSG_ONE, gmsgRetrieveFOV, NULL, pev );
			//WRITE_BYTE( drowning );
		MESSAGE_END();
	}

	

	//MODDD
	//If the minimumfiredelay mem's  value does NOT match minimumfiredelay, it must have been changed.
	//Force an RPG re-update to be safe.
	if(globalPSEUDO_minimumfiredelaymem != global_cheat_minimumfiredelay){
		//CVAR_SET_FLOAT("IGNOREminimumfiredelaymem", global_cheat_minimumfiredelay);
		globalPSEUDO_minimumfiredelaymem = global_cheat_minimumfiredelay;
		resetPlayerRPGRockets(GetClassPtr((CBasePlayer *)pev));
	}

	


	//easyForcePrintLine("HEY ClientCommand GOT CALLED! %s", pcmd);

	
	/*
	easyForcePrintLine("__________________");
	easyForcePrintLine("%s", pcmd);
	easyForcePrintLine("==================");
	
		int iii = 0;
		while(iii < 255){
			if(pcmd[iii] == '\0'){
				break;
			}
			iii++;
		}

		easyForcePrintLine("SIZE (index of 0): %d", iii);


		*/




	
	




	//if the "else" statement of this long line of possible commands is caught, it means we can check the hidden-vars for a match instead.
	//(assume "TRUE" until then; reaching the last "else" changes it to FALSE)
	BOOL caughtByFirst = TRUE;


	if ( FStrEq(pcmdRefinedRef, "say" ) )
	{
	
		
		//ClientPrint( &pEntity->v, HUD_PRINTCONSOLE, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaB" );

		//easyForcePrintLine("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaBBB" );

		Host_Say( pEntity, 0 );
	}
	else if ( FStrEq(pcmdRefinedRef, "say_team" ) )
	{
		Host_Say( pEntity, 1 );
	}
	else if ( FStrEq(pcmdRefinedRef, "fullupdate" ) )
	{
		GetClassPtr((CBasePlayer *)pev)->ForceClientDllUpdate(); 
	}
	else if ( FStrEq(pcmdRefinedRef, "give" ) )
	{
		if ( g_flWeaponCheat != 0.0)
		{
			//int iszItem = ALLOC_STRING( CMD_ARGV(1) );	// Make a copy of the classname
			//GetClassPtr((CBasePlayer *)pev)->GiveNamedItem( STRING(iszItem) );
			//MODDD - just send the CMD_ARG as it is, this method now does the other stuff.

			//Note that "CMD_ARGV(#)" never seems to return null.  It would just return an empty string, a
			//string with only the '\0' terminating character if nothing can be picked up.


			GetClassPtr((CBasePlayer *)pev)->GiveNamedItem( CMD_ARGV(1), attemptInterpretSpawnFlag(CMD_ARGV(2)) );

			//easyForcePrintLine("??? |%s| %d", CMD_ARGV(2), ( lengthOfString(CMD_ARGV(2))) );

		}
	}
	//MODDD - inserted new command: givedist.  Change the "givedoffset" var to change what distance items / weapons / npcs are spawned
	//from you.  Although, at large distances (above 100 usually), looking down too much will make it spawn underground and fall through
	//the world.
	else if ( FStrEq(pcmdRefinedRef, "givedist" ) )
	{

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;


		if ( tempplayer && g_flWeaponCheat != 0.0)
		{

			
			UTIL_MakeVectors(pev->v_angle + pev->punchangle);

			//For CBasePlayer, "pev->origin + pev->view_ofs" is the same as "GetGunPosition()"

			Vector vecDest = pev->origin + pev->view_ofs + gpGlobals->v_forward * global_offsetgivedistance;

			///tempplayer->debugDrawVectRecentGive1 = pev->origin + pev->view_ofs;
			///tempplayer->debugDrawVectRecentGive2 = vecDest;

			tempplayer->GiveNamedItem( CMD_ARGV(1), attemptInterpretSpawnFlag(CMD_ARGV(2)), vecDest);

		}

	}

	//MODDD - other command added.  Spawns item around where you are looking (experimental)
	//Also, "ent_create" does this since this seems to be the intention.
	else if( FStrEq(pcmdRefinedRef, "givelook" ) || FStrEq(pcmdRefinedRef, "ent_create" ) ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			

		if ( tempplayer && g_flWeaponCheat != 0.0)
		{

			edict_t		*pentIgnore;
			TraceResult tr;

			pentIgnore = tempplayer->edict();
			UTIL_MakeVectors(pev->v_angle + pev->punchangle);

			//a tiny bit in front for safety.
			Vector vecSrc = pev->origin + pev->view_ofs + gpGlobals->v_forward * 5;
			Vector vecDest = pev->origin + pev->view_ofs + gpGlobals->v_forward * 2048;

			//nah, precision for while ducking not necessary.
			/*
			Vector playerEyePos = Vector(tempplayer->body;
			if(pev->flags & FL_DUCKING){

			}
			*/

			UTIL_TraceLine( vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr );
			//tr.vecEndPos();

			if (tr.fAllSolid){
				ALERT ( at_console, "Could not determine spawn location.\n" );
				return;
			}

			if(tr.pHit != NULL){
				easyForcePrintLine("HIT SOMETHING? %s", STRING(tr.pHit->v.classname) );
			}else{
				easyForcePrintLine("DID NOT HIT SOMETHING?");
			}


			/*
			easyForcePrintLine("YEEEE %.2f", tr.flFraction);
			UTIL_printLineVector("VECTOA", tr.vecPlaneNormal);
			*/

			//tempplayer->debugDrawVectRecentGive1 = vecSrc;
			//tempplayer->debugDrawVectRecentGive2 = tr.vecEndPos;

			tempplayer->GiveNamedItem( CMD_ARGV(1),  attemptInterpretSpawnFlag(CMD_ARGV(2)),
									 tr.vecEndPos.x,
									 tr.vecEndPos.y,
									 tr.vecEndPos.z + global_offsetgivelookvertical,
									 TRUE, &tr);



			
		}

	}
	//block unused.
	/*else if( FStrEq(pcmd, "updateDifficulty" ) || FStrEq(pcmd, "updateDiff" ) ){
	
		easyForcePrintLine("THE ARG IS: \"%s\"", CMD_ARGV(1));

		if( !FStrEq(CMD_ARGV(1), "")){
			g_iSkillLevel = atoi(CMD_ARGV(1));
		}

		//g_pGameRules->ClientCommand( GetClassPtr((CBasePlayer *)pev), pcmd);

	
	
	}*/
	else if ( FStrEq(pcmdRefinedRef, "everything") || FStrEq(pcmdRefinedRef, "gimmie") || FStrEq(pcmdRefinedRef, "gimmi") || FStrEq(pcmdRefinedRef, "gimmy") || FStrEq(pcmdRefinedRef, "fuckmyshitup") ){

		if ( g_flWeaponCheat != 0.0)
		{
			CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
			if(playerRef){
				playerRef->grantAllItems();
				playerRef->giveMaxAmmo();
		
				playerRef->setHealth(100);
				playerRef->setArmorBattery(100);
				//playerRef->pev->flags |= FL_GODMODE;
				//playerRef->pev->flags |= MOVETYPE_NOCLIP;
			}

		}
	}else if ( FStrEq(pcmdRefinedRef, "supergun") || FStrEq(pcmdRefinedRef, "neverrunout") ){
		if ( g_flWeaponCheat != 0.0)
		{
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteclip, 1 );
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteammo, 1 );
		}

	}else if ( FStrEq(pcmdRefinedRef, "allcheats") || FStrEq(pcmdRefinedRef, "allcheat") || FStrEq(pcmdRefinedRef, "cheats") || FStrEq(pcmdRefinedRef, "cheat") || FStrEq(pcmdRefinedRef, "cheatersalwayswin") || FStrEq(pcmdRefinedRef, "fuckeverything") || FStrEq(pcmdRefinedRef, "iamshivagodofdeath")  ){
		if ( g_flWeaponCheat != 0.0)
		{
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteclip, 1 );
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteammo, 1 );
		EASY_CVAR_SET_DEBUGONLY(cheat_minimumfiredelay, 1 );
		//EASY_CVAR_SET("IGNOREminimumfiredelaymem", 1);
		globalPSEUDO_minimumfiredelaymem = 1;

		CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
			if(playerRef){
				playerRef->grantAllItems();
				playerRef->giveMaxAmmo();
		
				playerRef->setHealth(100);
				playerRef->setArmorBattery(100);
				//playerRef->pev->flags |= FL_GODMODE;
				//playerRef->pev->flags |= MOVETYPE_NOCLIP;
			}



		}

	}else if ( FStrEq(pcmdRefinedRef, "infiniteclip") ){
		if ( g_flWeaponCheat != 0.0)
		{
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteclip, global_cheat_infiniteclip==0?1:0 );
		}

	}else if ( FStrEq(pcmdRefinedRef, "infiniteammo") ){
		if ( g_flWeaponCheat != 0.0)
		{
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteammo, global_cheat_infiniteammo==0?1:0 );
		}
	}else if (  FStrEq(pcmdRefinedRef, "minimumfiredelay") || FStrEq(pcmdRefinedRef, "minimumdelay") || FStrEq(pcmdRefinedRef, "firedelay") || FStrEq(pcmdRefinedRef, "dakkadakkadakka")|| FStrEq(pcmdRefinedRef, "dakka") ||FStrEq(pcmdRefinedRef, "dakadakadaka") || FStrEq(pcmdRefinedRef, "daka") || FStrEq(pcmdRefinedRef, "needsmoredaka") || FStrEq(pcmdRefinedRef, "needsmoardaka") || FStrEq(pcmdRefinedRef, "needsmoredakka") || FStrEq(pcmdRefinedRef, "needsmoardakka") || FStrEq(pcmdRefinedRef, "notenoughdaka") || FStrEq(pcmdRefinedRef, "notenoughdakka") || FStrEq(pcmdRefinedRef, "notenuffdaka") || FStrEq(pcmdRefinedRef, "notenuffdakka") || FStrEq(pcmdRefinedRef, "notenufdaka") || FStrEq(pcmdRefinedRef, "notenufdakka")   || FStrEq(pcmdRefinedRef, "neverenoughdaka") || FStrEq(pcmdRefinedRef, "neverenoughdakka") || FStrEq(pcmdRefinedRef, "neverenuffdaka") || FStrEq(pcmdRefinedRef, "neverenuffdakka") || FStrEq(pcmdRefinedRef, "neverenufdaka") || FStrEq(pcmdRefinedRef, "neverenufdakka")   || FStrEq(pcmdRefinedRef, "nevaenoughdaka") || FStrEq(pcmdRefinedRef, "nevaenoughdakka") || FStrEq(pcmdRefinedRef, "nevaenuffdaka") || FStrEq(pcmdRefinedRef, "nevaenuffdakka") || FStrEq(pcmdRefinedRef, "nevaenufdaka") || FStrEq(pcmdRefinedRef, "nevaenufdakka")          ){

		if ( g_flWeaponCheat != 0.0)
		{
			EASY_CVAR_SET_DEBUGONLY(cheat_minimumfiredelay, global_cheat_minimumfiredelay==0?1:0 );
			//CVAR_SET_FLOAT("IGNOREminimumfiredelaymem", CVAR_GET_FLOAT("cheat_minimumfiredelay"));
			globalPSEUDO_minimumfiredelaymem = global_cheat_minimumfiredelay;
			resetPlayerRPGRockets( GetClassPtr((CBasePlayer *)pev) );
		}

	}else if (  FStrEq(pcmdRefinedRef, "superman")){

		//TODO: lock these cheats by sv_cheats (or g_flWeaponCheat ? )
		EASY_CVAR_SET_DEBUGONLY(normalSpeedMulti, 3);
		EASY_CVAR_SET_DEBUGONLY(noclipSpeedMulti, 5);
		EASY_CVAR_SET_DEBUGONLY(jumpForceMulti, 3);

	}else if( FStrEq(pcmdRefinedRef, "normalman")){

		EASY_CVAR_SET_DEBUGONLY(normalSpeedMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(noclipSpeedMulti, 2.5);
		EASY_CVAR_SET_DEBUGONLY(jumpForceMulti, 1);

	
	}else if ( FStrEq(pcmdRefinedRef, "disablecheats") || FStrEq(pcmdRefinedRef, "disablecheat") || FStrEq(pcmdRefinedRef, "nocheating") || FStrEq(pcmdRefinedRef, "nocheats") || FStrEq(pcmdRefinedRef, "nocheat") || FStrEq(pcmdRefinedRef, "cheatingisforpussies") || FStrEq(pcmdRefinedRef, "winnersdontdodrugs") ){

		CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
		
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteclip, 0 );
		EASY_CVAR_SET_DEBUGONLY(cheat_infiniteammo, 0 );
		EASY_CVAR_SET_DEBUGONLY(cheat_minimumfiredelay, 0 );
		globalPSEUDO_minimumfiredelaymem = 0;
		EASY_CVAR_SET_DEBUGONLY(cheat_nogaussrecoil, 0);
		resetPlayerRPGRockets( playerRef );

		//if the barnacle cheat is on, turn it of.
		if(cheat_barnacleEatsEverything == 1){
			cheat_barnacleEatsEverything = 2;
			if(playerRef){
				easyForcePrintLine("CHEAT OFF: Barnacles returned to normal.");
				//This is done so the player can save the cheat.
				playerRef->myRef_barnacleEatsEverything = cheat_barnacleEatsEverything;
			}
		}


	}else if ( FStrEq(pcmdRefinedRef, "thealmightybarnacle") ){
		if ( g_flWeaponCheat != 0.0)
		{
			if(cheat_barnacleEatsEverything == 0){
				cheat_barnacleEatsEverything = 1;
			}else if(cheat_barnacleEatsEverything == 1){
				cheat_barnacleEatsEverything = 2;
			}else if(cheat_barnacleEatsEverything == 2){
				cheat_barnacleEatsEverything = 1;
			}
			CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
			if(playerRef){
				//This is done so the player can save the cheat.
				playerRef->myRef_barnacleEatsEverything = cheat_barnacleEatsEverything;
			}
			
			//myRef_barnacleEatsEverything

			if(cheat_barnacleEatsEverything == 1){
				easyForcePrintLine("CHEAT ON: Barnacles can eat any monster (NPC)!");
			}else if(cheat_barnacleEatsEverything == 2){
				easyForcePrintLine("CHEAT OFF: Barnacles returned to normal.");
			}
		}
	}else if( FStrEq(pcmdRefinedRef, "fixrpg") ){
		//This command is also included in anything toggling / changing "cheat_minimumfiredelay".
		//It resets an important var (m_cActiveRockets) that, when non-zero, makes the player unable to reload
		//in follow (dot) fire mode.
		resetPlayerRPGRockets( GetClassPtr((CBasePlayer *)pev) );
	}else if( FStrEq(pcmdRefinedRef, "fixcvar") || FStrEq(pcmdRefinedRef, "resetcvar") || FStrEq(pcmdRefinedRef, "fixcvars") || FStrEq(pcmdRefinedRef, "reestcvars") || FStrEq(pcmdRefinedRef, "unfuckup") || FStrEq(pcmdRefinedRef, "unfuckupcvar") || FStrEq(pcmdRefinedRef, "unfuckupcvars") || FStrEq(pcmdRefinedRef, "unfuck") || FStrEq(pcmdRefinedRef, "unfuckcvar") || FStrEq(pcmdRefinedRef, "unfuckcvars") ){
		//reset the most annoying CVars.

		CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
		
		resetModCVars(playerRef, FALSE);


	}else if ( FStrEq(pcmdRefinedRef, "fixstuka") || FStrEq(pcmdRefinedRef, "stukafix")  ){

		
		//TODO.  FUCKING.  TODO.
		EASY_CVAR_SET_DEBUGONLY(STUcheckDistH, 6);
		EASY_CVAR_SET_DEBUGONLY(STUcheckDistV, 8);
		EASY_CVAR_SET_DEBUGONLY(STUcheckDistD, 10);
		EASY_CVAR_SET_DEBUGONLY(STUextraTriangH, 1.2f);
		EASY_CVAR_SET_DEBUGONLY(STUextraTriangV, 1.3f);
		EASY_CVAR_SET_DEBUGONLY(STUrepelMulti, 1.0f);
		EASY_CVAR_SET_DEBUGONLY(STUSpeedMulti, 1.0f);
		EASY_CVAR_SET_DEBUGONLY(drawDebugPathfinding2, 0);
		EASY_CVAR_SET_DEBUGONLY(STUExplodeTest, 0);
		EASY_CVAR_SET_DEBUGONLY(STUYawSpeedMulti, 0.88);
		EASY_CVAR_SET_DEBUGONLY(STUDetection, 1);
		
		
		


	}else if ( FStrEq(pcmdRefinedRef, "partyfix") || FStrEq(pcmdRefinedRef, "fixparty") || FStrEq(pcmdRefinedRef, "resetparty") || FStrEq(pcmdRefinedRef, "partyreset") ){

		EASY_CVAR_SET_DEBUGONLY(raveEffectSpawnInterval, 0.22)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMin, 0.08)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMax, 0.14)
		EASY_CVAR_SET_DEBUGONLY(strobeRadiusMin, 280)
		EASY_CVAR_SET_DEBUGONLY(strobeRadiusMax, 410)
		EASY_CVAR_SET_DEBUGONLY(strobeSpawnDistHori, 180)
		EASY_CVAR_SET_DEBUGONLY(strobeSpawnDistVertMin, 25)
		EASY_CVAR_SET_DEBUGONLY(strobeSpawnDistVertMax, 75)
		EASY_CVAR_SET_DEBUGONLY(strobeMultiColor, 1)
		EASY_CVAR_SET_DEBUGONLY(raveLaserEnabled, 1)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnFreq, 2.3)
		EASY_CVAR_SET_DEBUGONLY(raveLaserLength, 180)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnDistHoriMin, 50)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnDistHoriMax, 110)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnDistVertMin, -5)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnDistVertMax, 45)
		EASY_CVAR_SET_DEBUGONLY(raveLaserBrightnessMin, 1.0)
		EASY_CVAR_SET_DEBUGONLY(raveLaserBrightnessMax, 1.0)
		EASY_CVAR_SET_DEBUGONLY(raveLaserDurationMin, 0.06)
		EASY_CVAR_SET_DEBUGONLY(raveLaserDurationMax, 0.12)
		EASY_CVAR_SET_DEBUGONLY(raveLaserThicknessMin, 0.8)
		EASY_CVAR_SET_DEBUGONLY(raveLaserThicknessMax, 2.6)
		EASY_CVAR_SET_DEBUGONLY(raveLaserNoiseMin, 0)
		EASY_CVAR_SET_DEBUGONLY(raveLaserNoiseMax, 0)
		EASY_CVAR_SET_DEBUGONLY(raveLaserFrameRateMin, 10)
		EASY_CVAR_SET_DEBUGONLY(raveLaserFrameRateMax, 10)
		EASY_CVAR_SET_DEBUGONLY(raveLaserMultiColor, 1)
		EASY_CVAR_SET_DEBUGONLY(thatWasntGrass, 0)
		EASY_CVAR_SET_DEBUGONLY(thatWasntPunch, 0)

		

	}else if( FStrEq(pcmdRefinedRef, "resetSparks") || FStrEq(pcmdRefinedRef, "tooManySparks")   ){
	
		EASY_CVAR_SET_DEBUGONLY(sparksAllMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksEnvMulti, 0.5f);
		EASY_CVAR_SET_DEBUGONLY(sparksButtonMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksPlayerCrossbowMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksComputerHitMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksTurretDeathMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksOspreyHitMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksExplosionMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksBeamMulti, 1);
		EASY_CVAR_SET_DEBUGONLY(sparksAIFailMulti, 1);
		//CVAR_SET_FLOAT("shrapRand", 190);
		//CVAR_SET_FLOAT("shrapRandHeightExtra", 140);
		//CVAR_SET_FLOAT("shrapMode", 1);
		//CVAR_SET_FLOAT("explosionShrapnelMulti", 1);

	
	}else if( FStrEq(pcmdRefinedRef, "partyon") || FStrEq(pcmdRefinedRef, "party") || FStrEq(pcmdRefinedRef, "partymode") || FStrEq(pcmdRefinedRef, "whospikedthepunch") || FStrEq(pcmdRefinedRef, "clubon") ||FStrEq(pcmdRefinedRef, "rave") || FStrEq(pcmdRefinedRef, "technorave") || FStrEq(pcmdRefinedRef, "techno") || FStrEq(pcmdRefinedRef, "club") || FStrEq(pcmdRefinedRef, "clubbing") || FStrEq(pcmdRefinedRef, "breaktime") || FStrEq( pcmdRefinedRef, "ecstasy") || FStrEq(pcmdRefinedRef, "partytime") || FStrEq(pcmdRefinedRef, "indaclub") || FStrEq(pcmdRefinedRef, "intheclub") || FStrEq(pcmdRefinedRef, "whodruggedmyshit") || FStrEq(pcmdRefinedRef, "drugs") || FStrEq(pcmdRefinedRef, "thattastedfunny") || FStrEq(pcmdRefinedRef, "thattastesfunny") || FStrEq(pcmdRefinedRef, "tastesfunny") || FStrEq(pcmdRefinedRef, "krunk") || FStrEq(pcmdRefinedRef, "getfucked") || FStrEq(pcmdRefinedRef, "fuckedup") || FStrEq(pcmdRefinedRef, "getfuckedup") || FStrEq(pcmdRefinedRef, "letsgetwasted") || FStrEq(pcmdRefinedRef, "getwasted") ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		partyStart(tempplayer);


	}else if(  FStrEq(pcmdRefinedRef, "partyoff") || FStrEq(pcmdRefinedRef, "cluboff") ||FStrEq(pcmdRefinedRef, "icantseeshit") || FStrEq(pcmdRefinedRef, "kidsthesedays") || FStrEq(pcmdRefinedRef, "nomoreclubbing") || FStrEq(pcmdRefinedRef, "fucktheclub") || FStrEq(pcmdRefinedRef, "fuckdaclub") || FStrEq(pcmdRefinedRef, "breaktimeisover") || FStrEq(pcmdRefinedRef, "breaktimesover") || FStrEq(pcmdRefinedRef, "breaktimesup") || FStrEq(pcmdRefinedRef, "backtowork") || FStrEq(pcmdRefinedRef, "getbacktowork") || FStrEq(pcmdRefinedRef, "drugfree") || FStrEq(pcmdRefinedRef, "outtheclub") || FStrEq(pcmdRefinedRef, "outoftheclub") || FStrEq(pcmdRefinedRef, "dapopo") || FStrEq(pcmdRefinedRef, "popo") || FStrEq(pcmdRefinedRef, "police") || FStrEq(pcmdRefinedRef, "callthepolice") || FStrEq(pcmdRefinedRef, "thispartysucks") ){
		
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		partyOff(tempplayer);


	}else if(  FStrEq(pcmdRefinedRef, "hyperravebutnotext") || FStrEq(pcmdRefinedRef, "hyperavebutnotext") || FStrEq(pcmdRefinedRef, "hardcorebutnotext") || FStrEq(pcmdRefinedRef, "hyperravenotext") || FStrEq(pcmdRefinedRef, "hyperavenotext") || FStrEq(pcmdRefinedRef, "hardcorenotext")   ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		partyStart(tempplayer);


		
		EASY_CVAR_SET_DEBUGONLY(raveEffectSpawnInterval, 0.1)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMin, 0.06)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMax, 0.08)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnFreq, 2.3)
		//only difference: no crazy text.
		EASY_CVAR_SET_DEBUGONLY(imAllFuckedUp, 0)
		EASY_CVAR_SET_DEBUGONLY(thatWasntGrass, 1)
		EASY_CVAR_SET_DEBUGONLY(thatWasntPunch, 1)
		EASY_CVAR_SET_DEBUGONLY(fogTest, 2)

			
	
	}else if(  FStrEq(pcmdRefinedRef, "hyperrave") || FStrEq(pcmdRefinedRef, "hyperave") || FStrEq(pcmdRefinedRef, "hardcore")   ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		partyStart(tempplayer);


		
		EASY_CVAR_SET_DEBUGONLY(raveEffectSpawnInterval, 0.1)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMin, 0.06)
		EASY_CVAR_SET_DEBUGONLY(strobeDurationMax, 0.08)
		EASY_CVAR_SET_DEBUGONLY(raveLaserSpawnFreq, 2.3)
		EASY_CVAR_SET_DEBUGONLY(imAllFuckedUp, 1)
		EASY_CVAR_SET_DEBUGONLY(thatWasntGrass, 1)
		EASY_CVAR_SET_DEBUGONLY(thatWasntPunch, 1)
		EASY_CVAR_SET_DEBUGONLY(fogTest, 2)

			
	
	}else if(  FStrEq(pcmdRefinedRef, "fixCamera") || FStrEq(pcmdRefinedRef, "camerafix") || FStrEq(pcmdRefinedRef, "cameraReset") || FStrEq(pcmdRefinedRef, "cameraReset")    ){
	
		EASY_CVAR_SET_DEBUGONLY(myCameraSucks, 0)
		EASY_CVAR_SET_DEBUGONLY(cameraPosFixedX, -1)
		EASY_CVAR_SET_DEBUGONLY(cameraPosFixedY, -1)
		EASY_CVAR_SET_DEBUGONLY(cameraPosFixedZ, -1)
		EASY_CVAR_SET_DEBUGONLY(cameraPosOffX, 0)
		EASY_CVAR_SET_DEBUGONLY(cameraPosOffY, 0)
		EASY_CVAR_SET_DEBUGONLY(cameraPosOffZ, 260)
		EASY_CVAR_SET_DEBUGONLY(cameraRotFixedX, 90)
		EASY_CVAR_SET_DEBUGONLY(cameraRotFixedY, -1)
		EASY_CVAR_SET_DEBUGONLY(cameraRotFixedZ, -1)
		EASY_CVAR_SET_DEBUGONLY(cameraRotOffX, 0)
		EASY_CVAR_SET_DEBUGONLY(cameraRotOffY, 0)
		EASY_CVAR_SET_DEBUGONLY(cameraRotOffZ, 0)


	}else if ( FStrEq(pcmdRefinedRef, "fixprecache" ) || FStrEq(pcmdRefinedRef, "fixcache" ) || FStrEq(pcmdRefinedRef, "cachefix"   ) || FStrEq(pcmdRefinedRef, "precachefix")  ){
		

		CVAR_SET_FLOAT("precacheAll", 1);
		CVAR_SET_FLOAT("soundSentenceSave", 1);

		easyForcePrintLine("***precacheAll & soundSentenceSave activated.  Remember to restart if in-game, or close / re-launch if there has been any precache-related crash this session.***");


	}else if ( FStrEq(pcmdRefinedRef, "fvox" ) || FStrEq(pcmdRefinedRef, "fvoxtoggle" ) )
	{

		float thisTime = CVAR_GET_FLOAT("cl_fvox");
		if(thisTime == 1){
			CVAR_SET_FLOAT("cl_fvox", 0);
		}else{
			CVAR_SET_FLOAT("cl_fvox", 1);
		}

		//toggle FVOX on or off.
		//CVAR_SET_FLOAT( "cl_fvox", !CVAR_GET_FLOAT("cl_fvox") );

	}else if ( FStrEq(pcmdRefinedRef, "drop" ) )
	{
		// player is dropping an item. 
		GetClassPtr((CBasePlayer *)pev)->DropPlayerItem((char *)CMD_ARGV(1));
	}else if ( FStrEq(pcmdRefinedRef, "playbatterysound" ) )
	{
		
		int iszItem = ALLOC_STRING( CMD_ARGV(1) );
		const char* pszName = STRING(iszItem);

		int i = 0;
		int pszNameLength = 0;
		while(i < 99){
			if(pszName[i] == '\0'){
				pszNameLength =  i;
				break;
			}
			i++;
		}
		int result = 0;

		if(sscanf(pszName, "%d", &result) == EOF){
			easyForcePrintLine("That isn\'t a number, dillweed!");
		}else{

			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			
			if ( tempplayer)
			{
				//tempplayer->SetSuitUpdateNumber(tempplayer->pev->armorvalue, 0);
				
				/*
				if(result != 0){
					tempplayer->SetSuitUpdate("!HEV_BNOTICE", FALSE, SUIT_REPEAT_OK, 0.7f);
					tempplayer->SetSuitUpdateNumber(result, 0, -1, -1);
					tempplayer->SetSuitUpdate("!HEV_BPERCENT", FALSE, SUIT_REPEAT_OK, 0.8f);		 
				}else{
					tempplayer->SetSuitUpdateNumber(result, 0, -1, -1);
				}
				*/
				tempplayer->SetSuitUpdateNumber(result, 0, -1, FALSE);
				

			}

		}
		int istr = MAKE_STRING(pszName);
		easyForcePrintLine("help me %d", result);

		
		//delete[] stuff;


	}else if ( FStrEq(pcmdRefinedRef, "playtest2" ) )
	{


		int iszItem = ALLOC_STRING( CMD_ARGV(1) );
		const char* pszName = STRING(iszItem);

		int i = 0;
		int pszNameLength = 0;
		while(i < 99){
			if(pszName[i] == '\0'){
				pszNameLength =  i;
				break;
			}
			i++;
		}
		int result = 0;

		if(sscanf(pszName, "%d", &result) == EOF){
			easyForcePrintLine("That isn\'t a number, dillweed!");
		}else{
			
			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			
			if ( tempplayer){

				edict_t* tempEd = ENT(tempplayer->pev);

				switch(result){
				case 0:  //ENT(pev)???
					EMIT_SOUND(tempEd, CHAN_ITEM, "!playtestEnd", 1, ATTN_NORM);
				break;
				case 1:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/reload2.wav", 1, ATTN_NORM);
				break;
				case 2:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/reload3.wav", 1, ATTN_NORM);
				break;
				case 3:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/357_reload1.wav", 1, ATTN_NORM);
				break;
				case 4:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/guncock1.wav", 1, ATTN_NORM);
				break;
				case 5:
					EMIT_SOUND(tempEd, CHAN_ITEM, "items/gunpickup1.wav", 1, ATTN_NORM);
				break;
				case 6:
					EMIT_SOUND(tempEd, CHAN_ITEM, "items/gunpickup2.wav", 1, ATTN_NORM);
				break;
				case 7:
					EMIT_SOUND(tempEd, CHAN_ITEM, "items/gunpickup3.wav", 1, ATTN_NORM);
				break;
				case 8:
					EMIT_SOUND(tempEd, CHAN_ITEM, "items/gunpickup4.wav", 1, ATTN_NORM);
				break;
				case 9:
					EMIT_SOUND(tempEd, CHAN_ITEM, "vox/acknowledge.wav", 1, ATTN_NORM);
				break;
				case 10:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM);
				break;
				case 11:
					EMIT_SOUND(tempEd, CHAN_ITEM, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM);
				break;


				
				
				default:
					easyForcePrintLine("sound %d not found.", result);
				break;
				}

			
			}


		}






	}else if ( FStrEq(pcmdRefinedRef, "soundtest" ) ){


			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			
			if ( tempplayer){

				edict_t* tempEd = ENT(tempplayer->pev);
				
				int iszItem = ALLOC_STRING( CMD_ARGV(1) );
				const char* pszName = STRING(iszItem);


				//no, wait to start playing soon.
				//EMIT_SOUND(tempEd, CHAN_ITEM, pszName, 1, ATTN_NORM);
				
				copyString(pszName, queuedSound, 127 );

				//a slight delay.  A sound playing instantly may cut off unexpectedly.
				playQueuedTime = gpGlobals->time + 0.1f;
				
				playQueued = TRUE;

			}
		
	}else if ( FStrEq(pcmdRefinedRef, "sentencetest" ) ){


			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			
			if ( tempplayer){

				edict_t* tempEd = ENT(tempplayer->pev);
				
				int iszItem = ALLOC_STRING( CMD_ARGV(1) );
				const char* pszName = STRING(iszItem);


				//no, wait to start playing soon.
				//EMIT_SOUND(tempEd, CHAN_ITEM, pszName, 1, ATTN_NORM);
				

				if(pszName[0] != '!'){
					queuedSound[0] = '!';
					copyString(pszName, &queuedSound[1], 127 - 1);
					//offset.  Start at queuedSound[1], end at one less than usual size.
				}else{
					//eh, just overwrite that "!".
					copyString(pszName, queuedSound, 127 );
				}

				//a slight delay.  A sound playing instantly may cut off unexpectedly.
				playQueuedTime = gpGlobals->time + 0.1f;
				playQueued = TRUE;

			}
		
	}else if ( FStrEq(pcmdRefinedRef, "stopsoundtest" ) ){
			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			if ( tempplayer){
				edict_t* tempEd = ENT(tempplayer->pev);
				
				int iszItem = ALLOC_STRING( CMD_ARGV(1) );
				const char* pszName = STRING(iszItem);

				if(pszName[0] == '\0'){
					/*
					for(int i = 0; i < 20; i++){
						easyForcePrintLine("i: %d c: %d", i, pszName[i]);
						if(pszName[i] == '\0'){
							break;
						}
					}
					*/
					EMIT_SOUND( tempEd, CHAN_VOICE, "common/null.wav", 1.0, ATTN_IDLE );
					EMIT_SOUND( tempEd, CHAN_ITEM, "common/null.wav", 1.0, ATTN_IDLE );
					EMIT_SOUND( tempEd, CHAN_STREAM, "common/null.wav", 1.0, ATTN_IDLE );
				}else{
					EMIT_SOUND_DYN(tempEd, CHAN_STREAM, pszName, 0, 0, SND_STOP, PITCH_NORM);

				}

				//also turn off MP3 music.
				submitJukeboxOff(tempEd);	
			}

	}else if ( FStrEq(pcmdRefinedRef, "stopsentencetest" ) ){
			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
			if ( tempplayer){
				edict_t* tempEd = ENT(tempplayer->pev);
				
				int iszItem = ALLOC_STRING( CMD_ARGV(1) );
				const char* pszName = STRING(iszItem);

				if(pszName[0] == '\0'){
					EMIT_SOUND( tempEd, CHAN_VOICE, "common/null.wav", 1.0, ATTN_IDLE );
					EMIT_SOUND( tempEd, CHAN_ITEM, "common/null.wav", 1.0, ATTN_IDLE );
					EMIT_SOUND( tempEd, CHAN_STREAM, "common/null.wav", 1.0, ATTN_IDLE );

				}else{
					char tempsound[127];
					if(pszName[0] != '!'){
						tempsound[0] = '!';
						copyString(pszName, &tempsound[1], 127 - 1);
					}else{
						//eh, just overwrite that "!".
						copyString(pszName, tempsound, 127 );
					}

					EMIT_SOUND_DYN(tempEd, CHAN_STREAM, tempsound, 0, 0, SND_STOP, PITCH_NORM);
				}
				
				//also turn off MP3 music.
				submitJukeboxOff(tempEd);
			}//END OF if(tempplayer)

	}else if(FStrEq(pcmdRefinedRef, "coords") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		if(tempplayer){
			CBaseEntity* pEntityForward = FindEntityForward( tempplayer);
			if(pEntityForward == NULL){
				//assume we meant the player.
				interpretAsCoords(tempplayer, "Client");
			}else{
				interpretAsCoords(pEntityForward, STRING(pEntityForward->pev->classname) );
			}
		}
		
	}else if( FStrEq(pcmdRefinedRef, "mycoords") || FStrEq(pcmdRefinedRef, "playercoords") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		
		if(tempplayer){
			interpretAsCoords(tempplayer, "Client");
		}


	}else if( FStrEq(pcmdRefinedRef, "entcoords") || FStrEq(pcmdRefinedRef, "entitycoords") || FStrEq(pcmdRefinedRef, "monstercoords") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		
		CBaseEntity* pEntityForward = FindEntityForward( tempplayer );
		if(pEntityForward){
			interpretAsCoords(pEntityForward, STRING(pEntityForward->pev->classname) );
		}

	}else if( FStrEq(pcmdRefinedRef, "jukebox")){
		
		char tempCommandBuffer[128];
		//this OK?
		//char* cmdStringSend;
		//edict_t* tempEd = ENT(pev);
		
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		edict_t* tempEd = ENT(tempplayer->pev);
		//Not even this works?  Why?!

		//SERVER_COMMAND
		const char* arg1ref = CMD_ARGV(1);

		
		char arg1refFiltered[128];
		strncpy( &arg1refFiltered[0], &arg1ref[0], 127 );
		arg1refFiltered[127] = '\0';
		lowercase(&arg1refFiltered[0]);



		if(FStrEq(arg1refFiltered, "stop") || FStrEq(arg1refFiltered, "off" ) ){
			//don't try to play.  This is a code to stop any MP3 music playing now.
			submitJukeboxOff(pev);
			return;
		}



		if(isStringEmpty(arg1ref)){
			//no arguments?  Then we play a random HL.
			
			int choice = RANDOM_LONG(1, 17);
			/*
			if ( !pEntity->pvPrivateData )
				return;

			entvars_t *pev = &pEntity->v;
			*/

			//let's be careful with itoa or atoi...?
			//itoa(

			//Unwise to do it this way - any printouts between now and later will cause cmdStringSend to store that printout's contents instead.
			//The string buffer used in UTIL_VarArgs is shared throughout all calls to it. It is assumed it won't get reused until another printout
			//is made.
			//Not as bad here, but for future use keep this in mind.
			//sprintf does fine here.
			//cmdStringSend = UTIL_VarArgs("mp3 play media/Half-Life%02d.mp3", 0, choice);
			
			sprintf(&tempCommandBuffer[0], "mp3 play media/Half-Life%02d.mp3", choice);
			easyForcePrintLine("RESULT: %s", tempCommandBuffer);

		}else{

			//UNFORTUNATELY, this whole ignore case for playing MP3 files is pointless.
			//The M
			if(stringEndsWithIgnoreCase(arg1ref, ".mp3")){
				sprintf(&tempCommandBuffer[0], "mp3 play media/%s", arg1ref);
			}else{
				//doesn't end with .mp3?  add it there.
				sprintf(&tempCommandBuffer[0], "mp3 play media/%s.mp3", arg1ref);
			}

		}
		//CLIENT_COMMAND(pEntity, cmdStringSend);
		//CLIENT_COMMAND(tempEd, "mp3 play media/Half-Life11.mp3");
		//CLIENT_COMMAND(tempEd, cmdStringSend);
		submitJukeboxRequest(pev, tempCommandBuffer);



	}else if( FStrEq(pcmdRefinedRef, "lazy") || FStrEq(pcmdRefinedRef, "lazyass") || FStrEq(pcmdRefinedRef, "wakeup") || FStrEq(pcmdRefinedRef, "wakethefuckup") || FStrEq(pcmdRefinedRef, "rise") || FStrEq(pcmdRefinedRef, "revive") || FStrEq(pcmdRefinedRef, "risefromthegrave") || FStrEq(pcmdRefinedRef, "getthefuckup") || FStrEq(pcmdRefinedRef, "suprisemotherfucker") || FStrEq(pcmdRefinedRef, "suprisemothafucka") || FStrEq(pcmdRefinedRef, "suprisemotherfucka") || FStrEq(pcmdRefinedRef, "suprisemothafucker") || FStrEq(pcmdRefinedRef, "getup") || FStrEq(pcmdRefinedRef, "titties") || FStrEq(pcmdRefinedRef, "tits") || FStrEq(pcmdRefinedRef, "tittys") || FStrEq(pcmdRefinedRef, "alyxisnaked") || FStrEq(pcmdRefinedRef, "alyxisnude") || FStrEq(pcmdRefinedRef, "younodie") || FStrEq(pcmdRefinedRef, "myparentsarenthome")   ){
		

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);




		//First a check. Is the player dead? If so this is a self-revive.
		if(tempplayer->pev->deadflag != DEAD_NO){
			
			if(g_flWeaponCheat != 0.0){
				//can't do this without cheats.
				tempplayer->Spawn(TRUE);
			}else{
				easyForcePrintLine("Cheaters stay dead!");
			}

		}else{

			//Time to play necromancer. Perhaps.

			if(g_flWeaponCheat == 0.0){
				easyForcePrintLine("Necromancy is not your strongpoint.");
				return;
			}

			CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

			if(forwardEnt != NULL){
				CBaseMonster* tempMon;
				tempMon = forwardEnt->GetMonsterPointer();

				if(tempMon == NULL){
					easyForcePrintLine("ERROR: Thing in crosshairs is not a monster: %s.", forwardEnt->getClassname());
					return;
				}

				if(tempMon->pev->deadflag < DEAD_DEAD){
					//dying or NO? Deny.
					easyForcePrintLine("That isn\'t dead yet, genius.");
					return;
				}

				tempMon->startReanimation();

			}else{
				easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
			}
		
		
		}//END OF player dead check else.


		
		
	}else if( FStrEq(pcmdRefinedRef, "ent_remove") || FStrEq(pcmdRefinedRef, "remove") || FStrEq(pcmdRefinedRef, "removeent") || FStrEq(pcmdRefinedRef, "removeentity") || FStrEq(pcmdRefinedRef, "entremove") || FStrEq(pcmdRefinedRef, "entityremove") || FStrEq(pcmdRefinedRef, "kaplow") || FStrEq(pcmdRefinedRef, "ihateyou") || FStrEq(pcmdRefinedRef, "begone") || FStrEq(pcmdRefinedRef, "begonepeasant")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Can\'t remove things without cheats.");
			return;
		}
		
		CBaseEntity* pEntityForward = FindEntityForward( tempplayer );
		if ( pEntityForward )
		{
			//if ( pEntity->pev->takedamage )

			//in case there's any cleaning to do first (stop looping sounds).
			//pEntityForward->onDelete();
			//SUB_Remove calls this already, no need to manually  call onDelete.
			
			pEntityForward->pev->nextthink = gpGlobals->time;
			pEntityForward->SetThink(&CBaseEntity::SUB_Remove);
		}else{
			easyForcePrintLine("Could not find something to remove!");
		}
		
	}else if( FStrEq(pcmdRefinedRef, "health") || FStrEq(pcmdRefinedRef, "gethealth") || FStrEq(pcmdRefinedRef, "sethealth")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No health trickery for you, cheater!");
			return;
		}

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			//assume this is for the player.
			interpretAsHealth(tempplayer, arg1ref, "Client");
		}else{
			interpretAsHealth(forwardEnt, arg1ref, STRING(forwardEnt->pev->classname) );
		
			//easyForcePrintLine("MODEL: %s", STRING(forwardEnt->pev->model));
		}


	
	}else if( FStrEq(pcmdRefinedRef, "setmyhealth") || FStrEq(pcmdRefinedRef, "setplayerhealth") || FStrEq(pcmdRefinedRef, "getmyhealth") || FStrEq(pcmdRefinedRef, "getplayerhealth") || FStrEq(pcmdRefinedRef, "myhealth") || FStrEq(pcmdRefinedRef, "playerhealth")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		//tryStringToInt

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No health trickery for you, cheater!");
			return;
		}


		interpretAsHealth((CBaseEntity*) tempplayer, arg1ref, "Client" );

	}else if( FStrEq(pcmdRefinedRef, "setenthealth") || FStrEq(pcmdRefinedRef, "setmonsterhealth") ||  FStrEq(pcmdRefinedRef, "setentityhealth") || FStrEq(pcmdRefinedRef, "getenthealth") || FStrEq(pcmdRefinedRef, "getmonsterhealth") ||  FStrEq(pcmdRefinedRef, "getentityhealth") || FStrEq(pcmdRefinedRef, "monsterhealth") || FStrEq(pcmdRefinedRef, "enthealth") || FStrEq(pcmdRefinedRef, "entityhealth")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No health trickery for you, cheater!");
			return;
		}

		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			interpretAsHealth(forwardEnt, arg1ref, STRING(forwardEnt->pev->classname) );
		}else{
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}
	}else if( FStrEq(pcmdRefinedRef, "id") || FStrEq(pcmdRefinedRef, "getid")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No id trickery for you, cheater!");
			return;
		}

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			easyForcePrintLine("ERROR: Could not find a monster in crosshairs.");
		}else{
			CBaseMonster* tempMonster = forwardEnt->GetMonsterPointer();
			if(tempMonster == NULL){
				easyForcePrintLine("ERROR: Entity in crosshairs, \"%s\", is not a monster or subclass.", STRING(forwardEnt->pev->classname) );
			}else{
				easyForcePrintLine("ID:%d Class:%s", tempMonster->monsterID, STRING(tempMonster->pev->classname) );
			}
		}
	
	}else if( FStrEq(pcmdRefinedRef, "getsize") || FStrEq(pcmdRefinedRef, "sizes") ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			easyForcePrintLine("ERROR: Could not find an entity in crosshairs.");
		}else{
			
			easyForcePrintLine("------Sizedata of %s------", forwardEnt->getClassname());

			easyForcePrintLine("mins: (%.2f %.2f %.2f) maxs: (%.2f %.2f %.2f)", 
				forwardEnt->pev->mins.x,
				forwardEnt->pev->mins.y,
				forwardEnt->pev->mins.z,
				forwardEnt->pev->maxs.x,
				forwardEnt->pev->maxs.y,
				forwardEnt->pev->maxs.z
				);
			Vector temp = (forwardEnt->pev->maxs - forwardEnt->pev->mins);
			easyForcePrintLine("delta: (%.2f %.2f %.2f) size: %.2f", temp.x, temp.y, temp.z, (temp.x*temp.y*temp.z));

			easyForcePrintLine("absmin: (%.2f %.2f %.2f) absmax: (%.2f %.2f %.2f)", 
				forwardEnt->pev->absmin.x,
				forwardEnt->pev->absmin.y,
				forwardEnt->pev->absmin.z,
				forwardEnt->pev->absmax.x,
				forwardEnt->pev->absmax.y,
				forwardEnt->pev->absmax.z
				);
			temp = (forwardEnt->pev->absmax - forwardEnt->pev->absmin);
			easyForcePrintLine("delta: (%.2f %.2f %.2f) size: %.2f", temp.x, temp.y, temp.z, (temp.x*temp.y*temp.z));

			easyForcePrintLine("-------------------------"  );



		}

	
	}else if( FStrEq(pcmdRefinedRef, "drawpathtrack") || FStrEq(pcmdRefinedRef, "pathtrackdraw")  || FStrEq(pcmdRefinedRef, "drawtrackpath") || FStrEq(pcmdRefinedRef, "trackpathdraw")  ){
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CPathTrack* pathTrackTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		DebugLine_ClearAll();

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide path ID to draw.");
		if(!parseSuccess)return;

		pathTrackTarget = getPathTrackWithID(trackIDToFind);

		if(pathTrackTarget != NULL){
			::DebugLine_SetupPathTrack(pathTrackTarget);

			//Calling this because paths don't usually get a chance to ever show this information. They are invisible and might not even collide with a player-crosshair traceline anyways.
			pathTrackTarget->ReportGeneric();  
		}//END OF pathTrackTarget check


	
	}else if( FStrEq(pcmdRefinedRef, "teleportpathtrack") || FStrEq(pcmdRefinedRef, "pathtrackteleport") || FStrEq(pcmdRefinedRef, "teleporttrackpath") || FStrEq(pcmdRefinedRef, "trackpathteleport")   ){
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CPathTrack* pathTrackTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide path ID to move to.");
		if(!parseSuccess)return;

		pathTrackTarget = getPathTrackWithID(trackIDToFind);

		if(pathTrackTarget != NULL){
			//go there!
			tempplayer->pev->origin = pathTrackTarget->pev->origin;
			easyForcePrintLine("Teleport successful.");
		}

	
	}else if( FStrEq(pcmdRefinedRef, "enablepathtrack") || FStrEq(pcmdRefinedRef, "pathtrackenable") || FStrEq(pcmdRefinedRef, "enabletrackpath") || FStrEq(pcmdRefinedRef, "trackpathenable") ){
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CPathTrack* pathTrackTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide path ID to enable.");
		if(!parseSuccess)return;

		pathTrackTarget = getPathTrackWithID(trackIDToFind);

		if(pathTrackTarget != NULL){
			if(pathTrackTarget->pev->spawnflags & SF_PATH_DISABLED){
				pathTrackTarget->pev->spawnflags &= ~SF_PATH_DISABLED;
				easyForcePrintLine("SUCCESS - path_track:%d enabled.", trackIDToFind);
			}else{
				easyForcePrintLine("-path_track:%d already enabled.", trackIDToFind);
			}
		}//END OF pathTrackTarget check


	
	}else if( FStrEq(pcmdRefinedRef, "disablepathtrack") || FStrEq(pcmdRefinedRef, "pathtrackdisable") || FStrEq(pcmdRefinedRef, "disabletrackpath") || FStrEq(pcmdRefinedRef, "trackpathdisable") ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CPathTrack* pathTrackTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide path ID to disable.");
		if(!parseSuccess)return;

		pathTrackTarget = getPathTrackWithID(trackIDToFind);

		if(pathTrackTarget != NULL){
			if( !(pathTrackTarget->pev->spawnflags & SF_PATH_DISABLED) ){
				pathTrackTarget->pev->spawnflags |= SF_PATH_DISABLED;
				easyForcePrintLine("SUCCESS - path_track:%d disabled.", trackIDToFind);
			}else{
				easyForcePrintLine("-path_track:%d already disabled.", trackIDToFind);
			}
		}//END OF pathTrackTarget check

	
	
	}else if( FStrEq(pcmdRefinedRef, "drawtrackchange") || FStrEq(pcmdRefinedRef, "trackchangedraw")){
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CFuncTrackChange* trackChangeTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		DebugLine_ClearAll();

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide trackchange ID to draw.");
		if(!parseSuccess)return;

		trackChangeTarget = getTrackChangeWithID(trackIDToFind);

		if(trackChangeTarget != NULL){
			
			DebugLine_Setup(5, trackChangeTarget->pev->origin + Vector(0, 0, -80), trackChangeTarget->pev->origin + Vector(0, 0, 80), 32, 89, 89);

			if(trackChangeTarget->m_trackTop != NULL){
				::DebugLine_SetupPathTrack(trackChangeTarget->m_trackTop);
			}
			if(trackChangeTarget->m_trackBottom != NULL){
				::DebugLine_SetupPathTrack(trackChangeTarget->m_trackBottom);
			}

			//just a good idea.
			trackChangeTarget->ReportGeneric();  

		}//END OF trackChangeTarget check


	
	}else if( FStrEq(pcmdRefinedRef, "teleporttrackchange") || FStrEq(pcmdRefinedRef, "trackchangeteleport")){
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		int trackIDToFind = -1;
		CFuncTrackChange* trackChangeTarget;
		BOOL parseSuccess;

		if(g_flWeaponCheat == 0.0){easyForcePrintLine("No.");return;}

		parseSuccess = attemptParseStringToInt(&trackIDToFind, arg1ref, "ERROR! Input number ID could not be understood.", "ERROR! Must provide trackchange ID to move to.");
		if(!parseSuccess)return;

		trackChangeTarget = getTrackChangeWithID(trackIDToFind);

		if(trackChangeTarget != NULL){
			//go there!
			tempplayer->pev->origin = trackChangeTarget->pev->origin;
			easyForcePrintLine("Teleport successful.");
		}

	
	}else if( FStrEq(pcmdRefinedRef, "report")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt;
		
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No reports for you, sorry.");
			return;
		}

		forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			forwardEnt->ReportGeneric();
		}else{
			easyForcePrintLine("ERROR: Could not find an entity in crosshairs.");
		}



	
	}else if( FStrEq(pcmdRefinedRef, "reportai") || FStrEq(pcmdRefinedRef, "reportstate") || FStrEq(pcmdRefinedRef, "getai") || FStrEq(pcmdRefinedRef, "getstate") || FStrEq(pcmdRefinedRef, "printai") || FStrEq(pcmdRefinedRef, "printState")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt;
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No ai trickery for you, cheater!");
			return;
		}


		forwardEnt = FindEntityForward(tempplayer);

		int monsterIDToUse = -1;

		if(arg1ref != NULL && !isStringEmpty(arg1ref)){
			//try parsing it.
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				monsterIDToUse = numbAttempt;
			}catch(int){
				easyForcePrintLine("ERROR! Input number ID could not be understood.");
				return;
			}

			if(monsterIDToUse != -1){
				//try to find it.
				forwardEnt = ::getMonsterWithID(monsterIDToUse);
				if(forwardEnt == NULL){
					//still didn't find it?
					easyForcePrintLine("ERROR: Monster of ID %d not found.", monsterIDToUse);
					return;
				}
			}
		}//END OF arg check



		if(forwardEnt == NULL){
			//try looking nearby?


			
		
		}

		//easyForcePrintLine("ARE YOU DAFT MAN?! 1");
		if(forwardEnt != NULL){
			//easyForcePrintLine("ARE YOU DAFT MAN?! 2A");
			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon == NULL){
				easyForcePrintLine("ERROR: thing in crosshairs is not a monster.  It is \"%s\".", STRING(forwardEnt->pev->classname) );
				easyForcePrintLine("Have some other stats. nextthink:%.2f. ltime:%.2f Current time:%.2f", forwardEnt->pev->nextthink, forwardEnt->pev->ltime, gpGlobals->time);
			}else{
				//got it!
				//easyForcePrintLine("ARE YOU DAFT MAN?! 3");
				testMon->ReportAIState();

				//testMon->pev->waterlevel = 1;  //does foricng it work?
				//..no.

				//easyForcePrintLine("ARE YOU DAFT MAN?! 4");
			}
		}else{
			//easyForcePrintLine("ARE YOU DAFT MAN?! 2B");
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}



	}else if( FStrEq(pcmdRefinedRef, "squadinfo") || FStrEq(pcmdRefinedRef, "squad") || FStrEq(pcmdRefinedRef, "reportsquad") || FStrEq(pcmdRefinedRef, "squadreport") || FStrEq(pcmdRefinedRef, "squadstate")  ){
	
	
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);
	
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No squad trickery for you, cheater!");
			return;
		}

		if(forwardEnt == NULL){
			easyForcePrintLine("SquadInfo failed: nothing found?  Aim with crosshairs.");
			return;
		}

		CSquadMonster* squadTest = forwardEnt->MySquadMonsterPointer();

		if(squadTest != NULL){


			const char* netnameSafe;

			if(FStringNull(squadTest->pev->netname) || ((netnameSafe = STRING(squadTest->pev->netname)) == NULL) ){
				netnameSafe = "__NULL__";
			}
			//otherwise, trust that "netnameSafe" is ok.

			easyForcePrintLine("***SQUADINFO FOR %s:%d deadflag:%d hasSquadCapaBit:%d netname:%s***", squadTest->getClassname(), squadTest->monsterID, squadTest->pev->deadflag, (squadTest->m_afCapability & bits_CAP_SQUAD)!=0, netnameSafe  );

			if(squadTest->InSquad() == FALSE){
				easyForcePrintLine("...not in squad.");
				return;
			}

			CSquadMonster* leader = squadTest->MySquadLeader();
			if(leader == NULL){
				easyForcePrintLine("Error 24.  no leader...?");
			}

			if(squadTest->IsLeader()){
				easyForcePrintLine("Leader: -ME!- %s:%d deadflag:%d", leader->getClassname(), leader->monsterID, leader->pev->deadflag);
			}else{
				easyForcePrintLine("Leader: %s:%d deadflag:%d", leader->getClassname(), leader->monsterID, leader->pev->deadflag);
			}

			for(int i = 0; i < 4; i++){
				CSquadMonster* thisMember = leader->MySquadMember(i);
				if(thisMember == NULL){
					easyForcePrintLine("Slot %d: EMPTY", i);
				}else{
					if(thisMember == squadTest){
						easyForcePrintLine("Slot %d: -Me!- %s:%d deadflag:%d", i, thisMember->getClassname(), thisMember->monsterID, thisMember->pev->deadflag);
					}else{
						easyForcePrintLine("Slot %d: %s:%d deadflag:%d", i, thisMember->getClassname(), thisMember->monsterID, thisMember->pev->deadflag);
					}
				}

			}

			easyForcePrintLine("***END OF SQUADINFO***");
		}else{
			easyForcePrintLine("SquadInfo failed: not a SquadMonster!  Thing picked: %s", forwardEnt->getClassname() );
		}



	}else if(FStrEq(pcmdRefinedRef, "reportnetname")){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			easyForcePrintLine("ERROR: Could not find a monster in crosshairs.");
		}else{
			CBaseMonster* tempMonster = forwardEnt->GetMonsterPointer();
			if(tempMonster == NULL){
				easyForcePrintLine("ERROR: Entity in crosshairs, \"%s\", is not a monster or subclass.", STRING(forwardEnt->pev->classname) );
			}else{
				tempMonster->reportNetName();
			}
		}
	
	
	}else if( FStrEq(pcmdRefinedRef, "selfquery") || FStrEq(pcmdRefinedRef, "queryself") || FStrEq(pcmdRefinedRef, "reportself") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer*)pev);
		//pev->solid			= SOLID_SLIDEBOX;
		//pev->movetype


		easyForcePrintLine("PLAYER STATS:");
		tempplayer->ReportAIState();
	
	}else if( FStrEq(pcmdRefinedRef, "gib") || FStrEq(pcmdRefinedRef, "gibbed") || FStrEq(pcmdRefinedRef, "gibme") || FStrEq(pcmdRefinedRef, "diehard") || FStrEq(pcmdRefinedRef, "diehard") || FStrEq(pcmdRefinedRef, "explode") || FStrEq(pcmdRefinedRef, "asplode") || FStrEq(pcmdRefinedRef, "yourheadasplode") || FStrEq(pcmdRefinedRef, "myheadasplode") ||  FStrEq(pcmdRefinedRef, "headasplode") || FStrEq(pcmdRefinedRef, "yourheadexplode") || FStrEq(pcmdRefinedRef, "myheadexplode") ||  FStrEq(pcmdRefinedRef, "headexplode")    ){
		
		//gib the player, for some reason...
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No exploding.");
			return;
		}

		BOOL godModeMem = tempplayer->pev->flags & FL_GODMODE;
		
		
		if(godModeMem){
			tempplayer->pev->flags &= ~FL_GODMODE;
		}

		//"DMG_FALL" in there so we ignore battery.
		tempplayer->TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 255, DMG_FALL|DMG_ALWAYSGIB, 0);

		if(godModeMem){
			tempplayer->pev->flags |= FL_GODMODE;
		}



	}else if( FStrEq(pcmdRefinedRef, "pleasedontcrash") || FStrEq(pcmdRefinedRef, "pleasedonotcrash") || FStrEq(pcmdRefinedRef, "dontcrash") || FStrEq(pcmdRefinedRef, "donotcrash")){
		
		
		EASY_CVAR_SET(explosionShrapnelMulti, 0);
		EASY_CVAR_SET_DEBUGONLY(sparksAllMulti, 0);


	}else if( FStrEq(pcmdRefinedRef, "dividebyzero") || FStrEq(pcmdRefinedRef, "crash") || FStrEq(pcmdRefinedRef, "enditalready") || FStrEq(pcmdRefinedRef, "thissucks") || FStrEq(pcmdRefinedRef, "iwantmydesktop") || FStrEq(pcmdRefinedRef, "iwannagohome")   ){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Really now? Oh no you don\'t.");
			return;
		}

		//Hey, you asked for it!
		int i = 1337;
		int zero = 0;

		int crashTime = i / zero;
		//easyForcePrintLine("%d", crashTime);

	}else if( FStrEq(pcmdRefinedRef, "tpnode") || FStrEq(pcmdRefinedRef, "teleporttonode") || FStrEq(pcmdRefinedRef, "nodeteleport") || FStrEq(pcmdRefinedRef, "nodetp")){
	
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Teleportation on a whim is not normal, mortal.");
			return;
		}


		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		
		try{
			int numbAttempt = tryStringToInt(arg1ref);
			
			if(numbAttempt >= 0 && numbAttempt < WorldGraph.m_cNodes){
				tempplayer->pev->origin = -tempplayer->pev->mins + WorldGraph.m_pNodes[numbAttempt].m_vecOriginPeek;
			}else{
				easyForcePrintLine("ERROR: node index out of bounds!");
			}

		}catch(int){
			easyForcePrintLine("Problem reading number.  (arg must be whole number)");
		}

	}else if( FStrEq(pcmdRefinedRef, "tpmonster") || FStrEq(pcmdRefinedRef, "teleporttomonster") || FStrEq(pcmdRefinedRef, "monsterteleport") || FStrEq(pcmdRefinedRef, "monstertp" ) ){
	
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Teleportation on a whim is not normal, mortal.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);
		
		try{
			int numbAttempt = tryStringToInt(arg1ref);
			
			CBaseMonster* attemptedMonsterFetch = getMonsterWithID(numbAttempt);

			if(attemptedMonsterFetch != 0){
				easyForcePrintLine("Found it.");
				tempplayer->pev->origin = attemptedMonsterFetch->pev->origin;
			}else{
				easyForcePrintLine("The map says we\'re fucked!");
			}

		}catch(int){
			easyForcePrintLine("Problem reading number.  (arg must be whole number)");
		}

	}else if( FStrEq(pcmdRefinedRef, "nodecon") || FStrEq(pcmdRefinedRef, "nodeconnections") || FStrEq(pcmdRefinedRef, "shownodecon") || FStrEq(pcmdRefinedRef, "shownodeconnections") || FStrEq(pcmdRefinedRef, "nodeneighbors")){
	
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

			const char* arg1ref = CMD_ARGV(1);
		
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				WorldGraph.ShowNodeConnections(numbAttempt);
			}catch(int){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
			}

	}else if( FStrEq(pcmdRefinedRef, "getnode") || FStrEq(pcmdRefinedRef, "getclosestnode") || FStrEq(pcmdRefinedRef, "getnearnode") || FStrEq(pcmdRefinedRef, "getnearestnode") || FStrEq(pcmdRefinedRef, "node") || FStrEq(pcmdRefinedRef, "closestnode") || FStrEq(pcmdRefinedRef, "nearnode") || FStrEq(pcmdRefinedRef, "nearestnode")   ){
			
		BOOL special = FALSE;
		BOOL ai = FALSE;
		BOOL target = FALSE;

		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		for(int i = 0; i < 3; i++){
			const char* thisArg = CMD_ARGV(i+1);

			if(!isStringEmpty(thisArg)){
				char thisArgRefined[128];
				strncpy( &thisArgRefined[0], &thisArg[0], 127 );
				thisArgRefined[127] = '\0';
				lowercase(&thisArgRefined[0]);
				const char* thisArgRefindeRef = thisArgRefined;

				if(FStrEq(thisArgRefindeRef, "special")){
					special = TRUE;
				}else if(FStrEq(thisArgRefindeRef, "ai")){
					ai = TRUE;
				}else if(FStrEq(thisArgRefindeRef, "target")){
					target = TRUE;
				}else{
					easyForcePrintLine("WARNING: arg \"%s\" unknown",  thisArg);
				}
			}
		}

		
		/*
		const char* arg1ref = CMD_ARGV(1);
		const char* arg2ref = CMD_ARGV(2);
		
		if( stringEndsWith(pcmdRefinedRef, "special") || (!isStringEmpty(arg1ref) && FStrEq(arg1ref, "special"))  ){
			//save that!
			special = TRUE;
		}
		if( stringEndsWith(pcmdRefinedRef, "ai") || (!isStringEmpty(arg1ref) && FStrEq(arg1ref, "ai"))  ){
			//save that!
			special = TRUE;
		}

		if( (stringEndsWith(pcmdRefinedRef, "specialai") || (!isStringEmpty(arg1ref) && FStrEq(arg1ref, "specialai"))  )
			|| (stringEndsWith(pcmdRefinedRef, "aispecial") || (!isStringEmpty(arg1ref) && FStrEq(arg1ref, "aispecial")))
			|| (FStrEq(arg2ref, "ai") && FStrEq(arg2ref, "special"))
			|| (FStrEq(arg2ref, "special") && FStrEq(arg2ref, "ai"))){
			//save that!
			special = TRUE;
			ai = TRUE;
		}
		*/
		

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);

		Vector vecSearchStart;
		if(!target){
			//player's position.
			vecSearchStart = tempplayer->pev->origin;
		}else{
			//get where the player is looking...
			TraceResult tr;

			UTIL_MakeVectors(pev->v_angle + pev->punchangle);

			//a tiny bit in front for safety.
			Vector vecSrc = pev->origin + pev->view_ofs + gpGlobals->v_forward * 5;
			Vector vecDest = pev->origin + pev->view_ofs + gpGlobals->v_forward * 2048;

			//nah, precision for while ducking not necessary.
			/*
			Vector playerEyePos = Vector(tempplayer->body;
			if(pev->flags & FL_DUCKING){

			}
			*/

			UTIL_TraceLine( vecSrc, vecDest, dont_ignore_monsters, tempplayer->edict(), &tr );
			//tr.vecEndPos();

			if (tr.fAllSolid){
				easyForcePrintLine ( "ERROR: solid trace line?" );
				return;
			}
			
			vecSearchStart = tr.vecEndPos;
		}

		if(ai == TRUE){
			int nodeTest = WorldGraph.FindNearestNode(vecSearchStart, tempplayer);

				
			if(nodeTest == -1){
				easyForcePrintLine("***ERROR: no nearby acceptable nodes.***");
			}else{
				if(target){
					easyForcePrintLine("*AI: Target is near node #%d", nodeTest);
				}else{
					easyForcePrintLine("*AI: Client is near node #%d", nodeTest);
				}
				
				if(special){
					EASY_CVAR_SET_DEBUGONLY(drawNodeSpecial, nodeTest);
					EASY_CVAR_SET_DEBUGONLY(drawNodeAlternateTime, 0);
				}
			}
		}else{
			//the ol' fashioned way.
			int closestNodeID = -1;
			float closestNodeDist = 99999999;
			for(int i = 0; i < WorldGraph.m_cNodes; i++){
				float thisDist = (WorldGraph.m_pNodes[i].m_vecOrigin - vecSearchStart).Length();
				if(thisDist < closestNodeDist){
					closestNodeID = i;
					closestNodeDist = thisDist;
				}
			}//END OF for(int i...)
			if(closestNodeID == -1){
				easyForcePrintLine("***ERROR: closest to no node at all.  (?)");
			}else{
				if(target){
					easyForcePrintLine("Target is closest to node #%d", closestNodeID);
				}else{
					easyForcePrintLine("Client is closest to node #%d", closestNodeID);
				}
				if(special){
					EASY_CVAR_SET_DEBUGONLY(drawNodeSpecial, closestNodeID);
					EASY_CVAR_SET_DEBUGONLY(drawNodeAlternateTime, 0);
				}
			}
		}

	}else if( FStrEq(pcmdRefinedRef, "nodebugnode") || FStrEq(pcmdRefinedRef, "debugnodeoff") || FStrEq(pcmdRefinedRef, "debugnodesoff") || FStrEq(pcmdRefinedRef, "debugnodemodeoff")  || FStrEq(pcmdRefinedRef, "nonodedebug") || FStrEq(pcmdRefinedRef, "nodedebugoff") || FStrEq(pcmdRefinedRef, "nodesdebugoff") || FStrEq(pcmdRefinedRef, "nodedebugmodeoff")   ){
		debugNodeModeOff();
	}else if( stringStartsWith(pcmdRefinedRef, "debugnode") || stringStartsWith(pcmdRefinedRef, "debugnodes") || stringStartsWith(pcmdRefinedRef, "debugnodemode") || stringStartsWith(pcmdRefinedRef, "nodedebug") || stringStartsWith(pcmdRefinedRef, "nodesdebug") || stringStartsWith(pcmdRefinedRef, "nodedebugmode")   ){
		
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		const char* arg1ref = CMD_ARGV(1);
		if( stringEndsWith(pcmdRefinedRef, "weak") || stringEndsWith(pcmdRefinedRef, "soft") || stringEndsWith(pcmdRefinedRef, "lite") || (!isStringEmpty(arg1ref) && (FStrEq(arg1ref, "weak")) || (FStrEq(arg1ref, "soft")) || (FStrEq(arg1ref, "lite")) )  ){
			debugNodeModeWeak();
		}else{
			debugNodeMode();
		}


	}else if( FStrEq(pcmdRefinedRef, "printoutisolatednodes") || FStrEq(pcmdRefinedRef, "printisolatednodes") || FStrEq(pcmdRefinedRef, "printoutdeadnodes") || FStrEq(pcmdRefinedRef, "printdeadnodes")  || FStrEq(pcmdRefinedRef, "printoutorphannodes") || FStrEq(pcmdRefinedRef, "printorphannodes")    ){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		BOOL printedOutYet = FALSE;
		for(int i = 0; i < WorldGraph.m_cNodes; i++){
			if(WorldGraph.m_pNodes[i].m_cNumLinks < 1){
				if(!printedOutYet){
					easyForcePrintLine("***Isolated Nodes***");
					printedOutYet =  TRUE;
				}
				easyForcePrintLine("-Node #%d", i);
			}
		}

		if(printedOutYet){
			easyForcePrintLine("*********");
		}else{
			easyForcePrintLine("*No isolated nodes found.");
		}

	}else if( stringStartsWith(pcmdRefinedRef, "printnode") || stringStartsWith(pcmdRefinedRef, "printoutnode") ){

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		BOOL printBlockingEnt = FALSE;
		if(stringEndsWith(pcmdRefinedRef, "2")){
			printBlockingEnt = TRUE;
		}

		easyForcePrintLine("***NODE CONNECTION MAP***");
		for(int i = 0; i < WorldGraph.m_cNodes; i++){
			easyForcePrint("#%d: ", i);


			CNode& thisNode = WorldGraph.m_pNodes[i];

			if(thisNode.m_cNumLinks < 1){
				easyForcePrint("none (ISOLATED)");
			}else{
				BOOL firstDest = TRUE;
				for(int i2 = 0; i2 < thisNode.m_cNumLinks; i2++){
					//CNode& pLinkNode = WorldGraph.Node( WorldGraph.NodeLink( i, i2).m_iDestNode );

					
					int thisDest =  WorldGraph.NodeLink( i, i2).m_iDestNode;
					if(!firstDest){
						easyForcePrint(", %d", thisDest);
					}else{
						firstDest = FALSE;
						easyForcePrint("%d", thisDest);
					}
					
					if(printBlockingEnt){
						entvars_t* pevLinkEnt = WorldGraph.NodeLink( i, i2).m_pLinkEnt;
						easyForcePrint(" blk: %s", pevLinkEnt!=NULL?STRING(pevLinkEnt->classname):"X" );
					}

				}//END OF for(int i2 = 0...)
			}

			//easyForcePrint(" m_pLinkEnt: %s", thisNode.m_pLin

			easyForcePrint("\n");
		}//END OF for(int i = 0...)
		easyForcePrintLine("*************************");



	}else if( FStrEq(pcmdRefinedRef, "forgetenemyandtarget") || FStrEq(pcmdRefinedRef, "forgettargetandenemy")){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("You may not tamper with artificial intelligence!");
			return;
		}
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){

			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon != NULL){
				testMon->m_hEnemy = NULL;
				testMon->m_hTargetEnt = NULL;
				easyForcePrintLine("Worked cleared enemy & target, on %s:%d", testMon->getClassname(), testMon->monsterID);
			}

		}
	
	}else if( FStrEq(pcmdRefinedRef, "forgettarget")){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("You may not tamper with artificial intelligence!");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){

			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon != NULL){
				testMon->m_hTargetEnt = NULL;
				easyForcePrintLine("Worked cleared enemy & target, on %s:%d", testMon->getClassname(), testMon->monsterID);
			}

		}
	
	}else if( FStrEq(pcmdRefinedRef, "forgetenemy")){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("You may not tamper with artificial intelligence!");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){

			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon != NULL){
				testMon->m_hEnemy = NULL;
				easyForcePrintLine("Worked cleared enemy & target, on %s:%d", testMon->getClassname(), testMon->monsterID);
			}

		}
	
	}else if(FStrEq(pcmdRefinedRef, "removefirefly")){

		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;
		
		//TEST!
		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024 )) != NULL)
		{
			//CBarnacle* tempBarnacle = (CBarnacle*)pEntityTemp;
			//easyForcePrintLine("FOUND ONE? %d ", pEntityTemp->pev->deadflag == DEAD_NO);
			pEntityTemp->pev->effects &= ~(EF_BRIGHTFIELD);
		}


	
	}else if(FStrEq(pcmdRefinedRef, "showboundsall")){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}


		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;

		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024 )) != NULL)
		{
			//lol.
			//pEntityTemp->IsTriggered();
			//If this entity is an NPC, draw its bounds.
			if(pEntityTemp->pev->renderfx & ISNPC){
				UTIL_drawBox(pEntityTemp->pev->origin + pEntityTemp->pev->mins, pEntityTemp->pev->origin + pEntityTemp->pev->maxs);
				UTIL_drawBox(pEntityTemp->pev->absmin, pEntityTemp->pev->absmax);
			}
		}//END OF while(things in area)

	}else if(FStrEq(pcmdRefinedRef, "showbounds")){
		
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);

		edict_t		*pentIgnore;
		TraceResult tr;
		//isn't this already "pPlayer" that we have provided here now?
		//CBasePlayer* playerRef = GetClassPtr((CBasePlayer *)pev);
		CBasePlayer* playerRef = tempplayer;
		pentIgnore = playerRef ->edict();

		Vector vecOrigSrc = pev->origin;
		Vector vecDest = vecOrigSrc + gpGlobals->v_forward * 2048;


		//nah, precision for while ducking not necessary.
		/*
		Vector playerEyePos = Vector(tempplayer->body;
		if(pev->flags & FL_DUCKING){

		}
		*/

		UTIL_TraceLine ( pev->origin + pev->view_ofs, pev->origin + pev->view_ofs + gpGlobals->v_forward * 128, ignore_monsters, ENT(pev), & tr);
		UTIL_TraceLine( playerRef->GetGunPosition(), vecDest, dont_ignore_monsters, pentIgnore, &tr );
		//tr.vecEndPos();

		/*
		if (tr.fAllSolid){
			ALERT ( at_console, "Could not determine spawn location.\n" );
			return;
		}
		tempplayer->GiveNamedItem( CMD_ARGV(1), tr.vecEndPos.x,
									tr.vecEndPos.y,
									tr.vecEndPos.z + giveLookVertCVAR->value,
									TRUE);
		*/

		CBaseEntity *hitEntity = CBaseEntity::Instance(tr.pHit);
		if(hitEntity && hitEntity->pev->renderfx & ISNPC){
			//MODDD - FOR DEBUGGING !!!
			UTIL_drawBox(hitEntity->pev->origin + hitEntity->pev->mins, hitEntity->pev->origin + hitEntity->pev->maxs);
			UTIL_printLineVector("mins", hitEntity->pev->mins);
			UTIL_printLineVector("maxs", hitEntity->pev->maxs);
			UTIL_printLineVector("absmin", hitEntity->pev->absmin);
			UTIL_printLineVector("absmax", hitEntity->pev->absmax);

			//UTIL_drawBox(hitEntity->pev->absmin, hitEntity->pev->absmax);
			
			/*
			UTIL_drawBox(hitEntity->pev->origin, hitEntity->pev->origin + hitEntity->pev->maxs);
			UTIL_drawBox(hitEntity->pev->origin, hitEntity->pev->origin + hitEntity->pev->mins);
			*/
			//UTIL_drawLine( hitEntity->pev->origin, hitEntity->pev->origin + Vector(30, 30, 30);
			/*
			if(hitEntity->pev->renderfx & ISNPC){
				CBaseMonster* attemptMonster = static_cast<CBaseMonster*>(hitEntity);
				
				//easyForcePrintLine("YES YOU FAY!!! %s %d", STRING(hitEntity->pev->classname), attemptMonster->getHasPathFindingMod() );
				attemptMonster->canDrawDebugSurface = TRUE;
			}
			*/
		}else{
			if(!hitEntity){
				easyForcePrintLine("~NPC not found.  Target NPC with crosshair and try again.");
			}else{
				easyForcePrintLine("~invalid thing targeted.  name: %s", STRING(hitEntity->pev->classname) );
			}
		}

	}else if( ::stringStartsWith(pcmdRefinedRef, "hitbox") || ::stringStartsWith(pcmdRefinedRef, "showhitbox") ){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}



		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		edict_t		*pentIgnore;
		TraceResult tr;
		CBasePlayer* playerRef = tempplayer;
		pentIgnore = playerRef ->edict();

		Vector vecOrigSrc = pev->origin;
		Vector vecDest = vecOrigSrc + gpGlobals->v_forward * 2048;

		UTIL_TraceLine ( pev->origin + pev->view_ofs, pev->origin + pev->view_ofs + gpGlobals->v_forward * 128, ignore_monsters, ENT(pev), & tr);
		UTIL_TraceLine( playerRef->GetGunPosition(), vecDest, dont_ignore_monsters, pentIgnore, &tr );
		
		CBaseEntity *hitEntity = CBaseEntity::Instance(tr.pHit);
		CBaseMonster* tempMonsterPointer = NULL;
		if(hitEntity && ((tempMonsterPointer = hitEntity->GetMonsterPointer()) != NULL) && hitEntity->pev->renderfx & ISNPC){
			//MODDD - FOR DEBUGGING !!!

			easyForcePrintLine("FOUND!");

			//check. Was a number supplied?
			const char* arg1ref = CMD_ARGV(1);
			BOOL argSupplied = FALSE;
			int numbAttempt;
			if(!isStringEmpty(arg1ref)){
				argSupplied=TRUE;
				numbAttempt = tryStringToInt(arg1ref);
			}


			if(::stringEndsWith(pcmdRefinedRef, "bone")){
				if(argSupplied){
					tempMonsterPointer->showHitboxInfoOfBone(numbAttempt);
				}else{
					easyForcePrintLine("ERROR: bone number required.");
				}
			}else if(::stringEndsWith(pcmdRefinedRef, "group")){
				if(argSupplied){
					tempMonsterPointer->showHitboxInfoOfGroup(numbAttempt);
				}else{
					easyForcePrintLine("ERROR: group number required.");
				}
			}else if(argSupplied){
				//assume it is for ID (the "nth" bone, like first, 2nd, 3rd, etc.)
				tempMonsterPointer->showHitboxInfoNumber(numbAttempt);
			}else{
				//no arg supplied? proceed.
				tempMonsterPointer->showHitboxInfoAll();
			}

		}else{
			if(!hitEntity){
				easyForcePrintLine("~NPC not found.  Target NPC with crosshair and try again.");
			}else{
				easyForcePrintLine("~invalid thing targeted.  name: %s", STRING(hitEntity->pev->classname) );
			}
		}

	}else if(FStrEq(pcmdRefinedRef, "god2")){

		if(g_flWeaponCheat != 0.0){
			if(pev->flags & FL_GODMODE){
				pev->flags &= ~FL_GODMODE;
				easyForcePrintLine("godmode OFF");
			}else{
				pev->flags |= FL_GODMODE;
				easyForcePrintLine("godmode ON");
			}
		}else{
			easyForcePrintLine("No \'God\' you cheater!");
		}

	}else if(FStrEq(pcmdRefinedRef, "noclip2")){
	
		if(g_flWeaponCheat != 0.0){

			if(pev->deadflag == DEAD_NO){
				if(pev->movetype == MOVETYPE_NOCLIP){
					pev->movetype = MOVETYPE_BOUNCE;
					easyForcePrintLine("noclip OFF");
				}else{
					pev->movetype = MOVETYPE_NOCLIP;
					easyForcePrintLine("noclip ON");
				}
			}else{
				easyForcePrintLine("\'Noclip\' is not for the dead, try \'revive\' first.");
			}
		}else{
			easyForcePrintLine("No \'Noclip\' you cheater!");
		}

	}else if(FStrEq(pcmdRefinedRef, "cameraper_f")){
		globalPSEUDO_cameraMode = 0;

	}else if(FStrEq(pcmdRefinedRef, "cameraper_t")){
		globalPSEUDO_cameraMode = 1;
	
	}else if(FStrEq(pcmdRefinedRef, "sendautofov")){
	
		const char* arg1ref = CMD_ARGV(1);
		//easyForcePrintLine("errrrrrr ???? %s", arg1ref);
		if(!isStringEmpty(arg1ref)){
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				//MODDD NOTE:  is this being a global var ok, or should it be put on the local player?  oh well.
				globalPSEUDO_aspectratio_determined_fov = numbAttempt;
			}catch(int){

			}
		}


	}else if( FStrEq(pcmdRefinedRef, "crazyprintoutoff") ){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBaseEntity *pEntity = NULL;
		while ((pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, 5000 )) != NULL)
		{
			//CBaseMonster *pMon = pEntity->MyMonsterPointer( );
			//if(pMon != NULL){
			//if(pEntity != NULL){
				pEntity->crazyPrintout = FALSE;
			//}
		}
	
	}else if(  FStrEq(pcmdRefinedRef, "crazyprintout") || FStrEq(pcmdRefinedRef, "crazyprintouton") ){

		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon == NULL){
				easyForcePrintLine("ERROR: thing in crosshairs is not a monster.  It is \"%s\".", STRING(forwardEnt->pev->classname) );
			}else{
				//got it!
				testMon->crazyPrintout = !testMon->crazyPrintout;
				
				easyForcePrintLine("WORKED!  crazyPrintout is %s", testMon->crazyPrintout == 1?"ON":"OFF");

			}
		}else{
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}


	}else if(FStrEq(pcmdRefinedRef, "forcefailtask") || FStrEq(pcmdRefinedRef, "forcetaskfail") || FStrEq(pcmdRefinedRef, "failtask") || FStrEq(pcmdRefinedRef, "taskfail") ){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("You may not tamper with artificial intelligence!");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon == NULL){
				easyForcePrintLine("ERROR: thing in crosshairs is not a monster.  It is \"%s\".", STRING(forwardEnt->pev->classname) );
			}else{
				//got it!
				testMon->TaskFail();
				easyForcePrintLine("That monster, %s:%d, just failed...", testMon->getClassname(), testMon->monsterID);

			}
		}else{
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}


	}else if(FStrEq(pcmdRefinedRef, "forcetaskcomplete") || FStrEq(pcmdRefinedRef, "forcecompletetask") || FStrEq(pcmdRefinedRef, "taskcomplete") || FStrEq(pcmdRefinedRef, "completetask") ){


		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("You may not tamper with artificial intelligence!");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon == NULL){
				easyForcePrintLine("ERROR: thing in crosshairs is not a monster.  It is \"%s\".", STRING(forwardEnt->pev->classname) );
			}else{
				//got it!
				testMon->TaskComplete();
				easyForcePrintLine("That monster, %s:%d, just completed the current task.", testMon->getClassname(), testMon->monsterID);

			}
		}else{
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}


	}else if(  FStrEq(pcmdRefinedRef, "checklocalmove") || FStrEq(pcmdRefinedRef, "testchecklocalmove") ){
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		if(arg1ref == NULL || isStringEmpty(arg1ref)){
			easyForcePrintLine("No arg!  Need a distance to go for the check...");
			return;
		}

		float dist = (float)atof(arg1ref);
		//destination for storing something?
		float distReg = 0;

		UTIL_MakeVectors(tempplayer->pev->v_angle + tempplayer->pev->punchangle);
		
		Vector vecStart = tempplayer->pev->origin + Vector(0, 0, 10);
		Vector vecEnd = vecStart + gpGlobals->v_forward * dist;
		BOOL success = tempplayer->CheckLocalMove(vecStart, vecEnd, NULL, &distReg);
		//UTIL_TraceLine(tempplayer->pev->origin + tempplayer->pev->view_ofs + gpGlobals->v_forward * 5,pMe->pev->origin + pMe->pev->view_ofs + gpGlobals->v_forward * 2048,dont_ignore_monsters, pMe->edict(), &tr );
	

		::DebugLine_ClearAll();


		float fullLength = dist;


		if(success){
			easyForcePrintLine("SUCCESS!  CLEAR!");
			::DebugLine_Setup(0, vecStart, vecEnd, (1.0f));
		}else{
			easyForcePrintLine("Stopped this far: %.2f", distReg);
			::DebugLine_Setup(0, vecStart, vecEnd, (distReg / fullLength));
		}






		







	/*
	}else if( FStrEq(pcmdRefinedRef, "debugcine1")){

		
		CBaseEntity *pEntity = NULL;
		int searchRadius = 1024;

		while ((pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, searchRadius )) != NULL)
		{

			if(FClassnameIs(pEntity->pev, "scripted_sequence") ){
				CBaseMonster* monTest = NULL;
				if( (monTest = pEntity->GetMonsterPointer()) != NULL){
					//easyForcePrintLine("HEY I EXIST:::: %d", monTest->monsterID );
					monTest->ReportAIState();
				}
			}

		}

		return;


		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		
		tempplayer->pev->flags &= ~FL_GODMODE;
		tempplayer->TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 99999, DMG_ALWAYSGIB, 0);
		return;


		while ((pEntity = UTIL_FindEntityInSphere( pEntity, pev->origin, searchRadius )) != NULL)
		{
			CSquadMonster *pRecruit = pEntity->MySquadMonsterPointer( );



			BOOL canPrint = FALSE;

			int iMyClass = 0;

			if(pRecruit){
				iMyClass = pRecruit->Classify();
			}

			if(pRecruit && pRecruit->IsAlive() && !pRecruit->m_pCine && pRecruit->Classify() == CLASS_HUMAN_MILITARY &&
				   FStringNull( pRecruit->pev->netname ) ){

				canPrint = TRUE;
				easyForcePrintLine("debugger   i: %d class: %s insq: %d count: %d leadernotnull: %d ", "-1", STRING(pRecruit->pev->classname), pRecruit->MySquadMonsterPointer()->InSquad(), pRecruit->SquadCount(), (pRecruit->MySquadLeader() != NULL) );

			}

		}


	}
	
	else if( FStrEq(pcmdRefinedRef, "debugcine2")){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL){
			CBaseMonster* testMon = forwardEnt->GetMonsterPointer();
			if(testMon == NULL){
				easyForcePrintLine("ERROR: thing in crosshairs is not a monster.  It is \"%s\".", STRING(forwardEnt->pev->classname) );
			}else{
				//got it!
				if(testMon->m_pCine!=NULL){
					tempplayer->pev->origin = testMon->m_pCine->pev->origin;
				}
			}
		}else{
			easyForcePrintLine("ERROR: Could not find an entity / monster in crosshairs.");
		}
	
	}else if( FStrEq(pcmdRefinedRef, "debugcine3")){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		const char* arg1ref = CMD_ARGV(1);

		edict_t		*pEdicttt;
		CBaseEntity *pEntityyy;


		pEdicttt = g_engfuncs.pfnPEntityOfEntIndex( 1 );
		pEntityyy;
		if ( pEdicttt ){
			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdicttt++ ){
				if ( pEdicttt->free )	// Not in use
				continue;
		
				pEntityyy = CBaseEntity::Instance(pEdicttt);
				if ( !pEntityyy )
					continue;

				CBaseMonster* tempmon = NULL;
				if(  (tempmon = pEntityyy->GetMonsterPointer()) != NULL && tempmon->m_pCine != NULL){
					UTIL_SetOrigin(tempmon->m_pCine->pev, tempplayer->pev->origin + Vector(0, 0, 6));
				}

			}//END OF loop
		}

	*/
	}else if(  FStrEq(pcmdRefinedRef, "tracehull") || FStrEq(pcmdRefinedRef, "hulltrace") ){
		
		



		//typedef enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 };
		//HULL TRACE STYLE.
		//0: point_hull: (0, 0, 0), (0, 0, 0) ?
		//human_hull: VEC_HUMAN_HULL_MIN, VEC_HUMAN_HULL_MAX  ...that is, (-16, -16, 0), (16, 16, 72)
		//large_hull: ( -32, -32, 0 ), ( 32, 32, 64 ) );
		//head_hull:  (-16, -16, 0), (16, 16, 24)   (height unverified)
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		TraceResult tr;
		

		//Vector itsAngles = (Vector(-pev->v_angle.x, pev->v_angle.y, pev->v_angle.z) + pev->punchangle);
		//UTIL_MakeVectors(itsAngles);

		UTIL_MakeVectors(tempplayer->pev->v_angle + tempplayer->pev->punchangle);

		Vector what = gpGlobals->v_forward;

		Vector vecStart = pev->origin + pev->view_ofs + what * -10;
		Vector vecEnd = pev->origin + pev->view_ofs + what * 200;

	
		const char* arg1ref = CMD_ARGV(1);

		if(arg1ref == NULL || isStringEmpty(arg1ref)){
			//easyForcePrintLine("No arg!  Need a distance to go for the check...");
			//return;
			arg1ref = "1";
		}
		
		int argHullType = atoi(arg1ref);

		
		tempplayer->debugPoint1 = vecStart;
		tempplayer->debugPoint1Given = TRUE;

		tempplayer->debugPoint2 = vecEnd;
		tempplayer->debugPoint2Given = TRUE;

		//UTIL_SetSize(tempplayer->pev, Vector(-32, -32, 0), Vector(32, 32, 64) );
		

		//typedef enum { point_hull=0, human_hull=1, large_hull=2, head_hull=3 };
		UTIL_TraceHull( vecStart, vecEnd, dont_ignore_monsters, argHullType, ENT(pev), &tr);


		
		//UTIL_MakeVectors(tempplayer->pev->v_angle + tempplayer->pev->punchangle);


		easyForcePrintLine("Fract? %.2f", tr.flFraction);
		if(tr.fAllSolid){ tr.flFraction = 0; easyForcePrintLine("SOLID"); }

		

		//'tempplayer->debugPoint3 = pev->origin + pev->view_ofs + gpGlobals->v_forward * (5) + (gpGlobals->v_forward * (200 - 5) * (1  - tr.flFraction) );
		tempplayer->debugPoint3Given = TRUE;
		tempplayer->debugPoint3 = vecStart * (1 + -tr.flFraction) + vecEnd * tr.flFraction;
		//tempplayer->debugPoint3 = pev->origin + pev->view_ofs + what * 180;






	}else if( FStrEq(pcmdRefinedRef, "getorigin") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No origin trickery for you, cheater!");
			return;
		}

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			//No.
			easyForcePrintLine("NO entity found.");
			return;
		}else{
			easyForcePrintLine("Entity found:%s. Origin:(%.2f,%.2f,%.2f)", forwardEnt->getClassname(),
				forwardEnt->pev->origin.x,
				forwardEnt->pev->origin.y,
				forwardEnt->pev->origin.z
				);
		}


	
	}else if( FStrEq(pcmdRefinedRef, "setorigin") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);
		const char* arg2ref = CMD_ARGV(2);
		const char* arg3ref = CMD_ARGV(3);

		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No origin trickery for you, cheater!");
			return;
		}

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		


		if(arg1ref == NULL || arg2ref == NULL || arg3ref == NULL){
			easyForcePrintLine("Need 3 parameters.");
			return;
		}
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt == NULL){
			//no.
			easyForcePrintLine("NO entity found.");
			return;
		}else{

			float xVal = tryStringToFloat(arg1ref);

			float yVal = tryStringToFloat(arg2ref);
			float zVal = tryStringToFloat(arg3ref);

			//forwardEnt->pev->origin = Vector(xVal, yVal, zVal);
			UTIL_SetOrigin (forwardEnt->pev, Vector(xVal, yVal, zVal) );// take him off ground so engine doesn't instantly reset onground 
			//forwardEnt->pev->flags |= FL_ONGROUND
			forwardEnt->pev->flags &= ~FL_ONGROUND;

			//easyForcePrintLine("MODEL: %s", STRING(forwardEnt->pev->model));
		}


	
	}else if( FStrEq(pcmdRefinedRef, "setmyorigin") ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);
		const char* arg2ref = CMD_ARGV(2);
		const char* arg3ref = CMD_ARGV(3);

		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("No origin trickery for you, cheater!");
			return;
		}

		//ambiguous as to whether this is what is in the crosshairs or the player itself.  Try to figure it out:
		


		if(arg1ref == NULL || arg2ref == NULL || arg3ref == NULL){
			easyForcePrintLine("Need 3 parameters.");
			return;
		}
		
		

		float xVal = tryStringToFloat(arg1ref);

		float yVal = tryStringToFloat(arg2ref);
		float zVal = tryStringToFloat(arg3ref);

		//forwardEnt->pev->origin = Vector(xVal, yVal, zVal);
		UTIL_SetOrigin (tempplayer->pev, Vector(xVal, yVal, zVal) );// take him off ground so engine doesn't instantly reset onground 
		//forwardEnt->pev->flags |= FL_ONGROUND
		tempplayer->pev->flags &= ~FL_ONGROUND;

		//easyForcePrintLine("MODEL: %s", STRING(forwardEnt->pev->model));
		


	
	}else if( FStrEq(pcmdRefinedRef, "drawpath") || FStrEq(pcmdRefinedRef, "pathdraw") || FStrEq(pcmdRefinedRef, "drawnpcpath")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hey, that\'s a debugging feature mister!");
			return;
		}

		CBaseEntity* forwardEnt = NULL;
		const char* argTest = CMD_ARGV(1);

		if(!isStringEmpty(argTest)){
			//get the monster with this ID.
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				forwardEnt = getMonsterWithID(numbAttempt);
			}catch(int){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
				return;
			}
		}else{
			//no argument provided? Try to see if there is a monster in front of the player.
			forwardEnt = FindEntityForward(tempplayer);
		}

		if(forwardEnt == NULL){
			//can't do this.
			easyForcePrintLine("***No entity found.***");
		}else{
			CBaseMonster* tempMon;
			if(  (tempMon = forwardEnt->GetMonsterPointer() ) != NULL){
				if(tempMon->drawPathConstant){
					easyForcePrintLine("***Stopped drawing path for %s ID:%d.***", tempMon->getClassname(), tempMon->monsterID);
					tempMon->drawPathConstant = FALSE;
				}else{
					easyForcePrintLine("***Drawing path for %s ID:%d.***", tempMon->getClassname(), tempMon->monsterID);
					tempMon->drawPathConstant = TRUE;
				}
			}else{
				easyForcePrintLine("***Entity \"%s\" is not a monster / NPC.***", forwardEnt->getClassname() );
			}

		}


	
	}else if( FStrEq(pcmdRefinedRef, "stopdrawpath") || FStrEq(pcmdRefinedRef, "pathdrawstop") || FStrEq(pcmdRefinedRef, "stopdrawnpcpath") || FStrEq(pcmdRefinedRef, "drawpathstop") || FStrEq(pcmdRefinedRef, "drawnpcpathstop") ){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;

		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024*6 )) != NULL)
		{
			CBaseMonster* tempMon = NULL;
			//If this entity is an NPC, draw its bounds.
			if( (tempMon = pEntityTemp->GetMonsterPointer()) != NULL  ){
				tempMon->drawPathConstant = FALSE;
			}
		}//END OF while(things in area)

	}else if( FStrEq(pcmdRefinedRef, "alldrawpath") || FStrEq(pcmdRefinedRef, "pathdrawall") || FStrEq(pcmdRefinedRef, "alldrawnpcpath") || FStrEq(pcmdRefinedRef, "drawpathall") || FStrEq(pcmdRefinedRef, "drawnpcpathall") ){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;

		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024*6 )) != NULL)
		{
			CBaseMonster* tempMon = NULL;
			//If this entity is an NPC, draw its bounds.
			if( (tempMon = pEntityTemp->GetMonsterPointer()) != NULL  ){
				tempMon->drawPathConstant = TRUE;
			}
		}//END OF while(things in area)

	}else if( FStrEq(pcmdRefinedRef, "drawfov") || FStrEq(pcmdRefinedRef, "fovdraw") || FStrEq(pcmdRefinedRef, "drawnpcfov")  ){
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *) pev);
		const char* arg1ref = CMD_ARGV(1);

		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hey, that\'s a debugging feature mister!");
			return;
		}

		CBaseEntity* forwardEnt = NULL;
		const char* argTest = CMD_ARGV(1);

		if(!isStringEmpty(argTest)){
			//get the monster with this ID.
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				forwardEnt = getMonsterWithID(numbAttempt);
			}catch(int){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
				return;
			}
		}else{
			//no argument provided? Try to see if there is a monster in front of the player.
			forwardEnt = FindEntityForward(tempplayer);
		}

		if(forwardEnt == NULL){
			//can't do this.
			easyForcePrintLine("***No entity found.***");
		}else{
			CBaseMonster* tempMon;
			if(  (tempMon = forwardEnt->GetMonsterPointer() ) != NULL){
				if(tempMon->drawFieldOfVisionConstant){
					easyForcePrintLine("***Stopped drawing FOV for %s ID:%d.***", tempMon->getClassname(), tempMon->monsterID);
					tempMon->drawFieldOfVisionConstant = FALSE;
				}else{
					easyForcePrintLine("***Drawing FOV for %s ID:%d.***", tempMon->getClassname(), tempMon->monsterID);
					tempMon->drawFieldOfVisionConstant = TRUE;
				}
			}else{
				easyForcePrintLine("***Entity \"%s\" is not a monster / NPC.***", forwardEnt->getClassname() );
			}

		}


	
	}else if( FStrEq(pcmdRefinedRef, "stopdrawfov") || FStrEq(pcmdRefinedRef, "fovdrawstop") || FStrEq(pcmdRefinedRef, "stopdrawnpcfov") || FStrEq(pcmdRefinedRef, "drawfovstop") || FStrEq(pcmdRefinedRef, "drawnpcfovstop") ){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;

		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024*6 )) != NULL)
		{
			CBaseMonster* tempMon = NULL;
			//If this entity is an NPC, draw its bounds.
			if( (tempMon = pEntityTemp->GetMonsterPointer()) != NULL  ){
				tempMon->drawFieldOfVisionConstant = FALSE;
			}
		}//END OF while(things in area)

	}else if( FStrEq(pcmdRefinedRef, "alldrawfov") || FStrEq(pcmdRefinedRef, "fovdrawall") || FStrEq(pcmdRefinedRef, "alldrawnpcfov") || FStrEq(pcmdRefinedRef, "drawfovall") || FStrEq(pcmdRefinedRef, "drawnpcfovall") ){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Hm. No.");
			return;
		}

		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity *pEntityTemp = NULL;

		while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024*6 )) != NULL)
		{
			CBaseMonster* tempMon = NULL;
			//If this entity is an NPC, draw its bounds.
			if( (tempMon = pEntityTemp->GetMonsterPointer()) != NULL  ){
				tempMon->drawFieldOfVisionConstant = TRUE;
			}
		}//END OF while(things in area)

	}else if( FStrEq(pcmdRefinedRef, "hidemyshit") || FStrEq(pcmdRefinedRef, "hideall") || FStrEq(pcmdRefinedRef, "hidefp") ){

		EASY_CVAR_SET_DEBUGONLY_CLIENTONLY(drawHUD, -1);
		EASY_CVAR_SET_DEBUGONLY_CLIENTONLY(drawViewModel, 0);
		
	}else if( FStrEq(pcmdRefinedRef, "showmyshit") || FStrEq(pcmdRefinedRef, "showall") || FStrEq(pcmdRefinedRef, "showfp") ){
		EASY_CVAR_SET_DEBUGONLY_CLIENTONLY(drawHUD, 1);
		EASY_CVAR_SET_DEBUGONLY_CLIENTONLY(drawViewModel, 1);
		
	}else if( FStrEq(pcmdRefinedRef, "setsequence") || FStrEq(pcmdRefinedRef, "sequence") || FStrEq(pcmdRefinedRef, "setanimation") ){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = NULL;
		const char* arg1ref = CMD_ARGV(1);
		int seqNumb;
		
		if(!isStringEmpty(arg1ref)){
			//get the monster with this ID.
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				//forwardEnt = getMonsterWithID(numbAttempt);
				seqNumb = numbAttempt;
			}catch(int){
				easyForcePrintLine("Problem reading number.  (arg must be whole number)");
				return;
			}
		}else{
			easyForcePrintLine("Must provide sequence number (number from 0 to max number of sequences in model.");
			return;
		}
		


		{
			//no argument provided? Try to see if there is a monster in front of the player.
			forwardEnt = FindEntityForward(tempplayer);
		}


		if(forwardEnt == NULL){
			//can't do this.
			easyForcePrintLine("***No entity found.***");
		}else{
			
			CBaseMonster* tempMon;
			if(  (tempMon = forwardEnt->GetMonsterPointer() ) != NULL){
				tempMon->SetSequenceByIndex(seqNumb, 1);
			}else{
				easyForcePrintLine("***Entity \"%s\" is not a monster / NPC.***", forwardEnt->getClassname() );
			}
			
			

		}

	}else if( FStrEq(pcmdRefinedRef, "apacherocket") || FStrEq(pcmdRefinedRef, "bigrocket") || FStrEq(pcmdRefinedRef, "boom") ){
	
		
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Mere mortals cannot summon aircraft-grade rockets!");
			return;
		}

		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		
		static float side = 1.0;
		static int count;


		Vector itsAngles = (Vector(-pev->v_angle.x, pev->v_angle.y, pev->v_angle.z) + pev->punchangle);
			
		UTIL_MakeVectors(itsAngles);
		Vector vecSrc = pev->origin + pev->view_ofs + (gpGlobals->v_forward * 8);

		/*
		switch( m_iRockets % 5)
		{
		case 0:	vecSrc = vecSrc + gpGlobals->v_right * 10; break;
		case 1: vecSrc = vecSrc - gpGlobals->v_right * 10; break;
		case 2: vecSrc = vecSrc + gpGlobals->v_up * 10; break;
		case 3: vecSrc = vecSrc - gpGlobals->v_up * 10; break;
		case 4: break;
		}
		*/

		/*
		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecSrc );
			WRITE_BYTE( TE_SMOKE );
			WRITE_COORD( vecSrc.x );
			WRITE_COORD( vecSrc.y );
			WRITE_COORD( vecSrc.z );
			WRITE_SHORT( g_sModelIndexSmoke );
			WRITE_BYTE( 20 ); // scale * 10
			WRITE_BYTE( 12 ); // framerate
		MESSAGE_END();
		*/

		CBaseEntity *pRocket = CBaseEntity::Create( "hvr_rocket", vecSrc, itsAngles, tempplayer->edict() );
		if (pRocket){
			pRocket->pev->angles = itsAngles;
			//pRocket->pev->velocity = pev->velocity + gpGlobals->v_forward * 100;
			pRocket->pev->velocity = gpGlobals->v_forward * 165;
		}

	
	
	}else if( FStrEq(pcmdRefinedRef, "moveent")){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		
		const char* arg1ref = CMD_ARGV(1);
		//const char* arg2ref = CMD_ARGV(1);

		
		if(arg1ref == NULL || isStringEmpty(arg1ref)){ //|| arg2ref == NULL || isStringEmpty(arg2ref)){
			easyForcePrintLine("Insufficient args!  Provide (ID)");
			return;
		}




		if ( tempplayer && g_flWeaponCheat != 0.0)
		{

			edict_t		*pentIgnore;
			TraceResult tr;

			pentIgnore = tempplayer->edict();
			UTIL_MakeVectors(pev->v_angle + pev->punchangle);

			//a tiny bit in front for safety.
			Vector vecSrc = pev->origin + pev->view_ofs + gpGlobals->v_forward * 5;
			Vector vecDest = pev->origin + pev->view_ofs + gpGlobals->v_forward * 2048;

			//nah, precision for while ducking not necessary.
			/*
			Vector playerEyePos = Vector(tempplayer->body;
			if(pev->flags & FL_DUCKING){

			}
			*/

			int searchID = atoi(arg1ref);


			UTIL_TraceLine( vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr );
			//tr.vecEndPos();

			if (tr.fAllSolid){
				ALERT ( at_console, "Could not determine spawn location.\n" );
				return;
			}

			/*
			tempplayer->GiveNamedItem( CMD_ARGV(1),  CMD_ARGV(2),
									 tr.vecEndPos.x,
									 tr.vecEndPos.y,
									 tr.vecEndPos.z + global_offsetgivelookvertical,
									 TRUE, &tr);
			*/


			CBaseMonster* destMon = NULL;
			
			edict_t		*pEdicttt;
			CBaseEntity *pEntityyy;
			pEdicttt = g_engfuncs.pfnPEntityOfEntIndex( 1 );
			if ( pEdicttt ){
				for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdicttt++ ){
					if ( pEdicttt->free )	// Not in use
					continue;
		
					pEntityyy = CBaseEntity::Instance(pEdicttt);
					if ( !pEntityyy )
						continue;

					CBaseMonster* tempmon = NULL;
					if(  (tempmon = pEntityyy->GetMonsterPointer()) != NULL){
						if(tempmon->monsterID == searchID){
							destMon = tempmon;
							break;
						}
					}

				}//END OF loop
			}


			if(destMon == NULL){
				easyForcePrintLine("ERROR: could not find monster of id %d", searchID);
			}else{

				//MOVE!
				//destMon->pev->origin = tempplayer->pev->origin;
				UTIL_SetOrigin(destMon->pev, tempplayer->pev->origin);
				/*
				if(destMon->m_pCine != NULL){
					destMon->m_pCine->pev->origin = tempplayer->pev->origin;
				}
				*/
			}

			
		}//END OF tempPlayer and weapon cheat checks



	}
	else if(FStrEq(pcmdRefinedRef, "myasshungers")){
		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
		CBaseEntity* forwardEnt = FindEntityForward(tempplayer);

		if(forwardEnt != NULL && FClassnameIs(forwardEnt->pev, "monster_scientist")){

			CTalkMonster* sciRef = static_cast<CTalkMonster*>(forwardEnt);
			sciRef->initiateAss();
		}

	}else if(FStrEq(pcmdRefinedRef, "forcenodeupdate")){
		if(g_flWeaponCheat == 0.0){
			easyForcePrintLine("Enable cheats first. \"scheduleNodeUpdate\" does not require cheats.");
			return;
		}


		if(WorldGraph.m_cNodes > 0){
			//Need to keep track of created nodes for calling their ->spawn's.
			int nodeCount = WorldGraph.m_cNodes;
			CNodeEnt** aryNodeEnt = new CNodeEnt*[nodeCount];

			

			//Build node ent's from the existing nodes on the map to rebuild the nodes and connections.
			for(int i = 0; i < nodeCount; i++){
				const char* spawnName;
				if(WorldGraph.m_pNodes[i].m_afNodeInfo & bits_NODE_AIR){
					spawnName = "info_node_air";
				}else{
					spawnName = "info_node";
				}

				//"CreateManual" instead of "Create" skips the DispatchSpawn call.
				//Not that it may even be necessary at all for node entities.  Calling ->spawn straight should do it.
				CNodeEnt* thisNode = static_cast<CNodeEnt*>(CBaseEntity::CreateManual(
					spawnName,
					WorldGraph.m_pNodes[ i ].m_vecOrigin,
					Vector(0, WorldGraph.m_pNodes[ i ].m_flHintYaw, 0)
				));

				aryNodeEnt[i] = thisNode;
			
				thisNode->m_sHintType = WorldGraph.m_pNodes[ i ].m_sHintType;
				thisNode->m_sHintActivity = WorldGraph.m_pNodes[ i ].m_sHintActivity;

			}//END OF for

			

			//Clear the graph to signify it needs rebuilding.
			WorldGraph.InitGraph();
			WorldGraph.AllocNodes();
			
			//Now call each created node's Spawn.
			for(int i = 0; i < nodeCount; i++){
				aryNodeEnt[i]->Spawn();
			}
			
			

			delete[] aryNodeEnt;


			easyForcePrintLine("Nodes rebuilding...");
		}else{
			easyForcePrintLine("ERROR: no nodes present or building in progress.");
		}

	}else if(FStrEq(pcmdRefinedRef, "schedulenodeupdate")) {
		
		//the next map loaded will enforce regenerating the node graph regardless of coming from a file already or not.
		_scheduleNodeUpdate = TRUE;
		easyForcePrintLine("Scheduling node update. Start a map to rebuild nodes / skip node file.");

	}else if ( FStrEq(pcmdRefinedRef, "debug1" ) )
	{
		//YEAH
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		tempplayer->DebugCall1();
	}
	else if ( FStrEq(pcmdRefinedRef, "debug2" ) )
	{
		//YEAH
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		tempplayer->DebugCall2();
	}
	else if ( FStrEq(pcmdRefinedRef, "debug3" ) )
	{
		//YEAH
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		tempplayer->DebugCall3();
	}else if(  FStrEq(pcmdRefinedRef, "getnormalvector") || FStrEq(pcmdRefinedRef, "getnormal") || FStrEq(pcmdRefinedRef, "normalvector") || FStrEq(pcmdRefinedRef, "normal")  ){

		
		CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev) ;
		


		if ( tempplayer )
		{
			if(g_flWeaponCheat != 0.0){
				edict_t		*pentIgnore;
				TraceResult tr;

				pentIgnore = tempplayer->edict();
				UTIL_MakeVectors(pev->v_angle + pev->punchangle);

				//a tiny bit in front for safety.
				Vector vecSrc = pev->origin + pev->view_ofs + gpGlobals->v_forward * 5;
				Vector vecDest = pev->origin + pev->view_ofs + gpGlobals->v_forward * 2048;

				//nah, precision for while ducking not necessary.
				/*
				Vector playerEyePos = Vector(tempplayer->body;
				if(pev->flags & FL_DUCKING){

				}
				*/


				UTIL_TraceLine( vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr );
				//tr.vecEndPos();



				CBaseEntity* pEntityHit;
				if (!tr.fAllSolid  ){

					if(tr.flFraction < 1.0){

						CBaseEntity* pEntityHit = CBaseEntity::Instance(tr.pHit);

						easyForcePrintLine("Name:%s HitLoc:(%.2f, %.2f, %.2f) Normal:(%.2f, %.2f, %.2f)",
							(pEntityHit!=NULL?pEntityHit->getClassnameShort():"NULL"),
							tr.vecEndPos.x,
							tr.vecEndPos.y,
							tr.vecEndPos.z,
							tr.vecPlaneNormal.x,
							tr.vecPlaneNormal.y,
							tr.vecPlaneNormal.z
						);

					}else{
						easyForcePrintLine("WARNING: trace failed; flFraction is 1.0 (did not hit anything).");
					}

				}//END OF line trace valid check
				else{
					easyForcePrintLine("WARNING: trace failed; AllSolid.");
				}

			}else{
				easyForcePrintLine("Enable cheats to use getNormalVector.");
			}

		}//END OF player and cheat check






	}else if( FStrEq(pcmdRefinedRef, "chillbro" ) || FStrEq(pcmdRefinedRef, "chill" ) || FStrEq(pcmdRefinedRef, "relax" ) || FStrEq(pcmdRefinedRef, "relaxbuddy" ) || FStrEq(pcmdRefinedRef, "smokeweed" ) || FStrEq(pcmdRefinedRef, "fageddaboutit" ) || FStrEq(pcmdRefinedRef, "forgetaboutit" ) || FStrEq(pcmdRefinedRef, "thesearenothtedroidsyouarelookingfor" ) || FStrEq(pcmdRefinedRef, "jedimindtrick" ) ){
		
		if ( g_flWeaponCheat){
			CBasePlayer* tempplayer = GetClassPtr((CBasePlayer *)pev);
			CBaseEntity *pEntityTemp = NULL;

			while ((pEntityTemp = UTIL_FindEntityInSphere( pEntityTemp, tempplayer->pev->origin, 1024*2 )) != NULL)
			{
				CBaseMonster* monsterTest = pEntityTemp->GetMonsterPointer();
				if(monsterTest != NULL){
					monsterTest->m_afMemory &= ~(bits_MEMORY_SUSPICIOUS | bits_MEMORY_PROVOKED);
					monsterTest->TaskFail();
					monsterTest->m_hEnemy = NULL;
					monsterTest->m_hTargetEnt = NULL;
				}
			}//END OF while(things in area)
		}
		else{
			easyForcePrintLine("Captain Retrospect says: You shouldn\'t have pissed them off.\n");
		}
	}else if( FStrEq(pcmdRefinedRef, "removeallmonsters" )  ){
		
		if ( g_flWeaponCheat){
			edict_t		*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );
			CBaseEntity *pEntity;
			int			count;
			float		distance, delta;
			count = 0;
			if ( !pEdict )
				return;
			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
			{
				if ( pEdict->free )	// Not in use
					continue;

				//TEST WHY NO REMOV
				if(FClassnameIs(pEdict, "monster_barnacle")){
					int x = 66; //?
				}

				if ( !(pEdict->v.flags & (FL_CLIENT|FL_MONSTER)) )	// Not a client/monster ?
					continue;

				pEntity = CBaseEntity::Instance(pEdict);
				if ( !pEntity )
					continue;

				CBaseMonster* tempMonster = pEntity->MyMonsterPointer();
				if(tempMonster == NULL || FClassnameIs(tempMonster->pev, "player")){
					continue;  //not players or non-monsters.
				}



				//MODDD - 
				//if(/*tempMonster->monsterID == 4 ||*/ tempMonster->monsterID == 5 || tempMonster->monsterID == 6){
				//	//remove all but those! TESTING
				//	continue;
				//}


				easyForcePrintLine("*REMOVED %s", tempMonster->getClassname(), tempMonster->monsterID);
				//made it here? Remove it.
				//::UTIL_Remove(tempMonster);
				//tempMonster->onDelete();   automatically called by SUB_REMOVE, don't manually call this.
				tempMonster->SetThink(&CBaseEntity::SUB_Remove);
				tempMonster->pev->nextthink = gpGlobals->time;
			}//END OF list through all entities.

		}
		else{
			easyForcePrintLine("Nope.");
		}
	}else if( FStrEq(pcmdRefinedRef, "removeallentities" )  ){
		
		easyForcePrintLine("Sorry, too dangerous with or without cheats. Bye.");
		return;

		if ( g_flWeaponCheat){
			edict_t		*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );
			CBaseEntity *pEntity;
			int			count;
			float		distance, delta;
			count = 0;
			if ( !pEdict )
				return;
			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
			{
				if ( pEdict->free )	// Not in use
					continue;

				pEntity = CBaseEntity::Instance(pEdict);
				if ( !pEntity )
					continue;
				
				easyForcePrintLine("WHAT WERE YOU GONNA DELETE?? %s", pEntity->getClassname());

				if(FClassnameIs(pEntity->pev, "worldspawn") || FClassnameIs(pEntity->pev, "player")){
					continue;  //not the map (???) or players.
				}

				//made it here? Remove it.
				////::UTIL_Remove(tempMonster);
				//actually do it this way below instead.
				//pEntity->onDelete();
				//pEntity->SetThink(&CBaseEntity::SUB_Remove);
				//pEntity->pev->nextthink = gpGlobals->time;

			}//END OF list through all entities.

		}
		else{
			easyForcePrintLine("Nope.");
		}
	}else if( FStrEq(pcmdRefinedRef, "test")){

		
		MESSAGE_BEGIN( MSG_ONE, gmsgCliTest, NULL, pev );
		MESSAGE_END();

	}else if ( FStrEq(pcmdRefinedRef, "fov" ) )
	{
		if ( g_flWeaponCheat && CMD_ARGC() > 1)
		{
			GetClassPtr((CBasePlayer *)pev)->m_iFOV = atoi( CMD_ARGV(1) );
		}
		else
		{
			CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );
		}
	}
	else if ( FStrEq(pcmdRefinedRef, "use" ) )
	{
		GetClassPtr((CBasePlayer *)pev)->SelectItem((char *)CMD_ARGV(1));
	}
	else if (((pstr = strstr(pcmdRefinedRef, "weapon_")) != NULL)  && (pstr == pcmdRefinedRef))
	{
		//easyForcePrintLine("HOW THE %s", pcmdRefinedRef);
		GetClassPtr((CBasePlayer *)pev)->SelectItem(pcmd);
	}
	else if (FStrEq(pcmdRefinedRef, "lastinv" ))
	{
		GetClassPtr((CBasePlayer *)pev)->SelectLastItem();
	}
	else if ( FStrEq( pcmdRefinedRef, "spectate" ) && (pev->flags & FL_PROXY) )	// added for proxy support
	{
		CBasePlayer * pPlayer = GetClassPtr((CBasePlayer *)pev);

		edict_t *pentSpawnSpot = g_pGameRules->GetPlayerSpawnSpot( pPlayer );
		pPlayer->StartObserver( pev->origin, VARS(pentSpawnSpot)->angles);
	}
	//NOTE: sending the raw ,non-forced-lowercase string.  Hard to say what this "gameRules" wants it to do.
	else if ( g_pGameRules->ClientCommand( GetClassPtr((CBasePlayer *)pev), pcmd ) )
	{

		//easyForcePrintLine("AWWWWW SNAP SON????");

		// MenuSelect returns true only if the command is properly handled,  so don't print a warning
	}
	else
	{

		caughtByFirst = FALSE;

		//script moveed....
	}




	if(caughtByFirst){
		return;   //skip the hidden CVar stuff below, no need for extra interpretation.
	}

	

	
	EASY_CVAR_HIDDEN_LIST


		//NOTICE - so much as reaching here says that the hidden check didn't find anything. If it did, it would've
		//terminated this method early and never reached here.

		// tell the user they entered an unknown command
		char command[128];

		// check the length of the command (prevents crash)
		// max total length is 192 ...and we're adding a string below ("Unknown command: %s\n")
		strncpy( command, pcmd, 127 );
		command[127] = '\0';

		// tell the user they entered an unknown command
		ClientPrint( &pEntity->v, HUD_PRINTCONSOLE, UTIL_VarArgs( "Unknown command: %s\n", command ) );


	
		
	//Proven true by finding the player-entered string to be a reference to a hidden CVar to stop further checks.
	//BOOL hiddenCVarHandled = FALSE;






	/*
	if(FALSE){
	}
	//MODDD - insertion
	EASY_CVAR_HIDDEN_LIST
	//not here. we may still have another possible link below.


	if(hiddenCVarHandled)return;  //stop, we're done.
	

	if(FALSE){
	}
	EASY_CVAR_HIDDEN_LIST_2
	else{
		// tell the user they entered an unknown command
		char command[128];

		// check the length of the command (prevents crash)
		// max total length is 192 ...and we're adding a string below ("Unknown command: %s\n")
		strncpy( command, pcmd, 127 );
		command[127] = '\0';

		// tell the user they entered an unknown command
		ClientPrint( &pEntity->v, HUD_PRINTCONSOLE, UTIL_VarArgs( "Unknown command: %s\n", command ) );
	}
	*/




}//END OF... something really big.


/*
========================
ClientUserInfoChanged

called after the player changes
userinfo - gives dll a chance to modify it before
it gets sent into the rest of the engine.
========================
*/
void ClientUserInfoChanged( edict_t *pEntity, char *infobuffer )
{
	// Is the client spawned yet?
	if ( !pEntity->pvPrivateData )
		return;

	// msg everyone if someone changes their name,  and it isn't the first time (changing no name to current name)
	if ( pEntity->v.netname && STRING(pEntity->v.netname)[0] != 0 && !FStrEq( STRING(pEntity->v.netname), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" )) )
	{
		char sName[256];
		char *pName = g_engfuncs.pfnInfoKeyValue( infobuffer, "name" );
		strncpy( sName, pName, sizeof(sName) - 1 );
		sName[ sizeof(sName) - 1 ] = '\0';

		// First parse the name and remove any %'s
		for ( char *pApersand = sName; pApersand != NULL && *pApersand != 0; pApersand++ )
		{
			// Replace it with a space
			if ( *pApersand == '%' )
				*pApersand = ' ';
		}

		// Set the name
		g_engfuncs.pfnSetClientKeyValue( ENTINDEX(pEntity), infobuffer, "name", sName );

		char text[256];
		sprintf( text, "* %s changed name to %s\n", STRING(pEntity->v.netname), g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		MESSAGE_BEGIN( MSG_ALL, gmsgSayText, NULL );
			WRITE_BYTE( ENTINDEX(pEntity) );
			WRITE_STRING( text );
		MESSAGE_END();

		// team match?
		if ( g_teamplay )
		{
			UTIL_LogPrintf( "\"%s<%i><%s><%s>\" changed name to \"%s\"\n", 
				STRING( pEntity->v.netname ), 
				GETPLAYERUSERID( pEntity ), 
				GETPLAYERAUTHID( pEntity ),
				g_engfuncs.pfnInfoKeyValue( infobuffer, "model" ), 
				g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		}
		else
		{
			UTIL_LogPrintf( "\"%s<%i><%s><%i>\" changed name to \"%s\"\n", 
				STRING( pEntity->v.netname ), 
				GETPLAYERUSERID( pEntity ), 
				GETPLAYERAUTHID( pEntity ),
				GETPLAYERUSERID( pEntity ), 
				g_engfuncs.pfnInfoKeyValue( infobuffer, "name" ) );
		}
	}

	g_pGameRules->ClientUserInfoChanged( GetClassPtr((CBasePlayer *)&pEntity->v), infobuffer );
}

static int g_serveractive = 0;

void ServerDeactivate( void )
{
	// It's possible that the engine will call this function more times than is necessary
	//  Therefore, only run it one time for each call to ServerActivate 
	if ( g_serveractive != 1 )
	{
		return;
	}

	g_serveractive = 0;

	// Peform any shutdown operations here...
	//
}



void testcommand(){
	easyForcePrintLine("HA HA printout here.");
}


void ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	int				i;
	CBaseEntity		*pClass;



	globalPSEUDO_queueClientSendoff = TRUE;


	//can't really involve the calling player? That makes a lot of commands here less useful unfortunately. Oh well, they are debug features.
	//g_engfuncs.pfnAddServerCommand("testcommand", &testcommand );


	//MODDD
	//updateCVarRefs(NULL);
	//UPDATE: "GameDLLInit" method of game.cpp deemed the earliest point best for initial CVar update.

	// Every call to ServerActivate should be matched by a call to ServerDeactivate
	g_serveractive = 1;

	// Clients have not been initialized yet
	for ( i = 0; i < edictCount; i++ )
	{
		if ( pEdictList[i].free )
			continue;
		
		// Clients aren't necessarily initialized until ClientPutInServer()
		if ( i < clientMax || !pEdictList[i].pvPrivateData )
			continue;

		pClass = CBaseEntity::Instance( &pEdictList[i] );
		// Activate this entity if it's got a class & isn't dormant
		if ( pClass && !(pClass->pev->flags & FL_DORMANT) )
		{
			pClass->Activate();
		}
		else
		{
			ALERT( at_console, "Can't instance %s\n", STRING(pEdictList[i].v.classname) );
		}
	}

	// Link user messages here to make sure first client can get them...
	LinkUserMessages();


	//guaranteed to happen well after the last precache... or activate call reallly.
	OnMapLoadEnd();


	//..When does the game load gpGlobals->time from a save file exactly, from what it was at the time of save? No clue.
}//END OF ServerActivate


/*
================
PlayerPreThink

Called every frame before physics are run
================
*/





//Looks like "StartFrame", "PlayerPreThink", and "PlayerPostThink" are always called in the same order.
//We can take advantage of this and just 


void PlayerPreThink( edict_t *pEntity )
{
	

	//typical non-paused delta:
	//d:0.01669312

	//MODDD NOTE:
	//HOO BOY is this hacky as Hell. But it works. Tell if the game is paused or not.
	//For reasons that remain unexplained by modern Science, the current time and previousFrameTime will vary by a very small
	//amount even while paused and nothing else happens. Some tiny ammount gets added / subtracted. No clue.
	
	//gamePaused = ((gpGlobals->time - previousFrameTime) <= 0.005);
	//easyForcePrintLine("WHAT THE PUCKIN PUCK %.8f %.8f d:%.8f %d", gpGlobals->time, previousFrameTime, (gpGlobals->time - previousFrameTime), gamePaused);
	//previousFrameTime = gpGlobals->time;


	//easyForcePrintLine("??A %d", playerCanThink1);

	if(!playerCanThink1){
		//If somewhere want to know, "gamePaused".
		gamePaused = TRUE;

		//This client message call has been cut. Turns out serverside can issue an unpause order just fine.
		//	submitUnpauseRequest(&pEntity->v);
		
		if(!g_pGameRules->IsMultiplayer() && EASY_CVAR_GET(disablePauseSinglePlayer) ){
			SERVER_COMMAND("unpause\n");
		}
		//easyForcePrintLine("!!! UNPAUSE REQUEST");
		return;
	}

	//must be turned back on by StartFrame.
	playerCanThink1 = FALSE;

	gamePaused = FALSE;


	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);

	if (pPlayer)  //already factored in above; the whole method ends further above if "pPlayer" is null.
	pPlayer->PreThink( );
}

/*
================
PlayerPostThink

Called every frame after physics are run
================
*/


BOOL queueYMG_stopSend = FALSE;

void PlayerPostThink( edict_t *pEntity )
{
	
	//easyForcePrintLine("??B %d", playerCanThink2);
	if(!playerCanThink2){
		return;
	}
	//must be turned back on by StartFrame.
	playerCanThink2 = FALSE;


	//For organization's sake, we're going to not involve things that have nothing to do with the sent "pEntity" up there.
	
	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);

	//update CVar references continually, of course.

	//&pPlayer->pev->origin
	if(pPlayer != NULL){
		updateCVarRefs( pev );
	}else{
		updateCVarRefs ( NULL );
	}


	if(queueYMG_stopSend == TRUE){
		queueYMG_stopSend = FALSE;
		message_ymg_stop(ENT(pev));
	}



	if(playQueued && pPlayer != NULL && playQueuedTime <= gpGlobals->time){
		edict_t* tempEd = ENT(pPlayer->pev);
				
		
		//EMIT_SOUND_DYN( edict(), CHAN_VOICE, pszSentence, volume, attenuation, 0, GetVoicePitch());
		//easyForcePrintLine("??___?? %s", queuedSound);
		//or use temped there???
		//no, wait to start playing soon.

		//...why the HELL Does what channel is picked have a difference on how sounds work?  This is just weird.
		//-was CHAN_STREAM before.
		EMIT_SOUND_DYN(pPlayer->edict(), CHAN_VOICE, queuedSound, 1, ATTN_NORM, 0, 100);
		
		//called already.
		playQueued = FALSE;
	}



	if (pPlayer)
		pPlayer->PostThink( );


	if(!pPlayer){
		//The next line would've been skipped anyways.  Cheat logic need not apply if there is no "player" either.
		return;
	}

	
	
	
#if defined( CLIENT_WEAPONS )

	//iterate through the player's weapons.
	for ( int i = 0 ; i < MAX_ITEM_TYPES ; i++ ){
		if ( pPlayer->m_rgpPlayerItems[ i ] ){


			//
			//CBasePlayerItem *pPlayerItem = pPlayer->m_rgpPlayerItems[ i ];

			//while ( pPlayerItem ){
			//	CBasePlayerWeapon *gun;
			//	gun = (CBasePlayerWeapon *)pPlayerItem->GetWeaponPtr();

			//	//Essentially, if this "item" exists and is a "weapon":
			//	if(gun){
			
			//		/*
			//		if(clientCheat_infiniteclip->value == 1){
			//			//all weapons must have at least "1" in the clip for convenience.
			//			if(gun->m_iClip <= 0){
			//				gun->m_iClip = 1;
			//			}
			//			//Secondary tends not to have clips, just assume they're meant too then.
			//			if(gun->m_iSecondaryAmmoType > 0 && pPlayer->m_rgAmmo[gun->m_iSecondaryAmmoType] == 0){
			//				pPlayer->m_rgAmmo[gun->m_iSecondaryAmmoType] = 1;
			//			}
			//		}
			//		*/
			//		/*
			//		if(clientCheat_infiniteclip->value == 1){
			//			//all weapons must have at least "1" in the total ammo-count for convenience.
			//			if(gun->m_iPrimaryAmmoType > 0 && pPlayer->m_rgAmmo[gun->m_iPrimaryAmmoType] == 0){
			//				pPlayer->m_rgAmmo[gun->m_iPrimaryAmmoType] = 1;
			//			}
			//			//Secondary too, if available.
			//			if(gun->m_iSecondaryAmmoType > 0 && pPlayer->m_rgAmmo[gun->m_iSecondaryAmmoType] == 0){
			//				pPlayer->m_rgAmmo[gun->m_iSecondaryAmmoType] = 1;
			//			}
			//		}*/
			//				

			//	}//END OF if(gun)
			//}//END OF while (this particular chained item exists)
			//pPlayerItem = pPlayerItem->m_pNext;
		}//END OF if( this player's particular item exists)

	}//END OF for each item in the player's inventory

#endif
	














}




//Completely unused (not just empty, never even called in the first place). Tried going to a new map, transition, loading a game... this never fires.
void ParmsNewLevel( void )
{

}




//This gets called while changing levels at transitions or loading a saved game, but not on starting a map by console's "map x" or "changelevel x".
//During a level transition, this gets called once before "OnMapLoadStart", and then twice after. Why?
//~"OnBeforeChangeLevelTransition" is still called before either this or OnMapLoadStart.
//While loading a save game, this gets called after "OnMapLoadStart".
void ParmsChangeLevel( void )
{
	// retrieve the pointer to the save data
	SAVERESTOREDATA *pSaveData = (SAVERESTOREDATA *)gpGlobals->pSaveData;

	if ( pSaveData )
		pSaveData->connectionCount = BuildChangeList( pSaveData->levelList, MAX_LEVEL_CONNECTIONS );
}


//
// GLOBALS ASSUMED SET:  g_ulFrameCount
//








void StartFrame( void )
{
	
	//easyForcePrintLine("!!");
	//let the player think since there is a frame of logic this time around.
	playerCanThink1 = TRUE;
	playerCanThink2 = TRUE;

	//if(EASY_CVAR_GET(testVar) == 1)easyForcePrintLine("WHAT THE hay %.2f %.2f", gpGlobals->time, DebugLine_drawTime);
	if(gpGlobals->time >= DebugLine_drawTime){

		//::debugLine_setup(1, 
		//::debugLine_setupFract(0, 118, 759, 38, 118, 759, 600, abs(sin(gpGlobals->time*0.8)) );
		DebugLine_RenderAll();
		//::UTIL_drawLineFrame(118.48, 759.76, 37.03, 500, 500, 500, 12, 255, 0, 0);
		
		DebugLine_drawTime = gpGlobals->time + 0.09;
	}
	

	//MODDD - should "updateCVarRefs" just be done here to guarantee it just runs once for all players per frame? Unsure.




	if ( g_pGameRules )
		g_pGameRules->Think();

	if ( g_fGameOver )
		return;

	gpGlobals->teamplay = teamplay.value;
	g_ulFrameCount++;
}


extern int global_useSentenceSave;

/*
===============
GetGameDescription

Returns the descriptive name of this .dll.  E.g., Half-Life, or Team Fortress 2
===============
*/
const char *GetGameDescription()
{
	if ( g_pGameRules ) // this function may be called before the world has spawned, and the game rules initialized
		return g_pGameRules->GetGameDescription();
	else
		return "Half-Life";
}

/*
================
Sys_Error

Engine is going to shut down, allows setting a breakpoint in game .dll to catch that occasion
================
*/
void Sys_Error( const char *error_string )
{
	// Default case, do nothing.  MOD AUTHORS:  Add code ( e.g., _asm { int 3 }; here to cause a breakpoint for debugging your game .dlls
}

/*
================
PlayerCustomization

A new player customization has been registered on the server
UNDONE:  This only sets the # of frames of the spray can logo
animation right now.
================
*/
void PlayerCustomization( edict_t *pEntity, customization_t *pCust )
{
	entvars_t *pev = &pEntity->v;
	CBasePlayer *pPlayer = (CBasePlayer *)GET_PRIVATE(pEntity);

	if (!pPlayer)
	{
		ALERT(at_console, "PlayerCustomization:  Couldn't get player!\n");
		return;
	}

	if (!pCust)
	{
		ALERT(at_console, "PlayerCustomization:  NULL customization!\n");
		return;
	}

	switch (pCust->resource.type)
	{
	case t_decal:
		pPlayer->SetCustomDecalFrames(pCust->nUserData2); // Second int is max # of frames.
		break;
	case t_sound:
	case t_skin:
	case t_model:
		// Ignore for now.
		break;
	default:
		ALERT(at_console, "PlayerCustomization:  Unknown customization type!\n");
		break;
	}
}

/*
================
SpectatorConnect

A spectator has joined the game
================
*/
void SpectatorConnect( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorConnect( );
}

/*
================
SpectatorConnect

A spectator has left the game
================
*/
void SpectatorDisconnect( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorDisconnect( );
}

/*
================
SpectatorConnect

A spectator has sent a usercmd
================
*/
void SpectatorThink( edict_t *pEntity )
{
	entvars_t *pev = &pEntity->v;
	CBaseSpectator *pPlayer = (CBaseSpectator *)GET_PRIVATE(pEntity);

	if (pPlayer)
		pPlayer->SpectatorThink( );
}

////////////////////////////////////////////////////////
// PAS and PVS routines for client messaging
//

/*
================
SetupVisibility

A client can have a separate "view entity" indicating that his/her view should depend on the origin of that
view entity.  If that's the case, then pViewEntity will be non-NULL and will be used.  Otherwise, the current
entity's origin is used.  Either is offset by the view_ofs to get the eye position.

From the eye position, we set up the PAS and PVS to use for filtering network messages to the client.  At this point, we could
 override the actual PAS or PVS values, or use a different origin.

NOTE:  Do not cache the values of pas and pvs, as they depend on reusable memory in the engine, they are only good for this one frame
================
*/
void SetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char **pvs, unsigned char **pas )
{
	Vector org;
	edict_t *pView = pClient;

	// Find the client's PVS
	if ( pViewEntity )
	{
		pView = pViewEntity;
	}

	if ( pClient->v.flags & FL_PROXY )
	{
		*pvs = NULL;	// the spectator proxy sees
		*pas = NULL;	// and hears everything
		return;
	}

	org = pView->v.origin + pView->v.view_ofs;
	if ( pView->v.flags & FL_DUCKING )
	{
		org = org + ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	}

	*pvs = ENGINE_SET_PVS ( (float *)&org );
	*pas = ENGINE_SET_PAS ( (float *)&org );
}

//old entity_state.h include location

/*
AddToFullPack

Return 1 if the entity state has been filled in for the ent and the entity will be propagated to the client, 0 otherwise

state is the server maintained copy of the state info that is transmitted to the client
a MOD could alter values copied into state to send the "host" a different look for a particular entity update, etc.
e and ent are the entity that is being added to the update, if 1 is returned
host is the player's edict of the player whom we are sending the update to
player is 1 if the ent/e is a player and 0 otherwise
pSet is either the PAS or PVS that we previous set up.  We can use it to ask the engine to filter the entity against the PAS or PVS.
we could also use the pas/ pvs that we set in SetupVisibility, if we wanted to.  Caching the value is valid in that case, but still only for the current frame
*/
int AddToFullPack( struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet )
{
	int					i;

	// don't send if flagged for NODRAW and it's not the host getting the message
	if ( ( ent->v.effects == EF_NODRAW ) &&
		 ( ent != host ) )
		return 0;

	// Ignore ents without valid / visible models
	if ( !ent->v.modelindex || !STRING( ent->v.model ) )
		return 0;

	// Don't send spectators to other players
	if ( ( ent->v.flags & FL_SPECTATOR ) && ( ent != host ) )
	{
		return 0;
	}

	// Ignore if not the host and not touching a PVS/PAS leaf
	// If pSet is NULL, then the test will always succeed and the entity will be added to the update
	if ( ent != host )
	{
		if ( !ENGINE_CHECK_VISIBILITY( (const struct edict_s *)ent, pSet ) )
		{
			return 0;
		}
	}


	// Don't send entity to local client if the client says it's predicting the entity itself.
	if ( ent->v.flags & FL_SKIPLOCALHOST )
	{
		if ( ( hostflags & 1 ) && ( ent->v.owner == host ) )
			return 0;
	}
	
	if ( host->v.groupinfo )
	{
		UTIL_SetGroupTrace( host->v.groupinfo, GROUP_OP_AND );

		// Should always be set, of course
		if ( ent->v.groupinfo )
		{
			if ( g_groupop == GROUP_OP_AND )
			{
				if ( !(ent->v.groupinfo & host->v.groupinfo ) )
					return 0;
			}
			else if ( g_groupop == GROUP_OP_NAND )
			{
				if ( ent->v.groupinfo & host->v.groupinfo )
					return 0;
			}
		}

		UTIL_UnsetGroupTrace();
	}

	memset( state, 0, sizeof( *state ) );

	// Assign index so we can track this entity from frame to frame and
	//  delta from it.
	state->number	  = e;
	state->entityType = ENTITY_NORMAL;
	
	// Flag custom entities.
	if ( ent->v.flags & FL_CUSTOMENTITY )
	{
		state->entityType = ENTITY_BEAM;
	}

	// 
	// Copy state data
	//

	// Round animtime to nearest millisecond
	state->animtime   = (int)(1000.0 * ent->v.animtime ) / 1000.0;

	memcpy( state->origin, ent->v.origin, 3 * sizeof( float ) );
	memcpy( state->angles, ent->v.angles, 3 * sizeof( float ) );
	memcpy( state->mins, ent->v.mins, 3 * sizeof( float ) );
	memcpy( state->maxs, ent->v.maxs, 3 * sizeof( float ) );

	memcpy( state->startpos, ent->v.startpos, 3 * sizeof( float ) );
	memcpy( state->endpos, ent->v.endpos, 3 * sizeof( float ) );

	state->impacttime = ent->v.impacttime;
	state->starttime = ent->v.starttime;

	state->modelindex = ent->v.modelindex;
		
	state->frame      = ent->v.frame;

	state->skin       = ent->v.skin;
	state->effects    = ent->v.effects;

	// This non-player entity is being moved by the game .dll and not the physics simulation system
	//  make sure that we interpolate it's position on the client if it moves
	if ( !player &&
		 ent->v.animtime &&
		 ent->v.velocity[ 0 ] == 0 && 
		 ent->v.velocity[ 1 ] == 0 && 
		 ent->v.velocity[ 2 ] == 0 )
	{
		state->eflags |= EFLAG_SLERP;
	}

	state->scale	  = ent->v.scale;
	state->solid	  = ent->v.solid;
	state->colormap   = ent->v.colormap;

	state->movetype   = ent->v.movetype;
	state->sequence   = ent->v.sequence;
	state->framerate  = ent->v.framerate;
	state->body       = ent->v.body;

	for (i = 0; i < 4; i++)
	{
		state->controller[i] = ent->v.controller[i];
	}

	for (i = 0; i < 2; i++)
	{
		state->blending[i]   = ent->v.blending[i];
	}

	state->rendermode    = ent->v.rendermode;
	state->renderamt     = ent->v.renderamt; 
	state->renderfx      = ent->v.renderfx;
	state->rendercolor.r = ent->v.rendercolor.x;
	state->rendercolor.g = ent->v.rendercolor.y;
	state->rendercolor.b = ent->v.rendercolor.z;

	state->aiment = 0;
	if ( ent->v.aiment )
	{
		state->aiment = ENTINDEX( ent->v.aiment );
	}

	state->owner = 0;
	if ( ent->v.owner )
	{
		int owner = ENTINDEX( ent->v.owner );
		
		// Only care if owned by a player
		if ( owner >= 1 && owner <= gpGlobals->maxClients )
		{
			state->owner = owner;	
		}
	}

	// HACK:  Somewhat...
	// Class is overridden for non-players to signify a breakable glass object ( sort of a class? )
	if ( !player )
	{
		state->playerclass  = ent->v.playerclass;
	}

	// Special stuff for players only
	if ( player )
	{
		memcpy( state->basevelocity, ent->v.basevelocity, 3 * sizeof( float ) );

		state->weaponmodel  = MODEL_INDEX( STRING( ent->v.weaponmodel ) );
		state->gaitsequence = ent->v.gaitsequence;
		state->spectator = ent->v.flags & FL_SPECTATOR;
		state->friction     = ent->v.friction;

		state->gravity      = ent->v.gravity;
//		state->team			= ent->v.team;
//		
		state->usehull      = ( ent->v.flags & FL_DUCKING ) ? 1 : 0;
		state->health		= ent->v.health;
	}

	return 1;
}

// defaults for clientinfo messages
#define	DEFAULT_VIEWHEIGHT	28

/*
===================
CreateBaseline

Creates baselines used for network encoding, especially for player data since players are not spawned until connect time.
===================
*/
void CreateBaseline( int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs )
{
	baseline->origin		= entity->v.origin;
	baseline->angles		= entity->v.angles;
	baseline->frame			= entity->v.frame;
	baseline->skin			= (short)entity->v.skin;

	// render information
	baseline->rendermode	= (byte)entity->v.rendermode;
	baseline->renderamt		= (byte)entity->v.renderamt;
	baseline->rendercolor.r	= (byte)entity->v.rendercolor.x;
	baseline->rendercolor.g	= (byte)entity->v.rendercolor.y;
	baseline->rendercolor.b	= (byte)entity->v.rendercolor.z;
	baseline->renderfx		= (byte)entity->v.renderfx;

	if ( player )
	{
		baseline->mins			= player_mins;
		baseline->maxs			= player_maxs;

		baseline->colormap		= eindex;
		baseline->modelindex	= playermodelindex;
		baseline->friction		= 1.0;
		baseline->movetype		= MOVETYPE_WALK;

		baseline->scale			= entity->v.scale;
		baseline->solid			= SOLID_SLIDEBOX;
		baseline->framerate		= 1.0;
		baseline->gravity		= 1.0;

	}
	else
	{
		baseline->mins			= entity->v.mins;
		baseline->maxs			= entity->v.maxs;

		baseline->colormap		= 0;
		baseline->modelindex	= entity->v.modelindex;//SV_ModelIndex(pr_strings + entity->v.model);
		baseline->movetype		= entity->v.movetype;

		baseline->scale			= entity->v.scale;
		baseline->solid			= entity->v.solid;
		baseline->framerate		= entity->v.framerate;
		baseline->gravity		= entity->v.gravity;
	}
}

typedef struct
{
	char name[32];
	int	 field;
} entity_field_alias_t;

#define FIELD_ORIGIN0			0
#define FIELD_ORIGIN1			1
#define FIELD_ORIGIN2			2
#define FIELD_ANGLES0			3
#define FIELD_ANGLES1			4
#define FIELD_ANGLES2			5

static entity_field_alias_t entity_field_alias[]=
{
	{ "origin[0]",			0 },
	{ "origin[1]",			0 },
	{ "origin[2]",			0 },
	{ "angles[0]",			0 },
	{ "angles[1]",			0 },
	{ "angles[2]",			0 },
};

void Entity_FieldInit( struct delta_s *pFields )
{
	entity_field_alias[ FIELD_ORIGIN0 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ORIGIN0 ].name );
	entity_field_alias[ FIELD_ORIGIN1 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ORIGIN1 ].name );
	entity_field_alias[ FIELD_ORIGIN2 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ORIGIN2 ].name );
	entity_field_alias[ FIELD_ANGLES0 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ANGLES0 ].name );
	entity_field_alias[ FIELD_ANGLES1 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ANGLES1 ].name );
	entity_field_alias[ FIELD_ANGLES2 ].field		= DELTA_FINDFIELD( pFields, entity_field_alias[ FIELD_ANGLES2 ].name );
}

/*
==================
Entity_Encode

Callback for sending entity_state_t info over network. 
FIXME:  Move to script
==================
*/
void Entity_Encode( struct delta_s *pFields, const unsigned char *from, const unsigned char *to )
{
	entity_state_t *f, *t;
	int localplayer = 0;
	static int initialized = 0;

	if ( !initialized )
	{
		Entity_FieldInit( pFields );
		initialized = 1;
	}

	f = (entity_state_t *)from;
	t = (entity_state_t *)to;

	// Never send origin to local player, it's sent with more resolution in clientdata_t structure
	localplayer =  ( t->number - 1 ) == ENGINE_CURRENT_PLAYER();
	if ( localplayer )
	{
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}

	if ( ( t->impacttime != 0 ) && ( t->starttime != 0 ) )
	{
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );

		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ANGLES0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ANGLES1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ANGLES2 ].field );
	}

	if ( ( t->movetype == MOVETYPE_FOLLOW ) &&
		 ( t->aiment != 0 ) )
	{
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}
	else if ( t->aiment != f->aiment )
	{
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}
}

static entity_field_alias_t player_field_alias[]=
{
	{ "origin[0]",			0 },
	{ "origin[1]",			0 },
	{ "origin[2]",			0 },
};

void Player_FieldInit( struct delta_s *pFields )
{
	player_field_alias[ FIELD_ORIGIN0 ].field		= DELTA_FINDFIELD( pFields, player_field_alias[ FIELD_ORIGIN0 ].name );
	player_field_alias[ FIELD_ORIGIN1 ].field		= DELTA_FINDFIELD( pFields, player_field_alias[ FIELD_ORIGIN1 ].name );
	player_field_alias[ FIELD_ORIGIN2 ].field		= DELTA_FINDFIELD( pFields, player_field_alias[ FIELD_ORIGIN2 ].name );
}

/*
==================
Player_Encode

Callback for sending entity_state_t for players info over network. 
==================
*/
void Player_Encode( struct delta_s *pFields, const unsigned char *from, const unsigned char *to )
{
	entity_state_t *f, *t;
	int localplayer = 0;
	static int initialized = 0;

	if ( !initialized )
	{
		Player_FieldInit( pFields );
		initialized = 1;
	}

	f = (entity_state_t *)from;
	t = (entity_state_t *)to;

	// Never send origin to local player, it's sent with more resolution in clientdata_t structure
	localplayer =  ( t->number - 1 ) == ENGINE_CURRENT_PLAYER();
	if ( localplayer )
	{
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}

	if ( ( t->movetype == MOVETYPE_FOLLOW ) &&
		 ( t->aiment != 0 ) )
	{
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}
	else if ( t->aiment != f->aiment )
	{
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN0 ].field );
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN1 ].field );
		DELTA_SETBYINDEX( pFields, entity_field_alias[ FIELD_ORIGIN2 ].field );
	}
}

#define CUSTOMFIELD_ORIGIN0			0
#define CUSTOMFIELD_ORIGIN1			1
#define CUSTOMFIELD_ORIGIN2			2
#define CUSTOMFIELD_ANGLES0			3
#define CUSTOMFIELD_ANGLES1			4
#define CUSTOMFIELD_ANGLES2			5
#define CUSTOMFIELD_SKIN			6
#define CUSTOMFIELD_SEQUENCE		7
#define CUSTOMFIELD_ANIMTIME		8

entity_field_alias_t custom_entity_field_alias[]=
{
	{ "origin[0]",			0 },
	{ "origin[1]",			0 },
	{ "origin[2]",			0 },
	{ "angles[0]",			0 },
	{ "angles[1]",			0 },
	{ "angles[2]",			0 },
	{ "skin",				0 },
	{ "sequence",			0 },
	{ "animtime",			0 },
};

void Custom_Entity_FieldInit( struct delta_s *pFields )
{
	custom_entity_field_alias[ CUSTOMFIELD_ORIGIN0 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN0 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ORIGIN1 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN1 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ORIGIN2 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN2 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ANGLES0 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES0 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ANGLES1 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES1 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ANGLES2 ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES2 ].name );
	custom_entity_field_alias[ CUSTOMFIELD_SKIN ].field	= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_SKIN ].name );
	custom_entity_field_alias[ CUSTOMFIELD_SEQUENCE ].field= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_SEQUENCE ].name );
	custom_entity_field_alias[ CUSTOMFIELD_ANIMTIME ].field= DELTA_FINDFIELD( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANIMTIME ].name );
}

/*
==================
Custom_Encode

Callback for sending entity_state_t info ( for custom entities ) over network. 
FIXME:  Move to script
==================
*/
void Custom_Encode( struct delta_s *pFields, const unsigned char *from, const unsigned char *to )
{
	entity_state_t *f, *t;
	int beamType;
	static int initialized = 0;

	if ( !initialized )
	{
		Custom_Entity_FieldInit( pFields );
		initialized = 1;
	}

	f = (entity_state_t *)from;
	t = (entity_state_t *)to;

	beamType = t->rendermode & 0x0f;
		
	if ( beamType != BEAM_POINTS && beamType != BEAM_ENTPOINT )
	{
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN0 ].field );
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN1 ].field );
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ORIGIN2 ].field );
	}

	if ( beamType != BEAM_POINTS )
	{
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES0 ].field );
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES1 ].field );
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANGLES2 ].field );
	}

	if ( beamType != BEAM_ENTS && beamType != BEAM_ENTPOINT )
	{
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_SKIN ].field );
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_SEQUENCE ].field );
	}

	// animtime is compared by rounding first
	// see if we really shouldn't actually send it
	if ( (int)f->animtime == (int)t->animtime )
	{
		DELTA_UNSETBYINDEX( pFields, custom_entity_field_alias[ CUSTOMFIELD_ANIMTIME ].field );
	}
}

/*
=================
RegisterEncoders

Allows game .dll to override network encoding of certain types of entities and tweak values, etc.
=================
*/
void RegisterEncoders( void )
{
	DELTA_ADDENCODER( "Entity_Encode", Entity_Encode );
	DELTA_ADDENCODER( "Custom_Encode", Custom_Encode );
	DELTA_ADDENCODER( "Player_Encode", Player_Encode );
}

int GetWeaponData( struct edict_s *player, struct weapon_data_s *info )
{
#if defined( CLIENT_WEAPONS )
	int i;
	weapon_data_t *item;
	entvars_t *pev = &player->v;
	CBasePlayer *pl = ( CBasePlayer *) CBasePlayer::Instance( pev );
	CBasePlayerWeapon *gun;
	
	ItemInfo II;

	memset( info, 0, 32 * sizeof( weapon_data_t ) );

	if ( !pl )
		return 1;

	// go through all of the weapons and make a list of the ones to pack
	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		if ( pl->m_rgpPlayerItems[ i ] )
		{
			// there's a weapon here. Should I pack it?
			CBasePlayerItem *pPlayerItem = pl->m_rgpPlayerItems[ i ];

			while ( pPlayerItem )
			{
				gun = (CBasePlayerWeapon *)pPlayerItem->GetWeaponPtr();
				if ( gun && gun->UseDecrement() )
				{
					// Get The ID.
					memset( &II, 0, sizeof( II ) );
					gun->GetItemInfo( &II );

					if ( II.iId >= 0 && II.iId < 32 )
					{
						item = &info[ II.iId ];
					 	
						item->m_iId						= II.iId;
						item->m_iClip					= gun->m_iClip;

						item->m_flTimeWeaponIdle		= max( gun->m_flTimeWeaponIdle, -0.001 );
						item->m_flNextPrimaryAttack		= max( gun->m_flNextPrimaryAttack, -0.001 );
						item->m_flNextSecondaryAttack	= max( gun->m_flNextSecondaryAttack, -0.001 );
						item->m_fInReload				= gun->m_fInReload;
						item->m_fInSpecialReload		= gun->m_fInSpecialReload;
						item->fuser1					= max( gun->pev->fuser1, -0.001 );
						item->fuser2					= gun->m_flStartThrow;
						item->fuser3					= gun->m_flReleaseThrow;
						
						
						//WARNING - unreliable.  Do not use.
						//item->fuser4					= gun->fuser4;
						/*
						item->fuser5					= gun->fuser5;
						item->fuser6					= gun->fuser6;
						item->fuser7					= gun->fuser7;
						item->fuser8					= gun->fuser8;
						*/

						item->iuser1					= gun->m_chargeReady;
						item->iuser2					= gun->m_fInAttack;
						item->iuser3					= gun->m_fireState;
						
						
											
//						item->m_flPumpTime				= max( gun->m_flPumpTime, -0.001 );
					}
				}
				pPlayerItem = pPlayerItem->m_pNext;
			}
		}
	}
#else
	memset( info, 0, 32 * sizeof( weapon_data_t ) );
#endif
	return 1;
}

/*
=================
UpdateClientData

Data sent to current client only
engine sets cd to 0 before calling.
=================
*/
void UpdateClientData ( const struct edict_s *ent, int sendweapons, struct clientdata_s *cd )
{
	cd->flags			= ent->v.flags;
	cd->health			= ent->v.health;

	cd->viewmodel		= MODEL_INDEX( STRING( ent->v.viewmodel ) );

	cd->waterlevel		= ent->v.waterlevel;
	cd->watertype		= ent->v.watertype;
	cd->weapons			= ent->v.weapons;

	// Vectors
	cd->origin			= ent->v.origin;
	cd->velocity		= ent->v.velocity;
	cd->view_ofs		= ent->v.view_ofs;
	cd->punchangle		= ent->v.punchangle;

	cd->bInDuck			= ent->v.bInDuck;
	cd->flTimeStepSound = ent->v.flTimeStepSound;
	cd->flDuckTime		= ent->v.flDuckTime;
	cd->flSwimTime		= ent->v.flSwimTime;
	cd->waterjumptime	= ent->v.teleport_time;

	strcpy( cd->physinfo, ENGINE_GETPHYSINFO( ent ) );

	cd->maxspeed		= ent->v.maxspeed;
	cd->fov				= ent->v.fov;
	
	
	//MODDD - great emphasis! Can we send a special flag for one frame?
	//Should we send the flag by a separate client message
	cd->weaponanim		= ent->v.weaponanim;

	cd->pushmsec		= ent->v.pushmsec;

#if defined( CLIENT_WEAPONS )
	if ( sendweapons )
	{
		entvars_t *pev = (entvars_t *)&ent->v;
		CBasePlayer *pl = ( CBasePlayer *) CBasePlayer::Instance( pev );

		if ( pl )
		{
			cd->m_flNextAttack	= pl->m_flNextAttack;
			cd->fuser2			= pl->m_flNextAmmoBurn;
			cd->fuser3			= pl->m_flAmmoStartCharge;
			cd->vuser1.x		= pl->ammo_9mm;
			cd->vuser1.y		= pl->ammo_357;
			cd->vuser1.z		= pl->ammo_argrens;
			cd->ammo_nails		= pl->ammo_bolts;
			cd->ammo_shells		= pl->ammo_buckshot;
			cd->ammo_rockets	= pl->ammo_rockets;
			cd->ammo_cells		= pl->ammo_uranium;
			cd->vuser2.x		= pl->ammo_hornets;
			

			if ( pl->m_pActiveItem )
			{
				CBasePlayerWeapon *gun;
				gun = (CBasePlayerWeapon *)pl->m_pActiveItem->GetWeaponPtr();
				if ( gun && gun->UseDecrement() )
				{
					ItemInfo II;
					memset( &II, 0, sizeof( II ) );
					gun->GetItemInfo( &II );

					cd->m_iId = II.iId;

					cd->vuser3.z	= gun->m_iSecondaryAmmoType;
					cd->vuser4.x	= gun->m_iPrimaryAmmoType;
					cd->vuser4.y	= pl->m_rgAmmo[gun->m_iPrimaryAmmoType];
					cd->vuser4.z	= pl->m_rgAmmo[gun->m_iSecondaryAmmoType];
					
					if ( pl->m_pActiveItem->m_iId == WEAPON_RPG )
					{
						cd->vuser2.y = ( ( CRpg * )pl->m_pActiveItem)->m_fSpotActive;
						cd->vuser2.z = ( ( CRpg * )pl->m_pActiveItem)->m_cActiveRockets;
					}
				}
			}
		}
	}
#endif
}

/*
=================
CmdStart

We're about to run this usercmd for the specified player.  We can set up groupinfo and masking here, etc.
This is the time to examine the usercmd for anything extra.  This call happens even if think does not.
=================
*/
void CmdStart( const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed )
{
	entvars_t *pev = (entvars_t *)&player->v;
	CBasePlayer *pl = ( CBasePlayer *) CBasePlayer::Instance( pev );

	if( !pl )
		return;

	if ( pl->pev->groupinfo != 0 )
	{
		UTIL_SetGroupTrace( pl->pev->groupinfo, GROUP_OP_AND );
	}

	pl->random_seed = random_seed;
}

/*
=================
CmdEnd

Each cmdstart is exactly matched with a cmd end, clean up any group trace flags, etc. here
=================
*/
void CmdEnd ( const edict_t *player )
{
	entvars_t *pev = (entvars_t *)&player->v;
	CBasePlayer *pl = ( CBasePlayer *) CBasePlayer::Instance( pev );

	if( !pl )
		return;
	if ( pl->pev->groupinfo != 0 )
	{
		UTIL_UnsetGroupTrace();
	}
}

/*
================================
ConnectionlessPacket

 Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
  size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int	ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
{
	// Parse stuff from args
	int max_buffer_size = *response_buffer_size;

	// Zero it out since we aren't going to respond.
	// If we wanted to response, we'd write data into response_buffer
	*response_buffer_size = 0;

	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead.
	return 0;
}

/*
================================
GetHullBounds

  Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	int iret = 0;

	switch ( hullnumber )
	{
	case 0:				// Normal player
		mins = VEC_HULL_MIN;
		maxs = VEC_HULL_MAX;
		iret = 1;
		break;
	case 1:				// Crouched player
		mins = VEC_DUCK_HULL_MIN;
		maxs = VEC_DUCK_HULL_MAX;
		iret = 1;
		break;
	case 2:				// Point based hull
		mins = Vector( 0, 0, 0 );
		maxs = Vector( 0, 0, 0 );
		iret = 1;
		break;
	}

	return iret;
}

/*
================================
CreateInstancedBaselines

Create pseudo-baselines for items that aren't placed in the map at spawn time, but which are likely
to be created during play ( e.g., grenades, ammo packs, projectiles, corpses, etc. )
================================
*/
void CreateInstancedBaselines ( void )
{
	int iret = 0;
	entity_state_t state;

	memset( &state, 0, sizeof( state ) );

	// Create any additional baselines here for things like grendates, etc.
	// iret = ENGINE_INSTANCE_BASELINE( pc->pev->classname, &state );

	// Destroy objects.
	//UTIL_Remove( pc );
}

/*
================================
InconsistentFile

One of the ENGINE_FORCE_UNMODIFIED files failed the consistency check for the specified player
 Return 0 to allow the client to continue, 1 to force immediate disconnection ( with an optional disconnect message of up to 256 characters )
================================
*/
int	InconsistentFile( const edict_t *player, const char *filename, char *disconnect_message )
{
	// Server doesn't care?
	if ( CVAR_GET_FLOAT( "mp_consistency" ) != 1 )
		return 0;

	// Default behavior is to kick the player
	sprintf( disconnect_message, "Server is enforcing file consistency for %s\n", filename );

	// Kick now with specified disconnect message.
	return 1;
}

/*
================================
AllowLagCompensation

 The game .dll should return 1 if lag compensation should be allowed ( could also just set
  the sv_unlag cvar.
 Most games right now should return 0, until client-side weapon prediction code is written
  and tested for them ( note you can predict weapons, but not do lag compensation, too, 
  if you want.
================================
*/
int AllowLagCompensation( void )
{
	return 1;
}
