
#ifndef SHOTGUN_H
#define SHOTGUN_H

#include "extdll.h"
#include "weapons.h"



// special deathmatch shotgun spreads
#define VECTOR_CONE_DM_SHOTGUN	Vector( 0.08716, 0.04362, 0.00  )// 10 degrees by 5 degrees
#define VECTOR_CONE_DM_DOUBLESHOTGUN Vector( 0.17365, 0.04362, 0.00 ) // 20 degrees by 5 degrees

enum shotgun_e {
	SHOTGUN_IDLE = 0,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP,
	SHOTGUN_START_RELOAD,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_IDLE4,
	SHOTGUN_IDLE_DEEP
};



class CShotgun : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int	Save(CSave& save);
	int	Restore(CRestore& restore);
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	//MODDD - new
	CShotgun(void);

	BOOL reloadBlockFireCheck(BOOL isPrimary);
	void reloadFinishPump(void);
	BOOL reloadSemi(void);

	void Spawn(void);
	void Precache(void);
	int iItemSlot() { return 3; }
	int GetItemInfo(ItemInfo* p);


	//MODDD
	void customAttachToPlayer(CBasePlayer* pPlayer);

	int AddToPlayer(CBasePlayer* pPlayer);

	//MODDD - new.
	void ItemPreFrame(void);
	void ItemPostFrame(void);

	void PrimaryAttack(void);
	void SecondaryAttack(void);
	void FireShotgun(BOOL isPrimary);

	//MODDD	
	void Holster(int skiplocal = 0);

	BOOL Deploy();
	void Reload(void);
	void WeaponIdle(void);
	int m_fInReload;
	float m_flNextReload;
	int m_iShell;

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usDoubleFire;
	unsigned short m_usSingleFire;
};


#endif //SHOTGUN_H