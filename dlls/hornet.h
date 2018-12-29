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
//=========================================================
// Hornets
//=========================================================

//=========================================================
// Hornet Defines
//=========================================================
#define HORNET_TYPE_RED			0
#define HORNET_TYPE_ORANGE		1
#define HORNET_RED_SPEED		(float)600
#define HORNET_ORANGE_SPEED		(float)800
#define	HORNET_BUZZ_VOLUME		(float)0.8

extern int iHornetPuff;

//=========================================================
// Hornet - this is the projectile that the Alien Grunt fires.
//=========================================================
class CHornet : public CBaseMonster
{
public:
	CHornet();

	float hornetPseudoNextThink;
	float spiralStartTime;



	//MODDD
	BOOL hornetTouchedAnything;
	Vector vecFlightDirAlttt;
	Vector vecFlightDirTrue;
	Vector vecFlightDirMem;
	static float getDifficultyMod(void);
	static BOOL useSpiral(void);

	void Spawn( void );
	void Precache( void );
	int	 Classify ( void );
	int  IRelationship ( CBaseEntity *pTarget );
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	void IgniteTrail( void );
	void EXPORT StartTrack ( void );
	void EXPORT StartDart ( void );

	//MODDD - new for the kingpin to call.
	void EXPORT StartSpeedMissile(void);
	void EXPORT SpeedMissileDartStart(void);
	void EXPORT SpeedMissileDartContinuous(void);


	void EXPORT TrackTarget ( void );
	void EXPORT TrackTouch ( CBaseEntity *pOther );
	void EXPORT DartTouch( CBaseEntity *pOther );
	void EXPORT DieTouch ( CBaseEntity *pOther );
	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	//MODDD - new.
	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	GENERATE_KILLED_PROTOTYPE
	
	float massInfluence(void);

	float			m_flStopAttack;
	int				m_iHornetType;
	float			m_flFlySpeed;

	//MODDD - new.
	float expireTime;
	Vector speedMissileDartTarget;
	Vector speedMissileDartDirection;

};

