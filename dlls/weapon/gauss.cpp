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

#pragma once

#include "extdll.h"
#include "gauss.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"

#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "shake.h"
#include "gamerules.h"

#include "util_debugdraw.h"




// Bunch of stuff moved to gauss.h for including things needed by here and ev_hldm.


//NOTES ON VARS (from before the revert, may not be relevant anymore):
//m_fireState   -  the mode that the animation uses for detecting "phase" (stage towards showing the full spin anim, as opposed to the pre-delay and showing the start spin anim).
//m_flStartThrow - delay required for the charge animation to start playing
//m_flReleaseThrow  -  alternate time recording since charging.  "Charging" for a very short amount of time
//     registers as a "tap" (primary fire) instead, since (quake style) one mouse button is used for both primary
//     (tap) and secondary (hold down) attacks.


EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo)

EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil)
EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP)

EASY_CVAR_EXTERN_CLIENTSENDOFF_BROADCAST(gauss_mode)


#ifdef CLIENT_DLL
extern int g_irunninggausspred;
#endif



LINK_ENTITY_TO_CLASS(weapon_gauss, CGauss);




CGauss::CGauss(void) {

}//END OF CGauss constructor




// Save/restore for serverside only!
#ifndef CLIENT_DLL
TYPEDESCRIPTION	CGauss::m_SaveData[] =
{
	DEFINE_FIELD(CGauss, m_fInAttack, FIELD_INTEGER),
	//	DEFINE_FIELD( CGauss, m_flStartCharge, FIELD_TIME ),
	//	DEFINE_FIELD( CGauss, m_flPlayAftershock, FIELD_TIME ),
	//	DEFINE_FIELD( CGauss, m_flNextAmmoBurn, FIELD_TIME ),
	DEFINE_FIELD(CGauss, m_fPrimaryFire, FIELD_BOOLEAN),
};
IMPLEMENT_SAVERESTORE(CGauss, CBasePlayerWeapon);
#endif




float CGauss::GetFullChargeTime(void)
{
	if(IsMultiplayer())
	{
		return 1.5;
	}

	return 4;
}



void CGauss::customAttachToPlayer(CBasePlayer* pPlayer) {
	m_pPlayer->SetSuitUpdate("!HEV_GAUSS", FALSE, SUIT_NEXT_IN_30MIN, 4.3f);
}


void CGauss::Spawn()
{
	Precache();
	m_iId = WEAPON_GAUSS;
	SET_MODEL(ENT(pev), "models/w_gauss.mdl");

	m_iDefaultAmmo = GAUSS_DEFAULT_GIVE;

	FallInit();// get ready to fall down.
}


void CGauss::Precache(void)
{
	PRECACHE_MODEL("models/w_gauss.mdl");
	PRECACHE_MODEL("models/v_gauss.mdl");
	PRECACHE_MODEL("models/p_gauss.mdl");

	PRECACHE_SOUND("items/9mmclip1.wav");

	PRECACHE_SOUND("weapons/gauss2.wav");
	PRECACHE_SOUND("weapons/electro4.wav");
	PRECACHE_SOUND("weapons/electro5.wav");
	PRECACHE_SOUND("weapons/electro6.wav");
	PRECACHE_SOUND("ambience/pulsemachine.wav");

	precacheGunPickupSound();

	m_iGlow = PRECACHE_MODEL("sprites/hotglow.spr");
	m_iBalls = PRECACHE_MODEL("sprites/hotglow.spr");
	m_iBeam = PRECACHE_MODEL("sprites/smoke.spr");

	m_usGaussFire = PRECACHE_EVENT(1, "events/gauss.sc");
	m_usGaussSpin = PRECACHE_EVENT(1, "events/gaussspin.sc");
}

int CGauss::AddToPlayer(CBasePlayer* pPlayer)
{
	if (CBasePlayerWeapon::AddToPlayer(pPlayer))
	{
		MESSAGE_BEGIN(MSG_ONE, gmsgWeapPickup, NULL, pPlayer->pev);
		WRITE_BYTE(m_iId);
		MESSAGE_END();
		return TRUE;
	}
	return FALSE;
}

int CGauss::GetItemInfo(ItemInfo* p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "uranium";
	p->iMaxAmmo1 = URANIUM_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 3;

	//MODDD - now 0. RPG moved out. It used to take islot 3, iPosition 0.
	//p->iPosition = 1;
	p->iPosition = 0;

	p->iId = m_iId = WEAPON_GAUSS;
	p->iFlags = 0;
	p->iWeight = GAUSS_WEIGHT;

	return 1;
}

BOOL CGauss::Deploy()
{
	m_pPlayer->m_flPlayAftershock = 0.0;
	BOOL depResult = DefaultDeploy("models/v_gauss.mdl", "models/p_gauss.mdl", GAUSS_DRAW, "gauss", 0, 0, (36.0f + 1.0f) / (64.0f), -1);

	// undo the 'BlockLooping' that DefaultDeploy calls for in this case.
	stopBlockLooping();

	return depResult;
}

void CGauss::Holster(int skiplocal /* = 0 */)
{
	// NOTE - does this just stop sounds?
	PLAYBACK_EVENT_FULL(FEV_RELIABLE | FEV_GLOBAL, m_pPlayer->edict(), m_usGaussFire, 0.01, (float*)&m_pPlayer->pev->origin, (float*)&m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1);

	m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

	SendWeaponAnim(GAUSS_HOLSTER);
	m_fInAttack = 0;


	DefaultHolster(GAUSS_HOLSTER, skiplocal, 0, (31.0f + 1.0f) / (60.0f));
}


void CGauss::PrimaryAttack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		PlayEmptySound();
		// MODDD - changed from 0.15
		SetAttackDelays(UTIL_WeaponTimeBase() + 0.5);
		return;
	}

	if (PlayerPrimaryAmmoCount() < 2)
	{
		PlayEmptySound();
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
		return;
	}

	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;
	m_fPrimaryFire = TRUE;

	//MODDD - only reduce ammo if cheats are off.
	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip) == 0 && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo) == 0) {

		if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST(gauss_mode) != 1) {
			ChangePlayerPrimaryAmmoCount(-2);
		}
		else{
			// pre-release mode.  adjust damage to compensate later.
			ChangePlayerPrimaryAmmoCount(-5);
		}
	}


	StartFire();
	m_fInAttack = 0;
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;


	if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0) {
		// And, time between primary attacks changed from 0.2 to 0.25.  Time to secondary lengthened further.
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.25f;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.45f;

		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.25;
	}
	else {
		m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom);
	}
}

void CGauss::SecondaryAttack()
{
	// don't fire underwater
	if (m_pPlayer->pev->waterlevel == 3)
	{
		if (m_fInAttack != 0)
		{
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", 1.0, ATTN_NORM, 0, 80 + RANDOM_LONG(0, 0x3f));
			SendWeaponAnim(GAUSS_IDLE);
			m_fInAttack = 0;
		}
		else
		{
			PlayEmptySound();
		}

		SetAttackDelays(UTIL_WeaponTimeBase() + 0.5);
		return;
	}

	if (m_fInAttack == 0)
	{
		if (PlayerPrimaryAmmoCount() <= 0)
		{
			EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;
			return;
		}







		m_fPrimaryFire = FALSE;

		if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteclip) == 0 && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_infiniteammo) == 0) {
			ChangePlayerPrimaryAmmoCount(-1);// take one ammo just to start the spin
		}




		//TODO - MINOR.  Any reason why holding down both primary & secondary while charging causes the fired shot
		// to be invisible? or some odd combo, I forget.  If easy though.

		if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelay) == 0) {
			// proceed with usual charging.
		}
		else {
			// fire now, max damage!
			m_pPlayer->m_flStartCharge = -20;  //make it think this is always a full charge.

			StartFire();
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;

			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_minimumfiredelaycustom);
			return;
		}







		m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase();

		// spin up
		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		SendWeaponAnim(GAUSS_SPINUP);
		m_fInAttack = 1;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
		m_pPlayer->m_flStartCharge = gpGlobals->time;
		m_pPlayer->m_flAmmoStartCharge = UTIL_WeaponTimeBase() + GetFullChargeTime();

		PLAYBACK_EVENT_FULL(FEV_NOTHOST, m_pPlayer->edict(), m_usGaussSpin, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, 0.0, 0.0, 110, 0, 0, 0);

		m_iSoundState = SND_CHANGE_PITCH;
	}
	else if (m_fInAttack == 1)
	{
		if (m_flTimeWeaponIdle < UTIL_WeaponTimeBase())
		{
			SendWeaponAnim(GAUSS_SPIN);
			m_fInAttack = 2;
		}
	}
	else
	{
		// during the charging process, eat one bit of ammo every once in a while
		if (UTIL_WeaponTimeBase() >= m_pPlayer->m_flNextAmmoBurn && m_pPlayer->m_flNextAmmoBurn != 1000)
		{
			if(IsMultiplayer())
			{
				ChangePlayerPrimaryAmmoCount(-1);
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.1;
			}
			else
			{
				ChangePlayerPrimaryAmmoCount(-1);
				m_pPlayer->m_flNextAmmoBurn = UTIL_WeaponTimeBase() + 0.3;
			}
		}

		if (PlayerPrimaryAmmoCount() <= 0)
		{
			// out of ammo! force the gun to fire
			StartFire();
			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1;

			//MODDD - why not?
			SetAttackDelays(UTIL_WeaponTimeBase() + 1);

			return;
		}

		if (UTIL_WeaponTimeBase() >= m_pPlayer->m_flAmmoStartCharge)
		{
			// don't eat any more ammo after gun is fully charged.
			m_pPlayer->m_flNextAmmoBurn = 1000;
		}

		int pitch = (gpGlobals->time - m_pPlayer->m_flStartCharge) * (150 / GetFullChargeTime()) + 100;
		if (pitch > 250)
			pitch = 250;

		// ALERT( at_console, "%d %d %d\n", m_fInAttack, m_iSoundState, pitch );

		if (m_iSoundState == 0)
			ALERT(at_console, "sound state %d\n", m_iSoundState);

		PLAYBACK_EVENT_FULL(FEV_NOTHOST, m_pPlayer->edict(), m_usGaussSpin, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, 0.0, 0.0, pitch, 0, (m_iSoundState == SND_CHANGE_PITCH) ? 1 : 0, 0);

		m_iSoundState = SND_CHANGE_PITCH; // hack for going through level transitions

		m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_CHARGE_VOLUME;

		// m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.1;
		if (m_pPlayer->m_flStartCharge < gpGlobals->time - 10)
		{
			// Player charged up too long. Zap him.
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", 1.0, ATTN_NORM, 0, 80 + RANDOM_LONG(0, 0x3f));
			EMIT_SOUND_DYN(ENT(m_pPlayer->pev), CHAN_ITEM, "weapons/electro6.wav", 1.0, ATTN_NORM, 0, 75 + RANDOM_LONG(0, 0x3f));

			m_fInAttack = 0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.0;
			m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 1.0;

#ifndef CLIENT_DLL
			m_pPlayer->TakeDamage(VARS(eoNullEntity), VARS(eoNullEntity), 50, DMG_SHOCK);
			UTIL_ScreenFade(m_pPlayer, Vector(255, 128, 0), 2, 0.5, 128, FFADE_IN);
#endif
			SendWeaponAnim(GAUSS_IDLE);

			// Player may have been killed and this weapon dropped, don't execute any more code after this!
			return;
		}
	}
}

//=========================================================
// StartFire- since all of this code has to run and then 
// call Fire(), it was easier at this point to rip it out 
// of weaponidle() and make its own function then to try to
// merge this into Fire(), which has some identical variable names 
//=========================================================
void CGauss::StartFire(void)
{
	float flDamage;

	UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
	Vector vecAiming = gpGlobals->v_forward;
	Vector vecSrc = m_pPlayer->GetGunPosition(); // + gpGlobals->v_up * -8 + gpGlobals->v_right * 8;

	if (gpGlobals->time - m_pPlayer->m_flStartCharge > GetFullChargeTime())
	{
		flDamage = 200;
	}
	else
	{
		flDamage = 200 * ((gpGlobals->time - m_pPlayer->m_flStartCharge) / GetFullChargeTime());
	}



	if (m_fPrimaryFire)
	{
		// fixed damage on primary attack
#ifdef CLIENT_DLL
		flDamage = 20;
#else 
		flDamage = gSkillData.plrDmgGauss;
#endif
	}

	if (m_fInAttack != 3)
	{
		//ALERT ( at_console, "Time:%f Damage:%f\n", gpGlobals->time - m_pPlayer->m_flStartCharge, flDamage );

#ifndef CLIENT_DLL
		float flZVel = m_pPlayer->pev->velocity.z;

		if (!m_fPrimaryFire)
		{
			//MODDD - a cheat may disable the recoil force.
			if (EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(cheat_nogaussrecoil) == 0) {
				m_pPlayer->pev->velocity = m_pPlayer->pev->velocity - gpGlobals->v_forward * flDamage * 5;
			}
		}
		//MODDD - the 2nd condition is extra, from a new CVar.  Could allow z-force.
		if (!IsMultiplayer() && EASY_CVAR_GET_CLIENTSENDOFF_BROADCAST_DEBUGONLY(gaussRecoilSendsUpInSP) == 0)
		{
			// in deathmatch, gauss can pop you up into the air. Not in single play.
			m_pPlayer->pev->velocity.z = flZVel;
		}
#endif
		// player "shoot" animation
		m_pPlayer->SetAnimation(PLAYER_ATTACK1);
	}

	// time until aftershock 'static discharge' sound
	m_pPlayer->m_flPlayAftershock = gpGlobals->time + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 0.3, 0.8);


	//MODDD - is that good?
	m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (31.0 / 30.0) + randomIdleAnimationDelay();


	Fire(vecSrc, vecAiming, flDamage);
}

void CGauss::Fire(Vector vecOrigSrc, Vector vecDir, float flDamage)
{
	m_pPlayer->m_iWeaponVolume = GAUSS_PRIMARY_FIRE_VOLUME;

	Vector vecSrc = vecOrigSrc;
	Vector vecDest = vecSrc + vecDir * 8192;
	edict_t* pentIgnore;
	TraceResult tr, beam_tr;
	float flMaxFrac = 1.0;
	int	nTotal = 0;
	int fHasPunched = 0;
	int fFirstBeam = 1;
	int	nMaxHits = 10;

	pentIgnore = ENT(m_pPlayer->pev);

#ifdef CLIENT_DLL
	if (m_fPrimaryFire == false)
		g_irunninggausspred = true;
#endif

	// The main firing event is sent unreliably so it won't be delayed.
	PLAYBACK_EVENT_FULL(FEV_NOTHOST, m_pPlayer->edict(), m_usGaussFire, 0.0, (float*)&m_pPlayer->pev->origin, (float*)&m_pPlayer->pev->angles, flDamage, 0.0, 0, 0, m_fPrimaryFire ? 1 : 0, 0);

	// This reliable event is used to stop the spinning sound
	// It's delayed by a fraction of second to make sure it is delayed by 1 frame on the client
	// It's sent reliably anyway, which could lead to other delays

	PLAYBACK_EVENT_FULL(FEV_NOTHOST | FEV_RELIABLE, m_pPlayer->edict(), m_usGaussFire, 0.01, (float*)&m_pPlayer->pev->origin, (float*)&m_pPlayer->pev->angles, 0.0, 0.0, 0, 0, 0, 1);


	/*ALERT( at_console, "%f %f %f\n%f %f %f\n",
		vecSrc.x, vecSrc.y, vecSrc.z,
		vecDest.x, vecDest.y, vecDest.z );*/


		//	ALERT( at_console, "%f %f\n", tr.flFraction, flMaxFrac );

#ifndef CLIENT_DLL
	while (flDamage > 10 && nMaxHits > 0)
	{
		nMaxHits--;

		// ALERT( at_console, "." );
		UTIL_TraceLine(vecSrc, vecDest, dont_ignore_monsters, pentIgnore, &tr);


		if (tr.fAllSolid) {
			break;
		}

		//MODDD - moved above the 'pEntity NULL'-check below.
		// What does hitting something or not have to do with making a muzzle flash?
		if (fFirstBeam)
		{
			m_pPlayer->pev->effects |= EF_MUZZLEFLASH;
			fFirstBeam = 0;

			nTotal += 26;
		}

		// MODDD - added check for flFraction like FireBulletsPlayer does.  I assume that had a point.
		if (tr.flFraction >= 1.0) {
			break;
		}

		CBaseEntity* pEntity = CBaseEntity::Instance(tr.pHit);

		if (pEntity == NULL) {
			break;
		}
		///////////////////////////////////////////////////

		//MODDD - no need for this check, the world knows not to take damage.
		// Methods with TraceAttack methods will reject damage just like ones called by FirePlayerBullets do,
		// which doesn't even do a ThingHit->pev->takedamage check itself anyway
		//if (pEntity->pev->takedamage)
		{
			BOOL useBulletHitSound = TRUE;
			ClearMultiDamage();

			//MODDD - NEW.
			// This is an "AI Sound", or not a real one audible to the player, but one that checks for monsters nearby (distance) and alerts them if they are in hearing range.
			// TODO - egon can get this too, probably.
			attemptSendBulletSound(tr.vecEndPos, m_pPlayer->pev);

			const char* theName = pEntity->getClassname();

			pEntity->TraceAttack(m_pPlayer->pev, flDamage, vecDir, &tr, DMG_BULLET, DMG_GAUSS, TRUE, &useBulletHitSound);


			//MODDD - Play a texture-hit sound, it is a bullet after all.
			// And just force a bullet type to MP5 here, point is it's not the crowbar
			// NOTICE - just use FirePlayerBullets at this point for better support, whether to do texture-sounds or decals
			// server or clientside is a little more comlpex than just tacking it on here.
			// Entities can make noise this way though, just not the world under default CVars.
			if (useBulletHitSound) {
				TEXTURETYPE_PlaySound(&tr, vecSrc, vecDest, BULLET_PLAYER_MP5);
				DecalGunshot(&tr, BULLET_PLAYER_MP5);
			}

			ApplyMultiDamage(m_pPlayer->pev, m_pPlayer->pev);
		}


		//MODDD NOTE
		// Notice that the default (barely ever touched) "ReflectGauss" requires the thing hit (pEntity) to not be capable of taking damage (not an enemy or breakable).
		// If this is the case (incapable of taking dmg), it is reflectable and a check is done to see whether to reflect off the thing hit (enough degrees) or attempt
		// a punch thru (secondary gauss fire can go some distance through walls).
		// OTHERWISE, capable of taking DMG, this assumes that the beam hit a monster for "pEntity", and dealt damage to it.  Don't try to reflect, just try to go right
		// through.  Notice that the pEntity just hit is set to be the "ignore" entity as to not hit the same one again.  Setting the vecSrc (source) to be the hit
		// location (of the enemy struck) a slight ways forwards for the next line-trace at the next run-thru of the loop (where it may hit another enemy or hit a surface
		// as expected) is also a good sign of this idea.
		// ...Can block the reflection this early, preventing the "punch through" (go through a wall), or allow that much.


		if (pEntity->ReflectGauss())
		{
			float n;

			//MODDD - easy there!  Don't set pentIgnore to NULL so soon, do it if the surface hit warrants a reflection
			// (hit coming at enough of an angle).  Otherwise, the punch-through check that runs instead will
			// find the player at the start of the trace and go 'Oh look, something to punch through.  DAMGE.'
			//pentIgnore = NULL;

			n = -DotProduct(tr.vecPlaneNormal, vecDir);


			// for now, always try.
			BOOL reflectCheckPossible = TRUE;




			if (reflectCheckPossible && n < 0.5) // 60 degrees
			{
				// ALERT( at_console, "reflect %f\n", n );
				// reflect
				Vector r;

				//MODDD - should be safe now
				pentIgnore = NULL;

				r = 2.0 * tr.vecPlaneNormal * n + vecDir;
				flMaxFrac = flMaxFrac - tr.flFraction;
				vecDir = r;
				vecSrc = tr.vecEndPos + vecDir * 8;
				vecDest = vecSrc + vecDir * 8192;

				// explode a bit
			//MODDD NOTE - why was this "m_pPlayer->RadiusDamage"? No need to be specific to the player in the call,
			//             we already send our own PEV and the player's PEV as arguments.
			// No difference in just "RadiusDamage( ... )" alone.
			//m_pPlayer->RadiusDamage(...)
			// Also, Damange no longer depends on the reflection angle, was 'flDamage * n'.
			// 
				RadiusDamage(tr.vecEndPos, pev, m_pPlayer->pev, flDamage * 0.42, flDamage * 1.4, CLASS_NONE, DMG_BLAST, DMG_GAUSS);

				nTotal += 34;

				// lose energy
				if (n == 0) n = 0.1;
				flDamage = flDamage * (1 - n);
			}
			else
			{
				nTotal += 13;

				// limit it to one hole punch
				if (fHasPunched)
					break;
				fHasPunched = 1;


				BOOL punchAttempt = !m_fPrimaryFire;

				// try punching through wall if secondary attack (primary is incapable of breaking through)
				if (punchAttempt)
				{
					UTIL_TraceLine(tr.vecEndPos + vecDir * 8, vecDest, dont_ignore_monsters, pentIgnore, &beam_tr);
					if (!beam_tr.fAllSolid)
					{
						// trace backwards to find exit point
						UTIL_TraceLine(beam_tr.vecEndPos, tr.vecEndPos, dont_ignore_monsters, pentIgnore, &beam_tr);
						
						//MODDD - 'n' renamed to 'm' to a void a scope conflict
						float m = (beam_tr.vecEndPos - tr.vecEndPos).Length();

						if (m < flDamage)
						{
							if (m == 0) m = 1;
							flDamage -= m;

							// ALERT( at_console, "punch %f\m", m );
							nTotal += 21;

							//MODDD - redone.
							/*
							// exit blast damage
							//m_pPlayer->RadiusDamage( beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage, CLASS_NONE, DMG_BLAST );
							float damage_radius;

							if ( IsMultiplayer() )
							{
								damage_radius = flDamage * 1.75;  // Old code == 2.5
							}
							else
							{
								damage_radius = flDamage * 2.5;
							}

							::RadiusDamage( beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage, damage_radius, CLASS_NONE, DMG_BLAST, DMG_GAUSS );
							*/
							::RadiusDamage(beam_tr.vecEndPos + vecDir * 8, pev, m_pPlayer->pev, flDamage * 0.38, flDamage * 1.4, CLASS_NONE, DMG_BLAST, DMG_GAUSS);


							CSoundEnt::InsertSound(bits_SOUND_COMBAT, pev->origin, NORMAL_EXPLOSION_VOLUME, 3.0);

							nTotal += 53;

							vecSrc = beam_tr.vecEndPos + vecDir;
						}
						else {
							//MODDD - NEW.
							// So couldn't punch through something in the way.  Why do the loop again?
							// Nothing about vecSrc nor vecDest changed, so the same trace is being done again.
							// Except this time, with a NULL pentIgnore.  Redoing the trace from the player's weapon
							// to where the player is looking is likely to register a hit on the player from occuring 
							// within the player's bounds without being told to ignore that.
							flDamage = 0;
						}
					}
					else
					{
						//ALERT( at_console, "blocked %f\n", n );
						flDamage = 0;
					}
				}
				else
				{
					//ALERT( at_console, "blocked solid\n" );

					flDamage = 0;
				}

				//MODDD - now it is safe to turn this off.
				pentIgnore = NULL;

			}
		}
		else
		{
			BOOL canPierce = TRUE;

			if (canPierce) {

				vecSrc = tr.vecEndPos + vecDir;
				pentIgnore = ENT(pEntity->pev);

			}
			else {
				break;  //end
			}
		}
	}
#endif
	// ALERT( at_console, "%d bytes\n", nTotal );
}




void CGauss::WeaponIdle(void)
{
	ResetEmptySound();

	// play aftershock static discharge
	if (m_pPlayer->m_flPlayAftershock && m_pPlayer->m_flPlayAftershock < gpGlobals->time)
	{
		switch (RANDOM_LONG(0, 3))
		{
		case 0:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro4.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 1:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro5.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 2:	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/electro6.wav", RANDOM_FLOAT(0.7, 0.8), ATTN_NORM); break;
		case 3:	break; // no sound
		}
		m_pPlayer->m_flPlayAftershock = 0.0;
	}

	if (m_flTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;

	if (m_fInAttack != 0)
	{
		StartFire();
		m_fInAttack = 0;
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 2.0;

		//MODDD - why not?
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.4;
		m_flNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.6;
	}
	else
	{
		int iAnim;
		float flRand = RANDOM_FLOAT(0, 1);
		if (flRand <= 0.5)
		{
			iAnim = GAUSS_IDLE;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
		else if (flRand <= 0.75)
		{
			iAnim = GAUSS_IDLE2;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(m_pPlayer->random_seed, 10, 15);
		}
		else
		{
			iAnim = GAUSS_FIDGET;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (71.0f + 1.0f) / (30.0f);
		}

		//MODDD - 'return' found here, as-is.   Always stopped?  Why?
		//return;
		SendWeaponAnim(iAnim);

	}
}






class CGaussAmmo : public CBasePlayerAmmo
{
	void Spawn(void)
	{
		Precache();
		SET_MODEL(ENT(pev), "models/w_gaussammo.mdl");
		CBasePlayerAmmo::Spawn();
	}
	void Precache(void)
	{
		PRECACHE_MODEL("models/w_gaussammo.mdl");
		precacheAmmoPickupSound();
	}
	BOOL AddAmmo(CBaseEntity* pOther)
	{
		if (pOther->GiveAmmo(AMMO_URANIUMBOX_GIVE, "uranium", URANIUM_MAX_CARRY) != -1)
		{
			playAmmoPickupSound();

			//MODDD - Could not find any sentences or clips involving Gauss ammo, specifically (uranium).
			//Perhaps this is okay for both the Gauss and the Egon?
			//UPDATE: apparently, the Gauss's ammo is the universal, and the egon's ammo is just the
			//mp5 chain ammo (seems really unfitting, I think it was a placeholder).
			if (pOther->IsPlayer()) {
				CBasePlayer* pPlayer = (CBasePlayer*)pOther;
				pPlayer->SetSuitUpdate("!HEV_EGONPOWER", FALSE, SUIT_NEXT_IN_20MIN);
			}

			return TRUE;
		}
		return FALSE;
	}
};
LINK_ENTITY_TO_CLASS(ammo_gaussclip, CGaussAmmo);



