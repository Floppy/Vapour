//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// InterVAL.cpp - 23/11/2000 - Warren Moore
//	  Main application
//
// $Id: InterVAL.cpp,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#include "StdAfx.h"
#include "InterVAL.h"

#include "VALWin32.h"

#include "MainFrame.h"
#include "ChildFrame.h"
#include "DocAvatar.h"
#include "ViewAvatar.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===--- The one and only CInterVALApp object

CInterVALApp theApp;

//#===--- The one and only CVAL object

CVAL *g_poVAL = NULL;

/////////////////
// CInterVALApp

CInterVALApp::CInterVALApp() {
} // Contructor

BOOL CInterVALApp::InitInstance() {
	// Create the VAL object
	NEWBEGIN
	g_poVAL = (CVALWin32*) new CVALWin32();
	NEWEND("CInterVALApp::InitInstance - VAL object\n");
	if (!g_poVAL)
		return FALSE;

	if (!AfxSocketInit()) {
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	SetRegistryKey(_T("vapourtech"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_AVATAR,
		RUNTIME_CLASS(CDocAvatar),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CViewAvatar));
	AddDocTemplate(pDocTemplate);

	// Create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dont open a new MDI window
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
} // InitInstance

int CInterVALApp::ExitInstance() {
	// Delete the VAL object
	if (g_poVAL) {
		delete g_poVAL;
		g_poVAL = NULL;
	}
	
	return CWinApp::ExitInstance();
} // ExitInstance

void CInterVALApp::OnAppAbout() {
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
} // OnAppAbout

//#===--- MFC Mapping

BEGIN_MESSAGE_MAP(CInterVALApp, CWinApp)
	//{{AFX_MSG_MAP(CInterVALApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

