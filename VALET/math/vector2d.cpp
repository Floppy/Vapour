//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/math/vector2d.cpp"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: vector2d.cpp,v 1.5 2001/10/27 13:06:09 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

#include "VALET/vector2d.h"

// VALET includes
#include "VALET/log.h"
#include "VALET/scalar.h"

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>

namespace NVALET {
  
   CVector2D::CVector2D() {
      CLog("math","CVector2D::Constructor (default)", LL_OBJECT);
      for (int i=0; i<2; i++)
         m_pdComponents[i] = 0.0F;
   } //CVector2D()
  
   CVector2D::CVector2D(const CVector2D & oVec) {
      CLog("math","CVector2D::Constructor (CVector2D)", LL_OBJECT);
      for (int i=0; i<2; i++)
         m_pdComponents[i] = oVec.m_pdComponents[i];
   } //CVector2D(const CVector2D & vec)
  
   CVector2D::CVector2D(const double dX, const double dY) {
      CLog("math","CVector2D::Constructor (doubles)", LL_OBJECT);
      m_pdComponents[0] = dX;
      m_pdComponents[1] = dY;
   } //CVector2D(const double dX, const double dY)
  
   CVector2D::~CVector2D() {
      CLog("math","CVector2D::Destructor", LL_OBJECT);
   } //~CVector2D()
  
   CVector2D CVector2D::operator *(const double dScalar) const {
      CLog("math","CVector2D::operator* (double)");
      return CVector2D(*this) *= dScalar;
   } //operator *(const double dScalar) const
  
   CVector2D CVector2D::operator *(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator* (CVector2D)");
      return CVector2D(*this) *= oVec;
   } //operator *(const CVector2D & oVec) const
  
   CVector2D& CVector2D::operator *=(const double dScalar) {
      CLog("math","CVector2D::operator*= (double)");
      for (int i=0; i<2; i++)
         m_pdComponents[i] *= dScalar;
      return *this;
   } //operator *=(const double dScalar)
	
   CVector2D& CVector2D::operator *=(const CVector2D & oVec) {
      CLog("math","CVector2D::operator*= (CVector2D)");
      for (int i=0; i<2; i++)
         m_pdComponents[i] *= oVec.m_pdComponents[i];
      return *this;
   } //operator *=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator /(const double dScalar) const {
      CLog("math","CVector2D::operator/ (double)");
      return CVector2D(*this) /= dScalar;
   } //operator /(const double dScalar) const
	
   CVector2D CVector2D::operator /(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator/ (CVector2D)");
      return CVector2D(*this) /= oVec;
   } //operator /(const CVector2D & oVec) const
	
   CVector2D& CVector2D::operator /=(const double dScalar) {
      CLog("math","CVector2D::operator/= (double)");
      for (int i=0; i<2; i++)
         m_pdComponents[i] /= dScalar;
      return *this;
   } //operator /=(const double dScalar)
	
   CVector2D& CVector2D::operator /=(const CVector2D & oVec) {
      CLog("math","CVector2D::operator/= (CVector2D)");
      for (int i=0; i<2; i++)
         m_pdComponents[i] /= oVec.m_pdComponents[i];
      return *this;	
   } //operator /=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator +(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator+");
      return CVector2D(*this) += oVec;
   } //operator +(const CVector2D & vec) const
	
   CVector2D& CVector2D::operator +=(const CVector2D & oVec) {
      CLog("math","CVector2D::operator+=");
      for (int i=0; i<2; i++)
         m_pdComponents[i] += oVec.m_pdComponents[i];
      return *this;
   } //operator +=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator -(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator- (binary)");
      return CVector2D(*this) -= oVec;
   } //operator -(const CVector2D & oVec) const
	
   CVector2D& CVector2D::operator -=(const CVector2D & oVec) {
      CLog("math","CVector2D::operator-=");
      for (int i=0; i<2; i++)
         m_pdComponents[i] -= oVec.m_pdComponents[i];
      return *this;
   } //operator -=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator -(void) const {
      CLog("math","CVector2D::operator- (unary)");
      return CVector2D(*this).Invert();
   } //operator -(void) const
	
   CVector2D& CVector2D::Invert() {
      CLog("math","CVector2D::Invert");
      for (int i=0; i<2; i++)
         m_pdComponents[i] = -m_pdComponents[i];
      return *this;
   } //Invert()
	
   bool CVector2D::operator ==(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator==");
      for (int i=0; i<2; i++)
         if (m_pdComponents[i] != oVec.m_pdComponents[i]) 
            return false;
      return true;
   } //operator ==(const CVector2D & oVec) const
	
   bool CVector2D::operator !=(const CVector2D & oVec) const {
      CLog("math","CVector2D::operator!=");
      return !(*this==oVec);
   } //operator !=(const CVector2D & oVec) const
	
   double CVector2D::Dot(const CVector2D & oVec) const {
      CLog("math","CVector2D::Dot");
      return 
         m_pdComponents[0] * oVec.m_pdComponents[0] +
         m_pdComponents[1] * oVec.m_pdComponents[1];
   } //Dot(const CVector2D & oVec) const
   
   double CVector2D::Length() const {
      CLog("math","CVector2D::Length");
      return sqrt((m_pdComponents[0] * m_pdComponents[0]) + 
                  (m_pdComponents[1] * m_pdComponents[1]));
   } //Length() const
	
   CVector2D& CVector2D::SetLength(double dLength) {  
      CLog("math","CVector2D::SetLength");
      double dScale = dLength / Length();
      *this *= dScale;
      return *this;
   } //SetLength(double dLength)
	
   CVector2D& CVector2D::Normalise() {
      CLog("math","CVector2D::Normalise");
      return *this /= Length();
   } //Normalise()
	
   double CVector2D::X(void) const {
      return m_pdComponents[0];
   } //X(void) const
	
   double& CVector2D::X(void) {
      return m_pdComponents[0];
   } //X(void)
	
   double CVector2D::Y(void) const {
      return m_pdComponents[1];
   } //Y(void) const
	
   double& CVector2D::Y(void) {
      return m_pdComponents[1];
   } //Y(void)
	
   void CVector2D::ToDouble(double& dX, double& dY) const {
      CLog("math","CVector2D::ToDouble");
      dX = m_pdComponents[0];
      dY = m_pdComponents[1];
      return;
   } //ToDouble(double& dX, double& dY) const
	
   void CVector2D::FromDouble(double dX, double dY) {
      CLog("math","CVector2D::FromDouble");
      m_pdComponents[0] = dX;
      m_pdComponents[1] = dY;
      return;
   } //FromDouble(double dX, double dY)
	
   bool CVector2D::ParseString(const char* pcInput, int* piUsed) {
      CLog("math","CVector2D::ParseString");
      const int iNumComponents = 2;
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
      bool waiting = false;
      for (iCharIndex=0; (iCharIndex<iStrLength) && bRetVal && (iCurrentComponent < iNumComponents); iCharIndex++) {
         if (waiting) {
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
         m_pdComponents[iCurrentComponent] = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
      }
      if (iCurrentComponent != iNumComponents-1) bRetVal = false;
      if (piUsed != NULL) {
         *piUsed = iCharIndex;
      }
      return bRetVal;
   } //ParseString(const char* pcInput, int& piUsed)
	
   char* CVector2D::ToString(int iPrecision) const {
      CLog("math","CVector2D::ToString");
      // Allocate output buffer space and initialise it to a null string.
      char* pcOutput = (char*)malloc(484);
      pcOutput[0] = 0;
      // Create each string and concatenate it onto the output string.
      for (int i=0; i<2; i++) {
         CScalar oComponent(m_pdComponents[i]);
         char* pcStr = oComponent.ToString(iPrecision);
         strcat(pcOutput,pcStr);
         free(pcStr);
         if (i < 1) strcat(pcOutput," ");
      }
      int length = strlen(pcOutput) + 1;
      pcOutput = (char*)realloc(pcOutput,length);
      return pcOutput;
   } //ToString(int iPrecision) const

}

