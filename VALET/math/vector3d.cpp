//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/math/vector3d.cpp"
//! author     = "James Smith"
//! date       = "01/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: vector3d.cpp,v 1.12 2001/10/30 00:51:47 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

#include "VALET/vector3d.h"

// VALET includes
#include "VALET/log.h"
#include "VALET/axisrotation.h"
#include "VALET/quaternion.h"
#include "VALET/scalar.h"


// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>

namespace NVALET {
   
   CVector3D::CVector3D() {
      CLog("math","CVector3D::Constructor (default)", LL_OBJECT);
      for (int i=0; i<3; i++)
         m_pdComponents[i] = 0.0F;
   } //CVector3D()
  
   CVector3D::CVector3D(const CVector3D & oVec) {
      CLog("math","CVector3D::Constructor (CVector3D)", LL_OBJECT);
      for (int i=0; i<3; i++)
         m_pdComponents[i] = oVec.m_pdComponents[i];
   } //CVector3D(const CVector3D & vec)
  
   CVector3D::CVector3D(const double dX, const double dY, const double dZ) {
      CLog("math","CVector3D::Constructor (doubles)", LL_OBJECT);
      m_pdComponents[0] = dX;
      m_pdComponents[1] = dY;
      m_pdComponents[2] = dZ;
   } //CVector3D(const double dX, const double dY, const double dZ)
  
   CVector3D::~CVector3D() {
      CLog("math","CVector3D::Destructor", LL_OBJECT);
   } //~CVector3D()
  
   CVector3D CVector3D::operator *(const double dScalar) const {
      CLog("math","CVector3D::operator* (double)");
      return CVector3D(*this) *= dScalar;
   } //operator *(const double dScalar) const
  
   CVector3D CVector3D::operator *(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator* (CVector3D)");
      return CVector3D(*this) *= oVec;
   } //operator *(const CVector3D & oVec) const
  
   CVector3D& CVector3D::operator *=(const double dScalar) {
      CLog("math","CVector3D::operator*= (double)");
      for (int i=0; i<3; i++)
         m_pdComponents[i] *= dScalar;
      return *this;
   } //operator *=(const double dScalar)
	
   CVector3D& CVector3D::operator *=(const CVector3D & oVec) {
      CLog("math","CVector3D::operator*= (CVector3D)");
      for (int i=0; i<3; i++)
         m_pdComponents[i] *= oVec.m_pdComponents[i];
      return *this;
   } //operator *=(const CVector3D & oVec)
	
   CVector3D CVector3D::operator /(const double dScalar) const {
      CLog("math","CVector3D::operator/ (double)");
      return CVector3D(*this) /= dScalar;
   } //operator /(const double dScalar) const
	
   CVector3D CVector3D::operator /(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator/ (CVector3D)");
      return CVector3D(*this) /= oVec;
   } //operator /(const CVector3D & oVec) const
	
   CVector3D& CVector3D::operator /=(const double dScalar) {
      CLog("math","CVector3D::operator/ (double)");
      for (int i=0; i<3; i++)
         m_pdComponents[i] /= dScalar;
      return *this;
   } //operator /=(const double dScalar)
	
   CVector3D& CVector3D::operator /=(const CVector3D & oVec) {
      CLog("math","CVector3D::operator/= (CVector3D)");
      for (int i=0; i<3; i++)
         m_pdComponents[i] /= oVec.m_pdComponents[i];
      return *this;	
   } //operator /=(const CVector3D & oVec)
	
   CVector3D CVector3D::operator +(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator+");
      return CVector3D(*this) += oVec;
   } //operator +(const CVector3D & vec) const
	
   CVector3D& CVector3D::operator +=(const CVector3D & oVec) {
      CLog("math","CVector3D::operator+=");
      for (int i=0; i<3; i++)
         m_pdComponents[i] += oVec.m_pdComponents[i];
      return *this;
   } //operator +=(const CVector3D & oVec)
	
   CVector3D CVector3D::operator -(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator- (binary)");
      return CVector3D(*this) -= oVec;
   } //operator -(const CVector3D & oVec) const
	
   CVector3D& CVector3D::operator -=(const CVector3D & oVec) {
      CLog("math","CVector3D::operator-=");
      for (int i=0; i<3; i++)
         m_pdComponents[i] -= oVec.m_pdComponents[i];
      return *this;
   } //operator -=(const CVector3D & oVec)
	
   CVector3D CVector3D::operator -(void) const {
      CLog("math","CVector3D::operator- (unary)");
      return CVector3D(*this).Invert();
   } //operator -(void) const
	
   CVector3D& CVector3D::Invert() {
      CLog("math","CVector3D::Invert");
      for (int i=0; i<3; i++)
         m_pdComponents[i] = -m_pdComponents[i];
      return *this;
   } //Invert()
	
   bool CVector3D::operator ==(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator==");
      for (int i=0; i<3; i++)
         if (m_pdComponents[i] != oVec.m_pdComponents[i]) 
            return false;
      return true;
   } //operator ==(const CVector3D & oVec) const
	
   bool CVector3D::operator !=(const CVector3D & oVec) const {
      CLog("math","CVector3D::operator!=");
      return !(*this==oVec);
   } //operator !=(const CVector3D & oVec) const
	
   double CVector3D::Dot(const CVector3D & oVec) const {
      CLog("math","CVector3D::Dot");
      return 
         m_pdComponents[0] * oVec.m_pdComponents[0] +
         m_pdComponents[1] * oVec.m_pdComponents[1] +
         m_pdComponents[2] * oVec.m_pdComponents[2];
   } //Dot(const CVector3D & oVec) const
	
   CVector3D CVector3D::Cross(const CVector3D & oVec) const {
      CLog("math","CVector3D::Cross");
      double dX = m_pdComponents[1] * oVec.m_pdComponents[2] - m_pdComponents[2] * oVec.m_pdComponents[1];
      double dY = m_pdComponents[2] * oVec.m_pdComponents[0] - m_pdComponents[0] * oVec.m_pdComponents[2];
      double dZ = m_pdComponents[0] * oVec.m_pdComponents[1] - m_pdComponents[1] * oVec.m_pdComponents[0];
      return CVector3D(dX,dY,dZ);
   } //Cross(const CVector3D & oVec) const
	
	
   double CVector3D::Length() const {
      CLog("math","CVector3D::Length");
      return sqrt((m_pdComponents[0] * m_pdComponents[0]) + 
                  (m_pdComponents[1] * m_pdComponents[1]) + 
                  (m_pdComponents[2] * m_pdComponents[2]));
   } //Length() const
	
   CVector3D& CVector3D::SetLength(double dLength) {  
      CLog("math","CVector3D::SetLength");
      double dScale = dLength / Length();
      *this *= dScale; 
     return *this;
   } //SetLength(double dLength)
	
   CVector3D& CVector3D::Normalise() {
      CLog("math","CVector3D::Normalise");
      return *this /= Length();
   } //Normalise()
	
   CVector3D CVector3D::Rotate(const CAxisRotation & oRot) const {
      CLog("math","CVector3D::Rotate");
      CQuaternion oRotation(oRot);
      CQuaternion oVector(0, *this);
      CQuaternion oResult = oRotation.Inverse() * oVector * oRotation;
      return oResult.Vector();
   } //Rotate(const CAxisRotation & oRot) const
	
   double CVector3D::X(void) const {
      return m_pdComponents[0];
   } //X(void) const
	
   double& CVector3D::X(void) {
      return m_pdComponents[0];
   } //X(void)
	
   double CVector3D::Y(void) const {
      return m_pdComponents[1];
   } //Y(void) const
	
   double& CVector3D::Y(void) {
      return m_pdComponents[1];
   } //Y(void)
	
   double CVector3D::Z(void) const {
      return m_pdComponents[2];
   } //Z(void) const
	
   double& CVector3D::Z(void) {
      return m_pdComponents[2];
   } //Z(void)
	
   const double* CVector3D::Array(void) {
      return m_pdComponents;
   } //Array(void)

   void CVector3D::ToDouble(double& dX, double& dY, double& dZ) const {
      CLog("math","CVector3D::ToDouble");
      dX = m_pdComponents[0];
      dY = m_pdComponents[1];
      dZ = m_pdComponents[2];
      return;
   } //ToDouble(double& dX, double& dY, double& dZ) const
	
   void CVector3D::FromDouble(double dX, double dY, double dZ) {
      CLog("math","CVector3D::FromDouble");
      m_pdComponents[0] = dX;
      m_pdComponents[1] = dY;
      m_pdComponents[2] = dZ;
      return;
   } //FromDouble(double dX, double dY, double dZ)
	
   bool CVector3D::ParseString(const char* pcInput, int* piUsed) {
      CLog("math","CVector3D::ParseString");
      const int iNumComponents = 3;
      int iCurrentComponent = 0;
      bool bFractionalPart = false;
      bool bExponentPart = false;
      int iStrLength = strlen(pcInput);
      bool bRetVal = (iStrLength == 0) ? false : true;
      unsigned long ulIntegerPart = 0;
      unsigned long ulFractionalPart = 0;
      unsigned long ulFractionalDivisor = 1;
      long lExponentPart = 0;
      int iMantissaSign = 1;
      int iExponentSign = 1;
      int iCharIndex = 0;
      bool bWaiting = false;
      for (iCharIndex=0; (iCharIndex<iStrLength) && bRetVal && (iCurrentComponent < iNumComponents); iCharIndex++) {
         if (bWaiting) {
            // Assign to current component
            lExponentPart *= iExponentSign;
            m_pdComponents[iCurrentComponent] = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
            // Reset variables
            bFractionalPart = false;
            bExponentPart = false;
            ulIntegerPart = 0;
            ulFractionalPart = 0;
            ulFractionalDivisor = 1;
            lExponentPart = 0;
            iMantissaSign = 1;
            iExponentSign = 1;
            // Move on
            iCurrentComponent++;
            bWaiting = false;
         }
         if (isdigit(pcInput[iCharIndex])) {
            if (bExponentPart) {
               lExponentPart *= 10;
               lExponentPart += pcInput [iCharIndex] - '0';
            }
            else if (bFractionalPart) {
               ulFractionalPart *= 10;
               ulFractionalDivisor *= 10;
               ulFractionalPart += pcInput[iCharIndex] - '0';
            }
            else {
               ulIntegerPart *= 10;
               ulIntegerPart += pcInput[iCharIndex] - '0';
            }
         }
         else if (pcInput[iCharIndex] == '-') {
            if (bExponentPart) {
               iExponentSign = -1;
            }
            else {
               iMantissaSign = -1;
            }
         }
         else if (pcInput[iCharIndex] == ' ') {
            // Waiting for next component.
            bWaiting = true;
         }
         else if (pcInput[iCharIndex] == '.') {
            bFractionalPart = true;
         }
         else if ((pcInput[iCharIndex] == 'e') || (pcInput[iCharIndex] == 'E')) {
            bExponentPart = true;
         }
         else bRetVal = false;
      }
      // Assign last component
      if (bRetVal && (iCurrentComponent != iNumComponents)) {
         lExponentPart *= iExponentSign;
         m_pdComponents[iCurrentComponent] = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
      }
      if (iCurrentComponent != iNumComponents-1) bRetVal = false;
      if (piUsed != NULL) {
         *piUsed = iCharIndex;
      }
      return bRetVal;
   } //ParseString(const char* pcInput, int& piUsed)
	
   char* CVector3D::ToString(int iPrecision) const {
      CLog("math","CVector3D::ToString");
      // Allocate output buffer space and initialise it to a null string.
      char* pcOutput = (char*)malloc(484);
      pcOutput[0] = 0;
      // Create each string and concatenate it onto the output string.
      for (int i=0; i<3; i++) {
         CScalar oComponent(m_pdComponents[i]);
         char* str = oComponent.ToString(iPrecision);
         strcat(pcOutput,str);
         free(str);
         if (i < 2) strcat(pcOutput," ");
      }
      int length = strlen(pcOutput) + 1;
      pcOutput = (char*)realloc(pcOutput,length);
      return pcOutput;
   } //ToString(int iPrecision) const

}

