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
//! rcsid 		= "$Id: quaternion.cpp,v 1.9 2001/11/02 16:35:03 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

#include "VALET/quaternion.h"

// VALET includes
#include "VALET/log.h"

// Standard includes
#include <math.h>

namespace NVALET {

   CQuaternion::CQuaternion() :
      m_oVector(0,0,0)
   {
      CLog("math","CQuaternion::Constructor (default)",LL_OBJECT);
      m_dScalar = 1.0F;
   } //CQuaternion()

   CQuaternion::CQuaternion(const CQuaternion & oQuat) {
      CLog("math","CQuaternion::Constructor (copy)",LL_OBJECT);
      m_oVector = oQuat.m_oVector;
      m_dScalar = oQuat.m_dScalar;
      Normalise();
   } //CQuaternion(const CQuaternion & quat)

   CQuaternion::CQuaternion(const double dScalar, const CVector3D & oVector) {
      CLog("math","CQuaternion::Constructor (double,CVector3D)",LL_OBJECT);
      m_oVector = oVector;
      m_dScalar = dScalar;
      Normalise();
   } //CQuaternion(const double & dScalar, const CVector3D & oVector)

   CQuaternion::CQuaternion(const CAxisRotation & oRot) {
      CLog("math","CQuaternion::Constructor (CAxisRotation)",LL_OBJECT);
      CAxisRotation temp = oRot;
      temp.Normalise();
      double dHalfRotation = temp.Angle() / 2.0F;
      double dSinHalfRotation = sin(dHalfRotation);
      m_oVector = temp.Axis() * dSinHalfRotation;
      m_dScalar = cos(dHalfRotation);
      Normalise();
   } //CQuaternion(const CAxisRotation & oRot)

   CQuaternion::CQuaternion(const CEulerRotation & oRot) {
      CLog("math","CQuaternion::Constructor (CEulerRotation)",LL_OBJECT);
      CEulerType::EEulerAxis NextAngle[] = {CEulerType::EU_Y_AXIS, 
                                            CEulerType::EU_Z_AXIS, 
                                            CEulerType::EU_X_AXIS, 
                                            CEulerType::EU_Y_AXIS};
      CVector3D oEulerAngles = oRot.Angles();
      CEulerType oType = oRot.Type();
      int iFirstAxis = oType.InnerAxis();	
      int iSecondAxis = NextAngle[iFirstAxis + (oType.OddParity() ? 1 : 0)];
      int iThirdAxis = NextAngle[iFirstAxis + 1 - (oType.OddParity() ? 1 : 0)];
      if (oType.RotatingFrame()) {
         double dTemp = oEulerAngles.X();
         oEulerAngles.X() = oEulerAngles.Z();
         oEulerAngles.Z() = dTemp;
      }
      if (oType.OddParity()) {
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
      if (oType.Repetition()) {
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
      if (oType.OddParity()) {
         a[iSecondAxis] = -a[iSecondAxis];
      }	
      m_oVector.FromDouble(a[0], a[1], a[2]);
      Normalise();
      return;
   } //CQuaternion(const CEulerRotation & oRot)

   CQuaternion::~CQuaternion() {
      CLog("math","CQuaternion::Destructor",LL_OBJECT);
   } //~CQuaternion()

   CQuaternion& CQuaternion::operator=(const CQuaternion& oQuat) {
      CLog("math","CQuaternion::operator=");
      m_dScalar = oQuat.m_dScalar;
      m_oVector = oQuat.m_oVector;
      return *this;
   } //operator=(const CQuaternion& oQuat)

   CQuaternion CQuaternion::operator *(const CQuaternion& oQuat) const {
      CLog("math","CQuaternion::operator*");
      CQuaternion oResult;
      oResult.m_dScalar = m_dScalar * oQuat.m_dScalar - m_oVector.Dot(oQuat.m_oVector);
      oResult.m_oVector = oQuat.m_oVector.Cross(m_oVector) + (oQuat.m_oVector * m_dScalar) + (m_oVector * oQuat.m_dScalar);
      oResult.Normalise();
      return oResult;
   } //operator *(const CQuaternion& quat) const

   CQuaternion CQuaternion::Conjugate(void) const {
      CLog("math","CQuaternion::Conjugate");
      return CQuaternion(m_dScalar,-m_oVector);
   } //Conjugate(void) const

   double CQuaternion::Norm(void) const {
      CLog("math","CQuaternion::Norm");
      double dResult = 0.0F;
      dResult += m_dScalar * m_dScalar;
      double dX, dY, dZ;
      m_oVector.ToDouble(dX,dY,dZ);
      dResult += dX * dX;
      dResult += dY * dY;
      dResult += dZ * dZ;
      return dResult;
   } //Norm(void) const

   double CQuaternion::Length(void) const {
      CLog("math","CQuaternion::Length");
      return sqrt(Norm());
   } //Length(void) const

   CQuaternion& CQuaternion::Normalise(void) {
      CLog("math","CQuaternion::Normalise");
      double dMag = Length();
      m_dScalar /= dMag;
      m_oVector /= dMag;
      return *this;
   } //Normalise(void)

   CQuaternion CQuaternion::Inverse(void) const {
      CLog("math","CQuaternion::Inverse");
      return CQuaternion(Conjugate().Normalise());
   } //Inverse(void) const

   CQuaternion CQuaternion::SlerpTo(const CQuaternion& oQuat, double dAmount) const {
      CLog("math","CQuaternion::SlerpTo");
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
      double dResult = dScale0*dThisW+dScale1*dTgtW;
      CVector3D oVecResult(dScale0*dThisX+dScale1*dTgtX,dScale0*dThisY+dScale1*dTgtY,dScale0*dThisZ+dScale1*dTgtZ);
      return CQuaternion(dResult,oVecResult);
   } //SlerpTo(const CQuaternion& oQuat, double dAmount) const
	
   double CQuaternion::Scalar(void) const {
      return m_dScalar;
   } //Scalar(void) const

   CVector3D CQuaternion::Vector(void) const {
      return m_oVector;
   } //Vector(void) const

}
