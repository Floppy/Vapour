//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVis.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVis.cpp,v 1.1 2002/03/19 21:04:54 vap-warren Exp $

#include "stdafx.h"
#include "VTStrucVis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVTStrucVisApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x8fc94f0f, 0xf823, 0x4ec7, { 0x91, 0xed, 0x63, 0xde, 0x9c, 0x48, 0xbe, 0x7c } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

//////////////////////////////////////////////////////
// CVTStrucVisApp::InitInstance - DLL initialization

BOOL CVTStrucVisApp::InitInstance() {
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit) {
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}

///////////////////////////////////////////////////
// CVTStrucVisApp::ExitInstance - DLL termination

int CVTStrucVisApp::ExitInstance() {
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}


////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void) {
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


///////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void) {
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
