#ifndef _PROGRESS_
#define _PROGRESS_

#pragma once

//////////////
// CProgress

class CProgress : public CDialog {
public:
	CProgress(CWnd* pParent = NULL);

	BOOL Create();

	void SetMax(int iMax);
	void Step();

	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_oProgress;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _PROGRESS_
