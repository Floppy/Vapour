//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLBufferWin32.h - 23/07/2000 - Warren Moore
//	Render context for an OpenGL Win32 bitmap buffer
//
// $Id: RCOpenGLBufferWin32.h,v 1.5 2000/11/25 22:41:15 waz Exp $
//

#ifndef _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
#define _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_

#pragma once

#include "VAL.h"

#include "RenderContextStore.h"
#include "RenderContext.h"
#include "gl\gl.h"

//#===--- Defines

#define GLBW_MAX_TEXTURES					50

/////////////////////////
// CRCOpenGLBufferWin32

class CRCOpenGLBufferWin32 : public CRenderContext {
public:
	CRCOpenGLBufferWin32(CDisplayContext *poDisplay = NULL);
	~CRCOpenGLBufferWin32();

//#===--- RenderContextProxy Functions
	//#===--- Status Functions
	// Returns the unique ID for the context
	static const char *GetID();
	// Returns the version number of the context
	static float GetVersion();

//#===--- External Functions
	//#===--- Options
	// Render mode
	unsigned int GetMode() const;
	// Context resolution
	RCRESULT SetSize(unsigned int uWidth, unsigned int uHeight);
	RCRESULT Resize();
	// Flagged options
	RCRESULT SetOption(int iOption, unsigned int uValue);
	RCRESULT GetOption(int iOption, unsigned int &uValue);
	RCRESULT SetOption(int iOption, float fValue);
	RCRESULT GetOption(int iOption, float &fValue);

	//#===--- Context Control
	// Checks selection options for suitability, and passes in creatio options
	RCRESULT CheckSelection(RCOptionListVector &oOptionList);
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
	// Creates the memory bitmap render surface
	RCRESULT CreateBitmap();
	// Destroys the memory bitmap
	void DestroyBitmap();
	// Create the OpenGL context
	RCRESULT CreateContext();
	// Checks the format selected with the specified params
	// NB: Sets internal params to match those it could support
	RCRESULT CheckContextParams(PIXELFORMATDESCRIPTOR sFormat);

//#===--- Internal Data
	// Context Info
	CDC *m_poDC;														// Windows device context pointer
	HBITMAP	m_hBitmap, m_hOldBitmap;							// Windows DIBSection pointer
	unsigned char *m_pcData;										// Pointer to start of bitmap data
	HGLRC m_hGLRC;														// OpenGL render context handle
	int m_iPixelFormat;												// Current OpenGL pixel format
	// Texture Info
	unsigned int m_puTexNum[GLBW_MAX_TEXTURES];				// Texture handles
};

//#===--- Inline functions

inline unsigned int CRCOpenGLBufferWin32::GetMode() const {
	return RCV_OPENGL;
} // GetMode

#endif // _VAL_RENDERCONTEXTOPENGLBUFFERWIN32_
