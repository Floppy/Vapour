// DizzyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Dizzy.h"
#include "DizzyDialog.h"
#include "AboutDialog.h"
#include "AxisRotation.h"
#include "Quaternion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDizzyDialog dialog


CDizzyDialog::CDizzyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDizzyDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDizzyDialog)
	m_strResult = _T("0 1 0 0");
	m_strInput = _T("x y z rot");
	m_bOnTop = FALSE;
	m_iPrecision = 0;
	//}}AFX_DATA_INIT
	m_icoPaste = AfxGetApp()->LoadIcon(IDI_PASTE);
	m_icoCopy  = AfxGetApp()->LoadIcon(IDI_COPY);
	m_icoUp    = AfxGetApp()->LoadIcon(IDI_UPARROW);
	m_icoDown  = AfxGetApp()->LoadIcon(IDI_DOWNARROW);
}

void CDizzyDialog::RecalculateRotation()
{
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	int num = pListBox->GetCount();	
	if (num == 0) {
		CAxisRotation rotResult;
		char* pszResult = rotResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
	}
	else if (num == 1) {
		CString strOnly;
		pListBox->GetText(0,strOnly);
		CAxisRotation rotResult;
		rotResult.ParseString(strOnly);
		rotResult = rotResult.Normalise();
		char* pszResult = rotResult.ToString(m_iPrecision);
		m_strResult = pszResult;
		free(pszResult);
	}
	else {
		if (num > 1) {
			CString strFirst;
			pListBox->GetText(0, strFirst);
			CAxisRotation rotFirst;
			rotFirst.ParseString(strFirst);
			CQuaternion quatResult(rotFirst);
			for (int i=1; i<num; i++) {
				CString strData;
				pListBox->GetText(i, strData);
				CAxisRotation rotData;
				rotData.ParseString(strData);
				CQuaternion quatData(rotData);
				quatResult = quatData * quatResult;
			}	
			CAxisRotation rotResult(quatResult);
			char* pszResult = rotResult.ToString(m_iPrecision);
			m_strResult = pszResult;
			free(pszResult);
		}
	}
}

void CDizzyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDizzyDialog)
	DDX_Text(pDX, IDC_RESULT_BOX, m_strResult);
	DDX_Text(pDX, IDC_INPUT_BOX, m_strInput);
	DDX_Check(pDX, IDC_ONTOP, m_bOnTop);
	DDX_Text(pDX, IDC_PRECISION, m_iPrecision);
	DDV_MinMaxInt(pDX, m_iPrecision, 0, 9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDizzyDialog, CDialog)
	//{{AFX_MSG_MAP(CDizzyDialog)
	ON_BN_CLICKED(IDC_ABOUT_BUTTON, OnAbout)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, OnClearButton)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_COPY_BUTTON, OnCopyButton)
	ON_BN_CLICKED(IDC_UP_BUTTON, OnUpButton)
	ON_BN_CLICKED(IDC_DOWN_BUTTON, OnDownButton)
	ON_BN_CLICKED(IDC_ONTOP, OnOntop)
	ON_BN_CLICKED(IDC_PASTE_BUTTON, OnPasteButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_PRECISION_SPIN, OnDeltaposPrecisionSpin)
	ON_EN_KILLFOCUS(IDC_PRECISION, OnKillfocusPrecision)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDizzyDialog message handlers

BOOL CDizzyDialog::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID = pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND) {
        nID = ::GetDlgCtrlID((HWND)nID);
		if(nID) {
			switch (nID) {
			case IDC_INPUT_LIST:
				pTTT->lpszText = "Current rotation list"; break;
			case IDC_INPUT_BOX:
				pTTT->lpszText = "Enter rotation to be added"; break;
			case IDC_DELETE_BUTTON:
				pTTT->lpszText = "Delete selected rotation"; break;
			case IDC_CLEAR_BUTTON:
				pTTT->lpszText = "Clear all rotations"; break;
			case IDC_ADD_BUTTON:
				pTTT->lpszText = "Add rotation to list"; break;
			case IDOK:
				pTTT->lpszText = "Exit Dizzy"; break;
			case IDC_ABOUT_BUTTON:
				pTTT->lpszText = "About Dizzy"; break;
			case IDC_HELP_BUTTON:
				pTTT->lpszText = "Get help on using Dizzy"; break;
			case IDC_PASTE_BUTTON:
				pTTT->lpszText = "Paste from clipboard"; break;
			case IDC_COPY_BUTTON:
				pTTT->lpszText = "Copy result to clipboard"; break;
			case IDC_RESULT_BOX:
				pTTT->lpszText = "Result"; break;
			case IDC_UP_BUTTON:
				pTTT->lpszText = "Move selected rotation up"; break;
			case IDC_DOWN_BUTTON:
				pTTT->lpszText = "Move selected rotation down"; break;
			case IDC_ONTOP:
				UpdateData(TRUE);
				if (m_bOnTop) pTTT->lpszText = "Stop window from staying on top.";
				else pTTT->lpszText = "Keep window above others."; 
				break;
			case IDC_PRECISION:
				pTTT->lpszText = "The maximum number of figures to display after the decimal point."; break;
			case IDC_PRECISION_SPIN:
				pTTT->lpszText = "Changes the precision value."; break;
			default:
				TRACE("Unknown ToolTip - ID : %d\n", nID);
			}
            pTTT->hinst = AfxGetResourceHandle();            
			return(TRUE);
        }
    }
    return(FALSE);
}

void CDizzyDialog::OnAbout() 
{
	CAboutDialog dlg;
	dlg.DoModal();
}

void CDizzyDialog::OnAddButton() 
{
	UpdateData(true);
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	CAxisRotation inputRotation;
	bool bValidString = inputRotation.ParseString(m_strInput);
	if (bValidString) {
		int iOffset = pListBox->GetCurSel();
		if (iOffset != LB_ERR) {
			pListBox->InsertString(iOffset+1,m_strInput);
			pListBox->SetCurSel(iOffset+1);
		}	
		else {
			pListBox->AddString(m_strInput);
			pListBox->SetCurSel(pListBox->GetCount()-1);
		}
		RecalculateRotation();
		m_strInput = "";
		UpdateData(false);
	}
	else {
		::AfxMessageBox("Input must be a valid axis-angle rotation!");
	}
}

void CDizzyDialog::OnDeleteButton() 
{
	UpdateData(true);
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	if (pListBox->DeleteString(pListBox->GetCurSel()) != LB_ERR) 
	{
		RecalculateRotation();
	}	
	UpdateData(false);
}

void CDizzyDialog::OnClearButton() 
{
	UpdateData(true);
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	pListBox->ResetContent();	
	RecalculateRotation();
	UpdateData(false);
}


int CDizzyDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HICON pIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if (pIcon) {
		SetIcon(pIcon, true);
	}

	return 0;
}

BOOL CDizzyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Set up numeric edit controls
	m_InputBox.SubclassDlgItem(IDC_INPUT_BOX,this);
	m_InputBox.SetOptions(4);

	// Enable tooltips
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

	m_pFntCourier = new CFont;
	m_pFntCourier->CreateFont(8,0,0,0,400,FALSE, FALSE, 0,
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_MODERN, "Courier");
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	pListBox->SetFont(m_pFntCourier);
	CEdit* pEditInput = (CEdit*) GetDlgItem(IDC_INPUT_BOX);
	pEditInput->SetFont(m_pFntCourier);
	CEdit* pEditResult = (CEdit*) GetDlgItem(IDC_RESULT_BOX);
	pEditResult->SetFont(m_pFntCourier);

	// Set up spin control
	((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetRange(0,9);
	((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetPos(m_iPrecision);
	
	// Set up icon buttons
	((CButton*) GetDlgItem(IDC_PASTE_BUTTON))->SetIcon(m_icoPaste);
	((CButton*) GetDlgItem(IDC_COPY_BUTTON))->SetIcon(m_icoCopy);
	((CButton*) GetDlgItem(IDC_UP_BUTTON))->SetIcon(m_icoUp);
	((CButton*) GetDlgItem(IDC_DOWN_BUTTON))->SetIcon(m_icoDown);

	//Make window float on top or not
	if (m_bOnTop) {
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDizzyDialog::OnSysCommand(UINT nID, LPARAM lParam) {
	if ((nID & 0xFFF0) == IDM_ABOUT) {
		CAboutDialog dlg;
		dlg.DoModal();
	}
	else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

BOOL CDizzyDialog::DestroyWindow() 
{
	delete m_pFntCourier;
	
	return CDialog::DestroyWindow();
}

void CDizzyDialog::OnCopyButton() 
{
	CEdit* pEditResult = (CEdit*) GetDlgItem(IDC_RESULT_BOX);
	pEditResult->SetSel(0,-1);
	pEditResult->Copy();
}

void CDizzyDialog::OnUpButton() 
{
	UpdateData(true);
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	int iOffset = pListBox->GetCurSel();
	if ((iOffset != LB_ERR) && (iOffset != 0)) {
		CString strCurrent;
		pListBox->GetText(iOffset, strCurrent);
		pListBox->DeleteString(iOffset);
		iOffset--;
		pListBox->InsertString(iOffset, strCurrent);
		pListBox->SetCurSel(iOffset);
		RecalculateRotation();
	}
	UpdateData(false);
}

void CDizzyDialog::OnDownButton() 
{
	UpdateData(true);
	CListBox* pListBox = (CListBox*) GetDlgItem(IDC_INPUT_LIST);
	int iOffset = pListBox->GetCurSel();
	if ((iOffset != LB_ERR) && (iOffset != pListBox->GetCount()-1)) {
		CString strCurrent;
		pListBox->GetText(iOffset, strCurrent);
		pListBox->DeleteString(iOffset);
		iOffset++;
		pListBox->InsertString(iOffset, strCurrent);
		pListBox->SetCurSel(iOffset);
		RecalculateRotation();
	}
	UpdateData(false);
}

void CDizzyDialog::OnOntop() 
{
	UpdateData(TRUE);
	if (m_bOnTop) {
		SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	else {
		SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	}
	return;
}

void CDizzyDialog::OnPasteButton() 
{
	((CEdit*)GetDlgItem(IDC_INPUT_BOX))->Paste();
	return;
}

void CDizzyDialog::SetSettings(int iPrecision, int iOnTop)
{
	m_iPrecision = iPrecision;
	m_bOnTop = (iOnTop == 0) ? false : true;
	return;
} //SetSettings(int iPrecision, int iOnTop)

void CDizzyDialog::GetSettings(int* ipPrecision, int* ipOnTop)
{
	*ipPrecision = m_iPrecision;
	*ipOnTop = (int)m_bOnTop;
	return;	
} //GetSettings(int* ipPrecision, int* ipOnTop)

void CDizzyDialog::OnDeltaposPrecisionSpin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	m_iPrecision = pNMUpDown->iPos;
	if (!((m_iPrecision == 0) && (pNMUpDown->iDelta == -1)) && !((m_iPrecision == 9) && (pNMUpDown->iDelta == 1))) m_iPrecision += pNMUpDown->iDelta;
	RecalculateRotation();
	UpdateData(false);
	*pResult = 0;
} //OnDeltaposPrecisionSpin(NMHDR* pNMHDR, LRESULT* pResult)

void CDizzyDialog::OnKillfocusPrecision() 
{
	//Retrieve precision setting
	m_iPrecision = ((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->GetPos();
	//If greater than 9, make it 9
	if (m_iPrecision > 9) {
		((CSpinButtonCtrl*) GetDlgItem(IDC_PRECISION_SPIN))->SetPos(9);
		m_iPrecision = 9;
	}
	RecalculateRotation();
	UpdateData(false);
	return;
} //OnKillfocusPrecision()

void CDizzyDialog::OnDestroy() 
{
	WinHelp(0L, HELP_QUIT);
	CDialog::OnDestroy();
} //OnDestroy()