// PeekDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Peek.h"
#include "PeekDialog.h"
#include "PeekAboutDialog.h"
#include "PeekHelpDialog.h"
#include "Quaternion.h"
#include "AxisRotation.h"
#include "Vector3D.h"
#include "MathConstants.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeekDialog dialog


CPeekDialog::CPeekDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPeekDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeekDialog)
	m_strLookAt = _T("");
	m_strPosition = _T("");
	m_strResult = _T("");
	m_strUpVector = _T("");
	//}}AFX_DATA_INIT
}


void CPeekDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeekDialog)
	DDX_Text(pDX, IDC_LOOK_BOX, m_strLookAt);
	DDX_Text(pDX, IDC_POS_BOX, m_strPosition);
	DDX_Text(pDX, IDC_RESULT_BOX, m_strOutput);
	DDX_Text(pDX, IDC_UP_BOX, m_strUpVector);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeekDialog, CDialog)
	//{{AFX_MSG_MAP(CPeekDialog)
	ON_BN_CLICKED(IDC_HELP_BUTTON, OnHelpButton)
	ON_BN_CLICKED(IDC_ABOUT_BUTTON, OnAboutButton)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_UPDATE_BUTTON, OnUpdateButton)
	ON_BN_CLICKED(IDC_COPY_BUTTON, OnCopyButton)
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
	ON_EN_KILLFOCUS(IDC_LOOK_BOX, OnKillfocusLookBox)
	ON_EN_KILLFOCUS(IDC_POS_BOX, OnKillfocusPosBox)
	ON_EN_KILLFOCUS(IDC_UP_BOX, OnKillfocusUpBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeekDialog message handlers

BOOL CPeekDialog::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_POS_BOX:
				pTTT->lpszText = "Enter camera position"; break;
			case IDC_LOOK_BOX:
				pTTT->lpszText = "Enter target position"; break;
			case IDC_UP_BOX:
				pTTT->lpszText = "Enter 'up' direction"; break;
			case IDC_UPDATE_BUTTON:
				pTTT->lpszText = "Update result"; break;
			case IDC_ABOUT_BUTTON:
				pTTT->lpszText = "About Peek"; break;
			case IDC_HELP_BUTTON:
				pTTT->lpszText = "Display help"; break;
			case IDOK:
				pTTT->lpszText = "Exit"; break;
			case IDC_COPY_BUTTON:
				pTTT->lpszText = "Copy result to clipboard"; break;
			case IDC_RESULT_BOX:
				pTTT->lpszText = "Result"; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
}

void CPeekDialog::OnHelpButton() {
	CPeekHelpDialog HelpDlg;
	HelpDlg.DoModal();	
}

void CPeekDialog::OnAboutButton() {
	CPeekAboutDialog AboutDlg;
	AboutDlg.DoModal();
}

int CPeekDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HICON pIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (pIcon) {
		SetIcon(pIcon, true);
	}

	return 0;
}

BOOL CPeekDialog::OnInitDialog() {
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

   // Subclass edit boxes
   m_cPosBox.SubclassDlgItem(IDC_POS_BOX,this);
   m_cLookBox.SubclassDlgItem(IDC_LOOK_BOX,this);
   m_cUpBox.SubclassDlgItem(IDC_UP_BOX,this);

   // Setup edit boxes
   m_cPosBox.SetOptions(3);
   m_cLookBox.SetOptions(3);
   m_cUpBox.SetOptions(3);

   // Set edit box fonts
	m_fntCourier = new CFont;
	m_fntCourier->CreateFont(8,0,0,0,400,FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN, "Courier");
	CEdit* pEditPos = (CEdit*) GetDlgItem(IDC_POS_BOX);
	pEditPos->SetFont(m_fntCourier);
	CEdit* pEditLook = (CEdit*) GetDlgItem(IDC_LOOK_BOX);
	pEditLook->SetFont(m_fntCourier);
	CEdit* pEditUp = (CEdit*) GetDlgItem(IDC_UP_BOX);
	pEditUp->SetFont(m_fntCourier);
	CEdit* pEditResult = (CEdit*) GetDlgItem(IDC_RESULT_BOX);
	pEditResult->SetFont(m_fntCourier);


   // Load icon for copy button
	VERIFY(m_CopyLogo.AutoLoad(IDC_COPY_BUTTON, this));
	
	return TRUE;
}

BOOL CPeekDialog::DestroyWindow() {
	delete m_fntCourier;
	
	return CDialog::DestroyWindow();
}

void CPeekDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUT) {
		CPeekAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CPeekDialog::RecalculateOrientation(CString & strResult) {

   CVector3D vecInitialLook(0.0, 0.0, -1.0);
	CVector3D vecInitialUp(0.0, 1.0, 0.0);
	CVector3D vecPosition;
	CVector3D vecTarget;
	CVector3D vecUp;

   // Parse input
   vecPosition.ParseString(m_strPosition);
   vecPosition.ParseString(m_strLookAt);
   vecUp.ParseString(m_strUpVector);
   
   // Check validity of up vector
   if (vecUp == vecTarget) {
		vecUp = vecInitialUp;
	}
	else {
		vecUp = vecUp.Normalise();
	}
	
	//Calculate Heading rotation
	//  Calculate Heading vector
	CVector3D vecHeading = (vecTarget - vecPosition);
	vecHeading = vecHeading.Normalise();
	//  Calculate Rotation angle
	double dHeadingAngle = acos(vecInitialLook.Dot(vecHeading));
	CAxisRotation rotNormalRotation;
	//Calculate Normal vector to initial and aim vectors
	CVector3D vecHeadingNormal = vecInitialLook.Cross(vecHeading);
	if (vecHeadingNormal.Length() < 1e-8) {
		if (dHeadingAngle < 1e-8) {
			CAxisRotation rotTempNormalRotation(0.0, 1.0, 0.0, 0.0);
			rotNormalRotation = rotTempNormalRotation;
		}
		else {
			CAxisRotation rotTempNormalRotation(0.0, 1.0, 0.0, 3.14);
			rotNormalRotation = rotTempNormalRotation;
		}
	}
	else {
		vecHeadingNormal = vecHeadingNormal.Normalise();
		//Construct Normal Rotation
		CAxisRotation rotTempNormalRotation(vecHeadingNormal, dHeadingAngle);
		rotNormalRotation = rotTempNormalRotation;
	}

	//Calculate Up Rotation
	//	Rotate inital up vector by heading rotation to get current up vector
	CVector3D vecCurrentUp = vecInitialUp.Rotate(rotNormalRotation);
	vecCurrentUp = vecCurrentUp.Normalise();
	//	Transform final up vector into a vector perpendicular to the heading vector
	CVector3D vecUpNormal = vecUp.Cross(vecHeading);
	vecUpNormal = vecUpNormal.Normalise();
	CAxisRotation oUpRotation(-vecUpNormal, V_PI / 2);
	CVector3D vecRequiredUp = vecHeading.Rotate(oUpRotation);
	vecRequiredUp = vecRequiredUp.Normalise();
	//	Calculate angle between current and desired up vectors.
	double dUpAngle = acos(vecCurrentUp.Dot(vecRequiredUp));
	CAxisRotation rotUpRotation;
	if (dUpAngle > 1e-8) {
		CVector3D vecUpAxis = vecCurrentUp.Cross(vecRequiredUp);
		vecUpAxis.Normalise();
		//Rotate by angle around the correct axis
		CAxisRotation rotTempUpRotation(vecUpAxis, dUpAngle);
		rotUpRotation = rotTempUpRotation;
	}
	else {
		CAxisRotation rotTempUpRotation(0.0, 1.0, 0.0, 0.0);
		rotUpRotation = rotTempUpRotation;
	}

	//Combine Normal Rotation and Up Rotation by multiplying quaternions
	CQuaternion quatNormalRotation(rotNormalRotation);
	CQuaternion quatUpRotation(rotUpRotation);
	CQuaternion quatResult = quatNormalRotation * quatUpRotation;
	//Transform resultant quaternion back to Axis angle.
	CAxisRotation rotResult(quatResult);
	//All done!
	strResult = rotResult.ToString(2);
   return;
}

void CPeekDialog::OnUpdateButton() {
	UpdateData(true);	
	RecalculateOrientation(m_strResult);
	FormatOutput(m_strOutput);
	UpdateData(false);
}

void CPeekDialog::OnCopyButton() 
{
	CEdit* pEditResult = (CEdit*) GetDlgItem(IDC_RESULT_BOX);
	pEditResult->SetSel(0,-1);
	pEditResult->Copy();	
}

void CPeekDialog::FormatOutput(CString &strOutput)
{
	CString viewpoint = "Viewpoint {";
	CString position = " position ";
	CString orientation = " orientation ";
	CString close = " }";
	strOutput = viewpoint + position + m_strPosition + orientation + m_strResult + close;
}

void CPeekDialog::ResetData()
{
	m_strPosition = "0 0 10";
	m_strLookAt = "0 0 0";
	m_strUpVector = "0 1 0";
	m_strResult = "0 1 0 0";
	FormatOutput(m_strOutput);
}

void CPeekDialog::OnKillfocusLookBox() 
{
	UpdateData(true);	
	RecalculateOrientation(m_strResult);
	FormatOutput(m_strOutput);
	UpdateData(false);
}

void CPeekDialog::OnKillfocusPosBox() 
{
	UpdateData(true);	
	RecalculateOrientation(m_strResult);
	FormatOutput(m_strOutput);
	UpdateData(false);
}

void CPeekDialog::OnKillfocusUpBox() 
{
	UpdateData(true);	
	RecalculateOrientation(m_strResult);
	FormatOutput(m_strOutput);
	UpdateData(false);
}
