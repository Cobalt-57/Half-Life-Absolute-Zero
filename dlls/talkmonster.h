/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   This source code contains proprietary and confidential information of
*   Valve LLC and its suppliers.  Access to this code is restricted to
*   persons who have executed a written SDK license with Valve.  Any access,
*   use or distribution of this code by or to any unlicensed person is illegal.
*
****/
#ifndef TALKMONSTER_H
#define TALKMONSTER_H

#ifndef MONSTERS_H
#include "basemonster.h"
#endif

//=========================================================
// Talking monster base class
// Used for scientists and barneys
//=========================================================

#define TALKRANGE_MIN 500.0				// don't talk to anyone farther away than this

#define TLK_STARE_DIST	128				// anyone closer than this and looking at me is probably staring at me.

#define bit_saidDamageLight		(1<<0)	// bits so we don't repeat key sentences
#define bit_saidDamageMedium	(1<<1)
#define bit_saidDamageHeavy		(1<<2)
#define bit_saidHelloPlayer		(1<<3)
#define bit_saidWoundLight		(1<<4)
#define bit_saidWoundHeavy		(1<<5)
#define bit_saidHeard			(1<<6)
#define bit_saidSmelled			(1<<7)

#define TLK_CFRIENDS		3

typedef enum
{
	TLK_ANSWER = 0,
	TLK_QUESTION,
	TLK_IDLE,
	TLK_STARE,
	TLK_USE,
	TLK_UNUSE,
	TLK_STOP,
	TLK_NOSHOOT,
	TLK_HELLO,
	TLK_PHELLO,
	TLK_PIDLE,
	TLK_PQUESTION,
	TLK_PLHURT1,
	TLK_PLHURT2,
	TLK_PLHURT3,
	TLK_SMELL,
	TLK_WOUND,
	TLK_MORTAL,

	TLK_CGROUPS,					// MUST be last entry
} TALKGROUPNAMES;


enum
{
	SCHED_CANT_FOLLOW = LAST_COMMON_SCHEDULE + 1,
	SCHED_MOVE_AWAY,		// Try to get out of the player's way
	SCHED_MOVE_AWAY_FOLLOW,	// same, but follow afterward
	SCHED_MOVE_AWAY_FAIL,	// Turn back toward player


	LAST_TALKMONSTER_SCHEDULE,		// MUST be last
};

enum
{
	TASK_CANT_FOLLOW = LAST_COMMON_TASK + 1,
	TASK_MOVE_AWAY_PATH,
	TASK_WALK_PATH_FOR_UNITS,

	TASK_TLK_RESPOND,		// say my response
	TASK_TLK_SPEAK,			// question or remark
	TASK_TLK_HELLO,			// Try to say hello to player
	TASK_TLK_HEADRESET,		// reset head position
	TASK_TLK_STOPSHOOTING,	// tell player to stop shooting friend
	TASK_TLK_STARE,			// let the player know I know he's staring at me.
	TASK_TLK_LOOK_AT_CLIENT,// faces player if not moving and not talking and in idle.
	TASK_TLK_CLIENT_STARE,	// same as look at client, but says something if the player stares.
	TASK_TLK_EYECONTACT,	// maintain eyecontact with person who I'm talking to
	TASK_TLK_IDEALYAW,		// set ideal yaw to face who I'm talking to
	TASK_FACE_PLAYER,		// Face the player

	TASK_FOLLOW_SUCCESSFUL,  //MODDD new. Report that we made it to whoever we wanted to follow so we can reset the consecutive follow fails count. Put this after TASK_MOVE_TO_TARGET_RANGE in schedules.

	LAST_TALKMONSTER_TASK,			// MUST be last
};













class CTalkMonster : public CBaseMonster
{
public:
	
	EHANDLE closestCautiousNPC;
	EHANDLE closestCautiousNPC_memory;
	float closestCautiousNPC_distance;
	EHANDLE closestPassiveNPC;
	EHANDLE closestPassiveNPC_memory;
	float closestPassiveNPC_distance;


	//MODDD
	CTalkMonster(void);

	virtual int getMadSentencesMax(void);
	virtual int getMadInterSentencesMax(void);

	float nextMadEffect;
	virtual void MonsterThink(void);
	BOOL madDir;
	BOOL canGoRavingMad;
	virtual BOOL isTalkMonster(void);

	//This is a talk monster because the "TalkMonster" class is unaware of its children.  Can still work thanks to virtual methods.
	CTalkMonster* scientistTryingToHealMe;
	EHANDLE scientistTryingToHealMeEHANDLE;

	virtual void forgetHealNPC(void);

	virtual void ReportAIState(void);

	//MODDD
	const char*		madSentences[5];
	int				madSentencesMax;
	//EACH!
	//static const char*		madInterSentences[];
	//...each child will define "madInterSentences" instead, and use "madInterSentencesLocation" to refer to its own one.
	//int*				madInterSentencesMaxLocation;
	const char** madInterSentencesLocation;

	
	//MODDD - m_flPlayerDamage moved from the Barney to TalkMonster so that the Scientist also benefits from it.
	float m_flPlayerDamage;// how much pain has the player inflicted on me. Slowly decreases over time.
	float forgiveSuspiciousTime;
	float forgiveSomePlayerDamageTime;







	
	//MODDD - also new.
	virtual void	playPissed();
	virtual void	playInterPissed();

	void			TalkInit( void );				
	CBaseEntity		*FindNearestFriend(BOOL fPlayer);
	float			TargetDistance( void );
	void			StopTalking( void ) { SentenceStop(); }
	
	// Base Monster functions
	void			Precache( void );


	



	void			Touch(	CBaseEntity *pOther );

	//wasn't virtual? WHY?!!!
	GENERATE_KILLED_PROTOTYPE_VIRTUAL
	
	int				IRelationship ( CBaseEntity *pTarget );
	virtual int		CanPlaySentence( BOOL fDisregardState );
	virtual void	PlaySentence( const char *pszSentence, float duration, float volume, float attenuation );
	void			PlayScriptedSentence( const char *pszSentence, float duration, float volume, float attenuation, BOOL bConcurrent, CBaseEntity *pListener );
	void			KeyValue( KeyValueData *pkvd );


	//..these weren't virtual.
	//THESE. WERE. NOT. FUCKING. VIRTUAL.
	//OH YOU DIRTY MOTHERFUCKER. I\'LL PLAY JUMPROPE WITH YOUR GODDAMN INTESTINES IN HELL SHITLICKER
	// AI functions
	virtual void			SetActivity ( Activity newActivity );
	virtual Schedule_t		*GetScheduleOfType ( int Type );
	virtual void			StartTask( Task_t *pTask );
	virtual void			RunTask( Task_t *pTask );
	virtual void			HandleAnimEvent( MonsterEvent_t *pEvent );
	virtual void			PrescheduleThink( void );
	

	// Conversations / communication
	int				GetVoicePitch( void );
	void			IdleRespond( void );
	int				FIdleSpeak( void );
	int				FIdleStare( void );
	int				FIdleHello( void );
	void			IdleHeadTurn( Vector &vecFriend );
	int				FOkToSpeak( void );
	//MODDD - new version to allow talking during combat
	int				FOkToSpeakAllowCombat( float waitTime );
	void			TrySmellTalk( void );
	CBaseEntity		*EnumFriends( CBaseEntity *pentPrevious, int listNumber, BOOL bTrace );
	void			AlertFriends( void );
	void			ShutUpFriends( void );
	BOOL			IsTalking( void );
	void			Talk( float flDuration );	
	// For following
	BOOL			CanFollow( void );
	BOOL			IsFollowing( void ) { return m_hTargetEnt != NULL && m_hTargetEnt->IsPlayer(); }


	void			StopFollowing( BOOL clearSchedule );
	//MODDD - new version.
	void			StopFollowing( BOOL clearSchedule, BOOL playUnuseSentence );
	
	//MODDD - why not virtual?
	virtual void			StartFollowing( CBaseEntity *pLeader );


	virtual void	DeclineFollowing( void ) {}
	void			LimitFollowers( CBaseEntity *pPlayer, int maxFollowers );

	void EXPORT		FollowerUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	
	virtual void	SetAnswerQuestion( CTalkMonster *pSpeaker );
	virtual int		FriendNumber( int arrayNumber )	{ return arrayNumber; }

	//MODDD
	virtual BOOL getGermanModelRequirement(void);
	virtual void Activate(void);
	virtual void Spawn(void);
	virtual const char* getGermanModel(void);
	virtual const char* getNormalModel(void);

	virtual void setModel(void);
	virtual void setModel(const char* m);
	
	virtual void DeclineFollowingProvoked(CBaseEntity* pCaller);
	virtual void SayProvoked(void);
	virtual void SaySuspicious(void);
	virtual void SayLeaderDied(void);
	virtual void SayNearPassive(void);
	virtual void SayNearCautious(void);
	
	GENERATE_TRACEATTACK_PROTOTYPE_VIRTUAL
	GENERATE_TAKEDAMAGE_PROTOTYPE_VIRTUAL



	virtual int		Save( CSave &save );
	virtual int		Restore( CRestore &restore );
	static	TYPEDESCRIPTION m_SaveData[];

	
	static char *m_szFriends[TLK_CFRIENDS];		// array of friend names
	static float g_talkWaitTime;
	
	int			m_bitsSaid;						// set bits for sentences we don't want repeated
	int			m_nSpeak;						// number of times initiated talking
	int			m_voicePitch;					// pitch of voice for this head
	const char	*m_szGrp[TLK_CGROUPS];			// sentence group names
	float		m_useTime;						// Don't allow +USE until this time
	int			m_iszUse;						// Custom +USE sentence group (follow)
	int			m_iszUnUse;						// Custom +USE sentence group (stop following)

	float		m_flLastSaidSmelled;// last time we talked about something that stinks
	float		m_flStopTalkTime;// when in the future that I'll be done saying this sentence.

	//MODDD
	//BOOL goneMad;
	float madYaw;
	BOOL leaderRecentlyDied;
	float followAgainTime;
	int consecutiveFollowFails;
	float followResetFailTime;


	EHANDLE		m_hTalkTarget;	// who to look at while talking
	CUSTOM_SCHEDULES;
};


// Clients can push talkmonsters out of their way
#define		bits_COND_CLIENT_PUSH		( bits_COND_SPECIAL1 )
// Don't see a client right now.
#define		bits_COND_CLIENT_UNSEEN		( bits_COND_SPECIAL2 )


#endif		//TALKMONSTER_H
