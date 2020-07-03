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
// flashlight.cpp
//
// implementation of CHudFlashlight class
//

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"

#include <string.h>
#include <stdio.h>


EASY_CVAR_EXTERN(canShowWeaponSelectAtDeath)



DECLARE_MESSAGE(m_Flash, FlashBat)
DECLARE_MESSAGE(m_Flash, Flashlight)

#define BAT_NAME "sprites/%d_Flashlight.spr"








int CHudFlashlight::Init(void)
{
	m_fFade = 0;
	m_fOn = 0;

	HOOK_MESSAGE(Flashlight);
	HOOK_MESSAGE(FlashBat);

	m_iFlags |= HUD_ACTIVE;

	gHUD.AddHudElem(this);

	return 1;
};

void CHudFlashlight::Reset(void)
{
	m_fFade = 0;
	m_fOn = 0;
}

int CHudFlashlight::VidInit(void)
{
	int HUD_flash_empty = gHUD.GetSpriteIndex( "flash_empty" );
	int HUD_flash_full = gHUD.GetSpriteIndex( "flash_full" );
	int HUD_flash_beam = gHUD.GetSpriteIndex( "flash_beam" );

	//MODDD - new
	alphaFlashLightOnIndex = gHUD.GetSpriteIndex("alphaflashlighton");
	alphaFlashLightOffIndex = gHUD.GetSpriteIndex("alphaflashlightoff");


	m_SpriteHandle_t1 = gHUD.GetSprite(HUD_flash_empty);
	m_SpriteHandle_t2 = gHUD.GetSprite(HUD_flash_full);
	m_hBeam = gHUD.GetSprite(HUD_flash_beam);
	m_prc1 = &gHUD.GetSpriteRect(HUD_flash_empty);
	m_prc2 = &gHUD.GetSpriteRect(HUD_flash_full);
	m_prcBeam = &gHUD.GetSpriteRect(HUD_flash_beam);
	m_iWidth = m_prc2->right - m_prc2->left;

	return 1;
};

int CHudFlashlight:: MsgFunc_FlashBat(const char *pszName,  int iSize, void *pbuf )
{

	
	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();
	m_iBat = x;
	m_flBat = ((float)x)/100.0;

	return 1;
}

int CHudFlashlight:: MsgFunc_Flashlight(const char *pszName,  int iSize, void *pbuf )
{

	BEGIN_READ( pbuf, iSize );
	m_fOn = READ_BYTE();
	int x = READ_BYTE();
	m_iBat = x;
	m_flBat = ((float)x)/100.0;

	return 1;
}



int CHudFlashlight::Draw(float flTime)
{
	if ( gHUD.m_iHideHUDDisplay & ( HIDEHUD_FLASHLIGHT | HIDEHUD_ALL ) )
		return 1;

	int r, g, b, x, y, a;
	wrect_t rc;

	if (!(gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT)) ))
		return 1;


	if(gHUD.m_fPlayerDead){
		if(EASY_CVAR_GET(canShowWeaponSelectAtDeath) == 1){
			//If the weapon select sidebar can be shown, we're good.
		}else{
			//otherwise, don't draw anything here.
			return 0;
		}
		
	}








	if (m_fOn)
		a = 225;
	else
		a = MIN_ALPHA;

	//MODDD - old color scheme disabled.
	/*
	if (m_flBat < 0.20)
		UnpackRGB(r,g,b, RGB_REDISH);
	else
		UnpackRGB(r,g,b, RGB_YELLOWISH);
	*/
	gHUD.getGenericGUIColor(r, g, b);

	//UnpackRGB(r,g,b, RGB_YELLOWISH);
	//MODDD - everything disabled.  Er, replaced.
	
	//This means the weapon-select screen is up.  So draw the flash light.
	if( gHUD.canDrawSidebar() ){
		
		int alphaFlashLightWidth = gHUD.GetSpriteRect(alphaFlashLightOnIndex).right - gHUD.GetSpriteRect(alphaFlashLightOnIndex).left;
		int alphaFlashLightHeight = gHUD.GetSpriteRect(alphaFlashLightOnIndex).bottom - gHUD.GetSpriteRect(alphaFlashLightOnIndex).top;

		
		x = ScreenWidth - alphaFlashLightWidth - 20;
		y = ScreenHeight - (alphaFlashLightHeight) - 73;

		if ( m_fOn ){
			SPR_Set(gHUD.GetSprite(alphaFlashLightOnIndex), r, g, b );
			SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(alphaFlashLightOnIndex));
		}else{
			SPR_Set(gHUD.GetSprite(alphaFlashLightOffIndex), r, g, b );
			SPR_DrawAdditive( 0,  x, y, &gHUD.GetSpriteRect(alphaFlashLightOffIndex));
		}

	}
	/*
	
	ScaleColors(r, g, b, a);
	//m_hBeam
	int iFlashightHeight = m_prc1->bottom - m_prc1->top;

	//MODDD - new coords?
	//y = (m_prc1->bottom - m_prc2->top)/2;
	//x = ScreenWidth - m_iWidth - m_iWidth/2 ;

	y = (ScreenHeight - gHUD.m_iFontHeight*2.75 - (iFlashightHeight*1.5));
	x = ScreenWidth - m_iWidth*2 ;
	

	// Draw the flashlight casing
	SPR_Set(m_SpriteHandle_t1, r, g, b );
	SPR_DrawAdditive( 0,  x, y, m_prc1);

	if ( m_fOn )
	{  // draw the flashlight beam
		//MODDD
		//x = ScreenWidth - m_iWidth/2;
		x = ScreenWidth - m_iWidth ;

		SPR_Set( m_hBeam, r, g, b );
		SPR_DrawAdditive( 0, x, y, m_prcBeam );
	}

	// draw the flashlight energy level
	//MODDD
	//x = ScreenWidth - m_iWidth - m_iWidth/2 ;
	x = ScreenWidth - m_iWidth*2 ;


	int iOffset = m_iWidth * (1.0 - m_flBat);
	if (iOffset < m_iWidth)
	{
		rc = *m_prc2;
		rc.left += iOffset;

		SPR_Set(m_SpriteHandle_t2, r, g, b );
		SPR_DrawAdditive( 0, x + iOffset, y, &rc);
	}
	*/

	return 1;
}