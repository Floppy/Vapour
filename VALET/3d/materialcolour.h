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
//! rcsid 		= "$Id: materialcolour.h,v 1.2 2001/10/09 23:22:33 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3d.Surface Materials"

//#===--- Includes
#include "../arch/valet.h"

namespace NValet {

  //: Surface Material Colour
  // A class representing a surface material colour.

  class CMaterialColour {

  protected:
    
    double m_pComponents[4];
    //: The colour itself, in the order RGBA
    // The values are in the range 0..1

  public:    
    
    //:------------------------
    //: Contruction/Destruction

    CMaterialColour();
    //: Default Constructor
    // Creates a white colour
    
    CMaterialColour(double dRed, double dGreen, double dBlue, double dAlpha=1.0);
    //: Constructor
    // Creates a material colour of the appropriate colour 
   
    ~CMaterialColour();
    //: Destructor
    
    //:---------------------
    //: Assignment functions

    void R(float dVal) {m_pComponents[0] = dVal;}
    //: Set the red component.

    void G(float dVal) {m_pComponents[1] = dVal;}
    //: Set the green component.

    void B(float dVal) {m_pComponents[2] = dVal;}
    //: Set the blue component.

    void A(float dVal) {m_pComponents[3] = dVal;}
    //: Set the alpha component.

    //:-----------------
    //: Access functions

    float R(void) const {return m_pComponents[0];}
    //: Access the red component.

    float G(void) const {return m_pComponents[1];}
    //: Access the green component.

    float B(void) const {return m_pComponents[2];}
    //: Access the blue component.

    float A(void) const {return m_pComponents[3];}
    //: Access the alpha component.

    const double* Array(void) const {return m_pComponents;}
    //: Access the complete array.
    // Suitable for use with glColor4dv(), amongst others.
        
  }; 

}

#endif //_VALET_MATERIALCOLOUR_
