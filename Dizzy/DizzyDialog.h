#if !defined(AFX_DIZZYDIALOG_H__6C21C72F_7BD1_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_DIZZYDIALOG_H__6C21C72F_7BD1_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DizzyDialog.h : header file
//

#include "NumericEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CDizzyDialog dialog

class CDizzyDialog : public CDialog
{
// Construction
public:
	void SetSettings(int iPrecision, int iOnTop);
	void GetSettings(int* ipPrecision, int* ipOnTop);
	CDizzyDialog(CWnd* pParent = NULL);   // standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CDizzyDialog)
	enum { IDD = IDD_DIZZY_DIALOG };
	CString	m_strResult;
	CString	m_strInput;
	BOOL	m_bOnTop;
	int		m_iPrecision;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDizzyDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_icoCopy;
	HICON m_icoPaste;
	HICON m_icoUp;
	HICON m_icoDown;
	void RecalculateRotation();
	CNumericEdit m_InputBox;
	CFont* m_pFntCourier;

	// Generated message map functions
	//{{AFX_MSG(CDizzyDialog)
	afx_msg void OnAbout();
	afx_msg void OnAddButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnClearButton();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnCopyButton();
	afx_msg void OnUpButton();
	afx_msg void OnDownButton();
	afx_msg void OnOntop();
	afx_msg void OnPasteButton();
	afx_msg void OnDeltaposPrecisionSpin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusPrecision();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	DECLARE_MESSAGE_MAP()	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIZZYDIALOG_H__6C21C72F_7BD1_11D2_B744_80E203C10000__INCLUDED_)
