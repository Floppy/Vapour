//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// WJESFXDlg.h - 11/07/2000 - Warren Moore
//	Main dialog box
//
// $Id: WJESFXDlg.h,v 1.2 2000/07/11 19:20:25 waz Exp $
//

#ifndef _WJESFXDIALOG_
#define _WJESFXDIALOG_

#pragma once

///////////////
// CWJESFXDlg

class CWJESFXDlg : public CDialog {
public:
	CWJESFXDlg(CWnd* pParent = NULL);

	//{{AFX_DATA(CWJESFXDlg)
	enum { IDD = IDD_WJESFX_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CWJESFXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;

	//{{AFX_MSG(CWJESFXDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnAbout();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WJESFXDIALOG_
