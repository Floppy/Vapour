// CreditsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Twister.h"
#include "CreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreditsDialog dialog


CCreditsDialog::CCreditsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCreditsDialog::IDD, pParent)
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
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreditsDialog message handlers
