// DoneDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CCFix.h"
#include "DoneDialog.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoneDialog dialog

CDoneDialog::CDoneDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDoneDialog::IDD, pParent),
	m_oMicrosoftLink("http://support.microsoft.com/support/kb/articles/Q181/5/06.ASP")

{
	//{{AFX_DATA_INIT(CDoneDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_CCFIX);
}

void CDoneDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDoneDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDoneDialog, CDialog)
	//{{AFX_MSG_MAP(CDoneDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoneDialog message handlers

BOOL CDoneDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

    // Set up static link
    m_oMicrosoftLink.SubclassDlgItem(IDC_MICROSOFT_LINK,this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}
