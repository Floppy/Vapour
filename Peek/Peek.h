// Peek.h : main header file for the PEEK application
//

#if !defined(AFX_PEEK_H__C64FF465_7FE6_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_PEEK_H__C64FF465_7FE6_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPeekApp:
// See Peek.cpp for the implementation of this class
//

class CPeekApp : public CWinApp
{
public:
	CPeekApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeekApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPeekApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEEK_H__C64FF465_7FE6_11D2_B744_80E203C10000__INCLUDED_)
