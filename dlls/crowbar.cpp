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





//MODDD TODO - why is behavior inconsistent on hitting an area with missing piecs of the hitbox (func_pusable at least)?
//Example: goto a3a1, go forwards and to the left in the room with the chair and the computer.  Push the chair away and get on top of it
//         (noclip or jumpForceMulti).  crouch or stand near the top and look down and crowbar.  It's possible to rapid crowbar but still
//         do miss sounds, no hits.  sigh I thought this was fixed everywhere, guess that was just for map geometry.











#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"

#include "custom_debug.h"




#define	CROWBAR_BODYHIT_VOLUME 128
#define	CROWBAR_WALLHIT_VOLUME 512

LINK_ENTITY_TO_CLASS( weapon_crowbar, CCrowbar );



enum gauss_e {
	CROWBAR_IDLE = 0,
	CROWBAR_DRAW,
	CROWBAR_HOLSTER,
	CROWBAR_ATTACK1HIT,
	CROWBAR_ATTACK1MISS,
	CROWBAR_ATTACK2MISS,
	CROWBAR_ATTACK2HIT,
	CROWBAR_ATTACK3MISS,
	CROWBAR_ATTACK3HIT,
	//MODDD - two extras present in the model, may as well make available.
	CROWBAR_IDLE2,
	CROWBAR_IDLE3
};


void CCrowbar::Spawn( )
{
	Precache( );
	m_iId = WEAPON_CROWBAR;
	SET_MODEL(ENT(pev), "models/w_crowbar.mdl");
	m_iClip = -1;

	FallInit();// get ready to fall down.
}


void CCrowbar::Precache( void )
{
	PRECACHE_MODEL("models/v_crowbar.mdl");
	PRECACHE_MODEL("models/w_crowbar.mdl");
	PRECACHE_MODEL("models/p_crowbar.mdl");
	PRECACHE_SOUND("weapons/cbar_hit1.wav");
	PRECACHE_SOUND("weapons/cbar_hit2.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod1.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod2.wav");
	PRECACHE_SOUND("weapons/cbar_hitbod3.wav");
	PRECACHE_SOUND("weapons/cbar_miss1.wav");

	precacheGunPickupSound();

	m_usCrowbar = PRECACHE_EVENT ( 1, "events/crowbar.sc" );
}

int CCrowbar::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = NULL;
	p->iMaxAmmo1 = -1;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 0;
	p->iPosition = 0;
	p->iId = WEAPON_CROWBAR;
	p->iWeight = CROWBAR_WEIGHT;
	return 1;
}



BOOL CCrowbar::Deploy( )
{

	return DefaultDeploy( "models/v_crowbar.mdl", "models/p_crowbar.mdl", CROWBAR_DRAW, "crowbar", 0, 0, (13.0/24.0), -1 );
}

void CCrowbar::Holster( int skiplocal /* = 0 */ )
{
	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
	//SendWeaponAnim( CROWBAR_HOLSTER );

	DefaultHolster(CROWBAR_HOLSTER, skiplocal, 0, (13.0f/24.0f) );
}


//MODDD - little issue here. Never to bother to report whether we found ANY intersection.
//In such a case, the trace result is returned in the way it was found but there is no way to tell that this is due to failure.
//Checking to see if the trace changed at all seems hacky and may have a 1 in a billion chance of being wrong.
//Just return a Boolean: found a point at all so it can be trusted that the trace now holds that, or no suitable point was found.
BOOL FindHullIntersection( const Vector &vecSrc, TraceResult &tr, float *mins, float *maxs, edict_t *pEntity )
{
	BOOL success = FALSE;
	int			i, j, k;
	float		distance;
	float		*minmaxs[2] = {mins, maxs};
	TraceResult tmpTrace;
	Vector		vecHullEnd = tr.vecEndPos;
	Vector		vecEnd;

	distance = 1e6f;



	vecHullEnd = vecSrc + ((vecHullEnd - vecSrc)*2);
	UTIL_TraceLine( vecSrc, vecHullEnd, dont_ignore_monsters, pEntity, &tmpTrace );
	if ( tmpTrace.flFraction < 1.0 )
	{
		//alread hit a point? That must mean it's ok.
		tr = tmpTrace;
		return TRUE;
	}

	for ( i = 0; i < 2; i++ )
	{
		for ( j = 0; j < 2; j++ )
		{
			for ( k = 0; k < 2; k++ )
			{
				vecEnd.x = vecHullEnd.x + minmaxs[i][0];
				vecEnd.y = vecHullEnd.y + minmaxs[j][1];
				vecEnd.z = vecHullEnd.z + minmaxs[k][2];

				UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, pEntity, &tmpTrace );
				if ( tmpTrace.flFraction < 1.0 )
				{
					float thisDistance = (tmpTrace.vecEndPos - vecSrc).Length();
					if ( thisDistance < distance )
					{
						success = TRUE; //found something.
						tr = tmpTrace;
						distance = thisDistance;
					}
					//easyForcePrintLine("FindHullIntersection: YAY!!!!!");
				}else{
					//easyForcePrintLine("FindHullIntersection: NO.");
				}
			}
		}
	}

	return success;
}//END OF FindHullIntersection


void CCrowbar::PrimaryAttack()
{
	if (! Swing( 1 ))
	{
		SetThink( &CCrowbar::SwingAgain );
		pev->nextthink = gpGlobals->time + 0.1;
	}
}


void CCrowbar::Smack( )
{
	DecalGunshot( &m_trHit, BULLET_PLAYER_CROWBAR );
}


void CCrowbar::SwingAgain( void )
{
	Swing( 0 );
}











#ifndef CLIENT_DLL


Vector FireBulletsPlayerEh ( ULONG cShots, Vector vecSrc, Vector vecDirShooting, Vector vecSpread, float flDistance, int iBulletType, int iTracerFreq, int iDamage, entvars_t *pevAttacker, int shared_rand )
{
	static int tracerCount;
	TraceResult tr;
	Vector vecRight = gpGlobals->v_right;
	Vector vecUp = gpGlobals->v_up;
	float x, y, z;


	entvars_t* otherPev = pevAttacker;

	//always send!
	//if ( pevAttacker == NULL )
	//	pevAttacker = pev;  // the default attacker is ourselves

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
		UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(otherPev)/*pentIgnore*/, &tr);
		
		//easyPrintLine("flFraction?", tr.flFraction); 
		// do damage, paint decals



		//easyPrintLine("IS IT NULL????? %d", (CBaseEntity::Instance(tr.pHit)  == NULL) );
		if(CBaseEntity::Instance(tr.pHit)  != NULL){
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

			
			//easyPrintLine("NAME::: %s", STRING(pEntity->pev->classname) );
		}



		if (tr.flFraction != 1.0)
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);


			pEntity->TraceAttack(pevAttacker, iDamage, vecDir, &tr, DMG_CLUB | ((iDamage > 16) ? DMG_ALWAYSGIB : DMG_NEVERGIB) );
				
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				DecalGunshot( &tr, iBulletType );



			/*
			//easyPrintLine("PLAYER BULLET TYPE?! %d", iBulletType);
			if ( iDamage )
			{
				
				pEntity->TraceAttack(pevAttacker, iDamage, vecDir, &tr, DMG_BULLET | ((iDamage > 16) ? DMG_ALWAYSGIB : DMG_NEVERGIB) );
				
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				DecalGunshot( &tr, iBulletType );
				
			} 
			else switch(iBulletType)
			{
			default:
			case BULLET_PLAYER_9MM:	
				//easyPrintLine("BULLET TRACE?!");
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmg9MM, vecDir, &tr, DMG_BULLET); 
				break;

			case BULLET_PLAYER_MP5:		
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmgMP5, vecDir, &tr, DMG_BULLET); 
				break;

			case BULLET_PLAYER_BUCKSHOT:	
				 // make distance based!
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmgBuckshot, vecDir, &tr, DMG_BULLET); 
				break;
			
			case BULLET_PLAYER_357:		
				pEntity->TraceAttack(pevAttacker, gSkillData.plrDmg357, vecDir, &tr, DMG_BULLET); 
				break;
				
			case BULLET_NONE: // FIX


				pEntity->TraceAttack(pevAttacker, 50, vecDir, &tr, DMG_CLUB);
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecEnd, iBulletType);
				// only decal glass
				if ( !FNullEnt(tr.pHit) && VARS(tr.pHit)->rendermode != 0)
				{
					UTIL_DecalTrace( &tr,    25   + RANDOM_LONG(0,2) ); // "25" was "DECAL_GLASSBREAK1"
				}

				break;
			}
			
			//MODDD
			if (pEntity && pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE )
			{
				//easyPrintLine("WHAT IS THE THING I HIT %s", STRING(pEntity->pev->classname) );

			}else{

				//if ( FNullEnt(tr.pHit))
				{
					//IF REVERTING TO THIS, try to use global_muteRicochetSound instead!
					if(CVAR_GET_FLOAT("muteRicochetSound") != 2){
						MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, tr.vecEndPos );
							WRITE_BYTE( TE_GUNSHOT );
							WRITE_COORD( tr.vecEndPos.x );
							WRITE_COORD( tr.vecEndPos.y );
							WRITE_COORD( tr.vecEndPos.z );
						MESSAGE_END();
					}
					
				}
			}
			*/
		}//END OF if (tr.flFraction != 1.0)

		
		//easyPrintLine("NULL?? %d", FNullEnt(tr.pHit) );

		//COME BACK
			


		// make bullet trails
		UTIL_BubbleTrail( vecSrc, tr.vecEndPos, (flDistance * tr.flFraction) / 64.0 );
	}
	ApplyMultiDamage(otherPev, pevAttacker);

	return Vector( x * vecSpread.x, y * vecSpread.y, 0.0 );
}

#endif













int CCrowbar::Swing( int fFirst )
{
	BOOL specialMiss = FALSE;
	int fDidHit = FALSE;

	TraceResult tr;

	UTIL_MakeVectors (m_pPlayer->pev->v_angle);
	Vector vecSrc	= m_pPlayer->GetGunPosition( );
	Vector vecEnd	= vecSrc + gpGlobals->v_forward * 32; //32;

	//UTIL_TraceLine(vecSrc, vecEnd, dont_ignore_monsters, ENT(pev)/*pentIgnore*/, &tr);
	
	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, ENT( m_pPlayer->pev ), &tr );

	//MODDD - idle in one second from an attack.
	//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;



	
	
#ifndef CLIENT_DLL

	//easyForcePrintLine("-------------------------------------------------");



	//easyForcePrintLine("HERE HE GO A fl:%.2f ss:%d io:%d fas:%d hit:%s", tr.flFraction, tr.fStartSolid, tr.fInOpen, tr.fAllSolid, (tr.pHit!=NULL)?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL");
	
	if ( tr.flFraction >= 1.0 )
	{
		UTIL_TraceHull( vecSrc, vecEnd, dont_ignore_monsters, head_hull, ENT( m_pPlayer->pev ), &tr );
		if ( tr.flFraction < 1.0 )
		{
			// Calculate the point of intersection of the line (or hull) and the object we hit
			// This is and approximation of the "best" intersection
			
			//easyForcePrintLine("HERE HE GO B1 fl:%.2f ss:%d io:%d fas:%d hit:%s", tr.flFraction, tr.fStartSolid, tr.fInOpen, tr.fAllSolid, (tr.pHit!=NULL)?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL");
	
			
			
			////DebugLine_ClearAll
			//::DebugLine_Setup(0, Vector(tr.vecEndPos) + Vector(7, 0, 0), Vector(tr.vecEndPos) + Vector(-7, 0, 0), 255, 0, 0 );
			//::DebugLine_Setup(1, Vector(tr.vecEndPos) + Vector(0, -7, 0), Vector(tr.vecEndPos) + Vector(0, 7, 0), 255, 0, 0 );

			
			CBaseEntity *pHit = CBaseEntity::Instance( tr.pHit );
			if ( !pHit || pHit->IsBSPModel() ){
				/*
				if(pHit){
					easyForcePrintLine("!!!!!!!!!!!!!!!!!! %d", pHit->IsBSPModel() );
				}
				*/
				BOOL hullSuccess = FindHullIntersection( vecSrc, tr, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX, m_pPlayer->edict() );
				if(!hullSuccess){
					//If the HullIntersection failed to map a point, this is a failure to hit.
					specialMiss = TRUE;
				}
			}
			vecEnd = tr.vecEndPos;	// This is the point on the actual surface (the hull could have hit space)

			/*
			m_pPlayer->debugVect3Draw = TRUE;
			m_pPlayer->debugVect3Start = Vector(tr.vecEndPos) + Vector(7, 0, 0);
			m_pPlayer->debugVect3End = Vector(tr.vecEndPos) + Vector(-7, 0, 0);
			m_pPlayer->debugVect3Success = TRUE;
			m_pPlayer->debugVect4Draw = TRUE;
			m_pPlayer->debugVect4Start = Vector(tr.vecEndPos) + Vector(0, -7, 0);
			m_pPlayer->debugVect4End = Vector(tr.vecEndPos) + Vector(0, 7, 0);
			m_pPlayer->debugVect4Success = TRUE;
			*/

			//easyForcePrintLine("HERE HE GO B2 fl:%.2f ss:%d io:%d fas:%d hit:%s", tr.flFraction, tr.fStartSolid, tr.fInOpen, tr.fAllSolid, (tr.pHit!=NULL)?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL");
	
			/*
			if(tr.flFraction == 0){
				//Moved no distance from the player? This is suspect. Count as a miss.
				//This can happen from attacking close to the edge of a wall but parallel to it, particularly around a corner ever so slightly.
				//The hull just collides at its first point without moving at all. Not really possible for a real hit, especially after that FindHullIntersection.
				//OUT OF DATE WAY OF CHECKING, look above for that specialMiss = TRUE line.
				specialMiss = TRUE;
			}
			*/

		}
	}
	
#endif
	





	//( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	//swingMissChoice

	PLAYBACK_EVENT_FULL( FEV_NOTHOST, m_pPlayer->edict(), m_usCrowbar, 
	0.0, (float *)&g_vecZero, (float *)&g_vecZero, 0.0, 0.0, m_fInAttack, 0, FALSE, FALSE );


#ifndef CLIENT_DLL
	//Vector vecWhoCares = FireBulletsPlayerEh( 1, vecSrc, gpGlobals->v_forward, VECTOR_CONE_1DEGREES, 32, BULLET_PLAYER_9MM, 0, 0, m_pPlayer->pev, m_pPlayer->random_seed );
#endif

#ifndef CLIENT_DLL
	//easyForcePrintLine("HERE HE GO C fl:%.2f ss:%d io:%d fas:%d hit:%s", tr.flFraction, tr.fStartSolid, tr.fInOpen, tr.fAllSolid, (tr.pHit!=NULL)?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL");
#endif
	//easyForcePrintLine("OH my man WHAT YOU HIT %s", (tr.pHit!=NULL)?FClassname(CBaseEntity::Instance(tr.pHit)):"NULL");

	//MODDD - special way to miss.
	if ( tr.flFraction >= 1.0 || specialMiss )
	{

		

		if (fFirst)
		{

			/*
			#if CLIENT_DLL == 0
			swingMissChoice = (m_iSwingMiss++) % 3;
			#endif
			*/

//#if CLIENT_DLL
			switch(m_fInAttack)
			{
			case 0:
				//SendWeaponAnimBypass( CROWBAR_ATTACK1MISS, 1 );
				m_flTimeWeaponIdle = 0 + (10.0/22.0) + randomIdleAnimationDelay();
			break;
			case 1:
				//SendWeaponAnimBypass( CROWBAR_ATTACK2MISS, 1 );
				m_flTimeWeaponIdle = 0 + (13.0/22.0) + randomIdleAnimationDelay();
			break;
			case 2:
				//SendWeaponAnimBypass( CROWBAR_ATTACK3MISS, 1 );
				m_flTimeWeaponIdle = 0 + (18.0/24.0) + randomIdleAnimationDelay();
			 break;
			}
//#endif
			if(m_pPlayer->cheat_minimumfiredelayMem == 0){
				// miss
				m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
			}else{
				m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
			}

			// player "shoot" animation
			m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
			
			
			#ifndef CLIENT_DLL
				m_fInAttack = (++m_fInAttack) % 3;
				//easyPrintLine("SERVAH");
			#else
				//easyPrintLine("CLIENTT");
			#endif
			

		}

		

	}
	else
	{

		

		//int choicetemp2 = ((m_iSwing++) %3);
		//easyPrintLine("bee %d", choicetemp2);
		//switch( ((m_iSwing++) % 2) + 1 )
		switch( m_fireState )
		{
		case 0:
			SendWeaponAnim( CROWBAR_ATTACK1HIT );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (11.0/22.0) + randomIdleAnimationDelay();
			 break;
		case 1:
			SendWeaponAnim( CROWBAR_ATTACK2HIT );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (14.0/22.0) + randomIdleAnimationDelay();
			 break;
		case 2:
			SendWeaponAnim( CROWBAR_ATTACK3HIT );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (19.0/24.0) + randomIdleAnimationDelay();
			 break;
		}
		
		#ifndef CLIENT_DLL
			m_fireState = (++m_fireState) % 3;
			//easyPrintLine("SERVAH");
		#else
			//easyPrintLine("CLIENTT");
		#endif




		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );
		//at least do these things?
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;

		//easyPrintLine("efefefefefef");
		//return TRUE;

#ifndef CLIENT_DLL

		// hit
		fDidHit = TRUE;
		CBaseEntity *pEntity = CBaseEntity::Instance(tr.pHit);

		ClearMultiDamage( );

		BOOL useBulletHitSound;

		if ( (m_flNextPrimaryAttack + 1 < UTIL_WeaponTimeBase() ) || g_pGameRules->IsMultiplayer() )
		{
			// first swing does full damage... or apparently, Multiplayer lets all swings do full damage?
			pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgCrowbar, gpGlobals->v_forward, &tr, DMG_CLUB, 0, TRUE, &useBulletHitSound ); 
		}
		else
		{
			// subsequent swings do half
			pEntity->TraceAttack(m_pPlayer->pev, gSkillData.plrDmgCrowbar / 2, gpGlobals->v_forward, &tr, DMG_CLUB, 0,  TRUE, &useBulletHitSound ); 
		}	
		ApplyMultiDamage( m_pPlayer->pev, m_pPlayer->pev );

		// play thwack, smack, or dong sound
		float flVol = 1.0;
		int fHitWorld = TRUE;



		/*
		easyForcePrintLine("DID I HIT SOMETHING?");
		if(pEntity){
			easyForcePrintLine("YES: %d %d", pEntity->IsWorld(), pEntity->IsWorldAffiliated());
		}else{
			easyForcePrintLine("NO.");
		}
		*/

		if (pEntity && !pEntity->IsWorld() && !pEntity->IsWorldAffiliated() )
		{

			//MODDD - let's just have a Classify not NONE check and organic check instead.
			//if ( pEntity->Classify() != CLASS_NONE && pEntity->Classify() != CLASS_MACHINE )
			if(pEntity->Classify() != CLASS_NONE && pEntity->isOrganic() == TRUE)
			{

				if(useBulletHitSound){

					// play thwack or smack sound
					switch( RANDOM_LONG(0,2) )
					{
					case 0:
						EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod1.wav", 1, ATTN_NORM); break;
					case 1:
						EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod2.wav", 1, ATTN_NORM); break;
					case 2:
						EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hitbod3.wav", 1, ATTN_NORM); break;
					}
				}

				//This now always happens for flesh (organic) hits.
				m_pPlayer->m_iWeaponVolume = CROWBAR_BODYHIT_VOLUME;

				if ( !pEntity->IsAlive() ){
					//no, wait until the end...  use what it was going to return anyways instead.
					//also this was already set to TRUE by default. whatever.
					fDidHit = TRUE;
						//return TRUE;
				}
				else{
					flVol = 0.1;
				}


			}else{
				float fvolbar = 0;
				//doens't have any sort of Classify() section or inorganic? Make the usual crowbar metal-hitting sound.
				//easyForcePrintLine("ILL realize I just hit some metallic enemy");

				//Hitting a metalic foe? Do closer to full volume.
				fvolbar = 0.71;

				if(!useBulletHitSound){
					//reduce the sound instead.
					fvolbar = 0.34;
				}



				//!!!!!
				//For now, metallics don't care about "useBulletHitSound", ricochet or not they play this. ... besides a little volume reduction on the hit sound actually.

				// also play crowbar strike
				switch( RANDOM_LONG(0,1) )
				{
				case 0:
					EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit1.wav", fvolbar, ATTN_NORM, 0, 103 + RANDOM_LONG(0,3)); 
					break;
				case 1:
					EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit2.wav", fvolbar, ATTN_NORM, 0, 103 + RANDOM_LONG(0,3)); 
					break;
				}

				//MODDD - moved here, also for hitting metalic monsters. 0.6 multiple since that is a common reduction given by textures on world hits anyways.
				m_pPlayer->m_iWeaponVolume = flVol * CROWBAR_WALLHIT_VOLUME*0.6;

			}//END OF else OF class / organic check
			

			//hit some typical monster, or something clearly not part of the static map. As in, not even something like a prop.
			fHitWorld = FALSE;

		}//END OF hit pEntity (not NULL) check

		// play texture hit sound
		// UNDONE: Calculate the correct point of intersection when we hit with the hull instead of the line

		if (fHitWorld)
		{
			float fvolbar = TEXTURETYPE_PlaySound(&tr, vecSrc, vecSrc + (vecEnd-vecSrc)*2, BULLET_PLAYER_CROWBAR);

			if ( g_pGameRules->IsMultiplayer() )
			{
				// override the volume here, cause we don't play texture sounds in multiplayer, 
				// and fvolbar is going to be 0 from the above call.

				fvolbar = 1;
			}

			//easyForcePrintLine("ILL realize I hit the world or somethign world-related, here is my hitsound volume %.2f", fvolbar);

			//fvolbar = 1;

			// also play crowbar strike
			switch( RANDOM_LONG(0,1) )
			{
			case 0:
				EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit1.wav", fvolbar, ATTN_NORM, 0, 98 + RANDOM_LONG(0,3)); 
				break;
			case 1:
				EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/cbar_hit2.wav", fvolbar, ATTN_NORM, 0, 98 + RANDOM_LONG(0,3)); 
				break;
			}

			/*
			//MODDD - new.  Also serverside, make the particle effect when hitting a non-monster:
			//NO.  The crowbar won't do this now.
			MESSAGE_BEGIN( MSG_PAS, SVC_TEMPENTITY, tr.vecEndPos );
				WRITE_BYTE( TE_GUNSHOT );
				WRITE_COORD( tr.vecEndPos.x );
				WRITE_COORD( tr.vecEndPos.y );
				WRITE_COORD( tr.vecEndPos.z );
			MESSAGE_END();
			*/

			// delay the decal a bit
			m_trHit = tr;

			//MODDD - moved here, now just for hitting the world.
			m_pPlayer->m_iWeaponVolume = flVol * CROWBAR_WALLHIT_VOLUME;

		}//END OF if(fHitWorld)






		//??? INVESTIGATE W/ PRINTOUTS
		/*
		if(!(pEntity || fHitWorld)){
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.7;
		}
		*/

		//MODDD - moved to the above if-then.
		//m_pPlayer->m_iWeaponVolume = flVol * CROWBAR_WALLHIT_VOLUME;


#endif

		//MODDD
		if(m_pPlayer->cheat_minimumfiredelayMem == 0){
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25;
		}else{
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;
		}

		
		
		SetThink( &CCrowbar::Smack );
		pev->nextthink = UTIL_WeaponTimeBase() + 0.2;

	}
	return fDidHit;
}



//MODDD - added.
void CCrowbar::WeaponIdle( void )
{
	//ResetEmptySound( );

	//m_pPlayer->GetAutoaimVector( AUTOAIM_5DEGREES );


	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	int iAnim;
	//MODDD NOTE - "m_flTimeWeaponIdle" changes per "iAnim" choice are additions.
	switch ( RANDOM_LONG( 0, 2 ) )
	{
	case 0:	
		iAnim = CROWBAR_IDLE;
		m_flTimeWeaponIdle = (35.0/13.0) + randomIdleAnimationDelay();
	break;
	default:
	case 1:
		iAnim = CROWBAR_IDLE2;
		m_flTimeWeaponIdle = (80.0/15.0) + randomIdleAnimationDelay();
	break;
	case 2:
		iAnim = CROWBAR_IDLE3;
		m_flTimeWeaponIdle = (80.0/15.0) + randomIdleAnimationDelay();
	break;
	}

	SendWeaponAnim( iAnim );

	//MODDD - why was this random?
	//m_flTimeWeaponIdle = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 ); // how long till we do this again.
}