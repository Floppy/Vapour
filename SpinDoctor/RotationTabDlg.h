#if !defined(AFX_ROTATIONTABDLG_H__36CE6B20_4CB7_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_ROTATIONTABDLG_H__36CE6B20_4CB7_11D3_8CBF_00C0DF468F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RotationTabDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRotationTabDlg dialog

class CRotationTabDlg : public CDialog
{
// Construction
public:
	CRotationTabDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRotationTabDlg)
	enum { IDD = IDD_ROTATION_TAB };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRotationTabDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRotationTabDlg)
	afx_msg void OnAdd();
	afx_msg void OnSub();
	afx_msg void OnInvert();
	afx_msg void OnNorm();
	afx_msg void OnRotate();
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROTATIONTABDLG_H__36CE6B20_4CB7_11D3_8CBF_00C0DF468F89__INCLUDED_)
