//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AxisRotation.cpp - 12/04/2000 - James Smith
//	Axis-Angle rotation implementation
//
// $Id: AxisRotation.cpp,v 1.1 2000/06/16 21:59:22 waz Exp $
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
#include "AxisRotation.h"
#include "Quaternion.h"
#include "Vector3D.h"

//////////////////
// CAxisRotation

// Constructor - creates an null rotation
CAxisRotation::CAxisRotation() {
   m_oAxis.FromDouble(0,1,0);
   m_dRotation = 0.0F;
} //CAxisRotation()


// Constructor - creates a rotation from double values
CAxisRotation::CAxisRotation(const double & dX, const double & dY, const double & dZ, const double & dR) {
	CVector3D oNewVec(dX, dY, dZ);
	m_oAxis = oNewVec;
	m_dRotation = dR;
} //CAxisRotation(const double & dX, const double & dY, const double & dZ, const double & dR)


// Constructor - creates a rotation from a vector and a double rotation value
CAxisRotation::CAxisRotation(const CVector3D & oVec, const double & dRot) {
	m_oAxis = oVec;
	m_dRotation = dRot;
} //CAxisRotation(const CVector3D & oVec, const double & dRot)


// Constructor - creates a rotation from a quaternion
CAxisRotation::CAxisRotation(const CQuaternion & oQuat) {
	double dHalfRotation = acos(oQuat.GetScalar());
	double dSinHalfRotation = sin(dHalfRotation);
	m_dRotation = 2.0F * dHalfRotation;
	if (abs(dSinHalfRotation < 1e-8)) {
		CVector3D temp(0, 1, 0);
		m_oAxis = temp;
	}
	else {
		m_oAxis = oQuat.GetVector() / dSinHalfRotation;
	}
} //CAxisRotation(const CQuaternion & quat)


// Constructor - creates a rotation from an Euler rotation
CAxisRotation::CAxisRotation(const CEulerRotation & oEuler) {
   CQuaternion oQuat(oEuler);
   double dHalfRotation = acos(oQuat.GetScalar());
	double dSinHalfRotation = sin(dHalfRotation);
	m_dRotation = 2.0F * dHalfRotation;
	if (abs(dSinHalfRotation < 1e-8)) {
		CVector3D temp(0, 1, 0);
		m_oAxis = temp;
	}
	else {
		m_oAxis = oQuat.GetVector() / dSinHalfRotation;
	}
} //CAxisRotation(const CEulerRotation & oEuler)

// Copy Constructor
CAxisRotation::CAxisRotation(const CAxisRotation & oRot) {
	m_oAxis = oRot.m_oAxis;
	m_dRotation = oRot.m_dRotation;
} //CAxisRotation(const CAxisRotation & rot)


///////////////////////////////////////////////////////////////////////
// Operator functions /////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Assignment
CAxisRotation& CAxisRotation::operator=(const CAxisRotation& oRot) {
	m_oAxis = oRot.m_oAxis;
	m_dRotation = oRot.m_dRotation;
	return *this;
} //operator=(const CAxisRotation& oRot)

// Equality
bool CAxisRotation::operator ==(const CAxisRotation& oRot) const {
   return ((m_dRotation == oRot.m_dRotation) && (m_oAxis == oRot.m_oAxis));
}

///////////////////////////////////////////////////////////////////////
// Geometric functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


// Multiplies two rotations together
CAxisRotation CAxisRotation::MergeInside(CAxisRotation & oRot) const{
	CQuaternion first(*this);
	CQuaternion second(oRot);
	CAxisRotation rotResult(first * second);
	return rotResult;
}


// Multiplies two rotations together - in the opposite order
CAxisRotation CAxisRotation::MergeOutside(CAxisRotation & oRot) const{
	CQuaternion first(oRot);
	CQuaternion second(*this);
	CAxisRotation rotResult(first * second);
	return rotResult;
}


// Normalises the vector of rotation
CAxisRotation CAxisRotation::Normalise() const {
	CAxisRotation rotResult(m_oAxis.Normalise(), m_dRotation);
	return rotResult;
} //Normalise() const


//////////////////////////////////////////////////////////////////////
// Input functions ///////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


// Sets the rotation from a set of double values.
void CAxisRotation::FromDouble(double dX, double dY, double dZ, double dR) {
   m_dRotation = dR;
   m_oAxis.FromDouble(dX, dY, dZ);
   return;
}

// Parses a string anc converts it into an axis rotation
// The string must be only numbers in the format <X> <Y> <Z> <R>
bool CAxisRotation::ParseString(const char* strInput, int* used) {
	double pdInputData[4];
	const int iNumComponents = 4;
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
			pdInputData[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
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
		pdInputData[iCurrentComponent] = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
	}
	if (iCurrentComponent != iNumComponents-1) bRetVal = false;
	if (used != NULL) {
		*used = iCharIndex;
	}
	m_oAxis.FromDouble(pdInputData[0],pdInputData[1],pdInputData[2]);
	m_dRotation = pdInputData[3];
	return bRetVal;
} //ParseString(const char* strInput, int& used)


///////////////////////////////////////////////////////////////////////
// Output functions ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Converts the rotation to a string with maximum precision specified by <precision>
char* CAxisRotation::ToString(int precision) const {
	// Check precision overrun
	// Maximum precision is 9 digits, becuase of overrun on unsigned long.
	if (precision > 9) precision = 9;
	// Allocate temporary buffer space.
	char* buffer = (char*)malloc(128);
	// Allocate output buffer space and initialise it to a null string.
	char* output = (char*)malloc(512);
	output[0] = 0;
	// Copy axis vector and rotation into local structure
	double data[4];
	m_oAxis.ToDouble(data[0], data[1], data[2]);
	data[3] = m_dRotation;
	// Create each string and concatenate it onto the output string.
	for (int i=0; i<4; i++) {
		double num = data[i];
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
		if (i < 3) strcat(output," ");
	}
	int length = strlen(output) + 1;
	output = (char*)realloc(output,length);
	free(buffer);
	return output;
} //ToString(int precision) const


// Converts the rotation to a set of double values
void CAxisRotation::ToDouble(double& dX, double& dY, double& dZ, double& dR) const {
	m_oAxis.ToDouble(dX, dY, dZ);
	dR = m_dRotation;
} //ToDouble(double& dX, double& dY, double& dZ, double& dR) const