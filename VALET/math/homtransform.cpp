//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 	= "VALET/math/homtransform.cpp"
//! author 	= "James Smith"
//! date	= "02/10/2001"
//! lib 	= libVALETmath
//! rcsid 	= "$Id: homtransform.cpp,v 1.4 2001/10/24 21:33:25 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "VALET/homtransform.h"

// VALET includes
#include "VALET/log.h"

// Standard includes
#include <math.h>
#include <string.h>

namespace NVALET {

   CHomTransform::CHomTransform() {
      CLog oLog("math","CHomTransform::Constructor (default)",LL_OBJECT);
      memset(m_pdMatrix,0,12*sizeof(double));
      m_pdMatrix[0]  = 1;
      m_pdMatrix[5]  = 1;
      m_pdMatrix[10] = 1;
      return;
   } //CHomTransform()
  
   CHomTransform::CHomTransform(const CHomTransform& oTransform) {
      CLog oLog("math","CHomTransform::Constructor (copy)",LL_OBJECT);
      memcpy(m_pdMatrix,oTransform.m_pdMatrix,12*sizeof(double));
      return;
   } //CHomTransform(const CHomTransform& oTransform)
  
   CHomTransform::CHomTransform(const CVector3D& oTranslation) {
      CLog oLog("math","CHomTransform::Constructor (CVector3D)",LL_OBJECT);
      memset(m_pdMatrix,0,12*sizeof(double));
      m_pdMatrix[0]  = 1;
      m_pdMatrix[3]  = oTranslation.X();
      m_pdMatrix[5]  = 1;
      m_pdMatrix[7]  = oTranslation.Y();
      m_pdMatrix[10] = 1;
      m_pdMatrix[11] = oTranslation.Z();
      return;
   } //CHomTransform(const CVector3D& oTranslation)
  
   CHomTransform::CHomTransform(const CQuaternion& oQuat) {
      CLog oLog("math","CHomTransform::Constructor (CQuaternion)",LL_OBJECT);
      double dW = oQuat.Scalar();
      double dX=0, dY=0, dZ=0;
      oQuat.Vector().ToDouble(dX, dY, dZ);
      double d2X = dX + dX;
      double d2Y = dY + dY;
      double d2Z = dZ + dZ;
      double dXX = dX * d2X;
      double dXY = dX * d2Y;
      double dXZ = dX * d2Z;
      double dYY = dY * d2Y;
      double dYZ = dY * d2Z;
      double dZZ = dZ * d2Z;
      double dWX = dW * d2X;
      double dWY = dW * d2Y;
      double dWZ = dW * d2Z;
      m_pdMatrix[0]  = 1 - (dYY + dZZ);
      m_pdMatrix[1]  = dXY - dWZ;
      m_pdMatrix[2]  = dXZ + dWY;
      m_pdMatrix[3]  = 0;
      m_pdMatrix[4]  = dXY + dWZ;
      m_pdMatrix[5]  = 1 - (dXX + dZZ);
      m_pdMatrix[6]  = dYZ - dWX;
      m_pdMatrix[7]  = 0;
      m_pdMatrix[8]  = dXZ - dWY;
      m_pdMatrix[9]  = dYZ + dWX;
      m_pdMatrix[10] = 1 - (dXX + dYY);
      m_pdMatrix[11] = 0;
      return;
   } //CHomTransform(const CQuaternion& oQuat)
  
   CHomTransform::CHomTransform(const CQuaternion& oQuat, const CVector3D& oCentre) {
      CLog oLog("math","CHomTransform::Constructor (CQuaternion,CVector3D)",LL_OBJECT);
      *this = CHomTransform(oQuat);
      ChangeCentre(oCentre);
      return;
   } //CHomTransform(const CQuaternion& oQuat, const CVector3D& oCentre)
  
   CHomTransform::CHomTransform(const CAxisRotation& rotRotation) {
      CLog oLog("math","CHomTransform::Constructor (CAxisRotation)",LL_OBJECT);
      double dSinAngle = sin(rotRotation.Angle());
      double dSinHalfAngle = sin(rotRotation.Angle()/2);
      double dSinSquaredHalfAngle = dSinHalfAngle * dSinHalfAngle;
      double dX=0, dY=0, dZ=0;
      rotRotation.Axis().ToDouble(dX, dY, dZ);
      double dXX = dX * dX;
      double dTwoXY = 2 * dX * dY;
      double dTwoXZ = 2 * dX * dZ;
      double dYY = dY * dY;
      double dTwoYZ = 2 * dY * dZ;
      double dZZ = dZ * dZ;
      m_pdMatrix[0]  = 1 - (2 * dSinSquaredHalfAngle * (dYY + dZZ));
      m_pdMatrix[1]  = dTwoXY * dSinSquaredHalfAngle - dZ * dSinAngle;
      m_pdMatrix[2]  = dTwoXZ * dSinSquaredHalfAngle + dY * dSinAngle;
      m_pdMatrix[3]  = 0;
      m_pdMatrix[4]  = dTwoXY * dSinSquaredHalfAngle + dZ * dSinAngle;
      m_pdMatrix[5]  = 1 - (2 * dSinSquaredHalfAngle * (dXX + dZZ));
      m_pdMatrix[6]  = dTwoYZ * dSinSquaredHalfAngle - dX * dSinAngle;
      m_pdMatrix[7]  = 0;
      m_pdMatrix[8]  = dTwoXZ * dSinSquaredHalfAngle - dY * dSinAngle;
      m_pdMatrix[9]  = dTwoYZ * dSinSquaredHalfAngle + dX * dSinAngle;
      m_pdMatrix[10] = 1 - (2 * dSinSquaredHalfAngle * (dXX + dYY));
      m_pdMatrix[11] = 0;
      return;
   } //CHomTransform(const CAxisRotation& rotRotation)
  
   CHomTransform::CHomTransform(const CAxisRotation& rotRotation, const CVector3D& oCentre) {
      CLog oLog("math","CHomTransform::Constructor (CAxisRotation,CVector3D)",LL_OBJECT);
      *this = CHomTransform(rotRotation);
      ChangeCentre(oCentre);
      return;
   } //CHomTransform(const CAxisRotation& rotRotation, const CVector3D& oCentre)
  
   CHomTransform::~CHomTransform() {
      CLog oLog("math","CHomTransform::Destructor",LL_OBJECT);
   } //~CHomTransform

   CHomTransform& CHomTransform::operator=(const CHomTransform& oTransform) {
      CLog oLog("math","CHomTransform::operator=",LL_FUNCTION);
      memcpy(m_pdMatrix,oTransform.m_pdMatrix,12*sizeof(double));
      return *this;
   } //operator=(const CHomTransform& oTransform)
  
   CHomTransform CHomTransform::operator*(const CHomTransform& oTransform) const {
      CLog oLog("math","CHomTransform::operator* (CHomTransform)",LL_FUNCTION);
      CHomTransform oReturn;
      oReturn.m_pdMatrix[0]  = oTransform.m_pdMatrix[0] * m_pdMatrix[0] + oTransform.m_pdMatrix[4] * m_pdMatrix[1] + oTransform.m_pdMatrix[8]  * m_pdMatrix[2];
      oReturn.m_pdMatrix[1]  = oTransform.m_pdMatrix[1] * m_pdMatrix[0] + oTransform.m_pdMatrix[5] * m_pdMatrix[1] + oTransform.m_pdMatrix[9]  * m_pdMatrix[2];
      oReturn.m_pdMatrix[2]  = oTransform.m_pdMatrix[2] * m_pdMatrix[0] + oTransform.m_pdMatrix[6] * m_pdMatrix[1] + oTransform.m_pdMatrix[10] * m_pdMatrix[2];
      oReturn.m_pdMatrix[3]  = oTransform.m_pdMatrix[3] * m_pdMatrix[0] + oTransform.m_pdMatrix[7] * m_pdMatrix[1] + oTransform.m_pdMatrix[11] * m_pdMatrix[2] + m_pdMatrix[3];
      oReturn.m_pdMatrix[4]  = oTransform.m_pdMatrix[0] * m_pdMatrix[4] + oTransform.m_pdMatrix[4] * m_pdMatrix[5] + oTransform.m_pdMatrix[8]  * m_pdMatrix[6];
      oReturn.m_pdMatrix[5]  = oTransform.m_pdMatrix[1] * m_pdMatrix[4] + oTransform.m_pdMatrix[5] * m_pdMatrix[5] + oTransform.m_pdMatrix[9]  * m_pdMatrix[6];
      oReturn.m_pdMatrix[6]  = oTransform.m_pdMatrix[2] * m_pdMatrix[4] + oTransform.m_pdMatrix[6] * m_pdMatrix[5] + oTransform.m_pdMatrix[10] * m_pdMatrix[6];
      oReturn.m_pdMatrix[7]  = oTransform.m_pdMatrix[3] * m_pdMatrix[4] + oTransform.m_pdMatrix[7] * m_pdMatrix[5] + oTransform.m_pdMatrix[11] * m_pdMatrix[6] + m_pdMatrix[7];
      oReturn.m_pdMatrix[8]  = oTransform.m_pdMatrix[0] * m_pdMatrix[8] + oTransform.m_pdMatrix[4] * m_pdMatrix[9] + oTransform.m_pdMatrix[8]  * m_pdMatrix[10];
      oReturn.m_pdMatrix[9]  = oTransform.m_pdMatrix[1] * m_pdMatrix[8] + oTransform.m_pdMatrix[5] * m_pdMatrix[9] + oTransform.m_pdMatrix[9]  * m_pdMatrix[10];
      oReturn.m_pdMatrix[10] = oTransform.m_pdMatrix[2] * m_pdMatrix[8] + oTransform.m_pdMatrix[6] * m_pdMatrix[9] + oTransform.m_pdMatrix[10] * m_pdMatrix[10];
      oReturn.m_pdMatrix[11] = oTransform.m_pdMatrix[3] * m_pdMatrix[8] + oTransform.m_pdMatrix[7] * m_pdMatrix[9] + oTransform.m_pdMatrix[11] * m_pdMatrix[10] + m_pdMatrix[11];
      return oReturn;
   } //operator*(const CHomTransform& oTransform) const
  
   CVector3D CHomTransform::operator*(const CVector3D& oVec) const {
      CLog oLog("math","CHomTransform::operator* (CVector3D)",LL_FUNCTION);
      CVector3D oReturn;
      oReturn.X() = m_pdMatrix[0] * oVec.X() + m_pdMatrix[1] * oVec.Y() + m_pdMatrix[2]  * oVec.Z() + m_pdMatrix[3];
      oReturn.Y() = m_pdMatrix[4] * oVec.X() + m_pdMatrix[5] * oVec.Y() + m_pdMatrix[6]  * oVec.Z() + m_pdMatrix[7];
      oReturn.Z() = m_pdMatrix[8] * oVec.X() + m_pdMatrix[9] * oVec.Y() + m_pdMatrix[10] * oVec.Z() + m_pdMatrix[11];
      return oReturn;
   } //operator*(const CVector3D& oVec) const

   double CHomTransform::Element(const unsigned int iRow, const unsigned int iCol) const {
      CLog oLog("math","CHomTransform::Element",LL_FUNCTION);      
      return m_pdMatrix[iRow*4 + iCol];
   }

   void CHomTransform::ChangeCentre(const CVector3D& oCentre) {
      CLog oLog("math","CHomTransform::ChangeCentre",LL_FUNCTION);
      CVector3D oTranslation = (*this * -oCentre) + oCentre;
      m_pdMatrix[3]  = oTranslation.X();
      m_pdMatrix[7]  = oTranslation.Y();
      m_pdMatrix[11] = oTranslation.Z();
      return;
   }
  
}
