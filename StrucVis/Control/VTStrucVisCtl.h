//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.h,v 1.6 2002/03/23 21:41:19 vap-warren Exp $

#ifndef __VTSTRUCTVIS_CONTROL__
#define __VTSTRUCTVIS_CONTROL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For category support
#include <comcat.h>

#include "CortonaControl.h"
#include "UIDataPath.h"
#include "SimDataPath.h"

// Category support functions
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);

///////////////////
// CVTStrucVisCtl

#define AD_EMPTY              0x00000000
#define AD_UILOADING          0x00000001
#define AD_UILOADED           0x00000002
#define AD_UIACTIVE           0x00000003
#define AD_UIMASK             0xFFFFFFFC
#define AD_SIMLOADING         0x00000004
#define AD_SIMLOADED          0x00000008
#define AD_SIMACTIVE          0x0000000C
#define AD_SIMMASK            0xFFFFFFF3

class CVTStrucVisCtl : public COleControl {
// Dynamic construction
	DECLARE_DYNCREATE(CVTStrucVisCtl)
public:
	CVTStrucVisCtl();

//#===--- Member Functions
protected:

   bool InitCortona();
   // Starts the Cortona control, resetting the control if already initialised

   void ExitCortona();
   // Exits the Cortona control

   bool GetCortona();
   // Searches for a Cortona control within the current container

   void DrawPlaceholder(CDC* pDC, const CRect& rcBounds, bool bRun);
   // Draws the control placeholder
   // bRun indicates that the control is in run mode, not dev mode

   void UILoading();
   // Called by CUIDataPath to indicate that the data is loading

   void UILoaded();
   // Called by CUIDataPath to indicate that the data is loaded

   void SimLoading();
   // Called by CSimDataPath to indicate that the data is loading

   void SimLoaded();
   // Called by CSimDataPath to indicate that the data is loaded

//#===--- Private Data Types
protected:

   // Error states
   typedef enum TECortonaResult {
      CR_UNKNOWN = 0,               // Unknown
      CR_NOCONTAINER,               // Unable to connect to control container
      CR_NOENUMOBJECTS,             // Container does not support EnumObjects
      CR_NOCONTROL,                 // Cannot find Cortona control within container
      CR_OK,                        // Cortona initialised OK
   } ECortonaResult;

//#===--- Member Variables
protected:

   // State variables
   ECortonaResult m_eCortonaResult;          // GetCortona result
   DWORD m_uiAsyncFlags;                     // Asynchronous data flags

   // Control objects
   CCortonaControl m_oCortona;               // Cortona control manager
   CUIDataPath m_oUIData;                    // Asynchronous UI data file
   CSimDataPath m_oSimData;                  // Asynchronous simulation data file

//#===--- Windows Mappings

// Overrides
	//{{AFX_VIRTUAL(CVTStrucVisCtl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual void OnAmbientPropertyChange(DISPID dispid);
	//}}AFX_VIRTUAL

protected:
	~CVTStrucVisCtl();

	DECLARE_OLECREATE_EX(CVTStrucVisCtl)    // Class factory and guid
	DECLARE_OLETYPELIB(CVTStrucVisCtl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CVTStrucVisCtl)     // Property page IDs
	DECLARE_OLECTLTYPE(CVTStrucVisCtl)		 // Type name and misc status

// Message maps
	//{{AFX_MSG(CVTStrucVisCtl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVTStrucVisCtl)
	afx_msg BSTR GetSimData();
	afx_msg void SetSimData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUIData();
	afx_msg void SetUIData(LPCTSTR lpszNewValue);
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
   dispidUIData = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__
