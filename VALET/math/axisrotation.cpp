//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/math/axisrotation.cpp"
//! author     = "James Smith"
//! date       = "02/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: axisrotation.cpp,v 1.6 2001/10/27 13:06:08 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

#include "VALET/axisrotation.h"

// VALET includes
#include "VALET/scalar.h"
#include "VALET/log.h"

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>

namespace NVALET {

   CAxisRotation::CAxisRotation() :
      m_dAngle(0.0F),
      m_oAxis(0,1,0)
   {
      CLog oLog("math","CAxisRotation::Constructor (default)",LL_OBJECT);
   } //CAxisRotation()	
  
   CAxisRotation::CAxisRotation(const CAxisRotation & oRot) :
      m_dAngle(oRot.m_dAngle),
      m_oAxis(oRot.m_oAxis)
   {
      CLog oLog("math","CAxisRotation::Constructor (copy)",LL_OBJECT);
   } //CAxisRotation(const CAxisRotation & rot)
  
   CAxisRotation::CAxisRotation(const double dX, const double dY, const double dZ, const double dAngle) :
      m_dAngle(dAngle),
      m_oAxis(dX,dY,dZ)
   {
      CLog oLog("math","CAxisRotation::Constructor (doubles)",LL_OBJECT);
   } //CAxisRotation(const double dX, const double dY, const double dZ, const double dAngle)
  
   CAxisRotation::CAxisRotation(const CVector3D & oAxis, const double dAngle) :
      m_dAngle(dAngle),
      m_oAxis(oAxis)
   {
      CLog oLog("math","CAxisRotation::Constructor (CVector3D,double)",LL_OBJECT);
   } //CAxisRotation(const CVector3D & oAxis, const double dAngle)
  
   CAxisRotation::CAxisRotation(const CQuaternion & oQuat) {
      CLog oLog("math","CAxisRotation::Constructor (CQuaternion)",LL_OBJECT);
      double dHalfAngle = acos(oQuat.Scalar());
      double dSinHalfAngle = sin(dHalfAngle);
      m_dAngle = 2.0F * dHalfAngle;
      if (fabs(dSinHalfAngle) == 0) {
         CVector3D temp(0,1,0);
         m_oAxis = temp;
      }
      else {
         m_oAxis = oQuat.Vector() / dSinHalfAngle;
      }
   } //CAxisRotation(const CQuaternion & quat)
  
   CAxisRotation::CAxisRotation(const CEulerRotation & oRot) {
      CLog oLog("math","CAxisRotation::Constructor (CEulerRotation)",LL_OBJECT);
      CQuaternion oQuat(oRot);
      *this = CAxisRotation(oQuat);
   } //CAxisRotation(const CEulerRotation & oRot)
  
   CAxisRotation& CAxisRotation::operator=(const CAxisRotation& oRot) {
      CLog oLog("math","CAxisRotation::operator=");
      m_oAxis = oRot.m_oAxis;
      m_dAngle = oRot.m_dAngle;
      return *this;
   } //operator=(const CAxisRotation& oRot)
  
   bool CAxisRotation::operator ==(const CAxisRotation& oRot) const {
      CLog oLog("math","CAxisRotation::operator==");
      return ((m_dAngle == oRot.m_dAngle) && (m_oAxis == oRot.m_oAxis));
   }
  
   CAxisRotation CAxisRotation::MergeInside(const CAxisRotation & oRot) const{
      CLog oLog("math","CAxisRotation::MergeInside");
      CQuaternion first(*this);
      CQuaternion second(oRot);
      return CAxisRotation(first * second);
   }
  
   CAxisRotation CAxisRotation::MergeOutside(const CAxisRotation & oRot) const{
      CLog oLog("math","CAxisRotation::MergeOutside");
      return oRot.MergeInside(*this);
   }
  
   CAxisRotation& CAxisRotation::Normalise() {
      CLog oLog("math","CAxisRotation::Normalise");
      m_oAxis.Normalise();
      return *this;
   } //Normalise()
  
   CVector3D CAxisRotation::Axis(void) const {
      return m_oAxis;
   }
    
   CVector3D& CAxisRotation::Axis(void) {
      return m_oAxis;
   }
    
   double CAxisRotation::Angle(void) const {
      return m_dAngle;
   }
    
   double& CAxisRotation::Angle(void) {
      return m_dAngle;
   }

   void CAxisRotation::ToDouble(double& dX, double& dY, double& dZ, double& dAngle) const {
      CLog oLog("math","CAxisRotation::ToDouble");
      m_oAxis.ToDouble(dX, dY, dZ);
      dAngle = m_dAngle;
   } //ToDouble(double& dX, double& dY, double& dZ, double& dAngle) const
  
   void CAxisRotation::FromDouble(double dX, double dY, double dZ, double dAngle) {
      CLog oLog("math","CAxisRotation::FromDouble");
      m_dAngle = dAngle;
      m_oAxis.FromDouble(dX, dY, dZ);
      return;
   } //FromDouble(double dX, double dY, double dZ, double dAngle)
  
   bool CAxisRotation::ParseString(const char* pcInput, int* piUsed) {
      CLog oLog("math","CAxisRotation::ParseString");
      double pdInputData[4];
      const int iNumComponents = 4;
      int iCurrentComponent = 0;
      bool bFractionalPart = false;
      bool bExponentPart = false;
      int iStrLength = strlen(pcInput);
      bool bRetVal = (iStrLength == 0) ? false : true;
      unsigned long int ulIntegerPart = 0;
      unsigned long int ulFractionalPart = 0;
      unsigned long int ulFractionalDivisor = 1;
      long int lExponentPart = 0;
      int iMantissaSign = 1;
      int iExponentSign = 1;
      int iCharIndex = 0;
      bool waiting = false;
      for (iCharIndex=0; (iCharIndex<iStrLength) && bRetVal && (iCurrentComponent < iNumComponents); iCharIndex++) {
         if (waiting) {
            // Assign to current component
            lExponentPart *= iExponentSign;
            pdInputData[iCurrentComponent] = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
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
            waiting = false;
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
            waiting = true;
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
         pdInputData[iCurrentComponent] = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
      }
      if (iCurrentComponent != iNumComponents-1) bRetVal = false;
      if (piUsed != NULL) {
         *piUsed = iCharIndex;
      }
      m_oAxis.FromDouble(pdInputData[0],pdInputData[1],pdInputData[2]);
      m_dAngle = pdInputData[3];
      return bRetVal;
   } //ParseString(const char* pcInput, int& piUsed)
  
   char* CAxisRotation::ToString(int iPrecision) const {       
      CLog oLog("math","CAxisRotation::ToString");
      // Allocate output buffer space and initialise it to a null string.
      char* pcOutput = (char*)malloc(512);
      pcOutput[0] = 0;
      char* pcStr = 0;
      // Create axis string
      pcStr = m_oAxis.ToString(iPrecision);
      strcat(pcOutput,pcStr);
      free(pcStr);
      strcat(pcOutput," ");
      // Create angle string
      CScalar oComponent(m_dAngle);
      pcStr = oComponent.ToString(iPrecision);
      strcat(pcOutput,pcStr);
      free(pcStr);
      // Chop string and return
      int length = strlen(pcOutput) + 1;
      pcOutput = (char*)realloc(pcOutput,length);
      return pcOutput;
   } //ToString(int iPrecision) const
  
}
