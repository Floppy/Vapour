//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/skeleton.cpp"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: skeleton.cpp,v 1.3 2001/10/24 23:03:48 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Animation"

#include "VALET/skeleton.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {

   CSkeleton::CSkeleton() {
      CLog oLog("3d","CSkeleton::Constructor (default)",LL_OBJECT);
   }

   CSkeleton::~CSkeleton() {
      CLog oLog("3d","CSkeleton::Destructor",LL_OBJECT);
   }

   const CVector3D& CSkeleton::Translation(void) const {
      CLog oLog("3d","CSkeleton::Translation (const)",LL_FUNCTION);
      return m_oTranslation;
   }

   CVector3D& CSkeleton::Translation(void) {
      CLog oLog("3d","CSkeleton::Translation",LL_FUNCTION);
      return m_oTranslation;
   }

}
