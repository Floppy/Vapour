//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_MATERIALCOLOUR_
#define _VALET_3D_MATERIALCOLOUR_

////////////////
//! file       = "VALET/3d/materialcolour.h"
//! author     = "James Smith"
//! date       = "09/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: materialcolour.h,v 1.8 2001/10/24 22:23:54 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Materials"

//#===--- Includes
#include "VALET/valet.h"

namespace NVALET {

  //: Surface Material Colour
  // A class representing a surface material colour.

  class CMaterialColour {

  protected:
    
    double m_pdComponents[4];
    //: The colour itself, in the order RGBA
    // The values are in the range 0..1

  public:    
    
    //:-------------------------
    //: Construction/Destruction

    CMaterialColour();
    //: Default Constructor
    // Creates a white colour
    
    CMaterialColour(double dRed, double dGreen, double dBlue, double dAlpha=1.0);
    //: Constructor
    // Creates a material colour of the appropriate colour 
    //!param: dRed = the red component.
    //!param: dGreen = the green component.
    //!param: dBlue = the blue component.
    //!param: dAlpha = the alpha component.
   
    ~CMaterialColour();
    //: Destructor
    
    //:-----------------
    //: Access functions

    double Red(void) const;
    //: Access the red component.
    //!param: return = the red component.

    double& Red(void);
    //: Non-const access to the red component.
    // This can be used to modify the red component.
    //!param: return = a reference to the red component.

    double Green(void) const;
    //: Access the green component.
    //!param: return = the green component.

    double& Green(void);
    //: Non-const access to the green component.
    // This can be used to modify the green component.
    //!param: return = a reference to the green component.

    double Blue(void) const;
    //: Access the blue component.
    //!param: return = the blue component.

    double& Blue(void);
    //: Non-const access to the blue component.
    // This can be used to modify the blue component.
    //!param: return = a reference to the blue component.

    double Alpha(void) const;
    //: Access the alpha component.
    //!param: return = the alpha component.

    double& Alpha(void);
    //: Non-const access to the alpha component.
    // This can be used to modify the alpha component.
    //!param: return = a reference to the alpha component.

    const double* Array(void) const;
    //: Access the complete array.
    // Suitable for use with glColor4dv(), amongst others.
    //!param: return = an array of 4 double values, in the order RGBA.
        
  }; 

}

#endif //_VALET_3D_MATERIALCOLOUR_
