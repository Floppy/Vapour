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
//! rcsid      = "$Id: materialcolour.cpp,v 1.7 2001/10/24 22:23:54 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Materials"

#include "VALET/materialcolour.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {

   CMaterialColour::CMaterialColour() {
      CLog oLog("3d","CMaterialColour::Constructor (default)",LL_OBJECT);
      m_pdComponents[0] = 1.0;
      m_pdComponents[1] = 1.0;
      m_pdComponents[2] = 1.0;
      m_pdComponents[3] = 1.0;
      return;
   }
  
   CMaterialColour::CMaterialColour(double dR, double dG, double dB, double dA) {
      CLog oLog("3d","CMaterialColour::Constructor (doubles)",LL_OBJECT);
      m_pdComponents[0] = dR;
      m_pdComponents[1] = dG;
      m_pdComponents[2] = dB;
      m_pdComponents[3] = dA;
      return;
   }

   CMaterialColour::~CMaterialColour() {
      CLog oLog("3d","CMaterialColour::Destructor",LL_OBJECT);
   }

   double CMaterialColour::Red(void) const {
      CLog oLog("3d","CMaterialColour::Red (const)",LL_FUNCTION);
      return m_pdComponents[0];
   }
   
   double& CMaterialColour::Red(void) {
      CLog oLog("3d","CMaterialColour::Red",LL_FUNCTION);
      return m_pdComponents[0];
   }

   double CMaterialColour::Green(void) const {
      CLog oLog("3d","CMaterialColour::Green (const)",LL_FUNCTION);
      return m_pdComponents[1];
   }
   
   double& CMaterialColour::Green(void) {
      CLog oLog("3d","CMaterialColour::Green",LL_FUNCTION);
      return m_pdComponents[1];
   }

   double CMaterialColour::Blue(void) const {
      CLog oLog("3d","CMaterialColour::Blue (const)",LL_FUNCTION);
      return m_pdComponents[2];
   }

   double& CMaterialColour::Blue(void) {
      CLog oLog("3d","CMaterialColour::Blue",LL_FUNCTION);
      return m_pdComponents[2];
   }

   double CMaterialColour::Alpha(void) const {
      CLog oLog("3d","CMaterialColour::Alpha (const)",LL_FUNCTION);
      return m_pdComponents[3];
   }

   double& CMaterialColour::Alpha(void) {
      CLog oLog("3d","CMaterialColour::Alpha",LL_FUNCTION);
      return m_pdComponents[3];
   }

   const double* CMaterialColour::Array(void) const {
      CLog oLog("3d","CMaterialColour::Array",LL_FUNCTION);
      return m_pdComponents;
   }
        

}

