//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/Util.cpp"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: Util.cpp,v 1.3 2002/04/03 15:57:11 vap-warren Exp $"

#include "stdafx.h"

#include "Util.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////
// Utility Functions

int ParseFloat(CString oStr, const int iVal, float *pfArray) {
   // Check for an invalid float pointer or empty string
   if (!pfArray || (oStr == ""))
      return 0;
   // Start parsing
   int iCount = 0;
   CString oVal("");
   // Parse the string
   while ((iCount < iVal) && ((oVal = oStr.SpanIncluding(" -0123456789.")) != "")) {
      // Get the float value
      pfArray[iCount] = static_cast<float>(atof(oVal));
      // Update the string
      oStr = oStr.Right(oStr.GetLength() - oVal.GetLength());
      CString oSpace = oStr.SpanIncluding(" ,;");
      oStr = oStr.Right(oStr.GetLength() - oSpace.GetLength());
      // Increase the count
      iCount++;
   }
   return iCount;
}