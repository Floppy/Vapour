// CreditsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CCFix.h"
#include "CreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreditsDialog dialog


CCreditsDialog::CCreditsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreditsDialog::IDD, pParent),
	  m_oMicrosoftLink("http://support.microsoft.com/support/kb/articles/Q181/5/06.ASP")
{
	//{{AFX_DATA_INIT(CCreditsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCreditsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreditsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreditsDialog, CDialog)
	//{{AFX_MSG_MAP(CCreditsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreditsDialog message handlers

BOOL CCreditsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    // Set up static link
    m_oMicrosoftLink.SubclassDlgItem(IDC_MICROSOFT_LINK,this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
