// Copyright 2000 Vapour Technology Ltd.
//
// RenderObject.h - 28/02/2000 - Warren Moore
//	Base render object header
//
// $Id: RenderObject.h,v 1.1 2000/07/29 13:14:26 waz Exp $
//

#ifndef _VAL_RENDEROBJECT_
#define _VAL_RENDEROBJECT_

#pragma once

//#===--- Includes
#include "VAL.h"
#include "RenderContext.h"
#include "gl\gl.h"

//#===--- Defines


//////////////////
// CRenderObject

class CRenderObject {
public:
	CRenderObject(CRenderContext *pContext);
	virtual ~CRenderObject();

	//#===--- Initialisation/Shutdown
	virtual void Init() {};
	virtual void Exit() {};

	//#===--- Settings
	// Set an object from another
	void Set(CRenderObject *pObject);

	// Modify the position on the object
	void SetPosition(AXIS eAxis, float fPos);
	void SetPosition(float fX, float fY, float fZ);
	void SetPosition(CRenderObject *pObject);
	void TranslateBy(AXIS eAxis, float fPos);
	void GetPosition(float &fX, float &fY, float &fZ);

	// Modify rotation of the object
	void SetRotation(AXIS eAxis, float fAngle);
	void SetRotation(float fX, float fY, float fZ);
	void SetRotation(CRenderObject *pObject);
	void RotateBy(AXIS eAxis, float fAngle);
	void GetRotation(float &fX, float &fY, float &fZ);

	// Set a transition value for the object
	void SetTransition(float fTransition);

	//#===--- Selection
	virtual void Selection(unsigned int uSelection);
	virtual unsigned int Selection() const;

	//#===--- Render mode control
	virtual void RenderMode(unsigned int uMode);
	unsigned int RenderMode() const;

	//#===--- Render
	// Executes the object function
	virtual void Execute() = 0;

//#===--- Internal Functions
protected:

//#===--- Internal Data
protected:
	CRenderContext *m_pContext;										// Render context
	float m_fXPos, m_fYPos, m_fZPos;							// Object position
	float m_fXAngle, m_fYAngle, m_fZAngle;				// Object rotation
	unsigned int m_eMode;													// Render mode
	float m_fTransition;													// Transition value
	unsigned int m_uSelection;										// Selection value
};

//#===--- Inline Functions

inline void CRenderObject::GetPosition(float &fX, float &fY, float &fZ) {
	fX = m_fXPos;
	fY = m_fYPos;
	fZ = m_fZPos;
} // GetPosition

inline void CRenderObject::GetRotation(float &fX, float &fY, float &fZ) {
	fX = m_fXAngle;
	fY = m_fYAngle;
	fZ = m_fZAngle;
} // GetRotation

inline void CRenderObject::Selection(unsigned int uSelection) {
	m_uSelection = uSelection;
} // Selection (Set)

inline unsigned int CRenderObject::Selection() const {
	return m_uSelection;
} // Selection (Get)

inline RENDERMODE CRenderObject::RenderMode() const {
	return m_eRenderMode;
} // RenderMode (Get)

#endif // _VAL_RENDEROBJECT_
