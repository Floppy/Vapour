//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// BeamElement.h
// 19/03/2002 - James Smith
//
// $Id: BeamElement.h,v 1.5 2002/03/21 14:32:07 vap-warren Exp $

#ifndef __BEAM_ELEMENT__
#define __BEAM_ELEMENT__

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

   bool Display(void) const;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.

   void SetVisible(bool bVisible) const;
   // Set whether the beam is visible or not.

   void SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness);
   // Sets the sizes of the I-beam

   void SetNodes(int iFirstNode, int iSecondNode);
   // Sets which nodes the beam is between

   void SetStresses(float fFirstNode, float fSecondNode);
   // Sets stresses for individual nodes

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

   float m_pfStresses[2];
   // Per-node stress values
   
   float m_fHeight;
   float m_fWidth;
   float m_fFlange;
   float m_fWeb;
   // Beam dimensions

};

#endif // __BEAM_ELEMENT__
