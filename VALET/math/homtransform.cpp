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
//! rcsid 	= "$Id: homtransform.cpp,v 1.1 2001/10/02 22:08:30 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "homtransform.h"

// VALET includes

// Standard includes
#include <math.h>
#include <string.h>

namespace VALET {

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
  
  CHomTransform::CHomTransform(const CHomTransform& oTransform) {
    memcpy(m_dMatrix,oTransform.m_dMatrix,12*sizeof(double));
    return;
  } //CHomTransform(const CHomTransform& oTransform)
  
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
  
  CHomTransform::CHomTransform(const CQuaternion& oQuat, const CVector3D& vecCentre) {
    CVector3D vecInvCentre = -vecCentre;
    double dW = oQuat.Scalar();
    double dX, dY, dZ = 0;
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
    m_dMatrix[0]  = 1 - (dYY + dZZ);
    m_dMatrix[1]  = dXY - dWZ;
    m_dMatrix[2]  = dXZ + dWY;
    m_dMatrix[3]  = vecInvCentre.X() * m_dMatrix[0] + vecInvCentre.Y() * m_dMatrix[1] + vecInvCentre.Z() * m_dMatrix[2] + vecCentre.X();
    m_dMatrix[4]  = dXY + dWZ;
    m_dMatrix[5]  = 1 - (dXX + dZZ);
    m_dMatrix[6]  = dYZ - dWX;
    m_dMatrix[7]  = vecInvCentre.X() * m_dMatrix[4] + vecInvCentre.Y() * m_dMatrix[5] + vecInvCentre.Z() * m_dMatrix[6] + vecCentre.Y();
    m_dMatrix[8]  = dXZ - dWY;
    m_dMatrix[9]  = dYZ + dWX;
    m_dMatrix[10] = 1 - (dXX + dYY);
    m_dMatrix[11] = vecInvCentre.X() * m_dMatrix[8] + vecInvCentre.Y() * m_dMatrix[9] + vecInvCentre.Z() * m_dMatrix[10] + vecCentre.Z();
    return;
  } //CHomTransform(const CQuaternion& oQuat, const CVector3D& vecCentre)
  
  CHomTransform::CHomTransform(const CAxisRotation& rotRotation) {
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
  
  CHomTransform::CHomTransform(const CAxisRotation& rotRotation, const CVector3D& vecCentre) {
    CVector3D vecInvCentre = -vecCentre;
    double dSinAngle = sin(rotRotation.Angle());
    double dSinHalfAngle = sin(rotRotation.Angle()/2);
    double dSinSquaredHalfAngle = dSinHalfAngle * dSinHalfAngle;
    double dX, dY, dZ;
    rotRotation.Axis().ToDouble(dX, dY, dZ);
    double dXX = dX * dX;
    double dTwoXY = 2 * dX * dY;
    double dTwoXZ = 2 * dX * dZ;
    double dYY = dY * dY;
    double dTwoYZ = 2 * dY * dZ;
    double dZZ = dZ * dZ;
    m_dMatrix[0]  = 1 - (2 * dSinSquaredHalfAngle * (dYY + dZZ));
    m_dMatrix[1]  = dTwoXY * dSinSquaredHalfAngle - dZ * dSinAngle;
    m_dMatrix[2]  = dTwoXZ * dSinSquaredHalfAngle + dY * dSinAngle;
    m_dMatrix[3]  = vecInvCentre.X() * m_dMatrix[0] + vecInvCentre.Y() * m_dMatrix[1] + vecInvCentre.Z() * m_dMatrix[2] + vecCentre.X();
    m_dMatrix[4]  = dTwoXY * dSinSquaredHalfAngle + dZ * dSinAngle;
    m_dMatrix[5]  = 1 - (2 * dSinSquaredHalfAngle * (dXX + dZZ));
    m_dMatrix[6]  = dTwoYZ * dSinSquaredHalfAngle - dX * dSinAngle;
    m_dMatrix[7]  = vecInvCentre.X() * m_dMatrix[4] + vecInvCentre.Y() * m_dMatrix[5] + vecInvCentre.Z() * m_dMatrix[6] + vecCentre.Y();
    m_dMatrix[8]  = dTwoXZ * dSinSquaredHalfAngle - dY * dSinAngle;
    m_dMatrix[9]  = dTwoYZ * dSinSquaredHalfAngle + dX * dSinAngle;
    m_dMatrix[10] = 1 - (2 * dSinSquaredHalfAngle * (dXX + dYY));
    m_dMatrix[11] = vecInvCentre.X() * m_dMatrix[8] + vecInvCentre.Y() * m_dMatrix[9] + vecInvCentre.Z() * m_dMatrix[10] + vecCentre.Z();
    return;
  } //CHomTransform(const CAxisRotation& rotRotation, const CVector3D& vecCentre)
  
  CHomTransform& CHomTransform::operator=(const CHomTransform& oTransform) {
    memcpy(m_dMatrix,oTransform.m_dMatrix,12*sizeof(double));
    return *this;
  } //operator=(const CHomTransform& oTransform)
  
  CHomTransform CHomTransform::operator*(const CHomTransform& oTransform) const {
    CHomTransform oReturn;
    oReturn.m_dMatrix[0]  = oTransform.m_dMatrix[0] * m_dMatrix[0] + oTransform.m_dMatrix[4] * m_dMatrix[1] + oTransform.m_dMatrix[8]  * m_dMatrix[2];
    oReturn.m_dMatrix[1]  = oTransform.m_dMatrix[1] * m_dMatrix[0] + oTransform.m_dMatrix[5] * m_dMatrix[1] + oTransform.m_dMatrix[9]  * m_dMatrix[2];
    oReturn.m_dMatrix[2]  = oTransform.m_dMatrix[2] * m_dMatrix[0] + oTransform.m_dMatrix[6] * m_dMatrix[1] + oTransform.m_dMatrix[10] * m_dMatrix[2];
    oReturn.m_dMatrix[3]  = oTransform.m_dMatrix[3] * m_dMatrix[0] + oTransform.m_dMatrix[7] * m_dMatrix[1] + oTransform.m_dMatrix[11] * m_dMatrix[2] + m_dMatrix[3];
    oReturn.m_dMatrix[4]  = oTransform.m_dMatrix[0] * m_dMatrix[4] + oTransform.m_dMatrix[4] * m_dMatrix[5] + oTransform.m_dMatrix[8]  * m_dMatrix[6];
    oReturn.m_dMatrix[5]  = oTransform.m_dMatrix[1] * m_dMatrix[4] + oTransform.m_dMatrix[5] * m_dMatrix[5] + oTransform.m_dMatrix[9]  * m_dMatrix[6];
    oReturn.m_dMatrix[6]  = oTransform.m_dMatrix[2] * m_dMatrix[4] + oTransform.m_dMatrix[6] * m_dMatrix[5] + oTransform.m_dMatrix[10] * m_dMatrix[6];
    oReturn.m_dMatrix[7]  = oTransform.m_dMatrix[3] * m_dMatrix[4] + oTransform.m_dMatrix[7] * m_dMatrix[5] + oTransform.m_dMatrix[11] * m_dMatrix[6] + m_dMatrix[7];
    oReturn.m_dMatrix[8]  = oTransform.m_dMatrix[0] * m_dMatrix[8] + oTransform.m_dMatrix[4] * m_dMatrix[9] + oTransform.m_dMatrix[8]  * m_dMatrix[10];
    oReturn.m_dMatrix[9]  = oTransform.m_dMatrix[1] * m_dMatrix[8] + oTransform.m_dMatrix[5] * m_dMatrix[9] + oTransform.m_dMatrix[9]  * m_dMatrix[10];
    oReturn.m_dMatrix[10] = oTransform.m_dMatrix[2] * m_dMatrix[8] + oTransform.m_dMatrix[6] * m_dMatrix[9] + oTransform.m_dMatrix[10] * m_dMatrix[10];
    oReturn.m_dMatrix[11] = oTransform.m_dMatrix[3] * m_dMatrix[8] + oTransform.m_dMatrix[7] * m_dMatrix[9] + oTransform.m_dMatrix[11] * m_dMatrix[10] + m_dMatrix[11];
    return oReturn;
  } //operator*(const CHomTransform& oTransform) const
  
  CVector3D CHomTransform::operator*(const CVector3D& oVec) const {
    CVector3D vecReturn;
    vecReturn.X() = m_dMatrix[0] * oVec.X() + m_dMatrix[1] * oVec.Y() + m_dMatrix[2]  * oVec.Z() + m_dMatrix[3];
    vecReturn.Y() = m_dMatrix[4] * oVec.X() + m_dMatrix[5] * oVec.Y() + m_dMatrix[6]  * oVec.Z() + m_dMatrix[7];
    vecReturn.Z() = m_dMatrix[8] * oVec.X() + m_dMatrix[9] * oVec.Y() + m_dMatrix[10] * oVec.Z() + m_dMatrix[11];
    return vecReturn;
  } //operator*(const CVector3D& oVec) const
  
}
