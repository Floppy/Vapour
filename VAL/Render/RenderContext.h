//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.h - 23/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.h,v 1.2 2000/07/24 21:10:59 waz Exp $
//

#ifndef _VAL_RENDERCONTEXT_
#define _VAL_RENDERCONTEXT_

#pragma once

#include "VAL.h"

#include "Image.h"

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
	//#===--- Status Functions
	// Returns the unique ID for the context
	virtual const char *GetID() const;
	// Returns bool indicating whether the context has been created
	virtual bool Active() const;

	// Set a context option
	virtual void SetOption(int iOption, unsigned int uValue);
	// Retrieve context options
	virtual void GetOption(int iOption, unsigned int &uValue);

	// Create the context as per the supplied options
	virtual RCRESULT Create() = 0;
	// Destroy the current context
	virtual void Destroy() = 0;

	// Enable the context for use
	virtual RCRESULT Enable() = 0;
	// Disable the context
	virtual void Disable() = 0;

	// Create an image from the render area
	virtual RCRESULT Snapshot(CImage *&poImage) = 0;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	unsigned int m_uWidth, m_uHeight;							// Context resolution
	unsigned int m_uDepth;												// Colour depth (in bits per pixel)

	bool m_bCreated;															// Context created indicator
	bool m_bEnabled;															// Context enabled indicator
};

//#===--- Inline Functions

inline bool CRenderContext::Active() const {
	return m_bCreated;
} // Active

#endif // _VAL_RENDERCONTEXT_
