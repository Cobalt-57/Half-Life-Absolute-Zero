

#include "squidspit.h"
//#include "weapons.h"

#include	"decals.h"
#include	"game.h"



extern float global_bullsquidSpitTrajTimeMin;
extern float global_bullsquidSpitTrajTimeMax;
extern float global_bullsquidSpitTrajDistMin;
extern float global_bullsquidSpitTrajDistMax;

extern float global_bullsquidSpitGravityMulti;

extern float global_cl_bullsquidspitarc;
extern float global_bullsquidSpitUseAlphaModel;
extern float global_bullsquidSpitUseAlphaEffect;

extern float global_bullsquidSpitEffectSpread;

extern float global_bullsquidSpitEffectMin;
extern float global_bullsquidSpitEffectMax;
extern float global_bullsquidSpitEffectHitMin;
extern float global_bullsquidSpitEffectHitMax;
extern float global_bullsquidSpitEffectSpawn;
extern float global_bullsquidSpitEffectHitSpawn;


extern float global_bullsquidSpitAlphaScale;
extern float global_bullsquidSpitSpriteScale;







CSquidSpit::CSquidSpit(void){

}


int CSquidSpit::iSquidSpitSprite = 0;



LINK_ENTITY_TO_CLASS( squidspit, CSquidSpit );


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
	Vector vecVelDir2D = Vector(
		vecVelDir.x + RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread),
		vecVelDir.y + RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread),
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
	if(global_bullsquidSpitUseAlphaModel == 1){
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

	if(global_bullsquidSpitUseAlphaModel == 1){
		
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



//CSquidSpit::Shoot( pev, vecSpitOffset, vecSpitDir * 900, (m_hEnemy!=NULL&&((tempMon=m_hEnemy->MyMonsterPointer())!=NULL))?m_vecEnemyLKP+tempMon->pev->view_ofs:this->m_vecEnemyLKP, (tempMon!=NULL)?tempMon->pev->mins+tempMon->pev->origin: m_vecEnemyLKP, (tempMon!=NULL)?tempMon->pev->maxs+tempMon->pev->origin: m_vecEnemyLKP   );


void CSquidSpit::Shoot( CBaseMonster* argFiringEntity, Vector vecStart, Vector vecDirection, float argSpeed  ){

	CBaseMonster* tempMon = NULL;



	//Imply a destination too.
	CSquidSpit::Shoot(
		argFiringEntity->pev, vecStart, vecDirection, argSpeed,
		(argFiringEntity->m_hEnemy!=NULL&&((tempMon=argFiringEntity->m_hEnemy->MyMonsterPointer())!=NULL))?argFiringEntity->m_vecEnemyLKP+tempMon->pev->view_ofs:argFiringEntity->m_vecEnemyLKP,
		(tempMon!=NULL)?tempMon->pev->mins+tempMon->pev->origin: argFiringEntity->m_vecEnemyLKP,
		(tempMon!=NULL)?tempMon->pev->maxs+tempMon->pev->origin: argFiringEntity->m_vecEnemyLKP
	);

	//CSquidSpit::Shoot( this, vecSpitOffset, vecSpitDir, 900 );


}//END OF Shoot




//MODDD - new field, the enemy for location information.
void CSquidSpit::Shoot( entvars_t *pevOwner, Vector vecStart, Vector vecDirection, float argSpeed, const Vector& vecDest, const Vector& vecMinBounds, const Vector& vecMaxBounds  )
{
	int i = 0;

	Vector vecVelocity = vecDirection * argSpeed;


	//MODDD NOTICE TODO - should this go below with the first "... == 1" check for the CVar?
	//This means the non-alpha (retail) version of the spit effect will happen regardless of the arc mode. That may be wanted though.
	if(global_bullsquidSpitUseAlphaEffect == 0){
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


	
	if(global_bullsquidSpitUseAlphaModel == 1){
		pSpit->pev->scale = global_bullsquidSpitAlphaScale;
		//SCALE DOES NOT WORK WITH MODELS. OH WELL.
	}else{
		pSpit->pev->scale = global_bullsquidSpitSpriteScale;
	}
	


	if(global_cl_bullsquidspitarc == 1){
		//MODDD - new again.
		pSpit->pev->movetype = MOVETYPE_TOSS;
		pSpit->pev->solid = SOLID_BBOX;
		UTIL_SetSize( pSpit->pev, Vector( 0, 0, 0), Vector(0, 0, 0) );
	}else{
		

		//alpha spit effect will only happen for an arc value of 0 then. This might be intentional.
		if(global_bullsquidSpitUseAlphaEffect == 1){
			//Vector velocityFinalDir = vecVelocity.Normalize();
			//float spitSpeed = vecVelocity.Length() * 0.6;
			
			//Vector vecVelocityNorm = Vector(vecVelocity.x, vecVelocity.y, vecVelocity.z * 2).Normalize();
			Vector vecVelocityNorm = vecVelocity.Normalize();

			for(i = 0; i < global_bullsquidSpitEffectSpawn; i++){

				
				Vector particleDir = getParticleDir(vecVelocityNorm);
				//UTIL_BloodStream(vecStart, (velocityFinalDir + Vector(RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread) ) ) * spitSpeed  , BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectMin, (long)global_bullsquidSpitEffectMax));
				UTIL_BloodStream(vecStart, particleDir, BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectMin, (long)global_bullsquidSpitEffectMax));
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




	//if(targetEnt->IsPlayer() == TRUE){
		
		//dest = vecDest + Vector(RANDOM_FLOAT(-4, 4), RANDOM_FLOAT(-4, 4), RANDOM_FLOAT(-3, 3));

	//get the size of the thing...  reduce a bit, that's our target zone.







	//}else{
	//	dest = targetEnt->pev->origin + targetEnt->pev->view_ofs + Vector(RANDOM_FLOAT(-36, 36), RANDOM_FLOAT(-36, 36), RANDOM_FLOAT(-9, 2));
	//}

	///if(isHead == TRUE){
	//	dest = pevPlayer->origin + pevPlayer->view_ofs + Vector(RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 5), RANDOM_FLOAT(-5, 1));
	//}else{
		
	//}


	Vector distVector = ( dest ) - vecStart;

	Vector distVector2D = Vector(distVector.x, distVector.y, 0);

	Vector towardsPlayer = distVector.Normalize();
	Vector towardsPlayer2D =  distVector2D.Normalize();
				
	float distFloorwise = distVector.Length2D();
	float distVertical = distVector.z;

	//angle...
	/*
	float ang = 0;
	if(distVertical ==0){
		ang = 90 *(M_PI / 180.0);
	}else{
		ang = atan(distVertical / distFloorwise);
	}
	*/

	//velocity must be at least X.
	float velocitySpeed = 0;



	/*
	float timeMin = 0.70;
	float timeMax = 1.56;
	float distMin = 800;
	float distMax = 1400;
	*/


	/*
	bullsquidSpitTrajTimeMin
	bullsquidSpitTrajTimeMax
	bullsquidSpitTrajDistMin
	bullsquidSpitTrajDistMax
	*/

	float timeMin = global_bullsquidSpitTrajTimeMin;
	float timeMax = global_bullsquidSpitTrajTimeMax;
	float distMin = global_bullsquidSpitTrajDistMin;
	float distMax = global_bullsquidSpitTrajDistMax;

	float timeDelta = timeMax - timeMin;
	float distDelta = distMax - distMin;
	
	pSpit->pev->gravity = global_bullsquidSpitGravityMulti;

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





	/*
	if(velocitySpeed < 600){
		velocitySpeed = 600;
	}
	if(velocitySpeed > 900){
		velocitySpeed = 900;
	}

	*/


	/*
	if(isHead){
		pGib->pev->origin = pGib->pev->origin + (towardsPlayer) * 8;

		//little faster.
		velocitySpeed *= 1.11f;
	}
	*/


	//float velocityXComp = cos(ang) * velocity;
	//float velocityYComp = sin(ang) * velocity;

	//Vector velocityFloorwise = velocity * ;

	//110 / 250

	float timeToReachDest = distFloorwise / velocitySpeed;

	//grav?   sv_grav?
				
				
	float gravity = g_psv_gravity->value * (pSpit->pev->gravity != 0?pSpit->pev->gravity:1 );
	//easyForcePrintLine("???GGG %.2f", gravity);
	Vector velocityFinal = towardsPlayer2D * velocitySpeed;
	float velocityVertical = (distVertical + 0.5 * gravity * pow(timeToReachDest, 2.0f ) ) / (timeToReachDest);
	
	//easyForcePrintLine("WHYYYYYY %.2f :: %.2f %.2f %.2f   %.2f %.2f", velocityVertical, distVertical, gravity, timeToReachDest, towardsPlayer.x, towardsPlayer.y);

	velocityFinal.z = velocityVertical * 1.0f;
	velocityFinal.x *= 1.0;
	velocityFinal.y *= 1.0;

	//pGib->pev->velocity.z += 100;


	//Vector tempp = towardsPlayer2D * velocity;

	pSpit->pev->velocity = velocityFinal;
	
	if(global_bullsquidSpitUseAlphaModel == 1){
		//yes, right now.
		pSpit->pev->angles = UTIL_velocityToAngles(pSpit->pev->velocity);
	}

	
	//UTIL_BloodStream(vecSpitOffset, vecSpitDir, BloodColor(), RANDOM_LONG(40, 70));
	//UTIL_BloodStream(vecSpitOffset, vecSpitDir, BloodColor(), RANDOM_LONG(40, 70));
	//UTIL_BloodStream(vecSpitOffset, vecSpitDir, BloodColor(), RANDOM_LONG(40, 70));
	
	//global_bullsquidSpitUseAlphaEffect
	
	if(global_bullsquidSpitUseAlphaEffect == 1){
		
		//velocityFinal.z = velocityFinal.z * 2.6;
		//Vector velocityFinalDir = velocityFinal.Normalize();
		
		
		
		//easyForcePrintLine("THE FINAL SPEEEEEED::: %.2f",velocityFinal.Length());
		float spitSpeed = velocityFinal.Length() * 3;
		
		Vector vecVelocityNorm = velocityFinal.Normalize();

		for(i = 0; i < global_bullsquidSpitEffectSpawn; i++){
			//UTIL_BloodStream(vecStart, (velocityFinalDir + Vector(RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread) ) ) * spitSpeed  , BLOOD_COLOR_YELLOW, RANDOM_LONG(7, 15));
			//UTIL_BloodStream(vecStart, UTIL_RandomBloodVector()  , BLOOD_COLOR_YELLOW, RANDOM_LONG(38, 46));

			//extern float global_testVar;
			
			Vector particleDir = getParticleDir(vecVelocityNorm);

			//UTIL_BloodStream(vecStart, (velocityFinalDir + Vector(RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread) ) ).Normalize(), BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectMin, (long)global_bullsquidSpitEffectMax));
			UTIL_BloodStream(vecStart, particleDir, BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectMin, (long)global_bullsquidSpitEffectMax));
			
		}
	}

}//END OF Shoot



void CSquidSpit :: Touch ( CBaseEntity *pOther )
{
	int i = 0;
	TraceResult tr;
	int		iPitch;

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


		if(global_bullsquidSpitUseAlphaEffect == 0){
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

		}else if(global_bullsquidSpitUseAlphaEffect == 1){

			Vector velocityFinalDir = pev->velocity.Normalize();
			float spitSpeed = pev->velocity.Length() * 0.36;

			Vector velocityFlyOff = tr.vecPlaneNormal;
			//velocityFlyOff.z += pev->velocity.z *0.1;

			Vector velocityFlyOffNorm = velocityFlyOff.Normalize();



			for(i = 0; i < global_bullsquidSpitEffectHitSpawn; i++){
				Vector particleDir = getParticleDir(velocityFlyOffNorm);
				//UTIL_BloodStream(tr.vecEndPos, (velocityFlyOffNorm + Vector(RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread), RANDOM_FLOAT(-global_bullsquidSpitEffectSpread, global_bullsquidSpitEffectSpread) ) ) * spitSpeed  , BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectHitMin, (long)global_bullsquidSpitEffectHitMax));
				UTIL_BloodStream(tr.vecEndPos, particleDir, BLOOD_COLOR_YELLOW, RANDOM_LONG((long)global_bullsquidSpitEffectHitMin, (long)global_bullsquidSpitEffectHitMax));
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
