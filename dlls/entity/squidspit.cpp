
#include "squidspit.h"
//#include "weapons.h"
#include "decals.h"
#include "game.h"

EASY_CVAR_EXTERN(bullsquidSpitTrajTimeMin)
EASY_CVAR_EXTERN(bullsquidSpitTrajTimeMax)
EASY_CVAR_EXTERN(bullsquidSpitTrajDistMin)
EASY_CVAR_EXTERN(bullsquidSpitTrajDistMax)

EASY_CVAR_EXTERN(bullsquidSpitGravityMulti)

EASY_CVAR_EXTERN(cl_bullsquidspitarc)
EASY_CVAR_EXTERN(bullsquidSpitUseAlphaModel)
EASY_CVAR_EXTERN(bullsquidSpitUseAlphaEffect)

EASY_CVAR_EXTERN(bullsquidSpitEffectSpread)

EASY_CVAR_EXTERN(bullsquidSpitEffectMin)
EASY_CVAR_EXTERN(bullsquidSpitEffectMax)
EASY_CVAR_EXTERN(bullsquidSpitEffectHitMin)
EASY_CVAR_EXTERN(bullsquidSpitEffectHitMax)
EASY_CVAR_EXTERN(bullsquidSpitEffectSpawn)
EASY_CVAR_EXTERN(bullsquidSpitEffectHitSpawn)

EASY_CVAR_EXTERN(bullsquidSpitAlphaScale)
EASY_CVAR_EXTERN(bullsquidSpitSpriteScale)


LINK_ENTITY_TO_CLASS( squidspit, CSquidSpit );


int CSquidSpit::iSquidSpitSprite = 0;



CSquidSpit::CSquidSpit(void){

}




//IMPORTANT - dummied out!
GENERATE_TRACEATTACK_IMPLEMENTATION_DUMMY(CSquidSpit)
GENERATE_TAKEDAMAGE_IMPLEMENTATION_DUMMY(CSquidSpit)




TYPEDESCRIPTION	CSquidSpit::m_SaveData[] = 
{
	DEFINE_FIELD( CSquidSpit, m_maxFrame, FIELD_INTEGER ),
};

IMPLEMENT_SAVERESTORE( CSquidSpit, CBaseEntity );


BOOL CSquidSpit::usesSoundSentenceSave(void){
	return TRUE;
}


Vector getParticleDir(const Vector& vecVelDir){
	float effectSpread = EASY_CVAR_GET(bullsquidSpitEffectSpread);
	
	Vector vecVelDir2D = Vector(
		vecVelDir.x + RANDOM_FLOAT(-effectSpread, effectSpread),
		vecVelDir.y + RANDOM_FLOAT(-effectSpread, effectSpread),
		0).Normalize();

	return Vector(vecVelDir2D.x, vecVelDir2D.y, 1);
}



void CSquidSpit:: Spawn( void )
{
	pev->movetype = MOVETYPE_FLY;
	pev->classname = MAKE_STRING( "squidspit" );
	
	pev->solid = SOLID_BBOX;
	pev->rendermode = kRenderTransAlpha;
	pev->renderamt = 255;

	
	//MODDD - how's this?
	if(EASY_CVAR_GET(bullsquidSpitUseAlphaModel) == 1){
		SET_MODEL(ENT(pev), "models/spit.mdl");
	}else{
		SET_MODEL(ENT(pev), "sprites/bigspit.spr");
	}


	pev->frame = 0;
	pev->scale = 0.5;

	UTIL_SetSize( pev, Vector( 0, 0, 0), Vector(0, 0, 0) );

	m_maxFrame = (float) MODEL_FRAMES( pev->modelindex ) - 1;

}


//MODDD - different from the typical "precache" method. This one's static to be called by another class's precache instead.
//Such as the bullsquid's precache, since it expects to spawn these projectiles.
//This way they don't have to store the "iSquidSpitSprite", that's left up to this file.
void CSquidSpit::precacheStatic(void){
	CSquidSpit::iSquidSpitSprite = PRECACHE_MODEL("sprites/tinyspit.spr");// client side spittle.
	
	PRECACHE_MODEL("sprites/bigspit.spr");// spit projectile.
	PRECACHE_MODEL("models/spit.mdl");
}//END OF precacheStatic




//#include "cbase.h"

void CSquidSpit::Animate( void )
{

	if(EASY_CVAR_GET(bullsquidSpitUseAlphaModel) == 1){
		
		StudioFrameAdvance( );
		pev->angles = UTIL_velocityToAngles(pev->velocity);
	}else{
		
		//sprite.
		if ( pev->frame++ )
		{
			if ( pev->frame > m_maxFrame )
			{
				pev->frame = 0;
			}
		}

	}//END OF model / spirte checks

	pev->nextthink = gpGlobals->time + 0.1;


	
	//this->pev->gravity = 1;
	//easyForcePrintLine("MAH GRAV: %.2f", this->pev->gravity);
}


void CSquidSpit::Shoot( CBaseMonster* argFiringEntity, Vector vecStart, Vector vecDirection, float argSpeed  ){
	CBaseMonster* tempMon = NULL;

	//Imply a destination too.
	CSquidSpit::Shoot(
		argFiringEntity->pev, vecStart, vecDirection, argSpeed,
		(argFiringEntity->m_hEnemy!=NULL&&((tempMon=argFiringEntity->m_hEnemy->MyMonsterPointer())!=NULL))?argFiringEntity->m_vecEnemyLKP+tempMon->pev->view_ofs:argFiringEntity->m_vecEnemyLKP,
		(tempMon!=NULL)?tempMon->pev->mins+tempMon->pev->origin: argFiringEntity->m_vecEnemyLKP,
		(tempMon!=NULL)?tempMon->pev->maxs+tempMon->pev->origin: argFiringEntity->m_vecEnemyLKP
	);
}//END OF Shoot



//MODDD - new field, the enemy for location information.
void CSquidSpit::Shoot( entvars_t *pevOwner, Vector vecStart, Vector vecDirection, float argSpeed, const Vector& vecDest, const Vector& vecMinBounds, const Vector& vecMaxBounds  )
{
	int i = 0;
	Vector vecVelocity = vecDirection * argSpeed;

	//MODDD NOTICE TODO - should this go below with the first "... == 1" check for the CVar?
	//This means the non-alpha (retail) version of the spit effect will happen regardless of the arc mode. That may be wanted though.
	if(EASY_CVAR_GET(bullsquidSpitUseAlphaEffect) == 0){
		//no alpha effect? retail will just do this.
		//MODDD - use the blood particles instead.
		// spew the spittle temporary ents.
		MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, vecStart );
			WRITE_BYTE( TE_SPRITE_SPRAY );
			WRITE_COORD( vecStart.x);	// pos
			WRITE_COORD( vecStart.y);	
			WRITE_COORD( vecStart.z);	
			WRITE_COORD( vecDirection.x);	// dir
			WRITE_COORD( vecDirection.y);	
			WRITE_COORD( vecDirection.z);	
			WRITE_SHORT( CSquidSpit::iSquidSpitSprite );	// model
			WRITE_BYTE ( 15 );			// count
			WRITE_BYTE ( 210 );			// speed
			WRITE_BYTE ( 25 );			// noise ( client will divide by 100 )
		MESSAGE_END();
	}

	CSquidSpit *pSpit = GetClassPtr( (CSquidSpit *)NULL );
	pSpit->Spawn();
	
	UTIL_SetOrigin( pSpit->pev, vecStart );
	pSpit->pev->velocity = vecVelocity;
	pSpit->pev->owner = ENT(pevOwner);

	pSpit->SetThink ( &CSquidSpit::Animate );
	pSpit->pev->nextthink = gpGlobals->time + 0.1;


	
	if(EASY_CVAR_GET(bullsquidSpitUseAlphaModel) == 1){
		pSpit->pev->scale = EASY_CVAR_GET(bullsquidSpitAlphaScale);
		//SCALE DOES NOT WORK WITH MODELS. OH WELL.
	}else{
		pSpit->pev->scale = EASY_CVAR_GET(bullsquidSpitSpriteScale);
	}
	


	if(EASY_CVAR_GET(cl_bullsquidspitarc) == 1){
		//MODDD - new again.
		pSpit->pev->movetype = MOVETYPE_TOSS;
		pSpit->pev->solid = SOLID_BBOX;
		UTIL_SetSize( pSpit->pev, Vector( 0, 0, 0), Vector(0, 0, 0) );
	}else{
		

		//alpha spit effect will only happen for an arc value of 0 then. This might be intentional.
		if(EASY_CVAR_GET(bullsquidSpitUseAlphaEffect) == 1){
			//Vector velocityFinalDir = vecVelocity.Normalize();
			//float spitSpeed = vecVelocity.Length() * 0.6;
			
			//Vector vecVelocityNorm = Vector(vecVelocity.x, vecVelocity.y, vecVelocity.z * 2).Normalize();
			Vector vecVelocityNorm = vecVelocity.Normalize();

			for(i = 0; i < EASY_CVAR_GET(bullsquidSpitEffectSpawn); i++){
				Vector particleDir = getParticleDir(vecVelocityNorm);
				
				UTIL_SpawnBlood(vecStart, particleDir, COLOR_SQUIDSPIT, RANDOM_LONG((long)EASY_CVAR_GET(bullsquidSpitEffectMin), (long)EASY_CVAR_GET(bullsquidSpitEffectMax) ));
			}
		}

		UTIL_SetSize( pSpit->pev, Vector( 0, 0, 0), Vector(0, 0, 0) );
		//everything above is good enough.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//IMPORTANT! EARLY TERMINATION HERE. Below is stuff for figuring out how to shoot to arc towards the target correctly (gravity).
		//Clearly a bullsquidspitarc of 0 (or really non-1) tells us not to do that.
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		return;
	}

	/*
	if(targetEnt == NULL){
		//just rely on the "vecVelocity" already assigned?
		return;
	}
	*/

	Vector dest;
	//EyePosition( ) { return pev->origin + pev->view_ofs;

	Vector vecEnemySize = vecMinBounds - vecMaxBounds;

	if(vecEnemySize.x > 10 && vecEnemySize.y > 10 && vecEnemySize.z > 7){
		
	dest = Vector(
		RANDOM_FLOAT(vecMinBounds.x + 2.5, vecMaxBounds.x - 2.5),
		RANDOM_FLOAT(vecMinBounds.y + 2.5, vecMaxBounds.y - 2.5),
		RANDOM_FLOAT(vecMinBounds.z + 2.5, vecMaxBounds.z - 2.5)
		);

	}else{
		//size i too small, different.
		dest = vecDest + Vector(RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-2, 4));
	}


	Vector distVector = ( dest ) - vecStart;

	Vector distVector2D = Vector(distVector.x, distVector.y, 0);

	Vector towardsPlayer = distVector.Normalize();
	Vector towardsPlayer2D =  distVector2D.Normalize();
				
	float distFloorwise = distVector.Length2D();
	float distVertical = distVector.z;

	//velocity must be at least X.
	float velocitySpeed = 0;

	/*
	float timeMin = 0.70;
	float timeMax = 1.56;
	float distMin = 800;
	float distMax = 1400;
	*/

	float timeMin = EASY_CVAR_GET(bullsquidSpitTrajTimeMin);
	float timeMax = EASY_CVAR_GET(bullsquidSpitTrajTimeMax);
	float distMin = EASY_CVAR_GET(bullsquidSpitTrajDistMin);
	float distMax = EASY_CVAR_GET(bullsquidSpitTrajDistMax);

	float timeDelta = timeMax - timeMin;
	float distDelta = distMax - distMin;
	
	pSpit->pev->gravity = EASY_CVAR_GET(bullsquidSpitGravityMulti);

	//easyForcePrintLine("MAH GRAV: %.2f", this->pev->gravity);

	//how long do we want to take to reach the target?
	if(distFloorwise < distMin){
		//just minimum speed, whatever the time is (at minimum, hits in 0.6 seconds).  That is, 600 * 1.25.
		velocitySpeed = distMin / timeMin;
	}else if(distFloorwise > distMax){
		//too great?  cap it.  whatever the time is (at maximum, hits in 1.6 seconds).
		velocitySpeed = distMax / timeMax;   //about 1500 * 0.625, for reaching in 1.6 seconds.
	}else{
		//inbetween?  Let's scale the time it takes...
		//600 - 1500...
		float filter = (distFloorwise - distMin) / distDelta;
		//time range: 0.8 - 1.6.
		filter = filter*timeDelta + timeMin;
		
		//velocitySpeed = distFloorwise * 1.25; //hit in 0.8 of a second.

		velocitySpeed = distFloorwise * filter;
	}


	float timeToReachDest = distFloorwise / velocitySpeed;

	//grav?   sv_grav?
	float gravity = g_psv_gravity->value * (pSpit->pev->gravity != 0?pSpit->pev->gravity:1 );
	//easyForcePrintLine("???GGG %.2f", gravity);
	Vector velocityFinal = towardsPlayer2D * velocitySpeed;
	float velocityVertical = (distVertical + 0.5 * gravity * pow(timeToReachDest, 2.0f ) ) / (timeToReachDest);
	
	velocityFinal.z = velocityVertical * 1.0f;
	velocityFinal.x *= 1.0;
	velocityFinal.y *= 1.0;

	pSpit->pev->velocity = velocityFinal;
	
	if(EASY_CVAR_GET(bullsquidSpitUseAlphaModel) == 1){
		//yes, right now.
		pSpit->pev->angles = UTIL_velocityToAngles(pSpit->pev->velocity);
	}

	if(EASY_CVAR_GET(bullsquidSpitUseAlphaEffect) == 1){
		float spitSpeed = velocityFinal.Length() * 3;
		
		Vector vecVelocityNorm = velocityFinal.Normalize();

		for(i = 0; i < EASY_CVAR_GET(bullsquidSpitEffectSpawn); i++){
			Vector particleDir = getParticleDir(vecVelocityNorm);
			UTIL_SpawnBlood(vecStart, particleDir, COLOR_SQUIDSPIT, RANDOM_LONG((long)EASY_CVAR_GET(bullsquidSpitEffectMin), (long)EASY_CVAR_GET(bullsquidSpitEffectMax) ));
		}
	}

}//END OF Shoot



void CSquidSpit :: Touch ( CBaseEntity *pOther )
{
	int i = 0;
	TraceResult tr;
	int	iPitch;

	// splat sound
	iPitch = RANDOM_FLOAT( 90, 110 );

	EMIT_SOUND_FILTERED( ENT(pev), CHAN_VOICE, "bullchicken/bc_acid1.wav", 1, ATTN_NORM, 0, iPitch );	

	switch ( RANDOM_LONG( 0, 1 ) )
	{
	case 0:
		EMIT_SOUND_FILTERED( ENT(pev), CHAN_WEAPON, "bullchicken/bc_spithit1.wav", 1, ATTN_NORM, 0, iPitch );	
		break;
	case 1:
		EMIT_SOUND_FILTERED( ENT(pev), CHAN_WEAPON, "bullchicken/bc_spithit2.wav", 1, ATTN_NORM, 0, iPitch );	
		break;
	}

	if ( !pOther->pev->takedamage )
	{

		// make a splat on the wall
		UTIL_TraceLine( pev->origin, pev->origin + pev->velocity * 10, dont_ignore_monsters, ENT( pev ), &tr );
		UTIL_DecalTrace(&tr, DECAL_SPIT1 + RANDOM_LONG(0,1));


		if(EASY_CVAR_GET(bullsquidSpitUseAlphaEffect) == 0){
			// make some flecks
			MESSAGE_BEGIN( MSG_PVS, SVC_TEMPENTITY, tr.vecEndPos );
				WRITE_BYTE( TE_SPRITE_SPRAY );
				WRITE_COORD( tr.vecEndPos.x);	// pos
				WRITE_COORD( tr.vecEndPos.y);	
				WRITE_COORD( tr.vecEndPos.z);	
				WRITE_COORD( tr.vecPlaneNormal.x);	// dir
				WRITE_COORD( tr.vecPlaneNormal.y);	
				WRITE_COORD( tr.vecPlaneNormal.z);	
				WRITE_SHORT( CSquidSpit::iSquidSpitSprite );	// model
				WRITE_BYTE ( 5 );			// count
				WRITE_BYTE ( 30 );			// speed
				WRITE_BYTE ( 80 );			// noise ( client will divide by 100 )
			MESSAGE_END();

		}else if(EASY_CVAR_GET(bullsquidSpitUseAlphaEffect) == 1){

			Vector velocityFinalDir = pev->velocity.Normalize();
			float spitSpeed = pev->velocity.Length() * 0.36;

			Vector velocityFlyOff = tr.vecPlaneNormal;
			//velocityFlyOff.z += pev->velocity.z *0.1;

			Vector velocityFlyOffNorm = velocityFlyOff.Normalize();



			for(i = 0; i < EASY_CVAR_GET(bullsquidSpitEffectHitSpawn); i++){
				Vector particleDir = getParticleDir(velocityFlyOffNorm);
				UTIL_SpawnBlood(tr.vecEndPos, particleDir, COLOR_SQUIDSPIT, RANDOM_LONG((long)EASY_CVAR_GET(bullsquidSpitEffectHitMin), (long)EASY_CVAR_GET(bullsquidSpitEffectHitMax)));
			}

		}

	}
	else
	{
		//MODDD - bullsquid spit does toxic (poison) damage.
		//pOther->TakeDamage ( pev, pev, gSkillData.bullsquidDmgSpit, DMG_GENERIC );

		//MODDD - why wasn't the first set to pev->owner? don't we want to tell whoever took damage who dealt it?
		//Inspired by the hornet which does this right.
		pOther->TakeDamage ( pev, VARS( pev->owner ), gSkillData.bullsquidDmgSpit, DMG_POISON );
	}

	SetThink ( &CBaseEntity::SUB_Remove );
	pev->nextthink = gpGlobals->time;
}




float CSquidSpit::massInfluence(void){
	return 0.03f;
}//END OF massInfluence

int CSquidSpit::GetProjectileType(void){
	return PROJECTILE_ORGANIC_DUMB;
}







