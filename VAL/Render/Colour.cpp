//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Colour.cpp - 01/12/2000 - Warren Moore
//	  Colour class implementation
//
// $Id: Colour.cpp,v 1.1 2000/12/02 07:29:59 warren Exp $
//

#include "StdAfx.h"

#include "Colour.h"

#include "Image.h"
#include "Colour.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////
// CColour

//#===--- Constructors

CColour::CColour() {
	m_pfValue[0] = 1.0F; // R
	m_pfValue[1] = 1.0F; // G
	m_pfValue[2] = 1.0F; // B
	m_pfValue[3] = 1.0F; // A
} // Constructor

CColour::CColour(float fAll) {
	m_pfValue[0] = fAll; // R
	m_pfValue[1] = fAll; // G
	m_pfValue[2] = fAll; // B
	m_pfValue[3] = 1.0F; // A
} // Constructor

CColour::CColour(float fAll, float fAlpha) {
	m_pfValue[0] = fAll;   // R
	m_pfValue[1] = fAll;   // G
	m_pfValue[2] = fAll;   // B
	m_pfValue[3] = fAlpha; // A
} // Constructor

CColour::CColour(float fRed, float fGreen, float fBlue) {
	m_pfValue[0] = fRed;   // R
	m_pfValue[1] = fGreen; // G
	m_pfValue[2] = fBlue;  // B
	m_pfValue[3] = 1.0F;   // A
} // Constructor

CColour::CColour(float fRed, float fGreen, float fBlue, float fAlpha) {
	m_pfValue[0] = fRed;   // R
	m_pfValue[1] = fGreen; // G
	m_pfValue[2] = fBlue;  // B
	m_pfValue[3] = fAlpha; // A
} // Constructor
