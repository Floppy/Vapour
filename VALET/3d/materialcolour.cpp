//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/materialcolour.cpp"
//! author     = "James Smith"
//! date       = "09/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: materialcolour.cpp,v 1.8 2001/10/30 01:16:00 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Materials"

#include "VALET/materialcolour.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {

   CMaterialColour::CMaterialColour() {
      CLog oLog("3d","CMaterialColour::Constructor (default)",LL_OBJECT);
      m_pfComponents[0] = 1.0;
      m_pfComponents[1] = 1.0;
      m_pfComponents[2] = 1.0;
      m_pfComponents[3] = 1.0;
      return;
   }
  
   CMaterialColour::CMaterialColour(float fR, float fG, float fB, float fA) {
      CLog oLog("3d","CMaterialColour::Constructor (floats)",LL_OBJECT);
      m_pfComponents[0] = fR;
      m_pfComponents[1] = fG;
      m_pfComponents[2] = fB;
      m_pfComponents[3] = fA;
      return;
   }

   CMaterialColour::~CMaterialColour() {
      CLog oLog("3d","CMaterialColour::Destructor",LL_OBJECT);
   }

   float CMaterialColour::Red(void) const {
      CLog oLog("3d","CMaterialColour::Red (const)",LL_FUNCTION);
      return m_pfComponents[0];
   }
   
   float& CMaterialColour::Red(void) {
      CLog oLog("3d","CMaterialColour::Red",LL_FUNCTION);
      return m_pfComponents[0];
   }

   float CMaterialColour::Green(void) const {
      CLog oLog("3d","CMaterialColour::Green (const)",LL_FUNCTION);
      return m_pfComponents[1];
   }
   
   float& CMaterialColour::Green(void) {
      CLog oLog("3d","CMaterialColour::Green",LL_FUNCTION);
      return m_pfComponents[1];
   }

   float CMaterialColour::Blue(void) const {
      CLog oLog("3d","CMaterialColour::Blue (const)",LL_FUNCTION);
      return m_pfComponents[2];
   }

   float& CMaterialColour::Blue(void) {
      CLog oLog("3d","CMaterialColour::Blue",LL_FUNCTION);
      return m_pfComponents[2];
   }

   float CMaterialColour::Alpha(void) const {
      CLog oLog("3d","CMaterialColour::Alpha (const)",LL_FUNCTION);
      return m_pfComponents[3];
   }

   float& CMaterialColour::Alpha(void) {
      CLog oLog("3d","CMaterialColour::Alpha",LL_FUNCTION);
      return m_pfComponents[3];
   }

   const float* CMaterialColour::Array(void) const {
      CLog oLog("3d","CMaterialColour::Array",LL_FUNCTION);
      return m_pfComponents;
   }
        

}

