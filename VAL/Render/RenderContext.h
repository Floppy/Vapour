//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.h - 23/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.h,v 1.6 2000/11/21 16:37:53 waz Exp $
//

#ifndef _VAL_RENDERCONTEXT_
#define _VAL_RENDERCONTEXT_

#pragma once

#include "VAL.h"

#include "RenderContextStore.h"
#include "Image.h"

//#===--- Defines

//#===--- Options
#define RCO_UNKNOWN							0
	// Selection
#define RCO_MODE								1
#define RCO_MEDIUM							2
#define RCO_VIEW								3
	// Common
#define RCO_WIDTH								4
#define RCO_HEIGHT							5
#define RCO_COL_DEPTH						6
#define RCO_Z_BUF_SIZE						7
	// Context
#define RCO_NEARPLANE						8
#define RCO_FARPLANE							9
#define RCO_VIEWANGLE					  10
#define RCO_BACKRED						  11
#define RCO_BACKGREEN					  12
#define RCO_BACKBLUE						  13

//#===--- Option values
#define RCV_DONT_CARE						0
	// RCO_MODE
#define RCV_SOFTWARE							1
#define RCV_OPENGL							2
#define RCV_DIRECTX							3
	// RCO_MEDIUM
#define RCV_WINDOW							1
#define RCV_BUFFER							2
	// RCO_VIEW
#define RCV_NORMAL							1
#define RCV_STEREO							2
	// Buffer flags
#define RCB_FRONT								1
#define RCB_BACK								2
#define RCB_COLOUR							4
#define RCB_DEPTH								8
	// Projection modes
#define RCP_CURRENT							0
#define RCP_PERSPECTIVE						1
#define RCP_ORTHOGRAPHIC					2

//#===--- Data types
enum RCRESULT {
	RC_OK = 0,
	RC_OUT_OF_MEMORY,
	RC_INCORRECT_MODE,
	RC_UNSUPPORTED_MEDIUM,
	RC_UNSUPPORTED_VIEW,
	RC_MEDIA_ERROR,
	RC_FORMAT_NOT_AVAILABLE,
	RC_UNSUPPORTED_COLOUR_DEPTH,
	RC_UNSUPPORTED_Z_BUF_SIZE,
	RC_SET_FORMAT_ERROR,
	RC_CONTEXT_CREATE_ERROR,
	RC_NOT_ACTIVE,
	RC_NOT_CREATED,
	RC_INVALID_IMAGE_TYPE,
	RC_IMAGE_ERROR,
	RC_UNKNOWN_OPTION,
	RC_INVALID_VALUE,
	RC_NO_TEXTURE,
	RC_WRONG_BUFFER,
	RC_UNSUPPORTED_PROJECTION,
	RC_ERROR,
};

#define RC_ERROR_STRINGS \
	"OK", \
	"Out of memory", \
	"Incorrect mode", \
	"Unsupported medium",\
	"Unsupported view", \
	"Render media error", \
	"Render format not available", \
	"Renderer could not support required colour depth", \
	"Renderer could not support required z-buffer size", \
	"Error setting the chosen format", \
	"Error creating the render context", \
	"Context is not active", \
	"Context has not been created", \
	"An invalid image type has been supplied", \
	"An image error has occured", \
	"No matching option could be found", \
	"Invalid value supplied for option", \
	"Texture does not exist", \
	"An incorrect buffer has been specified", \
	"Specified projection method not supported", \
	"Unknown error", \

// DLL import/export definitions
#ifndef DLL
	#ifdef _VAPOUR_EXPORT_DLL_
		#define DLL __declspec(dllexport)
	#endif
	#ifdef _VAPOUR_IMPORT_DLL_
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

///////////////////
// CRenderContext

class CRenderContext {
public:
	CRenderContext();
	virtual ~CRenderContext();

//#===--- External Functions
	//#===--- Status Functions
	// Returns bool indicating whether the context has been created
	virtual bool Active() const;
	// Returns bool indicating whther the context has been enabled
	virtual bool Enabled() const;

	//#===--- Options
	// Render mode
	virtual unsigned int GetMode() const = 0;
	// Context resolution
	virtual RCRESULT SetSize(unsigned int uWidth, unsigned int uHeight);
	virtual RCRESULT GetSize(unsigned int &uWidth, unsigned int &uHeight);
	virtual RCRESULT Resize() = 0;
	// Flagged options
	virtual RCRESULT SetOption(int iOption, unsigned int uValue);
	virtual RCRESULT GetOption(int iOption, unsigned int &uValue);
	virtual RCRESULT SetOption(int iOption, float fValue);
	virtual RCRESULT GetOption(int iOption, float &fValue);

	//#===--- Context Control
	// Checks selection options for suitability, and passes in creatio options
	virtual RCRESULT CheckSelection(RCOptionListVector &oOptionList) = 0;
	// Create the context as per the supplied options
	virtual RCRESULT Create() = 0;
	// Destroy the current context
	virtual RCRESULT Destroy() = 0;

	// Enable the context for use
	virtual RCRESULT Enable() = 0;
	// Disable the context
	virtual RCRESULT Disable() = 0;

	//#===--- Buffer Control
	// Sets the buffer to render to
	virtual RCRESULT SetWriteBuffer(int iBuffer) = 0;
	// Clears the specified buffers
	virtual RCRESULT ClearBuffer(unsigned int iBufFlags) = 0;

	//#===--- Texture Control
	// Import a texture into the context, returns a texture handle
	virtual int ImportTexture(CImage &oImage) = 0;
	// Delete all textures or by handle
	virtual RCRESULT DeleteTextures() = 0;
	virtual RCRESULT DeleteTexture(int iHandle) = 0;
	// Set the texture ready for use
	virtual RCRESULT UseTexture(int iHandle) = 0;

	//#===--- View Properties
	// Refreshes (no args) the projection method, or sets a new one
	virtual RCRESULT SetProjectionMode(unsigned int uMode = RCP_CURRENT) = 0;
	// Returns the current projection method
	virtual unsigned int GetProjectionMode() const;

	//#===--- Export
	// Create an image from the render area. poImage must be NULL if you want the image to be created
	virtual RCRESULT Snapshot(CImage *&poImage) = 0;

	//#===--- Diagnostics
	// Returns an error string for a specified error
	virtual const char *GetErrorString(RCRESULT eResult) const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	unsigned int m_uWidth, m_uHeight;						// Active context resolution
	unsigned int m_uNewWidth, m_uNewHeight;				// Next context resolution
	unsigned int m_uColourDepth;								// Colour depth (in bits per pixel)
	unsigned int m_uZBuffer;									// Z buffer size (in bits per pixel)
	float m_fNearPlane, m_fFarPlane;							// View clipping planes
	float m_fViewAngle;											// Camera view angle
	float m_fBackRed, m_fBackGreen, m_fBackBlue;			// Context clear colour
	unsigned int m_uProjMode;									// Current projection mode

	bool m_bCreated;												// Context created indicator
	bool m_bEnabled;												// Context enabled indicator

	static const char *m_pcErrorString[];					// Error string table
};

//#===--- Inline Functions

inline bool CRenderContext::Active() const {
	return m_bCreated;
} // Active

inline bool CRenderContext::Enabled() const {
	return m_bEnabled;
} // Enabled

inline unsigned int CRenderContext::GetProjectionMode() const {
	return m_uProjMode;
} // GetProjectionMode

inline const char *CRenderContext::GetErrorString(RCRESULT eResult) const {
	return m_pcErrorString[(int)eResult];
} // GetErrorString

#endif // _VAL_RENDERCONTEXT_
