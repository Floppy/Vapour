//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.h
// 19/03/2002 - James Smith
//
// $Id: SlabElement.h,v 1.8 2002/03/21 23:35:30 vap-james Exp $

#ifndef __SLAB_ELEMENT__
#define __SLAB_ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CSlabElement : public CElement {
//#===--- Construction/Destruction
public:
   CSlabElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet);
   // Constructor

//#===--- Member Functions

   TElementType Type(void) const {return SLAB;}
   // Return the element type.

   unsigned int Node(int iIndex) {return m_piNodes[iIndex];}
   // Get the control node with the passed index

   bool Display(void) const;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.

   bool SetVisible(bool bVisible) const;
   // Set whether the beam is visible or not.
   // Returns true if successful

   void SetNodes(const unsigned int* piNodes);
   // Sets which nodes the slab is controlled by
   // This function expects a pointer to a 9-uint array

   void SetStresses(const float* pfStresses) const;
   // Sets stresses for individual nodes
   // This function expects a pointer to a 9-float array

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
   
   float m_fThickness;
   // Slab dimensions
};

#endif // __SLAB_ELEMENT__
