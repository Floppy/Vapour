// PeekHelpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Peek.h"
#include "PeekHelpDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPeekHelpDialog dialog


CPeekHelpDialog::CPeekHelpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPeekHelpDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeekHelpDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPeekHelpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeekHelpDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPeekHelpDialog, CDialog)
	//{{AFX_MSG_MAP(CPeekHelpDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeekHelpDialog message handlers
