//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// ProgressDlg.h - 11/07/2000 - Warren Moore
//	About dialog box
//
// $Id: ProgressDlg.h,v 1.1 2000/11/29 18:49:31 warren Exp $
//

#ifndef _PROGRESSDIALOG_
#define _PROGRESSDIALOG_

#pragma once

/////////////////
// CProgressDlg

class CProgressDlg : public CDialog {
public:
	CProgressDlg(CWnd* pParent = NULL);

	BOOL Create();

	void SetMax(int iMax);
	void Step();

	//{{AFX_DATA(CProgressDlg)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_oProgress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgressDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProgressDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _PROGRESSDIALOG_
