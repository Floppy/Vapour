// ScalarTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "ScalarTabDlg.h"
#include "SpinDoctorMessages.h"
#include "SpinDoctorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScalarTabDlg dialog


CScalarTabDlg::CScalarTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScalarTabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScalarTabDlg)
	m_bInvert = FALSE;
	//}}AFX_DATA_INIT
}


void CScalarTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScalarTabDlg)
	DDX_Check(pDX, IDC_INVERT, m_bInvert);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScalarTabDlg, CDialog)
	//{{AFX_MSG_MAP(CScalarTabDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_SUBTRACT, OnSubtract)
	ON_BN_CLICKED(IDC_MULTIPLY, OnMultiply)
	ON_BN_CLICKED(IDC_DIVIDE, OnDivide)
	ON_BN_CLICKED(IDC_SINE, OnSine)
	ON_BN_CLICKED(IDC_COSINE, OnCosine)
	ON_BN_CLICKED(IDC_TANGENT, OnTangent)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	ON_BN_CLICKED(IDC_CONVERT, OnConvert)
	ON_BN_CLICKED(IDC_PI, OnPi)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScalarTabDlg message handlers

BOOL CScalarTabDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_ADD:
				pTTT->lpszText = "Add two values."; break;
			case IDC_SUBTRACT:
				pTTT->lpszText = "Subtract two values."; break;
			case IDC_MULTIPLY:
				pTTT->lpszText = "Multiply two values."; break;
			case IDC_DIVIDE:
				pTTT->lpszText = "Divide two values."; break;
			case IDC_SINE:
				UpdateData(TRUE);
				if (m_bInvert) {
					pTTT->lpszText = "Calculate the inverse sine of A.";
					break;
				}
				else {
					pTTT->lpszText = "Calculate the sine of A.";
					break;
				}
			case IDC_COSINE:
				UpdateData(TRUE);
				if (m_bInvert) {
					pTTT->lpszText = "Calculate the inverse cosine of A.";
					break;
				}
				else {
					pTTT->lpszText = "Calculate the cosine of A.";
					break;
				}
			case IDC_TANGENT:
				UpdateData(TRUE);
				if (m_bInvert) {
					pTTT->lpszText = "Calculate the inverse tangent of A.";
					break;
				}
				else {
					pTTT->lpszText = "Calculate the tangent of A.";
					break;
				}
			case IDC_INVERT:
				pTTT->lpszText = "Toggle between normal or inverse trigonometric functions."; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
} //OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )

void CScalarTabDlg::OnAdd() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_ADD, 0, 0);
	return;
}

void CScalarTabDlg::OnSubtract() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_SUB, 0, 0);
	return;	
}

void CScalarTabDlg::OnMultiply() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_MULT, 0, 0);
	return;	
}

void CScalarTabDlg::OnDivide() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_DIV, 0, 0);
	return;
}

void CScalarTabDlg::OnSine() 
{
	UpdateData(TRUE);
	int invert = m_bInvert ? 1 : 0;
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_SIN, (WPARAM)invert, 0);
	return;
}

void CScalarTabDlg::OnCosine() 
{
	UpdateData(TRUE);
	int invert = m_bInvert ? 1 : 0;
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_COS, (WPARAM)invert, 0);
	return;
}

void CScalarTabDlg::OnTangent() 
{
	UpdateData(TRUE);
	int invert = m_bInvert ? 1 : 0;
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_TAN, (WPARAM)invert, 0);
	return;
}

void CScalarTabDlg::OnConvert() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_CONVERT, 0, 0);
	return;	
}

void CScalarTabDlg::OnPi() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_PI, 0, 0);
	return;	
}

BOOL CScalarTabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}