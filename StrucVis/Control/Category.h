//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// Category.h
// 27/03/2002 - Warren Moore
//
// $Id: Category.h,v 1.1 2002/04/02 22:34:40 vap-warren Exp $

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