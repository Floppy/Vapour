//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVis.h
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVis.h,v 1.1 2002/03/19 21:04:55 vap-warren Exp $

#ifndef __VTSTRUCTVIS__
#define __VTSTRUCTVIS__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

///////////////////
// CVTStrucVisApp

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
