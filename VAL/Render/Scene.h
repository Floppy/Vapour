//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scene.h - 06/10/2000 - Warren Moore
//	Base class for scene rendering classes
//
// $Id: Scene.h,v 1.0 2000/10/06 17:44:18 waz Exp $
//

#ifndef _VAL_SCENE_
#define _VAL_SCENE_

#pragma once

#include "VAL.h"

//#===--- Defines

//#===--- Data types
enum SCRESULT {
	SC_OK = 0,
	SC_OUT_OF_MEMORY,
	SC_ERROR,
};

#define RC_ERROR_STRINGS \
	"OK", \
	"Out of memory", \
	"Unknown error", \

///////////
// CScene

class CScene {
public:
	CScene();
	virtual ~CScene();

//#===--- External Functions

	//#===--- Diagnostics
	// Returns an error string for a specified error
	virtual const char *GetErrorString(SCRESULT eResult) const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	static const char *m_pcErrorString[];					// Error string table
};

//#===--- Inline Functions

inline const char *CRenderContext::GetErrorString(SCRESULT eResult) const {
	return m_pcErrorString[(int)eResult];
} // GetErrorString

#endif // _VAL_SCENE_
