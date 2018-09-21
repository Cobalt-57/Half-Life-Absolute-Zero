

//gets the "protoVersion" var, placed there for convenience.
#include "version.h"




void writeVersionInfo(char* aryChr, int maxLength);
void writeDateInfo(char* aryChr, int maxLength);
//const char* getDate(void);
void getDate(char* aryChr);
const char* determineVisualStudioVersion(void);







//write the constants from "version.h" to the "protoVersion" CVar.
void writeVersionInfo(char* aryChr, int maxLength){

	
	//char aryChr[128];
	aryChr[0] = '\0';   //safety.
	
	int mode = 0;

#ifdef _DEBUG
	const char* addon = "DEBUG";
#else
	const char* addon = "RELEASE";
#endif


#ifndef protoModVS
	//determine like so
	const char* addon2 = determineVisualStudioVersion();
#else
	const char* addon2 = protoModVS;
#endif


	int writeTo = 0;
	int readFrom = 0;
	int addonOff = 0;
	for(int i = 0; i < maxLength - 1;i++){
		if(mode == 0){
			if(protoModVersion[i] == '\0'){
				//end found!
				mode = 1;
				readFrom = 0;
				
				aryChr[i] = '_';
				//i -= 1;   //can overwrite this empty place.

			}else{
				aryChr[i] = protoModVersion[i];
			}
		}else if(mode == 1){
			//begin writing the extra.

			if(addon2[readFrom] == '\0'){
				//done!
				//aryChr[i] = '\0';
				//break;
				readFrom = 0;
				mode = 2;

				aryChr[i] = '_';
				//i -= 1;

			}else{
				aryChr[i] = addon2[readFrom];
				readFrom++;
			}
		}else if(mode == 2){
			//begin writing the extra next.

			if(addon[readFrom] == '\0'){
				//done!
				aryChr[i] = '\0';
				break;
			}else{
				aryChr[i] = addon[readFrom];
				readFrom++;
			}
		}

	}//END OF for loop
	aryChr[maxLength - 1] = '\0';


}

void writeDateInfo(char* aryChr, int maxLength){
	//yay.
	aryChr[0] = '\0';   //safety.


#ifndef protoModDate
	char aryChrDATE[128];
	const char* toWrite = aryChrDATE;
	//const char* toWrite = getDate();
	getDate(aryChrDATE);
#else
	const char* toWrite = protoModDate;
#endif

	
	int mode = 0;

	int writeTo = 0;
	int readFrom = 0;
	int addonOff = 0;
	for(int i = 0; i < maxLength - 1;i++){
		if(toWrite[i] == '\0'){
			//done!
			aryChr[i] = '\0';
			break;
		}else{
			aryChr[i] = toWrite[i];
		}
	}//END OF for loop
	aryChr[maxLength - 1] = '\0';

}






//THANK YOU,
//http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
#include <ctime>
void getDate(char* aryChr){
	time_t currentTime = time(0);
    struct tm * currentTimeData = localtime( & currentTime );

	//char  aryChr[128];
	sprintf(aryChr, "M:%d D:%d Y:%d", currentTimeData->tm_mon + 1, currentTimeData->tm_mday, currentTimeData->tm_year + 1900);
	//aryChr[127] = '\0';

	//return aryChr;
}


//THANK YOU,
//http://stackoverflow.com/questions/70013/how-to-detect-if-im-compiling-code-with-visual-studio-2008
const char* determineVisualStudioVersion(void){

#ifndef _MSC_VER
	return "UNKNOWN-A";
#elif _MSC_VER == 1100
	return "vs5";
#elif _MSC_VER == 1200
	return "vs6";
#elif _MSC_VER == 1300
	return "vs7";
#elif _MSC_VER == 1310
	return "vs7.1";
#elif _MSC_VER == 1400
	return "vs8";
#elif _MSC_VER == 1500
	return "vs9";
#elif _MSC_VER == 1600
	return "vs10";
#elif _MSC_VER == 1700
	return "vs11";
#elif _MSC_VER == 1800
	return "vs12";
#elif _MSC_VER == 1900
	return "vs14";
#else
	return "UNKNOWN-B";
#endif

}

