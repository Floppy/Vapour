// SpinDoctorDlg.h : header file
//

#if !defined(AFX_SPINDOCTORDLG_H__0A86D1E6_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_SPINDOCTORDLG_H__0A86D1E6_4534_11D3_8CBF_00C0DF468F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NumericEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorDlg dialog

class CSpinDoctorDlg : public CDialog
{
// Construction
public:
	void SetOptions(int iPrecision, int iAngleType, int iOnTop, int iSelectedTab);
	void GetOptions(int* piPrecision, int* piAngleType, int* piOnTop, int* piSelectedTab);
	CSpinDoctorDlg(CWnd* pParent = NULL);	// standard constructor

protected:
// Dialog Data
	//{{AFX_DATA(CSpinDoctorDlg)
	enum { IDD = IDD_SPINDOCTOR_DIALOG };
	CTabCtrl	m_tabControl;
	CString	m_strInputA;
	CString	m_strInputB;
	CString	m_strPrecision;
	CString	m_strResult;
	int m_iAngleType;
	BOOL	m_bOnTop;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpinDoctorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pfntCourier;
	HICON m_icoPaste;
	HICON m_icoCopy;
	HICON m_icoCopyA;
	HICON m_icoCopyB;
	HICON m_icoSwap;
	CNumericEdit m_ctrlInputB;
	CNumericEdit m_ctrlInputA;
	int m_iPrecision;
	HICON m_hIcon;
   int m_iCurrentTab;

	// Generated message map functions
	//{{AFX_MSG(CSpinDoctorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMovetoA();
	afx_msg void OnMovetoB();
	afx_msg void OnSwapAB();
	afx_msg void OnCopyResult();
	afx_msg void OnPasteB();
	afx_msg void OnPasteA();
	afx_msg void OnAboutButton();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusPrecision();
	afx_msg void OnAdd(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSub(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDot(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCross(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAngle(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNorm(WPARAM wParam, LPARAM lParam);
	afx_msg void OnReflect(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMult(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDiv(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSin(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCos(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTan(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInvert(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRotate(WPARAM wParam, LPARAM lParam);
	afx_msg void OnConvert(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPi(WPARAM wParam, LPARAM lParam);
	afx_msg void OnOntop();
	virtual void OnOK();
	//}}AFX_MSG
	BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINDOCTORDLG_H__0A86D1E6_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
