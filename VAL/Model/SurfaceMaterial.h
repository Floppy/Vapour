//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.h - 17/11/2000 - James Smith
//	  Surface material class declaration
//
// $Id: SurfaceMaterial.h,v 1.2 2000/12/02 07:34:32 warren Exp $
//

#ifndef _VAL_SURFACEMATERIAL_
#define _VAL_SURFACEMATERIAL_

#pragma once

// VAL includes
#include "Colour.h"
#include "Image.h"

// DLL import/export definitions
#ifndef DLL
	#ifdef VAL_DLL_EXPORT
		#define DLL __declspec(dllexport)
	#endif
	#ifdef VAL_DLL_IMPORT
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

/////////////////////
// CSurfaceMaterial

class DLL CSurfaceMaterial {

public:

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates a surface material
   CSurfaceMaterial();
   
   // Creates a surface material with a particular texture
   CSurfaceMaterial(CImage *poTexture);

   // Destructor
   virtual ~CSurfaceMaterial();

   ///////////////////////////////////////////////////////////////////////
   // Assignment Functions ///////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Assigns the diffuse colour of the material
   void SetAmbient(const CColour &oColour) { m_oAmbient = oColour; }

   // Assigns the diffuse colour of the material
   void SetDiffuse(const CColour &oColour) { m_oDiffuse = oColour; }

   // Assigns the specular colour of the material
   void SetSpecular(const CColour &oColour) { m_oSpecular = oColour; }

   // Assigns the emissive colour of the material
   void SetEmissive(const CColour &oColour) { m_oEmissive = oColour; }

   // Assigns the shininess of the material
   void SetShininess(float fShininess) { m_fShininess = fShininess; }

   // Assigns the texture for the material - any old texture is automatically deleted
   void SetTexture(CImage* pTexture);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Returns the diffuse colour of the material
   CColour Ambient(void) const { return m_oAmbient; }

   // Returns the diffuse colour of the material
   CColour Diffuse(void) const { return m_oDiffuse; }

   // Returns the specular colour of the material
   CColour Specular(void) const { return m_oSpecular; }

   // Returns the emissive colour of the material
   CColour Emissive(void) const { return m_oEmissive; }

   // Returns the shininess of the material
   float Shininess(void) const {return m_fShininess; }

   // Returns the texture for the material
   CImage *Texture(void) const {return m_poTexture;}

protected:
	
   ///////////////////////////////////////////////////////////////////////
   // Data ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // The ambient colour of the material
   CColour m_oAmbient;

   // The diffuse colour of the material
   CColour m_oDiffuse;

   // The specular colour of the material
   CColour m_oSpecular;

   // The emissive colour of the material
   CColour m_oEmissive;

   // The shininess of the material
   float m_fShininess;

   // A texture map to be applied to the surface
   CImage *m_poTexture;

};

#endif //_VAL_SURFACEMATERIAL_
