///////////////////////////////
// Wedgie - WJE file compiler
//
// Copyright 1999 Vapour Technology
// 02/12/99 - Warren Moore
// 
// WedgieApp.h
//

#ifndef _WEDGIE_
#define _WEDGIE_

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define REGISTRY_KEY	"Wedgie"

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

#endif // _WEDGIE_
