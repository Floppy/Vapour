// Dizzy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Dizzy.h"
#include "DizzyDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDizzyApp

BEGIN_MESSAGE_MAP(CDizzyApp, CWinApp)
	//{{AFX_MSG_MAP(CDizzyApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDizzyApp construction

CDizzyApp::CDizzyApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDizzyApp object

CDizzyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDizzyApp initialization

BOOL CDizzyApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Set registry key
	SetRegistryKey("vapourtech");
	const char* pszAppName = "Dizzy";

	CDizzyDialog dlg;
	m_pMainWnd = &dlg;

	dlg.SetSettings(GetProfileInt(pszAppName, "Precision", 2),GetProfileInt(pszAppName, "OnTop", 0));

	dlg.DoModal();

	// Save precision to registry
	int iPrecision, iOnTop;
	dlg.GetSettings(&iPrecision, &iOnTop);
	int success = WriteProfileInt(pszAppName, "Precision", iPrecision);
	success &= WriteProfileInt(pszAppName, "OnTop", iOnTop);
	if (!success) AfxMessageBox("Could not save settings!");

	return FALSE;
}
