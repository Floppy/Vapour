//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/material.cpp"
//! author     = "James Smith"
//! date       = "09/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: material.cpp,v 1.6 2001/10/24 22:23:53 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Materials"

#include "VALET/material.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {
  
   // Creates a surface material
   CMaterial::CMaterial() :
      m_oAmbient(0.4F, 0.4F, 0.4F, 1.0F),
      m_oDiffuse(0.9F, 0.9F, 0.9F, 1.0F),
      m_oSpecular(0.0F, 0.0F, 0.0F, 1.0F),
      m_oEmissive(0.0F, 0.0F, 0.0F, 0.0F),
      m_dShininess(0.0F)
   {
      CLog oLog("3d","CMaterial::Constructor (default)",LL_OBJECT);
   }
  
   // Destructor
   CMaterial::~CMaterial() {
      CLog oLog("3d","CMaterial::Destructor",LL_OBJECT);
   }

   CMaterialColour CMaterial::Ambient(void) const {
      CLog oLog("3d","CMaterial::Ambient (const)",LL_FUNCTION);
      return m_oAmbient; 
   }
    
   CMaterialColour& CMaterial::Ambient(void) {
      CLog oLog("3d","CMaterial::Ambient",LL_FUNCTION);
      return m_oAmbient; 
   }
    
   CMaterialColour CMaterial::Diffuse(void) const {
      CLog oLog("3d","CMaterial::Diffuse (const)",LL_FUNCTION);
      return m_oDiffuse; 
   }

   CMaterialColour& CMaterial::Diffuse(void) {
      CLog oLog("3d","CMaterial::Diffuse",LL_FUNCTION);
      return m_oDiffuse; 
   }
    
   CMaterialColour CMaterial::Specular(void) const {
      CLog oLog("3d","CMaterial::Specular (const)",LL_FUNCTION);
      return m_oSpecular; 
   }

   CMaterialColour& CMaterial::Specular(void) { 
      CLog oLog("3d","CMaterial::Specular",LL_FUNCTION);
      return m_oSpecular; 
   }
    
   CMaterialColour CMaterial::Emissive(void) const { 
      CLog oLog("3d","CMaterial::Emissive (const)",LL_FUNCTION);
      return m_oEmissive; 
   }

   CMaterialColour& CMaterial::Emissive(void) { 
      CLog oLog("3d","CMaterial::Emissive",LL_FUNCTION);
      return m_oEmissive; 
   }
    
   double CMaterial::Shininess(void) const {
      CLog oLog("3d","CMaterial::Shininess (const)",LL_FUNCTION);
      return m_dShininess; 
   }

   double& CMaterial::Shininess(void) {
      CLog oLog("3d","CMaterial::Shininess",LL_FUNCTION);
      return m_dShininess; 
   }
  
}


