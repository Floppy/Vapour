//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HomTransform.cpp - 12/04/2000 - James Smith
//	Homogeneous transformation matrix implementation
//
// $Id: HomTransform.cpp,v 1.1 2000/06/16 21:59:18 waz Exp $
//

// Windows includes/defines
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Standard includes
#include <math.h>

// libVapour includes
#include "HomTransform.h"

//////////////////
// CHomTransform

CHomTransform::CHomTransform() {
	m_dMatrix[0]  = 1;
	m_dMatrix[1]  = 0;
	m_dMatrix[2]  = 0;
	m_dMatrix[3]  = 0;
	m_dMatrix[4]  = 0;
	m_dMatrix[5]  = 1;
	m_dMatrix[6]  = 0;
	m_dMatrix[7]  = 0;
	m_dMatrix[8]  = 0;
	m_dMatrix[9]  = 0;
	m_dMatrix[10] = 1;
	m_dMatrix[11] = 0;
	return;
} //CHomTransform()

CHomTransform::CHomTransform(const CVector3D& vecTranslation) {
	double dX, dY, dZ;
	vecTranslation.ToDouble(dX, dY, dZ);
	m_dMatrix[0]  = 1;
	m_dMatrix[1]  = 0;
	m_dMatrix[2]  = 0;
	m_dMatrix[3]  = dX;
	m_dMatrix[4]  = 0;
	m_dMatrix[5]  = 1;
	m_dMatrix[6]  = 0;
	m_dMatrix[7]  = dY;
	m_dMatrix[8]  = 0;
	m_dMatrix[9]  = 0;
	m_dMatrix[10] = 1;
	m_dMatrix[11] = dZ;
	return;
} //CHomTransform(const CVector3D& vecTranslation)

CHomTransform::CHomTransform(const CQuaternion& oQuat) {
   double dW = oQuat.GetScalar();
   double dX, dY, dZ = 0;
   oQuat.GetVector().ToDouble(dX, dY, dZ);
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
   m_dMatrix[0]  = 1 - (dYY + dZZ);
	m_dMatrix[1]  = dXY - dWZ;
	m_dMatrix[2]  = dXZ + dWY;
	m_dMatrix[3]  = 0;
	m_dMatrix[4]  = dXY + dWZ;
	m_dMatrix[5]  = 1 - (dXX + dZZ);
	m_dMatrix[6]  = dYZ - dWX;
	m_dMatrix[7]  = 0;
	m_dMatrix[8]  = dXZ - dWY;
	m_dMatrix[9]  = dYZ + dWX;
	m_dMatrix[10] = 1 - (dXX + dYY);
	m_dMatrix[11] = 0;
   return;
} //CHomTransform(const CQuaternion& oQuat)

CHomTransform::CHomTransform(const CQuaternion& oQuat, const SPoint3D& pntCentre) {
	SPoint3D pntInvCentre;
	pntInvCentre.m_dComponents[0] = -pntCentre.m_dComponents[0];
	pntInvCentre.m_dComponents[1] = -pntCentre.m_dComponents[1];
	pntInvCentre.m_dComponents[2] = -pntCentre.m_dComponents[2];
   double dW = oQuat.GetScalar();
   double dX, dY, dZ = 0;
   oQuat.GetVector().ToDouble(dX, dY, dZ);
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
   m_dMatrix[0]  = 1 - (dYY + dZZ);
	m_dMatrix[1]  = dXY - dWZ;
	m_dMatrix[2]  = dXZ + dWY;
	m_dMatrix[3]  = pntInvCentre.m_dComponents[0] * m_dMatrix[0] + pntInvCentre.m_dComponents[1] * m_dMatrix[1] + pntInvCentre.m_dComponents[2] * m_dMatrix[2] + pntCentre.m_dComponents[0];
	m_dMatrix[4]  = dXY + dWZ;
	m_dMatrix[5]  = 1 - (dXX + dZZ);
	m_dMatrix[6]  = dYZ - dWX;
	m_dMatrix[7]  = pntInvCentre.m_dComponents[0] * m_dMatrix[4] + pntInvCentre.m_dComponents[1] * m_dMatrix[5] + pntInvCentre.m_dComponents[2] * m_dMatrix[6] + pntCentre.m_dComponents[1];
	m_dMatrix[8]  = dXZ - dWY;
	m_dMatrix[9]  = dYZ + dWX;
	m_dMatrix[10] = 1 - (dXX + dYY);
	m_dMatrix[11] = pntInvCentre.m_dComponents[0] * m_dMatrix[8] + pntInvCentre.m_dComponents[1] * m_dMatrix[9] + pntInvCentre.m_dComponents[2] * m_dMatrix[10] + pntCentre.m_dComponents[2];
   return;
} //CHomTransform(const CQuaternion& oQuat, const SPoint3D& pntCentre)

CHomTransform::CHomTransform(const CAxisRotation& rotRotation) {
	double dSinAngle = sin(rotRotation.GetAngle());
	double dSinHalfAngle = sin(rotRotation.GetAngle()/2);
	double dSinSquaredHalfAngle = dSinHalfAngle * dSinHalfAngle;
	double dX, dY, dZ;
	rotRotation.GetAxis().ToDouble(dX, dY, dZ);
	double dXX = dX * dX;
	double dTwoXY = 2 * dX * dY;
	double dTwoXZ = 2 * dX * dZ;
	double dYY = dY * dY;
	double dTwoYZ = 2 * dY * dZ;
	double dZZ = dZ * dZ;
	m_dMatrix[0]  = 1 - (2 * dSinSquaredHalfAngle * (dYY + dZZ));
	m_dMatrix[1]  = dTwoXY * dSinSquaredHalfAngle - dZ * dSinAngle;
	m_dMatrix[2]  = dTwoXZ * dSinSquaredHalfAngle + dY * dSinAngle;
	m_dMatrix[3]  = 0;
	m_dMatrix[4]  = dTwoXY * dSinSquaredHalfAngle + dZ * dSinAngle;
	m_dMatrix[5]  = 1 - (2 * dSinSquaredHalfAngle * (dXX + dZZ));
	m_dMatrix[6]  = dTwoYZ * dSinSquaredHalfAngle - dX * dSinAngle;
	m_dMatrix[7]  = 0;
	m_dMatrix[8]  = dTwoXZ * dSinSquaredHalfAngle - dY * dSinAngle;
	m_dMatrix[9]  = dTwoYZ * dSinSquaredHalfAngle + dX * dSinAngle;
	m_dMatrix[10] = 1 - (2 * dSinSquaredHalfAngle * (dXX + dYY));
	m_dMatrix[11] = 0;
	return;
} //CHomTransform(const CAxisRotation& rotRotation)

CHomTransform::CHomTransform(const CAxisRotation& rotRotation, const SPoint3D& pntCentre) {
	SPoint3D pntInvCentre;
	pntInvCentre.m_dComponents[0] = -pntCentre.m_dComponents[0];
	pntInvCentre.m_dComponents[1] = -pntCentre.m_dComponents[1];
	pntInvCentre.m_dComponents[2] = -pntCentre.m_dComponents[2];
	double dSinAngle = sin(rotRotation.GetAngle());
	double dSinHalfAngle = sin(rotRotation.GetAngle()/2);
	double dSinSquaredHalfAngle = dSinHalfAngle * dSinHalfAngle;
	double dX, dY, dZ;
	rotRotation.GetAxis().ToDouble(dX, dY, dZ);
	double dXX = dX * dX;
	double dTwoXY = 2 * dX * dY;
	double dTwoXZ = 2 * dX * dZ;
	double dYY = dY * dY;
	double dTwoYZ = 2 * dY * dZ;
	double dZZ = dZ * dZ;
	m_dMatrix[0]  = 1 - (2 * dSinSquaredHalfAngle * (dYY + dZZ));
	m_dMatrix[1]  = dTwoXY * dSinSquaredHalfAngle - dZ * dSinAngle;
	m_dMatrix[2]  = dTwoXZ * dSinSquaredHalfAngle + dY * dSinAngle;
	m_dMatrix[3]  = pntInvCentre.m_dComponents[0] * m_dMatrix[0] + pntInvCentre.m_dComponents[1] * m_dMatrix[1] + pntInvCentre.m_dComponents[2] * m_dMatrix[2] + pntCentre.m_dComponents[0];
	m_dMatrix[4]  = dTwoXY * dSinSquaredHalfAngle + dZ * dSinAngle;
	m_dMatrix[5]  = 1 - (2 * dSinSquaredHalfAngle * (dXX + dZZ));
	m_dMatrix[6]  = dTwoYZ * dSinSquaredHalfAngle - dX * dSinAngle;
	m_dMatrix[7]  = pntInvCentre.m_dComponents[0] * m_dMatrix[4] + pntInvCentre.m_dComponents[1] * m_dMatrix[5] + pntInvCentre.m_dComponents[2] * m_dMatrix[6] + pntCentre.m_dComponents[1];
	m_dMatrix[8]  = dTwoXZ * dSinSquaredHalfAngle - dY * dSinAngle;
	m_dMatrix[9]  = dTwoYZ * dSinSquaredHalfAngle + dX * dSinAngle;
	m_dMatrix[10] = 1 - (2 * dSinSquaredHalfAngle * (dXX + dYY));
	m_dMatrix[11] = pntInvCentre.m_dComponents[0] * m_dMatrix[8] + pntInvCentre.m_dComponents[1] * m_dMatrix[9] + pntInvCentre.m_dComponents[2] * m_dMatrix[10] + pntCentre.m_dComponents[2];
	return;
} //CHomTransform(const CAxisRotation& rotRotation, const SPoint3D& pntCentre)

CHomTransform::CHomTransform(const CHomTransform& htTransform) {
	memcpy(m_dMatrix,htTransform.m_dMatrix,12*sizeof(double));
	return;
} //CHomTransform(const CHomTransform& htTransform)

//////////////////////////////////////////////////////////////////////
// Operator functions
//////////////////////////////////////////////////////////////////////

CHomTransform& CHomTransform::operator=(const CHomTransform& htTransform) {
	memcpy(m_dMatrix,htTransform.m_dMatrix,12*sizeof(double));
	return *this;
} //operator=(const CHomTransform& htTransform)

CHomTransform CHomTransform::operator*(const CHomTransform& htTransform) const {
	CHomTransform htReturn;
	htReturn.m_dMatrix[0]  = htTransform.m_dMatrix[0] * m_dMatrix[0] + htTransform.m_dMatrix[4] * m_dMatrix[1] + htTransform.m_dMatrix[8]  * m_dMatrix[2];
	htReturn.m_dMatrix[1]  = htTransform.m_dMatrix[1] * m_dMatrix[0] + htTransform.m_dMatrix[5] * m_dMatrix[1] + htTransform.m_dMatrix[9]  * m_dMatrix[2];
	htReturn.m_dMatrix[2]  = htTransform.m_dMatrix[2] * m_dMatrix[0] + htTransform.m_dMatrix[6] * m_dMatrix[1] + htTransform.m_dMatrix[10] * m_dMatrix[2];
	htReturn.m_dMatrix[3]  = htTransform.m_dMatrix[3] * m_dMatrix[0] + htTransform.m_dMatrix[7] * m_dMatrix[1] + htTransform.m_dMatrix[11] * m_dMatrix[2] + m_dMatrix[3];
	htReturn.m_dMatrix[4]  = htTransform.m_dMatrix[0] * m_dMatrix[4] + htTransform.m_dMatrix[4] * m_dMatrix[5] + htTransform.m_dMatrix[8]  * m_dMatrix[6];
	htReturn.m_dMatrix[5]  = htTransform.m_dMatrix[1] * m_dMatrix[4] + htTransform.m_dMatrix[5] * m_dMatrix[5] + htTransform.m_dMatrix[9]  * m_dMatrix[6];
	htReturn.m_dMatrix[6]  = htTransform.m_dMatrix[2] * m_dMatrix[4] + htTransform.m_dMatrix[6] * m_dMatrix[5] + htTransform.m_dMatrix[10] * m_dMatrix[6];
	htReturn.m_dMatrix[7]  = htTransform.m_dMatrix[3] * m_dMatrix[4] + htTransform.m_dMatrix[7] * m_dMatrix[5] + htTransform.m_dMatrix[11] * m_dMatrix[6] + m_dMatrix[7];
	htReturn.m_dMatrix[8]  = htTransform.m_dMatrix[0] * m_dMatrix[8] + htTransform.m_dMatrix[4] * m_dMatrix[9] + htTransform.m_dMatrix[8]  * m_dMatrix[10];
	htReturn.m_dMatrix[9]  = htTransform.m_dMatrix[1] * m_dMatrix[8] + htTransform.m_dMatrix[5] * m_dMatrix[9] + htTransform.m_dMatrix[9]  * m_dMatrix[10];
	htReturn.m_dMatrix[10] = htTransform.m_dMatrix[2] * m_dMatrix[8] + htTransform.m_dMatrix[6] * m_dMatrix[9] + htTransform.m_dMatrix[10] * m_dMatrix[10];
	htReturn.m_dMatrix[11] = htTransform.m_dMatrix[3] * m_dMatrix[8] + htTransform.m_dMatrix[7] * m_dMatrix[9] + htTransform.m_dMatrix[11] * m_dMatrix[10] + m_dMatrix[11];
	return htReturn;
} //operator*(const CHomTransform& htTransform) const

SPoint3D CHomTransform::operator*(const SPoint3D& pntPoint) const {
	SPoint3D pntReturn;
	pntReturn.m_dComponents[0] = m_dMatrix[0] * pntPoint.m_dComponents[0] + m_dMatrix[1] * pntPoint.m_dComponents[1] + m_dMatrix[2] * pntPoint.m_dComponents[2] + m_dMatrix[3];
	pntReturn.m_dComponents[1] = m_dMatrix[4] * pntPoint.m_dComponents[0] + m_dMatrix[5] * pntPoint.m_dComponents[1] + m_dMatrix[6] * pntPoint.m_dComponents[2] + m_dMatrix[7];
	pntReturn.m_dComponents[2] = m_dMatrix[8] * pntPoint.m_dComponents[0] + m_dMatrix[9] * pntPoint.m_dComponents[1] + m_dMatrix[10] * pntPoint.m_dComponents[2] + m_dMatrix[11];
	return pntReturn;
} //operator*(const SPoint3D& pntPoint) const
