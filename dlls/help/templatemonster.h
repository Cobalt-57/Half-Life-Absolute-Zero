//******************************************************************************************************************
// ***TEMPLATE MONSTER***
// clone this and edit to make custom monsters.
// Comment out some things if unused, like save info (.cpp-wise too) and the CUSTOM_SCHEDULES in .h and DEFINE_CUSTOM_SCHEDULES, IMPLEMENT_CUSTOM_SCHEDULES in .cpp***
//******************************************************************************************************************

//=========================================================
// DISPLAY NAME (spawn_name) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TEMPLATEMONSTER_H
#define TEMPLATEMONSTER_H


class CTemplateMonster : public CBaseMonster{
public:

	static const char* pDeathSounds[];
	static const char* pAlertSounds[];
	static const char* pIdleSounds[];
	static const char* pPainSounds[];
	static const char* pAttackSounds[];
	static const char* pAttackHitSounds[];
	static const char* pAttackMissSounds[];

	/*
	//save info
	//////////////////////////////////////////////////////////////////////////////////
	static TYPEDESCRIPTION m_SaveData[];
	int Save( CSave &save ); 
	int Restore( CRestore &restore );
	//////////////////////////////////////////////////////////////////////////////////
	*/

	CTemplateMonster(void);

	CUSTOM_SCHEDULES;

	void DeathSound ( void );
	void AlertSound ( void );
	void IdleSound ( void );
	void PainSound ( void );
	void AttackSound ( void );
	
	void Precache(void);
	void Spawn(void);

	void SetObjectCollisionBox(void);

	void SetEyePosition(void);
	
	float getDistTooFar(void);
	float getDistLook(void);

	
	Schedule_t* GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void ScheduleChange();
	Schedule_t* GetStumpedWaitSchedule(void);
	
	void StartTask( Task_t* pTask );
	void RunTask( Task_t* pTask );


	BOOL FCanCheckAttacks(void);

	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );
	
	void EXPORT CustomTouch( CBaseEntity* pOther );
	
	void MonsterThink( void );
	void PrescheduleThink(void);

	int Classify(void);
	BOOL isOrganic(void);
	int getHullIndexForNodes(void);
	int IRelationship ( CBaseEntity *pTarget );
	float HearingSensitivity(void);
	int ISoundMask(void);

	
	BOOL noncombat_Look_ignores_PVS_check(void);
	
	BOOL violentDeathAllowed(void);
	BOOL violentDeathDamageRequirement(void);
	BOOL violentDeathClear(void);
	int violentDeathPriority(void);

	
	void ReportAIState( void );

	//NOTICE - make these the "_VIRTUAL" versions if this monster could possibly have child classes made.
	//         Such as, the CTalkMonster having child classes CBarney and CScientist.
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	void OnTakeDamageSetConditions(entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod);

	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE

	GENERATE_GIBMONSTERGIB_PROTOTYPE

	//uncomment and implement these if needed. The defaults are good for most cases.
	//GENERATE_GIBMONSTERSOUND_PROTOTYPE
	//GENERATE_GIBMONSTEREND_PROTOTYPE

	GENERATE_KILLED_PROTOTYPE

	void onDelete(void);

	void SetYawSpeed(void);

	BOOL getMonsterBlockIdleAutoUpdate(void);
	BOOL forceIdleFrameReset(void);
	BOOL canPredictActRepeat(void);
	BOOL usesAdvancedAnimSystem(void);

	void SetActivity ( Activity NewActivity );

	int tryActivitySubstitute(int activity);
	int LookupActivityHard(int activity);

	void HandleEventQueueEvent(int arg_eventID);
	void HandleAnimEvent(MonsterEvent_t* pEvent );

};//END OF class CTemplateMonster






#endif //END OF #ifdef TEMPLATEMONSTER_H






