

//PYTHON ISSUE.  Holding down after firing and running out of clip ammo makes it click a lot.  while zoomed it flickers oddly.

//Is it ok to force
//    #pragma once  
//...to stop some of those #ifdef starting files from giving intellisense errors? test with the VS5 (visual studio 95) or whatever builds.


//TODO - should infiniteLongJumpCharge being on also force the spring icon in ammo.cpp to look shaded? if so make it broadcasted (HIDDENC) to the client to see it there easily.


//Why doesn't the single-fire shotgun blast, third-person model, do a muzzle flash but the double-fire shotgun does?


//Can we get movement down ramps working properly?  That is snap to them instead of falling down them?
//going up them is fine, there may be movement code for that.


//MAJOR!  
//Does "ag_fire1", 2, and 3 being played clientside (cl_dll/ev_hldm.cpp) and never serverside mean only the player firing hornets hears the hornet fire sound?
//If so, this contradicts hearing the hornet fire sound from agrunts.  Why do they make the same ag_fire# noise for others to hear, but other players don't make the same noise for others to hear?


//"Can't find the world"
//Should bullsquidspit be reduced since it does that continual toxin damage already?
//Same for other status alignment dealers (besides bleeding melee damage)?


//There is a drastic idea in basemonster.cpp of prioritizing chumtoads over other types of relationships.  Trying now, see if it's a problem.

//QUESTION:  should damage enhancements per hitting player subhitboxes (head, arm, leg, body, etc.) be turned off for damage from NPC's?
//           or altogether during single player?
//           It seems if enemies never try to go for particular hitboxes for extra damage, getting hit there is just a matter of very
//           bad luck or looking at something from an unlucky angle.  It could make some viewpoints cause you to be much more likely
//           to take triple damage (headshot modifier for the player's TraceAttack, receiving).
//           Currently a damagetype for the 2nd bitmask (bitsDamageTypeMod), named DMG_HITBOX_EQUAL, can forbid damgage enhancements from sub hitboxes.
//           But enemies need that applied per enemy for it to work, if they take more/less damage from sub hitboxes.



//***Did we still want that tracer thing for all bullet player weapons?
//See CVAr playerWeaponTracerMode...




//~should increment between noticable releases.
#define protoModVersion "1.9k"

//~date of compilation.
//~to pick automatically, leave this commented out.  WARNING: always the date you ran half life, just freeze a date here!
//~__DATE__ seems to get time at compilation, that stays, as desired.
#define protoModDate __DATE__

//~version of visual studio used to compile.
//~to pick automatically, leave this commented out.
//#define protoModVS "vs?"

