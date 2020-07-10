
//=========================================================
// PANTHER EYE (panthereye)
//=========================================================

#ifndef PANTHER_EYE_H
#define PANTHER_EYE_H

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"


class CPantherEye : public CBaseMonster
{
public:

	float HearingSensitivity(void);
	BOOL testLeapNoBlock(void);

	int  IRelationship( CBaseEntity *pTarget );
	void panthereye_findCoverFromEnemy(void);

	virtual int	Restore( CRestore &restore );
	virtual int	Save( CSave &save );
	static TYPEDESCRIPTION m_SaveData[];
	
	float faceLookTime;
	float leapAttackCooldown;

	BOOL isPissable;
	BOOL isCornered;

	CPantherEye(void);

	float runawayTime;
	float pissedOffTime;
	float timeTillSneakAgain;
	float findCoverTaskDataMem;

	float m_iTargetRanderamt;
	int sneakMode;
	//-1 = not sneaking.
	//0 = sneaking.

	Vector debugDrawVect;

	void EXPORT LeapTouch ( CBaseEntity *pOther );
		
	Schedule_t* ChaseOrCircle(void);
	
	BOOL hasSeeEnemyFix(void);

	BOOL forceIdleFrameReset(void);
	BOOL usesAdvancedAnimSystem(void);
	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);
	

	void Spawn( void );
	void Precache( void );
	
	void SetEyePosition(void);

	void SetYawSpeed( void );
	int  Classify ( void );
	
	void HandleEventQueueEvent(int arg_eventID);
	void HandleAnimEvent( MonsterEvent_t *pEvent );

	int IgnoreConditions ( void );

	float newPathDelay;

	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	
	BOOL needsMovementBoundFix(void);
	void MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval );
	
	// No range attacks
	BOOL CheckRangeAttack1 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckRangeAttack2 ( float flDot, float flDist ) { return FALSE; }

	BOOL violentDeathAllowed(void);
	BOOL violentDeathClear(void);
	int violentDeathPriority(void);
	
	void OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod);


	float bugAnimBlockedTime;
	BOOL waitingForNewPath;

	//UNUSED, I guess.
	float chaseMode;
	//-1: not yet chasing anything.
	//0: trying to get into circle range...
	//1: circling around, ready to pick random position.
	//2: go to random position within circle edge
	//3: jump at target.
	//4: charge directly at foe.  (leads to attack melee1).

	float stareTime;

    float maxWaitPVSTime;

	float m_flNextFlinch;

	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( void );


	static const char *pAttackSounds[];
	static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];
	static const char *pLeapAttackHitSounds[];
	static const char *pDeathSounds[];
	
	//MODDD
	GENERATE_TAKEDAMAGE_PROTOTYPE
	GENERATE_TRACEATTACK_PROTOTYPE
	
	void MonsterThink ( void );
	void SetActivity ( Activity NewActivity );


	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );

	int getHullIndexForNodes(void);

	//originally 784.
	float CoverRadius( void ) { return DEFAULT_COVER_SEEK_DISTANCE * 1.8; }
	

	CUSTOM_SCHEDULES;
};




#endif //PANTHER_EYE_H