// Peek.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Peek.h"
#include "PeekDialog.h"
#include "Vector3D.h"

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
// CPeekApp

BEGIN_MESSAGE_MAP(CPeekApp, CWinApp)
	//{{AFX_MSG_MAP(CPeekApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeekApp construction

CPeekApp::CPeekApp()
{
	// TODO: add construction code here,
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPeekApp object

CPeekApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPeekApp initialization

BOOL CPeekApp::InitInstance()
{

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPeekDialog dlg;
	m_pMainWnd = &dlg;
	dlg.ResetData();
	dlg.DoModal();
	return FALSE;
}
