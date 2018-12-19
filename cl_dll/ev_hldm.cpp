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
#include "hud.h"
#include "cl_util.h"
#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "entity_types.h"
#include "usercmd.h"
#include "pm_defs.h"
#include "pm_materials.h"

#include "eventscripts.h"
#include "ev_hldm.h"

#include "r_efx.h"
#include "event_api.h"
#include "event_args.h"
#include "in_defs.h"

#include <string.h>

#include "r_studioint.h"
#include "com_model.h"

//MODDD - new include.  THESE WERE NEVER ON, deemed unnecessary.
//#include "hud_iface.h"
//#include "extdll.h"
//#include "util.h"  <-- includes  """ #include "enginecallback.h" """


//Why does this include mess so much stuff up? Whatever, I just need a PI constant.
#define M_PI		3.14159265358979323846
//#include "mathlib.h"


//ALSO UNNECESSARY, actually. just use "gEngfuncs".
//#include "enginecallback.h"
//should be acceptable.



extern engine_studio_api_t IEngineStudio;

static int tracerCount[ 32 ];

extern "C" char PM_FindTextureType( char *name );

void V_PunchAxis( int axis, float punch );
void VectorAngles( const float *forward, float *angles );

extern cvar_t *cl_lw;

extern "C"
{

// HLDM
void EV_FireGlock1( struct event_args_s *args  );
void EV_FireGlock2( struct event_args_s *args  );
void EV_FireShotGunSingle( struct event_args_s *args  );
void EV_FireShotGunDouble( struct event_args_s *args  );
void EV_FireMP5( struct event_args_s *args  );
void EV_FireMP52( struct event_args_s *args  );
void EV_FirePython( struct event_args_s *args  );
void EV_FireGauss( struct event_args_s *args  );
void EV_SpinGauss( struct event_args_s *args  );
void EV_Crowbar( struct event_args_s *args  );
void EV_FireCrossbow( struct event_args_s *args  );
void EV_FireCrossbow2( struct event_args_s *args  );
void EV_FireRpg( struct event_args_s *args  );
void EV_EgonFire( struct event_args_s *args  );
void EV_EgonStop( struct event_args_s *args  );
void EV_HornetGunFire( struct event_args_s *args  );
void EV_TripmineFire( struct event_args_s *args  );
void EV_SnarkFire( struct event_args_s *args  );

//MODDDMIRROR
void EV_Mirror( struct event_args_s *args ); 

void EV_TrainPitchAdjust( struct event_args_s *args );

//MODDD
void EV_Trail_EngineChoice(event_args_t* args);
void EV_imitation7(event_args_t* args);
void EV_Trail( event_args_t *args  );
void EV_rocketAlphaTrail( event_args_t *args  );

void EV_ShowBalls( event_args_t* args);
void EV_ShowBallsPowerup( event_args_t* args);
void EV_QuakeExplosionEffect( event_args_t* args);
void EV_HLDM_DecalGunshotCustomEvent( event_args_t* args);

void EV_FreakyLight( event_args_t* args);
void EV_FriendlyVomit( event_args_t* args);


}

#define VECTOR_CONE_1DEGREES Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES Vector( 0.06105, 0.06105, 0.06105 )	
#define VECTOR_CONE_8DEGREES Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES Vector( 0.17365, 0.17365, 0.17365 )




//MODDD - extern the client CVars here.
//EASY_CVAR_EXTERN_CLIENT_MASS




/*

EASY_CVAR_DECLARATION_CLIENT_MASS


//similar to how serverside's "updateCVarRefs" of combat.cpp, but this is just for the rave lights.
void updateCVarRefsClient(){

	EASY_CVAR_UPDATE_CLIENT_MASS

}
*/



extern float global2_strobeDurationMin;
extern float global2_strobeDurationMax;
extern float global2_strobeRadiusMin;
extern float global2_strobeRadiusMax;
extern float global2_strobeSpawnDistHori;
extern float global2_strobeSpawnDistVertMin;
extern float global2_strobeSpawnDistVertMax;
	
extern float global2_strobeMultiColor;

extern float global2_ravelaserlength;

	

	
extern float global2_raveLaserSpawnDistHoriMin;
extern float global2_raveLaserSpawnDistHoriMax;
extern float global2_raveLaserSpawnDistHoriMin;
extern float global2_raveLaserSpawnDistHoriMax;
extern float global2_raveLaserSpawnDistVertMin;
extern float global2_raveLaserSpawnDistVertMax;
extern float global2_raveLaserSpawnDistVertMin;
extern float global2_raveLaserSpawnDistVertMax;


extern float global2_raveLaserDurationMin;
extern float global2_raveLaserDurationMax;
extern float global2_raveLaserThicknessMin;
extern float global2_raveLaserThicknessMax;

extern float global2_raveLaserNoiseMin;
extern float global2_raveLaserNoiseMax;


extern float global2_raveLaserBrightnessMin;
extern float global2_raveLaserBrightnessMax;
	
extern float global2_raveLaserFrameRateMin;
extern float global2_raveLaserFrameRateMax;
	
	
extern float global2_raveLaserMultiColor;


extern float global2_raveLaserEnabled;
extern float global2_raveLaserLength;

extern float global2_raveLaserSpawnFreq;

extern float global2_muteRicochetSound;

extern float global2_muteBulletHitSounds;

extern float global2_rocketTrailAlphaInterval;
extern float global2_rocketTrailAlphaScale;


EASY_CVAR_EXTERN(gauss_mode)
EASY_CVAR_EXTERN(gauss_primaryonly)
EASY_CVAR_EXTERN(gauss_reflectdealsdamage)
EASY_CVAR_EXTERN(gauss_chargeanimdelay)
EASY_CVAR_EXTERN(gauss_chargeworkdelay)
EASY_CVAR_EXTERN(gauss_secondarychargetimereq)
EASY_CVAR_EXTERN(gauss_primaryreflects)
EASY_CVAR_EXTERN(gauss_primarypierces)
EASY_CVAR_EXTERN(gauss_secondaryreflects)
EASY_CVAR_EXTERN(gauss_secondarypierces)
EASY_CVAR_EXTERN(gauss_primarypunchthrough)
EASY_CVAR_EXTERN(gauss_secondarypunchthrough)

EASY_CVAR_EXTERN(playerWeaponForceSpreadMode)
EASY_CVAR_EXTERN(playerBulletHitEffectForceServer)

EASY_CVAR_EXTERN(mutePlayerWeaponFire)
EASY_CVAR_EXTERN(crossbowFirePlaysReloadSound)

EASY_CVAR_EXTERN(textureHitSoundPrintouts)

EASY_CVAR_EXTERN(playerWeaponTracerMode)

EASY_CVAR_EXTERN(egonEffectsMode)






// play a strike sound based on the texture that was hit by the attack traceline.  VecSrc/VecEnd are the
// original traceline endpoints used by the attacker, iBulletType is the type of bullet that hit the texture.
// returns volume of strike instrument (crowbar) to play
//MODDD - IMPORTANT. This will find a chTextureType of '\0' (null character; numeric 0) if the hit surface is an entity at all.
//        Or simply not part of the map geometry, probably that.
float EV_HLDM_PlayTextureSound( int idx, pmtrace_t *ptr, float *vecSrc, float *vecEnd, int iBulletType )
{
	// hit the world, try to play sound based on texture material type
	char chTextureType = CHAR_TEX_CONCRETE;
	float fvol;
	float fvolbar;
	char *rgsz[4];
	int cnt;
	float fattn = ATTN_NORM;
	int entity;
	char *pTextureName;
	char texname[ 64 ];
	char szbuffer[ 64 ];

	entity = gEngfuncs.pEventAPI->EV_IndexFromTrace( ptr );

	// FIXME check if playtexture sounds movevar is set
	//

	//MODDD - the above was not written by me, appeared as-is.
	if(global2_muteBulletHitSounds == 1){
		return 0;
	}


	chTextureType = 0;

	// Player
	if ( entity >= 1 && entity <= gEngfuncs.GetMaxClients() )
	{
		// hit body
		chTextureType = CHAR_TEX_FLESH;
	}
	else if ( entity == 0 )
	{
		// get texture from entity or world (world is ent(0))
		pTextureName = (char *)gEngfuncs.pEventAPI->EV_TraceTexture( ptr->ent, vecSrc, vecEnd );
		
		if ( pTextureName )
		{
			strcpy( texname, pTextureName );
			pTextureName = texname;

			// strip leading '-0' or '+0~' or '{' or '!'
			if (*pTextureName == '-' || *pTextureName == '+')
			{
				pTextureName += 2;
			}

			if (*pTextureName == '{' || *pTextureName == '!' || *pTextureName == '~' || *pTextureName == ' ')
			{
				pTextureName++;
			}
			
			// '}}'
			strcpy( szbuffer, pTextureName );
			szbuffer[ CBTEXTURENAMEMAX - 1 ] = 0;
				
			// get texture type
			chTextureType = PM_FindTextureType( szbuffer );	
		}
	}
	
	if(EASY_CVAR_GET(textureHitSoundPrintouts)==1)easyPrintLine("EV PLAY TEXTURE SOUND: %d, %c", (int)chTextureType, chTextureType);

	//MODDD - healthy default, just in case nothing was picked?
	cnt = 0;

	switch (chTextureType)
	{

	//...I mean, clever.
	//No, let's not do that.
	//default:
	case CHAR_TEX_CONCRETE: fvol = 0.9;	fvolbar = 0.6;
		rgsz[0] = "player/pl_step1.wav";
		rgsz[1] = "player/pl_step2.wav";
		cnt = 2;
		break;
	case CHAR_TEX_METAL: fvol = 0.9; fvolbar = 0.3;
		rgsz[0] = "player/pl_metal1.wav";
		rgsz[1] = "player/pl_metal2.wav";
		cnt = 2;
		break;
	case CHAR_TEX_DIRT:	fvol = 0.9; fvolbar = 0.1;
		rgsz[0] = "player/pl_dirt1.wav";
		rgsz[1] = "player/pl_dirt2.wav";
		rgsz[2] = "player/pl_dirt3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_VENT:	fvol = 0.5; fvolbar = 0.3;
		rgsz[0] = "player/pl_duct1.wav";
		rgsz[1] = "player/pl_duct1.wav";
		cnt = 2;
		break;
	case CHAR_TEX_GRATE: fvol = 0.9; fvolbar = 0.5;
		rgsz[0] = "player/pl_grate1.wav";
		rgsz[1] = "player/pl_grate4.wav";
		cnt = 2;
		break;
	case CHAR_TEX_TILE:	fvol = 0.8; fvolbar = 0.2;
		rgsz[0] = "player/pl_tile1.wav";
		rgsz[1] = "player/pl_tile3.wav";
		rgsz[2] = "player/pl_tile2.wav";
		rgsz[3] = "player/pl_tile4.wav";
		cnt = 4;
		break;
	case CHAR_TEX_SLOSH: fvol = 0.9; fvolbar = 0.0;
		rgsz[0] = "player/pl_slosh1.wav";
		rgsz[1] = "player/pl_slosh3.wav";
		rgsz[2] = "player/pl_slosh2.wav";
		rgsz[3] = "player/pl_slosh4.wav";
		cnt = 4;
		break;
	case CHAR_TEX_WOOD: fvol = 0.9; fvolbar = 0.2;
		rgsz[0] = "debris/wood1.wav";
		rgsz[1] = "debris/wood2.wav";
		rgsz[2] = "debris/wood3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_GLASS:
	case CHAR_TEX_COMPUTER:
		fvol = 0.8; fvolbar = 0.2;
		rgsz[0] = "debris/glass1.wav";
		rgsz[1] = "debris/glass2.wav";
		rgsz[2] = "debris/glass3.wav";
		cnt = 3;
		break;
	case CHAR_TEX_FLESH:
		
		if (iBulletType == BULLET_PLAYER_CROWBAR)
			return 0.0; // crowbar already makes this sound

		fvol = 1.0;	fvolbar = 0.2;
		rgsz[0] = "weapons/bullet_hit1.wav";
		rgsz[1] = "weapons/bullet_hit2.wav";
		fattn = 1.0;
		cnt = 2;
		break;
	}

	// play material hit sound
	if(cnt > 0){
		gEngfuncs.pEventAPI->EV_PlaySound( 0, ptr->endpos, CHAN_STATIC, rgsz[gEngfuncs.pfnRandomLong(0,cnt-1)], fvol, fattn, 0, 96 + gEngfuncs.pfnRandomLong(0,0xf) );
	}
	return fvolbar;
}

char *EV_HLDM_DamageDecal( physent_t *pe )
{
	static char decalname[ 32 ];
	int idx;

	if ( pe->classnumber == 1 )
	{
		idx = gEngfuncs.pfnRandomLong( 0, 2 );
		sprintf( decalname, "{break%i", idx + 1 );
	}
	else if ( pe->rendermode != kRenderNormal )
	{
		sprintf( decalname, "{bproof1" );
	}
	else
	{
		idx = gEngfuncs.pfnRandomLong( 0, 4 );
		sprintf( decalname, "{shot%i", idx + 1 );
	}
	return decalname;
}

void EV_HLDM_GunshotDecalTrace( pmtrace_t *pTrace, char *decalName )
{
	int iRand;
	physent_t *pe;



	//MODDD - Bullet effects disabled!
	//gEngfuncs.pEfxAPI->R_BulletImpactParticles( pTrace->endpos );

	
	if(global2_muteRicochetSound < 1 ){

		//redundant with TE_GUNSHOT now used instead. It automatically plays a ricochet sound.
		/*
		iRand = gEngfuncs.pfnRandomLong(0,0x7FFF);
		if ( iRand < (0x7fff/2) )// not every bullet makes a sound.
		{
			switch( iRand % 5)
			{
			case 0:	gEngfuncs.pEventAPI->EV_PlaySound( -1, pTrace->endpos, 0, "weapons/ric1.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
			case 1:	gEngfuncs.pEventAPI->EV_PlaySound( -1, pTrace->endpos, 0, "weapons/ric2.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
			case 2:	gEngfuncs.pEventAPI->EV_PlaySound( -1, pTrace->endpos, 0, "weapons/ric3.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
			case 3:	gEngfuncs.pEventAPI->EV_PlaySound( -1, pTrace->endpos, 0, "weapons/ric4.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
			case 4:	gEngfuncs.pEventAPI->EV_PlaySound( -1, pTrace->endpos, 0, "weapons/ric5.wav", 1.0, ATTN_NORM, 0, PITCH_NORM ); break;
			}
		}
		*/
	}

	pe = gEngfuncs.pEventAPI->EV_GetPhysent( pTrace->ent );
	//return;
	// Only decal brush models such as the world etc.
	//easyPrintLine("EV_HLDM - DECAL NAME: %s", decalName);
	if (  decalName && decalName[0] && pe && ( pe->solid == SOLID_BSP || pe->movetype == MOVETYPE_PUSHSTEP ) )
	{
		if ( CVAR_GET_FLOAT( "r_decals" ) )
		{
			gEngfuncs.pEfxAPI->R_DecalShoot( 
				gEngfuncs.pEfxAPI->Draw_DecalIndex( gEngfuncs.pEfxAPI->Draw_DecalIndexFromName( decalName ) ), 
				gEngfuncs.pEventAPI->EV_IndexFromTrace( pTrace ), 0, pTrace->endpos, 0 );
		}
	}
}

void EV_HLDM_DecalGunshot( pmtrace_t *pTrace, int iBulletType )
{
	physent_t *pe;

	pe = gEngfuncs.pEventAPI->EV_GetPhysent( pTrace->ent );

	if ( pe && pe->solid == SOLID_BSP )
	{
		switch( iBulletType )
		{
		case BULLET_PLAYER_9MM:
		case BULLET_MONSTER_9MM:
		case BULLET_PLAYER_MP5:
		case BULLET_MONSTER_MP5:
		case BULLET_PLAYER_BUCKSHOT:
		case BULLET_PLAYER_357:
		default:
			// smoke and decal
			//MODDD NTOE - this call is important.
			EV_HLDM_GunshotDecalTrace( pTrace, EV_HLDM_DamageDecal( pe ) );
			break;
		}
	}
}

int EV_HLDM_CheckTracer( int idx, float *vecSrc, float *end, float *forward, float *right, int iBulletType, int iTracerFreq, int *tracerCount )
{
	int tracer = 0;
	int i;
	qboolean player = idx >= 1 && idx <= gEngfuncs.GetMaxClients() ? true : false;

	if ( iTracerFreq != 0 && ( (*tracerCount)++ % iTracerFreq) == 0 )
	{
		vec3_t vecTracerSrc;

		if ( player )
		{
			vec3_t offset( 0, 0, -4 );

			// adjust tracer position for player
			for ( i = 0; i < 3; i++ )
			{
				vecTracerSrc[ i ] = vecSrc[ i ] + offset[ i ] + right[ i ] * 2 + forward[ i ] * 16;
			}
		}
		else
		{
			VectorCopy( vecSrc, vecTracerSrc );
		}
		
		if ( iTracerFreq != 1 )		// guns that always trace also always decal
			tracer = 1;

		switch( iBulletType )
		{
		case BULLET_PLAYER_MP5:
		case BULLET_MONSTER_MP5:
		case BULLET_MONSTER_9MM:
		case BULLET_MONSTER_12MM:
		default:
			EV_CreateTracer( vecTracerSrc, end );
			break;
		}
	}

	return tracer;
}


/*
================
FireBullets

Go to the trouble of combining multiple pellets into a single damage call.
================
*/
void EV_HLDM_FireBullets( int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY )
{
	int i;
	pmtrace_t tr;
	int iShot;
	int tracer;



	//MODDD - TODO. create and enable here.
	if(EASY_CVAR_GET(playerBulletHitEffectForceServer) == 1){
		//Stop. The server is doing this instead.
		return;
	}

	

	
	for ( iShot = 1; iShot <= cShots; iShot++ )	
	{
		vec3_t vecDir, vecEnd;
			
		float x, y, z;
		//We randomize for the Shotgun.
		if ( iBulletType == BULLET_PLAYER_BUCKSHOT )
		{
			do {
				x = gEngfuncs.pfnRandomFloat(-0.5,0.5) + gEngfuncs.pfnRandomFloat(-0.5,0.5);
				y = gEngfuncs.pfnRandomFloat(-0.5,0.5) + gEngfuncs.pfnRandomFloat(-0.5,0.5);
				z = x*x+y*y;
			} while (z > 1);

			for ( i = 0 ; i < 3; i++ )
			{
				vecDir[i] = vecDirShooting[i] + x * flSpreadX * right[ i ] + y * flSpreadY * up [ i ];
				vecEnd[i] = vecSrc[ i ] + flDistance * vecDir[ i ];
			}
		}//But other guns already have their spread randomized in the synched spread.
		else
		{

			for ( i = 0 ; i < 3; i++ )
			{
				vecDir[i] = vecDirShooting[i] + flSpreadX * right[ i ] + flSpreadY * up [ i ];
				vecEnd[i] = vecSrc[ i ] + flDistance * vecDir[ i ];
			}
		}

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );
	
		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();
	
		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	

		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr );

		if(EASY_CVAR_GET(playerWeaponTracerMode) == 1){
			//retail: do tracers here as expected, and not necessarily every bullet.
			tracer = EV_HLDM_CheckTracer( idx, vecSrc, tr.endpos, forward, right, iBulletType, iTracerFreq, tracerCount );
		}else{
			tracer = NULL;
		}

		// do damage, paint decals
		if ( tr.fraction != 1.0 )
		{
			switch(iBulletType)
			{
			default:
			case BULLET_PLAYER_9MM:		
				
				EV_HLDM_PlayTextureSound( idx, &tr, vecSrc, vecEnd, iBulletType );
				EV_HLDM_DecalGunshot( &tr, iBulletType );
			
					break;
			case BULLET_PLAYER_MP5:		
				
				//MODDD NOTE - if this was a tracer don't do the usual gunshot and texturesound? what? Does this matter anymore now that the server can call for tracers
				//             and do this regardless of CVar "playerWeaponTracerMode"?
				if ( !tracer )
				{
					EV_HLDM_PlayTextureSound( idx, &tr, vecSrc, vecEnd, iBulletType );
					EV_HLDM_DecalGunshot( &tr, iBulletType );
				}
				break;
			case BULLET_PLAYER_BUCKSHOT:
				
				EV_HLDM_DecalGunshot( &tr, iBulletType );
			
				break;
			case BULLET_PLAYER_357:
				
				EV_HLDM_PlayTextureSound( idx, &tr, vecSrc, vecEnd, iBulletType );
				EV_HLDM_DecalGunshot( &tr, iBulletType );
				
				break;

			}
		}

		gEngfuncs.pEventAPI->EV_PopPMStates();
	}
}


//INVESTIGATE ME!!!!!!!!!!!!!!!!!
//======================
//	    GLOCK START
//======================
void EV_FireGlock1( event_args_t *args )
{

	//float silencerOn = CVAR_GET_FLOAT("glockSilencerOn");

	

	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	int empty;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;
	
	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	int InAttack = args->iparam1;

	empty = args->bparam1;
	AngleVectors( angles, forward, right, up );

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex ("models/shell.mdl");// brass shell


	int silencerOn = 0;
	if(InAttack == 0 || InAttack == 1){
		silencerOn = 0;
	}else if(InAttack == 2){
		silencerOn = 1;
	}


	if ( EV_IsLocal( idx ) )
	{

		//MODDD - silencer has no flash. This is the flash of light, not the sprite which is controlled by the animation itslef.
		//It is blocked seprately by a separate flag for renderfx that makes it from server to clientside, called NOMUZZLEFLASH.
		//By default the sprite is allowed but must specifically be blocked for the silencer instead, actively done in entity.cpp.
		if(!silencerOn){
			EV_MuzzleFlash();
		}else{
			cl_entity_t *ent = GetViewEntity();
			if(ent){
				ent->curstate.effects &= ~EF_MUZZLEFLASH;
			}
		}

		//MODDD - use a body of "InAttack", coordinated with whether the silencer is on or not.
		//gEngfuncs.pEventAPI->EV_WeaponAnimation( empty ? GLOCK_SHOOT_EMPTY : GLOCK_SHOOT, 2 );
		gEngfuncs.pEventAPI->EV_WeaponAnimation( empty ? GLOCK_SHOOT_EMPTY : GLOCK_SHOOT, silencerOn );

		//MODDD - why was this commented before?
		V_PunchAxis( 0, -2.0 );
	}

	EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4 );

	//MODDD - why were these commented before?
	EV_EjectBrass ( ShellOrigin, ShellVelocity, angles[ YAW ], shell, TE_BOUNCE_SHELL ); 
	
	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		if(!silencerOn){
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun3.wav", gEngfuncs.pfnRandomFloat(0.92, 1.0), ATTN_NORM, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
		}else{

			//The range of "0.6 - 0.8" for volume is what the "hassassin" uses.  Reasonable?  ACTUALLY, reduced slightly more...
			switch(gEngfuncs.pfnRandomLong(0,1))
			{
			case 0:
				//easyPrintLine("snd 0");
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun1.wav", gEngfuncs.pfnRandomFloat(0.55, 0.75), ATTN_IDLE, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
				break;
			case 1:
				//easyPrintLine("snd 1");
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun2.wav", gEngfuncs.pfnRandomFloat(0.55, 0.75), ATTN_IDLE, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
				break;
			}

		}
	}

	EV_GetGunPosition( args, vecSrc, origin );
	
	VectorCopy( forward, vecAiming );

	EV_HLDM_FireBullets( idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_9MM, 0, 0, args->fparam1, args->fparam2 );
	
	/*
			gEngfuncs.GetLocalPlayer()->curstate.frame = 126;
		gEngfuncs.GetLocalPlayer()->curstate.framerate = -1;
		gEngfuncs.GetLocalPlayer()->prevstate.frame = 126;
		gEngfuncs.GetLocalPlayer()->prevstate.framerate = -1;
		gEngfuncs.GetLocalPlayer()->baseline.frame = 126;
		gEngfuncs.GetLocalPlayer()->baseline.framerate = -1;
		*/

}

void EV_FireGlock2( event_args_t *args )
{

	
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t vecSpread;
	vec3_t up, right, forward;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	int InAttack = args->iparam1;


	int silencerOn = 0;
	if(InAttack == 0 || InAttack == 1){
		silencerOn = 0;
	}else if(InAttack == 2){
		silencerOn = 1;
	}



	AngleVectors( angles, forward, right, up );

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex ("models/shell.mdl");// brass shell

	if ( EV_IsLocal( idx ) )
	{
		// Add muzzle flash to current weapon model
		//MODDD - silencer has no flash.
		if(!silencerOn){
			EV_MuzzleFlash();
		}else{
			cl_entity_t *ent = GetViewEntity();
			if(ent){
				ent->curstate.effects &= ~EF_MUZZLEFLASH;
			}
		}
		
		//MODDD - use a body of "InAttack", coordinated with whether the silencer is on or not.
		//gEngfuncs.pEventAPI->EV_WeaponAnimation( GLOCK_SHOOT, 2 );
		gEngfuncs.pEventAPI->EV_WeaponAnimation( GLOCK_SHOOT, silencerOn );

		//MODDD - why was this commented?
		V_PunchAxis( 0, -2.0 );
	}

	
	EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4 );
	
	//MODDD - why were these commented?
	EV_EjectBrass ( ShellOrigin, ShellVelocity, angles[ YAW ], shell, TE_BOUNCE_SHELL ); 
	

	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		if(!silencerOn){
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun3.wav", gEngfuncs.pfnRandomFloat(0.92, 1.0), ATTN_NORM, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
		}else{

			switch(gEngfuncs.pfnRandomLong(0,1))
			{
			case 0:
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun1.wav", gEngfuncs.pfnRandomFloat(0.55, 0.75), ATTN_IDLE, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
				break;
			case 1:
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/pl_gun2.wav", gEngfuncs.pfnRandomFloat(0.55, 0.75), ATTN_IDLE, 0, 98 + gEngfuncs.pfnRandomLong( 0, 3 ) );
				break;
			}

		}
	}

	EV_GetGunPosition( args, vecSrc, origin );
	
	VectorCopy( forward, vecAiming );

	EV_HLDM_FireBullets( idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_9MM, 0, &tracerCount[idx-1], args->fparam1, args->fparam2 );
	
	

}
//======================
//	   GLOCK END
//======================

//======================
//	  SHOTGUN START
//======================
void EV_FireShotGunDouble( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	int j;
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t vecSpread;
	vec3_t up, right, forward;
	float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	AngleVectors( angles, forward, right, up );

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex ("models/shotgunshell.mdl");// brass shell

	if ( EV_IsLocal( idx ) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation( SHOTGUN_FIRE2, 2 );
		V_PunchAxis( 0, -10.0 );
	}

	for ( j = 0; j < 2; j++ )
	{
		EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 32, -12, 6 );

		EV_EjectBrass ( ShellOrigin, ShellVelocity, angles[ YAW ], shell, TE_BOUNCE_SHOTSHELL ); 
	}

	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/dbarrel1.wav", gEngfuncs.pfnRandomFloat(0.98, 1.0), ATTN_NORM, 0, 85 + gEngfuncs.pfnRandomLong( 0, 0x1f ) );
	}

	EV_GetGunPosition( args, vecSrc, origin );
	VectorCopy( forward, vecAiming );

	//playerWeaponForceSpreadMode.
	//0 = no effect (pick based on single or multiplayer as usual)
	//1 = standard circle (single player)
	//2 = wider, less tall (multiplayer)
	if(EASY_CVAR_GET(playerWeaponForceSpreadMode)!=2 && (EASY_CVAR_GET(playerWeaponForceSpreadMode)==1 || !WEAPON_DEFAULT_MULTIPLAYER_CHECK) )
	{
		//easyForcePrintLine("FLAG C-SINGLEPLAYER");
		//single player circle
		EV_HLDM_FireBullets( idx, forward, right, up, 12, vecSrc, vecAiming, 2048, BULLET_PLAYER_BUCKSHOT, 0, &tracerCount[idx-1], 0.08716, 0.08716 );
	}
	else
	{
		//easyForcePrintLine("FLAG C-MULTIPLAYER");
		//multiplayer wide. NOTE: wider (double) spread than single barrel fire, same height as single barrel fire (half single player).
		EV_HLDM_FireBullets( idx, forward, right, up, 8, vecSrc, vecAiming, 2048, BULLET_PLAYER_BUCKSHOT, 0, &tracerCount[idx-1], 0.17365, 0.04362 );
	}
}

void EV_FireShotGunSingle( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	
	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t vecSpread;
	vec3_t up, right, forward;
	float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	AngleVectors( angles, forward, right, up );

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex ("models/shotgunshell.mdl");// brass shell

	if ( EV_IsLocal( idx ) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		gEngfuncs.pEventAPI->EV_WeaponAnimation( SHOTGUN_FIRE, 2 );

		V_PunchAxis( 0, -5.0 );
	}

	EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 32, -12, 6 );

	EV_EjectBrass ( ShellOrigin, ShellVelocity, angles[ YAW ], shell, TE_BOUNCE_SHOTSHELL ); 

	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/sbarrel1.wav", gEngfuncs.pfnRandomFloat(0.95, 1.0), ATTN_NORM, 0, 93 + gEngfuncs.pfnRandomLong( 0, 0x1f ) );
	}

	EV_GetGunPosition( args, vecSrc, origin );
	VectorCopy( forward, vecAiming );

	//MODDD - same, see above for the ShotGunSingle
	if(EASY_CVAR_GET(playerWeaponForceSpreadMode)!=2 && (EASY_CVAR_GET(playerWeaponForceSpreadMode)==1 || !WEAPON_DEFAULT_MULTIPLAYER_CHECK) )
	{
		EV_HLDM_FireBullets( idx, forward, right, up, 6, vecSrc, vecAiming, 2048, BULLET_PLAYER_BUCKSHOT, 0, &tracerCount[idx-1], 0.08716, 0.08716 );
	}
	else
	{
		//multiplayer wide. NOTE: same spread width as single player, less spread height than single player.
		EV_HLDM_FireBullets( idx, forward, right, up, 4, vecSrc, vecAiming, 2048, BULLET_PLAYER_BUCKSHOT, 0, &tracerCount[idx-1], 0.08716, 0.04362 );
	}
}
//======================
//	   SHOTGUN END
//======================

//======================
//	    MP5 START
//======================
void EV_FireMP5( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t ShellVelocity;
	vec3_t ShellOrigin;
	int shell;
	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;
	float flSpread = 0.01;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	AngleVectors( angles, forward, right, up );

	int fireAnim = args->iparam1;

	shell = gEngfuncs.pEventAPI->EV_FindModelIndex ("models/shell.mdl");// brass shell
	
	if ( EV_IsLocal( idx ) )
	{
		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();
		
		//MODDD - the fire anim is now picked by the server for more control on that end and sent here as that, instead of being randomized here.
		//gEngfuncs.pEventAPI->EV_WeaponAnimation( MP5_FIRE1 + gEngfuncs.pfnRandomLong(0,2), 2 );
		gEngfuncs.pEventAPI->EV_WeaponAnimation( MP5_FIRE1 + fireAnim, 2 );
		

		V_PunchAxis( 0, gEngfuncs.pfnRandomFloat( -2, 2 ) );
	}

	EV_GetDefaultShellInfo( args, origin, velocity, ShellVelocity, ShellOrigin, forward, right, up, 20, -12, 4 );

	EV_EjectBrass ( ShellOrigin, ShellVelocity, angles[ YAW ], shell, TE_BOUNCE_SHELL ); 

	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
		{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/hks1.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf ) );
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/hks2.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf ) );
			break;
		}
	}

	EV_GetGunPosition( args, vecSrc, origin );
	VectorCopy( forward, vecAiming );

	//MODDD NOTE - in either case, spread is identical because it comes from what the client provided us with (args->fparam1 and args->fparam2 here).
	//             Not going to bother checking the CVar here then, whatever was decided for this bullet already has been by those params.
	//             Intervene in mp5.cpp calling for this event instead.
	/*
	if ( gEngfuncs.GetMaxClients() > 1 )
	{ ...
	*/

	EV_HLDM_FireBullets( idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_MP5, 2, &tracerCount[idx-1], args->fparam1, args->fparam2 );

}

// We only predict the animation and sound
// The grenade is still launched from the server.
void EV_FireMP52( event_args_t *args )
{
	int idx;
	vec3_t origin;
	
	idx = args->entindex;
	VectorCopy( args->origin, origin );

	if ( EV_IsLocal( idx ) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( MP5_LAUNCH, 2 );
		V_PunchAxis( 0, -10 );
	}
	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
		{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/glauncher.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf ) );
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/glauncher2.wav", 1, ATTN_NORM, 0, 94 + gEngfuncs.pfnRandomLong( 0, 0xf ) );
			break;
		}
	}
}
//======================
//		 MP5 END
//======================

//======================
//	   PHYTON START 
//	     ( .357 )
//======================
void EV_FirePython( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	vec3_t vecSrc, vecAiming;
	vec3_t up, right, forward;
	float flSpread = 0.01;


	int pythonModel = args->iparam1;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	AngleVectors( angles, forward, right, up );

	if ( EV_IsLocal( idx ) )
	{


		// Python uses different body in multiplayer versus single player
		//MODDD - this will be handled by the sender, python.cpp, instead of right here.  Rely on "InAttack" as a received parameter.
		//int multiplayer = gEngfuncs.GetMaxClients() == 1 ? 0 : 1;

		// Add muzzle flash to current weapon model
		EV_MuzzleFlash();

		//MODDD - replaced.
		//gEngfuncs.pEventAPI->EV_WeaponAnimation( PYTHON_FIRE1, multiplayer ? 1 : 0 );
		gEngfuncs.pEventAPI->EV_WeaponAnimation( PYTHON_FIRE1, pythonModel );

		//Nah, just disable all punches when the minimumfiredelay cheat is on (see view.cpp).
		//if(CVAR_GET_FLOAT("cheat_minimumfiredelay") == 0 ){
			V_PunchAxis( 0, -10.0 );
		//}else{
		//	V_PunchAxis( 0, 0 );
		//}

	}


	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		switch( gEngfuncs.pfnRandomLong( 0, 1 ) )
		{
		case 0:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/357_shot1.wav", gEngfuncs.pfnRandomFloat(0.8, 0.9), ATTN_NORM, 0, PITCH_NORM );
			break;
		case 1:
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/357_shot2.wav", gEngfuncs.pfnRandomFloat(0.8, 0.9), ATTN_NORM, 0, PITCH_NORM );
			break;
		}
	}


	EV_GetGunPosition( args, vecSrc, origin );
	
	VectorCopy( forward, vecAiming );

	EV_HLDM_FireBullets( idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_357, 0, 0, args->fparam1, args->fparam2 );
}
//======================
//	    PHYTON END 
//	     ( .357 )
//======================

//======================
//	   GAUSS START 
//======================
#define SND_CHANGE_PITCH	(1<<7)		// duplicated in protocol.h change sound pitch

void EV_SpinGauss( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	int iSoundState = 0;

	int pitch;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	pitch = args->iparam1;

	iSoundState = args->bparam1 ? SND_CHANGE_PITCH : 0;

	//MODDD - NOTE - period sound.  Played continuously.
	gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "ambience/pulsemachine.wav", 1.0, ATTN_NORM, iSoundState, pitch );
}

/*
==============================
EV_StopPreviousGauss

==============================
*/
void EV_StopPreviousGauss( int idx )
{
	// Make sure we don't have a gauss spin event in the queue for this guy
	gEngfuncs.pEventAPI->EV_KillEvents( idx, "events/gaussspin.sc" );
	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_WEAPON, "ambience/pulsemachine.wav" );
}

extern float g_flApplyVel;


void EV_FireGauss( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;
	float flDamage = args->fparam1;
	int primaryfire = args->bparam1;

	int m_fPrimaryFire = args->bparam1;
	int m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	vec3_t vecSrc;
	vec3_t vecDest;
	edict_t		*pentIgnore;
	pmtrace_t tr, beam_tr;
	float flMaxFrac = 1.0;
	int	nTotal = 0;
	int fHasPunched = 0;
	int fFirstBeam = 1;
	int	nMaxHits = 10;
	physent_t *pEntity;
	int m_iBeam, m_iGlow, m_iBalls;
	vec3_t up, right, forward;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	VectorCopy( args->velocity, velocity );

	if ( args->bparam2 )
	{
		EV_StopPreviousGauss( idx );
		return;
	}

//	Con_Printf( "Firing gauss with %f\n", flDamage );
	EV_GetGunPosition( args, vecSrc, origin );

	m_iBeam = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/smoke.spr" );
	m_iBalls = m_iGlow = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow.spr" );
	
	AngleVectors( angles, forward, right, up );

	VectorMA( vecSrc, 8192, forward, vecDest );

	if ( EV_IsLocal( idx ) )
	{
		V_PunchAxis( 0, -2.0 );
		gEngfuncs.pEventAPI->EV_WeaponAnimation( GAUSS_FIRE2, 2 );

		if ( m_fPrimaryFire == false )
			 g_flApplyVel = flDamage;	
			 
	}
	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/gauss2.wav", 0.5 + flDamage * (1.0 / 400.0), ATTN_NORM, 0, 85 + gEngfuncs.pfnRandomLong( 0, 0x1f ) );
	}

	
	

	
//NOTE: on any changes, sync me up with gauss.cpp (server-side)'s "Fire" method with a similar loop.
	while (flDamage > 10 && nMaxHits > 0)
	{
		nMaxHits--;

		gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );
		
		// Store off the old count
		gEngfuncs.pEventAPI->EV_PushPMStates();
	
		// Now add in all of the players.
		gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	

		gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
		gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecDest, PM_STUDIO_BOX, -1, &tr );

		gEngfuncs.pEventAPI->EV_PopPMStates();

		if ( tr.allsolid )
			break;

		if (fFirstBeam)
		{
			if ( EV_IsLocal( idx ) )
			{
				// Add muzzle flash to current weapon model
				EV_MuzzleFlash();
			}
			fFirstBeam = 0;

			gEngfuncs.pEfxAPI->R_BeamEntPoint( 
				idx | 0x1000,
				tr.endpos,
				m_iBeam,
				0.1,
				m_fPrimaryFire ? 1.0 : 2.5,
				0.0,
				m_fPrimaryFire ? 128.0 : flDamage,
				0,
				0,
				0,
				m_fPrimaryFire ? 255 : 255,
				m_fPrimaryFire ? 128 : 255,
				m_fPrimaryFire ? 0 : 255
			);
		}
		else
		{
			gEngfuncs.pEfxAPI->R_BeamPoints( vecSrc,
				tr.endpos,
				m_iBeam,
				0.1,
				m_fPrimaryFire ? 1.0 : 2.5,
				0.0,
				m_fPrimaryFire ? 128.0 : flDamage,
				0,
				0,
				0,
				m_fPrimaryFire ? 255 : 255,
				m_fPrimaryFire ? 128 : 255,
				m_fPrimaryFire ? 0 : 255
			);
		}

		pEntity = gEngfuncs.pEventAPI->EV_GetPhysent( tr.ent );
		if ( pEntity == NULL )
			break;

		if ( pEntity->solid == SOLID_BSP )
		{
			float n;

			pentIgnore = NULL;

			n = -DotProduct( tr.plane.normal, forward );


			int reflectCheckPossible = FALSE;
			if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET(gauss_primaryreflects)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET(gauss_secondaryreflects)!=0)   ){
				reflectCheckPossible = TRUE;
			}
			//MODDD - involved "reflectCheckPossible"
			if (reflectCheckPossible && n < 0.5) // 60 degrees	
			{
				// ALERT( at_console, "reflect %f\n", n );
				// reflect
				vec3_t r;
			
				VectorMA( forward, 2.0 * n, tr.plane.normal, r );

				flMaxFrac = flMaxFrac - tr.fraction;
				
				VectorCopy( r, forward );

				VectorMA( tr.endpos, 8.0, forward, vecSrc );
				VectorMA( vecSrc, 8192.0, forward, vecDest );

				gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, vec3_origin, 0.2, m_iGlow, kRenderGlow, kRenderFxNoDissipation, flDamage * n / 255.0, flDamage * n * 0.5 * 0.1, FTENT_FADEOUT );

				vec3_t fwd;
				VectorAdd( tr.endpos, tr.plane.normal, fwd );

				gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 3, 0.1, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
									255, 100 );

				// lose energy
				if ( n == 0 )
				{
					n = 0.1;
				}
				
				flDamage = flDamage * (1 - n);

			}
			else
			{
				// tunnel
				EV_HLDM_DecalGunshot( &tr, BULLET_MONSTER_12MM );

				gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, vec3_origin, 1.0, m_iGlow, kRenderGlow, kRenderFxNoDissipation, flDamage / 255.0, 6.0, FTENT_FADEOUT );

				// limit it to one hole punch
				if (fHasPunched)
				{
					break;
				}
				fHasPunched = 1;


				//MODDD - see mirrored portion of serverside's gauss.cpp.
				//if ( !m_fPrimaryFire )
				int punchAttempt = FALSE;
				if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET(gauss_primarypunchthrough)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET(gauss_secondarypunchthrough)!=0)   ){
					punchAttempt = TRUE;
				}
				// try punching through wall if secondary attack (primary is incapable of breaking through)
				if(punchAttempt == TRUE)
				{
					vec3_t start;

					VectorMA( tr.endpos, 8.0, forward, start );

					// Store off the old count
					gEngfuncs.pEventAPI->EV_PushPMStates();
						
					// Now add in all of the players.
					gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );

					gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
					gEngfuncs.pEventAPI->EV_PlayerTrace( start, vecDest, PM_STUDIO_BOX, -1, &beam_tr );

					if ( !beam_tr.allsolid )
					{
						vec3_t delta;
						float n;

						// trace backwards to find exit point

						gEngfuncs.pEventAPI->EV_PlayerTrace( beam_tr.endpos, tr.endpos, PM_STUDIO_BOX, -1, &beam_tr );

						VectorSubtract( beam_tr.endpos, tr.endpos, delta );
						
						n = Length( delta );

						if (n < flDamage)
						{
							if (n == 0)
								n = 1;
							flDamage -= n;

							// absorption balls
							{
								vec3_t fwd;
								VectorSubtract( tr.endpos, forward, fwd );
								gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 3, 0.1, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
									255, 100 );
							}

	//////////////////////////////////// WHAT TO DO HERE
							// CSoundEnt::InsertSound ( bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3.0 );

							EV_HLDM_DecalGunshot( &beam_tr, BULLET_MONSTER_12MM );
							
							gEngfuncs.pEfxAPI->R_TempSprite( beam_tr.endpos, vec3_origin, 0.1, m_iGlow, kRenderGlow, kRenderFxNoDissipation, flDamage / 255.0, 6.0, FTENT_FADEOUT );
			
							// balls
							{
								vec3_t fwd;
								VectorSubtract( beam_tr.endpos, forward, fwd );
								gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, beam_tr.endpos, fwd, m_iBalls, (int)(flDamage * 0.3), 0.1, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 200,
									255, 40 );
							}
							
							VectorAdd( beam_tr.endpos, forward, vecSrc );
						}
					}
					else
					{
						flDamage = 0;
					}

					gEngfuncs.pEventAPI->EV_PopPMStates();
				}
				else
				{
					if ( m_fPrimaryFire )
					{
						// slug doesn't punch through ever with primary 
						// fire, so leave a little glowy bit and make some balls
						gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, vec3_origin, 0.2, m_iGlow, kRenderGlow, kRenderFxNoDissipation, 200.0 / 255.0, 0.3, FTENT_FADEOUT );
			
						{
							vec3_t fwd;
							VectorAdd( tr.endpos, tr.plane.normal, fwd );
							gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 8, 0.6, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
								255, 200 );
						}
					}

					flDamage = 0;
				}
			}
		}
		else
		{
			//MODDD
			int canPierce = FALSE;
			if( (m_fPrimaryFire==TRUE && EASY_CVAR_GET(gauss_primarypierces)!=0 ) || (m_fPrimaryFire==FALSE && EASY_CVAR_GET(gauss_secondarypierces)!=0)   ){
				canPierce = TRUE;
			}

			if(!canPierce){
				break;
			}



			VectorAdd( tr.endpos, forward, vecSrc );
		}
	}//END OF while(...)
}
//======================
//	   GAUSS END 
//======================


void createBall(int* sprite, Vector* loc){



	float randomStrength = 56;

	/*
	float randx = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	*/
	float randx = gEngfuncs.pfnRandomFloat(0, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(0, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(35, randomStrength +80);

	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randx *= -1;
	}
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randy *= -1;
	}
	
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randz *= -1;
	}
	

	
	vec3_t rot = Vector(randx, randy, randz);

	
	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( *loc, rot, 0.12f, *sprite, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 0.22f, FTENT_GRAVITY | FTENT_COLLIDEWORLD | FTENT_FADEOUT );
	eh->fadeSpeed = 3.3f;
	//easyPrintLine("??? %.2f", eh->bounceFactor);

}



void createBallPowerup(int* sprite, Vector* loc){



	float randomStrength = 56;

	/*
	float randx = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	*/
	float randx = gEngfuncs.pfnRandomFloat(0, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(0, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(35, randomStrength +80);

	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randx *= -1;
	}
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randy *= -1;
	}
	
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randz *= -1;
	}
	

	
	vec3_t rot = Vector(randx, randy, randz);

	
	//MODDD - color it red. how we dod that?
	//life is 2nd from the last with the flags yo.
	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( *loc, rot, 0.12f, *sprite, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 1.22f, FTENT_SLOWGRAVITY | FTENT_COLLIDEWORLD | FTENT_FADEOUT );
	eh->fadeSpeed = 0.6f;
	//eh->entity.baseline.gravity
	//eh->entity.curstate.gravity
	//eh->entity.prevstate.gravity
	//???


	//MODDD - remove "FTENT_GRAVITY" from above, yes or no?

	//unnecessary yes?
	//eh->flags |= (FTENT_SLOWGRAVITY);


	//easyPrintLine("??? %.2f", eh->bounceFactor);

}


void createBallVomit(int* sprite, Vector* loc, Vector* dir){



	float randomStrength = 80;

	/*
	float randx = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	*/
	float randx = dir->x * 270 + gEngfuncs.pfnRandomFloat(-20, randomStrength);
	float randy = dir->y * 270 + gEngfuncs.pfnRandomFloat(-20, randomStrength);
	float randz = 50 + gEngfuncs.pfnRandomFloat(-40, randomStrength);

	/*
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randx *= -1;
	}
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randy *= -1;
	}
	
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randz *= -1;
	}
	*/

	
	vec3_t rot = Vector(randx, randy, randz);

	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( *loc, rot, 0.26f, *sprite, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 0.14f, FTENT_GRAVITY | FTENT_COLLIDEWORLD | FTENT_FADEOUT );
	if(eh){
		eh->fadeSpeed = 3.4f;
	}else{
		easyForcePrintLine("WHY YOU FAIL");
	}
	//easyPrintLine("??? %.2f", eh->bounceFactor);

}










void generateFreakyLight(const Vector& arg_origin){

	//g_engfuncs.pfnCVarSetFloat

	//g_engfuncs.


	//clientside way:
	//gEngfuncs.pfnGetCvarPointer
	//NOTE: equivalent for serverside:
	//g_engfuncs.pfnCVarGetPointer
	



	/*
	float durationMin = 0.08;
	float durationMax = 0.14;
	float radiusMin = 280;
	float radiusMax = 410;
	float spawnDistHori = 180;
	float spawnDistVertMin = 25;
	float spawnDistVertMax = 75;
	int multiColor = TRUE;
	*/
	float durationMin = global2_strobeDurationMin;
	float durationMax = global2_strobeDurationMax;
	float radiusMin = global2_strobeRadiusMin;
	float radiusMax = global2_strobeRadiusMax;
	float spawnDistHori = global2_strobeSpawnDistHori;
	float spawnDistVertMin = global2_strobeSpawnDistVertMin;
	float spawnDistVertMax = global2_strobeSpawnDistVertMax;
	

	int multiColor;
	if(global2_strobeMultiColor == 1){
		multiColor = TRUE;
	}else{
		multiColor = FALSE;
	}


	
	const Vector& origin = arg_origin;

	float randx;
	float randy;
	float randz;

	dlight_t *dl = gEngfuncs.pEfxAPI->CL_AllocDlight (0);
	float temp;

	randx = gEngfuncs.pfnRandomFloat(30, spawnDistHori);
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randx *= -1;
	}
	randy = gEngfuncs.pfnRandomFloat(30, spawnDistHori);
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randy *= -1;
	}
	randz = gEngfuncs.pfnRandomFloat(spawnDistVertMin, spawnDistVertMax);

	randx += origin.x;
	randy += origin.y;
	randz += origin.z;

	float randRad = gEngfuncs.pfnRandomFloat(radiusMin, radiusMax);
	float randLife = gEngfuncs.pfnRandomFloat(durationMin, durationMax);

	dl->origin.x = randx;
	dl->origin.y = randy;
	dl->origin.z = randz;


	dl->dark = false;
	dl->die = gEngfuncs.GetClientTime() + randLife;
	

	int colorReceive[3];
	
	if(multiColor == TRUE){
		generateColor(colorReceive);
	}else{
		//always solid white.
		colorReceive[0] = 255;
		colorReceive[1] = 255;
		colorReceive[2] = 255;
	}



	dl->radius = randRad;
	dl->color.r = colorReceive[0];
	dl->color.g = colorReceive[1];
	dl->color.b = colorReceive[2];
	
}




void generateFreakyLaser(const Vector& arg_origin){
	//BEAM		*( *R_BeamPoints )				( float * start, float * end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	//BEAM		*( *R_BeamRing )				( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	
	const Vector& origin = arg_origin;

	int m_iLaserSprite = 0;
	
	//might be wiser to do this in some "init" function clientside-wide instead?  Same for other "EV_FindModelIndex" calls?
	m_iLaserSprite = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/laserbeam.spr" );

	float randOrigin1[3];
	float randOrigin2[3];

	//range??
	float mag = randomValue(global2_raveLaserLength, global2_raveLaserLength);

	float fltDeg = randomValue(0, CONST_2PI);
	if(fltDeg >= CONST_2PI){
		fltDeg -= CONST_2PI;
	}
	
	float x = cos(fltDeg) * mag;
	float y = sin(fltDeg) * mag;


	fltDeg += CONST_180_RAD;

	if(fltDeg >= CONST_2PI){
		fltDeg -= CONST_2PI;
	}

	//float fltMag = sqrt( pow(x, 2) + pow(y, 2) + pow(z, 2) );

	float x2 = cos(fltDeg) * mag;
	float y2 = sin(fltDeg) * mag;

	float randomShiftX = randomAbsoluteValue(global2_raveLaserSpawnDistHoriMin, global2_raveLaserSpawnDistHoriMax);
	float randomShiftY = randomAbsoluteValue(global2_raveLaserSpawnDistHoriMin, global2_raveLaserSpawnDistHoriMax);
	float randomShiftZ = randomAbsoluteValue(global2_raveLaserSpawnDistVertMin, global2_raveLaserSpawnDistVertMax);
	float randomShiftZ2 = randomAbsoluteValue(global2_raveLaserSpawnDistVertMin, global2_raveLaserSpawnDistVertMax);

	randOrigin1[0] = origin[0] + x + randomShiftX;
	randOrigin1[1] = origin[1] + y + randomShiftY;
	randOrigin1[2] = origin[2] + randomShiftZ;

	randOrigin2[0] = origin[0] + -x + randomShiftX;
	randOrigin2[1] = origin[1] + -y + randomShiftY;
	//test with the same "randomShiftZ" once?
	randOrigin2[2] = origin[2] + randomShiftZ2;


	float randLife = gEngfuncs.pfnRandomFloat(global2_raveLaserDurationMin, global2_raveLaserDurationMax);
	float randWidth = gEngfuncs.pfnRandomFloat(global2_raveLaserThicknessMin, global2_raveLaserThicknessMax);

	float randAmp = gEngfuncs.pfnRandomFloat(global2_raveLaserNoiseMin, global2_raveLaserNoiseMax);

	float randBrightness = gEngfuncs.pfnRandomFloat(global2_raveLaserBrightnessMin * 255, global2_raveLaserBrightnessMax * 255);
	//INTERPRET FROM 0 - 1, like  0.8 - 1.0!
	float frameRate = gEngfuncs.pfnRandomFloat(global2_raveLaserFrameRateMin, global2_raveLaserFrameRateMax);
	
	float speed = 1;
	float startFrame = 0;


	
	int colorReceive[3];
	
	if(global2_raveLaserMultiColor != 0){
		generateColor(colorReceive);
	}else{
		//always solid white.
		colorReceive[0] = 255;
		colorReceive[1] = 255;
		colorReceive[2] = 255;
	}




	//(float*)Vector(randx1, randy1, randz1)
	BEAM* someBeam = gEngfuncs.pEfxAPI->R_BeamPoints( randOrigin1, randOrigin2, m_iLaserSprite, randLife, randWidth, randAmp, randBrightness, speed, startFrame, frameRate, 0, 0, 0);

	
	//short someShort = gEngfuncs.pEfxAPI->R_LookupColor(colorReceive[0], colorReceive[1], colorReceive[2]);
	//easyPrintLine("HOWWWWWWWW %d::: <-- %d %d %d", someShort, colorReceive[0], colorReceive[1], colorReceive[2]);
	//what is this??

	
	//For unknown reasons, the color arguments, the 3 last args above, aren't working as expected.  Below seems to work though.  
	//Just check to see whether the created beam is null or not (may fail to create on too many requests or... who knows why else).
	//easyPrintLine("IS IT NULLLLL %d", (someBeam == NULL));
	if(someBeam == NULL){

	}else{
		//divided by 65025.0f (this is 256^2).  Why?  Because anything above 1/256^2 distorts colors, making even (255, 12, 0) show up as bright yellow.
		//Anything less than 1/256^2 is treated as getting closer to black, which makes it more transparent up to invisible at 0.
		someBeam->r = colorReceive[0]/65025.0f;
		someBeam->g = colorReceive[1]/65025.0f;
		someBeam->b = colorReceive[2]/65025.0f;	
	}


}





void EV_FreakyLight( event_args_t* args){
	int i = 0;

	//this is handled in method "UpdateClientData" of file "hud_update.cpp" instead.
	//updateCVarRefsClient();

	


	generateFreakyLight( (float*)&args->origin );

	if(global2_raveLaserEnabled == 1){
		
		//if there is a decimal in raveLaserSpawnFreq, guarantee spawning of the first whole number lasers (like in 2.4, spawn 2 lasers).
		//Treat the 0.4 decimal as a "40%" chance of producing another laser.  Rounds out nice and evenly when it happens a lot.
		int toGen = (int)global2_raveLaserSpawnFreq;   //truncate.
		//roundoff?
		float roundoff = ((int)(global2_raveLaserSpawnFreq * 10) % 10 ) / 10.0f;
		if(roundoff != 0 && gEngfuncs.pfnRandomLong(0, 1) <= roundoff){
			//extra.
			toGen++;
		}
		for(i = 0; i < toGen; i++){
			generateFreakyLaser( (float*)&args->origin );
		}
	}//END OF if(global2_raveLaserEnabled == 1)

}





	
void EV_FriendlyVomit( event_args_t* args){


	
	int duckyou = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow.spr" );

	int m_iHotglowGreen;
	m_iHotglowGreen = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow_green.spr" );
	

	vec3_t origin;
	vec3_t ang;
	//vec3_origin
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, ang);

	int ballsToSpawn = args->iparam1;
	
	int balls = ballsToSpawn;
	for(int i = 0; i < balls; i++){
		createBallVomit(&m_iHotglowGreen, &origin, &ang);
	}

}//END OF EV_FriendlyVomit







void shrapnelHitCallback( struct tempent_s *ent, struct pmtrace_s *ptr ){

	
	dlight_t *dl = gEngfuncs.pEfxAPI->CL_AllocDlight (0);
	VectorCopy ( ent->entity.origin, dl->origin );

	dl->dark = false;
	//time of "0.01" or "0.001"?
	//dl->die = gEngfuncs.GetClientTime() + 0.001; //Kill it right away
	dl->die = gEngfuncs.GetClientTime() + 0.03; //Kill it right away
	
	dl->radius = 80;
	dl->color.r = 255;
	dl->color.g = 120;
	dl->color.b = 0;

	
	
	
}

void createShrapnel(int* sprite, Vector* loc, int testArg, float testArg2, float testArg3){



	float randomStrength = 190; //121;
	float heightExtra = 140;

	randomStrength = testArg2;
	heightExtra = testArg3;


	/*
	float randx = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(-randomStrength, randomStrength);
	*/
	float randx = gEngfuncs.pfnRandomFloat(20, randomStrength);
	float randy = gEngfuncs.pfnRandomFloat(20, randomStrength);
	float randz = gEngfuncs.pfnRandomFloat(45, randomStrength + heightExtra);

	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randx *= -1;
	}
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randy *= -1;
	}
	
	if(gEngfuncs.pfnRandomLong(0, 1) == 0){
		randz *= -1;
	}
	

	
	vec3_t rot = Vector(randx, randy, randz);

	

	float lifeValue = gEngfuncs.pfnRandomFloat(2.25, 3.55);
	
	
	int flags = 0;
	if(testArg == 1){
		//no flash on hit.
		flags = BREAK_METAL | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD | FTENT_FADEOUT | FTENT_ROTATE;;
	}else{
		//flash on hit.
		flags = BREAK_METAL | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD | FTENT_FADEOUT | FTENT_ROTATE | FTENT_FLASHONHIT;
		//flags = BREAK_METAL | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD | FTENT_FADEOUT | FTENT_ROTATE | FTENT_FLICKER;
	}
	
	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( *loc, rot, 2.5f, *sprite, kRenderNormal, kRenderFxNone, 255.0 / 255.0, lifeValue, flags);
	eh->fadeSpeed = 0.5f;


	//gives this the metal hit sound effects.  I guess it is hardcoded what values of "hitSound" make what sounds, can't find the resulting sound calls for that anywhere.
	//Closest is entity.cpp, where this is referred to but still not used directly.
	//Calls a method that is pre-compiled in this line:
	//    Callback_TempEntPlaySound(pTemp, damp);
	//...assume it is what refers to this.


	//eh->hitSound = 2;    hit sound disabled, actually.  Enabling this gives it the break metal sounds though.
	eh->hitcallback = &shrapnelHitCallback;

	//no "BREAK_METAL " here.
	//eh->flags |= (FTENT_SLOWGRAVITY  | FTENT_SMOKETRAIL );


	eh->entity.baseline.angles[0] = gEngfuncs.pfnRandomFloat(-256, 256);
	eh->entity.baseline.angles[1] = gEngfuncs.pfnRandomFloat(-256, 256);
	eh->entity.baseline.angles[2] = gEngfuncs.pfnRandomFloat(-256, 256);
	

	




	//easyPrintLine("??? %.2f", eh->bounceFactor);
	//default bounce factor is 1.0.

}



void EV_QuakeExplosionEffect( event_args_t* args){

	
	Vector origin = args->origin;
	int shrapnel3D = gEngfuncs.pEventAPI->EV_FindModelIndex("models/shrapnel.mdl");
	
	//TEST IN SESSION!


	//R_Explosion

	////void( *R_Explosion )( float *pos, int model, float scale, float framerate, int flags );
	//gEngfuncs.pEfxAPI->R_Explosion( args->origin, m_iBalls, 0.3, 15, TE_EXPLFLAG_NONE );
	

	//void		( *R_BreakModel )				( float *pos, float *size, float *dir, float random, float life, int count, int modelIndex, char flags );
	//!!!!!!!!!!!!!!!!!!!!!!!!!


	int testArg = args->iparam1;
	
	float testArg2 = args->fparam1;
	float testArg3 = args->fparam2;

	//int shrapz = 14;
	int shrapz = args->iparam2;

	if(testArg == 0){
		//"FTENT_FLASHONHIT" doesn't seem to do anything for whatever reason.
		gEngfuncs.pEfxAPI->R_BreakModel(args->origin, Vector(0,0,0), Vector(0,0,100), 130, 2.5, shrapz, shrapnel3D,  BREAK_METAL | FTENT_SMOKETRAIL); //BREAK_METAL | FTENT_GRAVITY | FTENT_COLLIDEWORLD  | FTENT_SMOKETRAIL );
	}else{
	

		dlight_t *dl = gEngfuncs.pEfxAPI->CL_AllocDlight ( 0 );
		VectorCopy ( args->origin, dl->origin );

		dl->radius = 130;
		dl->dark = false;
		dl->die = gEngfuncs.GetClientTime() + 0.03; //Kill it right away
													 
		dl->color.r = 255;
		dl->color.g = 255;
		dl->color.b = 255;



		
		for(int i = 0; i < shrapz; i++){
			createShrapnel(&shrapnel3D, &origin, testArg, testArg2, testArg3);
		}
	}
	
	
	return;
	


}



//extern float global_sparkBallAmmountMulti;
//float global_sparkBallAmmountMulti = 1;
//No, use it to influence the parameter.

void EV_ShowBalls( event_args_t *args )
{

	/*
	if(CVAR_GET_FLOAT("sparkMod")){

	}
	*/


	

	int ballsToSpawn = args->iparam1;
	
	/*
	if(global_sparkBallAmmountMulti != 1){
		//multiplying by 1 is useless, so don't if it is.
		ballsToSpawn =  (int) ((float)ballsToSpawn * global_sparkBallAmmountMulti);
	}
	*/
	//...not working out, just trust this CVar affected "args->iparam1".
	//Check for 0 balls, however.

	if(ballsToSpawn == 0){
		return;  //nothing to do here
	}
	
	
	//pmtrace_t tr;
	int m_iBeam, m_iGlow, m_iBalls;
	m_iBalls = m_iGlow = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow.spr" );

	


	vec3_t origin;
	//vec3_origin
	VectorCopy( args->origin, origin );



	
	//vec3_t fwd;
	//VectorAdd( tr.endpos, tr.plane.normal, fwd );


	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 8, 0.6, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//		255, 200 );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, origin, origin, m_iBalls, 8, 0.1f, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//		255, 40 );
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//...( float *pos, float speed, float life, int count, int modelIndex );
	//gEngfuncs.pEfxAPI->R_TempSphereModel(origin, 20, 30, 8, m_iBalls);

	//void		( *R_Sprite_Spray )				( float * pos, float * dir, int modelIndex, int count, int speed, int iRand );
	

	//try render modes 3 & 5?
	//gEngfuncs.pEfxAPI->R_Spray(origin, rot, m_iBalls, 8, 10, 3, 3);
	//

	int balls = ballsToSpawn;
	for(int i = 0; i < balls; i++){
		createBall(&m_iBalls, &origin);
	}
		
		
	//eh->die = 1;
	//easyPrintLine("??? %.2f", eh->fadeSpeed);
	
	//eh->entity.dir = 7;
	//eh->entity.curstate.velocity.y = 7;
	//eh->entity.curstate.velocity.z = 7;
	


	//gEngfuncs.pEfxAPI->R_SparkShower( tr.endpos );

	

	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 8, 0.6, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//							255, 200 );

	//int type, float * start, float * end, int modelIndex, int count, float life, float size, float amplitude, int renderamt, float speed 


}



//copy to show that agrunts are powered up. Periodically emits, floats, and fades.
void EV_ShowBallsPowerup( event_args_t *args )
{

	/*
	if(CVAR_GET_FLOAT("sparkMod")){

	}
	*/


	int ballsToSpawn = args->iparam1;
	
	/*
	if(global_sparkBallAmmountMulti != 1){
		//multiplying by 1 is useless, so don't if it is.
		ballsToSpawn =  (int) ((float)ballsToSpawn * global_sparkBallAmmountMulti);
	}
	*/
	//...not working out, just trust this CVar affected "args->iparam1".
	//Check for 0 balls, however.

	if(ballsToSpawn == 0){
		return;  //nothing to do here
	}
	
	
	//pmtrace_t tr;
	int m_iBeam, m_iGlow, m_iBalls;
	m_iBalls = m_iGlow = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow.spr" );

	


	vec3_t origin;
	//vec3_origin
	VectorCopy( args->origin, origin );



	
	//vec3_t fwd;
	//VectorAdd( tr.endpos, tr.plane.normal, fwd );


	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 8, 0.6, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//		255, 200 );

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, origin, origin, m_iBalls, 8, 0.1f, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//		255, 40 );
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	//...( float *pos, float speed, float life, int count, int modelIndex );
	//gEngfuncs.pEfxAPI->R_TempSphereModel(origin, 20, 30, 8, m_iBalls);

	//void		( *R_Sprite_Spray )				( float * pos, float * dir, int modelIndex, int count, int speed, int iRand );
	

	//try render modes 3 & 5?
	//gEngfuncs.pEfxAPI->R_Spray(origin, rot, m_iBalls, 8, 10, 3, 3);
	//

	int balls = ballsToSpawn;
	for(int i = 0; i < balls; i++){
		createBallPowerup(&m_iBalls, &origin);
	}
		
		
	//eh->die = 1;
	//easyPrintLine("??? %.2f", eh->fadeSpeed);
	
	//eh->entity.dir = 7;
	//eh->entity.curstate.velocity.y = 7;
	//eh->entity.curstate.velocity.z = 7;
	


	//gEngfuncs.pEfxAPI->R_SparkShower( tr.endpos );

	

	//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 8, 0.6, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
	//							255, 200 );

	//int type, float * start, float * end, int modelIndex, int count, float life, float size, float amplitude, int renderamt, float speed 


}

void EV_HLDM_DecalGunshotCustomEvent( event_args_t *args )
{
	vec3_t origin;
	//vec3_origin
	VectorCopy( args->origin, origin );

	//EV_HLDM_FireBullets( idx, forward, right, up, 1, vecSrc, vecAiming, 8192, BULLET_PLAYER_9MM, 0, 0, args->fparam1, args->fparam2 );
	//void EV_HLDM_FireBullets( int idx, float *forward, float *right, float *up, int cShots, float *vecSrc, float *vecDirShooting, float flDistance, int iBulletType, int iTracerFreq, int *tracerCount, float flSpreadX, float flSpreadY )
	
	
	
	//pmtrace_t tr;
	//gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr );

	
//	EV_HLDM_DecalGunshot( origin, BULLET_PLAYER_9MM );

}






//======================
//	   CROWBAR START
//======================

enum crowbar_e {
	CROWBAR_IDLE = 0,
	CROWBAR_DRAW,
	CROWBAR_HOLSTER,
	CROWBAR_ATTACK1HIT,
	CROWBAR_ATTACK1MISS,
	CROWBAR_ATTACK2MISS,
	CROWBAR_ATTACK2HIT,
	CROWBAR_ATTACK3MISS,
	CROWBAR_ATTACK3HIT
};

int g_iSwing;

//Only predict the miss sounds, hit sounds are still played 
//server side, so players don't get the wrong idea.
void EV_Crowbar( event_args_t *args )
{
	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	
	//Play Swing sound
	gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/cbar_miss1.wav", 1, ATTN_NORM, 0, PITCH_NORM); 


	int swingMissChoice = args->iparam1;

	
	if ( EV_IsLocal( idx ) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( CROWBAR_ATTACK1MISS, 1 );


		
		//MODDD - handle by parameter now.
		//switch( (g_iSwing++) % 3 )
		switch(swingMissChoice)
		{
			case 0:
				gEngfuncs.pEventAPI->EV_WeaponAnimation ( CROWBAR_ATTACK1MISS, 1 ); break;
			case 1:
				gEngfuncs.pEventAPI->EV_WeaponAnimation ( CROWBAR_ATTACK2MISS, 1 ); break;
			case 2:
				gEngfuncs.pEventAPI->EV_WeaponAnimation ( CROWBAR_ATTACK3MISS, 1 ); break;
		}
		
	}
	
	
}
//======================
//	   CROWBAR END 
//======================

//======================
//	  CROSSBOW START
//======================
enum crossbow_e {
	CROSSBOW_IDLE1 = 0,	// full
	CROSSBOW_IDLE2,		// empty
	CROSSBOW_FIDGET1,	// full
	CROSSBOW_FIDGET2,	// empty
	CROSSBOW_FIRE1,		// full
	CROSSBOW_FIRE2,		// reload
	CROSSBOW_FIRE3,		// empty
	CROSSBOW_RELOAD,	// from empty
	CROSSBOW_DRAW1,		// full
	CROSSBOW_DRAW2,		// empty
	CROSSBOW_HOLSTER1,	// full
	CROSSBOW_HOLSTER2,	// empty
};

//=====================
// EV_BoltCallback
// This function is used to correct the origin and angles 
// of the bolt, so it looks like it's stuck on the wall.
//=====================
void EV_BoltCallback ( struct tempent_s *ent, float frametime, float currenttime )
{
	ent->entity.origin = ent->entity.baseline.vuser1;
	ent->entity.angles = ent->entity.baseline.vuser2;
}

void EV_FireCrossbow2( event_args_t *args )
{
	vec3_t vecSrc, vecEnd;
	vec3_t up, right, forward;
	pmtrace_t tr;

	int idx;
	vec3_t origin;
	vec3_t angles;
	vec3_t velocity;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );

	VectorCopy( args->velocity, velocity );
	
	AngleVectors( angles, forward, right, up );

	EV_GetGunPosition( args, vecSrc, origin );

	VectorMA( vecSrc, 8192, forward, vecEnd );

	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/xbow_fire1.wav", 1, ATTN_NORM, 0, 93 + gEngfuncs.pfnRandomLong(0,0xF) );

		if(EASY_CVAR_GET(crossbowFirePlaysReloadSound) > 0){
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_ITEM, "weapons/xbow_reload1.wav", gEngfuncs.pfnRandomFloat(0.95, 1.0), ATTN_NORM, 0, 93 + gEngfuncs.pfnRandomLong(0,0xF) );
		}
	}

	if ( EV_IsLocal( idx ) )
	{
		if ( args->iparam1 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( CROSSBOW_FIRE1, 1 );
		else if ( args->iparam2 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( CROSSBOW_FIRE3, 1 );
	}

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr );
	
	//We hit something
	if ( tr.fraction < 1.0 )
	{
		physent_t *pe = gEngfuncs.pEventAPI->EV_GetPhysent( tr.ent ); 

		//Not the world, let's assume we hit something organic ( dog, cat, uncle joe, etc ).
		if ( pe->solid != SOLID_BSP )
		{
			switch( gEngfuncs.pfnRandomLong(0,1) )
			{
			case 0:
				gEngfuncs.pEventAPI->EV_PlaySound( idx, tr.endpos, CHAN_BODY, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM, 0, PITCH_NORM ); break;
			case 1:
				gEngfuncs.pEventAPI->EV_PlaySound( idx, tr.endpos, CHAN_BODY, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM, 0, PITCH_NORM ); break;
			}
		}
		//Stick to world but don't stick to glass, it might break and leave the bolt floating. It can still stick to other non-transparent breakables though.
		else if ( pe->rendermode == kRenderNormal ) 
		{
			gEngfuncs.pEventAPI->EV_PlaySound( 0, tr.endpos, CHAN_BODY, "weapons/xbow_hit1.wav", gEngfuncs.pfnRandomFloat(0.95, 1.0), ATTN_NORM, 0, PITCH_NORM );
		
			//Not underwater, do some sparks...
			if ( gEngfuncs.PM_PointContents( tr.endpos, NULL ) != CONTENTS_WATER)
				 gEngfuncs.pEfxAPI->R_SparkShower( tr.endpos );

			vec3_t vBoltAngles;
			int iModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex( "models/crossbow_bolt.mdl" );

			VectorAngles( forward, vBoltAngles );

			TEMPENTITY *bolt = gEngfuncs.pEfxAPI->R_TempModel( tr.endpos - forward * 10, Vector( 0, 0, 0), vBoltAngles , 5, iModelIndex, TE_BOUNCE_NULL );
			
			if ( bolt )
			{
				bolt->flags |= ( FTENT_CLIENTCUSTOM ); //So it calls the callback function.
				bolt->entity.baseline.vuser1 = tr.endpos - forward * 10; // Pull out a little bit
				bolt->entity.baseline.vuser2 = vBoltAngles; //Look forward!

				//MODDD - be explicit! It's the address OF the method you want!
				//        compiler figures it out anyways I suppose.
				bolt->callback = &EV_BoltCallback; //So we can set the angles and origin back. (Stick the bolt to the wall)
			}
		}
	}

	gEngfuncs.pEventAPI->EV_PopPMStates();
}

//TODO: Fully predict the fliying bolt.
void EV_FireCrossbow( event_args_t *args )
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	
	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/xbow_fire1.wav", 1, ATTN_NORM, 0, 93 + gEngfuncs.pfnRandomLong(0,0xF) );

		if(EASY_CVAR_GET(crossbowFirePlaysReloadSound) > 0){
			gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_ITEM, "weapons/xbow_reload1.wav", gEngfuncs.pfnRandomFloat(0.95, 1.0), ATTN_NORM, 0, 93 + gEngfuncs.pfnRandomLong(0,0xF) );
		}
	}

	//Only play the weapon anims if I shot it. 
	if ( EV_IsLocal( idx ) )
	{
		if ( args->iparam1 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( CROSSBOW_FIRE1, 1 );
		else if ( args->iparam2 )
			gEngfuncs.pEventAPI->EV_WeaponAnimation( CROSSBOW_FIRE3, 1 );

		V_PunchAxis( 0, -2.0 );
	}
}
//======================
//	   CROSSBOW END 
//======================

//======================
//	    RPG START 
//======================
enum rpg_e {
	RPG_IDLE = 0,
	RPG_FIDGET,
	RPG_RELOAD,		// to reload
	RPG_FIRE2,		// to empty
	RPG_HOLSTER1,	// loaded
	RPG_DRAW1,		// loaded
	RPG_HOLSTER2,	// unloaded
	RPG_DRAW_UL,	// unloaded
	RPG_IDLE_UL,	// unloaded idle
	RPG_FIDGET_UL,	// unloaded fidget
};

void EV_FireRpg( event_args_t *args )
{
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	
	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "weapons/rocketfire1.wav", 0.9, ATTN_NORM, 0, PITCH_NORM );
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_ITEM, "weapons/glauncher.wav", 0.7, ATTN_NORM, 0, PITCH_NORM );
	}

	//Only play the weapon anims if I shot it. 
	if ( EV_IsLocal( idx ) )
	{
		gEngfuncs.pEventAPI->EV_WeaponAnimation( RPG_FIRE2, 1 );
	
		V_PunchAxis( 0, -5.0 );
	}
}
//======================
//	     RPG END 
//======================

//======================
//	    EGON END 
//======================
enum egon_e {
	EGON_IDLE1 = 0,
	EGON_FIDGET1,
	EGON_ALTFIREON,
	EGON_ALTFIRECYCLE,
	EGON_ALTFIREOFF,
	EGON_FIRE1,
	EGON_FIRE2,
	EGON_FIRE3,
	EGON_FIRE4,
	EGON_DRAW,
	EGON_HOLSTER
};

int g_fireAnims1[] = { EGON_FIRE1, EGON_FIRE2, EGON_FIRE3, EGON_FIRE4 };
int g_fireAnims2[] = { EGON_ALTFIRECYCLE };

enum EGON_FIRESTATE { FIRE_OFF, FIRE_CHARGE };
enum EGON_FIREMODE { FIRE_NARROW, FIRE_WIDE};

#define	EGON_PRIMARY_VOLUME		450
#define EGON_BEAM_SPRITE		"sprites/xbeam1.spr"
#define EGON_FLARE_SPRITE		"sprites/XSpark1.spr"
#define EGON_SOUND_OFF			"weapons/egon_off1.wav"
#define EGON_SOUND_RUN			"weapons/egon_run3.wav"
#define EGON_SOUND_STARTUP		"weapons/egon_windup2.wav"

#define ARRAYSIZE(p)		(sizeof(p)/sizeof(p[0]))

BEAM *pBeam;
BEAM *pBeam2;


void EV_EgonFire( event_args_t *args )
{
	int idx, iFireState, iFireMode;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	iFireState = args->iparam1;
	iFireMode = args->iparam2;
	
	int iStartup = args->bparam1;

	int hasSpiralBeam;



	if(EASY_CVAR_GET(egonEffectsMode == 3)){
		//only the narrow beam gets it.
		hasSpiralBeam = (iFireMode == FIRE_NARROW);
	}else{
		//otherwise, alsways has it.
		hasSpiralBeam = TRUE;
	}

	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		if ( iStartup )
		{
			if ( iFireMode == FIRE_WIDE )
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.98, ATTN_NORM, 0, 125 );
			else
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, EGON_SOUND_STARTUP, 0.9, ATTN_NORM, 0, 100 );
		}
		else
		{
			if ( iFireMode == FIRE_WIDE )
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.98, ATTN_NORM, 0, 125 );
			else
				gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_STATIC, EGON_SOUND_RUN, 0.9, ATTN_NORM, 0, 100 );
		}
	}


	//easyPrintLine("EGONEVENT1 %d, %d, %d, %d, %d", iStartup==1, EV_IsLocal( idx ), !pBeam, !pBeam2, (int)cl_lw->value );

	//Only play the weapon anims if I shot it.

	if ( EV_IsLocal( idx ) ){
		
		//MODDD - if FIRE_NARROW, assume normal anim.  Otherwise, assume FIRE_WIDE anim instead.
		//MODDD - circumstances for using NARROW and WIDE swapped.
		if ( iFireMode == FIRE_NARROW ){
			//gEngfuncs.pEventAPI->EV_WeaponAnimation ( g_fireAnims1[ gEngfuncs.pfnRandomLong( 0, 3 ) ], 1 );
			//MODDD - anims 1 and 2 seem to be defunct.  Removing from random range   [0, 3] --> [2, 3]
			gEngfuncs.pEventAPI->EV_WeaponAnimation ( g_fireAnims1[ gEngfuncs.pfnRandomLong( 2, 3 ) ], 1 );
		}else{//wide
			gEngfuncs.pEventAPI->EV_WeaponAnimation ( g_fireAnims2[0], 1 );
		}
		
	}


	//hasSpiralBeam = FALSE;

	//if ( hasSpiralBeam ){
	if(1){
		//gets the spiral.


		if ( iStartup == 1 && EV_IsLocal( idx ) && !pBeam && !pBeam2 && cl_lw->value ) //Adrian: Added the cl_lw check for those lital people that hate weapon prediction.
		{
			vec3_t vecSrc, vecEnd, origin, angles, forward, right, up;
			pmtrace_t tr;

			cl_entity_t *pl = gEngfuncs.GetEntityByIndex( idx );

			//easyPrintLine("EGONEVENT2 %d", pl != 0 );
			if ( pl )
			{
				VectorCopy( gHUD.m_vecAngles, angles );
			
				AngleVectors( angles, forward, right, up );

				EV_GetGunPosition( args, vecSrc, pl->origin );

				VectorMA( vecSrc, 2048, forward, vecEnd );

				gEngfuncs.pEventAPI->EV_SetUpPlayerPrediction( false, true );	
				
				// Store off the old count
				gEngfuncs.pEventAPI->EV_PushPMStates();
			
				// Now add in all of the players.
				gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	

				gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
				gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecEnd, PM_STUDIO_BOX, -1, &tr );

				gEngfuncs.pEventAPI->EV_PopPMStates();

				int iBeamModelIndex = gEngfuncs.pEventAPI->EV_FindModelIndex( EGON_BEAM_SPRITE );

				float r = 50.0f;
				float g = 50.0f;
				float b = 125.0f;

				if ( IEngineStudio.IsHardware() )
				{
					r /= 100.0f;
					g /= 100.0f;
				}
				
				if(hasSpiralBeam){
					//spiral purple beam.
					pBeam = gEngfuncs.pEfxAPI->R_BeamEntPoint ( idx | 0x1000, tr.endpos, iBeamModelIndex, 99999, 3.5, 0.2, 0.7, 55, 0, 0, r, g, b );
			
					//easyPrintLine("EGONEVENT3 %d", pBeam != 0 );
					if ( pBeam )
						 pBeam->flags |= ( FBEAM_SINENOISE );

				}
 
				//straight purple beam.
				pBeam2 = gEngfuncs.pEfxAPI->R_BeamEntPoint ( idx | 0x1000, tr.endpos, iBeamModelIndex, 99999, 5.0, 0.08, 0.7, 25, 0, 0, r, g, b );
				
			}
		}



	
	}//END OF spiral beam check



}//END OF EV_EgonFire

void EV_EgonStop( event_args_t *args )
{
	//easyPrintLine("EGONEVENT STOP???");
	int idx;
	vec3_t origin;

	idx = args->entindex;
	VectorCopy ( args->origin, origin );

	gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_STATIC, EGON_SOUND_RUN );
	
	if ( args->iparam1 )
		 gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, EGON_SOUND_OFF, 0.98, ATTN_NORM, 0, 100 );

	if ( EV_IsLocal( idx ) ) 
	{
		if ( pBeam )
		{
			pBeam->die = 0.0;
			pBeam = NULL;
		}
			
		
		if ( pBeam2 )
		{
			pBeam2->die = 0.0;
			pBeam2 = NULL;
		}
	}
}
//======================
//	    EGON END 
//======================

//======================
//	   HORNET START
//======================
enum hgun_e {
	HGUN_IDLE1 = 0,
	HGUN_FIDGETSWAY,
	HGUN_FIDGETSHAKE,
	HGUN_DOWN,
	HGUN_UP,
	HGUN_SHOOT
};

void EV_HornetGunFire( event_args_t *args )
{
	int idx, iFireMode;
	vec3_t origin, angles, vecSrc, forward, right, up;

	idx = args->entindex;
	VectorCopy( args->origin, origin );
	VectorCopy( args->angles, angles );
	iFireMode = args->iparam1;

	//Only play the weapon anims if I shot it.
	if ( EV_IsLocal( idx ) )
	{
		V_PunchAxis( 0, gEngfuncs.pfnRandomLong ( 0, 2 ) );
		gEngfuncs.pEventAPI->EV_WeaponAnimation ( HGUN_SHOOT, 1 );
	}

	
	if(EASY_CVAR_GET(mutePlayerWeaponFire) != 1 ){
		switch ( gEngfuncs.pfnRandomLong ( 0 , 2 ) )
		{
			case 0:	gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "agrunt/ag_fire1.wav", 1, ATTN_NORM, 0, 100 );	break;
			case 1:	gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "agrunt/ag_fire2.wav", 1, ATTN_NORM, 0, 100 );	break;
			case 2:	gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_WEAPON, "agrunt/ag_fire3.wav", 1, ATTN_NORM, 0, 100 );	break;
		}
	}


}
//======================
//	   HORNET END
//======================

//======================
//	   TRIPMINE START
//======================
enum tripmine_e {
	TRIPMINE_IDLE1 = 0,
	TRIPMINE_IDLE2,
	TRIPMINE_ARM1,
	TRIPMINE_ARM2,
	TRIPMINE_FIDGET,
	TRIPMINE_HOLSTER,
	TRIPMINE_DRAW,
	TRIPMINE_WORLD,
	TRIPMINE_GROUND,
};

//We only check if it's possible to put a trip mine
//and if it is, then we play the animation. Server still places it.
void EV_TripmineFire( event_args_t *args )
{
	int idx;
	vec3_t vecSrc, angles, view_ofs, forward;
	pmtrace_t tr;

	idx = args->entindex;
	VectorCopy( args->origin, vecSrc );
	VectorCopy( args->angles, angles );

	AngleVectors ( angles, forward, NULL, NULL );
		
	if ( !EV_IsLocal ( idx ) )
		return;

	// Grab predicted result for local player
	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight( view_ofs );

	vecSrc = vecSrc + view_ofs;

	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc, vecSrc + forward * 128, PM_NORMAL, -1, &tr );

	//Hit something solid

	if ( tr.fraction < 1.0 ){
		 //MODDD - doing this instead. 
		//gEngfuncs.pEventAPI->EV_WeaponAnimation ( TRIPMINE_DRAW, 0 );
		//by the way, the "PLACE" animation is named "TRIPMINE_ARM2" here instead.
		//gEngfuncs.pEventAPI->EV_WeaponAnimation ( TRIPMINE_ARM2, 0 );
		//No, play it in "tripmine.cpp" instead for more control over doing 2 animations in succession: place, then draw.
	}


	gEngfuncs.pEventAPI->EV_PopPMStates();
}
//======================
//	   TRIPMINE END
//======================

//======================
//	   SQUEAK START
//======================
enum squeak_e {
	SQUEAK_IDLE1 = 0,
	SQUEAK_FIDGETFIT,
	SQUEAK_FIDGETNIP,
	SQUEAK_DOWN,
	SQUEAK_UP,
	SQUEAK_THROW
};

#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )



void EV_SnarkFire( event_args_t *args )
{
	int idx;
	vec3_t vecSrc, angles, view_ofs, forward;
	pmtrace_t tr;

	idx = args->entindex;
	VectorCopy( args->origin, vecSrc );
	VectorCopy( args->angles, angles );

	AngleVectors ( angles, forward, NULL, NULL );
		
	if ( !EV_IsLocal ( idx ) )
		return;
	
	if ( args->ducking )
		vecSrc = vecSrc - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	
	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc + forward * 20, vecSrc + forward * 64, PM_NORMAL, -1, &tr );

	//Find space to drop the thing.
	if ( tr.allsolid == 0 && tr.startsolid == 0 && tr.fraction > 0.25 )
		 gEngfuncs.pEventAPI->EV_WeaponAnimation ( SQUEAK_THROW, 0 );
	
	gEngfuncs.pEventAPI->EV_PopPMStates();
}
//======================
//	   SQUEAK END
//======================





//MODDD - NEW
//======================
//	   CHUMTOAD START
//=====================
enum chumtoadweapon_e {  //key: frames, FPS
	CHUMTOADWEAPON_IDLE1 = 0, //31, 16
	CHUMTOADWEAPON_FIDGETLICK, //31, 16
	CHUMTOADWEAPON_FIDGETCROAK, //51, 16
	CHUMTOADWEAPON_DOWN, //21, 16
	CHUMTOADWEAPON_UP, //36, 30
	CHUMTOADWEAPON_THROW, //36, 24

};


#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )



void EV_ChumToadFire( event_args_t *args )
{
	int idx;
	vec3_t vecSrc, angles, view_ofs, forward;
	pmtrace_t tr;

	idx = args->entindex;
	VectorCopy( args->origin, vecSrc );
	VectorCopy( args->angles, angles );

	AngleVectors ( angles, forward, NULL, NULL );
		
	if ( !EV_IsLocal ( idx ) )
		return;
	
	if ( args->ducking )
		vecSrc = vecSrc - ( VEC_HULL_MIN - VEC_DUCK_HULL_MIN );
	
	// Store off the old count
	gEngfuncs.pEventAPI->EV_PushPMStates();

	// Now add in all of the players.
	gEngfuncs.pEventAPI->EV_SetSolidPlayers ( idx - 1 );	
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
	gEngfuncs.pEventAPI->EV_PlayerTrace( vecSrc + forward * 20, vecSrc + forward * 64, PM_NORMAL, -1, &tr );

	//Find space to drop the thing.
	//if ( tr.allsolid == 0 && tr.startsolid == 0 && tr.fraction > 0.25 )
		 gEngfuncs.pEventAPI->EV_WeaponAnimation ( CHUMTOADWEAPON_THROW, 0 );
	
	gEngfuncs.pEventAPI->EV_PopPMStates();
}
//======================
//	   CHUMTOAD END
//======================







void EV_TrainPitchAdjust( event_args_t *args )
{
	int idx;
	vec3_t origin;

	unsigned short us_params;
	int noise;
	float m_flVolume;
	int pitch;
	int stop;
	
	char sz[ 256 ];

	idx = args->entindex;
	
	VectorCopy( args->origin, origin );

	us_params = (unsigned short)args->iparam1;
	stop	  = args->bparam1;

	m_flVolume	= (float)(us_params & 0x003f)/40.0;
	noise		= (int)(((us_params) >> 12 ) & 0x0007);
	pitch		= (int)( 10.0 * (float)( ( us_params >> 6 ) & 0x003f ) );

	switch ( noise )
	{
	case 1: strcpy( sz, "plats/ttrain1.wav"); break;
	case 2: strcpy( sz, "plats/ttrain2.wav"); break;
	case 3: strcpy( sz, "plats/ttrain3.wav"); break; 
	case 4: strcpy( sz, "plats/ttrain4.wav"); break;
	case 5: strcpy( sz, "plats/ttrain6.wav"); break;
	case 6: strcpy( sz, "plats/ttrain7.wav"); break;
	default:
		// no sound
		strcpy( sz, "" );
		return;
	}

	if ( stop )
	{
		gEngfuncs.pEventAPI->EV_StopSound( idx, CHAN_STATIC, sz );
	}
	else
	{
		gEngfuncs.pEventAPI->EV_PlaySound( idx, origin, CHAN_STATIC, sz, m_flVolume, ATTN_NORM, SND_CHANGE_PITCH, pitch );
	}
}

int EV_TFC_IsAllyTeam( int iTeam1, int iTeam2 )
{
	return 0;
}







const float matrix_rot_x[3][3] = {
	1, 0, 0,
	0, 0, -1,
	0, 1, 0
};

const float matrix_rot_y[3][3] = {
	0, 0, 1,
	0, 1, 0,
	-1, 0, 0
};

const float matrix_rot_z[3][3] = {
	0, -1, 0,
	1, 0, 0,
	0, 0, 1
};

void matrixMult(const Vector& m_1, const float m_2[3][3], Vector& v_out){

	v_out.x = m_1.x * m_2[0][0] + m_1.y * m_2[0][1] + m_1.z * m_2[0][2];
	v_out.y = m_1.x * m_2[1][0] + m_1.y * m_2[1][1] + m_1.z * m_2[1][2];
	v_out.z = m_1.x * m_2[2][0] + m_1.y * m_2[2][1] + m_1.z * m_2[2][2];

}//END OF matrixMult



//If this is called, make sure a "log" folder exists under the same folder as hl.exe, NOT the mod folder (Absolute Zero)!
void writeColorPickerChoices(void){
	int r, g, b;
	FILE* myFile;
	
	myFile = fopen("log/r.txt", "w");  //"a+t");
	if(myFile){
		for(int i = 0; i <= 255; i++){
			r = i;
			g = 0;
			b = 0;
			fprintf(myFile, "%3d, %3d, %3d ::: %3d\n", r, g, b, gEngfuncs.pEfxAPI->R_LookupColor( r, g, b ));
		}
		fclose(myFile);
	}
	myFile = fopen("log/g.txt", "w");  //"a+t");
	if(myFile){
		for(int i = 0; i <= 255; i++){
			r = 0;
			g = i;
			b = 0;
			fprintf(myFile, "%3d, %3d, %3d ::: %3d\n", r, g, b, gEngfuncs.pEfxAPI->R_LookupColor( r, g, b ));
		}
		fclose(myFile);
	}
	myFile = fopen("log/b.txt", "w");  //"a+t");
	if(myFile){
		for(int i = 0; i <= 255; i++){
			r = 0;
			g = 0;
			b = i;
			fprintf(myFile, "%3d, %3d, %3d ::: %3d\n", r, g, b, gEngfuncs.pEfxAPI->R_LookupColor( r, g, b ));
		}
		fclose(myFile);
	}
}//END OF wirteColorPickerChoices



































//0.27f;
#define TEST_PARTICLE_LIFE 0.18f;
//0.0167f;
#define TEST_PARTICLE_SPAWN_CYCLE_TIME 0.0f;

//MODDD - methods slightly modified from entity.cpp, found commented out.
void TEST_ParticleCallback( struct particle_s *particle, float frametime )
{
	int i;

			

	const float timeStart = particle->die - TEST_PARTICLE_LIFE;
	const float timeSinceStart = gEngfuncs.GetClientTime() - timeStart;


	int r;
	int g;
	int b;
	
	b = 0;

	//warp from that bright yellow (255, 255, 0) to a smokey gray (121, 121, 121);

	//how far along am I?
	const float timeFract = timeSinceStart / TEST_PARTICLE_LIFE;

	/*
	r = 255 + (121 - 255)*timeFract;
	g = 255 + (121 - 255)*timeFract;
	b = 0;
	*/
	
	

	/*

	r = 255 + (0 - 255)*timeFract;
	//g = 255 + (0 - 255)*timeFract;
	g = 255 + (0 - 255)*timeFract;
	b = 0;
	*/

	//Unreliable, ugh.
	//Just look at the palette itself and see if there's a row that looks good.
	//Indexes work as left to right, top to bottom. 16 per row, starting at 0.  So if the count goes over 16 (like 16 itself including #0),
	//that picks the next row's first color. Up to the last #255.
	//particle->color = 	gEngfuncs.pEfxAPI->R_LookupColor( r, g, b );

	particle->color = 96 + (int)ceil(15.0f*(1.0f - timeFract));



	//

	/*
	r = 255;
	g = 255;
	b = 0;
			

	p->color = 	gEngfuncs.pEfxAPI->R_LookupColor( r, g, b );
	//What is the point of R_GetPackedColor?? Just setting p->color above seems to work alone?
	gEngfuncs.pEfxAPI->R_GetPackedColor( &p->packedColor, p->color );
	*/



	for ( i = 0; i < 3; i++ )
	{
		particle->org[ i ] += particle->vel[ i ] * frametime;
	}
}









//cvar_t *color = NULL;
void TEST_Particles( const Vector& v_origin, const Vector& v_velocity )
{
	//static float lasttime;
	//float curtime;
	
	//curtime = gEngfuncs.GetClientTime();

	//if ( ( curtime - lasttime ) < 2.0 )
	//	return;

	//lasttime = curtime;

	// Create a few particles
	
	
	if(v_velocity.Length() > 0){
		particle_t *p;
		int i, j;
		//const int numParticles = 26;
		//const int numParticles = 32;
		const int numParticles = 24;
		
		//LETS TEST.
		Vector forward = v_velocity.Normalize();
		Vector right;
		Vector up;

		if(forward == Vector(0, 0, 1)){
			//matches the up vector exactly?  A cross product with 0, 0, 1 would not work then.  It would be (0, 0, 0), or no direction at all.
			//Just use fixed vectors for the rest for this special case.
			
			//My "right" can be one way floor-wise, my "up" is another floor-wise. X and Y respectively, either order.
			right = Vector(1, 0, 0);
			up = Vector (0, 1, 0);

		}else{
			//Determine what the right and up vectors are. This helps with the trigonometry to place points around me in a circle, equidistantly spaced in degrees.
			
			//matrixMult(forward, matrix_rot_z, right);
			//matrixMult(right, matrix_rot_y, right);
			
			right = CrossProduct(forward, Vector(0, 0, 1));

			//Yes, this produces a good relative up vector. It does feel just diabolical.
			up = CrossProduct(right, forward);

			//Normalize both.
			right = right.Normalize();
			up = up.Normalize();

		}

		//At this point it is established what vector goes forward, up, and right from that.  Up and right and all possible combinations of each
		//in a circular fashion form a plane for drawing circles around. I'm sure that makes perfect sense.


		
		for ( i = 0; i < numParticles; i++ ){
			int r, g, b;
			p = gEngfuncs.pEfxAPI->R_AllocParticle( TEST_ParticleCallback );
			if ( !p )
				break;




			//for ( j = 0; j < 3; j++ )
			//{
				//p->org[ j ] = v_origin[ j ] + gEngfuncs.pfnRandomFloat( -32.0, 32.0 );;
				//p->vel[ j ] = gEngfuncs.pfnRandomFloat( -100.0, 100.0 );
			//}

			//First of all, get whatever portion around the circle this is.
		

			//Each 1/#'th will be covered.
			//For instance, if numParticles were 12, it would be 1/12'th.  so 0/12, 1/12, 2/12, 3/12, ... 9/12, 10/12, 11/12.
			//Exclude the last whole one (12/12) since that wraps around to the start of the circle, redundant with 0/12.
			float circleRad = 2*M_PI * (((float)i) / ((float)numParticles));
		
			float circle_x = cos(circleRad);
			float circle_y = sin(circleRad);
			




			//p->org = v_origin + (circle_x * v_right) * 3 + (circle_y * v_up) * 3;
			//p->vel = (circle_x * v_right) + (circle_y * v_up);
			p->org = v_origin + (circle_x * right) * 2 + (circle_y * up) * 2;
			
			p->vel = (circle_x * right) * 68 + (circle_y * up) * 68;
			//p->vel = Vector(0, 0, 0);


			//2, 1, 3
			//

	/*
	  2,  3, -1    A
	x 0,  0,  1
	= 3, -2,  0    B

	   3, -2,  0    B
	x  2,  3, -1    A
	=  2,  3, 13    C
	*/




			//p->org = v_origin;
			//p->vel = Vector(1, 1, 1);
			//CrossProduct(v_velocity, Vector(0, 0, 1));




			r = 255;
			g = 255;  //why is green so gray looking??
			b = 0;

			//wtf?
			/*
			someBeam->r = colorReceive[0]/65025.0f;
			someBeam->g = colorReceive[1]/65025.0f;
			someBeam->b = colorReceive[2]/65025.0f;	
			*/

			//p->color = 	gEngfuncs.pEfxAPI->R_LookupColor( r, g, b );
			p->color = 96 + 15;
			
			//What is the point of R_GetPackedColor?? Just setting p->color above seems to work alone?
			gEngfuncs.pEfxAPI->R_GetPackedColor( &p->packedColor, p->color );


			// p->die is set to current time so all you have to do is add an additional time to it
			//p->die += 1.5;
			//nah...
			//p->die = gEngfuncs.GetClientTime() + 0.3;
			p->die += TEST_PARTICLE_LIFE;


		}//END OF for each particle to spawn


	}//END OF velocity length check




}//END OF TEST_Particles



void EV_imitation7_think ( struct tempent_s *ent, float frametime, float currenttime )
{
	if ( currenttime < ent->entity.baseline.fuser1 )
		return;


	if ( ent->entity.origin == ent->entity.attachment[0] ){
		//HAVE LENIENCY.  Wait for this to be frozen for 0.15 seconds, sheesh.
		if(ent->entity.baseline.fuser2 != -1){
			if(ent->entity.baseline.fuser2 < gEngfuncs.GetClientTime() ){
				ent->die = gEngfuncs.GetClientTime();
			}
		}else{
			//countdown...
			ent->entity.baseline.fuser2 = gEngfuncs.GetClientTime() + 0.3f;
		}

	}else{
		ent->entity.baseline.fuser2 = -1;  //not dying now.
    	VectorCopy ( ent->entity.origin, ent->entity.attachment[0] );
	}


	
	//gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, vec3_origin, 0.2, m_iGlow, kRenderGlow, kRenderFxNoDissipation, flDamage * n / 255.0, flDamage * n * 0.5 * 0.1, FTENT_FADEOUT );

				//vec3_t fwd;
				//VectorAdd( tr.endpos, tr.plane.normal, fwd );

				//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 3, 0.1, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
				//					255, 100 );
	
	/*
	int eckz = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/explode1.spr" );

	//not sure if "life" is necessary, seems to expire at the end of the last frame without any looping and/or cycle tags.  (framerate is 10, I assume,  10 frames per second:  9 frames, so  9 / 10 = 0.9 seconds to finish the anim.
	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( ent->entity.origin, vec3_origin, global2_rocketTrailAlphaScale, eckz, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 0.91f, FTENT_SPRANIMATE );
	//eh->fadeSpeed = 3.3f;  ???
	//eh->entity.curstate.scale = global2_rocketTrailAlphaScale;
	*/


	//Instead of the little explosion puffs, radiate particles.
	
	vec3_t vecSrc, angles, forward, right, up;
	
	//I... guess this is velocity.
	vec3_t velo = ent->entity.origin - ent->entity.prevstate.origin;

	VectorCopy( ent->entity.origin, vecSrc );
	
	/*
	//VectorCopy( ent->entity.angles, angles );  //or "ent->entity.baseline.angles" ?
	VectorCopy( ent->entity.baseline.angles, angles );
	AngleVectors ( angles, forward, right, up );
	*/

	TEST_Particles(vecSrc, velo);




	//TEST - real slow for now!
	ent->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + TEST_PARTICLE_SPAWN_CYCLE_TIME;
	//ent->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 1.0f;
}//END OF EV_imitation7_think


void EV_imitation7(event_args_t* args){
	int iEntIndex = args->iparam1;
	TEMPENTITY *pTrailSpawner = NULL;

	
	pTrailSpawner = gEngfuncs.pEfxAPI->CL_TempEntAllocNoModel ( args->origin );
	

	if ( pTrailSpawner != NULL){
	    pTrailSpawner->entity.baseline.fuser2 = -1; //if that is fine?
	    //pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT );
		pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT | FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_COLLIDEWORLD );

	    pTrailSpawner->clientIndex = iEntIndex;
	    pTrailSpawner->callback = &EV_imitation7_think;

	    //pTrailSpawner->entity.baseline.sequence = args->iparam2 + 68;


	   
	    pTrailSpawner->die = gEngfuncs.GetClientTime() + 10; // Just in case
	   
		//When is my first think cycle?
	    pTrailSpawner->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 0.02f;
	}


}//END OF EV_imitation7



















//MODDD - new, testing some DMC - quake stuff.

void EV_RocketTrailCallback ( struct tempent_s *ent, float frametime, float currenttime )
{
	if ( currenttime < ent->entity.baseline.fuser1 )
		return;

	/*
	if ( ent->entity.origin == ent->entity.attachment[0] )
		ent->die = gEngfuncs.GetClientTime();
	else
    	VectorCopy ( ent->entity.origin, ent->entity.attachment[0] );
	*/

	
	if ( ent->entity.origin == ent->entity.attachment[0] ){
		//HAVE LENIENCY.  Wait for this to be frozen for 0.15 seconds, sheesh.
		if(ent->entity.baseline.fuser2 != -1){
			if(ent->entity.baseline.fuser2 < gEngfuncs.GetClientTime() ){
				ent->die = gEngfuncs.GetClientTime();
			}
		}else{
			//countdown...
			ent->entity.baseline.fuser2 = gEngfuncs.GetClientTime() + 0.15;
		}

	}else{
		ent->entity.baseline.fuser2 = -1;  //not dying now.
    	VectorCopy ( ent->entity.origin, ent->entity.attachment[0] );
	}






	//Not set up to re-think. Or is that automatic..?
	//that is set ent->entity.baseline.fuser1 to some GetClientTime() + 0.03 or something.

	/*
	//Make the Rocket light up. ( And only rockets, no Grenades ).
	if ( ent->entity.baseline.sequence == 70 )
	{
		dlight_t *dl = gEngfuncs.pEfxAPI->CL_AllocDlight ( 0 );
		VectorCopy ( ent->entity.origin, dl->origin );

		dl->radius = 160;
		dl->dark = true;
		dl->die = gEngfuncs.GetClientTime() + 0.001; //Kill it right away
													 
		dl->color.r = 255;
		dl->color.g = 255;
		dl->color.b = 255;
	}
	*/
}








#define GRENADE_TRAIL 1
#define ROCKET_TRAIL 2




void EV_Trail_EngineChoice_Think ( struct tempent_s *ent, float frametime, float currenttime ){

	if ( currenttime < ent->entity.baseline.fuser1 )
		return;

	if ( ent->entity.origin == ent->entity.attachment[0] ){
		//HAVE LENIENCY.  Wait for this to be frozen for 0.15 seconds, sheesh.
		if(ent->entity.baseline.fuser2 != -1){
			if(ent->entity.baseline.fuser2 < gEngfuncs.GetClientTime() ){
				ent->die = gEngfuncs.GetClientTime();
			}
		}else{
			//countdown...
			ent->entity.baseline.fuser2 = gEngfuncs.GetClientTime() + 0.3f;
		}

	}else{
		ent->entity.baseline.fuser2 = -1;  //not dying now.
    	VectorCopy ( ent->entity.origin, ent->entity.attachment[0] );
	}
	


	gEngfuncs.pEfxAPI->R_RocketTrail (ent->entity.prevstate.origin, ent->entity.origin, (int)ent->entity.baseline.iuser1 );
	

	//TEST - real slow for now!
	ent->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 0;//+ 0.0167;
	//ent->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 1.0f;

	

}//END OF EV_imitation7





//Similar to EV_Trail but pick what trail # to use from the engine (quake particles?) for choices 0 to 7 inclusive in iParam2 (the second whole number one, typically after giving entity index)
void EV_Trail_EngineChoice( event_args_t *args){
	
	int iEntIndex = args->iparam1;
	TEMPENTITY *pTrailSpawner = NULL;

	
	pTrailSpawner = gEngfuncs.pEfxAPI->CL_TempEntAllocNoModel ( args->origin );
	
	

	if ( pTrailSpawner != NULL)
	{
	    pTrailSpawner->entity.baseline.fuser2 = -1; //if that is fine?

	    //pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT | FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD );
	    pTrailSpawner->flags |= ( FTENT_SPIRAL|FTENT_PLYRATTACHMENT |FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_COLLIDEWORLD );

	    if(args->iparam2 == 6){
		    //orange trail.
		    pTrailSpawner->flags |= FTENT_ORANGETRAIL;
	    }else{
		    //smoky?
		    pTrailSpawner->flags |= FTENT_SMOKETRAIL;
	    }

	    pTrailSpawner->callback = &EV_Trail_EngineChoice_Think;
	    pTrailSpawner->clientIndex = iEntIndex;

		//save me!
		pTrailSpawner->entity.baseline.iuser1 = args->iparam2;

	    //pTrailSpawner->entity.baseline.sequence = args->iparam2 + 68;

	    pTrailSpawner->die = gEngfuncs.GetClientTime() + 10; // Just in case
	    pTrailSpawner->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 0; //+ 0.0167;
	}
	

}//END OF EV_Trail_EngineChoice








//

//NOTE - I am  trail.sc
//ALSO - yes, trailTypeTest may be sent to me as iParam2 but it clearly isn't having an influence here in that case.
//See entitiy.cpp's place where it checks for trailTypeTest.  That is much more inclusive of any partical effect ever.
void EV_Trail (event_args_t *args)
{


	int iEntIndex = args->iparam1;
	TEMPENTITY *pTrailSpawner = NULL;

	
	pTrailSpawner = gEngfuncs.pEfxAPI->CL_TempEntAllocNoModel ( args->origin );
	
	
	//gEngfuncs.pEfxAPI->R_RocketTrail(0

	if ( pTrailSpawner != NULL)
	{
	    pTrailSpawner->entity.baseline.fuser2 = -1; //if that is fine?

	    //pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT | FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD );
	    pTrailSpawner->flags |= ( FTENT_SPIRAL|FTENT_PLYRATTACHMENT |FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_COLLIDEWORLD );

	    if(args->iparam2 == 6){
		    //orange trail.
		    pTrailSpawner->flags |= FTENT_ORANGETRAIL;
	    }else{
		    //smoky?
		    pTrailSpawner->flags |= FTENT_SMOKETRAIL;
	    }

	    pTrailSpawner->callback = &EV_RocketTrailCallback;
	    pTrailSpawner->clientIndex = iEntIndex;



	    /*
	    //???
	    if ( args->iparam2 == GRENADE_TRAIL )
            pTrailSpawner->entity.baseline.sequence = 69;
	    else if ( args->iparam2 == ROCKET_TRAIL )
		    pTrailSpawner->entity.baseline.sequence = 70;
		    */
	    //pTrailSpawner->entity.baseline.sequence = args->iparam2 + 68;

	    pTrailSpawner->die = gEngfuncs.GetClientTime() + 10; // Just in case
	    pTrailSpawner->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + 0.5; // Don't try to die till 500ms ahead
	}


}
















void EV_rocketAlphaTrailThink ( struct tempent_s *ent, float frametime, float currenttime )
{
	if ( currenttime < ent->entity.baseline.fuser1 )
		return;


	if ( ent->entity.origin == ent->entity.attachment[0] ){
		//HAVE LENIENCY.  Wait for this to be frozen for 0.15 seconds, sheesh.
		if(ent->entity.baseline.fuser2 != -1){
			if(ent->entity.baseline.fuser2 < gEngfuncs.GetClientTime() ){
				ent->die = gEngfuncs.GetClientTime();
			}
		}else{
			//countdown...
			ent->entity.baseline.fuser2 = gEngfuncs.GetClientTime() + 0.15;
		}

	}else{
		ent->entity.baseline.fuser2 = -1;  //not dying now.
    	VectorCopy ( ent->entity.origin, ent->entity.attachment[0] );
	}


	//gEngfuncs.pEfxAPI->R_TempSprite( tr.endpos, vec3_origin, 0.2, m_iGlow, kRenderGlow, kRenderFxNoDissipation, flDamage * n / 255.0, flDamage * n * 0.5 * 0.1, FTENT_FADEOUT );

				//vec3_t fwd;
				//VectorAdd( tr.endpos, tr.plane.normal, fwd );

				//gEngfuncs.pEfxAPI->R_Sprite_Trail( TE_SPRITETRAIL, tr.endpos, fwd, m_iBalls, 3, 0.1, gEngfuncs.pfnRandomFloat( 10, 20 ) / 100.0, 100,
				//					255, 100 );
	
	int eckz = gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/explode1.spr" );

	//not sure if "life" is necessary, seems to expire at the end of the last frame without any looping and/or cycle tags.  (framerate is 10, I assume,  10 frames per second:  9 frames, so  9 / 10 = 0.9 seconds to finish the anim.
	TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( ent->entity.origin, vec3_origin, global2_rocketTrailAlphaScale, eckz, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 0.91f, FTENT_SPRANIMATE );
	//eh->fadeSpeed = 3.3f;  ???
	//eh->entity.curstate.scale = global2_rocketTrailAlphaScale;

	ent->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + global2_rocketTrailAlphaInterval;
}



//NOTE - I am  trailra.sc
void EV_rocketAlphaTrail (event_args_t *args)
{


	//gEngfuncs.pEventAPI->EV_FindModelIndex( "sprites/hotglow.spr" );
	//TEMPENTITY* eh = gEngfuncs.pEfxAPI->R_TempSprite( *loc, rot, 0.12f, *sprite, kRenderGlow, kRenderFxNoDissipation, 250.0 / 255.0, 0.22f, FTENT_GRAVITY | FTENT_COLLIDEWORLD | FTENT_FADEOUT );
	

	int iEntIndex = args->iparam1;
	TEMPENTITY *pTrailSpawner = NULL;


	
	pTrailSpawner = gEngfuncs.pEfxAPI->CL_TempEntAllocNoModel ( args->origin );
	
	
	//gEngfuncs.pEfxAPI->R_RocketTrail(0

	if ( pTrailSpawner != NULL)
	{
	   pTrailSpawner->entity.baseline.fuser2 = -1; //if that is fine?
		
	   //pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT | FTENT_COLLIDEKILL | FTENT_CLIENTCUSTOM | FTENT_SMOKETRAIL | FTENT_COLLIDEWORLD );
	   pTrailSpawner->flags |= ( FTENT_PLYRATTACHMENT  | FTENT_CLIENTCUSTOM );


	   pTrailSpawner->callback = &EV_rocketAlphaTrailThink;
	   pTrailSpawner->clientIndex = iEntIndex;


	   //pTrailSpawner->entity.baseline.sequence = args->iparam2 + 68;


	   
	   pTrailSpawner->die = gEngfuncs.GetClientTime() + 10; // Just in case

	   pTrailSpawner->entity.baseline.fuser1 = gEngfuncs.GetClientTime() + global2_rocketTrailAlphaInterval; // Don't try to die till 500ms ahead
	}

}



















//MODDDMIRROR
//======================
//	   MIRROR START
//======================
void EV_Mirror( event_args_t *args ) 
{
	vec3_t org;
	bool bNew = true;

	VectorCopy(args->origin,org);
	float dist = (float)args->iparam1;
	int type = args->iparam2;
	int bEnabled = args->bparam1;

	//we have mirror
	if (gHUD.numMirrors)
	{
		for (int ic=0;ic<32;ic++)
		{
			if (gHUD.Mirrors[ic].origin[0] == org[0] && gHUD.Mirrors[ic].origin[1] == org[1] && gHUD.Mirrors[ic].origin[2] == org[2])
			{
				if (bEnabled && !gHUD.Mirrors[ic].enabled ) gHUD.numMirrors++;

		         		else if (!bEnabled && gHUD.Mirrors[ic].enabled ) gHUD.numMirrors--;

		         		gHUD.Mirrors[ic].enabled = bEnabled;
				bNew = false;
				break;
			}
		}	
	}

	if (bNew)
	{
		if (gHUD.numMirrors >= 32) easyPrintLine("ERROR: Can't register mirror, maximum 32 allowed!\n");

		else
         		{
			VectorCopy(org,gHUD.Mirrors[gHUD.numMirrors].origin);
	         		gHUD.Mirrors[gHUD.numMirrors].type = type;
	        		gHUD.Mirrors[gHUD.numMirrors].enabled = bEnabled;
	        		gHUD.Mirrors[gHUD.numMirrors].radius = dist;
	        		gHUD.numMirrors++;
		}
	}
}

//======================
//	   MIRROR END
//======================

