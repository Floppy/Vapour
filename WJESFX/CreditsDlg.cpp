//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// CreditsDlg.cpp - 11/07/2000 - Warren Moore
//	Credits dialog box
//
// $Id: CreditsDlg.cpp,v 1.2 2000/07/11 19:20:27 waz Exp $
//

#include "stdafx.h"
#include "WJESFX.h"

#include "CreditsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////
// CCreditsDialog

CCreditsDialog::CCreditsDialog(CWnd* pParent)	: CDialog(CCreditsDialog::IDD, pParent) {
	//{{AFX_DATA_INIT(CCreditsDialog)
	//}}AFX_DATA_INIT
} // Contructor

void CCreditsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreditsDialog)
	//}}AFX_DATA_MAP
} // DoDataExchange

BEGIN_MESSAGE_MAP(CCreditsDialog, CDialog)
	//{{AFX_MSG_MAP(CCreditsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCreditsDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	// Set the correct game text
	CString strTemp;
	strTemp.Format("AvatarMe to '%s' Avatar Convertor", GAME_NAME);
	SetDlgItemText(IDC_CREDITS_EXPORT, strTemp);
	strTemp.Format("'%s' Avatar Installer", GAME_NAME);
	SetDlgItemText(IDC_CREDITS_INSTALL, strTemp);

	return TRUE;
} // OnInitDialog
