// DoneDialog.h : header file
//

#if !defined(AFX_DONEDIALOG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_)
#define AFX_DONEDIALOG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_

#include "StaticLink.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDoneDialog dialog

class CDoneDialog : public CDialog
{
private:

   CStaticLink m_oMicrosoftLink;
	
// Construction
public:
	CDoneDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDoneDialog)
	enum { IDD = IDD_DONE_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDoneDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDoneDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DONEDIALOG_H__A3245A88_0786_42C8_93AA_2138126A3CA8__INCLUDED_)
