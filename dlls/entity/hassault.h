//=========================================================
// HUMAN ASSAULT (hassault)
//=========================================================

#ifndef HASSAULT_H
#define HASSAULT_H

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "squadmonster.h"
#include "talkmonster.h"


//MODDD - making a "CBaseSquadMonster" instead to be more compatibile with squads (particularly as leaders to HGrunts)
class CHAssault : public CSquadMonster
{
public:
	
	float SafeSetBlending ( int iBlender, float flValue );


	float signal1Cooldown;
	float signal2Cooldown;

	float meleeBlockTime;

	BOOL nonStumpableCombatLook;
	int previousAnimationActivity;

	float fireDelay;
	
	//MODDD - addition
	BOOL couldRangedAttack1;
	BOOL couldRangedAttack2;
	BOOL couldMeleeAttack1;
	BOOL couldMeleeAttack2;

	BOOL forceBlockResidual;
	void ReportAIState( void );


	Vector GetGunPosition(void);
	Vector GetGunPositionAI(void);


	int IRelationship ( CBaseEntity *pTarget );
	
	float getBarnacleForwardOffset(void);
	float getBarnaclePulledTopOffset(void);
	Vector debugDrawVect;
	void onDelete(void);

	BOOL usesAdvancedAnimSystem(void);
	int tryActivitySubstitute(int activity);
	int LookupActivityHard(int activity);
	
   
	
	virtual int	Restore( CRestore &restore );
	virtual int	Save( CSave &save );
	static TYPEDESCRIPTION m_SaveData[];


	CHAssault(void);

	float meleeAttackTimeMax;
	float residualFireTime;
	float waittime;
	Schedule_t* recentSchedule;
	Schedule_t* recentRecentSchedule;
	float idleSpinSoundDelay;
	float chainFireSoundDelay;
	BOOL chainFiredRecently;
	float spinuptimeIdleSoundDelay;
	float residualFireTimeBehindCheck;
	//!!!
	Vector forceFireTargetPosition;

	//edict_t* forceFireTargetObject;
	EHANDLE forceFireTargetObject;

	float forceFireGiveUp;

	float alertSoundCooldown;


	int getIdleSpinChannel(void );
	int getSpinUpDownChannel(void );
	int getChainFireChannel(void);

	void resetChainFireSound(void);
	void attemptStopIdleSpinSound(void);
	void attemptStopIdleSpinSound(BOOL forceStop);

	void Spawn( void );
	void Precache( void );
	void SetYawSpeed( void );
	int  Classify ( void );
	BOOL isOrganic(void){return !CanUseGermanModel();}
	
	BOOL getGermanModelRequirement(void);
	const char* getGermanModel(void);
	const char* getNormalModel(void);


	void PrescheduleThink ( void );

	void HandleEventQueueEvent( int arg_eventID);
	void HandleAnimEvent( MonsterEvent_t *pEvent );
//	int IgnoreConditions ( void );

	float m_flNextFlinch;
	
	BOOL FCanCheckAttacks();
	void PainSound( void );
	void AlertSound( void );
	void IdleSound( void );
	void AttackSound( void );
	void DeathSound( void );
	void Shoot ( void );
	void ShootAtForceFireTarget();

	//MODDD - new.
	void SetObjectCollisionBox( void )
	{
		pev->absmin = pev->origin + Vector(-65, -65, 0);
		pev->absmax = pev->origin + Vector(65, 65, 72);
	}


	static const char *pAttackSounds[];
	//static const char *pIdleSounds[];
	static const char *pAlertSounds[];
	static const char *pPainSounds[];
	static const char *pAttackHitSounds[];
	static const char *pAttackMissSounds[];

	void MoveExecute( CBaseEntity *pTargetEnt, const Vector &vecDir, float flInterval );
	
	GENERATE_KILLED_PROTOTYPE

	BOOL CheckMeleeAttack1 ( float flDot, float flDist );
	BOOL CheckMeleeAttack2 ( float flDot, float flDist ){return false;};
	BOOL CheckRangeAttack1 ( float flDot, float flDist );
	BOOL CheckRangeAttack2 ( float flDot, float flDist );

	void SetActivity(Activity NewActivity);
	
	void SetTurnActivity(void);

	GENERATE_TRACEATTACK_PROTOTYPE
	GENERATE_TAKEDAMAGE_PROTOTYPE
	

	int m_voicePitch;

	BOOL firing;
	float spinuptime;
	float spinuptimeremain;

	float rageTimer;
	float movementBaseFramerate;

	BOOL soundgiven;
	int	m_iBrassShell;
	void MonsterThink( void );
	
	BOOL FValidateCover ( const Vector &vecCoverLocation );
	int ISoundMask ( void );

	//MODDD - May be a better approach than "monsterThink".
	Schedule_t *GetSchedule( void );
	Schedule_t* GetScheduleOfType( int Type);

	void StartTask ( Task_t *pTask );
	void RunTask ( Task_t *pTask );

	//MODDD - new.
	BOOL spinuptimeSet;
	int SquadRecruit( int searchRadius, int maxMembers );

	void AimAtEnemy(Vector& refVecShootOrigin, Vector& refVecShootDir);
	

	BOOL canResetBlend0(void);
	BOOL onResetBlend0(void);

	CUSTOM_SCHEDULES;

};


#endif //HASSAULT_H
