//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/Element.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Element.h,v 1.19 2002/04/04 11:01:35 vap-warren Exp $"

#ifndef __VTSTRUCVIS_ELEMENT__
#define __VTSTRUCVIS_ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"
#include "NodeSet.h"

//: Element types
enum TElementType {
   ELEMENT_NONE = 0xFF,
   ELEMENT_BEAM = 0x00,
   ELEMENT_SLAB = 0x01
};

//: Colouring schemes
enum TColourScheme {
   COLOUR_GROUP,
   COLOUR_STRESS
};

//: An interface to an element in a VRML scene
// This class provides an base class for other specialised elements.

class CElement {

public:

   //:-------------------------
   //: Construction/Destruction

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
   //: Constructor
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.
   //!param: poNodeSet - A pointer to a set of node positions.
   
   virtual ~CElement() {
      if (m_poNodePtr) {
         m_poNodePtr->Release();
         delete m_poNodePtr;
      }
   }
   //: Destructor

   //:-----------------
   //: Member Functions

   virtual TElementType Type(void) const = 0;
   //: The element type

   unsigned int ID(void) {return m_iElement;}
   //: Get the element's ID number

   unsigned int Group(void) {return m_iGroup;}
   //: Get the element's group number

   virtual unsigned int Node(int iIndex) = 0;
   //: Control node index.
   //!param: iIndex - The node to return.

   virtual bool Display(const char* pcURL) const = 0;
   //: Updates all information and displays the object
   // If the object is not already displayed, it is created.
   //!param: pcURL - a URL for the directory containing auxiliary VRML files.
   //!param: return - true if successful, false otherwise.

   void SetStressRange(float fMin, float fMax) const {
      m_fMinStress = fMin;
      m_fMaxStress = fMax;
      return;
   }
   //: Sets the range of possible stress values.
   //!param: fMin - the minimum stress value
   //!param: fMax - the maximum stress value
   
   virtual bool SetVisible(bool bVisible) const = 0;
   //: Set whether the element is visible or not.
   //!param: return - true if successful, false otherwise.

   virtual void SetTemp(float fTemp) const = 0;
   //: Set the temperature of the element

   virtual void SetNodes(const unsigned int* piNodes) = 0;
   //: Set which nodes the element is controlled by
   //!param: piNodes - a pointer to an array of unsigned integers.

   virtual void SetStresses(const float* pfStresses) const = 0;
   //: Set stresses for individual nodes
   //!param: pfStresses - a pointer to an array of floats.

   void SetID(unsigned int iID) {m_iElement = iID;}
   //: Set element's ID number.

   void SetGroup(unsigned int iGroup) {m_iGroup = iGroup;}
   //: Set group membership.

   void SetColour(const float* pfColour) const {
      memcpy(m_pfColour,pfColour,3*sizeof(float));
   }
   //: Manually set a solid colour for the element.
   // This colour is used for the COLOUR_GROUP colouring scheme

   void SetColourScheme(TColourScheme oScheme) const {
      m_oColourScheme = oScheme;
   }
   //: Set which colour scheme to use.

protected:

   //:-----------------
   //: Member Variables

   CCortonaUtil* m_poCortona;
   //: Cortona Utility interface

   CNodeSet* m_poNodeSet;
   //: Node Set

   mutable CCortonaNode *m_poNodePtr;
   //: Pointer to the node in the VRML world;

   unsigned int m_iElement;
   //: A unique element ID.

   unsigned int m_iGroup;
   //: Group ID

   mutable float m_fMinStress;
   //: Minimum stress value

   mutable float m_fMaxStress;
   //: Maximum stress value

   mutable float m_fTemp;
   //: Current temperature

   mutable float m_pfColour[3];
   //: A colour for the element

   mutable TColourScheme m_oColourScheme;
   //: Which colouring scheme to use

   //:--------
   //: Friends

   friend class CViewpoint;

};

#endif // __VTSTRUCVIS_ELEMENT__
