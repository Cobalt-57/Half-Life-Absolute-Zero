

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





//~should increment between noticable releases.
#define protoModVersion "1.9a"

//~date of compilation.
//~to pick automatically, leave this commented out.  WARNING: always the date you ran half life, just freeze a date here!
//~__DATE__ seems to get time at compilation, that stays, as desired.
#define protoModDate __DATE__

//~version of visual studio used to compile.
//~to pick automatically, leave this commented out.
//#define protoModVS "vs?"

