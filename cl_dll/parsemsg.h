/***
*
*	Copyright (c) 1996-2002, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
//  parsemsg.h
//

//MODDD - why was this sitting here dummying out ASSERT when elsewhere defines it to mean something?
// ...removing it completely causes something in cl_dll/custom_message.cpp to complain, MsgFunc_ResetHUD.
// Clearly that was meant to be a dummied call.  So eh.  Just going with that.  RENAME.
//#define ASSERT( x )
#define ASSERT_DUMMY( x )

void BEGIN_READ( void *buf, int size );
int READ_CHAR( void );
int READ_BYTE( void );
int READ_SHORT( void );
int READ_WORD( void );
int READ_LONG( void );
float READ_FLOAT( void );
char* READ_STRING( void );
float READ_COORD( void );
float READ_ANGLE( void );
float READ_HIRESANGLE( void );









