//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.cpp - 17/11/2000 - James Smith
//	Surface material class implementation
//
// $Id: SurfaceMaterial.cpp,v 1.3 2000/12/02 13:51:43 james Exp $
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
	m_oDiffuse(0.9F, 1.0F),
   m_oAmbient(0.4F, 1.0F),
	m_oSpecular(0.0F, 1.0F),
	m_oEmissive(0.0F, 0.0F),
	m_fShininess(0.0F),
	m_poTexture(NULL) {
}

// Creates a surface material with a particular texture
CSurfaceMaterial::CSurfaceMaterial(CImage *poTexture) :
	m_oDiffuse(0.9F, 1.0F),
   m_oAmbient(0.4F, 1.0F),
	m_oSpecular(0.0F, 1.0F),
	m_oEmissive(0.0F, 0.0F),
	m_fShininess(0.0F),
	m_poTexture(poTexture) {
} //CSurfaceMaterial(CImage * pTexture) 

// Destructor
CSurfaceMaterial::~CSurfaceMaterial() {
   // Destroy texture
   if (m_poTexture != NULL)
		delete m_poTexture;
}

// Assigns the texture for the material - any old texture is automatically deleted
void CSurfaceMaterial::SetTexture(CImage *poTexture) {
   if (m_poTexture)
		delete m_poTexture;
   m_poTexture = poTexture;
} //SetTexture(CImage *poTexture)
