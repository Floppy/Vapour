#if !defined(AFX_SCALARTABDLG_H__0A86D1F4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_SCALARTABDLG_H__0A86D1F4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScalarTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScalarTabDlg dialog

class CScalarTabDlg : public CDialog
{
// Construction
public:
	CScalarTabDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScalarTabDlg)
	enum { IDD = IDD_SCALAR_TAB };
	BOOL	m_bInvert;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScalarTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScalarTabDlg)
	afx_msg void OnAdd();
	afx_msg void OnSubtract();
	afx_msg void OnMultiply();
	afx_msg void OnDivide();
	afx_msg void OnSine();
	afx_msg void OnCosine();
	afx_msg void OnTangent();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	afx_msg void OnConvert();
	afx_msg void OnPi();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCALARTABDLG_H__0A86D1F4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
