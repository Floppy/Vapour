//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/SlabElement.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: SlabElement.h,v 1.17 2002/04/03 16:20:03 vap-james Exp $"

#ifndef __VTSTRUCVIS_SLABELEMENT__
#define __VTSTRUCVIS_SLABELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

//: An interface to a Slab element in a VRML scene
// This class provides an interface to the PROTO object contained SlabElement.wrl.

class CSlabElement : public CElement {

public:

   //:-------------------------
   //: Construction/Destruction

   CSlabElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet);
   //: Constructor
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.
   //!param: poNodeSet - A pointer to a set of node positions.

   ~CSlabElement();
   //: Destructor

   //:-----------------
   //: Member Functions

   TElementType Type(void) const {return ELEMENT_SLAB;}
   //: The element type

   unsigned int Node(int iIndex) {return m_piNodes[iIndex];}
   //: Control node index.
   //!param: iIndex - The node to return. Valid values are 0 to 8.

   bool Display(const char* pcURL) const;
   //: Updates all information and displays the object
   // If the object is not already displayed, it is created.
   //!param: pcURL - a URL for the directory containing SlabElement.wrl (i.e. excluding the filename).
   //!param: return - true if successful, false otherwise.

   bool SetVisible(bool bVisible) const;
   //: Set whether the beam is visible or not.
   //!param: return - true if successful, false otherwise.

   void SetTemp(float fTemp) const;
   //: Set the temperature of the element

   void SetSize(float fThickness);
   //: Set the size of the slab
   //!param: fThickness - the thickness of the slab

   void SetNodes(const unsigned int* piNodes);
   //: Set which nodes the beam is controlled by
   //!param: piNodes - a pointer to a length-9 array of unsigned integers.

   void SetStresses(const float* pfStresses) const;
   //: Set stresses for individual nodes
   //!param: pfStresses - a pointer to a length-9 array of floats.

   void SetCracks(unsigned int iLayer, const unsigned char* pcCracks) const;
   //: Set crack information for the specified layer.
   //!param: iLayer - the layers to set information for. Unused at present.
   //!param: pcCracks - a pointer to a length-9 array of unsigned chars.

private:

   void CalculateColours(float* pfColours) const;
   //: Calculates a set of colours for the element.
   //!param: pfColours - an array of 27 floats in which the function will return data.

   void CalculateNodePositions(float* pfNodes) const;
   //: Calculates 3D node positions for the element.
   //!param: pfColours - an array of 27 floats in which the function will return data.

protected:

   //:-----------------
   //: Member Variables

   unsigned int m_piNodes[9];
   //: The node indices that define the slab

   mutable float m_pfStresses[9];
   //: Per-node stress values
   
   mutable unsigned char m_pcCracks[9];
   //: Crack values

   float m_fThickness;
   //: Slab thickness

   mutable CCortonaField *m_ppoField[4];
   //: Field value caches
   // Positions, cracks, colours, visible
};

#endif // __VTSTRUCVIS_SLABELEMENT__
