//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/Util.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: Util.h,v 1.5 2002/04/04 21:03:37 vap-warren Exp $"

#ifndef __VTSTRUCVIS_UTIL__
#define __VTSTRUCVIS_UTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int ParseFloat(CString oStr, const int iNum, float *pfArray);
//: Parses a string for comma or semi-colon delimited float values
//!param: oStr - Reference to string object
//!param: iNum - Number of float in the supplied array
//!param: pfArray - Pointer to array of floats, bounds specified by iNum
//!return: Number of float successfully parsed

#endif // __VTSTRUCVIS_UTIL__