//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// AboutDialog.cpp - 23/11/2000 - Warren Moore
//	  InterVAL About dialog box
//
// $Id: AboutDialog.cpp,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#include "StdAfx.h"
#include "InterVAL.h"

#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
// CAboutDlg

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD) {
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
} // Contructor

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
} // DoDataExchange

//#===--- MFC Mapping

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

