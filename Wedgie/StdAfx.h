//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// StdAfx.h - 02/12/1999 - Warren Moore
//	  Windows includes
//
// $Id: StdAfx.h,v 1.2 2000/12/03 18:30:04 warren Exp $
//

#ifndef _WEDGIE_STDAFX_
#define _WEDGIE_STDAFX_

#pragma once

#define VC_EXTRALEAN					// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>					// MFC core and standard components
#include <afxext.h>					// MFC extensions
#include <afxdtctl.h>				// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>					// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//{{AFX_INSERT_LOCATION}}

#endif // _WEDGIE_STDAFX_
