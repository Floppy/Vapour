#ifndef _PROGRESS_
#define _PROGRESS_

#pragma once

//////////////
// CProgress

class CProgress : public CDialog {
public:
	CProgress(CWnd* pParent = NULL);

	BOOL Create();

	void SetFileName(const char *pText);
	void SetFileSize(unsigned int iSize);
	void SetFilePos(unsigned int iPos);
	void SetTotalSize(unsigned int iSize);
	void SetTotalPos(unsigned int iPos);

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_oTotalBar;
	CProgressCtrl	m_oFileBar;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	unsigned int m_iFileSize;
	unsigned int m_iTotalSize;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _PROGRESS_
