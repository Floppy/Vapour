//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLBufferWin32.cpp - 23/07/2000 - Warren Moore
//	Render context for an OpenGL bitmap buffer
//
// $Id: RCOpenGLBufferWin32.cpp,v 1.2 2000/07/24 21:08:24 waz Exp $
//

#include "StdAfx.h"

#include "VALWin32.h"
#include "RCOpenGLBufferWin32.h"

#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===--- Defines
#define RC_ID					"RCOpenGLBufferWin32-23072000"

////////////////////////////
// CRCOpenGLBufferWin32

CRCOpenGLBufferWin32::CRCOpenGLBufferWin32() : CRenderContext() {
	m_poDC = NULL;
	m_hBitmap = m_hOldBitmap = NULL;
	m_hGLRC = NULL;
	m_iPixelFormat = 0;
} // Contructor

CRCOpenGLBufferWin32::~CRCOpenGLBufferWin32() {
	if (m_bEnabled)
		Disable();
	if (m_bCreated)
		Destroy();
} // Destructor

const char *CRCOpenGLBufferWin32::GetID() const {
	return RC_ID;
} // GetID

void CRCOpenGLBufferWin32::SetOption(int iOption, unsigned int uValue) {
	// Too late if already created
	if (m_bCreated)
		return;
	// Process default options
	CRenderContext::SetOption(iOption, uValue);
	// Process specific options

} // SetOption (Unsigned Int)

void CRCOpenGLBufferWin32::GetOption(int iOption, unsigned int &uValue) {
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

RCRESULT CRCOpenGLBufferWin32::Create() {
	// Create the device context
	NEWBEGIN
	m_poDC = (CDC*) new CDC;
	NEWEND("CRCOpenGLBufferWin32::Create - Win Device Context")
	if (!m_poDC)
		return RC_ERROR;
	m_poDC->CreateCompatibleDC(NULL);
	// Create and set the BITMAPINFOHEADER
	BITMAPINFOHEADER sBIH;
	int iSize = sizeof(BITMAPINFOHEADER);
	memset(&sBIH, 0, iSize);
	// Fill in the header info. 
	sBIH.biSize = iSize;
	sBIH.biWidth = m_uWidth;
	sBIH.biHeight = m_uHeight;
	sBIH.biPlanes = 1;
	sBIH.biBitCount = m_uDepth;
	sBIH.biCompression = BI_RGB;
	// Create the DIB section for off-screen rendering
	m_hBitmap = CreateDIBSection(m_poDC->m_hDC,
															 (BITMAPINFO*) &sBIH,
															 DIB_PAL_COLORS,
															 (void**)(&m_pcData),
															 NULL,
															 0);
	// Select the new bitmap in the current DC
	if (m_hBitmap)
		m_hOldBitmap = (HBITMAP)::SelectObject(m_poDC->m_hDC, m_hBitmap);
	else 
		return RC_ERROR;
	// Create the pixel format
	RCRESULT eResult = SetPixelFormat();
	// Set the creation flag, and delete contexts if failed
	m_bCreated = (eResult == RC_OK);
	if (!m_bCreated)
		Destroy();

	return eResult;
} // Create

RCRESULT CRCOpenGLBufferWin32::SetPixelFormat() {
	// Set the preferred pixel format options
	PIXELFORMATDESCRIPTOR GLpfd = {
		sizeof(PIXELFORMATDESCRIPTOR),			// structure size
		1,																	// structure version
		PFD_DRAW_TO_BITMAP |								// properties
		PFD_SUPPORT_OPENGL |
		PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,											// colour type
		m_uDepth,														// preferred colour depth
		0, 0, 0, 0, 0, 0,										// colour bits (ignored)
		0,																	// no alpha buffer
		0,																	// alpha bits
		0,																	// no accumulation buffer
		0, 0, 0, 0,													// accum bits (ignored)
		32,																	// depth buffer
		0,																	// no stencil buffer
		0,																	// no auxiliary buffers
		PFD_MAIN_PLANE,											// main layer
		0,																	// reserved
		0, 0, 0,														// no layer, visible, damage masks
	};

	// Choose an appriate pixel format
	m_iPixelFormat = ::ChoosePixelFormat(m_poDC->m_hDC, &GLpfd);
	cout << "ChoosePixelFormat returned : " << m_iPixelFormat << endl;
	if (GLpfd.dwFlags & PFD_NEED_PALETTE) 
		cout << "Sorry, but 256 colour displays are not supported" << endl;

	// Error if pixel format 0 returned
	if (m_iPixelFormat == 0)
		return RC_ERROR;

	// Set the returned pixel format
	if (::SetPixelFormat(m_poDC->m_hDC, m_iPixelFormat, &GLpfd) != TRUE)
		return RC_ERROR;

	// Return error if palettized display given
//	if (GLpfd.dwFlags & PFD_NEED_PALETTE) {
//	}

	// If a valid format set, create the context
	if ((m_hGLRC = ::wglCreateContext(m_poDC->m_hDC)) == NULL)
		return RC_ERROR;

	return RC_OK;
} // SetPixelFormat

void CRCOpenGLBufferWin32::Destroy() {
	// Make sure the context isn't enabled
	if (m_bEnabled)
		Disable();
	// Delete the bitmap
	if (m_hBitmap) {
		// Select the old bitmap
		if (m_hOldBitmap)
			::SelectObject(m_poDC->m_hDC, m_hOldBitmap);
		// Delete the bitmap
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}
	// Destroy the device context
	if (m_poDC) {
		m_poDC->DeleteDC();
		delete m_poDC;
		m_poDC = NULL;
	}
	// Mark the context as detroyed
	m_bCreated = false;
} // Destroy

RCRESULT CRCOpenGLBufferWin32::Enable() {
	// Check we have an active context
	if (!m_bCreated)
		return RC_ERROR;
	::wglMakeCurrent(m_poDC->m_hDC, m_hGLRC);
	m_bEnabled = true;
	return RC_OK;
} // Enable

void CRCOpenGLBufferWin32::Disable() {
	::wglMakeCurrent(NULL, NULL);
	m_bEnabled = false;
} // Disable

RCRESULT CRCOpenGLBufferWin32::Snapshot(CImage *&poImage) {
	// Check we have an active context
	if (!m_bCreated)
		return RC_ERROR;
	// Set the image type
	IMAGETYPE sType = IT_UNKNOWN;
	switch (m_uDepth) {
		case 24:
			sType = IT_RGB;
			break;
		case 8: 
			sType = IT_PALETTE;
			break;
		default:
			ASSERT(false);
	}
	// Change the image, or create a new one
	if (!poImage) {
		// Create the image
		if (sType != IT_UNKNOWN) {
			NEWBEGIN
			poImage = (CImage*) new CImage(sType, m_uWidth, m_uHeight);
			NEWEND("CRCOpenGLBufferWin32::Snapshot - Snapshot image")
			if (!poImage)
				return RC_ERROR;
		}
		else
			return RC_ERROR;
	}
	else {
		poImage->Convert(sType);
		poImage->SetSize(m_uWidth, m_uHeight);
	}
	// Check the image to ok
	int iWidth = 0, iHeight = 0;
	poImage->GetSize(iWidth, iHeight);
	if ((iWidth != m_uWidth) || (iHeight != m_uHeight))
		return RC_ERROR;
	// Copy out the bitmap data
	if (sType == IT_RGB) {
		// Get the RGB bitmap data
		CBitmap *poBitmap = m_poDC->GetCurrentBitmap();
		BITMAP sBitmap;
		poBitmap->GetBitmap(&sBitmap);
		const unsigned int uILineStep = sBitmap.bmWidthBytes;
		const unsigned int uOLineStep = poImage->m_iLineSize * 4;
		// Loop vars
		unsigned char *pcIData = m_pcData; // DIB data
		unsigned char *pcIDataPtr = pcIData;
		unsigned char *pcOData = (unsigned char*)poImage->m_pData; // CImage object data
		unsigned char *pcODataPtr = pcOData;
		register unsigned int uX = m_uWidth, uY = m_uHeight;
		while (uY--) {
			pcIDataPtr = pcIData;
			pcODataPtr = pcOData;
			uX = m_uWidth;
			while (uX--) {
				// Read the pixels out in reverse order
				pcODataPtr[2] = *pcIDataPtr++;
				pcODataPtr[1] = *pcIDataPtr++;
				pcODataPtr[0] = *pcIDataPtr++;
				pcODataPtr += 3;
			}
			pcIData += uILineStep;
			pcOData += uOLineStep;
		}
	}
	else
		// Greyscale image
		memcpy(poImage->m_pData, m_pcData, m_uWidth * m_uHeight);

	return RC_OK;
} // Snapshot