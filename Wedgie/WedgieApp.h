//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// WedgieApp.h - 02/12/1999 - Warren Moore
//	  Main application declaration
//
// $Id: WedgieApp.h,v 1.2 2000/12/03 18:30:04 warren Exp $
//

#ifndef _WEDGIE_APP_
#define _WEDGIE_APP_

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define REGISTRY_KEY	"vapourtech"
#define WEDGIE_VERSION "v1.1"

///////////////
// CWedgieApp

class CWedgieApp : public CWinApp {
public:
	CWedgieApp();

	//{{AFX_VIRTUAL(CWedgieApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWedgieApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WEDGIE_APP_
