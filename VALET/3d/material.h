//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATERIAL_
#define _VALET_MATERIAL_

////////////////
//! file 		= "VALET/3d/material.h"
//! author 		= "James Smith"
//! date 		= "09/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: material.h,v 1.3 2001/10/10 09:02:31 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Materials"

//#===--- Includes
#include "../arch/valet.h"
#include "materialcolour.h"

namespace NValet {

  //: Surface Material
  // A class containing a description of a surface material for an object.
  // Including various lighting colours. It will also include a texture map in future.
  //!todo: Add texture map.

  class CMaterial {

  protected:
    
    CMaterialColour m_cAmbient;
    //: The ambient colour of the material
    
    CMaterialColour m_cDiffuse;
    //: The diffuse colour of the material
    
    CMaterialColour m_cSpecular;
    //: The specular colour of the material
    
    CMaterialColour m_cEmissive;
    //: The emissive colour of the material
    
    double m_dShininess;
    //: The shininess of the material
    
  public:    
    
    //:------------------------
    //: Contruction/Destruction

    CMaterial();
    //: Default Constructor
    // Creates a default surface material
    
    ~CMaterial();
    //: Destructor
    
    //:-----------------
    //: Access functions
    
    CMaterialColour Ambient(void) const { return m_cAmbient; }
    //: The ambient colour.
    // The colour under ambient lighting.
    //!param: return = the ambient colour.
    
    CMaterialColour& Ambient(void) { return m_cAmbient; }
    //: Non-const access to the diffuse colour.
    // The colour under ambient lighting.
    // This can be used to modify the ambient colour.
    //!param: return = a reference to the ambient colour.
    
    CMaterialColour Diffuse(void) const { return m_cDiffuse; }
    //: The diffuse colour.
    // The colour under normal diffuse lighting.
    //!param: return = the ambient colour.

    CMaterialColour& Diffuse(void) { return m_cDiffuse; }
    //: Non-const access to the diffuse colour.
    // The colour under normal diffuse lighting.
    // This can be used to modify the diffuse colour.
    //!param: return = a reference to the diffuse colour.
    
    CMaterialColour Specular(void) const { return m_cSpecular; }
    //: The specular colour.
    // This is the colour of any highlights on the surface.
    //!param: return = the specular colour.

    CMaterialColour& Specular(void) { return m_cSpecular; }
    //: Non-const access to the specular colour.
    // This is the colour of any highlights on the surface.
    // This can be used to modify the specular colour.
    //!param: return = a reference to the specular colour.
    
    CMaterialColour Emissive(void) const { return m_cEmissive; }
    //: The emissive colour.
    // The object glows with a light of this colour.
    //!param: return = the emissive colour.

    CMaterialColour& Emissive(void) { return m_cEmissive; }
    //: Non-const access to the emissive colour.
    // The object glows with a light of this colour.
    // This can be used to modify the emissive colour.
    //!param: return = a reference to the emissive colour.
    
    double Shininess(void) const {return m_dShininess; }
    //: Returns the shininess of the material
    //!param: return = the shininess.

    double& Shininess(void) {return m_dShininess; }
    //: Non-const access to the shininess of the material.
    // This can be used to modify the shininess.
    //!param: return = a reference to the shininess.
    
  }; 

}

#endif //_VALET_MATERIAL_
