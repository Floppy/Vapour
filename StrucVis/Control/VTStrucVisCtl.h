//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.h,v 1.20 2002/04/02 13:13:22 vap-warren Exp $

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

   bool SceneSetup(const unsigned char *pucData, const unsigned int uiLength);
   // Passes loaded data into the scene, when true is returned

   bool ShowFrame(const unsigned int uiFrame);
   // Displays the chosen frame

   void ShowFrame(const unsigned int uiFrame, const unsigned char *pucData, const unsigned int uiLength);
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

   bool FrameControl(unsigned int &uiFrame);
   // Update frame controller dependent on UI states, returns true if changed

   unsigned int NextFrame(const unsigned int uiFrame);
   // Returns the next available frame from current to the specified

   void ButtonControl(unsigned int &uiFrame);
   // Update the button states when UI event happens

   bool Interactive();
   // Checks all possible conditions to see if we are interactive

   bool Running();
   // Check whether we are in run or dev mode, returns true if running

   bool CheckSliders(const CPoint oPoint);
   // Checks the sliders for activation, returns true if a slider was set

   bool UpdateSliders(unsigned int &uiFrame);
   // Updates the UI values with the slider values, returning true if a value has changed

   void Update(bool bStart);
   // Freezes the display if start is true, unfreezes otherwise

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

   // Animation speeds
   const static int m_piAnimSpeed[5];

   // A virtual slider class
   class CSlider {

   //#===--- Member Functions
   public:

      CSlider();
      // Constructor

      void SetPos(const unsigned int uiX, const unsigned int uiY);
      // Set the position of the slider

      void SetSize(const unsigned int uiW, const unsigned int uiH);
      // Set the size of the slider

      void SetLimitI(const int iLow, const int iHigh, const int iVal);
      // Set the int limits

      void SetLimitUI(const unsigned uiLow, const unsigned int uiHigh, const unsigned int uiVal);
      // Set the unsigned int limits

      void SetLimitF(const float fLow, const float fHigh, const float fVal);
      // Set the float limits

      void SetSteps(const unsigned int uiSteps);
      // Sets the number of steps for the slider (set automatically for ints and unsigned ints)

      bool GetPosition(const CPoint oPoint);
      // Returns the true if the point is within the slider

      float GetPosition() const;
      // Returns the value set by the last GetPosition(const CPoint) call

      int GetInt();
      // Returns the int value (GetPosition must be called to update value)

      unsigned int GetUnsignedInt();
      // Returns the unsigned int value (GetPosition must be called to update value)

      float GetFloat();
      // Returns the float value (GetPosition must be called to update value)

   //#===--- Internal Variables
   protected:

      unsigned int m_uiX, m_uiY, m_uiW, m_uiH;     // Size vars
      unsigned int m_uiSteps;                      // Slider steps
      float m_fPos;                                //
      int m_iLow, m_iHigh;                         // Int limits
      unsigned m_uiLow, m_uiHigh;                  // Unsigned int limits
      float m_fLow, m_fHigh;                       // Float limits
   };

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
   ERunMode m_eRunMode;                      // Current run mode
   bool m_bRunning;                          // Animation running indicator
   bool m_bLoop;                             // Looping play indicator
   bool m_bStep;                             // Single step indicator
   bool m_bStressColour;                     // Stress colouring indicator
   unsigned int m_uiUITab;                   // UI tab marker
   float m_fXScale, m_fYScale, m_fZScale;    // Scale vars
   unsigned int m_uiAnimSpeed;               // Animation speed (5 = fast, 1 = slow);
   unsigned int m_uiStartGroup;              // Start group for UI list box
   bool *m_pbVisibleGroup;                   // Array of visible groups

   // Slider vars
   CSlider m_oSFrame;                        // Frame counter slider
   CSlider m_oSXScale;                       // X scale slider
   CSlider m_oSYScale;                       // Y scale slider
   CSlider m_oSZScale;                       // Z scale slider
   CSlider m_oSAnimSpeed;                    // Animation speed slider

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
	afx_msg BSTR GetPosition();
	afx_msg void SetPosition(LPCTSTR lpszNewValue);
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
	dispidPosition = 4L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__
