//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 	= "VALET/math/eulertype.cpp"
//! author 	= "James Smith"
//! date	= "27/10/2001"
//! lib 	= libVALETmath
//! rcsid 	= "$Id: eulertype.cpp,v 1.2 2001/10/28 14:55:55 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "VALET/eulertype.h"

// VALET includes
#include "VALET/log.h"

namespace NVALET {

   CEulerType::CEulerType(EEulerTypeID eID = EU_XYZS) {
      CLog("math","CEulerType::Constructor (EEulerTypeID)",LL_OBJECT);
      // Extract rotating frame flag
      m_bRotatingFrame = static_cast(bool,eID & 0x01);
      // Extract repetition flag
      m_bRepetition = static_cast(bool,eID & 0x02);
      // Extract odd parity flag
      m_bOddParity = static_cast(bool,eID & 0x04);
      // Extract inner axis
      m_eInnerAxis = static_cast(EEulerAxis,(eID & 0x18) >> 3);
      // Done
      return;
   }
    
   CEulerType::~CEulerType() {
      CLog("math","CEulerType::Destructor",LL_OBJECT);      
   }
   
   bool CEulerType::operator==(const CEulerType& oType) {
      CLog("math","CEulerType::operator==");
      return ((m_eInnerAxis == oType.m_eInnerAxis) &&
              (m_bRotatingFrame == oType.m_bRotatingFrame) &&
              (m_bRepetition == oType.m_bRepetition) &&
              (m_bOddParity == oType.m_bOddParity));
         
   }

   bool CEulerType::operator!=(const CEulerType& oType) {
      CLog("math","CEulerType::operator!=");
      return ((m_eInnerAxis != oType.m_eInnerAxis) ||
              (m_bRotatingFrame != oType.m_bRotatingFrame) ||
              (m_bRepetition != oType.m_bRepetition) ||
              (m_bOddParity != oType.m_bOddParity));
   }
 
}

