//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 	= "VALET/math/eulerrotation.cpp"
//! author 	= "James Smith"
//! date	= "02/10/2001"
//! lib 	= libVALETmath
//! rcsid 	= "$Id: eulerrotation.cpp,v 1.5 2001/10/27 13:06:08 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "VALET/eulerrotation.h"

// VALET includes
#include "VALET/log.h"

// Standard includes
#include <math.h>
#include <float.h>

namespace NVALET {

   CEulerRotation::CEulerRotation() :
      m_oType(CEulerType::EU_XYZS)
   {
      CLog("math","CEulerRotation::Constructor (default)",LL_OBJECT);
   } //CEulerRotation()
  
   CEulerRotation::CEulerRotation(const CEulerType& oType) :
      m_oType(oType)
   {
      CLog("math","CEulerRotation::Constructor (CEulerType)",LL_OBJECT);
   } //CEulerRotation(const CEulerType& oType)
  
   CEulerRotation::CEulerRotation(const CVector3D& oAngles, const CEulerType& oType) :
      m_oAngles(oAngles),
      m_oType(oType)
   {
      CLog("math","CEulerRotation::Constructor (CVector3D,CEulerType)",LL_OBJECT);
   } //CEulerRotation(const CVector3D& oAngles, const CEulerType& oType)
  
   CEulerRotation::CEulerRotation(double dAngle0, double dAngle1, double dAngle2, const CEulerType& oType) :
      m_oAngles(dAngle0,dAngle1,dAngle2),
      m_oType(oType)
   {
      CLog("math","CEulerRotation::Constructor (doubles, CEulerType)",LL_OBJECT);
   } //CEulerRotation(double dAngle0, double dAngle1, double dAngle2, const CEulerType& oType)


   CEulerRotation::CEulerRotation(const CHomTransform & oTransform, const CEulerType& oType) : 
      m_oType(oType)
   {
      CLog("math","CEulerRotation::Constructor (CHomTransform,CEulerType)",LL_OBJECT);
      CEulerType::EEulerAxis NextAngle[] = {CEulerType::EU_Y_AXIS, 
                                            CEulerType::EU_Z_AXIS, 
                                            CEulerType::EU_X_AXIS, 
                                            CEulerType::EU_Y_AXIS};
      int i = oType.InnerAxis();
      int j = NextAngle[i + oType.OddParity()];
      int k = NextAngle[i + 1 - oType.OddParity()];	
      if (oType.Repetition()) {
         double sy = sqrt(oTransform.Element(i,j)*oTransform.Element(i,j) + oTransform.Element(i,k)*oTransform.Element(i,k));
         if (sy > 16*FLT_EPSILON) {
            m_oAngles.X() = atan2(oTransform.Element(i,j),oTransform.Element(i,k));
            m_oAngles.Y() = atan2(sy, oTransform.Element(i,i));
            m_oAngles.Z() = atan2(oTransform.Element(j,i), -oTransform.Element(k,i));
         }
         else {
            m_oAngles.X() = atan2(-oTransform.Element(j,k), oTransform.Element(j,j));
            m_oAngles.Y() = atan2(sy, oTransform.Element(i,i));
            m_oAngles.Z() = 0;
         }
      }
      else {
         double cy = sqrt(oTransform.Element(i,i)*oTransform.Element(i,i) + oTransform.Element(j,i)*oTransform.Element(j,i));
         if (cy > 16*FLT_EPSILON) {
            m_oAngles.X() = atan2(oTransform.Element(k,j), oTransform.Element(k,k));
            m_oAngles.Y() = atan2(-oTransform.Element(k,i), cy);
            m_oAngles.Z() = atan2(oTransform.Element(j,i), oTransform.Element(i,i));
         }
         else {
            m_oAngles.X() = atan2(-oTransform.Element(j,k), oTransform.Element(j,j));
            m_oAngles.Y() = atan2(-oTransform.Element(k,i), cy);
            m_oAngles.Z() = 0;
         }
      }
      if (oType.OddParity()) {
         m_oAngles.Invert();
      }
      if (oType.RotatingFrame()) {
         double dTemp = m_oAngles.X();
         m_oAngles.X() = m_oAngles.Z();
         m_oAngles.Z() = dTemp;
      }
   } //CEulerRotation(const CHomTransform & oTransform, const CEulerType& oType)
  
   CEulerRotation::CEulerRotation(const CQuaternion& oQuat, const CEulerType& oType) {    
      CLog("math","CEulerRotation::Constructor (CQuaternion,CEulerType)",LL_OBJECT);
      CHomTransform oTransform(oQuat);
      *this = CEulerRotation(oTransform,oType);
   } //CEulerRotation(const CQuaternion& oQuat, const CEulerType& oType)
  
   CEulerRotation::~CEulerRotation() {
      CLog("math","CEulerRotation::Destructor",LL_OBJECT);
   } //~CEulerRotation()
  
   CEulerRotation& CEulerRotation::operator*=(const CEulerRotation& oScale) {
      CLog("math","CEulerRotation::operator*=");
      if (m_oType == oScale.m_oType)
         m_oAngles *= oScale.m_oAngles;
      return *this;
   } //operator*=(const CVector3D& oVec)
  
   bool CEulerRotation::Limit(const CEulerRotation& oMax, const CEulerRotation& oMin) {
      CLog("math","CEulerRotation::Limit");
      if ((m_oType != oMax.m_oType) || (m_oType != oMin.m_oType)) return false;
      bool bReturn = false;
      if (m_oAngles.X() > oMax.m_oAngles.X()) {
         m_oAngles.X() = oMax.m_oAngles.X(); 
         bReturn = true;
      }
      else if (m_oAngles.X() < oMin.m_oAngles.X()) {
         m_oAngles.X() = oMin.m_oAngles.X();
         bReturn = true;
      }
      if (m_oAngles.Y() > oMax.m_oAngles.Y()) {
         m_oAngles.Y() = oMax.m_oAngles.Y();
         bReturn = true;
      }
      else if (m_oAngles.Y() < oMin.m_oAngles.Y()) {
         m_oAngles.Y() = oMin.m_oAngles.Y();
         bReturn = true;
      }
      if (m_oAngles.Z() > oMax.m_oAngles.Z()) {
         m_oAngles.Z() = oMax.m_oAngles.Z();
         bReturn = true;
      }
      else if (m_oAngles.Z() < oMin.m_oAngles.Z()) {
         m_oAngles.Z() = oMin.m_oAngles.Z();
         bReturn = true;
      }
      return bReturn;
   } //Limit(const CVector3D& oMax, const CVector3D& oMin) 

   CVector3D CEulerRotation::Angles(void) const {
      return m_oAngles;
   }
    
   CVector3D& CEulerRotation::Angles(void) {
      return m_oAngles;
   }
    
   CEulerType CEulerRotation::Type(void) const {
      return m_oType;
   }
    
   CEulerType& CEulerRotation::Type(void) {
      return m_oType;
   }
 
}

