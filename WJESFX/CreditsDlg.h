#ifndef _CREDITSDIALOG_
#define _CREDITSDIALOG_

#pragma once

///////////////////
// CCreditsDialog

class CCreditsDialog : public CDialog {
public:
	CCreditsDialog(CWnd* pParent = NULL);

	//{{AFX_DATA(CCreditsDialog)
	enum { IDD = IDD_CREDITS_DIALOG };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCreditsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCreditsDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _CREDITSDIALOG_
