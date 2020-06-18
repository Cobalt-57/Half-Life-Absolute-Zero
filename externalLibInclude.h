

// Made because different versions of visual studio (at least 2019) need some new things
// included to work right, or you suffer from really cryptic errors.
// Using the VS2010 platform toolkit (project properties) works too from VS2019, but eh.
// Seemed odd to me for something as old as half-life to work fine as-is in VS2010 but not
// VS2019 with their own platform toolkits.

// Sections like this may be interesting for linux support to be all in one place later,
// if this would help (remove wherever things like this are in other files if so):
/*
#ifndef _WIN32  // LINUX
#include <dlfcn.h>
#include <unistd.h> // getcwd
#include <stdio.h> // sprintf
#endif
*/
// and this:
/*
#ifndef INC_NOWIN_H
#define INC_NOWIN_H
#ifndef _WIN32

#include <unistd.h>

#endif //!_WIN32
#endif //INC_NOWIN_H
*/





//MODDD - what is the point of "DEBUG" as opposed to Visual Studio's default "_DEBUG" ???
//Not even used.  Oh well, no harm leaving it in.  (actually used sometimes, like for AI fail sparks.)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Allow "DEBUG" in addition to default "_DEBUG"
#ifdef _DEBUG
#define DEBUG 1
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





// WARNING!  This should make this fix apply to VS2017 and 2019.  Not tested on 2017 though.
// Need for further versions unknown (at what version a new fix would be needed).



#if (_MSC_VER <= 1200)   //VS6 or vs5...
	// can't use "override", just ignore it.  It's just specific-ness
	// of something that should work anyway.
	#define override
	#define IS_VS6
	
#elif (_MSC_VER < 1900)    //after VS6 but before VS14 (2015), no change needed.

	
#else  //at or after vs14 (2015), we need to do something.

	// only for c++ you bastard
	#if defined(__cplusplus)
		#include <iostream>
		#include <cmath>
	#endif

#endif


// below here is always included.
#include "MATH.H"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

//TODO - include <string.h> here?  Should be removed elsewhere then.
// That's a lot of places.

