#if !defined(AFX_PEEKHELPDIALOG_H__03D1E961_7FF1_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_PEEKHELPDIALOG_H__03D1E961_7FF1_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeekHelpDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPeekHelpDialog dialog

class CPeekHelpDialog : public CDialog
{
// Construction
public:
	CPeekHelpDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPeekHelpDialog)
	enum { IDD = IDD_HELP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeekHelpDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPeekHelpDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEEKHELPDIALOG_H__03D1E961_7FF1_11D2_B744_80E203C10000__INCLUDED_)
