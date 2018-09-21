


#ifndef CUSTOMMESSAGE_H
#define CUSTOMMESSAGE_H


//#include "hud.h"
#include "hudbase.h"

class CHudBase;


class CCustomMessage: public CHudBase
{
public:

	CCustomMessage(void);



	int Init(void);
	void Reset(void);
	int VidInit(void);
	int Draw(float flTime);
	void Think(void);

	//JUKEBOX!
	int MsgFunc_JBoxReq(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_JBoxOff(const char *pszName, int iSize, void *pbuf);

	int MsgFunc_AutoMus(const char *pszName, int iSize, void *pbuf);

	int MsgFunc_CliTest(const char *pszName, int iSize, void *pbuf);
	
	
	
	int MsgFunc_UpdClientC(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_RstClientC(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_PntClientC(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_RetrieveFOV(const char *pszName, int iSize, void *pbuf);
	int MsgFunc_UpdBnclStat(const char *pszName,  int iSize, void *pbuf );
	int MsgFunc_UpdPlyA(const char *pszName,  int iSize, void *pbuf );
	
	int MsgFunc_MUnpause(const char *pszName,  int iSize, void *pbuf );

	int MsgFunc_UpdTWI(const char *pszName,  int iSize, void *pbuf );
	int MsgFunc_UpdJT(const char *pszName,  int iSize, void *pbuf );

	int MsgFunc_YMG(const char *pszName,  int iSize, void *pbuf );
	int MsgFunc_YMG_S(const char *pszName,  int iSize, void *pbuf );

	
};//END OF CCustomMesssage


#endif //END OF CUSTOMMESSAGE_H