//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.h - 17/11/2000 - James Smith
//	Surface material class declaration
//
// $Id: SurfaceMaterial.h,v 1.0 2000/11/21 16:47:16 waz Exp $
//

#ifndef _VAL_SURFACEMATERIAL_
#define _VAL_SURFACEMATERIAL_

#pragma once

// VAL includes
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

class DLL CSurfaceMaterial {

public:

   ///////////////////////////////////////////////////////////////////////
   // Structures /////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Colour components go from 0..1
   struct Colour {
      double m_dRed;
      double m_dGreen;
      double m_dBlue;
      double m_dAlpha;
   };

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates a surface material
   CSurfaceMaterial();
   
   // Creates a surface material with a particular texture
   CSurfaceMaterial(CImage* pTexture);

   // Destructor
   virtual ~CSurfaceMaterial();

   ///////////////////////////////////////////////////////////////////////
   // Assignment Functions ///////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Assigns the diffuse colour of the material
   void SetDiffuse(const Colour& colDiffuse) {m_colDiffuse = colDiffuse;}

   // Assigns the specular colour of the material
   void SetSpecular(const Colour& colSpecular) {m_colSpecular = colSpecular;}

   // Assigns the emissive colour of the material
   void SetEmissive(const Colour& colEmissive) {m_colEmissive = colEmissive;}

   // Assigns the shininess of the material
   void SetShininess(double dShininess) {m_dShininess = dShininess;}

   // Assigns the texture for the material - any old texture is automatically deleted
   void SetTexture(CImage* pTexture);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Returns the diffuse colour of the material
   Colour Diffuse(void) const {return m_colDiffuse;}

   // Returns the specular colour of the material
   Colour Specular(void) const {return m_colSpecular;}

   // Returns the emissive colour of the material
   Colour Emissive(void) const {return m_colEmissive;}

   // Returns the shininess of the material
   double Shininess(void) const {return m_dShininess;}

   // Returns the texture for the material
   CImage* Texture(void) const {return m_pTexture;}

protected:
	
   ///////////////////////////////////////////////////////////////////////
   // Data ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // The diffuse colour of the material
   Colour m_colDiffuse;

   // The specular colour of the material
   Colour m_colSpecular;

   // The emissive colour of the material
   Colour m_colEmissive;

   // The shininess of the material
   double m_dShininess;

   // A texture map to be applied to the surface
   CImage* m_pTexture;

};

#endif //_VAL_MATERIAL_
