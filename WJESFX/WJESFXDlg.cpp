//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// WJESFXDlg.cpp - 11/07/2000 - Warren Moore
//	Main dialog box
//
// $Id: WJESFXDlg.cpp,v 1.3 2000/07/11 19:20:25 waz Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "WJESFXDlg.h"
#include "AboutDlg.h"

#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////
// CWJESFXDlg

CWJESFXDlg::CWJESFXDlg(CWnd* pParent) : CDialog(CWJESFXDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CWJESFXDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
} // Constructor

void CWJESFXDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWJESFXDlg)
	//}}AFX_DATA_MAP
} // DoDataExchange

BEGIN_MESSAGE_MAP(CWJESFXDlg, CDialog)
	//{{AFX_MSG_MAP(CWJESFXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CWJESFXDlg::OnInitDialog() {
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
	
	// Set the correct game text
	CString strTemp;
	strTemp.Format("'%s' Avatar Installer", GAME_NAME);
	SetDlgItemText(IDC_MAIN_BOX, strTemp);
	strTemp.Format("Click Install to install your avatar into '%s'", GAME_NAME);
	SetDlgItemText(IDC_MAIN_TEXT, strTemp);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
} // OnInitDialog

void CWJESFXDlg::OnAbout() {
	CAboutDialog dlgAbout;
	dlgAbout.DoModal();
} // OnAbout

void CWJESFXDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
} // OnSysCommand

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWJESFXDlg::OnPaint() {
	if (IsIconic()) {
		CPaintDC dc(this);

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

HCURSOR CWJESFXDlg::OnQueryDragIcon() {
	return (HCURSOR) m_hIcon;
} // OnQueryDragIcon


void CWJESFXDlg::OnOK() {
	CProgressDlg oProgressDlg;

	((CWJESFXApp*)AfxGetApp())->GAME_INSTALL(oProgressDlg.Create() ? &oProgressDlg : NULL);
	
	CDialog::OnOK();
}
