//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// WedgieDlg.cpp - 02/12/1999 - Warren Moore
//	  Main wedgie dialog box and about box
//
// $Id: WedgieDlg.cpp,v 1.3 2000/12/03 18:30:04 warren Exp $
//

#include "stdafx.h"
#include "WedgieApp.h"
#include "WedgieDlg.h"

#include "FolderDialog.h"
#include "Wedgie.h"

#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
// CAboutDlg

class CAboutDlg : public CDialog {
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
} // Constructor

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
} // DoDataExchange

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////
// CWedgieDlg

CWedgieDlg::CWedgieDlg(CWnd* pParent) : CDialog(CWedgieDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CWedgieDlg)
	m_strDirName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
} // Constructor

void CWedgieDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWedgieDlg)
	DDX_Text(pDX, IDC_DIRECTORY_NAME, m_strDirName);
	//}}AFX_DATA_MAP
} // DoDataExchange

BEGIN_MESSAGE_MAP(CWedgieDlg, CDialog)
	//{{AFX_MSG_MAP(CWedgieDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDD_START, OnStart)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////
// Message handlers

BOOL CWedgieDlg::OnInitDialog() {
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE  unless you set the focus to a control
} // OnInitDialog

int CWedgieDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the window text
	char pcWindowText[STR_SIZE] = "";
	sprintf(pcWindowText, "Wedgie Compiler %s (%s)", WEDGIE_VERSION, __DATE__);
	SetWindowText(pcWindowText);

	return 0;
} // OnCreate

void CWedgieDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
} // OnSysCommand

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWedgieDlg::OnPaint() {
	if (IsIconic()) {
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
	else {
		CDialog::OnPaint();
	}
} // OnPaint

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWedgieDlg::OnQueryDragIcon() {
	return (HCURSOR) m_hIcon;
} // OnQueryDragIcon

void CWedgieDlg::OnBrowse() {
	CFolderDialog oFolderDlg(m_strDirName);
	if (oFolderDlg.DoModal() == IDOK) {
		m_strDirName = oFolderDlg.GetPathName();
		UpdateData(FALSE);
	}
} // OnBrowse

void CWedgieDlg::OnStart() {
	// Create the wedgie name
	CString strWJEName;
	strWJEName.Format("%s.wje", m_strDirName);
	// Ope the file
	fstream oFile;
	oFile.open(strWJEName, ios::out|ios::binary|ios::trunc);
	if (oFile.fail())
		return;
	CWedgie oWedgie;
	oWedgie.Open(&oFile, m_strDirName, true, false);
	CString strMessage;
	strMessage.Format("%s created containing %d files", strWJEName, oWedgie.Files());
	AfxMessageBox(strMessage, MB_OK);
} // OnStart

