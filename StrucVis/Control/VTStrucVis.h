//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Design"
//! userlevel =  Normal
//! file      = "Control/VTStrucVis.h"
//! author    = "Warren Moore"
//! date      = "5/3/2002"
//! rcsid     = "$Id: VTStrucVis.h,v 1.7 2002/04/25 15:44:05 vap-warren Exp $"

#ifndef __VTSTRUCTVIS__
#define __VTSTRUCTVIS__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

//: ActiveX control application instance

class CVTStrucVisApp : public COleControlModule {
public:

	BOOL InitInstance();

	int ExitInstance();

};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS__
