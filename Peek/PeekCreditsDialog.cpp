// PeekCreditsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Peek.h"
#include "PeekCreditsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeekCreditsDialog dialog


CPeekCreditsDialog::CPeekCreditsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPeekCreditsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeekCreditsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPeekCreditsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeekCreditsDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeekCreditsDialog, CDialog)
	//{{AFX_MSG_MAP(CPeekCreditsDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeekCreditsDialog message handlers
