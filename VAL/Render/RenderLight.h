//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderLight.h - 30/07/2000 - Warren Moore
//	Light render object
//
// $Id: RenderLight.h,v 1.0 2000/10/10 13:19:30 waz Exp $
//

#ifndef _VAL_RENDERLIGHT_
#define _VAL_RENDERLIGHT_

#pragma once

//#===--- Includes
#include "VAL.h"
#include "RenderObject.h"

//#===--- Defines

/////////////////
// CRenderLight

class CRenderLight {
public:
	CRenderLight(CRenderContext *poContext);
	virtual ~CRenderLight();

	//#===--- Initialisation/Shutdown
	void Init();

	//#===--- Settings

	//#===--- Render
	// Executes the object function
	virtual void Execute() = 0;

//#===--- Internal Functions
protected:

//#===--- Internal Data
protected:

};

//#===--- Inline Functions

#endif // _VAL_RENDERLIGHT_
