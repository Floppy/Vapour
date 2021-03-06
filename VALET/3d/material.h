//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_MATERIAL_
#define _VALET_3D_MATERIAL_

////////////////
//! file       = "VALET/3d/material.h"
//! author     = "James Smith"
//! date       = "09/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: material.h,v 1.8 2001/10/24 22:23:54 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Materials"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/materialcolour.h"

namespace NVALET {

  //: Surface Material
  // A class containing a description of a surface material for an object.
  // Including various lighting colours. It will also include a texture map in future.
  //!todo: Add texture map.

  class CMaterial {

  protected:
    
    CMaterialColour m_oAmbient;
    //: The ambient colour of the material
    
    CMaterialColour m_oDiffuse;
    //: The diffuse colour of the material
    
    CMaterialColour m_oSpecular;
    //: The specular colour of the material
    
    CMaterialColour m_oEmissive;
    //: The emissive colour of the material
    
    double m_dShininess;
    //: The shininess of the material
    
  public:    
    
    //:-------------------------
    //: Construction/Destruction

    CMaterial();
    //: Default Constructor
    // Creates a default surface material
    
    ~CMaterial();
    //: Destructor
    
    //:-----------------
    //: Access functions
    
    CMaterialColour Ambient(void) const;
    //: The ambient colour.
    // The colour under ambient lighting.
    //!param: return = the ambient colour.
    
    CMaterialColour& Ambient(void);
    //: Non-const access to the diffuse colour.
    // The colour under ambient lighting.
    // This can be used to modify the ambient colour.
    //!param: return = a reference to the ambient colour.
    
    CMaterialColour Diffuse(void) const;
    //: The diffuse colour.
    // The colour under normal diffuse lighting.
    //!param: return = the ambient colour.

    CMaterialColour& Diffuse(void);
    //: Non-const access to the diffuse colour.
    // The colour under normal diffuse lighting.
    // This can be used to modify the diffuse colour.
    //!param: return = a reference to the diffuse colour.
    
    CMaterialColour Specular(void) const;
    //: The specular colour.
    // This is the colour of any highlights on the surface.
    //!param: return = the specular colour.

    CMaterialColour& Specular(void);
    //: Non-const access to the specular colour.
    // This is the colour of any highlights on the surface.
    // This can be used to modify the specular colour.
    //!param: return = a reference to the specular colour.
    
    CMaterialColour Emissive(void) const;
    //: The emissive colour.
    // The object glows with a light of this colour.
    //!param: return = the emissive colour.

    CMaterialColour& Emissive(void);
    //: Non-const access to the emissive colour.
    // The object glows with a light of this colour.
    // This can be used to modify the emissive colour.
    //!param: return = a reference to the emissive colour.
    
    double Shininess(void) const;
    //: Returns the shininess of the material
    //!param: return = the shininess.

    double& Shininess(void);
    //: Non-const access to the shininess of the material.
    // This can be used to modify the shininess.
    //!param: return = a reference to the shininess.
    
  }; 

}

#endif //_VALET_3D_MATERIAL_
