// CCFixDlg.h : header file
//

#if !defined(AFX_CCFIXDLG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_)
#define AFX_CCFIXDLG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_

#include "StaticLink.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCCFixDlg dialog

class CCCFixDlg : public CDialog
{
private:

   CStaticLink m_oMicrosoftLink;
	
// Construction
public:
	CCCFixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCCFixDlg)
	enum { IDD = IDD_CCFIX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCFixDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCCFixDlg)
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
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCFIXDLG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_)
