//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SlabElement.h
// 19/03/2002 - James Smith
//
// $Id: SlabElement.h,v 1.14 2002/03/27 17:08:15 vap-warren Exp $

#ifndef __VTSTRUCVIS_SLABELEMENT__
#define __VTSTRUCVIS_SLABELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CSlabElement : public CElement {
//#===--- Construction/Destruction
public:
   CSlabElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet);
   // Constructor

   ~CSlabElement();
   // Destructor

//#===--- Member Functions

   TElementType Type(void) const {return ELEMENT_SLAB;}
   // Return the element type.

   unsigned int Node(int iIndex) {return m_piNodes[iIndex];}
   // Get the control node with the passed index

   bool Display(const char* pcURL) const;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.
   // Parameter is a URL for the auxiliary VRML files

   bool SetVisible(bool bVisible) const;
   // Set whether the beam is visible or not.
   // Returns true if successful

   void SetNodes(const unsigned int* piNodes);
   // Sets which nodes the slab is controlled by
   // This function expects a pointer to a 9-uint array

   void SetCracks(unsigned int iLayer, const unsigned char* pcCracks) const;
   // Sets crack information for the specified layer.
   // Layer parameter isn't used at the moment.

   void SetStresses(const float* pfStresses) const;
   // Sets stresses for individual nodes
   // This function expects a pointer to a 9-float array

   void SetTemp(float fTemp) const;
   // Sets the temperature of the element

   void SetSize(float fThickness);
   // Sets the sizes of the slab

private:

   void CalculateColours(float* pfColours) const;
   // Calculates a set of colours for the element.
   // Expects an array of 27 floats in pfColours to return data in.

   void CalculateNodePositions(float* pfNodes) const;
   // Calculates node positions for the element.
   // Expects an array of 27 floats in pfNodes to return data in.

//#===--- Member Variables
protected:

   unsigned int m_piNodes[9];
   // The node indices that define the slab

   mutable float m_pfStresses[9];
   // Per-node stress values
   
   mutable unsigned char m_pcCracks[9];
   // Crack values

   float m_fThickness;
   // Slab dimensions

   mutable CCortonaField *m_ppoField[4];
   // Field value caches
   // Positions, cracks, colours, visible
};

#endif // __VTSTRUCVIS_SLABELEMENT__
