// PeekAboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Peek.h"
#include "PeekAboutDialog.h"
#include "PeekCreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeekAboutDialog dialog


CPeekAboutDialog::CPeekAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPeekAboutDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeekAboutDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPeekAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeekAboutDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeekAboutDialog, CDialog)
	//{{AFX_MSG_MAP(CPeekAboutDialog)
	ON_BN_CLICKED(IDC_CREDIT_BUTTON, OnCreditButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeekAboutDialog message handlers

void CPeekAboutDialog::OnCreditButton() 
{
	CPeekCreditsDialog CreditDlg;
	CreditDlg.DoModal();
}

BOOL CPeekAboutDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	VERIFY(m_AboutLogo.AutoLoad(IDC_CREDIT_BUTTON, this));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
