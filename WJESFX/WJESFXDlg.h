#ifndef _WJESFX_DLG_
#define _WJESFX_DLG_

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

#endif // _WJESFX_DLG_
