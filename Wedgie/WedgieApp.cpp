///////////////////////////////
// Wedgie - WJE file compiler
//
// Copyright 1999 Vapour Technology
// 02/12/99 - Warren Moore
// 
// Wedgie.cpp
//

#include "stdafx.h"
#include "WedgieApp.h"
#include "WedgieDlg.h"

#include "VALWin32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CVAL *g_poVAL = NULL;

///////////////
// CWedgieApp

BEGIN_MESSAGE_MAP(CWedgieApp, CWinApp)
	//{{AFX_MSG_MAP(CWedgieApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CWedgieApp::CWedgieApp() {
} // Constructor

///////////////////////////////////////
// The one and only CWedgieApp object

CWedgieApp theApp;

BOOL CWedgieApp::InitInstance() {

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

// Create the VAL management object
	NEWBEGIN
	g_poVAL = (CVAL*) new CVALWin32;
	NEWEND("CWedgieApp::InitInstance - VAL management object")
	if (!g_poVAL) {
		AfxMessageBox("Unable to create VAL object", MB_OK|MB_ICONERROR);
		return FALSE;
	}

// Set the registry key
	SetRegistryKey(REGISTRY_KEY);

// Get the last directory
	CString strDirName;
	strDirName = GetProfileString("Application Settings", "Directory Name", "C:\\");

	CWedgieDlg dlg;
	dlg.m_strDirName = strDirName;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK) {
		strDirName = dlg.m_strDirName;
		WriteProfileString("Application Settings", "Directory Name", strDirName);
	}
	else if (nResponse == IDCANCEL) {

	}

	// Delete the VAL object
	delete g_poVAL;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
} // InitInstance

