// Twister.h : main header file for the TWISTER application
//

#if !defined(AFX_TWISTER_H__DF72A925_9386_11D2_B744_80E203C10000__INCLUDED_)
#define AFX_TWISTER_H__DF72A925_9386_11D2_B744_80E203C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTwisterApp:
// See Twister.cpp for the implementation of this class
//

class CTwisterApp : public CWinApp
{
public:
	CTwisterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwisterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTwisterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TWISTER_H__DF72A925_9386_11D2_B744_80E203C10000__INCLUDED_)
