//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/math/vector3d.cpp"
//! author 		= "James Smith"
//! date 		= "01/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: vector3d.cpp,v 1.5 2001/10/02 22:12:19 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

#include "vector3d.h"

// VALET includes
#include "axisrotation.h"
#include "quaternion.h"

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <stdio.h>

namespace VALET {

	CVector3D::CVector3D() {
	  m_pComponents[0] = 0.0F;
	  m_pComponents[1] = 0.0F;
	  m_pComponents[2] = 0.0F;
	} //CVector3D()
	
	CVector3D::CVector3D(const CVector3D & oVec) {
	  m_pComponents[0] = oVec.m_pComponents[0];
	  m_pComponents[1] = oVec.m_pComponents[1];
	  m_pComponents[2] = oVec.m_pComponents[2];
	} //CVector3D(const CVector3D & vec)
	
	CVector3D::CVector3D(const double dX, const double dY, const double dZ) {
	  m_pComponents[0] = dX;
	  m_pComponents[1] = dY;
	  m_pComponents[2] = dZ;
	} //CVector3D(const double dX, const double dY, const double dZ)
	
	CVector3D::~CVector3D() {
	} //~CVector3D()
	
	CVector3D CVector3D::operator *(const double dScalar) const {
	  return CVector3D(*this) *= dScalar;
	} //operator *(const double dScalar) const
	
	CVector3D CVector3D::operator *(const CVector3D & oVec) const {
	  return CVector3D(*this) *= oVec;
	} //operator *(const CVector3D & oVec) const
	
	CVector3D& CVector3D::operator *=(const double dScalar) {
	  m_pComponents[0] *= dScalar;
	  m_pComponents[1] *= dScalar;
	  m_pComponents[2] *= dScalar;
	  return *this;
	} //operator *=(const double dScalar)
	
	CVector3D& CVector3D::operator *=(const CVector3D & oVec) {
	  m_pComponents[0] *= oVec.m_pComponents[0];
	  m_pComponents[1] *= oVec.m_pComponents[1];
	  m_pComponents[2] *= oVec.m_pComponents[2];
	  return *this;
	} //operator *=(const CVector3D & oVec)
	
	CVector3D CVector3D::operator /(const double dScalar) const {
	  return CVector3D(*this) /= dScalar;
	} //operator /(const double dScalar) const
	
	CVector3D CVector3D::operator /(const CVector3D & oVec) const {
	  return CVector3D(*this) /= oVec;
	} //operator /(const CVector3D & oVec) const
	
	CVector3D& CVector3D::operator /=(const double dScalar) {
	  m_pComponents[0] /= dScalar;
	  m_pComponents[1] /= dScalar;
	  m_pComponents[2] /= dScalar;
	  return *this;
	} //operator /=(const double dScalar)
	
	CVector3D& CVector3D::operator /=(const CVector3D & oVec) {
	  m_pComponents[0] /= oVec.m_pComponents[0];
	  m_pComponents[1] /= oVec.m_pComponents[1];
	  m_pComponents[2] /= oVec.m_pComponents[2];
	  return *this;	
	} //operator /=(const CVector3D & oVec)
	
	CVector3D CVector3D::operator +(const CVector3D & oVec) const {
	  return CVector3D(*this) += oVec;
	} //operator +(const CVector3D & vec) const
	
	CVector3D& CVector3D::operator +=(const CVector3D & oVec) {
	  m_pComponents[0] += oVec.m_pComponents[0];
	  m_pComponents[1] += oVec.m_pComponents[1];
	  m_pComponents[2] += oVec.m_pComponents[2];
	  return *this;
	} //operator +=(const CVector3D & oVec)
	
	CVector3D CVector3D::operator -(const CVector3D & oVec) const {
	  return CVector3D(*this) -= oVec;
	} //operator -(const CVector3D & oVec) const
	
	CVector3D& CVector3D::operator -=(const CVector3D & oVec) {
	  m_pComponents[0] -= oVec.m_pComponents[0];
	  m_pComponents[1] -= oVec.m_pComponents[1];
	  m_pComponents[2] -= oVec.m_pComponents[2];
	  return *this;
	} //operator -=(const CVector3D & oVec)
	
	CVector3D CVector3D::operator -(void) const {
	  return CVector3D(*this).Invert();
	} //operator -(void) const
	
	CVector3D& CVector3D::Invert() {
	  m_pComponents[0] = -m_pComponents[0];
	  m_pComponents[1] = -m_pComponents[1];
	  m_pComponents[2] = -m_pComponents[2];
	  return *this;
	} //Invert()
	
	bool CVector3D::operator ==(const CVector3D & oVec) const
	{
	  return ((m_pComponents[0] == oVec.m_pComponents[0]) && 
		  (m_pComponents[1] == oVec.m_pComponents[1]) && 
		  (m_pComponents[2] == oVec.m_pComponents[2]));
	} //operator ==(const CVector3D & oVec) const
	
	bool CVector3D::operator !=(const CVector3D & oVec) const
	{
	  return ((m_pComponents[0] != oVec.m_pComponents[0]) || 
		  (m_pComponents[1] != oVec.m_pComponents[1]) || 
		  (m_pComponents[2] != oVec.m_pComponents[2]));
	} //operator !=(const CVector3D & oVec) const
	
	double CVector3D::Dot(const CVector3D & oVec) const {
	  return 
	    m_pComponents[0] * oVec.m_pComponents[0] +
	    m_pComponents[1] * oVec.m_pComponents[1] +
	    m_pComponents[2] * oVec.m_pComponents[2];
	} //Dot(const CVector3D & oVec) const
	
	CVector3D CVector3D::Cross(const CVector3D & oVec) const {
	  double dX = m_pComponents[1] * oVec.m_pComponents[2] - m_pComponents[2] * oVec.m_pComponents[1];
	  double dY = m_pComponents[2] * oVec.m_pComponents[0] - m_pComponents[0] * oVec.m_pComponents[2];
	  double dZ = m_pComponents[0] * oVec.m_pComponents[1] - m_pComponents[1] * oVec.m_pComponents[0];
	  return CVector3D(dX,dY,dZ);
	} //Cross(const CVector3D & oVec) const
	
	
	double CVector3D::Length() const {
	  return sqrt((m_pComponents[0] * m_pComponents[0]) + 
		      (m_pComponents[1] * m_pComponents[1]) + 
		      (m_pComponents[2] * m_pComponents[2]));
	} //Length() const
	
	CVector3D& CVector3D::SetLength(double dLength) {  
	  *this /= Length();
	  *this *= dLength;
	  return *this;
	} //SetLength(double dLength)
	
	CVector3D& CVector3D::Normalise() {
	  return *this /= Length();
	} //Normalise()
	
	CVector3D CVector3D::Rotate(const CAxisRotation & oRot) const {
		CQuaternion quatRotation(oRot);
		CQuaternion quatVector(0, *this);
		CQuaternion quatResult = quatRotation.Inverse() * quatVector * quatRotation;
		return quatResult.Vector();
	} //Rotate(const CAxisRotation & oRot) const
	
	double CVector3D::X(void) const {
	  return m_pComponents[0];
	} //X(void) const
	
	double& CVector3D::X(void) {
	  return m_pComponents[0];
	} //X(void)
	
	double CVector3D::Y(void) const {
	  return m_pComponents[1];
	} //Y(void) const
	
	double& CVector3D::Y(void) {
	  return m_pComponents[1];
	} //Y(void)
	
	double CVector3D::Z(void) const {
	  return m_pComponents[2];
	} //Z(void) const
	
	double& CVector3D::Z(void) {
	  return m_pComponents[2];
	} //Z(void)
	
	void CVector3D::ToDouble(double& dX, double& dY, double& dZ) const {
	  dX = m_pComponents[0];
	  dY = m_pComponents[1];
	  dZ = m_pComponents[2];
	  return;
	} //ToDouble(double& dX, double& dY, double& dZ) const
	
	void CVector3D::FromDouble(double dX, double dY, double dZ) {
	  m_pComponents[0] = dX;
	  m_pComponents[1] = dY;
	  m_pComponents[2] = dZ;
	  return;
	} //FromDouble(double dX, double dY, double dZ)
	
	bool CVector3D::ParseString(const char* strInput, int* used) {
	  const int iNumComponents = 3;
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
	
	char* CVector3D::ToString(int precision) const {
	  // Check precision overrun
	  // Maximum precision is 9 digits, becuase of overrun on unsigned long.
	  if (precision > 9) precision = 9;
	  // Allocate temporary buffer space.
	  char* buffer = (char*)malloc(128);
	  // Allocate output buffer space and initialise it to a null string.
	  char* output = (char*)malloc(484);
	  output[0] = 0;
	  // Create each string and concatenate it onto the output string.
	  for (int i=0; i<3; i++) {
	    double num = m_pComponents[i];
	    char sign = (num<0) ? '-' : '+';
	    unsigned long whole_part = (int)floor(fabs(num));
	    unsigned long fractional_part;
	    double temp = fabs(num) - (double)whole_part;
	    temp *= (pow(10,precision));
	    double floor_val = floor(temp);
	    double ceil_val = ceil(temp);
	    if ((ceil_val - temp) <= 0.5) 
	      fractional_part = (int) ceil_val;
	    else
	      fractional_part = (int) floor_val;
	    if (precision == 0) {
	      whole_part += fractional_part;
	      fractional_part = 0;
	    }
	    int digits = 0;
	    while ((fractional_part != 0) && (fractional_part % 10 == 0)) {
	      fractional_part /= 10;
	      digits++;
	    }
	    double temp_fractional_part = fractional_part;
	    while (temp_fractional_part >= 1) {
	      temp_fractional_part /= 10;
	      digits++;
	    }
	    if (sign == '-') strcat(output,"-");
	    sprintf(buffer,"%lu",whole_part);
	    strcat(output,buffer);
	    if (fractional_part) {
	      strcat(output,".");
	      for (int i=0; i<precision-digits; i++) {
		strcat(output,"0");
	      }
	      sprintf(buffer,"%lu",fractional_part);
	      strcat(output,buffer);
	    }
	    if (i < 2) strcat(output," ");
	  }
	  int length = strlen(output) + 1;
	  output = (char*)realloc(output,length);
	  free(buffer);
	  return output;
	} //ToString(int precision) const

}

