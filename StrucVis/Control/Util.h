//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Util.h
// 27/03/2002 - Warren Moore
//
// $Id: Util.h,v 1.1 2002/04/02 22:34:05 vap-warren Exp $

#ifndef __VTSTRUCVIS_UTIL__
#define __VTSTRUCVIS_UTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int ParseFloat(CString oStr, const int iNum, float *pfArray);
// Parses a string for delimited float values, and returns them in the array
// at pfArray of iNum floats
// Delimiters are ',' and ';'
// Returns the number of floats parsed

#endif // __VTSTRUCVIS_UTIL__