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
// hud.cpp
//
// implementation of CHud class
//



//MODDD - This controls whether cheat CVars can be modified directly in multiplayer.
#define CHEATS_ALLOWED_IN_MULTI_PLAYER 1

#if CHEATS_ALLOWED_IN_MULTI_PLAYER == 1
//If not, "PROTECTION_PLACE" is empty, contributing nothing to the CVar flags.
#define PROTECTION_PLACE 
#else
//If so, the flag "FCVAR_PROTECTED" is added.
#define PROTECTION_PLACE | FCVAR_PROTECTED
#endif



#include "hud.h"
#include "cl_util.h"
#include <string.h>
#include <stdio.h>
#include "parsemsg.h"
#include "hud_servers.h"
#include "vgui_int.h"
#include "vgui_TeamFortressViewport.h"

#include "demo.h"
#include "demo_api.h"
#include "vgui_scorepanel.h"

//MODDD - new.
#include "../versionAid.h"


//MODDD - externs
extern float global2_hud_version;
extern float global2_preE3UsesFailColors;
extern float global2_E3UsesFailColors;
extern float global2_weaponSelectUsesReloadSounds;

extern float global2PSEUDO_determinedFOV;

extern float global2_hud_drawsidebarmode;

extern cvar_t* cvar2_cl_server_interpolation;
extern float global2_cl_server_interpolation;


class CHLVoiceStatusHelper : public IVoiceStatusHelper
{
public:
	virtual void GetPlayerTextColor(int entindex, int color[3])
	{
		color[0] = color[1] = color[2] = 255;

		if( entindex >= 0 && entindex < sizeof(g_PlayerExtraInfo)/sizeof(g_PlayerExtraInfo[0]) )
		{
			int iTeam = g_PlayerExtraInfo[entindex].teamnumber;

			if ( iTeam < 0 )
			{
				iTeam = 0;
			}

			iTeam = iTeam % iNumberOfTeamColors;

			color[0] = iTeamColors[iTeam][0];
			color[1] = iTeamColors[iTeam][1];
			color[2] = iTeamColors[iTeam][2];
		}
	}

	virtual void UpdateCursorState()
	{
		gViewPort->UpdateCursorState();
	}

	virtual int	GetAckIconHeight()
	{
		return ScreenHeight - gHUD.m_iFontHeight*3 - 6;
	}

	virtual bool			CanShowSpeakerLabels()
	{
		if( gViewPort && gViewPort->m_pScoreBoard )
			return !gViewPort->m_pScoreBoard->isVisible();
		else
			return false;
	}
};
static CHLVoiceStatusHelper g_VoiceStatusHelper;


//MDODD - constructor added.
CHud::CHud() : m_iSpriteCount(0), m_pHudList(NULL), m_fPlayerDead(FALSE), recentTime(-1), m_prc_brokentransparency(NULL), frozenMem(FALSE), recentDamageBitmask(-1), PESUDO_cameraModeMem(-1)  {
	//start off assuming the player is NOT dead.

	alphaCrossHairIndex = -1;
	
	useAlphaCrosshairMem = -1;
	allowAlphaCrosshairWithoutGunsMem = -1;

	
}



extern client_sprite_t *GetSpriteList(client_sprite_t *pList, const char *psz, int iRes, int iCount);

extern cvar_t *sensitivity;
cvar_t *cl_lw = NULL;


cvar_t *cl_viewrollangle;
cvar_t *cl_viewrollspeed;

//NEWSDK: these cvars are Absent from the new SDK.
/*
cvar_t *cl_viewrollangle;
cvar_t *cl_viewrollspeed;
*/

void ShutdownInput (void);

//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_Logo(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Logo(pszName, iSize, pbuf );
}

//DECLARE_MESSAGE(m_Logo, Logo)
int __MsgFunc_ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_ResetHUD(pszName, iSize, pbuf );
}

int __MsgFunc_InitHUD(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_InitHUD( pszName, iSize, pbuf );
	return 1;
}

int __MsgFunc_ViewMode(const char *pszName, int iSize, void *pbuf)
{
	gHUD.MsgFunc_ViewMode( pszName, iSize, pbuf );
	return 1;
}

int __MsgFunc_SetFOV(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_SetFOV( pszName, iSize, pbuf );
}

int __MsgFunc_Concuss(const char *pszName, int iSize, void *pbuf)
{
	return gHUD.MsgFunc_Concuss( pszName, iSize, pbuf );
}

int __MsgFunc_GameMode(const char *pszName, int iSize, void *pbuf )
{
	return gHUD.MsgFunc_GameMode( pszName, iSize, pbuf );
}




// TFFree Command Menu
void __CmdFunc_OpenCommandMenu(void)
{
	if ( gViewPort )
	{
		gViewPort->ShowCommandMenu( gViewPort->m_StandardMenu );
	}
}

// TFC "special" command
void __CmdFunc_InputPlayerSpecial(void)
{
	if ( gViewPort )
	{
		gViewPort->InputPlayerSpecial();
	}
}

void __CmdFunc_CloseCommandMenu(void)
{
	if ( gViewPort )
	{
		gViewPort->InputSignalHideCommandMenu();
	}
}

void __CmdFunc_ForceCloseCommandMenu( void )
{
	if ( gViewPort )
	{
		gViewPort->HideCommandMenu();
	}
}

void __CmdFunc_ToggleServerBrowser( void )
{
	if ( gViewPort )
	{
		gViewPort->ToggleServerBrowser();
	}
}

// TFFree Command Menu Message Handlers
int __MsgFunc_ValClass(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ValClass( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamNames(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamNames( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_Feign(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Feign( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_Detpack(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Detpack( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_VGUIMenu(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_VGUIMenu( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_MOTD(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_MOTD( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_BuildSt(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_BuildSt( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_RandomPC(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_RandomPC( pszName, iSize, pbuf );
	return 0;
}
 
int __MsgFunc_ServerName(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ServerName( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_ScoreInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_ScoreInfo( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamScore(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamScore( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_TeamInfo(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_TeamInfo( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_Spectator(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_Spectator( pszName, iSize, pbuf );
	return 0;
}

int __MsgFunc_AllowSpec(const char *pszName, int iSize, void *pbuf)
{
	if (gViewPort)
		return gViewPort->MsgFunc_AllowSpec( pszName, iSize, pbuf );
	return 0;
}



/*
//MODDD - temp method
void tempGetHudMainRef2(CHudBase* tempRef){
	//tempRef->topHealth = m_Health.m_iHealth;
}
*/

void CHud::drawPartialFromBottom(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b){
	drawPartialFromBottom(arg_sprite, arg_rect, arg_portion, x, y, r, g, b, 0);
}
void CHud::drawPartialFromBottom(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b, const int& canDrawBrokenTrans){
	
	wrect_t rc = *arg_rect;
	int heightDiff = arg_rect->bottom - arg_rect->top;

	rc.top  += heightDiff * (-arg_portion + 1); //((float)(100-(min(100,m_iBat))) * 0.01);	// battery can go from 0 to 100 so * 0.01 goes from 0 to 1
	
	if (rc.bottom > rc.top)
	{

		//gHUD.drawAdditiveFilter( 0, x, y - iOffset + (rc.top - m_prc2->top), &rc);
		gHUD.drawAdditiveFilter( arg_sprite, r, g, b, 0, x, y + (rc.top - arg_rect->top), &rc, canDrawBrokenTrans);
		
	}

}

void CHud::drawPartialFromLeft(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b){
	drawPartialFromLeft(arg_sprite, arg_rect, arg_portion, x, y, r, g, b, 0);
}

void CHud::drawPartialFromLeft(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b, const int& canDrawBrokenTrans){
	
	wrect_t rc = *arg_rect;
	int widthDiff = arg_rect->right - arg_rect->left;

	//rc.left  += widthDiff * (-arg_portion + 1); //((float)(100-(min(100,m_iBat))) * 0.01);	// battery can go from 0 to 100 so * 0.01 goes from 0 to 1
	rc.right += (int)ceil(widthDiff * (arg_portion + -1));

	if (rc.right > rc.left){
		//gHUD.drawAdditiveFilter( 0, x, y - iOffset + (rc.top - m_prc2->top), &rc);
		gHUD.drawAdditiveFilter( arg_sprite, r, g, b, 0, x, y, &rc, canDrawBrokenTrans);
	}
}




void CHud::drawPartialFromRight(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b){
	drawPartialFromRight(arg_sprite, arg_rect, arg_portion, x, y, r, g, b, 0);
}
void CHud::drawPartialFromRight(const SpriteHandle_t & arg_sprite, const wrect_t* arg_rect, const float arg_portion, const int & x, const int & y,  int & r, int & g, int & b, const int& canDrawBrokenTrans){
	wrect_t rc = *arg_rect;
	int draw_x = x;
	int widthDiff = arg_rect->right - arg_rect->left;

	//rc.right += widthDiff * (arg_portion + -1);

	//do this instead?
	rc.left += widthDiff * (-arg_portion + 1);
	draw_x += widthDiff * (-arg_portion + 1);

	if (rc.right > rc.left){
		gHUD.drawAdditiveFilter( arg_sprite, r, g, b, 0, draw_x, y, &rc, canDrawBrokenTrans);
	}
	
	//alternate?
	/*
	float m_flBat = arg_portion;
	int m_iWidth = widthDiff;
	
    int iOffset = (int) floor(m_iWidth * (1.0 - m_flBat));
    if (iOffset < m_iWidth)
    {
        rc.left += iOffset;

        SPR_Set(arg_sprite, r, g, b );
        SPR_DrawAdditive( 0, x + iOffset, y, &rc);
    }
	*/
}




int CHud::canDrawSidebar(void){
	if ( (global2_hud_drawsidebarmode == 2) || ( (!gHUD.canDrawBottomStats && global2_hud_drawsidebarmode == 0) || (gHUD.canDrawBottomStats && global2_hud_drawsidebarmode == 1) ) ){
		return TRUE;
	}else{
		return FALSE;
	}
}


//inline
//maybe don't inline methods expected to call other methods.  Or if they're implemented in a file/place
//external to the class's {   } body.  I don't remember what about this caused the crash exactly.
void CHud::drawAdditiveFilter(int sprite, const int& r, const int& g, const int& b, int huh, int x, int y, wrect_t* rect){
	drawAdditiveFilter(sprite, r, g, b, huh, x, y, rect, 0);
}

//MODDD - new method.  Does the same thing as "drawAdditive", except this factors in "showBrokenHUDAlpha".
//If it is true (1), draw some faded gray on top of the image's rect.
void CHud::drawAdditiveFilter(int sprite, const int& r, const int& g, const int& b, int huh, int x, int y, wrect_t* rect, const int& canDrawBrokenTrans){
	

	//SPR_Draw
	//SPR_DrawAdditive
	//SPR_DrawHoles


	/*
	if(hud_version->value == 0 && showBrokenHUDAlpha->value == 1 && canDrawBrokenTrans == 1){
		//FillRGBA(0, 0, 35, 35, 88, 8, 8, 185);
		int times = 25;
		while(times > 0){
			SPR_Set(NULL, 0, 0, 0 );
			FillRGBA(x, y, rect->right - rect->left, rect->bottom - rect->top, 1, 1, 1, 255);
			times -= 1;
		}
	}
	*/

	
	if(canDrawBrokenTrans == 1){
		//does CVar checks.
		attemptDrawBrokenTrans(x, y, rect);
	}else if(canDrawBrokenTrans == 2){
		//
		attemptDrawBrokenTransLightAndWhite(x, y, rect);

		//attemptDrawBrokenTransLight(x, y, rect);
		//attemptDrawBrokenTransWhite(x, y, rect);
	}
	
	//SPR_Set(GetSprite(myFontIDZero + k), r, g, b );
	SPR_Set(sprite, r, g, b );
	SPR_DrawAdditive(huh, x, y, rect);
	
}



//inline
void CHud::attemptDrawBrokenTrans(int arg_startx, int arg_starty, wrect_t* rect){
	if(rect!=NULL)attemptDrawBrokenTrans(arg_startx, arg_starty, rect->right-rect->left, rect->bottom - rect->top);
}
//inline
void CHud::attemptDrawBrokenTransLight(int arg_startx, int arg_starty, wrect_t* rect){
	if(rect!=NULL)attemptDrawBrokenTransLight(arg_startx, arg_starty, rect->right-rect->left, rect->bottom - rect->top);
}
//inline
void CHud::attemptDrawBrokenTransWhite(int arg_startx, int arg_starty, wrect_t* rect){
	if(rect!=NULL)attemptDrawBrokenTransWhite(arg_startx, arg_starty, rect->right-rect->left, rect->bottom - rect->top);
}
//inline
void CHud::attemptDrawBrokenTransLightAndWhite(int arg_startx, int arg_starty, wrect_t* rect){
	if(rect!=NULL)attemptDrawBrokenTransLightAndWhite(arg_startx, arg_starty, rect->right-rect->left, rect->bottom - rect->top);
}




void CHud::attemptDrawBrokenTrans(int arg_startx, int arg_starty, int arg_width, int arg_height){
	
	if(global2_hud_version == 0 && showBrokenHUDAlpha->value > 0 && m_HUD_brokentransparency >= 0 ){
		
		//CUT - adjustable isn't working.
		float opaqueness = 1.0;

		wrect_t tempwrectThis = wrect_t();
		tempwrectThis.left = 0;
		tempwrectThis.top = 0;


				
		for(int y = 0; y < arg_height; y+=brokenTransHeight){
			for(int x = 0; x < arg_width; x+=brokenTransWidth){

				if(x + brokenTransWidth < arg_width){
					tempwrectThis.right = brokenTransWidth;
				}else{
					tempwrectThis.right = arg_width - x;
				}
				if(y + brokenTransHeight < arg_height){
					tempwrectThis.bottom = brokenTransHeight;
				}else{
					tempwrectThis.bottom = arg_height - y;
				}

				//draw the whole thing here.
				SPR_Set( GetSprite(this->m_HUD_brokentransparency), 255 * opaqueness, 255 * opaqueness, 255 * opaqueness );
				SPR_DrawHoles(0, arg_startx + x, arg_starty + y, &tempwrectThis);
				
			}
		}

	}

}




void CHud::attemptDrawBrokenTransLight(int arg_startx, int arg_starty, int arg_width, int arg_height){
	
	if(global2_hud_version == 0 && showBrokenHUDAlpha->value > 0 && m_HUD_brokentransparency0 >= 0){
		float opaqueness = 1.0;
		
		wrect_t tempwrectThis = wrect_t();
		tempwrectThis.left = 0;
		tempwrectThis.top = 0;
				
		for(int y = 0; y < arg_height; y+=brokenTransHeight){
			for(int x = 0; x < arg_width; x+=brokenTransWidth){

				if(x + brokenTransWidth < arg_width){
					tempwrectThis.right = brokenTransWidth;
				}else{
					tempwrectThis.right = arg_width - x;
				}
				if(y + brokenTransHeight < arg_height){
					tempwrectThis.bottom = brokenTransHeight;
				}else{
					tempwrectThis.bottom = arg_height - y;
				}

				//draw the whole thing here.
				SPR_Set( GetSprite(this->m_HUD_brokentransparency0), 255 * opaqueness, 255 * opaqueness, 255 * opaqueness );
				SPR_DrawHoles(0, arg_startx + x, arg_starty + y, &tempwrectThis);
				
			}
		}


	}

}



void CHud::attemptDrawBrokenTransWhite(int arg_startx, int arg_starty, int arg_width, int arg_height){
	
	if(global2_hud_version == 0 && showBrokenHUDAlpha->value > 0 && m_HUD_brokentransparencyw >= 0){
		float opaqueness = 0.04;
		

		wrect_t tempwrectThis = wrect_t();
		tempwrectThis.left = 0;
		tempwrectThis.top = 0;

				
		for(int y = 0; y < arg_height; y+=brokenTransHeight){
			for(int x = 0; x < arg_width; x+=brokenTransWidth){

				if(x + brokenTransWidth < arg_width){
					tempwrectThis.right = brokenTransWidth;
				}else{
					tempwrectThis.right = arg_width - x;
				}
				if(y + brokenTransHeight < arg_height){
					tempwrectThis.bottom = brokenTransHeight;
				}else{
					tempwrectThis.bottom = arg_height - y;
				}

				//draw the whole thing here.
				SPR_Set( GetSprite(this->m_HUD_brokentransparencyw), 255 * opaqueness, 255 * opaqueness, 255 * opaqueness );
				SPR_DrawHoles(0, arg_startx + x, arg_starty + y, &tempwrectThis);
				
			}
		}
	}

}


void CHud::attemptDrawBrokenTransLightAndWhite(int arg_startx, int arg_starty, int arg_width, int arg_height){
	
	if(global2_hud_version == 0 && showBrokenHUDAlpha->value > 0 && m_HUD_brokentransparencyw >= 0){
		float opaqueness = 0.04;
		

		wrect_t tempwrectThis = wrect_t();
		tempwrectThis.left = 0;
		tempwrectThis.top = 0;

				
		for(int y = 0; y < arg_height; y+=brokenTransHeight){
			for(int x = 0; x < arg_width; x+=brokenTransWidth){

				if(x + brokenTransWidth < arg_width){
					tempwrectThis.right = brokenTransWidth;
				}else{
					tempwrectThis.right = arg_width - x;
				}
				if(y + brokenTransHeight < arg_height){
					tempwrectThis.bottom = brokenTransHeight;
				}else{
					tempwrectThis.bottom = arg_height - y;
				}
				opaqueness = 1.0;
				SPR_Set( GetSprite(this->m_HUD_brokentransparency0), 255 * opaqueness, 255 * opaqueness, 255 * opaqueness );
				SPR_DrawHoles(0, arg_startx + x, arg_starty + y, &tempwrectThis);
				
				opaqueness = 0.04;
				//draw the whole thing here.
				SPR_Set( GetSprite(this->m_HUD_brokentransparencyw), 255 * opaqueness, 255 * opaqueness, 255 * opaqueness );
				SPR_DrawAdditive(0, arg_startx + x, arg_starty + y, &tempwrectThis);
				
			}
		}

	}

}









//MODDD - new.
void CHud::playWeaponSelectMoveSound(){

	if(global2_weaponSelectUsesReloadSounds != 1){
		PlaySound("common/wpn_moveselect.wav", 1);
	}else{

		long rand = gEngfuncs.pfnRandomLong(0,2);
		switch(rand){
		case 0:
			PlaySound("weapons/reload1.wav", 1);
		break;
		case 1:
			PlaySound("weapons/reload2.wav", 1);
		break;
		case 2:
			PlaySound("weapons/reload3.wav", 1);
		break;
		}
	}
}







void command_removeFireFly(){
	gEngfuncs.pfnClientCmd("removeFireFly2");
}
void command_showBounds(){
	gEngfuncs.pfnClientCmd("showBounds2");
}
void command_showBoundsAll(){
	gEngfuncs.pfnClientCmd("showBoundsAll2");
}


void command_sneaky(){
	//gEngfuncs.GetLocalPlayer()

	//gEngfuncs.GetLocalPlayer()->curstate.fl

	gEngfuncs.pfnClientCmd("impulse 105");
	gEngfuncs.pfnClientCmd("notarget");

	
	//gEngfuncs.pfnClientCmd("mp3 play media/Half-Life11.mp3");
	//HEY LOOK THIS WORKS.   Huh.  Noooo idea.  ah well....?  why?

	//CLIENT_COMMAND(tempEd, "mp3 play media/Half-Life11.mp3");


	//  mp3 play media/Half-Life11.mp3
	//~try this?  or
	//  mp3 play sound/media/Half-Life11.mp3
	//~sometime?

}



void command_updateCameraPerspectiveF(void){
	gEngfuncs.pfnClientCmd("cameraper_f");
}
void command_updateCameraPerspectiveT(void){
	gEngfuncs.pfnClientCmd("cameraper_t");
}


void command_god2(void){
	gEngfuncs.pfnClientCmd("god2");
}
void command_noclip2(void){
	gEngfuncs.pfnClientCmd("noclip2");
}
void command_fvoxtoggle(void){
	gEngfuncs.pfnClientCmd("fvoxtoggle");
}






// This is called every time the DLL is loaded
void CHud :: Init( void )
{
	

	HOOK_MESSAGE( Logo );
	HOOK_MESSAGE( ResetHUD );
	HOOK_MESSAGE( GameMode );
	HOOK_MESSAGE( InitHUD );
	HOOK_MESSAGE( ViewMode );
	HOOK_MESSAGE( SetFOV );
	HOOK_MESSAGE( Concuss );

	
	// TFFree CommandMenu
	HOOK_COMMAND( "+commandmenu", OpenCommandMenu );
	HOOK_COMMAND( "-commandmenu", CloseCommandMenu );
	HOOK_COMMAND( "ForceCloseCommandMenu", ForceCloseCommandMenu );
	HOOK_COMMAND( "special", InputPlayerSpecial );
	HOOK_COMMAND( "togglebrowser", ToggleServerBrowser );



	//MODDDDMIRROR - this block.
	viewEntityIndex = 0; // trigger_viewset stuff
	viewFlags = 0;
	m_iLogo = 0;
	m_iFOV = 0;
	numMirrors = 0;
	//m_iHUDColor = 0x00FFA000; //255,160,0 -- LRC






	//MODDD - associate this text in console with toggling the logo on and off ("1" or "0" afterwards):
	toggleLogo = gEngfuncs.pfnRegisterVariable ( "hud_logo", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	showBrokenHUDAlpha = gEngfuncs.pfnRegisterVariable ( "hud_brokentrans", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	
	
	CVAR_CREATE("cl_fvox", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	CVAR_CREATE("cl_ladder", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	CVAR_CREATE("cl_explosion", "0", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	
	CVAR_CREATE("pissedNPCs", "0", FCVAR_ARCHIVE);
	//MODDD - can drop in single player.  Usually off for whatever reason.

	
	//always start at what fvoxEnabled was.
	//CVAR_CREATE("IGNOREfvoxEnabledMem", CVAR_GET_STRING("cl_fvox") , FCVAR_CLIENTDLL);
	//CVAR_CREATE("IGNOREbarnacleCanGib", "0", FCVAR_ARCHIVE);
	
	//CVAR_CREATE("IGNOREremoveFireFly", "0", FCVAR_CLIENTDLL);
	//gEngfuncs.pfnAddCommand( "removeFireFly", command_removeFireFly );
	
	//CVAR_CREATE("IGNOREshowBounds", "0", FCVAR_CLIENTDLL);
	//gEngfuncs.pfnAddCommand( "showBounds", command_showBounds );
	//CVAR_CREATE("IGNOREshowBoundsAll", "0", FCVAR_CLIENTDLL);
	//gEngfuncs.pfnAddCommand( "showBoundsAll", command_showBoundsAll );
	
	//MODDD
	//CVAR_CREATE("IGNOREgod2", "0", FCVAR_CLIENTDLL);
	//CVAR_CREATE("IGNOREnoclip2", "0", FCVAR_CLIENTDLL);
	

	/*
	//mimmick built-in functions "god" and "noclip", so that the copies may be modified instead.
	gEngfuncs.pfnAddCommand( "god2", command_god2 );
	gEngfuncs.pfnAddCommand( "noclip2", command_noclip2 );
	//shortened version too (for auto-complete, if that's in there?)
	gEngfuncs.pfnAddCommand( "go", command_god2 );
	gEngfuncs.pfnAddCommand( "nocli", command_noclip2 );
	*/

	gEngfuncs.pfnAddCommand( "noclip_friendly", command_noclip2 );
	gEngfuncs.pfnAddCommand( "god_friendly", command_god2 );

	gEngfuncs.pfnAddCommand( "cl_fvoxtoggle", command_fvoxtoggle );







	
	
	//weaponSelectSoundPlayOnMousewheel = gEngfuncs.pfnRegisterVariable("weaponSelectSoundPlayOnMousewheel", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	//timedDamageDeathRemoveMode = CVAR_CREATE("timedDamageDeathRemoveMode", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	//weaponSelectUsesReloadSounds = CVAR_CREATE("weaponSelectUsesReloadSounds", "0", FCVAR_ARCHIVE);
	

	//CVar_cameraMode = CVAR_CREATE("IGNOREcameraMode", "0", FCVAR_CLIENTDLL);
	//CVar_cameraModeMem = 0;

	CVAR_CREATE("precacheAll", "1", FCVAR_ARCHIVE);
	CVAR_CREATE("soundSentenceSave", "1", FCVAR_ARCHIVE | FCVAR_CLIENTDLL);
	//Apply the method for referring to sounds by sentences.txt instead of precaching them to save precache space.
	




	//cvarHUD_letswatchamovie = CVAR_CREATE( "letswatchamovie", "0", FCVAR_CLIENTDLL );

	gEngfuncs.pfnAddCommand( "sneaky", command_sneaky );
	//Toggles both "impulse 105" (no player noise) and "notarget".



	char aryChr[128];
	char aryChrD[128];
	writeVersionInfo(aryChr, 128);
	writeDateInfo(aryChrD, 128);


	CVAR_CREATE("protoVersionC", aryChr, FCVAR_CLIENTDLL);
	CVAR_CREATE("protoDateC", aryChrD, FCVAR_CLIENTDLL);

	//to be written to by the server dll (default name: hl.dll) at game start.
	CVAR_CREATE("protoVersionS", "Start the game!", 0);
	CVAR_CREATE("protoDateS", "Start the game!", 0);
	
	
	EASY_CVAR_CREATE_CLIENT_MASS


	









	//CVAR_CREATE("barnacleEatsAnything", "0", FCVAR_ARCHIVE | FCVAR_PROTECTED | FCVAR_UNLOGGED | FCVAR_PRINTABLEONLY);
	//CVAR_CREATE("barnacleEatsBarnacles", "0", FCVAR_ARCHIVE);
	
	
	HOOK_MESSAGE( ValClass );
	HOOK_MESSAGE( TeamNames );
	HOOK_MESSAGE( Feign );
	HOOK_MESSAGE( Detpack );
	HOOK_MESSAGE( MOTD );
	HOOK_MESSAGE( BuildSt );
	HOOK_MESSAGE( RandomPC );
	HOOK_MESSAGE( ServerName );
	HOOK_MESSAGE( ScoreInfo );
	HOOK_MESSAGE( TeamScore );
	HOOK_MESSAGE( TeamInfo );

	HOOK_MESSAGE( Spectator );
	HOOK_MESSAGE( AllowSpec );

	// VGUI Menus
	HOOK_MESSAGE( VGUIMenu );


	cl_viewrollangle = CVAR_CREATE("cl_viewrollangle", "0.65", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	cl_viewrollspeed = CVAR_CREATE("cl_viewrollspeed", "300", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);


	CVAR_CREATE( "hud_classautokill", "1", FCVAR_ARCHIVE | FCVAR_USERINFO );		// controls whether or not to suicide immediately on TF class switch
	CVAR_CREATE( "hud_takesshots", "0", FCVAR_ARCHIVE );		// controls whether or not to automatically take screenshots at the end of a round
	





	m_iLogo = 0;
	m_iFOV = 0;

	CVAR_CREATE( "zoom_sensitivity_ratio", "1.2", 0 );

	
	cvar2_cl_server_interpolation = CVAR_CREATE( "cl_server_interpolation", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );






	//MODDD - changed how this works.  Remembers since last time, and the default is different.
	//default_fov = CVAR_CREATE( "default_fov", "90", 0 );
	//In the base game, the default_fov always starts out at 90.
	default_fov = CVAR_CREATE( "default_fov", "105", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	

	//aspectratio_determined_fov = CVAR_CREATE("aspectratio_determined_fov", "0", 0);

	




	/*
	//Do NOT remember the "old" value.  Always begins as empty!
	CVAR_CREATE("canApplyDefaultFOV", "0", FCVAR_CLIENTDLL);
	auto_adjust_fov_aspect = CVAR_CREATE("auto_adjust_fov_aspect", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE);
	
	//MODDD - new FOV vars for zooming.
	auto_adjust_zoomfov = CVAR_CREATE( "auto_adjust_zoomfov", "1", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	pythzon_oomfov = CVAR_CREATE( "python_zoomfov", "48", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	crossbow_zoomfov = CVAR_CREATE( "crossbow_zoomfov", "24", FCVAR_CLIENTDLL | FCVAR_ARCHIVE );
	*/


	
	//Not necessary, updated by player really early if needed.
	/*
	if(auto_adjust_zoomfov->value == 1){
		python_zoomfov->value = auto_adjust_zoomfov->value * 0.4444f;
		crossbow_zoomfov->value = auto_adjust_zoomfov->value * 0.2222f;
	}
	*/	

	
	
	

























	m_pCvarStealMouse = CVAR_CREATE( "hud_capturemouse", "1", FCVAR_ARCHIVE );
	m_pCvarDraw = CVAR_CREATE( "hud_draw", "1", FCVAR_ARCHIVE );
	cl_lw = gEngfuncs.pfnGetCvarPointer( "cl_lw" );

	m_pSpriteList = NULL;

	// Clear any old HUD list
	if ( m_pHudList )
	{
		HUDLIST *pList;
		while ( m_pHudList )
		{
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free( pList );
		}
		m_pHudList = NULL;
	}

	// In case we get messages before the first update -- time will be valid
	m_flTime = 1.0;

	m_CustomMessage.Init();
	m_Ammo.Init();
	m_Health.Init();
	m_SayText.Init();
	m_Spectator.Init();
	m_Geiger.Init();
	m_Train.Init();
	m_Battery.Init();
	m_Flash.Init();
	m_Message.Init();
	m_StatusBar.Init();
	m_DeathNotice.Init();
	m_AmmoSecondary.Init();
	m_TextMessage.Init();
	m_StatusIcons.Init();
	GetClientVoiceMgr()->Init(&g_VoiceStatusHelper, (vgui::Panel**)&gViewPort);

	m_Menu.Init();
	
	ServersInit();

	MsgFunc_ResetHUD(0, 0, NULL );


	
}

// CHud destructor
// cleans up memory allocated for m_rg* arrays
CHud :: ~CHud()
{
	delete [] m_rgSpriteHandle_ts;
	delete [] m_rgrcRects;
	delete [] m_rgszSpriteNames;

	if ( m_pHudList )
	{
		HUDLIST *pList;
		while ( m_pHudList )
		{
			pList = m_pHudList;
			m_pHudList = m_pHudList->pNext;
			free( pList );
		}
		m_pHudList = NULL;
	}

	ServersShutdown();
}






void CHud::getGenericGUIColor(int &r, int &g, int &b){
	
	if(global2_hud_version == 0){
		getGenericGreenColor(r, g, b);
	}else{
		getGenericOrangeColor(r, g, b);
	}

}


void CHud::getGenericEmptyColor(int &r, int &g, int &b){

	if(global2_hud_version == 0){

		if(global2_preE3UsesFailColors == 1){
			getGenericRedColor(r,g,b);
		}else{
			getGenericGreenColor(r, g, b);
		}

	}else{

		if(global2_E3UsesFailColors == 1){
			getGenericRedColor(r,g,b);
		}else{
			getGenericOrangeColor(r, g, b);
		}
	}

}




void CHud::getGenericRedColor(int &r, int &g, int &b){
	r = 245;
	g = 8;
	b = 8;
}

void CHud::getGenericOrangeColor(int &r, int &g, int &b){
	r = 255;
	g = 152;
	b = 0;
}


void CHud::getGenericGreenColor(int &r, int &g, int &b){
	r = 5;
	g = COLOR_PRE_E3_BRIGHTNESS;
	b = 5;
}







// GetSpriteIndex()
// searches through the sprite list loaded from hud.txt for a name matching SpriteName
// returns an index into the gHUD.m_rgSpriteHandle_ts[] array
// returns 0 if sprite not found
int CHud :: GetSpriteIndex( const char *SpriteName )
{

	//MODDD - printouts.
	//easyPrint( "SPRITE COUNT IS %d\n", m_iSpriteCount );
	//UTIL_ClientPrintAll( HUD_PRINTNOTIFY, numstr);

	
	// look through the loaded sprite name list for SpriteName
	for ( int i = 0; i < m_iSpriteCount; i++ )
	{
		if ( strncmp( SpriteName, m_rgszSpriteNames + (i * MAX_SPRITE_NAME_LENGTH), MAX_SPRITE_NAME_LENGTH ) == 0 )
			return i;
	}

	return -1; // invalid sprite
}

void CHud :: VidInit( void )
{

	//?
	//easyPrintLine("HUD::: VIDINIT!");

	m_scrinfo.iSize = sizeof(m_scrinfo);
	GetScreenInfo(&m_scrinfo);


	//gEngfuncs.pfnGetCvarPointer( "cl_lw" );???

	//MODDD - determine the FOV, given the screen size available at this point (by a ratio of width/height, plugged
	//        into a linear function).
	//cvar_t* aspectratio_determined_fov = CVAR_GET("aspectratio_determined_fov");
	//aspectratio_determined_fov->value = (int) (  ((float)ScreenWidth / (float)ScreenHeight) * 45 + 30   );
	//new formula.  ratio of 1.333 still yeilds FOV of 90, but ratio of 1.777 yields FOV of 105 instead of 110.
	


	int determinedFOV = (int) (  ((float)ScreenWidth / (float)ScreenHeight) * 33.75f + 45   );
	//aspectratio_determined_fov->value = determinedFOV;
	//gEngfuncs.Cvar_SetValue("aspectratio_determined_fov", determinedFOV );
	//send it off!
	
	global2PSEUDO_determinedFOV = determinedFOV;
	
	
	//easyPrintLine("HHHHHHHHHHHHHHHHHHHHHH %d %d, %.2f", ScreenWidth, ScreenHeight, (float)determinedFOV);


	//MODDDDMIRROR - this block.  Also here too (?)
	viewEntityIndex = 0; // trigger_viewset stuff
	viewFlags = 0;
	m_iLogo = 0;
	m_iFOV = 0;
	numMirrors = 0;
	//m_iHUDColor = 0x00FFA000; //255,160,0 -- LRC



	//m_hsprGNFOS = 0;

	//Why does loading it here avoid crashes sometimes? Who knows.
	m_hsprGNFOS = SPR_Load("sprites/ymg.spr");



	// ----------
	// Load Sprites
	// ---------
//	m_hsprFont = LoadSprite("sprites/%d_font.spr");
	
	m_hsprLogo = 0;	
	m_hsprCursor = 0;

	if (ScreenWidth < 640)
		m_iRes = 320;
	else
		m_iRes = 640;

	// Only load this once
	if ( !m_pSpriteList )
	{

		//MODDD - printouts.  And test edit.
		//m_iSpriteCountAllRes = 124;
		//easyPrint("THE SIZE IS %d\n", m_iSpriteCountAllRes);

		// we need to load the hud.txt, and all sprites within
		m_pSpriteList = SPR_GetList("sprites/hud.txt", &m_iSpriteCountAllRes);

		//MODDD - printouts.
		//easyPrint("THE SIZE IS 222 %d\n", m_iSpriteCountAllRes);


		if (m_pSpriteList)
		{
			// count the number of sprites of the appropriate res
			m_iSpriteCount = 0;
			client_sprite_t *p = m_pSpriteList;
			int j;
			for ( j = 0; j < m_iSpriteCountAllRes; j++ )
			{

				if ( p->iRes == m_iRes )
					m_iSpriteCount++;
				p++;
			}

			//MODDD - printouts.
			//easyPrint("THE SIZE IS 333 %d\n", m_iSpriteCount);



			// allocated memory for sprite handle arrays
 			m_rgSpriteHandle_ts = new SpriteHandle_t[m_iSpriteCount];
			m_rgrcRects = new wrect_t[m_iSpriteCount];
			m_rgszSpriteNames = new char[m_iSpriteCount * MAX_SPRITE_NAME_LENGTH];

			p = m_pSpriteList;
			int index = 0;
			for ( j = 0; j < m_iSpriteCountAllRes; j++ )
			{
				if ( p->iRes == m_iRes )
				{
					char sz[256];
					sprintf(sz, "sprites/%s.spr", p->szSprite);
					m_rgSpriteHandle_ts[index] = SPR_Load(sz);
					m_rgrcRects[index] = p->rc;
					strncpy( &m_rgszSpriteNames[index * MAX_SPRITE_NAME_LENGTH], p->szName, MAX_SPRITE_NAME_LENGTH );

					/*
					char tempString[MAX_SPRITE_NAME_LENGTH + 1];
					int tempindex = 0;
					for(tempindex = 0; tempindex < MAX_SPRITE_NAME_LENGTH; tempindex++){
						char tempChar = m_rgszSpriteNames[(j * MAX_SPRITE_NAME_LENGTH) + tempindex];
						if(tempChar == '\0'){
							break;
						}
						tempString[tempindex] = tempChar;
					}
					tempString[tempindex] = '\0';
					easyPrint("I LOADED SPRITE #%d: %s\n", j, tempString ) ;
					*/
					

					index++;
				}

				//MODDD - printouts.
				//easyPrint("I LOADED SPRITE #%d: %s\n", j, p->szName ) ;
				
				p++;
				//easyPrint("IS P NULL YET? %d\n", (p == NULL) );

			}


			//MODDD - printouts.
			/*
			easyPrint("EEEEEEEEEEEEEEEEEEEEEEEEEEEE\n" ) ;



			p = m_pSpriteList;
			int times = 0;
			int maxTimes = 125;
			while(p != nullptr){
				easyPrint("I SEE SPRITE #%d: %s : %s      %d\n", times, p->szName, p->szSprite, p->rc.right ) ;
				p++;
				//easyPrint("IS P NULL YET? %d   times: %d\n", (p == NULL), times );
				times++;

				if(times >= maxTimes){
					break;
				}

			}
			*/

		}
	}
	else
	{
		// we have already have loaded the sprite reference from hud.txt, but
		// we need to make sure all the sprites have been loaded (we've gone through a transition, or loaded a save game)
		client_sprite_t *p = m_pSpriteList;
		int index = 0;
		for ( int j = 0; j < m_iSpriteCountAllRes; j++ )
		{
			if ( p->iRes == m_iRes )
			{
				char sz[256];
				sprintf( sz, "sprites/%s.spr", p->szSprite );
				m_rgSpriteHandle_ts[index] = SPR_Load(sz);
				index++;
			}

			p++;
		}
	}

	// assumption: number_1, number_2, etc, are all listed and loaded sequentially
	m_HUD_number_0 = GetSpriteIndex( "number_0" );

	//MODDD - added
	m_HUD_number_0_health = GetSpriteIndex( "number_0health" );
	m_HUD_number_1_tiny = GetSpriteIndex( "number_1tiny" );

	//MODDD - altgui
	m_HUD_number_0_E3R = GetSpriteIndex("number_0_E3R");
	m_HUD_battery_empty_E3 = GetSpriteIndex("battery_empty_E3");

	m_HUD_battery_full_E3_glow = GetSpriteIndex("battery_full_E3_glow");
	m_HUD_battery_full_E3_minimal = GetSpriteIndex("battery_full_E3_minimal");


	alphaCrossHairIndex = GetSpriteIndex( "alphacrosshair" );
	if(alphaCrossHairIndex == -1){
		easyPrint("ALPHACROSSHAIR NOT LOADED A\n");
	}else{
		//easyPrint("ALPHACROSSHAIR LOADED\n");
	}
	
	m_HUD_brokentransparency = GetSpriteIndex("brokentrans");
	/*
	m_HUD_brokentransparency1 = GetSpriteIndex("brokentrans1");
	m_HUD_brokentransparency2 = GetSpriteIndex("brokentrans2");
	m_HUD_brokentransparency3 = GetSpriteIndex("brokentrans3");
	m_HUD_brokentransparency4 = GetSpriteIndex("brokentrans4");
	*/
	
	m_HUD_brokentransparency0 = GetSpriteIndex("brokentransalt");
	m_HUD_brokentransparencyw = GetSpriteIndex("brokentranswww");


	//try to get the new glock silencer and old RPG (if it wasn't there) in!
	m_glockSilencerWpnIcoActive = GetSpriteIndex("glocksilactive");
	m_glockSilencerWpnIcoInactive = GetSpriteIndex("glocksilinact");
	



	if(m_HUD_brokentransparency >= 0){
		m_prc_brokentransparency = &gHUD.GetSpriteRect( m_HUD_brokentransparency );
		brokenTransWidth = m_prc_brokentransparency->right - m_prc_brokentransparency->left;
		brokenTransHeight = m_prc_brokentransparency->bottom - m_prc_brokentransparency->top;

	}






	m_iFontHeight = m_rgrcRects[m_HUD_number_0].bottom - m_rgrcRects[m_HUD_number_0].top;

	m_CustomMessage.VidInit();
	m_Ammo.VidInit();
	m_Health.VidInit();
	m_Spectator.VidInit();
	m_Geiger.VidInit();
	m_Train.VidInit();
	m_Battery.VidInit();
	m_Flash.VidInit();
	m_Message.VidInit();
	m_StatusBar.VidInit();
	m_DeathNotice.VidInit();
	m_SayText.VidInit();
	m_Menu.VidInit();
	m_AmmoSecondary.VidInit();
	m_TextMessage.VidInit();
	m_StatusIcons.VidInit();
	GetClientVoiceMgr()->VidInit();
}

int CHud::MsgFunc_Logo(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	// update Train data
	m_iLogo = READ_BYTE();

	return 1;
}

float g_lastFOV = 0.0;

/*
============
COM_FileBase
============
*/
// Extracts the base name of a file (no path, no extension, assumes '/' as path separator)
void COM_FileBase ( const char *in, char *out)
{
	int len, start, end;

	len = strlen( in );
	
	// scan backward for '.'
	end = len - 1;
	while ( end && in[end] != '.' && in[end] != '/' && in[end] != '\\' )
		end--;
	
	if ( in[end] != '.' )		// no '.', copy to end
		end = len-1;
	else 
		end--;					// Found ',', copy to left of '.'


	// Scan backward for '/'
	start = len-1;
	while ( start >= 0 && in[start] != '/' && in[start] != '\\' )
		start--;

	if ( in[start] != '/' && in[start] != '\\' )
		start = 0;
	else 
		start++;

	// Length of new sting
	len = end - start + 1;

	// Copy partial string
	strncpy( out, &in[start], len );
	// Terminate it
	out[len] = 0;
}

/*
=================
HUD_IsGame

=================
*/
int HUD_IsGame( const char *game )
{
	const char *gamedir;
	char gd[ 1024 ];

	gamedir = gEngfuncs.pfnGetGameDirectory();
	if ( gamedir && gamedir[0] )
	{
		COM_FileBase( gamedir, gd );
		if ( !stricmp( gd, game ) )
			return 1;
	}
	return 0;
}

/*
=====================
HUD_GetFOV

Returns last FOV
=====================
*/
float HUD_GetFOV( void )
{
	if ( gEngfuncs.pDemoAPI->IsRecording() )
	{
		// Write it
		int i = 0;
		unsigned char buf[ 100 ];

		// Active
		*( float * )&buf[ i ] = g_lastFOV;
		i += sizeof( float );

		Demo_WriteBuffer( TYPE_ZOOM, i, buf );
	}

	if ( gEngfuncs.pDemoAPI->IsPlayingback() )
	{
		g_lastFOV = g_demozoom;
	}
	return g_lastFOV;
}



//MODDD - just note
//NOTE: while "hud_redraw" constantly forces m_iFOV to default_fov, let it be known that this method is perhaps almost entirely pointless.
int CHud::MsgFunc_SetFOV(const char *pszName,  int iSize, void *pbuf)
{
	BEGIN_READ( pbuf, iSize );

	int newfov = READ_BYTE();
	int def_fov = CVAR_GET_FLOAT( "default_fov" );

	//Weapon prediction already takes care of changing the fog. ( g_lastFOV ).
	if ( cl_lw && cl_lw->value )
		return 1;

	g_lastFOV = newfov;

	if ( newfov == 0 )
	{
		m_iFOV = def_fov;
	}
	else
	{
		m_iFOV = newfov;
	}

	// the clients fov is actually set in the client data update section of the hud

	// Set a new sensitivity
	if ( m_iFOV == def_fov )
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


void CHud::AddHudElem(CHudBase *phudelem)
{
	HUDLIST *pdl, *ptemp;

//phudelem->Think();

	if (!phudelem)
		return;

	pdl = (HUDLIST *)malloc(sizeof(HUDLIST));
	if (!pdl)
		return;

	memset(pdl, 0, sizeof(HUDLIST));
	pdl->p = phudelem;

	if (!m_pHudList)
	{
		m_pHudList = pdl;
		return;
	}

	ptemp = m_pHudList;

	while (ptemp->pNext)
		ptemp = ptemp->pNext;

	ptemp->pNext = pdl;
}

float CHud::GetSensitivity( void )
{
	return m_flMouseSensitivity;
}


