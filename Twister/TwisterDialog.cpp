// TwisterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Twister.h"
#include "TwisterDialog.h"
#include "AboutDialog.h"
#include "HelpDialog.h"
#include "AxisRotation.h"
#include "Quaternion.h"
#include "MathConstants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTwisterDialog dialog


CTwisterDialog::CTwisterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTwisterDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTwisterDialog)
	m_strX = _T("");
	m_strY = _T("");
	m_strZ = _T("");
	m_strResult = _T("");
	//}}AFX_DATA_INIT
}


void CTwisterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTwisterDialog)
	DDX_Text(pDX, IDC_X_INPUT, m_strX);
	DDX_Text(pDX, IDC_Y_INPUT, m_strY);
	DDX_Text(pDX, IDC_Z_INPUT, m_strZ);
	DDX_Text(pDX, IDC_RESULT_BOX, m_strResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTwisterDialog, CDialog)
	//{{AFX_MSG_MAP(CTwisterDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_ABOUT_BUTTON, OnAboutButton)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDC_UPDATE_BUTTON, OnUpdateButton)
	ON_BN_CLICKED(IDC_COPY_BUTTON, OnCopyButton)
	ON_BN_CLICKED(IDC_DEGREE_RADIO, OnDegreeRadio)
	ON_BN_CLICKED(IDC_RADIANS_RADIO, OnRadiansRadio)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTwisterDialog message handlers

void CTwisterDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUT) {
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CTwisterDialog::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_UPDATE_BUTTON:
				pTTT->lpszText = "Update result"; break;
			case IDC_ABOUT_BUTTON:
				pTTT->lpszText = "About Twister"; break;
			case IDC_HELP_BUTTON:
				pTTT->lpszText = "Display help"; break;
			case IDOK:
				pTTT->lpszText = "Exit"; break;
			case IDC_COPY_BUTTON:
				pTTT->lpszText = "Copy result to clipboard"; break;
			case IDC_DEGREE_RADIO:
				pTTT->lpszText = "Enter rotations in degrees"; break;
			case IDC_RADIANS_RADIO:
				pTTT->lpszText = "Enter rotations in radians"; break;
			case IDC_RESULT_BOX:
				pTTT->lpszText = "Result"; break;
			case IDC_X_INPUT:
				pTTT->lpszText = "Enter x-axis rotation"; break;
			case IDC_Y_INPUT:
				pTTT->lpszText = "Enter y-axis rotation"; break;
			case IDC_Z_INPUT:
				pTTT->lpszText = "Enter z-axis rotation"; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
}

BOOL CTwisterDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	EnableToolTips(TRUE);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL) {
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUT, strAboutMenu);
		}
	}
	
	fntCourier = new CFont;
	fntCourier->CreateFont(8,0,0,0,400,FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN, "Courier");
	((CEdit*) GetDlgItem(IDC_X_INPUT))->SetFont(fntCourier);
	((CEdit*) GetDlgItem(IDC_Y_INPUT))->SetFont(fntCourier);
	((CEdit*) GetDlgItem(IDC_Z_INPUT))->SetFont(fntCourier);
	((CEdit*) GetDlgItem(IDC_RESULT_BOX))->SetFont(fntCourier);

   // Subclass edit boxes
   m_cXBox.SubclassDlgItem(IDC_X_INPUT,this);
   m_cYBox.SubclassDlgItem(IDC_Y_INPUT,this);
   m_cZBox.SubclassDlgItem(IDC_Z_INPUT,this);

   // Setup edit boxes
   m_cXBox.SetOptions(1);
   m_cXBox.SetOptions(1);
   m_cXBox.SetOptions(1);

	m_strX = "0";
	m_strY = "0";
	m_strZ = "0";
	((CButton*) GetDlgItem(IDC_RADIANS_RADIO))->SetCheck(1);
	m_bRadians = TRUE;
	UpdateRotation(m_strResult);
	UpdateData(FALSE);

	VERIFY(m_CopyLogo.AutoLoad(IDC_COPY_BUTTON, this));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CTwisterDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HICON pIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (pIcon) {
		SetIcon(pIcon, true);
	}
	
	return 0;
}

void CTwisterDialog::OnAboutButton() 
{
	CAboutDialog dlg;
	dlg.DoModal();	
}

void CTwisterDialog::OnHelpButton() 
{
	CHelpDialog dlg;
	dlg.DoModal();		
}

BOOL CTwisterDialog::DestroyWindow() 
{
	delete fntCourier;
	
	return CDialog::DestroyWindow();
}

void CTwisterDialog::OnUpdateButton() 
{
	UpdateData(TRUE);
	UpdateRotation(m_strResult);
	UpdateData(FALSE);

}

void CTwisterDialog::UpdateRotation(CString & strResult)
{
	bool error = FALSE;
	double x, y, z;
	x=y=z=0;
	x = atof(m_strX);
	y = atof(m_strY);
	z = atof(m_strZ);
	if(!m_bRadians) {
		x *= V_DEGTORAD;
		y *= V_DEGTORAD;
		z *= V_DEGTORAD;
	}
	CAxisRotation rotX(1, 0, 0, x);
	CAxisRotation rotY(0, 1, 0, y);
	CAxisRotation rotZ(0, 0, 1, z);
	CQuaternion quatX(rotX);
	CQuaternion quatY(rotY);
	CQuaternion quatZ(rotZ);
	CQuaternion quatResult = quatZ * quatY * quatX;
	CAxisRotation rotResult(quatResult);
	strResult = rotResult.ToString(2);
	return;
}

void CTwisterDialog::OnCopyButton() 
{
	CEdit* pEditResult = (CEdit*) GetDlgItem(IDC_RESULT_BOX);
	pEditResult->SetSel(0,-1);
	pEditResult->Copy();	
}

void CTwisterDialog::OnDegreeRadio() 
{
	if (m_bRadians) {
		int length;
		UpdateData(TRUE);
		double x = atof(m_strX);
		double y = atof(m_strY);
		double z = atof(m_strZ);
		x *= V_RADTODEG;
		y *= V_RADTODEG;
		z *= V_RADTODEG;
		m_strX.Format("%.3f", x);
		length = m_strX.GetLength();
		if ((m_strX.GetAt(length-1) == '0') && (m_strX.GetAt(length-2) == '0')&& (m_strX.GetAt(length-3) == '0'))
			m_strX = m_strX.Left(length-4);
		m_strY.Format("%.3f", y);
		length = m_strY.GetLength();
		if ((m_strY.GetAt(length-1) == '0') && (m_strY.GetAt(length-2) == '0')&& (m_strY.GetAt(length-3) == '0'))
			m_strY = m_strY.Left(length-4);
		m_strZ.Format("%.3f", z);
		length = m_strZ.GetLength();
		if ((m_strZ.GetAt(length-1) == '0') && (m_strZ.GetAt(length-2) == '0')&& (m_strZ.GetAt(length-3) == '0'))
			m_strZ = m_strZ.Left(length-4);
		m_bRadians = FALSE;
		UpdateData(FALSE);	
	}
}

void CTwisterDialog::OnRadiansRadio() 
{
	if (!m_bRadians) {
		int length;
		UpdateData(TRUE);
		double x = atof(m_strX);
		double y = atof(m_strY);
		double z = atof(m_strZ);
		x *= V_DEGTORAD;
		y *= V_DEGTORAD;
		z *= V_DEGTORAD;
		m_strX.Format("%.3f", x);
		length = m_strX.GetLength();
		if ((m_strX.GetAt(length-1) == '0') && (m_strX.GetAt(length-2) == '0')&& (m_strX.GetAt(length-3) == '0'))
			m_strX = m_strX.Left(length-4);
		m_strY.Format("%.3f", y);
		length = m_strY.GetLength();
		if ((m_strY.GetAt(length-1) == '0') && (m_strY.GetAt(length-2) == '0')&& (m_strY.GetAt(length-3) == '0'))
			m_strY = m_strY.Left(length-4);
		m_strZ.Format("%.3f", z);
		length = m_strZ.GetLength();
		if ((m_strZ.GetAt(length-1) == '0') && (m_strZ.GetAt(length-2) == '0')&& (m_strZ.GetAt(length-3) == '0'))
			m_strZ = m_strZ.Left(length-4);
		m_bRadians = TRUE;
		UpdateData(FALSE);	
	}
}
