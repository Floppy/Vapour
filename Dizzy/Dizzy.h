// Dizzy.h : main header file for the DIZZY application
//

#if !defined(AFX_DIZZY_H__6C21C725_7BD1_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_DIZZY_H__6C21C725_7BD1_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDizzyApp:
// See Dizzy.cpp for the implementation of this class
//

class CDizzyApp : public CWinApp
{
public:
	CDizzyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDizzyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDizzyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIZZY_H__6C21C725_7BD1_11D2_B744_80E203C10000__INCLUDED_)
