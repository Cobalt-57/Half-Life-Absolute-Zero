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

#include "util.h"
#include "cbase.h"
#include "animating.h"




class CRopeSample;


/**
*	Represents a visible rope segment.
*	Segments require models to define an attachment 0 that, when the segment origin is subtracted from it, represents the length of the segment.
*/
class CRopeSegment : public CBaseAnimating
{
public:
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	CRopeSegment();

	void Precache();
	void Spawn();
	void Think();
	void Touch( CBaseEntity* pOther );
	static CRopeSegment* CreateSegment( CRopeSample* pSample, string_t iszModelName );
	CRopeSample* GetSample() { return m_pSample; }
	void ApplyExternalForce( const Vector& vecForce );
	void SetMassToDefault();
	void SetDefaultMass( const float flDefaultMass );
	void SetMass( const float flMass );
	void SetCauseDamageOnTouch( const BOOL bCauseDamage );
	void SetCanBeGrabbed( const BOOL bCanBeGrabbed );

private:
	CRopeSample* m_pSample;
	string_t m_iszModelName;
	float m_flDefaultMass;
	BOOL m_bCauseDamage;
	BOOL m_bCanBeGrabbed;
};
