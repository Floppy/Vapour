//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/math/quaternion.cpp"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: quaternion.cpp,v 1.6 2001/10/24 21:33:26 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

#include "VALET/quaternion.h"

// VALET includes
#include "VALET/log.h"

// Standard includes
#include <math.h>

// defines
#define X_AXIS 0x0
#define Y_AXIS 0x1
#define Z_AXIS 0x2

namespace NVALET {

   CQuaternion::CQuaternion() {
      CLog("math","CQuaternion::Constructor (default)",LL_OBJECT);
      m_dScalar = 0.0F;
   } //CQuaternion()

   CQuaternion::CQuaternion(const CQuaternion & oQuat) {
      CLog("math","CQuaternion::Constructor (copy)",LL_OBJECT);
      m_oVector = oQuat.m_oVector;
      m_dScalar = oQuat.m_dScalar;
   } //CQuaternion(const CQuaternion & quat)

   CQuaternion::CQuaternion(const double dScalar, const CVector3D & oVector) {
      CLog("math","CQuaternion::Constructor (double,CVector3D)",LL_OBJECT);
      m_oVector = oVector;
      m_dScalar = dScalar;
   } //CQuaternion(const double & dScalar, const CVector3D & oVector)

   CQuaternion::CQuaternion(const CAxisRotation & oRot) {
      CLog("math","CQuaternion::Constructor (CAxisRotation)",LL_OBJECT);
      CAxisRotation temp = oRot;
      temp.Normalise();
      double dHalfRotation = temp.Angle() / 2.0F;
      double dSinHalfRotation = sin(dHalfRotation);
      m_oVector = temp.Axis() * dSinHalfRotation;
      m_dScalar = cos(dHalfRotation);
   } //CQuaternion(const CAxisRotation & oRot)

   CQuaternion::CQuaternion(const CEulerRotation & oRot) {
      CLog("math","CQuaternion::Constructor (CEulerRotation)",LL_OBJECT);
      int NextAngle[] = {Y_AXIS, Z_AXIS, X_AXIS, Y_AXIS};
      CVector3D oEulerAngles = oRot.Angles();
      CEulerRotation::TEulerType tType = oRot.Type();
      int iFirstAxis = tType.m_tProperties.m_ucInnerAxis;	
      int iSecondAxis = NextAngle[iFirstAxis + tType.m_tProperties.m_bOddParity];
      int iThirdAxis = NextAngle[iFirstAxis + 1 - tType.m_tProperties.m_bOddParity];
      if (tType.m_tProperties.m_bRotatingFrame) {
         double dTemp = oEulerAngles.X();
         oEulerAngles.X() = oEulerAngles.Z();
         oEulerAngles.Z() = dTemp;
      }
      if (tType.m_tProperties.m_bOddParity) {
         oEulerAngles.Y() = -oEulerAngles.Y();
      }	
      double dHF = oEulerAngles.X() * 0.5;
      double dHS = oEulerAngles.Y() * 0.5;
      double dHT = oEulerAngles.Z() * 0.5;
      double dCHF = cos(dHF);
      double dCHS = cos(dHS);
      double dCHT = cos(dHT);
      double dSHF = sin(dHF);
      double dSHS = sin(dHS);
      double dSHT = sin(dHT);
      double dCHFxCHT = dCHF * dCHT;
      double dCHFxSHT = dCHF * dSHT;
      double dSHFxCHT = dSHF * dCHT;
      double dSHFxSHT = dSHF * dSHT;
      double a[3];
      if (tType.m_tProperties.m_bRepetition) {
         a[iFirstAxis]  = dCHS * (dCHFxSHT + dSHFxCHT);
         a[iSecondAxis] = dSHS * (dCHFxCHT + dSHFxSHT);
         a[iThirdAxis]  = dSHS * (dCHFxSHT - dSHFxCHT);
         m_dScalar      = dCHS * (dCHFxCHT - dSHFxSHT);
      }
      else {
         a[iFirstAxis]  = dCHS * dSHFxCHT - dSHS * dCHFxSHT;
         a[iSecondAxis] = dCHS * dSHFxSHT + dSHS * dCHFxCHT;
         a[iThirdAxis]  = dCHS * dCHFxSHT - dSHS * dSHFxCHT;
         m_dScalar      = dCHS * dCHFxCHT + dSHS * dSHFxSHT;
      }
      if (tType.m_tProperties.m_bOddParity) {
         a[iSecondAxis] = -a[iSecondAxis];
      }	
      m_oVector.FromDouble(a[0], a[1], a[2]);
      return;
   } //CQuaternion(const CEulerRotation & oRot)

   CQuaternion::~CQuaternion() {
      CLog("math","CQuaternion::Destructor",LL_OBJECT);
   } //~CQuaternion()

   CQuaternion& CQuaternion::operator=(const CQuaternion& oQuat) {
      CLog("math","CQuaternion::operator=", LL_FUNCTION);
      m_dScalar = oQuat.m_dScalar;
      m_oVector = oQuat.m_oVector;
      return *this;
   } //operator=(const CQuaternion& oQuat)

   CQuaternion CQuaternion::operator *(const CQuaternion& oQuat) const {
      CLog("math","CQuaternion::operator*", LL_FUNCTION);
      CQuaternion oResult;
      oResult.m_dScalar = m_dScalar * oQuat.m_dScalar - m_oVector.Dot(oQuat.m_oVector);
      oResult.m_oVector = oQuat.m_oVector.Cross(m_oVector) + (oQuat.m_oVector * m_dScalar) + (m_oVector * oQuat.m_dScalar);
      return oResult;
   } //operator *(const CQuaternion& quat) const

   CQuaternion CQuaternion::operator /(const double dScalar) const {
      CLog("math","CQuaternion::operator/", LL_FUNCTION);
      return CQuaternion(m_dScalar/dScalar,m_oVector/dScalar);
   } //operator /(const double dScalar)

   CQuaternion CQuaternion::Conjugate(void) const {
      CLog("math","CQuaternion::Conjugate", LL_FUNCTION);
      CQuaternion oResult;
      oResult.m_dScalar = m_dScalar;
      oResult.m_oVector = -m_oVector;
      return oResult;
   } //Conjugate(void) const

   double CQuaternion::Norm(void) const {
      CLog("math","CQuaternion::Norm", LL_FUNCTION);
      double dResult = 0.0F;
      dResult += m_dScalar * m_dScalar;
      double dX, dY, dZ;
      m_oVector.ToDouble(dX,dY,dZ);
      dResult += dX * dX;
      dResult += dY * dY;
      dResult += dZ * dZ;
      return dResult;
   } //Norm(void) const

   CQuaternion CQuaternion::Inverse(void) const {
      CLog("math","CQuaternion::Inverse", LL_FUNCTION);
      return CQuaternion((*this).Conjugate() / (*this).Norm());
   } //Inverse(void) const

   CQuaternion CQuaternion::SlerpTo(const CQuaternion& oQuat, double dAmount) const {
      CLog("math","CQuaternion::SlerpTo", LL_FUNCTION);
      double dOmega, dCos, dSin, dScale0, dScale1;
      // Extract values from quaternions
      double dThisX, dThisY, dThisZ, dThisW, dTgtX, dTgtY, dTgtZ, dTgtW;
      m_oVector.ToDouble(dThisX,dThisY,dThisZ);
      dThisW = m_dScalar;
      oQuat.m_oVector.ToDouble(dTgtX,dTgtY,dTgtZ);
      dTgtW = oQuat.m_dScalar;
      // Calculate cosine
      dCos = (dThisX * dTgtX) + (dThisY * dTgtY) + (dThisZ * dTgtZ) + (dThisW * dTgtW);
      // Adjust signs if necessary
      if (dCos < 0.0) {
         dCos = -dCos;
         dTgtX = -dTgtX;
         dTgtY = -dTgtY;
         dTgtZ = -dTgtZ;
         dTgtW = -dTgtW;
      }
      // Calculate coeffecients
      dOmega = acos(dCos);
      dSin = sin(dOmega);
      if (dSin != 0.0) {
         // Slerping!
         dScale0 = sin((1.0 - dAmount) * dOmega) / dSin;
         dScale1 = sin(dAmount * dOmega) / dSin;
      }
      else {
         // using linear to avoid divide-by-zero
         dScale0 = 1.0 - dAmount;
         dScale1 = dAmount;
      }   
      // Calcalate result   
      CVector3D oVecResult(dScale0*dThisX+dScale1*dTgtX,dScale0*dThisY+dScale1*dTgtY,dScale0*dThisZ+dScale1*dTgtZ);
      CQuaternion oQuatResult(dScale0*dThisW+dScale1*dTgtW,oVecResult);
      return oQuatResult;
   } //SlerpTo(const CQuaternion& oQuat, double dAmount) const
	
   double CQuaternion::Scalar(void) const {
      CLog("math","CQuaternion::Scalar (const)", LL_FUNCTION);
      return m_dScalar;
   } //Scalar(void) const

   double& CQuaternion::Scalar(void) {
      CLog("math","CQuaternion::Scalar", LL_FUNCTION);
      return m_dScalar;
   } //Scalar(void)

   CVector3D CQuaternion::Vector(void) const {
      CLog("math","CQuaternion::Vector (const)", LL_FUNCTION);
      return m_oVector;
   } //Vector(void) const

   CVector3D& CQuaternion::Vector(void) {
      CLog("math","CQuaternion::Vector", LL_FUNCTION);
      return m_oVector;
   } //Vector(void)

}
