//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// CreditsDlg.h - 11/07/2000 - Warren Moore
//	Credits dialog box
//
// $Id: CreditsDlg.h,v 1.2 2000/07/11 19:20:26 waz Exp $
//

#ifndef _CREDITSDIALOG_
#define _CREDITSDIALOG_

#pragma once

///////////////////
// CCreditsDialog

class CCreditsDialog : public CDialog {
public:
	CCreditsDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CCreditsDialog)
	enum { IDD = IDD_CREDITS_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCreditsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCreditsDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _CREDITSDIALOG_
