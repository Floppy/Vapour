//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderObject.h - 28/02/2000 - Warren Moore
//	Base render object header
//
// $Id: RenderObject.h,v 1.3 2000/10/10 17:52:51 waz Exp $
//

#ifndef _VAL_RENDEROBJECT_
#define _VAL_RENDEROBJECT_

#pragma once

//#===--- Includes
#include "VAL.h"
#include "RenderContext.h"

//#===--- Defines

// Render object modes
#define ROM_DEFAULT					0
#define ROM_WIRE						1
#define ROM_FLAT						2
#define ROM_TEXTURE					3
#define ROM_SELECTION				4

//////////////////
// CRenderObject

class CRenderObject {
public:
	CRenderObject(CRenderContext *poContext);
	virtual ~CRenderObject();

	//#===--- Initialisation/Shutdown
	virtual void Init() {};
	virtual void Exit() {};

	//#===--- Settings
	// Enable/Disable the object
	virtual void Enable();
	virtual void Disable();

	// Set an object from another
	void Set(CRenderObject *poObject);

	// Modify the position on the object
	void SetPosition(AXIS eAxis, float fPos);
	void SetPosition(float fX, float fY, float fZ);
	void SetPosition(CRenderObject *poObject);
	void TranslateBy(AXIS eAxis, float fPos);
	void GetPosition(float &fX, float &fY, float &fZ);

	// Modify rotation of the object
	void SetRotation(AXIS eAxis, float fAngle);
	void SetRotation(float fX, float fY, float fZ);
	void SetRotation(CRenderObject *poObject);
	void RotateBy(AXIS eAxis, float fAngle);
	void GetRotation(float &fX, float &fY, float &fZ);

	// Set a transition value for the object (between 0 and 1)
	void Transition(float fTransition);

	//#===--- Selection
	// Force a selection value
	virtual void Selection(unsigned int uSelection);
	// Return the current selection value
	virtual unsigned int Selection() const;

	//#===--- Render mode control
	virtual void RenderMode(unsigned int uMode);
	unsigned int RenderMode() const;

	//#===--- Render
	// Executes the object function
	virtual void Execute() = 0;

//#===--- Internal Functions
protected:
	// Stores redundant information concerning the render context
	virtual void GetRendererParams();

//#===--- Internal Data
protected:
	CRenderContext *m_poContext;									// Render context
	unsigned int m_uContextMode;									// Renderer mode info
	bool m_bEnabled;													// Enabled indicator
	float m_fXPos, m_fYPos, m_fZPos;								// Object position
	float m_fXAngle, m_fYAngle, m_fZAngle;						// Object rotation
	unsigned int m_uMode;											// Render mode
	float m_fTransition;												// Transition value
	unsigned int m_uSelection;										// Selection value
};

//#===--- Inline Functions

inline void CRenderObject::Enable() {
} // Enable

inline void CRenderObject::Disable() {

} // Disable

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

inline unsigned int CRenderObject::RenderMode() const {
	return m_uMode;
} // RenderMode (Get)

#endif // _VAL_RENDEROBJECT_
