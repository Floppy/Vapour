//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// SlabElement.h
// 19/03/2002 - James Smith
//
// $Id: SlabElement.h,v 1.2 2002/03/20 13:29:51 vap-james Exp $

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

   void SetNodes(int iFirstNode, int iSecondNode, int iThirdNode, 
                 int iFourthNode, int iFifthNode, int iSixthNode, 
                 int iSeventhNode, int iEighthNode, int iNinthNode);
   // Sets which nodes define the slab

//#===--- Member Variables
protected:

   unsigned int m_piNodes[9];
   // The node indices that define the slab

};

#endif // __SLAB_ELEMENT__
