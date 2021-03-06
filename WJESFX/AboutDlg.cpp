//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// AboutDlg.cpp - 11/07/2000 - Warren Moore
//	About dialog box
//
// $Id: AboutDlg.cpp,v 1.2 2000/07/11 19:20:27 waz Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "AboutDlg.h"
#include "CreditsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CAboutDialog

CAboutDialog::CAboutDialog(CWnd* pParent)	: CDialog(CAboutDialog::IDD, pParent) {
	//{{AFX_DATA_INIT(CAboutDialog)
	//}}AFX_DATA_INIT
} // Constructor

void CAboutDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDialog)
	//}}AFX_DATA_MAP
} // DoDataExchange

BOOL CAboutDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// Set up bitmap button
	VERIFY(m_AboutLogo.AutoLoad(IDC_CREDIT_BUTTON, this));
	
	// Get client area size
	CRect oWndRect, oButtonRect;
	this->GetClientRect(oWndRect);
	// Get client relative button coords
	CWnd *pButton = (CWnd*)GetDlgItem(IDC_CREDIT_BUTTON);
	pButton->GetWindowRect(oButtonRect);
	this->ScreenToClient(oButtonRect);
	// Calculate the horizontal shift
	int iBWidth = oButtonRect.Width();
	int iBOffset = (oWndRect.Width() - iBWidth) / 2;
	oButtonRect.left = oWndRect.left + iBOffset;
	oButtonRect.right = oButtonRect.left + iBWidth;
	// Shift the button
	pButton->MoveWindow(oButtonRect, FALSE);
	
	// Set the website URL
	m_oWebsiteURL.SubclassDlgItem(IDC_VAPOUR_URL, this);

	// Set the correct game text
	CString strTemp;
	strTemp.Format("'%s' Avatar Converter and Installer", GAME_NAME);
	SetDlgItemText(IDC_ABOUT_TITLE, strTemp);
	
	return TRUE;
} // OnInitDialog

void CAboutDialog::OnCreditButton() {
	CCreditsDialog dlg;
	dlg.DoModal();
} // OnCreditButton

BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	ON_BN_CLICKED(IDC_CREDIT_BUTTON, OnCreditButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

