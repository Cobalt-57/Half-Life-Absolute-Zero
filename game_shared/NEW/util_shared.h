
#ifndef UTIL_SHARED
#define UTIL_SHARED

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// preliminary includes
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////




#include "util_preprocessor.h"








#ifdef CLIENT_DLL

//necessary?
//#include "cvardef.h"





//Just need this to give the client access to BOOL, the FALSE / TRUE constants, and some file-related things like MAX_PATH.
#include "windows.h"

//#include "WinDef.h"
/*
//typedef int BOOL;
#define FALSE 0
#define TRUE (!FALSE)
*/

/*
//MODDD - NOTE. This is from extdll.h, usually serverside only. Is going everywhere safe?
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Prevent tons of unused windows definitions
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOWINRES
#define NOSERVICE
#define NOMCX
#define NOIME
#include "windows.h"
#else // _WIN32
#define FALSE 0
#define TRUE (!FALSE)
typedef unsigned long ULONG;
typedef unsigned char BYTE;
typedef int BOOL;
#define MAX_PATH PATH_MAX
#include <limits.h>
#include <stdarg.h>

//MODDD - moved to util_shared.cpp, mostly.
//#ifndef min
//#define min(a,b)  (((a) < (b)) ? (a) : (b))
//#endif
//#ifndef max
//#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define _vsnprintf(a,b,c,d) vsnprintf(a,b,c,d)
//#endif

#endif //_WIN32
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/


#else


#include "cdll_dll.h"





#endif

















#include "util_printout.h"



EASY_CVAR_EXTERN(hiddenMemPrintout)


//MODDD - from clientside's cl_util.h.
#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define min(a, b)  (((a) < (b)) ? (a) : (b))
#define fabs(x)	   ((x) > 0 ? (x) : 0 - (x))



/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
// extern things. CONTENT HERE.
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////



//#include "activity.h"
//#include "enginecallback.h"
class CBaseEntity;








extern BOOL globalflag_muteDeploySound;








//MODDD - have this.
extern const char* FClassname(CBaseEntity* derp);

extern BOOL stringStartsWith(const char* source, const char* startswith);


extern BOOL checkMatch(const char* src1, char* src2);
extern BOOL checkMatch(const char* src1, char* src2, int size);
extern BOOL checkMatchIgnoreCase(const char* src1, char* src2);
extern BOOL checkMatchIgnoreCase(const char* src1, char* src2, int size);


extern const char* tryIntToString(int arg_src);
extern int tryStringToInt(const char* arg_src);
extern float tryStringToFloat(const char* arg_src);
extern const char* tryFloatToString(float arg_src);
extern void tryFloatToStringBuffer(char* dest, float arg_src);



extern void lowercase(char* src);
extern void lowercase(char* src, int size);






extern float roundToNearest(float num);


extern void UTIL_substring(char* dest, const char* src, int startIndex, int endIndex);

extern int UTIL_findCharFirstPos(const char* search, char toFind);

extern void UTIL_appendTo(char* dest, const char* add, int appendStartLoc);

extern void appendTo(char* dest, const char* add, int* refIndex);
extern void appendToAndTerminate(char* dest, const char* add, int* refIndex);
extern void appendTo(char* dest, const char* add, int* refIndex, char endCharacter);
extern void appendToAndTerminate(char* dest, const char* add, int* refIndex, char endCharacter);
extern void appendTo(char* dest, const int numb, int* refIndex);
extern void appendToAndTerminate(char* dest, const int numb, int* refIndex);
extern void appendTo(char* dest, const int numb, int* refIndex, char endCharacter);
extern void appendToAndTerminate(char* dest, const int numb, int* refIndex, char endCharacter);
extern void appendTo(char* dest, const char add, int* refIndex);
extern void appendToAndTerminate(char* dest, const char add, int* refIndex);
extern void strncpyTerminate(char* dest, const char* send, int arg_length);


extern void copyString(const char* src, char* dest);
extern void copyString(const char* src, char* dest, int size);

extern void UTIL_appendToEnd(char* dest, const char* add);


extern int lengthOfString(const char* src);
extern int lengthOfString(const char* src, int storeSize);
extern BOOL isStringEmpty(const char* arg_src);
extern BOOL stringEndsWith(const char* arg_src, const char* arg_endsWith);
extern BOOL stringEndsWithIgnoreCase(const char* arg_src, const char* arg_endsWith);







extern BOOL checkSubFileExistence(const char* arg_subdir);





extern void determineHiddenMemPath(void);
extern BOOL checkValveSubFileExistence(const char* arg_subdir);

extern BOOL checkSubDirectoryExistence(const char* arg_subdir);
extern void loadHiddenCVars(void);
extern void saveHiddenCVars(void);



extern void convertIntToBinary(char* buffer, unsigned int arg, unsigned int binaryDigits);









/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

#endif //END OF UTIL_SHARED