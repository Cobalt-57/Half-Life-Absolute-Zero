

//******************************************************************************************************************
//***KING PIN***
//
//******************************************************************************************************************

//=========================================================
// KING PIN (monster_kingpin) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================

//Max range to search for powerup-able monsters. Bigger than the APPLY range below. If the closest monster is outside of APPLY but within SEARCH, this Kingpin may walk closer to get in APPLY range.
#define POWERUP_SEARCH_RANGE 2100
//Max range this Kingpin may use the Power Up ability. May also walk towards monsters and then use it if they are not too far away (within SEARCH range at least).
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


#define KINGPIN_SHOCKER_RADIUS 260
#define KINGPIN_MAX_BEAMS 4


class CKingpin : public CBaseMonster{
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

	
	static const char* pShockerFireSounds[];

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
	
	//thanks islave.
	CBeam* m_pBeam[KINGPIN_MAX_BEAMS];
	int m_iBeams;  //this is the soft max, or how many beams there are currently.
	//Can also be thought of as what ID is empty and next in line for making a beam of.
	//We're going to allow it to wrap around back to 0 if it reaches the hard max, KINGPIN_MAX_BEAMS. The max itself is not an available index.
	//example: a KINGPIN_MAX_BEAMS of 6 allows indexes 0 through 5 but not 6.  So if m_iBeams reaches 6, reset it to 0 to start from the beginning.
	//         Won't cause an issue unless a beam is already in that place, but they expire fast.

	//And this is new. At what time should a beam expire?  Needed since they don't remove themselves automatically.
	//ISlave's never did, they are manually cleared by "ClearBeams" each time there.
	float m_flBeamExpireTime[KINGPIN_MAX_BEAMS];


	float chargeFinishTime;

	int electricBarrageShotsFired;
	float electricBarrageNextFireTime;
	float electricBarrageStopTime;
	float electricBarrageIdleEndTime;

	float primaryAttackCooldownTime;
	float enemyHiddenResponseTime;
	float enemyHiddenChaseTime;
	float giveUpChaseTime;
	float administerShockerTime;


	float accumulatedDamageTaken;

	CSprite* chargeEffect;

	int m_iSpriteTexture;

	BOOL enemyNullTimeSet;





	CKingpin(void);


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
	Schedule_t* pickChaseOrStaySchedule(void);
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

	void OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod);

	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE
	
	GENERATE_KILLED_PROTOTYPE

	void onDelete(void);

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

	
	void SetObjectCollisionBox( void ){

		if(pev->deadflag != DEAD_NO){
			//no need to do anything special anymore I think.
			//CBaseMonster::SetObjectCollisionBox();
			pev->absmin = pev->origin + Vector(-64, -64, 0);
			pev->absmax = pev->origin + Vector(64, 64, 60);
		}else{
			// = DEAD_NO
			pev->absmin = pev->origin + Vector(-32, -32, 0);
			pev->absmax = pev->origin + Vector(32, 32, 110);
		}
		
	}//END OF SetObjectCollisionBox


	void ScheduleChange(void);




	void playForceFieldReflectSound(void);

	void playSuperBallFireSound(void);

	void playElectricBarrageStartSound(void);
	void playElectricBarrageLoopSound(void);
	void stopElectricBarrageLoopSound(void);
	void playElectricBarrageEndSound(void);
	
	void playElectricBarrageFireSound();
	void playElectricBarrageHitSound(CBaseEntity* arg_target, const Vector& arg_location);
	

	void playElectricLaserChargeSound(void);
	void stopElectricLaserChargeSound(void);
	void playElectricLaserFireSound(void);
	void playElectricLaserHitSound(CBaseEntity* arg_target, const Vector& arg_location);
	
	void fireElectricBarrageLaser(void);
	void fireElectricDenseLaser(CBaseEntity* arg_hitIntention, const Vector& arg_hitTargetPoint);
	void fireSuperBall(void);
	void fireSpeedMissile(void);
	void createSpeedMissileHornet(const Vector& arg_location, const Vector& arg_floatVelocity);

	
	CBeam*& getNextBeam(void);
	void SetupBeams(void);
	void ClearBeams(void);
	void CheckBeams(void);

	BOOL turnToFaceEnemyLKP(void);
	float getDotProductWithEnemyLKP(void);

	void createChargeEffect(void);
	void updateChargeEffect(void);
	void removeChargeEffect(void);

	BOOL FCanCheckAttacks(void);
	
	float getDistTooFar(void);
	float getDistLook(void);

	void setPrimaryAttackCooldown(void);

	void playShockerFireSound(CBaseEntity* arg_target, const Vector& arg_location);
	void administerShocker(void);

	BOOL needsMovementBoundFix(void);
	
	CBaseEntity* attemptFindTowardsPoint(const Vector& arg_searchPoint);

	int getHullIndexForNodes(void);

};//END OF class CKingpin








#endif //END OF #ifdef KINGPIN_H




