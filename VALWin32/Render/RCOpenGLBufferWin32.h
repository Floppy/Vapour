//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLBufferWin32.h - 23/07/2000 - Warren Moore
//	Render context for an OpenGL Win32 bitmap buffer
//
// $Id: RCOpenGLBufferWin32.h,v 1.2 2000/07/24 21:08:25 waz Exp $
//

#ifndef _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
#define _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_

#pragma once

#include "VAL.h"

#include "RenderContext.h"
#include "gl\gl.h"

//#===--- Defines

/////////////////////////
// CRCOpenGLBufferWin32

class CRCOpenGLBufferWin32 : public CRenderContext {
public:
	CRCOpenGLBufferWin32();
	virtual ~CRCOpenGLBufferWin32();

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

	// Create an image from the render area
	virtual RCRESULT Snapshot(CImage *&poImage);

protected:
//#===--- Internal Functions
	// Create the OpenGL context
	RCRESULT SetPixelFormat();

//#===--- Internal Data
	CDC *m_poDC;															// Windows device context pointer
	HBITMAP	m_hBitmap, m_hOldBitmap;					// Windows DIBSection pointer
	unsigned char *m_pcData;									// Pointer to start of bitmap data
	HGLRC m_hGLRC;														// OpenGL render context handle
	int m_iPixelFormat;												// Current OpenGL pixel format
};

#endif // _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
