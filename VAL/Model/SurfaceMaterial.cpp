//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.cpp - 17/11/2000 - James Smith
//	Surface material class implementation
//
// $Id: SurfaceMaterial.cpp,v 1.1 2000/11/30 09:37:06 warren Exp $
//

#include "stdafx.h"

#include "SurfaceMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
// Construction/Destruction ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Creates a surface material
CSurfaceMaterial::CSurfaceMaterial() :
	m_sDiffuse(),
	m_sSpecular(),
	m_sEmissive(0.0, 0.0, 0.0, 0.0),
	m_dShininess(0.0),
	m_pTexture(NULL) {
}

// Creates a surface material with a particular texture
CSurfaceMaterial::CSurfaceMaterial(CImage* pTexture) :
	m_sDiffuse(),
	m_sSpecular(),
	m_sEmissive(0.0, 0.0, 0.0, 0.0),
	m_dShininess(0.0),
	m_pTexture(NULL) {
} //CSurfaceMaterial(CImage * pTexture) 

// Destructor
CSurfaceMaterial::~CSurfaceMaterial() {
   // Destroy texture
   if (m_pTexture != NULL) delete m_pTexture;
   // Done
   return;
}

// Assigns the texture for the material - any old texture is automatically deleted
void CSurfaceMaterial::SetTexture(CImage* pTexture) {
   if (m_pTexture) delete m_pTexture;
   m_pTexture = pTexture;
} //SetTexture(CImage* pTexture)
