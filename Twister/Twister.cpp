// Twister.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Twister.h"
#include "TwisterDialog.h"

// VAL
#include "Val.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// VAL management object
CVAL *g_poVAL = NULL;

/////////////////////////////////////////////////////////////////////////////
// CTwisterApp

BEGIN_MESSAGE_MAP(CTwisterApp, CWinApp)
	//{{AFX_MSG_MAP(CTwisterApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwisterApp construction

CTwisterApp::CTwisterApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTwisterApp object

CTwisterApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTwisterApp initialization

BOOL CTwisterApp::InitInstance()
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

	CTwisterDialog dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}
