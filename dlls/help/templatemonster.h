

//******************************************************************************************************************
//***TEMPLATE MONSTER***
//clone this and edit to make custom monsters.
//Comment out some things if unused, like save info (.cpp-wise too) and the CUSTOM_SCHEDULES in .h and DEFINE_CUSTOM_SCHEDULES, IMPLEMENT_CUSTOM_SCHEDULES in .cpp***
//******************************************************************************************************************

//=========================================================
// DISPLAY NAME (spawn_name) (split into .h & .cpp for prototype & implementation details accordingly)
//=========================================================



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

#include "basemonster.h"


#ifndef TEMPLATEMONSTER_H
#define TEMPLATEMONSTER_H

//sequences in the anim, in the order they appear in the anim. Some anims have the same display name and so should just be referenced by order
//(numbered index), named well after purpose and based on display names for clarity. Safer this way.
enum templateMonster_sequence {  //key: frames, FPS
	TEMPLATEMONSTER_XXX,

};


//custom schedules
enum
{
	SCHED_TEMPLATEMONSTER_XXX = LAST_COMMON_SCHEDULE + 1,
	SCHED_TEMPLATEMONSTER_YYY,
	SCHED_TEMPLATEMONSTER_ZZZ,


};

//custom tasks
enum
{
	TASK_TEMPLATEMONSTER_XXX = LAST_COMMON_TASK + 1,
	TASK_TEMPLATEMONSTER_YYY,
	TASK_TEMPLATEMONSTER_ZZZ,
	

};


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
	virtual int Save( CSave &save ); 
	virtual int Restore( CRestore &restore );
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
	
	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void ScheduleChange();
	Schedule_t* GetStumpedWaitSchedule(void);
	
	void StartTask( Task_t *pTask );
	void RunTask( Task_t *pTask );

	
	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist );
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );
	
	void EXPORT CustomTouch ( CBaseEntity *pOther );
	
	void MonsterThink( void );
	void PrescheduleThink(void);

	int Classify(void);
	BOOL isOrganic(void);
	int IRelationship ( CBaseEntity *pTarget );
	
	void ReportAIState( void );

	//NOTICE - make these the "_VIRTUAL" versions if this monster could possibly have child classes made.
	//         Such as, the CTalkMonster having child classes CBarney and CScientist.
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE

	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	
	GENERATE_GIBMONSTER_PROTOTYPE

	GENERATE_GIBMONSTERGIB_PROTOTYPE

	//uncomment and implement these if needed. The defaults are good for most cases.
	//GENERATE_GIBMONSTERSOUND_PROTOTYPE
	//GENERATE_GIBMONSTEREND_PROTOTYPE

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

};//END OF class CTemplateMonster






#endif //END OF #ifdef TEMPLATEMONSTER_H






