//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// WedgieApp.cpp - 02/12/1999 - Warren Moore
//	  Main application source
//
// $Id: WedgieApp.cpp,v 1.2 2000/12/03 18:30:04 warren Exp $
//

#include "stdafx.h"
#include "WedgieApp.h"
#include "WedgieDlg.h"

#include "VALWin32.h"
#include "CommandLine.h"

#include <iostream.h>
#include "Wedgie.h"

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

	// Write the banner
	/*
	HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE) {
		unsigned long ulCount = 0;
		char pcText[STR_SIZE] = "Wedgie v1.0 (";
		strcat(pcText, __DATE__);
		strcat(pcText, ")\n");
		WriteConsole(hConsole, pcText, strlen(pcText), &ulCount, NULL);
	}
	*/
	cout << "Wedgie Compiler " << WEDGIE_VERSION << " (" << __DATE__ << ")" << endl;
	cout << "Copyright 1999-2000 Vapour Technology Ltd." << endl << endl;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLine oCmdLine;
	ParseCommandLine(oCmdLine);
	// Don't create the dialog if input dir supplied on command line
	const char *pcInputDir = NULL;
	if (pcInputDir = oCmdLine.GetInputDir()) {
		// Create the wedgie name
		char pcWJEName[STR_SIZE] = "";
		if ((pcInputDir[0] != '\\') && (pcInputDir[1] != '.') && (pcInputDir[1] != ':')) {
			const char *pcAppDir = g_poVAL->GetAppDir();
			if (pcAppDir) {
				strcpy(pcWJEName, pcAppDir);
				// Add the trailing slash
				if (pcWJEName[strlen(pcWJEName) - 1] != '\\')
				strcat(pcWJEName, "\\");
			}
		}
		strcat(pcWJEName, pcInputDir);
		strcat(pcWJEName, ".wje");
		// Open the file
		fstream oFile;
		oFile.open(pcWJEName, ios::out|ios::binary|ios::trunc);
		if (!oFile.fail()) {
			CWedgie oWedgie;
			if (oWedgie.Open(&oFile, pcInputDir, true, false) == WJE_OK)
				cout << pcWJEName << " created containing " << oWedgie.Files() << " files" << endl;
			else 
				cout << "Unable to create " << pcWJEName << endl;
			oWedgie.Close();
		}
	}
	else {
		CWedgieDlg dlg;
		dlg.m_strDirName = strDirName;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK) {
			strDirName = dlg.m_strDirName;
			WriteProfileString("Application Settings", "Directory Name", strDirName);
		}
	}

	// Delete the VAL object
	delete g_poVAL;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
} // InitInstance

