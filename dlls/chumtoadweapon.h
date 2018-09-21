


#ifndef CHUMTOADWEAPON_H
#define CHUMTOADWEAPON_H



#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"
//
#include "basemonster.h"




enum chumtoadweapon_e {  //key: frames, FPS
	CHUMTOADWEAPON_IDLE1 = 0, //31, 16
	CHUMTOADWEAPON_FIDGETLICK, //31, 16
	CHUMTOADWEAPON_FIDGETCROAK, //51, 16
	CHUMTOADWEAPON_DOWN, //21, 21
	CHUMTOADWEAPON_UP, //36, 36
	CHUMTOADWEAPON_THROW, //36, 24

};

//how far into the throw animation to do another check and spawn the chumtoad.
#define CHUMTOAD_THROW_DELAY (20.0f / 24.0f)





class CChumToadWeapon : public CBasePlayerWeapon{



    public:
	BOOL waitingForChumtoadThrow;
	float chumtoadThrowReverseDelay;
	int antiGravityPositionY;


	//MODDD
	CChumToadWeapon(void);
	BOOL usesSoundSentenceSave(void);


	//what. the. fuck.
	BOOL m_fInAttack;




	void EXPORT FallThinkCustom ( void );
	const char* GetPickupWalkerName(void);
	void Spawn( void );
	//void AttachToPlayer ( CBasePlayer *pPlayer );
	void FallInit( void );
	void EXPORT ItemRotate ( void );
	void Precache( void );
	void setModel(void);
	void setModel(const char* m);
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	BOOL checkThrowValid(Vector trace_origin, float* minFractionStore);
	void ThrowChumtoad(Vector vecSpawnPoint);

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );

	float randomIdleAnimationDelay(void);
	//MODDD - new
	void ItemPostFrame(void);

	void WeaponIdle( void );
	int m_fJustThrown;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usChumToadFire;
	static int numberOfEyeSkins;

};//END OF CChumToadWeapon



#endif //END OF #ifdef CHUMTOADWEAPON_H