//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderObject.cpp - 28/02/2000 - Warren Moore
//	Base render object implementation
//
// $Id: RenderObject.cpp,v 1.4 2000/11/25 22:34:44 waz Exp $
//

#include "StdAfx.h"

#include "RenderObject.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CRenderObject

CRenderObject::CRenderObject(CRenderContext *poContext) : 
	// Set the object defaults
	m_poContext(poContext),
	m_bEnabled(true),
	m_fXPos(0.0F),
	m_fYPos(0.0F),
	m_fZPos(0.0F),
	m_fXAngle(0.0F),
	m_fYAngle(0.0F),
	m_fZAngle(0.0F),
	m_uMode(ROM_DEFAULT),
	m_fTransition(1.0F),
	m_uSelection(0) {
	ASSERT(poContext);
	// Store renderer attributes
	GetRendererParams();
} // Constructor

CRenderObject::~CRenderObject() {
} // Destructor

//#===--- Settings

void CRenderObject::Set(CRenderObject *poObject) {
	ASSERT(poObject);
	if (poObject) {
		// Copy the position
		m_fXPos = poObject->m_fXPos;
		m_fYPos = poObject->m_fYPos;
		m_fZPos = poObject->m_fZPos;
		// Copy the rotation
		m_fXAngle = poObject->m_fXAngle;
		m_fYAngle = poObject->m_fYAngle;
		m_fZAngle = poObject->m_fZAngle;
	}
} // Set

void CRenderObject::SetPosition(AXIS eAxis, float fPos) {
	switch (eAxis) {
		case X:
			m_fXPos = fPos;
			break;
		case Y:
			m_fYPos = fPos;
			break;
		case Z:
			m_fZPos = fPos;
			break;
	}
} // SetPosition

void CRenderObject::SetPosition(float fX, float fY, float fZ) {
	m_fXPos = fX;
	m_fYPos = fY;
	m_fZPos = fZ;
} // SetPosition

void CRenderObject::SetPosition(CRenderObject *poObject) {
	ASSERT(poObject);
	if (poObject) {
		// Copy the position
		m_fXPos = poObject->m_fXPos;
		m_fYPos = poObject->m_fYPos;
		m_fZPos = poObject->m_fZPos;
	}
} // SetRotation

void CRenderObject::TranslateBy(AXIS eAxis, float fPos) {
	switch (eAxis) {
		case X:
			m_fXAngle += fPos;
			break;
		case Y:
			m_fYAngle += fPos;
			break;
		case Z:
			m_fZAngle += fPos;
			break;
	}
} // RotateBy

void CRenderObject::SetRotation(AXIS eAxis, float fAngle) {
	switch (eAxis) {
		case X:
			m_fXAngle = fmod(fAngle, 360.0F);
			break;
		case Y:
			m_fYAngle = fmod(fAngle, 360.0F);
			break;
		case Z:
			m_fZAngle = fmod(fAngle, 360.0F);
			break;
	}
} // SetRotation

void CRenderObject::SetRotation(float fX, float fY, float fZ) {
	m_fXAngle = fmod(fX, 360.0F);
	m_fYAngle = fmod(fY, 360.0F);
	m_fZAngle = fmod(fZ, 360.0F);
} // SetRotation

void CRenderObject::SetRotation(CRenderObject *poObject) {
	ASSERT(poObject);
	if (poObject) {
		// Copy the rotation
		m_fXAngle = poObject->m_fXAngle;
		m_fYAngle = poObject->m_fYAngle;
		m_fZAngle = poObject->m_fZAngle;
	}
} // SetRotation

void CRenderObject::RotateBy(AXIS eAxis, float fAngle) {
	switch (eAxis) {
		case X:
			fAngle += m_fXAngle;
			m_fXAngle = fmod(fAngle, 360.0F);
			break;
		case Y:
			fAngle += m_fYAngle;
			m_fYAngle = fmod(fAngle, 360.0F);
			break;
		case Z:
			fAngle += m_fZAngle;
			m_fZAngle = fmod(fAngle, 360.0F);
			break;
	}
} // RotateBy

void CRenderObject::Transition(float fTransition) {
	m_fTransition = (fTransition < 0.0F) ? 0.0F : (fTransition > 1.0F) ? 1.0F : fTransition;
} // SetTransition

void CRenderObject::RenderMode(unsigned int uMode) {
	m_uMode = uMode;
} // RenderMode (Set)

void CRenderObject::GetRendererParams() {
	// Store the renderer mode
	m_poContext->GetOption(RCO_MODE, m_uContextMode);
} // GetRendererParams
