// SpinDoctor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "SpinDoctorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorApp

BEGIN_MESSAGE_MAP(CSpinDoctorApp, CWinApp)
	//{{AFX_MSG_MAP(CSpinDoctorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorApp construction

CSpinDoctorApp::CSpinDoctorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpinDoctorApp object

CSpinDoctorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorApp initialization

BOOL CSpinDoctorApp::InitInstance()
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

	CSpinDoctorDlg dlg;
	m_pMainWnd = &dlg;

	dlg.SetSettings(GetProfileInt("SpinDoctor", "Precision", 2),GetProfileInt("SpinDoctor", "AngleType", 0),GetProfileInt("SpinDoctor", "OnTop", 0));

	dlg.DoModal();

	// Save precision to registry
	int iPrecision, iAngleType, iOnTop;
	dlg.GetSettings(&iPrecision, &iAngleType, &iOnTop);
	int success = WriteProfileInt("SpinDoctor", "Precision", iPrecision);
	success &= WriteProfileInt("SpinDoctor", "AngleType", iAngleType);
	success &= WriteProfileInt("SpinDoctor", "OnTop", iOnTop);
	if (!success) AfxMessageBox("Could not save settings!");

	// Since the dialog has been closed, return FALSE so that we exit the
	// application, rather than start the application's message pump.
	return FALSE;
}
