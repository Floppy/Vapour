//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// VALWin32.h - 03/07/2000 - Warren Moore
//	VAL management object for Windows
//
// $Id: VALWin32.h,v 1.1 2000/07/10 21:40:25 waz Exp $
//

#ifndef _VALWIN32_
#define _VALWIN32_

#pragma once

#include "VAL.h"

//#===--- Defines

//#===--- Data types

//////////////
// CVALWin32

class CVALWin32 : public CVAL {
public:
	CVALWin32();
	virtual ~CVALWin32();

//#===--- External Functions
	// Returns a pointer to a const string containing the working directory
	virtual const char *GetAppDir();
	// Return a pointer to a const string containing the exe name and complete path
	virtual const char *GetAppName();

protected:
//#===--- Internal Functions

//#===--- Internal Data
	char *m_pcAppDir;
	char *m_pcAppName;

};

#ifdef VAL_BUILD
extern CVAL *g_poVAL;
#endif // VAL_BUILD

#endif // _VALWIN32_
