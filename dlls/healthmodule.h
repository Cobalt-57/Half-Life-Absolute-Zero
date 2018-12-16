


#include "cbase.h"

//MODDD - new. Purely parent-less data class for storing info and behavior about healing to ease reuse between CWallHealth and
//        certain CRotDoor's using this.



//A class that a parent of a HealthModule (in the sense of having a HealthModule instance at all) should take to guarantee some event methods.
//And implement those of course.
//C does multi-inheritence, not sure if there's any possible difference for that if this is more of an "Interface" like in Java than a parent class.
class I_HealthModule_Parent{
public:
	virtual void I_HealthModule_ChargeEmpty(void){};
	virtual void I_HealthModule_ChargeRestored(void){};
	virtual void I_HealthModule_UseStart(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value){};
	virtual void I_HealthModule_UseContinuous(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value){};
	virtual void I_HealthModule_UseEnd(void){};
	virtual void I_HealthModule_SetThink_UseEnd(void){};
	virtual void I_HealthModule_SetThink_ChargeRestored(void){};
	
	virtual void I_HealthModule_SetThink_Custom(void){};

};



class HealthModule{
public:
	//NEW
	float rechargeDelay;
	float turnOffDelay;


	BOOL establishedParentYet;

	BOOL firstUseSinceEnd;  //is this a continuous "use" or the first in a while?

	//I need a way to hook into the engine for some features liky playing sounds. Some Entity to do that from.
	//I need to know it as both an entity (for its own entity variables and pev->...) and a HealthModule_Parent for sending events to.
	CBaseEntity* parentEntity_entity;
	I_HealthModule_Parent* parentEntity_event;


	
	HealthModule(void);

	//Yes sending the exact same thing twice is ugly but I don't know how else to say "I expect one object that is a subclass of 'this' and 'that'".
	void setupSpawn(CBaseEntity* arg_parentEntity_entity, I_HealthModule_Parent* arg_parentEntity_event);
	void setupRestore(CBaseEntity* arg_parentEntity_entity, I_HealthModule_Parent* arg_parentEntity_event);
	
	void CustomThink(void);

	void turnThinkOff(void);

	void Spawn( );
	void Precache( void );
	
	//void EXPORT Off(void);
	//void EXPORT Recharge(void);

	void UseEnd(void);
	void ChargeRestored(void);


	void KeyValue( KeyValueData *pkvd );
	BOOL IsWorldAffiliated(void);
	
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );


	void PlaySound(int channel, const char* sample, float volume, float attenuation);
	void StopSound(int channel, const char* sample);

	static TYPEDESCRIPTION m_SaveData[];
	int Save( CSave &save );
	int Restore( CRestore &restore );

	void stopSounds(void);



	
	float m_flNextCharge; 
	int		m_iReactivate ; // DeathMatch Delay until reactvated
	int		m_iJuice;
	int		m_iOn;			// 0 = off, 1 = startup, 2 = going
	float   m_flSoundTime;

};
