

//******************************************************************************************************************
//***KING PIN***
//
//******************************************************************************************************************

//=========================================================
// KING PIN (monster_kingpin) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================

//Max range to search for powerup-able monsters. Bigger than the APPLY range below. If the closest monster is outside of APPLY but within SEARCH, this KingPin may walk closer to get in APPLY range.
#define POWERUP_SEARCH_RANGE 2100
//Max range this KingPin may use the Power Up ability. May also walk towards monsters and then use it if they are not too far away (within SEARCH range at least).
#define POWERUP_APPLY_RANGE 1600


#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "schedule.h"
#include "activity.h"
#include "animation.h"

#include "defaultai.h"
#include "soundent.h"
#include "game.h"

//MODDD - why not?
#include "weapons.h"

#include "basemonster.h"


#ifndef KINGPIN_H
#define KINGPIN_H



class CKingPin : public CBaseMonster{
public:

	static const char* pDeathSounds[];
	static const char* pAlertSounds[];
	static const char* pIdleSounds[];
	static const char* pPainSounds[];
	static const char* pAttackSounds[];

	//NEW!!!
	static const char* pElectricBarrageHitSounds[];
	static const char* pElectricBarrageFireSounds[];
	static const char* pElectricBarrageEndSounds[];

	static const char* pAttackHitSounds[];
	static const char* pAttackMissSounds[];

	
	//save info
	//////////////////////////////////////////////////////////////////////////////////
	static TYPEDESCRIPTION m_SaveData[];
	virtual int Save( CSave &save ); 
	virtual int Restore( CRestore &restore );
	//////////////////////////////////////////////////////////////////////////////////
	


	//nah, don't keep track of this.
	//CBaseMonster* aryPoweredupCommandList[10];
	
	float powerUpNearbyMonstersCooldown;
	float forceEnemyOnPoweredUpMonstersCooldown;
	float forceEnemyOnPoweredUpMonstersHardCooldown;
	float forgetRecentInflictingMonsterCooldown;
	EHANDLE recentInflictingMonster;

	int m_voicePitch;



	CKingPin(void);


	CUSTOM_SCHEDULES;

	void DeathSound ( void );
	void AlertSound ( void );
	void IdleSound ( void );
	void PainSound ( void );
	void AttackSound ( void );
	
	void Precache(void);
	void Spawn(void);

	void SetEyePosition(void);
	
	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);
	
	void StartTask( Task_t *pTask );
	void RunTask( Task_t *pTask );
	
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );
	
	void EXPORT CustomTouch ( CBaseEntity *pOther );
	
	void MonsterThink( void );

	int Classify(void);
	BOOL isOrganic(void);
	int IRelationship ( CBaseEntity *pTarget );
	
	void ReportAIState( void );
	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE
	
	GENERATE_KILLED_PROTOTYPE

	void SetYawSpeed(void);

	BOOL getMonsterBlockIdleAutoUpdate(void);
	BOOL forceIdleFrameReset(void);
	BOOL usesAdvancedAnimSystem(void);

	void SetActivity ( Activity NewActivity );



	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);

	void HandleEventQueueEvent(int arg_eventID);
	void HandleAnimEvent(MonsterEvent_t *pEvent );

	
	CBaseMonster* findClosestPowerupableMonster(void);
	void powerUpMonsters(void);

	void removeFromPoweredUpCommandList(CBaseMonster* argToRemove);

	void deTargetMyCommandedMonsters(void);
	void forceEnemyOnPoweredUpMonsters(CBaseEntity* monsterToForce, BOOL argPassive);

	
	void playPsionicLaunchSound(void);

	
	void SetObjectCollisionBox( void )
	{
		/*
		//EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "garg deadflag? %d", pev->deadflag));
		//could it be re-adjusted for "DEAD_DEAD" too?
		if(pev->deadflag != DEAD_NO){
			//if we are dead?
			pev->absmin = pev->origin + Vector(-65, -65, 0);
			pev->absmax = pev->origin + Vector(65, 65, 72);
		}else{
			CBaseMonster::SetObjectCollisionBox();

		}
		*/

		//no need to do anything special anymore I think.
		CBaseMonster::SetObjectCollisionBox();
	}


	void ScheduleChange(void);



	void playForceFieldElectricBarrageFireSound(void);
	void playForceFieldElectricBarrageHitSound(void);

	void playForceFieldReflectSound(void);

	void playSuperBallFireSound(void);

	void playElectricBarrageStartSound(void);
	void playElectricBarrageLoopSound(void);
	void playElectricBarrageEndSound(void);
	

	void playElectricLaserChargeSound(void);
	void playElectricLaserFireSound(void);
	void playElectricLaserHitSound(void);





};//END OF class CKingPin








#endif //END OF #ifdef KINGPIN_H




