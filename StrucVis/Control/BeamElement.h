//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/BeamElement.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: BeamElement.h,v 1.20 2002/04/04 13:59:15 vap-warren Exp $"

#ifndef __VTSTRUCVIS_BEAMELEMENT__
#define __VTSTRUCVIS_BEAMELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

//: An interface to an I-Beam element in a VRML scene
// This class provides an interface to the PROTO object contained BeamElement.wrl.

class CBeamElement : public CElement {

public:

   //:-------------------------
   //: Construction/Destruction

   CBeamElement(CCortonaUtil *poCortona, CNodeSet* poNodeSet);
   //: Constructor
   //!param: poCortona - A pointer to a valid CortonaUtil object, used to communicate with the browser.
   //!param: poNodeSet - A pointer to a set of node positions.

   ~CBeamElement();
   //: Destructor

   //:-----------------
   //: Member Functions

   TElementType Type(void) const {return ELEMENT_BEAM;}
   //: The element type

   unsigned int Node(int iIndex) {return m_piNodes[iIndex];}
   //: Control node index.
   //!param: iIndex - The node to return. Valid values are 0 or 1.

   bool Display(const char* pcURL) const;
   //: Updates all information and displays the object
   // If the object is not already displayed, it is created.
   //!param: pcURL - a URL for the directory containing BeamElement.wrl (i.e. excluding the filename).
   //!return: true if successful, false otherwise.

   bool SetVisible(bool bVisible) const;
   //: Set whether the beam is visible or not.
   //!return: true if successful, false otherwise.

   void SetTemp(float fTemp) const;
   //: Set the temperature of the element

   void SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness);
   //: Set the size of the I-Beam
   //!param: fHeight - the height of the beam
   //!param: fWidth - the width of the beam
   //!param: fFlangeThickness - the thickness of the flange part of the beam
   //!param: fWebThickness - the thickness of the web part of the beam

   void SetNodes(const unsigned int* piNodes);
   //: Set which nodes the beam is controlled by
   //!param: piNodes - a pointer to a length-2 array of unsigned integers.

   void SetStresses(const float* pfStresses) const;
   //: Set stresses for individual nodes
   //!param: pfStresses - a pointer to a length-2 array of floats.

private:

   void CalculateColours(float* pfColours) const;
   //: Calculates a set of colours for the element.
   //!param: pfColours - an array of 6 floats in which the function will return data.

   void CalculateNodePositions(float* pfNodes) const;
   //: Calculates 3D node positions for the element.
   //!param: pfColours - an array of 6 floats in which the function will return data.

protected:

   //:-----------------
   //: Member Variables

   unsigned int m_piNodes[2];
   //: The node indices that define the beam.

   mutable float m_pfStresses[2];
   //: Per-node stress values
   
   float m_fHeight;
   //: Beam height
   
   float m_fWidth;
   //: Beam width

   float m_fFlange;
   //: Flange thickness

   float m_fWeb;
   //: Web thickness

   mutable CCortonaField *m_ppoField[3];
   //: Field value caches
   // Positions, colours, visible

};

#endif // __VTSTRUCVIS_BEAMELEMENT__
