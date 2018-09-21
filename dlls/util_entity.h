


//Some essential things needed practically everywhere serverside - even enginecallback.h - moved from util.h (mostly) and cbase.h to here.
//Things that can't be repeated or depend on other things themselves (like enginecallback.h) have been implemented in util_entity.cpp instead of inlined.

#ifndef UTIL_ENTITY_H
#define UTIL_ENTITY_H


//needed for some things in the newly included section below.
#include "windows.h"

//let this be available everywhere serverside.
#include "util_preprocessor.h"



//MODDD - most of this comes from the top of util.h. These are mostly some methods needed for working with
//        entities in general, which may as well be present everywhere serverside.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// More explicit than "int"
typedef int EOFFSET;

// In case it's not alread defined
typedef int BOOL;



//- from cbase.cpp
typedef struct _SelAmmo
{
	BYTE	Ammo1Type;
	BYTE	Ammo1;
	BYTE	Ammo2Type;
	BYTE	Ammo2;
} SelAmmo;




// Keeps clutter down a bit, when declaring external entity/global method prototypes
#define DECLARE_GLOBAL_METHOD(MethodName)  extern void DLLEXPORT MethodName( void )
#define GLOBAL_METHOD(funcname)					void DLLEXPORT funcname(void)

// This is the glue that hooks .MAP entity class names to our CPP classes
// The _declspec forces them to be exported by name so we can do a lookup with GetProcAddress()
// The function is used to intialize / allocate the object for the entity
#ifdef _WIN32
#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) \
	extern "C" _declspec( dllexport ) void mapClassName( entvars_t *pev ); \
	void mapClassName( entvars_t *pev ) { GetClassPtr( (DLLClassName *)pev ); }
#else
#define LINK_ENTITY_TO_CLASS(mapClassName,DLLClassName) extern "C" void mapClassName( entvars_t *pev ); void mapClassName( entvars_t *pev ) { GetClassPtr( (DLLClassName *)pev ); }
#endif


//
// Conversion among the three types of "entity", including identity-conversions.
//
#ifdef DEBUG
	//DBG_EntOfVars implementation moved to enginecallback.cpp. Assuming it wasn't inline for a reason (some inline methods making too many method calls is enough to crash... yeah),
    //and making it not inline here gave some "repeated definition" errors.
	extern edict_t *DBG_EntOfVars(const entvars_t *pev);
	inline edict_t *ENT(const entvars_t *pev)	{ return DBG_EntOfVars(pev); }
#else
	inline edict_t *ENT(const entvars_t *pev)	{ return pev->pContainingEntity; }
#endif




//MODDD NOTE - HOLD ON THERE. So nothing happens here? Just returns the same "pent" given? Just checking.
edict_t *ENT(edict_t *pent);

edict_t *ENT(EOFFSET eoffset);
EOFFSET OFFSET(EOFFSET eoffset);

//MODDD - both OFFSET method variants are implemented in util_entity.cpp instead.
EOFFSET OFFSET(const edict_t *pent);
EOFFSET OFFSET(entvars_t *pev);


inline entvars_t *VARS(entvars_t *pev)					{ return pev; }

inline entvars_t *VARS(edict_t *pent)			
{ 
	if ( !pent )
		return NULL;

	return &pent->v; 
}

entvars_t* VARS(EOFFSET eoffset);
int	  ENTINDEX(edict_t *pEdict);
edict_t* INDEXENT( int iEdictNum );

// Testing the three types of "entity" for nullity
#define eoNullEntity 0
inline BOOL FNullEnt(EOFFSET eoffset)			{ return eoffset == 0; }
inline BOOL FNullEnt(const edict_t* pent)	{ return pent == NULL || FNullEnt(OFFSET(pent)); }
inline BOOL FNullEnt(entvars_t* pev)				{ return pev == NULL || FNullEnt(OFFSET(pev)); }



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#endif //END OF UTIL_ENTITY_H