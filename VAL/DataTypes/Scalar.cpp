//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scalar.cpp - 18/05/2000 - James Smith
//	Scalar value class implementation
//
// $Id: Scalar.cpp,v 1.0 2000/08/18 12:03:32 waz Exp $
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
#include "Scalar.h"

//////////////
// CScalar

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScalar::CScalar(const double & dScl) {
	m_dScalar = dScl;
} //CScalar(const double & dScl)

CScalar::CScalar(const CScalar & oScl) {
	m_dScalar = oScl.m_dScalar;
} //CScalar(const CScalar & oScl)

CScalar::CScalar() {
	m_dScalar = 0;
} //CScalar()

CScalar::~CScalar() {
} //~CScalar()

//////////////////////////////////////////////////////////////////////
// Operator Functions
//////////////////////////////////////////////////////////////////////

CScalar::operator double() const {
	return m_dScalar;
} //operator double() const

CScalar::operator =(const double dScl) {
	m_dScalar = dScl;
} //operator =(const double dScl)

//////////////////////////////////////////////////////////////////////
// Input Functions
//////////////////////////////////////////////////////////////////////

bool CScalar::ParseString(const char* strInput, int* used) {
	const int iNumComponents = 1;
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
			m_dScalar = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
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
		m_dScalar = (iMantissaSign * (uliIntegerPart + ((double)uliFractionalPart / (double)uliFractionalDivisor))) * pow(10,liExponentPart);
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
char* CScalar::ToString(int precision) const {
	// Check precision overrun
	// Maximum precision is 9 digits, becuase of overrun on unsigned long.
	if (precision > 9) precision = 9;
	// Allocate temporary buffer space.
	char* buffer = (char*)malloc(128);
	// Allocate output buffer space and initialise it to a null string.
	char* output = (char*)malloc(484);
	output[0] = 0;
	// Create each string and concatenate it onto the output string.
	double num = m_dScalar;
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
	int length = strlen(output) + 1;
	output = (char*)realloc(output,length);
	free(buffer);
	return output;
} //ToString(int precision) const
