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
//! rcsid 	= "$Id: eulerrotation.cpp,v 1.1 2001/10/02 22:08:30 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "eulerrotation.h"

// VALET includes

// Standard includes
#include <math.h>
#include <float.h>

// defines
#define X_AXIS 0x0
#define Y_AXIS 0x1
#define Z_AXIS 0x2

namespace VALET {

  CEulerRotation::CEulerRotation() {
    m_uType.m_eID = XYZs;
  } //CEulerRotation()
  
  CEulerRotation::CEulerRotation(const UEulerType& uType) :
    m_uType(uType)
  {
  } //CEulerRotation(const UEulerType& uType)
  
  CEulerRotation::CEulerRotation(const CVector3D& vecAngles, const UEulerType& uType) :
    m_vecAngles(vecAngles),
    m_uType(uType)
  {
  } //CEulerRotation(const CVector3D& vecAngles, const UEulerType& uType)
  
  CEulerRotation::CEulerRotation(const CHomTransform & oTransform, const UEulerType& uType) : 
    m_uType(uType)
  {
    int NextAngle[] = {Y_AXIS, Z_AXIS, X_AXIS, Y_AXIS};
    int i = uType.m_sProperties.m_ucInnerAxis;
    int j = NextAngle[i + uType.m_sProperties.m_bOddParity];
    int k = NextAngle[i + 1 - uType.m_sProperties.m_bOddParity];	
    if (uType.m_sProperties.m_bRepetition) {
      double sy = sqrt(oTransform.Item(i,j)*oTransform.Item(i,j) + oTransform.Item(i,k)*oTransform.Item(i,k));
      if (sy > 16*FLT_EPSILON) {
	m_vecAngles.X() = atan2(oTransform.Item(i,j),oTransform.Item(i,k));
	m_vecAngles.Y() = atan2(sy, oTransform.Item(i,i));
	m_vecAngles.Z() = atan2(oTransform.Item(j,i), -oTransform.Item(k,i));
      }
      else {
	m_vecAngles.X() = atan2(-oTransform.Item(j,k), oTransform.Item(j,j));
	m_vecAngles.Y() = atan2(sy, oTransform.Item(i,i));
	m_vecAngles.Z() = 0;
      }
    }
    else {
      double cy = sqrt(oTransform.Item(i,i)*oTransform.Item(i,i) + oTransform.Item(j,i)*oTransform.Item(j,i));
      if (cy > 16*FLT_EPSILON) {
	m_vecAngles.X() = atan2(oTransform.Item(k,j), oTransform.Item(k,k));
	m_vecAngles.Y() = atan2(-oTransform.Item(k,i), cy);
	m_vecAngles.Z() = atan2(oTransform.Item(j,i), oTransform.Item(i,i));
      }
      else {
	m_vecAngles.X() = atan2(-oTransform.Item(j,k), oTransform.Item(j,j));
	m_vecAngles.Y() = atan2(-oTransform.Item(k,i), cy);
	m_vecAngles.Z() = 0;
      }
    }
    if (uType.m_sProperties.m_bOddParity) {
      m_vecAngles.Invert();
    }
    if (uType.m_sProperties.m_bRotatingFrame) {
      double dTemp = m_vecAngles.X();
      m_vecAngles.X() = m_vecAngles.Z();
      m_vecAngles.Z() = dTemp;
    }
  } //CEulerRotation(const CHomTransform & oTransform, const UEulerType& uType)
  
  CEulerRotation::CEulerRotation(const CQuaternion& oQuat, const UEulerType& uType) {    
    CHomTransform oTransform(oQuat);
    *this = CEulerRotation(oTransform,uType);
  } //CEulerRotation(const CQuaternion& oQuat, const UEulerType& uType)
  
  CEulerRotation::~CEulerRotation() {
  } //~CEulerRotation()
  
  //////////////////////////////////////////////////////////////////////
  // Utility Functions
  //////////////////////////////////////////////////////////////////////
  
  CEulerRotation& CEulerRotation::operator*=(const CVector3D& oVec) {
    m_vecAngles *= oVec;
    return *this;
  } //operator*=(const CVector3D& oVec)
  
  bool CEulerRotation::Limit(const CVector3D& vecMax, const CVector3D& vecMin) {
    bool bReturn = false;
    if (m_vecAngles.X() > vecMax.X()) {
      m_vecAngles.X() = vecMax.X(); 
      bReturn = true;
    }
    else if (m_vecAngles.X() < vecMin.X()) {
      m_vecAngles.X() = vecMin.X();
      bReturn = true;
    }
    if (m_vecAngles.Y() > vecMax.Y()) {
      m_vecAngles.Y() = vecMax.Y();
      bReturn = true;
    }
    else if (m_vecAngles.Y() < vecMin.Y()) {
      m_vecAngles.Y() = vecMin.Y();
      bReturn = true;
    }
    if (m_vecAngles.Z() > vecMax.Z()) {
      m_vecAngles.Z() = vecMax.Z();
      bReturn = true;
    }
    else if (m_vecAngles.Z() < vecMin.Z()) {
      m_vecAngles.Z() = vecMin.Z();
      bReturn = true;
    }
    return bReturn;
  } //Limit(const CVector3D& vecMax, const CVector3D& vecMin)
  
}
