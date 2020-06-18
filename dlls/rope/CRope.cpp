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
#include <utility>

#include "extdll.h"
#include "util.h"
#include "cbase.h"

#include "gamerules.h"

#include "CRopeSample.h"
#include "CRopeSegment.h"

#include "CRope.h"

static const char* const g_pszCreakSounds[] = 
{
	"items/rope/rope1.wav",
	"items/rope/rope2.wav",
	"items/rope/rope3.wav"
};



//MODDD TODO: restore save functionality?
TYPEDESCRIPTION	CRope::m_SaveData[] =
{
	DEFINE_FIELD( CRope, m_uiSegments, FIELD_INTEGER ),
	DEFINE_FIELD( CRope, m_bToggle, FIELD_BOOLEAN ),
	DEFINE_FIELD( CRope, m_bInitialDeltaTime, FIELD_BOOLEAN ),
	DEFINE_FIELD( CRope, m_flLastTime, FIELD_TIME ),
	DEFINE_FIELD( CRope, m_vecLastEndPos, FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( CRope, m_vecGravity, FIELD_VECTOR ),
	DEFINE_FIELD( CRope, m_flHookConstant, FIELD_FLOAT ),
	DEFINE_FIELD( CRope, m_flSpringDampning, FIELD_FLOAT ),
	DEFINE_FIELD( CRope, m_uiNumSamples, FIELD_INTEGER ),
	DEFINE_FIELD( CRope, m_SpringCnt, FIELD_INTEGER ),
	DEFINE_FIELD( CRope, m_bObjectAttached, FIELD_BOOLEAN ),
	DEFINE_FIELD( CRope, m_uiAttachedObjectsSegment, FIELD_INTEGER ),
	DEFINE_FIELD( CRope, m_flDetachTime, FIELD_TIME ),
	DEFINE_ARRAY( CRope, seg, FIELD_CLASSPTR, MAX_SEGMENTS ),
	DEFINE_ARRAY( CRope, altseg, FIELD_CLASSPTR, MAX_SEGMENTS ),
	DEFINE_ARRAY( CRope, m_CurrentSys, FIELD_CLASSPTR, MAX_SAMPLES ),
	DEFINE_ARRAY( CRope, m_TargetSys, FIELD_CLASSPTR, MAX_SAMPLES ),
	DEFINE_FIELD( CRope, m_bDisallowPlayerAttachment, FIELD_BOOLEAN ),
	DEFINE_FIELD( CRope, m_iszBodyModel, FIELD_STRING ),
	DEFINE_FIELD( CRope, m_iszEndingModel, FIELD_STRING ),
	DEFINE_FIELD( CRope, m_flAttachedObjectsOffset, FIELD_FLOAT ),
	DEFINE_FIELD( CRope, m_bMakeSound, FIELD_BOOLEAN ),
};
//IMPLEMENT_SAVE(CRope, CBaseDelay);




LINK_ENTITY_TO_CLASS( env_rope, CRope );

CRope::CRope()
{
	m_iszBodyModel = MAKE_STRING( "models/rope16.mdl" );
	m_iszEndingModel = MAKE_STRING( "models/rope16.mdl" );
}

CRope::~CRope()
{
	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		delete[] m_TempSys[ uiIndex ];
		m_TempSys[ uiIndex ] = NULL;
	}

	delete[] m_pSprings;
}

void CRope::KeyValue( KeyValueData* pkvd )
{
	if( FStrEq( pkvd->szKeyName, "segments" ) )
	{
		pkvd->fHandled = TRUE;

		m_uiSegments = strtoul( pkvd->szValue, NULL, 10 );

		if( m_uiSegments >= MAX_SEGMENTS )
			m_uiSegments = MAX_SEGMENTS - 1;

	}
	else if( FStrEq( pkvd->szKeyName, "bodymodel" ) )
	{
		pkvd->fHandled = TRUE;

		m_iszBodyModel = ALLOC_STRING( pkvd->szValue );
	}
	else if( FStrEq( pkvd->szKeyName, "endingmodel" ) )
	{
		pkvd->fHandled = TRUE;

		m_iszEndingModel = ALLOC_STRING( pkvd->szValue );
	}
	else if( FStrEq( pkvd->szKeyName, "disable" ) )
	{
		pkvd->fHandled = TRUE;

		m_bDisallowPlayerAttachment = strtol( pkvd->szValue, NULL, 10 ) != 0;
	}
	else
		CBaseDelay::KeyValue( pkvd );
}

void CRope::Precache()
{
	CBaseDelay::Precache();

	UTIL_PrecacheOther( "rope_segment" );
	UTIL_PrecacheOther( "rope_sample" );

	PRECACHE_SOUND_ARRAY( g_pszCreakSounds );
}

void CRope::Spawn()
{
	m_bMakeSound = TRUE;

	Precache();

	m_bSpringsInitialized = FALSE;

	m_vecGravity.x = m_vecGravity.y = 0;
	m_vecGravity.z = -50;

	m_bObjectAttached = FALSE;

	// Was GetFlags()
	pev->flags |= FL_ALWAYSTHINK;
	m_uiNumSamples = m_uiSegments + 1;

	for( size_t uiSample = 0; uiSample < m_uiNumSamples; ++uiSample )
	{
		m_CurrentSys[ uiSample ] = CRopeSample::CreateSample();
		m_CurrentSys[ uiSample ]->SetMasterRope( this );
	}

	memset( m_CurrentSys + m_uiNumSamples, 0, sizeof( CRopeSample* ) * ( MAX_SAMPLES - m_uiNumSamples ) );

	{
		CRopeSegment* pSegment = seg[ 0 ] = CRopeSegment::CreateSegment( m_CurrentSys[ 0 ], GetBodyModel() );

		pSegment->SetAbsOrigin( GetAbsOrigin() );

		pSegment = altseg[ 0 ] = CRopeSegment::CreateSegment( m_CurrentSys[ 0 ], GetBodyModel() );

		pSegment->SetAbsOrigin( GetAbsOrigin() );

	}

	Vector origin;
	Vector angles;

	const Vector vecGravity = m_vecGravity.Normalize();

	if( m_uiSegments > 2 )
	{
		//CRopeSample** ppCurrentSys = m_CurrentSys;

		for( size_t uiSeg = 1; uiSeg < m_uiSegments - 1; ++uiSeg )
		{
			CRopeSample* pSegSample = m_CurrentSys[ uiSeg ];
			seg[ uiSeg ] = CRopeSegment::CreateSegment( pSegSample, GetBodyModel() );

			altseg[ uiSeg ] = CRopeSegment::CreateSegment( pSegSample, GetBodyModel() );

			CRopeSegment* pCurrent = seg[ uiSeg - 1 ];

			pCurrent->GetAttachment( 0, origin, angles );
			ALERT( at_console, "Segments is as follows: pCurrent->pev->origin is %.2f %.2f %.2f\n", pCurrent->pev->origin.x, pCurrent->pev->origin.y, pCurrent->pev->origin.z);

			Vector vecPos = origin - pCurrent->GetAbsOrigin();

			const float flLength = vecPos.Length();

			origin = flLength * vecGravity + pCurrent->GetAbsOrigin();

			seg[ uiSeg ]->SetAbsOrigin( origin );
			altseg[ uiSeg ]->SetAbsOrigin( origin );
		}
	}

	CRopeSample* pSegSample = m_CurrentSys[ m_uiSegments - 1 ];
	seg[ m_uiSegments - 1 ] = CRopeSegment::CreateSegment( pSegSample, GetEndingModel() );

	altseg[ m_uiSegments - 1 ] = CRopeSegment::CreateSegment( pSegSample, GetEndingModel() );

	CRopeSegment* pCurrent = seg[ m_uiSegments - 2 ];

	pCurrent->GetAttachment( 0, origin, angles );

	Vector vecPos = origin - pCurrent->GetAbsOrigin();

	const float flLength = vecPos.Length();

	origin = flLength * vecGravity + pCurrent->GetAbsOrigin();

	seg[ m_uiSegments - 1 ]->SetAbsOrigin( origin );
	altseg[ m_uiSegments - 1 ]->SetAbsOrigin( origin );

	memset( seg + m_uiSegments, 0, sizeof( CRopeSegment* ) * ( MAX_SEGMENTS - m_uiSegments ) );
	memset( altseg + m_uiSegments, 0, sizeof( CRopeSegment* ) * ( MAX_SEGMENTS - m_uiSegments ) );

	memset( m_TempSys, 0, sizeof( m_TempSys ) );

	m_SpringCnt = 0;

	m_bInitialDeltaTime = TRUE;
	m_flHookConstant = 2500;
	m_flSpringDampning = 0.1;

	InitializeRopeSim();

	// Used to be SetThink()
	pev->nextthink = gpGlobals->time + 0.01;
}

void CRope::Think()
{
	if( !m_bSpringsInitialized )
	{
		InitializeSprings( m_uiSegments );
	}

	m_bToggle = !m_bToggle;

	RunSimOnSamples();

	CRopeSegment** ppPrimarySegs;
	CRopeSegment** ppHiddenSegs;

	if( m_bToggle )
	{
		ppPrimarySegs = altseg;
		ppHiddenSegs = seg;
	}
	else
	{
		ppPrimarySegs = seg;
		ppHiddenSegs = altseg;
	}

	SetRopeSegments( m_uiSegments, ppPrimarySegs, ppHiddenSegs );

	if( ShouldCreak() )
	{
		Creak();
	}

	// Used to be NextThink();
	pev->nextthink = gpGlobals->time + 0.001;
}

void CRope::Touch( CBaseEntity* pOther )
{
	//Nothing.
}



//generic save implementation.
BOOL CRope::Save( CSave &save )
{
	if ( !CBaseDelay::Save(save) )
		return FALSE;
	return save.WriteFields( "CRope", this, m_SaveData, ARRAYSIZE(m_SaveData) );
}

BOOL CRope::Restore( CRestore& restore )
{
	if( !CBaseDelay::Restore( restore ) )
	{
		return FALSE;
	}
	
	//MODDD NEW - Should we read from CRope on restore?
	BOOL readFieldsStatus = restore.ReadFields( "CRope", this, m_SaveData, ARRAYSIZE(m_SaveData) );


	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		m_TempSys[ uiIndex ] = new RopeSampleData[ m_uiNumSamples ];
	}

	m_bSpringsInitialized = FALSE;
	m_bInitialDeltaTime = TRUE;

	//return TRUE;
	return readFieldsStatus;
}

void CRope::InitializeRopeSim()
{
	for( size_t uiIndex = 0; uiIndex < MAX_TEMP_SAMPLES; ++uiIndex )
	{
		delete[] m_TempSys[ uiIndex ];
		m_TempSys[ uiIndex ] = NULL;
	}

	for( size_t uiSample = 0; uiSample < m_uiNumSamples; ++uiSample )
	{
		m_TargetSys[ uiSample ] = CRopeSample::CreateSample();

		m_TargetSys[ uiSample ]->SetMasterRope( this );
	}

	memset( m_TargetSys + m_uiNumSamples, 0, sizeof( CRopeSample* ) * ( MAX_SAMPLES - m_uiNumSamples ) );

	for( size_t uiIndex2 = 0; uiIndex2 < MAX_TEMP_SAMPLES; ++uiIndex2 )
	{
		m_TempSys[ uiIndex2 ] = new RopeSampleData[ m_uiNumSamples ];

		memset( m_TempSys[ uiIndex2 ], 0, sizeof( RopeSampleData ) * m_uiNumSamples );
	}

	for( size_t uiSeg = 0; uiSeg < m_uiSegments; ++uiSeg )
	{
		CRopeSegment* pSegment = seg[ uiSeg ];
		CRopeSample* pSample = pSegment->GetSample();

		struct RopeSampleData& data = pSample->GetData();

		data.mPosition = pSegment->GetAbsOrigin();
		//ALERT( at_console, "Samples is as follows: origin is %.2f %.2f %.2f\n", data.mPosition.x, data.mPosition.y, data.mPosition.z);
		data.mVelocity				= g_vecZero;
		data.mForce					= g_vecZero;
		data.mMassReciprocal		= 1;
		data.mApplyExternalForce	= FALSE;
		data.mExternalForce			= g_vecZero;

		pSegment->SetDefaultMass( data.mMassReciprocal );
	}

	{
		//Zero out the anchored segment's mass so it stays in place.
		CRopeSample * pSample = m_CurrentSys[ 0 ];

		pSample->GetData().mMassReciprocal = 0;
	}

	CRopeSegment* pSegment = seg[ m_uiSegments - 1 ];

	Vector vecOrigin, vecAngles;

	pSegment->GetAttachment( 0, vecOrigin, vecAngles );

	Vector vecDistance = vecOrigin - pSegment->GetAbsOrigin();

	const float flLength = vecDistance.Length();

	const Vector vecGravity = m_vecGravity.Normalize();

	vecOrigin = vecGravity * flLength + pSegment->GetAbsOrigin();

	CRopeSample* pSample = m_CurrentSys[ m_uiNumSamples - 1 ];

	struct RopeSampleData& data = pSample->GetData();

	data.mPosition = vecOrigin;
	//ALERT( at_console, "Samples is as follows: origin is %.2f %.2f %.2f\n", data.mPosition.x, data.mPosition.y, data.mPosition.z);

	m_vecLastEndPos = vecOrigin;

	data.mVelocity = g_vecZero;

	data.mForce = g_vecZero;

	data.mMassReciprocal = 0.2;

	data.mApplyExternalForce = FALSE;

	size_t uiNumSegs = 4;

	if( m_uiSegments <= 4 )
		uiNumSegs = m_uiSegments;

	for( size_t uiIndex3 = 0; uiIndex3 < uiNumSegs; ++uiIndex3 )
	{
		seg[ uiIndex3 ]->SetCanBeGrabbed( FALSE );
		altseg[ uiIndex3 ]->SetCanBeGrabbed( FALSE );
	}
}

void CRope::InitializeSprings( const size_t uiNumSprings )
{
	m_SpringCnt = uiNumSprings;

	m_pSprings = new Spring[ uiNumSprings ];

	if( uiNumSprings > 0 )
	{
		Vector vecOrigin, vecAngles;

		for( size_t uiIndex = 0; uiIndex < m_SpringCnt; ++uiIndex )
		{
			Spring& spring = m_pSprings[ uiIndex ];

			spring.p1 = uiIndex;
			spring.p2 = uiIndex + 1;

			if( uiIndex < m_uiSegments )
			{
				CRopeSegment* pSegment = seg[ uiIndex ];

				pSegment->GetAttachment( 0, vecOrigin, vecAngles );

				spring.restLength = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
			}
			else
				spring.restLength = 0;

			spring.hookConstant = m_flHookConstant;
			spring.springDampning = m_flSpringDampning;
		}
	}

	m_bSpringsInitialized = TRUE;
}



void CRope::RunSimOnSamples()
{
	float flDeltaTime = 0.025;

	if( m_bInitialDeltaTime )
	{
		m_bInitialDeltaTime = FALSE;
		m_flLastTime = gpGlobals->time;
		flDeltaTime = 0;
	}

	size_t uiIndex = 0;

	CRopeSample** ppSampleSource = m_CurrentSys;
	CRopeSample** ppSampleTarget = m_TargetSys;


	//MODDD - See what changing this does?
	int maxTimes = 6;

	while( TRUE )
	{
		maxTimes--;
		++uiIndex;

		ComputeForces( ppSampleSource );
		RK4Integrate( flDeltaTime, ppSampleSource, ppSampleTarget );

		//edited way. ?
		//if( ( uiIndex % 2 ) != 0 )
		//{
		//	m_flLastTime += 0.007;
		//	break;
		//}
		
		m_flLastTime += 0.007;

		if( gpGlobals->time <= m_flLastTime || maxTimes <= 0 )
		{
			if( ( uiIndex % 2 ) != 0 )
				break;
		}

		CRopeSample** ppSampleTemp = ppSampleSource;
		ppSampleSource = ppSampleTarget;
		ppSampleTarget = ppSampleTemp;
		//std::swap( ppSampleSource, ppSampleTarget );
	}

	m_flLastTime = gpGlobals->time;
}




void CRope::ComputeForces( RopeSampleData* pSystem )
{
	for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex )
	{
		ComputeSampleForce( pSystem[ uiIndex ] );
	}

	Spring* pSpring = m_pSprings;

	for( size_t uiIndex2 = 0; uiIndex2 < m_SpringCnt; ++uiIndex2, ++pSpring )
	{
		ComputeSpringForce( pSystem[ pSpring->p1 ], pSystem[ pSpring->p2 ], *pSpring );
	}
}

void CRope::ComputeForces( CRopeSample** ppSystem )
{
	for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex )
	{
		ComputeSampleForce( ppSystem[ uiIndex ]->GetData() );
	}

	Spring* pSpring = m_pSprings;

	for( size_t uiIndex2 = 0; uiIndex2 < m_SpringCnt; ++uiIndex2, ++pSpring )
	{
		ComputeSpringForce( ppSystem[ pSpring->p1 ]->GetData(), ppSystem[ pSpring->p2 ]->GetData(), *pSpring );
	}
}

void CRope::ComputeSampleForce( RopeSampleData& data )
{
	data.mForce = g_vecZero;

	if( data.mMassReciprocal != 0.0 )
	{
		data.mForce = data.mForce + ( m_vecGravity / data.mMassReciprocal );
	}

	if( data.mApplyExternalForce )
	{
		data.mForce = data.mForce + data.mExternalForce;

		data.mExternalForce = g_vecZero;
		data.mApplyExternalForce = FALSE;
	}

	if( DotProduct( m_vecGravity, data.mVelocity ) >= 0 )
	{
		data.mForce = data.mForce + data.mVelocity * -0.04;
	}
	else
	{
		data.mForce = data.mForce - data.mVelocity;
	}
}

void CRope::ComputeSpringForce( RopeSampleData& first, RopeSampleData& second, const Spring& spring )
{
	Vector vecDist = first.mPosition - second.mPosition;

	const double flDistance = vecDist.Length();

	const double flForce = ( flDistance - spring.restLength ) * spring.hookConstant;

	const double flNewRelativeDist = DotProduct( first.mVelocity - second.mVelocity, vecDist ) * spring.springDampning;

	vecDist.NormalizeInPlace();

	const double flSpringFactor = -( flNewRelativeDist / flDistance + flForce );

	const Vector vecForce = flSpringFactor * vecDist;

	first.mForce = first.mForce + vecForce;

	second.mForce = second.mForce - vecForce;
}

void CRope::RK4Integrate( const float flDeltaTime, CRopeSample** ppSampleSource, CRopeSample** ppSampleTarget )
{
	const float flDeltas[ MAX_TEMP_SAMPLES - 1 ] = 
	{
		flDeltaTime * 0.5f,
		flDeltaTime * 0.5f,
		flDeltaTime * 0.5f,
		flDeltaTime
	};

	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ 1 ];

		for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			//TAGGG - changed the order of the & and const.
			// VS6 complained about the order.
			struct RopeSampleData const& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * data.mForce * flDeltas[ 0 ];

			pTemp2->mVelocity = data.mVelocity * flDeltas[ 0 ];

			pTemp1->mMassReciprocal = data.mMassReciprocal;
			pTemp1->mVelocity = data.mVelocity + pTemp2->mForce;
			pTemp1->mPosition = data.mPosition + pTemp2->mVelocity;
		}

		ComputeForces( m_TempSys[ 0 ] );
	}

	for( size_t uiStep = 2; uiStep < MAX_TEMP_SAMPLES - 1; ++uiStep )
	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ uiStep ];

		for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			// const
			struct RopeSampleData& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * pTemp1->mForce * flDeltas[ uiStep - 1 ];

			pTemp2->mVelocity = pTemp1->mVelocity * flDeltas[ uiStep - 1 ];

			pTemp1->mMassReciprocal = data.mMassReciprocal;
			pTemp1->mVelocity = data.mVelocity + pTemp2->mForce;
			pTemp1->mPosition = data.mPosition + pTemp2->mVelocity;
			//ALERT( at_console, "Computed Position is as follows: origin is %.2f %.2f %.2f\n", data.mPosition.x, data.mPosition.y, data.mPosition.z);
		}

		ComputeForces( m_TempSys[ 0 ] );
	}

	{
		RopeSampleData* pTemp1 = m_TempSys[ 0 ];
		RopeSampleData* pTemp2 = m_TempSys[ 4 ];

		for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex, ++pTemp1, ++pTemp2 )
		{
			//TAGGG - VS6 complained about the order of "&" and const.
			struct RopeSampleData const& data = ppSampleSource[ uiIndex ]->GetData();

			pTemp2->mForce = data.mMassReciprocal * pTemp1->mForce * flDeltas[ 3 ];

			pTemp2->mVelocity = pTemp1->mVelocity * flDeltas[ 3 ];
		}
	}

	RopeSampleData* pTemp1 = m_TempSys[ 1 ];
	RopeSampleData* pTemp2 = m_TempSys[ 2 ];
	RopeSampleData* pTemp3 = m_TempSys[ 3 ];
	RopeSampleData* pTemp4 = m_TempSys[ 4 ];

	for( size_t uiIndex = 0; uiIndex < m_uiNumSamples; ++uiIndex, ++pTemp1, ++pTemp2, ++pTemp3, ++pTemp4 )
	{
		CRopeSample * pSource = ppSampleSource[ uiIndex ];
		CRopeSample * pTarget = ppSampleTarget[ uiIndex ];

		const Vector vecPosChange = 1.0f / 6.0f * ( pTemp1->mVelocity + ( pTemp2->mVelocity + pTemp3->mVelocity ) * 2 + pTemp4->mVelocity );

		const Vector vecVelChange = 1.0f / 6.0f * ( pTemp1->mForce + ( pTemp2->mForce + pTemp3->mForce ) * 2 + pTemp4->mForce );

		pTarget->GetData().mPosition = pSource->GetData().mPosition + ( vecPosChange );//* flDeltaTime );

		pTarget->GetData().mVelocity = pSource->GetData().mVelocity + ( vecVelChange );//* flDeltaTime );
	}
}

//TODO move to common header - Solokiller
static const Vector DOWN( 0, 0, -1 );

static const Vector RIGHT( 0, 1, 0 );

void GetAlignmentAngles( const Vector& vecTop, const Vector& vecBottom, Vector& vecOut )
{
	Vector vecDist = vecBottom - vecTop;

	Vector vecResult = vecDist.Normalize();

	const float flRoll = acos( DotProduct( vecResult, RIGHT ) ) * ( 180.0 / M_PI );

	vecOut.z = -flRoll;

	vecDist.y = 0;

	vecResult = vecDist.Normalize();

	const float flPitch = acos( DotProduct( vecResult, DOWN ) ) * ( 180.0 / M_PI );

	vecOut.x = ( vecResult.x >= 0.0 ) ? flPitch : -flPitch;
	vecOut.y = 0;
}

void TruncateEpsilon( Vector& vec )
{
	vec = ( vec * 10.0 + 0.5 * Vector(0.5,0.5,0.5) ) / 10.0;
}

void CRope::TraceModels( CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs )
{
	if( m_uiSegments > 1 )
	{
		Vector vecAngles;

		GetAlignmentAngles( 
			m_CurrentSys[ 0 ]->GetData().mPosition, 
			m_CurrentSys[ 1 ]->GetData().mPosition, 
			vecAngles );

		( *ppPrimarySegs )->SetAbsAngles( vecAngles );
	}

	TraceResult tr;

	if( m_bObjectAttached )
	{
		for( size_t uiSeg = 1; uiSeg < m_uiSegments; ++uiSeg )
		{
			CRopeSample* pSample = m_CurrentSys[ uiSeg ];

			Vector vecDist = pSample->GetData().mPosition - ppHiddenSegs[ uiSeg ]->GetAbsOrigin();
			
			vecDist.NormalizeInPlace();

			const float flTraceDist = ( uiSeg - m_uiAttachedObjectsSegment + 2 ) < 5 ? 50 : 10;

			const Vector vecTraceDist = vecDist * flTraceDist;

			const Vector vecEnd = pSample->GetData().mPosition + vecTraceDist;

			UTIL_TraceLine( ppHiddenSegs[ uiSeg ]->GetAbsOrigin(), vecEnd, ignore_monsters, edict(), &tr );
			
			if( tr.flFraction == 1.0 && tr.fAllSolid )
			{
				break;
			}

			if( tr.flFraction != 1.0 || tr.fStartSolid || !tr.fInOpen )
			{
				Vector vecOrigin = tr.vecEndPos - vecTraceDist;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
				
				Vector vecNormal = tr.vecPlaneNormal.Normalize() * 20000.0;

				struct RopeSampleData& data = ppPrimarySegs[ uiSeg ]->GetSample()->GetData();

				data.mApplyExternalForce = TRUE;

				data.mExternalForce = vecNormal;

				data.mVelocity = g_vecZero;
			}
			else
			{
				Vector vecOrigin = pSample->GetData().mPosition;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
			}
		}
	}
	else
	{
		for( size_t uiSeg = 1; uiSeg < m_uiSegments; ++uiSeg )
		{
			UTIL_TraceLine( 
				ppHiddenSegs[ uiSeg ]->GetAbsOrigin(),
				m_CurrentSys[ uiSeg ]->GetData().mPosition, 
				ignore_monsters, edict(), &tr );

			if( tr.flFraction == 1.0 )
			{
				Vector vecOrigin = m_CurrentSys[ uiSeg ]->GetData().mPosition;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );
			}
			else
			{
				//CBaseEntity* pEnt = GET_PRIVATE( tr.pHit );
				const Vector vecNormal = tr.vecPlaneNormal.Normalize();

				Vector vecOrigin = tr.vecEndPos + vecNormal * 10.0;

				TruncateEpsilon( vecOrigin );

				ppPrimarySegs[ uiSeg ]->SetAbsOrigin( vecOrigin );

				ppPrimarySegs[ uiSeg ]->GetSample()->GetData().mApplyExternalForce = TRUE;

				ppPrimarySegs[ uiSeg ]->GetSample()->GetData().mExternalForce = vecNormal * 40000.0;
			}
		}
	}

	Vector vecAngles;

	for( size_t uiSeg = 1; uiSeg < m_uiSegments; ++uiSeg )
	{
		CRopeSegment * pSegment = ppPrimarySegs[ uiSeg - 1 ];
		CRopeSegment *  pSegment2 = ppPrimarySegs[ uiSeg ];

		GetAlignmentAngles( pSegment->GetAbsOrigin(), pSegment2->GetAbsOrigin(), vecAngles );

		pSegment->SetAbsAngles( vecAngles );
	}

	if( m_uiSegments > 1 )
	{
		CRopeSample * pSample = m_CurrentSys[ m_uiNumSamples - 1 ];

		UTIL_TraceLine( m_vecLastEndPos, pSample->GetData().mPosition, ignore_monsters, edict(), &tr );
	
		if( tr.flFraction == 1.0 )
		{
			m_vecLastEndPos = pSample->GetData().mPosition;
		}
		else
		{
			m_vecLastEndPos = tr.vecEndPos;

			pSample->GetData().mApplyExternalForce = TRUE;

			pSample->GetData().mExternalForce = tr.vecPlaneNormal.Normalize() * 40000.0;
		}

		CRopeSegment * pSegment = ppPrimarySegs[ m_uiNumSamples - 2 ];

		GetAlignmentAngles( pSegment->GetAbsOrigin(), m_vecLastEndPos, vecAngles );

		pSegment->SetAbsAngles( vecAngles );
	}
}

void CRope::SetRopeSegments( const size_t uiNumSegments, 
							 CRopeSegment** ppPrimarySegs, CRopeSegment** ppHiddenSegs )
{
	if( uiNumSegments > 0 )
	{
		TraceModels( ppPrimarySegs, ppHiddenSegs );

		CRopeSegment** ppVisible = ppPrimarySegs;
		CRopeSegment** ppActualHidden = ppHiddenSegs;

		//In multiplayer, the constant toggling of visible segments makes them completely invisible.
		//So always make the seg segments visible. - Solokiller
		if( m_bToggle && g_pGameRules->IsMultiplayer() )
		{
			CRopeSegment** ppTemp = ppVisible;
			ppVisible = ppActualHidden;
			ppActualHidden = ppTemp;
			//std::swap( ppVisible, ppActualHidden );
		}

		ppVisible[ 0 ]->pev->solid = SOLID_TRIGGER;
		ppVisible[ 0 ]->pev->effects = 0;

		ppActualHidden[ 0 ]->pev->solid = SOLID_NOT;
		ppActualHidden[ 0 ]->pev->effects = EF_NODRAW;

		for( size_t uiIndex = 1; uiIndex < uiNumSegments; ++uiIndex )
		{
			CRopeSegment* pPrim = ppVisible[ uiIndex ];
			CRopeSegment* pHidden = ppActualHidden[ uiIndex ];

			pPrim->pev->solid = SOLID_TRIGGER;
			pPrim->pev->effects = 0;
			
			pHidden->pev->solid = SOLID_NOT;
			pHidden->pev->effects = EF_NODRAW;

			Vector vecOrigin = pPrim->GetAbsOrigin();
			//vecOrigin.x += 10.0;
			//vecOrigin.y += 10.0;

			pHidden->SetAbsOrigin( vecOrigin );
			//ALERT( at_console, "pPrim: %.2f %.2f %.2f\n", pPrim->pev->origin.x, pPrim->pev->origin.y, pPrim->pev->origin.z );
			//ALERT( at_console, "pHidden: %.2f %.2f %.2f\n", pHidden->pev->origin.x, pHidden->pev->origin.y, pHidden->pev->origin.z );
		}
	}
}

BOOL CRope::MoveUp( const float flDeltaTime )
{
	if( m_uiAttachedObjectsSegment > 4 )
	{
		float flDistance = flDeltaTime * 128.0;

		Vector vecOrigin, vecAngles;

		while( TRUE )
		{
			float flOldDist = flDistance;

			flDistance = 0;

			if( flOldDist <= 0 )
				break;

			if( m_uiAttachedObjectsSegment <= 3 )
				break;

			if( flOldDist > m_flAttachedObjectsOffset )
			{
				flDistance = flOldDist - m_flAttachedObjectsOffset;

				--m_uiAttachedObjectsSegment;

				float flNewOffset = 0;

				if( m_uiAttachedObjectsSegment < m_uiSegments )
				{
					CRopeSegment * pSegment = seg[ m_uiAttachedObjectsSegment ];

					pSegment->GetAttachment( 0, vecOrigin, vecAngles );

					flNewOffset = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
				}

				m_flAttachedObjectsOffset = flNewOffset;
			}
			else
			{
				m_flAttachedObjectsOffset -= flOldDist;
			}
		}
	}

	ALERT( at_console, "Player moved up.\n" );

	return TRUE;
}

BOOL CRope::MoveDown( const float flDeltaTime )
{
	if( !m_bObjectAttached )
		return FALSE;

	float flDistance = flDeltaTime * 128.0;

	Vector vecOrigin, vecAngles;

	CRopeSegment* pSegment;

	BOOL bOnRope = TRUE;

	BOOL bDoIteration = TRUE;

	while( bDoIteration )
	{
		bDoIteration = FALSE;

		if( flDistance > 0.0 )
		{
			float flNewDist = flDistance;
			float flSegLength = 0.0;

			while( bOnRope )
			{
				if( m_uiAttachedObjectsSegment < m_uiSegments )
				{
					pSegment = seg[ m_uiAttachedObjectsSegment ];

					pSegment->GetAttachment( 0, vecOrigin, vecAngles );

					flSegLength = ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
				}

				const float flOffset = flSegLength - m_flAttachedObjectsOffset;

				if( flNewDist <= flOffset )
				{
					m_flAttachedObjectsOffset += flNewDist;
					flDistance = 0;
					bDoIteration = TRUE;
					break;
				}

				if( m_uiAttachedObjectsSegment + 1 == m_uiSegments )
					bOnRope = FALSE;
				else
					++m_uiAttachedObjectsSegment;

				flNewDist -= flOffset;
				flSegLength = 0;

				m_flAttachedObjectsOffset = 0;

				if( flNewDist <= 0 )
					break;
			}
		}
	}

	ALERT( at_console, "Player moved down.\n" );

	return bOnRope;
}

Vector CRope::GetAttachedObjectsVelocity() const
{
	if( !m_bObjectAttached )
		return g_vecZero;

	return seg[ m_uiAttachedObjectsSegment ]->GetSample()->GetData().mVelocity;
}

void CRope::ApplyForceFromPlayer( const Vector& vecForce )
{
	if( !m_bObjectAttached )
		return;

	float flForce = 20000.0;

	if( m_uiSegments < 26 )
		flForce *= ( m_uiSegments / 26.0 );

	const Vector vecScaledForce = vecForce * flForce;

	ApplyForceToSegment( vecScaledForce, m_uiAttachedObjectsSegment );
}

void CRope::ApplyForceToSegment( const Vector& vecForce, const size_t uiSegment )
{
	if( uiSegment < m_uiSegments )
	{
		seg[ uiSegment ]->ApplyExternalForce( vecForce );
	}
	else if( uiSegment == m_uiSegments )
	{
		//Apply force to the last sample.

		RopeSampleData& data = m_CurrentSys[ uiSegment - 1 ]->GetData();

		data.mExternalForce = data.mExternalForce + vecForce;

		data.mApplyExternalForce = TRUE;
	}
}

void CRope::AttachObjectToSegment( CRopeSegment* pSegment )
{
	m_bObjectAttached = TRUE;

	m_flDetachTime = 0;

	SetAttachedObjectsSegment( pSegment );

	m_flAttachedObjectsOffset = 0;
}

void CRope::DetachObject()
{
	m_bObjectAttached = FALSE;
	m_flDetachTime = gpGlobals->time;
}

BOOL CRope::IsAcceptingAttachment() const
{
	if( gpGlobals->time - m_flDetachTime > 2.0 && !m_bObjectAttached )
	{
		return !m_bDisallowPlayerAttachment;
	}

	return FALSE;
}

BOOL CRope::ShouldCreak() const
{
	if( m_bObjectAttached && m_bMakeSound )
	{
		CRopeSample* pSample = seg[ m_uiAttachedObjectsSegment ]->GetSample();

		if( pSample->GetData().mVelocity.Length() > 20.0 )
		{
			if ( RANDOM_LONG( 1, 5 ) == 1 )
				return TRUE;
			else
				return FALSE;
		}
	}

	return FALSE;
}

void CRope::Creak()
{
	EMIT_SOUND( ENT(pev), CHAN_BODY,
				g_pszCreakSounds[ RANDOM_LONG( 0, ARRAYSIZE( g_pszCreakSounds ) - 1 ) ],
				VOL_NORM, ATTN_NORM );
}

float CRope::GetSegmentLength( size_t uiSegmentIndex ) const
{
	if( uiSegmentIndex < m_uiSegments )
	{
		Vector vecOrigin, vecAngles;

		CRopeSegment *const pSegment = seg[ uiSegmentIndex ];

		pSegment->GetAttachment( 0, vecOrigin, vecAngles );

		return ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
	}

	return 0;
}

float CRope::GetRopeLength() const
{
	float flLength = 0;

	Vector vecOrigin, vecAngles;

	for( size_t uiIndex = 0; uiIndex < m_uiSegments; ++uiIndex )
	{
		CRopeSegment *const pSegment = seg[ uiIndex ];

		pSegment->GetAttachment( 0, vecOrigin, vecAngles );

		flLength += ( pSegment->GetAbsOrigin() - vecOrigin ).Length();
	}

	ALERT( at_console, "Rope length is %.2f\n", flLength );
	return flLength;
}

Vector CRope::GetRopeOrigin() const
{
	return m_CurrentSys[ 0 ]->GetData().mPosition;
}

BOOL CRope::IsValidSegmentIndex( const size_t uiSegment ) const
{
	return uiSegment < m_uiSegments;
}

Vector CRope::GetSegmentOrigin( const size_t uiSegment ) const
{
	if( !IsValidSegmentIndex( uiSegment ) )
	{
		return g_vecZero;
	}

	return m_CurrentSys[ uiSegment ]->GetData().mPosition;
}

Vector CRope::GetSegmentAttachmentPoint( const size_t uiSegment ) const
{
	if( !IsValidSegmentIndex( uiSegment ) )
		return g_vecZero;

	Vector vecOrigin, vecAngles;

	CRopeSegment *const pSegment = m_bToggle ? altseg[ uiSegment ] : seg[ uiSegment ];

	pSegment->GetAttachment( 0, vecOrigin, vecAngles );

	return vecOrigin;
}

void CRope::SetAttachedObjectsSegment( CRopeSegment* pSegment )
{
	for( size_t uiIndex = 0; uiIndex < m_uiSegments; ++uiIndex )
	{
		if( seg[ uiIndex ] == pSegment || altseg[ uiIndex ] == pSegment )
		{
			m_uiAttachedObjectsSegment = uiIndex;
			break;
		}
	}
}

Vector CRope::GetSegmentDirFromOrigin( const size_t uiSegmentIndex ) const
{
	if( uiSegmentIndex >= m_uiSegments )
	{
		ALERT( at_console, "Past maximum segments.\n" );
		return g_vecZero;
	}

	//There is one more sample than there are segments, so this is fine.
	const Vector vecResult = 
		m_CurrentSys[ uiSegmentIndex + 1 ]->GetData().mPosition - 
		m_CurrentSys[ uiSegmentIndex ]->GetData().mPosition;

	return vecResult.Normalize();
}

Vector CRope::GetAttachedObjectsPosition() const
{
	if( !m_bObjectAttached )
	{
		ALERT( at_console, "Object not attached.\n" );
		return g_vecZero;
	}

	Vector vecResult;

	if( m_uiAttachedObjectsSegment < m_uiSegments )
	{
		vecResult = m_CurrentSys[ m_uiAttachedObjectsSegment ]->GetData().mPosition;
	}

	vecResult = vecResult + 
		( m_flAttachedObjectsOffset * GetSegmentDirFromOrigin( m_uiAttachedObjectsSegment ) );

	return vecResult;
}
