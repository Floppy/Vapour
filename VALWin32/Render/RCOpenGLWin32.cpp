//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLWin32.cpp - 24/07/2000 - Warren Moore
//	Render context for an OpenGL bitmap buffer
//
// $Id: RCOpenGLWin32.cpp,v 1.1 2000/07/24 15:53:18 waz Exp $
//

#include "StdAfx.h"

#include "VALWin32.h"
#include "RCOpenGLWin32.h"

#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===--- Defines
#define RC_ID					"RCOpenGLWin32-24072000"

///////////////////
// CRCOpenGLWin32

CRCOpenGLWin32::CRCOpenGLWin32() : CRenderContext() {
	m_poDC = NULL;
	m_psGLRC = NULL;
	m_iPixelFormat = 0;
} // Contructor

CRCOpenGLWin32::~CRCOpenGLWin32() {
	if (m_bEnabled)
		Disable();
	if (m_bCreated)
		Destroy();
} // Destructor

const char *CRCOpenGLWin32::GetID() const {
	return RC_ID;
} // GetID

void CRCOpenGLWin32::SetOption(int iOption, unsigned int uValue) {
	// Too late if already created
	if (m_bCreated)
		return;
	// Process default options
	CRenderContext::SetOption(iOption, uValue);
	// Process specific options

} // SetOption (Unsigned Int)

void CRCOpenGLWin32::GetOption(int iOption, unsigned int &uValue) {
	// Search for default options
	CRenderContext::GetOption(iOption, uValue);
	// Override for specific options
	switch (iOption) {
		case RC_WIDTH:
			uValue = m_uWidth;
			break;
		case RC_HEIGHT:
			uValue = m_uHeight;
			break;
		case RC_DEPTH:
			uValue = m_uDepth;
			break;
		default:
			break;
	}
} // GetOption (Unsigned Int)

RCRESULT CRCOpenGLWin32::Create() {
	// Create the device context
	NEWBEGIN
	m_poDC = (CDC*) new CDC;
	NEWEND("CRCOpenGLWin32::Create - Win Device Context")
	if (!m_poDC)
		return RC_ERROR;
	if (m_poDC->CreateDC("DISPLAY", NULL, NULL, NULL) == 0)
		return RC_ERROR;
	// Create the pixel format
	RCRESULT eResult = SetPixelFormat();
	// Set the creation flag, and delete contexts if failed
	m_bCreated = (eResult == RC_OK);
	if (!m_bCreated)
		Destroy();

	return eResult;
} // Create

RCRESULT CRCOpenGLWin32::SetPixelFormat() {
	// Set the preferred pixel format options
	PIXELFORMATDESCRIPTOR GLpfd = {
		sizeof(PIXELFORMATDESCRIPTOR),			// structure size
		1,																	// structure version
		PFD_SUPPORT_OPENGL |								// properties
		PFD_DRAW_TO_WINDOW,
		PFD_TYPE_RGBA,											// colour type
		m_uDepth,														// preferred colour depth
		0, 0, 0, 0, 0, 0,										// colour bits (ignored)
		0,																	// no alpha buffer
		0,																	// alpha bits
		0,																	// no accumulation buffer
		0, 0, 0, 0,													// accum bits (ignored)
		16,																	// depth buffer
		0,																	// no stencil buffer
		0,																	// no auxiliary buffers
		PFD_MAIN_PLANE,											// main layer
		0,																	// reserved
		0, 0, 0,														// no layer, visible, damage masks
	};

	// Choose an appriate pixel format
	m_iPixelFormat = ::ChoosePixelFormat(m_poDC->GetSafeHdc(), &GLpfd);
	cout << "GL ChoosePixelFormat returned : " << m_iPixelFormat << endl;
	if (GLpfd.dwFlags & PFD_NEED_PALETTE) 
		cout << "Sorry, but 256 colour displays are not supported" << endl;

	// Error if pixel format 0 returned
	if (m_iPixelFormat == 0) {
		DWORD uError = GetLastError();

		return RC_ERROR;
	}

	// Set the returned pixel format
	if (::SetPixelFormat(m_poDC->GetSafeHdc(), m_iPixelFormat, &GLpfd) != TRUE)
		return RC_ERROR;

	// Return error if palettized display given
//	if (GLpfd.dwFlags & PFD_NEED_PALETTE) {
//		return VGL_NEEDPALETTE;
//	}

	return RC_OK;
} // SetPixelFormat

void CRCOpenGLWin32::Destroy() {
	// Make sure the context isn't enabled
	if (m_bEnabled)
		Disable();
	// Destroy the device context
	if (m_poDC) {
		m_poDC->DeleteDC();
		delete m_poDC;
		m_poDC = NULL;
	}
	// Mark the context as detroyed
	m_bCreated = false;
} // Destroy

RCRESULT CRCOpenGLWin32::Enable() {
	return RC_OK;
} // Enable

void CRCOpenGLWin32::Disable() {
} // Disable

