//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATERIALCOLOUR_
#define _VALET_MATERIALCOLOUR_

////////////////
//! file 		= "VALET/3d/materialcolour.h"
//! author 		= "James Smith"
//! date 		= "09/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: materialcolour.h,v 1.5 2001/10/18 12:40:17 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Materials"

//#===--- Includes
#include "VALET/valet.h"

namespace NValet {

  //: Surface Material Colour
  // A class representing a surface material colour.

  class CMaterialColour {

  protected:
    
    double m_pComponents[4];
    //: The colour itself, in the order RGBA
    // The values are in the range 0..1

  public:    
    
    //:-------------------------
    //: Construction/Destruction

    CMaterialColour();
    //: Default Constructor
    // Creates a white colour
    
    CMaterialColour(double dR, double dG, double dB, double dA=1.0);
    //: Constructor
    // Creates a material colour of the appropriate colour 
    //!param: dR = the red component.
    //!param: dG = the green component.
    //!param: dB = the blue component.
    //!param: dA = the alpha component.
   
    ~CMaterialColour();
    //: Destructor
    
    //:-----------------
    //: Access functions

    double R(void) const {return m_pComponents[0];}
    //: Access the red component.
    //!param: return = the red component.

    double& R(void) {return m_pComponents[0];}
    //: Non-const access to the red component.
    // This can be used to modify the red component.
    //!param: return = a reference to the red component.

    double G(void) const {return m_pComponents[1];}
    //: Access the green component.
    //!param: return = the green component.

    double& G(void) {return m_pComponents[1];}
    //: Non-const access to the green component.
    // This can be used to modify the green component.
    //!param: return = a reference to the green component.

    double B(void) const {return m_pComponents[2];}
    //: Access the blue component.
    //!param: return = the blue component.

    double& B(void) {return m_pComponents[2];}
    //: Non-const access to the blue component.
    // This can be used to modify the blue component.
    //!param: return = a reference to the blue component.

    double A(void) const {return m_pComponents[3];}
    //: Access the alpha component.
    //!param: return = the alpha component.

    double& A(void) {return m_pComponents[3];}
    //: Non-const access to the alpha component.
    // This can be used to modify the alpha component.
    //!param: return = a reference to the alpha component.

    const double* Array(void) const {return m_pComponents;}
    //: Access the complete array.
    // Suitable for use with glColor4dv(), amongst others.
    //!param: return = an array of 4 double values, in the order RGBA.
        
  }; 

}

#endif //_VALET_MATERIALCOLOUR_
