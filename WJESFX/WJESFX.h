#ifndef _WJESFX_
#define _WJESFX_

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

///////////////
// CWJESFXApp

class CWJESFXApp : public CWinApp {
public:
	CWJESFXApp();

	void Install();

	//{{AFX_VIRTUAL(CWJESFXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWJESFXApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WJESFX_
