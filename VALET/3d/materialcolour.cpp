//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3d/materialcolour.cpp"
//! author 		= "James Smith"
//! date 		= "09/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: materialcolour.cpp,v 1.3 2001/10/10 09:02:32 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Materials"

#include "materialcolour.h"

// VALET includes

// Standard includes

namespace NValet {

  CMaterialColour::CMaterialColour() {
    m_pComponents[0] = 1.0;
    m_pComponents[1] = 1.0;
    m_pComponents[2] = 1.0;
    m_pComponents[3] = 1.0;
    return;
  }
  
  CMaterialColour::CMaterialColour(double dR, double dG, double dB, double dA) {
    m_pComponents[0] = dR;
    m_pComponents[1] = dG;
    m_pComponents[2] = dB;
    m_pComponents[3] = dA;
    return;
  }
  
}

