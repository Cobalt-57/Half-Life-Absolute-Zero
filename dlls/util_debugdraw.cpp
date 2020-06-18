


#include "extdll.h"
#include "util_debugdraw.h"

#include "util.h"

#include "vector_shared.h"
#include "util_printout.h"




//or "always the first time"
int DebugLine_drawTime = -1;

static int nextDebugLineID = 0;
static int nextDebugPathTrackDrawID = 0;

DebugDrawable aryDebugLines[DEBUG_LINES_MAX];
DebugDrawable_pathTrack aryDebugLines_pathTrack[DEBUG_PATHTRACK_DRAW_MAX];



void DebugLine::checkDrawLine(void){
	if(canDraw){
		UTIL_drawLineFrame( v1x, v1y, v1z, v2x, v2y, v2z, width, r, g, b);
		//UTIL_drawLineFrame( roundToNearest(v1x), roundToNearest(v1y), roundToNearest(v1z), roundToNearest(v2x), roundToNearest(v2y), roundToNearest(v2z), width, r, g, b);
	}
}
void DebugLine::setup(BOOL argCanDraw, int argR, int argG, int argB, int argWidth, float argV1x, float argV1y, float argV1z, float argV2x, float argV2y, float argV2z){
	canDraw = argCanDraw;
	r = argR;
	g = argG;
	b = argB;
	width = argWidth;
	v1x = argV1x;
	v1y = argV1y;
	v1z = argV1z;
	v2x = argV2x;
	v2y = argV2y;
	v2z = argV2z;
}


void DebugDrawable::checkDrawLines(void){
	l1.checkDrawLine();
	l2.checkDrawLine();
}



void DebugLine_ClearLine(int argID){
	aryDebugLines[argID].l1.canDraw = FALSE;
	aryDebugLines[argID].l2.canDraw = FALSE;
}


void DebugLine_ClearAll(){
	int i;
	nextDebugLineID = 0;
	nextDebugPathTrackDrawID = 0;
	for(i = 0; i < DEBUG_LINES_MAX; i++){
		aryDebugLines[i].l1.canDraw = FALSE;
		aryDebugLines[i].l2.canDraw = FALSE;
	}
	for(i = 0; i < DEBUG_PATHTRACK_DRAW_MAX; i++){
		aryDebugLines_pathTrack[i].canDraw = FALSE;
	}
}
void DebugLine_RenderAll(){
	int i = 0;
	//return;
	/*
	float fracto = abs(cos(gpGlobals->time/3));
	easyForcePrintLine("WHAT? %.2f : %.2f", gpGlobals->time, fracto);
	DebugLine_Setup(2, Vector(66.78, 805.35, 36.36), Vector(66.78, 805.35, 36.36) + Vector(-40, 200, 40), fracto );
	
	*/
	
	for(i = 0; i < DEBUG_PATHTRACK_DRAW_MAX; i++){
		aryDebugLines_pathTrack[i].checkDrawLines();
	}
	
	for(i = 0; i < DEBUG_LINES_MAX; i++){
		aryDebugLines[i].checkDrawLines();
	}
}//END OF DebugLine_RenderAll









void DebugDrawable_pathTrack::checkDrawLines(){


	if(canDraw){
		
		if(m_hSafeRef == NULL){
			//the path referred to disappeared? STOP!
			canDraw = FALSE;
			m_pPathTrackRef = NULL;
			return;
		}



		if(m_pPathTrackRef->pev->spawnflags & SF_PATH_DISABLED){
			//color it red.
			UTIL_drawLineFrame( m_pPathTrackRef->pev->origin + Vector(0, 0, -30), m_pPathTrackRef->pev->origin + Vector(0, 0, 30), DEBUG_LINE_WIDTH, 255, 0, 0);
		}else{
			//color it green.
			UTIL_drawLineFrame( m_pPathTrackRef->pev->origin + Vector(0, 0, -30), m_pPathTrackRef->pev->origin + Vector(0, 0, 30), DEBUG_LINE_WIDTH, 0, 255, 0);
		}

		if(m_pPathTrackRef->m_pprevious != NULL){
			//line to previous is yellow.
			UTIL_drawLineFrame( m_pPathTrackRef->pev->origin, m_pPathTrackRef->m_pprevious->pev->origin, DEBUG_LINE_WIDTH, 255, 255, 0);
		}
		if(m_pPathTrackRef->m_pnext != NULL){
			//line to next is blue.
			UTIL_drawLineFrame( m_pPathTrackRef->pev->origin, m_pPathTrackRef->m_pnext->pev->origin, DEBUG_LINE_WIDTH, 0, 0, 255);
		}
		if(m_pPathTrackRef->m_paltpath != NULL){
			//line to alternate is white. Rarely exists most likely.
			UTIL_drawLineFrame( m_pPathTrackRef->pev->origin, m_pPathTrackRef->m_paltpath->pev->origin, DEBUG_LINE_WIDTH, 255, 255, 255);
		}
	}
}//END OF DebugDrawable_pathTrack's checkDrawLines




void DebugLine_Setup(Vector vecStart, Vector vecEnd, int r, int g, int b){
	DebugLine_Setup(-1, vecStart, vecEnd, r, g, b);
}
void DebugLine_Setup(int argID, Vector vecStart, Vector vecEnd, int r, int g, int b){

	if(argID == -1){
		if(nextDebugLineID >= DEBUG_LINES_MAX){
			easyForcePrintLine("ERROR: Too many lines! Tried to create debug line of ID %d, max is %d", argID, DEBUG_LINES_MAX);
			return; //too many lines, don't.
		}
		argID = nextDebugLineID;
		nextDebugLineID++;
	}

	DebugDrawable& thisLine = aryDebugLines[argID];
	thisLine.l1.setup(TRUE, r, g, b, DEBUG_LINE_WIDTH, vecStart.x, vecStart.y, vecStart.z, vecEnd.x, vecEnd.y, vecEnd.z);
	
	thisLine.l2.canDraw = FALSE;  //only one line here.
}//END OF CreateDebugLine




void DebugLine_SetupPoint(Vector vecPoint, int r, int g, int b){
	DebugLine_SetupPoint(-1, vecPoint, r, g, b);
}
void DebugLine_SetupPoint(int argID, Vector vecPoint, int r, int g, int b){
	DebugLine_Setup(argID, vecPoint + Vector(0, 0, -30), vecPoint + Vector(0,0, 30), r, g, b);
}





void DebugLine_SetupPathTrack(CPathTrack* argPathTrack){
	DebugLine_SetupPathTrack(-1, argPathTrack);
}
void DebugLine_SetupPathTrack(int argID, CPathTrack* argPathTrack){

	if(argID == -1){
		if(nextDebugPathTrackDrawID >= DEBUG_PATHTRACK_DRAW_MAX){
			easyForcePrintLine("ERROR: Too many path_tracks to draw! Tried to create debug line of ID %d, max is %d", argID, DEBUG_PATHTRACK_DRAW_MAX);
			return; //too many lines, don't.
		}
		argID = nextDebugPathTrackDrawID;
		nextDebugPathTrackDrawID++;
	}


	DebugDrawable_pathTrack& thisDebugPathTrack = aryDebugLines_pathTrack[argID];
	thisDebugPathTrack.canDraw = TRUE;
	

	thisDebugPathTrack.m_hSafeRef = argPathTrack;
	//thisDebugPathTrack.m_hSafeRef.Set(argPathTrack->edict());

	thisDebugPathTrack.m_pPathTrackRef = argPathTrack;


	//easyForcePrintLine("WHAT THE FUCK?? %d", (thisDebugPathTrack.m_hSafeRef == NULL) );


}












void DebugLine_Setup(Vector vecStart, Vector vecEnd, float fraction, int successR, int successG, int successB, int failR, int failG, int failB){
	DebugLine_Setup(-1, vecStart, vecEnd, fraction, successR, successG, successB, failR, failG, failB);
}
//From here to there, draw a line to show that up to this distance has passed.
void DebugLine_Setup(int argID, Vector vecStart, Vector vecEnd, float fraction, int successR, int successG, int successB, int failR, int failG, int failB){
	
	if(argID == -1){
		if(nextDebugLineID + 1 >= DEBUG_LINES_MAX){
			easyForcePrintLine("ERROR: Too many lines! Tried to create debug line of ID %d, max is %d", argID, DEBUG_LINES_MAX);
			return; //too many lines, don't.
		}
		argID = nextDebugLineID;
		nextDebugLineID++;
	}

	fraction = clamp(fraction, 0, 1); //just in case.

	Vector vecDeltaFract = (vecEnd - vecStart) * fraction;

	float dist = (vecEnd - vecStart).Length();

	Vector vecDelta = (vecEnd - vecStart);
	Vector vecDir = vecDelta.Normalize();
	/*
	Vector vecDeltaFract = vecDir * dist * fraction;
	*/
	//Vector vecNewEnd = vecStart + vecDeltaFract;
	Vector vecNewEnd = vecStart + vecDelta * fraction;

	Vector& vecAltStart = vecNewEnd;
	//Vector& vecAltEnd = vecEnd;
	//Vector vecAltEnd = vecAltStart + vecDir * dist * (1 - fraction);
	Vector vecAltEnd = vecAltStart + vecDelta * (1 - fraction);
	
	DebugDrawable& thisLine = aryDebugLines[argID];

	if(fraction <= 0){
		//all fail, just draw the 2nd line only.
		thisLine.l1.setup(TRUE, failR, failG, failB, DEBUG_LINE_WIDTH, vecAltStart.x, vecAltStart.y, vecAltStart.z, vecAltEnd.x, vecAltEnd.y, vecAltEnd.z);
		thisLine.l2.canDraw = FALSE;
	}else if(fraction >= 1){
		//all success.
		thisLine.l1.setup(TRUE, successR, successG, successB, DEBUG_LINE_WIDTH, vecStart.x, vecStart.y, vecStart.z, vecNewEnd.x, vecNewEnd.y, vecNewEnd.z);
		thisLine.l2.canDraw = FALSE;
	}else{
		//inbetween.
		thisLine.l1.setup(TRUE, successR, successG, successB, DEBUG_LINE_WIDTH, vecStart.x, vecStart.y, vecStart.z, vecNewEnd.x, vecNewEnd.y, vecNewEnd.z);
		thisLine.l2.setup(TRUE, failR, failG, failB, DEBUG_LINE_WIDTH, vecAltStart.x, vecAltStart.y, vecAltStart.z, vecAltEnd.x, vecAltEnd.y, vecAltEnd.z);
	}





	/*
	Vector testvec1 = vecStart + vecDelta * fraction;
	float testvec2X = vecStart.x + vecDelta.x * fraction;
	float testvec2Y = vecStart.y + vecDelta.y * fraction;
	float testvec2Z = vecStart.z + vecDelta.z * fraction;

	easyForcePrintLine("ILL rub A CACTUS: (%.8f %.8f %.8f) : (%.8f %.8f %.8f)", testvec1.x, testvec1.y, testvec1.z, testvec2X, testvec2Y, testvec2Z);
	

	easyForcePrintLine("START: (%.8f %.8f %.8f)", vecStart.x, vecStart.y, vecStart.z);
	easyForcePrintLine("MMMM: (%.8f %.8f %.8f)", vecNewEnd.x, vecNewEnd.y, vecNewEnd.z);
	easyForcePrintLine("LITERAL END: (%.8f %.8f %.8f)", vecEnd.x, vecEnd.y, vecEnd.z);
	easyForcePrintLine("MY END: (%.8f %.8f %.8f)", vecAltEnd.x, vecAltEnd.y, vecAltEnd.z);
	easyForcePrintLine("DDD: (%.8f %.8f %.8f)", vecDelta.x, vecDelta.y, vecDelta.z);
	easyForcePrintLine("DDD * fract: (%.8f %.8f %.8f)", vecDelta.x * fraction, vecDelta.y * fraction, vecDelta.z * fraction);
	easyForcePrintLine("DDD * (1-fract): (%.8f %.8f %.8f)", vecDelta.x * (1-fraction), vecDelta.y * (1-fraction), vecDelta.z * (1-fraction));
	
	easyForcePrintLine("START + DDD*fract: (%.8f %.8f %.8f)",
		vecStart.x + vecDelta.x * fraction,
		vecStart.y + vecDelta.y * fraction,
		vecStart.z + vecDelta.z * fraction);
	
	easyForcePrintLine("MID + DDD*(1-fract): (%.8f %.8f %.8f)", vecNewEnd.x + vecDelta.x * (1-fraction), vecNewEnd.y + vecDelta.y * (1-fraction), vecNewEnd.z + vecDelta.z * (1-fraction));
	easyForcePrintLine("SUPERSUM: (%.8f %.8f %.8f)", vecStart.x + vecDelta.x * (fraction) + vecDelta.x * (1-fraction), vecStart.y + vecDelta.y * (fraction) + vecDelta.y * (1-fraction), vecStart.z + vecDelta.z * (fraction) + vecDelta.z * (1-fraction));
	easyForcePrintLine("FRACTTTTT %.8f", fraction);
	*/


}//END OF CreateDebugLine

void DebugLine_Color(int argID, int r, int g, int b){
	DebugDrawable& thisLine = aryDebugLines[argID];
	thisLine.l1.r = r;
	thisLine.l1.g = g;
	thisLine.l1.b = b;
	thisLine.l2.r = r;
	thisLine.l2.g = g;
	thisLine.l2.b = b;
}



void DebugLine_ColorSuccess(int argID){
	DebugLine_Color(argID, 0, 255, 0);
}
void DebugLine_ColorFail(int argID){
	DebugLine_Color(argID, 255, 0, 0);
}

//color a line green (success) or red (fail) based on the provided BOOL.
void DebugLine_ColorBool(int argID, BOOL argPass){
	if(argPass){
		DebugLine_ColorSuccess(argID);
	}else{
		DebugLine_ColorFail(argID);
	}
}