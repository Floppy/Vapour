//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Viewpoint.h
// 19/03/2002 - James Smith
//
// $Id: Viewpoint.h,v 1.7 2002/03/25 14:55:34 vap-james Exp $

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
