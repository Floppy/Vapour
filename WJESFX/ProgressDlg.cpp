//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// ProgressDlg.cpp - 11/07/2000 - Warren Moore
//	Installation progress dialog box
//
// $Id: ProgressDlg.cpp,v 1.1 2000/11/29 18:49:31 warren Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "ProgressDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CProgressDlg

CProgressDlg::CProgressDlg(CWnd* pParent)	: CDialog(CProgressDlg::IDD, pParent) {
	//{{AFX_DATA_INIT(CProgressDlg)
	//}}AFX_DATA_INIT
} // Constructor

void CProgressDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_oProgress);
	//}}AFX_DATA_MAP
} // DoDataExchange

BOOL CProgressDlg::Create() {
	return CDialog::Create(CProgressDlg::IDD);
} // Create

BOOL CProgressDlg::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// Set the progress bar
	m_oProgress.SetPos(0);
	m_oProgress.SetStep(1);
	m_oProgress.SetRange32(0, 100);

	// Set the correct game text
	CString strTemp;
	strTemp.Format("Installing '%s' Avatar Files", GAME_NAME);
	SetDlgItemText(IDC_PROGRESS_TEXT, strTemp);

	return TRUE;

} // OnInitDialog

void CProgressDlg::SetMax(int iMax) {
	// Set the progress bar range
	m_oProgress.SetRange32(0, iMax);
} // SetMax

void CProgressDlg::Step() {
	// Increment the progress bar
	m_oProgress.StepIt();
} // Step

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	//{{AFX_MSG_MAP(CProgressDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


