//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLBufferWin32.h - 23/07/2000 - Warren Moore
//	Render context for an OpenGL Win32 bitmap buffer
//
// $Id: RCOpenGLBufferWin32.h,v 1.3 2000/07/30 17:29:44 waz Exp $
//

#ifndef _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
#define _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_

#pragma once

#include "VAL.h"

#include "RenderContext.h"
#include "gl\gl.h"

//#===--- Defines

#define GLBW_MAX_TEXTURES					50

/////////////////////////
// CRCOpenGLBufferWin32

class CRCOpenGLBufferWin32 : public CRenderContext {
public:
	CRCOpenGLBufferWin32();
	~CRCOpenGLBufferWin32();

//#===--- External Functions
	//#===--- Status Functions
	// Returns the unique ID for the context
	const char *GetID() const;

	//#===--- Options
	// Context resolution (Note: Setting size destroys an active context)
	RCRESULT SetSize(unsigned int uWidth, unsigned int uHeight);
	// Flagged options
	RCRESULT SetOption(int iOption, unsigned int uValue);
	RCRESULT GetOption(int iOption, unsigned int &uValue);
	RCRESULT SetOption(int iOption, float fValue);
	RCRESULT GetOption(int iOption, float &fValue);

	//#===--- Context Control
	// Create the context as per the supplied options
	RCRESULT Create();
	// Destroy the current context
	RCRESULT Destroy();

	// Enable the context for use
	RCRESULT Enable();
	// Disable the context
	RCRESULT Disable();

	//#===--- Buffer Control
	// Sets the buffer to render to
	RCRESULT SetWriteBuffer(int iBuffer);
	// Clears the specified buffers
	RCRESULT ClearBuffer(unsigned int uBufFlags);

	//#===--- Texture Control
	// Import a texture into the context, returns a texture handle, -1 if failed
	int ImportTexture(CImage &oImage);
	// Delete all textures or by handle
	RCRESULT DeleteTextures();
	RCRESULT DeleteTexture(int iHandle);
	// Set the texture ready for use
	RCRESULT UseTexture(int iHandle);

	//#===--- View Properties
	// Refreshes (no args) the projection method, or sets a new one
	RCRESULT SetProjectionMode(unsigned int uMode = RCP_CURRENT);

	//#===--- Export
	// Create an image from the render area. poImage must be NULL if you want the image to be created
	RCRESULT Snapshot(CImage *&poImage);

protected:
//#===--- Internal Functions
	// Create the OpenGL context
	RCRESULT CreateContext();

//#===--- Internal Data
	// Context Info
	CDC *m_poDC;																	// Windows device context pointer
	HBITMAP	m_hBitmap, m_hOldBitmap;							// Windows DIBSection pointer
	unsigned char *m_pcData;											// Pointer to start of bitmap data
	HGLRC m_hGLRC;																// OpenGL render context handle
	int m_iPixelFormat;														// Current OpenGL pixel format
	// Texture Info
	unsigned int m_puTexNum[GLBW_MAX_TEXTURES];		// Texture handles
};

#endif // _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
