


#include "external_lib_include.h"
//#include <stdio.h>


#include "custom_message.h" //what. how were we fine without this.
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
//#include "pm_shared.h"
#include "r_efx.h"

//MODDD - why was this even defined here?  It's already in common/com_model.h.
// Moved to util_shared.h if anywhere even uses this.
//#define MAX_CLIENTS 32

#include <string.h>

/*
#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"
*/

//#include "ammohistory.h"
//#include "vgui_TeamFortressViewport.h"


extern float global2PSEUDO_grabbedByBarancle;

// exists in inputw32.cpp.
extern cvar_t* sensitivity;



EASY_CVAR_EXTERN_HASH_ARRAY

EASY_CVAR_EXTERN_MASS



//EASY_CVAR_EXTERN(canShowWeaponSelectAtDeath)
extern BEAM *pBeam;
extern BEAM *pBeam2;





//MODDD NOTE - kinda.. odd to have to do this, but yea. There is no
// 'in_camera.h', yet we need a method from it.
extern void CAM_ToFirstPerson(void);



//where?
int flag_apply_m_flTimeWeaponIdle = FALSE;
float stored_m_flTimeWeaponIdle = 0;
	
int flag_apply_m_fJustThrown = FALSE;
int stored_m_fJustThrown = 0;



//MODDD - from hud.cpp, at least what they replace was there.
DECLARE_MESSAGE_HUD(Logo)
DECLARE_MESSAGE_HUD(ResetHUD)
DECLARE_MESSAGE_HUD(InitHUD)
DECLARE_MESSAGE_HUD(ViewMode)
DECLARE_MESSAGE_HUD(SetFOV)
DECLARE_MESSAGE_HUD(Concuss)
DECLARE_MESSAGE_HUD(GameMode)



// gHUD-less methods use this form instead.
PROTOTYPE_MESSAGE(PrintCl);
PROTOTYPE_MESSAGE(JBoxReq);
PROTOTYPE_MESSAGE(JBoxOff);
PROTOTYPE_MESSAGE(AutoMus);
PROTOTYPE_MESSAGE(CliTest);
PROTOTYPE_MESSAGE(FirstAppr);

PROTOTYPE_MESSAGE(UpdClientC);
PROTOTYPE_MESSAGE(RstClientC);
PROTOTYPE_MESSAGE(PntClientC);
PROTOTYPE_MESSAGE(UpdBnclStat);
//PROTOTYPE_MESSAGE(UpdateCam);
PROTOTYPE_MESSAGE(UpdPlyA);
PROTOTYPE_MESSAGE(MUnpause);
PROTOTYPE_MESSAGE(UpdTWI);
PROTOTYPE_MESSAGE(UpdJT);
PROTOTYPE_MESSAGE(YMG);
PROTOTYPE_MESSAGE(YMG_S);





void Init_CustomMessage(void){
	
	//MODDD - from hud.cpp
	HOOK_MESSAGE( Logo );
	HOOK_MESSAGE( ResetHUD );
	HOOK_MESSAGE( GameMode );
	HOOK_MESSAGE( InitHUD );
	HOOK_MESSAGE( ViewMode );
	HOOK_MESSAGE( SetFOV );
	HOOK_MESSAGE( Concuss );

	
	//JUKEBOX!
	HOOK_MESSAGE(PrintCl);
	HOOK_MESSAGE(JBoxReq);
	HOOK_MESSAGE(JBoxOff);
	HOOK_MESSAGE(AutoMus);
	HOOK_MESSAGE(CliTest);
	HOOK_MESSAGE(FirstAppr);
	
	HOOK_MESSAGE(UpdClientC);
	HOOK_MESSAGE(RstClientC);
	HOOK_MESSAGE(PntClientC);
	HOOK_MESSAGE(UpdBnclStat);
	//HOOK_MESSAGE(UpdateCam);
	HOOK_MESSAGE(UpdPlyA);
	HOOK_MESSAGE(MUnpause);
	HOOK_MESSAGE(UpdTWI);
	HOOK_MESSAGE(UpdJT);
	HOOK_MESSAGE(YMG);
	HOOK_MESSAGE(YMG_S);
	

}//END OF Init




///////////////////////////////////////////////////////////////////////////////////////////////////
//MODDD - methods moved from the since-deleted hud_msg.cpp.
///////////////////////////////////////////////////////////////////////////////////////////////////


//MODDD - MsgFunc_Logo and MsgFunc_SetFOV moved from hud.cpp.

int CHud::MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	// update Train data
	m_iLogo = READ_BYTE();

	return 1;
}

//MODDD - just note
//NOTE: while "hud_redraw" constantly forces m_iPlayerFOV to default_fov, let it be known that this method is perhaps almost entirely pointless.
int CHud::MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	int newfov = READ_BYTE();

	//MODDD - you know the drill.
	//int def_fov = CVAR_GET_FLOAT( "default_fov" );
	int def_fov = getPlayerBaseFOV();
	

	//Weapon prediction already takes care of changing the fog. ( g_lastFOV ).
	if ( cl_lw && cl_lw->value )
		return 1;

	g_lastFOV = newfov;

	if ( newfov == 0 )
	{
		m_iPlayerFOV = def_fov;
	}
	else
	{
		m_iPlayerFOV = newfov;
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if ( m_iPlayerFOV == def_fov )
	{  
		// reset to saved sensitivity
		m_flMouseSensitivity = 0;
	}
	else
	{  
		// set a new sensitivity that is proportional to the change from the FOV default
		if(def_fov != 0){
			m_flMouseSensitivity = sensitivity->value * ((float)newfov / (float)def_fov) * CVAR_GET_FLOAT("zoom_sensitivity_ratio");
		}else{
			m_flMouseSensitivity = 0; //safety??
		}
	}

	return 1;
}


int CHud::MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf )
{
	//MODDD - this entire method used to be dummied.  Any reason why?
	//!!!
	
	ASSERT( iSize == 0 );

	// clear all hud data
	HUDLIST *pList = m_pHudList;

	while ( pList )
	{
		if ( pList->p )
			pList->p->Reset();
		pList = pList->pNext;
	}

	// reset sensitivity
	m_flMouseSensitivity = 0;

	// reset concussion effect
	m_iConcussionEffect = 0;
	
	return 1;
}

int CHud::MsgFunc_ViewMode( const char *pszName, int iSize, void *pbuf )
{
	CAM_ToFirstPerson();
	return 1;
}

int CHud::MsgFunc_InitHUD( const char *pszName, int iSize, void *pbuf )
{
	//MODDDMIRROR
	numMirrors = 0;
	
	// prepare all hud data
	HUDLIST *pList = m_pHudList;
	
	while (pList)
	{
		if ( pList->p )
			pList->p->InitHUDData();
		pList = pList->pNext;
	}
	
	//Probably not a good place to put this.
	pBeam = pBeam2 = NULL;
	return 1;
}

int CHud::MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_Teamplay = READ_BYTE();

	return 1;
}

//MODDD - MsgFunc_Damage removed.  This is likely stuck in an earlier phase of development,
// differnet intentions.
// Makes sense that it could not work at the HOOK_MESSAGE step (absent in hud.cpp unlike all
// other messages here), the message's name must be unique (globally) and this collides with
// the "MsgFunc_Damage" one in health.cpp.
/*
int CHud::MsgFunc_Damage(const char *pszName, int iSize, void *pbuf )
{
	int	armor, blood;
	Vector	from;
	int	i;
	float count;
	
	BEGIN_READ( pbuf, iSize );
	armor = READ_BYTE();
	blood = READ_BYTE();

	for (i=0 ; i<3 ; i++)
		from[i] = READ_COORD();

	count = (blood * 0.5) + (armor * 0.5);

	if (count < 10)
		count = 10;

	// TODO: kick viewangles,  show damage visually

	return 1;
}
*/


int CHud::MsgFunc_Concuss( const char *pszName, int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );
	m_iConcussionEffect = READ_BYTE();
	if (m_iConcussionEffect)
		this->m_StatusIcons.EnableIcon("dmg_concuss",255,160,0);
	else
		this->m_StatusIcons.DisableIcon("dmg_concuss");
	return 1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


IMPLEMENT_MESSAGE(PrintCl) {
	BEGIN_READ(pbuf, iSize);
	char* formattedString = READ_STRING();
	// print the string in console.   That's it.
	gEngfuncs.Con_Printf(formattedString);
	return 1;
}

//JUKEBOX!
IMPLEMENT_MESSAGE(JBoxReq){
	BEGIN_READ( pbuf, iSize );
	char* strReceive = READ_STRING();
	//Do the jukebox!
	//CLIENT_COMMAND(tempEd, "mp3 play media/Half-Life11.mp3");
	//gEngfuncs.pfnClientCmd("mp3 play media/Half-Life11.mp3");
	gEngfuncs.pfnClientCmd(strReceive);
	//use strReceive in there next!
	return 1;
}

//JUKEBOX!   Just send a "stop" request.
IMPLEMENT_MESSAGE(JBoxOff){
	gEngfuncs.pfnClientCmd("mp3 stop");
	return 1;
}

extern BOOL hasAutoMus;
IMPLEMENT_MESSAGE(AutoMus){
	if(hasAutoMus == FALSE){return 1;}  //don't even bother.

	//BEGIN_READ( pbuf, iSize );
	//char* strReceive = READ_STRING();
	gEngfuncs.pfnClientCmd("mp3 play media/AutoMus.mp3");
	return 1;
}

IMPLEMENT_MESSAGE(CliTest){
	gEngfuncs.pfnClientCmd("mp3 pause media/AutoMus.mp3");
	return 1;
}





IMPLEMENT_MESSAGE(FirstAppr){
	// Keep vars the server gets from the client in-check!
	lateCVarInit();

	return 1;
}//END OF MsgFunc_FirstAppr




IMPLEMENT_MESSAGE(UpdClientC){
#ifdef _DEBUG
//nothing to do here. This should never be called, this feature is unused for Debug mode.
//It already treats everything as ordinary CVars.
//CHANGE.  Even CVars have to be broadcasted from the server to the client for multipalyer to work right for
// players that join a game (as opposed to single-player or a player running the non-dedicated server).
// Point is, non-server running players don't get access to server-registered CVars, so they need to receive
// up-to-date values of them anytime they're changed for access (being cached clientside) for stuff that needs
// access to the same CVars clientside read-only, like almost any CVar involved with weapons, animations, submodels
// (wpn_allowGlockSilencer or whatever), etc.

	BEGIN_READ(pbuf, iSize);

	int argID = READ_SHORT();
	int argPRE = READ_SHORT();
	float arg = ((float)(argPRE)) / 100.0f;

	//????????????????

	*(aryCVarHash[argID]) = arg;
	easyForcePrintLine("CVAR DEBUG: Client: found ID %d. Set CVar %s to %.2f", argID, aryCVarHashName[argID], arg);

#else
	//Need to update hidden CVars meant to be broadcasted to clients. Receive the new value(s) here.

	BEGIN_READ( pbuf, iSize );
	
	int argID = READ_SHORT();
	int argPRE = READ_SHORT();
	float arg = ((float)(argPRE)) / 100.0f;
	
	//????????????????


	//if(EASY_CVAR_GET(hiddenMemPrintout) == 1)easyForcePrintLine("CVAR DEBUG: Client: Received ID %d, newval %.2f", argID, arg);


	*(aryCVarHash[argID]) = arg;
	if(EASY_CVAR_GET(hiddenMemPrintout)==1)easyForcePrintLine("CVAR DEBUG: Client: found ID %d. Set CVar %s to %.2f", argID, aryCVarHashName[argID], arg);
	

	//Save. Is this ok?
	::saveHiddenCVars();



	//this allows all others to follow the formula of "start with else... okay to end with nothing".
	/*
	
	if(FALSE){

	}
	EASY_CVAR_CLIENTSENDOFF_LIST_CLIENT
	*/
	//...

#endif
	return 1;
}//END OF MsgFunc_UpdClientC



IMPLEMENT_MESSAGE(RstClientC){
	// wait.  Shouldn't have to do this.
	// Already handled by resetModCVarsClintOnly.
	// Unfortunately that's not happening yet, see below.
	// DEBUG mode needs this at least?
	
#ifdef _DEBUG

	EASY_CVAR_SET(hud_logo, DEFAULT_hud_logo);
	EASY_CVAR_SET(cl_fvox, DEFAULT_cl_fvox);
	EASY_CVAR_SET(cl_holster, DEFAULT_cl_holster);
	EASY_CVAR_SET(cl_ladder, DEFAULT_cl_ladder);
	
#else
	resetModCVarsClientOnly();
#endif

	return 1;
}//END OF MsgFunc_RstClientC


IMPLEMENT_MESSAGE(PntClientC){
#ifdef _DEBUG

//nothing to do here. This should never be called, this feature is unused for Debug mode.
//It already treats everything as ordinary CVars.

#else
	//Need to update hidden CVars meant to be broadcasted to clients. Receive the new value(s) here.

	BEGIN_READ( pbuf, iSize );
	
	int argID = READ_SHORT();
	
	easyForcePrintLine("\"%s\" is %.2f. My ID was %d.",aryCVarHashName[argID], *aryCVarHash[argID], argID);\

#endif
	return 1;
}//END OF MsgFunc_PntClientC




IMPLEMENT_MESSAGE(UpdBnclStat){
	BEGIN_READ( pbuf, iSize );
	int arg = READ_SHORT();


	global2PSEUDO_grabbedByBarancle = (float)arg;

	return 1;
}


//MODDD - new way of updating player status (dead or alive).
IMPLEMENT_MESSAGE(UpdPlyA){

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();


	//This works because BOOLs are just integers that can only be 0 (off) or 1 (on).
	gHUD.m_fPlayerDead = !x;


	//if dead, and cannot show weapon select... force it off just in case.
	if(gHUD.m_fPlayerDead && EASY_CVAR_GET(canShowWeaponSelectAtDeath) == 0){
		gHUD.m_Ammo.gWR.gpLastSel = gHUD.m_Ammo.gWR.gpActiveSel;
		gHUD.m_Ammo.gWR.gpActiveSel = NULL;
	}
	

	return 1;
}

IMPLEMENT_MESSAGE(MUnpause){
	//BEGIN_READ( pbuf, iSize );
	//???
	
	//that is all.
	gEngfuncs.pfnClientCmd("unpause");
	

	return 1;
}

IMPLEMENT_MESSAGE(UpdTWI){
	
	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	//unpack.
	float newWeaponIdleTime = ((float)x) / 1000;
	
	flag_apply_m_flTimeWeaponIdle = TRUE;
	stored_m_flTimeWeaponIdle = newWeaponIdleTime;

	return 1;
}


IMPLEMENT_MESSAGE(UpdJT){
	
	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();
	
	//unpack... no action needed, straight int.
	int newJustThrown = x;
	
	flag_apply_m_fJustThrown = TRUE;
	stored_m_fJustThrown = newJustThrown;

	return 1;
}




IMPLEMENT_MESSAGE(YMG){
	//BEGIN_READ( pbuf, iSize );
	//???

	playingMov = TRUE;
	movieStartTime = gHUD.recentTime;
	

	return 1;
}

IMPLEMENT_MESSAGE(YMG_S){
	//BEGIN_READ( pbuf, iSize );
	//???

	playingMov = FALSE;
	
	

	return 1;
}









