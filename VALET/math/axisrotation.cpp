//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/math/axisrotation.cpp"
//! author 		= "James Smith"
//! date			= "02/10/2001"
//! lib 		   = libVALETmath
//! rcsid 		= "$Id: axisrotation.cpp,v 1.2 2001/10/02 22:15:30 vap-james Exp $"
//! userlevel	= Normal
//! docentry	= "VALET.Math.Geometry"

#include "axisrotation.h"

// VALET includes

// Standard includes
#include <math.h>
#include <string.h>
#include <alloc.h>
#include <ctype.h>
#include <stdio.h>

namespace VALET {

	CAxisRotation::CAxisRotation() :
		m_dAngle(0.0F),
	   m_oAxis(0,1,0)
	{
	} //CAxisRotation()	

	CAxisRotation::CAxisRotation(const CAxisRotation & oRot) :
	   m_dAngle(oRot.m_dAngle),
	   m_oAxis(oRot.m_oAxis)
	{
	} //CAxisRotation(const CAxisRotation & rot)

	CAxisRotation::CAxisRotation(const double dX, const double dY, const double dZ, const double dAngle) :
	   m_dAngle(dAngle),
	   m_oAxis(dX,dY,dZ)
	{
	} //CAxisRotation(const double dX, const double dY, const double dZ, const double dAngle)

	CAxisRotation::CAxisRotation(const CVector3D & oAxis, const double dAngle) :
		m_dAngle(dAngle),
		m_oAxis(oAxis)
	{
	} //CAxisRotation(const CVector3D & oAxis, const double dAngle)

	CAxisRotation::CAxisRotation(const CQuaternion & oQuat) {
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
	  CQuaternion oQuat(oRot);
	  *this = CAxisRotation(oQuat);
	} //CAxisRotation(const CEulerRotation & oRot)

	CAxisRotation& CAxisRotation::operator=(const CAxisRotation& oRot) {
		m_oAxis = oRot.m_oAxis;
		m_dAngle = oRot.m_dAngle;
		return *this;
	} //operator=(const CAxisRotation& oRot)

	bool CAxisRotation::operator ==(const CAxisRotation& oRot) const {
 	  return ((m_dAngle == oRot.m_dAngle) && (m_oAxis == oRot.m_oAxis));
	}

	CAxisRotation CAxisRotation::MergeInside(const CAxisRotation & oRot) const{
		CQuaternion first(*this);
		CQuaternion second(oRot);
		return CAxisRotation(first * second);
	}

	CAxisRotation CAxisRotation::MergeOutside(const CAxisRotation & oRot) const{
		return oRot.MergeInside(*this);
	}

	CAxisRotation& CAxisRotation::Normalise() {
		m_oAxis.Normalise();
		return *this;
	} //Normalise()

	void CAxisRotation::ToDouble(double& dX, double& dY, double& dZ, double& dAngle) const {
		m_oAxis.ToDouble(dX, dY, dZ);
		dAngle = m_dAngle;
	} //ToDouble(double& dX, double& dY, double& dZ, double& dAngle) const

	void CAxisRotation::FromDouble(double dX, double dY, double dZ, double dAngle) {
	   m_dAngle = dAngle;
	   m_oAxis.FromDouble(dX, dY, dZ);
		return;
	} //FromDouble(double dX, double dY, double dZ, double dAngle)

	void CAxisRotation::SetAngle(double dAngle) {
	   m_dAngle = dAngle;
	   return;
	} //SetAngle(double dAngle)

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
		m_dAngle = pdInputData[3];
		return bRetVal;
	} //ParseString(const char* strInput, int& used)

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
		data[3] = m_dAngle;
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

}
