#include "stdafx.h"
#include "WJESFX.h"

#include "CreditsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////
// CCreditsDialog

CCreditsDialog::CCreditsDialog(CWnd* pParent)	: CDialog(CCreditsDialog::IDD, pParent) {
	//{{AFX_DATA_INIT(CCreditsDialog)
	//}}AFX_DATA_INIT
} // Contructor

void CCreditsDialog::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreditsDialog)
	//}}AFX_DATA_MAP
} // DoDataExchange

BEGIN_MESSAGE_MAP(CCreditsDialog, CDialog)
	//{{AFX_MSG_MAP(CCreditsDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CCreditsDialog::OnInitDialog() {
	CDialog::OnInitDialog();
	
	return TRUE;
} // OnInitDialog
