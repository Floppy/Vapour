// VectorTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "VectorTabDlg.h"
#include "SpinDoctorMessages.h"
#include "SpinDoctorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVectorTabDlg dialog


CVectorTabDlg::CVectorTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVectorTabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVectorTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVectorTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVectorTabDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVectorTabDlg, CDialog)
	//{{AFX_MSG_MAP(CVectorTabDlg)
	ON_BN_CLICKED(IDC_ADD_V, OnAdd)
	ON_BN_CLICKED(IDC_SUB_V, OnSub)
	ON_BN_CLICKED(IDC_MULTIPLY_V, OnMultiply)
	ON_BN_CLICKED(IDC_DIVIDE_V, OnDivide)
	ON_BN_CLICKED(IDC_DOT, OnDot)
	ON_BN_CLICKED(IDC_CROSS_V, OnCross)
	ON_BN_CLICKED(IDC_ANGLE, OnAngle)
	ON_BN_CLICKED(IDC_NORM, OnNorm)
	ON_BN_CLICKED(IDC_REFLECT, OnReflect)
	ON_BN_CLICKED(IDC_LENGTH, OnLength)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVectorTabDlg message handlers

BOOL CVectorTabDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_ADD_V:
				pTTT->lpszText = "Add two values."; break;
			case IDC_SUB_V:
				pTTT->lpszText = "Subtract two values."; break;
			case IDC_MULTIPLY_V:
				pTTT->lpszText = "Multiply two values."; break;
			case IDC_DIVIDE_V:
				pTTT->lpszText = "Divide two values."; break;
			case IDC_DOT:
				pTTT->lpszText = "Calculate the dot product of two vectors."; break;
			case IDC_CROSS_V:
				pTTT->lpszText = "Calculate the cross product of two vectors."; break;
			case IDC_ANGLE:
				pTTT->lpszText = "Measure the angle between two vectors."; break;
			case IDC_NORM:
				pTTT->lpszText = "Make the length of the vector or rotation axis in A equal to 1."; break;
			case IDC_REFLECT:
				pTTT->lpszText = "Reflect the vector in A in any of the three axes."; break;
			case IDC_LENGTH:
				pTTT->lpszText = "Calculate the length of the vector in A."; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
} //OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )

void CVectorTabDlg::OnAdd() {
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_ADD, 0, 0);
	return;
} //OnAdd()

void CVectorTabDlg::OnSub() {
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_SUB, 0, 0);
	return;
} //OnSub()

void CVectorTabDlg::OnMultiply() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_MULT, 0, 0);
	return;
}

void CVectorTabDlg::OnDivide() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_DIV, 0, 0);
	return;
}

void CVectorTabDlg::OnDot() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_DOT, 0, 0);
	return;
}

void CVectorTabDlg::OnCross() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_CROSS, 0, 0);
	return;
}

void CVectorTabDlg::OnAngle() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_ANGLE, 0, 0);
	return;
}

void CVectorTabDlg::OnNorm() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_NORM, 0, 0);
	return;
}

void CVectorTabDlg::OnReflect() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_REFLECT, 0, 0);
	return;
}

void CVectorTabDlg::OnLength() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_LENGTH, 0, 0);
	return;
}

BOOL CVectorTabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
