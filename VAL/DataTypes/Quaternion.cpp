//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Quaternion.cpp - 04/03/2000 - James Smith
//	Quaternion class implementation
//
// $Id: Quaternion.cpp,v 1.1 2000/06/16 21:59:20 waz Exp $
//

// Windows includes/defines
#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Standard includes
#include <math.h>

// libVapour includes
#include "Quaternion.h"
#include "AxisRotation.h"
#include "Vector3D.h"

////////////////
// CQuaternion

CQuaternion::CQuaternion() {
	m_dScalar = 0.0F;
} //CQuaternion()

CQuaternion::CQuaternion(const CAxisRotation & oRot) {
	CAxisRotation temp = oRot.Normalise();
	double dHalfRotation = temp.GetAngle() / 2.0F;
	double dSinHalfRotation = sin(dHalfRotation);
	m_oVector = temp.GetAxis() * dSinHalfRotation;
	m_dScalar = cos(dHalfRotation);
} //CQuaternion(const CAxisRotation & oRot)

CQuaternion::CQuaternion(const CEulerRotation & oRot) {
	int NextAngle[] = {Y_AXIS, Z_AXIS, X_AXIS, Y_AXIS};
	double dFirst = oRot.Angle(0);
	double dSecond = oRot.Angle(1);
	double dThird = oRot.Angle(2);
	UEulerType uType = oRot.Type();
	int iFirstAxis = uType.m_sProperties.m_ucInnerAxis;	
	int iSecondAxis = NextAngle[iFirstAxis + uType.m_sProperties.m_bOddParity];
	int iThirdAxis = NextAngle[iFirstAxis +1 - uType.m_sProperties.m_bOddParity];
	if (uType.m_sProperties.m_bRotatingFrame) {
		double dTemp = dFirst;
		dFirst = dThird;
		dThird = dTemp;
	}
	if (uType.m_sProperties.m_bOddParity) {
		dSecond = -dSecond;
	}
	double dHF = dFirst * 0.5;
	double dHS = dSecond * 0.5;
	double dHT = dThird * 0.5;
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
	if (uType.m_sProperties.m_bRepetition) {
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
	if (uType.m_sProperties.m_bOddParity) {
		a[iSecondAxis] = -a[iSecondAxis];
	}
	m_oVector.FromDouble(a[0], a[1], a[2]);
	return;
} //CQuaternion(const CEulerRotation & oRot)

CQuaternion::CQuaternion(const CQuaternion & oQuat) {
	m_oVector = oQuat.m_oVector;
	m_dScalar = oQuat.m_dScalar;
} //CQuaternion(const CQuaternion & quat)

CQuaternion::CQuaternion(const double & dScalar, const CVector3D & oVector) {
	m_oVector = oVector;
	m_dScalar = dScalar;
} //CQuaternion(const double & dScalar, const CVector3D & oVector)

CQuaternion::~CQuaternion() {
} //~CQuaternion()

//////////////////////////////////////////////////////////////////////
// Operator Functions
//////////////////////////////////////////////////////////////////////

CQuaternion& CQuaternion::operator=(const CQuaternion& oQuat) {
   m_dScalar = oQuat.m_dScalar;
   m_oVector = oQuat.m_oVector;
	return *this;
} //operator=(const CQuaternion& oQuat)

CQuaternion CQuaternion::operator *(const CQuaternion& oQuat) const {
	CQuaternion result;
	result.m_dScalar = m_dScalar * oQuat.m_dScalar - m_oVector.Dot(oQuat.m_oVector);
	result.m_oVector = oQuat.m_oVector.Cross(m_oVector) + (oQuat.m_oVector * m_dScalar) + (m_oVector * oQuat.m_dScalar);
	return result;
} //operator *(const CQuaternion& quat) const

CQuaternion CQuaternion::operator /(const double & dScalar) const {
	CQuaternion result;
	result.m_dScalar = m_dScalar / dScalar;
	result.m_oVector = m_oVector / dScalar;
	return result;
} //operator /(const double & dScalar)

//////////////////////////////////////////////////////////////////////
//Quaternion Functions
//////////////////////////////////////////////////////////////////////
CQuaternion CQuaternion::SlerpTo(const CQuaternion& oQuat, double dAmount) const {
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
   CVector3D vecResult(dScale0*dThisX+dScale1*dTgtX,dScale0*dThisY+dScale1*dTgtY,dScale0*dThisZ+dScale1*dTgtZ);
   CQuaternion quatResult(dScale0*dThisW+dScale1*dTgtW,vecResult);
   return quatResult;
} //SlerpTo(const CQuaternion& oQuat, double dAmount) const

//////////////////////////////////////////////////////////////////////
// Geometric Functions
//////////////////////////////////////////////////////////////////////

CQuaternion CQuaternion::Conjugate(void) const {
	CQuaternion oResult;
	oResult.m_dScalar = m_dScalar;
	oResult.m_oVector = -m_oVector;
	return oResult;
} //Conjugate(void) const

double CQuaternion::Norm(void) const {
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
	CQuaternion oResult;
	oResult = (*this).Conjugate() / (*this).Norm();
	return oResult;
} //Inverse(void) const

//////////////////////////////////////////////////////////////////////
// Output Functions
//////////////////////////////////////////////////////////////////////

double CQuaternion::GetScalar(void) const {
	return m_dScalar;
} //GetScalar(void) const

CVector3D CQuaternion::GetVector(void) const {
	return m_oVector;
} //GetVector(void) const
