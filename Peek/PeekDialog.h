#if !defined(AFX_PEEKDIALOG_H__C64FF46F_7FE6_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_PEEKDIALOG_H__C64FF46F_7FE6_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// PeekDialog.h : header file
//

#include "NumericEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CPeekDialog dialog

class CPeekDialog : public CDialog
{
// Construction
public:
	void ResetData(void);
	void FormatOutput(CString & strOutput);
	void RecalculateOrientation(CString & strResult);
	CFont * m_fntCourier;
	CPeekDialog(CWnd* pParent = NULL);   // standard constructor
	CString	m_strResult;
   CNumericEdit m_cPosBox;
   CNumericEdit m_cLookBox;
   CNumericEdit m_cUpBox;

// Dialog Data
	//{{AFX_DATA(CPeekDialog)
	enum { IDD = IDD_PEEK_DIALOG };
	CString	m_strLookAt;
	CString	m_strPosition;
	CString	m_strOutput;
	CString	m_strUpVector;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeekDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBitmapButton m_CopyLogo;

	// Generated message map functions
	//{{AFX_MSG(CPeekDialog)
	afx_msg void OnHelpButton();
	afx_msg void OnAboutButton();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateButton();
	afx_msg void OnCopyButton();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnKillfocusLookBox();
	afx_msg void OnKillfocusPosBox();
	afx_msg void OnKillfocusUpBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEEKDIALOG_H__C64FF46F_7FE6_11D2_B744_80E203C10000__INCLUDED_)
