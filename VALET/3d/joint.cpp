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
//! rcsid 		= "$Id: joint.cpp,v 1.3 2001/10/23 22:42:13 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Animation"

#include "VALET/joint.h"

// VALET includes
#include "VALET/eulerrotation.h"

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

   const CAxisRotation& CJoint::Rotate(const CAxisRotation& oRot, bool bLimit, bool bDamp) {
      // Devolve to Euler rotation
      CEulerRotation::UEulerType uType;
      uType.m_eID = CEulerRotation::XYZs;
      CHomTransform htTemp(oRot);
      CEulerRotation rotEulerRotation(htTemp, uType);
      // Damp if enabled
      if (bDamp) rotEulerRotation *= m_vecDamping;
      // Convert back to axis/angle
      CAxisRotation rotDampedRotation(rotEulerRotation);
      // Add to current rotation
      rotDampedRotation.MergeInside(m_oRotation);
      // Set and limit using RotateAbs with damping turned off.
      return RotateAbs(rotDampedRotation,true,false);
   }

   const CAxisRotation& CJoint::RotateAbs(const CAxisRotation& oRot, bool bLimit, bool bDamp) {
      if (bLimit) {
         // Devolve to Euler rotation
         CEulerRotation::UEulerType uType;
         uType.m_eID = CEulerRotation::XYZs;
         CHomTransform htTemp(oRot);
         CEulerRotation rotEulerRotation(htTemp, uType);
         // Damp if enabled
         if (bDamp) rotEulerRotation *= m_vecDamping;
         // Limit
         rotEulerRotation.Limit(m_vecMaxRotation,m_vecMinRotation);
         // Convert back to axis/angle
         CAxisRotation rotLimitedRotation(rotEulerRotation);
         // Store
         m_oRotation = rotLimitedRotation;
      }
      else {
         m_oRotation = oRot;
      }
      // Set dirty flags
      m_bDirtyAngle = true;
      m_bDirtyShape = true;
      // Notify parents and children of dirtyness.
      if (m_pParent != JointPtr()) m_pParent->m_bDirtyShape = true;
      for (int i=0; i<NumChildren(); i++) {
         Child(i)->m_bDirtyShape = true;
      }
      return m_oRotation;
   }

}
