//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.h
// 19/03/2002 - James Smith
//
// $Id: SlabElement.h,v 1.4 2002/03/20 22:13:25 vap-james Exp $

#ifndef __SLAB_ELEMENT__
#define __SLAB_ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CSlabElement : public CElement {
//#===--- Construction/Destruction
public:
   CSlabElement(CCortonaUtil *pCortona, CNodeSet* pNodeSet);
   // Constructor

//#===--- Member Functions

   TElementType Type(void) const {return SLAB;}
   // Return the element type.

   bool Display(void) const;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.

   void SetVisible(bool bVisible) const;
   // Set whether the beam is visible or not.

   void SetNodes(int iFirstNode, int iSecondNode, int iThirdNode, 
                 int iFourthNode, int iFifthNode, int iSixthNode, 
                 int iSeventhNode, int iEighthNode, int iNinthNode);
   // Sets which nodes define the slab

   void SetStresses(float fFirstNode, float fSecondNode, float fThirdNode, 
                    float fFourthNode, float fFifthNode, float fSixthNode, 
                    float fSeventhNode, float fEighthNode, float fNinthNode);
   // Sets stresses for individual nodes

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

   float m_pfStresses[9];
   // Per-node stress values
   
   float m_fThickness;
   // Slab dimensions
};

#endif // __SLAB_ELEMENT__
