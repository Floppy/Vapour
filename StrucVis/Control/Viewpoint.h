//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Scene Management.VRML Interface"
//! userlevel =  Normal
//! file      = "Control/Viewpoint.h"
//! author    = "James Smith"
//! date      = "19/3/2002"
//! rcsid     = "$Id: Viewpoint.h,v 1.10 2002/04/03 15:57:13 vap-warren Exp $"

#ifndef __VTSTRUCVIS_VIEWPOINT__
#define __VTSTRUCVIS_VIEWPOINT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"
#include "Element.h"

class CViewpoint {
//#===--- Construction/Destruction
public:

   CViewpoint(CCortonaUtil *pCortona) :
      m_poCortona(pCortona),
      m_poNodePtr(NULL),
      m_bAnimate(false)
   {
      //Set();
   }
   // Constructor

   
   ~CViewpoint() {
      if (m_poNodePtr) {
         m_poNodePtr->Release();
         delete m_poNodePtr;
      }
   }
   // Destructor

   bool Set(const char* pcURL, float* pfPosition = NULL, float* pfRotation = NULL);
   // Set viewpoint position.
   // If NULL pointers are passed, default values are used.

   void Animate(bool bAnimate);
   // Animate smoothly betwen viewpoints?

   bool Connect(const CElement* pElement);
   // Connects the element to the viewpoint
   // for description display purposes.

   bool Redisplay(void);
   // Redisplays the viewpoint in the scene.

   bool GetCurrentPosition(float& fX, float& fY, float& fZ);
   // Gets the current viewpoint position as a set of numbers
   // Returns true if successful
   
   const char* GetCurrentPosition(void);
   // Gets the current viewpoint position as a string
   // Returns NULL on failure.
   // It is the client's responsibility to delete the memory returned
   
   bool GetCurrentOrientation(float& fX, float& fY, float& fZ, float& fA);
   // Gets the current viewpoint orientation as a set of numbers
   // Returns true if successful

   const char* GetCurrentOrientation(void);
   // Gets the current viewpoint orientation as a string
   // Returns NULL on failure.
   // It is the client's responsibility to delete the memory returned

//#===--- Member Variables
protected:

   CCortonaUtil* m_poCortona;
   // Cortona Utility interface

   mutable CCortonaNode *m_poNodePtr;
   // Pointer to the node in the VRML world;   

   bool m_bAnimate;
   // Smooth animation?

};

#endif // __VTSTRUCVIS_VIEWPOINT__
