// SpinDoctorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "SpinDoctor.h"
#include "AboutDialog.h"
#include "SpinDoctorDlg.h"
#include "VectorTabDlg.h"
#include "ScalarTabDlg.h"
#include "RotationTabDlg.h"
#include "SpinDoctorMessages.h"
#include "Vector3D.h"
#include "AxisRotation.h"
#include "Quaternion.h"
#include "MathConstants.h"
#include "ReflectDlg.h"
#include "Scalar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorDlg dialog

CSpinDoctorDlg::CSpinDoctorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpinDoctorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpinDoctorDlg)
	m_strInputA = _T("");
	m_strInputB = _T("");
	m_strPrecision = _T("");
	m_strResult = _T("");
	m_bOnTop = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon =    AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_icoPaste = AfxGetApp()->LoadIcon(IDI_PASTE);
	m_icoCopy =  AfxGetApp()->LoadIcon(IDI_COPY);
	m_icoCopyA = AfxGetApp()->LoadIcon(IDI_COPYA);
	m_icoCopyB = AfxGetApp()->LoadIcon(IDI_COPYB);
	m_icoSwap =  AfxGetApp()->LoadIcon(IDI_SWAP);
   m_iCurrentTab = 0;
   m_iPrecision = 2;
}

void CSpinDoctorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpinDoctorDlg)
	DDX_Control(pDX, IDC_TAB, m_tabControl);
	DDX_Text(pDX, IDC_INPUT_A, m_strInputA);
	DDX_Text(pDX, IDC_INPUT_B, m_strInputB);
	DDX_Text(pDX, IDC_PRECISION, m_strPrecision);
	DDX_Text(pDX, IDC_RESULT, m_strResult);
	DDX_Radio(pDX, IDC_RAD_RADIO, m_iAngleType);
	DDX_Check(pDX, IDC_ONTOP, m_bOnTop);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpinDoctorDlg, CDialog)
	//{{AFX_MSG_MAP(CSpinDoctorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MOVETO_A, OnMovetoA)
	ON_BN_CLICKED(IDC_MOVETO_B, OnMovetoB)
	ON_BN_CLICKED(IDC_SWAP_AB, OnSwapAB)
	ON_BN_CLICKED(IDC_COPY_RESULT, OnCopyResult)
	ON_BN_CLICKED(IDC_PASTE_B, OnPasteB)
	ON_BN_CLICKED(IDC_PASTE_A, OnPasteA)
	ON_BN_CLICKED(IDC_ABOUT_BUTTON, OnAboutButton)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnSelchangingTab)
	ON_EN_KILLFOCUS(IDC_PRECISION, OnKillfocusPrecision)
	ON_MESSAGE(SD_ADD, OnAdd)
	ON_MESSAGE(SD_SUB, OnSub)
	ON_MESSAGE(SD_DOT, OnDot)
	ON_MESSAGE(SD_CROSS, OnCross)
	ON_MESSAGE(SD_ANGLE, OnAngle)
	ON_MESSAGE(SD_NORM, OnNorm)
	ON_MESSAGE(SD_REFLECT, OnReflect)
	ON_MESSAGE(SD_MULT, OnMult)
	ON_MESSAGE(SD_DIV, OnDiv)
	ON_MESSAGE(SD_LENGTH, OnLength)
	ON_MESSAGE(SD_SIN, OnSin)
	ON_MESSAGE(SD_COS, OnCos)
	ON_MESSAGE(SD_TAN, OnTan)
	ON_MESSAGE(SD_INVERT, OnInvert)
	ON_MESSAGE(SD_ROTATE, OnRotate)
	ON_MESSAGE(SD_CONVERT, OnConvert)
	ON_MESSAGE(SD_PI, OnPi)
	ON_BN_CLICKED(IDC_ONTOP, OnOntop)
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorDlg message handlers

BOOL CSpinDoctorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Set up font for edit controls
	m_pfntCourier = new CFont;
	m_pfntCourier->CreateFont(8,0,0,0,400,FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN, "Courier");
	((CEdit*) GetDlgItem(IDC_INPUT_A))->SetFont(m_pfntCourier);
	((CEdit*) GetDlgItem(IDC_INPUT_B))->SetFont(m_pfntCourier);
	((CEdit*) GetDlgItem(IDC_RESULT))->SetFont(m_pfntCourier);
	
	// Set up numeric edit controls
	m_ctrlInputA.SubclassDlgItem(IDC_INPUT_A,this);
	m_ctrlInputA.SetOptions(4);
	m_ctrlInputB.SubclassDlgItem(IDC_INPUT_B,this);
	m_ctrlInputB.SetOptions(4);

	// Set up spin control
	((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetRange(0,9);
	((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetPos(m_iPrecision);
	m_strPrecision.Format("%d",m_iPrecision);
	
    // Set up the tab control...
    TC_ITEM tabItem;

    CVectorTabDlg *tabVector = new CVectorTabDlg(this);
    tabItem.mask       = TCIF_TEXT | TCIF_PARAM;
    tabItem.pszText    = "Vectors";
    tabItem.cchTextMax = strlen(tabItem.pszText);
    tabItem.lParam     = (LPARAM)tabVector;
    m_tabControl.InsertItem(0, &tabItem);

    CScalarTabDlg *tabScalar = new CScalarTabDlg(this);
    tabItem.mask       = TCIF_TEXT | TCIF_PARAM;
    tabItem.pszText    = "Scalars";
    tabItem.cchTextMax = strlen(tabItem.pszText);
    tabItem.lParam     = (LPARAM)tabScalar;
    m_tabControl.InsertItem(1, &tabItem);

    CRotationTabDlg *tabRotation = new CRotationTabDlg(this);
    tabItem.mask       = TCIF_TEXT | TCIF_PARAM;
    tabItem.pszText    = "Rotations";
    tabItem.cchTextMax = strlen(tabItem.pszText);
    tabItem.lParam     = (LPARAM)tabRotation;
    m_tabControl.InsertItem(2, &tabItem);

    RECT tabRect;
    m_tabControl.GetClientRect(&tabRect);
    m_tabControl.AdjustRect(FALSE, &tabRect);

    // Create tab dialogs
    tabVector->Create(CVectorTabDlg::IDD, &m_tabControl);
    tabScalar->Create(CScalarTabDlg::IDD, &m_tabControl);
    tabRotation->Create(CRotationTabDlg::IDD, &m_tabControl);

    // Set positions
    tabVector->SetWindowPos(NULL, tabRect.left, tabRect.top + 1, tabRect.right - tabRect.left, tabRect.bottom - tabRect.top, SWP_NOZORDER);
    tabScalar->SetWindowPos(NULL, tabRect.left, tabRect.top + 1, tabRect.right - tabRect.left, tabRect.bottom - tabRect.top, SWP_NOZORDER);
    tabRotation->SetWindowPos(NULL, tabRect.left, tabRect.top + 1, tabRect.right - tabRect.left, tabRect.bottom - tabRect.top, SWP_NOZORDER);

    // Hide all tabs
    tabVector->ShowWindow(SW_HIDE);
    tabScalar->ShowWindow(SW_HIDE);
    tabRotation->ShowWindow(SW_HIDE);

    // Display appropriate tab
    m_tabControl.SetCurSel(m_iCurrentTab);
    switch (m_iCurrentTab) {
    case 2:
       tabRotation->ShowWindow(SW_SHOW);
       break;
    case 1:
       tabScalar->ShowWindow(SW_SHOW);
       break;
    case 0:
    default:
       tabVector->ShowWindow(SW_SHOW);
       break;
    }

	// Set up icon buttons
	((CButton*) GetDlgItem(IDC_PASTE_A))->SetIcon(m_icoPaste);
	((CButton*) GetDlgItem(IDC_PASTE_B))->SetIcon(m_icoPaste);
	((CButton*) GetDlgItem(IDC_COPY_RESULT))->SetIcon(m_icoCopy);
	((CButton*) GetDlgItem(IDC_MOVETO_A))->SetIcon(m_icoCopyA);
	((CButton*) GetDlgItem(IDC_MOVETO_B))->SetIcon(m_icoCopyB);
	((CButton*) GetDlgItem(IDC_SWAP_AB))->SetIcon(m_icoSwap);

	//Enable tool tips
	EnableToolTips(TRUE);

	//Make window float on top or not
	if (m_bOnTop) {
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}

	return TRUE;
} //OnInitDialog()

void CSpinDoctorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDialog dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
	return;
} //OnSysCommand(UINT nID, LPARAM lParam)

void CSpinDoctorDlg::OnDestroy()
{
	//Destroy font
	delete m_pfntCourier;
	
	// Destroy tabs
	TC_ITEM tci;
    CWnd *pWnd;
    tci.mask = TCIF_PARAM;

    for (int i=0; i<m_tabControl.GetItemCount(); i++) {
		m_tabControl.GetItem(i, &tci);
		pWnd = (CWnd *)tci.lParam;
		pWnd->DestroyWindow();
		delete pWnd;
	}

	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
	return;
} //OnDestroy()

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSpinDoctorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	return;
} //OnPaint() 

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSpinDoctorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
} //OnQueryDragIcon()

void CSpinDoctorDlg::OnMovetoA() 
{
	UpdateData(TRUE);
	m_strInputA = m_strResult;
	UpdateData(FALSE);
	return;
} //OnMovetoA() 

void CSpinDoctorDlg::OnMovetoB() 
{
	UpdateData(TRUE);
	m_strInputB = m_strResult;
	UpdateData(FALSE);
	return;
} //OnMovetoB() 

void CSpinDoctorDlg::OnSwapAB() 
{
	UpdateData(TRUE);
	CString temp = m_strInputA;
	m_strInputA = m_strInputB;
	m_strInputB = temp;
	UpdateData(FALSE);
	return;
} //OnSwapAB() 

void CSpinDoctorDlg::OnCopyResult() 
{
	CEdit* pctrlResult = (CEdit*)GetDlgItem(IDC_RESULT);
	int start = 0;
	int end = 0;
	pctrlResult->GetSel(start, end);
	if (start == end) {
		pctrlResult->SetSel(0,-1,TRUE);
		pctrlResult->Copy();
		pctrlResult->SetSel(start,end);
	}
	else {
		pctrlResult->Copy();
	}
	return;
} //OnCopyResult() 

void CSpinDoctorDlg::OnPasteA() 
{
	((CEdit*)GetDlgItem(IDC_INPUT_A))->Paste();
	return;
} //OnPasteA() 

void CSpinDoctorDlg::OnPasteB() 
{
	((CEdit*)GetDlgItem(IDC_INPUT_B))->Paste();
	return;
} //OnPasteB() 

void CSpinDoctorDlg::OnAboutButton() 
{	
	CAboutDialog dlg;
	dlg.DoModal();
	return;
} //OnAboutButton() 

void CSpinDoctorDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_iCurrentTab = m_tabControl.GetCurSel();

   TC_ITEM tci;
   tci.mask = TCIF_PARAM;
   m_tabControl.GetItem(m_iCurrentTab, &tci);
   CWnd *pWnd = (CWnd *)tci.lParam;
   pWnd->ShowWindow(SW_SHOW);
	
	*pResult = 0;
	return;
} //OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult)

void CSpinDoctorDlg::OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int idx = m_tabControl.GetCurSel();
    TC_ITEM tci;
    tci.mask = TCIF_PARAM;
    m_tabControl.GetItem(idx, &tci);
    CWnd *pWnd = (CWnd *)tci.lParam;
    pWnd->ShowWindow(SW_HIDE);
	
	*pResult = 0;
	return;
} //OnSelchangingTab(NMHDR* pNMHDR, LRESULT* pResult)

void CSpinDoctorDlg::SetOptions(int iPrecision, int iAngleType, int iOnTop, int iSelectedTab) {
	m_iPrecision = iPrecision;
	m_iAngleType= iAngleType;
	m_bOnTop = iOnTop == 0 ? false : true;   
   m_iCurrentTab = iSelectedTab;
	return;
} //SetOptions(int iPrecision, int iAngleType, int iOnTop, int iSelectedTab)

void CSpinDoctorDlg::GetOptions(int* piPrecision, int* piAngleType, int* piOnTop, int* piSelectedTab) {
	*piPrecision = m_iPrecision;
	*piAngleType= m_iAngleType;
	*piOnTop = (int)m_bOnTop;
   *piSelectedTab = m_iCurrentTab;
	return;	
} //GetOptions(int* piPrecision, int* piAngleType, int* piOnTop, int* piSelectedTab)

void CSpinDoctorDlg::OnKillfocusPrecision() 
{
	//Retrieve precision setting
	m_iPrecision = ((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->GetPos();
	//If greater than 9, make it 9
	if (m_iPrecision > 9) {
		((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetPos(9);
		m_iPrecision = 9;
	}
	return;
} //OnKillfocusPrecision()

void CSpinDoctorDlg::OnAdd(WPARAM wParam, LPARAM lParam) {
	TRACE("addition\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = vecB.ParseString(m_strInputB);
	if (bReturnA && bReturnB) {
		CVector3D vecResult = vecA + vecB;
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		CAxisRotation rotA, rotB;
		bReturnA = rotA.ParseString(m_strInputA);
		bReturnB = rotB.ParseString(m_strInputB);
		if (bReturnA && bReturnB) {
			CAxisRotation rotResult = rotA.MergeOutside(rotB);
			char* pszResult = rotResult.ToString(m_iPrecision);
			m_strResult = pszResult;
			free(pszResult);
			UpdateData(FALSE);
		}
		else {
			CScalar sclA, sclB;
			bReturnA = sclA.ParseString(m_strInputA);
			bReturnB = sclB.ParseString(m_strInputB);
			if (bReturnA && bReturnB) {
				CScalar sclResult = sclA + sclB;
				char* pszResult = sclResult.ToString(m_iPrecision);
				m_strResult = pszResult;
				free(pszResult);
				UpdateData(FALSE);
			}
			else {
				::AfxMessageBox("A and B must both be either single scalar\nvalues, 3D vectors or 3D axis-angle rotations.",MB_OK,IDP_ADD);
			}			
		}
	}
	return;
} //OnAdd(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnSub(WPARAM wParam, LPARAM lParam) {
	TRACE("subtraction\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = vecB.ParseString(m_strInputB);
	if (bReturnA && bReturnB) {
		CVector3D vecResult = vecA - vecB;
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		CAxisRotation rotA, rotB;
		bReturnA = rotA.ParseString(m_strInputA);
		bReturnB = rotB.ParseString(m_strInputB);
		if (bReturnA && bReturnB) {
			double dX, dY, dZ, dR;
			rotB.ToDouble(dX, dY, dZ, dR);
			CAxisRotation rotInverse(dX,dY,dZ,-dR);
			CAxisRotation rotResult = rotA.MergeOutside(rotInverse);
			char* pszResult = rotResult.ToString(m_iPrecision);
			m_strResult = pszResult;
			free(pszResult);
			UpdateData(FALSE);
		}
		else {
			CScalar sclA, sclB;
			bReturnA = sclA.ParseString(m_strInputA);
			bReturnB = sclB.ParseString(m_strInputB);
			if (bReturnA && bReturnB) {
				CScalar sclResult = sclA - sclB;
				char* pszResult = sclResult.ToString(m_iPrecision);
				m_strResult = pszResult;
				free(pszResult);
				UpdateData(FALSE);
			}
			else {
				::AfxMessageBox("A and B must both be either single scalar\nvalues, 3D vectors or 3D axis-angle rotations.",MB_OK,IDP_SUB);
			}			
		}
	}
	return;
} //OnSub(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnDot(WPARAM wParam, LPARAM lParam) {
	TRACE("dot product\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = vecB.ParseString(m_strInputB);
	if (!(bReturnA && bReturnB)) ::AfxMessageBox("A and B must both be 3D vectors.",MB_OK,IDP_DOT);
	else {
		CScalar sclResult = vecA.Dot(vecB);
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnDot(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnCross(WPARAM wParam, LPARAM lParam) {
	TRACE("cross product\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = vecB.ParseString(m_strInputB);
	if (!(bReturnA && bReturnB)) ::AfxMessageBox("A and B must both be 3D vectors.",MB_OK,IDP_CROSS);
	else {
		CVector3D vecResult = vecA.Cross(vecB);
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnCross(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnAngle(WPARAM wParam, LPARAM lParam) {
	TRACE("angle\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = vecB.ParseString(m_strInputB);
	if (!(bReturnA && bReturnB)) ::AfxMessageBox("A and B must both be 3D vectors.",MB_OK,IDP_ANGLE);
	else {
		CScalar sclResult = vecA.Dot(vecB);
		sclResult = acos(sclResult/(vecA.Length()*vecB.Length()));
		if (m_iAngleType) sclResult = V_RADTODEG * sclResult;
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnAngle(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnNorm(WPARAM wParam, LPARAM lParam) {
	TRACE("normalise\n");
	UpdateData(TRUE);
	CVector3D vecA;
	bool bReturnA = vecA.ParseString(m_strInputA);
	if (bReturnA) {
		CVector3D vecResult = vecA.Normalise();
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		CAxisRotation rotA;
		bReturnA = rotA.ParseString(m_strInputA);
		if (bReturnA) {
			CAxisRotation rotResult = rotA.Normalise();
			char* pszResult = rotResult.ToString(m_iPrecision);
			m_strResult = pszResult;
			free(pszResult);
			UpdateData(FALSE);
		}
		else {
			::AfxMessageBox("A must be a 3D vector or a\n3D axis-angle rotation.",MB_OK,IDP_NORM);
		}
	}
	return;
} //OnNorm(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnReflect(WPARAM wParam, LPARAM lParam) {
	UpdateData(TRUE);
	static CReflectDlg dlg;
	CVector3D vecA;
	bool bReturnA = vecA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a 3D vector.",MB_OK,IDP_REFLECT);
	else {
		dlg.DoModal();
		double dX, dY, dZ;
		vecA.ToDouble(dX, dY, dZ);
		if (dlg.m_bX) {
			dX = -dX;
		}
		if (dlg.m_bY) {
			dY = -dY;
		}
		if (dlg.m_bZ) {
			dZ = -dZ;
		}
		CVector3D vecResult(dX, dY, dZ);
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnReflect(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnMult(WPARAM wParam, LPARAM lParam) {
	TRACE("multiplication\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	CScalar sclA, sclB;
	bool bReturnVecA = vecA.ParseString(m_strInputA);
	bool bReturnVecB = vecB.ParseString(m_strInputB);
	bool bReturnSclA = sclA.ParseString(m_strInputA);
	bool bReturnSclB = sclB.ParseString(m_strInputB);
	if (bReturnSclA && bReturnSclB) {
		CScalar sclResult = sclA * sclB;
		char *pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnSclA && bReturnVecB) {
		CVector3D vecResult = vecB * sclA;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnVecA && bReturnSclB) {
		CVector3D vecResult = vecA * sclB;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnVecA && bReturnVecB) {
		CVector3D vecResult = vecA * vecB;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		::AfxMessageBox("A and B must be either single scalar\nvalues, or 3D vectors.",MB_OK,IDP_MUL);
	}
	return;
} //OnMult(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnDiv(WPARAM wParam, LPARAM lParam) {
	TRACE("division\n");
	UpdateData(TRUE);
	CVector3D vecA, vecB;
	CScalar sclA, sclB;
	bool bReturnVecA = vecA.ParseString(m_strInputA);
	bool bReturnVecB = vecB.ParseString(m_strInputB);
	bool bReturnSclA = sclA.ParseString(m_strInputA);
	bool bReturnSclB = sclB.ParseString(m_strInputB);
	if (bReturnSclA && bReturnSclB) {
		CScalar sclResult = sclA / sclB;
		char *pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnSclA && bReturnVecB) {
		CVector3D vecResult = vecB / sclA;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnVecA && bReturnSclB) {
		CVector3D vecResult = vecA / sclB;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else if (bReturnVecA && bReturnVecB) {
		CVector3D vecResult = vecA / vecB;
		char *pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		::AfxMessageBox("A and B must be either single scalar\nvalues, or 3D vectors.",MB_OK,IDP_DIV);
	}
	return;
} //OnDiv(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnLength(WPARAM wParam, LPARAM lParam) {
	TRACE("length\n");
	UpdateData(TRUE);
	CVector3D vecA;
	bool bReturnA = vecA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a 3D vector.",MB_OK,IDP_LENGTH);
	else {
		CScalar sclResult = vecA.Length();
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnLength(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnSin(WPARAM wParam, LPARAM lParam) {
	if (wParam) TRACE("arc ");
	TRACE("sine\n");
	UpdateData(TRUE);
	CScalar sclA;
	bool bReturnA = sclA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a single scalar value.",MB_OK,IDP_SIN);
	else {
		if (m_iAngleType) sclA = V_DEGTORAD * sclA;
		CScalar sclResult;
		if (wParam) sclResult = asin(sclA);
		else sclResult = sin(sclA);
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnSin(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnCos(WPARAM wParam, LPARAM lParam) {
	if (wParam) TRACE("arc ");
	TRACE("cosine\n");
	UpdateData(TRUE);
	CScalar sclA;
	bool bReturnA = sclA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a single scalar value.",MB_OK,IDP_COS);
	else {
		if (m_iAngleType) sclA = V_DEGTORAD * sclA;
		CScalar sclResult;
		if (wParam) sclResult = acos(sclA);
		else sclResult = cos(sclA);
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnCos(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnTan(WPARAM wParam, LPARAM lParam) {
	if (wParam) TRACE("arc ");
	TRACE("tangent\n");
	UpdateData(TRUE);
	CScalar sclA;
	bool bReturnA = sclA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a single scalar value.",MB_OK,IDP_TAN);
	else {
		if (m_iAngleType) sclA = V_DEGTORAD * sclA;
		CScalar sclResult;
		if (wParam) sclResult = atan(sclA);
		else sclResult = tan(sclA);
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnTan(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnInvert(WPARAM wParam, LPARAM lParam) {
	TRACE("invert\n");
	UpdateData(TRUE);
	CAxisRotation rotA;
	bool bReturnA = rotA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a 3D axis-angle rotation.",MB_OK,IDP_INVERT);
	else {
		double dX, dY, dZ, dR;
		rotA.ToDouble(dX, dY, dZ, dR);
		CAxisRotation rotResult(dX,dY,dZ,-dR);
		char* pszResult = rotResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnInvert(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnRotate(WPARAM wParam, LPARAM lParam) {
	TRACE("rotation\n");
	UpdateData(TRUE);
	CVector3D vecA;
	CAxisRotation rotB;
	bool bReturnA = vecA.ParseString(m_strInputA);
	bool bReturnB = rotB.ParseString(m_strInputB);
	if (bReturnA && bReturnB) {
		CVector3D vecResult = vecA.Rotate(rotB);
		char* pszResult = vecResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	else {
		CAxisRotation rotA;
		CVector3D vecB;
		bReturnA = rotA.ParseString(m_strInputA);
		bReturnB = vecB.ParseString(m_strInputB);
		if (bReturnA && bReturnB) {
			CVector3D vecResult = vecB.Rotate(rotA);
			char* pszResult = vecResult.ToString(m_iPrecision);
			m_strResult = pszResult;
			free(pszResult);
			UpdateData(FALSE);
		}
		else {
			::AfxMessageBox("Either A or B must be a 3D vector and the\nother must be a 3D axis-angle rotation.",MB_OK,IDP_ROTATE);
		}
	}
	return;
} //OnRotate(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnConvert(WPARAM wParam, LPARAM lParam) {
	TRACE("convert\n");
	UpdateData(TRUE);
	CScalar sclA;
	bool bReturnA = sclA.ParseString(m_strInputA);
	if (!bReturnA) ::AfxMessageBox("A must be a single scalar value.",MB_OK,IDP_CONVERT);
	else {
		CScalar sclResult = m_iAngleType ? V_RADTODEG * sclA : V_DEGTORAD * sclA;
		char* pszResult = sclResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
		UpdateData(FALSE);
	}
	return;
} //OnConvert(WPARAM wParam, LPARAM lParam)

void CSpinDoctorDlg::OnPi(WPARAM wParam, LPARAM lParam) {
	TRACE("Pi\n");
	UpdateData(TRUE);
	CScalar sclResult = V_PI;
	char* pszResult = sclResult.ToString(m_iPrecision);
	m_strResult = pszResult;
	free(pszResult);
	UpdateData(FALSE);
	return;
} //OnPi(WPARAM wParam, LPARAM lParam)

BOOL CSpinDoctorDlg::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_INPUT_A:
				pTTT->lpszText = "Type the value for A here."; break;
			case IDC_INPUT_B:
				pTTT->lpszText = "Type the value for B here."; break;
			case IDC_RESULT:
				pTTT->lpszText = "The result of the last calculation."; break;
			case IDC_PASTE_A:
				pTTT->lpszText = "Paste the contents of the clipboard into A."; break;
			case IDC_PASTE_B:
				pTTT->lpszText = "Paste the contents of the clipboard into B."; break;
			case IDC_COPY_RESULT:
				pTTT->lpszText = "Copy the result onto the clipboard."; break;
			case IDC_MOVETO_A:
				pTTT->lpszText = "Copy the result into A."; break;
			case IDC_MOVETO_B:
				pTTT->lpszText = "Copy the result into B."; break;
			case IDC_SWAP_AB:
				pTTT->lpszText = "Swap the contents of A and B."; break;
			case IDC_TAB:
				pTTT->lpszText = "Select which set of operations to show."; break;
			case IDOK:
				pTTT->lpszText = "Exit SpinDoctor."; break;
			case IDC_ABOUT_BUTTON:
				pTTT->lpszText = "About SpinDoctor."; break;
			case ID_HELP:
				pTTT->lpszText = "Get help on using SpinDoctor."; break;
			case IDC_PRECISION:
				pTTT->lpszText = "The maximum number of figures to display after the decimal point."; break;
			case IDC_PRECISION_SPIN:
				pTTT->lpszText = "Changes the precision value."; break;
			case IDC_RAD_RADIO:
				pTTT->lpszText = "Use angles in radians."; break;
			case IDC_DEG_RADIO:
				pTTT->lpszText = "Use angles in degrees."; break;
			case IDC_ONTOP:
				UpdateData(TRUE);
				if (m_bOnTop) pTTT->lpszText = "Stop window from staying on top.";
				else pTTT->lpszText = "Keep window above others."; 
				break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
} //OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )

void CSpinDoctorDlg::OnOntop()
{
	UpdateData(TRUE);
	if (m_bOnTop) {
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	return;
} //OnOntop()

void CSpinDoctorDlg::OnOK()
{
	//Exit if button is focused
	if ((((CButton*)GetDlgItem(IDOK))->GetState()) && 0x0008) CDialog::OnOK();
} //OnOK()
