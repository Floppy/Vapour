// SpinDoctor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "SpinDoctorDlg.h"
#include "Registry.h"

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

   // Enable 3D Controls
#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	CRegistry oReg;
   int iPrecision, iAngleType, iOnTop, iSelectedTab;

	// Load registry settings
   oReg.OpenKey(CURRENT_USER, "Software\\vapourtech\\SpinDoctor", true);
   iPrecision = oReg.ReadInt("Precision", 2);
   iAngleType = oReg.ReadInt("AngleType", 0);
   iOnTop = oReg.ReadInt("OnTop", 0);
   iSelectedTab = oReg.ReadInt("SelectedTab", 0);
	oReg.CloseKey();
	
   // Create dialog
	CSpinDoctorDlg dlg;
	m_pMainWnd = &dlg;

   // Set information in main dialog
	dlg.SetOptions(iPrecision,iAngleType,iOnTop,iSelectedTab);

   // Display dialog
	dlg.DoModal();

   // Retrieve settings from dialog
   dlg.GetOptions(&iPrecision,&iAngleType,&iOnTop,&iSelectedTab);

	// Save registry settings
   if (oReg.OpenKey(CURRENT_USER, "Software\\vapourtech\\SpinDoctor", false)) {
      oReg.WriteInt("Precision", iPrecision);
      oReg.WriteInt("AngleType", iAngleType);
      oReg.WriteInt("OnTop", iOnTop);
      oReg.WriteInt("SelectedTab",iSelectedTab);
	   oReg.CloseKey();
   }
   else {
      ::AfxMessageBox("Could not save registry settings!",MB_ICONERROR|MB_OK);
   }
	
   // Finised, so return false
	return FALSE;
}
