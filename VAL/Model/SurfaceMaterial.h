//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SurfaceMaterial.h - 17/11/2000 - James Smith
//	Surface material class declaration
//
// $Id: SurfaceMaterial.h,v 1.1 2000/11/30 09:37:41 warren Exp $
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

///////////////////////////////////////////////////////////////////////
// Structures /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Colour components go from 0..1
struct SColour {
double m_dRed;
double m_dGreen;
double m_dBlue;
double m_dAlpha;

// Default 
SColour() :
	m_dRed(1.0),
	m_dGreen(1.0),
	m_dBlue(1.0),
	m_dAlpha(1.0) {
}

// RGB Constructor
SColour(double dRed, double dGreen, double dBlue) :
	m_dRed(dRed),
	m_dGreen(dGreen),
	m_dBlue(dBlue),
	m_dAlpha(1.0) {
}

// RGBA Constructor
SColour(double dRed, double dGreen, double dBlue, double dAlpha) :
	m_dRed(dRed),
	m_dGreen(dGreen),
	m_dBlue(dBlue),
	m_dAlpha(dAlpha) {
}

};

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
   CSurfaceMaterial(CImage* pTexture);

   // Destructor
   virtual ~CSurfaceMaterial();

   ///////////////////////////////////////////////////////////////////////
   // Assignment Functions ///////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Assigns the diffuse colour of the material
   void SetDiffuse(const SColour& sDiffuse) { m_sDiffuse = sDiffuse; }

   // Assigns the specular colour of the material
   void SetSpecular(const SColour& sSpecular) { m_sSpecular = sSpecular; }

   // Assigns the emissive colour of the material
   void SetEmissive(const SColour& sEmissive) { m_sEmissive = sEmissive;}

   // Assigns the shininess of the material
   void SetShininess(double dShininess) { m_dShininess = dShininess; }

   // Assigns the texture for the material - any old texture is automatically deleted
   void SetTexture(CImage* pTexture);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Returns the diffuse colour of the material
   SColour Diffuse(void) const {return m_sDiffuse; }

   // Returns the specular colour of the material
   SColour Specular(void) const {return m_sSpecular; }

   // Returns the emissive colour of the material
   SColour Emissive(void) const {return m_sEmissive; }

   // Returns the shininess of the material
   double Shininess(void) const {return m_dShininess; }

   // Returns the texture for the material
   CImage* Texture(void) const {return m_pTexture;}

protected:
	
   ///////////////////////////////////////////////////////////////////////
   // Data ///////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // The diffuse colour of the material
   SColour m_sDiffuse;

   // The specular colour of the material
   SColour m_sSpecular;

   // The emissive colour of the material
   SColour m_sEmissive;

   // The shininess of the material
   double m_dShininess;

   // A texture map to be applied to the surface
   CImage* m_pTexture;

};

#endif //_VAL_MATERIAL_
