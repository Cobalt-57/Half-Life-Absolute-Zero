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
#include "extdll.h"
#include "handgrenade.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"

//MODDD - extern
EASY_CVAR_EXTERN(cheat_minimumfiredelay)
EASY_CVAR_EXTERN(handGrenadePickupYieldsOne);

LINK_ENTITY_TO_CLASS( weapon_handgrenade, CHandGrenade );


//NOTICE: this file does not include the projectile.  See "ShootTimed" above called by "PrimaryAttack".
//        Looks like the projectile is still a generic GGrenade object with a hand grenade model just like MP5
//        grenades.

CHandGrenade::CHandGrenade(void){
	//NEW VAR.  If -500, we are cheating.  Better for syncing.
	m_fireState = 0;

	//Used to be a var called "replayDeploy".  If on, at the next idle-call, it will first play the deploy (AKA draw) anim first to bring the grenade into view.
	//NOTE: is a bitmask.  
	// 1 (2 to the 0th) is for "replayDeploy", a watered-down version.
	// 2 (2 to the 1st) is for "weaponRetired", which re-does the "DefaultDeploy" on call.
	m_fInAttack = 0;
}

//MODDD
void CHandGrenade::customAttachToPlayer(CBasePlayer *pPlayer ){
	m_pPlayer->SetSuitUpdate("!HEV_GRENADE", FALSE, SUIT_NEXT_IN_30MIN);
}

void CHandGrenade::Spawn( )
{
	Precache( );
	m_iId = WEAPON_HANDGRENADE;
	SET_MODEL(ENT(pev), "models/w_grenade.mdl");


	//MODDD - Aha!  I spot a mistake.
	// 'pev->dmg' is never effective for the equippable player weapons themselves, only the spawned
	// projectiles. CHandGrenade is the weapon, not the projectile.
	// There is no custom Hand Grenade projectile, we only use a generic CGrenade.
	/*
#ifndef CLIENT_DLL
	pev->dmg = gSkillData.plrDmgHandGrenade;
#endif
	*/
	

	/*
                               ...----....
                         ..-:"''         ''"-..
                      .-'                      '-.
                    .'              .     .       '.
                  .'   .          .    .      .    .''.
                .'  .    .       .   .   .     .   . ..:.
              .' .   . .  .       .   .   ..  .   . ....::.
             ..   .   .      .  .    .     .  ..  . ....:IA.
            .:  .   .    .    .  .  .    .. .  .. .. ....:IA.
           .: .   .   ..   .    .     . . .. . ... ....:.:VHA.
           '..  .  .. .   .       .  . .. . .. . .....:.::IHHB.
          .:. .  . .  . .   .  .  . . . ...:.:... .......:HIHMM.
         .:.... .   . ."::"'.. .   .  . .:.:.:II;,. .. ..:IHIMMA
         ':.:..  ..::IHHHHHI::. . .  ...:.::::.,,,. . ....VIMMHM
        .:::I. .AHHHHHHHHHHAI::. .:...,:IIHHHHHHMMMHHL:. . VMMMM
       .:.:V.:IVHHHHHHHMHMHHH::..:" .:HIHHHHHHHHHHHHHMHHA. .VMMM.
       :..V.:IVHHHHHMMHHHHHHHB... . .:VPHHMHHHMMHHHHHHHHHAI.:VMMI
       ::V..:VIHHHHHHMMMHHHHHH. .   .I":IIMHHMMHHHHHHHHHHHAPI:WMM
       ::". .:.HHHHHHHHMMHHHHHI.  . .:..I:MHMMHHHHHHHHHMHV:':H:WM
       :: . :.::IIHHHHHHMMHHHHV  .ABA.:.:IMHMHMMMHMHHHHV:'. .IHWW
       '.  ..:..:.:IHHHHHMMHV" .AVMHMA.:.'VHMMMMHHHHHV:' .  :IHWV
        :.  .:...:".:.:TPP"   .AVMMHMMA.:. "VMMHHHP.:... .. :IVAI
       .:.   '... .:"'   .   ..HMMMHMMMA::. ."VHHI:::....  .:IHW'
       ...  .  . ..:IIPPIH: ..HMMMI.MMMV:I:.  .:ILLH:.. ...:I:IM
     : .   .'"' .:.V". .. .  :HMMM:IMMMI::I. ..:HHIIPPHI::'.P:HM.
     :.  .  .  .. ..:.. .    :AMMM IMMMM..:...:IV":T::I::.".:IHIMA
     'V:.. .. . .. .  .  .   'VMMV..VMMV :....:V:.:..:....::IHHHMH
       "IHH:.II:.. .:. .  . . . " :HB"" . . ..PI:.::.:::..:IHHMMV"
        :IP""HHII:.  .  .    . . .'V:. . . ..:IH:.:.::IHIHHMMMMM"
        :V:. VIMA:I..  .     .  . .. . .  .:.I:I:..:IHHHHMMHHMMM
        :"VI:.VWMA::. .:      .   .. .:. ..:.I::.:IVHHHMMMHMMMMI
        :."VIIHHMMA:.  .   .   .:  .:.. . .:.II:I:AMMMMMMHMMMMMI
        :..VIHIHMMMI...::.,:.,:!"I:!"I!"I!"V:AI:VAMMMMMMHMMMMMM'
        ':.:HIHIMHHA:"!!"I.:AXXXVVXXXXXXXA:."HPHIMMMMHHMHMMMMMV
          V:H:I:MA:W'I :AXXXIXII:IIIISSSSSSXXA.I.VMMMHMHMMMMMM
            'I::IVA ASSSSXSSSSBBSBMBSSSSSSBBMMMBS.VVMMHIMM'"'
             I:: VPAIMSSSSSSSSSBSSSMMBSSSBBMMMMXXI:MMHIMMI
            .I::. "H:XIIXBBMMMMMMMMMMMMMMMMMBXIXXMMPHIIMM'
            :::I.  ':XSSXXIIIIXSSBMBSSXXXIIIXXSMMAMI:.IMM
            :::I:.  .VSSSSSISISISSSBII:ISSSSBMMB:MI:..:MM
            ::.I:.  ':"SSSSSSSISISSXIIXSSSSBMMB:AHI:..MMM.
            ::.I:. . ..:"BBSSSSSSSSSSSSBBBMMMB:AHHI::.HMMI
            :..::.  . ..::":BBBBBSSBBBMMMB:MMMMHHII::IHHMI
            ':.I:... ....:IHHHHHMMMMMMMMMMMMMMMHHIIIIHMMV"
              "V:. ..:...:.IHHHMMMMMMMMMMMMMMMMHHHMHHMHP'
                ':. .:::.:.::III::IHHHHMMMMMHMHMMHHHHM"
                 "::....::.:::..:..::IIIIIHHHHMMMHHMV"
                   "::.::.. .. .  ...:::IIHHMMMMHMV"
                     "V::... . .I::IHHMMV"'
                       '"VHVHHHAHHHHMMV:"'
	*/
	//source: http://www.ascii-art.de/ascii/s/skull.txt

	
	//if(EASY_CVAR_GET(handGrenadePickupYieldsOne) == 1)

	// Apparently, this method can be called before the player is present (and, even if so, hasn't made this pick-upable part of the inventory yet).
	// So, just make "m_iDefaultAmmo" a non-zero value and determine what to give in an overridden "ExtractAmmo" method.
	m_iDefaultAmmo = 1;
	
	FallInit();// get ready to fall down.
}

//overridden.
int CHandGrenade::ExtractAmmo( CBasePlayerWeapon *pWeapon ){
	int		iReturn;

	if ( pszAmmo1() != NULL )
	{
		// blindly call with m_iDefaultAmmo. It's either going to be a value or zero. If it is zero,
		// we only get the ammo in the weapon's clip, which is what we want. 
		

		//easyPrintLine(PLAYER NULL? %d", m_pPlayer == NULL);
		//MODDD - NOTE - uh, "m_pPlayer" is null here, but in the method, "AddPrimaryAmmo", it is not null.
		//This is some quantum-mechanics BS.

		//MODDD - Intercept.
		//...this time, having a non-zero ammo value means, not picked up yet.  The method is overridden for this class and will handle how much to give the player based on a CVar.
		if(m_iDefaultAmmo != 0){
			iReturn = pWeapon->AddPrimaryAmmo( m_iDefaultAmmo, (char *)pszAmmo1(), iMaxClip(), iMaxAmmo1() );
		}else{
			//not sure what else is really expected.
			iReturn = FALSE;
		}
		
		
		m_iDefaultAmmo = 0;
	}

	if ( pszAmmo2() != NULL )
	{
		iReturn = pWeapon->AddSecondaryAmmo( 0, (char *)pszAmmo2(), iMaxAmmo2() );
	}

	return iReturn;

}


BOOL CHandGrenade :: AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry )
{
	int iIdAmmo;

	//This is the real intervention.  The grenade gives only "one" grenade if the CVar is being used ( = 1), and 5 otherwise (retail value).
	if(m_pPlayer && EASY_CVAR_GET(handGrenadePickupYieldsOne) == 1 ){
		iCount = 1;
	}else{
		iCount = HANDGRENADE_DEFAULT_GIVE;
	}

	//easyPrintLine("PLAYER NULL 2?? %d", m_pPlayer == NULL);

	if (iMaxClip < 1)
	{
		m_iClip = -1;
		iIdAmmo = m_pPlayer->GiveAmmo( iCount, szName, iMaxCarry );
	}
	else if (m_iClip == 0)
	{
		int i;
		i = min( m_iClip + iCount, iMaxClip ) - m_iClip;
		m_iClip += i;
		iIdAmmo = m_pPlayer->GiveAmmo( iCount - i, szName, iMaxCarry );
	}
	else
	{
		iIdAmmo = m_pPlayer->GiveAmmo( iCount, szName, iMaxCarry );
	}
	
	// m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] = iMaxCarry; // hack for testing

	if (iIdAmmo > 0)
	{
		m_iPrimaryAmmoType = iIdAmmo;
		if (m_pPlayer->HasPlayerItem( this ) )
		{
			// play the "got ammo" sound only if we gave some ammo to a player that already had this gun.
			// if the player is just getting this gun for the first time, DefaultTouch will play the "picked up gun" sound for us.
			
			playAmmoPickupSound();
			//EMIT_SOUND(ENT(pev), CHAN_ITEM, "items/9mmclip1.wav", 1, ATTN_NORM);

		}
	}

	return iIdAmmo > 0 ? TRUE : FALSE;
}



void CHandGrenade::Precache( void )
{
	PRECACHE_MODEL("models/w_grenade.mdl");
	PRECACHE_MODEL("models/v_grenade.mdl");
	PRECACHE_MODEL("models/p_grenade.mdl");

	precacheGunPickupSound();
	precacheAmmoPickupSound(); //since this same entity doubles as ammo.

}

int CHandGrenade::GetItemInfo(ItemInfo *p)
{
	p->pszName = STRING(pev->classname);
	p->pszAmmo1 = "Hand Grenade";
	p->iMaxAmmo1 = HANDGRENADE_MAX_CARRY;
	p->pszAmmo2 = NULL;
	p->iMaxAmmo2 = -1;
	p->iMaxClip = WEAPON_NOCLIP;
	p->iSlot = 4;
	p->iPosition = 0;
	p->iId = m_iId = WEAPON_HANDGRENADE;
	p->iWeight = HANDGRENADE_WEIGHT;
	p->iFlags = ITEM_FLAG_LIMITINWORLD | ITEM_FLAG_EXHAUSTIBLE;

	return 1;
}


BOOL CHandGrenade::Deploy( )
{
	m_flReleaseThrow = -1;
	return DefaultDeploy( "models/v_grenade.mdl", "models/p_grenade.mdl", HANDGRENADE_DRAW, "crowbar" );
}

BOOL CHandGrenade::CanHolster( void )
{
	// can only holster hand grenades when not primed!
	return ( m_flStartThrow == 0 );
}

void CHandGrenade::ItemPreFrame(){
	CBasePlayerWeapon::ItemPreFrame();

	//if(m_pPlayer->cheat_minimumfiredelayMem == 1){
	if(EASY_CVAR_GET(cheat_minimumfiredelay) == 1){
		//cheating.
		m_fireState = -500;
	}else{
		if(m_fireState == -500){
			m_fireState = 0;
		}
	}
}

void CHandGrenade::Holster( int skiplocal /* = 0 */ )
{
	//m_pPlayer->m_flNextAttack = UTIL_WeaponTimeBase() + 0.5;

	m_fInAttack = 0; //remove both bits.

	//MODDD NEW - safe?
	m_flStartThrow = 0;

	if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] )
	{
		//SendWeaponAnim( HANDGRENADE_HOLSTER );
		DefaultHolster(HANDGRENADE_HOLSTER, skiplocal, 0, (16.0f/30.0f));
	}
	else
	{
		// no more grenades!
		m_pPlayer->pev->weapons &= ~(1<<WEAPON_HANDGRENADE);
		SetThink( &CBasePlayerItem::DestroyItem );
		pev->nextthink = gpGlobals->time + 0.1;
	}

	EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "common/null.wav", 1.0, ATTN_NORM);
}//END OF Holster


void CHandGrenade::PrimaryAttack()
{
	m_fInAttack &= (~1);

	//if the player is not cheating:
	if(m_fireState != -500){
		//MODDD
		// this method is called anytime primary fire is held down.
		// So check to see if the grenade should explode in the user's hands (held down too long)
		// ...nope!  Pineapple grenades don't explode in your hands.  But still do instantly on being
		// thrown after being held for too long. That wasn't an oversight this time, go figure.
		
		if (m_flStartThrow && gpGlobals->time >= m_flStartThrow + 3) {
			// Held down to long, explode at player origin!

#ifndef CLIENT_DLL
			//UTIL_MakeVectors(m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle);
			SimpleStaticExplode(m_pPlayer->GetGunPosition(), gSkillData.plrDmgHandGrenade, m_pPlayer);
#endif

			//m_fInAttack &= (~1);
			m_flReleaseThrow = 0;
			m_flStartThrow = 0;
			m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
			m_flTimeWeaponIdle = m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;// ensure that the animation can finish playing
			return;
		}
		

		//easyForcePrintLine("ARE YOU hello %d, %d", m_flStartThrow, m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ]);

		//"!m_flStartThrow"? Just say ==0 for fuck's sake.
		if ( m_flStartThrow==0 && m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] > 0 )
		{
			m_flStartThrow = gpGlobals->time;
			m_flReleaseThrow = 0;

			SendWeaponAnim( HANDGRENADE_PINPULL );
			//MODDD NOTE: does this really warrant a " + randomIdleAnimationDelay()" ?
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
		}
	}else{
		// CHEATING.
		m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + m_pPlayer->cheat_minimumfiredelaycustomMem;// ensure that the animation can finish playing

		//m_flTimeWeaponIdle = m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;// ensure that the animation can finish playing


		float flVel = 220 + 6 * 140;

		Vector angThrow = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;
		if ( angThrow.x < 0 )
			angThrow.x = -10 + angThrow.x * ( ( 90 - 10 ) / 90.0 );
		else
			angThrow.x = -10 + angThrow.x * ( ( 90 + 10 ) / 90.0 );

		//MODDD NOOOOOOOOOOOOOOOOOOOOOOOO
		//if ( flVel > 500 )
		//	flVel = 500;

		UTIL_MakeVectors( angThrow );
		Vector vecSrc = m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16;
		Vector vecThrow = gpGlobals->v_forward * flVel + m_pPlayer->pev->velocity;

		CGrenade::ShootTimed( m_pPlayer->pev, vecSrc, vecThrow, gSkillData.plrDmgHandGrenade, 0.3f );
	}//END OF cheat check
}//END OF PrimaryAttack



void CHandGrenade::WeaponIdle( void )
{
	/*
	//the retire-redeploy-on-ammo-check does not care if the idle delay is over yet or not.
	if(m_fInAttack & 2 && m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] > 0){

		
		DefaultDeploy( "models/v_grenade.mdl", "models/p_grenade.mdl", HANDGRENADE_DRAW, "crowbar" );
		
		//nah, defaultdeploy does this ok.
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase();

		//m_fInAttack &= (~1);
		//it is okay to set wholly to 0.  Forget about the scheduled "just-show-deploy-anim" if the full call from retirement is used.
		m_fInAttack = 0;
		return;
	}
	*/
	
	//MODDD - is this okay for grenades?
	if(m_pPlayer->pev->viewmodel == iStringNull){
		if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] > 0 ){

			globalflag_muteDeploySound = TRUE;
			Deploy();
			globalflag_muteDeploySound = FALSE;

			return;
		}
	}


	if ( m_flReleaseThrow == 0 && m_flStartThrow )
		 m_flReleaseThrow = UTIL_WeaponTimeBase();//gpGlobals->time;  //UTIL_WeaponTimeBase()????

	if ( m_flTimeWeaponIdle > UTIL_WeaponTimeBase() )
		return;

	//schedule to show the draw anim following throwing a grenade.
	if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] > 0 ){
		if(m_fInAttack & 1){
			SendWeaponAnim( HANDGRENADE_DRAW );
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (16.0/30.0) + randomIdleAnimationDelay();
			m_fInAttack &= (~1);
			return;
		}
	}
	

	//if m_fireState == -500, we are cheating.  No release-fire needed; it is instant.
	if ( m_flStartThrow && m_fireState != -500)
	{
		Vector angThrow = m_pPlayer->pev->v_angle + m_pPlayer->pev->punchangle;

		if ( angThrow.x < 0 )
			angThrow.x = -10 + angThrow.x * ( ( 90 - 10 ) / 90.0 );
		else
			angThrow.x = -10 + angThrow.x * ( ( 90 + 10 ) / 90.0 );


		float timeSinceThrow = gpGlobals->time - m_flStartThrow;

		float timeUntilBoom = 3 - timeSinceThrow;
		if (timeUntilBoom < 0) timeUntilBoom = 0;

		//MODDD - flVel now depends on the time held down.
		
		//short, medium, long?
		//NOTE: all anim times shortened (cut off from the end) due to there being time where the screen is just blank during the anim.
		if(timeSinceThrow < 1){
			SendWeaponAnim( HANDGRENADE_THROW1 );
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (19.0/13.0);
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + ((5.0 + 3.25)/13.0);
			m_fInAttack |= 1;
		}else if(timeSinceThrow < 2){
			SendWeaponAnim( HANDGRENADE_THROW2 );
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (19.0/20.0);
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + ((7.0 + 5.0) /20.0);
			m_fInAttack |= 1;
		}else{ // timeSinceThrow < 3
			SendWeaponAnim( HANDGRENADE_THROW3 );
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (19.0/30.0);
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + ((13.0 + 6.5) /30.0);
			m_fInAttack |= 1;
		}

		//float flVel = ( 90 - angThrow.x ) * 4;
		float flVel = 220 + timeSinceThrow * 140;
		//MODDD NOOOOOOOOOOOOOOOOOOOOOOOO why??
		//if ( flVel > 500 )
		//	flVel = 500;

		easyPrintLine("HAND GRENADE VELOCITY + THROW ANG: %.3f, %.3f", flVel, ((float) (( 90 - angThrow.x ) * 4))  );

		UTIL_MakeVectors( angThrow );

		Vector vecSrc = m_pPlayer->pev->origin + m_pPlayer->pev->view_ofs + gpGlobals->v_forward * 16;
		Vector vecThrow = gpGlobals->v_forward * flVel + m_pPlayer->pev->velocity;

		CGrenade::ShootTimed( m_pPlayer->pev, vecSrc, vecThrow, gSkillData.plrDmgHandGrenade, timeUntilBoom);
		
		//MODDD - section removed.   Odd, while testing, I could not trigger "THROW3" at all.
		//time
		/*
		if ( flVel < 500 )
		{
			SendWeaponAnim( HANDGRENADE_THROW1 );
			easyPrintLine("HAND GRENADE THROW1");
		}
		else if ( flVel < 1000 )
		{
			SendWeaponAnim( HANDGRENADE_THROW2 );
			easyPrintLine("HAND GRENADE THROW2");
		}
		else
		{
			SendWeaponAnim( HANDGRENADE_THROW3 );
			easyPrintLine("HAND GRENADE THROW3");
		}
		*/

		/*
		//Randomize animation instead.
		//MODDD - new way of showing grenade toss animations: just randomize animations.
		//MODDD - section also removed.  Using hold time -> distance -> animation instead.
		float flRand = RANDOM_FLOAT(0,1);
		if(flRand <= 0.333){
			//EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORM);
			SendWeaponAnim( HANDGRENADE_THROW1 );
		}else if(flRand <= 0.666){
			//EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/dbarrel1.wav", 0.8, ATTN_NORM);
			SendWeaponAnim( HANDGRENADE_THROW2 );
		}else{
			//EMIT_SOUND(ENT(m_pPlayer->pev), CHAN_WEAPON, "weapons/cbar_hit1.wav", 0.8, ATTN_NORM);
			SendWeaponAnim( HANDGRENADE_THROW3 );
		}
		*/

		// player "shoot" animation
		m_pPlayer->SetAnimation( PLAYER_ATTACK1 );

		m_flReleaseThrow = 0;
		m_flStartThrow = 0;
		m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;
		//MODDD - handle idle-delay above depending on the chosen anim's time.
		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;

		//MODDD - cheat check.
		if(m_pPlayer->cheat_infiniteclipMem == 0 && m_pPlayer->cheat_infiniteammoMem == 0){
			m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ]--;
		}

		//MODDD - no point to this section anymore.  WeaponIdles and the NextAttacks are set above already.
		/*
		if ( !m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] )
		{
			// just threw last grenade
			// set attack times in the future, and weapon idle in the future so we can see the whole throw
			// animation, weapon idle will automatically retire the weapon for us.
			m_flTimeWeaponIdle = m_flNextSecondaryAttack = m_flNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.5;// ensure that the animation can finish playing
		}
		*/
		return;
	}

	//MODDD - never observed as m_flReleaseThrow never goes above 0?  Well, whoops.
	/*
	else if ( m_flReleaseThrow > 0 )
	{
		// we've finished the throw, restart.
		m_flStartThrow = 0;

		if ( m_pPlayer->m_rgAmmo[ m_iPrimaryAmmoType ] )
		{
			SendWeaponAnim( HANDGRENADE_DRAW );
		}
		else
		{
			RetireWeapon();
			//"2" means, a full re-deploy is required, not just sending the anim.
			m_fInAttack |= 2;
			return;
		}

		//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );
		m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (15.0 / 30.0) + randomIdleAnimationDelay();
		

		m_flReleaseThrow = -1;
		return;
	}
	*/

	if ( m_pPlayer->m_rgAmmo[m_iPrimaryAmmoType] )
	{
		int iAnim;
		float flRand = UTIL_SharedRandomFloat( m_pPlayer->random_seed, 0, 1 );
		if (flRand <= 0.75)
		{
			iAnim = HANDGRENADE_IDLE;
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat( m_pPlayer->random_seed, 10, 15 );// how long till we do this again.
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (90.0 / 30.0) + randomIdleAnimationDelay();
		
		}
		else 
		{
			iAnim = HANDGRENADE_FIDGET;
			//m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + 75.0 / 30.0;
			m_flTimeWeaponIdle = UTIL_WeaponTimeBase() + (75.0 / 30.0) + randomIdleAnimationDelay();
		}
		
		SendWeaponAnim( iAnim );
	}
}



