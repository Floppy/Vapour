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
//! rcsid      = "$Id: vector2d.cpp,v 1.2 2001/10/23 16:01:53 vap-warren Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

#include "vector2d.h"

// VALET includes
#include "scalar.h"

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>

namespace NValet {
  
   CVector2D::CVector2D() {
      for (int i=0; i<2; i++)
         m_pComponents[i] = 0.0F;
   } //CVector2D()
  
   CVector2D::CVector2D(const CVector2D & oVec) {
      for (int i=0; i<2; i++)
         m_pComponents[i] = oVec.m_pComponents[i];
   } //CVector2D(const CVector2D & vec)
  
   CVector2D::CVector2D(const double dX, const double dY) {
      m_pComponents[0] = dX;
      m_pComponents[1] = dY;
   } //CVector2D(const double dX, const double dY)
  
   CVector2D::~CVector2D() {
   } //~CVector2D()
  
   CVector2D CVector2D::operator *(const double dScalar) const {
      return CVector2D(*this) *= dScalar;
   } //operator *(const double dScalar) const
  
   CVector2D CVector2D::operator *(const CVector2D & oVec) const {
      return CVector2D(*this) *= oVec;
   } //operator *(const CVector2D & oVec) const
  
   CVector2D& CVector2D::operator *=(const double dScalar) {
      for (int i=0; i<2; i++)
         m_pComponents[i] *= dScalar;
      return *this;
   } //operator *=(const double dScalar)
	
   CVector2D& CVector2D::operator *=(const CVector2D & oVec) {
      for (int i=0; i<2; i++)
         m_pComponents[i] *= oVec.m_pComponents[i];
      return *this;
   } //operator *=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator /(const double dScalar) const {
      return CVector2D(*this) /= dScalar;
   } //operator /(const double dScalar) const
	
   CVector2D CVector2D::operator /(const CVector2D & oVec) const {
      return CVector2D(*this) /= oVec;
   } //operator /(const CVector2D & oVec) const
	
   CVector2D& CVector2D::operator /=(const double dScalar) {
      for (int i=0; i<2; i++)
         m_pComponents[i] /= dScalar;
      return *this;
   } //operator /=(const double dScalar)
	
   CVector2D& CVector2D::operator /=(const CVector2D & oVec) {
      for (int i=0; i<2; i++)
         m_pComponents[i] /= oVec.m_pComponents[i];
      return *this;	
   } //operator /=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator +(const CVector2D & oVec) const {
      return CVector2D(*this) += oVec;
   } //operator +(const CVector2D & vec) const
	
   CVector2D& CVector2D::operator +=(const CVector2D & oVec) {
      for (int i=0; i<2; i++)
         m_pComponents[i] += oVec.m_pComponents[i];
      return *this;
   } //operator +=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator -(const CVector2D & oVec) const {
      return CVector2D(*this) -= oVec;
   } //operator -(const CVector2D & oVec) const
	
   CVector2D& CVector2D::operator -=(const CVector2D & oVec) {
      for (int i=0; i<2; i++)
         m_pComponents[i] -= oVec.m_pComponents[i];
      return *this;
   } //operator -=(const CVector2D & oVec)
	
   CVector2D CVector2D::operator -(void) const {
      return CVector2D(*this).Invert();
   } //operator -(void) const
	
   CVector2D& CVector2D::Invert() {
      for (int i=0; i<2; i++)
         m_pComponents[i] = -m_pComponents[i];
      return *this;
   } //Invert()
	
   bool CVector2D::operator ==(const CVector2D & oVec) const
   {
      for (int i=0; i<2; i++)
         if (m_pComponents[i] != oVec.m_pComponents[i]) 
            return false;
      return true;
   } //operator ==(const CVector2D & oVec) const
	
   bool CVector2D::operator !=(const CVector2D & oVec) const
   {
      return !(*this==oVec);
   } //operator !=(const CVector2D & oVec) const
	
   double CVector2D::Dot(const CVector2D & oVec) const {
      return 
         m_pComponents[0] * oVec.m_pComponents[0] +
         m_pComponents[1] * oVec.m_pComponents[1];
   } //Dot(const CVector2D & oVec) const
   
   double CVector2D::Length() const {
      return sqrt((m_pComponents[0] * m_pComponents[0]) + 
                  (m_pComponents[1] * m_pComponents[1]));
   } //Length() const
	
   CVector2D& CVector2D::SetLength(double dLength) {  
      *this /= Length();
      *this *= dLength;
      return *this;
   } //SetLength(double dLength)
	
   CVector2D& CVector2D::Normalise() {
      return *this /= Length();
   } //Normalise()
	
   double CVector2D::X(void) const {
      return m_pComponents[0];
   } //X(void) const
	
   double& CVector2D::X(void) {
      return m_pComponents[0];
   } //X(void)
	
   double CVector2D::Y(void) const {
      return m_pComponents[1];
   } //Y(void) const
	
   double& CVector2D::Y(void) {
      return m_pComponents[1];
   } //Y(void)
	
   void CVector2D::ToDouble(double& dX, double& dY) const {
      dX = m_pComponents[0];
      dY = m_pComponents[1];
      return;
   } //ToDouble(double& dX, double& dY) const
	
   void CVector2D::FromDouble(double dX, double dY) {
      m_pComponents[0] = dX;
      m_pComponents[1] = dY;
      return;
   } //FromDouble(double dX, double dY)
	
   bool CVector2D::ParseString(const char* strInput, int* used) {
      const int iNumComponents = 2;
      int iCurrentComponent = 0;
      bool bFractionalPart = false;
      bool bExponentPart = false;
      int iStrLength = strlen(strInput);
      bool bRetVal = (iStrLength == 0) ? false : true;
      unsigned long int uliIntegerPart = 0;
      unsigned long int uliFractionalPart = 0;
      unsigned long int uliFractionalDivisor = 1;
      long int liExponentPart = 0;
      int iMantissaSign = 1;
      int iExponentSign = 1;
      int iCharIndex = 0;
      bool waiting = false;
      for (iCharIndex=0; (iCharIndex<iStrLength) && bRetVal && (iCurrentComponent < iNumComponents); iCharIndex++) {
         if (waiting) {
            // Assign to current component
            liExponentPart *= iExponentSign;
            m_pComponents[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
            // Reset variables
            bFractionalPart = false;
            bExponentPart = false;
            uliIntegerPart = 0;
            uliFractionalPart = 0;
            uliFractionalDivisor = 1;
            liExponentPart = 0;
            iMantissaSign = 1;
            iExponentSign = 1;
            // Move on
            iCurrentComponent++;
            waiting = false;
         }
         if (isdigit(strInput[iCharIndex])) {
            if (bExponentPart) {
               liExponentPart *= 10;
               liExponentPart += strInput [iCharIndex] - '0';
            }
            else if (bFractionalPart) {
               uliFractionalPart *= 10;
               uliFractionalDivisor *= 10;
               uliFractionalPart += strInput[iCharIndex] - '0';
            }
            else {
               uliIntegerPart *= 10;
               uliIntegerPart += strInput[iCharIndex] - '0';
            }
         }
         else if (strInput[iCharIndex] == '-') {
            if (bExponentPart) {
               iExponentSign = -1;
            }
            else {
               iMantissaSign = -1;
            }
         }
         else if (strInput[iCharIndex] == ' ') {
            // Waiting for next component.
            waiting = true;
         }
         else if (strInput[iCharIndex] == '.') {
            bFractionalPart = true;
         }
         else if ((strInput[iCharIndex] == 'e') || (strInput[iCharIndex] == 'E')) {
            bExponentPart = true;
         }
         else bRetVal = false;
      }
      // Assign last component
      if (bRetVal && (iCurrentComponent != iNumComponents)) {
         liExponentPart *= iExponentSign;
         m_pComponents[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
      }
      if (iCurrentComponent != iNumComponents-1) bRetVal = false;
      if (used != NULL) {
         *used = iCharIndex;
      }
      return bRetVal;
   } //ParseString(const char* strInput, int& used)
	
   char* CVector2D::ToString(int precision) const {
      // Allocate output buffer space and initialise it to a null string.
      char* output = (char*)malloc(484);
      output[0] = 0;
      // Create each string and concatenate it onto the output string.
      for (int i=0; i<2; i++) {
         CScalar sComponent(m_pComponents[i]);
         char* str = sComponent.ToString(precision);
         strcat(output,str);
         free(str);
         if (i < 1) strcat(output," ");
      }
      int length = strlen(output) + 1;
      output = (char*)realloc(output,length);
      return output;
   } //ToString(int precision) const

}

