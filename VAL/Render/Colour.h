//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Colour.h - 01/12/2000 - Warren Moore
//	  Colour class declaration
//
// $Id: Colour.h,v 1.1 2000/12/02 07:29:59 warren Exp $
//

#ifndef _VAL_COLOUR_
#define _VAL_COLOUR_

#pragma once

////////////
// CColour

class CColour {
	//#===--- External Functions
public:

	//#===--- Constructors
	// Default constructor creates white colour, full alpha
	CColour();
	// First value sets all colour values and full alpha
	CColour(float fAll);
	// First value sets all colour values, second for alpha
	CColour(float fAll, float fAlpha);
	// Set individual colours and full alpha
	CColour(float fRed, float fGreen, float fBlue);
	// Set all values
	CColour(float fRed, float fGreen, float fBlue, float fAlpha);

	//#===--- Conversion Functions
	operator float*() const { return (float*)m_pfValue; }

	//#===--- Internal Functions
protected:

	//#===--- Internal Data
protected:

	float m_pfValue[4];					// Colour values RGBA

};

#endif // _VAL_COLOUR_
