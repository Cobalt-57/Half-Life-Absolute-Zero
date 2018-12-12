




//NOTICE - this file is not included by the client. Assume serverside.




#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "basemonster.h"
#include "weapons.h"
#include "nodes.h"
#include "player.h"
#include "gamerules.h"


LINK_ENTITY_TO_CLASS( crossbow_bolt, CCrossbowBolt );



EASY_CVAR_EXTERN(playerForceCrossbowMode)
EASY_CVAR_EXTERN(hassassinForceCrossbowMode)
EASY_CVAR_EXTERN(crossbowInheritsPlayerVelocity)
//EASY_CVAR_EXTERN(crossbowReloadSoundDelay)


CCrossbowBolt *CCrossbowBolt::BoltCreate( void )
{
	// Create a new entity with CCrossbowBolt private data
	CCrossbowBolt *pBolt = GetClassPtr( (CCrossbowBolt *)NULL );
	pBolt->pev->classname = MAKE_STRING("bolt");
	pBolt->Spawn();

	return pBolt;
}

void CCrossbowBolt::Spawn( )
{
	Precache( );
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	//MODDD - making these "Parabolic bolts" is easy!  Just change pev->movetype to MOVETYPE_TOSS !

	pev->gravity = 0.5;

	SET_MODEL(ENT(pev), "models/crossbow_bolt.mdl");
	
	//MODDD - this section has been added.
	///////////////////////////////////////////////////////
	MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);

	WRITE_BYTE(TE_BEAMFOLLOW);
	WRITE_SHORT(entindex());	// entity
	WRITE_SHORT(m_iTrail);	// model
	WRITE_BYTE(10); // life
	WRITE_BYTE(1.5);  // width
	WRITE_BYTE(224);   // r, g, b
	WRITE_BYTE(224);   // r, g, b
	WRITE_BYTE(255);   // r, g, b
	WRITE_BYTE(100);	// brightness
	
	MESSAGE_END();
	///////////////////////////////////////////////////////

	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));

	SetTouch( &CCrossbowBolt::BoltTouch );
	SetThink( &CCrossbowBolt::BubbleThink );
	
	//MODDD NOTE - why is this " + 0.2" instead of " + 0.1" like most think methods and even BubbleThink's own think refresh?
	//             The world may never know.
	pev->nextthink = gpGlobals->time + 0.2;
}


void CCrossbowBolt::Precache( )
{
	PRECACHE_MODEL ("models/crossbow_bolt.mdl");
	PRECACHE_SOUND("weapons/xbow_hitbod1.wav");
	PRECACHE_SOUND("weapons/xbow_hitbod2.wav");
	PRECACHE_SOUND("weapons/xbow_fly1.wav");
	PRECACHE_SOUND("weapons/xbow_hit1.wav");
	PRECACHE_SOUND("fvox/beep.wav");

	//MODDD - used to be this:
	//m_iTrail = PRECACHE_MODEL("sprites/streak.spr");
	m_iTrail = PRECACHE_MODEL("sprites/smoke.spr");
}


int	CCrossbowBolt :: Classify ( void )
{
	return	CLASS_NONE;
}

void CCrossbowBolt::BoltTouch( CBaseEntity *pOther )
{
	BOOL goingToExplode = FALSE;
	int iBitsDamage = 0;


	float* cvarToUse = NULL;

	if( pev->owner != NULL){
		const char* ownerClassName = STRING(pev->owner->v.classname);
		if( !strcmp(ownerClassName, "player")){
			cvarToUse = &EASY_CVAR_GET(playerForceCrossbowMode);
		}else if(!strcmp(ownerClassName, "monster_human_assassin")){
			cvarToUse = &EASY_CVAR_GET(hassassinForceCrossbowMode);
		}
	}
	
	//MODDD - filter w/ new CVar.

	if(cvarToUse != NULL){
		if((*cvarToUse)!=2 && ((*cvarToUse)==1 || !WEAPON_DEFAULT_MULTIPLAYER_CHECK) ){
			//singleplayer mode? nothing special.

		}else{
			goingToExplode = TRUE;
			//plain bolts explode in multiplayer mode.
		}
	}

	
	SetTouch( NULL );
	SetThink( NULL );

	if (pOther->pev->takedamage)
	{
		TraceResult tr = UTIL_GetGlobalTrace( );
		entvars_t	*pevOwner;



		pevOwner = VARS( pev->owner );

		// UNDONE: this needs to call TraceAttack instead
		ClearMultiDamage( );
		//MODDD - added DMG_POISON.
		gMultiDamage.type = DMG_BULLET | DMG_POISON;




		if(goingToExplode){
			//explosions gib. Look like it.
			gMultiDamage.type |= DMG_BLAST | DMG_ALWAYSGIB;
		}else{
			gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB | DMG_POISON;
		}




		if ( pOther->IsPlayer() )
		{
			//iBitsDamage = DMG_NEVERGIB;
			//easyForcePrintLine("OW! 1A %s health:%.2f dmg:%.2f", pOther->getClassname(), pOther->pev->health, gSkillData.plrDmgCrossbowClient);
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgCrossbowClient, pev->velocity.Normalize(), &tr, gMultiDamage.type ); 
			//easyForcePrintLine("OW! 2A %s health:%.2f", pOther->getClassname(), pOther->pev->health);
		}
		else
		{
			//iBitsDamage = DMG_BULLET | DMG_NEVERGIB;
			//easyForcePrintLine("OW! 1B %s health:%.2f dmg:%.2f", pOther->getClassname(), pOther->pev->health, gSkillData.plrDmgCrossbowMonster);
			pOther->TraceAttack(pevOwner, gSkillData.plrDmgCrossbowMonster, pev->velocity.Normalize(), &tr, gMultiDamage.type ); 
			//easyForcePrintLine("OW! 2B %s health:%.2f", pOther->getClassname(), pOther->pev->health);
		}


		//ClearMultiDamage();
		//gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;
		
		//easyForcePrintLine("OW! 3 %s health:%.2f", pOther->getClassname(), pOther->pev->health);
		//easyForcePrintLine("!!!CROSSBOWBOLT: ApplyMultiDamage CALL!!!");
		ApplyMultiDamage( pev, pevOwner );
		
		//easyForcePrintLine("OW! 4 %s health:%.2f", pOther->getClassname(), pOther->pev->health);


		pev->velocity = Vector( 0, 0, 0 );
		// play body "thwack" sound
		switch( RANDOM_LONG(0,1) )
		{
		case 0:
			EMIT_SOUND(ENT(pev), CHAN_BODY, "weapons/xbow_hitbod1.wav", 1, ATTN_NORM); break;
		case 1:
			EMIT_SOUND(ENT(pev), CHAN_BODY, "weapons/xbow_hitbod2.wav", 1, ATTN_NORM); break;
		}

		
		if(!goingToExplode){
			//single player? do this. But to myself, the crossbowbolt, to be clear. Multiplayer one's don't need to call this apparently.
			Killed( pev, pev, GIB_NEVER );
		}else{
			//not for multiplayer.

		}
		
	}
	else
	{
		EMIT_SOUND_DYN(ENT(pev), CHAN_BODY, "weapons/xbow_hit1.wav", RANDOM_FLOAT(0.95, 1.0), ATTN_NORM, 0, 98 + RANDOM_LONG(0,7));

		SetThink( &CBaseEntity::SUB_Remove );
		pev->nextthink = gpGlobals->time;// this will get changed below if the bolt is allowed to stick in what it hit.

		if ( FClassnameIs( pOther->pev, "worldspawn" ) )
		{
			// if what we hit is static architecture, can stay around for a while.
			Vector vecDir = pev->velocity.Normalize( );
			UTIL_SetOrigin( pev, pev->origin - vecDir * 12 );
			pev->angles = UTIL_VecToAngles( vecDir );
			pev->solid = SOLID_NOT;
			pev->movetype = MOVETYPE_FLY;
			pev->velocity = Vector( 0, 0, 0 );
			pev->avelocity.z = 0;
			pev->angles.z = RANDOM_LONG(0,360);
			pev->nextthink = gpGlobals->time + 10.0;
		}

		if (UTIL_PointContents(pev->origin) != CONTENTS_WATER)
		{
			UTIL_Sparks2( pev->origin, DEFAULT_SPARK_BALLS, global_sparksPlayerCrossbowMulti );
		}
	}

	if(goingToExplode){
		SetThink( &CCrossbowBolt::ExplodeThink );
		pev->nextthink = gpGlobals->time + 0.1;
	}
	
	
	//easyForcePrintLine("OW! 5 %s health:%.2f", pOther->getClassname(), pOther->pev->health);
}

void CCrossbowBolt::BubbleThink( void )
{
	pev->nextthink = gpGlobals->time + 0.1;

	if (pev->waterlevel == 0)
		return;

	UTIL_BubbleTrail( pev->origin - pev->velocity * 0.1, pev->origin, 1 );
}

void CCrossbowBolt::ExplodeThink( void )
{
	int iContents = UTIL_PointContents ( pev->origin );
	int iScale;
	
	pev->dmg = 40;
	iScale = 10;



	short spriteChosen;
	if (iContents != CONTENTS_WATER)
	{
		spriteChosen = g_sModelIndexFireball;
	}
	else
	{
		spriteChosen = g_sModelIndexWExplosion;
	}

	//MODDD - sent through filter.
	UTIL_Explosion(pev, pev->origin, spriteChosen, iScale, 15, TE_EXPLFLAG_NONE);
	

	entvars_t *pevOwner;

	if ( pev->owner )
		pevOwner = VARS( pev->owner );
	else
		pevOwner = NULL;

	pev->owner = NULL; // can't traceline attack owner if this is set

	//easyForcePrintLine("!!!CROSSBOWBOLT: RADIUS DAMAGE CALL!!!");
	::RadiusDamage( pev->origin, pev, pevOwner, pev->dmg, 128, CLASS_NONE, DMG_BLAST | DMG_ALWAYSGIB );

	UTIL_Remove(this);
}



float CCrossbowBolt::massInfluence(void){
	return 0.03f;
}//END OF massInfluence



