//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// DisplayContext.h - 23/11/2000 - Warren Moore
//	Base class for display contexts. Used to retrieve information from a display
//
// $Id: DisplayContext.h,v 1.1 2000/11/25 11:30:37 waz Exp $
//

#ifndef _VAL_DISPLAYCONTEXT_
#define _VAL_DISPLAYCONTEXT_

#pragma once

#include "VAL.h"

//#===--- Data types

enum DCRESULT {
	DC_OK = 0,
	DC_UNSUPPORTED_VALUE,
	DC_ERROR,
};

//#===--- Defines

// Valid values
#define DC_UNKNOWN			0
#define DC_WIN_DC				1

#define DC_ERROR_STRINGS \
	"OK", \
	"Unsupported value", \
	"Unknown error", \

//#===--- External linkage dependencies

////////////////////
// CDisplayContext

class CDisplayContext {
public:
	// Constructor
	CDiplayContext();
	// Destructor
	virtual ~CDisplayContext();

//#===--- External Functions

	// Returns a pointer to a display context
	virtual DCRESULT GetDisplayPointer(const int iValue, void *&pData) const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data

	static const char *m_pcErrorString[];					// Error string table
};

//#===--- Inline Functions

#endif // _VAL_DISPLAYCONTEXT_
