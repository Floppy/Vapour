//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.h - 02/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.h,v 1.1 2000/07/23 19:24:18 waz Exp $
//

#ifndef _VAL_RENDERCONTEXT_
#define _VAL_RENDERCONTEXT_

#pragma once

#include "VAL.h"

//#===--- Defines

// Creation options
#define RC_UNKNOWN				0
#define RC_WIDTH					1
#define RC_HEIGHT					2
#define RC_DEPTH					3

//#===--- Data types
enum RCRESULT {
	RC_OK = 0,
	RC_ERROR,
};

///////////////////
// CRenderContext

class CRenderContext {
public:
	CRenderContext();
	virtual ~CRenderContext();

//#===--- External Functions
	// Set a context option
	void SetOption(int iOption, int iValue);
	void SetOption(int iOption, const char *pcValue);

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data

};

#endif // _VAL_RENDERCONTEXT_
