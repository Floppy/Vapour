//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// BeamElement.h
// 19/03/2002 - James Smith
//
// $Id: BeamElement.h,v 1.11 2002/03/25 14:55:36 vap-james Exp $

#ifndef __VTSTRUCVIS_BEAMELEMENT__
#define __VTSTRUCVIS_BEAMELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CBeamElement : public CElement {
//#===--- Construction/Destruction
public:
   CBeamElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet);
   // Constructor

//#===--- Member Functions

   TElementType Type(void) const {return BEAM;}
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

   void SetTemp(float fTemp) const;
   // Sets the temperature of the element

   void SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness);
   // Sets the sizes of the I-beam

   void SetNodes(const unsigned int* piNodes);
   // Sets which nodes the beam is controlled by
   // This function expects a pointer to a 9-uint array

   void SetStresses(const float* pfStresses) const;
   // Sets stresses for individual nodes
   // This function expects a pointer to a 9-float array

private:

   void CalculateColours(float* pfColours) const;
   // Calculates a set of colours for the element.
   // Expects an array of 6 floats in pfColours to return data in.

   void CalculateNodePositions(float* pfNodes) const;
   // Calculates node positions for the element.
   // Expects an array of 6 floats in pfNodes to return data in.

   //#===--- Member Variables
protected:

   unsigned int m_piNodes[2];
   // The node indices that define the beam.

   mutable float m_pfStresses[2];
   // Per-node stress values
   
   float m_fHeight;
   float m_fWidth;
   float m_fFlange;
   float m_fWeb;
   // Beam dimensions

};

#endif // __VTSTRUCVIS_BEAMELEMENT__
