//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/joint.cpp"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: joint.cpp,v 1.6 2001/10/27 13:06:45 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Animation"

#include "VALET/joint.h"

// VALET includes
#include "VALET/log.h"
#include "VALET/eulertype.h"

// Standard includes
#include <math.h>

namespace NVALET {

   CJoint::CJoint(CVector3D oCentre, JointPtr pParent) :
      m_oCentre(oCentre),
      m_pParent(pParent),
      m_bDirtyAngle(false),
      m_bDirtyShape(false)
   {
      CLog oLog("3d","CJoint::Constructor (CVector3D,JointPtr)",LL_OBJECT);
      m_oMinRotation = CEulerRotation(-M_PI,-M_PI,-M_PI,CEulerType(CEulerType::EU_XYZS));
      m_oMaxRotation = CEulerRotation(M_PI,M_PI,M_PI,CEulerType(CEulerType::EU_XYZS));
      m_oDamping = CEulerRotation(1,1,1,CEulerType(CEulerType::EU_XYZS));
   }

   CJoint::~CJoint() {
      CLog oLog("3d","CJoint::Destructor",LL_OBJECT);
   }

   const CVector3D& CJoint::Centre(void) const {
      return m_oCentre;
   }

   const CEulerRotation& CJoint::MinRotation(void) const {
      return m_oMinRotation;
   }

   CEulerRotation& CJoint::MinRotation(void) {
      return m_oMinRotation;
   }

   const CEulerRotation& CJoint::MaxRotation(void) const {
      return m_oMaxRotation;
   }

   CEulerRotation& CJoint::MaxRotation(void) {
      return m_oMaxRotation;
   }

   const CEulerRotation& CJoint::Damping(void) const {
      return m_oDamping;
   }

   CEulerRotation& CJoint::Damping(void) {
      return m_oDamping;
   }
      
   const CJoint::JointPtr CJoint::Parent(void) const {
      return m_pParent;
   }

   int CJoint::NumChildren(void) const {
      return m_oChildren.size();
   }

   const CJoint::JointPtr CJoint::Child(int iChild) const {
      return m_oChildren[iChild];
   }

   const CAxisRotation& CJoint::Rotate(const CAxisRotation& oRot, bool bLimit, bool bDamp) {
      CLog oLog("3d","CJoint::Rotate");
      // Devolve to Euler rotation
      CHomTransform oTemp(oRot);
      CEulerRotation oEulerRotation(oTemp, CEulerType(CEulerType::EU_XYZS));
      // Damp if enabled
      if (bDamp) oEulerRotation *= m_oDamping;
      // Convert back to axis/angle
      CAxisRotation oDampedRotation(oEulerRotation);
      // Add to current rotation
      oDampedRotation.MergeInside(m_oRotation);
      // Set and limit using RotateAbs with damping turned off.
      return RotateAbs(oDampedRotation,true,false);
   }

   const CAxisRotation& CJoint::RotateAbs(const CAxisRotation& oRot, bool bLimit, bool bDamp) {
      CLog oLog("3d","CJoint::RotateAbs");
      if (bLimit) {
         // Devolve to Euler rotation
         CHomTransform oTemp(oRot);
         CEulerRotation oEulerRotation(oTemp, CEulerType(CEulerType::EU_XYZS));
         // Damp if enabled
         if (bDamp) oEulerRotation *= m_oDamping;
         // Limit
         oEulerRotation.Limit(m_oMaxRotation,m_oMinRotation);
         // Convert back to axis/angle
         CAxisRotation oLimitedRotation(oEulerRotation);
         // Store
         m_oRotation = oLimitedRotation;
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
