//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/Category.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: Category.h,v 1.2 2002/04/03 11:14:27 vap-james Exp $"

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

// Category support functions
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);

#endif // __VTSTRUCVIS_CATEGORY__