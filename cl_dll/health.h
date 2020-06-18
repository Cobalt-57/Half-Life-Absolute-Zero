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



#ifndef HEALTH_H
#define HEALTH_H

#include "hudbase.h"



#define DMG_IMAGE_LIFE		2	// seconds that image is up

//MODDD - SOOOOooooo these are all actually unused.
// DMG_IMAGE_LIFE above is a duration, not an ID in some list.
// Which looks to be sorted out alright without these constants.
// okie dokie.
/*
#define DMG_IMAGE_POISON	0
#define DMG_IMAGE_ACID		1
#define DMG_IMAGE_COLD		2
#define DMG_IMAGE_DROWN		3
#define DMG_IMAGE_BURN		4
#define DMG_IMAGE_NERVE		5
#define DMG_IMAGE_RAD		6
#define DMG_IMAGE_SHOCK		7
//tf defines
#define DMG_IMAGE_CALTROP	8
#define DMG_IMAGE_TRANQ		9
#define DMG_IMAGE_CONCUSS	10
#define DMG_IMAGE_HALLUC	11

//MODDD - new.
#define DMG_IMAGE_BLEED	12
*/

//MODDD - new size.
//#define NUM_DMG_TYPES		12
#define NUM_DMG_TYPES		13



//MODDD - damage type info (DMG_) moved to util_shared.h.




typedef struct
{
	float fExpire;
	float fBaseline;
	int	x, y;
} DAMAGE_IMAGE;


//
//-----------------------------------------------------
//

//OK. HOW IN THE HELL DOES THIS KNOW WHAT "CHudBase" IS??? NO INCLUDES IN THIS FILE. I'M LOSING MY FECKIN' MIND OVER THIS.
//Apparently this is only included by hud.h after it has declared the CHudBase class.  ...HACKY. but it works.
//Nah, just include hudbase.h with only the essentials so we don't run into redinition issues. Now CHudHealth can be
//included anywhere in hud.h instead, like at the top for neatness.
class CHudHealth: public CHudBase
{
public:

	//MODDD - constructor given.
	CHudHealth(void);

	virtual int Init( void );
	virtual int VidInit( void );
	virtual int Draw(float fTime);
	virtual void Reset( void );
	int MsgFunc_Health(const char *pszName,  int iSize, void *pbuf);
	int MsgFunc_Damage(const char *pszName,  int iSize, void *pbuf);

	//MODDD - new
	int MsgFunc_Drowning(const char *pszName,  int iSize, void *pbuf);
	int MsgFunc_HUDItemFsh(const char *pszName,  int iSize, void *pbuf);


	int m_iHealth;
	int m_HUD_dmg_bio;
	int m_HUD_cross;
	float m_fAttackFront, m_fAttackRear, m_fAttackLeft, m_fAttackRight;
	float m_fAttackFrontDamage, m_fAttackRearDamage, m_fAttackLeftDamage, m_fAttackRightDamage;
	void GetPainColor( int &r, int &g, int &b );
	float m_fFade;

	//MODDD - new
	void deriveColorFromHealth(int &r, int &g, int &b, int &a);
	void deriveColorFromHealth(int &r, int &g, int &b);
	void getPainColorMode(int mode, int& r, int& g, int& b);
	
private:
	SpriteHandle_t m_SpriteHandle_t;
	SpriteHandle_t m_hDamage;
	
	DAMAGE_IMAGE m_dmg[NUM_DMG_TYPES];
	int	m_bitsDamage;
	//MODDD - complimentary.
	int m_bitsDamageMod;
	int DrawPain(float fTime);
	int DrawDamage(float fTime);
	int DrawItemFlash(float flTime);

	//MODDD - now accepts the damage done too.
	void CalcDamageDirection(vec3_t vecFrom, int damageAmount, int rawDamageAmount);
	
	//MODDD - new
	void setUniformDamage(float damageAmount);
	
	void drawTimedDamageIcon(int arg_index, const int& r, const int& g, const int& b);
	void drawTimedDamageIcon(int arg_index, int arg_draw_x, int arg_draw_y, const int& r, const int& g, const int& b);


	//MODDD
	//void UpdateTiles(float fTime, long bits);
	void UpdateTiles(float fTime, long bits, long bitsMod);
	int drowning;  //bool not available?  just use "int".

	int itemFlashGiven;
	int itemFlashColorStartR;
	int itemFlashColorStartG;
	int itemFlashColorStartB;
	float itemFlashCumulative;
	


};	



#endif //END OF HEALTH_H
