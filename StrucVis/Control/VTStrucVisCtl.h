//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.h,v 1.3 2002/03/20 13:08:26 vap-warren Exp $

#ifndef __VTSTRUCTVIS_CONTROL__
#define __VTSTRUCTVIS_CONTROL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For category support
#include <comcat.h>

#include "CortonaControl.h"

// Category support functions
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);

////////////////////
// CVTStrucVisCtrl

class CVTStrucVisCtrl : public COleControl {
// Dynamic construction
	DECLARE_DYNCREATE(CVTStrucVisCtrl)
public:
	CVTStrucVisCtrl();

//#===--- Member Functions
   bool GetCortona();
   // Searches for a Cortona control within the current container

//#===--- Member Variables
protected:
   CString m_oDrawText;                      // Screen text
   CCortonaControl m_oControl;               // Cortona control manager

//#===--- Windows Mappings

// Overrides
	//{{AFX_VIRTUAL(CVTStrucVisCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	//}}AFX_VIRTUAL

protected:
	~CVTStrucVisCtrl();

	DECLARE_OLECREATE_EX(CVTStrucVisCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVTStrucVisCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVTStrucVisCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVTStrucVisCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVTStrucVisCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVTStrucVisCtrl)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CVTStrucVisCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVTStrucVisCtrl)
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__