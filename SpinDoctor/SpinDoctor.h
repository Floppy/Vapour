// SpinDoctor.h : main header file for the SPINDOCTOR application
//

#if !defined(AFX_SPINDOCTOR_H__0A86D1E4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
#define AFX_SPINDOCTOR_H__0A86D1E4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpinDoctorApp:
// See SpinDoctor.cpp for the implementation of this class
//

class CSpinDoctorApp : public CWinApp
{
public:
	CSpinDoctorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpinDoctorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpinDoctorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINDOCTOR_H__0A86D1E4_4534_11D3_8CBF_00C0DF468F89__INCLUDED_)
