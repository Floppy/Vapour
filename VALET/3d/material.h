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
//! rcsid 		= "$Id: material.h,v 1.2 2001/10/09 23:22:33 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3d.Surface Materials"

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
    
    float m_fShininess;
    //: The shininess of the material
    
  public:    
    
    CMaterial();
    //: Default Constructor
    // Creates a default surface material
    
    ~CMaterial();
    //: Destructor
    
    ///////////////////////////////////////////////////////////////////////
    // Assignment Functions ///////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    void Ambient(const CMaterialColour &oColour) { m_cAmbient = oColour; }
    //: Assigns the ambient colour of the material
    
    void Diffuse(const CMaterialColour &oColour) { m_cDiffuse = oColour; }
    //: Assigns the diffuse colour of the material
    
    void Specular(const CMaterialColour &oColour) { m_cSpecular = oColour; }
    //: Assigns the specular colour of the material
    
    void Emissive(const CMaterialColour &oColour) { m_cEmissive = oColour; }
    //: Assigns the emissive colour of the material
    
    void Shininess(float fShininess) { m_fShininess = fShininess; }
    //: Assigns the shininess of the material
    
    ///////////////////////////////////////////////////////////////////////
    // Access Functions ///////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    
    CMaterialColour Ambient(void) const { return m_cAmbient; }
    //: Returns the diffuse colour of the material
    
    CMaterialColour Diffuse(void) const { return m_cDiffuse; }
    //: Returns the diffuse colour of the material
    
    CMaterialColour Specular(void) const { return m_cSpecular; }
    //: Returns the specular colour of the material
    
    CMaterialColour Emissive(void) const { return m_cEmissive; }
    //: Returns the emissive colour of the material
    
    float Shininess(void) const {return m_fShininess; }
    //: Returns the shininess of the material
    
  }; 

}

#endif //_VALET_MATERIAL_
