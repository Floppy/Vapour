//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/Category.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: Category.h,v 1.5 2002/04/04 19:55:02 vap-warren Exp $"

#ifndef __VTSTRUCVIS_CATEGORY__
#define __VTSTRUCVIS_CATEGORY__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For category support
#include <comcat.h>

// External vars
extern const CATID CATID_SafeForScripting;
extern const CATID CATID_SafeForInitializing;

//:-----
//: Category support functions

HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
//: Create a component category

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
//: Register the categrory as implemented by the control

HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
//: Unregister the categrory as implemented by the control

HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
//: Register the categrory as required by the control

HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
//: Unregister the categrory as required by the control

#endif // __VTSTRUCVIS_CATEGORY__