//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// VAL.h - 03/07/2000 - Warren Moore
//	VAL management object for OS independent functions
//
// $Id: VAL.h,v 1.2 2000/07/10 09:11:51 waz Exp $
//

#ifndef _VAL_
#define _VAL_

#pragma once

// TODO : Merge Defs.h into VAL.h
#include "Defs.h"

//#===--- Defines

//#===--- Data types

/////////
// CVAL

class CVAL {
public:
	CVAL();
	virtual ~CVAL();

//#===--- External Functions
	// Returns a pointer to a const string containing the working directory
	virtual const char *GetAppDir() = 0;
	// Return a pointer to a const string containing the exe name and complete path
	virtual const char *GetAppName() = 0;

protected:
//#===--- Internal Functions

//#===--- Internal Data

};

#ifdef VAL_BUILD
extern CVAL *g_poVAL;
#endif // VAL_BUILD

#endif // _VAL_
