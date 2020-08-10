// NEW FILE, for methods implemented clientside (and not dummied) from hl_weapons.cpp.
// Note that there is no separate "cl_player" entity, this is just named to avoid matching player.cpp.
// Some includes are odd with same names, different folders.  A lot never specify folder anyway.


#include "hl_weapons.h"

// includes copied from player.cpp
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
#include "util_debugdraw.h"
// only included to see what some default AI schedules are such as "slSmallFlinsh" for another
// monster.
#include "dlls/defaultai.h"

#include "../hud_iface.h"
#include "../com_weapons.h"



//MODDD - default some stuff dangit!
CBasePlayer::CBasePlayer(void) {

	m_pLastItem = NULL;
	m_pQueuedActiveItem = NULL;

	m_pActiveItemCLIENTHISTORY = NULL;
	m_flNextAttackCLIENTHISTORY = -1;
}


//MODDD - uncertain if this should still be dummied.  Probably.
int CBasePlayer::GiveAmmo(int iCount, const char* szName, int iMax) {
	return 0;
}
int CBasePlayer::GiveAmmoID(int iCount, int iAmmoTypeId, int iMax) {
	return 0;
}


//MODDD - several other dummied methods moved over, undummy as needed.
BOOL CBasePlayer::AddPlayerItem(CBasePlayerItem* pItem) { return FALSE; }
BOOL CBasePlayer::RemovePlayerItem(CBasePlayerItem* pItem) { return FALSE; }
void CBasePlayer::ItemPreFrame() { }
void CBasePlayer::ItemPostFrame() { }

// Woa, that looks nice!
// Keep in mind, there is already an entire clientside-only weapons framework between hud/ammo.h/cpp and weapons_resource.h/cpp.
// The things stored in the local player may or may not synch up nicely with that in some/all respects.
void CBasePlayer::printOutWeapons(void) {

	for (int i = 0; i < 6; i++) {
		int i2 = 0;
		CBasePlayerItem* thisItem = m_rgpPlayerItems[i];
		while (thisItem) {

			easyForcePrintLine("slot:%d row:%d %s", i, i2, STRING(thisItem->pev->classname));
			i2++;
			thisItem = thisItem->m_pNext;
		}//END OF while(...)
	}//END OF for(...)

};

BOOL CBasePlayer::CanAddPlayerItem(int arg_iItemSlot, const char* arg_classname, const char* arg_ammoname, int arg_iMaxAmmo) { return FALSE; };



//MODDD - undummied.
int CBasePlayer::AmmoInventory(int iAmmoIndex)
{
	if (iAmmoIndex == -1)
	{
		return -1;
	}

	return m_rgAmmo[iAmmoIndex];
}

//MODDD - copied over.
void CBasePlayer::TabulateAmmo()
{
	ammo_9mm = AmmoInventory(AmmoIndex_9mm);
	ammo_357 = AmmoInventory(AmmoIndex_357);
	ammo_argrens = AmmoInventory(AmmoIndex_ARgrenades);
	ammo_bolts = AmmoInventory(AmmoIndex_bolts);
	ammo_buckshot = AmmoInventory(AmmoIndex_buckshot);
	ammo_rockets = AmmoInventory(AmmoIndex_rockets);
	ammo_uranium = AmmoInventory(AmmoIndex_uranium);
	ammo_hornets = AmmoInventory(AmmoIndex_Hornets);
}






//MODDD - IMPORTANT. Apparently this may as well have been dummied, but was found in the as-is script.
//        This method is never called for the client, any vars set by the server calling SelectItem on
//        its own side need to be sent to the client.
void CBasePlayer::SelectItem(const char* pstr)
{
	//easyPrintLine("MESSAGE11");
	if (!pstr)
		return;

	CBasePlayerItem* pItem = NULL;

	if (!pItem)
		return;


	if (pItem == m_pActiveItemCLIENTHISTORY)
		return;

	/*
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Holster( );

	m_pLastItem = m_pActiveItemCLIENTHISTORY;
	m_pActiveItemCLIENTHISTORY = pItem;

	if (m_pActiveItemCLIENTHISTORY)
	{
		m_pActiveItemCLIENTHISTORY->Deploy( );
	}
	*/

	easyForcePrintLine("$$$$ AHA I SHOULD NEVER EVER HAPPEN, REPORT ME IF I DO YOU HEAR?! $$$$");
}//END OF SelectItem


void CBasePlayer::setActiveItem(CBasePlayerItem* argItem) {

	m_pLastItem = m_pActiveItemCLIENTHISTORY;
	m_pActiveItemCLIENTHISTORY = argItem;

	if (m_pActiveItemCLIENTHISTORY)
	{
		m_pActiveItemCLIENTHISTORY->Deploy();
		m_pActiveItemCLIENTHISTORY->UpdateItemInfo();
	}
}


//MODDD - ... wait.  This is never called either.  OOPS..  
// FIXED, is now at least.
void CBasePlayer::setActiveItem_HolsterCheck(CBasePlayerItem* argItem) {

	// ******SCRIPT THIS REPLACES, exact or similar-intent repeated a few places
	/*
	// FIX, this needs to queue them up and delay
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Holster( );

	m_pLastItem = m_pActiveItemCLIENTHISTORY;
	m_pActiveItemCLIENTHISTORY = pItem;

	if (m_pActiveItemCLIENTHISTORY)
	{
		m_pActiveItemCLIENTHISTORY->Deploy( );
		m_pActiveItemCLIENTHISTORY->UpdateItemInfo( );
	}
	*/
	///////////////////////////////////////////////////////////////////////////


	//MODDD - if the weapon isn't selectable, why bother?   BAIL.
	if (!argItem->CanDeploy())
	{
		return;
	}


	if (m_pActiveItemCLIENTHISTORY) {
		//easyForcePrintLine("OH yeah AM I HOLSTERIN ALREADY %d", m_bHolstering);

		if (!m_bHolstering) {
			// don't holster the currently equipped weapon if already in the middle of holstering.
			m_pActiveItemCLIENTHISTORY->Holster();
			m_bHolstering = TRUE;
		}

		// cl_holster
		// and access through the CVar instead in this version!
		//if (fHolsterAnimsEnabled == TRUE) {
		if (EASY_CVAR_GET(cl_holster) != 0) {
			// using holster anim? Tell the currently equipped item to change to this weapon when that is over.
			m_pQueuedActiveItem = argItem;  //set this later instead, after the holster anim is done.
		}
		else {
			// Not using holster anims? Immediately change weapon.
			setActiveItem(argItem);
			m_bHolstering = FALSE;
		}

	}
	else {
		// just pick it now.
		setActiveItem(argItem);
	}

}







//MODDD - ... wait.  This is never called either.  OOPS.
void CBasePlayer::SelectLastItem(void)
{
	//easyPrintLine("MESSAGE12");
	if (!m_pLastItem)
	{
		return;
	}

	if (m_pActiveItemCLIENTHISTORY && !m_pActiveItemCLIENTHISTORY->CanHolster())
	{
		return;
	}

	//MODDD - disabled, replaced with the _HolsterCheck call below
	/*
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Holster( );

	CBasePlayerItem *pTemp = m_pActiveItemCLIENTHISTORY;
	m_pActiveItemCLIENTHISTORY = m_pLastItem;
	m_pLastItem = pTemp;
	m_pActiveItemCLIENTHISTORY->Deploy( );
	*/


	setActiveItem_HolsterCheck(m_pLastItem);
}








//MODDD NOTE.
// And If wanted, remove these from hl_baseentity.cpp and implement them here.
//GENERATE_DEADTAKEDAMAGE_IMPLEMENTATION_DUMMY_CLIENT(CBasePlayer)
//GENERATE_GIBMONSTER_IMPLEMENTATION_DUMMY_CLIENT(CBasePlayer)


GENERATE_KILLED_IMPLEMENTATION(CBasePlayer)
{
	// Holster weapon immediately, to allow it to cleanup
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Holster();

	g_irunninggausspred = false;

}

//MODDD - NEW.
void CBasePlayer::onDelete(void) {
	// ???  anything we want to do here?
}





/*
=====================
CBasePlayer::Spawn

=====================
*/
void CBasePlayer::Spawn(void)
{
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Deploy();

	g_irunninggausspred = false;
}
//MODDD - for the alt version, if ever called, do the same.  Doesn't involve that BOOL.
void CBasePlayer::Spawn(BOOL revived)
{
	if (m_pActiveItemCLIENTHISTORY)
		m_pActiveItemCLIENTHISTORY->Deploy();

	g_irunninggausspred = false;
}



void CBasePlayer::Activate(void)
{
	//um, what.

}


// other dummied methods.
BOOL CBasePlayer::HasPlayerItem(CBasePlayerItem* pCheckItem) { return FALSE; }

// should this be dummied? ever get called clientside anyway?
// Seems it isn't.  oh well.
BOOL CBasePlayer::SwitchWeapon(CBasePlayerItem* pWeapon) {
	if (!pWeapon->CanDeploy())
	{
		return FALSE;
	}

	ResetAutoaim();

	setActiveItem_HolsterCheck(pWeapon);
	return TRUE;
}

Vector CBasePlayer::GetGunPosition(void) { return g_vecZero; }
Vector CBasePlayer::GetGunPositionAI(void) { return g_vecZero; }
const char* CBasePlayer::TeamID(void) { return ""; }

void CBasePlayer::AddPoints(int score, BOOL bAllowNegativeScore) { }
void CBasePlayer::AddPointsToTeam(int score, BOOL bAllowNegativeScore) { }

BOOL CBasePlayer::usesSoundSentenceSave(void) { return FALSE; }


