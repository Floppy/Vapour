// CCFixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CCFix.h"
#include "CCFixDlg.h"
#include "DoneDialog.h"
#include "AboutDialog.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCCFixDlg dialog

CCCFixDlg::CCCFixDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCCFixDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCCFixDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_CCFIX);
}

void CCCFixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCCFixDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCCFixDlg, CDialog)
	//{{AFX_MSG_MAP(CCCFixDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCCFixDlg message handlers

BOOL CCCFixDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    // Set up static link
    m_oMicrosoftLink.SubclassDlgItem(IDC_MICROSOFT_LINK,this);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCCFixDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCCFixDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCCFixDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCCFixDlg::OnAbout() 
{
	CAboutDialog dlgAbout;
	dlgAbout.DoModal();
}

void CCCFixDlg::OnOK() 
{
	// Open registry
	CRegistry oReg;
	
	// Fix the .cc assocation key
	oReg.OpenKey(CLASSES_ROOT, ".cc", false);
	oReg.WriteString("", "cppfile");
	oReg.CloseKey();

	// Fix the .hh assocation key
	oReg.OpenKey(CLASSES_ROOT, ".hh", false);
	oReg.WriteString("", "hppfile");
	oReg.CloseKey();

	// Find and open the devstudio root key
	char strVCPPRoot[256] = "";
	const char* strVCPPRoots[] = {
		"Software\\Microsoft\\Developer",
		"Software\\Microsoft\\DevStudio\\5.0",
		"Software\\Microsoft\\DevStudio\\6.0",
	};
	for (int k=0; k<3; k++) {
		if (oReg.OpenKey(CURRENT_USER, strVCPPRoots[k], true)) {

			// Loop through possible compilers
			const int iNumCompilers = 6;
			const char* strCompilerKeys[iNumCompilers] = {
				"\\Build System\\Components\\Platforms\\Win32 (ALPHA)\\Tools\\C/C++ Compiler for Alpha",
				"\\Build System\\Components\\Platforms\\Win32 (PowerPC)\\Tools\\C/C++ Compiler for PowerPC",
				"\\Build System\\Components\\Platforms\\Win32 (x86)\\Tools\\32-bit C/C++ Compiler for 80x86",
				"\\Build System\\Components\\Platforms\\Win32 (WCE x86em)\\Tools\\C++ Compiler for 80x86em (Emulation)",
				"\\Build System\\Components\\Platforms\\Win32 (WCE MIPS)\\Tools\\C/C++ Compiler Mips R4100",
				"\\Build System\\Components\\Platforms\\Win32 (WCE SH)\\Tools\\C/C++ Compiler for SH",
			};
			for (int i=0; i<iNumCompilers; i++) {
				char strKey[256] = "";
				strcpy(strKey,strVCPPRoots[k]);
				strcat(strKey,strCompilerKeys[i]);
				// Try to open each key
				if (oReg.OpenKey(CURRENT_USER, strKey, true)) {
					char strInputSpec[256] = "";
					// See if .cc is already in the Input_Spec
					oReg.ReadString("Input_Spec","",strInputSpec);
					oReg.CloseKey();
					if (strlen(strInputSpec)>0 && strstr(strInputSpec,"*.cc")==NULL) {
						// If not, put it on the end
						oReg.OpenKey(CURRENT_USER, strKey, false);
						strcat(strInputSpec,";*.cc");
						oReg.WriteString("Input_Spec",strInputSpec);
						oReg.CloseKey();
					}
				}
			}

			//Fix IDE highlighting
			char strTextEditorKey[256] = "";
			strcpy(strTextEditorKey,strVCPPRoots[k]);
			strcat(strTextEditorKey,"\\Text Editor\\Tabs/Language Settings\\C/C++");
			if (oReg.OpenKey(CURRENT_USER,strTextEditorKey,true)) {
				char strFileExtensions[256] = "";
				oReg.ReadString("FileExtensions","",strFileExtensions);
				oReg.CloseKey();
				if (strlen(strFileExtensions)>0 && strstr(strFileExtensions,"cc")==NULL ) {
					strcat(strFileExtensions,";cc");
				}
				if (strlen(strFileExtensions)>0 && strstr(strFileExtensions,"hh")==NULL ) {
					strcat(strFileExtensions,";hh");
				}
				oReg.OpenKey(CURRENT_USER,strTextEditorKey,false);
				oReg.WriteString("FileExtensions",strFileExtensions);
				oReg.CloseKey();
			}
			
			oReg.CloseKey();
		}
	} 



	CDoneDialog donedlg;
	donedlg.DoModal();
		
	CDialog::OnOK();
}
