#include "stdafx.h"
#include "WedgieApp.h"

#include "Progress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
// CProgress

CProgress::CProgress(CWnd* pParent) : CDialog(CProgress::IDD, pParent) {
	//{{AFX_DATA_INIT(CProgress)
	//}}AFX_DATA_INIT
	m_iFileSize = 0;
	m_iTotalSize = 0;
} // Contructor

void CProgress::DoDataExchange(CDataExchange* pDX) {
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProgress)
	DDX_Control(pDX, IDC_TOTAL_BAR, m_oTotalBar);
	DDX_Control(pDX, IDC_FILE_BAR, m_oFileBar);
	//}}AFX_DATA_MAP
} // DoDataExchange

BOOL CProgress::Create() {
	return CDialog::Create(CProgress::IDD);
} // Create

BOOL CProgress::OnInitDialog() {
	CDialog::OnInitDialog();
	
	m_oFileBar.SetRange(0, 1000);
	m_oFileBar.SetPos(0);
	m_oTotalBar.SetRange(0, 1000);
	m_oTotalBar.SetPos(0);

	return TRUE;
} // OnInitDialog

void CProgress::SetFileName(const char *pText) {
	ASSERT(pText);
	SetDlgItemText(IDC_FILENAME, pText);
} // SetFileName

void CProgress::SetFileSize(unsigned int iSize) {
	m_iFileSize = iSize;
} // SetFileSize

void CProgress::SetFilePos(unsigned int iPos) {
	double dPercent = double(iPos) / double(m_iFileSize);
	m_oFileBar.SetPos((int)(dPercent * 1000));
} // SetFilePos

void CProgress::SetTotalSize(unsigned int iSize) {
	m_iTotalSize = iSize;
} // SetTotalSize

void CProgress::SetTotalPos(unsigned int iPos) {
	double dPercent = double(iPos) / double(m_iTotalSize);
	m_oTotalBar.SetPos((int)(dPercent * 1000));
} // SetTotalPos

BEGIN_MESSAGE_MAP(CProgress, CDialog)
	//{{AFX_MSG_MAP(CProgress)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
