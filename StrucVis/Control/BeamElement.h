//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// BeamElement.h
// 19/03/2002 - James Smith
//
// $Id: BeamElement.h,v 1.1 2002/03/20 02:06:24 vap-james Exp $

#ifndef __BEAM_ELEMENT__
#define __BEAM_ELEMENT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Element.h"

class CBeamElement : public CElement {
//#===--- Construction/Destruction
public:
   CBeamElement(CCortonaUtil *pCortona);
   // Constructor

//#===--- Member Functions

   TElementType Type(void) const {return BEAM;}
   // Return the element type.

   bool Display(void) const;
   // Updates all information and displays the object
   // Returns true if successful, false otherwise.
   // If the object is not already displayed, it is created.

   void SetSize(float fHeight, float fWidth, float fFlangeThickness, float fWebThickness);
   // Sets the sizes of the I-beam

   void SetNodes(int iFirstNode, int iSecondNode);
   // Sets which nodes the beam is between

//#===--- Member Variables
protected:

   unsigned int m_piNodes[2];
   // The node indices that define the beam.

};

#endif // __BEAM_ELEMENT__
