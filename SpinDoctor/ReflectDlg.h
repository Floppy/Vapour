#if !defined(AFX_REFLECTDLG_H__C641F541_4C37_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_REFLECTDLG_H__C641F541_4C37_11D3_8CBF_00C0DF468F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReflectDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReflectDlg dialog

class CReflectDlg : public CDialog
{
// Construction
public:
	CReflectDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReflectDlg)
	enum { IDD = IDD_REFLECT };
	BOOL	m_bX;
	BOOL	m_bY;
	BOOL	m_bZ;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReflectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReflectDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFLECTDLG_H__C641F541_4C37_11D3_8CBF_00C0DF468F89__INCLUDED_)
