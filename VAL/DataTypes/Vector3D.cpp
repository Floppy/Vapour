//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Vector3d.cpp - 18/05/2000 - James Smith
//	3D vector class implementation
//
// $Id: Vector3D.cpp,v 1.3 2000/07/31 17:40:21 waz Exp $
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

// VAL includes
#include "Vector3D.h"
#include "AxisRotation.h"
#include "Quaternion.h"

//////////////
// CVector3D

CVector3D::CVector3D(const double & dX, const double & dY, const double & dZ) {
	m_pdVector[0] = dX;
	m_pdVector[1] = dY;
	m_pdVector[2] = dZ;
} //CVector3D(const double & dX, const double & dY, const double & dZ)

CVector3D::CVector3D(const SPoint3D & pPoint) {
	m_pdVector[0] = pPoint.m_dComponents[0];
	m_pdVector[1] = pPoint.m_dComponents[1];
	m_pdVector[2] = pPoint.m_dComponents[2];
} //CVector3D(const SPoint3D & pPoint)

CVector3D::CVector3D(const CVector3D & oVec) {
	m_pdVector[0] = oVec.m_pdVector[0];
	m_pdVector[1] = oVec.m_pdVector[1];
	m_pdVector[2] = oVec.m_pdVector[2];
} //CVector3D(const CVector3D & vec)

CVector3D::CVector3D() {
	m_pdVector[0] = 0.0F;
	m_pdVector[1] = 0.0F;
	m_pdVector[2] = 0.0F;
} //CVector3D()

CVector3D::~CVector3D() {
} //~CVector3D()

//////////////////////////////////////////////////////////////////////
// Operator Functions
//////////////////////////////////////////////////////////////////////

CVector3D CVector3D::operator *(const double & dScalar) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] * dScalar;
	vecResult.m_pdVector[1] = m_pdVector[1] * dScalar;
	vecResult.m_pdVector[2] = m_pdVector[2] * dScalar;
	return vecResult;	
} //operator *(const double & dScalar) const

CVector3D CVector3D::operator *(const CVector3D & oVec) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] * oVec.m_pdVector[0];
	vecResult.m_pdVector[1] = m_pdVector[1] * oVec.m_pdVector[1];
	vecResult.m_pdVector[2] = m_pdVector[2] * oVec.m_pdVector[2];
	return vecResult;	
} //operator *(const CVector3D & oVec) const

CVector3D& CVector3D::operator *=(const double & dScalar) {
	m_pdVector[0] *= dScalar;
	m_pdVector[1] *= dScalar;
	m_pdVector[2] *= dScalar;
	return *this;	
} //operator *=(const double & dScalar)

CVector3D& CVector3D::operator *=(const CVector3D & oVec) {
	m_pdVector[0] *= oVec.m_pdVector[0];
	m_pdVector[1] *= oVec.m_pdVector[1];
	m_pdVector[2] *= oVec.m_pdVector[2];
	return *this;	
} //operator *=(const CVector3D & oVec)

CVector3D CVector3D::operator +(const SPoint3D & oPoint) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] + oPoint.m_dComponents[0];
	vecResult.m_pdVector[1] = m_pdVector[1] + oPoint.m_dComponents[1];
	vecResult.m_pdVector[2] = m_pdVector[2] + oPoint.m_dComponents[2];
	return vecResult;
} //operator +(const CVector3D & vec) const

CVector3D CVector3D::operator +(const CVector3D & oVec) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] + oVec.m_pdVector[0];
	vecResult.m_pdVector[1] = m_pdVector[1] + oVec.m_pdVector[1];
	vecResult.m_pdVector[2] = m_pdVector[2] + oVec.m_pdVector[2];
	return vecResult;
} //operator +(const CVector3D & vec) const

CVector3D& CVector3D::operator +=(const CVector3D & oVec) {
	m_pdVector[0] += oVec.m_pdVector[0];
	m_pdVector[1] += oVec.m_pdVector[1];
	m_pdVector[2] += oVec.m_pdVector[2];
	return *this;
} //operator +=(const CVector3D & oVec)

CVector3D CVector3D::operator -(const CVector3D & oVec) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] - oVec.m_pdVector[0];
	vecResult.m_pdVector[1] = m_pdVector[1] - oVec.m_pdVector[1];
	vecResult.m_pdVector[2] = m_pdVector[2] - oVec.m_pdVector[2];
	return vecResult;
} //operator -(const CVector3D & oVec) const

CVector3D& CVector3D::operator -=(const CVector3D & oVec) {
	m_pdVector[0] -= oVec.m_pdVector[0];
	m_pdVector[1] -= oVec.m_pdVector[1];
	m_pdVector[2] -= oVec.m_pdVector[2];
	return *this;
} //operator -=(const CVector3D & oVec)

CVector3D CVector3D::operator -(void) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = -m_pdVector[0];
	vecResult.m_pdVector[1] = -m_pdVector[1];
	vecResult.m_pdVector[2] = -m_pdVector[2];
	return vecResult;
} //operator -(void) const

CVector3D CVector3D::operator /(const double & dScalar) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] / dScalar;
	vecResult.m_pdVector[1] = m_pdVector[1] / dScalar;
	vecResult.m_pdVector[2] = m_pdVector[2] / dScalar;
	return vecResult;	
} //operator /(const double & dScalar) const

CVector3D CVector3D::operator /(const CVector3D & oVec) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[0] / oVec.m_pdVector[0];
	vecResult.m_pdVector[1] = m_pdVector[1] / oVec.m_pdVector[1];
	vecResult.m_pdVector[2] = m_pdVector[2] / oVec.m_pdVector[2];
	return vecResult;	
} //operator /(const CVector3D & oVec) const

CVector3D& CVector3D::operator /=(const double & dScalar) {
	m_pdVector[0] /= dScalar;
	m_pdVector[1] /= dScalar;
	m_pdVector[2] /= dScalar;
	return *this;	
} //operator /=(const double & dScalar)

CVector3D& CVector3D::operator /=(const CVector3D & oVec) {
	m_pdVector[0] /= oVec.m_pdVector[0];
	m_pdVector[1] /= oVec.m_pdVector[1];
	m_pdVector[2] /= oVec.m_pdVector[2];
	return *this;	
} //operator /=(const CVector3D & oVec)

bool CVector3D::operator ==(const CVector3D & oVec) const
{
	return ((m_pdVector[0] == oVec.m_pdVector[0]) && (m_pdVector[1] == oVec.m_pdVector[1]) && (m_pdVector[2] == oVec.m_pdVector[2]));
} //operator ==(const CVector3D & oVec) const

bool CVector3D::operator !=(const CVector3D & oVec) const
{
	return ((m_pdVector[0] != oVec.m_pdVector[0]) || (m_pdVector[1] != oVec.m_pdVector[1]) || (m_pdVector[2] != oVec.m_pdVector[2]));
} //operator !=(const CVector3D & oVec) const
//////////////////////////////////////////////////////////////////////
// Geometric Functions
//////////////////////////////////////////////////////////////////////

CVector3D CVector3D::Cross(const CVector3D & oVec) const {
	CVector3D vecResult;
	vecResult.m_pdVector[0] = m_pdVector[1] * oVec.m_pdVector[2] - m_pdVector[2] * oVec.m_pdVector[1];
	vecResult.m_pdVector[1] = m_pdVector[2] * oVec.m_pdVector[0] - m_pdVector[0] * oVec.m_pdVector[2];
	vecResult.m_pdVector[2] = m_pdVector[0] * oVec.m_pdVector[1] - m_pdVector[1] * oVec.m_pdVector[0];
	return vecResult;
} //Cross(const CVector3D & oVec) const

double CVector3D::Dot(const CVector3D & oVec) const {
	double dResult = 0.0F;
	dResult += m_pdVector[0] * oVec.m_pdVector[0];
	dResult += m_pdVector[1] * oVec.m_pdVector[1];
	dResult += m_pdVector[2] * oVec.m_pdVector[2];
	return dResult;
} //Dot(const CVector3D & oVec) const

double CVector3D::Length() const {
	return sqrt((m_pdVector[0] * m_pdVector[0]) + (m_pdVector[1] * m_pdVector[1]) + (m_pdVector[2] * m_pdVector[2]));
} //Length() const

CVector3D CVector3D::Normalise() const {
	CVector3D vecNormalised;
	double length = Length();
	if (length != 1) {
		vecNormalised = *this / length;
	}
	else {
		vecNormalised = *this;
	}
	return vecNormalised;
} //Normalise() const

CVector3D CVector3D::Rotate(const CAxisRotation & oRot) const {
	CVector3D vecResult;
	CQuaternion quatRotation(oRot);
	CQuaternion quatVector(0, *this);
	CQuaternion quatResult = quatRotation.Inverse() * quatVector * quatRotation;
	vecResult = quatResult.GetVector();
	return vecResult;
} //Rotate(const CAxisRotation & oRot) const

//////////////////////////////////////////////////////////////////////
// Input Functions
//////////////////////////////////////////////////////////////////////

void CVector3D::FromDouble(double dX, double dY, double dZ) {
	m_pdVector[0] = dX;
	m_pdVector[1] = dY;
	m_pdVector[2] = dZ;
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
			m_pdVector[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
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
		m_pdVector[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
	}
	if (iCurrentComponent != iNumComponents-1) bRetVal = false;
	if (used != NULL) {
		*used = iCharIndex;
	}
	return bRetVal;
} //ParseString(const char* strInput, int& used)

//////////////////////////////////////////////////////////////////////
// Output Functions
//////////////////////////////////////////////////////////////////////

/**
 * ToString
 * Variable precision string output.
 * Allocates memory internally. This memory must be freed by the user, using 
 * free(ret_val).
 */
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
		double num = m_pdVector[i];
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

void CVector3D::ToPoint3D(SPoint3D& sPoint) const {
	sPoint.m_dComponents[0] = m_pdVector[0];
	sPoint.m_dComponents[1] = m_pdVector[1];
	sPoint.m_dComponents[2] = m_pdVector[2];
	return;
} //ToPoint3D(SPoint3D& sPoint) const

void CVector3D::ToDouble(double & dX, double & dY, double & dZ) const {
	dX = m_pdVector[0];
	dY = m_pdVector[1];
	dZ = m_pdVector[2];
	return;
} //ToDouble(double & dX, double & dY, double & dZ) const

double CVector3D::X(void) const {
   return m_pdVector[0];
} //X(void) const

double CVector3D::Y(void) const {
   return m_pdVector[1];
} //Y(void) const

double CVector3D::Z(void) const {
   return m_pdVector[2];
} //Z(void) const
