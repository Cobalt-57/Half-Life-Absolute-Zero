

//MODDD - new.


#ifndef UTIL_PRINTOUT
#define UTIL_PRINTOUT






#ifdef CLIENT_DLL

#include "wrect.h"
#include "cl_dll.h"

#include "cvardef.h"



#else

#include "cdll_dll.h"

#include "activity.h"
#include "enginecallback.h"
//extern enginefuncs_t g_engfuncs;

#endif





//shared.
class Vector2D;
class Vector;




//Fun fact: "extern" is implied for method prototypes like this, whether the keyword itself is used or not.
//The counterpart is "static", which may be the default for variables instead? Don't know.
extern char	*UTIL_VarArgs( char *szFmt, ... );
extern char	*UTIL_VarArgs_ID( char *szFmt, ... );
extern char* UTIL_VarArgsVA( const char *szFmt, va_list argptr );
extern char* UTIL_VarArgsVA_ID( const char *szFmt, va_list argptr );
extern char* UTIL_VarArgsVA_Custom( const char *szFmt, const char* szPrefix, const int iPrefixLength, va_list argptr );



//

extern void easyPrint(const char *szFmt, ... );
extern void easyForcePrint(const char *szFmt, ...);
extern void easyPrintLine(const char *szFmt, ...);
extern void easyForcePrintLine(const char *szFmt, ...);


extern void UTIL_printLineVector(const Vector& theVector);
extern void UTIL_printLineVector(const float vX, const float vY, const float vZ);
extern void UTIL_printLineVector(const char* printLabel, const Vector& theVector);
extern void UTIL_printLineVector(const char* printLabel, const float vX, const float vY, const float vZ);
extern void UTIL_printVector(Vector& theVector);
extern void UTIL_printVector(const float vX, const float vY, const float vZ);
extern void UTIL_printVector(const char* printLabel, const Vector& theVector);
extern void UTIL_printVector(const char* printLabel, const float vX, const float vY, const float vZ);

extern void easyPrintLineDummy(const char *szFmt, ...);




#ifdef CLIENT_DLL




#else

// prints a message to each client. Can be filtered by the built-in strings from titles.txt.
extern void			UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );
inline void			UTIL_CenterPrintAll( const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL ) 
{
	UTIL_ClientPrintAll( HUD_PRINTCENTER, msg_name, param1, param2, param3, param4 );
}
// prints messages through the HUD
extern void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1 = NULL, const char *param2 = NULL, const char *param3 = NULL, const char *param4 = NULL );



extern void easyPrintClient(edict_t* pEdict, const char *szFmt, ... );
extern void easyForcePrintClient(edict_t* pEdict, const char *szFmt, ... );
extern void easyPrintLineClient(edict_t* pEdict, const char *szFmt, ... );
extern void easyForcePrintLineClient(edict_t* pEdict, const char *szFmt, ... );


#endif















#endif//END OF UTIL_PRINTOUT


