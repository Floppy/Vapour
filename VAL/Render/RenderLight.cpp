//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderLight.cpp - 30/07/2000 - Warren Moore
//	Light render object 
//
// $Id: RenderLight.cpp,v 1.0 2000/10/10 13:19:30 waz Exp $
//

#include "StdAfx.h"

#include "RenderLight.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////
// CRenderLight

CRenderLight::CRenderLight(CRenderContext *poContext) : CRenderObject(poContext) {
	ASSERT(poContext);
} // Constructor

CRenderLight::~CRenderLight() {
} // Destructor

//#===--- Settings

void CRenderLight::Execute() {
} // Execute
