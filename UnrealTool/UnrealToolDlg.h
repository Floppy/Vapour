// UnrealToolDlg.h : header file
//

#if !defined(AFX_UNREALTOOLDLG_H__C3EBAB42_D3F5_4932_BCA7_C6144483E10F__INCLUDED_)
#define AFX_UNREALTOOLDLG_H__C3EBAB42_D3F5_4932_BCA7_C6144483E10F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUnrealToolDlg dialog

class CUnrealToolDlg : public CDialog
{
// Construction
public:
	CUnrealToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUnrealToolDlg)
	enum { IDD = IDD_UNREALTOOL_DIALOG };
	CString	m_strByte0;
	CString	m_strByte1;
	CString	m_strByte2;
	CString	m_strByte3;
	CString	m_strByte4;
	CString	m_strIndex;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnrealToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUnrealToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCompact();
	afx_msg void OnUncompact();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNREALTOOLDLG_H__C3EBAB42_D3F5_4932_BCA7_C6144483E10F__INCLUDED_)
