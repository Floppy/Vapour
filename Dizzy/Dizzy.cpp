// Dizzy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Dizzy.h"
#include "DizzyDialog.h"
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

BOOL CDizzyApp::InitInstance() {

   // Enable 3D Controls
#ifdef _AFXDLL
	Enable3dControls();
#else
	Enable3dControlsStatic();
#endif

	CRegistry oReg;
   int iPrecision, iOnTop;

	// Load registry settings
   oReg.OpenKey(CURRENT_USER, "Software\\vapourtech\\Dizzy", true);
   iPrecision = oReg.ReadInt("Precision", 2);
   iOnTop = oReg.ReadInt("OnTop", 0);
	oReg.CloseKey();
	
   // Create dialog
   CDizzyDialog dlg;
	m_pMainWnd = &dlg;

   // Set information in main dialog
	dlg.SetSettings(iPrecision,iOnTop);

   // Display dialog
	dlg.DoModal();

   // Retrieve settings from dialog
   dlg.GetSettings(&iPrecision, &iOnTop);

	// Save registry settings
   if (oReg.OpenKey(CURRENT_USER, "Software\\vapourtech\\Dizzy", false)) {
      oReg.WriteInt("Precision", iPrecision);
      oReg.WriteInt("OnTop", iOnTop);
	   oReg.CloseKey();
   }
   else {
      ::AfxMessageBox("Could not save registry settings!",MB_ICONERROR|MB_OK);
   }
	
   // Finised, so return false
	return FALSE;
}
