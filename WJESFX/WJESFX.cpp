//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// WJESFX.cpp - 11/07/2000 - Warren Moore
//	Application and installation
//
// $Id: WJESFX.cpp,v 1.3 2000/07/11 19:20:24 waz Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "WJESFXDlg.h"
#include "Registry.h"
#include "FolderDialog.h"
#include <direct.h>
#include <errno.h>

#include "VALWin32.h"
#include "SFX.h"
#include "Wedgie.h"
#include "ProgressDlg.h"

//#===--- VALWin32 management object
CVAL *g_poVAL = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////
// CWJESFXApp

BEGIN_MESSAGE_MAP(CWJESFXApp, CWinApp)
	//{{AFX_MSG_MAP(CWJESFXApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CWJESFXApp::CWJESFXApp() {
} // Constructor

CWJESFXApp theApp;

BOOL CWJESFXApp::InitInstance() {

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Create the VALWin32 management object
	NEWBEGIN
	g_poVAL = (CVAL*) new CVALWin32;
	NEWEND("CWJESFXApp::InitInstance - CVALWin32 object")
	
	CWJESFXDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (g_poVAL) {
		delete g_poVAL;
		g_poVAL = NULL;
	}

	return FALSE;
} // InitInstance

void CWJESFXApp::InstallSims(CProgressDlg *poDlg) {
// Find the game directory
	CRegistry oReg;
	CString strSimPath = oReg.ReadString(LOCAL_MACHINE, "Software\\Maxis\\The Sims", "SIMS_DATA", "");
	if (strSimPath == "") {
		AfxMessageBox("Unable to find The Sims directory. Please select The Sims directory.", MB_OK);
		CFolderDialog oFolder;
		int iResult = oFolder.DoModal();
		if (iResult == IDOK) {
			strSimPath = oFolder.GetPathName();
		}
		else
			return;
	}
	// Create the GameData directory
	CString strTempDir;
	strTempDir.Format("%s\\GameData", strSimPath);
  int iResult = _mkdir(strTempDir);
  if ((iResult == -1) && (errno != EEXIST)) {
		AfxMessageBox("Unable to find or create the GameData directory", MB_OK|MB_ICONERROR);
		return;
	}
	strSimPath += "\\GameData\\";
	
	CSFX oSFX;
	// Has the end position been set?
	if (!oSFX.EndSet()) {
		// If not, set the end position then exit
		bool bSet = oSFX.SetEnd();
		AfxMessageBox((bSet ? "End position set correctly" : "Unable to set end position"), MB_OK);
	}
	else {
		// End position set to retreive data
		ifstream *poWJE = oSFX.GetWedgie();
		// Check the file stream is valid
		if (!poWJE) {
			AfxMessageBox("Unable to retrieve file handle", MB_OK|MB_ICONERROR);
			return;
		}
		// Create the decompressor and check the data has been supplied
		CWedgie oWedgie;
		WJERESULT eResult = oWedgie.Open((fstream*)poWJE, strSimPath);
		// Check the wedgie opened correctly
		if (eResult != WJE_OK) {
			if (poDlg)
				poDlg->DestroyWindow();
			AfxMessageBox("Missing model data", MB_OK|MB_ICONERROR);
			delete poWJE;
			return;
		}
		// Set up the dialog
		if (poDlg) {
			poDlg->ShowWindow(SW_SHOWNORMAL);
			poDlg->SetMax((int)oWedgie.Files());
			poDlg->UpdateWindow();
		}
		// Decompress the files
		unsigned int uCount = 0;
		while (uCount < oWedgie.Files()) {
			eResult = oWedgie.Extract(uCount);
			if (eResult != WJE_OK) {
				if (poDlg)
					poDlg->DestroyWindow();
				AfxMessageBox("Error decompressing data", MB_OK|MB_ICONERROR);
				oWedgie.Close();
				delete poWJE;
				return;
			}
			if (poDlg)
				poDlg->Step();
			uCount++;
		}
		oWedgie.Close();
		poWJE->close();
		delete poWJE;
		AfxMessageBox("Avatar installed successfully", MB_OK);
	}
	if (poDlg)
		poDlg->DestroyWindow();
} // InstallSims

void CWJESFXApp::InstallHL(CProgressDlg *poDlg) {
} // InstallHL

void CWJESFXApp::InstallUT(CProgressDlg *poDlg) {
} // InstallHL

