#if !defined(AFX_VECTORTABDLG_H__0A86D1F3_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_VECTORTABDLG_H__0A86D1F3_4534_11D3_8CBF_00C0DF468F89__INCLUDED_

#include "ReflectDlg.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VectorTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CVectorTabDlg dialog

class CVectorTabDlg : public CDialog
{
// Construction
public:
	CVectorTabDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVectorTabDlg)
	enum { IDD = IDD_VECTOR_TAB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVectorTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CReflectDlg m_dlgReflect;

	// Generated message map functions
	//{{AFX_MSG(CVectorTabDlg)
	afx_msg void OnAdd();
	afx_msg void OnSub();
	afx_msg void OnMultiply();
	afx_msg void OnDivide();
	afx_msg void OnDot();
	afx_msg void OnCross();
	afx_msg void OnAngle();
	afx_msg void OnNorm();
	afx_msg void OnReflect();
	afx_msg void OnLength();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VECTORTABDLG_H__0A86D1F3_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
