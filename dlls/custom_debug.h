


#ifndef CUSTOM_DEBUG_H
#define CUSTOM_DEBUG_H


#include "util_shared.h"  //includes util_printout.h

#include "cbase.h"
#include "trains.h"

class CPathTrack;

#define DEBUG_LINES_MAX 16
#define DEBUG_PATHTRACK_DRAW_MAX 2

//For anywhere else that wants to know this?
#define DEBUG_LINE_WIDTH 8



class DebugLine{
public:
	BOOL canDraw;
	int r;
	int g;
	int b;
	int width;
	float v1x;
	float v1y;
	float v1z;
	float v2x;
	float v2y;
	float v2z;
	void checkDrawLine(void);
	void setup(BOOL argCanDraw, int argR, int argG, int argB, int argWidth, float argV1x, float argV1y, float argV1z, float argV2x, float argV2y, float argV2z);

};


class DebugDrawable{
public:
	DebugLine l1;
	DebugLine l2;
	void checkDrawLines(void);

};


class DebugDrawable_pathTrack{
public:
	/*
	DebugLine l_prev;
	DebugLine l_next;
	DebugLine l_origin;
	DebugLine l_alt;
	*/
	BOOL canDraw;
	EHANDLE	m_hSafeRef;
	CPathTrack* m_pPathTrackRef;
	void checkDrawLines(void);

};




extern int DebugLine_drawTime;


extern DebugDrawable aryDebugLines[];


extern void DebugLine_ClearLine(int argID);

extern void DebugLine_ClearAll(void);
extern void DebugLine_RenderAll(void);

extern void DebugLine_Setup(Vector vecStart, Vector vecEnd, float fraction, int successR=0, int successG=255, int successB=0, int failR=255, int failG=0, int failB=0);
extern void DebugLine_Setup(int argID, Vector vecStart, Vector vecEnd, float fraction, int successR=0, int successG=255, int successB=0, int failR=255, int failG=0, int failB=0);

extern void DebugLine_Setup(Vector vecStart, Vector vecEnd, int r, int g, int b);
extern void DebugLine_Setup(int argID, Vector vecStart, Vector vecEnd, int r, int g, int b);

extern void DebugLine_SetupPoint(Vector vecPoint, int r, int g, int b);
extern void DebugLine_SetupPoint(int argID, Vector vecPoint, int r, int g, int b);

extern void DebugLine_SetupPathTrack(CPathTrack* argPathTrack);
extern void DebugLine_SetupPathTrack(int argID, CPathTrack* argPathTrack);




extern void DebugLine_Color(int argID, int r, int g, int b);
extern void DebugLine_ColorSuccess(int argID);
extern void DebugLine_ColorFail(int argID);
extern void DebugLine_ColorBool(int argID, BOOL argPass);




#endif //END OF CUSTOM_DEBUG_H





