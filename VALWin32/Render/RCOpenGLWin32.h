//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLWin32.h - 24/07/2000 - Warren Moore
//	Render context for an OpenGL Win32 bitmap buffer
//
// $Id: RCOpenGLWin32.h,v 1.1 2000/07/24 15:53:18 waz Exp $
//

#ifndef _VAL_RENDERCONTEXTOPENGLWIN32_
#define _VAL_RENDERCONTEXTOPENGLWIN32_

#pragma once

#include "VAL.h"

#include "RenderContext.h"
#include "gl\gl.h"

//#===--- Defines

///////////////////
// CRCOpenGLWin32

class CRCOpenGLWin32 : public CRenderContext {
public:
	CRCOpenGLWin32();
	virtual ~CRCOpenGLWin32();

//#===--- External Functions
	// Returns the unique ID for the context
	virtual const char *GetID() const;

	// Set a context option
	virtual void SetOption(int iOption, unsigned int uValue);
	// Retrieve context options
	virtual void GetOption(int iOption, unsigned int &uValue);

	// Create the context as per the supplied options
	virtual RCRESULT Create();
	// Destroy the current context
	virtual void Destroy();

	// Enable the context for use
	virtual RCRESULT Enable();
	// Disable the context
	virtual void Disable();

protected:
//#===--- Internal Functions
	// Create the OpenGL context
	RCRESULT SetPixelFormat();

//#===--- Internal Data
	CDC *m_poDC;							// Windows device context pointer
	HGLRC m_psGLRC;						// OpenGL render context handle
	int m_iPixelFormat;				// Current OpenGL pixel format
};

#endif // _VAL_RENDERCONTEXTOPENGLWIN32_
