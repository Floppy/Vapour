//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scene.h - 06/10/2000 - Warren Moore
//	Base class for scene rendering classes
//
// $Id: Scene.h,v 1.1 2000/10/10 17:53:03 waz Exp $
//

#ifndef _VAL_SCENE_
#define _VAL_SCENE_

#pragma once

#include "VAL.h"

#include "RenderContextStore.h"
#include "RenderContext.h"

//#===--- Defines

//#===--- Data types
enum SCRESULT {
	SC_OK = 0,
	SC_OUT_OF_MEMORY,
	SC_NOT_IMPLEMENTED,
	SC_CONTEXT_UNAVAILABLE,
	SC_NO_CONTEXT,
	SC_NO_DATA,
	SC_SNAPSHOT_ERROR,
	SC_ERROR,
};

#define SC_ERROR_STRINGS \
	"OK", \
	"Out of memory", \
	"Function not implemented in this scene", \
	"Unable to create a suitable render context", \
	"No render context has been created", \
	"No data available to render", \
	"Error creating render snapshot", \
	"Unknown error", \

//#===--- External linkage dependencies

extern CRenderContextStore g_oRenderContextStore;

///////////
// CScene

class CScene {
public:
	// Constructor
	CScene();
	// Destructor - cleans up render context
	virtual ~CScene();

//#===--- External Functions
	// Creates the scene
	virtual SCRESULT Create() = 0;
	// Destroys the scene
	virtual SCRESULT Destroy() = 0;
	// Renders the scene to the context
	virtual SCRESULT Render() = 0;
	// Returns an image of the previously rendered scene
	virtual SCRESULT Snapshot(CImage *&poImage) {return SC_NOT_IMPLEMENTED; };

	//#===--- Diagnostics
	// Returns an error string for a specified error
	virtual const char *GetErrorString(SCRESULT eResult) const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	CRenderContext *m_poRC;										// Render context

	static const char *m_pcErrorString[];					// Error string table
};

//#===--- Inline Functions

inline const char *CScene::GetErrorString(SCRESULT eResult) const {
	return m_pcErrorString[(int)eResult];
} // GetErrorString

#endif // _VAL_SCENE_
