//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderCamera.h - 25/11/2000 - Warren Moore
//	  Camera render object
//
// $Id: RenderCamera.h,v 1.1 2000/11/25 22:35:05 waz Exp $
//

#ifndef _VAL_RENDERCAMERA_
#define _VAL_RENDERCAMERA_

#pragma once

//#===--- Includes
#include "VAL.h"
#include "RenderObject.h"

//#===--- Defines

//////////////////
// CRenderCamera

class CRenderCamera : public CRenderObject {
public:
	CRenderCamera(CRenderContext *poContext);
	virtual ~CRenderCamera();

	//#===--- Initialisation/Shutdown

	//#===--- Settings

	//#===--- Render
	// Executes the object function
	// NB: Resets the projeciton matrix
	void Execute();

//#===--- Internal Functions
protected:

//#===--- Internal Data
protected:
	unsigned int m_uMode;
};

//#===--- Inline Functions

#endif // _VAL_RENDERCAMERA_
