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
//! rcsid 		= "$Id: materialcolour.cpp,v 1.1 2001/10/09 23:19:55 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3d"

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
  
  CMaterialColour::CMaterialColour(double dRed, double dGreen, double dBlue, double dAlpha) {
    m_pComponents[0] = dRed;
    m_pComponents[1] = dGreen;
    m_pComponents[2] = dBlue;
    m_pComponents[3] = dAlpha;
    return;
  }
  
}

