// UnrealTool.h : main header file for the UNREALTOOL application
//

#if !defined(AFX_UNREALTOOL_H__D1DA182D_6FC2_46FB_8B9E_5CE586B45120__INCLUDED_)
#define AFX_UNREALTOOL_H__D1DA182D_6FC2_46FB_8B9E_5CE586B45120__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUnrealToolApp:
// See UnrealTool.cpp for the implementation of this class
//

class CUnrealToolApp : public CWinApp
{
public:
	CUnrealToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUnrealToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUnrealToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UNREALTOOL_H__D1DA182D_6FC2_46FB_8B9E_5CE586B45120__INCLUDED_)
