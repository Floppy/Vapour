//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RCOpenGLWin32.cpp - 23/11/2000 - Warren Moore
//	Render context for an OpenGL window
//
// $Id: RCOpenGLWin32.cpp,v 1.5 2000/12/03 13:25:46 warren Exp $
//

#include "StdAfx.h"

#include "VALWin32.h"
#include "RenderContextProxy.h"
#include "RCOpenGLWin32.h"

#include "gl\glu.h"

#include <iostream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===--- Defines
#define RC_ID					"OpenGL Window Win32"
#define RC_VERSION			0.1F

extern CRenderContextStore g_oRenderContextStore;

///////////////////
// CRCOpenGLWin32

CRenderContextProxy<CRCOpenGLWin32> g_oRCProxyOpenGLWin32;

CRCOpenGLWin32::CRCOpenGLWin32(const CDisplayContext *poDisplay) : 
	CRenderContext(poDisplay),
	m_hDC(NULL),
	m_hGLRC(NULL),
	m_iPixelFormat(0),
	m_bDepthTest(true),
	m_uWidthOffset(0),
	m_uHeightOffset(0) {
	// Get the device context, if provided
	if (m_poDisplay) {
		void *pData = NULL;
		m_poDisplay->GetDisplayPointer(DC_WIN_DC, pData);
		m_hDC = (HDC)pData;
	}
	// Clear the texture table
	memset(m_puTexNum, 0, sizeof(m_puTexNum));
} // Contructor

CRCOpenGLWin32::~CRCOpenGLWin32() {
	if (m_bEnabled)
		Disable();
	if (m_bCreated)
		Destroy();
} // Destructor

//#===--- Options

const char *CRCOpenGLWin32::GetID() {
	return RC_ID;
} // GetID

float CRCOpenGLWin32::GetVersion() {
	return RC_VERSION;
} // GetVersion

RCRESULT CRCOpenGLWin32::SetSize(unsigned int uWidth, unsigned int uHeight) {
	CRenderContext::SetSize(uWidth, uHeight);
	return Resize();
} // SetSize

RCRESULT CRCOpenGLWin32::Resize() {
	// Stop here if nothing to change
	if ((m_uNewWidth == m_uWidth) && (m_uNewHeight == m_uHeight))
		return RC_OK;
	// Otherwise update new values
	m_uWidth = m_uNewWidth;
	m_uHeight = m_uNewHeight;
	// If context exists, reset the projection mode
	if (m_bCreated)
		return SetProjectionMode();
	// Create the new context
	return RC_OK;
} // Resize

RCRESULT CRCOpenGLWin32::SetOption(int iOption, unsigned int uValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_DEPTH_TEST:
			if (uValue == RCV_ENABLE)
				m_bDepthTest = true;
			else
				if (uValue == RCV_DISABLE)
					m_bDepthTest = false;
				else
					eResult = RC_INVALID_VALUE;
		case RCO_WIDTH_OFFSET:
			m_uWidthOffset = uValue;
			break;
		case RCO_HEIGHT_OFFSET:
			m_uHeightOffset = uValue;
			break;
		default:
			eResult =  CRenderContext::SetOption(iOption, uValue);
	}
	// Make any changes here
	return eResult;
} // SetOption (Int)

RCRESULT CRCOpenGLWin32::GetOption(int iOption, unsigned int &uValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_MODE:
			uValue = RCV_OPENGL;
			break;
		case RCO_MEDIUM:
			uValue = RCV_WINDOW;
			break;
		case RCO_VIEW:
			uValue = RCV_NORMAL;
			break;
		default:
			eResult = CRenderContext::GetOption(iOption, uValue);;
	}
	
	// Make any changes here
	return eResult;
} // GetOption (Int)

RCRESULT CRCOpenGLWin32::SetOption(int iOption, float fValue) {
	RCRESULT eResult = CRenderContext::SetOption(iOption, fValue);
	// Make any changes here
	return eResult;
} // SetOption (Float)

RCRESULT CRCOpenGLWin32::GetOption(int iOption, float &fValue) {
	RCRESULT eResult = CRenderContext::GetOption(iOption, fValue);
	// Make any changes here
	return eResult;
} // GetOption (Float)

//#===--- Context Control

RCRESULT CRCOpenGLWin32::CheckSelection(RCOptionListVector &oOptionList) {
	RCRESULT eResult = RC_OK;
	RCOptionListVector::iterator pOption = oOptionList.begin();
	// Loop through all options
	while ((eResult == RC_OK) && (pOption != oOptionList.end())) {
		SRCOptionTuple sOption = *pOption;
		if ((sOption.m_iOption != RCO_UNKNOWN) && (sOption.m_uValue != RCV_DONT_CARE)) {
			switch (sOption.m_iOption) {
				case RCO_UNKNOWN:
					break;
				case RCO_MODE:
					if (sOption.m_uValue != GetMode())
						eResult = RC_INCORRECT_MODE;
					break;
				case RCO_MEDIUM:
					if (sOption.m_uValue != RCV_WINDOW)
						eResult = RC_UNSUPPORTED_MEDIUM;
					break;
				case RCO_VIEW:
					if (sOption.m_uValue != RCV_NORMAL)
						eResult = RC_UNSUPPORTED_VIEW;
					break;
				default:
					eResult = CRenderContext::SetOption(sOption.m_iOption, sOption.m_uValue);
			}
		}
		// Step to the next option
		pOption++;
	}
	return eResult;
} // CheckSelection

RCRESULT CRCOpenGLWin32::Create() {
	if (m_bCreated)
		return RC_OK;
	// Create the pixel format
	RCRESULT eResult = CreateContext();
	// Set the creation flag, and initialise params. Delete if failed
	m_bCreated = (eResult == RC_OK);
	// Either set the projection mode or destroy the bitmap
	if (m_bCreated) {
		SetProjectionMode();
	}
	else
		Destroy();

	return eResult;
} // Create

RCRESULT CRCOpenGLWin32::CreateContext() {
	// Set context params
	memset(m_puTexNum, 0, sizeof(m_puTexNum));

	// Set the preferred pixel format options
	PIXELFORMATDESCRIPTOR sPixelFormat = {
		sizeof(PIXELFORMATDESCRIPTOR),					// structure size
		1,															// structure version
		PFD_DRAW_TO_WINDOW |									// set off-screen rendering
		PFD_SUPPORT_OPENGL |									// support OpenGL
		PFD_DOUBLEBUFFER,										// double buffered
		PFD_TYPE_RGBA,											// colour type
		m_uColourDepth,										// preferred colour depth
		0, 0, 0, 0, 0, 0,										// colour bits (ignored)
		0,															// no alpha buffer
		0,															// alpha bits
		0,															// no accumulation buffer
		0, 0, 0, 0,												// accum bits (ignored)
		m_uZBuffer,												// depth buffer
		0,															// no stencil buffer
		0,															// no auxiliary buffers
		PFD_MAIN_PLANE,										// main layer
		0,															// reserved
		0, 0, 0,													// no layer, visible, damage masks
	};

	// Choose an appriate pixel format
	m_iPixelFormat = ChoosePixelFormat(m_hDC, &sPixelFormat);

	// Error if pixel format returned 0
	if (m_iPixelFormat == 0)
		return RC_FORMAT_NOT_AVAILABLE;

	// Return error if not all parameters were met
	RCRESULT eResult = CheckContextParams(sPixelFormat);
	if (eResult != RC_OK)
		return eResult;

	// Set the returned pixel format
	if (SetPixelFormat(m_hDC, m_iPixelFormat, &sPixelFormat) != TRUE)
		return RC_SET_FORMAT_ERROR;

	// If a valid format set, create the context
	if ((m_hGLRC = wglCreateContext(m_hDC)) == NULL)
		return RC_CONTEXT_CREATE_ERROR;

	return RC_OK;
} // CreateContext

RCRESULT CRCOpenGLWin32::CheckContextParams(PIXELFORMATDESCRIPTOR sFormat) {
	// Return error if format does not support OpenGL
	if (!(sFormat.dwFlags & PFD_SUPPORT_OPENGL))
		return RC_FORMAT_NOT_AVAILABLE;
	// Return error if format does not support rendering to a memory bitmap
	if (!(sFormat.dwFlags & PFD_DRAW_TO_WINDOW))
		return RC_FORMAT_NOT_AVAILABLE;
	// Return error if format does not support double buffered rendering 
	if (!(sFormat.dwFlags & PFD_DOUBLEBUFFER))
		return RC_FORMAT_NOT_AVAILABLE;
	// Return error if palettized display given
	if (sFormat.dwFlags & PFD_NEED_PALETTE)
		return RC_FORMAT_NOT_AVAILABLE;
	// Check the colour depth
	if ((sFormat.iPixelType != PFD_TYPE_RGBA) || (sFormat.cColorBits != m_uColourDepth)) {
		m_uColourDepth = sFormat.cColorBits;
		return RC_UNSUPPORTED_COLOUR_DEPTH;
	}
	// Check the z-buffer size
	if (sFormat.cDepthBits != m_uZBuffer) {
		m_uZBuffer = sFormat.cDepthBits;
		return RC_UNSUPPORTED_Z_BUF_SIZE;
	}

	return RC_OK;
} // CheckContextParams

RCRESULT CRCOpenGLWin32::Destroy() {
	// Make sure the context isn't enabled
	if (m_bEnabled)
		Disable();
	// Don't destroy the device context, as it's not ours
	// Mark the context as detroyed
	m_bCreated = false;
	return RC_OK;
} // Destroy

RCRESULT CRCOpenGLWin32::Enable() {
	// Check we have an active context
	if (!m_bCreated)
		return RC_NOT_ACTIVE;
	wglMakeCurrent(m_hDC, m_hGLRC);
	m_bEnabled = true;
	return RC_OK;
} // Enable

RCRESULT CRCOpenGLWin32::Disable() {
	if (m_bCreated)
		wglMakeCurrent(NULL, NULL);
	m_bEnabled = false;
	return RC_OK;
} // Disable

RCRESULT CRCOpenGLWin32::BeginRender() {
	// Load the identity matrix
	glLoadIdentity();
	// Set the render options
	if (m_bDepthTest) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glClearDepth(1.0F);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	return RC_OK;
} // BeginRender

RCRESULT CRCOpenGLWin32::EndRender() {
	// Flush the back buffer to screen
	SwapBuffers(m_hDC);
	return RC_OK;
} // EndRender

RCRESULT CRCOpenGLWin32::SetWriteBuffer(int iBuffer) {
	return RC_OK;
} // SetWriteBUffer

RCRESULT CRCOpenGLWin32::ClearBuffer(unsigned int uBufFlags) {
	// Set the clear colour
	glClearColor(m_fBackRed, m_fBackGreen, m_fBackBlue, 0.0F);
	// Which buffers need clearing?
	unsigned int uGLFlags = 0;
	if (uBufFlags & RCB_COLOUR)
		uGLFlags |= GL_COLOR_BUFFER_BIT;
	if (uBufFlags & RCB_DEPTH)
		uGLFlags |= GL_DEPTH_BUFFER_BIT;
	// If valid, clear the buffers
	if (uGLFlags)
		glClear(uGLFlags);
	else
		return RC_WRONG_BUFFER;
	return RC_OK;
} // Clear Buffer

//#===--- Texture Control

int CRCOpenGLWin32::ImportTexture(CImage &oImage) {
	// Make sure we are created
	if (!m_bCreated)
		return -1;
	ASSERT(m_bEnabled);
	int iHandle = 0;
	// Find a free texture handle
	while (iHandle < GLBW_MAX_TEXTURES) {
		if (m_puTexNum[iHandle] == 0)
			break;
		iHandle++;
	}
	// If found, create the texture
	if (iHandle < GLBW_MAX_TEXTURES) {
		int iWidth = 0;
		int iHeight = 0;
		oImage.GetSize(iWidth, iHeight);
		// Create the texture object
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glGenTextures(1, &(m_puTexNum[iHandle]));
		ASSERT(m_puTexNum[iHandle] != 0);
		glBindTexture(GL_TEXTURE_2D, m_puTexNum[iHandle]);
		// Set the object parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iWidth, iHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, oImage.m_pData);
	}
	else
		// No free texture handle found
		return -1;
	// Return the texture handle
	return iHandle;
} // ImportTexture

RCRESULT CRCOpenGLWin32::DeleteTextures() {
	// Make sure we are created
	if (!m_bCreated)
		return RC_NOT_CREATED;
	ASSERT(m_bEnabled);
	// If texture exists, delete it
	int iHandle = 0;
	while (iHandle < GLBW_MAX_TEXTURES) {
		if (m_puTexNum[iHandle]) {
			glDeleteTextures(1, &(m_puTexNum[iHandle]));
			m_puTexNum[iHandle] = 0;
		}
		iHandle++;
	}
	return RC_OK;
} // DeleteTextures

RCRESULT CRCOpenGLWin32::DeleteTexture(int iHandle) {
	// Make sure we are created
	if (!m_bCreated)
		return RC_NOT_CREATED;
	ASSERT(m_bEnabled);
	RCRESULT eResult = RC_OK;
	// If texture exists, delete it
	if (m_puTexNum[iHandle]) {
		glDeleteTextures(1, &(m_puTexNum[iHandle]));
		m_puTexNum[iHandle] = 0;
	}
	else
		eResult = RC_NO_TEXTURE;
	return eResult;
} // DeleteTexture

RCRESULT CRCOpenGLWin32::UseTexture(int iHandle) {
	// Make sure we are created
	if (!m_bCreated)
		return RC_NOT_CREATED;
	// Make sure a valid texture handle is passed in
	if (iHandle < 0)
		return RC_NO_TEXTURE;
	RCRESULT eResult = RC_OK;
	// If texture exists, bind it
	if (m_puTexNum[iHandle])
		glBindTexture(GL_TEXTURE_2D, m_puTexNum[iHandle]);
	else
		eResult = RC_NO_TEXTURE;
	return eResult;
} // UseTexture

//#===--- Projection Control

RCRESULT CRCOpenGLWin32::SetProjectionMode(unsigned int uMode) {
	// Check the mode is supported
	if ((uMode != RCP_CURRENT) && (uMode != RCP_PERSPECTIVE))
		return RC_UNSUPPORTED_PROJECTION;
	// Store the mode
	if (uMode != RCP_CURRENT)
		m_uProjMode = uMode;
	// Set-up based on selected mode, if created
	RCRESULT eResult = RC_OK;
	if (m_bCreated && (Enable() == RC_OK)) {
		// Calculate the viewport aspect ratio
		GLdouble dAspect = (GLdouble) m_uWidth / (GLdouble) m_uHeight;
		// Select the projection method
		switch (m_uProjMode) {
			//#===--- Perspective
			case RCP_PERSPECTIVE:
				// Put the matrix in projection mode
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				// Set the perspective parameters
				gluPerspective(m_fViewAngle, dAspect, m_fNearPlane, m_fFarPlane);
				// Set the viewport to fill the context size
				glViewport(m_uWidthOffset, m_uHeightOffset, m_uWidth, m_uHeight);
				// Put the matrix in modelview mode
				glMatrixMode(GL_MODELVIEW);
				break;
			//#===--- Unsupported
			default:
				eResult = RC_UNSUPPORTED_PROJECTION;
		}
	}
	return eResult;
} // SetProjectionMode

//#===--- Export

RCRESULT CRCOpenGLWin32::Snapshot(CImage *&poImage) {
	/*
	// Check we have an active context
	if (!m_bCreated)
		return RC_NOT_ACTIVE;
	// Set the image type
	IMAGETYPE sType = IT_UNKNOWN;
	switch (m_uColourDepth) {
		case 24:
			sType = IT_RGB;
			break;
		case 8: 
			sType = IT_PALETTE;
			break;
		default:
			break;
	}
	// Change the image, or create a new one
	if (!poImage) {
		// Create the image
		if (sType != IT_UNKNOWN) {
			NEWBEGIN
			poImage = (CImage*) new CImage(sType, m_uWidth, m_uHeight);
			NEWEND("CRCOpenGLWin32::Snapshot - Snapshot image")
			if (!poImage)
				return RC_OUT_OF_MEMORY;
		}
		else
			return RC_INVALID_IMAGE_TYPE;
	}
	else {
		poImage->Convert(sType);
		poImage->SetSize(m_uWidth, m_uHeight);
	}
	// Check the image to ok
	int iWidth = 0, iHeight = 0;
	poImage->GetSize(iWidth, iHeight);
	if ((iWidth != m_uWidth) || (iHeight != m_uHeight))
		return RC_IMAGE_ERROR;
	// Copy out the bitmap data
	if (sType == IT_RGB) {
		// Get the RGB bitmap data
		CBitmap *poBitmap = m_poDC->GetCurrentBitmap();
		BITMAP sBitmap;
		poBitmap->GetBitmap(&sBitmap);
		const unsigned int uILineStep = sBitmap.bmWidthBytes;
		const unsigned int uOLineStep = poImage->m_iLineSize * 4;
		// Loop vars
		unsigned char *pcIData = m_pcData + (uILineStep * (m_uHeight - 1)); // DIB data
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
			pcIData -= uILineStep;
			pcOData += uOLineStep;
		}
	}
	else
		// Greyscale image
		memcpy(poImage->m_pData, m_pcData, m_uWidth * m_uHeight);

	return RC_OK;
	*/
	return RC_UNSUPPORTED_FUNCTION;
} // Snapshot