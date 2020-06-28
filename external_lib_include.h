
// KEEP IN MIND, C and C++ files can include this file.
// Check for '__cplusplus' if lines should apply to only either C or C++.

// Made because different versions of visual studio (at least 2019) need some new things
// included to work right, or you suffer from really cryptic errors.
// Using the VS2010 platform toolkit (project properties) works too from VS2019, but eh.
// Seemed odd to me for something as old as half-life to work fine as-is in VS2010 but not
// VS2019 with their own platform toolkits.

// Any includes/defines related to OS compatability should also go here instead to all
// be in one place for better comparing conflicts / redundancies.
// And in case future changes need to be made, this is much more pleasant than wandering
// around a lot of other files for "#ifdef _Win32" or references to "windows.h".

// TODO:
// There are still some other checks for OS's, see cl_dll/interface.cpp, dlls/cbase.cpp,
// and dlls/util.cpp.  Not exhaustive.
// Let's replace all the '#pragma once' 's surrounded by '#ifdef _win32' 's.
// Those are distracting for searches!
////////////////////////////////////////////////////////////////////////////////////////////


#ifndef EXTERNAL_LIB_INCLUDE_H
#define EXTERNAL_LIB_INCLUDE_H
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

// And including this early.
#include "ignore_warning_list.h"


// "cdecl" looks to be some C or C++ keyword that's kinda comparable to "stdcall".
// That is all.

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef _WIN32
	#include <windows.h>
#endif

// is 'limits.h' reallly only linux-only?  No clue.
#ifdef __linux__
	#include <unistd.h>
	#include <dlfcn.h>
	#include <limits.h>

#endif



#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
//TODO - include <string.h> here?  Should be removed elsewhere then.
// That's a lot of places.


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
#ifdef _MSC_VER

// Some visualstudio-specific fixes.
#if (_MSC_VER <= 1200)
	// VS6 or VS5...
	// can't use "override", just ignore it.  It's just describing something
	// that should work with or without it anyway (a method with the same
	// name/params as one in a parent class is implied overriding too).
#define override
#define IS_VS6

#elif (_MSC_VER < 1900)
	// after VS6 but before VS14 (2015), no change needed.
	// Tested only in VS2010 in this range though.

#else
	// at or after vs14 (2015), we need to do something.  For C++ at least.
	// (tested in 2019 only)

	#if defined(__cplusplus)
		#include <iostream>
		#include <cmath>
	#endif

#endif


#else
// No _MSC_VER?   ...what?
// Not really sure what to do then.



#endif //END OF _MSC_VER checks
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



// for C file features.
#ifdef __cplusplus
	//MODDD - thanks,
	//http://stackoverflow.com/questions/18100097/portable-way-to-check-if-directory-exists-windows-linux-c
	//MODDD - acording to that link this is portable at least.
	#include <sys/types.h>
	#include <sys/stat.h>

	// what.
	extern struct stat info;

	//MODDD - used by determineHiddenMemPath.
	#include <iostream>
	#include <fstream>
	#include <string>
	//using namespace std;
#endif





// And now for constants!



// For whatever reason, some places refer to a "DEBUG" macro instead of the "_DEBUG" one that
// visual studio actually provides.  Whatever the reason, old version weirdness, etc., just have
// both around then.
#ifdef _DEBUG
#define DEBUG 1
#endif




#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOWINRES
	#define NOSERVICE
	#define NOMCX
	#define NOIME

#endif
#ifdef __linux__
	#define MAX_PATH PATH_MAX
	#define CreateDirectory(p, n) mkdir(p, 0777)

	//MODDD - removed.  I don't see this changing for a different OS.  Already handled fine in general.
	//#define FALSE 0
	//#define TRUE (!FALSE)

	typedef unsigned long ULONG;
	typedef unsigned char BYTE;
	typedef int BOOL;
	#define MAX_PATH PATH_MAX

	//MODDD - min and max defines moved to common/const.h, mostly.
	// And that "_vsnprintf" thing.
#endif




// Is there a 'roundf' function?
#ifndef roundf
	#ifdef round
		// simple fix, just use 'round' for 'roundf' if we lack roundf.
		#define roundf round
	#else
		//what??  I guess we'll redirect to our own round for either type. okie dokie.
		#define roundf customRound
		#define round customRound
		extern float customRound(float arg);
	#endif
#endif



// cl_dll/cl_util.cpp and pm_shared/pm_math.c had this prototype floating around?
// I can only assume it's still expected to be filled by some system library, since
// it's never given an implementation anywhere in the as-is codebase.
// Maybe we don't even need this, commenting out to see if it still works.
//double sqrt(double x);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //EXTERNAL_LIB_INCLUDE_H