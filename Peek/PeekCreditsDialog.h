#if !defined(AFX_PEEKCREDITSDIALOG_H__C64FF472_7FE6_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_PEEKCREDITSDIALOG_H__C64FF472_7FE6_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeekCreditsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPeekCreditsDialog dialog

class CPeekCreditsDialog : public CDialog
{
// Construction
public:
	CPeekCreditsDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPeekCreditsDialog)
	enum { IDD = IDD_CREDITS_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeekCreditsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPeekCreditsDialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEEKCREDITSDIALOG_H__C64FF472_7FE6_11D2_B744_80E203C10000__INCLUDED_)
