//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// AboutDlg.h - 11/07/2000 - Warren Moore
//	About dialog box
//
// $Id: AboutDlg.h,v 1.2 2000/07/11 19:20:26 waz Exp $
//

#ifndef _ABOUTDIALOG_
#define _ABOUTDIALOG_

#pragma once

#include "StaticLink.h"

/////////////////
// CAboutDialog

class CAboutDialog : public CDialog {
public:
	CAboutDialog(CWnd* pParent = NULL);

//#===--- Internal Data
private:
	CBitmapButton m_AboutLogo;

	CStaticLink m_oWebsiteURL;

//#===--- MFC Mapping
	//{{AFX_DATA(CAboutDialog)
	enum { IDD = IDD_ABOUT_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDialog)
	afx_msg void OnCreditButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _ABOUTDIALOG_
