//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// AboutDialog.h - 23/11/2000 - Warren Moore
//	  InterVAL About dialog box
//
// $Id: AboutDialog.h,v 1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_ABOUTDLG_
#define _INTERVAL_ABOUTDLG_

#pragma once

//////////////
// CAboutDlg

class CAboutDlg : public CDialog {
public:
	CAboutDlg();

//#===--- MFC Mapping

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_ABOUTDLG_