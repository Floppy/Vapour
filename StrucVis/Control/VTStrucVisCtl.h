//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.h,v 1.4 2002/03/22 16:54:44 vap-warren Exp $

#ifndef __VTSTRUCTVIS_CONTROL__
#define __VTSTRUCTVIS_CONTROL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For category support
#include <comcat.h>

#include "CortonaControl.h"
#include "SimDataPath.h"

// Category support functions
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);

///////////////////
// CVTStrucVisCtl

class CVTStrucVisCtl : public COleControl {
// Dynamic construction
	DECLARE_DYNCREATE(CVTStrucVisCtl)
public:
	CVTStrucVisCtl();

//#===--- Member Functions

protected:
   bool InitCortona();
   // Calls GetCortona to find the control, then initialises it

   bool GetCortona();
   // Searches for a Cortona control within the current container

   friend class CSimDataPath;
   // Friend class to call InitCortona - REMOVE

//#===--- Member Variables
protected:
   CString m_oDrawText;                      // Screen text
   CCortonaControl m_oControl;               // Cortona control manager
   CSimDataPath m_oSimData;                  // Asynchronous simulation data file

//#===--- Windows Mappings

// Overrides
	//{{AFX_VIRTUAL(CVTStrucVisCtl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	//}}AFX_VIRTUAL

protected:
	~CVTStrucVisCtl();

	DECLARE_OLECREATE_EX(CVTStrucVisCtl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVTStrucVisCtl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVTStrucVisCtl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVTStrucVisCtl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CVTStrucVisCtl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVTStrucVisCtl)
	afx_msg BSTR GetSimData();
	afx_msg void SetSimData(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CVTStrucVisCtl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CVTStrucVisCtl)
	dispidSimData = 1L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__
