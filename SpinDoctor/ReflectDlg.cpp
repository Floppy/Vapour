// ReflectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SpinDoctor.h"
#include "ReflectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReflectDlg dialog


CReflectDlg::CReflectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReflectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReflectDlg)
	m_bX = FALSE;
	m_bY = FALSE;
	m_bZ = FALSE;
	//}}AFX_DATA_INIT
}


void CReflectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReflectDlg)
	DDX_Check(pDX, IDC_X_CHECK, m_bX);
	DDX_Check(pDX, IDC_Y_CHECK, m_bY);
	DDX_Check(pDX, IDC_Z_CHECK, m_bZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReflectDlg, CDialog)
	//{{AFX_MSG_MAP(CReflectDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReflectDlg message handlers

BOOL CReflectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
