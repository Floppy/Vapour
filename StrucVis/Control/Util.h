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
//! rcsid     = "$Id: Util.h,v 1.4 2002/04/04 11:01:38 vap-warren Exp $"

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