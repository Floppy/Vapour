//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/math/scalar.cpp"
//! author 		= "James Smith"
//! date 		= "01/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: scalar.cpp,v 1.6 2001/10/28 14:55:55 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

#include "VALET/scalar.h"

// VALET includes
#include "VALET/log.h"

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <stdio.h>

namespace NVALET {
  
   CScalar::CScalar() :
      m_dScalar(0)
   {
      CLog("math","CScalar::Constructor (default)",LL_OBJECT);
   } //CScalar()
 
   CScalar::CScalar(const CScalar& oScl) :
      m_dScalar(oScl.m_dScalar) 
   {
      CLog("math","CScalar::Constructor (CScalar)",LL_OBJECT);
   } //CScalar(const CScalar & oScl)

   CScalar::CScalar(const double dScl) :
      m_dScalar(dScl)
   {
      CLog("math","CScalar::Constructor (double)",LL_OBJECT);
   } //CScalar(const double dScl)
  
   CScalar::~CScalar() {
      CLog("math","CScalar::Destructor",LL_OBJECT);
   } //~CScalar()
  
   CScalar& CScalar::operator=(const double dScl) {
      CLog("math","CScalar::operator=");
      m_dScalar = dScl;
      return *this;
   } //operator=(const double dScl)
  
   bool CScalar::ParseString(const char* pcInput, int* piUsed) {
      CLog("math","CScalar::ParseString");
      const int iNumComponents = 1;
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
            m_dScalar = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
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
         m_dScalar = (iMantissaSign * (ulIntegerPart + ((double)ulFractionalPart / (double)ulFractionalDivisor))) * pow(10,lExponentPart);
      }
      if (iCurrentComponent != iNumComponents-1) bRetVal = false;
      if (piUsed != NULL) {
         *piUsed = iCharIndex;
      }
      return bRetVal;
   } //ParseString(const char* pcInput, int& piUsed)
  
   char* CScalar::ToString(int iPrecision) const {
      CLog("math","CScalar::ToString");
      // Check precision overrun
      // Maximum precision is 9 digits, becuase of overrun on unsigned long.
      if (iPrecision > 9) iPrecision = 9;
      // Allocate temporary buffer space.
      char* pcBuffer = (char*)malloc(128);
      // Allocate output buffer space and initialise it to a null string.
      char* pcOutput = (char*)malloc(484);
      pcOutput[0] = 0;
      // Create each string and concatenate it onto the output string.
      double dNum = m_dScalar;
      char cSign = (dNum<0) ? '-' : '+';
      unsigned long lWholePart = (int)floor(fabs(dNum));
      unsigned long lFractionalPart;
      double dTemp = fabs(dNum) - (double)lWholePart;
      dTemp *= (pow(10,iPrecision));
      double dFloor = floor(dTemp);
      double dCeil = ceil(dTemp);
      if ((dCeil - dTemp) <= 0.5) 
         lFractionalPart = static_cast(long,dCeil);
      else
         lFractionalPart = static_cast(long,dFloor);
      if (iPrecision == 0) {
         lWholePart += lFractionalPart;
         lFractionalPart = 0;
      }
      int iDigits = 0;
      while ((lFractionalPart != 0) && (lFractionalPart % 10 == 0)) {
         lFractionalPart /= 10;
         iDigits++;
      }
      double dTempFractionalPart = lFractionalPart;
      while (dTempFractionalPart >= 1) {
         dTempFractionalPart /= 10;
         iDigits++;
      }
      if (cSign == '-') strcat(pcOutput,"-");
      sprintf(pcBuffer,"%lu",lWholePart);
      strcat(pcOutput,pcBuffer);
      if (lFractionalPart) {
         strcat(pcOutput,".");
         for (int i=0; i<iPrecision-iDigits; i++) {
            strcat(pcOutput,"0");
         }
         sprintf(pcBuffer,"%lu",lFractionalPart);
         strcat(pcOutput,pcBuffer);
      }
      int length = strlen(pcOutput) + 1;
      pcOutput = (char*)realloc(pcOutput,length);
      free(pcBuffer);
      return pcOutput;
   } //ToString(int iPrecision) const

   double CScalar::ToDouble(void) const {
      CLog("math","CScalar::ToDouble");
      return m_dScalar;
   } //ToDouble(void) const
  
}
