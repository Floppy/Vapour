//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.cpp - 17/11/2000 - James Smith
//	Surface material class implementation
//
// $Id: SurfaceMaterial.cpp,v 1.0 2000/11/21 16:47:15 waz Exp $
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
CSurfaceMaterial::CSurfaceMaterial() {
   // Set diffuse colour
   m_colDiffuse.m_dRed   = 1.0;
   m_colDiffuse.m_dGreen = 1.0;
   m_colDiffuse.m_dBlue  = 1.0;
   m_colDiffuse.m_dAlpha = 1.0;
   // Set specular colour
   m_colSpecular.m_dRed   = 1.0;
   m_colSpecular.m_dGreen = 1.0;
   m_colSpecular.m_dBlue  = 1.0;
   m_colSpecular.m_dAlpha = 1.0;
   // Set emissive colour
   m_colEmissive.m_dRed   = 0.0;
   m_colEmissive.m_dGreen = 0.0;
   m_colEmissive.m_dBlue  = 0.0;
   m_colEmissive.m_dAlpha = 0.0;
   // Set shininess
   m_dShininess = 0;
   // Initialise texture
   m_pTexture = NULL;
   // Done
   return;
}

// Creates a surface material with a particular texture
CSurfaceMaterial::CSurfaceMaterial(CImage* pTexture) {
   // Set diffuse colour
   m_colDiffuse.m_dRed   = 1.0;
   m_colDiffuse.m_dGreen = 1.0;
   m_colDiffuse.m_dBlue  = 1.0;
   m_colDiffuse.m_dAlpha = 1.0;
   // Set specular colour
   m_colSpecular.m_dRed   = 1.0;
   m_colSpecular.m_dGreen = 1.0;
   m_colSpecular.m_dBlue  = 1.0;
   m_colSpecular.m_dAlpha = 1.0;
   // Set emissive colour
   m_colEmissive.m_dRed   = 0.0;
   m_colEmissive.m_dGreen = 0.0;
   m_colEmissive.m_dBlue  = 0.0;
   m_colEmissive.m_dAlpha = 0.0;
   // Set shininess
   m_dShininess = 0;
   // Initialise texture
   m_pTexture = pTexture;
   // Done
   return;} //CSurfaceMaterial(CImage * pTexture) 


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
