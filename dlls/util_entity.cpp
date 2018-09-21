
#include "extdll.h"
#include "util_entity.h"
#include "enginecallback.h"












//#ifndef CLIENT_DLL

edict_t *ENT(edict_t *pent)		{ return pent; }   

edict_t *ENT(EOFFSET eoffset)			{ return (*g_engfuncs.pfnPEntityOfEntOffset)(eoffset); }
EOFFSET OFFSET(EOFFSET eoffset)			{ return eoffset; }


EOFFSET OFFSET(const edict_t *pent)	
{ 
#if _DEBUG
	if ( !pent )
		ALERT( at_error, "Bad ent in OFFSET()\n" );
#endif
	return (*g_engfuncs.pfnEntOffsetOfPEntity)(pent); 
}

EOFFSET OFFSET(entvars_t *pev)				
{ 
#if _DEBUG
	if ( !pev )
		ALERT( at_error, "Bad pev in OFFSET()\n" );
#endif
	return OFFSET(ENT(pev)); 
}




#ifdef DEBUG

	///////////////////////////////////////////////////////////////////////////////////////////
	#ifndef CLIENT_DLL
	//SERVER: normal.
	edict_t *DBG_EntOfVars( const entvars_t *pev )
	{
		if (pev->pContainingEntity != NULL)
			return pev->pContainingEntity;
		ALERT(at_console, "entvars_t pContainingEntity is NULL, calling into engine");
		edict_t* pent = (*g_engfuncs.pfnFindEntityByVars)((entvars_t*)pev);
		if (pent == NULL)
			ALERT(at_console, "DAMN!  Even the engine couldn't FindEntityByVars!");
		((entvars_t *)pev)->pContainingEntity = pent;
		return pent;
	}
	///////////////////////////////////////////////////////////////////////////////////////////
	#else
	//CLIENT: was dummied out in hl_baseentity.cpp, clientside. Keep it that way here?
		edict_t *DBG_EntOfVars( const entvars_t *pev ) { return NULL; }
	#endif
	///////////////////////////////////////////////////////////////////////////////////////////

#else
//not in debug? no need, no DBG_EntOfVars mmethod at all and ENT is inlined.

#endif





entvars_t* VARS(EOFFSET eoffset)		{ return VARS(ENT(eoffset)); }
int	ENTINDEX(edict_t *pEdict)			{ return (*g_engfuncs.pfnIndexOfEdict)(pEdict); }
edict_t* INDEXENT( int iEdictNum )		{ return (*g_engfuncs.pfnPEntityOfEntIndex)(iEdictNum); }




//#endif