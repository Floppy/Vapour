// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Dizzy.h"
#include "AboutDialog.h"
#include "CreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog


CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CAboutDialog)
	ON_BN_CLICKED(IDC_CREDIT_BUTTON, OnCreditButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog message handlers

void CAboutDialog::OnCreditButton() 
{
	CCreditsDialog dlg;
	dlg.DoModal();
}

BOOL CAboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
   // Set up static link
   m_oVapourLink.SubclassDlgItem(IDC_VAPOURTECH_LINK,this);

	// Set up bitmap button
	VERIFY(m_AboutLogo.AutoLoad(IDC_CREDIT_BUTTON, this));
	
	// Get client area size
	CRect oWndRect, oButtonRect;
	this->GetClientRect(oWndRect);
	// Get client relative button coords
	CWnd *pButton = (CWnd*)GetDlgItem(IDC_CREDIT_BUTTON);
	pButton->GetWindowRect(oButtonRect);
	this->ScreenToClient(oButtonRect);
	// Calculate the horizontal shift
	int iBWidth = oButtonRect.Width();
	int iBOffset = (oWndRect.Width() - iBWidth) / 2;
	oButtonRect.left = oWndRect.left + iBOffset;
	oButtonRect.right = oButtonRect.left + iBWidth;
	// Shift the button
	pButton->MoveWindow(oButtonRect, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
