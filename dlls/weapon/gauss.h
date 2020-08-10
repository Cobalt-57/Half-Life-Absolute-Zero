
#ifndef GAUSS_H
#define GAUSS_H

#include "weapons.h"


#define GAUSS_PRIMARY_CHARGE_VOLUME	256// how loud gauss is while charging
#define GAUSS_PRIMARY_FIRE_VOLUME	450// how loud gauss is when discharged

enum gauss_e {
	GAUSS_IDLE = 0,
	GAUSS_IDLE2,
	GAUSS_FIDGET,
	GAUSS_SPINUP,
	GAUSS_SPIN,
	GAUSS_FIRE,
	GAUSS_FIRE2,
	GAUSS_HOLSTER,
	GAUSS_DRAW
};




class CGauss : public CBasePlayerWeapon
{
public:

	int ohMySon;
	int ohDearSonny;

	//MOTHERappreciater HOW DO YOU NOT HAVE THIS
	CGauss(void);

#ifndef CLIENT_DLL
	int	Save(CSave& save);
	int	Restore(CRestore& restore);
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn(void);
	void Precache(void);
	int iItemSlot(void) { return 4; }
	int GetItemInfo(ItemInfo* p);

	//MODDD
	void customAttachToPlayer(CBasePlayer* pPlayer);

	int AddToPlayer(CBasePlayer* pPlayer);

	BOOL Deploy(void);
	void Holster(int skiplocal = 0);


	//MODDD - new.
	void ItemPreFrame(void);
	void ItemPostFrame(void);

	void PrimaryAttack(void);
	void SecondaryAttack(void);


	//MODDD - new
	void chargeGauss(void);
	float getNextAmmoBurnDelay(void);
	void determineNextAmmoBurnAndUse(void);
	void chargeWork(void);
	void postChargeAnimCheck(void);
	void attemptFirePrimary(void);

	float getAmmoChargeInterval(void);

	void WeaponIdle(void);

	//MODDD - new
	void StartFireDecision(void);

	void StartFire(void);
	void Fire(Vector vecOrigSrc, Vector vecDirShooting, float flDamage);
	float GetFullChargeTime(void);
	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState; // don't save this

	// was this weapon just fired primary or secondary?
	// we need to know so we can pick the right set of effects. 
	BOOL m_fPrimaryFire;

	virtual BOOL UseDecrement(void)
	{
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usGaussFire;
	unsigned short m_usGaussSpin;
};



#endif //GAUSS_H