// UnrealToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdio.h"
#include "UnrealTool.h"
#include "UnrealToolDlg.h"
#include "Unreal.h"
#include <strstrea.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnrealToolDlg dialog

CUnrealToolDlg::CUnrealToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUnrealToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUnrealToolDlg)
	m_strByte0 = _T("");
	m_strByte1 = _T("");
	m_strByte2 = _T("");
	m_strByte3 = _T("");
	m_strByte4 = _T("");
	m_strIndex = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUnrealToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUnrealToolDlg)
	DDX_Text(pDX, IDC_CI_0, m_strByte0);
	DDX_Text(pDX, IDC_CI_1, m_strByte1);
	DDX_Text(pDX, IDC_CI_2, m_strByte2);
	DDX_Text(pDX, IDC_CI_3, m_strByte3);
	DDX_Text(pDX, IDC_CI_4, m_strByte4);
	DDX_Text(pDX, IDC_INDEX, m_strIndex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUnrealToolDlg, CDialog)
	//{{AFX_MSG_MAP(CUnrealToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COMPACT, OnCompact)
	ON_BN_CLICKED(IDC_UNCOMPACT, OnUncompact)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUnrealToolDlg message handlers

BOOL CUnrealToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
   SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CUnrealToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUnrealToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUnrealToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CUnrealToolDlg::OnCompact()  {

   // Get data from dialog
   UpdateData(TRUE);

   long int iIndex = 0;

   // Read integer string
   char* pszBuffer = m_strIndex.GetBuffer(32);
   istrstream strInput(pszBuffer);
   strInput.setf(ios::hex);
   strInput >> iIndex;
   m_strIndex.ReleaseBuffer();

   // Create compact index
   CUnrealCompactIndex oCIndex(iIndex);
   
   // Output as set of compact index bytes
   for (int i=0; i<oCIndex.NumBytes(); i++) {
      switch (i) {
      case 0:
         m_strByte0.Format("%02x",oCIndex.CompactIndex()[i]);
         break;
      case 1:
         m_strByte1.Format("%02x",oCIndex.CompactIndex()[i]);
         break;
      case 2:
         m_strByte2.Format("%02x",oCIndex.CompactIndex()[i]);
         break;
      case 3:
         m_strByte3.Format("%02x",oCIndex.CompactIndex()[i]);
         break;
      case 4:
         m_strByte4.Format("%02x",oCIndex.CompactIndex()[i]);
         break;
      default:
         break;
      }
   }

   // Clear remaining entries
   for ( ; i<5; i++) {
      switch (i) {
      case 0:
         m_strByte0.Format("");
         break;
      case 1:
         m_strByte1.Format("");
         break;
      case 2:
         m_strByte2.Format("");
         break;
      case 3:
         m_strByte3.Format("");
         break;
      case 4:
         m_strByte4.Format("");
         break;
      default:
         break;
      }
   }

   // Save data to dialog
   UpdateData(FALSE);	
}

void CUnrealToolDlg::OnUncompact() {

   // Get data from dialog
   UpdateData(TRUE);

   // Read input bytes
   unsigned char pBytes[5] = {0,0,0,0,0};
   for (int i=0; i<5; i++) {
      switch (i) {
      case 0: {
            char* pszBuffer = m_strByte0.GetBuffer(32);
            istrstream strInput(pszBuffer);
            strInput.setf(ios::hex);
            int iInput;
            strInput >> iInput;
            pBytes[0] = (char)iInput;
            m_strByte0.ReleaseBuffer();
         }
         break;
      case 1: {
            char* pszBuffer = m_strByte1.GetBuffer(32);
            istrstream strInput(pszBuffer);
            strInput.setf(ios::hex);
            int iInput;
            strInput >> iInput;
            pBytes[1] = (char)iInput;
            m_strByte1.ReleaseBuffer();
         }
         break;
      case 2: {
            char* pszBuffer = m_strByte2.GetBuffer(32);
            istrstream strInput(pszBuffer);
            strInput.setf(ios::hex);
            int iInput;
            strInput >> iInput;
            pBytes[2] = (char)iInput;
            m_strByte2.ReleaseBuffer();
         }
         break;
      case 3: {
            char* pszBuffer = m_strByte3.GetBuffer(32);
            istrstream strInput(pszBuffer);
            strInput.setf(ios::hex);
            int iInput;
            strInput >> iInput;
            pBytes[3] = (char)iInput;
            m_strByte3.ReleaseBuffer();
         }
         break;
      case 4: {
            char* pszBuffer = m_strByte4.GetBuffer(32);
            istrstream strInput(pszBuffer);
            strInput.setf(ios::hex);
            int iInput;
            strInput >> iInput;
            pBytes[4] = (char)iInput;
            m_strByte4.ReleaseBuffer();
         }
         break;
      default:
         break;
      }
   }
   
   // Create Compact Index
   CUnrealCompactIndex oCIndex(pBytes[0],pBytes[1],pBytes[2],pBytes[3],pBytes[4]);

   // Output as integer
   m_strIndex.Format("%08x",oCIndex.Index());

   // Save data to dialog
   UpdateData(FALSE);	
}
