//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderLight.cpp - 30/07/2000 - Warren Moore
//	Light render object 
//
// $Id: RenderLight.cpp,v 1.1 2000/10/10 17:52:04 waz Exp $
//

#include "StdAfx.h"

#include "RenderLight.h"

#include "gl\gl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////
// CRenderLight

CRenderLight::CRenderLight(CRenderContext *poContext) : CRenderObject(poContext) {
	ASSERT(poContext);
	m_bEnabled = false;
} // Constructor

CRenderLight::~CRenderLight() {
} // Destructor

void CRenderLight::Enable() {
	// Check the render context mode
	if (m_uContextMode != RCV_OPENGL)
		return;
	// Enable the light
	m_bEnabled = true;
	glEnable(GL_LIGHT0);
} // Enable

void CRenderLight::Disable() {
	// Check the render context mode
	if (m_uContextMode != RCV_OPENGL)
		return;
	// Disable the light
	m_bEnabled = false;
	glDisable(GL_LIGHT0);
} // Disable

//#===--- Settings

void CRenderLight::Execute() {
	// Check the render context mode
	if (m_uContextMode != RCV_OPENGL)
		return;
	// Check it's enabled
	if (!m_bEnabled)
		return;
	// Reset the matrix
	glLoadIdentity();
	// Place the light
	GLfloat fLAmbient[4] = {0.1F, 0.1F, 0.1F, 1.0F};
	GLfloat fLWhite[4] = {1.0F, 1.0F, 1.0F, 1.0F};
	glLightfv(GL_LIGHT0, GL_AMBIENT, fLAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fLWhite);
	glLightfv(GL_LIGHT0, GL_SPECULAR, fLWhite);
	GLfloat fLPos[4] = { m_fXPos, m_fYPos, m_fZPos, 1.0F};
	glLightfv(GL_LIGHT0, GL_POSITION, fLPos);
} // Execute

