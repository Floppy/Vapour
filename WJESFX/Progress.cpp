#include "stdafx.h"
#include "WJESFX.h"

#include "Progress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
// CProgress

CProgress::CProgress(CWnd* pParent)	: CDialog(CProgress::IDD, pParent) {
	//{{AFX_DATA_INIT(CProgress)
	//}}AFX_DATA_INIT
} // Constructor

void CProgress::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_oProgress);
	//}}AFX_DATA_MAP
} // DoDataExchange

BOOL CProgress::Create() {
	return CDialog::Create(CProgress::IDD);
} // Create

BOOL CProgress::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_oProgress.SetPos(0);
	m_oProgress.SetStep(1);
	m_oProgress.SetRange32(0, 100);

	return TRUE;
} // OnInitDialog

void CProgress::SetMax(int iMax) {
	m_oProgress.SetRange32(0, iMax);
} // SetMax

void CProgress::Step() {
	m_oProgress.StepIt();
} // Step

BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


