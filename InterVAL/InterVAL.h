//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// InterVAL.h - 23/11/2000 - Warren Moore
//	  Main application
//
// $Id: InterVAL.h,v 1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_INTERVALAPP_
#define _INTERVAL_INTERVALAPP_

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

/////////////////
// CInterVALApp

class CInterVALApp : public CWinApp {

//#===--- External Functions
public:
	CInterVALApp();

//#===--- Internal Functions
protected:

//#===--- Internal Data
protected:

//#===--- MFC Mapping

	//{{AFX_VIRTUAL(CInterVALApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CInterVALApp)
	afx_msg void OnAppAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_INTERVALAPP_
