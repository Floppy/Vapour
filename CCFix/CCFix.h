// CCFix.h : main header file for the CCFIX application
//

#if !defined(AFX_CCFIX_H__4E2B65C8_0F4D_49F0_BFCB_AB5305FE86F9__INCLUDED_)
#define AFX_CCFIX_H__4E2B65C8_0F4D_49F0_BFCB_AB5305FE86F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCCFixApp:
// See CCFix.cpp for the implementation of this class
//

class CCCFixApp : public CWinApp
{
public:
	CCCFixApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCCFixApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCCFixApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCFIX_H__4E2B65C8_0F4D_49F0_BFCB_AB5305FE86F9__INCLUDED_)
