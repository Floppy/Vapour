//=============---
// Arup Simulation Visualisation System
//-------------
// Copyright 2002 Vapour Technology Ltd.
//
// Viewpoint.h
// 19/03/2002 - James Smith
//
// $Id: Viewpoint.h,v 1.3 2002/03/22 10:52:02 vap-james Exp $

#ifndef __VIEWPOINT__
#define __VIEWPOINT__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaUtil.h"

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

   bool Set(float* pfPosition = NULL, float* pfRotation = NULL);
   // Set viewpoint position.
   // If NULL pointers are passed, default values are used.

   void Animate(bool bAnimate);
   // Animate smoothly betwen viewpoints?

//#===--- Member Variables
protected:

   CCortonaUtil* m_poCortona;
   // Cortona Utility interface

   mutable CCortonaNode *m_poNodePtr;
   // Pointer to the node in the VRML world;   

   bool m_bAnimate;
   // Smooth animation?

};

#endif // __VIEWPOINT__
