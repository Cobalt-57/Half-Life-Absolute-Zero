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


#ifndef ROPE_H
#define ROPE_H


#include "util.h"
#include "cbase.h"
#include "basetoggle.h"




class CRopeSegment;
class CRopeSample;

struct RopeSampleData;

/**
*	Represents a spring that keeps samples a given distance apart.
*/
struct Spring
{
	size_t p1;
	size_t p2;
	float restLength;
	float hookConstant;
	float springDampning;
};

static const size_t MAX_SEGMENTS = 63;
static const size_t MAX_SAMPLES = 64;
static const size_t MAX_TEMP_SAMPLES = 5;

/**
*	A rope with a number of segments.
*	Uses an RK4 integrator with dampened springs to simulate rope physics.
*/
class CRope : public CBaseDelay
{
public:

	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];


	CRope();
	~CRope();

	void KeyValue( KeyValueData* pkvd );
	void Precache();
	void Spawn();
	void Think();
	void Touch( CBaseEntity* pOther );

	void InitializeRopeSim();
	void InitializeSprings( const size_t uiNumSprings );
	void RunSimOnSamples();
	void ComputeForces( RopeSampleData* pSystem );
	void ComputeForces( CRopeSample** ppSystem );
	void ComputeSampleForce( RopeSampleData& data );
	void ComputeSpringForce( RopeSampleData& first, RopeSampleData& second, const Spring& spring );
	void RK4Integrate( const float flDeltaTime, CRopeSample** ppSampleSource, CRopeSample** ppSampleTarget );
	void TraceModels( CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs );
	void SetRopeSegments( const size_t uiNumSegments, 
						  CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs );
	BOOL MoveUp( const float flDeltaTime );
	BOOL MoveDown( const float flDeltaTime );
	Vector GetAttachedObjectsVelocity() const;
	void ApplyForceFromPlayer( const Vector& vecForce );
	void ApplyForceToSegment( const Vector& vecForce, const size_t uiSegment );
	void AttachObjectToSegment( CRopeSegment* pSegment );
	void DetachObject();
	BOOL IsObjectAttached() const { return m_bObjectAttached; }
	BOOL IsAcceptingAttachment() const;
	size_t GetNumSegments() const { return m_uiSegments; }
	CRopeSegment** GetSegments() { return seg; }
	CRopeSegment** GetAltSegments() { return altseg; }
	BOOL GetToggleValue() const { return m_bToggle; }
	BOOL IsSoundAllowed() const { return m_bMakeSound; }
	void SetSoundAllowed( const bool bAllowed )
	{
		m_bMakeSound = bAllowed;
	}
	BOOL ShouldCreak() const;
	void Creak();
	string_t GetBodyModel() const { return m_iszBodyModel; }
	string_t GetEndingModel() const { return m_iszEndingModel; }
	float GetSegmentLength( size_t uiSegmentIndex ) const;
	float GetRopeLength() const;
	Vector GetRopeOrigin() const;
	BOOL IsValidSegmentIndex( const size_t uiSegment ) const;
	Vector GetSegmentOrigin( const size_t uiSegment ) const;
	Vector GetSegmentAttachmentPoint( const size_t uiSegment ) const;
	void SetAttachedObjectsSegment( CRopeSegment* pSegment );
	Vector GetSegmentDirFromOrigin( const size_t uiSegmentIndex ) const;
	Vector GetAttachedObjectsPosition() const;

private:
	size_t m_uiSegments;

	CRopeSegment* seg[ MAX_SEGMENTS ];
	CRopeSegment* altseg[ MAX_SEGMENTS ];

	BOOL m_bToggle;

	BOOL m_bInitialDeltaTime;

	float m_flLastTime;

	Vector m_vecLastEndPos;
	Vector m_vecGravity;
	float m_flHookConstant;
	float m_flSpringDampning;

	CRopeSample* m_CurrentSys[ MAX_SAMPLES ];
	CRopeSample* m_TargetSys[ MAX_SAMPLES ];
	RopeSampleData* m_TempSys[ MAX_TEMP_SAMPLES ];

	size_t m_uiNumSamples;

	Spring* m_pSprings;

	size_t m_SpringCnt;

	BOOL m_bSpringsInitialized;

	BOOL m_bObjectAttached;

	size_t m_uiAttachedObjectsSegment;
	float m_flAttachedObjectsOffset;
	float m_flDetachTime;

	string_t m_iszBodyModel;
	string_t m_iszEndingModel;

	BOOL m_bDisallowPlayerAttachment;

	BOOL m_bMakeSound;
};



#endif // ROPE_H
