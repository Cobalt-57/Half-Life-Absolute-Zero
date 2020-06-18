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
//  cdll_int.c
//
// this implementation handles the linking of the engine to the DLL
//

#include "hud.h"
#include "cl_util.h"
#include "netadr.h"
#include "vgui_schememanager.h"

extern "C"
{
#include "pm_shared.h"
}

#include <string.h>
#include "hud_servers.h"
#include "vgui_int.h"
#include "interface.h"


EASY_CVAR_EXTERN(drawHUD);




cl_enginefunc_t gEngfuncs;
CHud gHUD;
TeamFortressViewport *gViewPort = NULL;

void InitInput (void);
void EV_HookEvents( void );
void IN_Commands( void );

/*
========================== 
    Initialize

Called when the DLL is first loaded.
==========================
*/
extern "C" 
{
int		DLLEXPORT_2 Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion );
int		DLLEXPORT_2 HUD_VidInit( void );
void	DLLEXPORT_2 HUD_Init( void );
int		DLLEXPORT_2 HUD_Redraw( float flTime, int intermission );
int		DLLEXPORT_2 HUD_UpdateClientData( client_data_t *cdata, float flTime );
void	DLLEXPORT_2 HUD_Reset ( void );
void	DLLEXPORT_2 HUD_PlayerMove( struct playermove_s *ppmove, int server );
void	DLLEXPORT_2 HUD_PlayerMoveInit( struct playermove_s *ppmove );
char	DLLEXPORT_2 HUD_PlayerMoveTexture( char *name );
int		DLLEXPORT_2 HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
int		DLLEXPORT_2 HUD_GetHullBounds( int hullnumber, float *mins, float *maxs );
void	DLLEXPORT_2 HUD_Frame( double time );
void	DLLEXPORT_2 HUD_VoiceStatus(int entindex, qboolean bTalking);
void	DLLEXPORT_2 HUD_DirectorMessage( int iSize, void *pbuf );
}

/*
================================
HUD_GetHullBounds

  Engine calls this to enumerate player collision hulls, for prediction.  Return 0 if the hullnumber doesn't exist.
================================
*/
int DLLEXPORT_2 HUD_GetHullBounds( int hullnumber, float *mins, float *maxs )
{
	int iret = 0;

	switch ( hullnumber )
	{
	case 0:				// Normal player
		mins = Vector(-16, -16, -36);
		maxs = Vector(16, 16, 36);
		iret = 1;
		break;
	case 1:				// Crouched player
		mins = Vector(-16, -16, -18 );
		maxs = Vector(16, 16, 18 );
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
HUD_ConnectionlessPacket

 Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
  size of the response_buffer, so you must zero it out if you choose not to respond.
================================
*/
int	DLLEXPORT_2 HUD_ConnectionlessPacket( const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size )
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

void DLLEXPORT_2 HUD_PlayerMoveInit( struct playermove_s *ppmove )
{
	PM_Init( ppmove );
}

char DLLEXPORT_2 HUD_PlayerMoveTexture( char *name )
{
	return PM_FindTextureType( name );
}

void DLLEXPORT_2 HUD_PlayerMove( struct playermove_s *ppmove, int server )
{
	PM_Move( ppmove, server );
}

int DLLEXPORT_2 Initialize( cl_enginefunc_t *pEnginefuncs, int iVersion )
{
	gEngfuncs = *pEnginefuncs;

	if (iVersion != CLDLL_INTERFACE_VERSION)
		return 0;

	memcpy(&gEngfuncs, pEnginefuncs, sizeof(cl_enginefunc_t));

	EV_HookEvents();

	return 1;
}


/*
==========================
	HUD_VidInit

Called when the game initializes
and whenever the vid_mode is changed
so the HUD can reinitialize itself.
==========================
*/

int DLLEXPORT_2 HUD_VidInit( void )
{
	gHUD.VidInit();

	VGui_Startup();

	return 1;
}

/*
==========================
	HUD_Init

Called whenever the client connects
to a server.  Reinitializes all 
the hud variables.
==========================
*/

EASY_CVAR_EXTERN_MASS
EASY_CVAR_DECLARE_HASH_ARRAY

BOOL hasAutoMus = FALSE;

void DLLEXPORT_2 HUD_Init( void )
{

	EASY_CVAR_HASH_MASS



	determineHiddenMemPath();
	
	hasAutoMus = checkSubFileExistence("media/AutoMus.mp3");

	loadHiddenCVars();

	// nope, this call will happen before starting any game (game startup to the menu), which isn't the place to call lateCVarInit.
	//lateCVarInit();

	//MODDD - see if we need to make any special printouts in the future.
	//testForHelpFile();

	InitInput();
	gHUD.Init();
	Scheme_Init();
}


/*
==========================
	HUD_Redraw

called every screen frame to
redraw the HUD.
===========================
*/



float globalPSEUDO_drawHUDMem = -1;

int DLLEXPORT_2 HUD_Redraw( float time, int intermission )
{


	if(globalPSEUDO_drawHUDMem != EASY_CVAR_GET(drawHUD)){
		gHUD.m_Ammo.updateCrosshair();
		globalPSEUDO_drawHUDMem = EASY_CVAR_GET(drawHUD);
	}


	//MODDD - the ultamite blocker!
	if(EASY_CVAR_GET(drawHUD) != 1 && EASY_CVAR_GET(drawHUD) != 2){
		return 1;
	}


	//MODDD - for things that can not access time ordinarily.
	gHUD.recentTime = time;

	gHUD.Redraw( time, intermission );

	return 1;
}


/*
==========================
	HUD_UpdateClientData

called every time shared client
dll/engine data gets changed,
and gives the cdll a chance
to modify the data.

returns 1 if anything has been changed, 0 otherwise.
==========================
*/



int DLLEXPORT_2 HUD_UpdateClientData(client_data_t *pcldata, float flTime )
{
	IN_Commands();


	return gHUD.UpdateClientData(pcldata, flTime );
}

/*
==========================
	HUD_Reset

Called at start and end of demos to restore to "non"HUD state.
==========================
*/

void DLLEXPORT_2 HUD_Reset( void )
{
	gHUD.VidInit();
}

/*
==========================
HUD_Frame

Called by engine every frame that client .dll is loaded
==========================
*/

void DLLEXPORT_2 HUD_Frame( double time )
{
	ServersThink( time );

	GetClientVoiceMgr()->Frame(time);
}


/*
==========================
HUD_VoiceStatus

Called when a player starts or stops talking.
==========================
*/

void DLLEXPORT_2 HUD_VoiceStatus(int entindex, qboolean bTalking)
{
	GetClientVoiceMgr()->UpdateSpeakerStatus(entindex, bTalking);
}

/*
==========================
HUD_DirectorEvent

Called when a director event message was received
==========================
*/

void DLLEXPORT_2 HUD_DirectorMessage( int iSize, void *pbuf )
{
	 gHUD.m_Spectator.DirectorMessage( iSize, pbuf );
}


