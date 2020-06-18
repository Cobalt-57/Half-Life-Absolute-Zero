
// should increment between noticeable releases.
#define MODINFO_Version "1.9k"

// date of compilation.
// Picked automatically if commented out (but it resets with each run of the game)
#define MODINFO_Date __DATE__

// Version of visual studio used to compile.
// To determine automatically, leave this commented out.
//#define MODINFO_VS "vs?"



//////////////////////////////////
// Little project-wide config, for handling the allowed names for spawning by map or 'give' commands
// (like "monster_human_grunt").

// Note that give commands wil still check for whether a version of a name with "monster_" in front
// is valid if the current isn't found.  That is, "give human_grunt" will still check and see that
// "monster_human_grunt" exists, regardless of EXTRA_NAMES choice below.
//Maps do not use this extra check.


// 0: no effect.
// 1: original names from as-is script are no longer linked (like monster_human_grunt)
#define REMOVE_ORIGINAL_NAMES 0

// 0: no extra names.
// 1: Just one alternate name (a version without "monster_";  "human_grunt").
// 2: Same as 1, but also grants possible extra names, such as "hgrunt".
#define EXTRA_NAMES 2


// In debug mode, prefixes for printouts, a "CL" or "SV" label to indicate whether they came from
// the client or server, is included in any 'easyPrintLine' printouts.
// In release mode, this is usually skipped. Unless FORCE_PRINTOUT_PREFIX is 1.
#define FORCE_PRINTOUT_PREFIX 1

