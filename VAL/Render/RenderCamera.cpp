//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderCamera.cpp - 23/11/2000 - Warren Moore
//	  Camera render object 
//
// $Id: RenderCamera.cpp,v 1.1 2000/11/25 22:34:44 waz Exp $
//

#include "StdAfx.h"

#include "RenderCamera.h"

#include "gl\gl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CRenderCamera

CRenderCamera::CRenderCamera(CRenderContext *poContext) : CRenderObject(poContext) {
} // Constructor

CRenderCamera::~CRenderCamera() {
} // Destructor

//#===--- Settings

void CRenderCamera::Execute() {
	// Check the render context mode
	if (m_uContextMode != RCV_OPENGL)
		return;
	// Check it's enabled
	if (!m_bEnabled)
		return;
	// Move to the object position
	glTranslatef(m_fXPos, m_fYPos, m_fZPos);
	// Rotate the object
	glRotatef(m_fXAngle, 1.0F, 0.0F, 0.0F);
	glRotatef(m_fYAngle, 0.0F, 1.0F, 0.0F);
	glRotatef(m_fZAngle, 0.0F, 0.0F, 1.0F);
} // Execute

