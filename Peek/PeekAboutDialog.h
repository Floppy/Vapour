#if !defined(AFX_PEEKABOUTDIALOG_H__C64FF471_7FE6_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_PEEKABOUTDIALOG_H__C64FF471_7FE6_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeekAboutDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPeekAboutDialog dialog

class CPeekAboutDialog : public CDialog
{
// Construction
public:
	CPeekAboutDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPeekAboutDialog)
	enum { IDD = IDD_ABOUT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeekAboutDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPeekAboutDialog)
	afx_msg void OnCreditButton();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmapButton m_AboutLogo;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEEKABOUTDIALOG_H__C64FF471_7FE6_11D2_B744_80E203C10000__INCLUDED_)
