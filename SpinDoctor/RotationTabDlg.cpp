// RotationTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "RotationTabDlg.h"
#include "SpinDoctorDlg.h"
#include "SpinDoctorMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRotationTabDlg dialog


CRotationTabDlg::CRotationTabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRotationTabDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRotationTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRotationTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRotationTabDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRotationTabDlg, CDialog)
	//{{AFX_MSG_MAP(CRotationTabDlg)
	ON_BN_CLICKED(IDC_ADDR, OnAdd)
	ON_BN_CLICKED(IDC_SUBR, OnSub)
	ON_BN_CLICKED(IDC_INVERT_ROTATION, OnInvert)
	ON_BN_CLICKED(IDC_NORM_R, OnNorm)
	ON_BN_CLICKED(IDC_ROTATE, OnRotate)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRotationTabDlg message handlers

BOOL CRotationTabDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_NORM_R:
				pTTT->lpszText = "Make the length of the vector or rotation axis in A equal to 1."; break;
			case IDC_INVERT_ROTATION:
				pTTT->lpszText = "Invert the rotation in A."; break;
			case IDC_ADDR:
				pTTT->lpszText = "Add two values."; break;
			case IDC_SUBR:
				pTTT->lpszText = "Subtract two values."; break;
			case IDC_ROTATE:
				pTTT->lpszText = "Apply a rotation to a vector."; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
} //OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )

void CRotationTabDlg::OnAdd() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_ADD, 0, 0);
	return;
}

void CRotationTabDlg::OnSub() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_SUB, 0, 0);
	return;
}

void CRotationTabDlg::OnInvert() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_INVERT, 0, 0);
	return;
}

void CRotationTabDlg::OnNorm() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_NORM, 0, 0);
	return;
}

void CRotationTabDlg::OnRotate() 
{
	((CSpinDoctorDlg*)AfxGetMainWnd())->PostMessage(SD_ROTATE, 0, 0);
	return;
}

BOOL CRotationTabDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips(TRUE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
