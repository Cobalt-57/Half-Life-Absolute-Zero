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

===== player.cpp ========================================================

  functions dealing with the player

*/

#include "extdll.h"
#include "util.h"

#include "cbase.h"
#include "player.h"
#include "trains.h"
#include "nodes.h"
#include "weapons.h"
#include "soundent.h"
#include "basemonster.h"
#include "shake.h"
#include "decals.h"
#include "gamerules.h"
#include "game.h"
#include "hltv.h"

#include "custom_debug.h"

// #define DUCKFIX

EASY_CVAR_EXTERN(friendlyPianoFollowVolume)
EASY_CVAR_EXTERN(playerUseDrawDebug)
EASY_CVAR_EXTERN(playerFadeOutRate)


EASY_CVAR_EXTERN(holsterAnims)




extern DLL_GLOBAL ULONG		g_ulModelIndexPlayer;
extern DLL_GLOBAL BOOL		g_fGameOver;
extern DLL_GLOBAL	BOOL	g_fDrawLines;
int gEvilImpulse101;
extern DLL_GLOBAL int		g_iSkillLevel, gDisplayTitle;


BOOL gInitHUD = TRUE;

extern void CopyToBodyQue(entvars_t* pev);
extern void respawn(entvars_t *pev, BOOL fCopyCorpse);
extern Vector VecBModelOrigin(entvars_t *pevBModel );
extern edict_t *EntSelectSpawnPoint( CBaseEntity *pPlayer );

// the world node graph
extern CGraph	WorldGraph;

#define TRAIN_ACTIVE	0x80 
#define TRAIN_NEW		0xc0
#define TRAIN_OFF		0x00
#define TRAIN_NEUTRAL	0x01
#define TRAIN_SLOW		0x02
#define TRAIN_MEDIUM	0x03
#define TRAIN_FAST		0x04 
#define TRAIN_BACK		0x05

#define	FLASH_DRAIN_TIME	 1.2 //100 units/3 minutes
#define	FLASH_CHARGE_TIME	 0.2 // 100 units/20 seconds  (seconds per unit)

//MODDD
extern float cheat_barnacleEatsEverything;

extern float global_myStrobe;
extern unsigned short g_sFreakyLight;

extern float global_raveEffectSpawnInterval;
extern float global_germanCensorship;

extern float global_mutePlayerPainSounds;
extern float global_geigerChannel;

extern float global_drawDebugBloodTrace;

extern float global_python_zoomfov;
extern float global_crossbow_zoomfov;
extern float global_canApplyDefaultFOV;
extern float global_auto_adjust_fov_aspect;
extern float global_auto_adjust_zoomfov;

extern float global_autoSneaky;
extern float global_infiniteLongJumpCharge;
extern float global_cheat_infiniteclip;
extern float global_cheat_infiniteammo;
extern float global_cheat_minimumfiredelay;
extern float global_cheat_minimumfiredelaycustom;
extern float global_cheat_nogaussrecoil;
extern float global_gaussRecoilSendsUpInSP;
extern float global_cheat_touchNeverExplodes;

extern float global_endlessFlashlightBattery;

extern float global_normalSpeedMulti;
extern float global_noclipSpeedMulti;
extern float global_jumpForceMulti;

extern float global_timedDamageEndlessOnHard;


extern float global_drawNodeAll;
extern float global_drawNodeSpecial;
extern float global_drawNodeConnections;
extern float global_drawNodeAlternateTime;
extern float global_nodeSearchStartVerticalOffset;

extern float globalPSEUDO_forceFirstPersonIdleDelay;

extern float global_timedDamageDeathRemoveMode;

extern float globalPSEUDO_cameraMode;
extern float globalPSEUDO_aspectratio_determined_fov;

extern float global_mirrorsDoNotReflectPlayer;

extern float global_barnacleCanGib;

extern float global_canDropInSinglePlayer;
extern float global_timedDamageIgnoresArmor;
extern float global_itemBatteryPrerequisite;





extern float global_timedDamageDisableViewPunch;
extern float global_batteryDrainsAtDeath;
extern float global_batteryDrainsAtAdrenalineMode;
//extern float global_canTakeLongJump;
extern float global_printOutCommonTimables;
extern float global_playerBrightLight;
extern float global_disablePainPunchAutomatic;
extern float global_timedDamageReviveRemoveMode;

extern float global_playerExtraPainSoundsMode;



//MASS!  Try to remove those that are not used in here..
///////////////////////////////////////////////////////////////////////////////////


extern float global_weaponPickupPlaysAnyReloadSounds;


extern float global_playerReviveInvincibilityTime;
extern float global_playerReviveBuddhaMode;
extern float global_playerReviveTimeBlocksTimedDamage;

extern float global_drawDebugCine;

extern float global_wpn_glocksilencer;

extern float global_painFlashIgnoreArmor;

EASY_CVAR_EXTERN(nothingHurts)
EASY_CVAR_EXTERN(RadiusDamageDrawDebug)
EASY_CVAR_EXTERN(customLogoSprayMode)




///////////////////////////////////////////////////////////////////////////////////





// Global Savedata for player
TYPEDESCRIPTION	CBasePlayer::m_playerSaveData[] = 
{
	//MODDDREMOVE - don't see anything here different?
	DEFINE_FIELD(CBasePlayer, m_flFlashLightTime, FIELD_TIME),
	DEFINE_FIELD(CBasePlayer, m_iFlashBattery, FIELD_INTEGER),

	DEFINE_FIELD(CBasePlayer, m_afButtonLast, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_afButtonPressed, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_afButtonReleased, FIELD_INTEGER),

	DEFINE_ARRAY(CBasePlayer, m_rgItems, FIELD_INTEGER, MAX_ITEMS),
	DEFINE_FIELD(CBasePlayer, m_afPhysicsFlags, FIELD_INTEGER),

	DEFINE_FIELD(CBasePlayer, m_flTimeStepSound, FIELD_TIME),
	DEFINE_FIELD(CBasePlayer, m_flTimeWeaponIdle, FIELD_TIME),
	DEFINE_FIELD(CBasePlayer, m_flSwimTime, FIELD_TIME),
	DEFINE_FIELD(CBasePlayer, m_flDuckTime, FIELD_TIME),
	DEFINE_FIELD(CBasePlayer, m_flWallJumpTime, FIELD_TIME),

	DEFINE_FIELD(CBasePlayer, m_flSuitUpdate, FIELD_TIME),
	DEFINE_ARRAY(CBasePlayer, m_rgSuitPlayList, FIELD_INTEGER, CSUITPLAYLIST),
	DEFINE_FIELD(CBasePlayer, m_iSuitPlayNext, FIELD_INTEGER),
	DEFINE_ARRAY(CBasePlayer, m_rgiSuitNoRepeat, FIELD_INTEGER, CSUITNOREPEAT),
	DEFINE_ARRAY(CBasePlayer, m_rgflSuitNoRepeatTime, FIELD_TIME, CSUITNOREPEAT),
	DEFINE_FIELD(CBasePlayer, m_lastDamageAmount, FIELD_INTEGER),

	DEFINE_ARRAY(CBasePlayer, m_rgpPlayerItems, FIELD_CLASSPTR, MAX_ITEM_TYPES),
	DEFINE_FIELD(CBasePlayer, m_pActiveItem, FIELD_CLASSPTR),
	DEFINE_FIELD(CBasePlayer, m_pLastItem, FIELD_CLASSPTR),

	DEFINE_ARRAY(CBasePlayer, m_rgAmmo, FIELD_INTEGER, MAX_AMMO_SLOTS),
	DEFINE_FIELD(CBasePlayer, m_idrowndmg, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_idrownrestored, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_tSneaking, FIELD_TIME),

	DEFINE_FIELD(CBasePlayer, m_iTrain, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_bitsHUDDamage, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_flFallVelocity, FIELD_FLOAT),
	DEFINE_FIELD(CBasePlayer, m_iTargetVolume, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_iWeaponVolume, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_iExtraSoundTypes, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_iWeaponFlash, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_fLongJump, FIELD_BOOLEAN),
	DEFINE_FIELD(CBasePlayer, m_fInitHUD, FIELD_BOOLEAN),
	
	//MODDD - moved to monsters.cpp for saving.
	//DEFINE_FIELD(CBasePlayer, m_tbdPrev, FIELD_TIME),

	DEFINE_FIELD(CBasePlayer, m_pTank, FIELD_EHANDLE),
	DEFINE_FIELD(CBasePlayer, m_iHideHUD, FIELD_INTEGER),
	DEFINE_FIELD(CBasePlayer, m_iFOV, FIELD_INTEGER),

	//MODDD - NEW VAR
	DEFINE_FIELD(CBasePlayer, airTankAirTime, FIELD_FLOAT),
	DEFINE_FIELD(CBasePlayer, longJumpCharge, FIELD_FLOAT),
	DEFINE_FIELD(CBasePlayer, m_fLongJumpMemory, FIELD_BOOLEAN),
	DEFINE_FIELD(CBasePlayer, longJumpDelay, FIELD_FLOAT),
	DEFINE_FIELD(CBasePlayer, longJump_waitForRelease, FIELD_BOOLEAN),
	DEFINE_FIELD(CBasePlayer, lastDuckVelocityLength, FIELD_FLOAT),
	//DEFINE_FIELD(CBasePlayer, lastDuckVelocityLength, FIELD_FLOAT),
	
	//DEFINE_FIELD(CBasePlayer, glockSilencerOnVar, FIELD_FLOAT),
	//DEFINE_FIELD(CBasePlayer, egonAltFireOnVar, FIELD_FLOAT),
	
	DEFINE_FIELD(CBasePlayer, recoveryIndex, FIELD_INTEGER),
	
	DEFINE_FIELD(CBasePlayer, recentlyGibbed, FIELD_BOOLEAN),
	DEFINE_FIELD(CBasePlayer, airTankWaitingStart, FIELD_BOOLEAN),
	DEFINE_FIELD(CBasePlayer, hasLongJumpItem, FIELD_BOOLEAN),

	//moved to all CbaseMonster's now.
	//DEFINE_ARRAY( CBasePlayer, m_rgbTimeBasedFirstFrame, FIELD_BOOLEAN, CDMG_TIMEBASED ),
	
	
	DEFINE_ARRAY(CBasePlayer, m_rgSuitPlayListDuration, FIELD_FLOAT, CSUITPLAYLIST),

	DEFINE_FIELD(CBasePlayer, foundRadiation, FIELD_BOOLEAN),
	
	//MODDD - no longer saved.
	//DEFINE_FIELD(CBasePlayer, fvoxOn, FIELD_BOOLEAN),
	
	DEFINE_FIELD(CBasePlayer, myRef_barnacleEatsEverything, FIELD_FLOAT),

	//is that really necessary?
	//DEFINE_FIELD(CBasePlayer, playerBrightLightMem, FIELD_FLOAT),
	DEFINE_FIELD(CBasePlayer, alreadySpawned, FIELD_BOOLEAN),
	
	
	DEFINE_FIELD( CBasePlayer, m_fNoPlayerSound, FIELD_INTEGER ), // NOTE: added back in.  No issue.
	
	DEFINE_FIELD( CBasePlayer, hasGlockSilencer, FIELD_INTEGER ),
	


	
		
//MODDD - rope things. Save info.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CLIENT_DLL
	DEFINE_FIELD( CBasePlayer, m_pRope, FIELD_CLASSPTR ),
	DEFINE_FIELD( CBasePlayer, m_flLastClimbTime, FIELD_TIME ),
	DEFINE_FIELD( CBasePlayer, m_bIsClimbing, FIELD_BOOLEAN ),
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//DEFINE_FIELD( CBasePlayer, m_fDeadTime, FIELD_FLOAT ), // only used in multiplayer games
	//DEFINE_FIELD( CBasePlayer, m_fGameHUDInitialized, FIELD_INTEGER ), // only used in multiplayer games
	//DEFINE_FIELD( CBasePlayer, m_flStopExtraSoundTime, FIELD_TIME ),
	//DEFINE_FIELD( CBasePlayer, m_fKnownItem, FIELD_INTEGER ), // reset to zero on load
	//DEFINE_FIELD( CBasePlayer, m_iPlayerSound, FIELD_INTEGER ),	// Don't restore, set in Precache()
	//DEFINE_FIELD( CBasePlayer, m_pentSndLast, FIELD_EDICT ),	// Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_flSndRoomtype, FIELD_FLOAT ),	// Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_flSndRange, FIELD_FLOAT ),	// Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_fNewAmmo, FIELD_INTEGER ), // Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_flgeigerRange, FIELD_FLOAT ),	// Don't restore, reset in Precache()
	//DEFINE_FIELD( CBasePlayer, m_flgeigerDelay, FIELD_FLOAT ),	// Don't restore, reset in Precache()
	//DEFINE_FIELD( CBasePlayer, m_igeigerRangePrev, FIELD_FLOAT ),	// Don't restore, reset in Precache()
	//DEFINE_FIELD( CBasePlayer, m_iStepLeft, FIELD_INTEGER ), // Don't need to restore
	//DEFINE_ARRAY( CBasePlayer, m_szTextureName, FIELD_CHARACTER, CBTEXTURENAMEMAX ), // Don't need to restore
	//DEFINE_FIELD( CBasePlayer, m_chTextureType, FIELD_CHARACTER ), // Don't need to restore
	//DEFINE_FIELD( CBasePlayer, m_fNoPlayerSound, FIELD_BOOLEAN ), // Don't need to restore, debug
	//DEFINE_FIELD( CBasePlayer, m_iUpdateTime, FIELD_INTEGER ), // Don't need to restore
	//DEFINE_FIELD( CBasePlayer, m_iClientHealth, FIELD_INTEGER ), // Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_iClientBattery, FIELD_INTEGER ), // Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_iClientHideHUD, FIELD_INTEGER ), // Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_fWeapon, FIELD_BOOLEAN ),  // Don't restore, client needs reset
	//DEFINE_FIELD( CBasePlayer, m_nCustomSprayFrames, FIELD_INTEGER ), // Don't restore, depends on server message after spawning and only matters in multiplayer
	//DEFINE_FIELD( CBasePlayer, m_vecAutoAim, FIELD_VECTOR ), // Don't save/restore - this is recomputed
	//DEFINE_ARRAY( CBasePlayer, m_rgAmmoLast, FIELD_INTEGER, MAX_AMMO_SLOTS ), // Don't need to restore
	//DEFINE_FIELD( CBasePlayer, m_fOnTarget, FIELD_BOOLEAN ), // Don't need to restore
	//DEFINE_FIELD( CBasePlayer, m_nCustomSprayFrames, FIELD_INTEGER ), // Don't need to restore
	
};	


//MODDD - LinkUserMessages and message ID's moved to client.cpp.


LINK_ENTITY_TO_CLASS( player, CBasePlayer );

//MODDD - new
inline
void CBasePlayer::resetLongJumpCharge(){

	if(m_fLongJump){
		//Can render an empty spring icon.
		longJumpCharge = 0;
	}else{
		//don't even render to GUI.
		longJumpCharge = -1;
	}

}

void CBasePlayer :: PainSound( void )
{
	
	float	flRndSound;//sound randomizer

	//disallow making noise if this CVar is on.
	if(global_mutePlayerPainSounds == 1 || global_playerExtraPainSoundsMode == 2){
		//playerExtraPainSoundsMode of 2 suggets that we don't want to use the default PainSound method at all.
		return;
	}

	flRndSound = RANDOM_FLOAT ( 0 , 1 ); 
	
	if ( flRndSound <= 0.33 )
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain5.wav", 1, ATTN_NORM, TRUE);
	else if ( flRndSound <= 0.66 )	
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain6.wav", 1, ATTN_NORM, TRUE);
	else
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain7.wav", 1, ATTN_NORM, TRUE);
}


//A chance of 3 / 5 to play pain, 2 / 5 nothing.  Spotted across the script, condensed here for ease of (re)use.
void CBasePlayer :: PainChance( void )
{

	//disallow making noise if this CVar is on.
	if(global_mutePlayerPainSounds == 1){
		return;
	}

		//NOTICE that #4 and #5 are possible (which, as of writing, do nothing at all: no sound).
	switch (RANDOM_LONG(1,5)) 
	{
	case 1: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain5.wav", 1, ATTN_NORM, TRUE);
		break;
	case 2: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain6.wav", 1, ATTN_NORM, TRUE);
		break;
	case 3: 
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain7.wav", 1, ATTN_NORM, TRUE);
		break;
	}

}



/* 
 *
 */
Vector VecVelocityForDamage(float flDamage)
{
	Vector vec(RANDOM_FLOAT(-100,100), RANDOM_FLOAT(-100,100), RANDOM_FLOAT(200,300));

	if (flDamage > -50)
		vec = vec * 0.7;
	else if (flDamage > -200)
		vec = vec * 2;
	else
		vec = vec * 10;
	
	return vec;
}

#if 0 /*
static void ThrowGib(entvars_t *pev, char *szGibModel, float flDamage)
{
	edict_t *pentNew = CREATE_ENTITY();
	entvars_t *pevNew = VARS(pentNew);

	pevNew->origin = pev->origin;
	SET_MODEL(ENT(pevNew), szGibModel);
	UTIL_SetSize(pevNew, g_vecZero, g_vecZero);

	pevNew->velocity		= VecVelocityForDamage(flDamage);
	pevNew->movetype		= MOVETYPE_BOUNCE;
	pevNew->solid			= SOLID_NOT;
	pevNew->avelocity.x		= RANDOM_FLOAT(0,600);
	pevNew->avelocity.y		= RANDOM_FLOAT(0,600);
	pevNew->avelocity.z		= RANDOM_FLOAT(0,600);
	CHANGE_METHOD(ENT(pevNew), em_think, SUB_Remove);
	pevNew->ltime		= gpGlobals->time;
	pevNew->nextthink	= gpGlobals->time + RANDOM_FLOAT(10,20);
	pevNew->frame		= 0;
	pevNew->flags		= 0;
}
	
	
static void ThrowHead(entvars_t *pev, char *szGibModel, floatflDamage)
{
	SET_MODEL(ENT(pev), szGibModel);
	pev->frame			= 0;
	pev->nextthink		= -1;
	pev->movetype		= MOVETYPE_BOUNCE;
	pev->takedamage		= DAMAGE_NO;
	pev->solid			= SOLID_NOT;
	pev->view_ofs		= Vector(0,0,8);
	UTIL_SetSize(pev, Vector(-16,-16,0), Vector(16,16,56));
	pev->velocity		= VecVelocityForDamage(flDamage);
	pev->avelocity		= RANDOM_FLOAT(-1,1) * Vector(0,600,0);
	pev->origin.z -= 24;
	ClearBits(pev->flags, FL_ONGROUND);
}


*/ 
#endif

int TrainSpeed(int iSpeed, int iMax)
{
	float fSpeed, fMax;
	int iRet = 0;

	fMax = (float)iMax;
	fSpeed = iSpeed;

	fSpeed = fSpeed/fMax;

	if (iSpeed < 0)
		iRet = TRAIN_BACK;
	else if (iSpeed == 0)
		iRet = TRAIN_NEUTRAL;
	else if (fSpeed < 0.33)
		iRet = TRAIN_SLOW;
	else if (fSpeed < 0.66)
		iRet = TRAIN_MEDIUM;
	else
		iRet = TRAIN_FAST;

	return iRet;
}

//MODDD - DeathSound method that takes no 
void CBasePlayer :: DeathSound( void ){
	//assume a revive is not planned.
	DeathSound(FALSE);
}


void CBasePlayer :: DeathSound( BOOL plannedRevive )
{
	// water death sounds
	//MODDDREMOVE
	if (pev->waterlevel == 3)
	{
		EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/h2odeath.wav", 1, ATTN_NONE, TRUE);
		return;
	}

	// temporarily using pain sounds for death sounds
	//NOTICE that #4 and #5 are possible (which, as of writing, do nothing at all: no sound).
	//MODDD - redirected...
	PainChance();

	//MODDD
	//Try something more specific later?
	
	// play one of the suit death alarms
	if(plannedRevive == FALSE){
		//play the usual beeps and flatline.  "Cause of Death" FVOX (player suit voice), if 
		//implemented, will probably be played when not planning a revive too.
		EMIT_GROUPNAME_SUIT(ENT(pev), "HEV_DEAD");
	}else{

		/*
		float fvol = CVAR_GET_FLOAT("suitvolume");
		if (fvol > 0.05){

			//SENTENCEG_PlayRndSz(ENT(pev), "HEV_DEAD_ADR", fvol, ATTN_NORM, 0, PITCH_NORM);

			//example?
			//SENTENCEG_PlayRndSz( ENT(pev), "HG_ALERT", HGRUNT_SENTENCE_VOLUME, GRUNT_ATTN, 0, m_voicePitch);
			//WRONG.
			//EMIT_SOUND_DYN(ENT(pev), CHAN_STATIC, "HEV_DEAD_ADR", fvol, ATTN_NORM, 0, PITCH_NORM);
		}
		//EMIT_SOUND_SUIT(ENT(pev), "HEV_DEAD_ADR");
		//for now, don't play the flatline.
		//SetSuitUpdate("!HEV_DEAD_ADR", FALSE, SUIT_NEXT_IN_30SEC);
		*/
		EMIT_GROUPNAME_SUIT(ENT(pev), "HEV_DEADALT");

	}


}

// override takehealth
// bitsDamageType indicates type of damage healed. 

int CBasePlayer :: TakeHealth( float flHealth, int bitsDamageType )
{
	return CBaseMonster :: TakeHealth (flHealth, bitsDamageType);

}

Vector CBasePlayer :: GetGunPosition( )
{
//	UTIL_MakeVectors(pev->v_angle);
//	m_HackedGunPos = pev->view_ofs;
	Vector origin;
	
	origin = pev->origin + pev->view_ofs;

	return origin;
}
Vector CBasePlayer::GetGunPositionAI(){
	return GetGunPosition();
}













//=========================================================
// TraceAttack
//=========================================================
//MODDD - uses the maximum args to guarantee this version ends up getting called.
GENERATE_TRACEATTACK_IMPLEMENTATION(CBasePlayer)
{


	//easyPrintLine("IM BLEEEEEEEEEEEEEEEEEEDIN");
	
	//Just a print-out that I needed.
	/*
	easyPrint("Yes.......%d\n", 0);
	int eye = 0;
	for (eye = 1; eye < MAX_AMMO_SLOTS; eye++)
	{
		easyPrint("HELP %s\n", CBasePlayerItem::AmmoInfoArray[eye].pszName);

	}
	*/



	
	//easyForcePrintLine("AddMultiDamage CALL FROM TRACEATTACK. Attacker:%s Victim:%s hitgrp:%d Dmg:%.2f", pevAttacker!=NULL?STRING(pevAttacker->classname):"NULL", this->getClassname(), ptr->iHitgroup, flDamage);


	if ( pev->takedamage )
	{
		m_LastHitGroup = ptr->iHitgroup;

		switch ( ptr->iHitgroup )
		{
		case HITGROUP_GENERIC:
			break;
		case HITGROUP_HEAD:
			flDamage *= gSkillData.plrHead;
			break;
		case HITGROUP_CHEST:
			flDamage *= gSkillData.plrChest;
			break;
		case HITGROUP_STOMACH:
			flDamage *= gSkillData.plrStomach;
			break;
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			flDamage *= gSkillData.plrArm;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			flDamage *= gSkillData.plrLeg;
			break;
		default:
			break;
		}



		//MODDD
		//SpawnBlood(ptr->vecEndPos, BloodColor(), flDamage);// a little surface blood.


		if(EASY_CVAR_GET(RadiusDamageDrawDebug) == 1)DebugLine_Setup(1, ptr->vecEndPos + Vector(0, 0, -20), ptr->vecEndPos + Vector(0, 0, 20), 0, 0, 255);
		DrawAlphaBlood(flDamage, ptr->vecEndPos);


		//already seems to play?  Verify!
		/*
		if(useBulletHitSound && (pevAttacker != NULL && (pevAttacker->renderfx & (ISPLAYER | ISNPC))) ){
			UTIL_playFleshHitSound(pev);
		}
		*/

		//easyForcePrintLine("AddMultiDamage CALL FROM TRACEATTACK. Attacker:%s Victim:%s hitgrp:%d Dmg:%.2f", pevAttacker!=NULL?STRING(pevAttacker->classname):"NULL", this->getClassname(), ptr->iHitgroup, flDamage);
		
		TraceBleed( flDamage, vecDir, ptr, bitsDamageType, bitsDamageTypeMod );
		AddMultiDamage( pevAttacker, this, flDamage, bitsDamageType, bitsDamageTypeMod );
	}//END OF if pev->takedamage

}//END OF TraceAttack

/*
	Take some damage.  
	NOTE: each call to TakeDamage with bitsDamageType set to a time-based damage
	type will cause the damage time countdown to be reset.  Thus the ongoing effects of poison, radiation
	etc are implemented with subsequent calls to TakeDamage using DMG_GENERIC.
*/

#define ARMOR_RATIO	 0.2	// Armor Takes 80% of the damage
#define ARMOR_BONUS  0.5	// Each Point of Armor is work 1/x points of health



//MODDD - this one NOW accepts the "unsigned int".  It may be used to better convey a greater power of 2.
//Otherwise, capacity is wasted on negatives (unused).
GENERATE_TAKEDAMAGE_IMPLEMENTATION(CBasePlayer)
{
	//easyPrintLine("PLAYER TAKEDAMAGE FLAGS: %d, %d", bitsDamageType, bitsDamageTypeMod);
	


	
	if(reviveSafetyTime != -1){
		
		if(reviveSafetyTime >= gpGlobals->time){
			if(global_playerReviveBuddhaMode == 1){
				buddhaMode = TRUE;
			}else{
				blockDamage = TRUE;
			}
			if(global_playerReviveTimeBlocksTimedDamage == 1){
				blockTimedDamage = TRUE;
			}
		}else{
			reviveSafetyTime = -1;
			buddhaMode = FALSE;
			blockDamage = FALSE;
		}

	}


	//MODDD - don't make these noises if waiting for a revive.
	//If dead, don't play.
	//Also, was this intended ONLY for fall damage?  If so, just make that the requirement.
	
	if(IsAlive()){
		//must be alive to play ANY paint sound effects at all, regardless of setting.
		//Note that a fatal fall will play the sound effect on its own, and it will not need to be played here.
		BOOL pass = FALSE;

		//no chance at passing if hit sounds are muted.
		if(global_mutePlayerPainSounds != 1){

			if(global_playerExtraPainSoundsMode == 0){
				//no extra pain sounds.
				
			}/*else if(global_playerExtraPainSoundsMode == 1){
				if(bitsDamageType & (DMG_FALL) ){
					pass = TRUE;
				}
				
			}*/
			else if(global_playerExtraPainSoundsMode == 1 || global_playerExtraPainSoundsMode == 2 ){
				pass = TRUE;
			}
		
		}//END OF if(global_mutePlayerPainSounds != 1))

		if(pass){
			//AMD889 ADDED THIS, is it the old landing stuff?
			//MODDD - redirected:
			//PainSound();  pain sounds?
			PainChance();
		}

		//} //END OF if(bitsDamageType & (DMG_FALL)
	}//END OF if(!IsAlive)
	

	// have suit diagnose the problem - ie: report damage type
	int bitsDamage = bitsDamageType;
	int bitsDamageMod = bitsDamageTypeMod;
	int ffound = TRUE;
	int fmajor;
	int fcritical;
	int fTookDamage;
	int ftrivial;
	float flRatio;
	float flBonus;
	float flHealthPrev = pev->health;

	flBonus = ARMOR_BONUS;
	flRatio = ARMOR_RATIO;

	if ( ( bitsDamageType & DMG_BLAST ) && g_pGameRules->IsMultiplayer() )
	{
		// blasts damage armor more.
		flBonus *= 2;
	}

	// Already dead
	if ( !IsAlive() )
		return 0;
	// go take the damage first

	
	CBaseEntity *pAttacker = CBaseEntity::Instance(pevAttacker);

	if ( !g_pGameRules->FPlayerCanTakeDamage( this, pAttacker ) )
	{
		// Refuse the damage
		return 0;
	}

	// keep track of amount of damage last sustained
	m_lastDamageAmount = flDamage;

	// Armor. 
	//MODDD - new if-then includes the possibility of marked "DMG_TIMEBASED" (the whole mask being used, not just one piece) ignoring damage or not.
	//if (pev->armorvalue && !(bitsDamageType & (DMG_FALL | DMG_DROWN)) )// armor doesn't protect against fall or drown damage!
	
	//easyPrintLine("B4 DMG : %.2f", flDamage);
	
	
	//flDamage = 2;   //force all to 2?
	
	//easyPrintLine("DAMAGE PRE DETAILS %d %.8f", fTookDamage, flDamage);


	//How much damage was sustained since the last sendoff?  Like pev->dmg_take, but this gets only raw damage before doing armor damage reductions.
	rawDamageSustained += flDamage;


	if (pev->armorvalue && !(bitsDamageType & (DMG_FALL | DMG_DROWN)) && 
		!(bitsDamage & DMG_ARMORBLOCKEXCEPTION || bitsDamageMod & DMG_ARMORBLOCKEXCEPTIONMOD ) &&
		(  !(bitsDamageMod & DMG_TIMEDEFFECTIGNORE)   &&   (global_timedDamageIgnoresArmor == 0 || !(bitsDamageMod & (DMG_TIMEDEFFECT) ) ))
		
	)// armor doesn't protect against fall or drown damage!  ... or "DMG_TIMEDEFFECTIGNORE".
	{
		float flNew = flDamage * flRatio;

		float flArmor;

		flArmor = (flDamage - flNew) * flBonus;

		//easyPrintLine("ARMOR RED: %.2f", flArmor);

		// Does this use more armor than we have?
		if (flArmor > pev->armorvalue)
		{
			flArmor = pev->armorvalue;
			flArmor *= (1/flBonus);
			flNew = flDamage - flArmor;
			pev->armorvalue = 0;
		}
		else
			pev->armorvalue -= flArmor;
		
		flDamage = flNew;
	}

	// this cast to INT is critical!!! If a player ends up with 0.5 health, the engine will get that
	// as an int (zero) and think the player is dead! (this will incite a clientside screentilt, etc)
	//MODDD - added the new "bitsDamageMod".  Nothing else seems to rely on either bitmask, but this can't hurt.

	
	
	if(EASY_CVAR_GET(nothingHurts) > 0){
		//nothing hurts, the player does not take damage with this cheat on.
		flDamage = 0;
	}

	fTookDamage = GENERATE_TAKEDAMAGE_PARENT_CALL(CBaseMonster);
	












	//easyPrintLine("AR DMG : %.2f", flDamage);

	// reset damage time countdown for each type of time based damage player just sustained

	//uh, brackets... what?
	//HEY, found a use for them.  How... resourceful of me?

	if(!blockTimedDamage)
	{
		for (int i = 0; i < CDMG_TIMEBASED; i++){
			//MODDD
			//if (bitsDamageType & (DMG_PARALYZE << i))

			int* m_bitsDamageTypeRef = 0;
			if(i <= 7){
				//use the old bitmask.
				m_bitsDamageTypeRef = &bitsDamageType;
			}else{
				//use the new bitmask.
				m_bitsDamageTypeRef = &bitsDamageTypeMod;
			}


			if ((*m_bitsDamageTypeRef) & (convert_itbd_to_damage(i) )){
					m_rgbTimeBasedDamage[i] = 0;
					//MODDD - next frame this is brought up will be the first one again.
					m_rgbTimeBasedFirstFrame[i] = TRUE;
			}


		}//END OF for(int i = 0...)
	}

	// tell director about it
	MESSAGE_BEGIN( MSG_SPEC, SVC_DIRECTOR );
		WRITE_BYTE ( 9 );	// command length in bytes
		WRITE_BYTE ( DRC_CMD_EVENT );	// take damage event
		WRITE_SHORT( ENTINDEX(this->edict()) );	// index number of primary entity
		WRITE_SHORT( ENTINDEX(ENT(pevInflictor)) );	// index number of secondary entity
		WRITE_LONG( 5 );   // eventflags (priority and flags)
	MESSAGE_END();


	// how bad is it, doc?

	ftrivial = (pev->health > 75 || m_lastDamageAmount < 5);
	fmajor = (m_lastDamageAmount > 25);
	fcritical = (pev->health < 30);

	// handle all bits set in this damage message,
	// let the suit give player the diagnosis

	// UNDONE: add sounds for types of damage sustained (ie: burn, shock, slash )

	// UNDONE: still need to record damage and heal messages for the following types

		// DMG_BURN	
		// DMG_FREEZE
		// DMG_BLAST
		// DMG_SHOCK


	//HM.  This seems redundant with the base monster class... ?  It does this now.

	/*
	m_bitsDamageType |= bitsDamage; // Save this so we can report it to the client
	//MODDD - complimentary.f
	m_bitsDamageTypeMod |= bitsDamageMod;
	*/



	m_bitsHUDDamage = -1;  // make sure the damage bits get resent
	m_bitsModHUDDamage = -1;


	//MODDD - NOTE
	//freeze damage is checked here instead, since "fTookDamage" isn't called for freeze damage.
	//It is sent from the map, indifferent to SLOWFREEZE's damage (seems to be for the unused
	//instigator of continual freeze damage, even though ordinary freeze damage doesn't seem
	//to be timed).  The map uses its own damage (0.1), which affects armor but not health
	//(b/c damage done to health is truncated first).
	if (bitsDamage & DMG_FREEZE){
		SetSuitUpdateAndForceBlock("!HEV_FREEZE", FALSE, SUIT_NEXT_IN_30SEC);	
		bitsDamage &= ~DMG_FREEZE;
		ffound = TRUE;
	}

	//while (fTookDamage && (!ftrivial || (bitsDamage & DMG_TIMEBASED)) && ffound && bitsDamage)
	//including "bitsDamageMod".  It has its own mask for its timed damages.
	


	//if "global_playerReviveTimeBlocksTimedDamage" is 2, we're blocking notifications about new timed damage as well.
	BOOL blockTimedDamageUpdates = (blockTimedDamage && global_playerReviveTimeBlocksTimedDamage == 2);

	//...These are just statuses & removals of non-timed damage for handling them.  No issue to still just report these.  (in regards to revive-invincibility-post-delay)
	while (fTookDamage && (!ftrivial || (bitsDamage & (DMG_BURN | DMG_SHOCK) || bitsDamage & DMG_TIMEBASED) || (bitsDamageMod & DMG_TIMEBASEDMOD)  ) && ffound && bitsDamage)
	{
		ffound = FALSE;

		//MODDD - play messages for these too
		if (bitsDamage & DMG_BURN){
			if(!blockTimedDamageUpdates){
				SetSuitUpdateAndForceBlock("!HEV_FIRE", FALSE, SUIT_NEXT_IN_30SEC);	
			}
			bitsDamage &= ~DMG_BURN;
			ffound = TRUE;
		}
		


		




		if (bitsDamage & DMG_CLUB)
		{
			if(!blockTimedDamageUpdates){
				if (!fmajor)
					SetSuitUpdate("!HEV_DMG4", FALSE, SUIT_NEXT_IN_30SEC);	// minor fracture
			}
			bitsDamage &= ~DMG_CLUB;
			ffound = TRUE;
		}



		if (bitsDamage & (DMG_FALL | DMG_CRUSH))
		{
			if(!blockTimedDamageUpdates){
				if (fmajor)
					SetSuitUpdate("!HEV_DMG5", FALSE, SUIT_NEXT_IN_30SEC);	// major fracture
				else
					SetSuitUpdate("!HEV_DMG4", FALSE, SUIT_NEXT_IN_30SEC);	// minor fracture
			}


			//MODDD - unsure if this should be for onky DMG_FALL or also for DMG_CRUSH
			if (bitsDamage & (DMG_FALL))
			{
				if(!blockTimedDamageUpdates){
					if (fmajor){
						//MODDD - changed to a custom sentence.
						//SetSuitUpdate("!HEV_HEAL1", FALSE, SUIT_NEXT_IN_30SEC, 6.8f);	// major fracture
						SetSuitUpdate("!HEV_FALLDMG", FALSE, SUIT_NEXT_IN_30SEC, 9.3f);	// major fracture
					}
				}
			}


			bitsDamage &= ~(DMG_FALL | DMG_CRUSH);
			ffound = TRUE;
		}
		
		if (bitsDamage & DMG_BULLET)
		{
			if(!blockTimedDamageUpdates){
				if (m_lastDamageAmount > 5)
					SetSuitUpdate("!HEV_DMG6", FALSE, SUIT_NEXT_IN_30SEC, 3.9f);	// blood loss detected
				else{
					//MODDDREMOVE - ?
					SetSuitUpdate("!HEV_DMG0", FALSE, SUIT_NEXT_IN_30SEC, 3.9f);	// minor laceration
				}
			}

			bitsDamage &= ~DMG_BULLET;
			ffound = TRUE;
		}

		if (bitsDamage & DMG_SLASH)
		{
			if(!blockTimedDamageUpdates){
				if (fmajor)
					SetSuitUpdate("!HEV_DMG1", FALSE, SUIT_NEXT_IN_30SEC, 3.9f);	// major laceration
				else
					SetSuitUpdate("!HEV_DMG0", FALSE, SUIT_NEXT_IN_30SEC, 3.9f);	// minor laceration
			}

			bitsDamage &= ~DMG_SLASH;
			ffound = TRUE;
		}
		
		if (bitsDamage & DMG_SONIC)
		{
			if(!blockTimedDamageUpdates){
				if (fmajor)
					SetSuitUpdate("!HEV_DMG2", FALSE, SUIT_NEXT_IN_1MIN, 3.9f);	// internal bleeding
			}
			bitsDamage &= ~DMG_SONIC;
			ffound = TRUE;
		}

		if (bitsDamage & (DMG_POISON | DMG_PARALYZE))
		{
			if(!blockTimedDamageUpdates){
				SetSuitUpdate("!HEV_DMG3", FALSE, SUIT_NEXT_IN_1MIN, 4.6f);	// blood toxins detected
			}

			bitsDamage &= ~(DMG_POISON | DMG_PARALYZE);
			ffound = TRUE;
		}

		if (bitsDamage & DMG_ACID)
		{
			if(!blockTimedDamageUpdates){
				SetSuitUpdate("!HEV_DET1", FALSE, SUIT_NEXT_IN_1MIN, 4.3f);	// hazardous chemicals detected
			}
			bitsDamage &= ~DMG_ACID;
			ffound = TRUE;
		}

		if (bitsDamage & DMG_NERVEGAS)
		{
			if(!blockTimedDamageUpdates){
				SetSuitUpdate("!HEV_DET0", FALSE, SUIT_NEXT_IN_1MIN, 4.2f);	// biohazard detected
			}
			bitsDamage &= ~DMG_NERVEGAS;
			ffound = TRUE;
		}

		if (bitsDamage & DMG_RADIATION)
		{
			if(!blockTimedDamageUpdates){
				SetSuitUpdate("!HEV_DET2", FALSE, SUIT_NEXT_IN_1MIN);	// radiation detected
			}

			bitsDamage &= ~DMG_RADIATION;
			ffound = TRUE;
		}
		if (bitsDamage & DMG_SHOCK)
		{
			if(!blockTimedDamageUpdates){
				//MODDD - added.
				SetSuitUpdate("!HEV_SHOCK", FALSE, SUIT_NEXT_IN_30SEC);	
			}

			bitsDamage &= ~DMG_SHOCK;
			ffound = TRUE;
		}
		//MODDD - new
		if (bitsDamageMod & DMG_BLEEDING)
		{
			if(!blockTimedDamageUpdates){
				//SetSuitUpdate("!???", FALSE, SUIT_NEXT_IN_1MIN);
			}
			//bleeding?
			bitsDamageMod &= ~DMG_BLEEDING;
			ffound = TRUE;
		}
	}

	//MODDD - if "timedDamageDisableViewPunch" is on, timed damage will not throw the view off for a moment.
	//... not this "!(bitsDamageType & DMG_TIMEBASED || bitsDamageTypeMod & DMG_TIMEBASEDMOD)", because that is for starting timed damage.
	//That is okay anyways.  The continued effects (DMG_TIMEDEFFECT) are what this CVar is concerned with.
	if(global_disablePainPunchAutomatic != 1){
		if(global_timedDamageDisableViewPunch == 0 || !(bitsDamageTypeMod & (DMG_TIMEDEFFECT | DMG_TIMEDEFFECTIGNORE) )   ){
			//pev->punchangle.x = -2;
		}
	}




	//Don't print out these plain damage notices on taking damage during the post-revive-invincibility-delay.  We kinda already know...
	if(!(blockDamage || buddhaMode)){

		if (fTookDamage && !ftrivial && fmajor && flHealthPrev >= 75) 
		{
			// first time we take major damage...
			// turn automedic on if not on
			SetSuitUpdate("!HEV_MED1", FALSE, SUIT_NEXT_IN_30MIN, 5.2f);	// automedic on

			// give morphine shot if not given recently
			SetSuitUpdate("!HEV_HEAL7", FALSE, SUIT_NEXT_IN_30MIN, 4.5f);	// morphine shot
		}
	
		if (fTookDamage && !ftrivial && fcritical && flHealthPrev < 75)
		{

			// already took major damage, now it's critical...
			if (pev->health < 6)
				SetSuitUpdate("!HEV_HLTH3", FALSE, SUIT_NEXT_IN_10MIN, 4.2f);	// near death
			else if (pev->health < 20)
				SetSuitUpdate("!HEV_HLTH2", FALSE, SUIT_NEXT_IN_10MIN, 4.5f);	// health critical
	
			// give critical health warnings
			if (!RANDOM_LONG(0,3) && flHealthPrev < 50)
				SetSuitUpdate("!HEV_DMG7", FALSE, SUIT_NEXT_IN_5MIN, 4.5f); //seek medical attention
		}



		// if we're taking time based damage, warn about its continuing effects
		//MODDD - include new damage mask.
		//Wait... isn't that odd?   normally, timed damage is just sent as "generic" (besides the inital strike, in order to avoid
		//a feedback loop of permanent duration resetting).  That type has been changed to "DMG_TIMEDEFFECT" for differentiating for
		//the "timed damage ignores armor" cvar.   TEST WITHOUT THE "DMG_TIMEBASED" IN THERE BELOW.
		//if (fTookDamage && (bitsDamageType & DMG_TIMEBASED) && flHealthPrev < 75)
		//NOTICE::: Problem solved.  Apparently,  "DMG_GENERIC" WAS under the mask of "DMG_TIMEBASED".  So, it counted.
		//Must count "DMG_TIMEDEFFECT" under the new mask to match this.

		//easyPrintLine("RANDO STATUS %d %d %d    %d ", bitsDamageTypeMod, DMG_TIMEBASEDMOD, bitsDamageTypeMod & DMG_TIMEBASEDMOD,    bitsDamageType & DMG_TIMEBASED  );
	
		if(global_printOutCommonTimables == 1){
			easyPrintLine("RAD TIME: %d  BLEED TIME %d  POISON TIME %d ", m_rgbTimeBasedDamage[itbd_Radiation], m_rgbTimeBasedDamage[itbd_Bleeding], m_rgbTimeBasedDamage[itbd_Poison]);
		}


		//if (fTookDamage && (bitsDamageType & DMG_TIMEBASED || bitsDamageTypeMod & DMG_TIMEBASEDMOD || bitsDamageTypeMod ) && flHealthPrev < 75)
		if (fTookDamage && (bitsDamageTypeMod & (DMG_TIMEDEFFECT | DMG_TIMEDEFFECTIGNORE) ) && flHealthPrev < 75)
		{
			if (flHealthPrev < 50)
			{
				if (!RANDOM_LONG(0,3))
					SetSuitUpdate("!HEV_DMG7", FALSE, SUIT_NEXT_IN_5MIN); //seek medical attention
			}
			else
				SetSuitUpdate("!HEV_HLTH1", FALSE, SUIT_NEXT_IN_10MIN);	// health dropping
		}


	
	}//END OF (if NOT during post-invincibility-delay)







	return fTookDamage;
}

//=========================================================
// PackDeadPlayerItems - call this when a player dies to
// pack up the appropriate weapons and ammo items, and to
// destroy anything that shouldn't be packed.
//
// This is pretty brute force :(
//=========================================================
void CBasePlayer::PackDeadPlayerItems( void )
{
	int iWeaponRules;
	int iAmmoRules;
	int i;
	CBasePlayerWeapon *rgpPackWeapons[ 20 ];// 20 hardcoded for now. How to determine exactly how many weapons we have?
	int iPackAmmo[ MAX_AMMO_SLOTS + 1];
	int iPW = 0;// index into packweapons array
	int iPA = 0;// index into packammo array

	memset(rgpPackWeapons, NULL, sizeof(rgpPackWeapons) );
	memset(iPackAmmo, -1, sizeof(iPackAmmo) );

	// get the game rules 
	iWeaponRules = g_pGameRules->DeadPlayerWeapons( this );
 	iAmmoRules = g_pGameRules->DeadPlayerAmmo( this );

	
	if(!alreadyDroppedItemsAtDeath){
		//when scheduled to drop items, they stay on the player until respawn is called.  This script will
		//not know any better and will continue dropping duplicates of the same things the player was holding
		//at the time of death.
		//return;
		alreadyDroppedItemsAtDeath = TRUE;
	}else{
		return;
	}

	if ( iWeaponRules == GR_PLR_DROP_GUN_NO && iAmmoRules == GR_PLR_DROP_AMMO_NO )
	{
		// nothing to pack. Remove the weapons and return. Don't call create on the box!
		//MODDD - schedule for respawn instead of clearing items here.
		scheduleRemoveAllItems = TRUE;
		scheduleRemoveAllItemsIncludeSuit = TRUE;
		//RemoveAllItems( TRUE );
		return;
	}

// go through all of the weapons and make a list of the ones to pack
	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		if ( m_rgpPlayerItems[ i ] )
		{
			// there's a weapon here. Should I pack it?
			CBasePlayerItem *pPlayerItem = m_rgpPlayerItems[ i ];

			while ( pPlayerItem )
			{
				switch( iWeaponRules )
				{
				case GR_PLR_DROP_GUN_ACTIVE:
					if ( m_pActiveItem && pPlayerItem == m_pActiveItem )
					{
						// this is the active item. Pack it.
						rgpPackWeapons[ iPW++ ] = (CBasePlayerWeapon *)pPlayerItem;
					}
					break;

				case GR_PLR_DROP_GUN_ALL:
					rgpPackWeapons[ iPW++ ] = (CBasePlayerWeapon *)pPlayerItem;
					break;

				default:
					break;
				}

				pPlayerItem = pPlayerItem->m_pNext;
			}
		}
	}

// now go through ammo and make a list of which types to pack.
	if ( iAmmoRules != GR_PLR_DROP_AMMO_NO )
	{
		for ( i = 0 ; i < MAX_AMMO_SLOTS ; i++ )
		{
			if ( m_rgAmmo[ i ] > 0 )
			{
				// player has some ammo of this type.
				switch ( iAmmoRules )
				{
				case GR_PLR_DROP_AMMO_ALL:
					iPackAmmo[ iPA++ ] = i;
					break;

				case GR_PLR_DROP_AMMO_ACTIVE:
					if ( m_pActiveItem && i == m_pActiveItem->PrimaryAmmoIndex() ) 
					{
						// this is the primary ammo type for the active weapon
						iPackAmmo[ iPA++ ] = i;
					}
					else if ( m_pActiveItem && i == m_pActiveItem->SecondaryAmmoIndex() ) 
					{
						// this is the secondary ammo type for the active weapon
						iPackAmmo[ iPA++ ] = i;
					}
					break;

				default:
					break;
				}
			}
		}
	}

// create a box to pack the stuff into.
	CWeaponBox *pWeaponBox = (CWeaponBox *)CBaseEntity::Create( "weaponbox", pev->origin, pev->angles, edict() );

	pWeaponBox->pev->angles.x = 0;// don't let weaponbox tilt.
	pWeaponBox->pev->angles.z = 0;

	pWeaponBox->SetThink( &CWeaponBox::Kill );
	pWeaponBox->pev->nextthink = gpGlobals->time + 120;

// back these two lists up to their first elements
	iPA = 0;
	iPW = 0;

// pack the ammo
	while ( iPackAmmo[ iPA ] != -1 )
	{
		pWeaponBox->PackAmmo( MAKE_STRING( CBasePlayerItem::AmmoInfoArray[ iPackAmmo[ iPA ] ].pszName ), m_rgAmmo[ iPackAmmo[ iPA ] ] );
		iPA++;
	}

// now pack all of the items in the lists
	while ( rgpPackWeapons[ iPW ] )
	{
		// weapon unhooked from the player. Pack it into der box.
		pWeaponBox->PackWeapon( rgpPackWeapons[ iPW ] );

		iPW++;
	}

	pWeaponBox->pev->velocity = pev->velocity * 1.2;// weaponbox has player's velocity, then some.


	scheduleRemoveAllItems = TRUE;
	scheduleRemoveAllItemsIncludeSuit = TRUE;
	//RemoveAllItems( TRUE );// now strip off everything that wasn't handled by the code above.
}

void CBasePlayer::RemoveAllItems( BOOL removeSuit )
{
	if (m_pActiveItem)
	{
		ResetAutoaim( );
		m_pActiveItem->Holster( );
		m_pActiveItem = NULL;
	}

	m_pLastItem = NULL;

	int i;
	CBasePlayerItem *pPendingItem;
	for (i = 0; i < MAX_ITEM_TYPES; i++)
	{
		m_pActiveItem = m_rgpPlayerItems[i];
		while (m_pActiveItem)
		{
			pPendingItem = m_pActiveItem->m_pNext; 
			m_pActiveItem->Drop( );
			m_pActiveItem = pPendingItem;
		}
		m_rgpPlayerItems[i] = NULL;
	}
	m_pActiveItem = NULL;

	pev->viewmodel		= 0;
	pev->weaponmodel	= 0;
	

	//MODDD - this will be done at respawn instead of at here.,
	if ( removeSuit ){
		pev->weapons = 0;
	}else{
		pev->weapons &= ~WEAPON_ALLWEAPONS;
	}

	for ( i = 0; i < MAX_AMMO_SLOTS;i++)
		m_rgAmmo[i] = 0;


	//MODDD - probably so.
	this->hasGlockSilencer = FALSE;


	UpdateClientData();
	// send Selected Weapon Message to our client
	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pev );
		WRITE_BYTE(0);
		WRITE_BYTE(0);
		WRITE_BYTE(0);
	MESSAGE_END();

}

/*
 * GLOBALS ASSUMED SET:  g_ulModelIndexPlayer
 *
 * ENTITY_METHOD(PlayerDie)
 */










//MODDD - override this for behavior to start a fadeout with germancensorship on
//since gibbing fails and leads to this intead.
//Again, note that the player has its own PlayerDeathThink method that must be
//set instead of some Fadeout think, so this PlayerDeathThink
//must handle fadeout on its own.
void CBasePlayer::FadeMonster(){

	//StopAnimation();
	
	//Don't interrupt the physics, the player still falls.
	

	pev->animtime = gpGlobals->time;
	pev->effects |= EF_NOINTERP;
}



//entvars_t *g_pevLastInflictor;  // Set in combat.cpp.  Used to pass the damage inflictor for death messages.
								// Better solution:  Add as parameter to all Killed() functions.
//MODDD - the above comment may be a little outdated. "g_pevLastInflictor" is never referred too, but used
//        to hold the last pevInflictor sent along with TakeDamage, as seen in Combat.cpp.
//        Now, the pevAttacker is sent to Killed methods only instead. Assuming only knowing the attacker
//        is necessary, but this could be edited to also receive the pevInflictor that takeDamage does too
//        more neatly.
GENERATE_KILLED_IMPLEMENTATION(CBasePlayer)
{

	//gee, I don't think you're doing this right now.
	m_bHolstering = FALSE;
	m_pQueuedActiveItem = NULL;
	m_fCustomHolsterWaitTime = -1;


	friendlyCheckTime = -1;

	//do a friendly check, all friendlies. Any that has this player as its enemy will stop playing the horror sound.
	CBaseEntity *pEntity = NULL;
	while((pEntity = UTIL_FindEntityByClassname( pEntity, "monster_friendly" )) != NULL){
		CFriendly* tempMrFriendly = static_cast<CFriendly*>(pEntity);

		//targeting this player?
		if(tempMrFriendly->m_hEnemy && tempMrFriendly && tempMrFriendly->m_hEnemy == this){
			tempMrFriendly->horrorSelected = FALSE;
			tempMrFriendly->stopHorrorSound();
		}
	}//END OF while(friendly check)





	barnacleLocked = FALSE;
	//in case "CBaseMonster::Killed" isn't called.

	CSound *pSound;

	

	// Holster weapon immediately, to allow it to cleanup
	if ( m_pActiveItem )
		m_pActiveItem->Holster( );

	//MODDD - the pevInflictor is finally provided!
	//g_pGameRules->PlayerKilled( this, pevAttacker, g_pevLastInflictor );
	g_pGameRules->PlayerKilled( this, pevAttacker, pevInflictor );

	if ( m_pTank != NULL )
	{
		m_pTank->Use( this, this, USE_OFF, 0 );
		m_pTank = NULL;
	}

	// this client isn't going to be thinking for a while, so reset the sound until they respawn
	pSound = CSoundEnt::SoundPointerForIndex( CSoundEnt::ClientSoundIndex( edict() ) );
	{
		if ( pSound )
		{
			pSound->Reset();
		}
	}

	SetAnimation( PLAYER_DIE );
	
	m_iRespawnFrames = 0;

	pev->modelindex = g_ulModelIndexPlayer;    // don't use eyes

	pev->deadflag		= DEAD_DYING;
	pev->movetype		= MOVETYPE_TOSS;
	ClearBits( pev->flags, FL_ONGROUND );
	if (pev->velocity.z < 10)
		pev->velocity.z += RANDOM_FLOAT(0,300);

	// clear out the suit message cache so we don't keep chattering
	SetSuitUpdate(NULL, FALSE, 0);

	// send "health" update message to zero
	m_iClientHealth = 0;
	MESSAGE_BEGIN( MSG_ONE, gmsgHealth, NULL, pev );
		WRITE_BYTE( m_iClientHealth );
	MESSAGE_END();

	




	// Tell Ammo Hud that the player is dead
	MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pev );
		WRITE_BYTE(0);
		WRITE_BYTE(0XFF);
		WRITE_BYTE(0xFF);
	MESSAGE_END();

	//MODDD - new way of telling the HUD that the player is dead.  The above is
	//a bit indirect of a way that is incompatible with the current approach.
	deadflagmem = pev->deadflag;
	MESSAGE_BEGIN( MSG_ONE, gmsgUpdatePlayerAlive, NULL, pev );
		WRITE_SHORT( IsAlive() );
	MESSAGE_END();


	// reset FOV
	pev->fov = m_iFOV = m_iClientFOV = 0;

	MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, pev );
		WRITE_BYTE(0);
	MESSAGE_END();






	
	
	BOOL gibbedThisFrame = FALSE; //only good this call.

	// UNDONE: Put this in, but add FFADE_PERMANENT and make fade time 8.8 instead of 4.12
	// UTIL_ScreenFade( edict(), Vector(128,0,0), 6, 15, 255, FFADE_OUT | FFADE_MODULATE );

	if ( ( pev->health < -40 && iGib != GIB_NEVER ) || iGib == GIB_ALWAYS )
	{
		//pev->solid			= SOLID_NOT;   //but GibMonster already does this.
		//GibMonster();	// This clears pev->model
		GENERATE_GIBMONSTER_CALL;
		
		//let's leave the EF_NODRAW up to GibMonster.
		//pev->effects |= EF_NODRAW;

		//MODDD - added.
		recentlyGibbed = TRUE;

		gibbedThisFrame = TRUE;

		//return;   //GOT YOUUUU
	}
	//MODDD - added.
	else{
		//recentlyGibbed = FALSE;   Keep this on until Spawn() gets called again

	}


	//MODDD
	//For now, if the player has adrenaline and hasn't been gibbed, tell "DeathSound" this.
	if( !recentlyGibbed && m_rgItems[ITEM_ADRENALINE] > 0){

		if(global_batteryDrainsAtAdrenalineMode == 1){
			pev->armorvalue = 0;
			m_iClientBattery = pev->armorvalue;
			MESSAGE_BEGIN( MSG_ONE, gmsgBattery, NULL, pev );
				WRITE_SHORT( (int)pev->armorvalue);
			MESSAGE_END();
		}

		//also, this CVar.
		if(global_timedDamageReviveRemoveMode == 1){
			attemptResetTimedDamage(TRUE);
		}

		DeathSound(TRUE);
	}else{

		if(global_timedDamageDeathRemoveMode > 0){
			attemptResetTimedDamage(TRUE);
		}

		if(global_batteryDrainsAtDeath == 1){
			pev->armorvalue = 0;
			m_iClientBattery = pev->armorvalue;
			MESSAGE_BEGIN( MSG_ONE, gmsgBattery, NULL, pev );
				WRITE_SHORT( (int)pev->armorvalue);
			MESSAGE_END();
		}

		DeathSound(FALSE);
	}
	

	//MODDD - This is supposed to be skipped, as per original script (was here, and is skipped if the player was gibbed by a now defunct "return").
	if(gibbedThisFrame){
		pev->angles.x = 0;
		pev->angles.z = 0;

		SetThink(&CBasePlayer::PlayerDeathThink);
		pev->nextthink = gpGlobals->time + 0.1;
	}


}


// Set the activity based on an event or current state
void CBasePlayer::SetAnimation( PLAYER_ANIM playerAnim )
{
	int animDesired;
	float speed;
	char szAnim[64];

	speed = pev->velocity.Length2D();

	if (pev->flags & FL_FROZEN)
	{
		speed = 0;
		playerAnim = PLAYER_IDLE;
	}

	switch (playerAnim) 
	{
	case PLAYER_JUMP:
		m_IdealActivity = ACT_HOP;
		break;
	
	case PLAYER_SUPERJUMP:
		m_IdealActivity = ACT_LEAP;
		break;
	
	case PLAYER_DIE:
		m_IdealActivity = ACT_DIESIMPLE;
		m_IdealActivity = GetDeathActivity( );
		break;

	case PLAYER_ATTACK1:	
		switch( m_Activity )
		{
		case ACT_HOVER:
		case ACT_SWIM:
		case ACT_HOP:
		case ACT_LEAP:
		case ACT_DIESIMPLE:
			m_IdealActivity = m_Activity;
			break;
		default:
			m_IdealActivity = ACT_RANGE_ATTACK1;
			break;
		}
		break;
	case PLAYER_IDLE:
	case PLAYER_WALK:
		if ( !FBitSet( pev->flags, FL_ONGROUND ) && (m_Activity == ACT_HOP || m_Activity == ACT_LEAP) )	// Still jumping
		{
			m_IdealActivity = m_Activity;
		}
		else if ( pev->waterlevel > 1 )
		{
			if ( speed == 0 )
				m_IdealActivity = ACT_HOVER;
			else
				m_IdealActivity = ACT_SWIM;
		}
		else
		{
			m_IdealActivity = ACT_WALK;
		}
		break;
	}

	switch (m_IdealActivity)
	{
	case ACT_HOVER:
	case ACT_LEAP:
	case ACT_SWIM:
	case ACT_HOP:
	case ACT_DIESIMPLE:
	default:
		if ( m_Activity == m_IdealActivity)
			return;
		m_Activity = m_IdealActivity;

		animDesired = LookupActivity( m_Activity );
		// Already using the desired animation?
		if (pev->sequence == animDesired)
			return;

		pev->gaitsequence = 0;
		pev->sequence		= animDesired;
		pev->frame			= 0;
		ResetSequenceInfo( );
		return;

	case ACT_RANGE_ATTACK1:
		if ( FBitSet( pev->flags, FL_DUCKING ) )	// crouching
			strcpy( szAnim, "crouch_shoot_" );
		else
			strcpy( szAnim, "ref_shoot_" );
		strcat( szAnim, m_szAnimExtention );
		animDesired = LookupSequence( szAnim );
		if (animDesired == -1)
			animDesired = 0;

		if ( pev->sequence != animDesired || !m_fSequenceLoops )
		{
			pev->frame = 0;
		}

		if (!m_fSequenceLoops)
		{
			pev->effects |= EF_NOINTERP;
		}

		m_Activity = m_IdealActivity;

		pev->sequence		= animDesired;
		ResetSequenceInfo( );
		break;

	case ACT_WALK:
		if (m_Activity != ACT_RANGE_ATTACK1 || m_fSequenceFinished)
		{
			if ( FBitSet( pev->flags, FL_DUCKING ) )	// crouching
				strcpy( szAnim, "crouch_aim_" );
			else
				strcpy( szAnim, "ref_aim_" );
			strcat( szAnim, m_szAnimExtention );
			animDesired = LookupSequence( szAnim );
			if (animDesired == -1)
				animDesired = 0;
			m_Activity = ACT_WALK;
		}
		else
		{
			animDesired = pev->sequence;
		}
	}

	if ( FBitSet( pev->flags, FL_DUCKING ) )
	{
		if ( speed == 0)
		{
			pev->gaitsequence	= LookupActivity( ACT_CROUCHIDLE );
			// pev->gaitsequence	= LookupActivity( ACT_CROUCH );
		}
		else
		{
			pev->gaitsequence	= LookupActivity( ACT_CROUCH );
		}
	}
	else if ( speed > 220 )
	{
		pev->gaitsequence	= LookupActivity( ACT_RUN );
	}
	else if (speed > 0)
	{
		pev->gaitsequence	= LookupActivity( ACT_WALK );
	}
	else
	{
		// pev->gaitsequence	= LookupActivity( ACT_WALK );
		pev->gaitsequence	= LookupSequence( "deep_idle" );
	}


	// Already using the desired animation?
	if (pev->sequence == animDesired)
		return;

	//ALERT( at_console, "Set animation to %d\n", animDesired );
	// Reset to first frame of desired animation
	pev->sequence		= animDesired;
	pev->frame			= 0;
	ResetSequenceInfo( );

	//NOTICE - hard to work wtih, doing this in hl_wpn_glock.cpp instead (communicate that the silencer is on to the third person model).
	//pev->iuser1 = 4000;
	//pev->effects = 4;
}

/*
===========
TabulateAmmo
This function is used to find and store 
all the ammo we have into the ammo vars.
============
*/
void CBasePlayer::TabulateAmmo()
{
	ammo_9mm = AmmoInventory( GetAmmoIndex( "9mm" ) );
	ammo_357 = AmmoInventory( GetAmmoIndex( "357" ) );
	ammo_argrens = AmmoInventory( GetAmmoIndex( "ARgrenades" ) );
	ammo_bolts = AmmoInventory( GetAmmoIndex( "bolts" ) );
	ammo_buckshot = AmmoInventory( GetAmmoIndex( "buckshot" ) );
	ammo_rockets = AmmoInventory( GetAmmoIndex( "rockets" ) );
	ammo_uranium = AmmoInventory( GetAmmoIndex( "uranium" ) );
	ammo_hornets = AmmoInventory( GetAmmoIndex( "Hornets" ) );
}







void CBasePlayer::DebugCall1(){
	debugPoint1 = pev->origin;
	debugPoint1.z += 2;
	debugPoint1Given = TRUE;
	debugPoint3Given = FALSE;
}
void CBasePlayer::DebugCall2(){
	debugPoint2 = pev->origin;
	debugPoint2.z += 2;
	debugPoint2Given = TRUE;
	debugPoint3Given = FALSE;
}
void CBasePlayer::DebugCall3(){
	//try a local move.
	
	debugPoint3Given = FALSE;


	float distReg;

	UTIL_MakeVectors(this->pev->v_angle + this->pev->punchangle);
		
	Vector vecStart = debugPoint1;
	Vector vecEnd = debugPoint2;

	//BOOL success = this->CheckLocalMove(vecStart, vecStart + gpGlobals->v_forward * dist, NULL, &distReg);
	BOOL success = this->CheckLocalMove(vecStart, vecEnd, NULL, &distReg);
	//UTIL_TraceLine(tempplayer->pev->origin + tempplayer->pev->view_ofs + gpGlobals->v_forward * 5,pMe->pev->origin + pMe->pev->view_ofs + gpGlobals->v_forward * 2048,dont_ignore_monsters, pMe->edict(), &tr );
	




	if(success){
		easyForcePrintLine("SUCCESS!  CLEAR!");
		debugPoint3 = vecEnd;
		debugPoint3Given = TRUE;
	}else{
		
		float totalDist = (vecEnd - vecStart).Length();
		easyForcePrintLine("Stopped this far: %.2f OUT OF %.2f", distReg, totalDist);
		debugPoint3Given = TRUE;
		Vector tempDir = (vecEnd - vecStart).Normalize();

		//debugPoint3 = vecStart + (tempDir * (distReg ));
		//debugPoint3 = vecStart + (vecEnd - vecStart) * (distReg / totalDist);
		float fracto = distReg / totalDist;
		debugPoint3 = vecStart * (1 + -fracto) + vecEnd * fracto;

		//UTIL_printVector("vecStart", vecStart);
		//UTIL_printVector("tempDir", tempDir);
		//UTIL_printVector("debugPoint3", debugPoint3);
		//UTIL_printVector("vecEnd", vecEnd);
	}


}//END OF DebugCall3()




//MODDD - player constructor.
CBasePlayer::CBasePlayer(void){


	m_bHolstering = FALSE;
	m_pQueuedActiveItem = NULL;
	m_fCustomHolsterWaitTime = -1;


	superDuperDelay = -2;

	friendlyCheckTime = -1;
	
	closestFriendlyMemEHANDLE = NULL;
	closestFriendlyMem = NULL;
	horrorPlayTimePreDelay = -1;
	horrorPlayTime = -1;

	debugVect1Draw = FALSE;
	debugVect2Draw = FALSE;
	debugVect3Draw = FALSE;
	debugVect4Draw = FALSE;
	debugVect5Draw = FALSE;



	debugPoint1Given = FALSE;
	debugPoint2Given = FALSE;
	debugPoint3Given = FALSE;

	m_flStartCharge = -1;  //okay?
	m_flStartChargeAnim = -1;
	m_flStartChargePreSuperDuper = -1;


	reviveSafetyTime = -1;
	
	grabbedByBarnacle = FALSE;
	grabbedByBarnacleMem = FALSE;

	queueTotalFOVUpdate = TRUE;
	queueZoomFOVUpdate = FALSE;

	//forceNoWeaponLoop = TRUE;

	nextSpecialNode = -1;
	nextSpecialNodeAlternateTime = -1;

	skillMem = -1;

	iWasFrozenToday = FALSE;

	//oldWaterMoveTime = -1;

	recentlyGrantedGlockSilencer = FALSE;
	m_fLongJumpMemory = FALSE;
	recentlySaidBattery = -1;  //do not save, meant to relate to what was recently said in-game yet.

	altLadderStep = FALSE;   //this alternates b/w the two view punches (left & right).


	if(cheat_barnacleEatsEverything == 0){
		//then it has not been set yet.  This player will.    no, the player hasn't loaded "myref..." yes, so it will be 0 always.
		//cheat_barnacleEatsEverything = myRef_barnacleEatsEverything;
	}

	alreadyPassedLadderCheck = FALSE;

	nextMadEffect = -1;

	
	alreadyDroppedItemsAtDeath = FALSE;
	sentCarcassScent = FALSE;

	hasGlockSilencer = FALSE;
	hasGlockSilencerMem = -1;

	//always start ready?
	//clearWeaponFlag = -1;

	
	antidoteQueued = FALSE;
	radiationQueued = FALSE;
	adrenalineQueued = FALSE;

	
	for(int i = 0; i < CSUITPLAYLIST; i++){
		m_rgSuitPlayListEvent[i] = NULL;
		m_rgSuitPlayListEventDelay[i] = -1;
		m_rgSuitPlayListFVoxCutoff[i] = -1;
	}

	currentSuitSoundEventTime = -1;
	currentSuitSoundEvent = NULL;
	currentSuitSoundFVoxCutoff = -1;

	sentenceFVoxCutoffStop = -1;

	rawDamageSustained = 0;

}//END OF CBasePlayer constructor






/*
===========
WaterMove
============
*/
#define AIRTIME	12		// lung full of air lasts this many seconds

void CBasePlayer::WaterMove()
{
	int air;

	if (pev->movetype == MOVETYPE_NOCLIP)
		return;

	if (pev->health < 0)
		return;

	// waterlevel 0 - not in water
	// waterlevel 1 - feet in water
	// waterlevel 2 - waist in water
	// waterlevel 3 - head in water


	//MODDD - all new to keep track of how much time passed between this call and the last one.
	float timeDelta;

	if(oldWaterMoveTime != -1){
		timeDelta = gpGlobals->time - oldWaterMoveTime;
	}else{
		timeDelta = 0;
	}
	oldWaterMoveTime = gpGlobals->time;

	//easyPrint("timedelta: %.12f\n", timeDelta);
	//easyPrint("delta: %.12f\n", oldWaterMoveTime);



	//MODDD
	//If not in the water, OR is in the water, but has air left in the air tank... (also requires at least one battery charge, if that cvar is on)
	if (pev->waterlevel != 3 || (pev->waterlevel == 3 && airTankAirTime > 0 && (global_itemBatteryPrerequisite == 0 || pev->armorvalue > 0 ) ) )
	{
		//MODDD
		//If underwater, use some air (implied we have some from the above if-statement's second condition)
		if(pev->waterlevel == 3){
			//easyPrint(" AIR UPDATE %.3f\n", airTankAirTime);
			//easyPrint(" AIR UPDATE DELTA %.3f\n", timeDelta);


			airTankAirTime -= timeDelta;
			if(airTankAirTime < 0){
				airTankAirTime = 0;
			}else{

				//if using the air tank for the first time since entering the water (immediately),
				//play the air sound.

				//MODDD - if going 
				if(airTankWaitingStart == TRUE){
					//pPlayer->edict() ???
					EMIT_SOUND_FILTERED( ENT(pev), CHAN_ITEM, "items/airtank1.wav", 1, ATTN_NORM, TRUE );
					airTankWaitingStart = FALSE;
				}
		


			}
			
			airTankAirTimeNeedsUpdate = TRUE;
		}else{
			//If not underwater, the air-trigger sound is ready to play.
			airTankWaitingStart = TRUE;
		}
		//This way, under
		
		// not underwater
		// play 'up for air' sound
		if (pev->air_finished < gpGlobals->time)
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_wade1.wav", 1, ATTN_NORM);
		else if (pev->air_finished < gpGlobals->time + 9)
			EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_wade2.wav", 1, ATTN_NORM);

		pev->air_finished = gpGlobals->time + AIRTIME;
		pev->dmg = 2;

		// if we took drowning damage, give it back slowly
		if (m_idrowndmg > m_idrownrestored)
		{
			// set drowning damage bit.  hack - dmg_drownrecover actually
			// makes the time based damage code 'give back' health over time.
			// make sure counter is cleared so we start count correctly.
			
			// NOTE: this actually causes the count to continue restarting
			// until all drowning damage is healed.

			m_bitsDamageType |= DMG_DROWNRECOVER;
			m_bitsDamageType &= ~DMG_DROWN;
			m_rgbTimeBasedDamage[itbd_DrownRecover] = 0;
		}
		drowning = FALSE;
	}
	else
	{	
		//No air and underwater.
		
		//MODDD - getting an airtank will re-trigger the air sound.
		airTankWaitingStart = TRUE;

		// fully under water
		// stop restoring damage while underwater
		m_bitsDamageType &= ~DMG_DROWNRECOVER;
		m_rgbTimeBasedDamage[itbd_DrownRecover] = 0;


		if (pev->air_finished < gpGlobals->time)		// drown!
		{
			//MODDD
			drowning = TRUE;

			if (pev->pain_finished < gpGlobals->time)
			{
				// take drowning damage
				pev->dmg += 1;
				if (pev->dmg > 5)
					pev->dmg = 5;
				TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), pev->dmg, DMG_DROWN);
				pev->pain_finished = gpGlobals->time + 1;
				
				// track drowning damage, give it back when
				// player finally takes a breath

				m_idrowndmg += pev->dmg;
			} 
		}
		else
		{
			m_bitsDamageType &= ~DMG_DROWN;
			//MODDD
			drowning = FALSE;
		}


	}



	if (!pev->waterlevel)
	{
		if (FBitSet(pev->flags, FL_INWATER))
		{       
			ClearBits(pev->flags, FL_INWATER);
		}
		return;
	}
	
	// make bubbles

	air = (int)(pev->air_finished - gpGlobals->time);
	if (!RANDOM_LONG(0,0x1f) && RANDOM_LONG(0,AIRTIME-1) >= air)
	{
		switch (RANDOM_LONG(0,3))
			{
			case 0:	EMIT_SOUND(ENT(pev), CHAN_BODY, "player/pl_swim1.wav", 0.8, ATTN_NORM); break;
			case 1:	EMIT_SOUND(ENT(pev), CHAN_BODY, "player/pl_swim2.wav", 0.8, ATTN_NORM); break;
			case 2:	EMIT_SOUND(ENT(pev), CHAN_BODY, "player/pl_swim3.wav", 0.8, ATTN_NORM); break;
			case 3:	EMIT_SOUND(ENT(pev), CHAN_BODY, "player/pl_swim4.wav", 0.8, ATTN_NORM); break;
		}
	}

	if (pev->watertype == CONTENT_LAVA)		// do damage
	{
		if (pev->dmgtime < gpGlobals->time)
			TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 10 * pev->waterlevel, DMG_BURN);
	}
	else if (pev->watertype == CONTENT_SLIME)		// do damage
	{
		pev->dmgtime = gpGlobals->time + 1;
		TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 4 * pev->waterlevel, DMG_ACID);
	}
	
	if (!FBitSet(pev->flags, FL_INWATER))
	{
		SetBits(pev->flags, FL_INWATER);
		pev->dmgtime = 0;
	}
}


// TRUE if the player is attached to a ladder
BOOL CBasePlayer::IsOnLadder( void )
{ 
	return ( pev->movetype == MOVETYPE_FLY );
}




//PlayerDeathThink is called in the middle of the Player's PreThink method if not gibbed.
//If gibbed, this method is set to the entire new Think (none of PreThink gets  called)
void CBasePlayer::PlayerDeathThink(void)
{

	float flForward;


	if (FBitSet(pev->flags, FL_ONGROUND))
	{
		flForward = pev->velocity.Length() - 20;
		if (flForward <= 0)
			pev->velocity = g_vecZero;
		else    
			pev->velocity = flForward * pev->velocity.Normalize();
	}

	if ( HasWeapons() )
	{
		// we drop the guns here because weapons that have an area effect and can kill their user
		// will sometimes crash coming back from CBasePlayer::Killed() if they kill their owner because the
		// player class sometimes is freed. It's safer to manipulate the weapons once we know
		// we aren't calling into any of their code anymore through the player pointer.
		PackDeadPlayerItems();
	}

	//MODDD NOTE - This area blocks turning the DEAD_DEAD flag on until the player death anim has finished or taken too long.
	if (pev->modelindex && (!m_fSequenceFinished) && (pev->deadflag == DEAD_DYING))
	{
		if(pev->frame >= (255*0.4)){
			this->recognizablyDead = TRUE;
		}
	
		StudioFrameAdvance( );

		m_iRespawnFrames++;				// Note, these aren't necessarily real "frames", so behavior is dependent on # of client movement commands
		if ( m_iRespawnFrames < 120 )   // Animations should be no longer than this
			return;
	}
	
	

	// once we're done animating our death and we're on the ground, we want to set movetype to None so our dead body won't do collisions and stuff anymore
	// this prevents a bug where the dead body would go to a player's head if he walked over it while the dead player was clicking their button to respawn
	if ( pev->movetype != MOVETYPE_NONE && FBitSet(pev->flags, FL_ONGROUND) )
		pev->movetype = MOVETYPE_NONE;


	if (pev->deadflag == DEAD_DYING)
		pev->deadflag = DEAD_DEAD;
	
	StopAnimation();

	pev->effects |= EF_NOINTERP;
	pev->framerate = 0.0;






	
	//recoveryIndex:
	//-1: not out of health or in the dead / incapacitated state.
	// 0: in incapacitated state, planned to revive, waiting for delay start
	//    (when on ground, or after 6 seconds pass, just start anyways).
	// 1: done waiting, force a revive delay.
	// 2: can revive. (skipped, revive is just done then and there, but could be used
	//    to let the user click and revive I suppose)
	// 3: dead; can NOT revive, schedule respawn or let the player click-out.


	//MODDD - adrenaline "injection" - no pun intended.
	//When "dead", and there is at least 1 Adrenaline, use it and respawn in-place in a few seconds.
	if( recoveryIndex == -1){

		if( !recentlyGibbed && this->m_rgItems[ITEM_ADRENALINE] > 0){
			recoveryIndex = 0;
			recoveryIndexMem = 0;
			recoveryDelay = gpGlobals->time + 6;
			recoveryDelayMin = gpGlobals->time + 0.4f;

			if(global_timedDamageReviveRemoveMode == 2){
				attemptResetTimedDamage(TRUE);
			}

		}else{
			recoveryIndex = 3;
			recoveryIndexMem = 3;
		}

	}

	if(recoveryIndex == 0){
		//by the way, "pev->model != 0" is a gib-check.  The model being null (== 0) means, gibbed.  No "body" to recover.
		//nah, just use "recentlyGibbed", new var.
		//search "iGib" for more details on gibbing here (and in basemonster.h).
		if( !recentlyGibbed && this->m_rgItems[ITEM_ADRENALINE] > 0){

			


			//note that, if the player is not on the ground BUT otherwise meets conditions to recover,
			//the respawn will still be stalled until the player hits the ground (where the timer starts and
			//the player revives).
			//Even if on the ground, a minimum time (recoveryDelayMin) must have passed to start, including midair time, usually small.
			if ( (FBitSet(pev->flags, FL_ONGROUND) && gpGlobals->time >= recoveryDelayMin ) || gpGlobals->time >= recoveryDelay){
				//can recover.

				/*
				*/
				if(!adrenalineQueued){
					adrenalineQueued = TRUE;
					SetSuitUpdateEventFVoxCutoff("!HEV_ADR_USE", FALSE, SUIT_REPEAT_OK, 0.41 - 0.07, &CBasePlayer::consumeAdrenaline, 0.41 - 0.07 + 0.55);
				}
				//if(revived){
					//If revived, send the signal to reset falling velocity.
					g_engfuncs.pfnSetPhysicsKeyValue( edict(), "res", "1" );
				//}

			}

		}else{
			//can not recover.  Let ordinary respawn-mechanisms handle this situation.
			recoveryIndex = 3;
			recoveryIndexMem = 3;
		}
	}


	if(recoveryIndex == 1 && gpGlobals->time > recoveryDelay){
		//recover!
		recoveryIndex = -1;
		recoveryIndexMem = -1;
		recoveryDelay = -1;
		recoveryDelayMin = -1;
		GetClassPtr( (CBasePlayer *)pev)->Spawn( TRUE );

		//invincibility delay?
		if(global_playerReviveInvincibilityTime > 0){
			reviveSafetyTime = gpGlobals->time + global_playerReviveInvincibilityTime;
		}
		return;

	}


	//ordinary respawn can not handle the situation if the player may revive.
	if(recoveryIndex != 3){
		return;
	}

	//...if execution wasn't stopped by the above check, the player is dead with no chance of revival.
	
	//MODDD - if landed on the ground, send the same scent the AI does at death to alert scavengers (eaters) of this.
	if(FBitSet(pev->flags, FL_ONGROUND) && pev->velocity.Length() == 0 && !sentCarcassScent){
		sentCarcassScent = TRUE;
		CSoundEnt::InsertSound ( bits_SOUND_CARCASS, pev->origin, 384, 30 );
	}



	//Since this think method overrides fade logic in case of german censorship, can do it here.

	//Assume we're fading out:
	if(recentlyGibbed && !(pev->effects & EF_NODRAW) ){
		//If German Censorship is on, we've been gibbed, and we're still drawn, 
		//do a fade for this frame.

		if (pev->rendermode == kRenderNormal){
			pev->renderamt = 255;
			pev->rendermode = kRenderTransTexture;
		}else{
			
			//pev->renderamt = max(pev->renderamt - gpGlobals->frametime * EASY_CVAR_GET(playerFadeOutRate) , 0) ;
			pev->renderamt -= gpGlobals->frametime * EASY_CVAR_GET(playerFadeOutRate);

			if(pev->renderamt <= 0){
				//stop the transparency, just go straight to normally opaque but invisible with EF_NODRAW as expected.
				pev->rendermode = ::kRenderNormal;
				pev->renderamt = 0;
				pev->effects |= EF_NODRAW;
			}

		}

	}//END OF fade check
	else{
		//Should be invisible? can force it to be safe.
		//pev->effects |= EF_NODRAW;
	}














	BOOL fAnyButtonDown = (pev->button & ~IN_SCORE );
	
	// wait for all buttons released
	if (pev->deadflag == DEAD_DEAD)
	{
		if (fAnyButtonDown)
			return;

		if ( g_pGameRules->FPlayerCanRespawn( this ) )
		{
			m_fDeadTime = gpGlobals->time;
			pev->deadflag = DEAD_RESPAWNABLE;
		}
		
		return;
	}



// if the player has been dead for one second longer than allowed by forcerespawn, 
// forcerespawn isn't on. Send the player off to an intermission camera until they 
// choose to respawn.
	if ( g_pGameRules->IsMultiplayer() && ( gpGlobals->time > (m_fDeadTime + 6) ) && !(m_afPhysicsFlags & PFLAG_OBSERVER) )
	{
		// go to dead camera. 
		StartDeathCam();
	}


	
// wait for any button down,  or mp_forcerespawn is set and the respawn time is up
	if (!fAnyButtonDown 
		&& !( g_pGameRules->IsMultiplayer() && forcerespawn.value > 0 && (gpGlobals->time > (m_fDeadTime + 5))) )
		return;

	pev->button = 0;
	m_iRespawnFrames = 0;

	//ALERT(at_console, "Respawn\n");

	if(scheduleRemoveAllItems = TRUE){
		RemoveAllItems(scheduleRemoveAllItemsIncludeSuit);

		scheduleRemoveAllItemsIncludeSuit = FALSE;
		scheduleRemoveAllItems = FALSE;
	}
	

	respawn(pev, !(m_afPhysicsFlags & PFLAG_OBSERVER) );// don't copy a corpse if we're in deathcam.
	pev->nextthink = -1;
}





//=========================================================
// StartDeathCam - find an intermission spot and send the
// player off into observer mode
//=========================================================
void CBasePlayer::StartDeathCam( void )
{
	edict_t *pSpot, *pNewSpot;
	int iRand;

	if ( pev->view_ofs == g_vecZero )
	{
		// don't accept subsequent attempts to StartDeathCam()
		return;
	}

	pSpot = FIND_ENTITY_BY_CLASSNAME( NULL, "info_intermission");	

	if ( !FNullEnt( pSpot ) )
	{
		// at least one intermission spot in the world.
		iRand = RANDOM_LONG( 0, 3 );

		while ( iRand > 0 )
		{
			pNewSpot = FIND_ENTITY_BY_CLASSNAME( pSpot, "info_intermission");
			
			if ( pNewSpot )
			{
				pSpot = pNewSpot;
			}

			iRand--;
		}

		CopyToBodyQue( pev );
		StartObserver( pSpot->v.origin, pSpot->v.v_angle );
	}
	else
	{
		// no intermission spot. Push them up in the air, looking down at their corpse
		TraceResult tr;
		CopyToBodyQue( pev );
		UTIL_TraceLine( pev->origin, pev->origin + Vector( 0, 0, 128 ), ignore_monsters, edict(), &tr );
		StartObserver( tr.vecEndPos, UTIL_VecToAngles( tr.vecEndPos - pev->origin  ) );
		return;
	}
}

void CBasePlayer::StartObserver( Vector vecPosition, Vector vecViewAngle )
{
	m_afPhysicsFlags |= PFLAG_OBSERVER;

	pev->view_ofs = g_vecZero;
	pev->angles = pev->v_angle = vecViewAngle;
	pev->fixangle = TRUE;
	pev->solid = SOLID_NOT;
	pev->takedamage = DAMAGE_NO;
	pev->movetype = MOVETYPE_NONE;
	pev->modelindex = 0;
	UTIL_SetOrigin( pev, vecPosition );
}

// 
// PlayerUse - handles USE keypress
//
#define	PLAYER_SEARCH_RADIUS	(float)64

void CBasePlayer::PlayerUse ( void )
{
	// Was use pressed or released?
	if ( ! ((pev->button | m_afButtonPressed | m_afButtonReleased) & IN_USE) )
		return;
	
	// Hit Use on a train?
	if ( m_afButtonPressed & IN_USE )
	{
		if ( m_pTank != NULL )
		{
			// Stop controlling the tank
			// TODO: Send HUD Update
			m_pTank->Use( this, this, USE_OFF, 0 );
			m_pTank = NULL;
			return;
		}
		else
		{
			if ( m_afPhysicsFlags & PFLAG_ONTRAIN )
			{
				m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
				m_iTrain = TRAIN_NEW|TRAIN_OFF;
				return;
			}
			else
			{	// Start controlling the train!
				CBaseEntity *pTrain = CBaseEntity::Instance( pev->groundentity );

				if ( pTrain && !(pev->button & IN_JUMP) && FBitSet(pev->flags, FL_ONGROUND) && (pTrain->ObjectCaps() & FCAP_DIRECTIONAL_USE) && pTrain->OnControls(pev) )
				{
					m_afPhysicsFlags |= PFLAG_ONTRAIN;
					m_iTrain = TrainSpeed(pTrain->pev->speed, pTrain->pev->impulse);
					m_iTrain |= TRAIN_NEW;
					//MODDD - soundsentencesave
					EMIT_SOUND_FILTERED( ENT(pev), CHAN_ITEM, "plats/train_use1.wav", 0.8, ATTN_NORM, TRUE);
					return;
				}
			}
		}
	}

	CBaseEntity *pObject = NULL;
	CBaseEntity *pClosest = NULL;
	Vector		vecLOS;
	float flMaxDot = VIEW_FIELD_NARROW;
	float flDot;

	UTIL_MakeVectors ( pev->v_angle );// so we know which way we are facing

	Vector closestLOS;
	

	//MODDD - slight adjustment. Go forwards a bit to search instead... UNDONE.
	while ((pObject = UTIL_FindEntityInSphere( pObject, pev->origin, PLAYER_SEARCH_RADIUS )) != NULL)
	//while ((pObject = UTIL_FindEntityInSphere( pObject, pev->origin + gpGlobals->v_forward * 64, PLAYER_SEARCH_RADIUS )) != NULL)
	{

		//MODDD - distance check... UNDONE.
		if (pObject->ObjectCaps() & (FCAP_IMPULSE_USE | FCAP_CONTINUOUS_USE | FCAP_ONOFF_USE))
		//if (((pObject->pev->origin - this->pev->origin).Length() <= 64) && pObject->ObjectCaps() & (FCAP_IMPULSE_USE | FCAP_CONTINUOUS_USE | FCAP_ONOFF_USE))
		{
			// !!!PERFORMANCE- should this check be done on a per case basis AFTER we've determined that
			// this object is actually usable? This dot is being done for every object within PLAYER_SEARCH_RADIUS
			// when player hits the use key. How many objects can be in that area, anyway? (sjb)
			vecLOS = (VecBModelOrigin( pObject->pev ) - (pev->origin + pev->view_ofs));
			
			// This essentially moves the origin of the target to the corner nearest the player to test to see 
			// if it's "hull" is in the view cone
			vecLOS = UTIL_ClampVectorToBoxNonNormalized( vecLOS, pObject->pev->size * 0.5 );
			
			Vector vecLOSNorm = vecLOS.Normalize();

			flDot = DotProduct (vecLOSNorm , gpGlobals->v_forward);
			if (flDot > flMaxDot )
			{// only if the item is in front of the user
				pClosest = pObject;
				closestLOS = vecLOS; //MODDD - new.
				flMaxDot = flDot;
//				ALERT( at_console, "%s : %f\n", STRING( pObject->pev->classname ), flDot );
			}
//			ALERT( at_console, "%s : %f\n", STRING( pObject->pev->classname ), flDot );
		}
	}
	pObject = pClosest;



	BOOL flUseSuccess = FALSE;
	
	//easyForcePrintLine("aaaaaaaaAAAaaaa %d", pObject==NULL);

	// Found an object
	if (pObject )
	{

		//!!!!!!!!!!
		//!!!UNDONE: traceline here to prevent USEing buttons through walls		
		edict_t		*pentIgnore;
		TraceResult tr;

		pentIgnore = this->edict();
		UTIL_MakeVectors(pev->v_angle);// + pev->punchangle);
		
		//Vector vecDirTowardsClosest = (pObject->pev->origin - this->pev->origin).Normalize();
		Vector vecDirTowardsClosest = closestLOS;

		//(vecDirTowardsClosest - this->pev->origin).Length(); 
		//float distToClosest = (pObject->pev->origin - this->pev->origin).Length();
		float distToClosest = vecDirTowardsClosest.Length();
		

		//notice: NOT "gpGlobals->v_forward", which is just what direction the player happens to be facing. We want a straight line to the object we are trying to touch.
		Vector vecSrc = pev->origin + pev->view_ofs + vecDirTowardsClosest * 0;

		//why a little over 1 (100%)? To go a little further just to make sure we hit it.
		Vector vecDest = pev->origin + pev->view_ofs + vecDirTowardsClosest * 1.1;//(0+distToClosest+1); //(0+PLAYER_SEARCH_RADIUS);

		//nah, precision for while ducking not necessary.
		/*
		Vector playerEyePos = Vector(tempplayer->body;
		if(pev->flags & FL_DUCKING){

		}
		*/

		
	
		//easyForcePrintLine("?????????????????????????????????");

		UTIL_TraceLine( vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr );
		//tr.vecEndPos();

		/*
		//could still be ok? Skip this check.
		if (tr.fAllSolid){
			//trace failed this early? just stop.
			return;
		}
		*/
		
		//debugVect1Draw = TRUE;
		if(EASY_CVAR_GET(playerUseDrawDebug)){
			::DebugLine_ClearAll();
		}
		

		//debugVect1End

		if(EASY_CVAR_GET(playerUseDrawDebug))DebugLine_Setup(0, vecSrc, vecSrc+vecDirTowardsClosest, tr.flFraction);


		//tr.flFraction is in case it goes too far by a bit.
		if(tr.pHit != NULL ){

			CBaseEntity* hitEntity = CBaseEntity::Instance(tr.pHit);

			float distToPointHit = (vecDirTowardsClosest.Length());

			//easyForcePrintLine("HIT SOMETHING? %s fract:%.2f distoff:%.2f", STRING(tr.pHit->v.classname), (tr.flFraction), distToPointHit*(1 - (tr.flFraction)) );

			//may hit worldspawn, which does not block. Not sure why it even counts as hit if so (flFraction stays 1)
			if(hitEntity->pev == pObject->pev || distToPointHit<=5 || distToPointHit*(1 - (tr.flFraction)) <= 10 ){
				//the trace-hit entity matches the entity selected to "use" on? this is valid.
				flUseSuccess = TRUE;
			}else{

				if(::FClassnameIs(hitEntity->pev, "worldspawn")  ){
					//possible exception. See if this is the case.
					if(tr.flFraction>=1.0){
						//missed touching anything? just count it.
						flUseSuccess = TRUE;
					}else{
						//worldspawn but still an inadequate "fraction" of the distance reached towards the target?
						//Try from the view point of the player to the target instead.
						Vector vecSrc2 = pev->origin + pev->view_ofs;

						Vector vecDest2 = VecBModelOrigin(pObject->pev);

						TraceResult tr2;
						UTIL_TraceLine( vecSrc2, vecDest2, dont_ignore_monsters, pentIgnore, &tr2 );
						
						float distToPointHit2 = (vecDest2 - (vecSrc2)).Length();
						
						if(EASY_CVAR_GET(playerUseDrawDebug))DebugLine_Setup(0, vecSrc2, vecDest2, tr2.flFraction);


						//debugVect1End =  vecDest2;

						//easyForcePrintLine("HIT SOMETHING B? %s fract:%.2f distoff:%.2f", STRING(tr2.pHit->v.classname), (tr2.flFraction), distToPointHit2*(1 - (tr2.flFraction)) );
						
						if(tr2.pHit != NULL){
							CBaseEntity* hitEntity2 = CBaseEntity::Instance(tr2.pHit);
							if(hitEntity2->pev == pObject->pev || distToPointHit2<=5 || distToPointHit2*(1 - (tr2.flFraction)) <= 10){
								//it's good!
								flUseSuccess = TRUE;
								
							}else{
								flUseSuccess = FALSE;
							}
						}else{
							flUseSuccess = TRUE;
						}

					}//END OF inner center test
				}else{
					//mismatch? not ok.
					flUseSuccess = FALSE;
				}
			}
			

		}else{
			//hit nothing? Just assume it worked.
			flUseSuccess = TRUE;
		}


		//if(EASY_CVAR_GET(playerUseDrawDebug))aryDebugLines[0].setSuccess(flUseSuccess);

		//if successful, force it all green.
		if(flUseSuccess){DebugLine_ColorSuccess(0);};

		/*
		tempplayer->GiveNamedItem( CMD_ARGV(1),  attemptInterpretSpawnFlag(CMD_ARGV(2)),
									tr.vecEndPos.x,
									tr.vecEndPos.y,
									tr.vecEndPos.z + global_offsetgivelookvertical,
									TRUE, &tr);
		*/
	}else{
		//no object touched? stop.
		flUseSuccess = FALSE;

		if(EASY_CVAR_GET(playerUseDrawDebug)){
			debugVect1Draw = FALSE;
			debugVect2Draw = FALSE;
			debugVect3Draw = FALSE;
			debugVect4Draw = FALSE;
			debugVect5Draw = FALSE;
		}
	}


	//MODDD DEBUG FEATURE - STOP FOR NOW.
	//return;


	if(flUseSuccess == TRUE){
		


		int caps = pObject->ObjectCaps();

		if ( m_afButtonPressed & IN_USE ){
			EMIT_SOUND_FILTERED( ENT(pev), CHAN_ITEM, "common/wpn_select.wav", 0.4, ATTN_NORM, TRUE);
		}

		if ( ( (pev->button & IN_USE) && (caps & FCAP_CONTINUOUS_USE) ) ||
			 ( (m_afButtonPressed & IN_USE) && (caps & (FCAP_IMPULSE_USE|FCAP_ONOFF_USE)) ) )
		{
			if ( caps & FCAP_CONTINUOUS_USE )
				m_afPhysicsFlags |= PFLAG_USING;

			pObject->Use( this, this, USE_SET, 1 );
		}
		// UNDONE: Send different USE codes for ON/OFF.  Cache last ONOFF_USE object to send 'off' if you turn away
		else if ( (m_afButtonReleased & IN_USE) && (pObject->ObjectCaps() & FCAP_ONOFF_USE) )	// BUGBUG This is an "off" use
		{
			pObject->Use( this, this, USE_SET, 0 );
		}
	}
	else
	{

		//NOTE - is that a little hard to read? m_afButtonPressed is a bitmask of inputs. IN_USE is on when the player pressed the USE key this frame, I assume.
		//      So this just means, only play the "deny" sound if pressing the USE key.
		if ( m_afButtonPressed & IN_USE ){
			EMIT_SOUND_FILTERED( ENT(pev), CHAN_ITEM, "common/wpn_denyselect.wav", 0.4, ATTN_NORM, TRUE);
		}
	}
}//END OF PlayerUse



void CBasePlayer::Jump()
{
	Vector		vecWallCheckDir;// direction we're tracing a line to find a wall when walljumping
	Vector		vecAdjustedVelocity;
	Vector		vecSpot;
	TraceResult	tr;
	
	if (FBitSet(pev->flags, FL_WATERJUMP))
		return;
	
	if (pev->waterlevel >= 2)
	{
		return;
	}

	// jump velocity is sqrt( height * gravity * 2)

	// If this isn't the first frame pressing the jump button, break out.
	if ( !FBitSet( m_afButtonPressed, IN_JUMP ) )
		return;         // don't pogo stick

	if ( !(pev->flags & FL_ONGROUND) || !pev->groundentity )
	{
		return;
	}

// many features in this function use v_forward, so makevectors now.
	UTIL_MakeVectors (pev->angles);

	// ClearBits(pev->flags, FL_ONGROUND);		// don't stairwalk
	
	SetAnimation( PLAYER_JUMP );

	//MODDD - require a slightly stricter condition:
	/*
	if ( m_fLongJump &&
		(pev->button & IN_DUCK) &&
		( pev->flDuckTime > 0 ) &&
		pev->velocity.Length() > 50 )
	{
		SetAnimation( PLAYER_SUPERJUMP );
	}
	*/

	
	/*if ( m_fLongJump &&
		(pev->button & IN_DUCK) &&
		( pev->flDuckTime >= 0 ) &&
		pev->velocity.Length() > 0 &&
		//!pev->button & IN_JUMP &&
		FBitSet ( pev->flags, FL_ONGROUND ) &&
		longJumpCharge == PLAYER_LONGJUMPCHARGE_MAX)
		*/

	//MODDD - CHECK POINT CHECKPOINT
	//MODDD - nah, let's just use the same condition that means we would've longjumped:

#if LONGJUMPUSESDELAY == 1
	if (m_fLongJump && longJumpCharge >= PLAYER_LONGJUMPCHARGE_MAX)
	{
		SetAnimation( PLAYER_SUPERJUMP );
	}
#else
	//easyPrint("yes anim? %.2f\n", (lastDuckVelocityLength)  );
	if (m_fLongJump && longJump_waitForRelease && lastDuckVelocityLength > 7 && (global_itemBatteryPrerequisite || pev->armorvalue > 0 ))
	{
		SetAnimation( PLAYER_SUPERJUMP );
	}

#endif

	/*
	if(longJumpCharge != 0){
		longJumpChargeNeedsUpdate = TRUE;
		longJumpCharge = 0;
	}
	*/





	// If you're standing on a conveyor, add it's velocity to yours (for momentum)
	entvars_t *pevGround = VARS(pev->groundentity);
	if ( pevGround && (pevGround->flags & FL_CONVEYOR) )
	{
		pev->velocity = pev->velocity + pev->basevelocity;
	}
}



// This is a glorious hack to find free space when you've crouched into some solid space
// Our crouching collisions do not work correctly for some reason and this is easier
// than fixing the problem :(
void FixPlayerCrouchStuck( edict_t *pPlayer )
{
	TraceResult trace;

	// Move up as many as 18 pixels if the player is stuck.
	for ( int i = 0; i < 18; i++ )
	{
		UTIL_TraceHull( pPlayer->v.origin, pPlayer->v.origin, dont_ignore_monsters, head_hull, pPlayer, &trace );
		if ( trace.fStartSolid )
			pPlayer->v.origin.z ++;
		else
			break;
	}
}

void CBasePlayer::Duck( )
{
	if (pev->button & IN_DUCK) 
	{
		if ( m_IdealActivity != ACT_LEAP )
		{
			SetAnimation( PLAYER_WALK );
		}
	}
}

//
// ID's player as such.
//
int  CBasePlayer::Classify ( void )
{
	return CLASS_PLAYER;
}


void CBasePlayer::AddPoints( int score, BOOL bAllowNegativeScore )
{
	// Positive score always adds
	if ( score < 0 )
	{
		if ( !bAllowNegativeScore )
		{
			if ( pev->frags < 0 )		// Can't go more negative
				return;
			
			if ( -score > pev->frags )	// Will this go negative?
			{
				score = -pev->frags;		// Sum will be 0
			}
		}
	}

	pev->frags += score;

	MESSAGE_BEGIN( MSG_ALL, gmsgScoreInfo );
		WRITE_BYTE( ENTINDEX(edict()) );
		WRITE_SHORT( pev->frags );
		WRITE_SHORT( m_iDeaths );
		WRITE_SHORT( 0 );
		WRITE_SHORT( g_pGameRules->GetTeamIndex( m_szTeamName ) + 1 );
	MESSAGE_END();
}


void CBasePlayer::AddPointsToTeam( int score, BOOL bAllowNegativeScore )
{
	int index = entindex();

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CBaseEntity *pPlayer = UTIL_PlayerByIndex( i );

		if ( pPlayer && i != index )
		{
			if ( g_pGameRules->PlayerRelationship( this, pPlayer ) == GR_TEAMMATE )
			{
				pPlayer->AddPoints( score, bAllowNegativeScore );
			}
		}
	}
}

//Player ID
void CBasePlayer::InitStatusBar()
{
	m_flStatusBarDisappearDelay = 0;
	m_SbarString1[0] = m_SbarString0[0] = 0; 
}

//MODDD - uh, found as-is.  WHAT IS THIS???!
void CBasePlayer::UpdateStatusBar()
{
	int newSBarState[ SBAR_END ];
	char sbuf0[ SBAR_STRING_SIZE ];
	char sbuf1[ SBAR_STRING_SIZE ];

	memset( newSBarState, 0, sizeof(newSBarState) );
	strcpy( sbuf0, m_SbarString0 );
	strcpy( sbuf1, m_SbarString1 );

	// Find an ID Target
	TraceResult tr;
	UTIL_MakeVectors( pev->v_angle + pev->punchangle );
	Vector vecSrc = EyePosition();
	Vector vecEnd = vecSrc + (gpGlobals->v_forward * MAX_ID_RANGE);
	UTIL_TraceLine( vecSrc, vecEnd, dont_ignore_monsters, edict(), &tr);

	if (tr.flFraction != 1.0)
	{
		if ( !FNullEnt( tr.pHit ) )
		{
			CBaseEntity *pEntity = CBaseEntity::Instance( tr.pHit );

			if (pEntity->Classify() == CLASS_PLAYER )
			{
				newSBarState[ SBAR_ID_TARGETNAME ] = ENTINDEX( pEntity->edict() );
				strcpy( sbuf1, "1 %p1\n2 Health: %i2%%\n3 Armor: %i3%%" );

				// allies and medics get to see the targets health
				if ( g_pGameRules->PlayerRelationship( this, pEntity ) == GR_TEAMMATE )
				{
					newSBarState[ SBAR_ID_TARGETHEALTH ] = 100 * (pEntity->pev->health / pEntity->pev->max_health);
					newSBarState[ SBAR_ID_TARGETARMOR ] = pEntity->pev->armorvalue; //No need to get it % based since 100 it's the max.
				}

				m_flStatusBarDisappearDelay = gpGlobals->time + 1.0;
			}
		}
		else if ( m_flStatusBarDisappearDelay > gpGlobals->time )
		{
			// hold the values for a short amount of time after viewing the object
			newSBarState[ SBAR_ID_TARGETNAME ] = m_izSBarState[ SBAR_ID_TARGETNAME ];
			newSBarState[ SBAR_ID_TARGETHEALTH ] = m_izSBarState[ SBAR_ID_TARGETHEALTH ];
			newSBarState[ SBAR_ID_TARGETARMOR ] = m_izSBarState[ SBAR_ID_TARGETARMOR ];
		}
	}

	BOOL bForceResend = FALSE;

	if ( strcmp( sbuf0, m_SbarString0 ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgStatusText, NULL, pev );
			WRITE_BYTE( 0 );
			WRITE_STRING( sbuf0 );
		MESSAGE_END();

		strcpy( m_SbarString0, sbuf0 );

		// make sure everything's resent
		bForceResend = TRUE;
	}

	if ( strcmp( sbuf1, m_SbarString1 ) )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgStatusText, NULL, pev );
			WRITE_BYTE( 1 );
			WRITE_STRING( sbuf1 );
		MESSAGE_END();

		strcpy( m_SbarString1, sbuf1 );

		// make sure everything's resent
		bForceResend = TRUE;
	}

	// Check values and send if they don't match
	for (int i = 1; i < SBAR_END; i++)
	{
		if ( newSBarState[i] != m_izSBarState[i] || bForceResend )
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgStatusValue, NULL, pev );
				WRITE_BYTE( i );
				WRITE_SHORT( newSBarState[i] );
			MESSAGE_END();

			m_izSBarState[i] = newSBarState[i];
		}
	}
}






#define CLIMB_SHAKE_FREQUENCY	22	// how many frames in between screen shakes when climbing
#define	MAX_CLIMB_SPEED			200	// fastest vertical climbing speed possible
#define	CLIMB_SPEED_DEC			15	// climbing deceleration rate
#define	CLIMB_PUNCH_X			-7  // how far to 'punch' client X axis when climbing
#define CLIMB_PUNCH_Z			7	// how far to 'punch' client Z axis when climbing

void CBasePlayer::PreThink(void)
{



	/*
	if(normalSpeedMultiMem != global_normalSpeedMulti){
		normalSpeedMultiMem = global_normalSpeedMulti;
		//keep this CVar in sync with pm_shared...
		if(normalSpeedMultiMem != 0){



			char buffer[13];
			tryFloatToStringBuffer(buffer, global_normalSpeedMulti);

			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "nsm", buffer );
		}else{
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "nsm", "0" );
		}
	}
	if(jumpForceMultiMem != global_jumpForceMulti){
		jumpForceMultiMem = global_jumpForceMulti;
		//keep this CVar in sync with pm_shared...
		if(jumpForceMultiMem != 0){

			char buffer[13];
			tryFloatToStringBuffer(buffer, global_jumpForceMulti);
			
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "jfm", buffer );
		}else{
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "jfm", "0" );
		}
	}
	*/
	//return;


	if(global_testVar == -1)
		return;
	

	//MODDD - this is required for some things to modify velocity, such as touch methods forcing the player off something's back.
	if(fApplyTempVelocity){
		fApplyTempVelocity = FALSE;
		pev->velocity = velocityApplyTemp;
	}






	/*
	extern float global_testVar;
	if(global_testVar == 2){
		global_testVar = 1;
		UTIL_printLineVector("MIN", pev->mins);
		UTIL_printLineVector("MAX", pev->maxs);

		MIN: (-16.00, -16.00, -36.00)
		MAX: (16.00, 16.00, 36.00)
	}
	*/




	if(debugPoint1Given){
		UTIL_drawLineFrame(debugPoint1 - Vector(0,0,3), debugPoint1 + Vector(0,0,3), 9, 0, 255, 0);
	}
	if(debugPoint2Given){
		UTIL_drawLineFrame(debugPoint2 - Vector(0,0,3), debugPoint2 + Vector(0,0,3), 9, 55, 225, 70);
	}
	if(debugPoint3Given){
		UTIL_drawLineFrame(debugPoint1, debugPoint3, 15, 0, 255, 0);
		UTIL_drawLineFrame(debugPoint3, debugPoint2, 15, 255, 0, 0);
	}else if(debugPoint1Given && debugPoint2Given){
		UTIL_drawLineFrame(debugPoint1, debugPoint2, 12, 255, 255, 255);
	}



	
	int r = 0, g = 0, b = 0, thicc=0;
	if(debugVect1Draw){
		r = 0;g=0;b=0;
		if(debugVect1Success){g=255;thicc=4;}else{r= 255;thicc=12;}
		UTIL_drawLineFrame(debugVect1Start, debugVect1End, thicc, r, g, b);
	}
	if(debugVect2Draw){
		r = 0;g=0;b=0;
		if(debugVect2Success){g=255;thicc=4;}else{r= 255;thicc=12;}
		UTIL_drawLineFrame(debugVect2Start, debugVect2End, thicc, r, g, b);
	}
	if(debugVect3Draw){
		r = 0;g=0;b=0;
		if(debugVect3Success){g=255;thicc=4;}else{r= 255;thicc=12;}
		UTIL_drawLineFrame(debugVect3Start, debugVect3End, thicc, r, g, b);
	}
	if(debugVect4Draw){
		r = 0;g=0;b=0;
		if(debugVect4Success){g=255;thicc=4;}else{r= 255;thicc=12;}
		UTIL_drawLineFrame(debugVect4Start, debugVect4End, thicc, r, g, b);
	}
	if(debugVect5Draw){
		r = 0;g=0;b=0;
		if(debugVect5Success){g=255;thicc=4;}else{r= 255;thicc=12;}
		UTIL_drawLineFrame(debugVect5Start, debugVect5End, thicc, r, g, b);
	}

	



	//horrorSelected


	if(friendlyCheckTime != -1 && friendlyCheckTime <= gpGlobals->time){
		
		CBaseEntity *pEntity = NULL;
		CFriendly* closestFriendly = NULL;
		float closestDistanceYet = 3000;  //must be this close to care.
		while((pEntity = UTIL_FindEntityByClassname( pEntity, "monster_friendly" )) != NULL){

			if(!pEntity)continue;

			CFriendly* tempMrFriendly = static_cast<CFriendly*>(pEntity);

			if(!tempMrFriendly || !tempMrFriendly->pev->deadflag == DEAD_NO)continue;

			//MODDD NOTICE: do we need to do something to a friendly that used to be on but will no longer be on after this? Keep track of having horrorSelected TRUE at first?
			//if(tempMrFriendly->horrorSelected == TRUE){
			//    ...
		    //}


			if(tempMrFriendly->m_hEnemy && tempMrFriendly->m_hEnemy->IsPlayer() == FALSE){
				tempMrFriendly->horrorSelected = FALSE;  //If targeting a non-player, this must be off.
				tempMrFriendly->stopHorrorSound();
			}

			
			float thisDist = (pEntity->pev->origin - pev->origin).Length();
			//easyForcePrintLine("OKAYYY??? %d %.2f", tempMrFriendly->monsterID, thisDist);
			//closest Friendly and is targeting this player?

			if(tempMrFriendly->m_hEnemy == this){
				tempMrFriendly->horrorSelected = FALSE;
			}

			if( thisDist < closestDistanceYet && tempMrFriendly->m_hEnemy && tempMrFriendly->m_hEnemy == this && (tempMrFriendly->m_pSchedule != ::slSmallFlinch  ) ){
				tempMrFriendly->horrorSelected = FALSE;  //off for this player unless it is the closest.
				//tempMrFriendly->stopHorrorSound(); careful, this may be the closest one. we are unsure. don't interrupt the sound yet.
				closestFriendly = tempMrFriendly;
				closestDistanceYet = thisDist;
			}


		}//END OF while(friendly check)

		if(closestFriendly != NULL){
			//selected!
			closestFriendly->horrorSelected = TRUE;
			closestFriendlyMem = closestFriendly;
			closestFriendlyMemEHANDLE = closestFriendly;
		}else{
			closestFriendlyMem = NULL;
			closestFriendlyMemEHANDLE = NULL;
		}


		friendlyCheckTime = gpGlobals->time + 0.8;
	}



	if(closestFriendlyMemEHANDLE && (pev->deadflag == DEAD_NO) && EASY_CVAR_GET(friendlyPianoFollowVolume) > 0 ){

		if(horrorPlayTimePreDelay != -1 && horrorPlayTimePreDelay < gpGlobals->time){
			horrorPlayTimePreDelay = -1;
		}

		if(horrorPlayTimePreDelay == -1){

			if(horrorPlayTime == -1){
				horrorPlayTime = 0;
			}

			if(horrorPlayTime != -1 && horrorPlayTime < gpGlobals->time){
				//CHECK: only play if I'm the closest to the player? plus a general distance check too?
				horrorPlayTime = gpGlobals->time + 0.558;
				//ATTN_STATIC ?



				//TODO: make the volume a factor of distance?
				float tempVol = ( ((-(this->pev->origin - closestFriendlyMemEHANDLE->pev->origin).Length()) / 3000) + EASY_CVAR_GET(friendlyPianoFollowVolume)+0.1 );

				if(tempVol < 0){
					//don't play? no point.
				}else{
					if(tempVol > EASY_CVAR_GET(friendlyPianoFollowVolume) ){tempVol = EASY_CVAR_GET(friendlyPianoFollowVolume);}

					//easyForcePrintLine("WHERE ELSE SUCKAH %.2f", tempVol);

					//EMIT_SOUND_FILTERED( edict(), CHAN_VOICE, "friendly/friendly_horror.wav", 1.0, 1.8, 0, 100 );
					EMIT_SOUND_FILTERED( this->edict(), CHAN_STATIC, "friendly/friendly_horror.wav", tempVol, 1.8, 0, 100 );
				}

			}
		}


	}//END OF closestFriendly check














	//MODDD

	if(global_drawDebugBloodTrace == 1){
		UTIL_drawLineFrameBoxAround(debugDrawVect, 9, 30, 255, 0, 0);
		//UTIL_drawLineFrameBoxAround(debugDrawVectB, 9, 30, 255, 255, 255);
		
		UTIL_drawLineFrame(debugDrawVect2, debugDrawVect3, 9, 0, 255, 0);
		UTIL_drawLineFrame(debugDrawVect4, debugDrawVect5, 9, 0, 0, 255);
		
		UTIL_drawLineFrame(debugDrawVectRecentGive1, debugDrawVectRecentGive2, 9, 255, 255, 255);
		
	}


	
	
	extern float global_testVar;
	if(global_testVar == 666){
	
		//~started off from "UTIL_EntitiesInBox"
		edict_t		*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );
		CBaseEntity *pEntity;
		if (pEdict){

			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
			{
				if ( pEdict->free )	// Not in use
					continue;


				CBaseEntity* thisEnt = CBaseEntity::Instance(pEdict);


				/*
				if(thisEnt != NULL){
					CBaseMonster* monsterTest = thisEnt->GetMonsterPointer();
					if(monsterTest != NULL){
						easyPrint("MONSTER: %s:%d", monsterTest->getClassname(), monsterTest->monsterID);
						//continue;

						if(monsterTest->m_hTargetEnt != NULL){
							CBaseMonster* monsterTestTargetTest = monsterTest->m_hTargetEnt->GetMonsterPointer();
							easyPrint(" TARGET: %s:%d", monsterTest->m_hTargetEnt->getClassname(), monsterTestTargetTest!=NULL?monsterTestTargetTest->monsterID:-1  );
						}else{
							easyPrint(" TARGET: none");
						}

						if(monsterTest->m_hEnemy != NULL){
							CBaseMonster* monsterTestEnemyTest = monsterTest->m_hEnemy->GetMonsterPointer();
							easyPrint(" ENEMY: %s:%d", monsterTest->m_hEnemy->getClassname(), monsterTestEnemyTest!=NULL?monsterTestEnemyTest->monsterID:-1  );
						}else{
							easyPrint(" ENEMY: none");
						}
						easyPrint("\n");

					}else{
						easyPrintLine("ENTITY: %s", thisEnt->getClassname() );
					}
				}
				*/

				if(thisEnt != NULL){
					CBaseMonster* monsterTest = thisEnt->GetMonsterPointer();
					if(monsterTest != NULL){
						if(monsterTest->m_hEnemy != NULL && FClassnameIs(monsterTest->m_hEnemy->pev, "monster_zombie")){
							CBaseMonster* monsterTestEnemy = monsterTest->m_hEnemy->GetMonsterPointer();
							if(monsterTestEnemy != NULL){
								easyPrintLine("Monster: %s:%d Target:%s:%d", monsterTest->getClassname(), monsterTest->monsterID, monsterTestEnemy->getClassname(), monsterTestEnemy->monsterID);
							}
						}
					}
				}





			}//END OF for(int i...)
		}//END OF if(pEdict)
		EASY_CVAR_SET_DEBUGONLY(testVar, 0);
	}






	int specialNode = -1;
	if(global_drawNodeAlternateTime > 0){
		if(gpGlobals->time > 3 && gpGlobals->time > nextSpecialNodeAlternateTime){
			//easyPrintLine("IM A DURR %d", nextSpecialNode);
			nextSpecialNode++;
			if(nextSpecialNode >= WorldGraph.m_cNodes){
				nextSpecialNode = 0;
			}
			nextSpecialNodeAlternateTime = gpGlobals->time + global_drawNodeAlternateTime;
			easyForcePrintLine("SPECIAL NODE: %d", nextSpecialNode);
		}
		specialNode = nextSpecialNode;
	}else{
		nextSpecialNode = -1;
		if(global_drawNodeSpecial >= 0 && global_drawNodeSpecial < WorldGraph.m_cNodes){
			specialNode = (int)global_drawNodeSpecial;
		}
	}


	if(global_drawNodeAll >= 1 || global_drawNodeConnections >= 2){
		
	
		//easyPrintLine("NODEZ: %d", WorldGraph.m_cNodes);

		for(int i = 0; i < WorldGraph.m_cNodes; i++){
			CNode thisNode = WorldGraph.m_pNodes[i];
			//thisNode.m_afNodeInfo
			//UTIL_drawLineFrameBoxAround(thisNode.m_vecOrigin, 9, 30, 255, 255, 0);
			
			if(specialNode != i){

				float nodeDist = (WorldGraph.m_pNodes[i].m_vecOrigin - this->pev->origin).Length();

				if(global_drawNodeAll == 1){
					UTIL_drawLineFrame(thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z + 20, thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z - 20, 9, 255, 0, 0);
					if(global_drawNodeConnections == 2){
						//special node connections!
						WorldGraph.ShowNodeConnectionsFrame(i);
					}
				}else if(global_drawNodeAll >= 10){
					if( nodeDist < global_drawNodeAll){
						UTIL_drawLineFrame(thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z + 20, thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z - 20, 9, 255, 0, 0);
						if(global_drawNodeConnections == 2){
							//special node connections!
							WorldGraph.ShowNodeConnectionsFrame(i);
						}
					};
				}
				/*
				if(global_drawNodeConnections == 2){
					//special node connections!
					WorldGraph.ShowNodeConnectionsFrame(i);
				}else if(global_drawNodeConnections >= 10){
					//require the node itself to be close enough to the player first, to limit drawing.
					
					if( nodeDist < global_drawNodeConnections){
						WorldGraph.ShowNodeConnectionsFrame(i);
					};
				}
				*/
			}else{
				UTIL_drawLineFrame(thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z + 20, thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z - 20, 9, 255, 255, 255);
				if(global_drawNodeConnections >= 1){
					//special node connections!
					WorldGraph.ShowNodeConnectionsFrame(specialNode);
				}
			}
			
			//easyPrintLine("WHERE BE I?? %.2f %.2f %.2f", thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z);
		}
			//WorldGraph.m_pNodes[i].m_vecOriginPeek.z = 
			//	WorldGraph.m_pNodes[i].m_vecOrigin.z = tr.vecEndPos.z + NODE_HEIGHT

	}else{
		if(specialNode != -1){
			CNode thisNode = WorldGraph.m_pNodes[specialNode];
			//still a special node to draw at least.
			UTIL_drawLineFrame(thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z + 15, thisNode.m_vecOrigin.x, thisNode.m_vecOrigin.y, thisNode.m_vecOrigin.z - 15, 7, 255, 255, 255);
			if(global_drawNodeConnections >= 1){
				//special node connections!
				WorldGraph.ShowNodeConnectionsFrame(specialNode);
			}
		}
	}


	//NOTE::: it appears that the end of retail ends the game by making the client's "pev->flags" add "FL_FROZEN".
	//easyPrintLine("MY DETAILZ %d %d %d %d %s", pev->deadflag, pev->effects, pev->flags, pev->renderfx, m_pActiveItem!=NULL?STRING(m_pActiveItem->pev->classname):"NULL");
		


	//bla bla bla, draw scene stuff.
	if(global_drawDebugCine == 1)
	{
		edict_t		*pEdicttt;
		CBaseEntity *pEntityyy;

		//ENGINE_SET_PVS(Vector(2639.43, -743.85, -251.64));
		
		pEdicttt = g_engfuncs.pfnPEntityOfEntIndex( 1 );
		pEntityyy;
		if ( pEdicttt ){
			for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdicttt++ ){
				if ( pEdicttt->free )	// Not in use
				continue;
		
				pEntityyy = CBaseEntity::Instance(pEdicttt);
				if ( !pEntityyy )
					continue;

				//pEntityyy->alreadySaved = FALSE;


				CBaseMonster* tempmon = NULL;
				//if(  (tempmon = pEntityyy->GetMonsterPointer()) != NULL && tempmon->m_pCine != NULL){
					////tempmon->m_pCine->pev->origin = tempmon->pev->origin + Vector(0,0, 6);
				//}

				if(FClassnameIs(pEntityyy->pev,"scripted_sequence") || FClassnameIs(pEntityyy->pev,"aiscripted_sequence") || FClassnameIs(pEntityyy->pev,"scripted_sentence")   ){
					UTIL_drawLineFrame(pEntityyy->pev->origin.x, pEntityyy->pev->origin.y, pEntityyy->pev->origin.z + 15, pEntityyy->pev->origin.x, pEntityyy->pev->origin.y, pEntityyy->pev->origin.z - 15, 7, 0, 255, 12);

				}

			}//END OF loop
		}//END OF if(pEdicttt (first)  )

	}//END OF draw cine's check









	int buttonsChanged = (m_afButtonLast ^ pev->button);	// These buttons have changed this frame
	
	// Debounced button codes for pressed/released
	// UNDONE: Do we need auto-repeat?
	m_afButtonPressed =  buttonsChanged & pev->button;		// The changed ones still down are "pressed"
	m_afButtonReleased = buttonsChanged & (~pev->button);	// The ones not down are "released"

	g_pGameRules->PlayerThink( this );

	if ( g_fGameOver )
		return;         // intermission or finale

	UTIL_MakeVectors(pev->v_angle);             // is this still used?
	
	ItemPreFrame( );
	WaterMove();

	if ( g_pGameRules && g_pGameRules->FAllowFlashlight() )
		m_iHideHUD &= ~HIDEHUD_FLASHLIGHT;
	else
		m_iHideHUD |= HIDEHUD_FLASHLIGHT;


	// JOHN: checks if new client data (for HUD and view control) needs to be sent to the client
	UpdateClientData();
	
	CheckTimeBasedDamage();

	CheckSuitUpdate();


	if(currentSuitSoundEventTime != -1 && currentSuitSoundEventTime <= gpGlobals->time){
		//If waiting on an event and it is time to play it, do just that.
		currentSuitSoundEventTime = -1;
		
		//Call the local method, "eventMethod", provided.  That is horrendous.
		(this->*(currentSuitSoundEvent))();

	}//END OF currentSuitSoundEventTime check


	if(!fvoxOn && currentSuitSoundFVoxCutoff != -1 && currentSuitSoundFVoxCutoff <= gpGlobals->time){
		//If waiting to cut off this sound...
		currentSuitSoundFVoxCutoff = -1;

		int isentence = sentenceFVoxCutoffStop;

		if(isentence < 10000){
			char sentence[CBSENTENCENAME_MAX+1];
			strcpy(sentence, "!");
			strcat(sentence, gszallsentencenames[isentence]);

			STOP_SOUND_SUIT(ENT(pev), sentence);

		}else{
			//not supported (number readings are all or nothing).
		}

	}//END OF currentSuitSoundEventTime check




	








	if (pev->deadflag >= DEAD_DYING)
	{
		PlayerDeathThink();
		return;
	}







//MODDD - rope things.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//easyForcePrintLine("ROPE INFO?? onrope?:%d notnull?:%d", m_afPhysicsFlags & PFLAG_ONROPE, (m_pRope!=NULL) );
	if( m_afPhysicsFlags & PFLAG_ONROPE && m_pRope )
	{


		pev->velocity = g_vecZero;

		Vector vecAttachPos = m_pRope->GetAttachedObjectsPosition();

		UTIL_SetOrigin( pev, vecAttachPos );

		Vector vecForce;

		

		/*
		//TODO: This causes sideways acceleration that doesn't occur in Op4. - Solokiller
		//TODO: should be IN_MOVERIGHT and IN_MOVELEFT - Solokiller
		if( pev->button & ( IN_DUCK ) )
		{
			vecForce.x = gpGlobals->v_right.x;
			vecForce.y = gpGlobals->v_right.y;
			vecForce.z = 0;
			
			m_pRope->ApplyForceFromPlayer( vecForce );
		}
		if( pev->button & ( IN_JUMP ) )
		{
			vecForce.x = -gpGlobals->v_right.x;
			vecForce.y = -gpGlobals->v_right.y;
			vecForce.z = 0;
			m_pRope->ApplyForceFromPlayer( vecForce );
			//m_pRope->ApplyForceFromPlayer( Vector(100, 0, 0)  );
		}
		*/




		//Determine if any force should be applied to the rope, or if we should move around. - Solokiller
		if( pev->button & ( IN_BACK | IN_FORWARD ) )
		{
			if( ( gpGlobals->v_forward.x * gpGlobals->v_forward.x + 
				gpGlobals->v_forward.y * gpGlobals->v_forward.y - 
				gpGlobals->v_forward.z * gpGlobals->v_forward.z ) <= 0.0 )
			{
				if( m_bIsClimbing )
				{
					const float flDelta = gpGlobals->time - m_flLastClimbTime;
					m_flLastClimbTime = gpGlobals->time;
					if( pev->button & ( IN_FORWARD ) )
					{
						if( gpGlobals->v_forward.z < 0.0 )
						{
							if( !m_pRope->MoveDown( flDelta ) )
							{
								//Let go of the rope, detach. - Solokiller
								pev->movetype = MOVETYPE_WALK;
								pev->solid = SOLID_SLIDEBOX;

								m_afPhysicsFlags &= ~PFLAG_ONROPE;
								m_pRope->DetachObject();
								m_pRope = NULL;
								m_bIsClimbing = FALSE;
							}
						}
						else
						{
							m_pRope->MoveUp( flDelta );
						}
					}
					if( pev->button & ( IN_BACK ) )
					{
						if( gpGlobals->v_forward.z < 0.0 )
						{
							m_pRope->MoveUp( flDelta );
						}
						else if( !m_pRope->MoveDown( flDelta ) )
						{
							//Let go of the rope, detach. - Solokiller
							pev->movetype = MOVETYPE_WALK;
							pev->solid = SOLID_SLIDEBOX;
							m_afPhysicsFlags &= ~PFLAG_ONROPE;
							m_pRope->DetachObject();
							m_pRope = NULL;
							m_bIsClimbing = TRUE;
						}
					}
				}
				else
				{
					m_bIsClimbing = TRUE;
					m_flLastClimbTime = gpGlobals->time;
				}
			}
			else
			{
				vecForce.x = gpGlobals->v_forward.x;
				vecForce.y = gpGlobals->v_forward.y;
				vecForce.z = 0.0;
				if( pev->button & ( IN_BACK ) )
				{
					vecForce.x = -gpGlobals->v_forward.x;
					vecForce.y = -gpGlobals->v_forward.y;
					vecForce.z = 0;
				}
				m_pRope->ApplyForceFromPlayer( vecForce );
				m_bIsClimbing = FALSE;
			}
		}
		else
		{
			m_bIsClimbing = FALSE;
		}

		if( m_afButtonPressed & IN_JUMP )
		{
			//We've jumped off the rope, give us some momentum - Solokiller
			pev->movetype = MOVETYPE_WALK;
			pev->solid = SOLID_SLIDEBOX;
			m_afPhysicsFlags &= ~PFLAG_ONROPE;

			Vector vecDir = gpGlobals->v_up * 165.0 + gpGlobals->v_forward * 150.0;

			Vector vecVelocity = m_pRope->GetAttachedObjectsVelocity() * 2;

			vecVelocity.NormalizeInPlace();

			vecVelocity = vecVelocity * 200;

			pev->velocity = ( vecVelocity + vecDir );

			m_pRope->DetachObject();
			m_pRope = NULL;
			m_bIsClimbing = FALSE;
		}
		return;
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








	// So the correct flags get sent to client asap.
	//
	if ( m_afPhysicsFlags & PFLAG_ONTRAIN )
		pev->flags |= FL_ONTRAIN;
	else 
		pev->flags &= ~FL_ONTRAIN;

	// Train speed control
	if ( m_afPhysicsFlags & PFLAG_ONTRAIN )
	{
		CBaseEntity *pTrain = CBaseEntity::Instance( pev->groundentity );
		float vel;
		
		if ( !pTrain )
		{
			TraceResult trainTrace;
			// Maybe this is on the other side of a level transition
			UTIL_TraceLine( pev->origin, pev->origin + Vector(0,0,-38), ignore_monsters, ENT(pev), &trainTrace );

			// HACKHACK - Just look for the func_tracktrain classname
			if ( trainTrace.flFraction != 1.0 && trainTrace.pHit )
			pTrain = CBaseEntity::Instance( trainTrace.pHit );


			if ( !pTrain || !(pTrain->ObjectCaps() & FCAP_DIRECTIONAL_USE) || !pTrain->OnControls(pev) )
			{
				//ALERT( at_error, "In train mode with no train!\n" );
				m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
				m_iTrain = TRAIN_NEW|TRAIN_OFF;
				return;
			}
		}
		else if ( !FBitSet( pev->flags, FL_ONGROUND ) || FBitSet( pTrain->pev->spawnflags, SF_TRACKTRAIN_NOCONTROL ) || (pev->button & (IN_MOVELEFT|IN_MOVERIGHT) ) )
		{
			// Turn off the train if you jump, strafe, or the train controls go dead
			m_afPhysicsFlags &= ~PFLAG_ONTRAIN;
			m_iTrain = TRAIN_NEW|TRAIN_OFF;
			return;
		}

		pev->velocity = g_vecZero;
		vel = 0;
		if ( m_afButtonPressed & IN_FORWARD )
		{
			vel = 1;
			pTrain->Use( this, this, USE_SET, (float)vel );
		}
		else if ( m_afButtonPressed & IN_BACK )
		{
			vel = -1;
			pTrain->Use( this, this, USE_SET, (float)vel );
		}

		//MODDD - now also allows a pTrain->pev->iuser1 value of 1 to trigger a UI update.
		if (vel || pTrain->pev->iuser1 == 1)
		{
			m_iTrain = TrainSpeed(pTrain->pev->speed, pTrain->pev->impulse);
			m_iTrain |= TRAIN_ACTIVE|TRAIN_NEW;
			pTrain->pev->iuser1 = 0;
		}

	} else if (m_iTrain & TRAIN_ACTIVE)
		m_iTrain = TRAIN_NEW; // turn off train


	
	float timeDelta;


	if(oldThinkTime != -1){
		timeDelta = gpGlobals->time - oldThinkTime;
	}else{
		timeDelta = 0;
	}
	oldThinkTime = gpGlobals->time;

	
	if(m_fLongJump){


		if((global_itemBatteryPrerequisite == 0 || pev->armorvalue > 0 )){

#if LONGJUMPUSESDELAY == 1

		if ((pev->button & IN_DUCK) &&
			( pev->flDuckTime >= 0 ) &&
			//pev->velocity.Length() > 0 &&
			//!pev->button & IN_JUMP &&
			FBitSet ( pev->flags, FL_ONGROUND )

			)
		{
		
			longJumpChargeNeedsUpdate = TRUE;
		
			if(longJumpCharge != PLAYER_LONGJUMPCHARGE_MAX){
				longJumpCharge += timeDelta;
				//easyPrint("YAY timedelta: %.3f\n", timeDelta);
				if(longJumpCharge >= PLAYER_LONGJUMPCHARGE_MAX ){
					longJumpCharge = PLAYER_LONGJUMPCHARGE_MAX;
					g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "1" );	
				}

			}else{
				//easyPrint("EH timedelta: %.3f\n", timeDelta);
			}

			/*
			easyPrint("timedelta: %.3f\n", timeDelta);
			easyPrint("old think time: %.3f\n", oldThinkTime);
			easyPrint("THE JumP is %.3f\n", longJumpCharge);
			*/

		}else{
			if(longJumpCharge != 0){
				g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );
				longJumpCharge = 0;
				longJumpChargeNeedsUpdate = TRUE;
			}

		}

#else
		//long jump uses limited charges + delay b/w successive use.

		if(longJumpDelay > 0){
			longJumpDelay -= timeDelta;
		}


		if ((longJumpDelay <= 0 && (infiniteLongJumpChargeMem == 1 || longJumpCharge >= LONGJUMP_CHARGEUSE) && pev->button & IN_DUCK) &&
			( pev->flDuckTime >= 0 ) &&
			//pev->velocity.Length() > 0 &&
			//!pev->button & IN_JUMP &&
			FBitSet ( pev->flags, FL_ONGROUND )
			
			)
		{
			
			//easyPrint("YAY timedelta: %.3f\n", timeDelta);
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "1" );
			longJump_waitForRelease = TRUE;

			//Store the player's velocity length (just "speed") to "lastDuckVelocityLength", since any other references
			//to current velocity length after this will just refer to the velocity of a jump.  As in, of course there was movement,
			//I care if the duck jump is considered stationary (no movement in any direction, no long jump).
			lastDuckVelocityLength = pev->velocity.Length();

			/*
			easyPrint("timedelta: %.3f\n", timeDelta);
			easyPrint("old think time: %.3f\n", oldThinkTime);
			easyPrint("THE JumP is %.3f\n", longJumpCharge);
			*/

		}else{
			
			if(longJump_waitForRelease){

				//The player must have just jumped, and the velocity's "length" (speed as a single number) must be over 7, a requirement
				//of pm_shared.c for doing the superjump.

				if(pev->oldbuttons & IN_JUMP && lastDuckVelocityLength > 7 ){
					//easyPrint("pev->velocity? %.3f\n", lastDuckVelocityLength);

					if(infiniteLongJumpChargeMem != 1){
						longJumpCharge -= LONGJUMP_CHARGEUSE;
					}

					longJumpDelay = PLAYER_LONGJUMP_DELAY;
					longJumpChargeNeedsUpdate = TRUE;
					
					//that is, if the charge has run out BUT "infiniteLongJumpChargeMem" is 1, don't play this message (spammy)
					if(longJumpCharge == 0 && infiniteLongJumpChargeMem != 1){
						//play the out of ammo message.
						SetSuitUpdate("!HEV_LJDEPLETED", FALSE, 0);
					}

				}

				

				longJump_waitForRelease = FALSE;
				g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );
				
			}


		}



		

#endif

		}//END OF if((global_itemBatteryPrerequisite == 0 || pev->armorvalue > 0 ))



	}//END OF if(m_fLongJump)









	if (pev->button & IN_JUMP)
	{
		// If on a ladder, jump off the ladder
		// else Jump
		Jump();
	}


	// If trying to duck, already ducked, or in the process of ducking
	if ((pev->button & IN_DUCK) || FBitSet(pev->flags,FL_DUCKING) || (m_afPhysicsFlags & PFLAG_DUCKING) )
		Duck();

	


	/*
	if ( m_fLongJump &&
		(pev->button & IN_DUCK) &&
		( pev->flDuckTime > 0 ) &&
		pev->velocity.Length() > 0 &&
		!pev->button & IN_JUMP &&
		FBitSet ( pev->flags, FL_ONGROUND )
		)

		*/
	//USED TO DO SOME LONG JUMP LOGIC HERE.  Now doing it before jump instead.
	

	if ( !FBitSet ( pev->flags, FL_ONGROUND ) )
	{
		m_flFallVelocity = -pev->velocity.z;
	}

	// StudioFrameAdvance( );//!!!HACKHACK!!! Can't be hit by traceline when not animating?

	// Clear out ladder pointer
	m_hEnemy = NULL;

	if ( m_afPhysicsFlags & PFLAG_ONBARNACLE )
	{
		pev->velocity = g_vecZero;
	}




}


//MODDD - checkpoint
/* Time based Damage works as follows: 
	1) There are several types of timebased damage:

		#define DMG_PARALYZE		(1 << 14)	// slows affected creature down
		#define DMG_NERVEGAS		(1 << 15)	// nerve toxins, very bad
		#define DMG_POISON			(1 << 16)	// blood poisioning
		#define DMG_RADIATION		(1 << 17)	// radiation exposure
		#define DMG_DROWNRECOVER	(1 << 18)	// drown recovery
		#define DMG_ACID			(1 << 19)	// toxic chemicals or acid burns
		#define DMG_SLOWBURN		(1 << 20)	// in an oven
		#define DMG_SLOWFREEZE		(1 << 21)	// in a subzero freezer

	2) A new hit inflicting tbd restarts the tbd counter - each monster has an 8bit counter,
		per damage type. The counter is decremented every second, so the maximum time 
		an effect will last is 255/60 = 4.25 minutes.  Of course, staying within the radius
		of a damaging effect like fire, nervegas, radiation will continually reset the counter to max.

	3) Every second that a tbd counter is running, the player takes damage.  The damage
		is determined by the type of tdb.  
			Paralyze		- 1/2 movement rate, 30 second duration.
			Nervegas		- 5 points per second, 16 second duration = 80 points max dose.
			Poison			- 2 points per second, 25 second duration = 50 points max dose.
			Radiation		- 1 point per second, 50 second duration = 50 points max dose.
			Drown			- 5 points per second, 2 second duration.
			Acid/Chemical	- 5 points per second, 10 second duration = 50 points max.
			Burn			- 10 points per second, 2 second duration.
			Freeze			- 3 points per second, 10 second duration = 30 points max.

	4) Certain actions or countermeasures counteract the damaging effects of tbds:

		Armor/Heater/Cooler - Chemical(acid),burn, freeze all do damage to armor power, then to body
							- recharged by suit recharger
		Air In Lungs		- drowning damage is done to air in lungs first, then to body
							- recharged by poking head out of water
							- 10 seconds if swiming fast
		Air In SCUBA		- drowning damage is done to air in tanks first, then to body
							- 2 minutes in tanks. Need new tank once empty.
		Radiation Syringe	- Each syringe full provides protection vs one radiation dosage
		Antitoxin Syringe	- Each syringe full provides protection vs one poisoning (nervegas or poison).
		Health kit			- Immediate stop to acid/chemical, fire or freeze damage.
		Radiation Shower	- Immediate stop to radiation damage, acid/chemical or fire damage.
		
	
*/

// If player is taking time based damage, continue doing damage to player -
// this simulates the effect of being poisoned, gassed, dosed with radiation etc -
// anything that continues to do damage even after the initial contact stops.
// Update all time based damage counters, and shut off any that are done.

// The m_bitsDamageType bit MUST be set if any damage is to be taken.
// This routine will detect the initial on value of the m_bitsDamageType
// and init the appropriate counter.  Only processes damage every second.

//MODDD - REVERTED TO OLD TIMED DAMAGE
//these used to be commented.  Revived; the ones in cbase.h are now commented out.
//CHANGE: new constants that revolve around difficulty are used for DURATION instead.

//NOTICE: damage & duration constants moved to CBaseMonster!




void CBasePlayer::updateTimedDamageDurations(int difficultyIndex){


	//forcing some defaults here.
	if(difficultyIndex == -1){
		difficultyIndex = 2;
		//skillMem = -1;  //force update ASAP.
		//no, just make the skillMem the right version...
		skillMem = difficultyIndex;

		//about to update again anyways, so, return now?
		//NAH   return;
	}

	//switch((int)(CVAR_skillMem) ){
	switch(difficultyIndex){
	case 1://easy
		CBaseMonster::paralyzeDuration = PARALYZE_DURATION_EASY;
		CBaseMonster::nervegasDuration = NERVEGAS_DURATION_EASY;
		CBaseMonster::poisonDuration = POISON_DURATION_EASY;
		CBaseMonster::radiationDuration = RADIATION_DURATION_EASY;
		CBaseMonster::acidDuration = ACID_DURATION_EASY;
		CBaseMonster::slowburnDuration = SLOWBURN_DURATION_EASY;
		CBaseMonster::slowfreezeDuration = SLOWFREEZE_DURATION_EASY;
		CBaseMonster::bleedingDuration = BLEEDING_DURATION_EASY;
	break;
	case 2://medium
		CBaseMonster::paralyzeDuration = PARALYZE_DURATION_MEDIUM;
		CBaseMonster::nervegasDuration = NERVEGAS_DURATION_MEDIUM;
		CBaseMonster::poisonDuration = POISON_DURATION_MEDIUM;
		CBaseMonster::radiationDuration = RADIATION_DURATION_MEDIUM;
		CBaseMonster::acidDuration = ACID_DURATION_MEDIUM;
		CBaseMonster::slowburnDuration = SLOWBURN_DURATION_MEDIUM;
		CBaseMonster::slowfreezeDuration = SLOWFREEZE_DURATION_MEDIUM;
		CBaseMonster::bleedingDuration = BLEEDING_DURATION_MEDIUM;
	break;
	case 3://hard
		CBaseMonster::paralyzeDuration = PARALYZE_DURATION_HARD;
		CBaseMonster::nervegasDuration = NERVEGAS_DURATION_HARD;
		CBaseMonster::poisonDuration = POISON_DURATION_HARD;
		CBaseMonster::radiationDuration = RADIATION_DURATION_HARD;
		CBaseMonster::acidDuration = ACID_DURATION_HARD;
		CBaseMonster::slowburnDuration = SLOWBURN_DURATION_HARD;
		CBaseMonster::slowfreezeDuration = SLOWFREEZE_DURATION_HARD;
		CBaseMonster::bleedingDuration = BLEEDING_DURATION_HARD;
	break;
	}
	

}







/* */


//MODDD - checkpoint.
//~Overrides "CheckTimeBasedDamage" from monster.cpp.
void CBasePlayer::CheckTimeBasedDamage() 
{
	int i;
	BYTE bDuration = 0;

	static float gtbdPrev = 0.0;



	//MODDD - check other too!
	//if (!(m_bitsDamageType & DMG_TIMEBASED))
	if (!(m_bitsDamageType & DMG_TIMEBASED) && !(m_bitsDamageTypeMod & (DMG_TIMEBASEDMOD))  )
		return;

	// only check for time based damage approx. every 2 seconds
	if (abs(gpGlobals->time - m_tbdPrev) < 2.0)
		return;
	
	m_tbdPrev = gpGlobals->time;

	for (i = 0; i < CDMG_TIMEBASED; i++)
	{


		int* m_bitsDamageTypeRef = 0;
		if(i <= 7){
			//use the old bitmask.
			m_bitsDamageTypeRef = &m_bitsDamageType;
		}else{
			//use the new bitmask.
			m_bitsDamageTypeRef = &m_bitsDamageTypeMod;
		}


		// make sure bit is set for damage type
		//if (m_bitsDamageType & (DMG_PARALYZE << i))
		if((*m_bitsDamageTypeRef) & (convert_itbd_to_damage(i))  )
		{

			


			int damageType = 0;
			/*
			if(timedDamageIgnoresArmorMem == 1){
				//timed damage ignores armor.
				damageType = DMG_FALL;
			}else{
				//timed damage hits armor first.
				damageType = DMG_GENERIC;
			}
			*/
			//no, can't just do that.

			//instead, a damgeType being "DMG_TIMEDEFFECT" means,
			//it is generic, but use this for telling whether to
			//apply the armor-bypass or not without the sideeffect of what-
			//ever happens to DMG_FALL.
			damageType = DMG_TIMEDEFFECT;


			switch (i)
			{
				//NOTE: should these ignore armor or not?  I feel like they kind of should.
				//If so, make the damage type (DMG_GENERIC) become "DMG_FALL" instead (known to ignore armor).
			case itbd_Paralyze:
				// UNDONE - flag movement as half-speed
				bDuration = paralyzeDuration;
				//MODDD - see above.
				//bDuration = PARALYZE_DURATION;
				break;
			case itbd_NerveGas:
				//MODDD - comment undone.
				TakeDamage(pev, pev, NERVEGAS_DAMAGE, 0, damageType);	
				
				bDuration = nervegasDuration;
				//MODDD - see above.
				//bDuration = NERVEGAS_DURATION;
				break;
			case itbd_Poison:
				TakeDamage(pev, pev, POISON_DAMAGE, 0, damageType | DMG_TIMEDEFFECTIGNORE);
				
				bDuration = poisonDuration;
				//MODDD - see above.
				//bDuration = POISON_DURATION;
				break;
			case itbd_Radiation:
				//MODDD - comment on "TakeDamage" undone.
				TakeDamage(pev, pev, RADIATION_DAMAGE, 0, damageType | DMG_TIMEDEFFECTIGNORE);
				
				bDuration = radiationDuration;
				//MODDD - see above.
				//bDuration = RADIATION_DURATION;
				break;
			case itbd_DrownRecover:
				// NOTE: this hack is actually used to RESTORE health
				// after the player has been drowning and finally takes a breath
				if (m_idrowndmg > m_idrownrestored)
				{
					int idif = min(m_idrowndmg - m_idrownrestored, 10);

					TakeHealth(idif, DMG_GENERIC);
					m_idrownrestored += idif;
				}
				bDuration = 4;	// get up to 5*10 = 50 points back
				break;
			case itbd_Acid:
				//MODDD - comment undone.
				TakeDamage(pev, pev, ACID_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = acidDuration;
				//bDuration = ACID_DURATION;
				break;
			case itbd_SlowBurn:
				//MODDD - comment undone.
				TakeDamage(pev, pev, SLOWBURN_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = slowburnDuration;
				//bDuration = SLOWBURN_DURATION;
				break;
			case itbd_SlowFreeze:
				//easyPrintLine("DO YOU EVER TAKE FREEZE DAMAGE?");
				//this won't be called, as the map's called freeze effect never starts like this.
				//MODDD - comment undone.
				TakeDamage(pev, pev, SLOWFREEZE_DAMAGE, 0, damageType);
				//MODDD - see above.
				
				bDuration = slowfreezeDuration;
				//bDuration = SLOWFREEZE_DURATION;
				break;
				//MODDD - new.
			case itbd_Bleeding:
				//this will always ignore the armor (hence DMG_TIMEDEFFECT).
				TakeDamage(pev, pev, BLEEDING_DAMAGE, 0, damageType | DMG_TIMEDEFFECTIGNORE);
				
				bDuration = bleedingDuration;
				break;

			default:
				bDuration = 0;
			}




			/*
			MODDD - diagnositic.
			if(i == itbd_Radiation){
					easyPrintLine("timebaseddamage for rad? %d", m_rgbTimeBasedDamage[i]);
			}
			*/


			if (m_rgbTimeBasedDamage[i] > 0)
			{
				// use up an antitoxin on poison or nervegas after a few seconds of damage
				//MODDD - instead of referring to constants like "NERVEGASDURATION", it is referring to the
				//variable "nervegasDuration", which is set according to difficulty.  Same for poison.
				if (((i == itbd_NerveGas) && (m_rgbTimeBasedDamage[i] < nervegasDuration)) ||
					((i == itbd_Poison)   && (m_rgbTimeBasedDamage[i] < poisonDuration)))
				{

					//6m_rgItems[ITEM_ADRENALINE]

					if (!antidoteQueued && m_rgItems[ITEM_ANTIDOTE] && (global_itemBatteryPrerequisite == 0 || pev->armorvalue > 0 )  )
					{
						antidoteQueued = TRUE;
						//not yet!  Wait for the hissing sound.
						//m_rgbTimeBasedDamage[i] = 0;
						//m_rgItems[ITEM_ANTIDOTE]--;

						//MODDD - this used to refer to "HEV_HEAL4".  "HEV_HEAL5" refers to an antidote,
						//HEAL4, re-used below for the radiation item (power canister / syringe), refers to "anti-toxins".
						//SetSuitUpdateFVoxlessFriendlyEvent("!HEV_ANT_USE", FALSE, SUIT_REPEAT_OK, -1, -2, consumeAntidote);
						SetSuitUpdateEventFVoxCutoff("!HEV_ANT_USE", FALSE, SUIT_REPEAT_OK, 1.36, &CBasePlayer::consumeAntidote, 1.36 + 0.55);
					}
				}
				
				//MODDD - for the radiation instead.  Ditto for the variable.
				if (((i == itbd_Radiation) && (m_rgbTimeBasedDamage[i] < radiationDuration))  )
				{

					if (!radiationQueued && m_rgItems[ITEM_RADIATION] && (global_itemBatteryPrerequisite == 0 || pev->armorvalue > 0 ))
					{
						radiationQueued = TRUE;

						//m_rgbTimeBasedDamage[i] = 0;
						//m_rgItems[ITEM_RADIATION]--;
						//SetSuitUpdate("!HEV_RAD_USE", FALSE, SUIT_REPEAT_OK);

						SetSuitUpdateEventFVoxCutoff("!HEV_RAD_USE", FALSE, SUIT_REPEAT_OK, 1.28, &CBasePlayer::consumeRadiation, 1.28 + 0.55);
					}
					//MODDD - TODO - have damage types + use a radiation syringe / power canister.


				}


				if(g_iSkillLevel == 3 && global_timedDamageEndlessOnHard == 1){
					//Hard mode is on, and "timedDamageEndlessOnHard" is on...
					//Do NOT decrement non-curable durations.
					//However, still decrement only ONCE on curables to satisfy the one-second-passing rule for canisters to work.
					if(!m_rgbTimeBasedFirstFrame[i] &&
						(i == itbd_NerveGas || i == itbd_Poison || i ==  itbd_Radiation || i == itbd_Bleeding) ){
						//DO NOTHING.  Only the appropriate cure can fix it.
					}else if(m_rgbTimeBasedDamage[i] != 0){
						m_rgbTimeBasedDamage[i]--;
						m_rgbTimeBasedFirstFrame[i] = FALSE;
					}

				}else{
					//work as normal.  Decrement durations.
					if(m_rgbTimeBasedDamage[i] != 0){
						m_rgbTimeBasedDamage[i]--;
					}
				}

				// decrement damage duration, detect when done.
				//MODDD - change to how that works.
				//if (!m_rgbTimeBasedDamage[i] || --m_rgbTimeBasedDamage[i] == 0)
				if(!m_rgbTimeBasedDamage[i])
				{
					removeTimedDamage(i, m_bitsDamageTypeRef);
				}

			}
			else{
				// first time taking this damage type - init damage duration
				//MODDD - probably a bit redundant, but ah well.
				m_rgbTimeBasedFirstFrame[i] = TRUE;
				m_rgbTimeBasedDamage[i] = bDuration;
			}
		}
	}
}//END OF CheckTimeBasedDamage(...)


void CBasePlayer::removeTimedDamage(int arg_type, int* m_bitsDamageTypeRef){
	m_rgbTimeBasedDamage[arg_type] = 0;
	//MODDD
	m_rgbTimeBasedFirstFrame[arg_type] = TRUE;
	//MODDD
	// if we're done, clear damage bits
	//m_bitsDamageType &= ~(DMG_PARALYZE << i);	
	(*m_bitsDamageTypeRef) &= ~(convert_itbd_to_damage(arg_type));
}//END OF removeTimedDamage(...)





/*
THE POWER SUIT

The Suit provides 3 main functions: Protection, Notification and Augmentation. 
Some functions are automatic, some require power. 
The player gets the suit shortly after getting off the train in C1A0 and it stays
with him for the entire game.

Protection

	Heat/Cold
		When the player enters a hot/cold area, the heating/cooling indicator on the suit 
		will come on and the battery will drain while the player stays in the area. 
		After the battery is dead, the player starts to take damage. 
		This feature is built into the suit and is automatically engaged.
	Radiation Syringe
		This will cause the player to be immune from the effects of radiation for N seconds. Single use item.
	Anti-Toxin Syringe
		This will cure the player from being poisoned. Single use item.
	Health
		Small (1st aid kits, food, etc.)
		Large (boxes on walls)
	Armor
		The armor works using energy to create a protective field that deflects a
		percentage of damage projectile and explosive attacks. After the armor has been deployed,
		it will attempt to recharge itself to full capacity with the energy reserves from the battery.
		It takes the armor N seconds to fully charge. 

Notification (via the HUD)

x	Health
x	Ammo  
x	Automatic Health Care
		Notifies the player when automatic healing has been engaged. 
x	Geiger counter
		Classic Geiger counter sound and status bar at top of HUD 
		alerts player to dangerous levels of radiation. This is not visible when radiation levels are normal.
x	Poison
	Armor
		Displays the current level of armor. 

Augmentation 

	Reanimation (w/adrenaline)
		Causes the player to come back to life after he has been dead for 3 seconds. 
		Will not work if player was gibbed. Single use.
	Long Jump
		Used by hitting the ??? key(s). Caused the player to further than normal.
	SCUBA	
		Used automatically after picked up and after player enters the water. 
		Works for N seconds. Single use.	
	
Things powered by the battery

	Armor		
		Uses N watts for every M units of damage.
	Heat/Cool	
		Uses N watts for every second in hot/cold area.
	Long Jump	
		Uses N watts for every jump.
	Alien Cloak	
		Uses N watts for each use. Each use lasts M seconds.
	Alien Shield	
		Augments armor. Reduces Armor drain by one half
 
*/

// if in range of radiation source, ping geiger counter

#define GEIGERDELAY 0.25

int CBasePlayer::getGeigerChannel(){
	return global_geigerChannel;
}



void CBasePlayer :: UpdateGeigerCounter( void )
{

	//only the server is calling this.
	
	//can run into a "total_channels == MAX_CHANNEL" printout from the engine if too many sounds are queued up while the game is paused.
	//This issue should be fixed now.


	
	//easyForcePrintLine("WHAT THE DUCKIN DUCK %.8f %.8f d:%.8f %d", gpGlobals->time, previousFrameTime, (gpGlobals->time - previousFrameTime), frameElapsed);
	//easyForcePrintLine("WELL???!!! %d", frameElapsed);


	BYTE range;

	// delay per update ie: don't flood net with these msgs
	//MODDD - the geiger sound should still attempt to play each frame when in range, like it did in the client.
	//if (gpGlobals->time < m_flgeigerDelay)
	//	return;
	if (gpGlobals->time >= m_flgeigerDelay){

	m_flgeigerDelay = gpGlobals->time + GEIGERDELAY;
		
	// send range to radition source to client

	range = (BYTE) (m_flgeigerRange / 4);


	if (range != m_igeigerRangePrev)
	{
		m_igeigerRangePrev = range;

		MESSAGE_BEGIN( MSG_ONE, gmsgGeigerRange, NULL, pev );
			WRITE_BYTE( range );
		MESSAGE_END();
	}


	// reset counter and semaphore
	if (!RANDOM_LONG(0,3))
		m_flgeigerRange = 1000;

	//MODDD - sound-related clientside geiger.cpp script moved here for better control.

	}//END OF if (gpGlobals->time < m_flgeigerDelay)



	int pct;
	float flvol;
	float rg[3];
	int i;


	
	

	//MODDD - around the time we start hearing geiger sounds, play "radiation detected".
	if(m_flgeigerRange < 800){
		if(!foundRadiation){
			SetSuitUpdate("!HEV_DET2", FALSE, SUIT_NEXT_IN_1MIN);	// radiation detected
		}else{
			forceRepeatBlock("!HEV_DET2", FALSE, SUIT_NEXT_IN_1MIN);
			//after leaving radiation, the use
		}
		foundRadiation = TRUE;
	}//END OF if(m_flgeigerRange < 800)
	else{
		//mark to let the suit's timer cool down.
		foundRadiation = FALSE;
	}


	if (m_flgeigerRange < 1000 && m_flgeigerRange > 0)
	{

		

		//MODDD - this has been moved to player.cpp for better control over channels.
		//(see the method "UpdateGeigerCounter").
		
		// peicewise linear is better than continuous formula for this
		if (m_flgeigerRange > 800)
		{
			pct = 0;			//Con_Printf ( "range > 800\n");
		}
		else if (m_flgeigerRange > 600)
		{
			pct = 2;
			flvol = 0.4;		//Con_Printf ( "range > 600\n");
			rg[0] = 1;
			rg[1] = 1;
			i = 2;
		}
		else if (m_flgeigerRange > 500)
		{
			pct = 4;
			flvol = 0.5;		//Con_Printf ( "range > 500\n");
			rg[0] = 1;
			rg[1] = 2;
			i = 2;
		}
		else if (m_flgeigerRange > 400)
		{
			pct = 8;
			flvol = 0.6;		//Con_Printf ( "range > 400\n");
			rg[0] = 1;
			rg[1] = 2;
			rg[2] = 3;
			i = 3;
		}
		else if (m_flgeigerRange > 300)
		{
			pct = 8;
			flvol = 0.7;		//Con_Printf ( "range > 300\n");
			rg[0] = 2;
			rg[1] = 3;
			rg[2] = 4;
			i = 3;
		}
		else if (m_flgeigerRange > 200)
		{
			pct = 28;
			flvol = 0.78;		//Con_Printf ( "range > 200\n");
			rg[0] = 2;
			rg[1] = 3;
			rg[2] = 4;
			i = 3;
		}
		else if (m_flgeigerRange > 150)
		{
			pct = 40;
			flvol = 0.80;		//Con_Printf ( "range > 150\n");
			rg[0] = 3;
			rg[1] = 4;
			rg[2] = 5;
			i = 3;
		}
		else if (m_flgeigerRange > 100)
		{
			pct = 60;
			flvol = 0.85;		//Con_Printf ( "range > 100\n");
			rg[0] = 3;
			rg[1] = 4;
			rg[2] = 5;
			i = 3;
		}
		else if (m_flgeigerRange > 75)
		{
			pct = 80;
			flvol = 0.9;		//Con_Printf ( "range > 75\n");
			//gflGeigerDelay = cl.time + GEIGERDELAY * 0.75;
			rg[0] = 4;
			rg[1] = 5;
			rg[2] = 6;
			i = 3;
		}
		else if (m_flgeigerRange > 50)
		{
			pct = 90;
			flvol = 0.95;		//Con_Printf ( "range > 50\n");
			rg[0] = 5;
			rg[1] = 6;
			i = 2;
		}
		else
		{
			pct = 95;
			flvol = 1.0;		//Con_Printf ( "range < 50\n");
			rg[0] = 5;
			rg[1] = 6;
			i = 2;
		}


		
		flvol = (flvol * ((rand() & 127)) / 255) + 0.25; // UTIL_RandomFloat(0.25, 0.5);

		if ((rand() & 127) < pct || (rand() & 127) < pct)
		{
			//S_StartDynamicSound (-1, 0, rgsfx[rand() % i], r_origin, flvol, 1.0, 0, 100);	
			char sz[256];
			
			int j = rand() & 1;
			if (i > 2)
				j += rand() & 1;




			sprintf(sz, "player/geiger%d.wav", j + 1);
			//EMIT_SOUND(ENT(pev), CHAN_ITEM, sz, flvol, 0.05f);

			//prrrr
			Vector headPos;

			headPos.x = pev->origin.x + pev->view_ofs.x;
			headPos.y = pev->origin.y + pev->view_ofs.y;
			headPos.z = pev->origin.z + pev->view_ofs.z;


			//ATTN_NORM is used for the suit?  I thought it would be "ATTN_STATIC".
			//Should the suit not use CHAN_STATIC if in multiplayer?  Other comments here warn against using "CHAN_STATIC" in multiplayer.
			
			//MODDD - soundsentencesave... CANCELED
			EMIT_SOUND_DYN(ENT(pev), getGeigerChannel(), sz, flvol, ATTN_NORM, 0, 100);

			///EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_pain5.wav", 1, ATTN_NORM);
			//UTIL_EmitAmbientSound(ENT(pev), headPos, sz, flvol, ATTN_STATIC, 0, 100);



			//sprintf(sz, "player/geiger%d.wav", j + 1);
			//PlaySound(sz, flvol);
			
		}
		
	}


	
	//easyForcePrintLine("DO I BUCKO %d", usesSoundSentenceSave() );

}

/*
================
CheckSuitUpdate

Play suit update if it's time
================
*/

//#define SUITSOUNDFILTER(SND) EMIT_SOUND_SUIT(ENT(pev), SND);
inline
void CBasePlayer::suitSoundFilter(const char* snd){

	char dest[127];
	dest[0] = '!';

	strcpy( &dest[1], snd);

	EMIT_SOUND_SUIT( ENT(pev), snd);

	//strcpy
	//strncpy


}


#define SUITUPDATETIME	3.5
#define SUITFIRSTUPDATETIME 0.1

void CBasePlayer::CheckSuitUpdate()
{
	int i;
	int isentence = 0;
	int isearch = m_iSuitPlayNext;
	
	// Ignore suit updates if no suit
	if ( !(pev->weapons & (1<<WEAPON_SUIT)) )
		return;

	// if in range of radiation source, ping geiger counter
	UpdateGeigerCounter();

	if ( g_pGameRules->IsMultiplayer() )
	{
		// don't bother updating HEV voice in multiplayer.
		return;
	}

	if ( gpGlobals->time >= m_flSuitUpdate && m_flSuitUpdate > 0)
	{


		//MODDD - this will force going to another custom clip if given.
		if(obligedCustomSentence == 0){

			// play a sentence off of the end of the queue
			for (i = 0; i < CSUITPLAYLIST; i++)
			{
			if (isentence = m_rgSuitPlayList[isearch])
				break;
			
			if (++isearch == CSUITPLAYLIST)
				isearch = 0;
			}


			if(isentence >= 10000){
				//playing a real-time battery speech.
				batterySayPhase = 0;
			}else{
				//non-battery related messages will reset "recentlySaidBattery".
				recentlySaidBattery = -1;
			}

		}else{
			isentence = obligedCustomSentence;
			//reset, needs to be another "obligedCustomSentence" to happen again.
			obligedCustomSentence = 0;
			
		}

		
		//COMMENTED OUT, annoying fast.
		//easyPrintLine ("WHAT IS SENTENCE %d", isentence );

		if (isentence)
		{
			m_rgSuitPlayList[isearch] = 0;

			
			//Clearly "isearch" is the picked entry to play.  Get the other info needed and then clear it too.
			
			if(m_rgSuitPlayListEventDelay[isearch] != -1){
				currentSuitSoundEventTime = gpGlobals->time + m_rgSuitPlayListEventDelay[isearch];
				currentSuitSoundEvent = m_rgSuitPlayListEvent[isearch];
			}else{
				currentSuitSoundEventTime = -1;
				currentSuitSoundEvent = NULL;
			}


			if(!fvoxOn && m_rgSuitPlayListFVoxCutoff[isearch] != -1){
				currentSuitSoundFVoxCutoff = gpGlobals->time + m_rgSuitPlayListFVoxCutoff[isearch];
				sentenceFVoxCutoffStop = isentence;
			}else{
				currentSuitSoundFVoxCutoff = -1;
				sentenceFVoxCutoffStop = -1;
			}
			


			m_rgSuitPlayListEventDelay[isearch] = -1;
			m_rgSuitPlayListEvent[isearch] = NULL;

			



			if (isentence > 0)
			{
				
				// play sentence number
				//MODDD - if the sentence number is less than 10,000, it is an ordinary sentence.
				//Play as usual.
				if(isentence < 10000){
					char sentence[CBSENTENCENAME_MAX+1];
					strcpy(sentence, "!");
					strcat(sentence, gszallsentencenames[isentence]);
					EMIT_SOUND_SUIT(ENT(pev), sentence);


					m_flSuitUpdate = gpGlobals->time + m_rgSuitPlayListDuration[isearch];


				}else{
					//otherwise, this is something custom:

					
					int determiner = isentence - 10000;
					float timeToSay = 0.8f;

					



					if(getBatteryValueRealTime && batterySayPhase < 1){
						isentence = 10000 + pev->armorvalue;
						determiner = isentence - 10000;
					}



					if(batterySayPhase == 0){
						//use the current battery value.
						//isentence = 10000 + pev->armorvalue;

						//First, does a recent battery reading match what we will say now (like picking up 2
						//batteries and taking no damage between the time to play those messages)?
						//If so, don't play this number.
						if(recentlySaidBattery == determiner){
							m_flSuitUpdate = gpGlobals->time + timeToSay;
							batterySayPhase = -1;
							return;
						}


						recentlySaidBattery = determiner;  //do not repeat this immediately.

						if(determiner != 0){
							//say the first part (power is) and end later (percent).
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_BNOTICE");
							timeToSay = 0.68f;

							obligedCustomSentence = isentence;
							determiner = 0;

							m_flSuitUpdate = gpGlobals->time + timeToSay;
							batterySayPhase++;
							return;	
						}

						/*
						if(batteryInitiative){
							
							
						}else{
							determiner = isentence - 10000;
						}
						*/
					}else if(batterySayPhase == 2){

						EMIT_SOUND_SUIT(ENT(pev), "!HEV_BPERCENT");
						timeToSay = 0.77f;
						m_flSuitUpdate = gpGlobals->time + timeToSay;
						batterySayPhase = -1;
						return;	
					}
					



					if(determiner >= 10 && timeToSay < 20){
						//extra time for the teens (and 11, 12):
						timeToSay = 1.12f;
					}
					if(determiner >= 20){
						timeToSay = 0.9f;
					}

					//EMIT_SOUND_SUIT(ENT(pev), sentence);

					

					switch(determiner){
						//NOTE: case 0 should be handled separately (HEV_NOPOWER).
					
					case 0:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_NOPOWER");
						timeToSay = 1.9f;
						batterySayPhase = -1;
						//do not follow up with "percent" if that was planned.
					break;
					case 1:
						//EMIT_SOUND_SUIT(ENT(pev), "fvox/one.wav");
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb1");
						timeToSay = 0.77f;
					break;
					case 2:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb2");
						timeToSay = 0.75f;
					break;
					case 3:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb3");
						timeToSay = 0.83f;
					break;
					case 4:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb4");
						timeToSay = 0.85f;
					break;
					case 5:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb5");
						timeToSay = 0.99f;
					break;
					case 6:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb6");
						timeToSay = 0.90f;
					break;
					case 7:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb7");
						timeToSay = 0.83f;
					break;
					case 8:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb8");
						timeToSay = 0.75f;
					break;
					case 9:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb9");
						timeToSay = 0.86f;
					break;
					case 10:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb10");
						timeToSay = 0.74f;
					break;
					case 11:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb11");
						timeToSay = 0.88f;
					break;
					case 12:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb12");
						timeToSay = 0.88f;
					break;
					case 13:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb13");
						timeToSay = 1.13f;

					break;
					case 14:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb14");
						timeToSay = 1.10f;
					break;
					case 15:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb15");
						timeToSay = 1.10f;
					break;
					case 16:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb16");
						timeToSay = 1.11f;
					break;
					case 17:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb17");
						timeToSay = 1.16f;
					break;
					case 18:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb18");
						timeToSay = 0.84f;
					break;
					case 19:
						EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb19");
						timeToSay = 0.85f;
					break;
					default:

						if(determiner >= 20 && determiner < 30){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb20");
							obligedCustomSentence = isentence - 20;
							timeToSay = 0.79f;
						}else if(determiner >= 30 && determiner < 40){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb30");
							obligedCustomSentence = isentence - 30;
							timeToSay = 0.85f;
						}else if(determiner >= 40 && determiner < 50){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb40");
							obligedCustomSentence = isentence - 40;
							timeToSay = 0.85f;
						}else if(determiner >= 50 && determiner < 60){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb50");
							obligedCustomSentence = isentence - 50;
							timeToSay = 0.89f;
						}else if(determiner >= 60 && determiner < 70){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb60");
							obligedCustomSentence = isentence - 60;
							timeToSay = 1.00f;
						}else if(determiner >= 70 && determiner < 80){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb70");
							obligedCustomSentence = isentence - 70;
							timeToSay = 0.98;
						}else if(determiner >= 80 && determiner < 90){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb80");
							obligedCustomSentence = isentence - 80;
							timeToSay = 0.76f;
						}else if(determiner >= 90 && determiner < 100){
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb90");
							obligedCustomSentence = isentence - 90;
							timeToSay = 0.91f;
						}else if(determiner == 100){
							
							EMIT_SOUND_SUIT(ENT(pev), "!HEV_numb100");
							obligedCustomSentence = isentence - 100;
							timeToSay = 0.96f;
						}


						if(obligedCustomSentence == 10000){
							obligedCustomSentence = 0;
							//why?  because "obligedCustomSentence" being 10,000 means saying "zero".
							//You do not say "twenty zero" or "fourty zero" to denote a zero place in the one's
							//position when saying a word.  It's just "twenty" or "fourty", so forcing
							//"obligedCustomSentence" to 0 achieves this: don't queue anything after saying the
							//ten's place.

							if(determiner >= 20 && determiner <= 100){
								//add an extra pause after this if not followed by anything.
								timeToSay += 0.10f;
							}

						}
						
						
					break;

					}//END OF switch(determiner)

					if(batterySayPhase == 1 && obligedCustomSentence == 0 ){   //determiner > 0 && determiner != 5001){
						//if not 0, 

						obligedCustomSentence = 10000;
						batterySayPhase++;
						//queue saying "percent".

					}


					//NO, the suitPlayDuration will be ignored for these custom calls.
					//Standardize it or customize it above.
					//m_flSuitUpdate = gpGlobals->time + m_rgSuitPlayListDuration[isearch];
					m_flSuitUpdate = gpGlobals->time + timeToSay;
					
				}//END OF else OF if(isentence < 10000)
				//EMIT_SOUND_SUIT(ENT(pev), "scientist/scream25.wav");
				//EMIT_SOUND_DYN(ENT(pev), CHAN_STATIC, "scientist/scream25.wav", CVAR_GET_FLOAT("suitvolume"), ATTN_NORM, 0, 100);


			}
			else
			{
				// play sentence group
				EMIT_GROUPID_SUIT(ENT(pev), -isentence);

				m_flSuitUpdate = gpGlobals->time + m_rgSuitPlayListDuration[isearch];
			}






			//m_iSuitPlayNext
		//m_flSuitUpdate = gpGlobals->time + SUITUPDATETIME;
		//NOTICE: line moved above, per the "if-else" scopes for more control each time.
		//m_flSuitUpdate = gpGlobals->time + m_rgSuitPlayListDuration[isearch];
		}
		else
			// queue is empty, don't check 
			m_flSuitUpdate = 0;
	}
}






BOOL CBasePlayer::SetSuitUpdatePRE(){
	return SetSuitUpdatePRE(FALSE);
}//END OF SetSuitUpdatePRE(...)

//MODDD - new method to contain things commonly repeated throughout "SetSuitUpdate" method clones before the action.
// Returns a boolean (true/false) whether this request is allowed to happen or blocked.
BOOL CBasePlayer::SetSuitUpdatePRE(BOOL fvoxException ){
	
	// Ignore suit updates if no suit

	if ( !(pev->weapons & (1<<WEAPON_SUIT))  )
		return FALSE;
	
	if ( g_pGameRules->IsMultiplayer() )
	{
		// due to static channel design, etc. We don't play HEV sounds in multiplayer right now.
		return FALSE;
	}

	//MODDD - also don't play if FVOX is no longer "enabled" and this is NOT the notification to turn it on / off (exceptions).
	//if(fvoxOn == 0 && !(name == "!HEV_V0" || name == "!HEV_V1")  ){
	if(fvoxOn == 0 && !fvoxException){
		return FALSE;
	}


	//made it here? sounds ok.
	return TRUE;

}//END OF SetSuitUpdatePRE(...)

//MODDD - assume this is not an exception to the "fvoxOn" setting (whether to play FVox sounds or not)
BOOL CBasePlayer::SetSuitUpdatePRE(char *name, int fgroup, int& isentence ){
	return SetSuitUpdatePRE(name, fgroup, isentence, FALSE);
}

//MODDD - new method to contain things commonly repeated throughout "SetSuitUpdate" method clones before the action.
// Returns a boolean (true/false) whether this request is allowed to happen or blocked.
BOOL CBasePlayer::SetSuitUpdatePRE(char *name, int fgroup, int& isentence, BOOL fvoxException ){
	
	int i;
	//int isentence;
	
	
	// Ignore suit updates if no suit

	if ( !(pev->weapons & (1<<WEAPON_SUIT))  )
		return FALSE;
	
	if ( g_pGameRules->IsMultiplayer() )
	{
		// due to static channel design, etc. We don't play HEV sounds in multiplayer right now.
		return FALSE;
	}


	//MODDD - also don't play if FVOX is no longer "enabled" and this is NOT the notification to turn it on / off (exceptions).
	//if(fvoxOn == 0 && !(name == "!HEV_V0" || name == "!HEV_V1")  ){
	if(fvoxOn == 0 && !fvoxException){
		return FALSE;
	}



	// if name == NULL, then clear out the queue

	if (!name)
	{
		for (i = 0; i < CSUITPLAYLIST; i++)
			m_rgSuitPlayList[i] = 0;
		return FALSE;
	}
	// get sentence or group number
	if (!fgroup)
	{
		isentence = SENTENCEG_Lookup(name, NULL);
		if (isentence < 0)
			return FALSE;
	}
	else
		// mark group number as negative
		isentence = -SENTENCEG_GetIndex(name);

	//made it here? sounds ok.
	return TRUE;

}//END OF SetSuitUpdatePRE(...)










BOOL CBasePlayer::SetSuitUpdatePOST(int iempty, int isentence, float fNoRepeatTime, float playDuration, BOOL canPlay){

	return SetSuitUpdateEventPOST(iempty, isentence, fNoRepeatTime, playDuration, canPlay, -1, NULL);
	
}//END OF SetSuitUpdatePOST(...)


BOOL CBasePlayer::SetSuitUpdateEventPOST(int iempty, int isentence, float fNoRepeatTime, float playDuration, BOOL canPlay, float eventDelay, void (CBasePlayer::*eventMethod)() ){
	
	return SetSuitUpdateEventFVoxCutoffPOST(iempty, isentence, fNoRepeatTime, playDuration, canPlay, eventDelay, eventMethod, -1);
}

BOOL CBasePlayer::SetSuitUpdateEventFVoxCutoffPOST(int iempty, int isentence, float fNoRepeatTime, float playDuration, BOOL canPlay, float eventDelay, void (CBasePlayer::*eventMethod)(), float fvoxCutoff ){

	

	if (fNoRepeatTime)
	{
		if (iempty < 0)
			iempty = RANDOM_LONG(0, CSUITNOREPEAT-1); // pick random slot to take over
		m_rgiSuitNoRepeat[iempty] = isentence;
		m_rgflSuitNoRepeatTime[iempty] = fNoRepeatTime + gpGlobals->time;
	}

	// find empty spot in queue, or overwrite last spot
	
	
	//MODDD - ADDITION
	//============================================================================
	if(!canPlay){
		return FALSE;
	}
	//============================================================================



	m_rgSuitPlayList[m_iSuitPlayNext] = isentence;
	m_rgSuitPlayListDuration[m_iSuitPlayNext] = playDuration;

	//defaults.
	//m_rgSuitPlayListEventDelay[m_iSuitPlayNext] = -1;
	//m_rgSuitPlayListEvent[m_iSuitPlayNext] = NULL;
	m_rgSuitPlayListEventDelay[m_iSuitPlayNext] = eventDelay;
	m_rgSuitPlayListEvent[m_iSuitPlayNext] = eventMethod;

	m_rgSuitPlayListFVoxCutoff[m_iSuitPlayNext] = fvoxCutoff;
	
	m_iSuitPlayNext++;



	if (m_iSuitPlayNext == CSUITPLAYLIST)
		m_iSuitPlayNext = 0;

	if (m_flSuitUpdate <= gpGlobals->time)
	{
		if (m_flSuitUpdate == 0)
			// play queue is empty, don't delay too long before playback
			m_flSuitUpdate = gpGlobals->time + SUITFIRSTUPDATETIME;
		else{
			//MODDD - no, trust the update is fine from when it was last set, let it continue unabridged.
			//m_flSuitUpdate = gpGlobals->time + SUITUPDATETIME; 
		}
	}


	return TRUE;
}//END OF SetSuitUpdateEventFVoxCutoffPOST(...)












BOOL CBasePlayer::SetSuitUpdateNoRepeatSweep(int& iempty, int isentence){
	int i;

	//MODDD - ADDITION - innocent (TRUE) until proven guilty (FALSE).
	//============================================================================
	BOOLEAN canPlay = TRUE;
	//============================================================================
	
	// check norepeat list - this list lets us cancel
	// the playback of words or sentences that have already
	// been played within a certain time.

	for (i = 0; i < CSUITNOREPEAT; i++)
	{
		if (isentence == m_rgiSuitNoRepeat[i])
			{
			// this sentence or group is already in 
			// the norepeat list

				if (m_rgflSuitNoRepeatTime[i] < gpGlobals->time){
					// norepeat time has expired, clear it out
					m_rgiSuitNoRepeat[i] = 0;
					m_rgflSuitNoRepeatTime[i] = 0.0;
					iempty = i;
					break;
				}
				else{
					//Clear it out anyways!  Reset the timer.
					m_rgiSuitNoRepeat[i] = 0;
					m_rgflSuitNoRepeatTime[i] = 0.0;
					iempty = i;
					canPlay = FALSE;
					//return;
				}
			}
		// keep track of empty slot
		if (!m_rgiSuitNoRepeat[i])
			iempty = i;
	}

	return canPlay;

}//END OF SetSuitUpdateNoRepeatSweep(...)

BOOL CBasePlayer::SetSuitUpdateCheckNoRepeatApply(int& iempty, int isentence){
	int i;
	
	// check norepeat list - this list lets us cancel
	// the playback of words or sentences that have already
	// been played within a certain time.

	for (i = 0; i < CSUITNOREPEAT; i++)
	{
		if (isentence == m_rgiSuitNoRepeat[i])
		{
		// this sentence or group is already in 
		// the norepeat list

		if (m_rgflSuitNoRepeatTime[i] < gpGlobals->time)
			{
			// norepeat time has expired, clear it out
			m_rgiSuitNoRepeat[i] = 0;
			m_rgflSuitNoRepeatTime[i] = 0.0;
			iempty = i;
			break;
			}
		else
			{
			// don't play, still marked as norepeat
			return FALSE;
			}
		}
		// keep track of empty slot
		if (!m_rgiSuitNoRepeat[i])
			iempty = i;
	}
	return TRUE;  //I think?
}//END OF SetSuitUpdateNoRepeatSweep(...)

BOOL CBasePlayer::SetSuitUpdateCheckNoRepeat(int& iempty, int isentence){
	int i;
	// check norepeat list - this list lets us cancel
	// the playback of words or sentences that have already
	// been played within a certain time.


	for (i = 0; i < CSUITNOREPEAT; i++)
	{
		if (isentence == m_rgiSuitNoRepeat[i])
			{
			// this sentence or group is already in 
			// the norepeat list

				if (m_rgflSuitNoRepeatTime[i] < gpGlobals->time){
					// norepeat time has expired, clear it out
					//m_rgiSuitNoRepeat[i] = 0;
					//m_rgflSuitNoRepeatTime[i] = 0.0;
					//iempty = i;
					//nah, just RETURN TRUE below.
					break;
				}
				else{
					//Clear it out anyways!  Reset the timer.  ...no, this means we can't play.
					//m_rgiSuitNoRepeat[i] = 0;
					//m_rgflSuitNoRepeatTime[i] = 0.0;
					//iempty = i;
					return FALSE;
				}
			}

		// keep track of empty slot
		//NOTE - iempty won't be referred to.  Remove iempty as a parameter and remove this action?
		if (!m_rgiSuitNoRepeat[i])
			iempty = i;
	}


	return TRUE;

}//END OF SetSuitUpdateCheckNoRepeat(...)










void CBasePlayer::SetSuitUpdateNumber(int number, float fNoRepeatTime, int noRepeatID, BOOL arg_getBatteryValueRealTime)
{
	return SetSuitUpdateNumber(number, fNoRepeatTime, noRepeatID, arg_getBatteryValueRealTime, FALSE);
}

//MODDD - new var, "playDuration".  Defaults to "SUITUPDATETIME", 3.5 seconds, 
//NOTE: for "noRepeatTime" to be useful, this would need an ID, such as, printing the number
//for what?  health, battery, etc.?
//void CBasePlayer::SetSuitUpdateNumber(int number, float fNoRepeatTime, int noRepeatID, BOOL arg_getBatteryValueRealTime)
void CBasePlayer::SetSuitUpdateNumber(int number, float fNoRepeatTime, int noRepeatID, BOOL arg_getBatteryValueRealTime, BOOL fvoxException)
{
	SetSuitUpdatePRE(fvoxException);

	
	//That is, get the battery's current value the moment the number reading speech is loaded (if TRUE).
	getBatteryValueRealTime = arg_getBatteryValueRealTime;
	

	//This just sets this sound up for playing, and doesn't add it to the conventional no-repeat list.
	SetSuitUpdatePOST(0, 10000 + number, 0, 0, TRUE);


}
///m_rgSuitPlayListDuration







//MODDD - new method.
//Like "SetSuitUpdate", but only sets a sound to be blocked from repeating WITHOUT queueing it or playing it at all.
//This way, if some event is needed that happens in quick succession but uses different clips (curing bleeding via
//the wall-health charge kit has a delay, so both clips will play one after the other), this should be called to
//make the others wait for a delay too.
//The point is to stop sounds with the same purpose ("bleeding has stopped", "feeling better", etc.) from playing soon
//after one plays simply because they are not the exact same sound from 10 seconds ago. Those with the same purpose
//should also be blocked to not be spammy in case of a lot of potential messages.


void CBasePlayer::forceRepeatBlock(char *name, int fgroup, float fNoRepeatTime){
	forceRepeatBlock(name, fgroup, fNoRepeatTime, FALSE);
}
void CBasePlayer::forceRepeatBlock(char *name, int fgroup, float fNoRepeatTime, BOOL fvoxException){
	
	int i;
	int iempty = -1;
	int isentence;
	
	BOOL passPRE = SetSuitUpdatePRE(name, fgroup, isentence, fvoxException);
	//Must pass the PRE check to move on with this request.
	if(!passPRE) return;

	//MODDD - compare with old copy of forceRepeatBlock.  Is this simplification okay?

	SetSuitUpdateNoRepeatSweep(iempty, isentence);
	
	// sentence is not in norepeat list, save if norepeat time was given

	BOOL passPOST = SetSuitUpdatePOST(iempty, isentence, fNoRepeatTime, 0, FALSE);

}


BOOL CBasePlayer::suitCanPlay(char *name, int fgroup){
	return suitCanPlay(name, fgroup, FALSE);
}

//can the player play this sound at the moment, or is it repeat-blocked?
BOOL CBasePlayer::suitCanPlay(char *name, int fgroup, BOOL fvoxException){
	int i;
	int isentence;
	int iempty = -1;
	

	
	BOOL passPRE = SetSuitUpdatePRE(name, fgroup, isentence, fvoxException);
	//Must pass the PRE check to move on with this request.
	if(!passPRE) return FALSE;


	BOOL noRepeatPass = SetSuitUpdateCheckNoRepeat(iempty, isentence);
	if(!noRepeatPass){
		return FALSE;
	}
	// sentence is not in norepeat list, save if norepeat time was given

	//No need for the "post" part that plays / updates the norepeat list. This just asked the question, and it was answered.
	return TRUE;
}


// add sentence to suit playlist queue. if fgroup is TRUE, then
// name is a sentence group (HEV_AA), otherwise name is a specific
// sentence name ie: !HEV_AA0.  If fNoRepeatTime is specified in
// seconds, then we won't repeat playback of this word or sentence
// for at least that number of seconds.



void CBasePlayer::SetSuitUpdateFVoxException(char *name, int fgroup, float fNoRepeatTime){
	SetSuitUpdate(name, fgroup, fNoRepeatTime, SUITUPDATETIME, TRUE);
}
void CBasePlayer::SetSuitUpdateFVoxException(char *name, int fgroup, float fNoRepeatTime, float playDuration){
	SetSuitUpdate(name, fgroup, fNoRepeatTime, playDuration, TRUE);
}

void CBasePlayer::SetSuitUpdate(char *name, int fgroup, float fNoRepeatTime){
	SetSuitUpdate(name, fgroup, fNoRepeatTime, SUITUPDATETIME, FALSE);
}
void CBasePlayer::SetSuitUpdate(char *name, int fgroup, float fNoRepeatTime, float playDuration){
	SetSuitUpdate(name, fgroup, fNoRepeatTime, playDuration, FALSE);
}

//MODDD - new var, "playDuration".  Defaults to "SUITUPDATETIME", 3.5 seconds, 
void CBasePlayer::SetSuitUpdate(char *name, int fgroup, float fNoRepeatTime, float playDuration, BOOL fvoxException )
{
	int i;
	int iempty = -1;
	int isentence;
	
	BOOL passPRE = SetSuitUpdatePRE(name, fgroup, isentence, fvoxException);
	if(!passPRE) return;

	BOOL noRepeatPass = SetSuitUpdateCheckNoRepeatApply(iempty, isentence);
	if(!noRepeatPass){
		return;
	}

	BOOL passPOST = SetSuitUpdatePOST(iempty, isentence, fNoRepeatTime, playDuration, TRUE);

}







void CBasePlayer::SetSuitUpdateAndForceBlock(char *name, int fgroup, float fNoRepeatTime){
	SetSuitUpdateAndForceBlock(name, fgroup, fNoRepeatTime, SUITUPDATETIME, FALSE);
}
void CBasePlayer::SetSuitUpdateAndForceBlock(char *name, int fgroup, float fNoRepeatTime, float playDuration){
	SetSuitUpdateAndForceBlock(name, fgroup, fNoRepeatTime, playDuration, FALSE);
}

//MODDD - This is just a clone of "SetSuitUpdate" that has the change from "ForceSuitUpdateReset" or whatever that was called.
void CBasePlayer::SetSuitUpdateAndForceBlock(char *name, int fgroup, float fNoRepeatTime, float playDuration, BOOL fvoxException){
	
	int i;
	int iempty = -1;
	int isentence;
	
	BOOL passPRE = SetSuitUpdatePRE(name, fgroup, isentence, fvoxException);
	//Must pass the PRE check to move on with this request.
	if(!passPRE) return;

	BOOL canPlay = SetSuitUpdateNoRepeatSweep(iempty, isentence);

	// sentence is not in norepeat list, save if norepeat time was given

	BOOL passPOST = SetSuitUpdatePOST(iempty, isentence, fNoRepeatTime, playDuration, canPlay);


}









//SetSuitUpdateFVoxlessFriendlyEvent("!HEV_ANT_USE", FALSE, SUIT_REPEAT_OK, -1, -2, consumeAntidote);

//fvoxCutoff

void CBasePlayer::SetSuitUpdateEvent(char *name, int fgroup, float fNoRepeatTime, float eventDelay, void (CBasePlayer::*eventMethod)() ){
	SetSuitUpdateEvent(name, fgroup, fNoRepeatTime, SUITUPDATETIME, eventDelay, eventMethod);
}
void CBasePlayer::SetSuitUpdateEvent(char *name, int fgroup, float fNoRepeatTime, float playDuration, float eventDelay, void (CBasePlayer::*eventMethod)() ){
	SetSuitUpdateEventFVoxCutoff(name, fgroup, fNoRepeatTime, playDuration, FALSE, eventDelay, eventMethod, -1);

}
void CBasePlayer::SetSuitUpdateEvent(char *name, int fgroup, float fNoRepeatTime, float playDuration, BOOL fvoxException, float eventDelay, void (CBasePlayer::*eventMethod)() ){
	SetSuitUpdateEventFVoxCutoff(name, fgroup, fNoRepeatTime, playDuration, fvoxException, eventDelay, eventMethod, -1);

}

void CBasePlayer::SetSuitUpdateEventFVoxCutoff(char *name, int fgroup, float fNoRepeatTime, float eventDelay, void (CBasePlayer::*eventMethod)(), float fvoxOffCutoff ){
	SetSuitUpdateEventFVoxCutoff(name, fgroup, fNoRepeatTime, SUITUPDATETIME, FALSE, eventDelay, eventMethod, fvoxOffCutoff);
}
void CBasePlayer::SetSuitUpdateEventFVoxCutoff(char *name, int fgroup, float fNoRepeatTime, float playDuration, float eventDelay, void (CBasePlayer::*eventMethod)(), float fvoxOffCutoff ){
	SetSuitUpdateEventFVoxCutoff(name, fgroup, fNoRepeatTime, playDuration, FALSE, eventDelay, eventMethod, fvoxOffCutoff);
}
void CBasePlayer::SetSuitUpdateEventFVoxCutoff(char *name, int fgroup, float fNoRepeatTime, float playDuration, BOOL fvoxException, float eventDelay, void (CBasePlayer::*eventMethod)(), float fvoxOffCutoff ){
	int i;
	int iempty = -1;
	int isentence;
	
	BOOL passPRE = SetSuitUpdatePRE(name, fgroup, isentence, fvoxException||(fvoxOffCutoff!=-1) );
	//Must pass the PRE check to move on with this request.
	if(!passPRE) return;

	BOOL canPlay = SetSuitUpdateNoRepeatSweep(iempty, isentence);

	// sentence is not in norepeat list, save if norepeat time was given

	BOOL passPOST = SetSuitUpdateEventFVoxCutoffPOST(iempty, isentence, fNoRepeatTime, playDuration, canPlay, eventDelay, eventMethod, fvoxOffCutoff);


}//END OF SetSuitUpdateEventFVoxCutoff











/*
================
CheckPowerups

Check for turning off powerups

GLOBALS ASSUMED SET:  g_ulModelIndexPlayer
================
*/
	static void
CheckPowerups(entvars_t *pev)
{
	if (pev->health <= 0)
		return;

	pev->modelindex = g_ulModelIndexPlayer;    // don't use eyes
}


//=========================================================
// UpdatePlayerSound - updates the position of the player's
// reserved sound slot in the sound list.
//=========================================================
void CBasePlayer :: UpdatePlayerSound ( void )
{
	int iBodyVolume;
	int iVolume;
	CSound *pSound;

	pSound = CSoundEnt::SoundPointerForIndex( CSoundEnt :: ClientSoundIndex( edict() ) );

	if ( !pSound )
	{
		ALERT ( at_console, "Client lost reserved sound!\n" );
		return;
	}

	pSound->m_iType = bits_SOUND_NONE;

	// now calculate the best target volume for the sound. If the player's weapon
	// is louder than his body/movement, use the weapon volume, else, use the body volume.
	
	if ( FBitSet ( pev->flags, FL_ONGROUND ) )
	{	
		iBodyVolume = pev->velocity.Length(); 

		// clamp the noise that can be made by the body, in case a push trigger,
		// weapon recoil, or anything shoves the player abnormally fast. 
		if ( iBodyVolume > 512 )
		{
			iBodyVolume = 512;
		}
	}
	else
	{
		iBodyVolume = 0;
	}

	if ( pev->button & IN_JUMP )
	{
		iBodyVolume += 100;
	}

// convert player move speed and actions into sound audible by monsters.
	if ( m_iWeaponVolume > iBodyVolume )
	{
		m_iTargetVolume = m_iWeaponVolume;

		// OR in the bits for COMBAT sound if the weapon is being louder than the player. 
		pSound->m_iType |= bits_SOUND_COMBAT;
	}
	else
	{
		m_iTargetVolume = iBodyVolume;
	}

	// decay weapon volume over time so bits_SOUND_COMBAT stays set for a while
	m_iWeaponVolume -= 250 * gpGlobals->frametime;
	if ( m_iWeaponVolume < 0 )
	{
		iVolume = 0;
	}


	// if target volume is greater than the player sound's current volume, we paste the new volume in 
	// immediately. If target is less than the current volume, current volume is not set immediately to the
	// lower volume, rather works itself towards target volume over time. This gives monsters a much better chance
	// to hear a sound, especially if they don't listen every frame.
	iVolume = pSound->m_iVolume;

	if ( m_iTargetVolume > iVolume )
	{
		iVolume = m_iTargetVolume;
	}
	else if ( iVolume > m_iTargetVolume )
	{
		iVolume -= 250 * gpGlobals->frametime;

		if ( iVolume < m_iTargetVolume )
		{
			iVolume = 0;
		}
	}

	if ( m_fNoPlayerSound )
	{
		// debugging flag, lets players move around and shoot without monsters hearing.
		iVolume = 0;
	}

	if ( gpGlobals->time > m_flStopExtraSoundTime )
	{
		// since the extra sound that a weapon emits only lasts for one client frame, we keep that sound around for a server frame or two 
		// after actual emission to make sure it gets heard.
		m_iExtraSoundTypes = 0;
	}

	if ( pSound )
	{
		pSound->m_vecOrigin = pev->origin;
		pSound->m_iType |= ( bits_SOUND_PLAYER | m_iExtraSoundTypes );
		pSound->m_iVolume = iVolume;
	}

	// keep track of virtual muzzle flash
	m_iWeaponFlash -= 256 * gpGlobals->frametime;
	if (m_iWeaponFlash < 0)
		m_iWeaponFlash = 0;

	//UTIL_MakeVectors ( pev->angles );
	//gpGlobals->v_forward.z = 0;

	// Below are a couple of useful little bits that make it easier to determine just how much noise the 
	// player is making. 
	// UTIL_ParticleEffect ( pev->origin + gpGlobals->v_forward * iVolume, g_vecZero, 255, 25 );
	//ALERT ( at_console, "%d/%d\n", iVolume, m_iTargetVolume );
}



void CBasePlayer::PostThink()
{
	if(global_testVar == -1)return;
	int filterediuser4 = pev->iuser4 & ~(FLAG_JUMPED | FLAG_RESET_RECEIVED);
	

	//CBasePlayer* tempplayerTTT = this;
	//easyPrintLine("VIEW ANGLES?! %.2f %.2f %.2f", tempplayerTTT->pev->v_angle.x, tempplayerTTT->pev->v_angle.y, tempplayerTTT->pev->v_angle.z);

	//easyPrintLine("MY VIEW ANGLES: %.2f, %.2f, %.2f", pev->angles.x, pev->angles.y, pev->angles.z);


	
	if(global_myStrobe == 1){
		if(nextMadEffect <= gpGlobals->time){
			//send effect!
			UTIL_generateFreakyLight(pev->origin);

			nextMadEffect = gpGlobals->time + global_raveEffectSpawnInterval;
		}
	}


	if ( g_fGameOver )
		goto pt_end;         // intermission or finale

	if (!IsAlive())
		goto pt_end;

	// Handle Tank controlling
	if ( m_pTank != NULL )
	{ // if they've moved too far from the gun,  or selected a weapon, unuse the gun
		if ( m_pTank->OnControls( pev ) && !pev->weaponmodel )
		{  
			m_pTank->Use( this, this, USE_SET, 2 );	// try fire the gun
		}
		else
		{  // they've moved off the platform
			m_pTank->Use( this, this, USE_OFF, 0 );
			m_pTank = NULL;
		}
	}

// do weapon stuff
	ItemPostFrame( );

// check to see if player landed hard enough to make a sound
// falling farther than half of the maximum safe distance, but not as far a max safe distance will
// play a bootscrape sound, and no damage will be inflicted. Fallling a distance shorter than half
// of maximum safe distance will make no sound. Falling farther than max safe distance will play a 
// fallpain sound, and damage will be inflicted based on how far the player fell

	if ( (FBitSet(pev->flags, FL_ONGROUND)) && (pev->health > 0) && m_flFallVelocity >= PLAYER_FALL_PUNCH_THRESHHOLD )
	{
		// ALERT ( at_console, "%f\n", m_flFallVelocity );

		float fallSpeedToleranceMulti = 1;
		float fallDamageReduction = 1;
		if(jumpForceMultiMem > 1){
			//a jump force multiple above 1 will increase the tolerance for falls.
			fallSpeedToleranceMulti = sqrt(jumpForceMultiMem);
			fallDamageReduction = jumpForceMultiMem;
		}


		if (pev->watertype == CONTENT_WATER)
		{
			// Did he hit the world or a non-moving entity?
			// BUG - this happens all the time in water, especially when 
			// BUG - water has current force
			// if ( !pev->groundentity || VARS(pev->groundentity)->velocity.z == 0 )
				// EMIT_SOUND(ENT(pev), CHAN_BODY, "player/pl_wade1.wav", 1, ATTN_NORM);
		}
		else if ( m_flFallVelocity > PLAYER_MAX_SAFE_FALL_SPEED * fallSpeedToleranceMulti )
		{// after this point, we start doing damage
			

			float flFallDamage = g_pGameRules->FlPlayerFallDamage( this ) / fallDamageReduction;

			//MODDD - see "float CHalfLifeRules::FlPlayerFallDamage( CBasePlayer *pPlayer )" for why this may still be a bit wonky.
			//Fall damage for super jumps is still odd because the received "Fall Damage" does not factor in "fallDamageReduction".
			//It just subtracts PLAYER_MAX_SAFE_FALL_SPEED from m_flFallVelocity.  Properly, it would subtract
			//(PLAYER_MAX_SAFE_FALL_SPEED * fallSpeedToleranceMulti) from m_flFallVelocity.;

			if ( flFallDamage > pev->health )
			{//splat
				// note: play on item channel because we play footstep landing on body channel
				EMIT_SOUND_FILTERED(ENT(pev), CHAN_ITEM, "common/bodysplat.wav", 1, ATTN_NORM, FALSE);
			}

			if ( flFallDamage > 0 )
			{
				TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), flFallDamage, DMG_FALL ); 

				//MODDD NOTE: - this causes any "x" axis punchangle from pm_shared's "checkfalling" method
				//(applies punch on hitting the ground from a fall) to just become "0" in an instant, making it ineffective!
				//pev->punchangle.x = 0;
			}
		}

		if ( IsAlive() )
		{
			SetAnimation( PLAYER_WALK );
		}
    }

	if (FBitSet(pev->flags, FL_ONGROUND))
	{		
		if (m_flFallVelocity > 64 && !g_pGameRules->IsMultiplayer())
		{
			CSoundEnt::InsertSound ( bits_SOUND_PLAYER, pev->origin, m_flFallVelocity, 0.2 );
			// ALERT( at_console, "fall %f\n", m_flFallVelocity );
		}
		m_flFallVelocity = 0;
	}

	// select the proper animation for the player character	
	if ( IsAlive() )
	{
		if (!pev->velocity.x && !pev->velocity.y)
			SetAnimation( PLAYER_IDLE );
		else if ((pev->velocity.x || pev->velocity.y) && (FBitSet(pev->flags, FL_ONGROUND)))
			SetAnimation( PLAYER_WALK );
		else if (pev->waterlevel > 1)
			SetAnimation( PLAYER_WALK );
	}

	StudioFrameAdvance( );
	CheckPowerups(pev);

	UpdatePlayerSound();

	// Track button info so we can detect 'pressed' and 'released' buttons next frame
	m_afButtonLast = pev->button;



	//this means, play one of the sounds & punch.
	//NOTE: coordinate the right-hand-side value with "ladderCycle" inside of "pm_shared.c".
	
	
	
	
	if(filterediuser4 > 0.3833 * 10000 && !alreadyPassedLadderCheck){
		float flvol = 1;
		int	rndSound;//sound randomizer

		//can't do this again until another frame passes that recognizes "filterediuser4" was below the threshold at some point before passing it again.
		alreadyPassedLadderCheck = TRUE;

		switch((int)cl_ladderMem){
			case 0:
				//don't do anything.
			break;
			case 1:
				flvol = 0.35f;
				if(FBitSet(pev->flags, FL_DUCKING)){
					flvol *= 0.35f;  //again.
				}
				
				rndSound = altLadderStep*2 + RANDOM_LONG(0, 1);

				//play retail's sounds.
				switch(rndSound){

					case 0:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_ladder1.wav", flvol, ATTN_NORM); break;
					case 1:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_ladder3.wav", flvol, ATTN_NORM); break;
					// left foot
					case 2:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_ladder2.wav", flvol, ATTN_NORM); break;
					case 3:	EMIT_SOUND(ENT(pev), CHAN_VOICE, "player/pl_ladder4.wav", flvol, ATTN_NORM); break;
				}

			break;
			case 2:
				//play a random pain sound, don't factor in whether this is the right or left step (as far as I know)
				flvol = 1;  //Var not used yet!
				rndSound = RANDOM_LONG(0, 3); 
	
				switch(rndSound){
				case 0:
					//MODDD - soundsentencesave
					EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain2.wav", 1, ATTN_NORM, TRUE);
				break;
				case 1:
					EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain4.wav", 1, ATTN_NORM, TRUE);
				break;
				case 2:
					EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain5.wav", 1, ATTN_NORM, TRUE);
				break;
				case 3:
					EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/pl_pain6.wav", 1, ATTN_NORM, TRUE);
				break;
				}//END OF switch(rndSound)
			break;
		}

		
		
		//#'s 1 and 2 will give the view punch.
		if(cl_ladderMem == 1 || cl_ladderMem == 2){
			if(altLadderStep){
				pev->punchangle.z = 7;
			}else{
				pev->punchangle.z = -7;
			}
		}
		altLadderStep = !altLadderStep;  //alternates.
		
	}else{
		if(filterediuser4 < 0.3833 * 10000){
			//reset!
			alreadyPassedLadderCheck = FALSE;
		}

	}
	


	//pev->iuser4 = 47;
	//easyPrintLine("Weeeee %d", pev->iuser4);
	


pt_end:
#if defined( CLIENT_WEAPONS )
		// Decay timers on weapons
	// go through all of the weapons and make a list of the ones to pack
	for ( int i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		if ( m_rgpPlayerItems[ i ] )
		{
			CBasePlayerItem *pPlayerItem = m_rgpPlayerItems[ i ];

			while ( pPlayerItem )
			{
				CBasePlayerWeapon *gun;

				gun = (CBasePlayerWeapon *)pPlayerItem->GetWeaponPtr();
				
				if ( gun && gun->UseDecrement() )
				{
					gun->m_flNextPrimaryAttack		= max( gun->m_flNextPrimaryAttack - gpGlobals->frametime, -1.0 );
					gun->m_flNextSecondaryAttack	= max( gun->m_flNextSecondaryAttack - gpGlobals->frametime, -0.001 );

					if ( gun->m_flTimeWeaponIdle != 1000 )
					{
						gun->m_flTimeWeaponIdle		= max( gun->m_flTimeWeaponIdle - gpGlobals->frametime, -0.001 );
					}

					if ( gun->pev->fuser1 != 1000 )
					{
						gun->pev->fuser1	= max( gun->pev->fuser1 - gpGlobals->frametime, -0.001 );
					}

					// Only decrement if not flagged as NO_DECREMENT
//					if ( gun->m_flPumpTime != 1000 )
				//	{
				//		gun->m_flPumpTime	= max( gun->m_flPumpTime - gpGlobals->frametime, -0.001 );
				//	}
					
				}

				pPlayerItem = pPlayerItem->m_pNext;
			}
		}
	}

	m_flNextAttack -= gpGlobals->frametime;
	if ( m_flNextAttack < -0.001 )
		m_flNextAttack = -0.001;
	
	if ( m_flNextAmmoBurn != 1000 )
	{
		m_flNextAmmoBurn -= gpGlobals->frametime;
		
		if ( m_flNextAmmoBurn < -0.001 )
			m_flNextAmmoBurn = -0.001;
	}

	if ( m_flAmmoStartCharge != 1000 )
	{
		m_flAmmoStartCharge -= gpGlobals->frametime;
		
		if ( m_flAmmoStartCharge < -0.001 )
			m_flAmmoStartCharge = -0.001;
	}
	


	

#else
	return;
#endif

	//pev->waterlevel = 3;


}


// checks if the spot is clear of players
BOOL IsSpawnPointValid( CBaseEntity *pPlayer, CBaseEntity *pSpot )
{
	CBaseEntity *ent = NULL;

	if ( !pSpot->IsTriggered( pPlayer ) )
	{
		return FALSE;
	}

	while ( (ent = UTIL_FindEntityInSphere( ent, pSpot->pev->origin, 128 )) != NULL )
	{
		// if ent is a client, don't spawn on 'em
		if ( ent->IsPlayer() && ent != pPlayer )
			return FALSE;
	}

	return TRUE;
}


DLL_GLOBAL CBaseEntity	*g_pLastSpawn;
inline int FNullEnt( CBaseEntity *ent ) { return (ent == NULL) || FNullEnt( ent->edict() ); }

/*
============
EntSelectSpawnPoint

Returns the entity to spawn at

USES AND SETS GLOBAL g_pLastSpawn
============
*/
edict_t *EntSelectSpawnPoint( CBaseEntity *pPlayer )
{
	CBaseEntity *pSpot;
	edict_t		*player;

	player = pPlayer->edict();

// choose a info_player_deathmatch point
	if (g_pGameRules->IsCoOp())
	{
		pSpot = UTIL_FindEntityByClassname( g_pLastSpawn, "info_player_coop");
		if ( !FNullEnt(pSpot) )
			goto ReturnSpot;
		pSpot = UTIL_FindEntityByClassname( g_pLastSpawn, "info_player_start");
		if ( !FNullEnt(pSpot) ) 
			goto ReturnSpot;
	}
	else if ( g_pGameRules->IsDeathmatch() )
	{
		pSpot = g_pLastSpawn;
		// Randomize the start spot
		for ( int i = RANDOM_LONG(1,5); i > 0; i-- )
			pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
		if ( FNullEnt( pSpot ) )  // skip over the null point
			pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );

		CBaseEntity *pFirstSpot = pSpot;

		do 
		{
			if ( pSpot )
			{
				// check if pSpot is valid
				if ( IsSpawnPointValid( pPlayer, pSpot ) )
				{
					if ( pSpot->pev->origin == Vector( 0, 0, 0 ) )
					{
						pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
						continue;
					}

					// if so, go to pSpot
					goto ReturnSpot;
				}
			}
			// increment pSpot
			pSpot = UTIL_FindEntityByClassname( pSpot, "info_player_deathmatch" );
		} while ( pSpot != pFirstSpot ); // loop if we're not back to the start

		// we haven't found a place to spawn yet,  so kill any guy at the first spawn point and spawn there
		if ( !FNullEnt( pSpot ) )
		{
			CBaseEntity *ent = NULL;
			while ( (ent = UTIL_FindEntityInSphere( ent, pSpot->pev->origin, 128 )) != NULL )
			{
				// if ent is a client, kill em (unless they are ourselves)
				if ( ent->IsPlayer() && !(ent->edict() == player) )
					ent->TakeDamage( VARS(INDEXENT(0)), VARS(INDEXENT(0)), 300, DMG_GENERIC );
			}
			goto ReturnSpot;
		}
	}

	// If startspot is set, (re)spawn there.
	if ( FStringNull( gpGlobals->startspot ) || !strlen(STRING(gpGlobals->startspot)))
	{
		pSpot = UTIL_FindEntityByClassname(NULL, "info_player_start");
		if ( !FNullEnt(pSpot) )
			goto ReturnSpot;
	}
	else
	{
		pSpot = UTIL_FindEntityByTargetname( NULL, STRING(gpGlobals->startspot) );
		if ( !FNullEnt(pSpot) )
			goto ReturnSpot;
	}

ReturnSpot:
	if ( FNullEnt( pSpot ) )
	{
		ALERT(at_error, "PutClientInServer: no info_player_start on level");
		return INDEXENT(0);
	}

	g_pLastSpawn = pSpot;
	return pSpot->edict();
}



//MODDD - public setter methods.
void CBasePlayer::setHealth(int newHealth){
	pev->health = newHealth;
}
void CBasePlayer::setArmorBattery(int newBattery){
	pev->armorvalue = newBattery;
}

void CBasePlayer::grantAllItems(){

	/*
	pev->weapons |= WEAPON_ALLWEAPONS;
	*/
	//THIS SHORTCUT IS UNWISE!!!

	pev->weapons |= (1<<WEAPON_SUIT);

	//All weapons granted, all ammo accessible.
	m_fLongJump = TRUE;
	longJumpChargeNeedsUpdate = TRUE;


	//It is up to anything with special deploy sounds to deny playing extra sounds if this is set.
	globalflag_muteDeploySound = TRUE;

		GiveNamedItemIfLacking( "weapon_crowbar" );
		GiveNamedItemIfLacking( "weapon_9mmhandgun" ); //same as "weapon_glock"
		GiveNamedItemIfLacking( "weapon_9mmAR" ); //same as "weapon_mp5"
		GiveNamedItemIfLacking( "weapon_357" ); //same as "weapon_python"
		GiveNamedItemIfLacking( "weapon_crossbow" );
		GiveNamedItemIfLacking( "weapon_gauss" );
		GiveNamedItemIfLacking( "weapon_hornetgun" );
		GiveNamedItemIfLacking( "weapon_tripmine" );
		GiveNamedItemIfLacking( "weapon_rpg" );

		GiveNamedItemIfLacking( "weapon_egon" );

		GiveNamedItemIfLacking( "weapon_satchel" );
		GiveNamedItemIfLacking( "weapon_shotgun" );
		GiveNamedItemIfLacking( "weapon_handgrenade" );
		GiveNamedItemIfLacking( "weapon_snark" );
		GiveNamedItemIfLacking( "weapon_chumtoad" );

	globalflag_muteDeploySound = FALSE;


		//ItemInfoArray[ m_iId ].pszAmmo1 = 4;

		//well that was needless.
		/*
		CBasePlayerItem* test = FindNamedPlayerItem("weapon_9mmhandgun");
		if(test != NULL){
			CBasePlayerWeapon* test2 = (CBasePlayerWeapon *)test->GetWeaponPtr();
			if(test2 != NULL){
				CGlock* test3 = (CGlock*)test2;
				if(test3 != NULL){
					//WE GOT IT!
					this->hasGlockSilencer = 1;
				}
			}
		}
		*/
		this->hasGlockSilencer = 1;


		//Eliminate HEV chatter from all these new weapons (since the new HEV messages 
		//play upon receiving a weapon now)
		SetSuitUpdate(NULL, FALSE, 0);

}


void CBasePlayer::giveMaxAmmo(){
	GiveAmmo( 999, "9mm", _9MM_MAX_CARRY );
	GiveAmmo( 999, "357", _357_MAX_CARRY );
	GiveAmmo( 999, "ARgrenades", M203_GRENADE_MAX_CARRY );
	GiveAmmo( 999, "bolts", BOLT_MAX_CARRY );
	GiveAmmo( 999, "buckshot", BUCKSHOT_MAX_CARRY );
	GiveAmmo( 999, "rockets", ROCKET_MAX_CARRY );
	GiveAmmo( 999, "uranium", URANIUM_MAX_CARRY );
	GiveAmmo( 999, "Hornets", HORNET_MAX_CARRY );

	GiveAmmo( 999, "Hand Grenade", HANDGRENADE_MAX_CARRY );
	GiveAmmo( 999, "Satchel Charge", SATCHEL_MAX_CARRY );
	GiveAmmo( 999, "Snarks", SNARK_MAX_CARRY );
	GiveAmmo( 999, "Trip Mine", TRIPMINE_MAX_CARRY );
	GiveAmmo( 999, "Chum Toads", CHUMTOAD_MAX_CARRY );
	
#if LONGJUMPUSESDELAY == 0
	longJumpCharge = 100;
	longJumpChargeNeedsUpdate = TRUE;
#endif


}


BOOL CBasePlayer::playerHasSuit(){
	return pev->weapons & (1<<WEAPON_SUIT);
}
BOOL CBasePlayer::playerHasLongJump(){
	return m_fLongJump;
}


//This holds a lot of commands common between just defaulting several attributes to say, "Please update me with the real value soon".
//Methods that use this heavily are
//Spawn
//Precache
//Restore
//ForceClientDllUpdate
void CBasePlayer::commonReset(void){
	

	//negative 2 means, don't prompt the user about this change.
	fvoxEnabledMem = -2;

	//or should this always just be forced to "global_barnacleCanGib" to stop a re-do each time?  Might not be necessary so much.
	barnacleCanGibMem = -1;

	iWasFrozenToday = -1;

	hasGlockSilencerMem = -1;
	
	normalSpeedMultiMem = -1;
	noclipSpeedMultiMem = -1;
	jumpForceMultiMem = -1;

	//not for a CVar.
	clearWeaponFlag = -1;


	autoSneakyMem = -2;  //because -1 is actually a valid value for triggering a check in this case.


	infiniteLongJumpChargeMem = -1;

	//alphaCrosshairMem = -1;

	cameraModeMem = -1;
	mirrorsDoNotReflectPlayerMem = -1;

	playerBrightLightMem = -1;

	if(cheat_barnacleEatsEverything == 0){
		if(myRef_barnacleEatsEverything == 0){
			cheat_barnacleEatsEverything = 2;
			myRef_barnacleEatsEverything = 2;
		}else{
			cheat_barnacleEatsEverything = myRef_barnacleEatsEverything;
		}

	}

	drowning = FALSE;  //!!! FOR NOW.
	drowningMem = -1;

	batterySayPhase = -1;
	obligedCustomSentence = 0;  //reset.

	//NOTICE::: for now, obligedCustomSentence will be reset when loading or entering a new place.
	//It doesn't need to be in use for long, and most messages interrupted aren't really that important.
	obligedCustomSentence = 0;

	updateTimedDamageDurations(-1);


	if (PLAYER_ALWAYSHASLONGJUMP){
		m_fLongJump = TRUE;
	}
	

	//MODDD
	deadflagmem = -1;

	
	//MODDD
		//only spawn does this below, actually.  proceed?
	m_fLongJumpMemory = m_fLongJump;
	longJumpDelay = 0;
	longJump_waitForRelease = FALSE;

	//re-acquire pointers.
	the_default_fov = 0;


	//MODDD - added
	m_iClientAntidote = -1;
	m_iClientAdrenaline = -1;
	m_iClientRadiation = -1;

	recoveryIndexMem = -5;
	recoveryIndex = -1;  //nah, this is okay.
	recoveryDelay = -1;  //force this to re-evaluate if needed.
	recoveryDelayMin = -1;

	recentlyGibbed = FALSE;
	

	canApplyDefaultFOVMem = 0;  //assume "no".  Will be contradicted if necessary.
	
	//MODDD
	oldWaterMoveTime = -1;
	oldThinkTime = -1;
	
	//No, don't always reset airTankTime!
	//airTankAirTime = 0;
	airTankAirTimeMem = -1;
	longJumpChargeMem = -1;
	//resetLongJumpCharge();
	airTankAirTimeNeedsUpdate = TRUE;
	
	longJumpChargeNeedsUpdate = TRUE;

	pythonZoomFOV = -1;
	crossbowZoomFOV = -1;
	auto_adjust_zoomfovMem = -1;  //force update.
	the_default_fovmem = -1;
	auto_adjust_fov_aspectmem = -1;



	cheat_infiniteclipMem = -1;
	cheat_infiniteammoMem = -1;
	cheat_minimumfiredelayMem = -1;
	cheat_minimumfiredelaycustomMem = -1;



	cheat_nogaussrecoilMem = -1;
	gaussRecoilSendsUpInSPMem = -1;









	cl_ladderMem = -1;



	//MODDD - also new.  Just a check to see if the user does not have long jump, yet "longJumpCharge" is not negative 1 (don't draw to GUI).
	if(m_fLongJumpMemory != m_fLongJump || (!m_fLongJump && longJumpCharge != -1) ){
		//easyPrint("TEST2 %d\n", 0);
		m_fLongJumpMemory = m_fLongJump;
		resetLongJumpCharge();
		longJumpChargeNeedsUpdate = TRUE;
	}

	//MODDD - this actually enables the long jump.  The "animation" elsewhere isn't the physical long jump.
	/*
	if ( m_fLongJump )
	{
		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "1" );
	}
	else
	{
		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );
	}
	*/
	//Just disable for now, we re-enable in real time when we want to long jump.
	g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );

	//A cheap way to reset fallingvelocity if hitting the ground before reviving with adrenaline.
	g_engfuncs.pfnSetPhysicsKeyValue( edict(), "res", "0" );


	//MODDD - resetting here should be okay.
	m_flgeigerRange = 1000;
	m_igeigerRangePrev = 1000;


	//MODDD - unsure.  But most client things are reset, I think this should be okay.
	m_bitsHUDDamage		= -1;
	m_bitsModHUDDamage		= -1;

	//MODDD
	pev->renderfx |= ISPLAYER;

}

void CBasePlayer::autoSneakyCheck(void){


	float autoSneakyValue = global_autoSneaky;

	if(autoSneakyValue == 1){
		turnOnSneaky();
	}else if(autoSneakyValue == -1){
		turnOffSneaky();
	}
}

void CBasePlayer::turnOnSneaky(void){
		
	//g_engfuncs.pfnClientCmd("impulse 105");
	pev->flags |= FL_NOTARGET;
	m_fNoPlayerSound = TRUE;
	
}

void CBasePlayer::turnOffSneaky(void){
	
	pev->flags &= (~FL_NOTARGET);
	m_fNoPlayerSound = FALSE;
	
}



//MODDD - new.  Without args, assume we're not reviving from adrenaline.
void CBasePlayer::Spawn( void ){

	Spawn(FALSE);

}


void CBasePlayer::Spawn( BOOL revived )
{
	this->recognizablyDead = FALSE;
	this->iAmDead = FALSE;  //just in case these happen.



	/*
	//pev->friction		= 1.0;  //multiplayer requires this to be set to move. Why? dunno
	
	pev->takedamage		= DAMAGE_AIM;
	//pev->solid			= (int)global_testVar;
	//pev->movetype		= MOVETYPE_STEP;  //FORCED!!!
	pev->solid			= SOLID_SLIDEBOX;
	pev->movetype		= MOVETYPE_BOUNCE;
	pev->health = 100;
	//pev->classname		= MAKE_STRING("player");
	g_pGameRules->GetPlayerSpawnSpot( this );
	pev->classname		= MAKE_STRING("zzzwhat");
	SET_MODEL(ENT(pev), "models/player.mdl");
	g_ulModelIndexPlayer = pev->modelindex;
	return;

	*/



	//just in case.
	pev->rendermode = kRenderNormal;
	pev->renderamt = 0;  //yes, kRenderNormal needs this to be 0 to look normal (even opaque)

	
	m_bHolstering = FALSE;
	m_pQueuedActiveItem = NULL;
	m_fCustomHolsterWaitTime = -1;


	//pev->rendermode = kRenderTransTexture;



	friendlyCheckTime = 0; //can check.

	//flag set at death for some reason.  Undo in spawn.
	pev->effects &= ~EF_NOINTERP;

	//haven't died yet...
	alreadyDroppedItemsAtDeath = FALSE;
	sentCarcassScent = FALSE;

	updateTimedDamageDurations(-1);

	//the air-tank air sound may play at contact with water.
	airTankWaitingStart = TRUE;

	//easyPrintLine("IS SPAWN CALLED?!");

	//PRECACHE_MODEL("models/player/gman/Gman.mdl");

	pev->classname		= MAKE_STRING("player");
	if(!revived){

		if(!alreadySpawned){
			//MODDD - line moved here.
			//~...actually, save-restore already defaults this to false.  No need to here.
			//m_fNoPlayerSound = FALSE;// normal sound behavior.

		}
		

		drowning = FALSE;

		//FVOX messages play when this is true.
		fvoxOn = TRUE;
		
		foundRadiation = FALSE;

		//MODDD - start without longjump item until otherwise told (can still use the "long jump" ability
		//as long as "m_fLongJump" is on, and I believe it is just always on now).
		hasLongJumpItem = FALSE;

		pev->health			= 100;
		//always set battery to 0 at a true respawn.
		//It will be reset if needed before now.
		pev->armorvalue		= 0;

		//MODDD - moved here.  Always occurs at a true spawn.
		m_bitsDamageType = 0;
		//MODDD
		m_bitsDamageTypeMod = 0;
		
		
	}else{



		//m_bitsDamageType = 0;
		//m_bitsDamageTypeMod = 0;

		if(global_batteryDrainsAtAdrenalineMode == 3){
			pev->armorvalue		= 0;
		}

		//MODDD - the player will not recover previous drowning-induced drown damage after an
		//        underwater "incapacitation".

		//should breathe-time be explicitly reset?  it may be already.
		//pev->air_finished = gpGlobals->time + AIRTIME;

		m_idrowndmg = 0;
		pev->health = 30;

		//damages reset here if the CVar is right:
		if(global_timedDamageReviveRemoveMode == 3){
			attemptResetTimedDamage(TRUE);
		}

	}

	

	pev->takedamage		= DAMAGE_AIM;

	//hm, experiment?
	pev->solid			= SOLID_SLIDEBOX;


	pev->movetype		= MOVETYPE_BOUNCE;


	//MODDD - unreliable with health mods above.
	//pev->max_health		= pev->health;
	pev->max_health = 100;


	pev->flags		   &= FL_PROXY;	// keep proxy flag sey by engine
	pev->flags		   |= FL_CLIENT;
	pev->air_finished	= gpGlobals->time + 12;
	pev->dmg			= 2;				// initial water damage
	pev->effects		= 0;
	pev->deadflag		= DEAD_NO;
	pev->dmg_take		= 0;
	pev->dmg_save		= 0;
	pev->friction		= 1.0;
	pev->gravity		= 1.0;
	m_bitsHUDDamage		= -1;
	//MODDD
	m_bitsModHUDDamage		= -1;


	m_afPhysicsFlags	= 0;
	m_fLongJump			= FALSE;// no longjump module. 


	if(!revived){
		//These are some vars that are best reset only at spawn.  On revive, just leave them the way they are.

		recentlyGibbed = FALSE;
		recentlyGibbedMem = FALSE;

		//glockSilencerOnVar = 0;
		//egonAltFireOnVar = 0;
		//At start, assume the silencer and egon-altfire is off.

		lastDuckVelocityLength = 0;

		airTankAirTime = 0;
		//start with no air tank.

		longJumpCharge = 0;
		//start with no charge.

		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );
		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "hl", "1" );

		//new phyiscs var: player ladder movement.  This is related to a CVar that may be changed.
		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "plm", "0" );

	}//END OF if(!revived)




	//may as well turn these off, either way.  It would have been done by now if it was going to be done.
	scheduleRemoveAllItemsIncludeSuit = FALSE;
	scheduleRemoveAllItems = FALSE;

	recoveryIndex = -1;
	recoveryDelay = -1;
	recoveryDelayMin = -1;




	

	pev->fov = m_iFOV				= 0;// init field of view.
	m_iClientFOV		= -1; // make sure fov reset is sent

	m_flNextDecalTime	= 0;// let this player decal as soon as he spawns.

	m_flgeigerDelay = gpGlobals->time + 2.0;	// wait a few seconds until user-defined message registrations
												// are recieved by all clients
	
	m_flTimeStepSound	= 0;
	m_iStepLeft = 0;
	m_flFieldOfView		= 0.5;// some monsters use this to determine whether or not the player is looking at them.

	m_bloodColor	= BLOOD_COLOR_RED;
	m_flNextAttack	= UTIL_WeaponTimeBase();
	StartSneaking();

	m_iFlashBattery = 99;
	m_flFlashLightTime = 1; // force first message

// dont let uninitialized value here hurt the player
	m_flFallVelocity = 0;

	g_pGameRules->SetDefaultPlayerTeam( this );


	
	if(!revived){
		//get the spawn spot.
		g_pGameRules->GetPlayerSpawnSpot( this );
	}else{
		//not spawning, reviving.  re-appear close to the same spot, probably adjust the view ang. to look forward.
		pev->origin = pev->origin + Vector(0,0,1);
		pev->v_angle  = g_vecZero;
		pev->velocity = g_vecZero;

		//pev->angles = VARS(pentSpawnSpot)->angles;
		pev->angles = Vector( 0,pev->angles.y,pev->angles.z);
		//that is, force the "x" rotation to 0, use the others as they were.
		//This makes the player face straight forward, in the sense of not up or down.

		pev->punchangle = g_vecZero;
		pev->fixangle = TRUE;
	}
	//otherwise, leave it as it is.

	//

	SET_MODEL(ENT(pev), "models/player.mdl");

	//SET_MODEL(ENT(pev), "models/player/gman/Gman.mdl");
	
    g_ulModelIndexPlayer = pev->modelindex;
	pev->sequence		= LookupActivity( ACT_IDLE );

	if ( FBitSet(pev->flags, FL_DUCKING) ) 
		UTIL_SetSize(pev, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX);
	else
		UTIL_SetSize(pev, VEC_HULL_MIN, VEC_HULL_MAX);

    pev->view_ofs = VEC_VIEW;
	Precache();
	m_HackedGunPos		= Vector( 0, 32, 0 );

	if ( m_iPlayerSound == SOUNDLIST_EMPTY )
	{
		ALERT ( at_console, "Couldn't alloc player sound slot!\n" );
	}


	if(!revived){
		m_pLastItem = NULL;
		m_fWeapon = FALSE;
	}

	m_fInitHUD = TRUE;
	m_iClientHideHUD = -1;  // force this to be recalculated
	m_pClientActiveItem = NULL;
	m_iClientBattery = -1;

	
	if(this->m_pActiveItem != NULL){
		/*
		CBasePlayerWeapon* testWeap = (CBasePlayerWeapon *)m_pActiveItem->GetWeaponPtr();
		if(testWeap != 0){
			
		}
		*/
	}
	
	//deploy the active weapon, since the HUD was cleared.
	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}



	//MODDD - only do this on a true spawn, not a revive.
	if(!revived){
		// reset all ammo values to 0
		for ( int i = 0; i < MAX_AMMO_SLOTS; i++ )
		{
			m_rgAmmo[i] = 0;
			m_rgAmmoLast[i] = 0;  // client ammo values also have to be reset  (the death hud clear messages does on the client side)
		}
	}else{
		
		for ( int i = 0; i < MAX_AMMO_SLOTS; i++ )
		{
			m_rgAmmoLast[i] = 0;  // client ammo values also have to be reset  (the death hud clear messages does on the client side)
		}
	}//END OF else OF if(!revived)


	m_lastx = m_lasty = 0;
	
	m_flNextChatTime = gpGlobals->time;

	commonReset();

	

	//TabulateAmmo();
	//SendAmmoUpdate();

	g_pGameRules->PlayerSpawn( this );

	//Force a client re-update at revive!

	//MODDD - here too.
	pev->renderfx |= ISPLAYER;


	//nah, just always do this if turned on.
	//if(!alreadySpawned){
		//autoSneakyCheck();
	//}
	alreadySpawned = TRUE;

		
}//END OF Spawn




//MODDD - added from inheritance heirarchy
void CBasePlayer::Activate(void){
	
	//MODDD - this solves the egon "click" on the first use since load issue... I think.
	TabulateAmmo();

	CBaseMonster::Activate();
}



void CBasePlayer :: Precache( void )
{
	// in the event that the player JUST spawned, and the level node graph
	// was loaded, fix all of the node graph pointers before the game starts.
	
	// !!!BUGBUG - now that we have multiplayer, this needs to be moved!
	if ( WorldGraph.m_fGraphPresent && !WorldGraph.m_fGraphPointersSet )
	{
		if ( !WorldGraph.FSetGraphPointers() )
		{
			ALERT ( at_console, "**Graph pointers were not set!\n");
		}
		else
		{
			ALERT ( at_console, "**Graph Pointers Set!\n" );
		} 
	}

	// SOUNDS / MODELS ARE PRECACHED in ClientPrecache() (game specific)
	// because they need to precache before any clients have connected

	// init geiger counter vars during spawn and each time
	// we cross a level transition


	//m_flgeigerRange = 1000;
	//m_igeigerRangePrev = 1000;   //Moved to common reset.  Seems safe.

	//MODDD - NOTE::: only done on spawn now.  Damages should be remembered between maps.
	//m_bitsDamageType = 0;  //NOTE::: should this be done in common reset too?
	//m_bitsDamageTypeMod = 0;

	//m_bitsHUDDamage = -1;  done in commonreset.

	m_iClientBattery = -1;

	
	superDuperDelay = -2;
	commonReset();


	m_iTrain = TRAIN_NEW;


	easyPrintLine("LINKING USER MESSAGES...");

	// Make sure any necessary user messages have been registered
	LinkUserMessages();

	m_iUpdateTime = 5;  // won't update for 1/2 a second

	if ( gInitHUD )
		m_fInitHUD = TRUE;


	

}


int CBasePlayer::Save( CSave &save )
{
	if ( !CBaseMonster::Save(save) )
		return 0;

	return save.WriteFields( "PLAYER", this, m_playerSaveData, ARRAYSIZE(m_playerSaveData) );
}


//
// Marks everything as new so the player will resend this to the hud.
//
void CBasePlayer::RenewItems(void)
{

}





int CBasePlayer::Restore( CRestore &restore )
{


	friendlyCheckTime = 0;  //can check again.

	globalPSEUDO_forceFirstPersonIdleDelay = 1;


	
	////easyPrintLine("PLAYER RESTORE METHOD CALLED!");
	//loading the game implies the player has spawned once before.
	alreadySpawned = TRUE;

	if ( !CBaseMonster::Restore(restore) )
		return 0;

	int status = restore.ReadFields( "PLAYER", this, m_playerSaveData, ARRAYSIZE(m_playerSaveData) );

	
	SAVERESTOREDATA *pSaveData = (SAVERESTOREDATA *)gpGlobals->pSaveData;
	// landmark isn't present.
	if ( !pSaveData->fUseLandmark )
	{
		ALERT( at_console, "No Landmark:%s\n", pSaveData->szLandmarkName );

		// default to normal spawn
		edict_t* pentSpawnSpot = EntSelectSpawnPoint( this );
		pev->origin = VARS(pentSpawnSpot)->origin + Vector(0,0,1);
		pev->angles = VARS(pentSpawnSpot)->angles;
	}
	pev->v_angle.z = 0;	// Clear out roll
	pev->angles = pev->v_angle;

	pev->fixangle = TRUE;           // turn this way immediately

// Copied from spawn() for now
	m_bloodColor	= BLOOD_COLOR_RED;

    g_ulModelIndexPlayer = pev->modelindex;

	if ( FBitSet(pev->flags, FL_DUCKING) ) 
	{
		// Use the crouch HACK
		//FixPlayerCrouchStuck( edict() );
		// Don't need to do this with new player prediction code.
		UTIL_SetSize(pev, VEC_DUCK_HULL_MIN, VEC_DUCK_HULL_MAX);
	}
	else
	{
		UTIL_SetSize(pev, VEC_HULL_MIN, VEC_HULL_MAX);
	}

	g_engfuncs.pfnSetPhysicsKeyValue( edict(), "hl", "1" );

	
	//autoSneakyCheck();
	superDuperDelay = -2;
	commonReset();

	RenewItems();

#if defined( CLIENT_WEAPONS )
	// HACK:	This variable is saved/restored in CBaseMonster as a time variable, but we're using it
	//			as just a counter.  Ideally, this needs its own variable that's saved as a plain float.
	//			Barring that, we clear it out here instead of using the incorrect restored time value.
	m_flNextAttack = UTIL_WeaponTimeBase();
#endif




	/*
	//MODDD - if we have a weapon out, make it work (animate / not) the same way as one recently deployed would.
	if(m_pActiveItem != NULL){
		forceNoWeaponLoop = TRUE;
		CBasePlayerWeapon* wpnTest = (CBasePlayerWeapon *) m_pActiveItem->GetWeaponPtr();
		//gun = (CBasePlayerWeapon *)pPlayerItem->GetWeaponPtr();
		if(wpnTest != NULL){
			wpnTest->m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + wpnTest->randomIdleAnimationDelay();
		}
	}
	*/

	//assume we have a weapon out.
	// + randomIdleAnimationDelay()



	return status;
}



void CBasePlayer::SelectNextItem( int iItem )
{
	CBasePlayerItem *pItem;

	pItem = m_rgpPlayerItems[ iItem ];
	
	if (!pItem)
		return;

	if (pItem == m_pActiveItem)
	{
		// select the next one in the chain
		pItem = m_pActiveItem->m_pNext; 
		if (! pItem)
		{
			return;
		}

		CBasePlayerItem *pLast;
		pLast = pItem;
		while (pLast->m_pNext)
			pLast = pLast->m_pNext;

		// relink chain
		pLast->m_pNext = m_pActiveItem;
		m_pActiveItem->m_pNext = NULL;
		m_rgpPlayerItems[ iItem ] = pItem;
	}

	ResetAutoaim( );

	// FIX, this needs to queue them up and delay
	if (m_pActiveItem)
	{
		m_pActiveItem->Holster( );
	}
	
	m_pActiveItem = pItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}

	//MODDD - should this go to "commonReset" too?
	pev->renderfx |= ISPLAYER;

}


//MODDD - IMPORTANT. Keep any changes with this in check with hl_weapons.cpp's version for clientside.
void CBasePlayer::SelectItem(const char *pstr)
{
	if (!pstr)
		return;

	CBasePlayerItem *pItem = NULL;

	for (int i = 0; i < MAX_ITEM_TYPES; i++)
	{
		if (m_rgpPlayerItems[i])
		{
			pItem = m_rgpPlayerItems[i];
	
			while (pItem)
			{
				if (FClassnameIs(pItem->pev, pstr))
					break;
				pItem = pItem->m_pNext;
			}
		}

		if (pItem)
			break;
	}

	if (!pItem)
		return;

	
	if (pItem == m_pActiveItem 
		//MODDD - new extra condition. If it is the case that we're switching out weapons (holster anim playing;
		//        m_pQueuedActiveItem not null), and we happen to pick the original item, it is ok to do that switch.
		&& !( m_pQueuedActiveItem != NULL && pItem != m_pQueuedActiveItem)
		)
		return;
	
	ResetAutoaim( );



	
	// FIX, this needs to queue them up and delay
	/*
	if (m_pActiveItem)
		m_pActiveItem->Holster( );
	
	m_pLastItem = m_pActiveItem;
	m_pActiveItem = pItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}
	*/

	//MODDD - above comment is from the as-is project. I shall obey thy command, developers from long ago.



	if (m_pActiveItem ){
		//easyForcePrintLine("OH yeah AM I HOLSTERIN ALREADY %d", m_bHolstering);

		if(!m_bHolstering){
			//don't holster the currently equipped weapon if already in the middle of holstering.
			m_pActiveItem->Holster( );
			m_bHolstering = TRUE;
		}


		if(EASY_CVAR_GET(holsterAnims) == 1){
			//using holster anim? Tell the currently equipped item to change to this weapon when that is over.
			m_pQueuedActiveItem = pItem;  //set this later instead, after the holster anim is done.
		}else{
			//Not using holster anims? Immediately change weapon.
			setActiveItem(pItem);
			m_bHolstering = FALSE;
		}

	}else{
		//just pick it now.
		setActiveItem(pItem);

	}
	

	
}//END OF SelectItem


void CBasePlayer::setActiveItem(CBasePlayerItem* argItem){
	
	m_pLastItem = m_pActiveItem;
	m_pActiveItem = argItem;

	if (m_pActiveItem)
	{
		m_pActiveItem->Deploy( );
		m_pActiveItem->UpdateItemInfo( );
	}
}



void CBasePlayer::SelectLastItem(void)
{
	if (!m_pLastItem)
	{
		return;
	}

	if ( m_pActiveItem && !m_pActiveItem->CanHolster() )
	{
		return;
	}

	ResetAutoaim( );

	// FIX, this needs to queue them up and delay
	if (m_pActiveItem)
		m_pActiveItem->Holster( );
	
	CBasePlayerItem *pTemp = m_pActiveItem;
	m_pActiveItem = m_pLastItem;
	m_pLastItem = pTemp;
	m_pActiveItem->Deploy( );
	m_pActiveItem->UpdateItemInfo( );
}

//==============================================
// HasWeapons - do I have any weapons at all?
//==============================================
BOOL CBasePlayer::HasWeapons( void )
{
	int i;

	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		if ( m_rgpPlayerItems[ i ] )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CBasePlayer::SelectPrevItem( int iItem )
{
}


const char *CBasePlayer::TeamID( void )
{
	if ( pev == NULL )		// Not fully connected yet
		return "";

	// return their team name
	return m_szTeamName;
}


//==============================================
// !!!UNDONE:ultra temporary SprayCan entity to apply
// decal frame at a time. For PreAlpha CD
//==============================================
class CSprayCan : public CBaseEntity
{
public:
	void	Spawn ( entvars_t *pevOwner );
	void	Think( void );

	virtual int	ObjectCaps( void ) { return FCAP_DONT_SAVE; }
};

void CSprayCan::Spawn ( entvars_t *pevOwner )
{
	pev->origin = pevOwner->origin + Vector ( 0 , 0 , 32 );
	pev->angles = pevOwner->v_angle;
	pev->owner = ENT(pevOwner);
	pev->frame = 0;

	pev->nextthink = gpGlobals->time + 0.1;
	//MODDD - soundsentencesave. This one's ok to play through it.
	EMIT_SOUND_FILTERED(ENT(pev), CHAN_VOICE, "player/sprayer.wav", 1, ATTN_NORM, TRUE);
}

void CSprayCan::Think( void )
{
	TraceResult	tr;	
	int playernum;
	int nFrames;
	CBasePlayer *pPlayer;


	pPlayer = (CBasePlayer *)GET_PRIVATE(pev->owner);

	if (pPlayer)
		nFrames = pPlayer->GetCustomDecalFrames();
	else
		nFrames = -1;

	playernum = ENTINDEX(pev->owner);
	
	// ALERT(at_console, "Spray by player %i, %i of %i\n", playernum, (int)(pev->frame + 1), nFrames);


	UTIL_MakeVectors(pev->angles);
	UTIL_TraceLine ( pev->origin, pev->origin + gpGlobals->v_forward * 128, ignore_monsters, pev->owner, & tr);

	

	switch( (int)EASY_CVAR_GET(customLogoSprayMode) ){
		case 1:{
			nFrames = 6;
			//easyForcePrintLine("MY NUMBERS %d", playernum);
			UTIL_DecalTrace( &tr, DECAL_LAMBDA1 + pev->frame );
			//UTIL_PlayerDecalTrace( &tr, playernum, DECAL_LAMBDA1 + pev->frame, FALSE );
		
			if ( pev->frame++ >= (nFrames - 1))
				UTIL_Remove( this );
			break;
		}

		//case 2: ... etc.

		default:{  //0 or other unrecognized values
			// No customization present.
			if (nFrames == -1)
			{
				UTIL_DecalTrace( &tr, DECAL_LAMBDA6 );
				UTIL_Remove( this );
			}
			else
			{
				UTIL_PlayerDecalTrace( &tr, playernum, pev->frame, TRUE );
				// Just painted last custom frame.
				if ( pev->frame++ >= (nFrames - 1))
					UTIL_Remove( this );
			}
			break;
		}
	}//END OF else OF customLogoSprayMode



	pev->nextthink = gpGlobals->time + 0.1;
}

class	CBloodSplat : public CBaseEntity
{
public:
	void	Spawn ( entvars_t *pevOwner );
	void	Spray ( void );
};

void CBloodSplat::Spawn ( entvars_t *pevOwner )
{
	pev->origin = pevOwner->origin + Vector ( 0 , 0 , 32 );
	pev->angles = pevOwner->v_angle;
	pev->owner = ENT(pevOwner);

	SetThink ( &CBloodSplat::Spray );
	pev->nextthink = gpGlobals->time + 0.1;
}

void CBloodSplat::Spray ( void )
{
	TraceResult	tr;	
	
	//MODDD - can't spray blood in german censorship mode.  If the player is a robot, spray oil maybe?
	//MODDD TODO: oil?
	//if ( g_Language != LANGUAGE_GERMAN )
	if(global_germanCensorship != 1)
	{
		UTIL_MakeVectors(pev->angles);
		UTIL_TraceLine ( pev->origin, pev->origin + gpGlobals->v_forward * 128, ignore_monsters, pev->owner, & tr);

		UTIL_BloodDecalTrace( &tr, BLOOD_COLOR_RED );
	}
	SetThink ( &CBaseEntity::SUB_Remove );
	pev->nextthink = gpGlobals->time + 0.1;
}

//==============================================




void CBasePlayer::GiveNamedItemIfLacking( const char *pszName ){

	if(!HasNamedPlayerItem(pszName)){
		GiveNamedItem(pszName);
	}

}








edict_t* overyLongComplicatedProcessForCreatingAnEntity(const char* entityName){
	
	/*
	//originally:

	int iszItem = ALLOC_STRING( entityName );	// Make a copy of the classname
	const char* pszNameFinal = STRING(iszItem);
		
	int istr = MAKE_STRING(pszNameFinal);
	edict_t* spent = CREATE_NAMED_ENTITY(istr);
	
	return spent;
	
	*/
	
	return CREATE_NAMED_ENTITY( MAKE_STRING( STRING( ALLOC_STRING( entityName ) ) )   );
	
}











	
edict_t* CBasePlayer::GiveNamedItem( const char *pszName ){
	return GiveNamedItem(pszName, NULL);
}

//send right to the player's origin like the retail "give" command does (which likely referred to this method now)
edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags  )
{


	char resultpre[128];
	strncpy( &resultpre[0], &pszName[0], 127 );
	resultpre[127] = '\0';
	lowercase(&resultpre[0]);

	//Case exceptions.  Some names DO use caps, unfortunately, so this does the correction to the guaranteed all-lowercased text to get the right entity text.
	
	//If we didn't precache everything, the air tank isn't either.
	if(CVAR_GET_FLOAT("precacheAll") == 0){
		
		if(FStrEq(resultpre, "item_airtank")){
			//just do the effect.  This will stop a possible precache error, since "give" implies we didn't need to see the model.


			if(airTankAirTime < PLAYER_AIRTANK_TIME_MAX){
			//pPlayer->SetSuitUpdate("!HEV_DET4", FALSE, SUIT_NEXT_IN_1MIN);
			airTankAirTime = PLAYER_AIRTANK_TIME_MAX;
		
			if(gmsgUpdateAirTankAirTime > 0){
				MESSAGE_BEGIN( MSG_ONE, gmsgUpdateAirTankAirTime, NULL, pev );
				WRITE_SHORT( airTankAirTime);
				MESSAGE_END();
			}
			MESSAGE_BEGIN( MSG_ONE, gmsgItemPickup, NULL, pev );
			WRITE_STRING( STRING(pev->classname) );
			MESSAGE_END();

			//MODDD QUESTION - does the precache sentence save system handle this, or is it guaranteed precached like other player sounds?
			EMIT_SOUND_FILTERED( edict(), CHAN_ITEM, "items/airtank1.wav", 1, ATTN_NORM, TRUE );
			}

			return NULL;
		}

	}//END OF if(CVAR_GET_FLOAT("precacheAll") == 0)


	edict_t* thing = GiveNamedItem(pszName, pszSpawnFlags, pev->origin);
	//Because this is the default, sending to the origin, call "DispatchTouch" too.
	if(thing != NULL){
		DispatchTouch( thing, ENT( pev ) );
	}

	//spawn flag checking is already done in the above "GiveNamedItem".

	return thing;
}






edict_t* CBasePlayer::GiveNamedItem( const char *pszName, float xCoord, float yCoord, float zCoord ){
	return GiveNamedItem(pszName, NULL, Vector(xCoord, yCoord, zCoord), FALSE, NULL);
}

edict_t* CBasePlayer::GiveNamedItem( const char *pszName, float xCoord, float yCoord, float zCoord, BOOL factorSpawnSize ){
	return GiveNamedItem(pszName, NULL, Vector(xCoord, yCoord, zCoord), factorSpawnSize, NULL);
}
edict_t* CBasePlayer::GiveNamedItem( const char *pszName, const Vector& coord ){
	return GiveNamedItem(pszName, NULL, coord, FALSE, NULL);
}
edict_t* CBasePlayer::GiveNamedItem( const char *pszName, const Vector& coord, BOOL factorSpawnSize ){
	return GiveNamedItem(pszName, NULL, coord, factorSpawnSize, NULL);
}


edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, float xCoord, float yCoord, float zCoord ){
	return GiveNamedItem(pszName, pszSpawnFlags, Vector(xCoord, yCoord, zCoord), FALSE, NULL);
}

edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, float xCoord, float yCoord, float zCoord, BOOL factorSpawnSize){
	return GiveNamedItem(pszName, pszSpawnFlags, Vector(xCoord, yCoord, zCoord), factorSpawnSize, NULL);
}
edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, const Vector& coord){
	return GiveNamedItem(pszName, pszSpawnFlags, coord, FALSE, NULL);
}



edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, const Vector& coord, BOOL factorSpawnSize ){
	return GiveNamedItem(pszName, pszSpawnFlags, coord, factorSpawnSize, NULL);
}



edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, float xCoord, float yCoord, float zCoord, BOOL factorSpawnSize, TraceResult* tr ){
	return GiveNamedItem(pszName, pszSpawnFlags, Vector(xCoord, yCoord, zCoord), factorSpawnSize, tr);
}

//int spawnFlagValue = attemptInterpretSpawnFlag(pszSpawnFlags);
edict_t* CBasePlayer::GiveNamedItem( const char *pszName, int pszSpawnFlags, const Vector& coord, BOOL factorSpawnSize, TraceResult* tr )
{
	edict_t	*pent;
	int istr;
	entvars_t *pentpev;
	
	int iszItem;
	char* pszNameFinal;  //will just point at the name used to (attempt to?) genreate the object.


	char resultpre[128];
	char result[128];
	strncpy( &resultpre[0], &pszName[0], 127 );
	resultpre[127] = '\0';
	lowercase(&resultpre[0]);

	//Case exceptions.  Some names DO use caps, unfortunately, so this does the correction to the guaranteed all-lowercased text to get the right entity text.
	if(FStrEq(resultpre, "weapon_9mmar")){
		strcpy(resultpre, "weapon_9mmAR\0");
		//strcpy
	}else if(FStrEq(resultpre, "ammo_argrenades")){
		strcpy(resultpre, "ammo_ARgrenades\0");
	}else if(FStrEq(resultpre, "ammo_argrenades")){
		strcpy(resultpre, "ammo_ARgrenades\0");
	}else if(FStrEq(resultpre, "ammo_9mmar")){
		strcpy(resultpre, "ammo_9mmAR\0");
	}

	

		

	pent = overyLongComplicatedProcessForCreatingAnEntity(&resultpre[0]);
	pszNameFinal = &resultpre[0];
	
	//MODDD - pre-check.
	if ( FNullEnt( pent ) && !stringStartsWith(resultpre, "monster_") )
	{
		//try putting "monster_" in front?
		
		//char command[128];
		
		result[0]='m';
		result[1]='o';
		result[2]='n';
		result[3]='s';
		result[4]='t';
		result[5]='e';
		result[6]='r';
		result[7]='_';

		strncpy( &result[8], &resultpre[0], 127-8 );
		result[127] = '\0';

		pent = overyLongComplicatedProcessForCreatingAnEntity(result);
		pszNameFinal = &result[0];
	}
	
	//int istr = MAKE_STRING( boot );
	//pent = CREATE_NAMED_ENTITY(istr);
	if ( FNullEnt( pent ) )
	{
		ALERT ( at_console, "NULL Ent in GiveNamedItem!\n" );
		return NULL;
	}





	
	CBaseEntity* temptest = Instance(pent);


	//If the "monster_barancle" was spawned, push it down a bit to be safe.  If any of it clips
	//through the ceiling, the tongue won't drop.
	float extraOffset = 0;
	//easyPrintLine("WHAT %s", pszNameFinal);
	if(FStrEq(pszNameFinal, "monster_barnacle") || FStrEq(pszNameFinal, "barnacle")  ){
		extraOffset = -2;
	}




	int forceDynamicIndex = 0;
	//-1 = force non-dynamic (as though by map)
	//0 = inactive.
	//1 = force dynamic.

	int addedSpawnFlags = 0;
	Vector coordMod(0,0,0);


	//TODO: to make name-lookups easier, add a virtual method to the monster class named 
	//"getPreferredClassname".  That would return "monster_stuka" for bats regardless of what was
	//used to spawn it.


	if(FStrEq(pszNameFinal, "monster_stuka") || FStrEq(pszNameFinal, "stuka") || FStrEq(pszNameFinal, "monster_stukabat") || FStrEq(pszNameFinal, "monster_stuka")  ){
		//extraOffset = -2;
		

		if(tr != NULL){

			//STUKA DIMENSIONS:
			//UTIL_SetSize( pev, Vector( -8, -8, 0 ), Vector( 8, 8, 28 ));

			if(tr->vecPlaneNormal[2] < 0.3 && tr->vecPlaneNormal[2] > -0.3){
				//completely flat-ways surface selected for placement.
				coordMod.x += tr->vecPlaneNormal[0] * 5;
				coordMod.y += tr->vecPlaneNormal[1] * 5;
				coordMod.z += tr->vecPlaneNormal[2] * 5;
			}else if(tr->vecPlaneNormal[2] < -0.3){
				//placed on a ceiling, mark as such.
				//addedSpawnFlags |= SF_STUKABAT_CEILING;
				//no, the flag is "SF_MONSTER_STUKA_ONGROUND"...
				//Force this as a non-dynamic spawn and it will snap to  the ceiling without the flag, actually.

				coordMod.z -= 28;

				forceDynamicIndex = -1;
			}

		}//END OF if(tr != NULL)

	}//END OF if(<stukabat spawned>)



	
	//easyPrintLine("DAHHH %s ::: %.2f", STRING(pent->v.classname), extraOffset);

	pentpev = VARS( pent );
	
	if(factorSpawnSize){

		pentpev->origin = Vector(	coord.x + coordMod.x - (pentpev->size.x / 2),
										coord.y + coordMod.y - (pentpev->size.y / 2),
										coord.z + coordMod.z - pentpev->size.z + extraOffset
										);

	}else{
		pentpev->origin = Vector(	coord.x + coordMod.x,
										coord.y + coordMod.y,
										coord.z + coordMod.z + extraOffset
										);
		
	}


	//if( forceDynamicIndex != -1 && (isStringEmpty(pszSpawnFlags) || forceDynamicIndex == 1)  ){
	if( forceDynamicIndex != -1 && (pszSpawnFlags==-1 || forceDynamicIndex == 1)  ){
		//if we were given a null spawn flag, tell the spawn about this.
		if(temptest != NULL){
			temptest->DefaultSpawnNotice();
			
		}else{
			easyPrintLine("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			easyPrintLine("SUPER DUPER ERROR OF UNFABULOUSNESS PLEASE REPORT2");
			easyPrintLine("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		}

	}else{
		//see if there are any spawn flags from the client (in this spawn call)
		//int spawnFlagValue = attemptInterpretSpawnFlag(pszSpawnFlags);
		int spawnFlagValue = pszSpawnFlags;
		if(spawnFlagValue >= 0){
			//pent->v.spawnflags = spawnFlagValue;
			if(temptest != NULL){
				temptest->ForceSpawnFlag(spawnFlagValue);
			
			}else{
				easyPrintLine("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
				easyPrintLine("SUPER DUPER ERROR OF UNFABULOUSNESS PLEASE REPORT");
				easyPrintLine("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			}
		}

	}
	//IMPORTANT! Anything spawned by me is also "non-respawnable", regardless of the suggested flag (if any).
	pent->v.spawnflags |= SF_NORESPAWN;

	
	//virtual BOOL isBasePlayerWeapon(void){return FALSE;};
	


	

	if(temptest->isBasePlayerWeapon()){
		CBasePlayerWeapon* tempWeap = static_cast<CBasePlayerWeapon*>(temptest);
		//If the thing we called for can spawn a pickupWalker, just skip to doing that instead.
		const char* pickupWalkerNameTest = tempWeap->GetPickupWalkerName();
		if(::isStringEmpty(pickupWalkerNameTest)){
			//no walker? nothing unusual.
		}else{
			//there is a walker! Just skip to spawning that instead.
			CBaseEntity* newWalker = tempWeap->pickupWalkerReplaceCheck();
			
			if(newWalker != NULL){
				//send the walker instead.
				::UTIL_Remove(temptest);
				edict_t* myEd = ENT(newWalker->pev);
				DispatchSpawn(myEd);
				return myEd;
			}

			//...or call giveNamedItem, using the walker name instead?  Choices, choices!
		}
	}
	




	DispatchSpawn( pent );
	
	return pent;
}




//MODDD - "findEntityForward" has been moved to util.cpp.

BOOL CBasePlayer :: FlashlightIsOn( void )
{
	return FBitSet(pev->effects, EF_DIMLIGHT);
}


void CBasePlayer :: FlashlightTurnOn( void )
{
	if ( !g_pGameRules->FAllowFlashlight() )
	{
		return;
	}

	if ( (pev->weapons & (1<<WEAPON_SUIT)) )
	{
		//MODDD - channel changed from "CHAN_WEAPON" to "CHAN_STREAM".
		//MODDD - soundsentencesave
		EMIT_SOUND_FILTERED( ENT(pev), CHAN_STREAM, SOUND_FLASHLIGHT_ON, 1.0, ATTN_NORM, 0, PITCH_NORM, TRUE );
		SetBits(pev->effects, EF_DIMLIGHT);
		MESSAGE_BEGIN( MSG_ONE, gmsgFlashlight, NULL, pev );
		WRITE_BYTE(1);
		WRITE_BYTE(m_iFlashBattery);
		MESSAGE_END();

		m_flFlashLightTime = FLASH_DRAIN_TIME + gpGlobals->time;

	}
}


void CBasePlayer :: FlashlightTurnOff( void )
{
	//MODDD - channel changed from "CHAN_WEAPON" to "CHAN_STREAM".
	//MODDD - soundsentencesave
	EMIT_SOUND_FILTERED( ENT(pev), CHAN_STREAM, SOUND_FLASHLIGHT_OFF, 1.0, ATTN_NORM, 0, PITCH_NORM, TRUE );
    ClearBits(pev->effects, EF_DIMLIGHT);
	MESSAGE_BEGIN( MSG_ONE, gmsgFlashlight, NULL, pev );
	WRITE_BYTE(0);
	WRITE_BYTE(m_iFlashBattery);
	MESSAGE_END();

	m_flFlashLightTime = FLASH_CHARGE_TIME + gpGlobals->time;

}

/*
===============
ForceClientDllUpdate

When recording a demo, we need to have the server tell us the entire client state
so that the client side .dll can behave correctly.
Reset stuff so that the state is transmitted.
===============
*/
void CBasePlayer :: ForceClientDllUpdate( void )
{

	
	m_iClientHealth  = -1;
	m_iClientBattery = -1;

	
	m_iTrain |= TRAIN_NEW;  // Force new train message.
	m_fWeapon = FALSE;          // Force weapon send
	m_fKnownItem = FALSE;    // Force weaponinit messages.
	m_fInitHUD = TRUE;		// Force HUD gmsgResetHUD message
	

	commonReset();

	// Now force all the necessary messages
	//  to be sent.
	UpdateClientData();
}

/*
============
ImpulseCommands
============
*/
extern float g_flWeaponCheat;

void CBasePlayer::ImpulseCommands( )
{
	TraceResult	tr;// UNDONE: kill me! This is temporary for PreAlpha CDs

	// Handle use events
	PlayerUse();
		
	int iImpulse = (int)pev->impulse;
	switch (iImpulse)
	{
	case 99:
		{

		int iOn;

		if (!gmsgLogo)
		{
			iOn = 1;
			gmsgLogo = REG_USER_MSG("Logo", 1);
		} 
		else 
		{
			iOn = 0;
		}
		
		ASSERT( gmsgLogo > 0 );
		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgLogo, NULL, pev );
			WRITE_BYTE(iOn);
		MESSAGE_END();

		if(!iOn)
			gmsgLogo = 0;
		break;
		}
	case 100:
        // temporary flashlight for level designers
        if ( FlashlightIsOn() )
		{
			FlashlightTurnOff();
		}
        else 
		{
			FlashlightTurnOn();
		}
		break;

	case	201:// paint decal
		

		
		if ( gpGlobals->time < m_flNextDecalTime )
		{
			// too early!
			break;
		}
		

		UTIL_MakeVectors(pev->v_angle);
		UTIL_TraceLine ( pev->origin + pev->view_ofs, pev->origin + pev->view_ofs + gpGlobals->v_forward * 128, ignore_monsters, ENT(pev), & tr);

		if ( tr.flFraction != 1.0 )
		{// line hit something, so paint a decal
			m_flNextDecalTime = gpGlobals->time + decalfrequency.value;
			CSprayCan *pCan = GetClassPtr((CSprayCan *)NULL);
			pCan->Spawn( pev );
		}

		break;

	default:
		// check all of the cheat impulse commands now
		CheatImpulseCommands( iImpulse );
		break;
	}
	
	pev->impulse = 0;
}

//MODDD - keep track of "sv_cheats".  Moved to player.h.
//cvar_t* sv_cheatsRef = 0;

BOOL doneCheatYet = FALSE;
//=========================================================
//=========================================================
void CBasePlayer::CheatImpulseCommands( int iImpulse )
{
	/*
	if(!doneCheatYet){
	//MODDD - PLEASE just give me stuff.
		GiveNamedItem( "item_battery" );
		GiveNamedItem( "weapon_crowbar" );
		GiveNamedItem( "weapon_9mmhandgun" );
		GiveNamedItem( "ammo_9mmclip" );
		GiveNamedItem( "weapon_shotgun" );
		GiveNamedItem( "ammo_buckshot" );
		GiveNamedItem( "weapon_9mmAR" );
		GiveNamedItem( "ammo_9mmAR" );
		GiveNamedItem( "ammo_ARgrenades" );
		GiveNamedItem( "weapon_handgrenade" );
		GiveNamedItem( "weapon_tripmine" );

		GiveNamedItem( "weapon_357" );
		GiveNamedItem( "ammo_357" );
		GiveNamedItem( "weapon_crossbow" );
		GiveNamedItem( "ammo_crossbow" );
		GiveNamedItem( "weapon_egon" );
		GiveNamedItem( "weapon_gauss" );
		GiveNamedItem( "ammo_gaussclip" );
		GiveNamedItem( "weapon_rpg" );
		GiveNamedItem( "ammo_rpgclip" );
		GiveNamedItem( "weapon_satchel" );
		GiveNamedItem( "weapon_snark" );
		GiveNamedItem( "weapon_hornetgun" );
		doneCheatYet = TRUE;
	}

	*/






#if !defined( HLDEMO_BUILD )

	
	if(sv_cheatsRef == 0){
		sv_cheatsRef = CVAR_GET_POINTER( "sv_cheats" );
	}
	
	//MODDD - update "g_flWeaponCheat" to what sv_cheats is.
	if(sv_cheatsRef != 0){
		if(sv_cheatsRef->value == 1){
			g_flWeaponCheat = 1;
		}else{
			g_flWeaponCheat = 0;
		}
	}





	if ( g_flWeaponCheat == 0.0 )
	{
		return;
	}

	CBaseEntity *pEntity;
	TraceResult tr;

	switch ( iImpulse )
	{
	case 76:
		{
			if (!giPrecacheGrunt)
			{
				giPrecacheGrunt = 1;
				ALERT(at_console, "You must now restart to use Grunt-o-matic.\n");
			}
			else
			{
				UTIL_MakeVectors( Vector( 0, pev->v_angle.y, 0 ) );
				Create("monster_human_grunt", pev->origin + gpGlobals->v_forward * 128, pev->angles);
			}
			break;
		}


	case 101:
		gEvilImpulse101 = TRUE;
		GiveNamedItem( "item_suit" );
		GiveNamedItem( "item_battery" );
		GiveNamedItem( "weapon_crowbar" );
		GiveNamedItem( "weapon_9mmhandgun" );
		GiveNamedItem( "ammo_9mmclip" );
		GiveNamedItem( "weapon_shotgun" );
		GiveNamedItem( "ammo_buckshot" );
		GiveNamedItem( "weapon_9mmAR" );
		GiveNamedItem( "ammo_9mmAR" );
		GiveNamedItem( "ammo_ARgrenades" );
		GiveNamedItem( "weapon_handgrenade" );
		GiveNamedItem( "weapon_tripmine" );
#ifndef OEM_BUILD
		GiveNamedItem( "weapon_357" );
		GiveNamedItem( "ammo_357" );
		GiveNamedItem( "weapon_crossbow" );
		GiveNamedItem( "ammo_crossbow" );
		GiveNamedItem( "weapon_egon" );
		GiveNamedItem( "weapon_gauss" );
		GiveNamedItem( "ammo_gaussclip" );
		GiveNamedItem( "weapon_rpg" );
		GiveNamedItem( "ammo_rpgclip" );
		GiveNamedItem( "weapon_satchel" );
		GiveNamedItem( "weapon_snark" );
		GiveNamedItem( "weapon_hornetgun" );
		GiveNamedItem( "weapon_chumtoad" );
		
#endif
		gEvilImpulse101 = FALSE;
		break;

	case 102:
		// Gibbage!!!
		CGib::SpawnRandomGibs( pev, 1, GIB_HUMAN_ID );
		break;

	case 103:
		// What the hell are you doing?
		pEntity = FindEntityForward( this );
		if ( pEntity )
		{
			CBaseMonster *pMonster = pEntity->MyMonsterPointer();
			if ( pMonster )
				pMonster->ReportAIState();
		}
		break;

	case 104:
		// Dump all of the global state varaibles (and global entity names)
		gGlobalState.DumpGlobals();
		break;

	case 105:// player makes no sound for monsters to hear.
		{
			if ( m_fNoPlayerSound )
			{
				ALERT ( at_console, "Player is audible\n" );
				m_fNoPlayerSound = FALSE;
			}
			else
			{
				ALERT ( at_console, "Player is silent\n" );
				m_fNoPlayerSound = TRUE;
			}
			break;
		}

	case 106:
		// Give me the classname and targetname of this entity.
		pEntity = FindEntityForward( this );
		if ( pEntity )
		{
			ALERT ( at_console, "Classname: %s", STRING( pEntity->pev->classname ) );
			
			if ( !FStringNull ( pEntity->pev->targetname ) )
			{
				ALERT ( at_console, " - Targetname: %s\n", STRING( pEntity->pev->targetname ) );
			}
			else
			{
				ALERT ( at_console, " - TargetName: No Targetname\n" );
			}

			ALERT ( at_console, "Model: %s\n", STRING( pEntity->pev->model ) );
			if ( pEntity->pev->globalname )
				ALERT ( at_console, "Globalname: %s\n", STRING( pEntity->pev->globalname ) );
		}
		break;

	case 107:
		{
			TraceResult tr;

			edict_t		*pWorld = g_engfuncs.pfnPEntityOfEntIndex( 0 );

			Vector start = pev->origin + pev->view_ofs;
			Vector end = start + gpGlobals->v_forward * 1024;
			UTIL_TraceLine( start, end, ignore_monsters, edict(), &tr );
			if ( tr.pHit )
				pWorld = tr.pHit;
			const char *pTextureName = TRACE_TEXTURE( pWorld, start, end );
			if ( pTextureName )
				ALERT( at_console, "Texture: %s\n", pTextureName );
		}
		break;
	case	195:// show shortest paths for entire level to nearest node
		{
			Create("node_viewer_fly", pev->origin, pev->angles);
		}
		break;
	case	196:// show shortest paths for entire level to nearest node
		{
			Create("node_viewer_large", pev->origin, pev->angles);
		}
		break;
	case	197:// show shortest paths for entire level to nearest node
		{
			Create("node_viewer_human", pev->origin, pev->angles);
		}
		break;
	case	199:// show nearest node and all connections
		{
			ALERT ( at_console, "%d\n", WorldGraph.FindNearestNode ( pev->origin, bits_NODE_GROUP_REALM ) );
			WorldGraph.ShowNodeConnections ( WorldGraph.FindNearestNode ( pev->origin, bits_NODE_GROUP_REALM ) );
		}
		break;
	case	202:// Random blood splatter
		UTIL_MakeVectors(pev->v_angle);
		UTIL_TraceLine ( pev->origin + pev->view_ofs, pev->origin + pev->view_ofs + gpGlobals->v_forward * 128, ignore_monsters, ENT(pev), & tr);

		if ( tr.flFraction != 1.0 )
		{// line hit something, so paint a decal
			CBloodSplat *pBlood = GetClassPtr((CBloodSplat *)NULL);
			pBlood->Spawn( pev );
		}
		break;
	case	203:// remove creature.
		pEntity = FindEntityForward( this );
		if ( pEntity )
		{
			if ( pEntity->pev->takedamage )
				pEntity->SetThink(&CBaseEntity::SUB_Remove);
		}
		break;
	}
#endif	// HLDEMO_BUILD
}

//
// Add a weapon to the player (Item == Weapon == Selectable Object)
//
int CBasePlayer::AddPlayerItem( CBasePlayerItem *pItem )
{
	CBasePlayerItem *pInsert;
	
	pInsert = m_rgpPlayerItems[pItem->iItemSlot()];

	while (pInsert)
	{
		if (FClassnameIs( pInsert->pev, STRING( pItem->pev->classname) ))
		{
			if (pItem->AddDuplicate( pInsert ))
			{
				g_pGameRules->PlayerGotWeapon ( this, pItem );
				pItem->CheckRespawn();

				// ugly hack to update clip w/o an update clip message
				pInsert->UpdateItemInfo( );
				if (m_pActiveItem)
					m_pActiveItem->UpdateItemInfo( );

				pItem->Kill( );
			}
			else if (gEvilImpulse101)
			{
				// FIXME: remove anyway for deathmatch testing
				pItem->Kill( );
			}
			//easyForcePrintLine("ALREADY GOT ME SONNY");
			return FALSE;
		}
		pInsert = pInsert->m_pNext;
	}


	if (pItem->AddToPlayer( this ))
	{
		g_pGameRules->PlayerGotWeapon ( this, pItem );
		pItem->CheckRespawn();

		pItem->m_pNext = m_rgpPlayerItems[pItem->iItemSlot()];
		m_rgpPlayerItems[pItem->iItemSlot()] = pItem;

		// should we switch to this item?
		if ( g_pGameRules->FShouldSwitchWeapon( this, pItem ) )
		{
			SwitchWeapon( pItem );
		}

		return TRUE;
	}
	else if (gEvilImpulse101)
	{
		// FIXME: remove anyway for deathmatch testing
		pItem->Kill( );
	}
	return FALSE;
}//END OF AddPlayerItem(...)



void CBasePlayer::printOutWeapons(){
	//CBasePlayerWeapon whut;
	//whut.PrintState();

	for(int i = 0; i < 6; i++){
		int i2 = 0;
		CBasePlayerItem* thisItem = m_rgpPlayerItems[i];
		while(thisItem){

			easyForcePrintLine("%d:%d %s", i, i2, STRING(thisItem->pev->classname) );
			i2++;
			thisItem = thisItem->m_pNext;
		}//END OF while(...)
	}//END OF for(...)

}//END OF printOutWeapons



//TEST: see if, given an item with this iItemSlot and classname string, we are capable of adding it (not out of ammo).
BOOL CBasePlayer::CanAddPlayerItem( int arg_iItemSlot, const char* arg_classname, const char* arg_ammoname, int arg_iMaxAmmo)
{

	//printOutWeapons();

	CBasePlayerItem *pInsert;
	
	pInsert = m_rgpPlayerItems[arg_iItemSlot];

	while (pInsert)
	{

		//easyForcePrintLine("MATCH TEST?? %s", STRING(pInsert->pev->classname) ); 
		if (FClassnameIs( pInsert->pev, arg_classname ))
		{
			//if (pItem->AddDuplicate( pInsert ))
			if(g_pGameRules->CanHaveAmmo(this, arg_ammoname, arg_iMaxAmmo) )
			{
				//g_pGameRules->PlayerGotWeapon ( this, pItem );
				//pItem->CheckRespawn();

				// ugly hack to update clip w/o an update clip message
				/*
				pInsert->UpdateItemInfo( );
				if (m_pActiveItem)
					m_pActiveItem->UpdateItemInfo( );

				pItem->Kill( );
				*/
				return TRUE;  //we can add it!
			}
			else 
			{
				//The player already has this item, but this item rejects taking ammo? Denied.
				//return FALSE;
				//...oh, already falls to FALSE below. neat.
				//pItem->Kill( );
			}
			//easyForcePrintLine("ALREADY GOT ME SONNY");
			return FALSE;
		}
		pInsert = pInsert->m_pNext;
	}


	//For now, if the player doesn't already have it, just assume "yes".

	return TRUE;
}//END OF CanAddPlayerItem(...)








int CBasePlayer::RemovePlayerItem( CBasePlayerItem *pItem )
{
	if (m_pActiveItem == pItem)
	{
		ResetAutoaim( );
		pItem->Holster( );
		pItem->pev->nextthink = 0;// crowbar may be trying to swing again, etc.
		pItem->SetThink( NULL );
		m_pActiveItem = NULL;
		pev->viewmodel = 0;
		pev->weaponmodel = 0;
	}
	else if ( m_pLastItem == pItem )
		m_pLastItem = NULL;

	CBasePlayerItem *pPrev = m_rgpPlayerItems[pItem->iItemSlot()];

	if (pPrev == pItem)
	{
		m_rgpPlayerItems[pItem->iItemSlot()] = pItem->m_pNext;
		return TRUE;
	}
	else
	{
		while (pPrev && pPrev->m_pNext != pItem)
		{
			pPrev = pPrev->m_pNext;
		}
		if (pPrev)
		{
			pPrev->m_pNext = pItem->m_pNext;
			return TRUE;
		}
	}
	return FALSE;
}


//
// Returns the unique ID for the ammo, or -1 if error
//
int CBasePlayer :: GiveAmmo( int iCount, char *szName, int iMax )
{
	if ( !szName )
	{
		// no ammo.
		return -1;
	}

	if ( !g_pGameRules->CanHaveAmmo( this, szName, iMax ) )
	{
		// game rules say I can't have any more of this ammo type.
		return -1;
	}

	int i = 0;

	i = GetAmmoIndex( szName );

	if ( i < 0 || i >= MAX_AMMO_SLOTS )
		return -1;

	int iAdd = min( iCount, iMax - m_rgAmmo[i] );
	if ( iAdd < 1 )
		return i;

	m_rgAmmo[ i ] += iAdd;


	if ( gmsgAmmoPickup )  // make sure the ammo messages have been linked first
	{
		// Send the message that ammo has been picked up
		MESSAGE_BEGIN( MSG_ONE, gmsgAmmoPickup, NULL, pev );
			WRITE_BYTE( GetAmmoIndex(szName) );		// ammo ID
			WRITE_BYTE( iAdd );		// amount
		MESSAGE_END();
	}

	TabulateAmmo();

	return i;
}


/*
============
ItemPreFrame

Called every frame by the player PreThink
============
*/
void CBasePlayer::ItemPreFrame()
{
	if(global_testVar == -1)return;


	//Even though ItemPostFrame() turns this off faster, it turns it off too fast. At least one full frame must run
	//with the "res" physics flag left on to send to the client to be effective and block the jump-land sound.
	if(pev->iuser4 & FLAG_RESET_RECEIVED){
		pev->iuser4 &= ~FLAG_RESET_RECEIVED;
		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "res", "0" );
		//turn this phyiscs flag off, we got the signal back.
		//easyForcePrintLine("HOLY thing thinger-thinger");
	}




	//MODDD - moved here.  May as well terminate here if NULL, so that the new "think" method only happens if at least there is an item to "think" for.
	if (!m_pActiveItem)
		return;








	//MODDD - just the first time, in case of load / starting out with an item
	if(globalPSEUDO_forceFirstPersonIdleDelay == 1){
		
		CBasePlayerWeapon* weapTest = (CBasePlayerWeapon*)m_pActiveItem->GetWeaponPtr();

		if(weapTest != NULL){
			//forceNoWeaponLoop = TRUE;
			weapTest->forceBlockLooping();
			weapTest->m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + weapTest->randomIdleAnimationDelay();
			globalPSEUDO_forceFirstPersonIdleDelay = 0;
		}
		
	}


	//MODDD - reference to new method.  Override this method in any item / weapon to get a "think"
	//frame method that does notdepend on "m_flNextAttack" not being used at the moment.
	m_pActiveItem->ItemPreFrameThink( );

#if defined( CLIENT_WEAPONS )
    if ( m_flNextAttack > 0 )
#else
    if ( gpGlobals->time < m_flNextAttack )
#endif
	{
		return;
	}

	

	m_pActiveItem->ItemPreFrame( );
}


/*
============
ItemPostFrame

Called every frame by the player PostThink
============
*/
void CBasePlayer::ItemPostFrame()
{
	static int fInSelect = FALSE;

	// check if the player is using a tank
	if ( m_pTank != NULL )
		return;

	
	BOOL canCallItemPostFrame = TRUE;

	//MODDD - moved here.  May as well terminate here if NULL, so that the new "think" method only happens if at least there is an item to "think" for.
	//(actually, just use a bool for more control)
	if (!m_pActiveItem)
		canCallItemPostFrame = FALSE;

	//MODDD - reference to new method.  Override this method in any item / weapon to get a "think"
	//frame method that does notdepend on "m_flNextAttack" not being used at the moment.
	if(canCallItemPostFrame){
		m_pActiveItem->ItemPostFrameThink( );
	}	

	//MODDDD - now always done, actually, BUT with a few edits to ensure the same logic.
	//MODDDD - no, reverted to normal for now...
	//m_pActiveItem->ItemPostFrame( );
	
		

#if defined( CLIENT_WEAPONS )
	//easyForcePrintLine("AAA I WILL be a decent fellow %.2f", m_flNextAttack);
    if ( m_flNextAttack > 0 )
#else
	//easyForcePrintLine("BBB I WILL be a decent fellow %.2f %.2f", gpGlobals->time, m_flNextAttack);
    if ( gpGlobals->time < m_flNextAttack )
#endif
	{
		return;
	}

	//MODDD - also moved here.  "ImpulseCommands", also responsible for the "use" key, should happen regardless of having a weapon or not.
	ImpulseCommands();

	//MODDD - this is done (the if-then) instead so that "impulseCommands" can happen regardless of whether the weapon is null or not.
	if(canCallItemPostFrame){
		m_pActiveItem->ItemPostFrame( );
	}
	
}

int CBasePlayer::AmmoInventory( int iAmmoIndex )
{
	if (iAmmoIndex == -1)
	{
		return -1;
	}

	return m_rgAmmo[ iAmmoIndex ];
}




int CBasePlayer::GetAmmoIndex(const char *psz)
{
	int i;

	if (!psz)
		return -1;


	for (i = 1; i < MAX_AMMO_SLOTS; i++)
	{
		
		if ( !CBasePlayerItem::AmmoInfoArray[i].pszName )
			continue;

		if (stricmp( psz, CBasePlayerItem::AmmoInfoArray[i].pszName ) == 0)
			return i;
	}

	return -1;
}

// Called from UpdateClientData
// makes sure the client has all the necessary ammo info,  if values have changed
void CBasePlayer::SendAmmoUpdate(void)
{
	for (int i=0; i < MAX_AMMO_SLOTS;i++)
	{

		//2 is the glock ID.
		/*
		if(i == 2){
			easyPrintLine("GLOCK AMMO: ! %d, %d, %d", i,  m_rgAmmo[i], m_rgAmmoLast[i]);
		}
		*/

		if (m_rgAmmo[i] != m_rgAmmoLast[i])
		{
			m_rgAmmoLast[i] = m_rgAmmo[i];

			ASSERT( m_rgAmmo[i] >= 0 );
			ASSERT( m_rgAmmo[i] < 255 );

			// send "Ammo" update message
			MESSAGE_BEGIN( MSG_ONE, gmsgAmmoX, NULL, pev );
				WRITE_BYTE( i );
				WRITE_BYTE( max( min( m_rgAmmo[i], 254 ), 0 ) );  // clamp the value to one byte
			MESSAGE_END();
		}
	}
}



int boolToInt(BOOL& someVar){
	if(someVar == FALSE){
		return 0;
	}else{
		return 1;
	}
}


/*
=========================================================
	UpdateClientData

resends any changed player HUD info to the client.
Called every frame by PlayerPreThink
Also called at start of demo recording and playback by
ForceClientDllUpdate to ensure the demo gets messages
reflecting all of the HUD state info.
=========================================================
*/
void CBasePlayer :: UpdateClientData( void )
{

	if(this->superDuperDelay == -2){
		superDuperDelay = gpGlobals->time + 1;
	}





	if (m_fInitHUD)
	{
		m_fInitHUD = FALSE;
		gInitHUD = FALSE;

		MESSAGE_BEGIN( MSG_ONE, gmsgResetHUD, NULL, pev );
			WRITE_BYTE( 0 );
		MESSAGE_END();

		

		if ( !m_fGameHUDInitialized )
		{
			MESSAGE_BEGIN( MSG_ONE, gmsgInitHUD, NULL, pev );
			MESSAGE_END();

			g_pGameRules->InitHUD( this );
			m_fGameHUDInitialized = TRUE;
			if ( g_pGameRules->IsMultiplayer() )
			{
				FireTargets( "game_playerjoin", this, this, USE_TOGGLE, 0 );
			}
		}

		FireTargets( "game_playerspawn", this, this, USE_TOGGLE, 0 );

		InitStatusBar();
	}

	if ( m_iHideHUD != m_iClientHideHUD )
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgHideWeapon, NULL, pev );
			WRITE_BYTE( m_iHideHUD );
		MESSAGE_END();




		m_iClientHideHUD = m_iHideHUD;
	}

	//easyPrintLine("EEEEEEE %d", m_rgbTimeBasedDamage[itbd_Bleeding]);
	//easyPrintLine("EEDDD %u %u, %u, %u", 1 << 28, 1 << 29, 1 << 30, 1 << 31);



	if(the_default_fov == 0){
		the_default_fov = CVAR_GET_POINTER( "default_fov" );
		fvoxEnabled = CVAR_GET_POINTER("cl_fvox");
		//cl_fvoxMem = CVAR_GET_POINTER("IGNOREfvoxEnabledMem");
		cl_ladder = CVAR_GET_POINTER("cl_ladder");
		//g_engfuncs.pfnSetPhysicsKeyValue( edict(), "slj", "0" );
	}

	//easyPrintLine("??????? %d", gpGlobals->maxEntities);
	



	if(superDuperDelay > -1 && superDuperDelay < gpGlobals->time){
		MESSAGE_BEGIN( MSG_ONE, gmsgAutoMus, NULL, pev );
		MESSAGE_END();

		//superDuperDelay = -1;
		superDuperDelay = gpGlobals->time + 60*4.5;
	}



	//SCOPE
	{
	BOOL tempB = ( (pev->flags & FL_FROZEN) != 0);
	if(iWasFrozenToday != tempB ){
		iWasFrozenToday = tempB;
		//easyPrintLine("I WAS FROZEN, TODAY!? %d", tempB);
		MESSAGE_BEGIN( MSG_ONE, gmsgUpdateFreezeStatus, NULL, pev );
			WRITE_BYTE( tempB );
		MESSAGE_END();
	}

	}//END OF SCOPE

	
	if(noclipSpeedMultiMem != global_noclipSpeedMulti){
		noclipSpeedMultiMem = global_noclipSpeedMulti;
		
		if(noclipSpeedMultiMem != 0){

			char buffer[13];
			tryFloatToStringBuffer(buffer, global_noclipSpeedMulti);

			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "ncm", buffer );
		}else{
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "ncm", "0" );
		}
	}
	if(normalSpeedMultiMem != global_normalSpeedMulti){
		normalSpeedMultiMem = global_normalSpeedMulti;
		//keep this CVar in sync with pm_shared...
		if(normalSpeedMultiMem != 0){



			char buffer[13];
			tryFloatToStringBuffer(buffer, global_normalSpeedMulti);

			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "nsm", buffer );
		}else{
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "nsm", "0" );
		}
	}
	if(jumpForceMultiMem != global_jumpForceMulti){
		jumpForceMultiMem = global_jumpForceMulti;
		//keep this CVar in sync with pm_shared...
		if(jumpForceMultiMem != 0){

			char buffer[13];
			tryFloatToStringBuffer(buffer, global_jumpForceMulti);
			
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "jfm", buffer );
		}else{
			g_engfuncs.pfnSetPhysicsKeyValue( edict(), "jfm", "0" );
		}
	}

	if(infiniteLongJumpChargeMem != global_infiniteLongJumpCharge){
		infiniteLongJumpChargeMem = global_infiniteLongJumpCharge;

	}



	if(clearWeaponFlag == -1){
		MESSAGE_BEGIN( MSG_ONE, gmsgClearWeapon, NULL, pev );
			//WRITE_SHORT( (int)useAlphaCrosshair->value);
		MESSAGE_END();
		clearWeaponFlag = 1;
	}

	if(autoSneakyMem != global_autoSneaky){
		autoSneakyMem = global_autoSneaky;
		autoSneakyCheck();
	}




	if(cameraModeMem != globalPSEUDO_cameraMode || mirrorsDoNotReflectPlayerMem != global_mirrorsDoNotReflectPlayer){
		cameraModeMem = globalPSEUDO_cameraMode;
		mirrorsDoNotReflectPlayerMem = global_mirrorsDoNotReflectPlayer;

		BOOL allowPlayerMarker = FALSE;
		CBaseEntity *pEntityTemp = NULL;


		/*
		MESSAGE_BEGIN( MSG_ONE, gmsgUpdateCam, NULL, pev );
			//WRITE_SHORT( (int)pev->armorvalue);
		MESSAGE_END();
		*/



		
		/*
		if(cameraModeMem == 0 && mirrorsDoNotReflectPlayerMem != 1){
			allowPlayerMarker = TRUE;
		}else{
			allowPlayerMarker = FALSE;
		}
		//easyPrintLine("CHANGE STATUS: %d   %.2f %.2f ", allowPlayerMarker, cameraModeMem, mirrorsDoNotReflectPlayerMem);
		
		if(allowPlayerMarker){
			while ((pEntityTemp = UTIL_FindEntityByClassname( pEntityTemp, "player_marker" )) != NULL){
					//pEntityTemp->pev->effects &= ~128;  //can't draw in first person!
					//pEntityTemp->pev->renderfx &= ~NOREFLECT;
					//pEntityTemp->pev->renderfx |= ISPLAYER;

				pEntityTemp->pev->effects &= ~128;
			}
		}else{
			while ((pEntityTemp = UTIL_FindEntityByClassname( pEntityTemp, "player_marker" )) != NULL){
					//pEntityTemp->pev->effects &= ~128;  //can't draw in first person!
					//pEntityTemp->pev->renderfx |= NOREFLECT;
				pEntityTemp->pev->effects |= 128;
			}
		}
		*/



	}







	if(global_barnacleCanGib != barnacleCanGibMem){


		/*
		const char* arg1ref = CMD_ARGV(1);
		if(!isStringEmpty(arg1ref)){
			try{
				int numbAttempt = tryStringToInt(arg1ref);
				//MODDD NOTE:  is this being a global var ok, or should it be put on the local player?  oh well.

				if(global_barnacleCanGib == numbAttempt){
					//already matches?  Don't do anything.
					return;
				}
				global_barnacleCanGib = numbAttempt;
			}catch(int err){
				return;
			}
		}else{
			return;
		}
		*/
		barnacleCanGibMem = global_barnacleCanGib;
	
		//MODDD - section
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		//If this CVar is changed, update all existing dead barnacles.  This makes barnacle corpses that were once unkillable killable, or vice versa.
	
			CBaseEntity *pEntityTemp = NULL;
		
			while ((pEntityTemp = UTIL_FindEntityByClassname( pEntityTemp, "monster_barnacle" )) != NULL)
			{
				//CBarnacle* tempBarnacle = (CBarnacle*)pEntityTemp;
				//easyForcePrintLine("FOUND ONE? %d ", pEntityTemp->pev->deadflag == DEAD_NO);
				if (pEntityTemp->pev->deadflag == DEAD_NO)
				{
				
				}else{

					if(global_barnacleCanGib == 0){
						pEntityTemp->pev->takedamage = DAMAGE_NO;
						pEntityTemp->pev->solid = SOLID_NOT;
					}else{
						pEntityTemp->pev->takedamage = DAMAGE_AIM;
						pEntityTemp->pev->solid = SOLID_SLIDEBOX;
					}
				}
			}
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}



	if(grabbedByBarnacleMem != grabbedByBarnacle){
		grabbedByBarnacleMem = grabbedByBarnacle;
		//update!  The client must know this (to apply a camera offset as to not clip the barnacle tentacle when looking up)
		MESSAGE_BEGIN( MSG_ONE, gmsgUpdBnclStat, NULL, pev );
			WRITE_BYTE( grabbedByBarnacle );
		MESSAGE_END();
	}



	if(drowningMem != drowning){
		//easyPrintLine("HELP %d %d" , drowningMem, drowning);
	

		drowningMem = drowning;

		MESSAGE_BEGIN( MSG_ONE, gmsgDrowning, NULL, pev );
			WRITE_BYTE( drowning );
		MESSAGE_END();
	}

	if(playerBrightLightMem != global_playerBrightLight){
		playerBrightLightMem = global_playerBrightLight;
		if(global_playerBrightLight == 1){
			pev->effects |= EF_BRIGHTLIGHT;
		}else{
			pev->effects &= ~EF_BRIGHTLIGHT;
		}

	}

	if(cl_ladderMem != cl_ladder->value){
		cl_ladderMem = cl_ladder->value;

		//thanks,
		//http://stackoverflow.com/questions/9655202/how-to-convert-integer-to-string-in-c


		int filter = cl_ladderMem;
		if(filter < 0){
			filter = 0;
		}
		if(filter > 2){
			filter = 2;
		}

		//"compatiblestring" is just going to be "cl_ladderMem" converted to a string (required by the physics key as a value).
		char compatiblestring[2];
		sprintf(compatiblestring, "%d", filter);
		compatiblestring[1] = '\0';

		g_engfuncs.pfnSetPhysicsKeyValue( edict(), "plm", compatiblestring );
		
	}
	



	if(fvoxEnabledMem == -2 && fvoxEnabled != NULL){

		//initial set:  bring the loaded value to there, no updates.
		fvoxOn = (int)(fvoxEnabled->value == 1);
		fvoxEnabledMem = fvoxEnabled->value;

		//CVAR_SET_FLOAT("cl_fvox", fvoxOn);
	}


	if(fvoxEnabled->value != fvoxEnabledMem && fvoxEnabledMem != -2){
		
		fvoxOn = (int)(fvoxEnabled->value == 1);
		fvoxEnabledMem = fvoxEnabled->value;

		if(fvoxEnabledMem == 1){
			//just turned it on.
			SetSuitUpdateFVoxException("!HEV_V0", FALSE, SUIT_REPEAT_OK);
		}else{
			//just turned it off.  Clear other queud messages.
			SetSuitUpdate(NULL, FALSE ,0);
			SetSuitUpdateFVoxException("!HEV_V1", FALSE, SUIT_REPEAT_OK);
		}

	}



	





	/*
	
	if(glockSilencerOn->value != glockSilencerOnVar){
		CVAR_SET_FLOAT("glockSilencerOn", glockSilencerOnVar);
		//glockSilencerOnMem = glockSilencerOn->value;
	}
	if(egonAltFireOn->value != egonAltFireOnVar){
		CVAR_SET_FLOAT("egonAltFireOn", egonAltFireOnVar);
	}
	*/












	

	//easyForcePrintLine("TTEEEE %.2f", globalPSEUDO_aspectratio_determined_fov	);

	//NOTE: we cool?
	//(the_default_fov->value == -1 && globalPSEUDO_aspectratio_determined_fov != -1) ||

	if(globalPSEUDO_aspectratio_determined_fov == -1){
		//prepare...
		queueTotalFOVUpdate = TRUE;
	}




	//easyForcePrintLine("ARE YOU THAT KINDA amazingly STUPID: %.2f %d", globalPSEUDO_aspectratio_determined_fov, queueTotalFOVUpdate);
	if ( globalPSEUDO_aspectratio_determined_fov != -1 && (queueTotalFOVUpdate == TRUE || ( 
		
		global_auto_adjust_fov_aspect != auto_adjust_fov_aspectmem ||
		(global_auto_adjust_fov_aspect == 1 && (pythonZoomFOV != global_python_zoomfov) || (crossbowZoomFOV != global_crossbow_zoomfov) ) ||
		the_default_fov->value != the_default_fovmem ||
		m_iFOV != m_iClientFOV ||
		auto_adjust_zoomfovMem != global_auto_adjust_zoomfov||
		canApplyDefaultFOVMem != global_canApplyDefaultFOV)
		))
	{

		queueTotalFOVUpdate = FALSE;

		/*
		int fovToUse = m_iFOV;
		//|| m_iFOV == (int)python_zoomfov->value || m_iFOV == (int)crossbow_zoomfov->value
		if(m_iFOV == 0 || m_iFOV == (int)pythonZoomFOV || m_iFOV == (int)crossbowZoomFOV  ){
			fovToUse = (int)the_default_fov->value;
		}
		*/

		int fovToUse = (int)the_default_fov->value;

		canApplyDefaultFOVMem = (int) global_canApplyDefaultFOV;
		the_default_fovmem = (int)the_default_fov->value;
		
		//easyPrintLine("VALUe1 %.2f %d: ", the_default_fov->value, fovToUse);
		//easyPrintLine("ASPECT AID... %.2f %d", aspectratio_determined_fov->value, (int)aspectratio_determined_fov->value);

		//if "auto_adjust_fov_aspect" was changed, do a re-check.
		if( (global_auto_adjust_fov_aspect != auto_adjust_fov_aspectmem || global_canApplyDefaultFOV == 1) && global_auto_adjust_fov_aspect == 1 ){
			int newDefaultFOV = (int)globalPSEUDO_aspectratio_determined_fov;
			
			//NOTE - fov can be made invible by just sending off by the usual MESSAGE procedure.
			CVAR_SET_FLOAT("default_fov", (float)newDefaultFOV);
			the_default_fovmem = newDefaultFOV;


			//CALL CONSTANT METHODS!!???

			EASY_CVAR_SET_DEBUGONLY(canApplyDefaultFOV, 0)
			//CVAR_SET_FLOAT("canApplyDefaultFOV", 0);
			canApplyDefaultFOVMem = 0;

			
			//wait, wasn't this done right above?
			//the_default_fov->value = (float)newDefaultFOV;
			//the_default_fovmem = newDefaultFOV;

			fovToUse = newDefaultFOV;
			m_iFOV = newDefaultFOV;
		}

		auto_adjust_fov_aspectmem = (int)global_auto_adjust_fov_aspect;

		
		//easyPrintLine("VALUe2 %.2f %d: ", the_default_fov->value, fovToUse);


		MESSAGE_BEGIN( MSG_ONE, gmsgSetFOV, NULL, pev );
			WRITE_BYTE( m_iFOV );
		MESSAGE_END();
		//I think this should update "m_iClientFOV".

		auto_adjust_zoomfovMem = global_auto_adjust_zoomfov;

		//easyPrintLine("FOV STATS?! (%.2f), (%d)", auto_adjust_zoomfov->value,  auto_adjust_zoomfovMem);
		//easyPrintLine("VALUe3 %.2f %d: ", the_default_fov->value, fovToUse);

		
			//easyPrintLine("PYTHOS: %.2f ::: <- ::: %.2f : %.2f : %.2f", global_python_zoomfov, aspectratio_determined_fov->value, global_auto_adjust_zoomfov, global_auto_adjust_fov_aspect);
			
			
			

		if(global_auto_adjust_zoomfov == 1){

			//CVAR_SET_FLOAT("python_zoomfov", (float)((int)(fovToUse * 0.4444f) ));
			//CVAR_SET_FLOAT("crossbow_zoomfov", (float)((int)(fovToUse * 0.2222f) ));


			
			EASY_CVAR_SET_DEBUGONLY(python_zoomfov, (float)((int)(fovToUse * 0.4444f) ) );
			EASY_CVAR_SET_DEBUGONLY(crossbow_zoomfov, (float)((int)(fovToUse * 0.2222f) ) );
			queueZoomFOVUpdate = FALSE;

			
			//TODO:  this keeps updating on just zooming in / out... why though?
			//easyForcePrintLine("GAHHHHH %d %.2f %.2f", fovToUse, global_python_zoomfov, global_crossbow_zoomfov);



			//python_zoomfov->value = (int)(fovToUse * 0.4444f);
			//crossbow_zoomfov->value = (int)(fovToUse * 0.2222f);

			//easyPrintLine("Did it update?!? (%.2f), %d, %0.2f, %d", crossbow_zoomfov->value, fovToUse, fovToUse * 0.2222f, (int)(fovToUse * 0.2222f));
		}
		
		//easyPrintLine("VALUe4 %.2f %d: ", the_default_fov->value, fovToUse);
		//easyPrintLine("VALUe5 %.2f: ", CVAR_GET_FLOAT("default_fov") );


		// cache FOV change at end of function, so weapon updates can see that FOV has changed



	}//END OF super massive FOV checker.


	
	if( pythonZoomFOV != global_python_zoomfov){
		pythonZoomFOV = global_python_zoomfov;
		//queueZoomFOVUpdate = TRUE;
	}
	if( crossbowZoomFOV != global_crossbow_zoomfov){
		//easyPrintLine("Crossbow val? %.2f", crossbow_zoomfov->value );
		crossbowZoomFOV = global_crossbow_zoomfov;
		//queueZoomFOVUpdate = TRUE;
	}

	
	//easyForcePrintLine("disregard you 666 %.2f %.2f", global_python_zoomfov, global_crossbow_zoomfov);

	/*
	if(queueZoomFOVUpdate && globalPSEUDO_aspectratio_determined_fov != -1){
		
		//DO IT
		//is that okay?
		int fovToUse = (int)the_default_fov->value;

		EASY_CVAR_SET_DEBUGONLY(python_zoomfov, (float)((int)(fovToUse * 0.4444f) ) );
		EASY_CVAR_SET_DEBUGONLY(crossbow_zoomfov, (float)((int)(fovToUse * 0.2222f) ) );
		queueZoomFOVUpdate = FALSE;
		
	}
	*/









	if(skillMem != g_iSkillLevel){
		skillMem = g_iSkillLevel;
		
		/*
		//We'll let util.cpp do this instead...
		g_iSkillLevel = (int)CVAR_skillMem;
		g_pGameRules->RefreshSkillData();
		*/
		
		updateTimedDamageDurations((int)g_iSkillLevel);
	}
	//easyPrintLine("DIFFICULTY: %d", g_iSkillLevel);


	// HACKHACK -- send the message to display the game title
	if (gDisplayTitle)
	{
		MESSAGE_BEGIN( MSG_ONE, gmsgShowGameTitle, NULL, pev );
		WRITE_BYTE( 0 );
		MESSAGE_END();
		gDisplayTitle = 0;
	}

	if (pev->health != m_iClientHealth)
	{
		int iHealth = max( pev->health, 0 );  // make sure that no negative health values are sent

		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgHealth, NULL, pev );
			WRITE_BYTE( iHealth );
		MESSAGE_END();

		m_iClientHealth = pev->health;
	}


	//MODDD 
	//Whenever the deadFlag is altered, tell the GUI about it.
	if(pev->deadflag != deadflagmem){
		deadflagmem = pev->deadflag;


		MESSAGE_BEGIN( MSG_ONE, gmsgUpdatePlayerAlive, NULL, pev );
			WRITE_SHORT( IsAlive() );
		MESSAGE_END();

	}


	//MODDD - update cheat vars.
	

	if(cheat_infiniteclipMem != global_cheat_infiniteclip){
		cheat_infiniteclipMem = global_cheat_infiniteclip;
	}
	if(cheat_infiniteammoMem != global_cheat_infiniteammo){
		cheat_infiniteammoMem = global_cheat_infiniteammo;
	}
	if(cheat_minimumfiredelayMem != global_cheat_minimumfiredelay){
		cheat_minimumfiredelayMem = global_cheat_minimumfiredelay;
	}
	if(cheat_minimumfiredelaycustomMem != global_cheat_minimumfiredelaycustom){
		cheat_minimumfiredelaycustomMem = global_cheat_minimumfiredelaycustom;
	}
	if(cheat_nogaussrecoilMem != global_cheat_nogaussrecoil){
		cheat_nogaussrecoilMem = global_cheat_nogaussrecoil;
	}
	if(gaussRecoilSendsUpInSPMem != global_gaussRecoilSendsUpInSP){
		gaussRecoilSendsUpInSPMem = global_gaussRecoilSendsUpInSP;
	}

	

	
	if(recoveryIndexMem != recoveryIndex){
		recoveryIndexMem = recoveryIndex;


		if(recoveryIndex == 0){
			//expected to recover sometime...
			recoveryDelay = gpGlobals->time + 6.0f;
			recoveryDelayMin = gpGlobals->time + 0.4f;
		}
		if(recoveryIndex == 1){
			//expected to recover sometime...
			recoveryDelay = gpGlobals->time + RANDOM_FLOAT(2.5f, 3.8f);
		}
	}


	if(recentlyGibbedMem != recentlyGibbed){
		recentlyGibbedMem = recentlyGibbed;
	}





	
	//MODDD - the next four if-then's are new.
	if (m_rgItems[ITEM_ANTIDOTE] != m_iClientAntidote)
	{
		m_iClientAntidote = m_rgItems[ITEM_ANTIDOTE];

		ASSERT( gmsgAntidoteP > 0 );
		MESSAGE_BEGIN( MSG_ONE, gmsgAntidoteP, NULL, pev );
		WRITE_SHORT( m_rgItems[ITEM_ANTIDOTE]);
		MESSAGE_END();
	}

	if (m_rgItems[ITEM_ADRENALINE] != m_iClientAdrenaline)
	{
		m_iClientAdrenaline = m_rgItems[ITEM_ADRENALINE];

		ASSERT( gmsgAdrenalineP > 0 );
		MESSAGE_BEGIN( MSG_ONE, gmsgAdrenalineP, NULL, pev );
		WRITE_SHORT( m_rgItems[ITEM_ADRENALINE]);
		MESSAGE_END();
	}

	if (m_rgItems[ITEM_RADIATION] != m_iClientRadiation)
	{
		m_iClientRadiation = m_rgItems[ITEM_RADIATION];

		ASSERT( gmsgRadiationP > 0 );
		MESSAGE_BEGIN( MSG_ONE, gmsgRadiationP, NULL, pev );
		WRITE_SHORT( m_rgItems[ITEM_RADIATION]);
		MESSAGE_END();
	}

	if(airTankAirTimeNeedsUpdate || airTankAirTime != airTankAirTimeMem){
		airTankAirTimeNeedsUpdate = FALSE;
		airTankAirTimeMem = airTankAirTime;

		//airTankAirTime = 30;
		//easyPrint("yes here is tank airtime %d\n", airTankAirTime);
		
		MESSAGE_BEGIN( MSG_ONE, gmsgUpdateAirTankAirTime, NULL, pev );
		WRITE_SHORT( (int)airTankAirTime);
		//WRITE_ANGLE
		MESSAGE_END();
		
	}


	if(m_fLongJumpMemory != m_fLongJump || (!m_fLongJump && longJumpCharge != -1) ){
		//easyPrint("TEST1 %d\n", 0);
		m_fLongJumpMemory = m_fLongJump;
		resetLongJumpCharge();
		longJumpChargeNeedsUpdate = TRUE;
	}


	if(longJumpChargeNeedsUpdate || longJumpChargeMem != longJumpCharge){
		longJumpChargeNeedsUpdate = FALSE;
		longJumpChargeMem = longJumpCharge;

		MESSAGE_BEGIN( MSG_ONE, gmsgUpdateLongJumpCharge, NULL, pev );
		
		WRITE_SHORT( (int)(longJumpCharge*100) );
		//WRITE_SHORT( (int)longJumpCharge);
		//WRITE_ANGLE
		MESSAGE_END();
		
	}

	

	if (pev->armorvalue != m_iClientBattery)
	{
		m_iClientBattery = pev->armorvalue;

		ASSERT( gmsgBattery > 0 );
		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgBattery, NULL, pev );
			WRITE_SHORT( (int)pev->armorvalue);
		MESSAGE_END();

		/*
		ASSERT( gmsgTester > 0 );
		MESSAGE_BEGIN( MSG_ONE, gmsgTester, NULL, pev );
			WRITE_SHORT( (int)pev->armorvalue);
		MESSAGE_END();
		*/
	}


	float hasGlockSilencerTEST = ( !(global_wpn_glocksilencer==0) && (hasGlockSilencer || global_wpn_glocksilencer==2 ));
	//easyForcePrintLine("disregardIN WHAT %d");
	if ( hasGlockSilencerTEST != hasGlockSilencerMem)
	{
		hasGlockSilencerMem = hasGlockSilencerTEST;

		MESSAGE_BEGIN( MSG_ONE, gmsgHasGlockSilencer, NULL, pev );
			WRITE_SHORT( (int)hasGlockSilencerTEST);
		MESSAGE_END();

		//UpdClientC
		

		/*
		ASSERT( gmsgTester > 0 );
		MESSAGE_BEGIN( MSG_ONE, gmsgTester, NULL, pev );
			WRITE_SHORT( (int)pev->armorvalue);
		MESSAGE_END();
		*/
	}



	/*
	//NOPE.  SEND BOTH NOW!
	float* damageToRead;
	if(global_painFlashIgnoreArmor == 1){
		//ignores armor damage reduction; what enemies intended to deal since last sendoff.
		damageToRead = &rawDamageSustained;
	}else{
		//factors in armor damage reduction.
		damageToRead = &pev->dmg_take;
	}
	*/


	//MODDD
	//if (pev->dmg_take || pev->dmg_save || m_bitsHUDDamage != m_bitsDamageType)
	//if ( *damageToRead || pev->dmg_save || m_bitsHUDDamage != m_bitsDamageType || m_bitsModHUDDamage != m_bitsDamageTypeMod)
	if (  pev->dmg_take || rawDamageSustained || pev->dmg_save || m_bitsHUDDamage != m_bitsDamageType || m_bitsModHUDDamage != m_bitsDamageTypeMod)
	{
		// Comes from inside me if not set
		Vector damageOrigin = pev->origin;
		// send "damage" message
		// causes screen to flash, and pain compass to show direction of damage
		edict_t *other = pev->dmg_inflictor;
		if ( other )
		{
			CBaseEntity *pEntity = CBaseEntity::Instance(other);
			if ( pEntity )
				damageOrigin = pEntity->Center();
		}

		// only send down damage type that have hud art
		int visibleDamageBits = m_bitsDamageType & DMG_SHOWNHUD;
		//MODDD
		int visibleDamageBitsMod = m_bitsDamageTypeMod & DMG_SHOWNHUDMOD;

		MESSAGE_BEGIN( MSG_ONE, gmsgDamage, NULL, pev );
			WRITE_BYTE( pev->dmg_save );

			WRITE_BYTE( pev->dmg_take );
			//MODDD
			WRITE_BYTE( rawDamageSustained );
			
			WRITE_LONG( visibleDamageBits );
			//MODDD
			WRITE_LONG( visibleDamageBitsMod );
			WRITE_COORD( damageOrigin.x );
			WRITE_COORD( damageOrigin.y );
			WRITE_COORD( damageOrigin.z );
		MESSAGE_END();
	
		pev->dmg_take = 0;
		pev->dmg_save = 0;
		rawDamageSustained = 0;
		m_bitsHUDDamage = m_bitsDamageType;
		//MODDD
		m_bitsModHUDDamage = m_bitsDamageTypeMod;
		
		// Clear off non-time-based damage indicators
		m_bitsDamageType &= DMG_TIMEBASED;
		//MODDD
		m_bitsModHUDDamage &= (DMG_TIMEBASEDMOD);
	}

	// Update Flashlight
	if ((m_flFlashLightTime) && (m_flFlashLightTime <= gpGlobals->time))
	{

		//This only appleis if the endlessFlashlightBattery CVar is 0 (off).
		if(global_endlessFlashlightBattery == 0){
			if (FlashlightIsOn())
			{
				if (m_iFlashBattery)
				{
					m_flFlashLightTime = FLASH_DRAIN_TIME + gpGlobals->time;
					m_iFlashBattery--;
				
					if (!m_iFlashBattery)
						FlashlightTurnOff();
				}
			}
			else
			{
				if (m_iFlashBattery < 100)
				{
					m_flFlashLightTime = FLASH_CHARGE_TIME + gpGlobals->time;
					m_iFlashBattery++;
				}
				else
					m_flFlashLightTime = 0;
			}
		

			MESSAGE_BEGIN( MSG_ONE, gmsgFlashBattery, NULL, pev );
			WRITE_BYTE(m_iFlashBattery);
			MESSAGE_END();

		}//END OF if(endlessFlashlightBattery->value == 0)
	}


	if (m_iTrain & TRAIN_NEW)
	{
		ASSERT( gmsgTrain > 0 );
		// send "health" update message
		MESSAGE_BEGIN( MSG_ONE, gmsgTrain, NULL, pev );
			WRITE_BYTE(m_iTrain & 0xF);
		MESSAGE_END();

		m_iTrain &= ~TRAIN_NEW;
	}

	//
	// New Weapon?
	//
	if (!m_fKnownItem)
	{
		m_fKnownItem = TRUE;

	// WeaponInit Message
	// byte  = # of weapons
	//
	// for each weapon:
	// byte		name str length (not including null)
	// bytes... name
	// byte		Ammo Type
	// byte		Ammo2 Type
	// byte		bucket
	// byte		bucket pos
	// byte		flags	
	// ????		Icons
		
		// Send ALL the weapon info now
		int i;

		for (i = 0; i < MAX_WEAPONS; i++)
		{
			ItemInfo& II = CBasePlayerItem::ItemInfoArray[i];

			if ( !II.iId )
				continue;

			const char *pszName;
			if (!II.pszName)
				pszName = "Empty";
			else
				pszName = II.pszName;

			MESSAGE_BEGIN( MSG_ONE, gmsgWeaponList, NULL, pev );  
				WRITE_STRING(pszName);			// string	weapon name
				WRITE_BYTE(GetAmmoIndex(II.pszAmmo1));	// byte		Ammo Type
				WRITE_BYTE(II.iMaxAmmo1);				// byte     Max Ammo 1
				WRITE_BYTE(GetAmmoIndex(II.pszAmmo2));	// byte		Ammo2 Type
				WRITE_BYTE(II.iMaxAmmo2);				// byte     Max Ammo 2
				WRITE_BYTE(II.iSlot);					// byte		bucket
				WRITE_BYTE(II.iPosition);				// byte		bucket pos
				WRITE_BYTE(II.iId);						// byte		id (bit index into pev->weapons)
				WRITE_BYTE(II.iFlags);					// byte		Flags
			MESSAGE_END();
		}
	}


	SendAmmoUpdate();

	// Update all the items
	for ( int i = 0; i < MAX_ITEM_TYPES; i++ )
	{
		if ( m_rgpPlayerItems[i] )  // each item updates it's successors
			m_rgpPlayerItems[i]->UpdateClientData( this );
	}

	// Cache and client weapon change
	m_pClientActiveItem = m_pActiveItem;
	m_iClientFOV = m_iFOV;

	// Update Status Bar
	if ( m_flNextSBarUpdateTime < gpGlobals->time )
	{
		UpdateStatusBar();
		m_flNextSBarUpdateTime = gpGlobals->time + 0.2;
	}
}


//=========================================================
// FBecomeProne - Overridden for the player to set the proper
// physics flags when a barnacle grabs player.
//=========================================================
BOOL CBasePlayer :: FBecomeProne ( void )
{
	//MODDD - how convenient!  The red carpet's rolled out all for me.
	this->grabbedByBarnacle = TRUE;

	m_afPhysicsFlags |= PFLAG_ONBARNACLE;
	return TRUE;
}

//=========================================================
// BarnacleVictimBitten - bad name for a function that is called
// by Barnacle victims when the barnacle pulls their head
// into its mouth. For the player, just die.
//=========================================================
void CBasePlayer :: BarnacleVictimBitten ( entvars_t *pevBarnacle )
{
	TakeDamage ( pevBarnacle, pevBarnacle, pev->health + pev->armorvalue, DMG_SLASH | DMG_ALWAYSGIB );
}

//=========================================================
// BarnacleVictimReleased - overridden for player who has
// physics flags concerns. 
//=========================================================
void CBasePlayer :: BarnacleVictimReleased ( void )
{
	//MODDD - same!
	this->grabbedByBarnacle = FALSE;

	m_afPhysicsFlags &= ~PFLAG_ONBARNACLE;
}


//=========================================================
// Illumination 
// return player light level plus virtual muzzle flash
//=========================================================
int CBasePlayer :: Illumination( void )
{
	int iIllum = CBaseEntity::Illumination( );

	iIllum += m_iWeaponFlash;
	if (iIllum > 255)
		return 255;
	return iIllum;
}


void CBasePlayer :: EnableControl(BOOL fControl)
{
	if (!fControl)
		pev->flags |= FL_FROZEN;
	else
		pev->flags &= ~FL_FROZEN;

}


#define DOT_1DEGREE   0.9998476951564
#define DOT_2DEGREE   0.9993908270191
#define DOT_3DEGREE   0.9986295347546
#define DOT_4DEGREE   0.9975640502598
#define DOT_5DEGREE   0.9961946980917
#define DOT_6DEGREE   0.9945218953683
#define DOT_7DEGREE   0.9925461516413
#define DOT_8DEGREE   0.9902680687416
#define DOT_9DEGREE   0.9876883405951
#define DOT_10DEGREE  0.9848077530122
#define DOT_15DEGREE  0.9659258262891
#define DOT_20DEGREE  0.9396926207859
#define DOT_25DEGREE  0.9063077870367

//=========================================================
// Autoaim
// set crosshair position to point to enemey
//=========================================================
Vector CBasePlayer :: GetAutoaimVector( float flDelta )
{
	if (g_iSkillLevel == SKILL_HARD)
	{
		UTIL_MakeVectors( pev->v_angle + pev->punchangle );
		return gpGlobals->v_forward;
	}

	Vector vecSrc = GetGunPosition( );
	float flDist = 8192;

	// always use non-sticky autoaim
	// UNDONE: use sever variable to chose!
	if (1 || g_iSkillLevel == SKILL_MEDIUM)
	{
		m_vecAutoAim = Vector( 0, 0, 0 );
		// flDelta *= 0.5;
	}

	BOOL m_fOldTargeting = m_fOnTarget;
	Vector angles = AutoaimDeflection(vecSrc, flDist, flDelta );

	// update ontarget if changed
	if ( !g_pGameRules->AllowAutoTargetCrosshair() )
		m_fOnTarget = 0;
	else if (m_fOldTargeting != m_fOnTarget)
	{
		m_pActiveItem->UpdateItemInfo( );
	}

	if (angles.x > 180)
		angles.x -= 360;
	if (angles.x < -180)
		angles.x += 360;
	if (angles.y > 180)
		angles.y -= 360;
	if (angles.y < -180)
		angles.y += 360;

	if (angles.x > 25)
		angles.x = 25;
	if (angles.x < -25)
		angles.x = -25;
	if (angles.y > 12)
		angles.y = 12;
	if (angles.y < -12)
		angles.y = -12;


	// always use non-sticky autoaim
	// UNDONE: use sever variable to chose!
	if (0 || g_iSkillLevel == SKILL_EASY)
	{
		m_vecAutoAim = m_vecAutoAim * 0.67 + angles * 0.33;
	}
	else
	{
		m_vecAutoAim = angles * 0.9;
	}

	// m_vecAutoAim = m_vecAutoAim * 0.99;

	// Don't send across network if sv_aim is 0
	if ( g_psv_aim->value != 0 )
	{
		if ( m_vecAutoAim.x != m_lastx ||
			 m_vecAutoAim.y != m_lasty )
		{
			SET_CROSSHAIRANGLE( edict(), -m_vecAutoAim.x, m_vecAutoAim.y );
			
			m_lastx = m_vecAutoAim.x;
			m_lasty = m_vecAutoAim.y;
		}
	}

	// ALERT( at_console, "%f %f\n", angles.x, angles.y );

	UTIL_MakeVectors( pev->v_angle + pev->punchangle + m_vecAutoAim );
	return gpGlobals->v_forward;
}


Vector CBasePlayer :: AutoaimDeflection( Vector &vecSrc, float flDist, float flDelta  )
{
	edict_t		*pEdict = g_engfuncs.pfnPEntityOfEntIndex( 1 );
	CBaseEntity	*pEntity;
	float		bestdot;
	Vector		bestdir;
	edict_t		*bestent;
	TraceResult tr;

	if ( g_psv_aim->value == 0 )
	{
		m_fOnTarget = FALSE;
		return g_vecZero;
	}

	UTIL_MakeVectors( pev->v_angle + pev->punchangle + m_vecAutoAim );

	// try all possible entities
	bestdir = gpGlobals->v_forward;
	bestdot = flDelta; // +- 10 degrees
	bestent = NULL;

	m_fOnTarget = FALSE;

	UTIL_TraceLine( vecSrc, vecSrc + bestdir * flDist, dont_ignore_monsters, edict(), &tr );


	if ( tr.pHit && tr.pHit->v.takedamage != DAMAGE_NO)
	{
		// don't look through water
		if (!((pev->waterlevel != 3 && tr.pHit->v.waterlevel == 3) 
			|| (pev->waterlevel == 3 && tr.pHit->v.waterlevel == 0)))
		{
			if (tr.pHit->v.takedamage == DAMAGE_AIM)
				m_fOnTarget = TRUE;

			return m_vecAutoAim;
		}
	}

	for ( int i = 1; i < gpGlobals->maxEntities; i++, pEdict++ )
	{
		Vector center;
		Vector dir;
		float dot;

		if ( pEdict->free )	// Not in use
			continue;
		
		if (pEdict->v.takedamage != DAMAGE_AIM)
			continue;
		if (pEdict == edict())
			continue;
//		if (pev->team > 0 && pEdict->v.team == pev->team)
//			continue;	// don't aim at teammate
		if ( !g_pGameRules->ShouldAutoAim( this, pEdict ) )
			continue;

		pEntity = Instance( pEdict );
		if (pEntity == NULL)
			continue;

		if (!pEntity->IsAlive())
			continue;

		// don't look through water
		if ((pev->waterlevel != 3 && pEntity->pev->waterlevel == 3) 
			|| (pev->waterlevel == 3 && pEntity->pev->waterlevel == 0))
			continue;

		center = pEntity->BodyTarget( vecSrc );

		dir = (center - vecSrc).Normalize( );

		// make sure it's in front of the player
		if (DotProduct (dir, gpGlobals->v_forward ) < 0)
			continue;

		dot = fabs( DotProduct (dir, gpGlobals->v_right ) ) 
			+ fabs( DotProduct (dir, gpGlobals->v_up ) ) * 0.5;

		// tweek for distance
		dot *= 1.0 + 0.2 * ((center - vecSrc).Length() / flDist);

		if (dot > bestdot)
			continue;	// to far to turn

		UTIL_TraceLine( vecSrc, center, dont_ignore_monsters, edict(), &tr );
		if (tr.flFraction != 1.0 && tr.pHit != pEdict)
		{
			// ALERT( at_console, "hit %s, can't see %s\n", STRING( tr.pHit->v.classname ), STRING( pEdict->v.classname ) );
			continue;
		}

		// don't shoot at friends
		if (IRelationship( pEntity ) < 0)
		{
			if ( !pEntity->IsPlayer() && !g_pGameRules->IsDeathmatch())
				// ALERT( at_console, "friend\n");
				continue;
		}

		// can shoot at this one
		bestdot = dot;
		bestent = pEdict;
		bestdir = dir;
	}

	if (bestent)
	{
		bestdir = UTIL_VecToAngles (bestdir);
		bestdir.x = -bestdir.x;
		bestdir = bestdir - pev->v_angle - pev->punchangle;

		if (bestent->v.takedamage == DAMAGE_AIM)
			m_fOnTarget = TRUE;

		return bestdir;
	}

	return Vector( 0, 0, 0 );
}


void CBasePlayer :: ResetAutoaim( )
{
	if (m_vecAutoAim.x != 0 || m_vecAutoAim.y != 0)
	{
		m_vecAutoAim = Vector( 0, 0, 0 );
		SET_CROSSHAIRANGLE( edict(), 0, 0 );
	}
	m_fOnTarget = FALSE;
}

/*
=============
SetCustomDecalFrames

  UNDONE:  Determine real frame limit, 8 is a placeholder.
  Note:  -1 means no custom frames present.
=============
*/
void CBasePlayer :: SetCustomDecalFrames( int nFrames )
{
	if (nFrames > 0 &&
		nFrames < 8)
		m_nCustomSprayFrames = nFrames;
	else
		m_nCustomSprayFrames = -1;
}

/*
=============
GetCustomDecalFrames

  Returns the # of custom frames this player's custom clan logo contains.
=============
*/
int CBasePlayer :: GetCustomDecalFrames( void )
{
	return m_nCustomSprayFrames;
}


//=========================================================
// DropPlayerItem - drop the named item, or if no name,
// the active item. 
//=========================================================
void CBasePlayer::DropPlayerItem ( char *pszItemName )
{
	//MODDD - new cvar.
	//if ( !g_pGameRules->IsMultiplayer() || (weaponstay.value > 0) )
	if ( global_canDropInSinglePlayer == 0 && (!g_pGameRules->IsMultiplayer() || (weaponstay.value > 0)) )
	{
		// no dropping in single player.
		return;
	}

	if ( !strlen( pszItemName ) )
	{
		// if this string has no length, the client didn't type a name!
		// assume player wants to drop the active item.
		// make the string null to make future operations in this function easier
		pszItemName = NULL;
	} 

	CBasePlayerItem *pWeapon;
	int i;

	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		pWeapon = m_rgpPlayerItems[ i ];

		while ( pWeapon )
		{
			if ( pszItemName )
			{
				// try to match by name. 
				if ( !strcmp( pszItemName, STRING( pWeapon->pev->classname ) ) )
				{
					// match! 
					break;
				}
			}
			else
			{
				// trying to drop active item
				if ( pWeapon == m_pActiveItem )
				{
					// active item!
					break;
				}
			}

			pWeapon = pWeapon->m_pNext; 
		}

		
		// if we land here with a valid pWeapon pointer, that's because we found the 
		// item we want to drop and hit a BREAK;  pWeapon is the item.
		if ( pWeapon )
		{
			g_pGameRules->GetNextBestWeapon( this, pWeapon );

			UTIL_MakeVectors ( pev->angles ); 

			pev->weapons &= ~(1<<pWeapon->m_iId);// take item off hud

			CWeaponBox *pWeaponBox = (CWeaponBox *)CBaseEntity::Create( "weaponbox", pev->origin + gpGlobals->v_forward * 10, pev->angles, edict() );
			pWeaponBox->pev->angles.x = 0;
			pWeaponBox->pev->angles.z = 0;
			pWeaponBox->PackWeapon( pWeapon );
			pWeaponBox->pev->velocity = gpGlobals->v_forward * 300 + gpGlobals->v_forward * 100;
			
			// drop half of the ammo for this weapon.
			int	iAmmoIndex;

			iAmmoIndex = GetAmmoIndex ( pWeapon->pszAmmo1() ); // ???
			
			if ( iAmmoIndex != -1 )
			{
				// this weapon weapon uses ammo, so pack an appropriate amount.
				if ( pWeapon->iFlags() & ITEM_FLAG_EXHAUSTIBLE )
				{
					// pack up all the ammo, this weapon is its own ammo type
					pWeaponBox->PackAmmo( MAKE_STRING(pWeapon->pszAmmo1()), m_rgAmmo[ iAmmoIndex ] );
					m_rgAmmo[ iAmmoIndex ] = 0; 

				}
				else
				{
					// pack half of the ammo
					pWeaponBox->PackAmmo( MAKE_STRING(pWeapon->pszAmmo1()), m_rgAmmo[ iAmmoIndex ] / 2 );
					m_rgAmmo[ iAmmoIndex ] /= 2; 
				}

			}


			/*
			easyPrintLine("DID YOU DO THE AMMO UPDATE???");

			SendAmmoUpdate();

			//MODDD - update the GUI.  Why was this not here?
			MESSAGE_BEGIN( MSG_ONE, gmsgCurWeapon, NULL, pev );
				WRITE_BYTE(0);
				WRITE_BYTE(0);
				WRITE_BYTE(0);
			MESSAGE_END();
			*/

			return;// we're done, so stop searching with the FOR loop.
		}
	}
}


//m_pPlayer->HasPlayerItem( this )




BOOL CBasePlayer::HasPlayerItem( CBasePlayerItem *pCheckItem )
{
	CBasePlayerItem *pItem = m_rgpPlayerItems[pCheckItem->iItemSlot()];

	while (pItem)
	{
		if (FClassnameIs( pItem->pev, STRING( pCheckItem->pev->classname) ))
		{
			return TRUE;
		}
		pItem = pItem->m_pNext;
	}

	return FALSE;
}

//MODDD - new version. given an item slot and classname instead of an item object.
BOOL CBasePlayer::HasPlayerItem( int arg_iItemSlot, const char* arg_className )
{
	CBasePlayerItem *pItem = m_rgpPlayerItems[arg_iItemSlot];

	while (pItem)
	{
		if (FClassnameIs( pItem->pev, arg_className ))
		{
			return TRUE;
		}
		pItem = pItem->m_pNext;
	}

	return FALSE;
}



//=========================================================
// HasNamedPlayerItem Does the player already have this item?
//=========================================================
BOOL CBasePlayer::HasNamedPlayerItem( const char *pszItemName )
{
	CBasePlayerItem *pItem;
	int i;
 
	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		pItem = m_rgpPlayerItems[ i ];
		
		while (pItem)
		{
			if ( !strcmp( pszItemName, STRING( pItem->pev->classname ) ) )
			{
				return TRUE;
			}
			pItem = pItem->m_pNext;
		}
	}

	return FALSE;
}


//MODDD - new.
CBasePlayerItem* CBasePlayer::FindNamedPlayerItem(const char *pszItemName){

	CBasePlayerItem *pItem;
	int i;
 
	for ( i = 0 ; i < MAX_ITEM_TYPES ; i++ )
	{
		pItem = m_rgpPlayerItems[ i ];
		
		while (pItem)
		{
			if ( !strcmp( pszItemName, STRING( pItem->pev->classname ) ) )
			{
				return pItem;
			}
			pItem = pItem->m_pNext;
		}
	}

	return FALSE;

	//gun = (CBasePlayerWeapon *)pPlayerItem->GetWeaponPtr();

}








//=========================================================
// 
//=========================================================
BOOL CBasePlayer :: SwitchWeapon( CBasePlayerItem *pWeapon ) 
{
	if ( !pWeapon->CanDeploy() )
	{
		return FALSE;
	}
	
	ResetAutoaim( );
	
	if (m_pActiveItem)
	{
		m_pActiveItem->Holster( );
	}

	m_pActiveItem = pWeapon;
	pWeapon->Deploy( );

	return TRUE;
}



//MODDD - SOME NEW PLAYER METHODS
void CBasePlayer::consumeAntidote(){
	//m_rgbTimeBasedDamage[itbd_NerveGas] = 0;
	//m_rgbTimeBasedDamage[itbd_Poison] = 0;
	removeTimedDamage(itbd_NerveGas, &m_bitsDamageType);
	removeTimedDamage(itbd_Poison, &m_bitsDamageType);
	m_rgItems[ITEM_ANTIDOTE]--;

	antidoteQueued = FALSE;

	MESSAGE_BEGIN( MSG_ONE, gmsgHUDItemFlash, NULL, pev );
		WRITE_BYTE( 0 );
	MESSAGE_END();


}//END OF consumeAntidote

void CBasePlayer::consumeRadiation(){
	removeTimedDamage(itbd_Radiation, &m_bitsDamageType);
	m_rgItems[ITEM_RADIATION]--;

	radiationQueued = FALSE;
	
	MESSAGE_BEGIN( MSG_ONE, gmsgHUDItemFlash, NULL, pev );
		WRITE_BYTE( 1 );
	MESSAGE_END();

}//END OF consumeRadiation

void CBasePlayer::consumeAdrenaline(){


	m_rgItems[ITEM_ADRENALINE]--;

	
	recoveryIndex = 1;
	recoveryIndexMem = 1;
	recoveryDelay = gpGlobals->time + RANDOM_FLOAT(2.5f, 3.9f);

	//m_rgItems[ITEM_ADRENALINE] --;
	//SetSuitUpdate("!HEV_ADR_USE", FALSE, SUIT_REPEAT_OK);

	if(global_batteryDrainsAtAdrenalineMode == 2){
		//MODDD - discharge battery.
		pev->armorvalue = 0;
		m_iClientBattery = pev->armorvalue;
		MESSAGE_BEGIN( MSG_ONE, gmsgBattery, NULL, pev );
			WRITE_SHORT( (int)pev->armorvalue);
		MESSAGE_END();
	}

	adrenalineQueued = FALSE;
	
	MESSAGE_BEGIN( MSG_ONE, gmsgHUDItemFlash, NULL, pev );
		WRITE_BYTE( 2 );
	MESSAGE_END();

}//END OF consumeAdrenaline
















/*
void CBasePlayer::Think(void)
{
	//NEVER CALLED
	CBaseMonster::Think();
	

	//easyPrintLine("PLAYER THINK CALLED?");
	//...strangely, this doesn't seem to get called.  Not an issue though, postThink is good enough.

}

void CBasePlayer::MonsterThink(void)
{
	//NEVER CALLED.  Player uses different think methods, which are linked differently.
	//The call "SetThink ( &CBaseMonster::CallMonsterThink );" from "StartMonster" in monsters.cpp, which is called (in a little time) from "MonsterInit" (also in monsters.cpp), is what sets "MonsterThink" to be the think method for here.
	CBaseMonster::MonsterThink();
	

	//easyPrintLine("PLAYER THINK CALLED?");
	//...strangely, this doesn't seem to get called.  Not an issue though, postThink is good enough.

}
*/











//=========================================================
// Dead HEV suit prop
//=========================================================
class CDeadHEV : public CBaseMonster
{
public:
	void Spawn( void );
	int	Classify ( void ) { return	CLASS_HUMAN_MILITARY; }

	void KeyValue( KeyValueData *pkvd );

	int	m_iPose;// which sequence to display	-- temporary, don't need to save
	static char *m_szPoses[4];
};

char *CDeadHEV::m_szPoses[] = { "deadback", "deadsitting", "deadstomach", "deadtable" };

void CDeadHEV::KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "pose"))
	{
		m_iPose = atoi(pkvd->szValue);
		pkvd->fHandled = TRUE;
	}
	else 
		CBaseMonster::KeyValue( pkvd );
}

LINK_ENTITY_TO_CLASS( monster_hevsuit_dead, CDeadHEV );

//=========================================================
// ********** DeadHEV SPAWN **********
//=========================================================
void CDeadHEV :: Spawn( void )
{
	PRECACHE_MODEL("models/player.mdl");
	SET_MODEL(ENT(pev), "models/player.mdl");

	pev->effects		= 0;
	pev->yaw_speed		= 8;
	pev->sequence		= 0;
	pev->body			= 1;
	m_bloodColor		= BLOOD_COLOR_RED;

	pev->sequence = LookupSequence( m_szPoses[m_iPose] );

	if (pev->sequence == -1)
	{
		ALERT ( at_console, "Dead hevsuit with bad pose\n" );
		pev->sequence = 0;
		pev->effects = EF_BRIGHTFIELD;
	}

	// Corpses have less health
	pev->health			= 8;

	MonsterInitDead();
}


class CStripWeapons : public CPointEntity
{
public:
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );

private:
};

LINK_ENTITY_TO_CLASS( player_weaponstrip, CStripWeapons );

void CStripWeapons :: Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer = NULL;

	if ( pActivator && pActivator->IsPlayer() )
	{
		pPlayer = (CBasePlayer *)pActivator;
	}
	else if ( !g_pGameRules->IsDeathmatch() )
	{
		pPlayer = (CBasePlayer *)CBaseEntity::Instance( g_engfuncs.pfnPEntityOfEntIndex( 1 ) );
	}

	if ( pPlayer ){
		//I think it is okay to remove items here?  unsure.
		pPlayer->RemoveAllItems( FALSE );
	}
}


class CRevertSaved : public CPointEntity
{
public:
	void	Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void	EXPORT MessageThink( void );
	void	EXPORT LoadThink( void );
	void	KeyValue( KeyValueData *pkvd );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	inline	float	Duration( void ) { return pev->dmg_take; }
	inline	float	HoldTime( void ) { return pev->dmg_save; }
	inline	float	MessageTime( void ) { return m_messageTime; }
	inline	float	LoadTime( void ) { return m_loadTime; }

	inline	void	SetDuration( float duration ) { pev->dmg_take = duration; }
	inline	void	SetHoldTime( float hold ) { pev->dmg_save = hold; }
	inline	void	SetMessageTime( float time ) { m_messageTime = time; }
	inline	void	SetLoadTime( float time ) { m_loadTime = time; }

private:
	float	m_messageTime;
	float	m_loadTime;
};

LINK_ENTITY_TO_CLASS( player_loadsaved, CRevertSaved );

TYPEDESCRIPTION	CRevertSaved::m_SaveData[] = 
{
	DEFINE_FIELD( CRevertSaved, m_messageTime, FIELD_FLOAT ),	// These are not actual times, but durations, so save as floats
	DEFINE_FIELD( CRevertSaved, m_loadTime, FIELD_FLOAT ),
};

IMPLEMENT_SAVERESTORE( CRevertSaved, CPointEntity );

void CRevertSaved :: KeyValue( KeyValueData *pkvd )
{
	if (FStrEq(pkvd->szKeyName, "duration"))
	{
		SetDuration( atof(pkvd->szValue) );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "holdtime"))
	{
		SetHoldTime( atof(pkvd->szValue) );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "messagetime"))
	{
		SetMessageTime( atof(pkvd->szValue) );
		pkvd->fHandled = TRUE;
	}
	else if (FStrEq(pkvd->szKeyName, "loadtime"))
	{
		SetLoadTime( atof(pkvd->szValue) );
		pkvd->fHandled = TRUE;
	}
	else 
		CPointEntity::KeyValue( pkvd );
}

void CRevertSaved :: Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	UTIL_ScreenFadeAll( pev->rendercolor, Duration(), HoldTime(), pev->renderamt, FFADE_OUT );
	pev->nextthink = gpGlobals->time + MessageTime();
	SetThink( &CRevertSaved::MessageThink );
}


void CRevertSaved :: MessageThink( void )
{
	UTIL_ShowMessageAll( STRING(pev->message) );
	float nextThink = LoadTime() - MessageTime();
	if ( nextThink > 0 ) 
	{
		pev->nextthink = gpGlobals->time + nextThink;
		SetThink( &CRevertSaved::LoadThink );
	}
	else
		LoadThink();
}


void CRevertSaved :: LoadThink( void )
{
	if ( !gpGlobals->deathmatch )
	{
		SERVER_COMMAND("reload\n");
	}
}





//MODDDMIRROR
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//==========================================================
// player marker for right mirroring a player in env_mirror
//==========================================================

class CPlayerMarker : public CBaseEntity
{
public:
	void Spawn( void );
	void Precache ( void );
};

//NOTICE - the player marker is no longer physically necessary. It only exists to force the rendered to render at least once while
//         the mirror is in view, but that may even already happen regardless. This can't hurt.
//         In short, null.mdl is no longer required.
LINK_ENTITY_TO_CLASS( player_marker, CPlayerMarker );

void CPlayerMarker :: Spawn( void )
{
	Precache();
	//SET_MODEL( ENT(pev), "models/null.mdl" );
	SET_MODEL( ENT(pev), "models/player.mdl" );

	ALERT(at_aiconsole, "DEBUG: Player_marker coordinates is %f %f %f \n", pev->origin.x, pev->origin.y, pev->origin.z);
	
	//this->pev->renderfx = FX_DUMMY;
	this->pev->renderfx = kRenderFxDummy;

	//MODDD
	//pev->effects |= 128;

	//pev->effects |= 128;
}


void CPlayerMarker :: Precache( void )
{
	//PRECACHE_MODEL( "models/null.mdl" );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






//=========================================================
// Multiplayer intermission spots.
//=========================================================
class CInfoIntermission:public CPointEntity
{
	void Spawn( void );
	void Think( void );
};

void CInfoIntermission::Spawn( void )
{
	UTIL_SetOrigin( pev, pev->origin );
	pev->solid = SOLID_NOT;
	pev->effects = EF_NODRAW;
	pev->v_angle = g_vecZero;

	pev->nextthink = gpGlobals->time + 2;// let targets spawn!

}

void CInfoIntermission::Think ( void )
{
	edict_t *pTarget;

	// find my target
	pTarget = FIND_ENTITY_BY_TARGETNAME( NULL, STRING(pev->target) );

	if ( !FNullEnt(pTarget) )
	{
		pev->v_angle = UTIL_VecToAngles( (pTarget->v.origin - pev->origin).Normalize() );
		pev->v_angle.x = -pev->v_angle.x;
	}
}






LINK_ENTITY_TO_CLASS( info_intermission, CInfoIntermission );
















