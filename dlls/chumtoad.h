

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "soundent.h"
#include "gamerules.h"




#include	"schedule.h"
#include	"activity.h"
#include	"animation.h"

#include "defaultai.h"
#include	"game.h"




//????
//#include "effects.h"


#include "basemonster.h"

//v_chub.mdl
//chumtoad.mdl
//w_sqknest.mdl

//NOTES ABOUT SNARK:
//class of weapon and nest (pickup-able): CSqueak
//class of spawned creature on throw: CSqueakGrenade

//TODO: make CSQueak nest use the "Walk" animation and travel a short distance from spawn in random directions?

/*

chumtoad small walking npc that flees from preys, should attract bullsquids and agrunts - maps going to be featured in (c1a1 onwards) + weapon (should behave like the snark weapon)
    world pickable entity should roam around (or make it being able to follow paths like monster_generics) apply the same for the snarks counterpart
    note: add blinking system on both the npc and player weapon, also apply to snarks
    should feign death if the hunter npc gets real close, faking death should have a 50% chance of making the prey to actually buying it, and thus scaring the hunter away (or make it chase another chum or entity, including the player)(edited)
	*/






#ifndef CHUMTOAD_H
#define CHUMTOAD_H




#define CHUMTOAD_CROAK_ATTRACT_RANGE 500


#define CHUMTOAD_NORMAL_FOV 0.3
#define CHUMTOAD_PLAYDEAD_FOV VIEW_FIELD_FULL


enum chumtoad_e {  //key: frames, FPS
	CHUMTOAD_IDLE1 = 0,  //31, 16
	CHUMTOAD_IDLE2,  //31, 16
	CHUMTOAD_IDLE3,  //31, 16
	CHUMTOAD_FLINCH1, //23, 16
	CHUMTOAD_FLINCH2, //11, 16
	CHUMTOAD_HOP1,     //19, 60
	CHUMTOAD_HOP2,     //19, 40
	CHUMTOAD_SWIM, //31, 16
	CHUMTOAD_DIE1, //36, 
	CHUMTOAD_DIE2, //51, 25
	CHUMTOAD_DIE3, //41, 30
	CHUMTOAD_PLAYDEAD_START, //26, 16
	CHUMTOAD_PLAYDEAD_IDLE, //21, 40
	CHUMTOAD_PLAYDEAD_END, //26, 16

};



//or a parent of "CGrenade" like the CSqueakGrenade does?

class CChumToad : public CBaseMonster{
	
	

	/*
	void SetObjectCollisionBox( void )
	{
		//EASY_CVAR_PRINTIF_PRE(gargantuaPrintout, easyPrintLine( "YOU indubitable father-ignorant child %d", pev->deadflag));
		//could it be re-adjusted for "DEAD_DEAD" too?
			//if we are dead?
			pev->absmin = pev->origin + Vector( -6, -6, 0 );
			pev->absmax = pev->origin + Vector( 6, 6, 6 );
	}
	*/

    public:

	float testTimer;
	EHANDLE m_hEntitySittingOn;


    CChumToad(void);


	void HandleEventQueueEvent(int arg_eventID);

	void SetYawSpeed(void);
	BOOL getMonsterBlockIdleAutoUpdate(void);
	int Classify(void);
	BOOL getForceAllowNewEnemy(CBaseEntity* pOther);



	//save info
	//////////////////////////////////////////////////////////////////////////////////
	virtual int		Save( CSave &save ); 
	virtual int		Restore( CRestore &restore );
	
	static	TYPEDESCRIPTION m_SaveData[];
	//////////////////////////////////////////////////////////////////////////////////
	
	void setModel(void);
	void setModel(const char* m);

	void Spawn(void);
	void Precache(void);

	void firstLand(void);
	void Land(void);

	void forwardHop(void);
	void aimlessHop(void);
	void randomDelay(void);

	BOOL playDeadSuccessful;
	BOOL playDeadFooling(CBaseEntity* whoWantsToKnow);



	void onPlayDead();
	void playDeadSendMonstersAway();
	void onDeathAnimationEnd(void);

	CBaseEntity* getEntityBelow(void);


	BOOL IsAlive_FromAI( CBaseMonster* whoWantsToKnow );
	
	void ReportAIState( void );

	BOOL isForceHated(CBaseEntity *pBy);
	int forcedRelationshipWith(CBaseEntity *pWith);

	int IRelationship ( CBaseEntity *pTarget );

	
	BOOL forceIdleFrameReset(void);
	BOOL usesAdvancedAnimSystem(void);

	
	int LookupActivityHard(int activity);
	int tryActivitySubstitute(int activity);

	
	
	//????
	void DeathSound ( void );
	void AlertSound ( void );
	void IdleSound ( void );
	void PainSound ( void );

	
	static const char *pDeathSounds[];
	static const char *pAlertSounds[];
	static const char *pIdleSounds[];
	static const char *pPainSounds[];


	

	void setAnimationSmart(const char* arg_animName);
	void setAnimationSmart(const char* arg_animName, float arg_frameRate);
	void setAnimationSmart(int arg_animIndex, float arg_frameRate);
	void setAnimationSmartAndStop(const char* arg_animName);
	void setAnimationSmartAndStop(const char* arg_animName, float arg_frameRate);
	void setAnimationSmartAndStop(int arg_animIndex, float arg_frameRate);

	
	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE




	
	// No attacks
	BOOL CheckRangeAttack1 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckRangeAttack2 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckMeleeAttack1 ( float flDot, float flDist ) { return FALSE; }
	BOOL CheckMeleeAttack2 ( float flDot, float flDist ) { return FALSE; }

	//MODDD
	//int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType );
	//int TakeDamage( entvars_t *pevInflictor, entvars_t *pevAttacker, float flDamage, int bitsDamageType, int bitsDamageTypeMod );
	
	void MonsterThink ( void );

	
	void EXPORT ChumToadTouch ( CBaseEntity *pOther );
	void SetActivity ( Activity NewActivity );


	Schedule_t *GetSchedule( void ) override;
	Schedule_t* GetScheduleOfType( int Type) override;

	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );



	
	CUSTOM_SCHEDULES;


	GENERATE_DEADTAKEDAMAGE_PROTOTYPE
	GENERATE_GIBMONSTER_PROTOTYPE
	GENERATE_GIBMONSTERGIB_PROTOTYPE
	GENERATE_KILLED_PROTOTYPE


	Vector vecHopDest;

	BOOL initFall;
	BOOL generalFall;
	float previousZ;


	float forceStopInitFallTimer;
	float landTimer;
	float stopHopDelay;
	float delayTimer;
	float passiveCroakDelay;
	float playDeadForbiddenTimer;
	float panicTimer;

	float toadPlayDeadTimer;
	float toadPlayDeadAnimationTimer;

	float playDeadSendoffTimer;


	int m_iMyClass;
	static int numberOfEyeSkins;
	
	EHANDLE m_hOwner;
	


};//END OF CChumToad




const char *CChumToad::pDeathSounds[] = 
{
	"chumtoad/cht_croak_short.wav",
};
const char *CChumToad::pAlertSounds[] = 
{
	"chumtoad/cht_croak_short.wav",
	"chumtoad/cht_croak_medium.wav",
};
const char *CChumToad::pPainSounds[] = 
{
	"chumtoad/cht_croak_short.wav",
	"chumtoad/cht_croak_medium.wav",
};

const char *CChumToad::pIdleSounds[] = 
{
	"chumtoad/cht_croak_medium.wav",
	"chumtoad/cht_croak_long.wav",

};










#endif //END OF #ifdef CHUMTOAD_H
