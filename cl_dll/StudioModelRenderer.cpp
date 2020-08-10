//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// studio_model.cpp
// routines for setting up to draw 3DStudio models

#include "external_lib_include.h"
//MODDD - you know the drill
//#include "windows.h"

#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "com_model.h"
#include "studio.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "dlight.h"
#include "triangleapi.h"
#include "studio_util.h"
#include "r_studioint.h"

#include "StudioModelRenderer.h"
#include "GameStudioModelRenderer.h"

// Global engine <-> studio model rendering code interface
engine_studio_api_t IEngineStudio;


//MODDD - This section is new.
/////////////////////////////////////////////////////////
void(*GL_StudioDrawShadow_Old)(void);

__declspec(naked) void DropShadows(void)
{

	_asm
	{
		push ecx;
		jmp[GL_StudioDrawShadow_Old];
	}

}
/////////////////////////////////////////////////////////


//MODDD - extern
EASY_CVAR_EXTERN(chromeEffect)
EASY_CVAR_EXTERN(mirrorsReflectOnlyNPCs)
EASY_CVAR_EXTERN(mirrorsDoNotReflectPlayer)
EASY_CVAR_EXTERN(r_shadows)
EASY_CVAR_EXTERN(cl_interp_entity)
EASY_CVAR_EXTERN(cl_interp_viewmodel)
EASY_CVAR_EXTERN(drawViewModel)
EASY_CVAR_EXTERN(r_glowshell_debug)

extern float global2PSEUDO_IGNOREcameraMode;



//MODDD - Turn on to avoid rendering anything.  Debug ahoy!
// Going off of reload sounds not called by script still playing, events (entity.cpp)
// still happen without rendering.
#define STUDIO_NO_RENDERING 0




// until told otherwise.  Any drawing method should start with this set some way though,
// relying on a previous run's setting is bad
drawtype_e g_drawType = DRAWTYPE_ENTITY;

// For recording the viewmodel.  Only needs to be updated once a frame, no idea how efficient
// the 'GetViewModel()' engine call is.  Rendering needs to be fast.
cl_entity_s* g_viewModelRef = NULL;



// All cases of this:
//    m_pCurrentEntity->curstate.renderfx & ISPLAYER
// replaced with:
//    g_drawType == DRAWTYPE_PLAYER

// and this
//    (m_pCurrentEntity->curstate.renderfx & ISVIEWMODEL) == ISVIEWMODEL
// as well checks between m_pCurrentEntity and gEngfuncs.GetViewModel(),  replaced with
//    g_drawType == DRAWTYPE_VIEWMODEL




// Pretty sure the max number of entities is around 1024, but the game crashes at trying to spawn over 900 anyway.
// Or at least when an index reaches 900.
// MAX_MAP_ENTITIES ???  that constant was set to 1024.  Close enough to the observed 900 anyway, unless that's some
// freak coincidence.
float ary_g_prevTime[1024];
float ary_g_prevFrame[1024];


//static float g_prevTime;
//static float g_prevFrame;
static float g_OLDprevTime;
static float g_debugPrevTime;










/////////////////////
// Implementation of CStudioModelRenderer.h

/*
====================
Init

====================
*/

void CStudioModelRenderer::Init( void )
{
	// Set up some variables shared with engine
	m_pCvarHiModels			= IEngineStudio.GetCvar( "cl_himodels" );
	m_pCvarDeveloper		= IEngineStudio.GetCvar( "developer" );
	m_pCvarDrawEntities		= IEngineStudio.GetCvar( "r_drawentities" );

	m_pChromeSprite			= IEngineStudio.GetChromeSprite();

	IEngineStudio.GetModelCounters( &m_pStudioModelCount, &m_pModelsDrawn );

	// Get pointers to engine data structures
	m_pbonetransform		= (float (*)[MAXSTUDIOBONES][3][4])IEngineStudio.StudioGetBoneTransform();
	m_plighttransform		= (float (*)[MAXSTUDIOBONES][3][4])IEngineStudio.StudioGetLightTransform();
	m_paliastransform		= (float (*)[3][4])IEngineStudio.StudioGetAliasTransform();
	m_protationmatrix		= (float (*)[3][4])IEngineStudio.StudioGetRotationMatrix();

	//MODDD - safety
	for (int i = 0; i < 1024; i++) {
		ary_g_prevTime[i] = 0;
		ary_g_prevFrame[i] = 0;
	}

}

/*
====================
CStudioModelRenderer

====================
*/
CStudioModelRenderer::CStudioModelRenderer( void )
{
	m_fDoInterp			= 1;
	m_fGaitEstimation	= 1;
	m_pCurrentEntity	= NULL;
	m_pCvarHiModels		= NULL;
	m_pCvarDeveloper	= NULL;
	m_pCvarDrawEntities	= NULL;
	m_pChromeSprite		= NULL;
	m_pStudioModelCount	= NULL;
	m_pModelsDrawn		= NULL;
	m_protationmatrix	= NULL;
	m_paliastransform	= NULL;
	m_pbonetransform	= NULL;
	m_plighttransform	= NULL;
	m_pStudioHeader		= NULL;
	m_pBodyPart			= NULL;
	m_pSubModel			= NULL;
	m_pPlayerInfo		= NULL;
	m_pRenderModel		= NULL;
}


/*
====================
~CStudioModelRenderer

====================
*/
CStudioModelRenderer::~CStudioModelRenderer( void )
{
}

/*
====================
StudioCalcBoneAdj

====================
*/


void CStudioModelRenderer::StudioCalcBoneAdj( float dadt, float *adj, const byte *pcontroller1, const byte *pcontroller2, byte mouthopen )
{
	StudioCalcBoneAdj(dadt, adj, pcontroller1, pcontroller2, mouthopen, FALSE);
}

// Only for small adjustable parts of the model, like the angle of the head in yaw and pitch.
void CStudioModelRenderer::StudioCalcBoneAdj ( float dadt, float *adj, const byte *pcontroller1, const byte *pcontroller2, byte mouthopen, byte IsReflection )
{
	int i;
	int j;
	float value;
	mstudiobonecontroller_t *pbonecontroller;
	
	pbonecontroller = (mstudiobonecontroller_t *)((byte *)m_pStudioHeader + m_pStudioHeader->bonecontrollerindex);

	for (j = 0; j < m_pStudioHeader->numbonecontrollers; j++)
	{
		i = pbonecontroller[j].index;
		if (i <= 3)
		{
			// check for 360% wrapping
			if (pbonecontroller[j].type & STUDIO_RLOOP)
			{
				if (abs(pcontroller1[i] - pcontroller2[i]) > 128)
				{
					int a, b;
					a = (pcontroller1[j] + 128) % 256;
					b = (pcontroller2[j] + 128) % 256;
					value = ((a * dadt) + (b * (1 - dadt)) - 128) * (360.0/256.0) + pbonecontroller[j].start;
				}
				else 
				{
					value = ((pcontroller1[i] * dadt + (pcontroller2[i]) * (1.0 - dadt))) * (360.0/256.0) + pbonecontroller[j].start;
				}
			}
			else 
			{
				
				value = (pcontroller1[i] * dadt + pcontroller2[i] * (1.0 - dadt)) / 255.0;
				if (value < 0) value = 0;
				if (value > 1.0) value = 1.0;
				value = (1.0 - value) * pbonecontroller[j].start + value * pbonecontroller[j].end;

				//NOTE: value of -90 is 90 degrees left (right angle), 90 is 90 degrees right, intuitive.   0 is just straight ahead.
				//MODDD

				// 1?
				if(i == 0 && IsReflection){
					// MIRROR-RELATED!  Jeez is it hard to just say that.
					// Fix for rotatable NPC heads not rotating properly in the mirrors.
					// And best for only the yaw, the pitch (and likely roll) doesn't need this.
					// Unfortunately this is just a guess, we'd need a way for the model to say which is the yaw for this to
					// properly work.  Thinking this is a sympton of a bigger problem though...
					value *= -1;
				}
			
			}
			// Con_DPrintf( "%d %d %f : %f\n", m_pCurrentEntity->curstate.controller[j], m_pCurrentEntity->latched.prevcontroller[j], value, dadt );
		}
		else
		{
			if (m_pCurrentEntity->curstate.eflags & 4) {
				easyForcePrintLine("OH hey ther");
			}


			//MODDD - support for custom mouth-open values for scientist mouthes to be open on corpses.
			// Yes, we aim for odd details like that.
			// Anyway, little issue.  Mouths seem fixed to receiving hardcoded "mouthopen" values in this
			// method for setting that controller, to look like the mouth is 'saying' the playing sound.
			// Ordinarily, this leaves no room for specifying what mouth value we want serverside.
			// Turns out, we can consistently just check to see if the current controller index
			// (pcontroller1[j]) is non-zero, which implies we want to force the mouth value.
			// Combined with an actual "mouthopen" value (mouth controller suggetsion) of 0, it is allowed.
			///////////////////////////////////////////////////////////////////////////////////////////////////////////
			while (TRUE) {
				// for breakpoints
				/*
				int i1 = pbonecontroller[0].index;
				int i2 = pbonecontroller[1].index;
				int i3 = pbonecontroller[2].index;
				int i4 = pbonecontroller[3].index;

				byte b1 = pcontroller1[0];
				byte b2 = pcontroller1[1];
				byte b3 = pcontroller1[2];
				byte b4 = pcontroller1[3];
				*/

				// PROCEDURAL LOOP - break to skip to the end, does not run more than once.

				//if (m_pStudioHeader->numbonecontrollers < 4) {
					// No need for all that, pcontroller1 represents the most recent controller choice anyway
					// (pcontroller2 is the previous frame's one, both are available to help with interpolation,
					// if I understand right)
					//float valueTest = (pcontroller1[i] * dadt + pcontroller2[i] * (1.0 - dadt)) / 255.0;
					//byte valueTest = pcontroller1[i];
					byte valueTest = pcontroller1[j];
					if (valueTest != 0 && mouthopen == 0) {
						// Just ignore interp (cur/prev checks), this isn't expected to be forced differnetly
						// very often.
						//value = (pcontroller1[i] * dadt + pcontroller2[i] * (1.0 - dadt)) / 255.0;
						value = (((float)valueTest) / 255.0f);
						if (value < 0) value = 0;
						if (value > 1.0) value = 1.0;
						value = (1.0 - value) * pbonecontroller[j].start + value * pbonecontroller[j].end;

						break;  //skip the rest
					}
				//}
				///////////////////////////////////////////////////////////////////////////////////////////////////////////
				
				// Default mouth behavior, work with mouthopen (as something is being said)
				value = mouthopen / 64.0;
				if (value > 1.0) value = 1.0;
				value = (1.0 - value) * pbonecontroller[j].start + value * pbonecontroller[j].end;
				// Con_DPrintf("%d %f\n", mouthopen, value );
				
				break;  //end
			}//END OF procedural loop

		}
		
		switch(pbonecontroller[j].type & STUDIO_TYPES)
		{
		case STUDIO_XR:
		case STUDIO_YR:
		case STUDIO_ZR:
			adj[j] = value * (M_PI / 180.0f);
			break;
		case STUDIO_X:
		case STUDIO_Y:
		case STUDIO_Z:
			adj[j] = value;
			break;
		}
		
	}//END OF for(j = 0...)
}


/*
====================
StudioCalcBoneQuaterion

====================
*/
void CStudioModelRenderer::StudioCalcBoneQuaterion( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *q )
{
	int				j, k;
	vec4_t			q1, q2;
	vec3_t			angle1, angle2;
	mstudioanimvalue_t	*panimvalue;

	for (j = 0; j < 3; j++)
	{
		if (panim->offset[j+3] == 0)
		{
			angle2[j] = angle1[j] = pbone->value[j+3]; // default;
		}
		else
		{
			panimvalue = (mstudioanimvalue_t *)((byte *)panim + panim->offset[j+3]);
			k = frame;
			// DEBUG
			if (panimvalue->num.total < panimvalue->num.valid)
				k = 0;
			while (panimvalue->num.total <= k)
			{
				k -= panimvalue->num.total;
				panimvalue += panimvalue->num.valid + 1;
				// DEBUG
				if (panimvalue->num.total < panimvalue->num.valid)
					k = 0;
			}
			// Bah, missing blend!
			if (panimvalue->num.valid > k)
			{
				angle1[j] = panimvalue[k+1].value;

				if (panimvalue->num.valid > k + 1)
				{
					angle2[j] = panimvalue[k+2].value;
				}
				else
				{
					if (panimvalue->num.total > k + 1)
						angle2[j] = angle1[j];
					else
						angle2[j] = panimvalue[panimvalue->num.valid+2].value;
				}
			}
			else
			{
				angle1[j] = panimvalue[panimvalue->num.valid].value;
				if (panimvalue->num.total > k + 1)
				{
					angle2[j] = angle1[j];
				}
				else
				{
					angle2[j] = panimvalue[panimvalue->num.valid + 2].value;
				}
			}
			angle1[j] = pbone->value[j+3] + angle1[j] * pbone->scale[j+3];
			angle2[j] = pbone->value[j+3] + angle2[j] * pbone->scale[j+3];
		}

		if (pbone->bonecontroller[j+3] != -1)
		{
			angle1[j] += adj[pbone->bonecontroller[j+3]];
			angle2[j] += adj[pbone->bonecontroller[j+3]];
		}
	}

	if (!VectorCompare( angle1, angle2 ))
	{
		AngleQuaternion( angle1, q1 );
		AngleQuaternion( angle2, q2 );
		QuaternionSlerp( q1, q2, s, q );
	}
	else
	{
		AngleQuaternion( angle1, q );
	}
}

/*
====================
StudioCalcBonePosition

====================
*/
void CStudioModelRenderer::StudioCalcBonePosition( int frame, float s, mstudiobone_t *pbone, mstudioanim_t *panim, float *adj, float *pos )
{
	int j, k;
	mstudioanimvalue_t	*panimvalue;

	for (j = 0; j < 3; j++)
	{
		pos[j] = pbone->value[j]; // default;
		if (panim->offset[j] != 0)
		{
			panimvalue = (mstudioanimvalue_t *)((byte *)panim + panim->offset[j]);
			/*
			if (i == 0 && j == 0)
				Con_DPrintf("%d  %d:%d  %f\n", frame, panimvalue->num.valid, panimvalue->num.total, s );
			*/
			
			k = frame;
			// DEBUG
			if (panimvalue->num.total < panimvalue->num.valid)
				k = 0;
			// find span of values that includes the frame we want
			while (panimvalue->num.total <= k)
			{
				k -= panimvalue->num.total;
				panimvalue += panimvalue->num.valid + 1;
  				// DEBUG
				if (panimvalue->num.total < panimvalue->num.valid)
					k = 0;
			}
			// if we're inside the span
			if (panimvalue->num.valid > k)
			{
				// and there's more data in the span
				if (panimvalue->num.valid > k + 1)
				{
					pos[j] += (panimvalue[k+1].value * (1.0 - s) + s * panimvalue[k+2].value) * pbone->scale[j];
				}
				else
				{
					pos[j] += panimvalue[k+1].value * pbone->scale[j];
				}
			}
			else
			{
				// are we at the end of the repeating values section and there's another section with data?
				if (panimvalue->num.total <= k + 1)
				{
					pos[j] += (panimvalue[panimvalue->num.valid].value * (1.0 - s) + s * panimvalue[panimvalue->num.valid + 2].value) * pbone->scale[j];
				}
				else
				{
					pos[j] += panimvalue[panimvalue->num.valid].value * pbone->scale[j];
				}
			}
		}
		if ( pbone->bonecontroller[j] != -1 && adj )
		{
			pos[j] += adj[pbone->bonecontroller[j]];
		}
	}
}

/*
====================
StudioSlerpBones

====================
*/
void CStudioModelRenderer::StudioSlerpBones( vec4_t q1[], float pos1[][3], vec4_t q2[], float pos2[][3], float s )
{
	int i;
	vec4_t q3;
	float s1;

	if (s < 0) s = 0;
	else if (s > 1.0) s = 1.0;

	s1 = 1.0 - s;

	for (i = 0; i < m_pStudioHeader->numbones; i++)
	{
		QuaternionSlerp( q1[i], q2[i], s, q3 );
		q1[i][0] = q3[0];
		q1[i][1] = q3[1];
		q1[i][2] = q3[2];
		q1[i][3] = q3[3];
		pos1[i][0] = pos1[i][0] * s1 + pos2[i][0] * s;
		pos1[i][1] = pos1[i][1] * s1 + pos2[i][1] * s;
		pos1[i][2] = pos1[i][2] * s1 + pos2[i][2] * s;
	}
}

/*
====================
StudioGetAnim

====================
*/
mstudioanim_t *CStudioModelRenderer::StudioGetAnim( model_t *m_pSubModel, mstudioseqdesc_t *pseqdesc )
{
	mstudioseqgroup_t *pseqgroup;
	cache_user_t *paSequences;

	pseqgroup = (mstudioseqgroup_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqgroupindex) + pseqdesc->seqgroup;

	if (pseqdesc->seqgroup == 0)
	{
		return (mstudioanim_t *)((byte *)m_pStudioHeader + pseqgroup->data + pseqdesc->animindex);
	}

	paSequences = (cache_user_t *)m_pSubModel->submodels;

	if (paSequences == NULL)
	{
		paSequences = (cache_user_t *)IEngineStudio.Mem_Calloc( 16, sizeof( cache_user_t ) ); // UNDONE: leak!
		m_pSubModel->submodels = (dmodel_t *)paSequences;
	}

	if (!IEngineStudio.Cache_Check( (struct cache_user_s *)&(paSequences[pseqdesc->seqgroup])))
	{
		gEngfuncs.Con_DPrintf("loading %s\n", pseqgroup->name );
		IEngineStudio.LoadCacheFile( pseqgroup->name, (struct cache_user_s *)&paSequences[pseqdesc->seqgroup] );
	}
	return (mstudioanim_t *)((byte *)paSequences[pseqdesc->seqgroup].data + pseqdesc->animindex);
}

/*
====================
StudioPlayerBlend

====================
*/
void CStudioModelRenderer::StudioPlayerBlend( mstudioseqdesc_t *pseqdesc, int *pBlend, float *pPitch )
{
	// calc up/down pointing
	*pBlend = (*pPitch * 3);
	if (*pBlend < pseqdesc->blendstart[0])
	{
		*pPitch -= pseqdesc->blendstart[0] / 3.0;
		*pBlend = 0;
	}
	else if (*pBlend > pseqdesc->blendend[0])
	{
		*pPitch -= pseqdesc->blendend[0] / 3.0;
		*pBlend = 255;
	}
	else
	{
		if (pseqdesc->blendend[0] - pseqdesc->blendstart[0] < 0.1) // catch qc error
			*pBlend = 127;
		else
			*pBlend = 255 * (*pBlend - pseqdesc->blendstart[0]) / (pseqdesc->blendend[0] - pseqdesc->blendstart[0]);
		*pPitch = 0;
	}
}

/*
====================
StudioSetUpTransform

====================
*/
void CStudioModelRenderer::StudioSetUpTransform (int trivial_accept)
{
	//ASS
	//if (trivial_accept >= 1024) {
	//	return;
	//}
	//
	int i;
	vec3_t angles;
	vec3_t modelpos;
	
	int ic = trivial_accept - 1024;

// tweek model origin	
	//for (i = 0; i < 3; i++)
	//	modelpos[i] = m_pCurrentEntity->origin[i];

	VectorCopy_f( m_pCurrentEntity->origin, modelpos );

// TODO: should really be stored with the entity instead of being reconstructed
// TODO: should use a look-up table
// TODO: could cache lazily, stored in the entity
	angles[ROLL] = m_pCurrentEntity->curstate.angles[ROLL];
	angles[PITCH] = m_pCurrentEntity->curstate.angles[PITCH];
	angles[YAW] = m_pCurrentEntity->curstate.angles[YAW];


	//Con_DPrintf("Angles %4.2f prev %4.2f for %i\n", angles[PITCH], m_pCurrentEntity->index);
	//Con_DPrintf("movetype %d %d\n", m_pCurrentEntity->movetype, m_pCurrentEntity->aiment );
	if (m_pCurrentEntity->curstate.movetype == MOVETYPE_STEP)
	{


		if (m_pCurrentEntity->curstate.renderfx & STOPINTR) {
			int x = 222;
		}

		float f = 0;
		float d;
		// don't do it if the goalstarttime hasn't updated in a while.

		// NOTE:  Because we need to interpolate multiplayer characters, the interpolation time limit
		//  was increased to 1.0 s., which is 2x the max lag we are accounting for.



		/*
		// printouts.
		if (m_pCurrentEntity->curstate.renderfx & ISNPC) {
			float denomoTest = (m_pCurrentEntity->curstate.animtime - m_pCurrentEntity->latched.prevanimtime);
			easyForcePrint("AAAAA cltime:%.2f curanim:%.2f latchedprevanim:%.2f, cltime-curanim:%.2f denomo:%.2f ordinaryf:", m_clTime, m_pCurrentEntity->curstate.animtime, m_pCurrentEntity->latched.prevanimtime, (m_clTime - m_pCurrentEntity->curstate.animtime), denomoTest);
			if (denomoTest > 0) {
				float reso = (m_clTime - m_pCurrentEntity->curstate.animtime) / denomoTest;
				easyForcePrintLine("%.2f", denomoTest);
			}
			else {
				// 0 or under?  Don't bother with denomoTest.
				easyForcePrintLine("N/A");
			}
		}
		*/

		if (m_fDoInterp)
		{
			if (
				(m_clTime < m_pCurrentEntity->curstate.animtime + 1.0f)
				// MODDD - Do we need this check now?  Glitchiness can happen when animtime is way too close or even under prevanimtime...
				// however that's managed.
				//&& (m_pCurrentEntity->curstate.animtime != m_pCurrentEntity->latched.prevanimtime)
			)
			{
				float denomo = (m_pCurrentEntity->curstate.animtime - m_pCurrentEntity->latched.prevanimtime);

				
				// 0.013?  0.026?   0.06?   0.0599?
				if (fabs(denomo) <= EASY_CVAR_GET(interpolation_movetypestep_mindelta)) {
					// Too small of a denominator?  I suspect something is up.
					f = 0;


					//MODDD - NEW.  Same fix for things recently touching the ground losing interp.
					// Assuming the issues are related.
					///////////////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////////////

					if (m_fDoInterp && (m_pCurrentEntity->curstate.renderfx & ISNPC) && !(m_pCurrentEntity->curstate.renderfx & STOPINTR) ) {
						g_OLDprevTime = ary_g_prevTime[m_pCurrentEntity->index];

						// CAREFUL!  Check for these condtions too.
						// An animation not even playing or one that's ended (out of bounds without looping)
						// Then again, this might not even be necessary.  The as-is dfdt-giving script (furthest above)
						// does no checks like this.  Disabling, should not be a problem.
						/*
						if (
							m_pCurrentEntity->curstate.framerate == 0 ||
							(
								(!(pseqdesc->flags & STUDIO_LOOPING)) &&
								(
									(m_pCurrentEntity->curstate.framerate > 0 && m_pCurrentEntity->curstate.frame >= 255) ||
									(m_pCurrentEntity->curstate.framerate < 0 && m_pCurrentEntity->curstate.frame <= 0)
								)
							)
						) {
							// 0 framerate, or our frame is out of bounds & no plans on looping?  do not!
							// (this is for a breakpoint)
							int x = 45;
						}
						else
						*/
						{


							// MODDD - !!!!!!!!!!
							///////////////////////////////////////////////////////////////////////
							// Why do this check again?  Elsewhere handles this
							/*
							// It would be nice to tell when a server update occurred for whether it's worth bothering to check
							// curstate.frame for a change but even that is a check all the same
							if (ary_g_prevFrame[m_pCurrentEntity->index] != m_pCurrentEntity->curstate.frame) {
								ary_g_prevFrame[m_pCurrentEntity->index] = m_pCurrentEntity->curstate.frame;
								ary_g_prevTime[m_pCurrentEntity->index] = m_clTime;

							}
							*/
							///////////////////////////////////////////////////////////////////////



							//m_pCurrentEntity->curstate.iuser4 = ary_g_prevFrame[m_pCurrentEntity->index];
							//m_pCurrentEntity->curstate.fuser4 = ary_g_prevTime[m_pCurrentEntity->index];


					// f = (m_clTime - m_pCurrentEntity->curstate.animtime) / denomo;
					// f = (m_clTime - m_pCurrentEntity->curstate.animtime) / (m_pCurrentEntity->curstate.animtime - m_pCurrentEntity->latched.prevanimtime)

							// of course it was!
							//if (f == 0) {
								// try this.
								// m_clTime ?  gpGlobals->time ?


								/*
								float diffo = m_clTime - ary_g_prevTime[m_pCurrentEntity->index];
								if (diffo > 0) {
									dfdt = diffo * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;
								}
								*/

								/*
								float diffo = m_clTime - ary_g_prevTime[m_pCurrentEntity->index];
								if (fabs(diffo) > EASY_CVAR_GET(interpolation_movetypestep_mindelta)) {
									//f = (m_clTime - m_pCurrentEntity->curstate.animtime) / diffo;
									f = (m_clTime - m_pCurrentEntity->curstate.animtime) / diffo;
									//f = (m_clTime - ary_g_prevTime[m_pCurrentEntity->index]) * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;
								}
								*/

								/*
								float denomoTest2 = (ary_g_prevTime[m_pCurrentEntity->index] - m_pCurrentEntity->latched.prevanimtime);
								if (denomoTest2 > EASY_CVAR_GET(interpolation_movetypestep_mindelta)) {
									float attempt = (m_clTime - ary_g_prevTime[m_pCurrentEntity->index]) / denomoTest2;
									f = attempt;
								}
								*/

									float attempt = (m_clTime - ary_g_prevTime[m_pCurrentEntity->index]);
									f = attempt;

							//}
						}
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////////////
					///////////////////////////////////////////////////////////////////////////////////////////////////




				}
				else {
					// ok.  This runs if there wasn't a problem with 'denomo' (not too small or negative).
					f = (m_clTime - m_pCurrentEntity->curstate.animtime) / denomo;
				}

				//Con_DPrintf("%4.2f %.2f %.2f\n", f, m_pCurrentEntity->curstate.animtime, m_clTime);

				/*
				//MODDD - breakpoint
				if (f > 10 || f < -10) {
					int x = 222;
				}
				*/

			}

			// ugly hack to interpolate angle, position. current is reached 0.1 seconds after being set
			f = f - 1.0;
		}
		else
		{
			f = 0;
		}










		////if (m_pCurrentEntity->curstate.renderfx & STOPINTR) {
		//	if (f < -1) {
		//		f = -1;
		//	}
		//	if (f > 1) {
		//		f = 1;
		//	}
		////}
		

		//////////////////////////////////////////////////////////

		/*
		for (i = 0; i < 3; i++)a
		{
			modelpos[i] += (m_pCurrentEntity->origin[i] - m_pCurrentEntity->latched.prevorigin[i]) * f;
		}
		*/

		vec3_t tempVecto;
		for (i = 0; i < 3; i++)
		{
			tempVecto[i] = m_pCurrentEntity->origin[i] - m_pCurrentEntity->latched.prevorigin[i];
		}

		float len = tempVecto.Length();
		//if (len > 24) {
		//	// o no
		//}
		//else {
			for (i = 0; i < 3; i++)
			{
				modelpos[i] += tempVecto[i] * f;
			}
		//}

		// NOTE:  Because multiplayer lag can be relatively large, we don't want to cap
		//  f at 1.5 anymore.
		//if (f > -1.0 && f < 1.5) {}

//			Con_DPrintf("%.0f %.0f\n",m_pCurrentEntity->msg_angles[0][YAW], m_pCurrentEntity->msg_angles[1][YAW] );
		for (i = 0; i < 3; i++)
		{
			float ang1, ang2;

			ang1 = m_pCurrentEntity->angles[i];
			ang2 = m_pCurrentEntity->latched.prevangles[i];

			d = ang1 - ang2;
			if (d > 180)
			{
				d -= 360;
			}
			else if (d < -180)
			{	
				d += 360;
			}

			angles[i] += d * f;
		}
		//Con_DPrintf("%.3f \n", f );
		
	}
	else if ( m_pCurrentEntity->curstate.movetype != MOVETYPE_NONE ) 
	{
		VectorCopy_f( m_pCurrentEntity->angles, angles );
	}

	//Con_DPrintf("%.0f %0.f %0.f\n", modelpos[0], modelpos[1], modelpos[2] );
	//Con_DPrintf("%.0f %0.f %0.f\n", angles[0], angles[1], angles[2] );
	
	//MODDD - very significant.  Better transformation than before, as when done at this point of time, it does not cause the culling glitch.
	if(trivial_accept >= 1024){
		//angles[0] *= -1;
		angles[1] *= -1;
		//angles[2] *= -1;
	}
	
	angles[PITCH] = -angles[PITCH];
	AngleMatrix (angles, (*m_protationmatrix));
            

	/*
	//if (trivial_accept >= 1024) {

	//*(m_pbonetransform[i][0][1]) *= 1;
	//*(m_pbonetransform[i][1][1]) *= -1;
	//*(m_pbonetransform[i][2][1]) *= 1;
	float scalex = EASY_CVAR_GET(ctt1);
	float scaley = EASY_CVAR_GET(ctt2);
	float scalez = EASY_CVAR_GET(ctt3);
	(*m_protationmatrix)[0][0] *= scalex;
	(*m_protationmatrix)[1][0] *= scalex;
	(*m_protationmatrix)[2][0] *= scalex;
	(*m_protationmatrix)[0][1] *= scaley;
	(*m_protationmatrix)[1][1] *= scaley;
	(*m_protationmatrix)[2][1] *= scaley;
	(*m_protationmatrix)[0][2] *= scalez;
	(*m_protationmatrix)[1][2] *= scalez;
	(*m_protationmatrix)[2][2] *= scalez;
	
	//}
	*/


	if ( !IEngineStudio.IsHardware() )
	{
		static float viewmatrix[3][4];

		VectorCopy_f (m_vRight, viewmatrix[0]);
		VectorCopy_f (m_vUp, viewmatrix[1]);
		VectorInverse (viewmatrix[1]);
		VectorCopy_f (m_vNormal, viewmatrix[2]);

		if(trivial_accept >= 1024){
			
			(*m_protationmatrix)[0][3] = modelpos[0] - m_vRenderOrigin[0];
			(*m_protationmatrix)[1][3] = modelpos[1] - m_vRenderOrigin[1];
			(*m_protationmatrix)[2][3] = modelpos[2] - m_vRenderOrigin[2];


			//CHECKPOINT3
			switch (gHUD.Mirrors[ic].type)
			{
			case 0:
	       		(*m_protationmatrix)[0][0] *= 1;
	      		(*m_protationmatrix)[0][1] *= 1;
	       		(*m_protationmatrix)[0][2] *= 1;
	      		(*m_protationmatrix)[0][3] += gHUD.Mirrors[ic].origin[0]*2 - modelpos[0]*2;
			break;
			case 1:
	       	    (*m_protationmatrix)[1][1] *= 1;
	      	    (*m_protationmatrix)[1][0] *= 1;
	       	    (*m_protationmatrix)[1][2] *= 1;
	      	    (*m_protationmatrix)[1][3] += gHUD.Mirrors[ic].origin[1]*2 - modelpos[1]*2;
			break;
			case 2:
				// no idea how to translate this.?  is this okay?
				// Added back in the other missing [2][0], [2][1], and [2][3] areas.  Being absent is still the same as
				// being multiplied by 1 or changed by an amount of 0 (no change).
				(*m_protationmatrix)[2][0] *= 1;
				(*m_protationmatrix)[2][1] *= 1;
				(*m_protationmatrix)[2][2] *= -1;//minimal matrix transform - for right calculate origin of monster. G-cont
				(*m_protationmatrix)[2][3] += 0;
			break;
			}

			ConcatTransforms (viewmatrix, (*m_protationmatrix), (*m_paliastransform));
			
			(*m_protationmatrix)[0][3] = modelpos[0];
			(*m_protationmatrix)[1][3] = modelpos[1];
			(*m_protationmatrix)[2][3] = modelpos[2];
			
		}else{

			//no reflection?   Nothing unusual.
			(*m_protationmatrix)[0][3] = modelpos[0] - m_vRenderOrigin[0];
			(*m_protationmatrix)[1][3] = modelpos[1] - m_vRenderOrigin[1];
			(*m_protationmatrix)[2][3] = modelpos[2] - m_vRenderOrigin[2];

			//CHECKPOINT5
			ConcatTransforms (viewmatrix, (*m_protationmatrix), (*m_paliastransform));
		
			
			//PARAMETERIZE FOR REFLECTION!!!   When inactive, the crome effect from source is seen (software only so far)
			
			//...also, when inactive, can we just force it for ONLY the viewmodel-attachments (beam, gauss laser, etc.), perhaps??
			
			
			(*m_protationmatrix)[0][3] = modelpos[0];
			(*m_protationmatrix)[1][3] = modelpos[1];
			(*m_protationmatrix)[2][3] = modelpos[2];
		}


		//!!!
		trivial_accept = 0; //was going to be this anyways., as it was never used beforehand (in the retail copy, this was never non-zero at any point in the entire method, let alone here).


		// do the scaling up of x and y to screen coordinates as part of the transform
		// for the unclipped case (it would mess up clipping in the clipped case).
		// Also scale down z, so 1/z is scaled 31 bits for free, and scale down x and y
		// correspondingly so the projected x and y come out right
		// FIXME: make this work for clipped case too?
		if (trivial_accept)
		{
			for (i=0 ; i<4 ; i++)
			{
				(*m_paliastransform)[0][i] *= m_fSoftwareXScale *
						(1.0 / (ZISCALE * 0x10000));
				(*m_paliastransform)[1][i] *= m_fSoftwareYScale *
						(1.0 / (ZISCALE * 0x10000));
				(*m_paliastransform)[2][i] *= 1.0 / (ZISCALE * 0x10000);

			}
		}

	}
	else{
	//!!!!!!!!!!!!!!!!!!!!!!!!!
	
		//!!!ASS
		if(trivial_accept >= 1024){
		//if(TRUE){
			//mirror
			
			//(*m_protationmatrix)[0][3] = modelpos[0] - m_vRenderOrigin[0];
			//(*m_protationmatrix)[1][3] = modelpos[1] - m_vRenderOrigin[1];
			//(*m_protationmatrix)[2][3] = modelpos[2] - m_vRenderOrigin[2];
			
			(*m_protationmatrix)[0][3] = modelpos[0];
			//(*m_protationmatrix)[1][3] = gHUD.Mirrors[ic].origin[1] - modelpos[1];
			(*m_protationmatrix)[1][3] = modelpos[1];
			(*m_protationmatrix)[2][3] = modelpos[2];
			
			//CHECKPOINT3
			switch (gHUD.Mirrors[ic].type)
			{
			case 0:
	       	    (*m_protationmatrix)[0][0] *= 1;
	      	    (*m_protationmatrix)[0][1] *= 1;
	       	    (*m_protationmatrix)[0][2] *= 1;
	      	    (*m_protationmatrix)[0][3] = gHUD.Mirrors[ic].origin[0]*2 - modelpos[0];
            break;
			case 1:
	       	    (*m_protationmatrix)[1][1] *= 1;
	      	    (*m_protationmatrix)[1][0] *= 1;
	       	    (*m_protationmatrix)[1][2] *= 1;
				//if (trivial_accept >= 1024) {
				(*m_protationmatrix)[1][3] = gHUD.Mirrors[ic].origin[1] * 2 - modelpos[1];
				//}
            break;
			case 2:
				(*m_protationmatrix)[2][0] *= 1;
				(*m_protationmatrix)[2][1] *= 1;
				(*m_protationmatrix)[2][2] *= -1;//minimal matrix transform - for right calculate origin of monster. G-cont
				(*m_protationmatrix)[2][3] += 0;
            break;
            }

			
		}else{
			//no mirror 
			
			/*
			static float viewmatrix[3][4];

			VectorCopy_f (m_vRight, viewmatrix[0]);
			VectorCopy_f (m_vUp, viewmatrix[1]);
			VectorInverse (viewmatrix[1]);
			VectorCopy_f (m_vNormal, viewmatrix[2]);
			*/

		
			(*m_protationmatrix)[0][3] = modelpos[0];
			(*m_protationmatrix)[1][3] = modelpos[1];
			(*m_protationmatrix)[2][3] = modelpos[2];

			//ConcatTransforms (viewmatrix, (*m_protationmatrix), (*m_protationmatrix));
		}

	}//ELSE.  is that okay?
	
	//MODDD NOTE - this seems to have no bearing on when software mode is being used, so I guess it is okay to happen regardless of either?
			
}


/*
====================
StudioEstimateInterpolant
====================
*/
float CStudioModelRenderer::StudioEstimateInterpolant( void )
{
	float dadt = 1.0;

	if ( m_fDoInterp && ( m_pCurrentEntity->curstate.animtime >= m_pCurrentEntity->latched.prevanimtime + 0.01 ) )
	{
		dadt = (m_clTime - m_pCurrentEntity->curstate.animtime) / 0.1;
		if (dadt > 2.0)
		{
			dadt = 2.0;
		}
	}

	return dadt;
}

/*
====================
StudioCalcRotations
====================
*/
void CStudioModelRenderer::StudioCalcRotations ( float pos[][3], vec4_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f){
	//assume not a reflection.
	StudioCalcRotations(pos, q, pseqdesc, panim, f, FALSE);
}

void CStudioModelRenderer::StudioCalcRotations ( float pos[][3], vec4_t *q, mstudioseqdesc_t *pseqdesc, mstudioanim_t *panim, float f, byte isReflection )
{
	int				i;
	int				frame;
	mstudiobone_t		*pbone;

	float			s;
	float			adj[MAXSTUDIOCONTROLLERS];
	float			dadt;

	if (f > pseqdesc->numframes - 1)
	{
		f = 0;	// bah, fix this bug with changing sequences too fast
	}
	// BUG ( somewhere else ) but this code should validate this data.
	// This could cause a crash if the frame # is negative, so we'll go ahead
	//  and clamp it here
	else if ( f < -0.01 )
	{
		f = -0.01;
	}

	frame = (int)f;

	// Con_DPrintf("%d %.4f %.4f %.4f %.4f %d\n", m_pCurrentEntity->curstate.sequence, m_clTime, m_pCurrentEntity->animtime, m_pCurrentEntity->frame, f, frame );

	// Con_DPrintf( "%f %f %f\n", m_pCurrentEntity->angles[ROLL], m_pCurrentEntity->angles[PITCH], m_pCurrentEntity->angles[YAW] );

	// Con_DPrintf("frame %d %d\n", frame1, frame2 );


	dadt = StudioEstimateInterpolant( );

	s = (f - frame);

	// add in programtic controllers
	pbone		= (mstudiobone_t *)((byte *)m_pStudioHeader + m_pStudioHeader->boneindex);


	StudioCalcBoneAdj( dadt, adj, m_pCurrentEntity->curstate.controller, m_pCurrentEntity->latched.prevcontroller, m_pCurrentEntity->mouth.mouthopen, isReflection );

	
	for (i = 0; i < m_pStudioHeader->numbones; i++, pbone++, panim++) 
	{
		StudioCalcBoneQuaterion( frame, s, pbone, panim, adj, q[i] );

		StudioCalcBonePosition( frame, s, pbone, panim, adj, pos[i] );
		// if (0 && i == 0)
		//	Con_DPrintf("%d %d %d %d\n", m_pCurrentEntity->curstate.sequence, frame, j, k );
	}

	//MODDD - unconventional?
	//return; ???  was this supposed to be commented out?

	if (pseqdesc->motiontype & STUDIO_X)
	{
		pos[pseqdesc->motionbone][0] = 0.0;
	}
	if (pseqdesc->motiontype & STUDIO_Y)
	{
		pos[pseqdesc->motionbone][1] = 0.0;
	}
	if (pseqdesc->motiontype & STUDIO_Z)
	{
		pos[pseqdesc->motionbone][2] = 0.0;
	}

	s = 0 * ((1.0 - (f - (int)(f))) / (pseqdesc->numframes)) * m_pCurrentEntity->curstate.framerate;

	

	if (pseqdesc->motiontype & STUDIO_LX)
	{
		pos[pseqdesc->motionbone][0] += s * pseqdesc->linearmovement[0];
	}
	if (pseqdesc->motiontype & STUDIO_LY)
	{
		pos[pseqdesc->motionbone][1] += s * pseqdesc->linearmovement[1];
	}
	if (pseqdesc->motiontype & STUDIO_LZ)
	{
		pos[pseqdesc->motionbone][2] += s * pseqdesc->linearmovement[2];
	}

	
}

/*
====================
Studio_FxTransform

====================
*/
void CStudioModelRenderer::StudioFxTransform( cl_entity_t *ent, float transform[3][4] )
{

//	float scaled = -1;
	//transform[0][1] *= scaled;
	//transform[1][1] *= scaled;
	//transform[2][1] *= scaled;


	//MODDD - only involve the primary bits.
	switch( (ent->curstate.renderfx & RENDERFX_PRIMARY_BITS) )
	{
	case kRenderFxDistort:
	case kRenderFxHologram:

		//MODDD - this can't happen if using as a bitmask-flag.
		//...Nevermind, no prevention needed then.



		if ( gEngfuncs.pfnRandomLong(0,49) == 0 )
		{
			int axis = gEngfuncs.pfnRandomLong(0,1);
			if ( axis == 1 ) // Choose between x & z
				axis = 2;
			VectorScale( transform[axis], gEngfuncs.pfnRandomFloat(1,1.484), transform[axis] );
		}
		else if ( gEngfuncs.pfnRandomLong(0,49) == 0 )
		{
			float offset;
			int axis = gEngfuncs.pfnRandomLong(0,1);
			if ( axis == 1 ) // Choose between x & z
				axis = 2;
			offset = gEngfuncs.pfnRandomFloat(-10,10);
			transform[gEngfuncs.pfnRandomLong(0,2)][3] += offset;
		}
	break;
	case kRenderFxExplode:
		{
			float scale;

			scale = 1.0 + ( m_clTime - ent->curstate.animtime) * 10.0;
			if ( scale > 2 )	// Don't blow up more than 200%
				scale = 2;
			transform[0][1] *= scale;
			transform[1][1] *= scale;
			transform[2][1] *= scale;
		}
	break;
	//MODDD - new
	case kRenderFxImplode:
		{
			float scale;
			//easyForcePrintLine("WHAT IS THAT %.2f %.2f %.2f", transform[0][3], transform[1][3], transform[2][3]);

			scale = 1.0 - ( m_clTime - ent->curstate.animtime) * 2.2;
			if ( scale < 0.01 )
				scale = 0.01;
			transform[0][1] *= scale;
			transform[1][1] *= scale;
			transform[2][1] *= scale;
			//??
			transform[0][0] *= scale;
			transform[1][0] *= scale;
			transform[2][0] *= scale;
			transform[0][2] *= scale;
			transform[1][2] *= scale;
			transform[2][2] *= scale;
			//transform[0][3] *= scale;
			//transform[1][3] *= scale;
			//The second index being 3 actually sets the position of this instead of the scale, at first indeces 0, 1, 2 (x, y, z).
			//So this little edit keeps the monster on the ground while it shrinks, if it does at all (at a scale of 1, it is unchanged)
			transform[2][3] = transform[2][3] - (transform[2][3] * (1 - scale) ); //0 + scale;
		}
	break;

	}
}

/*
====================
StudioEstimateFrame

====================
*/


float CStudioModelRenderer::StudioEstimateFrame( mstudioseqdesc_t *pseqdesc )
{
	double dfdt;
	double f;
	//return 0;
	BOOL DEBUG_NeededFix = FALSE;
	double DEBUG_old_f;

	//??? StudioEstimateInterpolant


	//if (m_pCurrentEntity->curstate.renderfx & ISNPC) {
	//	int x = 45;
	//}




	// NOPE.  This is a bad idea actually.
	//if (
	//	(m_pCurrentEntity->prevstate.frame == m_pCurrentEntity->curstate.frame)
	//	) {
	//	if (m_pCurrentEntity->curstate.animtime != m_pCurrentEntity->prevstate.animtime) {
	//		m_pCurrentEntity->curstate.animtime = m_pCurrentEntity->prevstate.animtime;
	//	}
	//}



	//MODDD NOTICE - should other places have this "renderfx & STOPINTR" check too.?
	if ( m_fDoInterp && !(m_pCurrentEntity->curstate.renderfx & STOPINTR) )
	{
		if ( m_clTime < m_pCurrentEntity->curstate.animtime )
		{
			dfdt = 0;
		}
		else
		{
			dfdt = (m_clTime - m_pCurrentEntity->curstate.animtime) * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;

		}
	}
	else
	{
		dfdt = 0;
	}

	


	//MODDD - viewmodel interp
	// If interpolation is turned off and this is a viewmodel, have to be a little more careful with the approach.
	// Disabling interp completely for viewmodels causes them to be stuck at the first frame of animation, never changes
	// without getting a new anim completely.  So handle its 'no interp' differently.

	// Idea.  Get how much the anim should've increased by in a 0.1 time period, and only move in increments of that much.
	// Pretty weird to have to go through effort so simulate an interp-less state.
	// Eh be a little faster than 0.1 (0.08), view anims can be fast and nothing to gain from any real 'accuracy' anyway.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// I... still don't know how m_fDoInterp would ever be off, but if it were not worth bothering.
	// The viewmodel would be frozen to the first frame the whole time without the server to set curstate.animtime.
	if(m_fDoInterp && EASY_CVAR_GET(cl_interp_viewmodel) == 0) {
		if(g_drawType == DRAWTYPE_VIEWMODEL) {

			// example of modulo (however helpful it may be).  Or subtracting a remainder maybe?
			//m_pPlayerInfo->gaitframe = m_pPlayerInfo->gaitframe - (int)(m_pPlayerInfo->gaitframe / pseqdesc->numframes) * pseqdesc->numframes;

			// 37 - (int)(37 / 30)
			// ...guess the times 'psesdesc->numframes' isn't that relevant.

			float typicalDuration = 0.08 * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;

			dfdt = (m_clTime - m_pCurrentEntity->curstate.animtime) * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;

			/*
			// always 0 ?
			float whut = gpGlobals->frametime;
			// what about this?  Looks to work, but the point is to get the time between server sendoff's.  We can't look to other entities
			// for that info (another serverside-rendered entity's curstae.animtime) without something stupidly hacky.
			float whut2 = m_clTime - m_clOldTime;
			*/

			int diviso = (int)(dfdt / typicalDuration);
			dfdt = diviso * typicalDuration;

		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////




	//MODDD - interp physics issue
	// Seems on reaching the ground recently, the curstate.animtime gets instantly set to the
	// current time before this point of script gets a chance to see what it really was
	// at the time of a server sendoff.
	// That is, isntead of seing the curstate.animtime go from say 36.4, 36.5, 36.6, etc., it
	// suddenly goes up every single render frame: 34.4, 34.41, 34.42, 34.43, etc.
	// Problem is, interp won't know how much time really passed so we can't judge how much
	// to push the imaginary frame 'f' set further below.
	// curstate.frame is the same the whole time during those 0.1 second intervals in any case,
	// regardless of curstate.anim time correctly updating every 0.1 second interval or 
	// incorrectly (way too often).  As the point of interp is to see how much to add to
	// curstate.frame to simulate time passing before getting the next curstate.frame,
	// not having a good curstate.animtime throws it off.


	// Beware, this includes the player (ISPLAYER includes the ISNPC flag).  Can check to see that the result
	// of the bistmask == ISNPC, but including the player itself isn't terrible here anyway.
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (m_fDoInterp && (m_pCurrentEntity->curstate.renderfx & ISNPC)  && !(m_pCurrentEntity->curstate.renderfx & STOPINTR) ) {
		g_OLDprevTime = ary_g_prevTime[m_pCurrentEntity->index];

		// CAREFUL!  Check for these condtions too.
		// An animation not even playing or one that's ended (out of bounds without looping)
		// Then again, this might not even be necessary.  The as-is dfdt-giving script (furthest above)
		// does no checks like this.  Disabling, should not be a problem.
		/*
		if (
			m_pCurrentEntity->curstate.framerate == 0 ||
			(
				(!(pseqdesc->flags & STUDIO_LOOPING)) &&
				(
					(m_pCurrentEntity->curstate.framerate > 0 && m_pCurrentEntity->curstate.frame >= 255) ||
					(m_pCurrentEntity->curstate.framerate < 0 && m_pCurrentEntity->curstate.frame <= 0)
				)
			)
		) {
			// 0 framerate, or our frame is out of bounds & no plans on looping?  do not!
			// (this is for a breakpoint)
			int x = 45;
		}
		else
		*/
		{
			
			// It would be nice to tell when a server update occurred for whether it's worth bothering to check
			// curstate.frame for a change but even that is a check all the same
			if (ary_g_prevFrame[m_pCurrentEntity->index] != m_pCurrentEntity->curstate.frame) {
				ary_g_prevFrame[m_pCurrentEntity->index] = m_pCurrentEntity->curstate.frame;
				ary_g_prevTime[m_pCurrentEntity->index] = m_clTime;
				
			}

			//m_pCurrentEntity->curstate.iuser4 = ary_g_prevFrame[m_pCurrentEntity->index];
			//m_pCurrentEntity->curstate.fuser4 = ary_g_prevTime[m_pCurrentEntity->index];



			if (dfdt == 0) {
				DEBUG_NeededFix = TRUE;
				// try this.
				// m_clTime ?  gpGlobals->time ?
				float diffo = m_clTime - ary_g_prevTime[m_pCurrentEntity->index];
				if (diffo > 0) {
					dfdt = diffo * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;
				}
			}
		}
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////




	/*
	if((m_pCurrentEntity->curstate.renderfx & ISNPC) && !( g_drawType == DRAWTYPE_VIEWMODEL)   ){
		//Check these?
		easyForcePrintLine("YO WHATS GOOD %d %d %d %d %.2f %.2f %.2f %.2f",
			m_pCurrentEntity->curstate.iuser1,
			m_pCurrentEntity->curstate.iuser2,
			m_pCurrentEntity->curstate.iuser3,
			m_pCurrentEntity->curstate.iuser4,
			m_pCurrentEntity->curstate.fuser1,
			m_pCurrentEntity->curstate.fuser2,
			m_pCurrentEntity->curstate.fuser3,
			m_pCurrentEntity->curstate.fuser4
		);
	}
	*/


	//dfdt = 0;
	//if(!strcmp(m_pCurrentEntity->curstate->
	
	if (pseqdesc->numframes <= 1)
	{
		f = 0;
	}
	else
	{
		f = (m_pCurrentEntity->curstate.frame * (pseqdesc->numframes - 1)) / 256.0;
	}
	DEBUG_old_f = f;

	f += dfdt;

	//MODDD - viewmodel idle anims may be forced not to loop, to remain static until a new anim is called.
	
	
	BOOL animateBackwards = ( g_drawType == DRAWTYPE_VIEWMODEL && (m_pCurrentEntity->curstate.iuser1 == 200)  );
	//animateBackwards = FALSE;
	/*
	BOOL animateBackwards = FALSE;
	if(m_pCurrentEntity->player & 2){
		m_pCurrentEntity->player &= ~2;
		animateBackwards = TRUE;
	}
	*/

	if( g_drawType == DRAWTYPE_VIEWMODEL ){
		//easyPrintLine("it is view model? %d ::: %d", m_pCurrentEntity->curstate.renderfx, m_pCurrentEntity->curstate.renderfx & FORCE_NOLOOP);
	}

	if(animateBackwards){
		
		f = pseqdesc->numframes - f - 1;
	}

	if( g_drawType == DRAWTYPE_VIEWMODEL){
		//easyPrintLineDummy("ILL CUT YER NUTS OFF s:%d f:%.2f mf:%d b?%d", m_pCurrentEntity->curstate.sequence, f, pseqdesc->numframes, animateBackwards);
	}

	if (pseqdesc->flags & STUDIO_LOOPING &&  !( g_drawType == DRAWTYPE_VIEWMODEL && (m_pCurrentEntity->curstate.renderfx & FORCE_NOLOOP)) )
	//if (pseqdesc->flags & STUDIO_LOOPING) 
	{
		if (pseqdesc->numframes > 1)
		{
			//MODDD - Oh.  Casted to 'int' to remove whole multiples of the animation in frames instead, ok.
			f -= (int)(f / (pseqdesc->numframes - 1)) *  (pseqdesc->numframes - 1);
		}
		if (f < 0) 
		{
			f += (pseqdesc->numframes - 1);
		}
	}
	else 
	{
		//frames: 10.
		//allowed range: 0-9, inclusive.

		//inv: 9-0

		//0: 9
		//1: 8
		//...
		//7: 2
		//8: 1
		//9: 0

		//if(!animateBackwards){
			if (f >= pseqdesc->numframes - 1.001) 
			{
				f = pseqdesc->numframes - 1.001;
			}
			if (f < 0.0) 
			{
				f = 0.0;
			}
		//}
		/*
		else{
			
			//f += 1;
			if (f >= pseqdesc->numframes - 1.001) 
			{
				f = pseqdesc->numframes - 1.001;
			}
			if (f < 0.0) 
			{
				f = 0.0;
			}
			
		}
		*/
	}




	
	if( animateBackwards  ){
		//easyPrintLine("WHAT THE?? f:%.2g if:%.2g n:%d", f, (pseqdesc->numframes - f - 1), pseqdesc->numframes);
		//easyPrintLineDummy("I WILL please no %.2g %d", f, pseqdesc->numframes);
		//10 frames. values 0 - 8 valid.

		//f = pseqdesc->numframes - f - 1;
	}


	/*
WEAPONIN: 9 9
CLIENTFR: 160 14.00 15
CLIENTFR: 160 14.00 15
WEAPONIN: 9 9
CLIENTFR: 160 14.
*/
	//iAnim
	//m_pCurrentEntity->curstate.

	if(g_drawType == DRAWTYPE_VIEWMODEL  ){
		//easyPrintLine("CLIENT FRAME: %.2f %.2f %.2f :: %d %.2f", f, m_pCurrentEntity->curstate.frame, currentFrame, pseqdesc->numframes, dfdt );
		//easyPrintLine("CLIENTFR: %d %.2f %d", m_pCurrentEntity->curstate.renderfx, f, pseqdesc->numframes);

		//easyPrintLineDummy("RENDER: id:%d ai:%d seq:%d f:%.2f n:%d BACKWARDS: %d", m_pCurrentEntity->index, pseqdesc->animindex, m_pCurrentEntity->curstate.sequence, f, pseqdesc->numframes, animateBackwards );
	}


	// Debug printouts for seeing how the 'interp physics issue' fix above works.
	// Best with only one NPC on the map, kill in slow motion with CVars set to put it in the air a tiny bit (monsterKilledToss set to 1 or 2),
	// with a low host_frametime (like 0.001) and watch how these printouts change.  Compare to the printouts during just an idle animation,
	// without the headcrab in combat or anything (spawn with 'autosneaky 1') to see interp working correctly without the fix running nor needed.
	
	/*
	if (g_debugPrevTime != m_clTime) {
		
		if (m_pCurrentEntity->curstate.renderfx & ISNPC) {
			if (!DEBUG_NeededFix) {
				easyForcePrintLine("nofix recframe:%.3f times:%.3f,%.3f d:%.3f final:%.2f F:%.2f->%.2f", m_pCurrentEntity->curstate.frame, m_clTime, m_pCurrentEntity->curstate.animtime, (m_clTime - m_pCurrentEntity->curstate.animtime), dfdt, DEBUG_old_f, f);
			}
			else {
				easyForcePrintLine("yefix recframe:%.3f times:%.3f,%.3f d:%.3f final:%.2f F:%.2f->%.2f", m_pCurrentEntity->curstate.frame, m_clTime, g_OLDprevTime, (m_clTime - g_OLDprevTime), dfdt, DEBUG_old_f, f);
			}
			g_debugPrevTime = m_clTime;
			//easyForcePrintLine("MY INDEX: %d isnpc:%d", m_pCurrentEntity->index, (m_pCurrentEntity->curstate.renderfx & ISNPC) == ISNPC);
		}
	}
	*/

	

	return f;
}









// !!! TESTING
float CStudioModelRenderer::StudioEstimateFrameNOINTERP(mstudioseqdesc_t* pseqdesc)
{
	double dfdt;
	double f;
	//return 0;

	//??? StudioEstimateInterpolant

	//MODDD NOTICE - should other places have this "renderfx & STOPINTR" check too.?
	if (m_fDoInterp && !(m_pCurrentEntity->curstate.renderfx & STOPINTR))
	{
		if (m_clTime < m_pCurrentEntity->curstate.animtime)
		{
			dfdt = 0;
		}
		else
		{
			dfdt = (m_clTime - m_pCurrentEntity->curstate.animtime) * m_pCurrentEntity->curstate.framerate * pseqdesc->fps;

		}
	}
	else
	{
		dfdt = 0;
	}


	/*
	if((m_pCurrentEntity->curstate.renderfx & ISNPC) && !( g_drawType == DRAWTYPE_VIEWMODEL)   ){
		//Check these?
		easyForcePrintLine("YO WHATS GOOD %d %d %d %d %.2f %.2f %.2f %.2f",
			m_pCurrentEntity->curstate.iuser1,
			m_pCurrentEntity->curstate.iuser2,
			m_pCurrentEntity->curstate.iuser3,
			m_pCurrentEntity->curstate.iuser4,
			m_pCurrentEntity->curstate.fuser1,
			m_pCurrentEntity->curstate.fuser2,
			m_pCurrentEntity->curstate.fuser3,
			m_pCurrentEntity->curstate.fuser4
		);
	}
	*/


	//dfdt = 0;
	//if(!strcmp(m_pCurrentEntity->curstate->

	if (pseqdesc->numframes <= 1)
	{
		f = 0;
	}
	else
	{
		f = (m_pCurrentEntity->curstate.frame * (pseqdesc->numframes - 1)) / 256.0;
	}

	f += dfdt;

	//MODDD - viewmodel idle anims may be forced not to loop, to remain static until a new anim is called.


	BOOL animateBackwards = (g_drawType == DRAWTYPE_VIEWMODEL && (m_pCurrentEntity->curstate.iuser1 == 200));
	//animateBackwards = FALSE;
	/*
	BOOL animateBackwards = FALSE;
	if(m_pCurrentEntity->player & 2){
		m_pCurrentEntity->player &= ~2;
		animateBackwards = TRUE;
	}
	*/

	if (g_drawType == DRAWTYPE_VIEWMODEL) {
		//easyPrintLine("it is view model? %d ::: %d", m_pCurrentEntity->curstate.renderfx, m_pCurrentEntity->curstate.renderfx & FORCE_NOLOOP);
	}

	if (animateBackwards) {

		f = pseqdesc->numframes - f - 1;
	}

	if (g_drawType == DRAWTYPE_VIEWMODEL) {
		//easyPrintLineDummy("ILL CUT YER NUTS OFF s:%d f:%.2f mf:%d b?%d", m_pCurrentEntity->curstate.sequence, f, pseqdesc->numframes, animateBackwards);
	}

	if (pseqdesc->flags & STUDIO_LOOPING && !(g_drawType == DRAWTYPE_VIEWMODEL && (m_pCurrentEntity->curstate.renderfx & FORCE_NOLOOP)))
		//if (pseqdesc->flags & STUDIO_LOOPING) 
	{
		if (pseqdesc->numframes > 1)
		{
			f -= (int)(f / (pseqdesc->numframes - 1)) * (pseqdesc->numframes - 1);
		}
		if (f < 0)
		{
			f += (pseqdesc->numframes - 1);
		}
	}
	else
	{
		//frames: 10.
		//allowed range: 0-9, inclusive.

		//inv: 9-0

		//0: 9
		//1: 8
		//...
		//7: 2
		//8: 1
		//9: 0

		//if(!animateBackwards){
		if (f >= pseqdesc->numframes - 1.001)
		{
			f = pseqdesc->numframes - 1.001;
		}
		if (f < 0.0)
		{
			f = 0.0;
		}
		//}
		/*
		else{

			//f += 1;
			if (f >= pseqdesc->numframes - 1.001)
			{
				f = pseqdesc->numframes - 1.001;
			}
			if (f < 0.0)
			{
				f = 0.0;
			}

		}
		*/
	}

	if (animateBackwards) {
		//easyPrintLine("WHAT THE?? f:%.2g if:%.2g n:%d", f, (pseqdesc->numframes - f - 1), pseqdesc->numframes);
		//easyPrintLineDummy("I WILL please no %.2g %d", f, pseqdesc->numframes);
		//10 frames. values 0 - 8 valid.

		//f = pseqdesc->numframes - f - 1;
	}


	/*
WEAPONIN: 9 9
CLIENTFR: 160 14.00 15
CLIENTFR: 160 14.00 15
WEAPONIN: 9 9
CLIENTFR: 160 14.
*/
//iAnim
//m_pCurrentEntity->curstate.

	if (g_drawType == DRAWTYPE_VIEWMODEL) {
		//easyPrintLine("CLIENT FRAME: %.2f %.2f %.2f :: %d %.2f", f, m_pCurrentEntity->curstate.frame, currentFrame, pseqdesc->numframes, dfdt );
		//easyPrintLine("CLIENTFR: %d %.2f %d", m_pCurrentEntity->curstate.renderfx, f, pseqdesc->numframes);

		easyPrintLineDummy("RENDER: id:%d ai:%d seq:%d f:%.2f n:%d BACKWARDS: %d", m_pCurrentEntity->index, pseqdesc->animindex, m_pCurrentEntity->curstate.sequence, f, pseqdesc->numframes, animateBackwards);
	}

	return f;
}


















/*
====================
StudioSetupBones

====================
*/

//MODDD - new specific.
void CStudioModelRenderer::StudioSetupBones (){
	//assume false.
	StudioSetupBones(FALSE);
}

void CStudioModelRenderer::StudioSetupBones ( byte isReflection )
{
	for(int i1 = 0; i1 < MAXSTUDIOBONES; i1++){
		for(int i2 = 0; i2 < 3; i2++){
			for(int i3 = 0; i3 < 4; i3++){
				m_plighttransformMOD[i1][i2][i3] = (*m_plighttransform)[i1][i2][i3];
			}
		}
	}

	int				i;
	double			f;

	mstudiobone_t		*pbones;
	mstudioseqdesc_t	*pseqdesc;
	mstudioanim_t		*panim;

	static float	pos[MAXSTUDIOBONES][3];
	static vec4_t		q[MAXSTUDIOBONES];
	float			bonematrix[3][4];

	static float	pos2[MAXSTUDIOBONES][3];
	static vec4_t		q2[MAXSTUDIOBONES];
	static float	pos3[MAXSTUDIOBONES][3];
	static vec4_t		q3[MAXSTUDIOBONES];
	static float	pos4[MAXSTUDIOBONES][3];
	static vec4_t		q4[MAXSTUDIOBONES];


	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("O its a viewmodel??? id:%d seq:%d", m_pCurrentEntity->index, m_pCurrentEntity->curstate.sequence  );
	}


	//is this okay?
	m_pCurrentEntity->curstate.iuser1 = 0;

	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq) 
	{
		//easyPrintLineDummy("!! Sequence too high! 1");

		if( g_drawType == DRAWTYPE_VIEWMODEL){
			if(m_pCurrentEntity->curstate.sequence & 128){
				m_pCurrentEntity->curstate.sequence &= ~128;  //just take away the 128, 8th bit. 2^7th, starting from 2^0th (1) as the first bit.
				//m_pCurrentEntity->player |= 2;  //This is hacky as fuck.
				m_pCurrentEntity->curstate.iuser1 = 200;
			}else{
				
			}

			//Not using "64" as a flag for sequence anymore. Remove this later!
			if(m_pCurrentEntity->curstate.sequence & 64){
				m_pCurrentEntity->curstate.sequence &= ~64;
			}
		}

	}else{

	}
	
	
	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq){
		m_pCurrentEntity->curstate.sequence = 0;
		easyPrintLineDummy("hockey 1a");
	}



	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("AM I BACKWARDS id:%d b:%d", m_pCurrentEntity->index, m_pCurrentEntity->curstate.iuser1  );
	}


	pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + m_pCurrentEntity->curstate.sequence;

	f = StudioEstimateFrame( pseqdesc );

	if (m_pCurrentEntity->latched.prevframe > f)
	{
		//Con_DPrintf("%f %f\n", m_pCurrentEntity->prevframe, f );
	}

	panim = StudioGetAnim( m_pRenderModel, pseqdesc );
	StudioCalcRotations( pos, q, pseqdesc, panim, f, isReflection );

	if (pseqdesc->numblends > 1)
	{
		float s;
		float dadt;

		panim += m_pStudioHeader->numbones;
		StudioCalcRotations( pos2, q2, pseqdesc, panim, f, isReflection );

		dadt = StudioEstimateInterpolant();
		s = (m_pCurrentEntity->curstate.blending[0] * dadt + m_pCurrentEntity->latched.prevblending[0] * (1.0 - dadt)) / 255.0;

		StudioSlerpBones( q, pos, q2, pos2, s );

		if (pseqdesc->numblends == 4)
		{
			panim += m_pStudioHeader->numbones;
			StudioCalcRotations( pos3, q3, pseqdesc, panim, f, isReflection );

			panim += m_pStudioHeader->numbones;
			StudioCalcRotations( pos4, q4, pseqdesc, panim, isReflection );

			s = (m_pCurrentEntity->curstate.blending[0] * dadt + m_pCurrentEntity->latched.prevblending[0] * (1.0 - dadt)) / 255.0;
			StudioSlerpBones( q3, pos3, q4, pos4, s );

			s = (m_pCurrentEntity->curstate.blending[1] * dadt + m_pCurrentEntity->latched.prevblending[1] * (1.0 - dadt)) / 255.0;
			StudioSlerpBones( q, pos, q3, pos3, s );
		}
	}
	





	//MODDD - Is something in here to blame for a sequence recently hitting the ground having slower anim interpolation
	// (catches up soon and looks a little jarring?)
	// Actually no, at least not more than negligibly.
	// See the 'interp physics issue' comment.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	if (m_fDoInterp &&
		m_pCurrentEntity->latched.sequencetime &&
		( m_pCurrentEntity->latched.sequencetime + 0.2 > m_clTime ) && 
		( m_pCurrentEntity->latched.prevsequence < m_pStudioHeader->numseq ))
	{
		// blend from last sequence
		static float	pos1b[MAXSTUDIOBONES][3];
		static vec4_t	q1b[MAXSTUDIOBONES];
		float			s;

		pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + m_pCurrentEntity->latched.prevsequence;
		panim = StudioGetAnim( m_pRenderModel, pseqdesc );
		// clip prevframe
		StudioCalcRotations( pos1b, q1b, pseqdesc, panim, m_pCurrentEntity->latched.prevframe, isReflection );

		if (pseqdesc->numblends > 1)
		{
			panim += m_pStudioHeader->numbones;
			StudioCalcRotations( pos2, q2, pseqdesc, panim, m_pCurrentEntity->latched.prevframe, isReflection );

			s = (m_pCurrentEntity->latched.prevseqblending[0]) / 255.0;
			StudioSlerpBones( q1b, pos1b, q2, pos2, s );

			if (pseqdesc->numblends == 4)
			{
				panim += m_pStudioHeader->numbones;
				StudioCalcRotations( pos3, q3, pseqdesc, panim, m_pCurrentEntity->latched.prevframe, isReflection );

				panim += m_pStudioHeader->numbones;
				StudioCalcRotations( pos4, q4, pseqdesc, panim, m_pCurrentEntity->latched.prevframe, isReflection );

				s = (m_pCurrentEntity->latched.prevseqblending[0]) / 255.0;
				StudioSlerpBones( q3, pos3, q4, pos4, s );

				s = (m_pCurrentEntity->latched.prevseqblending[1]) / 255.0;
				StudioSlerpBones( q1b, pos1b, q3, pos3, s );
			}
		}

		s = 1.0 - (m_clTime - m_pCurrentEntity->latched.sequencetime) / 0.2;
		StudioSlerpBones( q, pos, q1b, pos1b, s );
	}
	else
	{
		//Con_DPrintf("prevframe = %4.2f\n", f);
		m_pCurrentEntity->latched.prevframe = f;
	}
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







	pbones = (mstudiobone_t *)((byte *)m_pStudioHeader + m_pStudioHeader->boneindex);

	// calc gait animation
	if (m_pPlayerInfo && m_pPlayerInfo->gaitsequence != 0)
	{
		if (m_pPlayerInfo->gaitsequence >= m_pStudioHeader->numseq) 
		{
			m_pPlayerInfo->gaitsequence = 0;
		}

		pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + m_pPlayerInfo->gaitsequence;

		panim = StudioGetAnim( m_pRenderModel, pseqdesc );
		StudioCalcRotations( pos2, q2, pseqdesc, panim, m_pPlayerInfo->gaitframe, isReflection );

		for (i = 0; i < m_pStudioHeader->numbones; i++)
		{
			if (strcmp( pbones[i].name, "Bip01 Spine") == 0)
				break;
			memcpy( pos[i], pos2[i], sizeof( pos[i] ));
			memcpy( q[i], q2[i], sizeof( q[i] ));
		}
	}


	for (i = 0; i < m_pStudioHeader->numbones; i++) 
	{
		QuaternionMatrix( q[i], bonematrix );

		bonematrix[0][3] = pos[i][0];
		bonematrix[1][3] = pos[i][1];
		bonematrix[2][3] = pos[i][2];

		
		
		// WOA THIS ONES FREAKY
		/*
		float scalex = EASY_CVAR_GET(ctt1);
		float scaley = EASY_CVAR_GET(ctt2);
		float scalez = EASY_CVAR_GET(ctt3);
		bonematrix[0][0] *= 1;
		bonematrix[1][0] *= 1;
		bonematrix[2][0] *= 1;
		bonematrix[0][1] *= scalex;
		bonematrix[1][1] *= scaley;
		bonematrix[2][1] *= scalez;
		bonematrix[0][2] *= 1;
		bonematrix[1][2] *= 1;
		bonematrix[2][2] *= 1;
		*/
		



		if (pbones[i].parent == -1) 
		{
			//????
			//ConcatTransforms (viewmatrix, (*m_protationmatrix), (*m_paliastransform));
			
			//MODDDMIRROR NOTE: this line seems pretty important!
			if ( IEngineStudio.IsHardware() )
			{
				//TODO: make a flag that checks for only the view model (1st person).

				ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_pbonetransform)[i]);

				//if(EASY_CVAR_GET(chromeEffect) != 1 || !g_drawType == DRAWTYPE_VIEWMODEL ){
				if(EASY_CVAR_GET(chromeEffect) != 1){
					// MatrixCopy should be faster...
					//ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransform)[i]);
					MatrixCopy( (*m_pbonetransform)[i], (*m_plighttransform)[i] );
					
					MatrixCopy( (*m_pbonetransform)[i], (m_plighttransformMOD)[i] );
					//??????????
					//ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransform)[i]);
					//ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransformMOD)[i]);
				}else{
					
					float (m_protationmatrixClone)[ 3 ][ 4 ];

					/*
					//no, let it fall through from the previous time (likely StudioSetUpTransform)
					(*m_protationmatrix)[0][3] = m_pCurrentEntity->origin[0];
					(*m_protationmatrix)[1][3] = m_pCurrentEntity->origin[1];
					(*m_protationmatrix)[2][3] = m_pCurrentEntity->origin[2];
					*/

					for(int i1 = 0; i1 < 3; i1++){
						for(int i2 = 0; i2 < 4; i2++){
							m_protationmatrixClone[i1][i2] = (*m_protationmatrix)[i1][i2];
						}
					}
					(m_protationmatrixClone)[0][3] -= m_vRenderOrigin[0];
					(m_protationmatrixClone)[1][3] -= m_vRenderOrigin[1];
					(m_protationmatrixClone)[2][3] -= m_vRenderOrigin[2];


					ConcatTransforms ((*m_protationmatrix), bonematrix, (m_plighttransformMOD)[i]);
					ConcatTransforms ((m_protationmatrixClone), bonematrix, (*m_plighttransform)[i]);

				}

				//Software:
				//~position = (my)origin - renderOrigin
				//~origin = (my)origin

				//Hardware
				//~position = (my)origin
				//~origin = (my)origin - renderOrigin  (?)


			}
			else
			{
				//If software...
				
				ConcatTransforms ((*m_paliastransform), bonematrix, (*m_pbonetransform)[i]);
				
				for(int i1 = 0; i1 < MAXSTUDIOBONES; i1++){
					for(int i2 = 0; i2 < 3; i2++){
						for(int i3 = 0; i3 < 4; i3++){
							m_plighttransformMOD[i1][i2][i3] = (*m_plighttransform)[i1][i2][i3];
						}
					}
				}

				/*
				if( g_drawType == DRAWTYPE_VIEWMODEL ){
					easyPrintLine("viewmodel TIME!!!!");
				}
				*/

				//if(EASY_CVAR_GET(chromeEffect) != 1 || !( g_drawType == DRAWTYPE_VIEWMODEL ) ){
				if(EASY_CVAR_GET(chromeEffect) != 1){

					/*
					//no, let it fall through from the previous time (likely StudioSetUpTransform)
					(*m_protationmatrix)[0][3] = m_pCurrentEntity->origin[0];
					(*m_protationmatrix)[1][3] = m_pCurrentEntity->origin[1];
					(*m_protationmatrix)[2][3] = m_pCurrentEntity->origin[2];
					*/

					ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransform)[i]);
					ConcatTransforms ((*m_protationmatrix), bonematrix, (m_plighttransformMOD)[i]);

				}else{
					
					float (m_protationmatrixClone)[ 3 ][ 4 ];

					/*
					//no, let it fall through from the previous time (likely StudioSetUpTransform)
					(*m_protationmatrix)[0][3] = m_pCurrentEntity->origin[0];
					(*m_protationmatrix)[1][3] = m_pCurrentEntity->origin[1];
					(*m_protationmatrix)[2][3] = m_pCurrentEntity->origin[2];
					*/

					for(int i1 = 0; i1 < 3; i1++){
						for(int i2 = 0; i2 < 4; i2++){
							m_protationmatrixClone[i1][i2] = (*m_protationmatrix)[i1][i2];
						}
					}

					//checkpoint4
					
					(m_protationmatrixClone)[0][3] -= m_vRenderOrigin[0];
					(m_protationmatrixClone)[1][3] -= m_vRenderOrigin[1];
					(m_protationmatrixClone)[2][3] -= m_vRenderOrigin[2];
					
					ConcatTransforms ((*m_protationmatrix), bonematrix, (m_plighttransformMOD)[i]);
					ConcatTransforms ((m_protationmatrixClone), bonematrix, (*m_plighttransform)[i]);

				}

			}//END OF hardware / software rendering-mode check

			//*(m_pbonetransform[i][0][1]) *= 1;
			//*(m_pbonetransform[i][1][1]) *= -1;
			//*(m_pbonetransform[i][2][1]) *= 1;
			

			
			//MODDD - well I tried.
			/*
			if (isReflection) {
				float scalex = EASY_CVAR_GET(ctt1);
				float scaley = EASY_CVAR_GET(ctt2);
				float scalez = EASY_CVAR_GET(ctt3);
				(*m_pbonetransform[i])[0][0] *= 1;
				(*m_pbonetransform[i])[1][0] *= 1;
				(*m_pbonetransform[i])[2][0] *= 1;
				(*m_pbonetransform[i])[0][1] *= scalex;
				(*m_pbonetransform[i])[1][1] *= scaley;
				(*m_pbonetransform[i])[2][1] *= scalez;
				(*m_pbonetransform[i])[0][2] *= 1;
				(*m_pbonetransform[i])[1][2] *= 1;
				(*m_pbonetransform[i])[2][2] *= 1;
			}
			*/
			

			// Apply client-side effects to the transformation matrix
			StudioFxTransform( m_pCurrentEntity, (*m_pbonetransform)[i] );
			
		}
		else 
		{
			ConcatTransforms ((*m_pbonetransform)[pbones[i].parent], bonematrix, (*m_pbonetransform)[i]);
			ConcatTransforms ((*m_plighttransform)[pbones[i].parent], bonematrix, (*m_plighttransform)[i]);
			ConcatTransforms ((m_plighttransformMOD)[pbones[i].parent], bonematrix, (m_plighttransformMOD)[i]);

		}

		/*
		(*m_pbonetransform)[i][0][3] = 0;
		(*m_pbonetransform)[i][1][3] = 0;
		(*m_pbonetransform)[i][2][3] = 0;
		(*m_pbonetransform)[i][3][3] = 0;
		*/

	}//END OF for loop
}


/*
====================
StudioSaveBones

====================
*/
void CStudioModelRenderer::StudioSaveBones( void )
{
	int	i;

	mstudiobone_t		*pbones;
	pbones = (mstudiobone_t *)((byte *)m_pStudioHeader + m_pStudioHeader->boneindex);

	m_nCachedBones = m_pStudioHeader->numbones;

	for (i = 0; i < m_pStudioHeader->numbones; i++) 
	{
		strcpy( m_nCachedBoneNames[i], pbones[i].name );
		MatrixCopy( (*m_pbonetransform)[i], m_rgCachedBoneTransform[i] );
		MatrixCopy( (*m_plighttransform)[i], m_rgCachedLightTransform[i] );
	}
}


/*
====================
StudioMergeBones

====================
*/

//MODDD - new specific
void CStudioModelRenderer::StudioMergeBones ( model_t *m_pSubModel){
	StudioMergeBones(m_pSubModel, FALSE);
}

void CStudioModelRenderer::StudioMergeBones ( model_t *m_pSubModel, byte isReflection )
{
	int				i, j;
	double			f;
	int				do_hunt = true;

	mstudiobone_t		*pbones;
	mstudioseqdesc_t	*pseqdesc;
	mstudioanim_t		*panim;

	static float	pos[MAXSTUDIOBONES][3];
	float			bonematrix[3][4];
	static vec4_t		q[MAXSTUDIOBONES];

	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq) 
	{
		if( g_drawType == DRAWTYPE_VIEWMODEL){
			//easyPrintLineDummy("!! Sequence too high! 2");
			if(m_pCurrentEntity->curstate.sequence & 128){
				m_pCurrentEntity->curstate.sequence &= ~128;  //that is all? remove this bit(ch).
			}
		
			if(m_pCurrentEntity->curstate.sequence & 64){
				m_pCurrentEntity->curstate.sequence &= ~64;
			}
		}

	}

	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq){
		easyPrintLineDummy("hockey 3");
		m_pCurrentEntity->curstate.sequence = 0;
	}




	pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + m_pCurrentEntity->curstate.sequence;

	f = StudioEstimateFrame( pseqdesc );

	if (m_pCurrentEntity->latched.prevframe > f)
	{
		//Con_DPrintf("%f %f\n", m_pCurrentEntity->prevframe, f );
	}

	panim = StudioGetAnim( m_pSubModel, pseqdesc );
	StudioCalcRotations( pos, q, pseqdesc, panim, f, isReflection );

	pbones = (mstudiobone_t *)((byte *)m_pStudioHeader + m_pStudioHeader->boneindex);


	for (i = 0; i < m_pStudioHeader->numbones; i++) 
	{
		for (j = 0; j < m_nCachedBones; j++)
		{
			if (stricmp(pbones[i].name, m_nCachedBoneNames[j]) == 0)
			{
				MatrixCopy( m_rgCachedBoneTransform[j], (*m_pbonetransform)[i] );
				MatrixCopy( m_rgCachedLightTransform[j], (*m_plighttransform)[i] );
				break;
			}
		}
		if (j >= m_nCachedBones)
		{
			QuaternionMatrix( q[i], bonematrix );

			bonematrix[0][3] = pos[i][0];
			bonematrix[1][3] = pos[i][1];
			bonematrix[2][3] = pos[i][2];

			if (pbones[i].parent == -1) 
			{
				if ( IEngineStudio.IsHardware() )
				{
					ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_pbonetransform)[i]);

					// MatrixCopy should be faster...
					//ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransform)[i]);
					MatrixCopy( (*m_pbonetransform)[i], (*m_plighttransform)[i] );
				}
				else
				{
					ConcatTransforms ((*m_paliastransform), bonematrix, (*m_pbonetransform)[i]);
					ConcatTransforms ((*m_protationmatrix), bonematrix, (*m_plighttransform)[i]);
				}

				// Apply client-side effects to the transformation matrix
				StudioFxTransform( m_pCurrentEntity, (*m_pbonetransform)[i] );
			} 
			else 
			{
				ConcatTransforms ((*m_pbonetransform)[pbones[i].parent], bonematrix, (*m_pbonetransform)[i]);
				ConcatTransforms ((*m_plighttransform)[pbones[i].parent], bonematrix, (*m_plighttransform)[i]);
			}
		}
	}
}






//MODDDMIRROR.  no duh.

int CStudioModelRenderer::StudioDrawReflection(int flags)
{
	alight_t lighting;
	vec3_t dir;



	/*
	//MODDD - well I tried.  Any attempt to make something look truly 'reflected' (flipped) still keeps several faces on the model going the wrong way,
	// so you see them show the backs through (such as seeing a scientist's face through the back of the head).
	float scalex = EASY_CVAR_GET(ctt1);
	float scaley = EASY_CVAR_GET(ctt2);
	float scalez = EASY_CVAR_GET(ctt3);
	(*m_protationmatrix)[0][0] *= 1;
	(*m_protationmatrix)[1][0] *= 1;
	(*m_protationmatrix)[2][0] *= 1;
	(*m_protationmatrix)[0][1] *= scalex;
	(*m_protationmatrix)[1][1] *= scaley;
	(*m_protationmatrix)[2][1] *= scalez;
	(*m_protationmatrix)[0][2] *= 1;
	(*m_protationmatrix)[1][2] *= 1;
	(*m_protationmatrix)[2][2] *= 1;
	*/



	//NOTE: used to be "TRI_FRONT"?
	gEngfuncs.pTriAPI->CullFace(TRI_NONE);

	if (flags & STUDIO_RENDER)
	{
		// see if the bounding box lets us trivially reject, also sets
		if (!IEngineStudio.StudioCheckBBox()) {//no need disabled frustrum cull for "mirroring" models. G-Cont
			//MODDD - just try the next mirror.
			return 0;
			//continue;
		}
		(*m_pModelsDrawn)++;
		(*m_pStudioModelCount)++; // render data cache cookie

		if (m_pStudioHeader->numbodyparts == 0) {
			//MODDD - just try the next mirror.
			return 1;
			//continue;
		}
	}


	if (m_pCurrentEntity->curstate.movetype == MOVETYPE_FOLLOW)
	{
		//StudioMergeBones( m_pRenderModel );
		//head fix.
		StudioMergeBones(m_pRenderModel, TRUE);
	}
	else
	{
		//StudioSetupBones( );
		StudioSetupBones(TRUE);
	}
	StudioSaveBones();

	//MODDD - ATTACHMENTS - seems unconnected to either part of the player?
	if (flags & STUDIO_EVENTS)
	{
		StudioCalcAttachments();
		IEngineStudio.StudioClientEvents();
		// copy attachments into global entity array

		if (m_pCurrentEntity->index > 0)
		{
			cl_entity_t* ent = gEngfuncs.GetEntityByIndex(m_pCurrentEntity->index);
			memcpy(ent->attachment, m_pCurrentEntity->attachment, sizeof(vec3_t) * 4);
		}
	}
	if (flags & STUDIO_RENDER)
	{
		lighting.plightvec = dir;
		IEngineStudio.StudioDynamicLight(m_pCurrentEntity, &lighting);

		IEngineStudio.StudioEntityLight(&lighting);

		// model and frame independant
		IEngineStudio.StudioSetupLighting(&lighting);

		// get remap colors
		m_nTopColor = m_pCurrentEntity->curstate.colormap & 0xFF;
		m_nBottomColor = (m_pCurrentEntity->curstate.colormap & 0xFF00) >> 8;

		IEngineStudio.StudioSetRemapColors(m_nTopColor, m_nBottomColor);

		StudioRenderModel();
	}
	//gEngfuncs.pTriAPI->CullFace( TRI_FRONT );
	gEngfuncs.pTriAPI->CullFace(TRI_NONE);

	return 1;
}



//MODDD - TEMPREF
extern void RenderFog();


/*
====================
StudioDrawModel

====================
*/

#include "triangleapi.h"
int CStudioModelRenderer::StudioDrawModel( int flags )
{
#if STUDIO_NO_RENDERING == 1
	return 0;
#endif


	//easyPrintLine("FLAGZ %d", (flags & 128));
	alight_t lighting;
	vec3_t dir;

	IEngineStudio.GetTimes( &m_nFrameCount, &m_clTime, &m_clOldTime );

	//MODDDMIRROR - makes the player show up in first person (for mirror reflections), wtf?
	// ALSO, looks like this is how to judge the start of a new frame.  Niiiiice that this is what we have to
	// resort to.    Thanks though Spirit of HL.
	if (m_nCachedFrameCount != m_nFrameCount)
	{
		b_PlayerMarkerParsed = false;
		m_nCachedFrameCount = m_nFrameCount;

		//MODDD - while we're at it, refresh g_viewModelRef.  Just safety, and no reason to do after the
		// start of a frame
		g_viewModelRef = gEngfuncs.GetViewModel();

	}//END OF frameCount checks
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//if (!strcmp(m_pCurrentEntity->model->name,"models/null.mdl"))
	//if(  (m_pCurrentEntity->curstate.renderfx & RENDERFX_PRIMARY_BITS) == kRenderFxHologram)

	//(g_drawType == DRAWTYPE_PLAYER)   no, to determine whether to make the first-person reflection, we don't need the actual player on hand.
	

	//easyPrintLineDummy("OH no MY friend %.2f %.2f", global2PSEUDO_IGNOREcameraMode, EASY_CVAR_GET(mirrorsDoNotReflectPlayer));
	if(  global2PSEUDO_IGNOREcameraMode == 0 && EASY_CVAR_GET(mirrorsDoNotReflectPlayer) != 1)
	{
		if (!b_PlayerMarkerParsed)
		{
			cl_entity_t *player = gEngfuncs.GetLocalPlayer();
			entity_state_t *shinyplr = IEngineStudio.GetPlayerState( 0 );

			int save_interp;
			save_interp = m_fDoInterp;
			m_fDoInterp = 0;

			// draw as though it were a player
			flags |= 2048;

			//cl_entity_t* tempMem = m_pCurrentEntity;

			m_pCurrentEntity = player;

			// Ordinarily it would be a good idea to save the g_drawType for restoring after this call,
			// but it hasn't been set yet for the real current "m_pCurrentEntity".  So no need.
			// This sets g_drawType to DRAWTYPE_PLAYER for any other calls it makes on its own.
			StudioDrawPlayer( flags, shinyplr );

			//MODDD - why keep that 2048 bit after the draw call???
			flags &= ~2048;

			b_PlayerMarkerParsed = true;
			m_fDoInterp = save_interp;

			//m_pCurrentEntity = tempMem;
		}
		//return 1;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	m_pCurrentEntity = IEngineStudio.GetCurrentEntity();
	
	//MODDD - so is it any ordinary entity or the viewmodel?
	// The viewmodel is a special case that needs some things handled differnetly, as the server
	// has no direct interaction with it and so customization, even playing anims backwards, has
	// to be done a lot differently.
	if (g_viewModelRef != m_pCurrentEntity) {
		g_drawType = DRAWTYPE_ENTITY;
	}
	else {
		g_drawType = DRAWTYPE_VIEWMODEL;
	}




	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG1. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	
	if(EASY_CVAR_GET(cl_interp_entity) != 0){
		// not off, typical setting.
		m_fDoInterp = 1;
	}else{
		// off, only apply to server-sent entities. this is too bizarre (frozen on first frame) for viewmodel stuff.
		// Leaving that to its own CVar 'cl_interp_viewmodel' now.
		if( g_drawType == DRAWTYPE_VIEWMODEL){
			//usual setting
			m_fDoInterp = 1;
		}else{
			//don't
			m_fDoInterp = 0;
		}
	}


	/*
	if(m_pCurrentEntity->curstate.iuser1 == 66){
		easyPrintLineDummy("OH niiiii MY MAN %d", m_pCurrentEntity->curstate.iuser1);
	}
	*/

	//MODDD - only check against the primary bits.
	if( ( (m_pCurrentEntity->curstate.renderfx & RENDERFX_PRIMARY_BITS) == kRenderFxDummy) ){
		//Don't render, this model was probably made just to grap the rendering system's attention
		//for at least one call this frame.
		return 0;
	}

	//MODDD - if this is the model, the user turned it off for some reason. Don't draw.
	if(EASY_CVAR_GET(drawViewModel) <= 0 && g_drawType == DRAWTYPE_VIEWMODEL ){
		return 0;
	}

	IEngineStudio.GetTimes( &m_nFrameCount, &m_clTime, &m_clOldTime );
	IEngineStudio.GetViewInfo( m_vRenderOrigin, m_vUp, m_vRight, m_vNormal );
	IEngineStudio.GetAliasScale( &m_fSoftwareXScale, &m_fSoftwareYScale );


	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG2. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}
	

	//m_pCurrentEntity->curstate.renderfx

	//huh, flags yo.
	//m_pCurrentEntity->curstate.renderfx
	if(m_pCurrentEntity->curstate.renderfx & ISNPC){
		//easyPrintLine("da anglez %.2f %.2f %.2f", m_pCurrentEntity->angles[0], m_pCurrentEntity->angles[1], m_pCurrentEntity->angles[2]);
		//m_pCurrentEntity->angles[0] += 1;
		//m_pCurrentEntity->angles[1] += 1;
		//m_pCurrentEntity->angles[2] += 1;
	}


	/*
	easyPrintLineDummy("MY FLAGS?! %d RENDERFX: %d BITS: %d%d%d%d%d", flags, m_pCurrentEntity->curstate.renderfx,
		(m_pCurrentEntity->curstate.renderfx&1)!=0,
		(m_pCurrentEntity->curstate.renderfx&2)!=0,
		(m_pCurrentEntity->curstate.renderfx&4)!=0,
		(m_pCurrentEntity->curstate.renderfx&8)!=0,
		(m_pCurrentEntity->curstate.renderfx&16)!=0
	);
	*/

	//if(flags & 128){
		//m_vRenderOrigin[0] += 66;
		//m_vRenderOrigin[1] += 66;
		//m_vRenderOrigin[2] += 66;
		
	//WOAH.... WOAH. not do that...
	/*
		m_pCurrentEntity->angles[0] = 0;
		m_pCurrentEntity->angles[1] = 0;
		m_pCurrentEntity->angles[2] = 0;
		*/
	//}

	//MODDD - old location of 1st-person drawing logic.

	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG3. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}
	
	//MODDD - only check against the primary bits.
	if ((m_pCurrentEntity->curstate.renderfx & RENDERFX_PRIMARY_BITS) == kRenderFxDeadPlayer)
	{
		entity_state_t deadplayer;

		int result;
		int save_interp;

		//MODDD NOTE - so "renderamt" records what number'd player this is?
		// oooo-kay.   var re-use can be weird.
		if (m_pCurrentEntity->curstate.renderamt <= 0 || m_pCurrentEntity->curstate.renderamt > gEngfuncs.GetMaxClients() )
			return 0;

		// get copy of player
		deadplayer = *(IEngineStudio.GetPlayerState( m_pCurrentEntity->curstate.renderamt - 1 )); //cl.frames[cl.parsecount & CL_UPDATE_MASK].playerstate[m_pCurrentEntity->curstate.renderamt-1];

		// clear weapon, movement state
		deadplayer.number = m_pCurrentEntity->curstate.renderamt;
		deadplayer.weaponmodel = 0;
		deadplayer.gaitsequence = 0;

		deadplayer.movetype = MOVETYPE_NONE;
		VectorCopy_f( m_pCurrentEntity->curstate.angles, deadplayer.angles );
		VectorCopy_f( m_pCurrentEntity->curstate.origin, deadplayer.origin );

		save_interp = m_fDoInterp;
		m_fDoInterp = 0;
		
		// draw as though it were a player
		result = StudioDrawPlayer( flags, &deadplayer );

		// WARNING!!! That call overwrites g_drawType.
		// However this method ends soon after, so not really concerned.  Keep in mind for future reference if much chances though.
		
		m_fDoInterp = save_interp;
		return result;
	}
	
	//MODDD - brute force tactic.  Not advisable for real use.
	//m_pCurrentEntity->curstate.body = 1;

	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG4. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	m_pRenderModel = m_pCurrentEntity->model;
	m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (m_pRenderModel);
	IEngineStudio.StudioSetHeader( m_pStudioHeader );
	IEngineStudio.SetRenderModel( m_pRenderModel );




	/*
	int ic = 0;

	StudioSetUpTransform(1024 + ic); //HELP ME
	//THIS USED TO GO BEFORE!!!

	mirror_id = ic;
	//NOTE: used to be "TRI_FRONT"?
	gEngfuncs.pTriAPI->CullFace(TRI_NONE);

	if (flags & STUDIO_RENDER)
	{
		// see if the bounding box lets us trivially reject, also sets
		if (!IEngineStudio.StudioCheckBBox()) {//no need disabled frustrum cull for "mirroring" models. G-Cont
			//MODDD - just try the next mirror.
			//return 0;
			//continue;
		}
		(*m_pModelsDrawn)++;
		(*m_pStudioModelCount)++; // render data cache cookie

		if (m_pStudioHeader->numbodyparts == 0) {
			//MODDD - just try the next mirror.
			//return 1;
			//continue;
		}
	}

	
	if (m_pCurrentEntity->curstate.movetype == MOVETYPE_FOLLOW)
	{
		//StudioMergeBones( m_pRenderModel );
		//head fix.
		StudioMergeBones( m_pRenderModel, TRUE );
	}
	else
	{
		//StudioSetupBones( );
		StudioSetupBones(TRUE);
	}
	StudioSaveBones( );

	//MODDD - ATTACHMENTS - seems unconnected to either part of the player?
	if (flags & STUDIO_EVENTS)
	{
		StudioCalcAttachments( );
		IEngineStudio.StudioClientEvents( );
		// copy attachments into global entity array

		if ( m_pCurrentEntity->index > 0 )
		{
			cl_entity_t *ent = gEngfuncs.GetEntityByIndex( m_pCurrentEntity->index );
			memcpy( ent->attachment, m_pCurrentEntity->attachment, sizeof( vec3_t ) * 4 );
		}
	}
	
	
	if (flags & STUDIO_RENDER)
	{
		lighting.plightvec = dir;
		IEngineStudio.StudioDynamicLight(m_pCurrentEntity, &lighting );

		IEngineStudio.StudioEntityLight( &lighting );

		// model and frame independant
		IEngineStudio.StudioSetupLighting (&lighting);

		// get remap colors
		m_nTopColor = m_pCurrentEntity->curstate.colormap & 0xFF;
				m_nBottomColor = (m_pCurrentEntity->curstate.colormap & 0xFF00) >> 8;

		IEngineStudio.StudioSetRemapColors( m_nTopColor, m_nBottomColor );

		StudioRenderModel( );
	}
	//gEngfuncs.pTriAPI->CullFace( TRI_FRONT );
	gEngfuncs.pTriAPI->CullFace( TRI_NONE );

	*/











	StudioSetUpTransform( 0 );
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG5. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	//MODDD - NEW.
	gEngfuncs.pTriAPI->CullFace(TRI_NONE);

	if (flags & STUDIO_RENDER)
	{

		//MODDD MIRROR
		/////////////////////////////////////////////////////////////////////////////////////////////
		//
		//// see if the bounding box lets us trivially reject, also sets
		//if (!IEngineStudio.StudioCheckBBox ())
		//	return 0;
		//
		//(*m_pModelsDrawn)++;
		//(*m_pStudioModelCount)++; // render data cache cookie
		//
		//if (m_pStudioHeader->numbodyparts == 0)
		//	return 1;
		//
		/////////////////////////////////////////////////////////////////////////////////////////////

		if( g_drawType == DRAWTYPE_VIEWMODEL){
			easyPrintLineDummy("FLAG5a. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
		}

		//HACK - it is possible a call to "IEngineStudio.StudioCheckBBox" resets curstate.sequence to 0 if it is set to an invalid value.
		//Need to maintain the old value if it must be kept regardless.

		int oldSeq = m_pCurrentEntity->curstate.sequence;

		//play it safe.
		if( g_drawType == DRAWTYPE_VIEWMODEL){
			m_pCurrentEntity->curstate.sequence &= ~(128 | 64);
		}

		if (!IEngineStudio.StudioCheckBBox ())
		{
			vec3_t delta;
			float dist;
			if( g_drawType == DRAWTYPE_VIEWMODEL){
				easyPrintLineDummy("FLAG5b. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
			}
			VectorSubtract_f(gHUD.Mirrors[mirror_id].origin,m_pCurrentEntity->origin,delta);
			dist = Length(delta);
			if( g_drawType == DRAWTYPE_VIEWMODEL){
				easyPrintLineDummy("FLAG5c. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
			}
			
			if ((gHUD.numMirrors<0) || (gHUD.Mirrors[mirror_id].radius < dist)){
				return 0;
			}
        }//END OF !StudioCheckBBox check
		
		(*m_pModelsDrawn)++;
		(*m_pStudioModelCount)++; // render data cache cookie
		if( g_drawType == DRAWTYPE_VIEWMODEL){
			easyPrintLineDummy("FLAG5d. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
		}

		if (m_pStudioHeader->numbodyparts == 0)
			return 1;
		/////////////////////////////////////////////////////////////////////////////////////////////

		//Now, restore the sequence!
		m_pCurrentEntity->curstate.sequence = oldSeq;

	}//END OF STUDIO_RENDER check
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG6. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}


	if (m_pCurrentEntity->curstate.movetype == MOVETYPE_FOLLOW)
	{
		StudioMergeBones( m_pRenderModel );
	}
	else
	{
		StudioSetupBones( );
	}
	StudioSaveBones( );
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG7. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	//MODDD - ATTACHMENTS - This seems to include the player's first-person effect start point.
	//MODDD - IMPORTANT WARNING.
	//This alsocalls events for even serverside entities sent over, like monster models.
	//Unfortunately StudioClientEvents() is hardcoded so it looks impossible.
	//to tell it to behave differently with interpolation turned off. Oh well.
		


	if (flags & STUDIO_EVENTS)
	{
		if(m_pCurrentEntity->curstate.renderfx & ISNPC){
			easyPrintLineDummy("OHdear");
		}

		//Setting the framerate to 0 before doing attachments can fool StudioClientEvents into thinking no time passed?
		//Bizarre, see if this is needed to convey frozen parts of things with STOPINTR on.
		//m_pCurrentEntity->curstate.framerate = 0;
		//m_fDoInterp = 0;

		////////////Uncomment these to see.
		//float oldFramerate = m_pCurrentEntity->curstate.framerate;
		//m_pCurrentEntity->curstate.framerate = 0;
		StudioCalcAttachments( );
		//m_pCurrentEntity->curstate.framerate = oldFramerate;

		IEngineStudio.StudioClientEvents( );
		// copy attachments into global entity array
		if ( m_pCurrentEntity->index > 0 )
		{
			cl_entity_t *ent = gEngfuncs.GetEntityByIndex( m_pCurrentEntity->index );

			memcpy( ent->attachment, m_pCurrentEntity->attachment, sizeof( vec3_t ) * 4 );
		}
	}

	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG8. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}
	
	if (flags & STUDIO_RENDER)
	{
		//DISABLE BLOCK FOR silhouette MODE!
		//////////////////////////////////////////////////////////////////////

		lighting.plightvec = dir;
		IEngineStudio.StudioDynamicLight(m_pCurrentEntity, &lighting);

		IEngineStudio.StudioEntityLight(&lighting);

		// model and frame independant
		IEngineStudio.StudioSetupLighting(&lighting);

		//////////////////////////////////////////////////////////////////////////
		
		// get remap colors
		m_nTopColor = m_pCurrentEntity->curstate.colormap & 0xFF;
		m_nBottomColor = (m_pCurrentEntity->curstate.colormap & 0xFF00) >> 8;

		IEngineStudio.StudioSetRemapColors(m_nTopColor, m_nBottomColor);

		StudioRenderModel();
		//////////////////////////////////////////////////////////////////////////

		gEngfuncs.pTriAPI->CullFace(TRI_NONE);
	}
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG9. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	//MODDD -
	//This was a test.
	//Oddly, it seemed to work on Xache, surrounding the user in pitch-black fog in a very short distance.
	//In our pre-steampipe build (not the original AZ?), it seems to do nothing at all.
	/*
	float rgb[3] = {1, 1, 1};
	gEngfuncs.pTriAPI->Fog(rgb, 0, 99, 1);
	*/

	//MODDDMIRROR - addition
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
          //G-Cont. you may choose any check - any nice works ;)
	//if ((gHUD.numMirrors>0 && !(m_pCurrentEntity->model->name[7]=='v' && m_pCurrentEntity->model->name[8]=='_')))
	
	int canReflect = TRUE;
	if(EASY_CVAR_GET(mirrorsReflectOnlyNPCs) == 1 && !(m_pCurrentEntity->curstate.renderfx & ISNPC) && !(g_drawType == DRAWTYPE_PLAYER)  ){
		// if this CVar is on and this entity is not an npc, skip drawing to mirror.
		// Player models not included (still reflected).  Players may or may not use the ISNPC flag, it gives no new information there.
		canReflect = FALSE;
	}
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG10. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}
	
	if(g_drawType == DRAWTYPE_PLAYER){
		//canReflect = FALSE;
	}

	// that ok?
	//if ((canReflect && gHUD.numMirrors>0 && (gEngfuncs.GetViewModel() != m_pCurrentEntity)))
	if ((canReflect && gHUD.numMirrors>0 && (g_drawType != DRAWTYPE_VIEWMODEL)))
	{
		for (int ic = 0; ic < gHUD.numMirrors; ic++)
		{
			//Parsing mirror
		    if (!gHUD.Mirrors[ic].enabled)
			{
				continue;
			}

			/*
			m_pRenderModel = m_pCurrentEntity->model;
			m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (m_pRenderModel);
			IEngineStudio.StudioSetHeader( m_pStudioHeader );
			IEngineStudio.SetRenderModel( m_pRenderModel );
			*/

			vec3_t delta;
			float dist;
			VectorSubtract_f(gHUD.Mirrors[ic].origin,m_pCurrentEntity->origin,delta);
			dist = Length(delta);

			if (gHUD.Mirrors[ic].radius < dist)
			{
				continue;
			}
			
			StudioSetUpTransform( 1024 + ic  ); //HELP ME
			//THIS USED TO GO BEFORE!!!

			mirror_id = ic;
			
			StudioDrawReflection(flags);
		}

	}//END OF mirror check.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
	if( !(flags & 128 ) ){
		StudioDrawModel(flags | 128);
	}
	*/
	
	if( g_drawType == DRAWTYPE_VIEWMODEL){
		easyPrintLineDummy("FLAG11. s:%d b:%d", m_pCurrentEntity->curstate.sequence, m_pCurrentEntity->curstate.iuser1 );
	}

	return 1;
}

/*
====================
StudioEstimateGait

====================
*/
void CStudioModelRenderer::StudioEstimateGait( entity_state_t *pplayer )
{
	float dt;
	vec3_t est_velocity;

	dt = (m_clTime - m_clOldTime);
	if (dt < 0)
		dt = 0;
	else if (dt > 1.0)
		dt = 1;

	if (dt == 0 || m_pPlayerInfo->renderframe == m_nFrameCount)
	{
		m_flGaitMovement = 0;
		return;
	}

	// VectorAdd_f( pplayer->velocity, pplayer->prediction_error, est_velocity );
	if ( m_fGaitEstimation )
	{
		VectorSubtract_f( m_pCurrentEntity->origin, m_pPlayerInfo->prevgaitorigin, est_velocity );
		VectorCopy_f( m_pCurrentEntity->origin, m_pPlayerInfo->prevgaitorigin );
		m_flGaitMovement = Length( est_velocity );
		if (dt <= 0 || m_flGaitMovement / dt < 5)
		{
			m_flGaitMovement = 0;
			est_velocity[0] = 0;
			est_velocity[1] = 0;
		}
	}
	else
	{
		VectorCopy_f( pplayer->velocity, est_velocity );
		m_flGaitMovement = Length( est_velocity ) * dt;
	}

	if (est_velocity[1] == 0 && est_velocity[0] == 0)
	{
		float flYawDiff = m_pCurrentEntity->angles[YAW] - m_pPlayerInfo->gaityaw;
		flYawDiff = flYawDiff - (int)(flYawDiff / 360) * 360;
		if (flYawDiff > 180)
			flYawDiff -= 360;
		if (flYawDiff < -180)
			flYawDiff += 360;

		if (dt < 0.25)
			flYawDiff *= dt * 4;
		else
			flYawDiff *= dt;

		m_pPlayerInfo->gaityaw += flYawDiff;
		m_pPlayerInfo->gaityaw = m_pPlayerInfo->gaityaw - (int)(m_pPlayerInfo->gaityaw / 360) * 360;

		m_flGaitMovement = 0;
	}
	else
	{
		m_pPlayerInfo->gaityaw = (atan2(est_velocity[1], est_velocity[0]) * 180.0f / M_PI);
		if (m_pPlayerInfo->gaityaw > 180)
			m_pPlayerInfo->gaityaw = 180;
		if (m_pPlayerInfo->gaityaw < -180)
			m_pPlayerInfo->gaityaw = -180;
	}

}

/*
====================
StudioProcessGait

====================
*/
void CStudioModelRenderer::StudioProcessGait( entity_state_t *pplayer )
{
	mstudioseqdesc_t	*pseqdesc;
	float dt;
	int iBlend;
	float flYaw;	 // view direction relative to movement

	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq) 
	{
		//easyPrintLineDummy("!! Sequence too high! 3");
		
		if( g_drawType == DRAWTYPE_VIEWMODEL){
		
			if(m_pCurrentEntity->curstate.sequence & 128){
				m_pCurrentEntity->curstate.sequence &= ~128;  //that is all? remove this bit(ch).
			}
			if(m_pCurrentEntity->curstate.sequence & 64){
				m_pCurrentEntity->curstate.sequence &= ~64;
			}
		}

	}

	//re-check.
	if (m_pCurrentEntity->curstate.sequence >=  m_pStudioHeader->numseq) {
		easyPrintLineDummy("hockey 2");
		m_pCurrentEntity->curstate.sequence = 0;
	}



	pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + m_pCurrentEntity->curstate.sequence;

	StudioPlayerBlend( pseqdesc, &iBlend, &m_pCurrentEntity->angles[PITCH] );

	m_pCurrentEntity->latched.prevangles[PITCH] = m_pCurrentEntity->angles[PITCH];
	m_pCurrentEntity->curstate.blending[0] = iBlend;
	m_pCurrentEntity->latched.prevblending[0] = m_pCurrentEntity->curstate.blending[0];
	m_pCurrentEntity->latched.prevseqblending[0] = m_pCurrentEntity->curstate.blending[0];

	// Con_DPrintf("%f %d\n", m_pCurrentEntity->angles[PITCH], m_pCurrentEntity->blending[0] );

	dt = (m_clTime - m_clOldTime);
	if (dt < 0)
		dt = 0;
	else if (dt > 1.0)
		dt = 1;

	StudioEstimateGait( pplayer );

	// Con_DPrintf("%f %f\n", m_pCurrentEntity->angles[YAW], m_pPlayerInfo->gaityaw );

	// calc side to side turning
	flYaw = m_pCurrentEntity->angles[YAW] - m_pPlayerInfo->gaityaw;
	flYaw = flYaw - (int)(flYaw / 360) * 360;
	if (flYaw < -180)
		flYaw = flYaw + 360;
	if (flYaw > 180)
		flYaw = flYaw - 360;

	if (flYaw > 120)
	{
		m_pPlayerInfo->gaityaw = m_pPlayerInfo->gaityaw - 180;
		m_flGaitMovement = -m_flGaitMovement;
		flYaw = flYaw - 180;
	}
	else if (flYaw < -120)
	{
		m_pPlayerInfo->gaityaw = m_pPlayerInfo->gaityaw + 180;
		m_flGaitMovement = -m_flGaitMovement;
		flYaw = flYaw + 180;
	}

	// adjust torso
	m_pCurrentEntity->curstate.controller[0] = ((flYaw / 4.0) + 30) / (60.0 / 255.0);
	m_pCurrentEntity->curstate.controller[1] = ((flYaw / 4.0) + 30) / (60.0 / 255.0);
	m_pCurrentEntity->curstate.controller[2] = ((flYaw / 4.0) + 30) / (60.0 / 255.0);
	m_pCurrentEntity->curstate.controller[3] = ((flYaw / 4.0) + 30) / (60.0 / 255.0);
	m_pCurrentEntity->latched.prevcontroller[0] = m_pCurrentEntity->curstate.controller[0];
	m_pCurrentEntity->latched.prevcontroller[1] = m_pCurrentEntity->curstate.controller[1];
	m_pCurrentEntity->latched.prevcontroller[2] = m_pCurrentEntity->curstate.controller[2];
	m_pCurrentEntity->latched.prevcontroller[3] = m_pCurrentEntity->curstate.controller[3];

	m_pCurrentEntity->angles[YAW] = m_pPlayerInfo->gaityaw;
	if (m_pCurrentEntity->angles[YAW] < -0)
		m_pCurrentEntity->angles[YAW] += 360;
	m_pCurrentEntity->latched.prevangles[YAW] = m_pCurrentEntity->angles[YAW];

	if (pplayer->gaitsequence >= m_pStudioHeader->numseq) 
	{
		pplayer->gaitsequence = 0;
	}

	pseqdesc = (mstudioseqdesc_t *)((byte *)m_pStudioHeader + m_pStudioHeader->seqindex) + pplayer->gaitsequence;

	// calc gait frame
	if (pseqdesc->linearmovement[0] > 0)
	{
		m_pPlayerInfo->gaitframe += (m_flGaitMovement / pseqdesc->linearmovement[0]) * pseqdesc->numframes;
	}
	else
	{
		m_pPlayerInfo->gaitframe += pseqdesc->fps * dt;
	}

	// do modulo
	m_pPlayerInfo->gaitframe = m_pPlayerInfo->gaitframe - (int)(m_pPlayerInfo->gaitframe / pseqdesc->numframes) * pseqdesc->numframes;
	if (m_pPlayerInfo->gaitframe < 0)
		m_pPlayerInfo->gaitframe += pseqdesc->numframes;
}

/*
====================
StudioDrawPlayer

====================
*/
int CStudioModelRenderer::StudioDrawPlayer( int flags, entity_state_t *pplayer )
{
#if STUDIO_NO_RENDERING == 1
	return 0;
#endif

	alight_t lighting;
	vec3_t dir;

	//MODDDMIRROR - this if-then surrounds the usual line.
	if (!(flags & 2048))
	{
 		m_pCurrentEntity = IEngineStudio.GetCurrentEntity();
	}

	//MODDD - clearly.
	// Any manual calls to StudioDrawPlayer (from this file instead of the engine) need to set g_drawType
	// back to the previous value before this call, if it was found yet for some other entity and significant.
	g_drawType = DRAWTYPE_PLAYER;

	IEngineStudio.GetTimes( &m_nFrameCount, &m_clTime, &m_clOldTime );
	IEngineStudio.GetViewInfo( m_vRenderOrigin, m_vUp, m_vRight, m_vNormal );
	IEngineStudio.GetAliasScale( &m_fSoftwareXScale, &m_fSoftwareYScale );

	// Con_DPrintf("DrawPlayer %d\n", m_pCurrentEntity->blending[0] );

	// Con_DPrintf("DrawPlayer %d %d (%d)\n", m_nFrameCount, pplayer->player_index, m_pCurrentEntity->curstate.sequence );

	// Con_DPrintf("Player %.2f %.2f %.2f\n", pplayer->velocity[0], pplayer->velocity[1], pplayer->velocity[2] );

	m_nPlayerIndex = pplayer->number - 1;

	if (m_nPlayerIndex < 0 || m_nPlayerIndex >= gEngfuncs.GetMaxClients())
		return 0;

	m_pRenderModel = IEngineStudio.SetupPlayerModel( m_nPlayerIndex );
	if (m_pRenderModel == NULL)
		return 0;

	m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (m_pRenderModel);
	IEngineStudio.StudioSetHeader( m_pStudioHeader );
	IEngineStudio.SetRenderModel( m_pRenderModel );


	//MODDDMIRROR
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	//MODDD
	int canReflect = TRUE;
	//(CVAR_GET_FLOAT("mirrorsDoNotReflectPlayer") == 1) &&
	//if(  (m_pCurrentEntity->curstate.renderfx & NOREFLECT) ){

	//if(g_drawType == DRAWTYPE_PLAYER)easyPrintLineDummy("MY friendAA %.2f %.2f", global2PSEUDO_IGNOREcameraMode, EASY_CVAR_GET(mirrorsDoNotReflectPlayer));

	//in third person and we're told not to reflect the player? Don't do that then.
	if( (g_drawType == DRAWTYPE_PLAYER) && global2PSEUDO_IGNOREcameraMode == 1 && EASY_CVAR_GET(mirrorsDoNotReflectPlayer) == 1) {
		//easyPrintLine("STATUS: %d %d", (int)CVAR_GET_FLOAT("IGNOREcameraMode"), (int)CVAR_GET_FLOAT("mirrorsDoNotReflectPlayer"));
		canReflect = FALSE;
	}

	
	
	if (!(flags & 2048)){ //MODDDMIRROR
		
		////MODDDMIRROR
		/////////////////////////////////////////////////////////////////////////////////////////
		m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (m_pRenderModel);
		IEngineStudio.StudioSetHeader( m_pStudioHeader );
		IEngineStudio.SetRenderModel( m_pRenderModel );
		//////////////////////////////////////////////////////////////////////////////////////////

	if (pplayer->gaitsequence)
	{
		vec3_t orig_angles;
		m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );

		VectorCopy_f( m_pCurrentEntity->angles, orig_angles );
	
		StudioProcessGait( pplayer );

		m_pPlayerInfo->gaitsequence = pplayer->gaitsequence;
		m_pPlayerInfo = NULL;

		StudioSetUpTransform( 0 );
		VectorCopy_f( orig_angles, m_pCurrentEntity->angles );
	}
	else
	{
		m_pCurrentEntity->curstate.controller[0] = 127;
		m_pCurrentEntity->curstate.controller[1] = 127;
		m_pCurrentEntity->curstate.controller[2] = 127;
		m_pCurrentEntity->curstate.controller[3] = 127;
		m_pCurrentEntity->latched.prevcontroller[0] = m_pCurrentEntity->curstate.controller[0];
		m_pCurrentEntity->latched.prevcontroller[1] = m_pCurrentEntity->curstate.controller[1];
		m_pCurrentEntity->latched.prevcontroller[2] = m_pCurrentEntity->curstate.controller[2];
		m_pCurrentEntity->latched.prevcontroller[3] = m_pCurrentEntity->curstate.controller[3];
		
		m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );
		m_pPlayerInfo->gaitsequence = 0;

		StudioSetUpTransform( 0 );
	}

	if (flags & STUDIO_RENDER)
	{
		// see if the bounding box lets us trivially reject, also sets
		if (!IEngineStudio.StudioCheckBBox ())
			return 0;

		(*m_pModelsDrawn)++;
		(*m_pStudioModelCount)++; // render data cache cookie

		if (m_pStudioHeader->numbodyparts == 0)
			return 1;
	}

	m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );
	StudioSetupBones( );
	StudioSaveBones( );
	m_pPlayerInfo->renderframe = m_nFrameCount;

	m_pPlayerInfo = NULL;

	//MODDD - ATTACHMENT - puts effects on the third person model (not the reflection, the original).
	if (flags & STUDIO_EVENTS)
	{
		StudioCalcAttachments( );
		IEngineStudio.StudioClientEvents( );
		// copy attachments into global entity array
		if ( m_pCurrentEntity->index > 0 )
		{
			cl_entity_t *ent = gEngfuncs.GetEntityByIndex( m_pCurrentEntity->index );

			memcpy( ent->attachment, m_pCurrentEntity->attachment, sizeof( vec3_t ) * 4 );
		}
	}

	if (flags & STUDIO_RENDER)
	{
		if (m_pCvarHiModels->value && m_pRenderModel != m_pCurrentEntity->model  )
		{
			// show highest resolution multiplayer model
			m_pCurrentEntity->curstate.body = 255;
		}

		if (!(m_pCvarDeveloper->value == 0 && !IsMultiplayer() ) && ( m_pRenderModel == m_pCurrentEntity->model ) )
		{
			m_pCurrentEntity->curstate.body = 1; // force helmet
		}

		lighting.plightvec = dir;
		IEngineStudio.StudioDynamicLight(m_pCurrentEntity, &lighting );

		IEngineStudio.StudioEntityLight( &lighting );

		// model and frame independant
		IEngineStudio.StudioSetupLighting (&lighting);
		
		//MODDD - don't mind me, just screwing around.		
		/*
				lighting.plightvec = Vector(45, 45, 45);
				//lighting.shadelight = 24;   //no idea what this is.  Or "ambientLight".
		*/


				
		m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );

		// get remap colors
		m_nTopColor = m_pPlayerInfo->topcolor;
		m_nBottomColor = m_pPlayerInfo->bottomcolor;
		if (m_nTopColor < 0)
			m_nTopColor = 0;
		if (m_nTopColor > 360)
			m_nTopColor = 360;
		if (m_nBottomColor < 0)
			m_nBottomColor = 0;
		if (m_nBottomColor > 360)
			m_nBottomColor = 360;

		IEngineStudio.StudioSetRemapColors( m_nTopColor, m_nBottomColor );

		StudioRenderModel( );
		m_pPlayerInfo = NULL;

		if (pplayer->weaponmodel)
		{
			cl_entity_t saveent = *m_pCurrentEntity;

			model_t *pweaponmodel = IEngineStudio.GetModelByIndex( pplayer->weaponmodel );

			m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (pweaponmodel);
			IEngineStudio.StudioSetHeader( m_pStudioHeader );

			StudioMergeBones( pweaponmodel);

			IEngineStudio.StudioSetupLighting (&lighting);

			StudioRenderModel( );

			StudioCalcAttachments( );

			*m_pCurrentEntity = saveent;
		}
	}
	
	}//MODDDMIRROR



	if (canReflect && gHUD.numMirrors>0)
	{
		//StudioSetUpTransform( 0 );//G-cont. transform must be first!
		//MODDD - transform this way to avoid the culling issue (done in StudioDrawModel too)
		
		for (int ic = 0; ic < gHUD.numMirrors; ic++)
		{
			m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (m_pRenderModel);
			IEngineStudio.StudioSetHeader( m_pStudioHeader );
			IEngineStudio.SetRenderModel( m_pRenderModel );

			if (!gHUD.Mirrors[ic].enabled)
			{
				continue;
			}

			//Parsing mirror
			StudioSetUpTransform( 1024 + ic );

			vec3_t delta;
			float dist;
			VectorSubtract_f(gHUD.Mirrors[ic].origin,m_pCurrentEntity->origin,delta);
			dist = Length(delta);

			if (gHUD.Mirrors[ic].radius < dist)
			{
				continue;
			}

			mirror_id = ic;

			gEngfuncs.pTriAPI->CullFace( TRI_NONE ); 
 
			if (pplayer->gaitsequence)
			{
	         	vec3_t orig_angles;
				m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );

				VectorCopy_f( m_pCurrentEntity->angles, orig_angles );
				StudioProcessGait( pplayer );

				m_pPlayerInfo->gaitsequence = pplayer->gaitsequence;
				m_pPlayerInfo = NULL;

				//StudioSetUpTransform( 0 );
	          	VectorCopy_f( orig_angles, m_pCurrentEntity->angles );
			}
         	else //player in jump (or duck)
			{
				m_pCurrentEntity->curstate.controller[0] = 127;
				m_pCurrentEntity->curstate.controller[1] = 127;
				m_pCurrentEntity->curstate.controller[2] = 127;
				m_pCurrentEntity->curstate.controller[3] = 127;
				m_pCurrentEntity->latched.prevcontroller[0] = m_pCurrentEntity->curstate.controller[0];
				m_pCurrentEntity->latched.prevcontroller[1] = m_pCurrentEntity->curstate.controller[1];
				m_pCurrentEntity->latched.prevcontroller[2] = m_pCurrentEntity->curstate.controller[2];
				m_pCurrentEntity->latched.prevcontroller[3] = m_pCurrentEntity->curstate.controller[3];
		
				m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );
				m_pPlayerInfo->gaitsequence = 0;

				//StudioSetUpTransform( 0 );
			}
          	if (flags & STUDIO_RENDER)
			{
				// see if the bounding box lets us trivially reject, also sets
				if (!IEngineStudio.StudioCheckBBox ()){
					//return 0;
					//MODDD - just try the next mirror.
					continue;
				}

				(*m_pModelsDrawn)++;
				(*m_pStudioModelCount)++; // render data cache cookie

				if (m_pStudioHeader->numbodyparts == 0){
					//return 1;
					//MODDD - just try the next mirror.
					continue;
				}
			}

			m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );
			StudioSetupBones( );
			StudioSaveBones( );
			m_pPlayerInfo->renderframe = m_nFrameCount;

			m_pPlayerInfo = NULL;

			//MODDD - ATTACHMENT - sets the attachment start to the reflection's weapon point, for BOTH first-person & third-person.
			//HACKY SOLUTION: disable updating the studio attachments here, or else we override the player's usual.
			/*
			if (flags & STUDIO_EVENTS)
			{
				StudioCalcAttachments( );
				IEngineStudio.StudioClientEvents( );
				// copy attachments into global entity array
				if ( m_pCurrentEntity->index > 0 )
				{
					cl_entity_t *ent = gEngfuncs.GetEntityByIndex( m_pCurrentEntity->index );
					memcpy( ent->attachment, m_pCurrentEntity->attachment, sizeof( vec3_t ) * 4 );
				}
			}
			*/

			if (flags & STUDIO_RENDER)
			{
				if (m_pCvarHiModels->value && m_pRenderModel != m_pCurrentEntity->model  )
				{
					// show highest resolution multiplayer model
					m_pCurrentEntity->curstate.body = 255;
				}

				if (!(m_pCvarDeveloper->value == 0 && !IsMultiplayer() ) && ( m_pRenderModel == m_pCurrentEntity->model ) )
				{
					m_pCurrentEntity->curstate.body = 1; // force helmet
				}

	         	lighting.plightvec = dir;
				IEngineStudio.StudioDynamicLight(m_pCurrentEntity, &lighting );

				IEngineStudio.StudioEntityLight( &lighting );

				// model and frame independant
				IEngineStudio.StudioSetupLighting (&lighting);

				m_pPlayerInfo = IEngineStudio.PlayerInfo( m_nPlayerIndex );

				// get remap colors
				m_nTopColor = m_pPlayerInfo->topcolor;
				m_nBottomColor = m_pPlayerInfo->bottomcolor;
				if (m_nTopColor < 0)
					m_nTopColor = 0;
				if (m_nTopColor > 360)
					m_nTopColor = 360;
				if (m_nBottomColor < 0)
		         			m_nBottomColor = 0;
				if (m_nBottomColor > 360)
					m_nBottomColor = 360;

				IEngineStudio.StudioSetRemapColors( m_nTopColor, m_nBottomColor );

				StudioRenderModel( );
				m_pPlayerInfo = NULL;
				
				if (pplayer->weaponmodel)
				{
					cl_entity_t saveent = *m_pCurrentEntity;
					model_t *pweaponmodel = IEngineStudio.GetModelByIndex( pplayer->weaponmodel );

					m_pStudioHeader = (studiohdr_t *)IEngineStudio.Mod_Extradata (pweaponmodel);
					IEngineStudio.StudioSetHeader( m_pStudioHeader );

					StudioMergeBones( pweaponmodel);
          				IEngineStudio.StudioSetupLighting (&lighting);

					StudioRenderModel( );
					StudioCalcAttachments( );
					*m_pCurrentEntity = saveent;
				}
			}
		} //end for

		// MODDD - was still TRI_FRONT???
		//gEngfuncs.pTriAPI->CullFace( TRI_FRONT );
		gEngfuncs.pTriAPI->CullFace(TRI_NONE);
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 1;
}

/*
====================
StudioCalcAttachments

====================
*/
void CStudioModelRenderer::StudioCalcAttachments( void )
{
	//!!!
	//return;

	int i;
	mstudioattachment_t *pattachment;

	if ( m_pStudioHeader->numattachments > 4 )
	{
		gEngfuncs.Con_DPrintf( "Too many attachments on %s\n", m_pCurrentEntity->model->name );
		exit( -1 );
	}

	// calculate attachment points
	pattachment = (mstudioattachment_t *)((byte *)m_pStudioHeader + m_pStudioHeader->attachmentindex);
	for (i = 0; i < m_pStudioHeader->numattachments; i++)
	{
		//checkpoint6
		//MODDD - PAY ATTENTION TO ME!!!
		//VectorTransform( pattachment[i].org, (*m_plighttransform)[pattachment[i].bone],  m_pCurrentEntity->attachment[i] );
		VectorTransform( pattachment[i].org, (m_plighttransformMOD)[pattachment[i].bone],  m_pCurrentEntity->attachment[i] );
	}
}



/*
====================
StudioRenderModel

====================
*/

//MODDD NOTE IMPORTANT - it seems this, StudioRenderModel, is the public method to be called by anything that wishes to currently render something out.
//                       That is, other similarly named methods, such as StudioRenderFinal, and the _Hardware and _Software variants, are meant to be 
//                       called HERE insetad, and nowhere else.  They are too specific and unhelpful for anywhere else to call directly.

void CStudioModelRenderer::StudioRenderModel( void )
{
	//MODDD - It is possible that renderfx choices are still meant to be only single values from 0 to 31 like in the as-is game.
	//        If the engine expects them this way, and only this way, we need to make sure they're sent over in that state.
	//        Any numbers outside of that range are just our own powers of 2 to make combinations out of one or several.
	//        But if they cause the engine to misread renderfx, the retail behavior from the engine won't work as expected.

	//Saving any secondary bits before the cutoff.
	int secondaryBitsMem = (m_pCurrentEntity->curstate.renderfx & RENDERFX_SECONDARY_BITS );
	//Cutoff. Only keep primary bits, solid values from 0 to 31 inclusive.
	m_pCurrentEntity->curstate.renderfx = m_pCurrentEntity->curstate.renderfx & RENDERFX_PRIMARY_BITS;


	IEngineStudio.SetChromeOrigin();
	IEngineStudio.SetForceFaceFlags( 0 );


	//MODDD - only count the primary bits (what was available in retail, or "as-is").
	//        ...already removed the secondary bits from renderfx during these calls up until the end of this method. Proceed as usual.
	//if ( (m_pCurrentEntity->curstate.renderfx & RENDERFX_PRIMARY_BITS) == kRenderFxGlowShell )
	if ( m_pCurrentEntity->curstate.renderfx == kRenderFxGlowShell )
	{
		//MODDD - hold on! Remember what the cutoff bits were to re-apply them.
		//        A plain equals assignment would forget all of those.
		//        ...already done, nevermind.
		//int secondaryBitsMem = (m_pCurrentEntity->curstate.renderfx & RENDERFX_SECONDARY_BITS );
		m_pCurrentEntity->curstate.renderfx = kRenderFxNone;
		
		StudioRenderFinal( );
		
		//MODDD - here and below, force allow the RenderMode calls if "renderDebug" is 1 or 2.
		if ( (EASY_CVAR_GET(r_glowshell_debug) == 1 || EASY_CVAR_GET(r_glowshell_debug) == 2) || !IEngineStudio.IsHardware() )
		{
			gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
		}
		
		IEngineStudio.SetForceFaceFlags( STUDIO_NF_CHROME );
		
		gEngfuncs.pTriAPI->SpriteTexture( m_pChromeSprite, 0 );

		//MODDD - and restore the secondary bits, if any.
		//        ...also, secondary bits will come back after all other calls in this method. So not yet.
		m_pCurrentEntity->curstate.renderfx = kRenderFxGlowShell;
		//m_pCurrentEntity->curstate.renderfx = kRenderFxGlowShell | secondaryBitsMem;

		StudioRenderFinal( );
		
		if ( (EASY_CVAR_GET(r_glowshell_debug) == 1 || EASY_CVAR_GET(r_glowshell_debug) == 2) || !IEngineStudio.IsHardware() )
		{
			gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
		}

		//MODDD - new. probably doesn't matter, but safety.
		IEngineStudio.SetForceFaceFlags( 0 );
	}
	else
	{
		//IEngineStudio.SetForceFaceFlags( STUDIO_NF_CHROME );
		StudioRenderFinal( );
	}

	//MODDD - now restore the secondary bits.
	m_pCurrentEntity->curstate.renderfx = m_pCurrentEntity->curstate.renderfx | secondaryBitsMem;
}

/*
====================
StudioRenderFinal_Software

====================
*/
void CStudioModelRenderer::StudioRenderFinal_Software( void )
{
	int i;

	// Note, rendermode set here has effect in SW
	IEngineStudio.SetupRenderer( 0 ); 

	if (m_pCvarDrawEntities->value == 2)
	{
		IEngineStudio.StudioDrawBones( );
	}
	else if (m_pCvarDrawEntities->value == 3)
	{
		IEngineStudio.StudioDrawHulls( );
	}
	else
	{
		for (i=0 ; i < m_pStudioHeader->numbodyparts ; i++)
		{
			IEngineStudio.StudioSetupModel( i, (void **)&m_pBodyPart, (void **)&m_pSubModel );
			IEngineStudio.StudioDrawPoints( );
		}
	}

	if (m_pCvarDrawEntities->value == 4)
	{
		gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
		IEngineStudio.StudioDrawHulls( );
		gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
	}

	if (m_pCvarDrawEntities->value == 5)
	{
		IEngineStudio.StudioDrawAbsBBox( );
	}
	
	IEngineStudio.RestoreRenderer();
}

/*
====================
StudioRenderFinal_Hardware

====================
*/
void CStudioModelRenderer::StudioRenderFinal_Hardware( void )
{
	int i;
	int rendermode;
	
	//easyPrintLine("EFFECTS::: %d, %d", m_pCurrentEntity->curstate.effects, m_pCurrentEntity->curstate.renderfx);
	rendermode = IEngineStudio.GetForceFaceFlags() ? kRenderTransAdd : m_pCurrentEntity->curstate.rendermode;


	IEngineStudio.SetupRenderer( rendermode );
	
	if (m_pCvarDrawEntities->value == 2)
	{
		IEngineStudio.StudioDrawBones();
	}
	else if (m_pCvarDrawEntities->value == 3)
	{
		IEngineStudio.StudioDrawHulls();
	}
	else
	{
		for (i=0 ; i < m_pStudioHeader->numbodyparts ; i++)
		{
			IEngineStudio.StudioSetupModel( i, (void **)&m_pBodyPart, (void **)&m_pSubModel );
			
			if (m_fDoInterp)
			{
				// interpolation messes up bounding boxes.
				m_pCurrentEntity->trivial_accept = 0; 
			}


			IEngineStudio.GL_SetRenderMode( rendermode );
			IEngineStudio.StudioDrawPoints();
			
			//MODDD - VERY FREAKIN' IMPORTANT.  Below has... side effects if not careful.  With the glow effect being on.  Who knows.
			//But DONT Skimp out on calling IEngineStudio.GL_StudioDrawShadow regardless (which seems to be completely unrelated to the Old shadows).
			//If this is not called, the glow render effect (agrunts powered up by a kingpin) will cause all kinds of other bad order rendering issues.
			//No idea whaaaaat wires are crossed simply by not calling this.  But whatever.
			//Now, a CVar, renderDebug, controls what choice is made.  Either run GL_StudioDrawShadow here, or skip StudioRenderModel's !IsHardware checks
			//(force a pass).  Or do both, or neither (you madman).  See the docs.
			if(EASY_CVAR_GET(r_glowshell_debug) == 0 || EASY_CVAR_GET(r_glowshell_debug) == 2){
				IEngineStudio.GL_StudioDrawShadow();
			}
			
			//////////////////////////////////////////////////////////////////////////////////////////////////
			//MODDD - why "+ 32"?  Who comes up with this?  The world may never know.
			GL_StudioDrawShadow_Old = (void(*)(void))(((unsigned int)IEngineStudio.GL_StudioDrawShadow) + 32);

			//MODDD TODO - we also have a viewmodel check too, but regardless.
			
			// that ok?
			//if (EASY_CVAR_GET(r_shadows) == 1 && gEngfuncs.GetViewModel() != m_pCurrentEntity) // FIX : Avoid view model
			if (EASY_CVAR_GET(r_shadows) == 1 && g_drawType != DRAWTYPE_VIEWMODEL) // FIX : Avoid view model
			{
				//DropShadows();
				
				if( !( (m_pCurrentEntity->curstate.renderfx & DONOTDRAWSHADOW) == DONOTDRAWSHADOW) ){
					DropShadows();
				}
			}
			//////////////////////////////////////////////////////////////////////////////////////////////////
			
		}
	}

	if ( m_pCvarDrawEntities->value == 4 )
	{
		gEngfuncs.pTriAPI->RenderMode( kRenderTransAdd );
		IEngineStudio.StudioDrawHulls( );
		gEngfuncs.pTriAPI->RenderMode( kRenderNormal );
	}

	IEngineStudio.RestoreRenderer();
}

/*
====================
StudioRenderFinal

====================
*/
void CStudioModelRenderer::StudioRenderFinal(void)
{
	if ( IEngineStudio.IsHardware() )
	{
		StudioRenderFinal_Hardware();
	}
	else
	{
		StudioRenderFinal_Software();
	}
}

