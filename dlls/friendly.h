
//=========================================================
// MR FRIENDLY (friendly) (split into .h & .cpp for prototype & implementation details accordingly)
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


#ifndef FRIENDLY_H
#define FRIENDLY_H


enum friendly_sequence{  //key: frames, FPS
	FRIENDLY_IDLE1,
	FRIENDLY_WALK,
	FRIENDLY_SMALLFLINCH,
	FRIENDLY_SMALLFLINCH2,
	FRIENDLY_ATTACK,
	FRIENDLY_DOUBLEWHIP,
	FRIENDLY_DIESIMPLE,
	FRIENDLY_VOMIT
};




//custom schedules
enum
{
	SCHED_FRIENDLY_SEEK_CORPSE = LAST_COMMON_SCHEDULE + 1,
	SCHED_FRIENDLY_SEEK_PLAYER_CORPSE,
	SCHED_FRIENDLY_SEEK_CORPSE_FAIL,
	SCHED_FRIENDLY_TAKE_COVER_FROM_PLAYER,
	SCHED_FRIENDLY_TAKE_COVER_FROM_PLAYER_FAIL,
	SCHED_FRIENDLY_STARE_AT_PLAYER,
};


//custom tasks
enum
{
	TASK_FRIENDLY_SEEK_CORPSE = LAST_COMMON_TASK + 1,
	TASK_FRIENDLY_SEEK_CORPSE_FAIL_WAIT,
	TASK_FRIENDLY_EAT_CORPSE,
	TASK_FRIENDLY_EAT_POST_WAIT,
	TASK_FRIENDLY_GENERIC_WAIT,
	TASK_FRIENDLY_SETUP_STAREBACK,
	TASK_FRIENDLY_TAKE_COVER_FROM_PLAYER,
	TASK_FRIENDLY_TAKE_COVER_FROM_PLAYER_FAIL_WAIT,
	TASK_FRIENDLY_STARE_AT_PLAYER,
	TASK_WAIT_FOR_MOVEMENT_RANGE_CHECK_TARGET,

};




class LinkedListTempNode{
	public:
	EHANDLE ent;
	LinkedListTempNode* next;
	BOOL hitThisFrame;

	LinkedListTempNode(){
		ent = NULL;
		next = NULL;
		hitThisFrame = FALSE;
	}
	~LinkedListTempNode(){
		if(next != NULL){
			delete next;
			next = NULL;
		}
	}

};

class LinkedListTemp{
	public:
	LinkedListTempNode* head;

	BOOL contains(CBaseEntity* argTest){

		LinkedListTempNode* thisNode = head;

		while(thisNode != NULL){

			if(thisNode->ent != NULL && thisNode->ent.Get() == argTest->edict()){
				return TRUE;
			}
			thisNode = thisNode->next;
		}

		return FALSE;

	}//END OF contains
	
	LinkedListTempNode* getNonEmpty(BOOL& mustAppend){

		if(head == NULL)return NULL; //all we can do.

		LinkedListTempNode* thisNode = head;

		while(thisNode != NULL){
			if(thisNode->ent == NULL){
				//if this node's ent has expired 
				mustAppend = FALSE;
				return thisNode;
			}else if(thisNode->next == NULL){
				//or if the next is null, this is good.
				mustAppend = TRUE;
				return thisNode;
			}else{
				//not null? g othere next.
				thisNode = thisNode->next;
			}
		}

		return NULL; //???

	}//END OF getLastNode

	void append(CBaseEntity* argEnt){
		
		if(contains(argEnt)){
			//forget it.
			return;
		}

		BOOL mustAppend;
		LinkedListTempNode* someNode = getNonEmpty(mustAppend);


		if(someNode == NULL){
			//first one. send to the head.
			head = new LinkedListTempNode();
			head->ent = argEnt;
		}else{
			//append.
			if(mustAppend){
				someNode->next = new LinkedListTempNode();
				someNode->next->ent = argEnt;
			}else{
				someNode->ent = argEnt;
			}
			
		}
	}//END OF append


	LinkedListTemp(){
		head = NULL;
	}
	~LinkedListTemp(){
		if(head != NULL){
			delete head;
			head = NULL;
		}
	}

};











class CFriendly : public CBaseMonster{
public:

	LinkedListTemp shieldSapList;
	
	int m_iVomitBallSpriteLocal;
	
	BOOL m_fPissedAtPlayer;
	BOOL m_fPissedAtPlayerAlly;
	BOOL m_fPissedAtHumanMilitary;

	BOOL playedVomitSoundYet;

	float nextVomitHitSoundAllowed;

	static const char* pDeathSounds[];
	static const char* pAlertSounds[];
	static const char* pIdleSounds[];
	static const char* pPainSounds[];
	static const char* pAttackSounds[];
	static const char* pAttackHitSounds[];
	static const char* pAttackMissSounds[];
	static const char* pVomitVoiceSounds[];
	static const char* pVomitHitSounds[];
	static const char* pChewSounds[];
	static const char* pVomitSounds[];
	
	float horrorPlayTime;
	float horrorPlayTimePreDelay;
	BOOL horrorSelected;

	float eatFinishTimer;
	float eatFinishPostWaitTimer;
	float nextPlayerSightCheck;
	float waitTime;
	float timeToStare;
	float vomitCooldown;
	float nextChewSound;
	EHANDLE playerToLookAt;
	EHANDLE corpseToSeek;

	BOOL rapidVomitCheck;
	BOOL rapidVomitCheck_ScheduleFinish;
	float nextNormalThinkTime;




	void stopHorrorSound(void);
	
	//save info
	//////////////////////////////////////////////////////////////////////////////////
	static TYPEDESCRIPTION m_SaveData[];
	virtual int Save( CSave &save ); 
	virtual int Restore( CRestore &restore );
	//////////////////////////////////////////////////////////////////////////////////

	CFriendly(void);

	CUSTOM_SCHEDULES;

	void DeathSound ( void );
	void AlertSound ( void );
	void IdleSound ( void );
	void PainSound ( void );
	void AttackSound ( void );
	void VomitSound(void);
	void VomitVoiceSound(void);
	void VomitHitSound(edict_t* pevToPlayAt);

	
	void Precache(void);
	void Spawn(void);
	
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


	
	CBaseEntity* getNearestDeadBody(void);
	void friendly_findCoverFromPlayer( entvars_t* pevPlayerToHideFrom, float flMoveWaitFinishedDelay );

	void attemptAddToShieldSapList(CBaseEntity* argEnt);
	void unlinkShieldSapList(void);
	

	BOOL isProvokable(void);
	BOOL isProvoked(void);
	
	void ScheduleChange( void );
	
	int getHullIndexForNodes(void);

};//END OF class CFriendly







#endif //END OF #ifdef FRIENDLY_H






