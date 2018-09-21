/***
*
*	Copyright (c) 1996-2001, Valve LLC. All rights reserved.
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

#include "CRope.h"

/**
*	An electrified wire.
*	Can be toggled on and off. Starts on.
*/
class CElectrifiedWire : public CRope
{
public:
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	CElectrifiedWire();
	
	void KeyValue( KeyValueData* pkvd );
	void Precache();
	void Spawn();
	void Think();

	void Use( CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float flValue );
	BOOL IsActive() const { return m_bIsActive; }
	BOOL ShouldDoEffect( const int iFrequency );
	void DoSpark( const size_t uiSegment, const BOOL bExertForce );
	void DoLightning();

private:
	BOOL m_bIsActive;

	int m_iTipSparkFrequency;
	int m_iBodySparkFrequency;
	int m_iLightningFrequency;

	int m_iXJoltForce;
	int m_iYJoltForce;
	int m_iZJoltForce;

	size_t m_uiNumUninsulatedSegments;
	size_t m_uiUninsulatedSegments[ MAX_SEGMENTS ];

	int m_iLightningSprite;

	float m_flLastSparkTime;
};

