//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// VAL.h - 03/07/2000 - Warren Moore
//	VAL management object for OS independent functions
//
// $Id: VAL.h,v 1.4 2000/07/19 08:52:02 waz Exp $
//

#ifndef _VAL_
#define _VAL_

#pragma once

//#===--- Macros

#define NEWBEGIN	try { 
#define NEWEND(x)	} catch (CMemoryException *pException) { TRACE("Out of memory : %s\n", x); pException->Delete(); } 

//#===--- Defines

// String length
#define STR_SIZE				1024

//#===--- Data types

//#===--- Includes
#include "ProgressControl.h"

// Generic File Return Codes
enum FRESULT {
	F_OK,
	F_DOESNOTEXIST,
	F_FILEERROR,
	F_WRONGFILETYPE,
	F_WRONGIMAGETYPE,
	F_OUTOFMEMORY,
	F_WRONGCONTEXT,
	F_HASNTCHANGED,
	F_NOFILENAME,
	F_NOFILEEXTENSION,
	F_NODATATOSAVE,
	F_ERROR,
};

/////////
// CVAL

class CVAL {
public:
	CVAL();
	virtual ~CVAL();

//#===--- External Functions
	//#===--- Application system calls
	// Returns a pointer to a const string containing the working directory
	virtual const char *GetAppDir() = 0;
	// Return a pointer to a const string containing the exe name and complete path
	virtual const char *GetAppName() = 0;

	//#===--- Progress control calls
	// Set handler functions
	virtual void SetProgressTextFunction(const char *pcName, TEXT_FUNC pText);
	virtual void SetProgressFunction(const char *pcName, HANDLER_FUNC pHandler);
	// Progress management functions
	// Text output
	virtual void SetProgressText(const char *pcName, const char *pcText);
	// Progress output
	virtual void SetProgressMax(const char *pcName, unsigned int uMax);
	virtual void SetProgressPos(const char *pcName, unsigned int uPercent);
	virtual void StepProgress(const char *pcName);

protected:
//#===--- Internal Functions

//#===--- Internal Data
	CProgressControl *m_poProgressCtrl;

};

#ifdef VAL_BUILD
extern CVAL *g_poVAL;
#endif // VAL_BUILD

#endif // _VAL_
