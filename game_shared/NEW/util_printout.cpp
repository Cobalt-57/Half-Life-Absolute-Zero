


#ifdef CLIENT_DLL
//***************************CLIENT INCLUDES******************************
#include "util_printout.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

extern float global2_enableModPrintouts;


#else
//***************************SERVER INCLUDES******************************
#include "extdll.h"
#include "util_printout.h"

#include "enginecallback.h"
#include "progdefs.h"

//reference:
//http://stackoverflow.com/questions/7031116/how-to-create-function-like-printf-variable-argument
//http://stackoverflow.com/questions/3530771/passing-variable-arguments-to-another-function-that-accepts-a-variable-argument
//
extern float global_hgruntPrintout;
extern float global_panthereyePrintout;
extern float global_squadmonsterPrintout;
extern float global_hassaultPrintout;
extern float global_gargantuaPrintout;
extern float global_barnaclePrintout;
extern float global_houndeyePrintout;
extern float global_enableModPrintouts;
extern int gmsgTextMsg, gmsgSayText;


#endif


//Some other client-server dependent settings.
#ifdef CLIENT_DLL
//is there a difference between printout methods pfnConsolePrint and Con_Printf besides pfnConsolePrint
//taking strictly one argument (the string to print) as opposed to a variable list of arguments to combine by itself like printf?

#define DEFAULT_ENGINE_HANDLE gEngfuncs
#define DEFAULT_PRINTOUT_METHOD Con_Printf
#define DEFAULT_PRINTOUT_ID "CL: "
#else

#define DEFAULT_ENGINE_HANDLE g_engfuncs
#define DEFAULT_PRINTOUT_METHOD pfnServerPrint
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



#define PRINT_SERVER_TO_CLIENT_VA\
	va_list argptr;\
	va_start(argptr, szFmt);\
	g_engfuncs.pfnClientPrintf(pEdict, print_console, UTIL_VarArgsVA(szFmt, argptr ));\
	va_end(argptr);

#define PRINT_SERVER_TO_CLIENT_VA_LINE\
	char* bufRef;\
	va_list argptr;\
	va_start(argptr, szFmt);\
	bufRef = UTIL_VarArgsVA_Custom(szFmt, "CL-SV: ", 7, argptr );\
	strcat(bufRef, "\n");\
	g_engfuncs.pfnClientPrintf(pEdict, print_console, bufRef );\
	va_end(argptr);

	

//CLIENT_PRINTF -> pfnClientPrintf
	
//ClientPrint ???







char* UTIL_VarArgs( char *szFmt, ... )
{
	va_list		argptr;
	static char		string[1024];
	
	va_start (argptr, szFmt);
	vsprintf (string, szFmt,argptr);
	va_end (argptr);

	return string;	
}


char* UTIL_VarArgs_ID( char *szFmt, ... )
{
	va_list		argptr;
	static char		string[1024];
	char* currentEnd;


	//string[0] = '\0';
	currentEnd = ::strcpy(&string[0], DEFAULT_PRINTOUT_ID );
	
	va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	va_end (argptr);

	return string;	
}



/*
void UTIL_easyPrint(char* stringarg, int numbarg){

	char numstr[128];
	sprintf(numstr, stringarg, numbarg);
	UTIL_ClientPrintAll( HUD_PRINTNOTIFY, numstr);

}
*/

//#define UTIL_easyPrint(a, ...) UTIL_ClientPrintAll( HUD_PRINTNOTIFY, UTIL_VarArgs( a, ##__VA_ARGS__ );
//CLIENT_PRINTF( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );

//g_engfuncs.pfnClientPrintf( pEntity, print_console, UTIL_VarArgs( "\"fov\" is \"%d\"\n", (int)GetClassPtr((CBasePlayer *)pev)->m_iFOV ) );
//#define UTIL_easyPrintClient(edict, str, ...) g_engfuncs.pfnClientPrintf( edict, print_console, UTIL_VarArgs( str, ##__VA_ARGS__ ) );


//This is essentially "UTIL_VarArgs" that accepts a "va_list" argument instead.  How it is applied involes the sender (wherever the call to this
//method is made) having responsibility for having "va_start" before calling this method, and "va_end" afterwards.
char* UTIL_VarArgsVA( const char *szFmt, va_list argptr )
{
	//va_list		argptr;
	static char		string[1024];
	
	//va_start (argptr, szFmt);
	vsprintf (string, szFmt,argptr);
	//va_end (argptr);

	return string;
}
char* UTIL_VarArgsVA_ID( const char *szFmt, va_list argptr )
{
	//va_list		argptr;
	static char		string[1024];
	char* currentEnd;
	
	#ifndef _DEBUG
		currentEnd = &string[0];  //just the start, nothing special.
	#else
		//put a client/server indicator, just a few letters in front to tell where this printout came from.
		//The final string will start after that.
		//string[0] = '\0';
		currentEnd = ::strcpy(&string[0], DEFAULT_PRINTOUT_ID );
		currentEnd = &string[4]; //??? WHY? 
		/*
		currentEnd = &string[0];
		string[0] = 'a';
		string[1] = 's';
		string[2] = 's';
		string[3] = '\0';
		currentEnd = &string[3];
		*/
		//currentEnd = &string[0+3];
	#endif

	//va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	//va_end (argptr);

	return string;
}

char* UTIL_VarArgsVA_Custom( const char *szFmt, const char* szPrefix, const int iPrefixLength, va_list argptr )
{
	//va_list		argptr;
	static char		string[1024];
	char* currentEnd;
	
	#ifndef _DEBUG
		currentEnd = &string[0];  //just the start, nothing special.
	#else
		currentEnd = ::strcpy(&string[0], szPrefix );
		currentEnd = &string[iPrefixLength];  //???? WHY
	#endif
	//va_start (argptr, szFmt);
	vsprintf (currentEnd, szFmt,argptr);
	//va_end (argptr);
	return string;
}










//MODDD: added for debugging convenience.  The same idea could be used
//in other client-side methods, I imagine (not sure about server-side).
//See my glock code for info about serverside-printouts.

void easyPrint(const char *szFmt, ... )
{
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	PRINT_VA
}

void easyForcePrint(const char *szFmt, ... )
{
	PRINT_VA
}

void easyPrintLine(const char *szFmt, ... )
{
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	PRINT_VA_LINE
}
void easyForcePrintLine(const char *szFmt, ... )
{
	PRINT_VA_LINE
}

//PrintLine methods without arguments. That means, just print a new line.
void easyPrintLine()
{
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}	
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD("\n");
}
void easyForcePrintLine()
{
	DEFAULT_ENGINE_HANDLE.DEFAULT_PRINTOUT_METHOD("\n");
}



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
	












//#ifndef CLIENT_DLL








void UTIL_printLineVector(const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
	easyPrintLine("vector: (%.2f, %.2f, %.2f)", theVector.x, theVector.y, theVector.z);
}

void UTIL_printLineVector(const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	//UTIL_printOutVectorLine("", vX, vY, vZ);
	easyPrintLine("vector: (%.2f, %.2f, %.2f)", vX, vY, vZ);
}

void UTIL_printLineVector(const char* printLabel, const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, theVector.x, theVector.y, theVector.z);
}

void UTIL_printLineVector(const char* printLabel, const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	easyPrintLine("%s: (%.2f, %.2f, %.2f)", printLabel, vX, vY, vZ);
}


void UTIL_printVector(const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	//UTIL_printOutVectorLine("", theVector.x, theVector.y, theVector.z);
	easyPrint("(%.2f, %.2f, %.2f) ", theVector.x, theVector.y, theVector.z);
}

void UTIL_printVector(const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	//UTIL_printOutVectorLine("", vX, vY, vZ);
	easyPrint("(%.2f, %.2f, %.2f) ", vX, vY, vZ);
}

void UTIL_printVector(const char* printLabel, const Vector& theVector){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, theVector.x, theVector.y, theVector.z);
}

void UTIL_printVector(const char* printLabel, const float vX, const float vY, const float vZ){
	if(EASY_CVAR_GET(enableModPrintouts) == 0){
		return;
	}
	easyPrint("%s: (%.2f, %.2f, %.2f) ", printLabel, vX, vY, vZ);
}

//#endif





//Don't do anything! You cheap bastard.
void easyPrintLineDummy(const char *szFmt, ...){
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




















#else



//This prints directly to a particular provided client from the server. This is a bit different from the two larger methods further blow that can search titles.txt for 
//something to plug in too.
void easyPrintClient(edict_t* pEdict, const char *szFmt, ... ){
	if(global_enableModPrintouts == 0){
		return;
	}
	PRINT_SERVER_TO_CLIENT_VA
}

void easyForcePrintClient(edict_t* pEdict, const char *szFmt, ... ){
	PRINT_SERVER_TO_CLIENT_VA
}

void easyPrintLineClient(edict_t* pEdict, const char *szFmt, ... ){
	if(global_enableModPrintouts == 0){
		return;
	}
	PRINT_SERVER_TO_CLIENT_VA_LINE
}
void easyForcePrintLineClient(edict_t* pEdict, const char *szFmt, ... ){
	PRINT_SERVER_TO_CLIENT_VA_LINE
}




//Note that the below methods can search titles.txt for messages and plug things in after reading them. Best just leave this system be.
void UTIL_ClientPrintAll( int msg_dest, const char *msg_name, const char *param1, const char *param2, const char *param3, const char *param4 )
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








//Not really doing printouts by these clone methods anymore, remove them at some point.

void easyPrintLineHgrunt(const char *szFmt, ...){

	//removed
	/*
	if(global_hgruntPrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
	*/
}
void easyPrintLinePanthereye(const char *szFmt, ...){
	if(global_panthereyePrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}
void easyPrintLineSquadmonster(const char *szFmt, ...){
	if(global_squadmonsterPrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}
void easyPrintLineHassault(const char *szFmt, ...){
	if(global_hassaultPrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}
void easyPrintLineGargantua(const char *szFmt, ...){
	if(global_gargantuaPrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}
void easyprintlineBarnacle(const char *szFmt, ...){
	if(global_barnaclePrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}
void easyPrintLineHoundeye(const char *szFmt, ...){
	if(global_houndeyePrintout == 1){
		va_list argptr;
		va_start(argptr, szFmt);
		g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
		va_end(argptr);
	}
}

	/*
	va_list argptr;
	va_start(argptr, szFmt);
	g_engfuncs.pfnServerPrint( UTIL_VarArgs( "%s\n", UTIL_VarArgsVA(szFmt, argptr ) )  );
	va_end(argptr);
	*/


//TEMP!!!   Comment out contents to stop much of the AI spam.
void easyPrintLineGroup1(const char *szFmt, ...){
	
	
}


void easyPrintLineGroup2(const char *szFmt, ...){
	
}

void easyPrintLineGroup3(const char *szFmt, ...){
	
}


void easyPrintLineGroup4(const char *szFmt, ...){
	
}




#endif //END OF server details




