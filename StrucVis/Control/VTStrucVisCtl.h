//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.h,v 1.14 2002/03/25 15:57:03 vap-warren Exp $

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
#include "CortonaUtil.h"
#include "SceneManager.h"

// Category support functions
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription);
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid);
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid);

///////////////////
// CVTStrucVisCtl

// Asynchronous data flag defines
#define AD_EMPTY              0x00000000
#define AD_UILOADING          0x00000001
#define AD_UILOADED           0x00000002
#define AD_UIACTIVE           0x00000003
#define AD_UIMASK             0xFFFFFFFC
#define AD_SIMLOADING         0x00000004
#define AD_SIMLOADED          0x00000008
#define AD_SIMACTIVE          0x0000000C
#define AD_SIMMASK            0xFFFFFFF3

// Timer IDs
#define TI_MOUSEOVER          100
#define TI_ANIMATE            101

class CVTStrucVisCtl : public COleControl {
// Dynamic construction
	DECLARE_DYNCREATE(CVTStrucVisCtl)
public:
	CVTStrucVisCtl();

//#===--- Member Functions
public:

   //--- Asynchronous data-driven functions

   void UILoading();
   // Called by CUIDataPath to indicate that the data is loading

   void UILoaded();
   // Called by CUIDataPath to indicate that the data is loaded

   void SimLoading();
   // Called by CSimDataPath to indicate that the data is loading

   void SimLoaded();
   // Called by CSimDataPath to indicate that the data is loaded

   bool SceneSetup(const unsigned char *pucData, unsigned int uiLength);
   // Passes loaded data into the scene, when true is returned

   void ShowFrame(unsigned int uiFrame);
   // Displays the chosen frame

   void ShowFrame(const unsigned char *pucData, unsigned int uiLength);
   // Passes the frame data to the scene manager for display
   // Only to be called by CSimDataPath

protected:

   //--- Cortona Functions

   bool InitCortona();
   // Starts the Cortona control, resetting the control if already initialised

   void ExitCortona();
   // Exits the Cortona control

   bool GetCortona();
   // Searches for a Cortona control within the current container

   //--- Rendering Functions

   void DrawPlaceholder(CDC *pDC, const CRect &rcBounds, bool bRun);
   // Draws the control placeholder
   // bRun indicates that the control is in run mode, not dev mode

   void DrawUI(CDC *pDC, const CRect &rcBounds, bool bRun);
   // Render the VCR-style interface

   //--- UI Functions

   void GoInteractive();
   // Called by SceneSetup to indicate that enough data is present
   // to go into interactive mode

   bool LoadBitmap();
   // Tries to load the bitmap from the UIDataPath object, and sets
   // m_eUIResult accordingly - returns true if successful

   void FrameControl();
   // Update frame controller dependent on UI states

   void UIControl();
   // Update the UI state when UI event happens

   bool Interactive();
   // Checks all possible conditions to see if we are interactive

//#===--- Private Data Types
protected:

   // Cortona error states
   typedef enum TECortonaResult {
      CR_UNKNOWN = 0,               // Unknown
      CR_NOCONTAINER,               // Unable to connect to control container
      CR_NOENUMOBJECTS,             // Container does not support EnumObjects
      CR_NOCONTROL,                 // Cannot find Cortona control within container
      CR_OK,                        // Cortona initialised OK
   } ECortonaResult;

   // UI data error states
   typedef enum TEUIResult {
      UI_UNKNOWN = 0,               // Unknown
      UI_NOTSET,                    // No UI data path is set
      UI_NOTLOADED,                 // The UI data has not finished loading
      UI_DATAERROR,                 // Error loading the UI data
      UI_NOTBITMAP,                 // UI data is not a bitmap
      UI_WRONGDEPTH,                // The bitmap is of an incompatible colour depth
      UI_WRONGTYPE,                 // The bitmap is of an incompatible compression type
      UI_WRONGFORMAT,               // Bitmap must be packed and not have a colour table
      UI_OK,                        // UI loaded OK
   } EUIResult;

   // Simulation data error states
   typedef enum TESimResult {
      SD_UNKNOWN = 0,               // Unknown
      SD_ERROR,                     // Set when all data is loaded, yet setup not completed
      SD_OK,                        // Sim data loaded OK
   } ESimResult;

   // Player run mode
   typedef enum TERunMode {
      RM_PAUSE = 0,                 // Not running
      RM_PLAY,                      // Playing
      RM_PLAYREV,                   // Reverse playing
      RM_REWIND,                    // Rewind
      RM_FASTFORWARD,               // Fast forward
   } ERunMode;

//#===--- Member Variables
protected:

   // State variables
   ECortonaResult m_eCortonaResult;          // GetCortona result
   EUIResult m_eUIResult;                    // UIDataPath result
   ESimResult m_eSimResult;                  // SimDataPath result
   unsigned int m_uiAsyncFlags;              // Asynchronous data flags
   CString m_oWRLPath;                       // Scene WRL path

   // UI vars
   bool m_bLButtonDown;                      // Left mouse button down indicator
   bool m_bMouseOver;                        // Mouse over control indicator
   int m_iUIZone;                            // Current remote button under mouse (-1 if none)
   unsigned int m_uiFrame;                   // Current animation frame
   bool m_bDirty;                            // Frame change indicator
   ERunMode m_eRunMode;                      // Current run mode
   bool m_bRunning;                          // Animation running indicator
   bool m_bLoop;                             // Looping play indicator

   // Render buffers
   CBitmap m_oBackBuffer;                    // Screen back buffer
   CSize m_oBufferSize;                      // Current back buffer size
   CBitmap m_oUIBitmap;                      // UI bitmap object
   
   // Control objects
   CCortonaControl m_oCortona;               // Cortona control manager
   CUIDataPath m_oUIData;                    // Asynchronous UI data file
   CSimDataPath m_oSimData;                  // Asynchronous simulation data file
   CCortonaUtil *m_poCortonaUtil;            // Cortona Utility class
   CSceneManager *m_poScene;                 // Scene manager

//#===--- Windows Mappings

// Overrides
	//{{AFX_VIRTUAL(CVTStrucVisCtl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual DWORD GetControlFlags();
	virtual void OnAmbientPropertyChange(DISPID dispid);
	protected:
	virtual void OnDrawMetafile(CDC* pDC, const CRect& rcBounds);
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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CVTStrucVisCtl)
	afx_msg BSTR GetSimData();
	afx_msg void SetSimData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUIData();
	afx_msg void SetUIData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetWRLPath();
	afx_msg void SetWRLPath(LPCTSTR lpszNewValue);
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
	dispidWRLPath = 3L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__
