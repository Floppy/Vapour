// Copyright 2000 Vapour Technology Ltd.
//
// RenderObject.cpp - 28/02/2000 - Warren Moore
//	Base render object implementation
//
// $Id: RenderObject.cpp,v 1.1 2000/07/29 13:14:26 waz Exp $
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

CRenderObject::CRenderObject(CScene *pScene) {
	ASSERT(pScene);
	m_pScene = pScene;
	m_fXPos = m_fYPos = m_fZPos = 0.0F;
	m_fXAngle = m_fYAngle = m_fZAngle = 0.0F;
	m_eRenderMode = RM_DEFAULT;
	m_fTransition = 1.0F;
	m_uSelection = 0;
} // Constructor

CRenderObject::~CRenderObject() {
} // Destructor

//#===--- Settings

void CRenderObject::Set(CRenderObject *pObject) {
	ASSERT(pObject);
// Copy the position
	m_fXPos = pObject->m_fXPos;
	m_fYPos = pObject->m_fYPos;
	m_fZPos = pObject->m_fZPos;
// Copy the rotation
	m_fXAngle = pObject->m_fXAngle;
	m_fYAngle = pObject->m_fYAngle;
	m_fZAngle = pObject->m_fZAngle;
} // Set

void CRenderObject::SetPosition(AXIS eAxis, float fPos) {
	switch (eAxis) {
		case X:
			m_fXPos = (GLfloat)fPos;
			break;
		case Y:
			m_fYPos = (GLfloat)fPos;
			break;
		case Z:
			m_fZPos = (GLfloat)fPos;
			break;
	}
} // SetPosition

void CRenderObject::SetPosition(float fX, float fY, float fZ) {
	m_fXPos = (GLfloat)fX;
	m_fYPos = (GLfloat)fY;
	m_fZPos = (GLfloat)fZ;
} // SetPosition

void CRenderObject::SetPosition(CRenderObject *pObject) {
	ASSERT(pObject);
// Copy the position
	m_fXPos = pObject->m_fXPos;
	m_fYPos = pObject->m_fYPos;
	m_fZPos = pObject->m_fZPos;
} // SetRotation

void CRenderObject::TranslateBy(AXIS eAxis, float fPos) {
	switch (eAxis) {
		case X:
			m_fXAngle += (GLfloat)fPos;
			break;
		case Y:
			m_fYAngle += (GLfloat)fPos;
			break;
		case Z:
			m_fZAngle += (GLfloat)fPos;
			break;
	}
} // RotateBy

void CRenderObject::SetRotation(AXIS eAxis, float fAngle) {
	switch (eAxis) {
		case X:
			m_fXAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
		case Y:
			m_fYAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
		case Z:
			m_fZAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
	}
} // SetRotation

void CRenderObject::SetRotation(float fX, float fY, float fZ) {
	m_fXAngle = (GLfloat)(fmod(fX, 360.0F));
	m_fYAngle = (GLfloat)(fmod(fY, 360.0F));
	m_fZAngle = (GLfloat)(fmod(fZ, 360.0F));
} // SetRotation

void CRenderObject::SetRotation(CRenderObject *pObject) {
	ASSERT(pObject);
// Copy the rotation
	m_fXAngle = pObject->m_fXAngle;
	m_fYAngle = pObject->m_fYAngle;
	m_fZAngle = pObject->m_fZAngle;
} // SetRotation

void CRenderObject::RotateBy(AXIS eAxis, float fAngle) {
	switch (eAxis) {
		case X:
			fAngle += (float)m_fXAngle;
			m_fXAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
		case Y:
			fAngle += (float)m_fYAngle;
			m_fYAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
		case Z:
			fAngle += (float)m_fZAngle;
			m_fZAngle = (GLfloat)(fmod(fAngle, 360.0F));
			break;
	}
} // RotateBy

void CRenderObject::SetTransition(float fTransition) {
	m_fTransition = (fTransition < 0.0F) ? 0.0F : (fTransition > 1.0F) ? 1.0F : fTransition;
} // SetTransition

void CRenderObject::RenderMode(RENDERMODE eMode) {
	m_eRenderMode = eMode;
} // RenderMode (Set)
