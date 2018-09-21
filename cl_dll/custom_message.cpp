

#include "custom_message.h" //what. how were we fine without this.
#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
//#include "pm_shared.h"

#include <string.h>
#include <stdio.h>

/*
#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"
*/

//#include "ammohistory.h"
//#include "vgui_TeamFortressViewport.h"







//where?
int flag_apply_m_flTimeWeaponIdle = FALSE;
float stored_m_flTimeWeaponIdle = 0;
	
int flag_apply_m_fJustThrown = FALSE;
int stored_m_fJustThrown = 0;




extern int flag_apply_m_flTimeWeaponIdle;
extern float stored_m_flTimeWeaponIdle;
extern int flag_apply_m_fJustThrown;
extern int stored_m_fJustThrown;







extern float global2PSEUDO_grabbedByBarancle;
extern float global2_canShowWeaponSelectAtDeath;

EASY_CVAR_EXTERN_HASH_ARRAY

EASY_CVAR_EXTERN_MASS




//JUKEBOX!
DECLARE_MESSAGE( m_CustomMessage, JBoxReq )
DECLARE_MESSAGE( m_CustomMessage, JBoxOff )
DECLARE_MESSAGE( m_CustomMessage, AutoMus )
DECLARE_MESSAGE( m_CustomMessage, CliTest)
DECLARE_MESSAGE( m_CustomMessage, UpdClientC )
DECLARE_MESSAGE( m_CustomMessage, RstClientC )
DECLARE_MESSAGE( m_CustomMessage, PntClientC )
DECLARE_MESSAGE( m_CustomMessage, RetrieveFOV )
DECLARE_MESSAGE( m_CustomMessage, UpdBnclStat )
DECLARE_MESSAGE( m_CustomMessage, UpdPlyA)
DECLARE_MESSAGE( m_CustomMessage, MUnpause)
DECLARE_MESSAGE( m_CustomMessage, UpdTWI)
DECLARE_MESSAGE( m_CustomMessage, UpdJT)
DECLARE_MESSAGE( m_CustomMessage, YMG)
DECLARE_MESSAGE( m_CustomMessage, YMG_S)





CCustomMessage::CCustomMessage(void){

}


int CCustomMessage::Init(void){
	
	gHUD.AddHudElem(this);
	
	//JUKEBOX!
	HOOK_MESSAGE(JBoxReq);
	HOOK_MESSAGE(JBoxOff);
	HOOK_MESSAGE(AutoMus);
	HOOK_MESSAGE(CliTest);
	HOOK_MESSAGE(UpdClientC);
	HOOK_MESSAGE(RstClientC);
	HOOK_MESSAGE(PntClientC);
	HOOK_MESSAGE(RetrieveFOV);
	HOOK_MESSAGE(UpdBnclStat);
	//HOOK_MESSAGE(UpdateCam);
	HOOK_MESSAGE(UpdPlyA);
	HOOK_MESSAGE(MUnpause);
	HOOK_MESSAGE(UpdTWI);
	HOOK_MESSAGE(UpdJT);
	HOOK_MESSAGE(YMG);
	HOOK_MESSAGE(YMG_S);
	

	Reset();

	return 1;
}//END OF Init

void CCustomMessage::Reset(void){

	
	//m_iFlags |= HUD_ACTIVE; //!!!

}


int CCustomMessage::VidInit(void){

	//???

	return 1;
}





int CCustomMessage::Draw(float flTime){



	return 1;
}



void CCustomMessage::Think(void){




}












//JUKEBOX!
int CCustomMessage::MsgFunc_JBoxReq(const char *pszName, int iSize, void *pbuf){

	
	BEGIN_READ( pbuf, iSize );
	char* strReceive = READ_STRING();

	//Do the jukebox!

	//updateCrosshair((int)CVAR_GET_FLOAT("useAlphaCrosshair"), x);
	//CLIENT_COMMAND(tempEd, "mp3 play media/Half-Life11.mp3");
	
	//gEngfuncs.pfnClientCmd("mp3 play media/Half-Life11.mp3");
	gEngfuncs.pfnClientCmd(strReceive);
	//use strReceive in there next!


	return 1;
}


//JUKEBOX!   Just send a "stop" request.
int CCustomMessage::MsgFunc_JBoxOff(const char *pszName, int iSize, void *pbuf){
	gEngfuncs.pfnClientCmd("mp3 stop");
	return 1;
}


extern BOOL hasAutoMus;
int CCustomMessage::MsgFunc_AutoMus(const char *pszName, int iSize, void *pbuf){
	if(hasAutoMus == FALSE){return 1;}  //don't even bother.

	//BEGIN_READ( pbuf, iSize );
	//char* strReceive = READ_STRING();
	gEngfuncs.pfnClientCmd("mp3 play media/AutoMus.mp3");
	return 1;
}
//

int CCustomMessage::MsgFunc_CliTest(const char *pszName, int iSize, void *pbuf){
	gEngfuncs.pfnClientCmd("mp3 pause media/AutoMus.mp3");
	return 1;
}












int CCustomMessage::MsgFunc_UpdClientC(const char *pszName, int iSize, void *pbuf){
#ifdef _DEBUG

//nothing to do here. This should never be called, this feature is unused for Debug mode.
//It already treats everything as ordinary CVars.

#else
	//Need to update hidden CVars meant to be broadcasted to clients. Receive the new value(s) here.

	BEGIN_READ( pbuf, iSize );
	
	int argID = READ_SHORT();
	int argPRE = READ_SHORT();
	float arg = ((float)(argPRE)) / 100.0f;
	
	//????????????????


	//if(global2_hiddenMemPrintout == 1)easyForcePrintLine("CVAR DEBUG: Client: Received ID %d, newval %.2f", argID, arg);


	*(aryCVarHash[argID]) = arg;
	if(global2_hiddenMemPrintout==1)easyForcePrintLine("CVAR DEBUG: Client: found ID %d. Set CVar %s to %.2f", argID, aryCVarHashName[argID], arg);
	

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





int CCustomMessage::MsgFunc_RstClientC(const char *pszName, int iSize, void *pbuf){
#ifdef _DEBUG

//nothing to do here. This should never be called, this feature is unused for Debug mode.
//It already treats everything as ordinary CVars.

#else
	
	resetModCVarsClientOnly();


#endif
	return 1;
}//END OF MsgFunc_RstClientC




int CCustomMessage::MsgFunc_PntClientC(const char *pszName, int iSize, void *pbuf){
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










int CCustomMessage::MsgFunc_RetrieveFOV(const char *pszName, int iSize, void *pbuf){
	//BEGIN_READ( pbuf, iSize );
	//???
	sendAutoFOV();

	return 1;
}

int CCustomMessage::MsgFunc_UpdBnclStat(const char *pszName, int iSize, void *pbuf){
	BEGIN_READ( pbuf, iSize );
	int arg = READ_SHORT();


	global2PSEUDO_grabbedByBarancle = (float)arg;

	return 1;
}


//MODDD - new way of updating player status (dead or alive).
int CCustomMessage::MsgFunc_UpdPlyA(const char *pszName, int iSize, void *pbuf){

	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();


	//This works because BOOLs are just integers that can only be 0 (off) or 1 (on).
	gHUD.m_fPlayerDead = !x;


	//if dead, and cannot show weapon select... force it off just in case.
	if(gHUD.m_fPlayerDead && global2_canShowWeaponSelectAtDeath == 0){
		gHUD.m_Ammo.gWR.gpLastSel = gHUD.m_Ammo.gWR.gpActiveSel;
		gHUD.m_Ammo.gWR.gpActiveSel = NULL;
	}
	

	return 1;
}

int CCustomMessage::MsgFunc_MUnpause(const char *pszName, int iSize, void *pbuf){
	//BEGIN_READ( pbuf, iSize );
	//???
	
	//that is all.
	gEngfuncs.pfnClientCmd("unpause");
	

	return 1;
}

int CCustomMessage::MsgFunc_UpdTWI(const char *pszName, int iSize, void *pbuf){
	
	BEGIN_READ( pbuf, iSize );
	int x = READ_SHORT();

	//unpack.
	float newWeaponIdleTime = ((float)x) / 1000;
	
	flag_apply_m_flTimeWeaponIdle = TRUE;
	stored_m_flTimeWeaponIdle = newWeaponIdleTime;

	return 1;
}


int CCustomMessage::MsgFunc_UpdJT(const char *pszName, int iSize, void *pbuf){
	
	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();
	
	//unpack... no action needed, straight int.
	int newJustThrown = x;
	
	flag_apply_m_fJustThrown = TRUE;
	stored_m_fJustThrown = newJustThrown;

	return 1;
}







extern int playingMov;
extern float movieStartTime;

int CCustomMessage::MsgFunc_YMG(const char *pszName, int iSize, void *pbuf){
	//BEGIN_READ( pbuf, iSize );
	//???

	playingMov = TRUE;
	movieStartTime = gHUD.recentTime;
	

	return 1;
}

int CCustomMessage::MsgFunc_YMG_S(const char *pszName, int iSize, void *pbuf){
	//BEGIN_READ( pbuf, iSize );
	//???

	playingMov = FALSE;
	
	

	return 1;
}









