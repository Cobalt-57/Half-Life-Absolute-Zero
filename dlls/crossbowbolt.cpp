




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
EASY_CVAR_EXTERN(crossbowBoltDirectionAffectedByWater)
//EASY_CVAR_EXTERN(crossbowReloadSoundDelay)


CCrossbowBolt::CCrossbowBolt(void){
	recentVelocity = Vector(0,0,0);
	noDamage = FALSE;
	realNextThink = 0;
}//END OF CCrossbowBolt constructor


CCrossbowBolt *CCrossbowBolt::BoltCreate(const Vector& arg_velocity, float arg_speed ){
	return BoltCreate(arg_velocity, arg_speed, TRUE, FALSE);
}
CCrossbowBolt *CCrossbowBolt::BoltCreate(const Vector& arg_velocity, float arg_speed, BOOL useTracer, BOOL arg_noDamage ){
	// Create a new entity with CCrossbowBolt private data
	CCrossbowBolt *pBolt = GetClassPtr( (CCrossbowBolt *)NULL );
	pBolt->pev->classname = MAKE_STRING("bolt");

	
	pBolt->m_velocity = arg_velocity;
	pBolt->m_speed = arg_speed;

	pBolt->pev->velocity = arg_velocity;
	pBolt->pev->speed = arg_speed;

	pBolt->Spawn(useTracer, arg_noDamage);


	return pBolt;
}




void CCrossbowBolt::Spawn(){
	Spawn(TRUE, FALSE);
}

void CCrossbowBolt::Spawn(BOOL useTracer, BOOL arg_noDamage)
{
	noDamage = arg_noDamage;

	Precache( );
	pev->movetype = MOVETYPE_FLY;
	pev->solid = SOLID_BBOX;

	//MODDD - making these "Parabolic bolts" is easy!  Just change pev->movetype to MOVETYPE_TOSS !

	pev->gravity = 0.5;

	SET_MODEL(ENT(pev), "models/crossbow_bolt.mdl");
	

	//MODDD - this section has been added.
	///////////////////////////////////////////////////////
	if(useTracer){

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
	}//END OF useTracer check
	///////////////////////////////////////////////////////


	UTIL_SetOrigin( pev, pev->origin );
	UTIL_SetSize(pev, Vector(0, 0, 0), Vector(0, 0, 0));

	SetTouch( &CCrossbowBolt::BoltTouch );
	SetThink( &CCrossbowBolt::BubbleThink );
	

	//MODDD NOTE - why is this " + 0.2" instead of " + 0.1" like most think methods and even BubbleThink's own think refresh?
	//             The world may never know.
	//pev->nextthink = gpGlobals->time + 0.2;
	realNextThink = gpGlobals->time + 0.2f;
	pev->nextthink = gpGlobals->time;
	
}


void CCrossbowBolt::Precache( )
{
	PRECACHE_MODEL ("models/crossbow_bolt.mdl");
	PRECACHE_SOUND("weapons/xbow_hitbod1.wav");
	PRECACHE_SOUND("weapons/xbow_hitbod2.wav");
	
	//MODDD - do we even actually ever play this? Not even in clientside events (cl_dll/ev_hldm.cpp)? 
	//        That's ok, this sounds terrible anyways. At least at the same default volume as everything else which is blisteringly loud in any sound player for some reason.
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

	//MODDD - is that safe?
	recentVelocity = pev->velocity;



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


		if(!noDamage){

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
				pOther->TraceAttack(pevOwner, gSkillData.plrDmgCrossbowClient, pev->velocity.Normalize(), &tr, gMultiDamage.type, gMultiDamage.typeMod ); 
				//easyForcePrintLine("OW! 2A %s health:%.2f", pOther->getClassname(), pOther->pev->health);
			}
			else
			{
				//iBitsDamage = DMG_BULLET | DMG_NEVERGIB;
				//easyForcePrintLine("OW! 1B %s health:%.2f dmg:%.2f", pOther->getClassname(), pOther->pev->health, gSkillData.plrDmgCrossbowMonster);
				pOther->TraceAttack(pevOwner, gSkillData.plrDmgCrossbowMonster, pev->velocity.Normalize(), &tr, gMultiDamage.type, gMultiDamage.typeMod ); 
				//easyForcePrintLine("OW! 2B %s health:%.2f", pOther->getClassname(), pOther->pev->health);
			}

			//ClearMultiDamage();
			//gMultiDamage.type = DMG_BULLET | DMG_NEVERGIB;
		
			//easyForcePrintLine("OW! 3 %s health:%.2f", pOther->getClassname(), pOther->pev->health);
			//easyForcePrintLine("!!!CROSSBOWBOLT: ApplyMultiDamage CALL!!!");
			ApplyMultiDamage( pev, pevOwner );

		}//END OF noDamage

		
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

		//MODDD - can stick to other map-related things so long as they don't move.
		//if ( FClassnameIs( pOther->pev, "worldspawn" ) )
		//if(pOther->IsWorldOrAffiliated() && pOther->pev->movetype == MOVETYPE_NONE)
		//...why do some static map things use MOVETYPE_PUSH? foget this.
		if(pOther->IsWorld())
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


//this Think is picked for the bolt anytime, regardless of being in the water or not.
//It only makes bubbles if it is underwater.
void CCrossbowBolt::BubbleThink( void )
{


	//think every single frame of game logic, but only run the retail logic every 0.1 seconds for the same behavior there.
	pev->nextthink = gpGlobals->time;
	
	
	if(EASY_CVAR_GET(crossbowBoltDirectionAffectedByWater) != 1){
		//forcing velocity to the one that fired me's intention every frame can preserve direction underwater.
		pev->velocity = m_velocity;
	}



	if(gpGlobals->time >= realNextThink ){
		//one typical think cycle.
		realNextThink = gpGlobals->time + 0.1;	



		if (pev->waterlevel == 0)
			return;

		UTIL_BubbleTrail( pev->origin - pev->velocity * 0.1, pev->origin, 1 );

	}//END OF realNextThink check


	

	
}//END OF BubbleThink


void CCrossbowBolt::ExplodeThink( void )
{
	int iContents = UTIL_PointContents ( pev->origin );
	int iScale;
	
	
	//NEW?
	int shrapMod = 1;  //safe?
	Vector vecSpot;


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



	//MODDD - mimicking how ggrenade.cpp's Explode method, called by some touch method (ExplodeTouch) does it with its trace it sends.
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TraceResult pTrace;
	vecSpot = pev->origin - recentVelocity.Normalize() * 32;
	UTIL_TraceLine( vecSpot, vecSpot + recentVelocity.Normalize() * 64, ignore_monsters, ENT(pev), &pTrace );
	
	//MODDD - new
	Vector explosionEffectStartRetail = pev->origin;
	Vector explosionEffectStartQuake = pev->origin;


	Vector explosionOrigin = pev->origin; //by default.

	// Pull out of the wall a bit
	if ( pTrace.flFraction != 1.0 )
	{

		//MODDD - let's not change our own origin, just record this.
		//pev->origin = ...
		//used to double as both the explosionOrigin and explosionEffectStartRetail ?
		explosionOrigin = pTrace.vecEndPos + (pTrace.vecPlaneNormal * (pev->dmg - 24) * 0.6);
		explosionEffectStartRetail = pTrace.vecEndPos + (pTrace.vecPlaneNormal * (pev->dmg*1.1 - 24) * 0.6);

		//MODDD
		explosionEffectStartQuake = pTrace.vecEndPos + (pTrace.vecPlaneNormal * 5);
		
	}

	//...
	CSoundEnt::InsertSound ( bits_SOUND_COMBAT, explosionOrigin, NORMAL_EXPLOSION_VOLUME / 2.0f, 3.0 );
	entvars_t *pevOwner;
	if ( pev->owner )
		pevOwner = VARS( pev->owner );
	else
		pevOwner = NULL;
	//...


	UTIL_Explosion(pev, explosionEffectStartRetail, spriteChosen, iScale, 15, TE_EXPLFLAG_NONE, explosionEffectStartQuake, shrapMod );
	//MODDD - sending explosionOrigin instead of defaulting to pev->origin.
	//RadiusDamageAutoRadius ( explosionOrigin, pev, pevOwner, pev->dmg, CLASS_NONE, bitsDamageType, bitsDamageTypeMod );
	RadiusDamage( explosionOrigin, pev, pevOwner, pev->dmg, 128, CLASS_NONE, DMG_BLAST | DMG_ALWAYSGIB );
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////










	/*
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
	*/



	UTIL_Remove(this);
}



float CCrossbowBolt::massInfluence(void){
	return 0.03f;
}//END OF massInfluence



