//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// WedgieDlg.h - 02/12/1999 - Warren Moore
//	  Main application dialog
//
// $Id: WedgieDlg.h,v 1.2 2000/12/03 18:30:04 warren Exp $
//

#ifndef _WEDGIE_DIALOG_
#define _WEDGIE_DIALOG_

#pragma once

///////////////
// CWedgieDlg

class CWedgieDlg : public CDialog {
public:
	CWedgieDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CWedgieDlg)
	enum { IDD = IDD_WEDGIE_DIALOG };
	CString	m_strDirName;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CWedgieDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CWedgieDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBrowse();
	afx_msg void OnStart();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WEDGIE_DIALOG_
