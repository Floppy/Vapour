//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3d/joint.cpp"
//! author 		= "James Smith"
//! date 		= "18/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: joint.cpp,v 1.1 2001/10/18 15:45:16 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Animation"

#include "VALET/joint.h"

// VALET includes

// Standard includes
#include <math.h>

namespace NValet {

   CJoint::CJoint(CVector3D vecCentre, JointPtr pParent) :
      m_vecCentre(vecCentre),
      m_vecMinRotation(-M_PI,-M_PI,-M_PI),
      m_vecMaxRotation(M_PI,M_PI,M_PI),
      m_vecDamping(1,1,1),
      m_pParent(pParent),
      m_bDirtyAngle(false),
      m_bDirtyShape(false)
   {
   }

   CJoint::~CJoint() {
   }


}
