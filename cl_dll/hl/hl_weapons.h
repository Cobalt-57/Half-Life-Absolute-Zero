// NEW FILE.  For getting the globals from hl_weapons.cpp easily


#ifndef HL_WEAPONS_H
#define HL_WEAPONS_H

#include "extdll.h"
#include "util.h"
#include "cbase.h"

// better know what this is at least
class CBasePlayer;


extern CBasePlayer localPlayer;

extern float g_flApplyVel;
extern int g_irunninggausspred;

extern vec3_t previousorigin;



//clientside only variable.
extern BOOL reloadBlocker;

// MODDD - TEST.
extern float flNextAttackChangeMem;


extern BOOL blockUntilModelChange;
extern int oldModel;
extern int queuedBlockedModelAnim;

extern float forgetBlockUntilModelChangeTime;
extern float resistTime;
extern float seqPlayDelay;
extern int seqPlay;

#endif //HL_WEAPONS_H
