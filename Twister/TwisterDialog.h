#if !defined(AFX_TWISTERDIALOG_H__73BD3681_93A6_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_TWISTERDIALOG_H__73BD3681_93A6_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TwisterDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTwisterDialog dialog

class CTwisterDialog : public CDialog
{
// Construction
public:
	CTwisterDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTwisterDialog)
	enum { IDD = IDD_TWISTER_DIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwisterDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateRotation(CString & strResult);
	CFont* fntCourier;
	bool m_bRadians;
	CString	m_strX;
	CString	m_strY;
	CString	m_strZ;
	CString	m_strResult;
	CBitmapButton m_CopyLogo;

	// Generated message map functions
	//{{AFX_MSG(CTwisterDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnAboutButton();
	afx_msg void OnHelpButton();
	afx_msg void OnUpdateButton();
	afx_msg void OnCopyButton();
	afx_msg void OnDegreeRadio();
	afx_msg void OnRadiansRadio();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWISTERDIALOG_H__73BD3681_93A6_11D2_B744_80E203C10000__INCLUDED_)
