//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// Element.h
// 19/03/2002 - James Smith
//
// $Id: Element.h,v 1.7 2002/03/21 14:49:07 vap-james Exp $

#ifndef __ELEMENT__
#define __ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"
#include "NodeSet.h"

enum TElementType {
   BEAM,
   SLAB
};

enum TColourScheme {
   SOLID,
   STRESS
};

class CElement {
//#===--- Construction/Destruction
public:

   CElement(CCortonaUtil *pCortona, CNodeSet* pNodeSet) :
      m_poCortona(pCortona),
      m_poNodeSet(pNodeSet),
      m_poNodePtr(NULL),
      m_iElement(0),
      m_iGroup(0),
      m_fMinStress(0.0f),
      m_fMaxStress(1000000.0f),
      m_oColourScheme(SOLID)
   {
         m_pfColour[0] = m_pfColour[1] = m_pfColour[2] = 0.5;
   }
   // Constructor

   
   virtual ~CElement() {
      if (m_poNodePtr) {
         m_poNodePtr->Release();
         delete m_poNodePtr;
      }
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
   
   unsigned int ID(void) {return m_iElement;}
   // Get the element's ID number

   unsigned int Group(void) {return m_iGroup;}
   // Get the element's group number

   void SetID(unsigned int iID) {m_iElement = iID;}
   // Set element ID number.

   void SetGroup(unsigned int iGroup) {m_iGroup = iGroup;}
   // Set group membership.

   void SetColour(float fRed, float fGreen, float fBlue) const {
      m_pfColour[0] = fRed;
      m_pfColour[1] = fGreen;
      m_pfColour[2] = fBlue;
   }
   // Manually set a solid colour for the element

   void SetColourScheme(TColourScheme oScheme) const {
      m_oColourScheme = oScheme;
   }
   // Set the colour scheme to use

   virtual bool SetVisible(bool bVisible) const = 0;
   // Set whether the beam is visible or not.
   // Returns true if successful

//#===--- Member Variables
protected:

   CCortonaUtil* m_poCortona;
   // Cortona Utility interface

   CNodeSet* m_poNodeSet;
   // Node Set

   mutable CCortonaNode *m_poNodePtr;
   // Pointer to the node in the VRML world;

   unsigned int m_iElement;
   // A unique element ID.
   unsigned int m_iGroup;
   // Which group the element belongs to   

   mutable float m_fMinStress;
   mutable float m_fMaxStress;
   // Lower and upper limits on stress values.

   mutable float m_pfColour[3];
   // A manually set colour for the element

   mutable TColourScheme m_oColourScheme;
   // Which colouring scheme to use

};

#endif // __ELEMENT__
