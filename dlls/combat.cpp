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
/*

===== combat.cpp ========================================================

  functions dealing with damage infliction & death

*/

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "soundent.h"
#include "decals.h"
#include "animation.h"
#include "weapons.h"
#include "func_break.h"

//MODDD - okay?
#include "squadmonster.h"
#include	"game.h"

//MODDD - necessary anymore?
#include "player.h"

//MODDD - needed for a spawnflag. OR scripted spawnflags could be moved to cbase.h to be available everywhere if needed, they are uniquely named anyways to avoid conflicts.
//        (in name, not necessarily in the bit occupied)
#include "scripted.h"


#include "custom_debug.h"




extern DLL_GLOBAL Vector		g_vecAttackDir;
extern DLL_GLOBAL int			g_iSkillLevel;

extern Vector VecBModelOrigin( entvars_t* pevBModel );
//extern entvars_t *g_pevLastInflictor;




extern float global_cheat_iwantguts;

extern float globalPSEUDO_canApplyGermanCensorship;
extern BOOL globalPSEUDO_germanModel_hgibFound;



extern GibInfo_t aryGibInfo[];





//MODDD - hidden cheat.  Not a CVar, so that it can't be seen by console's autocomplete
float cheat_barnacleEatsEverything = 0;








extern float global_germanCensorship;
extern float global_muteRicochetSound;
extern float global_bulletholeAlertRange;
extern float global_nothingHurts;

extern float global_timedDamageAffectsMonsters;
extern float global_bulletHoleAlertPrintout;
extern float global_bulletholeAlertStukaOnly;

EASY_CVAR_EXTERN(playerBulletHitEffectForceServer)

EASY_CVAR_EXTERN(baseEntityDamagePushNormalMulti)
EASY_CVAR_EXTERN(baseEntityDamagePushVerticalBoost)
EASY_CVAR_EXTERN(baseEntityDamagePushVerticalMulti)
EASY_CVAR_EXTERN(baseEntityDamagePushVerticalMinimum)

EASY_CVAR_EXTERN(RadiusDamageDrawDebug)
EASY_CVAR_EXTERN(AlienRadiationImmunity)

EASY_CVAR_EXTERN(germanRobotGibs)
EASY_CVAR_EXTERN(germanRobotBleedsOil)
EASY_CVAR_EXTERN(germanRobotDamageDecal)
EASY_CVAR_EXTERN(germanRobotGibsDecal)
EASY_CVAR_EXTERN(monsterFadeOutRate)

EASY_CVAR_EXTERN(playerWeaponTracerMode)








void EstablishGutLoverGib(CGib* pGib, entvars_t* pevVictim, entvars_t* pevPlayer, BOOL isHead);
void EstablishGutLoverGib(CGib* pGib, entvars_t* pevVictim, const Vector gibSpawnOrigin, entvars_t* pevPlayer, BOOL isHead);




































void EstablishGutLoverGib(CGib* pGib, entvars_t* pevVictim, entvars_t* pevPlayer, BOOL isHead){
	if(!pevVictim)return;
	EstablishGutLoverGib(pGib, pevVictim, pevVictim->origin + pevVictim->view_ofs, pevPlayer, isHead);
}


void EstablishGutLoverGib(CGib* pGib, entvars_t* pevVictim, const Vector gibSpawnOrigin, entvars_t* pevPlayer, BOOL isHead){
	

	//can't do this without the victim.
	if(!pevVictim)return;


	if(isHead == TRUE){
		pGib->pev->origin = gibSpawnOrigin;
		
	}else{
		// spawn the gib somewhere in the monster's bounding volume
		pGib->pev->origin.x = pevVictim->absmin.x + pevVictim->size.x * (RANDOM_FLOAT ( 0 , 1 ) );
		pGib->pev->origin.y = pevVictim->absmin.y + pevVictim->size.y * (RANDOM_FLOAT ( 0 , 1 ) );
		pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (RANDOM_FLOAT ( 0 , 1 ) ) + 1;	// absmin.z is in the floor because the engine subtracts 1 to enlarge the box

	}


	Vector dest;

	if(isHead == TRUE){
		dest = pevPlayer->origin + pevPlayer->view_ofs + Vector(RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 1));
	}else{
		dest = pevPlayer->origin + pevPlayer->view_ofs + Vector(RANDOM_FLOAT(-36, 36), RANDOM_FLOAT(-36, 36), RANDOM_FLOAT(-9, 2));
	}


	Vector distVector = ( dest ) - pGib->pev->origin;

	Vector distVector2D = Vector(distVector.x, distVector.y, 0);

	Vector towardsPlayer = distVector.Normalize();
	Vector towardsPlayer2D =  distVector2D.Normalize();
				
	float distFloorwise = distVector.Length2D();
	float distVertical = distVector.z;

	//angle...
	/*
	float ang = 0;
	if(distVertical ==0){
		ang = 90 *(M_PI / 180.0);
	}else{
		ang = atan(distVertical / distFloorwise);
	}
	*/

	//velocity must be at least X.
	float velocitySpeed = max(400, distFloorwise);

	if(isHead){
		pGib->pev->origin = pGib->pev->origin + (towardsPlayer) * 8;

		//little faster.
		velocitySpeed *= 1.11f;
	}


	//float velocityXComp = cos(ang) * velocity;
	//float velocityYComp = sin(ang) * velocity;

	//Vector velocityFloorwise = velocity * ;

	//110 / 250

	float timeToReachDest = distFloorwise / velocitySpeed;

	//grav?   sv_grav?
				
				
	float gravity = g_psv_gravity->value;
	//easyForcePrintLine("???GGG %.2f", gravity);
	Vector velocityFinal = towardsPlayer2D * velocitySpeed;
	float velocityVertical = (distVertical + 0.5 * gravity * pow(timeToReachDest, 2.0f ) ) / (timeToReachDest);
	
	//easyForcePrintLine("WHYYYYYY %.2f :: %.2f %.2f %.2f   %.2f %.2f", velocityVertical, distVertical, gravity, timeToReachDest, towardsPlayer.x, towardsPlayer.y);

	velocityFinal.z = velocityVertical * 1.0f;
	velocityFinal.x *= 1.0;
	velocityFinal.y *= 1.0;

	//pGib->pev->velocity.z += 100;


	//Vector tempp = towardsPlayer2D * velocity;

	pGib->pev->velocity = velocityFinal;
}




















// HACKHACK -- The gib velocity equations don't work
void CGib :: LimitVelocity( void )
{
	float length = pev->velocity.Length();

	// ceiling at 1500.  The gib velocity equation is not bounded properly.  Rather than tune it
	// in 3 separate places again, I'll just limit it here.
	if ( length > 1500.0 )
		pev->velocity = pev->velocity.Normalize() * 1500;		// This should really be sv_maxvelocity * 0.75 or something
}


void CGib :: SpawnStickyGibs( entvars_t *pevVictim, Vector vecOrigin, int cGibs ){
	CGib::SpawnStickyGibs(pevVictim, vecOrigin, cGibs, TRUE);
}
void CGib :: SpawnStickyGibs( entvars_t *pevVictim, Vector vecOrigin, int cGibs, BOOL spawnsDecal )
{
	int i;

	//if ( g_Language == LANGUAGE_GERMAN )
	if(global_germanCensorship == 1)
	{
		// no sticky gibs in germany right now!
		//MODDD TODO - above comment found as-is.  Can re-enable and just use german gibs instead.
		//             ...actually, it seems "SpawnStikcyGibs" is never called as-is.  Huh.
		return; 
	}

	for ( i = 0 ; i < cGibs ; i++ )
	{
		CGib *pGib = GetClassPtr( (CGib *)NULL );

		pGib->Spawn( "models/stickygib.mdl", spawnsDecal );
		pGib->pev->body = RANDOM_LONG(0,2);

		if ( pevVictim )
		{
			
			/*
			pGib->pev->origin.x = pevVictim->absmin.x + pevVictim->size.x * (RANDOM_FLOAT ( 0 , 1 ) );
			pGib->pev->origin.y = pevVictim->absmin.y + pevVictim->size.y * (RANDOM_FLOAT ( 0 , 1 ) );
			pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (RANDOM_FLOAT ( 0 , 1 ) );
			*/

			/*
			pGib->pev->origin.x = pevVictim->absmin.x + pevVictim->size.x * (0.5 );
			pGib->pev->origin.y = pevVictim->absmin.y + pevVictim->size.y * (0.5 );
			pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (0.5 ) + 1;	// absmin.z is in the floor because the engine subtracts 1 to enlarge the box
			*/

			pGib->pev->origin.x = vecOrigin.x;
			pGib->pev->origin.y = vecOrigin.y;
			pGib->pev->origin.z = vecOrigin.z;




			edict_t		*pentPlayer;
			if ( global_cheat_iwantguts && ((pentPlayer = FIND_CLIENT_IN_PVS( pGib->edict() )) != NULL)  )
			{
				// 5% chance head will be thrown at player's face.
				entvars_t	*pevPlayer;
				pevPlayer = VARS( pentPlayer );
				/*
				pGib->pev->velocity = ( ( pevPlayer->origin + pevPlayer->view_ofs ) - pGib->pev->origin ).Normalize() ;  //* 300
				
				// mix in some noise
				pGib->pev->velocity.x += RANDOM_FLOAT ( -0.09, 0.09 );
				pGib->pev->velocity.y += RANDOM_FLOAT ( -0.09, 0.09 );
				pGib->pev->velocity.z += RANDOM_FLOAT ( -0.09, 0.09 );
				
				pGib->pev->velocity = pGib->pev->velocity * RANDOM_FLOAT ( 300, 320 );

				//cheat the Z pos a bit..
				//pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (RANDOM_FLOAT ( 0.4 , 0.8 ) ) + 1;
				pGib->pev->origin.z += 20;

				pGib->pev->velocity.z += 100;

				//this overrides the usual "velocity" from above the new if-statement (cheat_iwantguts), to instead be towards the player camera.
				*/



				EstablishGutLoverGib(pGib, pevVictim, pevPlayer, FALSE);

				//pGib->pev->solid = SOLID_BBOX;
				//UTIL_SetSize ( pGib->pev, Vector( 0 , 0 , 0 ), Vector ( 0, 0, 0 ) );



			}else{
				//how it usually goes.
				pGib->pev->origin.x = vecOrigin.x + RANDOM_FLOAT( -3, 3 );
				pGib->pev->origin.y = vecOrigin.y + RANDOM_FLOAT( -3, 3 );
				pGib->pev->origin.z = vecOrigin.z + RANDOM_FLOAT( -3, 3 );

				// make the gib fly away from the attack vector
				pGib->pev->velocity = g_vecAttackDir * -1;
				// mix in some noise
			
				pGib->pev->velocity.x += RANDOM_FLOAT ( -0.15, 0.15 );
				pGib->pev->velocity.y += RANDOM_FLOAT ( -0.15, 0.15 );
				pGib->pev->velocity.z += RANDOM_FLOAT ( -0.15, 0.15 );


				pGib->pev->velocity = pGib->pev->velocity * 900;
			}


			/*
			if ( global_cheat_iwantguts )
			{
				edict_t		*pentPlayer = FIND_CLIENT_IN_PVS( pGib->edict() );
				if(pentPlayer != NULL){
					// 5% chance head will be thrown at player's face.
					entvars_t	*pevPlayer;

					pevPlayer = VARS( pentPlayer );
					pGib->pev->velocity = ( ( pevPlayer->origin + pevPlayer->view_ofs ) - pGib->pev->origin ).Normalize() * 300;
					pGib->pev->velocity.z += 100;

					//this overrides the usual "velocity" from above the new if-statement (cheat_iwantguts), to instead be towards the player camera.
				}
			}
			*/














			pGib->pev->avelocity.x = RANDOM_FLOAT ( 250, 400 );
			pGib->pev->avelocity.y = RANDOM_FLOAT ( 250, 400 );

			// copy owner's blood color
			pGib->m_bloodColor = (CBaseEntity::Instance(pevVictim))->BloodColor();
		
			
			//DONT THROW OFF THE EMPEROR'S GROVE
			if(global_cheat_iwantguts < 1){
				if ( pevVictim->health > -50)
				{
					pGib->pev->velocity = pGib->pev->velocity * 0.7;
				}
				else if ( pevVictim->health > -200)
				{
					pGib->pev->velocity = pGib->pev->velocity * 2;
				}
				else
				{
					pGib->pev->velocity = pGib->pev->velocity * 4;
				}
			}
			
			pGib->pev->movetype = MOVETYPE_TOSS;
			pGib->pev->solid = SOLID_BBOX;
			UTIL_SetSize ( pGib->pev, Vector ( 0, 0 ,0 ), Vector ( 0, 0, 0 ) );
			pGib->SetTouch ( &CGib::StickyGibTouch );
			pGib->SetThink (NULL);
		}
		pGib->LimitVelocity();
	}
}






void CGib :: SpawnHeadGib( entvars_t *pevVictim )
{
	if(!pevVictim)return;
	SpawnHeadGib(pevVictim, pevVictim->origin + pevVictim->view_ofs, TRUE);
}
void CGib::SpawnHeadGib( entvars_t *pevVictim, BOOL spawnDecals )
{
	if(!pevVictim)return;
	SpawnHeadGib(pevVictim, pevVictim->origin + pevVictim->view_ofs, spawnDecals);
}
void CGib::SpawnHeadGib( entvars_t *pevVictim, const Vector gibSpawnOrigin )
{
	SpawnHeadGib(pevVictim, gibSpawnOrigin, TRUE);
}
void CGib::SpawnHeadGib( entvars_t *pevVictim, const Vector gibSpawnOrigin, BOOL spawnDecals )
{

	//MODDD - is this check ok?  Otherwise the spawned head gib doesn't get a location.
	if(!pevVictim)return;


	CGib *pGib = GetClassPtr( (CGib *)NULL );

	
	//if ( g_Language == LANGUAGE_GERMAN )
	if( global_germanCensorship == FALSE ){
		pGib->Spawn( "models/hgibs.mdl", spawnDecals );// throw one head
		pGib->pev->body = 0;
	}else if(getGermanModelsAllowed() && globalPSEUDO_germanModel_hgibFound){  //if(global_tryLoadGermanGibs == 1){
		//Just do this.
		pGib->Spawn( aryGibInfo[GIB_GERMAN_ID].modelPath, spawnDecals);
		pGib->pev->body = RANDOM_LONG( aryGibInfo[GIB_GERMAN_ID].bodyMin, aryGibInfo[GIB_GERMAN_ID].bodyMax  );

	}else{
		//give up, no gib.
		return;
	}

	if ( pevVictim )
	{
		pGib->pev->origin = gibSpawnOrigin;
		
		edict_t		*pentPlayer = FIND_CLIENT_IN_PVS( pGib->edict() );
		
		if ( (RANDOM_LONG ( 0, 100 ) <= 5 || (global_cheat_iwantguts >= 1 ) ) && pentPlayer )
		{
				// 5% chance head will be thrown at player's face.
				entvars_t	*pevPlayer;
				pevPlayer = VARS( pentPlayer );
				
			if(global_cheat_iwantguts == 0){
				//ordinary.
				pGib->pev->velocity = ( ( pevPlayer->origin + pevPlayer->view_ofs ) - pGib->pev->origin ).Normalize() * 300;
				pGib->pev->velocity.z += 100;
			}else{
				EstablishGutLoverGib(pGib, pevVictim, gibSpawnOrigin, pevPlayer, TRUE);

			}

		}
		else
		{
			pGib->pev->velocity = Vector (RANDOM_FLOAT(-100,100), RANDOM_FLOAT(-100,100), RANDOM_FLOAT(200,300));
		}


		pGib->pev->avelocity.x = RANDOM_FLOAT ( 100, 200 );
		pGib->pev->avelocity.y = RANDOM_FLOAT ( 100, 300 );

		// copy owner's blood color
		pGib->m_bloodColor = (CBaseEntity::Instance(pevVictim))->BloodColor();
	

		//DONT THROW OFF THE EMPEROR'S GROVE
		if(global_cheat_iwantguts < 1){
			if ( pevVictim->health > -50)
			{
				pGib->pev->velocity = pGib->pev->velocity * 0.7;
			}
			else if ( pevVictim->health > -200)
			{
				pGib->pev->velocity = pGib->pev->velocity * 2;
			}
			else
			{
				pGib->pev->velocity = pGib->pev->velocity * 4;
			}
		}
	}
	pGib->LimitVelocity();
}






void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, int argSpawnGibID){
	//re-route to below to determine from the usual factors (violence CVar, is human, etc.)
	CGib::SpawnRandomGibs(pevVictim, cGibs, argSpawnGibID, TRUE);

}

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, int argSpawnGibID, BOOL spawnDecals){
	
	
	/*
		//if ( g_Language == LANGUAGE_GERMAN )
		if(global_germanCensorship == FALSE  ){
			if ( human )
			{
				// human pieces
				pGib->Spawn( "models/hgibs.mdl", spawnDecals );
				pGib->pev->body = RANDOM_LONG(1,HUMAN_GIB_COUNT-1);// start at one to avoid throwing random amounts of skulls (0th gib)
			}
			else
			{
				// aliens
				pGib->Spawn( "models/agibs.mdl", spawnDecals );
				pGib->pev->body = RANDOM_LONG(0,ALIEN_GIB_COUNT-1);
			}
		}else if( getGermanModelsAllowed() && globalPSEUDO_germanModel_hgibFound){   //if(global_tryLoadGermanGibs == 1){
			//german censorship replaces human gibs with gears.
			if ( human )
			{
				//pGib->Spawn( "models/germangibs.mdl" );
				pGib->Spawn( GERMAN_GIB_PATH, spawnDecals );
				//pGib->pev->body = RANDOM_LONG(0,GERMAN_GIB_COUNT-1);
				//NO, MORE FLEXIBLE (note that "getNumberOfBodyParts returns an inclusive maximum):
				pGib->pev->body = RANDOM_LONG(0,pGib->getNumberOfBodyParts() );
			}
			else
			{
				// aliens
				pGib->Spawn( "models/agibs.mdl", spawnDecals );
				pGib->pev->body = RANDOM_LONG(0,ALIEN_GIB_COUNT-1);
			}

		}else{
			//give up, no gib.
			return;
		}
	*/



	/*
	if ( argSpawnGibID )
	{
		// human pieces
		SpawnRandomGibs(pevVictim, cGibs, argSpawnGibID, spawnDecals, aryGibInfo[GIB_HUMAN_ID]);// start at one to avoid throwing random amounts of skulls (0th gib)
	}
	else
	{
		// aliens
		SpawnRandomGibs(pevVictim, cGibs, argSpawnGibID, spawnDecals, aryGibInfo[GIB_ALIEN_ID]);// start at one to avoid throwing random amounts of skulls (0th gib)
	}
	*/


	if(argSpawnGibID == GIB_DUMMY_ID){
		//This is the dummy ID. Don't do anything, it is empty and signifies blocking a GIB call.
		return;
	}


	//SpawnRandomGibs(pevVictim, cGibs, aryGibInfo[argSpawnGibID], spawnDecals);
	const GibInfo_t& gibInfoChoice = aryGibInfo[argSpawnGibID];
	CGib::SpawnRandomGibs(pevVictim, cGibs, gibInfoChoice.modelPath, gibInfoChoice.bodyMin, gibInfoChoice.bodyMax, spawnDecals, gibInfoChoice.bloodColor);






	/*

	//if ( g_Language == LANGUAGE_GERMAN )
	if(global_germanCensorship == FALSE  ){
		if ( human )
		{
			// human pieces
			SpawnRandomGibs(pevVictim, cGibs, human, spawnDecals, aryGibInfo[GIB_HUMAN_ID]);// start at one to avoid throwing random amounts of skulls (0th gib)
		}
		else
		{
			// aliens
			SpawnRandomGibs(pevVictim, cGibs, human, spawnDecals, aryGibInfo[GIB_ALIEN_ID]);// start at one to avoid throwing random amounts of skulls (0th gib)
		}
	}else{   //if(global_tryLoadGermanGibs == 1){
		//german censorship replaces human gibs with gears.
		if ( human)
		{

			if(getGermanModelsAllowed() && EASY_CVAR_GET(germanRobotGibs)==1 && globalPSEUDO_germanModel_hgibFound){
				// gears instead, they are now robots.
				SpawnRandomGibs(pevVictim, cGibs, human, spawnDecals, aryGibInfo[GIB_GERMAN_ID]);
			}
		}
		else
		{
			// aliens
			SpawnRandomGibs(pevVictim, cGibs, human, spawnDecals, aryGibInfo[GIB_ALIEN_ID]);
		}

	}
	*/


}//END OF SpawnRandomGibs

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, int argSpawnGibID, BOOL spawnDecals, int argBloodColor){
	if(argSpawnGibID == GIB_DUMMY_ID){
		//This is the dummy ID. Don't do anything, it is empty and signifies blocking a GIB call.
		return;
	}
	const GibInfo_t& gibInfoChoice = aryGibInfo[argSpawnGibID];
	//Since we provided a blood color, force this insetad of the one given by this gibInfo choice.
	CGib::SpawnRandomGibs(pevVictim, cGibs, gibInfoChoice.modelPath, gibInfoChoice.bodyMin, gibInfoChoice.bodyMax, spawnDecals, argBloodColor);
}


void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const GibInfo_t& gibInfoChoice){
	CGib::SpawnRandomGibs(pevVictim, cGibs, gibInfoChoice.modelPath, gibInfoChoice.bodyMin, gibInfoChoice.bodyMax, TRUE, gibInfoChoice.bloodColor);
}

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const GibInfo_t& gibInfoChoice, BOOL spawnDecals){
	//goes straight to the final method. This lets the user provide just a gibInfo reference to work off of.
	CGib::SpawnRandomGibs(pevVictim, cGibs, gibInfoChoice.modelPath, gibInfoChoice.bodyMin, gibInfoChoice.bodyMax, spawnDecals, gibInfoChoice.bloodColor );
}


void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const GibInfo_t& gibInfoChoice, BOOL spawnDecals, int argBloodColor){
	//Provided a blood color? Use that instead of the one that comes with this gibInfo choice.
	CGib::SpawnRandomGibs(pevVictim, cGibs, gibInfoChoice.modelPath, gibInfoChoice.bodyMin, gibInfoChoice.bodyMax, spawnDecals, argBloodColor );
}

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const char* argGibPath, int gibBodyMin, int gibBodyMax){
	CGib::SpawnRandomGibs(pevVictim, cGibs, argGibPath, gibBodyMin, gibBodyMax, TRUE, (CBaseEntity::Instance(pevVictim))->BloodColor() );
}

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const char* argGibPath, int gibBodyMin, int gibBodyMax, BOOL spawnDecals){
	CGib::SpawnRandomGibs(pevVictim, cGibs, argGibPath, gibBodyMin, gibBodyMax, spawnDecals, (CBaseEntity::Instance(pevVictim))->BloodColor() );
}

void CGib::SpawnRandomGibs(entvars_t* pevVictim, int cGibs, const char* argGibPath, int gibBodyMin, int gibBodyMax, BOOL spawnDecals, int argBloodColor){
	


	int cSplat;


	for ( cSplat = 0 ; cSplat < cGibs ; cSplat++ )
	{
		CGib *pGib = GetClassPtr( (CGib *)NULL );

		pGib->Spawn( argGibPath, spawnDecals );
		pGib->pev->body = RANDOM_LONG(gibBodyMin,gibBodyMax);// start at one to avoid throwing random amounts of skulls (0th gib)
			


		if ( pevVictim )
		{

			
			pGib->pev->origin.x = pevVictim->absmin.x + pevVictim->size.x * (0.5 );
			pGib->pev->origin.y = pevVictim->absmin.y + pevVictim->size.y * (0.5 );
			pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (0.5 ) + 1;	// absmin.z is in the floor because the engine subtracts 1 to enlarge the box
			
			

			edict_t		*pentPlayer;
			if ( global_cheat_iwantguts && ((pentPlayer = FIND_CLIENT_IN_PVS( pGib->edict() )) != NULL)  )
			{
				// 5% chance head will be thrown at player's face.
				entvars_t	*pevPlayer;
				pevPlayer = VARS( pentPlayer );
				/*
				pGib->pev->velocity = ( ( pevPlayer->origin + pevPlayer->view_ofs ) - pGib->pev->origin ).Normalize() ;  //* 300
				
				// mix in some noise
				pGib->pev->velocity.x += RANDOM_FLOAT ( -0.06, 0.06 );
				pGib->pev->velocity.y += RANDOM_FLOAT ( -0.06, 0.06 );
				pGib->pev->velocity.z += RANDOM_FLOAT ( -0.06, 0.06 );
				
				float pickedLength = RANDOM_FLOAT ( 300, 320 );
				pGib->pev->velocity = pGib->pev->velocity * pickedLength;

				//cheat the Z pos a bit..
				pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (RANDOM_FLOAT ( 0.3 , 0.65 ) ) + 1;
				//pGib->pev->origin.z += 20;
				*/

				EstablishGutLoverGib(pGib, pevVictim, pevPlayer, FALSE);


			}else{
				//how it usually goes.
				pGib->pev->origin.x = pevVictim->absmin.x + pevVictim->size.x * (RANDOM_FLOAT ( 0 , 1 ) );
				pGib->pev->origin.y = pevVictim->absmin.y + pevVictim->size.y * (RANDOM_FLOAT ( 0 , 1 ) );
				pGib->pev->origin.z = pevVictim->absmin.z + pevVictim->size.z * (RANDOM_FLOAT ( 0 , 1 ) ) + 1;	// absmin.z is in the floor because the engine subtracts 1 to enlarge the box
			

				// make the gib fly away from the attack vector
				pGib->pev->velocity = g_vecAttackDir * -1;
			
				// mix in some noise
				pGib->pev->velocity.x += RANDOM_FLOAT ( -0.25, 0.25 );
				pGib->pev->velocity.y += RANDOM_FLOAT ( -0.25, 0.25 );
				pGib->pev->velocity.z += RANDOM_FLOAT ( -0.25, 0.25 );

				pGib->pev->velocity = pGib->pev->velocity * RANDOM_FLOAT ( 300, 400 );
			}



			pGib->pev->avelocity.x = RANDOM_FLOAT ( 100, 200 );
			pGib->pev->avelocity.y = RANDOM_FLOAT ( 100, 300 );

			// copy owner's blood color.
			//MODDD NOTE - only for decals it looks  like.
			pGib->m_bloodColor = argBloodColor;
			//pGib->m_bloodColor = (CBaseEntity::Instance(pevVictim))->BloodColor();

			
			//DONT THROW OFF THE EMPEROR'S GROVE
			if(global_cheat_iwantguts < 1){
				if ( pevVictim->health > -50)
				{
					pGib->pev->velocity = pGib->pev->velocity * 0.7;
				}
				else if ( pevVictim->health > -200)
				{
					pGib->pev->velocity = pGib->pev->velocity * 2;
				}
				else
				{
					pGib->pev->velocity = pGib->pev->velocity * 4;
				}
			}

			pGib->pev->solid = SOLID_BBOX;
			UTIL_SetSize ( pGib->pev, Vector( 0 , 0 , 0 ), Vector ( 0, 0, 0 ) );
		}
		pGib->LimitVelocity();
	}
}//END OF SpawnRandomGibs



float CGib::massInfluence(void){
	return 0.11f;
}//END OF massInfluence




GENERATE_TRACEATTACK_IMPLEMENTATION_DUMMY(CGib)
GENERATE_TAKEDAMAGE_IMPLEMENTATION_DUMMY(CGib)









BOOL CBaseMonster :: HasHumanGibs( void )
{
	int myClass = Classify();

	//MODDD NOTICE - this is before the "IsOrganic()" check was made.
	//This will still catch robot replacement models, but that is fine, they are handled in GibMonster under a HasHumanGibs check.
	if ( myClass == CLASS_HUMAN_MILITARY ||
		 myClass == CLASS_PLAYER_ALLY	||
		 myClass == CLASS_HUMAN_PASSIVE  ||
		 myClass == CLASS_PLAYER )

		 return TRUE;

	return FALSE;
}


BOOL CBaseMonster :: HasAlienGibs( void )
{
	int myClass = Classify();

	if ( myClass == CLASS_ALIEN_MILITARY ||
		 myClass == CLASS_ALIEN_MONSTER	||
		 myClass == CLASS_ALIEN_PASSIVE  ||
		 myClass == CLASS_INSECT  ||
		 myClass == CLASS_ALIEN_PREDATOR  ||
		 myClass == CLASS_ALIEN_PREY )

		 return TRUE;

	return FALSE;
}


void CBaseMonster::FadeMonster( void )
{
	StopAnimation();
	pev->velocity = g_vecZero;
	pev->movetype = MOVETYPE_NONE;
	pev->avelocity = g_vecZero;
	pev->animtime = gpGlobals->time;
	pev->effects |= EF_NOINTERP;
	SUB_StartFadeOut();
}


//=========================================================
// GibMonster - create some gore and get rid of a monster's
// model.
//=========================================================




BOOL CBaseMonster::DetermineGibHeadBlock(void){
	//by default, there isn't a case to block head generation. This does not guarantee a head gib is generated for all monsters.
	//Only enforces a block (restriction) on spawning a head if this returns TRUE. Such as a headless hgrunt corpse gibbing: no head to give.
	return FALSE;
}









//MODDD - merging CallGibMonster and GibMonster into one GibMonster method. No need for these to be separate.






//Same as "CallGibMonster", now GibMonster, but always just instantly deletes and does some things out of courtesy... if this even matters.
void CBaseMonster::cleanDelete(){
	
	//???
	BOOL fade = FALSE;

	pev->takedamage = DAMAGE_NO;
	pev->solid = SOLID_NOT;// do something with the body. while monster blows up
	
	pev->effects = EF_NODRAW; // make the model invisible
	if ( ShouldFadeOnDeath() && !fade )
		UTIL_Remove(this);
}




//This is some basic script for monsters to gib. It assumes any monsters that fall under the HasHumanGibs() check will spawn blood red gibs,
//classes fall under the HasAlienGibs() check will spawn yellow gibs.
//This also checks for robot-replaced monsters in german censorship (still fall under the HasHumanGibs() check) and lets them spawn
//metal, gear, etc. gibs if allowed instead.
//In a particular monster, override GibMonster to give it its own specific gibbing behavior.
//parameter: BOOL fGibSpawnsDecal
GENERATE_GIBMONSTER_IMPLEMENTATION(CBaseMonster)
{

	BOOL gibbed = GibMonsterGib(fGibSpawnsDecal);

	GibMonsterSound(gibbed);
	GibMonsterEnd(gibbed);

}//END OF GibMonster



//parameter: BOOL fGibSpawnsDecal
GENERATE_GIBMONSTERGIB_IMPLEMENTATION(CBaseMonster){


	

	TraceResult	tr;
	BOOL gibbed = FALSE;
	
	//MODDD NOTE - Mainly, GibMonster means we really plan on gibbing the monster, but filter out a few more possibilities, like being disallowed by violence_Xgibs per
	//             this monster's type, or german censorship not allowing gibs for humans or even robot replacements (germanRobotGibs of 0).


	// only humans throw skulls !!!UNDONE - eventually monsters will have their own sets of gibs
	if ( HasHumanGibs() )
	{
		
		//Old check... mostly.
		//if ( global_germanCensorship == 1 || CVAR_GET_FLOAT("violence_hgibs") != 0 )	// Only the player will ever get here   ...Why was this comment here as-is? only the player? what?
		
		
		//MODDD - Little intervention here when spawning human gibs. Check to see if german censorship is on.

		if(global_germanCensorship==0){
			//turned off? usual behavior, only require violence_hgibs to be off.
			if(CVAR_GET_FLOAT("violence_hgibs") != 0){
				BOOL spawnHeadBlock = this->DetermineGibHeadBlock();
				if(!spawnHeadBlock){
					//HeadGib assumes human, only option.
					CGib::SpawnHeadGib( pev, fGibSpawnsDecal );
				}
				CGib::SpawnRandomGibs( pev, 4, GIB_HUMAN_ID, fGibSpawnsDecal );	// throw some human gibs.
				gibbed = TRUE;  //MODDD - only count as gibbed if we managed to spawn gibs. Any blocking from germancensorship makes the model fade out instead of disappear instantly.
			}
		}else{
			//with german censorship on, a call for human gibs gets replaced with robot gibs only if we're allowed to use german models (separate CVar), robot gibs are allowed and the robot gib model was found.
			//Notice that german censorship ignores the "violence_hgibs", and would already disallow spawning gibs.
			if( CanUseGermanModel() && EASY_CVAR_GET(germanRobotGibs)==1 && globalPSEUDO_germanModel_hgibFound==1 )
			{
				//robots do not have a head gib.
				//if(!spawnHeadBlock){
				//	CGib::SpawnHeadGib( pev, gibsSpawnDecal );
				//}

				if(EASY_CVAR_GET(germanRobotGibsDecal)==0){
					//this disallows robot gib decals made on hitting the ground.
					fGibSpawnsDecal = FALSE;
				}

				CGib::SpawnRandomGibs( pev, 6, GIB_GERMAN_ID, fGibSpawnsDecal );	// throw some robot gibs.
				gibbed = TRUE;
			}
		}
		



	}
	else if ( HasAlienGibs() )
	{
		//NOTE: using real alien gibs.  If "agibs" are banned, then don't show.
		//if ( global_germanCensorship == 1 || CVAR_GET_FLOAT("violence_agibs") != 0 )	// Should never get here, but someone might call it directly

		//MODDD - no involvement from germancensorship, keep to using violence_agibs.
		if(CVAR_GET_FLOAT("violence_agibs") != 0)
		{
			//2152205.94
			Vector temp = this->pev->absmax - this->pev->absmin;
			//easyForcePrintLine("SUPER SIZE MULTI: %.2f %.2f %.2f : %.2f %.2f %.2f : %.2f %.2f %.2f", temp.x, temp.y, temp.z   pev->  );
			
			//garg size: 262144   ... and agrunt size too??? really?
			//ABS::: 5478400.00  for garg, better discrepency!!!gibsSpawnDecals
			//easyForcePrintLine("IM A no  %.2f %d %d ", temp.x * temp.y * temp.z, 200000,  (temp.x * temp.y * temp.z) < 200000);
			if(temp.x * temp.y * temp.z < 5000000){
				CGib::SpawnRandomGibs( pev, 4, GIB_ALIEN_ID, fGibSpawnsDecal );	// Throw alien gibs
			}else{
				CGib::SpawnRandomGibs( pev, 20, GIB_ALIEN_ID, fGibSpawnsDecal );	// Throw alien gibs
			}
			gibbed = TRUE;
			
		}
	}else{
		//Neither human nor alien? Should this assume some other form of mechanical? Turrets have their own gibbing script now and robot replacements still register as "HasHumanGibs()" to be replaced in there.

		//Default behavior is to just fade out, probably fine.

	}

	return gibbed;
}//END OF GibMonsterGib



//parameter: BOOL fGibbed
GENERATE_GIBMONSTERSOUND_IMPLEMENTATION(CBaseMonster){

	
	
	//MODDD - only play this sound if organic. This excludes german robot replacements.
	if( isOrganic() ){

		//Only organic things that gibbed can play this sound.
		if(fGibbed){
			//MODDD - use soundsentencesave, always available in sentence form.
			//Also, german censorship will block this sound for humans regardless of robot models.
			//EMIT_SOUND_FILTERED(ENT(pev), CHAN_WEAPON, "common/bodysplat.wav", 1, ATTN_NORM, TRUE);
			UTIL_playOrganicGibSound(pev);
		}
	}else{
		//Any metallic things exploding or fading out play this sound instead. Includes german robot replacements gibbing.
		UTIL_playMetalGibSound(pev);
	}



}//END OF GibMonsterSound

//parameter: BOOL fGibbed
GENERATE_GIBMONSTEREND_IMPLEMENTATION(CBaseMonster){
	

	//Gee golly gee, ya think?
	this->recognizablyDead = TRUE;


	
	pev->takedamage = DAMAGE_NO;
	pev->solid = SOLID_NOT;// do something with the body. while monster blows up


	if ( fGibbed )
	{
		pev->effects = EF_NODRAW; // make the model invisible.
		
		if ( !IsPlayer() )
		{
			//GibMonster(DetermineGibHeadBlock(), gibsSpawnDecals);  //you are here.
			// don't remove players!
			SetThink ( &CBaseEntity::SUB_Remove );
			pev->nextthink = gpGlobals->time;
		}else{

			//???
		}
	}
	else
	{
		//NOTICE - the Player overrides this method to not try and delete itself
		//at the end of fade, not that changing the think method would work here anyways
		//to even get to that point.
		FadeMonster();
	}

	pev->deadflag = DEAD_DEAD;
	FCheckAITrigger();

	// don't let the status bar glitch for players.with <0 health.
	//MODDD NOTE - should this check only be for players then..?
	if (pev->health < -99)
	{
		pev->health = 0;
	}
	
	//If we were supposed to fade on death but for some reason didn't (gibbed), do this to be safe.
	if ( ShouldFadeOnDeath() && fGibbed )  //AKA, not fade.
		UTIL_Remove(this);



}//END OF GibMonsterEnd






//MODDD - Reorganized this a bit to remove some redundancies.  Should have equivalent behavior.

//=========================================================
// GetDeathActivity - determines the best type of death
// anim to play.
//=========================================================
Activity CBaseMonster :: GetDeathActivity ( void )
{
	Activity	deathActivity;
	//BOOL		fTriedDirection;
	BOOL		fCanTryDirection = FALSE; //MODDD - new.
	float		flDot;
	TraceResult	tr;
	Vector		vecSrc;
	int violentDeathPriorityValue;

	//MODDD - how soon should I check for the violentDeath compared to other ACT's and return early if violent death is allowed?
	violentDeathPriorityValue = violentDeathPriority();

	if(!violentDeathAllowed()){
		//then forget it.
		violentDeathPriorityValue = -1;
	}

	if ( pev->deadflag != DEAD_NO )
	{
		// don't run this while dying.
		return m_IdealActivity;
	}


	if(violentDeathPriorityValue == 1 && violentDeathDamageRequirement() && violentDeathClear()){
		return ACT_DIEVIOLENT;
	}


	vecSrc = Center();

	//fTriedDirection = FALSE;
	deathActivity = ACT_DIESIMPLE;// in case we can't find any special deaths to do.



	switch ( m_LastHitGroup )
	{
		// try to pick a region-specific death.
	case HITGROUP_HEAD:
		if(LookupActivity(ACT_DIE_HEADSHOT) != ACTIVITY_NOT_AVAILABLE){
			//do it!
			deathActivity = ACT_DIE_HEADSHOT;
		}else{
			//try that instead
			fCanTryDirection = TRUE;
		}
		break;

	case HITGROUP_STOMACH:
		if(LookupActivity(ACT_DIE_GUTSHOT) != ACTIVITY_NOT_AVAILABLE){
			//do it!
			deathActivity = ACT_DIE_GUTSHOT;
		}else{
			//try that instead
			fCanTryDirection = TRUE;
		}
		break;

	case HITGROUP_GENERIC:
		// try to pick a death based on attack direction
		fCanTryDirection = TRUE;
		break;

	default:
		// try to pick a death based on attack direction
		fCanTryDirection = TRUE;
		break;
	}






	if(fCanTryDirection){

		
		if(violentDeathPriorityValue == 2 && violentDeathDamageRequirement() && violentDeathClear()){
			return ACT_DIEVIOLENT;
		}

		UTIL_MakeVectors ( pev->angles );
		flDot = DotProduct ( gpGlobals->v_forward, g_vecAttackDir * -1 );
		//Haven't found a good death activity above (has a sequence available)? Try the directions.
		if ( flDot > 0.3 )
		{
			if(LookupActivity(ACT_DIEFORWARD) != ACTIVITY_NOT_AVAILABLE){
				//One more check.
				// make sure there's room to fall forward
				UTIL_TraceHull ( vecSrc, vecSrc + gpGlobals->v_forward * 64, dont_ignore_monsters, head_hull, edict(), &tr );
				// Nothing in the way? it's good.
				if ( tr.flFraction == 1.0 )
				{
					deathActivity = ACT_DIEFORWARD;
				}
			}//END OF ACT_DIEFORWARD check
		}
		else if ( flDot <= -0.3 )
		{

			//MODDD INJECTION - is ACT_DIEVIOLENT possible?
			// It must be allowed by this particular monster to be used, regardless of having a sequence mapped by the model.
			// This way we can promise some attention was paid to whether it needs a trace check or not, which varries per monster.
			// There is no standard length of trace that will satisfy all violent death animations.  violentDeathClear will handle that.
			
			if(violentDeathPriorityValue == 3 && violentDeathDamageRequirement() && violentDeathClear()){
				return ACT_DIEVIOLENT;
			}
			
			/*
			if(violentDeathAllowed() && violentDeathDamageRequirement() && violentDeathClear()){
				//Apparently this is OK.
				//But if there are ever multiple violent death anims, we need to use this as a signal to pick
				//a more specific one later possibly, like what one passed its own distance check.
				deathActivity = ACT_DIEVIOLENT;
			}else
			*/
			{
				//give DIEBACKWARD a chance as usual.
				if(LookupActivity(ACT_DIEBACKWARD) != ACTIVITY_NOT_AVAILABLE){
					//One more check.
					// make sure there's room to fall backward
					UTIL_TraceHull ( vecSrc, vecSrc - gpGlobals->v_forward * 64, dont_ignore_monsters, head_hull, edict(), &tr );
					// Nothing in the way? it's good.
					if ( tr.flFraction == 1.0 )
					{
						deathActivity = ACT_DIEBACKWARD;
					}
				}//END OF ACT_DIEBACKWARD check
			}



		}//END OF flDot checks
	}//END OF fCanTryDirection



	return deathActivity;
}//END OF GetDeathActivity

//=========================================================
// GetSmallFlinchActivity - determines the best type of flinch
// anim to play.
//=========================================================
Activity CBaseMonster :: GetSmallFlinchActivity ( void )
{
	Activity	flinchActivity;
	BOOL		fTriedDirection;
	float		flDot;

	fTriedDirection = FALSE;
	UTIL_MakeVectors ( pev->angles );
	flDot = DotProduct ( gpGlobals->v_forward, g_vecAttackDir * -1 );
	
	switch ( m_LastHitGroup )
	{
		// pick a region-specific flinch
	case HITGROUP_HEAD:
		flinchActivity = ACT_FLINCH_HEAD;
		break;
	case HITGROUP_STOMACH:
		flinchActivity = ACT_FLINCH_STOMACH;
		break;
	case HITGROUP_LEFTARM:
		flinchActivity = ACT_FLINCH_LEFTARM;
		break;
	case HITGROUP_RIGHTARM:
		flinchActivity = ACT_FLINCH_RIGHTARM;
		break;
	case HITGROUP_LEFTLEG:
		flinchActivity = ACT_FLINCH_LEFTLEG;
		break;
	case HITGROUP_RIGHTLEG:
		flinchActivity = ACT_FLINCH_RIGHTLEG;
		break;
	case HITGROUP_GENERIC:
	default:
		// just get a generic flinch.
		flinchActivity = ACT_SMALL_FLINCH;
		break;
	}


	// do we have a sequence for the ideal activity?
	if ( LookupActivity ( flinchActivity ) == ACTIVITY_NOT_AVAILABLE )
	{
		flinchActivity = ACT_SMALL_FLINCH;
	}

	return flinchActivity;
}//END OF GetSmallFlinchActivity


Activity CBaseMonster::GetBigFlinchActivity(void){
	
	Activity flinchActivity;

	flinchActivity = ACT_BIG_FLINCH;

	// do we have a sequence for this big flinch?
	if ( LookupActivity ( flinchActivity ) == ACTIVITY_NOT_AVAILABLE )
	{
		flinchActivity = ACT_SMALL_FLINCH;
	}

	return flinchActivity;
}//END OF GetBigFlinchActivity







void CBaseMonster::BecomeDead( void )
{
	pev->takedamage = DAMAGE_YES;// don't let autoaim aim at corpses.
	
	// give the corpse half of the monster's original maximum health. 
	pev->health = pev->max_health / 2;
	pev->max_health = 5; // max_health now becomes a counter for how many blood decals the corpse can place.

	// make the corpse fly away from the attack vector
	pev->movetype = MOVETYPE_TOSS;
	//pev->flags &= ~FL_ONGROUND;
	//pev->origin.z += 2;
	//pev->velocity = g_vecAttackDir * -1;
	//pev->velocity = pev->velocity * RANDOM_FLOAT( 300, 400 );
}


BOOL CBaseMonster::ShouldGibMonster( int iGib )
{

	if(global_cheat_iwantguts >= 1){
		//VIOLENCE!  BLOOD AND GUTS!!!
		return TRUE;
	}

	//easyForcePrintLine("ShouldGibMonster: My iGib is %d. Curhealth:%.2f : req:%d", iGib, pev->health, GIB_HEALTH_VALUE);

	if ( ( iGib == GIB_NORMAL && pev->health < GIB_HEALTH_VALUE ) || ( iGib == GIB_ALWAYS || iGib == GIB_ALWAYS_NODECAL ) )
		return TRUE;
	
	return FALSE;
}





//Ever wondered how the map knows a monster was killed for events / logic, such as unlocking doors or spawning other enemies in predetermined locations?
//It's "FCheckAITrigger". It is called right before gibbing and every frame of logic in monsterstate.cpp, so that a death anim would trigger the condition too.

/*
============
Killed
============
*/
//void CBaseMonster :: Killed( entvars_t *pevAttacker, int iGib )
GENERATE_KILLED_IMPLEMENTATION(CBaseMonster)
{
	//m_pfnThink
		//UTIL_Remove(this);

	//return;

	easyPrintLine("Killed: %s:%d. deadflag:%d. MY iGib WAS %d", getClassname(), monsterID, pev->deadflag, iGib);


	/*
	//Determined in GibMonster itself intead now.
	BOOL gibsSpawnDecals = !(iGib == GIB_ALWAYS_NODECAL);

	if(CanUseGermanModel() && EASY_CVAR_GET(germanRobotGibsDecal)==0){
		//If german censorship is on and the oil decal for robot gibs is disabled (assuming robot gibs are enabled), the spawned gibs will not make decals.
		gibsSpawnDecals = FALSE;
	}
	*/


	unsigned int	cCount = 0;
	BOOL			fDone = FALSE;

	//MODDD - EMERGENCY TEST FLAG
	iAmDead = TRUE;

	//MODDD - if stuck to a barnacle, not anymore.
	barnacleLocked = FALSE;

	if ( HasMemory( bits_MEMORY_KILLED ) )
	{
		if ( ShouldGibMonster( iGib ) ){
			//GibMonster(DetermineGibHeadBlock(), gibsSpawnDecals);
			GENERATE_GIBMONSTER_CALL;
		}
		return;
	}
	Remember( bits_MEMORY_KILLED );

	// clear the deceased's sound channels.(may have been firing or reloading when killed)
	EMIT_SOUND(ENT(pev), CHAN_WEAPON, "common/null.wav", 1, ATTN_NORM);

	//pDeathSounds
	//MODDD - TODO: for voice maybe at some point?  Unless that would stop death-cries or something.


	m_IdealMonsterState = MONSTERSTATE_DEAD;
	// Make sure this condition is fired too (TakeDamage breaks out before this happens on death)
	SetConditions( bits_COND_LIGHT_DAMAGE );
	

	// tell owner ( if any ) that we're dead.This is mostly for MonsterMaker functionality.
	CBaseEntity *pOwner = CBaseEntity::Instance(pev->owner);
	if ( pOwner )
	{
		easyPrintLine("DO I, %s, HAVE A OWNERS? %s", getClassname(), pOwner->getClassname());
		pOwner->DeathNotice( pev );
	}
	

	// Before any mods to health, record what it was for monitoring overkill.
	//////killedHealth = pev->health;
	// ACTUALLY just make it the amount of damage dealth by the last attack.
	// may feel a little better this way.
	


	if	( ShouldGibMonster( iGib ) )
	{
		//GibMonster(DetermineGibHeadBlock(), gibsSpawnDecals);
		GENERATE_GIBMONSTER_CALL;
		return;
	}
	else if ( pev->flags & FL_MONSTER )
	{

		//WARNING - bad assumption! Leave what to do to the touch method up to the monster in question.
		//Fliers may want to detect when they touch the ground to stop a falling cycler.
		//SetTouch( NULL );
		OnKilledSetTouch();
		BecomeDead();
	}
	
	// don't let the status bar glitch for players.with <0 health.
	if (pev->health < -99)
	{
		pev->health = 0;
	}

	//MODDD - just to make sure.
	m_bitsDamageType = 0;
	m_bitsDamageTypeMod = 0;

	
	//pev->enemy = ENT( pevAttacker );//why? (sjb)
	
	m_IdealMonsterState = MONSTERSTATE_DEAD;


	firstTimeKilled = FALSE;  //certainly won't be anymore.

}//END OF Killed




//
// fade out - slowly fades a entity out, then removes it.
//
// DON'T USE ME FOR GIBS AND STUFF IN MULTIPLAYER! 
// SET A FUTURE THINK AND A RENDERMODE!!
//MODDD NOTE - luckily this doesn't actually fade out the player, 
//but only because the player immediately sets the Think on its own to PlayerDeathThink
//after calling GibMonster. This overrides the SUB_FadeOut Set Think call below.
//But the player should still not call this method anyways.
void CBaseEntity :: SUB_StartFadeOut ( void )
{
	if (pev->rendermode == kRenderNormal)
	{
		pev->renderamt = 255;
		pev->rendermode = kRenderTransTexture;
	}

	pev->solid = SOLID_NOT;
	pev->avelocity = g_vecZero;

	pev->nextthink = gpGlobals->time + 0.1;
	SetThink ( &CBaseEntity::SUB_FadeOut );
}

void CBaseEntity :: SUB_FadeOut ( void  )
{
	//Multiply the fade by 0.1 because that's how long this think method is expected to delay before getting called again.
	//That effectively means with each second, the monster's opacity (renderamt) drops by an amount of "monsterFadeOutRate".
	
	const float fadeMoveAmount = EASY_CVAR_GET(monsterFadeOutRate)*0.1;

	if ( pev->renderamt > fadeMoveAmount )
	{
		pev->renderamt -= fadeMoveAmount;
		pev->nextthink = gpGlobals->time + 0.1;
	}
	else 
	{
		pev->renderamt = 0;
		pev->nextthink = gpGlobals->time + 0.2;
		SetThink ( &CBaseEntity::SUB_Remove );
	}
}

//=========================================================
// WaitTillLand - in order to emit their meaty scent from
// the proper location, gibs should wait until they stop 
// bouncing to emit their scent. That's what this function
// does.
//=========================================================
void CGib :: WaitTillLand ( void )
{
	if (!IsInWorld())
	{
		UTIL_Remove( this );
		return;
	}

	if ( pev->velocity == g_vecZero )
	{
		SetThink (&CBaseEntity::SUB_StartFadeOut);
		pev->nextthink = gpGlobals->time + m_lifeTime;

		// If you bleed, you stink!... unless you're a robot, gears don't really attract eaters.
		// But exceptions are exceptions (GermanModelOrganicLogic).
		if ( m_bloodColor != DONT_BLEED && (GermanModelOrganicLogic() || m_bloodColor != BLOOD_COLOR_BLACK)  )
		{
			// ok, start stinkin!
			CSoundEnt::InsertSound ( bits_SOUND_MEAT, pev->origin, 384, 25 );
		}
	}
	else
	{
		// wait and check again in another half second.
		pev->nextthink = gpGlobals->time + 0.5;
	}
}

//
// Gib bounces on the ground or wall, sponges some blood down, too!
//
void CGib :: BounceGibTouch ( CBaseEntity *pOther )
{
	Vector	vecSpot;
	TraceResult	tr;
	
	//if ( RANDOM_LONG(0,1) )
	//	return;// don't bleed everytime

	if (pev->flags & FL_ONGROUND)
	{
		pev->velocity = pev->velocity * 0.9;
		pev->angles.x = 0;
		pev->angles.z = 0;
		pev->avelocity.x = 0;
		pev->avelocity.z = 0;
	}
	else
	{
		//if ( g_Language != LANGUAGE_GERMAN && m_cBloodDecals > 0 && m_bloodColor != DONT_BLEED )
		if( (global_germanCensorship != 1 || m_bloodColor == BLOOD_COLOR_BLACK) && m_cBloodDecals > 0 && m_bloodColor != DONT_BLEED )
		{
			vecSpot = pev->origin + Vector ( 0 , 0 , 8 );//move up a bit, and trace down.
			UTIL_TraceLine ( vecSpot, vecSpot + Vector ( 0, 0, -24 ),  ignore_monsters, ENT(pev), & tr);

			UTIL_BloodDecalTrace( &tr, m_bloodColor );

			m_cBloodDecals--; 
		}
		//MODDD TODO: leave holes in a "robot" we can assume we hit, if it is a "human" we hit (always a robot)?

		if ( m_material != matNone && RANDOM_LONG(0,2) == 0 )
		{
			float volume;
			float zvel = fabs(pev->velocity.z);
		
			volume = 0.8 * min(1.0, ((float)zvel) / 450.0);

			CBreakable::MaterialSoundRandom( edict(), (Materials)m_material, volume );
		}
	}
}

//
// Sticky gib puts blood on the wall and stays put. 
//
void CGib :: StickyGibTouch ( CBaseEntity *pOther )
{
	Vector	vecSpot;
	TraceResult	tr;
	
	SetThink ( &CBaseEntity::SUB_Remove );
	pev->nextthink = gpGlobals->time + 10;

	if ( !FClassnameIs( pOther->pev, "worldspawn" ) )
	{
		pev->nextthink = gpGlobals->time;
		return;
	}

	UTIL_TraceLine ( pev->origin, pev->origin + pev->velocity * 32,  ignore_monsters, ENT(pev), & tr);

	UTIL_BloodDecalTrace( &tr, m_bloodColor );

	pev->velocity = tr.vecPlaneNormal * -1;
	pev->angles = UTIL_VecToAngles ( pev->velocity );
	pev->velocity = g_vecZero; 
	pev->avelocity = g_vecZero;
	pev->movetype = MOVETYPE_NONE;
}




void CGib :: Spawn( const char *szGibModel )
{
	CGib::Spawn(szGibModel, TRUE);
}
//
// Throw a chunk
//
void CGib :: Spawn( const char *szGibModel, BOOL spawnsDecal )
{
	pev->movetype = MOVETYPE_BOUNCE;
	pev->friction = 0.55; // deading the bounce a bit
	
	// sometimes an entity inherits the edict from a former piece of glass,
	// and will spawn using the same render FX or rendermode! bad!
	pev->renderamt = 255;
	pev->rendermode = kRenderNormal;
	pev->renderfx = kRenderFxNone;
	pev->solid = SOLID_SLIDEBOX;/// hopefully this will fix the VELOCITY TOO LOW crap
	pev->classname = MAKE_STRING("gib");

	SET_MODEL(ENT(pev), szGibModel);
	UTIL_SetSize(pev, Vector( 0, 0, 0), Vector(0, 0, 0));

	pev->nextthink = gpGlobals->time + 4;
	m_lifeTime = 25;
	SetThink ( &CGib::WaitTillLand );
	SetTouch ( &CGib::BounceGibTouch );

	m_material = matNone;
	m_cBloodDecals = 5;// how many blood decals this gib can place (1 per bounce until none remain). 

	if(!spawnsDecal){
		//none.
		m_cBloodDecals = 0;
	}

}



//=========================================================
// CheckTraceHullAttack - expects a length to trace, amount 
// of damage to do, and damage type. Returns a pointer to
// the damaged entity in case the monster wishes to do
// other stuff to the victim (punchangle, etc)
//
// Used for many contact-range melee attacks. Bites, claws, etc.
//=========================================================
//MODDD - note that the old form, missing the 2nd mask, now  redirects to the new form and just sends "0"
//for the 2nd mask.  Safe to assume no damages meant there if nothing is supplied for it.
CBaseEntity* CBaseMonster::CheckTraceHullAttack( float flDist, int iDamage, int iDmgType ){
	return CheckTraceHullAttack(Vector(0,0,0), flDist, iDamage, iDmgType, 0, NULL);
}
CBaseEntity* CBaseMonster::CheckTraceHullAttack( float flDist, int iDamage, int iDmgType, TraceResult* out_traceResult ){
	return CheckTraceHullAttack(Vector(0,0,0), flDist, iDamage, iDmgType, 0, out_traceResult);
}
CBaseEntity* CBaseMonster::CheckTraceHullAttack( float flDist, int iDamage, int iDmgType, int iDmgTypeMod ){
	return CheckTraceHullAttack(Vector(0,0,0), flDist, iDamage, iDmgType, iDmgTypeMod, NULL);
}
CBaseEntity* CBaseMonster::CheckTraceHullAttack( float flDist, int iDamage, int iDmgType, int iDmgTypeMod, TraceResult* out_traceResult ){
	return CheckTraceHullAttack(Vector(0,0,0), flDist, iDamage, iDmgType, iDmgTypeMod, out_traceResult);
}
CBaseEntity* CBaseMonster::CheckTraceHullAttack( const Vector vecStartOffset, float flDist, int iDamage, int iDmgType ){
	return CheckTraceHullAttack(vecStartOffset, flDist, iDamage, iDmgType, 0, NULL);
}
CBaseEntity* CBaseMonster::CheckTraceHullAttack( const Vector vecStartOffset, float flDist, int iDamage, int iDmgType, TraceResult* out_traceResult ){
	return CheckTraceHullAttack(vecStartOffset, flDist, iDamage, iDmgType, 0, out_traceResult);
}
CBaseEntity* CBaseMonster :: CheckTraceHullAttack( const Vector vecStartOffset, float flDist, int iDamage, int iDmgType, int iDmgTypeMod ){
	return CheckTraceHullAttack(vecStartOffset, flDist, iDamage, iDmgType, iDmgTypeMod, NULL);
}




//MODDD - version of the same method below that can expect the 2nd damge mask too.
//TODO - send the entity (monster) calling this method as a paramter and use it to see if the thing hit matches its "m_hEnemy" (if this calling entity has one).
//       If it does not match the enemy, try separate checks in case something else (crate, friend, etc.) was accidentally hit?
CBaseEntity* CBaseMonster :: CheckTraceHullAttack( const Vector vecStartOffset, float flDist, int iDamage, int iDmgType, int iDmgTypeMod, TraceResult* out_traceResult )
{
	TraceResult tr;

	if (IsPlayer())
		UTIL_MakeVectors( pev->angles );
	else
		UTIL_MakeAimVectors( pev->angles );

	Vector vecStart = pev->origin + vecStartOffset;
	vecStart.z += pev->size.z * 0.5;
	Vector vecEnd = vecStart + (gpGlobals->v_forward * flDist );
	//Vector vecEnd = pev->origin + (gpGlobals->v_forward * flDist );

	edict_t* thingHit = NULL;
	UTIL_TraceHull( vecStart, vecEnd, dont_ignore_monsters, head_hull, ENT(pev), &tr );
	
	//by default.
	thingHit = tr.pHit;


	if(tr.pHit == NULL){
		//misssed. it is possible we are swinging at something below us. Do a re-check for slightly lower.
		TraceResult trB;
		
		Vector vecStartB = pev->origin + vecStartOffset + Vector(0, 0, pev->size.z*0.4);
		Vector vecEndB = vecStartB + (gpGlobals->v_forward * flDist*1.05 ) + Vector(0, 0, -flDist*1.05/3.6);
		UTIL_TraceHull( vecStartB, vecEndB, dont_ignore_monsters, head_hull, ENT(pev), &trB );
		
		//if(trB.pHit!=NULL)easyForcePrintLine("ALT A? %s", STRING(trB.pHit->v.classname));
		//else easyForcePrintLine("ALT A? null");

		if(trB.pHit != NULL && strcmp(STRING(trB.pHit ->v.classname), "worldspawn") != 0){
			//hit something, and it isn't worldpawn? Use that instead!
			//easyForcePrintLine("ALT A!");
			thingHit = trB.pHit;
		}else{
			//hit sound anyways?
			//TEXTURETYPE_PlaySound(&tr, vecStart, vecEnd, 0);
		}

		/*
			CBaseEntity* pPlayerEntityScan = NULL;
			while( (pPlayerEntityScan = UTIL_FindEntityByClassname(pPlayerEntityScan, "player")) != NULL){
				CBasePlayer* tempPlayer = static_cast<CBasePlayer*>(pPlayerEntityScan);
				tempPlayer->debugVect1Draw = TRUE;
				tempPlayer->debugVect1Start = vecStartB;
				tempPlayer->debugVect1End = vecEndB + (vecEndB - vecStartB)*trB.flFraction;
				tempPlayer->debugVect1Success = (trB.flFraction < 1.0);
		
			}
			*/
	}else if(tr.pHit != NULL && strcmp(STRING(tr.pHit->v.classname), "worldspawn") == 0){
		//hit the world? is it possible we're swinging at something above us from an incline? Do a re-check for slightly higher.
		TraceResult trB;
		
		Vector vecStartB = pev->origin + vecStartOffset + Vector(0, 0, pev->size.z*0.6);
		Vector vecEndB = vecStartB + (gpGlobals->v_forward * flDist*1.05 ) + Vector(0, 0, flDist*1.05/2.7);
		UTIL_TraceHull( vecStartB, vecEndB, dont_ignore_monsters, head_hull, ENT(pev), &trB );
		
		//easyForcePrintLine("ALT B?");
		if(trB.pHit != NULL && strcmp(STRING(trB.pHit ->v.classname), "worldspawn") != 0){
			//hit something, and it isn't worldpawn? Use that instead!
			//easyForcePrintLine("ALT B!");
			thingHit = trB.pHit;
		}else{
			//hit sound anyways?
			//TEXTURETYPE_PlaySound(&tr, vecStartB, vecEndB, 0);
		}
	}else{
		//original hit  wasn't worldspawn? ok.
		
	}






	/*
	if( tr.pHit){
		easyForcePrintLine("DID WE DO IT?? %s %d", STRING(tr.pHit->v.classname), iDamage);
	}else{
		easyForcePrintLine("DID WE DO IT?? none %d", iDamage);
	}
	*/



	if ( thingHit )
	{
		CBaseEntity *pEntity = CBaseEntity::Instance( thingHit );
		
		
		if ( iDamage > 0 )
		{
			pEntity->TakeDamage( pev, pev, iDamage, iDmgType, iDmgTypeMod );


			//MODDD - draw blood.
			UTIL_fromToBlood(this, pEntity, (float)iDamage, flDist, &tr.vecEndPos, &vecStart, &vecEnd);

			/*
			for(int i = 0; i < 10; i++){
				UTIL_BloodStream(tr.pHit->, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
			}
			*/

		}

		if(out_traceResult != NULL){
			//copy our lineTrace to the destination if provided.
			*out_traceResult = tr;
		}

		return pEntity;
	}

	//don't copy anything?

	return NULL;
}




//=========================================================
// FInViewCone - returns true is the passed ent is in
// the caller's forward view cone. The dot product is performed
// in 2d, making the view cone infinitely tall. 
//=========================================================
BOOL CBaseMonster :: FInViewCone ( CBaseEntity *pEntity )
{
	//return FALSE;

	Vector2D	vec2LOS;
	float	flDot;

	UTIL_MakeVectors ( pev->angles );
	
	vec2LOS = ( pEntity->pev->origin - pev->origin ).Make2D();
	vec2LOS = vec2LOS.Normalize();

	flDot = DotProduct (vec2LOS , gpGlobals->v_forward.Make2D() );

	if ( flDot > m_flFieldOfView )
	{
		//if(FClassnameIs(this->pev, "monster_human_assault")&&FClassnameIs(pEntity->pev, "player"))easyForcePrintLine("FInViewCone: %s%d : %s dot:%.2f fov:%.2f pass:%d", this->getClassname(), monsterID, pEntity->getClassname(), flDot, m_flFieldOfView, 1);
		return TRUE;
	}
	else
	{
		//if(FClassnameIs(this->pev, "monster_human_assault")&&FClassnameIs(pEntity->pev, "player"))easyForcePrintLine("FInViewCone: %s%d : %s dot:%.2f fov:%.2f pass:%d", this->getClassname(), monsterID, pEntity->getClassname(), flDot, m_flFieldOfView, 0);
		return FALSE;
	}
}

//=========================================================
// FInViewCone - returns true is the passed vector is in
// the caller's forward view cone. The dot product is performed
// in 2d, making the view cone infinitely tall. 
//=========================================================
BOOL CBaseMonster :: FInViewCone ( Vector *pOrigin )
{


	Vector2D	vec2LOS;
	float		flDot;

	UTIL_MakeVectors ( pev->angles );
	
	vec2LOS = ( *pOrigin - pev->origin ).Make2D();
	vec2LOS = vec2LOS.Normalize();

	flDot = DotProduct (vec2LOS , gpGlobals->v_forward.Make2D() );

	if ( flDot > m_flFieldOfView )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//=========================================================
// FVisible - returns true if a line can be traced from
// the caller's eyes to the target
//=========================================================
BOOL CBaseEntity :: FVisible ( CBaseEntity *pEntity )
{
	TraceResult tr;
	Vector		vecLookerOrigin;
	Vector		vecTargetOrigin;
	
	if (FBitSet( pEntity->pev->flags, FL_NOTARGET ))
		return FALSE;

	// don't look through water
	//MODDD - some monsters that want to surface to do a ranged attack in the water, like archers,
	//        may need the ability to see through water.
	//This if-then is the addition.
	if(SeeThroughWaterLine() == FALSE){
		if ((pev->waterlevel != 3 && pEntity->pev->waterlevel == 3) 
			|| (pev->waterlevel == 3 && pEntity->pev->waterlevel == 0))
			return FALSE;
	}


	vecLookerOrigin = pev->origin + pev->view_ofs;//look through the caller's 'eyes'
	vecTargetOrigin = pEntity->EyePosition();

	UTIL_TraceLine(vecLookerOrigin, vecTargetOrigin, ignore_monsters, ignore_glass, ENT(pev)/*pentIgnore*/, &tr);
	
	if (tr.flFraction != 1.0)
	{
		return FALSE;// Line of sight is not established
	}
	else
	{
		return TRUE;// line of sight is valid.
	}
}

//=========================================================
// FVisible - returns true if a line can be traced from
// the caller's eyes to the target vector
//=========================================================
BOOL CBaseEntity :: FVisible ( const Vector &vecOrigin )
{
	TraceResult tr;
	Vector		vecLookerOrigin;
	
	vecLookerOrigin = EyePosition();//look through the caller's 'eyes'

	UTIL_TraceLine(vecLookerOrigin, vecOrigin, ignore_monsters, ignore_glass, ENT(pev)/*pentIgnore*/, &tr);
	
	if (tr.flFraction != 1.0)
	{
		return FALSE;// Line of sight is not established
	}
	else
	{
		return TRUE;// line of sight is valid.
	}
}






















/*
================
TraceAttack
================
*/
//CBaseEntity's "TraceAttack" method doesn't seem to be used very often, if at all.  All NPCs are of "CBaseMonster" which never calls its parent class (CBaseEntity)'s "TraceAttack".
GENERATE_TRACEATTACK_IMPLEMENTATION(CBaseEntity)
{
	Vector vecOrigin = ptr->vecEndPos - vecDir * 4;

	if ( pev->takedamage )
	{
		AddMultiDamage( pevAttacker, this, flDamage, bitsDamageType, bitsDamageTypeMod );

		int blood = BloodColor();
		
		if ( blood != DONT_BLEED )
		{
			//MODDD - old way.
			//SpawnBlood(vecOrigin, blood, flDamage);// a little surface blood.
			if(useBloodEffect){
				//MODDD!!!!!!
				DrawAlphaBlood(flDamage, ptr );
			}else{
				//MODDD TODO - breakpoint, see who sent this with FALSE here? repeat for useBulletHitSound?
			}

			TraceBleed( flDamage, vecDir, ptr, bitsDamageType, bitsDamageTypeMod );
			
			//if(useBulletHitSound && (pevAttacker != NULL && (pevAttacker->renderfx & (ISPLAYER | ISNPC))) ){
			
			//UTIL_playFleshHitSound(pev);

			//By default, if a monster wants to play a bullet hit sound on me, allow it.
			//if(useBulletHitSound)*useBulletHitSound=TRUE;
			

		}
	}
}





//MODDD - CBaseEntity's TakeDamage and Killed moved to here.

// inflict damage on this entity.  bitsDamageType indicates type of damage inflicted, ie: DMG_CRUSH
GENERATE_TAKEDAMAGE_IMPLEMENTATION(CBaseEntity)
{
	Vector			vecTemp;

	if (!pev->takedamage)
		return 0;

	// UNDONE: some entity types may be immune or resistant to some bitsDamageType
	
	// if Attacker == Inflictor, the attack was a melee or other instant-hit attack.
	// (that is, no actual entity projectile was involved in the attack so use the shooter's origin). 
	if ( pevAttacker == pevInflictor )	
	{
		vecTemp = pevInflictor->origin - ( VecBModelOrigin(pev) );
	}
	else
	// an actual missile was involved.
	{
		vecTemp = pevInflictor->origin - ( VecBModelOrigin(pev) );
	}

// this global is still used for glass and other non-monster killables, along with decals.
	g_vecAttackDir = vecTemp.Normalize();
		
// save damage based on the target's armor level

// figure momentum add (don't let hurt brushes or other triggers move player)





	//the "(pev->movetype == MOVETYPE_TOSS && pev->flags & FL_ONGROUND)" condition was added in case this monster is a tossable that is against the ground. If so it can also be pushed.
	
	if(EASY_CVAR_GET(baseEntityDamagePushNormalMulti) != 0 || EASY_CVAR_GET(baseEntityDamagePushVerticalBoost) != 0 || EASY_CVAR_GET(baseEntityDamagePushVerticalMulti) ){
		if ((!FNullEnt(pevInflictor)) && (pev->movetype == MOVETYPE_WALK || pev->movetype == MOVETYPE_STEP || (pev->movetype == MOVETYPE_TOSS && pev->flags & FL_ONGROUND)) && (pevAttacker->solid != SOLID_TRIGGER) )
		{
			Vector vecDir = pev->origin - (pevInflictor->absmin + pevInflictor->absmax) * 0.5;
			vecDir = vecDir.Normalize();

			//MODDD - note. This can cause hit entities to glitch out like mad in their line of movement when the velocity pushes them against the ground.
			//        Would forcing them to lift into the air a bit help?

			
			//float flForce = flDamage * ((32 * 32 * 72.0) / (pev->size.x * pev->size.y * pev->size.z)) * 5;
			float sizeFactor = ((32 * 32 * 72.0) / (pev->size.x * pev->size.y * pev->size.z));
			float flForce = flDamage * sizeFactor * EASY_CVAR_GET(baseEntityDamagePushNormalMulti);
			

			
			//if (flForce > 1000.0) 
			//	flForce = 1000.0;
			//pev->velocity = pev->velocity + vecDir * flForce;


			Vector newVelocity = pev->velocity + vecDir * flForce + Vector(0, 0, EASY_CVAR_GET(baseEntityDamagePushVerticalBoost) + EASY_CVAR_GET(baseEntityDamagePushVerticalMulti)*sizeFactor   );

			
			if(newVelocity.z < EASY_CVAR_GET(baseEntityDamagePushVerticalMinimum)){
				//A push less than this in the Z coord is not allowed.
				newVelocity.z = EASY_CVAR_GET(baseEntityDamagePushVerticalMinimum);
			}


			//
			
			//vector / oldlength = normal
			//normal * newlength = right

			//(vector / oldlength) * newlength = right ...
			//

			//oldlength = sqrt(vector.x^2 + vector,y^2 + vector.z^2)
			//oldlength = sqrt(dot prod(vector*vector)) ...

			//vector / (sqrt(dotprod(vector*vector)) * newlength = right.

			//1600 * (1000 / 1600)

			if(pev->flags & FL_ONGROUND){
				::UTIL_SetOrigin(pev, Vector(pev->origin.x, pev->origin.y, pev->origin.z + 0.3));
				pev->flags &= ~FL_ONGROUND;  //is this ok?
				
				//NOTICE - to really have any effective pushing beyond a little weird-looking glitchiness, we really need this MOVETYPE_TOSS.
				//Other movetypes like STEP can instantly lock to the ground.
				//If it were formal for entities meant to be pushed to revert movetype to MOVETYPE_STEP on touching the ground since going airborne (however slight),
				//this would be more effective. Unknown if just having a MOVETYPE_TOSS permanently otherwise is even an issue come to think of it?
				//pev->movetype = MOVETYPE_TOSS;
			}


			//maxed at 1000.
			if (newVelocity.Length() > 1000.0) 
				newVelocity = newVelocity * (1000.0 / newVelocity.Length());

			pev->velocity = newVelocity; //and apply.

			
		}
	}


// do the damage
	pev->health -= flDamage;
	if (pev->health <= 0)
	{
		Killed( pevAttacker, GIB_NORMAL );
		return 0;
	}

	return 1;
}





// give health
int CBaseEntity::TakeHealth( float flHealth, int bitsDamageType )
{
	if (!pev->takedamage)
		return 0;

// heal
	if ( pev->health >= pev->max_health )
		return 0;

	pev->health += flHealth;

	if (pev->health > pev->max_health)
		pev->health = pev->max_health;

	return 1;
}









//void CBaseEntity::Killed( entvars_t *pevAttacker, int iGib )
GENERATE_KILLED_IMPLEMENTATION(CBaseEntity)
{
	iAmDead = TRUE;

	pev->takedamage = DAMAGE_NO;
	pev->deadflag = DEAD_DEAD;

	//MODDD - do we need to?
	firstTimeKilled = FALSE;

	UTIL_Remove( this );
}






void CBaseEntity::DrawAlphaBlood(float flDamage, const Vector& vecDrawLoc, int amount){
	//MODDD - TODO: INVESTIGATE: RandomBloodVector() ?
	UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), amount);
}



void CBaseEntity::DrawAlphaBloodSlash(float flDamage, const Vector& vecDrawLoc, const Vector& vecTraceLine  ){
	DrawAlphaBloodSlash(flDamage, vecDrawLoc, vecTraceLine, FALSE);
}

//takes a TraceResult ONLY so that info like the surface it hit 
void CBaseEntity::DrawAlphaBloodSlash(float flDamage, const Vector& vecDrawLoc, const Vector& vecTraceLine, const BOOL& extraBlood  ){
	
	Vector vecDirUp = Vector(0, 0, 1);
	Vector vecCross = CrossProduct ( vecTraceLine, vecDirUp);


	//draw blood across "vecCross":

	for(int i = 0; i <= 4; i++){

		Vector vecThisDrawLoc = vecDrawLoc + vecCross * (i - 2) * 3.5;

		if(!extraBlood){
			UTIL_BloodStream(vecThisDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(4, 6));
		}else{
			UTIL_BloodStream(vecThisDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(36, 56));
		}
		
	}

	

}





//MODDD - new
void CBaseEntity::DrawAlphaBlood(float flDamage, TraceResult *ptr ){
	//MODDD NOTE - how good is ptr->vecEndPos???
	DrawAlphaBlood(flDamage, ptr->vecEndPos);
}


void CBaseEntity::DrawAlphaBlood(float flDamage, const Vector& vecDrawLoc){

	


	//easyPrintLine("Death shot? %d", pev->health <= flDamage);
	/*
	if (pev->health <= flDamage)
	{
		//KILLING!
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));

	}else{
		
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
	}
	*/
	


	int drawBloodVersionValue = 0;
	int drawTripleBloodValue = 1;

	if (pev->health <= flDamage)
	{

		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(40, 70));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(40, 70));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(40, 70));
		
	}
	else
	{
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(4, 7));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(4, 7));
		UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(4, 7));
			
	}












	/*
	float drawBloodVersionValue = CVAR_GET_FLOAT("drawBloodVersion");
	float drawTripleBloodValue = CVAR_GET_FLOAT("drawTripleBlood");

	//Old Blood FX - by Osiris / OsirisGodoftheDead / THE_YETI
	if (pev->health <= flDamage)
	{

		if(drawBloodVersionValue == 0){
			UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
			UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
			UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
			//UTIL_BloodDrips( pev->origin, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			
		}else if(drawBloodVersionValue == 1){
			UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
			UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );

		}else if(drawBloodVersionValue == 2){
			UTIL_BloodDrips( vecDrawLoc, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			UTIL_BloodDrips( pev->origin, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			
		}else if(drawBloodVersionValue == 3){
			SpawnBlood(vecDrawLoc, BloodColor(), flDamage);// a little surface blood.
		}

	}
	else
	{
		//0 = the alpha version
		//1 = weapons.cpp's version
		//2 = unsure?

		if(drawBloodVersionValue == 0){
			//easyPrint("BV\n", 0 ) ;
			UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
			//UTIL_BloodDrips(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
			
			if(drawTripleBloodValue == 1){
				UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
				//UTIL_BloodDrips(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
				UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
				//UTIL_BloodDrips(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(3, 6));
			}


			//UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
			//UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );

		}else if(drawBloodVersionValue == 1){
			//easyPrint("BV\n ", 1 ) ;
			UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
			UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );

			if(drawTripleBloodValue == 1){
				UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
				UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
				UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
				UTIL_BloodDrips( vecDrawLoc, g_vecAttackDir, BloodColor(), (int)flDamage );
			}


		}else if(drawBloodVersionValue == 2){
			//easyPrint("BV\n ", 2 ) ;
			UTIL_BloodDrips( vecDrawLoc, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			UTIL_BloodDrips( pev->origin, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			
			if(drawTripleBloodValue == 1){
				UTIL_BloodDrips( vecDrawLoc, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
				UTIL_BloodDrips( pev->origin, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
				UTIL_BloodDrips( vecDrawLoc, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
				UTIL_BloodDrips( pev->origin, UTIL_RandomBloodVector(), (BYTE)70, RANDOM_LONG(10, 50) );
			}



		}else if(drawBloodVersionValue == 3){
			SpawnBlood(vecDrawLoc, BloodColor(), flDamage);// a little surface blood.
		}

	}
	*/

	//UTIL_BloodStream(vecDrawLoc, UTIL_RandomBloodVector(), BloodColor(), RANDOM_LONG(50, 100));
			

}






//=========================================================
// TraceAttack
//=========================================================

GENERATE_TRACEATTACK_IMPLEMENTATION(CBaseMonster)
{
	if ( pev->takedamage )
	{
		m_LastHitGroup = ptr->iHitgroup;

		switch ( ptr->iHitgroup )
		{
		case HITGROUP_GENERIC:
			break;
		case HITGROUP_HEAD:
			flDamage *= gSkillData.monHead;
			break;
		case HITGROUP_CHEST:
			flDamage *= gSkillData.monChest;
			break;
		case HITGROUP_STOMACH:
			flDamage *= gSkillData.monStomach;
			break;
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			flDamage *= gSkillData.monArm;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			flDamage *= gSkillData.monLeg;
			break;
		default:
			break;
		}



		//MODDD - surrounded by parameter.  The gargantua has customized bleeding.
		
		//MODDD - similar case below, robot requires this CVar to emit black oil blood.
		if( !(this->CanUseGermanModel() && EASY_CVAR_GET(germanRobotBleedsOil)==0 ) ){
			if( useBloodEffect){
				//MODDD!!!!!!
				CBaseEntity::DrawAlphaBlood(flDamage, ptr );
			}
		}

		/*
		if(useBulletHitSound && (pevAttacker != NULL && (pevAttacker->renderfx & (ISPLAYER | ISNPC))) ){
			//UTIL_playFleshHitSound(pev);
		}
		*/


		//MODDD!!!
		//SpawnBlood(ptr->vecEndPos, BloodColor(), flDamage);// a little surface blood.
		
		//NOTE: "SpawnBlood" does the same thing as "UTIL_BloodStream".  Should've seen that sooner, eh, whoops.
		//Apparently, "TraceBleed" draws the blood texture on a nearby surface (floor, wall)...

		//Can TraceBleed all the time with germancensorship off.
		//With german censorship on, check germanRobotDamageDecal before drawing robot blood (oil). 
		//If this monster has a german model replacement but this CVar is off, block the TraceBleed request.
		//Note that "CanUseGermanModel" is always false when GermanCensorship is turned off.
		//If TraceBleed is called with a monster with red blood (no german robot model provided), this will get denied anyways.
		if( !(this->CanUseGermanModel() && EASY_CVAR_GET(germanRobotDamageDecal)==0 ) ){
			TraceBleed( flDamage, vecDir, ptr, bitsDamageType, bitsDamageTypeMod );
		}

		
		AddMultiDamage( pevAttacker, this, flDamage, bitsDamageType, bitsDamageTypeMod );
		
	}//END OF pev->takedamage check

}//END OF CBaseMonster's Traceattack implementation




/*
============
TakeDamage

The damage is coming from inflictor, but get mad at attacker
This should be the only function that ever reduces health.
bitsDamageType indicates the type of damage sustained, ie: DMG_SHOCK

Time-based damage: only occurs while the monster is within the trigger_hurt.
When a monster is poisoned via an arrow etc it takes all the poison damage at once.

GLOBALS ASSUMED SET:  g_iSkillLevel
============
*/

//MODDD - new TakeDamage interception.
//MODDD - TODO. extra idea. Perhaps with a deadflag of DEAD_DYING,
//        DeadTakeDamage should be called too? Just an idea.
GENERATE_TAKEDAMAGE_IMPLEMENTATION(CBaseMonster){

	//TEMP
	//bitsDamageType |= DMG_POISON;

	//
	//if(compareStrings(STRING(this->pev->classname), "monster_human_grunt" )  ){
	//	easyPrintLine("IM HIT YO: %s ::: %d %d", STRING(this->pev->classname), bitsDamageType, bitsDamageTypeMod);
	//}
	//

	float	flTake;
	Vector	vecDir;

	if (!pev->takedamage)
		return 0;


	////virtual BOOL	IsAlive( void ) { return (pev->deadflag != DEAD_DEAD); } inv:  == DEAD_DEAD
	//virtual BOOL	IsAlive( void ) { return (pev->deadflag == DEAD_NO); }       inv:  != DEAD_NO

	//if(!IsAlive())

	//Retail's way here. Only count as dead.
	if(pev->deadflag == DEAD_DEAD)
	{

		/*
		if ( m_MonsterState == MONSTERSTATE_SCRIPT && pev->spawnflag & SF_SCRIPT_NOINTERRUPT )
		{
			//Require the deadflag to be all the way dead, not dying.
			if(pev->deadflag == DEAD_DEAD){
				//okay to proceed, would have in retail.
			}else{
				//not okay. Don't damage scripted entities in a DYING state.
				SetConditions( bits_COND_LIGHT_DAMAGE );
				return 0;
			}
		}
		*/



		//timed damages NOT allowed for corpses.
		bitsDamageType &= ~ DMG_TIMEBASED;
		bitsDamageTypeMod &= ~ DMG_TIMEBASEDMOD;

		return DeadTakeDamage( pevInflictor, pevAttacker, flDamage, bitsDamageType, bitsDamageTypeMod );
	}

	if ( pev->deadflag == DEAD_NO )
	{
		// no pain sound during death animation.
		PainSound();// "Ouch!"
	}

	//!!!LATER - make armor consideration here!
	flTake = flDamage;


	if(pev->deadflag == DEAD_NO){

		if(!blockTimedDamage){
			// set damage type sustained
			m_bitsDamageType |= bitsDamageType;
			m_bitsDamageTypeMod |= bitsDamageTypeMod;
		}else{
			m_bitsDamageType |= (bitsDamageType & ~DMG_TIMEBASED);
			m_bitsDamageTypeMod |= (bitsDamageTypeMod & ~DMG_TIMEBASEDMOD);
		}
	}else{
		//dead?  can't take any timed damages.
		m_bitsDamageType = 0;
		m_bitsDamageTypeMod = 0;
	}


	// grab the vector of the incoming attack. ( pretend that the inflictor is a little lower than it really is, so the body will tend to fly upward a bit).
	vecDir = Vector( 0, 0, 0 );
	if (!FNullEnt( pevInflictor ))
	{
		CBaseEntity *pInflictor = CBaseEntity :: Instance( pevInflictor );
		if (pInflictor)
		{
			vecDir = ( pInflictor->Center() - Vector ( 0, 0, 10 ) - Center() ).Normalize();
			vecDir = g_vecAttackDir = vecDir.Normalize();
		}
	}

	// add to the damage total for clients, which will be sent as a single
	// message at the end of the frame
	// todo: remove after combining shotgun blasts?
	if ( IsPlayer() )
	{
		if ( pevInflictor )
			pev->dmg_inflictor = ENT(pevInflictor);

		pev->dmg_take += flTake;

		// check for godmode or invincibility
		if ( pev->flags & FL_GODMODE )
		{
			return 0;
		}
	}
	
	//if ( !FNullEnt(pevInflictor) && (pevAttacker->solid != SOLID_TRIGGER) )



	// if this is a player, move him around!
	if ( ( !FNullEnt( pevInflictor ) ) && (pev->movetype == MOVETYPE_WALK) && (!pevAttacker || pevAttacker->solid != SOLID_TRIGGER) )
	{
		pev->velocity = pev->velocity + vecDir * -DamageForce( flDamage );
	}



	easyPrintLine("CBaseMonster:: name:%s:%d TOOK DAMAGE. health:%.2f Damage:%.2f Blast:%d Gib:: N:%d A:%d", getClassname(), monsterID, pev->health, flDamage, (bitsDamageType & DMG_BLAST), (bitsDamageType & DMG_NEVERGIB), (bitsDamageType & DMG_ALWAYSGIB) );
	//easyForcePrintLine("TakeDamage. %s:%d health:%.2f gib damge bits: %d %d", this->getClassname(), monsterID, pev->health, (bitsDamageType&DMG_NEVERGIB), (bitsDamageType&DMG_ALWAYSGIB) );
	

	if(global_nothingHurts == 0 || (global_nothingHurts == 2 && Instance(pevAttacker)->IsPlayer() ) ){
		// do the damage

		if(this->blockDamage == TRUE){
			//don't take damage.
			return 0;
		}else if(this->buddhaMode == TRUE){
			//allow the loss of health, but have a minimum of 1 hit point.
			pev->health = max(pev->health - flTake, 1);
		}else{
			//ordinary take-damage.
			pev->health -= flTake;
		}
	}//END Of "nothingHurts" CVar check.

	
	// HACKHACK Don't kill monsters in a script.  Let them break their scripts first
	if ( m_MonsterState == MONSTERSTATE_SCRIPT )
	{
		if(  (!m_pCine || m_pCine->CanInterrupt())  ) {
			SetConditions( bits_COND_LIGHT_DAMAGE );
		}
		return 0;
	}



	
	if ( pev->health <= 0 )
	{
		//MODDD - removing this. We can send the inflictor to killed now.
		//g_pevLastInflictor = pevInflictor;
		

		lastDamageReceived = flDamage;


		attemptResetTimedDamage(TRUE);

		float MYHEALTH = pev->health;
		
		if ( bitsDamageType & DMG_ALWAYSGIB )
		{
			Killed( pevInflictor, pevAttacker, GIB_ALWAYS );
		}
		else if ( bitsDamageType & DMG_NEVERGIB )
		{
			Killed( pevInflictor, pevAttacker, GIB_NEVER );
		}
		else
		{
			Killed( pevInflictor, pevAttacker, GIB_NORMAL );
		}

		//g_pevLastInflictor = NULL;

		return 0;
	}
	//MODDD - else, if not killed by this strike:
	else{

		

		if(!IsPlayer()){

			int myClassify = Classify();
			for (int i = 0; i < CDMG_TIMEBASED; i++){
				//MODDD
				//if (bitsDamageType & (DMG_PARALYZE << i))

				
				if(
					(i == itbd_Radiation && 
					(myClassify == CLASS_MACHINE ||
					(EASY_CVAR_GET(AlienRadiationImmunity) == 1 && 
						(
						myClassify == CLASS_ALIEN_MILITARY ||
						myClassify == CLASS_ALIEN_PASSIVE ||
						myClassify == CLASS_ALIEN_MONSTER ||
						myClassify == CLASS_ALIEN_PREY ||
						myClassify == CLASS_ALIEN_PREDATOR ||
						myClassify == CLASS_PLAYER_BIOWEAPON ||
						myClassify == CLASS_ALIEN_BIOWEAPON ||
						myClassify == CLASS_BARNACLE 
						)
					)
					)
					) ||
					(i == itbd_Poison &&
					myClassify == CLASS_MACHINE  //machines are also immunte to poison.
					)
				){
					continue;  //skip the damage check, this monster does not get this type of damage, at least not timed.
				}

				int* m_bitsDamageTypeRef = 0;
				if(i <= 7){
					//use the old bitmask.
					m_bitsDamageTypeRef = &bitsDamageType;
				}else{
					//use the new bitmask.
					m_bitsDamageTypeRef = &bitsDamageTypeMod;
				}

				//
				if ((*m_bitsDamageTypeRef) & (convert_itbd_to_damage(i) )){
					m_rgbTimeBasedDamage[i] = 0;
					//MODDD - next frame this is brought up will be the first one again.
					m_rgbTimeBasedFirstFrame[i] = TRUE;
				}



			}//END OF for(int i = 0...)

			
		}//END OF not player check.

	}

	// react to the damage (get mad)
	if ( (pev->flags & FL_MONSTER) && !FNullEnt(pevAttacker) )
	{
		if ( pevAttacker->flags & (FL_MONSTER | FL_CLIENT) )
		{// only if the attack was a monster or client!
			


			if( !(bitsDamageTypeMod & (DMG_TIMEDEFFECT|DMG_TIMEDEFFECTIGNORE)) ){
				//MODDD - if this is continual damage (e.g. poison, radiation), don't allow the LKP to be updated! timed damage tells us nothing and shouldn't disturb anything.
				BOOL updatedEnemyLKP = FALSE;  //turn on if we do.

				// enemy's last known position is somewhere down the vector that the attack came from.
				if (pevInflictor)
				{
					//MODDD NOTE - "bits_COND_SEE_ENEMY" appears to still be on for monsters that have a straight line to their enemy but aren't necessarily turned to them.
					//             So the condition being off is kindof an odd choice?
					if (m_hEnemy == NULL || pevInflictor == m_hEnemy->pev || !HasConditions(bits_COND_SEE_ENEMY))
					{
						setEnemyLKP_Investigate(pevInflictor->origin);
						updatedEnemyLKP = TRUE;
					}else{
						//MODDD NOTE -don't update the LKP at all? This will happen if...
						//    (m_hEnemy != NULL && pev->inflictor != m_hEnemy->pev && HasConditions(bits_COND_SEE_ENEMY))
						//...so while looking straight at an enemy or being able to and taking damage from another source,
						//the LKP is unaffected. that is fine.
					}
				}
				else
				{
					setEnemyLKP_Investigate(pev->origin + ( g_vecAttackDir * 64 ));
					updatedEnemyLKP = TRUE;
				}

				//MODDD - If we didn't change the LKP, why look in its direction?
				//...undone, perhaps we should look at the enemy if they damaged us? whatever, err on defalt behavior.
				//if(updatedEnemyLKP){
					MakeIdealYaw( m_vecEnemyLKP );

					//MODDD - also let the enemy know to not get stumped on investigating the LKP.
					// It makes no sense to investigate the LKP set by getting hit by something and give up on
					// going outside of cover to find the enemy. 
					//unstumpable = TRUE;
				//}

			}//END OF timed damage check



			// add pain to the conditions 
			// !!!HACKHACK - fudged for now. Do we want to have a virtual function to determine what is light and 
			// heavy damage per monster class?
			// MODDD - I hear your cries, original devs. So shall it be done!
			// Default behavior for the base monster in OnTakeDamageSetConditions, plus not triggering schedule-interrupting conditions
			// ("Took Damage Recently" or something) for timed damage, which just looks annoying. Why react to predictable damage?
			OnTakeDamageSetConditions(pevInflictor, pevAttacker, flDamage, bitsDamageType, bitsDamageTypeMod);

		}
	}

	return 1;
}//END OF TakeDamage







//MODDD - may incorporate the bitsDamageTypeMod (extra damage bitmask) in the future. For now, just the plain one works.
// take health
int CBaseMonster :: TakeHealth (float flHealth, int bitsDamageType)
{
	if (!pev->takedamage)
		return 0;

	// clear out any damage types we healed.
	// UNDONE: generic health should not heal any
	// UNDONE: time-based damage

	m_bitsDamageType &= ~(bitsDamageType & ~DMG_TIMEBASED);
	
	return CBaseEntity::TakeHealth(flHealth, bitsDamageType);
}





//NO.  See "monsters.cpp", this already exists.
/*
TYPEDESCRIPTION	CBaseMonster::m_SaveData[] = 
{
	DEFINE_ARRAY( CBaseMonster, m_rgbTimeBasedDamage, FIELD_BOOLEAN, CDMG_TIMEBASED ),
	DEFINE_ARRAY( CBaseMonster, m_rgbTimeBasedFirstFrame, FIELD_BOOLEAN, CDMG_TIMEBASED ),
	
	DEFINE_FIELD(CBaseMonster, m_bitsDamageType, FIELD_INTEGER),
	DEFINE_FIELD(CBaseMonster, m_bitsDamageTypeMod, FIELD_INTEGER),
	//bitsDamageType
};
*/




/*
void CBaseMonster::PreThink(void)
{


	CheckTimeBasedDamage();

}
*/
//MonsterThink...?  already used though (but the checkTimeBasedDamage call could be inserted into there instead).  "Think" here is fine I believe.
//OH THIS OILY LITTLE man.  IT CAN SUCK OFF A THOUSAND SHOTGUNS AND BURN IN HELL.
/*
void CBaseMonster::Think(void)
{
	CBaseEntity::Think();
	//easyPrintLine("heeeee");

	
	////MODDD: REVERTMOD

	//THE GODS HAVE FORBIDDEN THOU FROM DOING THIS.  FEEL THY WRATH IN THE FORM OF RANDOM, DIGUSTING, HARD-TO-TRACE CRASHES.
	//
	//if(global_timedDamageAffectsMonsters == 1){
	//	CheckTimeBasedDamage();
	//}
	//

}
*/

//THIS IS UNACCEPTABLE.  FOR SHAAAAAAAME.
/*
void CBaseMonster::CheckTimeBasedDamage(void){

}
*/

//(can't tell why?  Welcome to the club.)




void CBaseMonster::attemptResetTimedDamage(BOOL forceReset){

	if(forceReset || m_bitsDamageType != 0 || m_bitsDamageTypeMod != 0){
		for (int i = 0; i < CDMG_TIMEBASED; i++){
			m_rgbTimeBasedDamage[i] = 0;
			m_rgbTimeBasedFirstFrame[i] = TRUE;
		}

		m_bitsDamageType = 0;
		m_bitsDamageTypeMod = 0;
	}

}







//=========================================================
// DeadTakeDamage - takedamage function called when a monster's
// corpse is damaged.
//=========================================================
//int CBaseMonster :: DeadTakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod )
GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION(CBaseMonster)
{
	Vector			vecDir;

	// grab the vector of the incoming attack. ( pretend that the inflictor is a little lower than it really is, so the body will tend to fly upward a bit).
	vecDir = Vector( 0, 0, 0 );
	if (!FNullEnt( pevInflictor ))
	{
		CBaseEntity *pInflictor = CBaseEntity :: Instance( pevInflictor );
		if (pInflictor)
		{
			vecDir = ( pInflictor->Center() - Vector ( 0, 0, 10 ) - Center() ).Normalize();
			vecDir = g_vecAttackDir = vecDir.Normalize();
		}
	}


	//MODDD - TODO: intriguing... see later.
#if 0// turn this back on when the bounding box issues are resolved.

	pev->flags &= ~FL_ONGROUND;
	pev->origin.z += 1;
	
	// let the damage scoot the corpse around a bit.
	if ( !FNullEnt(pevInflictor) && (pevAttacker->solid != SOLID_TRIGGER) )
	{
		pev->velocity = pev->velocity + vecDir * -DamageForce( flDamage );
	}

#endif

	// kill the corpse if enough damage was done to destroy the corpse and the damage is of a type that is allowed to destroy the corpse.
	if ( bitsDamageType & DMG_GIB_CORPSE )
	{
		//MODDD NOTE - the same "Killed" as running out of health the first time while alive?
		//             That's... an akward design choice. Why not a separate "CorpseKilled" or something?
		//             Other places may need to be aware they may be getting their "Killed" called from a dead monster probably wanting to gib.
		if ( pev->health <= flDamage )
		{
			pev->health = -50;
			Killed( pevInflictor, pevAttacker, GIB_ALWAYS );
			return 0;
		}
		// Accumulate corpse gibbing damage, so you can gib with multiple hits
		pev->health -= flDamage * 0.1;
	}
	
	return 1;
}


float CBaseMonster :: DamageForce( float damage )
{ 
	float force = damage * ((32 * 32 * 72.0) / (pev->size.x * pev->size.y * pev->size.z)) * 5;
	
	if ( force > 1000.0) 
	{
		force = 1000.0;
	}

	return force;
}















/*
//...actually not anymore. Any calls to RadiusDamage, even from RadiusDamageAutoRadius, end up getting redirected to RadiusDamageTest if the RadiusDamageDrawDebug CVar is set.
void CBaseMonster :: RadiusDamageAutoRadiusTest(entvars_t* pevInflictor, entvars_t*	pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType )
{
	::RadiusDamageTest( pev->origin, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, 0 );
}
*/
void RadiusDamageTest( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )
{

	CBaseEntity *pEntity = NULL;
	TraceResult	tr;
	float		flAdjustedDamage, falloff;
	Vector		vecSpot;

	if ( flRadius )
		falloff = flDamage / flRadius;
	else
		falloff = 1.0;

	int bInWater = (UTIL_PointContents ( vecSrc ) == CONTENTS_WATER);

	vecSrc.z += 1;// in case grenade is lying on the ground

	if ( !pevAttacker )
		pevAttacker = pevInflictor;
	



	DebugLine_ClearAll();



	easyForcePrintLine("RadiusDamageTest: Searching... intended damage:%.2f radius:%.2f", flDamage, flRadius);
	// iterate on all entities in the vicinity.
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, vecSrc, flRadius )) != NULL)
	{
		
		//easyForcePrintLine("RadiusDamage: Scanning... %s", pEntity->getClassname());

		if ( pEntity->pev->takedamage != DAMAGE_NO )
		{

			easyForcePrintLine("RadiusDamageTest: Entity in range: %s", pEntity->getClassname());
			//continue;



			// UNDONE: this should check a damage mask, not an ignore
			if ( iClassIgnore != CLASS_NONE && pEntity->Classify() == iClassIgnore )
			{// houndeyes don't hurt other houndeyes with their attack
				continue;
			}

			// blast's don't tavel into or out of water
			if (bInWater && pEntity->pev->waterlevel == 0)
				continue;
			if (!bInWater && pEntity->pev->waterlevel == 3)
				continue;






			easyForcePrintLine("RadiusDamageTest: Calling BodyTarget.");

			vecSpot = pEntity->BodyTarget( vecSrc );
			
			easyForcePrintLine("RadiusDamageTest: Source:(%.2f,%.2f,%.2f) Bodyspot:(%.2f,%.2f,%.2f)", vecSrc.x, vecSrc.y, vecSrc.z, vecSpot.x, vecSpot.y, vecSpot.z);





			UTIL_TraceLine ( vecSrc, vecSpot, dont_ignore_monsters, ENT(pevInflictor), &tr );

			if(pEntity->IsPlayer()){
				DebugLine_Setup(0, vecSrc, vecSpot, tr.flFraction);

			}


			easyForcePrintLine("RadiusDamageTest: Trace test...");

			if ( tr.flFraction == 1.0 || tr.pHit == pEntity->edict() )
			{// the explosion can 'see' this entity, so hurt them!
				
				/*
				if (tr.fStartSolid)
				{
					// if we're stuck inside them, fixup the position and distance
					tr.vecEndPos = vecSrc;
					tr.flFraction = 0.0;
				}
				*/


				easyForcePrintLine("IS IT SOLID?! ss:%d as:%d io:%d", tr.fStartSolid, tr.fAllSolid, tr.fInOpen);

				easyForcePrintLine("RadiusDamageTest: Trace hit successful (hit the target or nothing blocking). Thing hit: %s fract:%.2f", (tr.pHit!=NULL)?STRING(tr.pHit->v.classname):"NULL", tr.flFraction );
				
				// decrease damage for an ent that's farther from the bomb.
				flAdjustedDamage = ( vecSrc - tr.vecEndPos ).Length() * falloff;
				flAdjustedDamage = flDamage - flAdjustedDamage;
			

				
				if ( flAdjustedDamage < 0 )
				{
					flAdjustedDamage = 0;

					easyForcePrintLine("RadiusDamageTest: damage was 0, blocked.");

					//MODDD - waitasec. If we're going to do "0" damage, why bother with any damage effects at all?  Skip acting on this entity.
					continue;
				}

				easyForcePrintLine("RadiusDamageTest: adjDamage:%.2f.", flAdjustedDamage );
				
				// ALERT( at_console, "hit %s\n", STRING( pEntity->pev->classname ) );
				//MODDD - both ways of taking damage send bitsDamageTypeMod.
				if (tr.flFraction != 1.0)
				{
					easyForcePrintLine("RadiusDamageTest: that same trace was a direct hit! Trace attack.");
					ClearMultiDamage( );
					//TODO - why do you put player blood at the blast center sometimes when the player & a baddie are hit at the same time???


					
					if(pEntity->IsPlayer()){

						/*
						CBasePlayer* thisNameSucks = static_cast<CBasePlayer*>(pEntity);
						thisNameSucks->debugVect1Draw = FALSE;
						thisNameSucks->debugVect2Draw = FALSE;
						thisNameSucks->debugVect3Draw = FALSE;
						thisNameSucks->debugVect4Draw = FALSE;
						thisNameSucks->debugVect5Draw = FALSE;

						//
		
						thisNameSucks->debugVect1Start = vecSrc;
						thisNameSucks->debugVect1End = vecSrc + (vecSpot-vecSrc)*tr.flFraction; //(distToClosest+1)*tr.flFraction
						thisNameSucks->debugVect1Draw = TRUE;
						thisNameSucks->debugVect1Success = TRUE;
						
						thisNameSucks->debugVect2Start = tr.vecEndPos + Vector(0, 0, -7);
						thisNameSucks->debugVect2End = tr.vecEndPos + Vector(0, 0, 7);
						thisNameSucks->debugVect2Draw = TRUE;
						thisNameSucks->debugVect2Success = FALSE;

						*/
					}
		

					//tr.vecEndPos

					pEntity->TraceAttack( pevInflictor, flAdjustedDamage, (tr.vecEndPos - vecSrc).Normalize( ), &tr, bitsDamageType, bitsDamageTypeMod );
					ApplyMultiDamage( pevInflictor, pevAttacker );
				}
				else
				{
					easyForcePrintLine("RadiusDamageTest: that same trace was not a direct hit. Forcing a takeDamage call.");
					pEntity->TakeDamage ( pevInflictor, pevAttacker, flAdjustedDamage, bitsDamageType, bitsDamageTypeMod );
				}
			}else{
				easyForcePrintLine("RadiusDamageTest: Trace hit blocked, something in the way? No hit.");
			}
		}
	}
}



























//
// RadiusDamage - this entity is exploding, or otherwise needs to inflict damage upon entities within a certain range.
// 
// only damage ents that can clearly be seen by the explosion!








//From CBaseMonster to assume the position of the monster called on (pev->origin).
void CBaseMonster :: RadiusDamageAutoRadius(entvars_t* pevInflictor, entvars_t*	pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType )
{
	::RadiusDamage( pev->origin, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, 0 );
}
void CBaseMonster :: RadiusDamageAutoRadius(entvars_t* pevInflictor, entvars_t*	pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )
{
	::RadiusDamage( pev->origin, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, bitsDamageTypeMod );
}



//MODDD - these versions now don't belong to CBaseMonster. Why did they? They don't take anything from the monster they are called on.
void RadiusDamageAutoRadius( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType )
{
	::RadiusDamage( vecSrc, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, 0);
}
void RadiusDamageAutoRadius( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )
{
	::RadiusDamage( vecSrc, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, bitsDamageTypeMod );
}
//MODDD - monster versions restored too.
void CBaseMonster::RadiusDamageAutoRadius( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType )
{
	::RadiusDamage( vecSrc, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, 0);
}
void CBaseMonster::RadiusDamageAutoRadius( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )
{
	::RadiusDamage( vecSrc, pevInflictor, pevAttacker, flDamage, flDamage * 2.5, iClassIgnore, bitsDamageType, bitsDamageTypeMod );
}

//MODDD - version that supports bitsDamageTypeMod below. The old one now just passes along a blank (none) bitsDamageTypeMOD value. Healthy default.
void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType )
{
	RadiusDamage(vecSrc, pevInflictor, pevAttacker, flDamage, flRadius, iClassIgnore, bitsDamageType, 0);
}
void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod )
{

	if(EASY_CVAR_GET(RadiusDamageDrawDebug) == 1){
		//pipe it to here instead
		RadiusDamageTest(vecSrc, pevInflictor, pevAttacker, flDamage, flRadius, iClassIgnore, bitsDamageType, bitsDamageTypeMod);
		return;
	}


	CBaseEntity *pEntity = NULL;
	TraceResult	tr;
	float		flAdjustedDamage, falloff;
	Vector		vecSpot;

	if ( flRadius )
		falloff = flDamage / flRadius;
	else
		falloff = 1.0;

	int bInWater = (UTIL_PointContents ( vecSrc ) == CONTENTS_WATER);

	vecSrc.z += 1;// in case grenade is lying on the ground

	if ( !pevAttacker )
		pevAttacker = pevInflictor;


	// iterate on all entities in the vicinity.
	while ((pEntity = UTIL_FindEntityInSphere( pEntity, vecSrc, flRadius )) != NULL)
	{
		//easyForcePrintLine("RadiusDamage: Scanning... %s", pEntity->getClassname());

		if ( pEntity->pev->takedamage != DAMAGE_NO )
		{
			// UNDONE: this should check a damage mask, not an ignore
			if ( iClassIgnore != CLASS_NONE && pEntity->Classify() == iClassIgnore )
			{// houndeyes don't hurt other houndeyes with their attack
				continue;
			}

			// blast's don't tavel into or out of water
			if (bInWater && pEntity->pev->waterlevel == 0)
				continue;
			if (!bInWater && pEntity->pev->waterlevel == 3)
				continue;

			vecSpot = pEntity->BodyTarget( vecSrc );
			
			UTIL_TraceLine ( vecSrc, vecSpot, dont_ignore_monsters, ENT(pevInflictor), &tr );

			if ( tr.flFraction == 1.0 || tr.pHit == pEntity->edict() )
			{// the explosion can 'see' this entity, so hurt them!
				
				
				/*
				//MODDD - no? This is what makes it look like a collision happened at the source of the explosion with the player,
				//  but the player's blood spawns at the source of the explosion instead of where the player was hit.
				//  It's ok to start solid, the rest of the logic works fine. This might be an out-of-date fix from an earlier point of development
				//  in the as-is game.
				if (tr.fStartSolid)
				{
					// if we're stuck inside them, fixup the position and distance
					tr.vecEndPos = vecSrc;
					tr.flFraction = 0.0;
				}
				*/
				
				// decrease damage for an ent that's farther from the bomb.
				flAdjustedDamage = ( vecSrc - tr.vecEndPos ).Length() * falloff;
				flAdjustedDamage = flDamage - flAdjustedDamage;
			
				if ( flAdjustedDamage < 0 )
				{
					flAdjustedDamage = 0;

					//MODDD - waitasec. If we're going to do "0" damage, why bother with any damage effects at all?  Skip acting on this entity.
					continue;
				}
				
				// ALERT( at_console, "hit %s\n", STRING( pEntity->pev->classname ) );
				//MODDD - both ways of taking damage send bitsDamageTypeMod.
				if (tr.flFraction != 1.0)
				{
					ClearMultiDamage( );
					//TODO - why do you put player blood at the blast center sometimes when the player & a baddie are hit at the same time???
					pEntity->TraceAttack( pevInflictor, flAdjustedDamage, (tr.vecEndPos - vecSrc).Normalize( ), &tr, bitsDamageType, bitsDamageTypeMod );
					ApplyMultiDamage( pevInflictor, pevAttacker );
				}
				else
				{
					pEntity->TakeDamage ( pevInflictor, pevAttacker, flAdjustedDamage, bitsDamageType, bitsDamageTypeMod );
				}
			}
		}
	}

	//easyForcePrintLine("I FINISHED OKAY");

}





























/*
================
FireBullets

Go to the trouble of combining multiple pellets into a single damage call.

This version is used by Monsters.
================
*/
void CBaseEntity::FireBullets(ULONG cShots, Vector vecSrc, Vector vecDirShooting, Vector vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker )
{
	static int tracerCount;
	int tracer;
	TraceResult tr;
	Vector vecRight = gpGlobals->v_right;
	Vector vecUp = gpGlobals->v_up;

	if ( pevAttacker == NULL )
		pevAttacker = pev;  // the default attacker is ourselves

	ClearMultiDamage();
	gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;
	for (ULONG iShot = 1; iShot <= cShots; iShot++)
	{
		// get circular gaussian spread
		float x, y, z;
		do {
			x = RANDOM_FLOAT(-0.5,0.5) + RANDOM_FLOAT(-0.5,0.5);
			y = RANDOM_FLOAT(-0.5,0.5) + RANDOM_FLOAT(-0.5,0.5);
			z = x*x+y*y;
		} while (z > 1);

		Vector vecDir = vecDirShooting +
						x * vecSpread.x * vecRight +
						y * vecSpread.y * vecUp;
		Vector vecEnd;

		vecEnd = vecSrc + vecDir * flDistance;
		UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev)/*pentIgnore*/, &tr);

		tracer = 0;
		if (iTracerFreq != 0 && (tracerCount++ % iTracerFreq) == 0)
		{
			Vector vecTracerSrc;

			if ( IsPlayer() )
			{// adjust tracer position for player
				vecTracerSrc = vecSrc + Vector ( 0 , 0 , -4 ) + gpGlobals->v_right * 2 + gpGlobals->v_forward * 16;
			}
			else
			{
				vecTracerSrc = vecSrc;
			}
			
			if ( iTracerFreq != 1 )		// guns that always trace also always decal
				tracer = 1;
			switch( iBulletType )
			{
			case BULLET_MONSTER_MP5:
			case BULLET_MONSTER_9MM:
			case BULLET_MONSTER_12MM:
			default:

				MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, vecTracerSrc );
					WRITE_BYTE( TE_TRACER );
					WRITE_COORD( vecTracerSrc.x );
					WRITE_COORD( vecTracerSrc.y );
					WRITE_COORD( vecTracerSrc.z );
					WRITE_COORD( tr.vecEndPos.x );
					WRITE_COORD( tr.vecEndPos.y );
					WRITE_COORD( tr.vecEndPos.z );
				MESSAGE_END();
				break;
			}
		}



		// do damage, paint decals
		if (tr.flFraction != 1.0)
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
			BOOL bulletHitEffectAllowed = TRUE;
			BOOL doDefaultBulletHitEffectCheck = FALSE;

			if ( iDamage )
			{
				pEntity->TraceAttack(pevAttacker, iDamage, vecDir, &tr, DMG_BULLET | ((iDamage > 16) ? DMG_ALWAYSGIB : DMG_NEVERGIB), 0, TRUE, &bulletHitEffectAllowed);
				
				//TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				//DecalGunshot( &tr, iBulletType );
				doDefaultBulletHitEffectCheck = TRUE;
			} 
			//MODDD - changing how this works. Instead of always playing the textureHit sound, we will let the hit entity determine whether it wants to play
			//        the texture sound / generate a hit decal. For instance, shots that hit helmets and ricochet shouldn't play flesh hit sounds at the same
			//        time like the texture system wants to. That means, bullets merely ALLOW the bullethit sound to be played, but it doesn't have to be.
			//        Acutally, little issue with this approach. TEXTURETYPE_PlaySound needs a fair amount of information (trace result, evSrc, vecEnd, and
			//        our iBulletType). We really don't need another TraceAttack overload just for this
			//        So, little compromise. Instead, traceAttack gets to set the "useBulletHitsound" variable which this method will send by reference.
			//        If the hit entity allows the bullet hit sound to play, then we can call TEXTURETYPE_PlaySound and DecalGunshot from here as usual.
			//        This also means, no need for checking to see if the bitsDamage bitmask includes DMG_BULLET. "useBulletHitSound" being provided at all
			//        and not null alone tells us we're seeing if it is ok to play the sound (typically bullets checking).
			else switch(iBulletType)
			{
			default:
			case BULLET_MONSTER_9MM:
				pEntity->TraceAttack(pevAttacker, gSkillData.monDmg9MM, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				//TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				//DecalGunshot( &tr, iBulletType );
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_MONSTER_MP5:
				pEntity->TraceAttack(pevAttacker, gSkillData.monDmgMP5, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				//TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				//DecalGunshot( &tr, iBulletType );
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_MONSTER_12MM:		
				pEntity->TraceAttack(pevAttacker, gSkillData.monDmg12MM, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);

				//if(EASY_CVAR_GET(tracerMakesBulletHitEffect
				if ( !tracer )
				{
					//TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
					//DecalGunshot( &tr, iBulletType );
					doDefaultBulletHitEffectCheck = TRUE;  //so tracers don't even get a normal shot at this? Isn't that a bit weird? Maybe CVar this.
				}
			break;
			case BULLET_NONE: // FIX 
				pEntity->TraceAttack(pevAttacker, 50, vecDir, &tr, DMG_CLUB, 0, TRUE, &bulletHitEffectAllowed);

				//BULLET_NONE? When would this happen? And different logic for doing decals? why? Not even DMG_BULLET above?
				//Just leaving this logic as it is.

				if(bulletHitEffectAllowed == TRUE){
					TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
					// only decal glass
					if ( !FNullEnt(tr.pHit) && VARS(tr.pHit)->rendermode != 0)
					{
						UTIL_DecalTrace( &tr, DECAL_GLASSBREAK1 + RANDOM_LONG(0,2) );
					}
				}
			break;
			}

			if(doDefaultBulletHitEffectCheck && bulletHitEffectAllowed){
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				DecalGunshot( &tr, iBulletType );
			}


			//MODDD
			if (pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE )
			{
				//easyPrintLine("WHAT IS THE THING I HIT %s", STRING(pEntity->pev->classname) );

			}else{

				//if ( FNullEnt(tr.pHit))
				{

					if(global_muteRicochetSound < 1){
						MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, tr.vecEndPos );
							WRITE_BYTE( TE_GUNSHOT );
							WRITE_COORD( tr.vecEndPos.x );
							WRITE_COORD( tr.vecEndPos.y );
							WRITE_COORD( tr.vecEndPos.z );
						MESSAGE_END();
					}
				}
			}



		}
		// make bullet trails
		UTIL_BubbleTrail( vecSrc, tr.vecEndPos, (flDistance * tr.flFraction) / 64.0 );
	}
	ApplyMultiDamage(pev, pevAttacker);
}










/*
================
FireBulletsPlayer

Go to the trouble of combining multiple pellets into a single damage call.

This version is used by Players, uses the random seed generator to sync client and server side shots.

//MODDD - extra note. Notice that the BULLET_PLAYER... cases below don't do the TEXTURETYPE_PLAYSOUND and DecalGunshot calls. These are both handled by the client instead
//        (see the EV_HLDM_PlayTextureSound method of cl_dll/ev_hldm.cpp).
//        But this can only work for hits on map geometry. Hits on entities, even func_breakable, do not give their texture types for sounds (glass ,etc.).
//        Playing from the server seems to be enough for them (Notice the breakable plays a sound of their own in TakeDamage).
//        Looks like the main idea is, map geometry needs something else to play sounds for them (which is why monsters firing bullets, through the plain FireBullets above, 
//        have to call the aforementionde TEXTURETYPE_PLAYSOUND and DecalGunshot methods: to guarantee a sound plays. Even though this is redundant with the sound hit entities
//        such as, say, func_breakable already make on hit. That may or may not be terrible, but could be correctable. Check if the hit thing is the map, and, if so, play
//        a hit sound because we know the map won't on its own and the client can't (that is only for player-bullet hits). So the monster must in that case. Otherwise trust
//        the hit entity... not the map... handles playing sounds for taking hits correctly.
//        ACTUALLY... looks like most things do expect the attacker/shooter to handle the sound part at least. Func_breakables are more of an exception to play a sound no matter
//        where they are hit. It's fine to make the player play a texturehitsound when not hitting the map (worldspawn) still.

================
*/
Vector CBaseEntity::FireBulletsPlayer ( ULONG cShots, Vector vecSrc, Vector vecDirShooting, Vector vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker, int shared_rand )
{
	static int tracerCount;
	int tracer;
	TraceResult tr;
	Vector vecRight = gpGlobals->v_right;
	Vector vecUp = gpGlobals->v_up;
	float x, y, z;

	if ( pevAttacker == NULL )
		pevAttacker = pev;  // the default attacker is ourselves

	ClearMultiDamage();
	gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;

	for ( ULONG iShot = 1; iShot <= cShots; iShot++ )
	{
		//Use player's random seed.
		// get circular gaussian spread
		x = UTIL_SharedRandomFloat( shared_rand + iShot, -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 1 + iShot ) , -0.5, 0.5 );
		y = UTIL_SharedRandomFloat( shared_rand + ( 2 + iShot ), -0.5, 0.5 ) + UTIL_SharedRandomFloat( shared_rand + ( 3 + iShot ), -0.5, 0.5 );
		z = x * x + y * y;

		Vector vecDir = vecDirShooting +
						x * vecSpread.x * vecRight +
						y * vecSpread.y * vecUp;
		Vector vecEnd;

		vecEnd = vecSrc + vecDir * flDistance;
		UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev)/*pentIgnore*/, &tr);
		

		if(EASY_CVAR_GET(playerWeaponTracerMode) == 2){
			//Render tracer effects serverside, and always.
			iTracerFreq = 1;

			//MODDD - why was this bit removed from the player's FireBullets?
			tracer = 0;
			if (iTracerFreq != 0 && (tracerCount++ % iTracerFreq) == 0)
			{
				Vector vecTracerSrc;

				if ( IsPlayer() )
				{// adjust tracer position for player
					vecTracerSrc = vecSrc + Vector ( 0 , 0 , -4 ) + gpGlobals->v_right * 2 + gpGlobals->v_forward * 16;
				}
				else
				{
					vecTracerSrc = vecSrc;
				}
			
				if ( iTracerFreq != 1 )		// guns that always trace also always decal
					tracer = 1;

				//so... everything? ok then
				switch( iBulletType )
				{
				case BULLET_MONSTER_MP5:
				case BULLET_MONSTER_9MM:
				case BULLET_MONSTER_12MM:
				default:

					MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, vecTracerSrc );
						WRITE_BYTE( TE_TRACER );
						WRITE_COORD( vecTracerSrc.x );
						WRITE_COORD( vecTracerSrc.y );
						WRITE_COORD( vecTracerSrc.z );
						WRITE_COORD( tr.vecEndPos.x );
						WRITE_COORD( tr.vecEndPos.y );
						WRITE_COORD( tr.vecEndPos.z );
					MESSAGE_END();
					break;
				}
			}
		
		}//END OF playerWeaponTracerMode check









		//easyPrintLine("IS IT NULL????? %d", (CBaseEntity::Instance(tr.pHit)  == NULL) );
		if(CBaseEntity::Instance(tr.pHit)  != NULL){
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

			
			//easyPrintLine("NAME::: %s", STRING(pEntity->pev->classname) );
		}




		//easyPrintLine("flFraction?", tr.flFraction); 
		// do damage, paint decals
		if (tr.flFraction != 1.0)
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);
			BOOL bulletHitEffectAllowed = TRUE; //by default.
			BOOL doDefaultBulletHitEffectCheck = FALSE;  //set to TRUE if a case relies on a common default for this. Leave FALSE if the case handles this itself.

			//m_MonsterState == MONSTERSTATE_SCRIPT
			//!?!?

			//tr.pHit->v.origin
			//Note that this is an "AI Sound", or not a real one audible to the player, but one that checks for monsters nearby (distance) and alerts them if they are in hearing range.
			attemptSendBulletSound(tr.vecEndPos, pevAttacker);


			//easyPrintLine("FireBulletsPlayer: iDamage: %d PLAYER BULLET TYPE?! %d THING HIT: %s", iDamage, iBulletType, pEntity->getClassname());
			if ( iDamage )
			{
				//MODDD NOTE
				//Why does this area, completely unused (the player never uses "iDamage" in FirePlayerBullets, relies in iBulletType to get a default damage value and pick from a below case),
				//have the TEXTURETYPE_PlaySound and DecalGunshot calls that the NPC's FireBullets method has? The world may never know.
				pEntity->TraceAttack(pevAttacker, iDamage, vecDir, &tr, DMG_BULLET | ((iDamage > 16) ? DMG_ALWAYSGIB : DMG_NEVERGIB), 0, TRUE, &bulletHitEffectAllowed);
				//TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				//DecalGunshot( &tr, iBulletType );
				
				doDefaultBulletHitEffectCheck = TRUE;
			} 
			else switch(iBulletType)
			{
			default:
			case BULLET_PLAYER_9MM:	
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmg9MM, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				//If what we hit was an entity, we need to play the sound from the server. Clientside's texture sound player won't catch this.
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_PLAYER_MP5:	
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmgMP5, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_PLAYER_BUCKSHOT:	
				 // make distance based!
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmgBuckshot, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_PLAYER_357:		
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmg357, vecDir, &tr, DMG_BULLET, 0, TRUE, &bulletHitEffectAllowed);
				doDefaultBulletHitEffectCheck = TRUE;
			break;
			case BULLET_NONE: // FIX

				pEntity->TraceAttack(pevAttacker, 50, vecDir, &tr, DMG_CLUB, 0, TRUE, &bulletHitEffectAllowed);
				
				//if( !FClassnameIs(pEntity->pev, "worldspawn") && bulletHitEffectAllowed){
				if(bulletHitEffectAllowed){
					TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
					// only decal glass
					if ( !FNullEnt(tr.pHit) && VARS(tr.pHit)->rendermode != 0)
					{
						UTIL_DecalTrace( &tr, DECAL_GLASSBREAK1 + RANDOM_LONG(0,2) );
					}
				}

				break;
			}//END OF switch

			//MODDD TODO - remove the "worldspawn" check and have worldspawn itself (CWorld, world.cpp) override TraceAttack to disallow if it is the player making the
			//             request? That sounds neat.

			

			//MODDD TODO - switch! Same for the above worldspawn check too.
			//Also, if the "playerBulletHitEffectForceServer" CVar is set to 1, the client won't make hitsound / decal effects in ev_hldm.cpp. Instead, it will happen here serverside
			//to be broadcast to all clients like all other effects (by NPCs, etc.).
			
			
			//if(doDefaultBulletHitEffectCheck && (EASY_CVAR_GET(playerBulletHitEffectForceServer)==1 || !FClassnameIs(pEntity->pev, "worldspawn")) && bulletHitEffectAllowed){
			//if(doDefaultBulletHitEffectCheck && !FClassnameIs(pEntity->pev, "worldspawn") && bulletHitEffectAllowed){

			if(doDefaultBulletHitEffectCheck && bulletHitEffectAllowed){
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				DecalGunshot( &tr, iBulletType );
			}




			/*
			if(iDamage == 0 &&
				iBulletType != BULLET_NONE &&
				( !FNullEnt(tr.pHit)) &&
				!FClassnameIs(pEntity->pev, "worldspawn"))
			{
				//Play a sound since the client won't.
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				if(VARS(tr.pHit)->rendermode != 0)){
					DecalGunshot( &tr, iBulletType );
				}
			}
			*/


			
			//MODDD
			if (pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE )
			{
				//easyPrintLine("WHAT IS THE THING I HIT %s", STRING(pEntity->pev->classname) );

			}else{

				//if ( FNullEnt(tr.pHit))
				{
					
					if(global_muteRicochetSound < 1){
						MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, tr.vecEndPos );
							WRITE_BYTE( TE_GUNSHOT );
							WRITE_COORD( tr.vecEndPos.x );
							WRITE_COORD( tr.vecEndPos.y );
							WRITE_COORD( tr.vecEndPos.z );
						MESSAGE_END();
					}
					
					
				}
			}
			
		}//END OF if (tr.flFraction != 1.0)

		
		//easyPrintLine("NULL?? %d", FNullEnt(tr.pHit) );

		//COME BACK
			


		// make bullet trails
		UTIL_BubbleTrail( vecSrc, tr.vecEndPos, (flDistance * tr.flFraction) / 64.0 );
	}
	ApplyMultiDamage(pev, pevAttacker);

	return Vector( x * vecSpread.x, y * vecSpread.y, 0.0 );
}



void CBaseEntity :: TraceBleed( float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType )
{
	TraceBleed(flDamage, vecDir, ptr, bitsDamageType, 0);

}

void CBaseEntity :: TraceBleed( float flDamage, Vector vecDir, TraceResult *ptr, int bitsDamageType, int bitsDamageTypeMod )
{
	if (BloodColor() == DONT_BLEED)
		return;
	
	if (flDamage == 0)
		return;

	//can involve "bitsDamageTypeMod" if necessary too.
	//Seems safe to imply any bleeding damage (DMG_BLEEDING for the bitsDamageTypeMod new bitmask) will come with DMG_SLASH so that blood is dealt.
	//Eh, letting bleeding damage allow the effect. why not.
	if (
		!(bitsDamageType & (DMG_CRUSH | DMG_BULLET | DMG_SLASH | DMG_BLAST | DMG_CLUB | DMG_MORTAR)) &&
		!( bitsDamageTypeMod & (DMG_BLEEDING) )
		)
		return;
	
	// make blood decal on the wall! 
	TraceResult Bloodtr;
	Vector vecTraceDir; 
	float flNoise;
	int cCount;
	int i;

/*
	if ( !IsAlive() )
	{
		// dealing with a dead monster. 
		if ( pev->max_health <= 0 )
		{
			// no blood decal for a monster that has already decalled its limit.
			return; 
		}
		else
		{
			pev->max_health--;
		}
	}
*/

	if (flDamage < 10)
	{
		flNoise = 0.1;
		cCount = 1;
	}
	else if (flDamage < 25)
	{
		flNoise = 0.2;
		cCount = 2;
	}
	else
	{
		flNoise = 0.3;
		cCount = 4;
	}

	for ( i = 0 ; i < cCount ; i++ )
	{
		vecTraceDir = vecDir * -1;// trace in the opposite direction the shot came from (the direction the shot is going)

		vecTraceDir.x += RANDOM_FLOAT( -flNoise, flNoise );
		vecTraceDir.y += RANDOM_FLOAT( -flNoise, flNoise );
		vecTraceDir.z += RANDOM_FLOAT( -flNoise, flNoise );

		//MODDD - TODO: vecTraceDir * -172?   Doesn't that make the "* -1" above redundant (the minus in -172)?  Why this double negative?
		UTIL_TraceLine( ptr->vecEndPos, ptr->vecEndPos + vecTraceDir * -172, ignore_monsters, ENT(pev), &Bloodtr);

		if ( Bloodtr.flFraction != 1.0 )
		{
			UTIL_BloodDecalTrace( &Bloodtr, BloodColor() );
		}
	}
}

//=========================================================
//=========================================================
void CBaseMonster :: MakeDamageBloodDecal ( int cCount, float flNoise, TraceResult *ptr, const Vector &vecDir )
{
	// make blood decal on the wall! 
	TraceResult Bloodtr;
	Vector vecTraceDir; 
	int i;

	if ( !IsAlive() )
	{
		// dealing with a dead monster. 
		if ( pev->max_health <= 0 )
		{
			// no blood decal for a monster that has already decalled its limit.
			return; 
		}
		else
		{
			pev->max_health--;
		}
	}

	for ( i = 0 ; i < cCount ; i++ )
	{
		vecTraceDir = vecDir;

		vecTraceDir.x += RANDOM_FLOAT( -flNoise, flNoise );
		vecTraceDir.y += RANDOM_FLOAT( -flNoise, flNoise );
		vecTraceDir.z += RANDOM_FLOAT( -flNoise, flNoise );

		UTIL_TraceLine( ptr->vecEndPos, ptr->vecEndPos + vecTraceDir * 172, ignore_monsters, ENT(pev), &Bloodtr);

/*
		MESSAGE_BEGIN( MSG_BROADCAST, SVC_TEMPENTITY );
			WRITE_BYTE( TE_SHOWLINE);
			WRITE_COORD( ptr->vecEndPos.x );
			WRITE_COORD( ptr->vecEndPos.y );
			WRITE_COORD( ptr->vecEndPos.z );
			
			WRITE_COORD( Bloodtr.vecEndPos.x );
			WRITE_COORD( Bloodtr.vecEndPos.y );
			WRITE_COORD( Bloodtr.vecEndPos.z );
		MESSAGE_END();
*/

		if ( Bloodtr.flFraction != 1.0 )
		{
			UTIL_BloodDecalTrace( &Bloodtr, BloodColor() );
		}
	}
}
