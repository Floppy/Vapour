//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextOGLBuffer.cpp - 23/07/2000 - Warren Moore
//	Render context for an OpenGL bitmap buffer
//
// $Id: RCOpenGLBufferWin32.cpp,v 1.1 2000/07/24 12:38:11 waz Exp $
//

#include "StdAfx.h"

#include "VALWin32.h"
#include "RenderContextOGLBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////
// CRenderContextOGLBuffer

CRenderContextOGLBuffer::CRenderContextOGLBuffer() {
} // Contructor

CRenderContextOGLBuffer::~CRenderContextOGLBuffer() {
} // Destructor

void CRenderContextOGLBuffer::SetOption(int iOption, unsigned int uValue) {
	// Too late if already created
	if (m_bCreated)
		return;
	// Process default options
	CRenderContext::SetOption(iOption, uValue);
	// Process specific options

} // SetOption (Unsigned Int)

void CRenderContextOGLBuffer::GetOption(int iOption, unsigned int &uValue) {
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
	}
} // GetOption (Unsigned Int)

