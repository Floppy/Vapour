//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// Element.h
// 19/03/2002 - James Smith
//
// $Id: Element.h,v 1.1 2002/03/20 02:06:24 vap-james Exp $

#ifndef __ELEMENT__
#define __ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"

enum TElementType {
   BEAM,
   SLAB
};

class CElement {
//#===--- Construction/Destruction
public:

   CElement(CCortonaUtil *pCortona) :
      m_pCortona(pCortona),
      m_pNodePtr(NULL)
   {
      // Default values for the stress range
      m_fMinStress = 0.0;
      m_fMaxStress = 1000000.0;
      // Default scale factor
      m_pfDisplacementScale[0] = m_pfDisplacementScale[1] = m_pfDisplacementScale[2] = 1.0;
   }
   // Constructor
   
   ~CElement() {
      if (m_pNodePtr) m_pNodePtr->Release();
   }
   // Destructor

//#===--- Member Functions

   virtual TElementType Type(void) const = 0;
   // Return the element type (beam or slab).

   virtual bool Display(void) const = 0;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.

   void SetStressRange(float fMin, float fMax) const {
      m_fMinStress = fMin;
      m_fMaxStress = fMax;
      return;
   }
   // Sets the range of possible stress values.
   // This will affect colouring of objects
   
   void SetDisplacementScale(float fX, float fY, float fZ) const {
      m_pfDisplacementScale[0] = fX;
      m_pfDisplacementScale[1] = fY;
      m_pfDisplacementScale[2] = fZ;
      return;
   }
   // Set displacement scale factor.

   void SetGroup(unsigned int iGroup) {m_iGroup = iGroup;}
   // Set group membership.

//#===--- Member Variables
protected:

   CCortonaUtil* m_pCortona;
   // Cortona Utility interface

   mutable INodeObject* m_pNodePtr;
   // Pointer to the node in the VRML world;

   unsigned int m_iGroup;
   // Which group the element belongs to

   mutable float m_fMinStress;
   mutable float m_fMaxStress;
   // Lower and upper limits on stress values.

   mutable float m_pfDisplacementScale[3];
   // Scale factors for node displacements.

};

#endif // __BEAM_ELEMENT__
