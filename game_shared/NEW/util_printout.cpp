
// GENERAL NOTE:
// To my knowledge, there isn't a way to send variable arguments (...) to another method that also takes
// variable arguments directly.  Just have to copy them to a new string with the parameters filled in and
// send that to the 2nd method.
// "In English please?"
// Say you have a method that's a wrapper for a more specific engine call for printouts, like 
// easyForcePrintLine, which calls g_engfuncs.pfnAlertMessage.  pfnAlertMessage can take a variable
// number of arguments, just like easyForcePrintLine.  But we have no way of saying, "give my arguments
// from easyForcePrintLine to pfnAlertMessage for filling in on its own", like if I call 
// easyForcePrintLine("some text to print %.2f %s", 46.3f, "text"),  you can't specify to give the format
// string and 2 extra parameters to pfnAlertMessage.  You have to write the format string & 2 params to 
// a temporary string first, and give that filled string  ("some text to print 46.3 text") to
// pfnAlertMessage.  This is done most often through  UTIL_VarArgsVA(szFmt, argptr ): fills a temp
// char buffer with the filled format string and returns a pointer to it to be printed out as one
// parameter by pfnAlertMessage in this case.

// Sending the standard name for the argument list, "argptr", without involving UTIL_VarArgsVA, will still
// compile, but will NOT WORK.  It is only a pointer to the arguments in memory, and any printout method
// won't know what to do with that.  You'll get garbage.

// And, most printout methods don't even work with ellipses anyway, so they would need UTIL_VarArgsVA
// to pre-fill the string even if sending variable arguments directly from method to method were possible.


#include "version.h"

#ifdef CLIENT_DLL
//***************************CLIENT INCLUDES******************************
#include "util_printout.h"
#include "util_preprocessor.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

EASY_CVAR_EXTERN(enableModPrintouts)


#else
//***************************SERVER INCLUDES******************************
#include "extdll.h"
#include "util_printout.h"
#include "util_preprocessor.h"

#include "enginecallback.h"
#include "progdefs.h"

#include "cbase.h"

//reference:
//http://stackoverflow.com/questions/7031116/how-to-create-function-like-printf-variable-argument
//http://stackoverflow.com/questions/3530771/passing-variable-arguments-to-another-function-that-accepts-a-variable-argument
//
EASY_CVAR_EXTERN(hgruntPrintout)
EASY_CVAR_EXTERN(panthereyePrintout)
EASY_CVAR_EXTERN(squadmonsterPrintout)
EASY_CVAR_EXTERN(hassaultPrintout)
EASY_CVAR_EXTERN(gargantuaPrintout)
EASY_CVAR_EXTERN(barnaclePrintout)
EASY_CVAR_EXTERN(houndeyePrintout)
EASY_CVAR_EXTERN(enableModPrintouts)

extern int gmsgTextMsg, gmsgSayText;


#endif


// no need to vastly overcomplicate includes if we just need this from other util files right now.
extern void convertIntToBinary(char* buffer, unsigned int arg, unsigned int binaryDigits);



/*
//alert types for reference:
typedef enum
{
	at_notice,
	at_console,		// same as at_notice, but forces a ConPrintf, not a message box
	at_aiconsole,	// same as at_console, but only shown if developer level is 2!
	at_warning,
	at_error,
	at_logged		// Server print to console ( only in multiplayer games ).
} ALERT_TYPE;
*/



//Some other client-server dependent settings.
#ifdef CLIENT_DLL
//is there a difference between printout methods pfnConsolePrint and Con_DPrintf besides pfnConsolePrint
//taking strictly one argument (the string to print) as opposed to a variable list of arguments to combine by itself like printf?
// AND still not clear on whether to use Con_Printf or Con_DPrintf.

#define DEFAULT_ENGINE_HANDLE gEngfuncs
#define DEFAULT_PRINTOUT_METHOD Con_DPrintf
#define DEFAULT_PRINTOUT_ID "CL: "

#else

#define DEFAULT_ENGINE_HANDLE g_engfuncs
#define DEFAULT_PRINTOUT_METHOD pfnAlertMessage
#define DEFAULT_PRINTOUT_ID "SV: "

#endif



//***************************SHARED INCLUDES******************************
EASY_CVAR_EXTERN(enableModPrintouts)
EASY_CVAR_EXTERN(hiddenmem);

/*
#ifndef _DEBUG
	#define PRINT_RAW DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgsVA(szFmt, argptr ) );
	#define PRINT_RAW_LINE DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
#else
	#define PRINT_RAW DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgsVA(szFmt, argptr ) );
	#define PRINT_RAW_LINE DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgs_ID( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
#endif

#define PRINT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	PRINT_RAW\
	va_end(argptr);

#define PRINT_VA_LINE\
	va_list argptr;\
	va_start(argptr, szFmt);\
	PRINT_RAW_LINE\
	va_end(argptr);
	*/
	



// Because the alert print method for the server needs to be told "at_console",
// just going to re-do these separately for the client and server, that's the only difference.
// Not screwing around with "comma or not" differences between parameters after or not,
// really, really done with that.

#ifdef CLIENT_DLL
#define PRINT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgsVA(szFmt, argptr ) );\
	va_end(argptr);

#define PRINT_VA_LINE\
	char* bufRef;\
	va_list argptr;\
	va_start(argptr, szFmt);\
	bufRef = UTIL_VarArgsVA_ID(szFmt, argptr );\
	strcat(bufRef, "\n");\
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( bufRef );\
	va_end(argptr);

#else

// ALSO, serverside's "DEFAULT_PRINTOUT_METHOD" (pfnAlertMessage) can already handle arguments, that engine method had a "..."
// in the parameter list.  Just throwing that out there.
// ...nope nope nope, don't try to skip putting the args together.  The method will just freak out on getting argptr and do the printouts all wrong.
/*
#define PRINT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, szFmt, argptr );\
	va_end(argptr);
*/
// !!! old way:
#define PRINT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, UTIL_VarArgsVA(szFmt, argptr ) );\
	va_end(argptr);



#define PRINT_VA_LINE\
	char* bufRef;\
	va_list argptr;\
	va_start(argptr, szFmt);\
	bufRef = UTIL_VarArgsVA_ID(szFmt, argptr );\
	strcat(bufRef, "\n");\
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, bufRef );\
	va_end(argptr);




#define PRINT_SERVER_CONSOLE_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	g_engfuncs.pfnServerPrint( UTIL_VarArgsVA(szFmt, argptr ) );\
	va_end(argptr);

#define PRINT_SERVER_CONSOLE_VA_LINE\
	char* bufRef;\
	va_list argptr;\
	va_start(argptr, szFmt);\
	bufRef = UTIL_VarArgsVA_ID(szFmt, argptr );\
	strcat(bufRef, "\n");\
	g_engfuncs.pfnServerPrint( bufRef );\
	va_end(argptr);



#define PRINT_SERVER_TO_CLIENT_VA(arg_destEdict, arg_printType)\
	va_list argptr;\
	va_start(argptr, szFmt);\
	g_engfuncs.pfnClientPrintf(arg_destEdict, arg_printType, UTIL_VarArgsVA(szFmt, argptr ));\
	va_end(argptr);

#define PRINT_SERVER_TO_CLIENT_VA_LINE(arg_destEdict, arg_printType)\
	char* bufRef;\
	va_list argptr;\
	va_start(argptr, szFmt);\
	bufRef = UTIL_VarArgsVA_Custom(szFmt, "SV-CL: ", 7, argptr );\
	strcat(bufRef, "\n");\
	g_engfuncs.pfnClientPrintf(arg_destEdict, arg_printType, bufRef );\
	va_end(argptr);



//CLIENT_PRINTF -> pfnClientPrintf

//ClientPrint ???



#endif// END OF Client/Serverside check




char* UTIL_VarArgs( char *szFmt, ... )
{
	va_list		argptr;
	static char arychr_buffer[1024];
	
	va_start (argptr, szFmt);
	vsprintf (arychr_buffer, szFmt,argptr);
	va_end (argptr);

	return arychr_buffer;
}


char* UTIL_VarArgs_ID( char *szFmt, ... )
{
	va_list		argptr;
	static char arychr_buffer[1024];
	char* currentEnd;

	//arychr_buffer[0] = '\0';
	currentEnd = ::strcpy(&arychr_buffer[0], DEFAULT_PRINTOUT_ID );
	
	va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	va_end (argptr);

	return arychr_buffer;
}



/*
void UTIL_easyPrint(char* stringarg, int numbarg){

	char numstr[128];
	sprintf(numstr, stringarg, numbarg);
	ClientPrintAll( HUD_PRINTNOTIFY, numstr);

}
*/

//#define UTIL_easyPrint(a, ...) ClientPrintAll( HUD_PRINTNOTIFY, UTIL_VarArgs( a, ##__VA_ARGS__ );
//CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );

//g_engfuncs.pfnClientPrintf( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );
//#define UTIL_easyPrintClient(edict, str, ...) g_engfuncs.pfnClientPrintf( edict, print_console, UTIL_VarArgs( str, ##__VA_ARGS__ ) );


//This is essentially "UTIL_VarArgs" that accepts a "va_list" argument instead.  How it is applied involes the sender (wherever the call to this
//method is made) having responsibility for having "va_start" before calling this method, and "va_end" afterwards.
char* UTIL_VarArgsVA( const char *szFmt, va_list argptr )
{
	//va_list		argptr;
	static char arychr_buffer[1024];
	
	//va_start (argptr, szFmt);
	vsprintf (arychr_buffer, szFmt,argptr);
	//va_end (argptr);

	return arychr_buffer;
}

char* UTIL_VarArgsVA_ID( const char *szFmt, va_list argptr )
{
	//va_list		argptr;
	static char arychr_buffer[1024];
	char* currentEnd;
	
	#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
		//put a client/server indicator, just a few letters in front to tell where this printout came from.
		//The final string will start after that.
		//arychr_buffer[0] = '\0';
		currentEnd = ::strcpy(&arychr_buffer[0], DEFAULT_PRINTOUT_ID);
		currentEnd = &arychr_buffer[4]; //??? WHY? 
		/*
		currentEnd = &arychr_buffer[0];
		arychr_buffer[0] = 'a';
		arychr_buffer[1] = 's';
		arychr_buffer[2] = 's';
		arychr_buffer[3] = '\0';
		currentEnd = &arychr_buffer[3];
		*/
		//currentEnd = &arychr_buffer[0+3];
	#else
		currentEnd = &arychr_buffer[0];  //just the start, nothing special.
	#endif

	//va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	//va_end (argptr);

	return arychr_buffer;
}

char* UTIL_VarArgsVA_Custom( const char *szFmt, const char* szPrefix, const int iPrefixLength, va_list argptr )
{
	//va_list		argptr;
	static char arychr_buffer[1024];
	char* currentEnd;

	#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
		currentEnd = ::strcpy(&arychr_buffer[0], szPrefix);
		currentEnd = &arychr_buffer[iPrefixLength];  //???? WHY
	#else
		currentEnd = &arychr_buffer[0];  //just the start, nothing special.
	#endif
	//va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	//va_end (argptr);
	return arychr_buffer;
}












// NOTE - only do these if we're VS6. 
// We have a new way for after VS6.
// NOPE.  Dropping that idea!
//#ifdef IS_VS6
//MODDD: added for debugging convenience.  The same idea could be used
//in other client-side methods, I imagine (not sure about server-side).
//See my glock code for info about serverside-printouts.

void easyPrint(char *szFmt, ... )
{
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		PRINT_VA
	}
}
void easyForcePrint(char *szFmt, ... )
{
	PRINT_VA
}

void easyPrintStarter(void)
{
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
#ifdef CLIENT_DLL
		DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(DEFAULT_PRINTOUT_ID);
#else
		DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, DEFAULT_PRINTOUT_ID);
#endif
}
#endif
}
void easyForcePrintStarter(void)
{
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
#ifdef CLIENT_DLL
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(DEFAULT_PRINTOUT_ID);
#else
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, DEFAULT_PRINTOUT_ID);
#endif
#endif
}

void easyPrintLine(char *szFmt, ... )
{
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		PRINT_VA_LINE
	}
}
void easyForcePrintLine(char *szFmt, ... )
{
	PRINT_VA_LINE
}


//PrintLine methods without arguments. That means, just print a new line.
void easyPrintLine(void)
{
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
#ifdef CLIENT_DLL
		DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD("\n");
#else
		DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, "\n");
#endif
	}
}
void easyForcePrintLine(void)
{
#ifdef CLIENT_DLL
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD("\n");
#else
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD(ALERT_TYPE::at_console, "\n");
#endif
}
//#endif




/*
REFERNECE!!!/////////////////////////////////////////

#ifndef _DEBUG
	#define PRINT_RAW DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgsVA(szFmt, argptr ) );
	#define PRINT_RAW_LINE DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
#else
	#define PRINT_RAW DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgsVA(szFmt, argptr ) );
	#define PRINT_RAW_LINE DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD( UTIL_VarArgs_ID( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
#endif

#define PRINT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	PRINT_RAW\
	va_end(argptr);

#define PRINT_VA_LINE\
	va_list argptr;\
	va_start(argptr, szFmt);\
	PRINT_RAW_LINE\
	va_end(argptr);

*/
	




//Similar to above, but prints the converted int to standard output (easyForcePrintLine) without giving a reference to the binary string to the caller.
void printIntAsBinary(unsigned int arg, unsigned int binaryDigits) {
	char binaryBuffer[33];
	convertIntToBinary(binaryBuffer, arg, binaryDigits);
	easyForcePrint(binaryBuffer);
}//END OF printIntAsBinary

//Same as above but add a newline character.
void printLineIntAsBinary(unsigned int arg, unsigned int binaryDigits) {
	char binaryBuffer[33];
	convertIntToBinary(binaryBuffer, arg, binaryDigits);
	easyForcePrintLine(binaryBuffer);
}//END OF printIntAsBinary


#ifdef CLIENT_DLL

#else
//Similar to above, but prints the converted int to standard output (easyForcePrintLine) without giving a reference to the binary string to the caller.
void printIntAsBinaryClient(edict_t* pEntity, unsigned int arg, unsigned int binaryDigits) {
	char binaryBuffer[33];
	convertIntToBinary(binaryBuffer, arg, binaryDigits);
	easyForcePrintClient(pEntity, binaryBuffer);
}//END OF printIntAsBinary

//Same as above but add a newline character.
void printLineIntAsBinaryClient(edict_t* pEntity, unsigned int arg, unsigned int binaryDigits) {
	char binaryBuffer[33];
	convertIntToBinary(binaryBuffer, arg, binaryDigits);
	easyForcePrintLineClient(pEntity, binaryBuffer);
}//END OF printIntAsBinary
#endif















//#ifndef CLIENT_DLL

void UTIL_printLineVector(const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
		easyPrintLine("vector: (%.2f, %.2f, %.2f)", theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printLineVector(const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		//UTIL_printOutVectorLine("", vX, vY, vZ);
		easyPrintLine("vector: (%.2f, %.2f, %.2f)", vX, vY, vZ);
	}
}

void UTIL_printLineVector(char* printLabel, const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printLineVector(char* printLabel, const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
	easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, vX, vY, vZ);
	}
}


void UTIL_printVector(const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
		easyPrint("(%.2f, %.2f, %.2f) ", theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printVector(const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		//UTIL_printOutVectorLine("", vX, vY, vZ);
		easyPrint("(%.2f, %.2f, %.2f) ", vX, vY, vZ);
	}
}

void UTIL_printVector(char* printLabel, const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printVector(char* printLabel, const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, vX, vY, vZ);
	}
}

//#endif //CLIENT_DLL check



#ifdef CLIENT_DLL

#else

void UTIL_printLineVectorClient(edict_t* pEntity, const Vector& theVector) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
		easyPrintLine("vector: (%.2f, %.2f, %.2f)", theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printLineVectorClient(edict_t* pEntity, const float vX, const float vY, const float vZ) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		//UTIL_printOutVectorLine("", vX, vY, vZ);
		easyPrintLine("vector: (%.2f, %.2f, %.2f)", vX, vY, vZ);
	}
}

void UTIL_printLineVectorClient(edict_t* pEntity, char* printLabel, const Vector& theVector) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printLineVectorClient(edict_t* pEntity, char* printLabel, const float vX, const float vY, const float vZ) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, vX, vY, vZ);
	}
}


void UTIL_printVectorClient(edict_t* pEntity, const Vector& theVector) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
		easyPrint("(%.2f, %.2f, %.2f) ", theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printVectorClient(edict_t* pEntity, const float vX, const float vY, const float vZ) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		//UTIL_printOutVectorLine("", vX, vY, vZ);
		easyPrint("(%.2f, %.2f, %.2f) ", vX, vY, vZ);
	}
}

void UTIL_printVectorClient(edict_t* pEntity, char* printLabel, const Vector& theVector) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, theVector.x, theVector.y, theVector.z);
	}
}

void UTIL_printVectorClient(edict_t* pEntity, char* printLabel, const float vX, const float vY, const float vZ) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, vX, vY, vZ);
	}
}
#endif






//Don't do anything! You cheap bastard.
void easyPrintLineDummy(char *szFmt, ...){
	//PRINT_VA_LINE
}




















//Implementations below.

#ifdef CLIENT_DLL

//MODDD - new console printout method.

//Also, NOTICE:::
//cl_util.h "prototypes" most methods in here so that they can be accessed mostly anywhere else in the client.
//MODDD TODO - Should they be moved to util_printout.h which cl_util.h could then call??


//necessary for virtual (continuous, unknown type / number of) arguments.
//#include "stdarg.h"  ???

/*
inline void UTIL_easyPrint(edict_t* pEdict, char *szFmt, ... ){
	va_list argptr;
	va_start(argptr, szFmt);
	gEngfuncs.Con_Printf("CONSTRUCTA\n");
	//g_engfuncs.pfnClientPrintf(pEdict, print_console, UTIL_VarArgsVA(szFmt, argptr ));
	va_end(argptr);
}

inline void UTIL_easyPrintLine(edict_t* pEdict, char *szFmt, ... ){
	va_list argptr;
	va_start(argptr, szFmt);
	gEngfuncs.Con_Printf("CONSTRUCTA\n");
	//g_engfuncs.pfnClientPrintf(pEdict, print_console, UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) ) );
	va_end(argptr);
}
*/


// call gEngfuncs.pfnClientCmd and handle the fancy parameter stuff for me.
// Sends a message from the client to the server for the current player as though the user typed something
// into console (processed in dlls/client.cpp, ClientCommand method)
// Easy way to tell the serverside version of the player of any changes to client-specific settings that 
// are still important for the server (cl_fvox, or fov strangely enough)
void easyClientCommand(char* szFmt, ...){
	va_list argptr; va_start(argptr, szFmt);
	gEngfuncs.pfnClientCmd(UTIL_VarArgsVA(szFmt, argptr));
	va_end(argptr);
}//END OF easyClientCmd

void easyServerCommand(char* szFmt, ...){
	va_list argptr; va_start(argptr, szFmt);
	gEngfuncs.pfnServerCmd(UTIL_VarArgsVA(szFmt, argptr));
	va_end(argptr);
}//END OF easyServerCmd


#else

/*
// reference from engine/eiface.h.  Required for pfnClientPrintf calls.
typedef enum
{
	print_console,
	print_center,
	print_chat,
} PRINT_TYPE;
*/


//#ifdef IS_VS6
// This prints directly to a particular provided client from the server. This is a bit different
// from the two "ClientPrint" methods further below that can search titles.txt for something to
// plug in too.
void easyPrintClient(edict_t* pEdict, const char* szFmt, ... ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		PRINT_SERVER_TO_CLIENT_VA(pEdict, PRINT_TYPE::print_console)
	}
}
void easyForcePrintClient(edict_t* pEdict, const char* szFmt, ... ){
	PRINT_SERVER_TO_CLIENT_VA(pEdict, PRINT_TYPE::print_console)
}

void easyPrintStarterClient(edict_t* pEdict) {
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		g_engfuncs.pfnClientPrintf(pEdict, PRINT_TYPE::print_console, "SV-CL: ");
	}
#endif
}
void easyForcePrintStarterClient(edict_t* pEdict) {
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
	g_engfuncs.pfnClientPrintf(pEdict, PRINT_TYPE::print_console, "SV-CL: ");
#endif
}

void easyPrintLineClient(edict_t* pEdict, const char* szFmt, ... ){
	if(EASY_CVAR_GET(enableModPrintouts) != 0){
		PRINT_SERVER_TO_CLIENT_VA_LINE(pEdict, PRINT_TYPE::print_console)
	}
}
void easyForcePrintLineClient(edict_t* pEdict, const char* szFmt, ... ){
	PRINT_SERVER_TO_CLIENT_VA_LINE(pEdict, PRINT_TYPE::print_console)
}
void easyPrintLineClient(edict_t* pEdict) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		g_engfuncs.pfnClientPrintf(pEdict, PRINT_TYPE::print_console, "\n");
	}
}
void easyForcePrintLineClient(edict_t* pEdict) {
	g_engfuncs.pfnClientPrintf(pEdict, PRINT_TYPE::print_console, "\n");
}

// NOTICE!  Prints to the server console, which may also be the first player's console if the
// player is not running a dedicated server or doing single player.
// If running a dedicated server, there is some separate command-line window besides the game
// that should receive messages by this call.
void easyPrintServer(const char* szFmt, ...) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		PRINT_SERVER_CONSOLE_VA
	}
}
void easyForcePrintServer(const char* szFmt, ...) {
	PRINT_SERVER_CONSOLE_VA
}

void easyPrintStarterServer(void) {
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		g_engfuncs.pfnServerPrint(DEFAULT_PRINTOUT_ID);
	}
#endif
}
void easyForcePrintStarterServer(void) {
#if (defined(_DEBUG) || FORCE_PRINTOUT_PREFIX != 0)
	g_engfuncs.pfnServerPrint(DEFAULT_PRINTOUT_ID);
#endif
}

void easyPrintLineServer(const char* szFmt, ...) {
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		PRINT_SERVER_CONSOLE_VA_LINE
	}
}
void easyForcePrintLineServer(const char* szFmt, ...) {
	PRINT_SERVER_CONSOLE_VA_LINE
}
void easyPrintLineServer(void){
	if (EASY_CVAR_GET(enableModPrintouts) != 0) {
		g_engfuncs.pfnServerPrint("\n");
	}
}
void easyForcePrintLineServer(void){
	g_engfuncs.pfnServerPrint("\n");
}


//AKA, "CLIENT_COMMAND"
//g_engfuncs.pfnClientCommand
//ALSO, this engine call already supports the "..." ellipses.  Just throwing that out there.
void easyClientCommand(edict_t* pEdict, char* szFmt, ...){
	va_list argptr; va_start(argptr, szFmt);
	g_engfuncs.pfnClientCommand(pEdict, UTIL_VarArgsVA(szFmt, argptr));
	//g_engfuncs.pfnClientCommand(pEdict, szFmt);
	va_end(argptr);
}

//AKA, "SERVER_COMMAND"
//g_engfuncs.pfnServerCommand
void easyServerCommand(char* szFmt, ...){
	va_list argptr; va_start(argptr, szFmt);
	g_engfuncs.pfnServerCommand(UTIL_VarArgsVA(szFmt, argptr));
	va_end(argptr);
}

//#endif //VS6 check







//Note that the below methods can search titles.txt for messages and plug things in after reading them. Best just leave this system be.
// RENAMED.  Was "ClientPrintAll" in the base SDK.
// As all other as-is console commands meant to be called directly don't start with "UTIL_", why should this one?
// Especially from "ClientPrint" not doing so.

// The "ClientPrint" methods work like easyPrintLineClient calls above (they refer to the server's pfnServerPrint
// method that leaves the rest up to the engine instead of interpreting this result in cl_dll/text_message.cpp).
// Since the "ClientPrint" methods allow for custom interpretation on the client, that's what allows reading from
// titles.txt.
// For anything that doesn't need that feature, may as well use the easyPrintClient ones above, more flexible
// (does not have a limit of 4 paramters after the message string).
// See cl_dll/text_message.cpp for the MsgFunc_TextMsg

// !!! IMPORTANT.
// If you don't need any text to be interpreted by clientside text files for its own substitutions
// (starts with #), there isn't a need to use the 4 parameters at all.

// They're also a little cumbersome, since the 4 parameters must be strings. Parsing numbers to strings
// can work, but why the extra effort when sprintf ends up doing that sooner or later anyway.
// Just use the usual "UTIL_VarArgs" stuff to put any parameters in the format string and just send
// that by itself.  That's likely what any other printout from server to client are doing in some way
// anyway.
// Example from dlls/multiplayer_gamerules.cpp that proves this point:
//     ClientPrintAll( HUD_PRINTNOTIFY,
//       UTIL_VarArgs(
//         "%s has joined the game\n", 
//         (pl->pev->netname&& STRING(pl->pev->netname)[0] != 0) ? STRING(pl->pev->netname) : "unconnected"
//       )
//     );

// ***AND SO, easyForcePrintClient AND easyPrintBroadcast/Force were born.
// The existing "easyPrintClient" was actually fine, but its engine call depends on developer being 
// non-zero.  So nope, may as well make that one inspired by ClientPrint below too.
// Removing the whole "enableModPrintouts" thing because that's just dumbly redundant with the 
// existing "developer" CVar.

/*
// For the "msg_dest" parameter, ClientPrint methods use one of these choices:
#define HUD_PRINTNOTIFY		1
#define HUD_PRINTCONSOLE	2
#define HUD_PRINTTALK		3
#define HUD_PRINTCENTER		4
*/

void ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	MESSAGE_BEGIN( MSG_ALL, gmsgTextMsg );
		WRITE_BYTE( msg_dest );
		WRITE_STRING( msg_name );

		if ( param1 )
			WRITE_STRING( param1 );
		if ( param2 )
			WRITE_STRING( param2 );
		if ( param3 )
			WRITE_STRING( param3 );
		if ( param4 )
			WRITE_STRING( param4 );

	MESSAGE_END();
}

void ClientPrint( entvars_t *client, int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
{
	MESSAGE_BEGIN( MSG_ONE, gmsgTextMsg, NULL, client );
		WRITE_BYTE( msg_dest );
		WRITE_STRING( msg_name );

		if ( param1 )
			WRITE_STRING( param1 );
		if ( param2 )
			WRITE_STRING( param2 );
		if ( param3 )
			WRITE_STRING( param3 );
		if ( param4 )
			WRITE_STRING( param4 );

	MESSAGE_END();
}



// And also from dlls/util.cpp because why not.
// Not renaming because I give no shits about this.
void UTIL_SayText(const char* pText, CBaseEntity* pEntity)
{
	if (!pEntity->IsNetClient())
		return;

	MESSAGE_BEGIN(MSG_ONE, gmsgSayText, NULL, pEntity->edict());
	WRITE_BYTE(pEntity->entindex());
	WRITE_STRING(pText);
	MESSAGE_END();
}

void UTIL_SayTextAll(const char* pText, CBaseEntity* pEntity)
{
	MESSAGE_BEGIN(MSG_ALL, gmsgSayText, NULL);
	WRITE_BYTE(pEntity->entindex());
	WRITE_STRING(pText);
	MESSAGE_END();
}








//TEMP!!!   Comment out contents to stop much of the AI spam.
void easyPrintLineGroup1(char* szFmt, ...) {


}


void easyPrintLineGroup2(char* szFmt, ...) {

}

void easyPrintLineGroup3(char* szFmt, ...) {

}


void easyPrintLineGroup4(char* szFmt, ...) {

}


/*
	va_list argptr;
	va_start(argptr, szFmt);
	g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
	va_end(argptr);
*/



#endif //END OF server details




