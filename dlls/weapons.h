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
#ifndef WEAPONS_H
#define WEAPONS_H


#include "cbase.h"  //why not?!
#include "animating.h"

#include "basemonster.h"

#include "effects.h"

class CBasePlayer;
extern int gmsgWeapPickup;

void DeactivateSatchels( CBasePlayer *pOwner );

// Contact Grenade / Timed grenade / Satchel Charge
class CGrenade : public CBaseMonster
{
public:

	//MODDD - added for compatibility.
	//Actually, not necessary.
	//void Activate( void );

	void Spawn( void );

	typedef enum { SATCHEL_DETONATE = 0, SATCHEL_RELEASE } SATCHELCODE;

	static CGrenade *ShootTimed( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity, float time );
	static CGrenade *ShootContact( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity );
	static CGrenade *ShootSatchelCharge( entvars_t *pevOwner, Vector vecStart, Vector vecVelocity );
	static void UseSatchelCharges( entvars_t *pevOwner, SATCHELCODE code );

	//MODDD - parameters are ignored, removed.
	//void Explode( Vector vecSrc, Vector vecAim );


	//MODD - why do we even need these?
	GENERATE_TRACEATTACK_PROTOTYPE_VIRTUAL
	GENERATE_TAKEDAMAGE_PROTOTYPE_VIRTUAL


	void Explode();
	void Explode( TraceResult *pTrace, int bitsDamageType );
	void Explode( TraceResult *pTrace, int bitsDamageType, int bitsDamageTypeMod );
	void Explode( TraceResult *pTrace, int bitsDamageType, int bitsDamageTypeMod, float shrapMod );

	void EXPORT Smoke( void );

	void EXPORT BounceTouch( CBaseEntity *pOther );
	void EXPORT SlideTouch( CBaseEntity *pOther );
	void EXPORT ExplodeTouch( CBaseEntity *pOther );
	void EXPORT DangerSoundThink( void );
	void EXPORT PreDetonate( void );
	void EXPORT Detonate( void );
	void EXPORT DetonateUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	void EXPORT TumbleThink( void );

	virtual void BounceSound( void );
	virtual int	BloodColor( void ) { return DONT_BLEED; }

	virtual float massInfluence(void);
	virtual int GetProjectileType(void);
	
	GENERATE_KILLED_PROTOTYPE_VIRTUAL
	//virtual void Killed( entvars_t *pevAttacker, int iGib );

	virtual BOOL isOrganic(void);
	virtual BOOL usesSoundSentenceSave(void);



	BOOL m_fRegisteredSound;// whether or not this grenade has issued its DANGER sound to the world sound list yet.
};


// constant items
#define ITEM_HEALTHKIT		1
#define ITEM_ANTIDOTE		2
#define ITEM_SECURITY		3
#define ITEM_BATTERY		4

//MODDD - added for new syringes
#define ITEM_ADRENALINE		5
#define ITEM_RADIATION		6

#define WEAPON_NONE				0
#define WEAPON_CROWBAR			1
#define	WEAPON_GLOCK			2
#define WEAPON_PYTHON			3
#define WEAPON_MP5				4
#define WEAPON_CHAINGUN			5  //WOA HOLD ON NOW!!! CHAINGUN? WOO.    definitely cut or never went forward with it though.
#define WEAPON_CROSSBOW			6
#define WEAPON_SHOTGUN			7
#define WEAPON_RPG				8
#define WEAPON_GAUSS			9
#define WEAPON_EGON				10
#define WEAPON_HORNETGUN		11
#define WEAPON_HANDGRENADE		12
#define WEAPON_TRIPMINE			13
#define	WEAPON_SATCHEL			14
#define	WEAPON_SNARK			15
//MODDD - new
#define WEAPON_CHUMTOAD			16


#define WEAPON_ALLWEAPONS		(~(1<<WEAPON_SUIT))

#define WEAPON_SUIT				31	// ?????

#define MAX_WEAPONS			32

//MODDD - Stored in const.h instead.  Coordination with two separate constants not necessary, refer to the same one either place.
//MODDD - coordinate with Player.cpp's long jump charge!
//#define PLAYER_LONGJUMPCHARGE_MAX 100
//#define PLAYER_LONGJUMP_PICKUPADD 25

#define MAX_NORMAL_BATTERY	100


// weapon weight factors (for auto-switching)   (-1 = noswitch)
#define CROWBAR_WEIGHT		0
#define GLOCK_WEIGHT		10
#define PYTHON_WEIGHT		15
#define MP5_WEIGHT			15
#define SHOTGUN_WEIGHT		15
#define CROSSBOW_WEIGHT		10
#define RPG_WEIGHT			20
#define GAUSS_WEIGHT		20
#define EGON_WEIGHT			20
#define HORNETGUN_WEIGHT	10
#define HANDGRENADE_WEIGHT	5
#define SNARK_WEIGHT		5
#define SATCHEL_WEIGHT		-10
#define TRIPMINE_WEIGHT		-10
//MODDD
#define CHUMTOAD_WEIGHT		5 


// weapon clip/carry ammo capacities
#define URANIUM_MAX_CARRY		100

//MODD - was 150.
#define	_9MM_MAX_CARRY			120
#define _357_MAX_CARRY			36
#define BUCKSHOT_MAX_CARRY		125
#define BOLT_MAX_CARRY			50
#define ROCKET_MAX_CARRY		5
#define HANDGRENADE_MAX_CARRY	20
#define SATCHEL_MAX_CARRY		5
#define TRIPMINE_MAX_CARRY		5
#define SNARK_MAX_CARRY			15
#define HORNET_MAX_CARRY		8
#define M203_GRENADE_MAX_CARRY	12
//MODDD
#define CHUMTOAD_MAX_CARRY		5



// the maximum amount of ammo each weapon's clip can hold
#define WEAPON_NOCLIP			-1

//#define CROWBAR_MAX_CLIP		WEAPON_NOCLIP
#define GLOCK_MAX_CLIP			13
#define PYTHON_MAX_CLIP			6
#define MP5_MAX_CLIP			30
#define MP5_DEFAULT_AMMO		30
#define SHOTGUN_MAX_CLIP		8
#define CROSSBOW_MAX_CLIP		5
#define RPG_MAX_CLIP			1
#define GAUSS_MAX_CLIP			WEAPON_NOCLIP
#define EGON_MAX_CLIP			WEAPON_NOCLIP
#define HORNETGUN_MAX_CLIP		WEAPON_NOCLIP
#define HANDGRENADE_MAX_CLIP	WEAPON_NOCLIP
#define SATCHEL_MAX_CLIP		WEAPON_NOCLIP
#define TRIPMINE_MAX_CLIP		WEAPON_NOCLIP
#define SNARK_MAX_CLIP			WEAPON_NOCLIP


// the default amount of ammo that comes with each gun when it spawns
//MODDD - NOTE: is actually "12", but we add one in the case of first getting the weapon for the first time or reloading when not empty.  IF the CVar "glockOldReloadLogic" is on.
#define GLOCK_DEFAULT_GIVE			13

#define PYTHON_DEFAULT_GIVE			6
#define MP5_DEFAULT_GIVE			30
#define MP5_DEFAULT_AMMO			30
#define MP5_M203_DEFAULT_GIVE		0
#define SHOTGUN_DEFAULT_GIVE		12
#define CROSSBOW_DEFAULT_GIVE		5
#define RPG_DEFAULT_GIVE			1
#define GAUSS_DEFAULT_GIVE			20
#define EGON_DEFAULT_GIVE			20
#define HANDGRENADE_DEFAULT_GIVE	5
#define SATCHEL_DEFAULT_GIVE		1
#define TRIPMINE_DEFAULT_GIVE		1
#define SNARK_DEFAULT_GIVE			5
#define HIVEHAND_DEFAULT_GIVE		8
//MODDD
#define CHUMTOAD_DEFAULT_GIVE		1

// The amount of ammo given to a player by an ammo item.
#define AMMO_URANIUMBOX_GIVE	20
#define AMMO_GLOCKCLIP_GIVE		GLOCK_MAX_CLIP
#define AMMO_357BOX_GIVE		PYTHON_MAX_CLIP
#define AMMO_MP5CLIP_GIVE		MP5_MAX_CLIP
#define AMMO_CHAINBOX_GIVE		200
#define AMMO_M203BOX_GIVE		2
#define AMMO_BUCKSHOTBOX_GIVE	12
#define AMMO_CROSSBOWCLIP_GIVE	CROSSBOW_MAX_CLIP
#define AMMO_RPGCLIP_GIVE		RPG_MAX_CLIP
#define AMMO_URANIUMBOX_GIVE	20
#define AMMO_SNARKBOX_GIVE		5

// bullet types
typedef	enum
{
	BULLET_NONE = 0,
	BULLET_PLAYER_9MM, // glock
	BULLET_PLAYER_MP5, // mp5
	BULLET_PLAYER_357, // python
	BULLET_PLAYER_BUCKSHOT, // shotgun
	BULLET_PLAYER_CROWBAR, // crowbar swipe

	BULLET_MONSTER_9MM,
	BULLET_MONSTER_MP5,
	BULLET_MONSTER_12MM,
} Bullet;


#define ITEM_FLAG_SELECTONEMPTY		1
#define ITEM_FLAG_NOAUTORELOAD		2
#define ITEM_FLAG_NOAUTOSWITCHEMPTY	4
#define ITEM_FLAG_LIMITINWORLD		8
#define ITEM_FLAG_EXHAUSTIBLE		16 // A player can totally exhaust their ammo supply and lose this weapon

#define WEAPON_IS_ONTARGET 0x40

typedef struct
{
	int		iSlot;
	int		iPosition;
	const char	*pszAmmo1;	// ammo 1 type
	int		iMaxAmmo1;		// max ammo 1
	const char	*pszAmmo2;	// ammo 2 type
	int		iMaxAmmo2;		// max ammo 2
	const char	*pszName;
	int		iMaxClip;
	int		iId;
	int		iFlags;
	int		iWeight;// this value used to determine this weapon's importance in autoselection.
} ItemInfo;

typedef struct
{
	const char *pszName;
	int iId;
} AmmoInfo;

// Items that the player has in their inventory that they can use
class CBasePlayerItem : public CBaseAnimating
{
public:
	virtual void SetObjectCollisionBox( void );

	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];

	virtual int AddToPlayer( CBasePlayer *pPlayer );	// return TRUE if the item you want the item added to the player inventory
	
	
	virtual int AddDuplicate( CBasePlayerItem *pItem ) { return FALSE; }	// return TRUE if you want your duplicate removed from world
	//MODDD NOTE - not sure if this was intentional or not, but this method actually returns how much ammo the given item actually gave to the player.  As seen in weapons.cpp's implementation, if any ammo is given, remove this item.  Being "0" (no ammo given) is the same as returning FALSE, or saying not to remove the item (did not touch).
	
	//MODDD - the glock has an exception for being picked up anyways: giving the player the silencer.
	virtual BOOL weaponCanHaveExtraCheck( CBasePlayer* pPlayer){return FALSE; }
	virtual BOOL weaponPlayPickupSoundException( CBasePlayer* pPlayer){return FALSE; }


	

	void EXPORT DestroyItem( void );

	//MODDD - new
	void EXPORT DefaultTouchRemoveThink( CBaseEntity *pOther );	// default weapon touch

	void EXPORT DefaultTouch( CBaseEntity *pOther );	// default weapon touch


	void EXPORT FallThink ( void );// when an item is first spawned, this think is run to determine when the object has hit the ground.
	void EXPORT Materialize( void );// make a weapon visible and tangible
	void EXPORT AttemptToMaterialize( void );  // the weapon desires to become visible and tangible, if the game rules allow for it
	CBaseEntity* Respawn ( void );// copy a weapon
	
	//MODDD - why not virtual before???
	virtual void FallInit( void );

	void CheckRespawn( void );
	virtual int GetItemInfo(ItemInfo *p) { return 0; };	// returns 0 if struct not filled out
	virtual BOOL CanDeploy( void ) { return TRUE; };
	virtual BOOL Deploy( )								// returns is deploy was successful
		 { return TRUE; };

	virtual BOOL CanHolster( void ) { return TRUE; };// can this weapon be put away right now?
	virtual void Holster( int skiplocal = 0 );
	virtual void UpdateItemInfo( void ) { return; };

	virtual void ItemPreFrame( void )	{ return; }		// called each frame by the player PreThink
	virtual void ItemPostFrame( void ) { return; }		// called each frame by the player PostThink
	//MODDD - it appears the above two are actually not called when the player's "m_flNextAttack" var is on (delay before being able to fire again).
	//So, new methods that work regardless of that:
	virtual void ItemPreFrameThink( void )	{ return; }		// called each frame by the player PreThink, even when "m_flNextAttak" is on.
	virtual void ItemPostFrameThink( void ) { return; }		// called each frame by the player PostThink, even when "m_flNextAttak" is on.
	

	virtual void Drop( void );
	virtual void Kill( void );
	virtual void AttachToPlayer ( CBasePlayer *pPlayer );

	virtual void customAttachToPlayer( CBasePlayer *pPlayer) { };  //can default to doing nothing.

	virtual int PrimaryAmmoIndex() { return -1; };
	virtual int SecondaryAmmoIndex() { return -1; };

	virtual int UpdateClientData( CBasePlayer *pPlayer ) { return 0; }

	virtual CBasePlayerItem *GetWeaponPtr( void ) { return NULL; };

	static ItemInfo ItemInfoArray[ MAX_WEAPONS ];
	static AmmoInfo AmmoInfoArray[ MAX_AMMO_SLOTS ];

	CBasePlayer	*m_pPlayer;
	CBasePlayerItem *m_pNext;
	int		m_iId;												// WEAPON_???

	virtual int iItemSlot( void ) { return 0; }			// return 0 to MAX_ITEMS_SLOTS, used in hud

	int			iItemPosition( void ) { return ItemInfoArray[ m_iId ].iPosition; }
	const char	*pszAmmo1( void )	{ return ItemInfoArray[ m_iId ].pszAmmo1; }
	int			iMaxAmmo1( void )	{ return ItemInfoArray[ m_iId ].iMaxAmmo1; }
	const char	*pszAmmo2( void )	{ return ItemInfoArray[ m_iId ].pszAmmo2; }
	int			iMaxAmmo2( void )	{ return ItemInfoArray[ m_iId ].iMaxAmmo2; }
	const char	*pszName( void )	{ return ItemInfoArray[ m_iId ].pszName; }
	int			iMaxClip( void )	{ return ItemInfoArray[ m_iId ].iMaxClip; }
	int			iWeight( void )		{ return ItemInfoArray[ m_iId ].iWeight; }
	int			iFlags( void )		{ return ItemInfoArray[ m_iId ].iFlags; }

	// int		m_iIdPrimary;										// Unique Id for primary ammo
	// int		m_iIdSecondary;										// Unique Id for secondary ammo
};


// inventory items that 
class CBasePlayerWeapon : public CBasePlayerItem
{
public:
	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );

	static	TYPEDESCRIPTION m_SaveData[];

	//MODDD - constructor.
	CBasePlayerWeapon();
	//MODDD - new var.  Like "pev->buttons" from the player, but acconuts for whether firing with the weapon is allowed at the moment (can't be done in weapons themselves, only in weapons.cpp)
	int buttonFiltered;
	//MODDD - new.
	
	void setchargeReady(int arg);
	int getchargeReady(void);
	void forceBlockLooping(void);
	void stopBlockLooping(void);



	BOOL isBasePlayerWeapon(void){return TRUE;};



	//MODDD - new.  Get the time to add to an idle animation's delay (beyondthe bare minimum to finish the current anim of course)
	//Made virtual. For the lazy ass in you, go overwrite this to add 0 seconds for no static delays for things not meant to be lifeless like snarks or chumtoads.
	virtual float randomIdleAnimationDelay(void);


	int bothFireButtonsMode;
	//0 = can not press both at the same time (nothing happens, not even NeitherHeld).
	//1 = can not press both at the same time (NeitherHeld() is called only).
	//2 = usual behavior: only "secondaryPressed" is called, "primaryNotPressed" forced.
	//3 = same as 2, but "primaryNotPressed" is not called.
	//4 = "bothPressed" called only.
	//5 = "bothPressed" called as well as the two "not"'s.
		

	// generic weapon versions of CBasePlayerItem calls
	virtual int AddToPlayer( CBasePlayer *pPlayer );
	virtual int AddDuplicate( CBasePlayerItem *pItem );

	virtual int ExtractAmmo( CBasePlayerWeapon *pWeapon ); //{ return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up
	virtual int ExtractClipAmmo( CBasePlayerWeapon *pWeapon );// { return TRUE; };			// Return TRUE if you can add ammo to yourself when picked up

	virtual int AddWeapon( void ) { ExtractAmmo( this ); return TRUE; };	// Return TRUE if you want to add yourself to the player

	// generic "shared" ammo handlers
	//MODDD - "AddPrimaryAmmo" has been made "virtual" so that overriden methods in child classes get priority.
	virtual BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry );
	virtual BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry, BOOL forcePickupSound );
	
	BOOL AddSecondaryAmmo( int iCount, char *szName, int iMaxCarry );

	virtual void UpdateItemInfo( void ) {};	// updates HUD state

	int m_iPlayEmptySound;
	int m_fFireOnEmpty;		// True when the gun is empty and the player is still holding down the
							// attack key(s)
	virtual BOOL PlayEmptySound( void );
	virtual void ResetEmptySound( void );

	virtual void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );  // skiplocal is 1 if client is predicting weapon animations
	virtual void SendWeaponAnimReverse( int iAnim, int skiplocal = 1, int body = 0 ); 
	//MODDD - new version that works more like a direct client call... actually does server and client.
	virtual void SendWeaponAnimBypass(int iAnim, int body = 0);
	virtual void SendWeaponAnimBypassReverse(int iAnim, int body = 0);
	//THIS is a client-only call. Careful.
	virtual void SendWeaponAnimClientOnly(int iAnim, int body = 0);
	virtual void SendWeaponAnimClientOnlyReverse(int iAnim, int body = 0);

	virtual void SendWeaponAnimServerOnly(int iAnim, int body = 0);
	virtual void SendWeaponAnimServerOnlyReverse(int iAnim, int body = 0);





	
	//MODDD
	virtual const char* GetPickupWalkerName(void);


	virtual BOOL CanDeploy( void );
	virtual BOOL IsUseable( void );

	//MODDD - see implementation.
	BOOL Deploy();

	//MODDD - new parameter (optional): "delayAnimTime".  Really a custom time for starting weapon idle animations (trying to play an idle anim during a deploy will ignore the idle call, causing the deploy anim, on finishing, to just be stalled while it is IMPLIED the idle anim is playing (the delay set), but it is not (the gun is just static).
	//BOOL DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal = 0, int body = 0 );
	BOOL DefaultDeploy( char *szViewModel, char *szWeaponModel, int iAnim, char *szAnimExt, int skiplocal = 0, int body = 0, float deployAnimTime = 1.0, float fireDelayTime = 0.5 );
	
	void DefaultHolster(int iAnim, int skiplocal = 0, int body = 0, float holsterAnimTime = 1.0);



	int DefaultReload( int iClipSize, int iAnim, float fDelay, int body = 0 );




	//MODDD - new.
	virtual void ItemPostFrameThink(void);


	//MODDD - added so that "hl_weapons.cpp" in clientside may override it.  But it will have to be called over there to do anything.
	virtual void ItemPreFrame( void );

	virtual void ItemPostFrame( void );	// called each frame by the player PostThink
	// called by CBasePlayerWeapons ItemPostFrame()
	
	//MODDD
	virtual void customAttachToPlayer(CBasePlayer *pPlayer ) {};


	
	virtual void PrimaryAttack( void ) { return; }				// do "+ATTACK"
	virtual void SecondaryAttack( void ) { return; }			// do "+ATTACK2"
	virtual void Reload( void ) { return; }						// do "+RELOAD"
	virtual void WeaponIdle( void ) { return; }					// called when no buttons pressed
	virtual int UpdateClientData( CBasePlayer *pPlayer );		// sends hud info to client dll, if things have changed
	virtual void RetireWeapon( void );
	virtual BOOL ShouldWeaponIdle( void ) {return FALSE; };
	virtual void Holster( int skiplocal = 0 );
	virtual BOOL UseDecrement( void ) { return FALSE; };

	//MODDD - new
	virtual void PrimaryNotHeld( void ) { return; };
	virtual void SecondaryNotHeld( void ) { return; };
	virtual void NeitherHeld( void ) { return; };
	virtual void BothHeld( void ) { return; };
	
	int	PrimaryAmmoIndex(); 
	int	SecondaryAmmoIndex(); 

	void PrintState( void );
	
	//MODDD - new
	virtual CBaseEntity* pickupWalkerReplaceCheck();

	virtual CBasePlayerItem *GetWeaponPtr( void ) { return (CBasePlayerItem *)this; };

	float m_flPumpTime;
	int		m_fInSpecialReload;									// Are we in the middle of a reload for the shotguns
	float	m_flNextPrimaryAttack;								// soonest time ItemPostFrame will call PrimaryAttack
	float	m_flNextSecondaryAttack;							// soonest time ItemPostFrame will call SecondaryAttack
	float	m_flTimeWeaponIdle;									// soonest time ItemPostFrame will call WeaponIdle
	int		m_iPrimaryAmmoType;									// "primary" ammo index into players m_rgAmmo[]
	int		m_iSecondaryAmmoType;								// "secondary" ammo index into players m_rgAmmo[]
	int		m_iClip;											// number of shots left in the primary weapon clip, -1 it not used
	int		m_iClientClip;										// the last version of m_iClip sent to hud dll
	int		m_iClientWeaponState;								// the last version of the weapon state sent to hud dll (is current weapon, is on target)
	int		m_fInReload;										// Are we in the middle of a reload;

	int		m_iDefaultAmmo;// how much ammo you get when you pick up this weapon as placed by a level designer.
	
};


class CBasePlayerAmmo : public CBaseEntity
{
public:


	virtual void Spawn( void );
	void EXPORT DefaultTouch( CBaseEntity *pOther ); // default weapon touch
	virtual BOOL AddAmmo( CBaseEntity *pOther ) { return TRUE; };

	CBaseEntity* Respawn( void );
	void EXPORT Materialize( void );
};


extern DLL_GLOBAL	short	g_sModelIndexLaser;// holds the index for the laser beam
extern DLL_GLOBAL	const char *g_pModelNameLaser;

extern DLL_GLOBAL	short	g_sModelIndexLaserDot;// holds the index for the laser beam dot
extern DLL_GLOBAL	short	g_sModelIndexFireball;// holds the index for the fireball
extern DLL_GLOBAL	short	g_sModelIndexSmoke;// holds the index for the smoke cloud
extern DLL_GLOBAL	short	g_sModelIndexWExplosion;// holds the index for the underwater explosion
extern DLL_GLOBAL	short	g_sModelIndexBubbles;// holds the index for the bubbles model
extern DLL_GLOBAL	short	g_sModelIndexBloodDrop;// holds the sprite index for blood drops
extern DLL_GLOBAL	short	g_sModelIndexBloodSpray;// holds the sprite index for blood spray (bigger)

extern void ClearMultiDamage(void);
extern void ApplyMultiDamage(entvars_t* pevInflictor, entvars_t* pevAttacker );
extern void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType);
extern void AddMultiDamage( entvars_t *pevInflictor, CBaseEntity *pEntity, float flDamage, int bitsDamageType, int bitsDamageTypeMod );

extern void DecalGunshot( TraceResult *pTrace, int iBulletType );
extern void SpawnBlood(Vector vecSpot, int bloodColor, float flDamage);

//MODDD - extra damage bitmask support.
extern int DamageDecal( CBaseEntity *pEntity, int bitsDamageType );
extern int DamageDecal( CBaseEntity *pEntity, int bitsDamageType, int bitsDamageTypeMod );



extern void RadiusDamageTest( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod );



//MODDD - these versions moved from basemonster.h to here, weapons.h. They are inspecific to the monster called on and don't need to be for monsters.
//NOTICE: any RadiusDamage methods that don't provide flRadius have been renamed to "RadiusDamageAutoRadius" to avoid some call ambiguity (the parameters you supply could go to unintended places if say,
//        more than one overload accepts the same amount of numbers but gives them a different purpose... The computer may make a bad decision)
extern void RadiusDamageAutoRadius(Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType );
extern void RadiusDamageAutoRadius(Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod  );
//MODDD - added bitsDamageTypeMod versions.
extern void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType );
extern void RadiusDamage( Vector vecSrc, entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, float flRadius, int iClassIgnore, int bitsDamageType, int bitsDamageTypeMod );



typedef struct 
{
	CBaseEntity		*pEntity;
	float			amount;
	int				type;
	//MODDD - 2nd damge bitmask added.
	int				typeMod;
} MULTIDAMAGE;

extern MULTIDAMAGE gMultiDamage;


#define LOUD_GUN_VOLUME			1000
#define NORMAL_GUN_VOLUME		600
#define QUIET_GUN_VOLUME		200

#define	BRIGHT_GUN_FLASH		512
#define NORMAL_GUN_FLASH		256
#define	DIM_GUN_FLASH			128

#define BIG_EXPLOSION_VOLUME	2048
#define NORMAL_EXPLOSION_VOLUME	1024
#define SMALL_EXPLOSION_VOLUME	512

#define	WEAPON_ACTIVITY_VOLUME	64

#define VECTOR_CONE_1DEGREES	Vector( 0.00873, 0.00873, 0.00873 )
#define VECTOR_CONE_2DEGREES	Vector( 0.01745, 0.01745, 0.01745 )
#define VECTOR_CONE_3DEGREES	Vector( 0.02618, 0.02618, 0.02618 )
#define VECTOR_CONE_4DEGREES	Vector( 0.03490, 0.03490, 0.03490 )
#define VECTOR_CONE_5DEGREES	Vector( 0.04362, 0.04362, 0.04362 )
#define VECTOR_CONE_6DEGREES	Vector( 0.05234, 0.05234, 0.05234 )
#define VECTOR_CONE_7DEGREES	Vector( 0.06105, 0.06105, 0.06105 )
#define VECTOR_CONE_8DEGREES	Vector( 0.06976, 0.06976, 0.06976 )
#define VECTOR_CONE_9DEGREES	Vector( 0.07846, 0.07846, 0.07846 )
#define VECTOR_CONE_10DEGREES	Vector( 0.08716, 0.08716, 0.08716 )
#define VECTOR_CONE_15DEGREES	Vector( 0.13053, 0.13053, 0.13053 )
#define VECTOR_CONE_20DEGREES	Vector( 0.17365, 0.17365, 0.17365 )

//=========================================================
// CWeaponBox - a single entity that can store weapons
// and ammo. 
//=========================================================
class CWeaponBox : public CBaseEntity
{
	void Precache( void );
	void Spawn( void );
	void Touch( CBaseEntity *pOther );
	void KeyValue( KeyValueData *pkvd );
	BOOL IsEmpty( void );
	int  GiveAmmo( int iCount, char *szName, int iMax, int *pIndex = NULL );
	void SetObjectCollisionBox( void );

public:
	void EXPORT Kill ( void );
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
	
	BOOL HasWeapon( CBasePlayerItem *pCheckItem );
	BOOL PackWeapon( CBasePlayerItem *pWeapon );
	BOOL PackAmmo( int iszName, int iCount );
	
	CBasePlayerItem	*m_rgpPlayerItems[MAX_ITEM_TYPES];// one slot for each 

	int m_rgiszAmmo[MAX_AMMO_SLOTS];// ammo names
	int	m_rgAmmo[MAX_AMMO_SLOTS];// ammo quantities

	int m_cAmmoTypes;// how many ammo types packed into this box (if packed by a level designer)
};

#ifdef CLIENT_DLL
//MODDD - also externed in cl_util.h now.
extern bool bIsMultiplayer(void);

void LoadVModel ( char *szViewModel, CBasePlayer *m_pPlayer );
#endif


//NOTICE::::
//THIS IS IMPLEMENTED IN hl_wpn_glock.cpp, in the server's "wpn_shared" folder.
//Why?  no clue.

class CGlock : public CBasePlayerWeapon
{
public:

	BOOL scheduleGlockDeletion;
	BOOL includesGlockSilencer;
	//BOOL playerHasGlockYet;
	BOOL rememberOneLessRound;

	BOOL weaponCanHaveExtraCheck(CBasePlayer* pPlayer);
	BOOL weaponPlayPickupSoundException( CBasePlayer* pPlayer);

	
	void setExtraBulletFalse(void);
	void setExtraBulletTrue(void);
	BOOL getExtraBullet(void);



//MODDD - this is necessary for  saving and loading, apparently.
#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	static float getUsingGlockOldReloadLogic(void);


	//MODDD
	virtual int ExtractAmmo( CBasePlayerWeapon *pWeapon );

	BOOL nextAnimBackwards;

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 2; }
	int GetItemInfo(ItemInfo *p);

	int everSent;

	//MODDD
	CGlock();
	void customAttachToPlayer(CBasePlayer *pPlayer );
	void ItemPreFrame(void );
	void ItemPostFrame(void );

	void Holster( int skiplocal = 0 );


	void SendWeaponAnim( int iAnim, int skiplocal = 1, int body = 0 );


	void PrimaryAttack( void );
	void SecondaryAttack( void );
	
	void GlockFire( float flSpread, float flCycleTime, BOOL fUseAutoAim );
	BOOL Deploy( void );
	void Reload( void );
	void WeaponIdle( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

	
private:
	int m_iShell;
	
	float oldTime;
	float currentTime;
	float timeDelta;

	BOOL legalHoldSecondary;
	BOOL startedSecondaryHoldAttempt;
	float animationTime;
	float holdingSecondaryCurrent;
	float holdingSecondaryTarget0;
	float holdingSecondaryTarget1;
	float holdingSecondaryTarget2;

	float holdingSecondaryTarget3;
	float holdingSecondaryTarget4;
	
	float timeSinceDeployed;

	BOOL toggledSilencerYet;

	unsigned short m_usFireGlock1;
	unsigned short m_usFireGlock2;



};



class CCrowbar : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 1; }
	void EXPORT SwingAgain( void );
	void EXPORT Smack( void );
	int GetItemInfo(ItemInfo *p);

	void PrimaryAttack( void );
	int Swing( int fFirst );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	int m_iSwing;

	//MODDD
	int m_iSwingMiss;

	TraceResult m_trHit;

	//MODDD - added.
	void WeaponIdle( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
private:
	unsigned short m_usCrowbar;
};







class CLaserSpotPython : public CBaseEntity
{
	void Spawn( void );
	void Precache( void );

	int	ObjectCaps( void ) { return FCAP_DONT_SAVE; }

public:
	void Suspend( float flSuspendTime );
	void EXPORT Revive( void );
	
	static CLaserSpotPython *CreateSpot( void );
};





class CPython : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 2; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void Reload( void );

	//MODDD - added.
	void customAttachToPlayer(CBasePlayer *pPlayer );
	CPython();

	void ItemPostFrameThink(void);

	void ItemPostFrame();
	void ItemPreFrame();
	void spotDeleteCheck();
	void updateModel();
	void UpdateSpot();


	void WeaponIdle( void );

	float m_flSoundDelay;

	//MODDD - added.
	int m_fSpotActive;
	int m_iShell;
	CLaserSpotPython* m_pSpot;
	

	BOOL m_fInZoom;// don't save this. 

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usFirePython;
};

class CMP5 : public CBasePlayerWeapon
{
public:
	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 3; }
	int GetItemInfo(ItemInfo *p);
	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int SecondaryAmmoIndex( void );
	BOOL Deploy( void );
	//MODDD - new
	void Holster( int skiplocal = 0 );

	void Reload( void );
	void WeaponIdle( void );
	float m_flNextAnimTime;
	int m_iShell;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usMP5;
	unsigned short m_usMP52;
};

class CCrossbow : public CBasePlayerWeapon
{
public:

	//MODDD - new.
	CCrossbow(void);


	void Spawn( void );
	void Precache( void );
	int iItemSlot( ) { return 3; }
	int GetItemInfo(ItemInfo *p);

	void FireBolt( void );
	void FireSniperBolt( void );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );
	int AddToPlayer( CBasePlayer *pPlayer );
	BOOL Deploy( );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );

	int m_fInZoom; // don't save this
	
	//MODDD - new.
	float crossbowReloadSoundTimer;

	//MODDD - new.
	void ItemPostFrameThink(void);
	void ItemPostFrame( void );



	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usCrossbow;
	unsigned short m_usCrossbow2;
};

class CShotgun : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	//MODDD - new
	CShotgun(void);

	BOOL reloadBlockFireCheck(void);
	void reloadFinishPump(void);
	BOOL reloadSemi(void);

	void Spawn( void );
	void Precache( void );
	int iItemSlot( ) { return 3; }
	int GetItemInfo(ItemInfo *p);


	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );

	//MODDD - new.
	void ItemPreFrame( void );
	void ItemPostFrame( void );

	void PrimaryAttack( void );
	void SecondaryAttack( void );

	//MODDD	
	void Holster( int skiplocal = 0 );

	BOOL Deploy( );
	void Reload( void );
	void WeaponIdle( void );
	int m_fInReload;
	float m_flNextReload;
	int m_iShell;

	virtual BOOL UseDecrement( void )
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

class CLaserSpot : public CBaseEntity
{
	void Spawn( void );
	void Precache( void );

	int	ObjectCaps( void ) { return FCAP_DONT_SAVE; }

public:
	void Suspend( float flSuspendTime );
	void EXPORT Revive( void );
	
	static CLaserSpot *CreateSpot( void );
};

class CRpg : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	void Reload( void );

	//MODDD - moved down.
	//int iItemSlot( void ) { return 4; }
	int iItemSlot( void ) { return 3; }

	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	BOOL CanHolster( void );
	void Holster( int skiplocal = 0 );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	void WeaponIdle( void );

	void UpdateSpot( void );
	BOOL ShouldWeaponIdle( void ) { return TRUE; };

	CLaserSpot *m_pSpot;
	int m_fSpotActive;
	int m_cActiveRockets;// how many missiles in flight from this launcher right now?

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usRpg;

};

class CRpgRocket : public CGrenade
{
public:
	//MODDD - new
	CRpgRocket(void);

	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
	void Spawn( void );
	void Precache( void );
	void EXPORT FollowThink( void );
	void EXPORT IgniteThink( void );
	void EXPORT RocketTouch( CBaseEntity *pOther );

	void onDelete(void);

	float massInfluence(void);
	int GetProjectileType(void);


	static CRpgRocket *CreateRpgRocket( Vector vecOrigin, Vector vecAngles, CBaseEntity *pOwner, CRpg *pLauncher );
	static CRpgRocket *CreateRpgRocket( Vector vecOrigin, Vector vecAngles, Vector arg_vecMoveDirection, CBaseEntity *pOwner, CRpg *pLauncher );

	int m_iTrail;
	float m_flIgniteTime;
	CRpg *m_pLauncher;// pointer back to the launcher that fired me. 

	BOOL ignited;

	Vector vecMoveDirectionMemory;

	//MODDD - new
	BOOL alreadyDeleted;

};

class CGauss : public CBasePlayerWeapon
{
public:

	int ohShitSon;
	int ohFuckSonny;
	
	//MOTHERappreciater HOW DO YOU NOT HAVE THIS
	CGauss(void);

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	void Holster( int skiplocal = 0  );


	//MODDD - new.
	void ItemPreFrame( void );
	void ItemPostFrame(void);

	void PrimaryAttack( void );
	void SecondaryAttack( void );


	//MODDD - new
	void chargeGauss(void);
	float getNextAmmoBurnDelay(void);
	void determineNextAmmoBurnAndUse(void);
	void chargeWork(void);
	void postChargeAnimCheck(void);
	void attemptFirePrimary(void);

	float getAmmoChargeInterval(void);

	void WeaponIdle( void );
	
	//MODDD - new
	void StartFireDecision( void);

	void StartFire( void );
	void Fire( Vector vecOrigSrc, Vector vecDirShooting, float flDamage );
	float GetFullChargeTime( void );
	int m_iBalls;
	int m_iGlow;
	int m_iBeam;
	int m_iSoundState; // don't save this

	// was this weapon just fired primary or secondary?
	// we need to know so we can pick the right set of effects. 
	BOOL m_fPrimaryFire;

	virtual BOOL UseDecrement( void )
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


class CEgon : public CBasePlayerWeapon
{
public:
#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	CEgon::CEgon();
	
	int dummy;

	BOOL canStartSecondary;
	BOOL secondarySwitched;
	BOOL holdingPrimary;
	BOOL holdingSecondary;

	//BOOL waitForAnimReset;
	int queueAnim;

	BOOL legalHoldSecondary;
	BOOL startedSecondaryHoldAttempt;
	float animationTime;
	float holdingSecondaryCurrent;
	float holdingSecondaryTarget0;
	float holdingSecondaryTarget1;
	float holdingSecondaryTarget2;

	
	//MODDD - also new.
	float timeSinceDeployed;

	BOOL toggledYet;
	BOOL altFireOn;
	int animationSequence;



	float oldTime;
	float currentTime;

	float timeDelta;

	int lastSentAnim;

	//MODDD - ??
	void ItemPostFrame( void );
	void ItemPreFrame( void );
	void PrimaryNotHeld( void );
	void SecondaryNotHeld( void );
	void NeitherHeld( void );
	void BothHeld( void );
	BOOL effectsExist;
	
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );

	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );

	void UpdateEffect( const Vector &startPoint, const Vector &endPoint, float timeBlend );

	void CreateEffect ( void );
	void DestroyEffect ( void );

	void EndAttack( void );
	void Attack( void );
	void PrimaryAttack( void );
	//MODDD -added
	void SecondaryAttack( void );

	void WeaponIdle( void );

	float m_flAmmoUseTime;// since we use < 1 point of ammo per update, we subtract ammo on a timer.

	float GetPulseInterval( void );
	float GetDischargeInterval( void );

	void Fire( const Vector &vecOrigSrc, const Vector &vecDir );

	BOOL HasAmmo( void );

	void UseAmmo( int count );
	
	enum EGON_FIREMODE { FIRE_NARROW, FIRE_WIDE};

	CBeam				*m_pBeam;
	CBeam				*m_pNoise;
	CSprite				*m_pSprite;

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

	unsigned short m_usEgonStop;

private:
	float				m_shootTime;
	EGON_FIREMODE		m_fireMode;
	float				m_shakeTime;
	BOOL				m_deployed;

	unsigned short m_usEgonFire;
};

class CHgun : public CBasePlayerWeapon
{
public:
	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 4; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );

	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	BOOL IsUseable( void );
	void Holster( int skiplocal = 0 );
	void Reload( void );
	void WeaponIdle( void );
	float m_flNextAnimTime;

	float m_flRechargeTime;
	
	int m_iFirePhase;// don't save me.

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
private:
	unsigned short m_usHornetFire;
};



class CHandGrenade : public CBasePlayerWeapon
{
public:

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );
	int ExtractAmmo( CBasePlayerWeapon *pWeapon );
	BOOL AddPrimaryAmmo( int iCount, char *szName, int iMaxClip, int iMaxCarry );

	//A flag for playing the deploy anim again (after throwing), so the grenade doen't just appear in an instant.
	BOOL replayDeploy;


	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);

	void PrimaryAttack( void );
	BOOL Deploy( void );
	BOOL CanHolster( void );
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );

	//MODDD - new.
	CHandGrenade(void);
	void ItemPreFrame( void );
	
	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
};

class CSatchel : public CBasePlayerWeapon
{
public:

#ifndef CLIENT_DLL
	int		Save( CSave &save );
	int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];
#endif

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	CSatchel();
	void customAttachToPlayer(CBasePlayer *pPlayer );

	int AddToPlayer( CBasePlayer *pPlayer );
	void PrimaryAttack( void );
	void SecondaryAttack( void );
	int AddDuplicate( CBasePlayerItem *pOriginal );
	BOOL CanDeploy( void );
	BOOL Deploy( void );
	BOOL IsUseable( void );
	
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );
	void Throw( void );
	
	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}
};


class CTripmine : public CBasePlayerWeapon
{
public:

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );

	void Spawn( void );
	void Precache( void );
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);
	void SetObjectCollisionBox( void )
	{
		//!!!BUGBUG - fix the model!
		pev->absmin = pev->origin + Vector(-16, -16, -5);
		pev->absmax = pev->origin + Vector(16, 16, 28); 
	}


	//MODDD - new.
	float holdingSecondaryTarget0;
	float holdingSecondaryTarget1;
	float holdingSecondaryTarget2;

	CTripmine();
	void ItemPreFrame( void );
	void ItemPostFrame( void );

	void PrimaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );
	void WeaponIdle( void );

	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usTripFire;

};

//TODO: make it move??  OR would it make more sense to make a monster that acts as if the player touched this?
class CSqueak : public CBasePlayerWeapon
{
public:
	//MODDD
	CSqueak();
	BOOL m_fInAttack;

	const char* GetPickupWalkerName(void);
	void Spawn( void );
	void Precache( void );
	void setModel(void);
	void setModel(const char* m);
	int iItemSlot( void ) { return 5; }
	int GetItemInfo(ItemInfo *p);

	//MODDD
	void customAttachToPlayer(CBasePlayer *pPlayer );


	void PrimaryAttack( void );
	void SecondaryAttack( void );
	BOOL Deploy( void );
	void Holster( int skiplocal = 0 );

	float randomIdleAnimationDelay(void);

	//MODDD - new
	void ItemPostFrame(void);

	void WeaponIdle( void );
	int m_fJustThrown;

	static int numberOfEyeSkins;


	virtual BOOL UseDecrement( void )
	{ 
#if defined( CLIENT_WEAPONS )
		return TRUE;
#else
		return FALSE;
#endif
	}

private:
	unsigned short m_usSnarkFire;
};













//WARNING: SERVER ONLY! NOTICE THAT!!!
#ifndef CLIENT_DLL


#define BOLT_AIR_VELOCITY	2000
#define BOLT_WATER_VELOCITY	1000

//MODDD
extern float global_sparksPlayerCrossbowMulti;




// UNDONE: Save/restore this?  Don't forget to set classname and LINK_ENTITY_TO_CLASS()
// 
// OVERLOADS SOME ENTVARS:
//
// speed - the ideal magnitude of my velocity
class CCrossbowBolt : public CBaseEntity
{

	//MODDD - a ... defined constructor defaults to private, but the auto-generated one didn't? I got nothing.
public:
	CCrossbowBolt(void);

	void Spawn( void );
	void Spawn( BOOL useTracer, BOOL arg_noDamage );
	void Precache( void );
	int  Classify ( void );
	void EXPORT BubbleThink( void );
	void EXPORT BoltTouch( CBaseEntity *pOther );
	void EXPORT ExplodeThink( void );

	float massInfluence(void);
	int GetProjectileType(void);

	Vector GetVelocityLogical(void);
	void SetVelocityLogical(const Vector& arg_newVelocity);


	int m_iTrail;
	Vector recentVelocity;
	Vector m_velocity;  //velocity set by some outside source. Kept for memory in case of other influences.
	float m_speed;  //why is this set separately? isn't it redundant being supplied this and a velocity, which is kinda both a direction and speed in one? who knows.
	float realNextThink;
	BOOL noDamage;

	static CCrossbowBolt *BoltCreate(const Vector& arg_velocity, float arg_speed );
	static CCrossbowBolt *BoltCreate(const Vector& arg_velocity, float arg_speed, BOOL useTracer, BOOL arg_noDamage );

};



#endif
//MODDDD - NOTE: all the above was server-side only.  Easy to miss this...




extern int giAmmoIndex;

//MODDD NEW - prototype so that other places can call this method too.
extern void AddAmmoNameToAmmoRegistry( const char *szAmmoname );

























#endif // WEAPONS_H
