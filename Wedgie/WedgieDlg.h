///////////////////////////////
// Wedgie - WJE file compiler
//
// Copyright 1999 Vapour Technology
// 02/12/99 - Warren Moore
// 
// WedgieDlg.h
//

#ifndef _WEDGIEDLG_
#define _WEDGIEDLG_

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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WEDGIEDLG_
