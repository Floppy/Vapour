//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/Element.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Element.h,v 1.16 2002/04/03 11:14:30 vap-james Exp $"

#ifndef __VTSTRUCVIS_ELEMENT__
#define __VTSTRUCVIS_ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"
#include "NodeSet.h"

enum TElementType {
   ELEMENT_NONE = 0xFF,
   ELEMENT_BEAM = 0x00,
   ELEMENT_SLAB = 0x01
};

enum TColourScheme {
   COLOUR_GROUP,
   COLOUR_STRESS
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
      m_oColourScheme(COLOUR_GROUP)
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

   virtual bool Display(const char* pcURL) const = 0;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.
   // Parameter is a URL for the auxiliary VRML files

   virtual void SetNodes(const unsigned int* piNodes) = 0;
   // Sets which nodes govern this element
   // This will be overridden and each child will 
   // expect a certain number of floats in the array.

   void SetStressRange(float fMin, float fMax) const {
      m_fMinStress = fMin;
      m_fMaxStress = fMax;
      return;
   }
   // Sets the range of possible stress values.
   // This will affect colouring of objects
   
   virtual void SetStresses(const float* pfStresses) const = 0;
   // Sets the stress values for the nodes
   // This will be overridden and each child will 
   // expect a certain number of floats in the array.

   virtual void SetTemp(float fTemp) const = 0;
   // Sets the temperature of the element

   unsigned int ID(void) {return m_iElement;}
   // Get the element's ID number

   unsigned int Group(void) {return m_iGroup;}
   // Get the element's group number

   virtual unsigned int Node(int iIndex) = 0;
   // Get the control node with the passed index

   void SetID(unsigned int iID) {m_iElement = iID;}
   // Set element ID number.

   void SetGroup(unsigned int iGroup) {m_iGroup = iGroup;}
   // Set group membership.

   void SetColour(const float* pfColour) const {
      memcpy(m_pfColour,pfColour,3*sizeof(float));
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

   mutable float m_fTemp;
   // Current temperature

   mutable float m_pfColour[3];
   // A manually set colour for the element

   mutable TColourScheme m_oColourScheme;
   // Which colouring scheme to use

//#===--- Member Variables
   friend class CViewpoint;

};

#endif // __VTSTRUCVIS_ELEMENT__
