//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Design"
//! userlevel =  Normal
//! file      = "Control/VTStrucVisCtl.h"
//! author    = "Warren Moore"
//! date      = "5/3/2002"
//! rcsid     = "$Id: VTStrucVisCtl.h,v 1.28 2002/04/25 15:44:06 vap-warren Exp $"

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

//: ActiveX control application body
// Central class for the integration of data access, control properties and user interface behaviour

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

// Scale factor limits
#define SF_MAX                50.0f
#define SF_MIN                 1.0f
#define SF_STEPS              49

class CVTStrucVisCtl : public COleControl {
//:-----
//: Construction/Destruction

	DECLARE_DYNCREATE(CVTStrucVisCtl)
public:

	CVTStrucVisCtl();
   //: Dynamic construction

public:

//:-----
//: Asynchronous data-driven functions

   void UILoading();
   // Called by CUIDataPath to indicate that the data is loading

   void UILoaded();
   // Called by CUIDataPath to indicate that the data has loaded

   void SimLoading();
   // Called by CSimDataPath to indicate that the data is loading

   void SimLoaded();
   // Called by CSimDataPath to indicate that the data has loaded

   bool SceneSetup(const unsigned char *pucData, const unsigned int uiLength);
   //: Set up the scene manager
   // Passes loaded data into the scene until the scene manager has enough data to go interactive
   //!param: pucData - Pointer to supplied data
   //!param: uiLength - Size of supplied data in bytes
   //!return: true if the scene is capable of going interactive

   bool ShowFrame(const unsigned int uiFrame);
   //: Show a frame
   // Called by user interface actions to display a frame
   //!param: uiFrame - Number of frame (zero-based) to display
   //!return: true if the frame was displayed immediately

   void ShowFrame(const unsigned int uiFrame, const unsigned char *pucData, const unsigned int uiLength);
   //: Show a frame from the supplied data
   // Called by the simulation data path to display a frame from the supplied data.
   // Data is passed to the scene manager
   //!param: uiFrame - Number of frame (zero-based) to display
   //!param: pucData - Pointer to supplied data
   //!param: uiLength - Size of supplied data in bytes

protected:

//:-----
//: Cortona functions

   bool InitCortona();
   //: Initialise the Cortona control
   // Gets a handle to the Cortona control, creates the scene manager and initialises 
   // the scene with the local parameters
   //!return: true if control and scene are initialised correctly

   void ExitCortona();
   //: Release the Cortona control and scene

   bool GetCortona();
   //: Find the local Cortona control
   // Queries the local ActiveX container and searches for the first available Cortona control
   //!return: true if found and attached

//:-----
//: Rendering Functions

   void DrawPlaceholder(CDC *pDC, const CRect &rcBounds, bool bRun);
   //: Draw the user interface placeholder
   // Displays the logo and and current control status if running and interactive, but just logo if not
   //!param: pDC - Pointer to client display context
   //!param: rcBounds - Size of display context
   //!param: bRun - True if the control is running and interactive

   void DrawUI(CDC *pDC, const CRect &rcBounds, bool bRun);
   //: Draw the VCR style user interface
   //!param: pDC - Pointer to client display context
   //!param: rcBounds - Size of display context
   //!param: bRun - True if the control is running and interactive

//:-----
//: UI Functions

   void GoInteractive();
   //: Prepare the control to go interactive
   // Called by SceneSetup when enough data is present to go interactive

   bool LoadBitmap();
   //: Loads the bitmap from the user interface data path
   // Sets m_eUIResult to indicate success or failure of load
   //!return: true if successful

   bool FrameControl(unsigned int &uiFrame);
   //: Update the frame control
   // Called by the animation timer to update the frame dependent on user interface actions
   //!param: uiFrame - Reference to the updated frame number
   //!return: true if a user interface element or frame has changed

   unsigned int NextFrame(const unsigned int uiFrame);
   //: Get the next available frame between the target and current frames
   //!param: uiFrame - Desired frame number

   void ButtonControl(unsigned int &uiFrame);
   //: Update the user interface button states
   // Called by mouse button actions to update the user interface
   //!param: uiFrame - Reference to the updated frame number

   bool Interactive();
   //: Check control parameters to see if we're interactive
   //!return: true if interactive

   bool Running();
   //: Check control parameters to see if we're running
   // Control can be running or in development mode
   //!return: true if running

   bool CheckSliders(const CPoint oPoint);
   //: Check the virtual user interface sliders
   //!param: oPoint - Mouse coordinates
   //!return: true if a user interface element has changed

   bool UpdateSliders(unsigned int &uiFrame);
   //: Update the control with the slider values
   //!param: uiFrame - Reference to the updated frame number
   //!return: true if a user interface element or frame has changed

   void Update(bool bStart);
   //: Called to indicate the start of a frame update
   // Sorts variables and freezes display
   //!param: bStart - true at start of frame, false at end

//:-----
//: Private Data Types
protected:

   typedef enum TECortonaResult {
      CR_UNKNOWN = 0,               // Unknown
      CR_NOCONTAINER,               // Unable to connect to control container
      CR_NOENUMOBJECTS,             // Container does not support EnumObjects
      CR_NOCONTROL,                 // Cannot find Cortona control within container
      CR_OK,                        // Cortona initialised OK
   } ECortonaResult;
   // Cortona error states

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
   // UI data error states

   typedef enum TESimResult {
      SD_UNKNOWN = 0,               // Unknown
      SD_ERROR,                     // Set when all data is loaded, yet setup not completed
      SD_OK,                        // Sim data loaded OK
   } ESimResult;
   // Simulation data error states

   typedef enum TERunMode {
      RM_PAUSE = 0,                 // Not running
      RM_PLAY,                      // Playing
      RM_PLAYREV,                   // Reverse playing
      RM_REWIND,                    // Rewind
      RM_FASTFORWARD,               // Fast forward
   } ERunMode;
   // Player run mode

   const static int m_piAnimSpeed[5];
   // Array of timer speeds for the five speed settings (in milliseconds)

   //: A virtual slider class

   class CSlider {
   public:

      CSlider();
      //: Constructor

      void SetPos(const unsigned int uiX, const unsigned int uiY);
      //: Set the on-screen position of the slider

      void SetSize(const unsigned int uiW, const unsigned int uiH);
      //: Set the screen size of the slider

      void SetLimitI(const int iLow, const int iHigh, const int iVal);
      //: Set the integer limits and current position of the slider

      void SetLimitUI(const unsigned uiLow, const unsigned int uiHigh, const unsigned int uiVal);
      //: Set the unsigned integer limits and current position of the slider

      void SetLimitF(const float fLow, const float fHigh, const float fVal);
      //: Set the float limits and current position of the slider

      void SetSteps(const unsigned int uiSteps);
      //: Sets the number of steps for the slider
      // This is set automatically for integers and unsigned integers

      bool GetPosition(const CPoint oPoint);
      //: Update the slider value from the mouse coordinates
      //!param: oPoint - Mouse coordinates
      //!return: true if the slider value has changed

      float GetPosition() const;
      //: Get the slider position
      // Returns the positional value set by the last GetPosition call
      //!return: Value between 0.0 and 1.0

      int GetInt();
      //: Get the integer value of the slider
      // Returns the slider value set by the last GetPosition call
      //!return: Integer value between m_iLow and m_iHigh

      unsigned int GetUnsignedInt();
      //: Get the unsigned integer value of the slider
      // Returns the slider value set by the last GetPosition call
      //!return: Unsigned integer value between m_uiLow and m_uiHigh

      float GetFloat();
      //: Get the float value of the slider
      // Returns the slider value set by the last GetPosition call
      //!return: Float value between m_fLow and m_fHigh

   protected:

      unsigned int m_uiX, m_uiY, m_uiW, m_uiH;     //: Size vars
      unsigned int m_uiSteps;                      //: Slider steps
      float m_fPos;                                //: Current float position
      int m_iLow, m_iHigh;                         //: Int limits
      unsigned m_uiLow, m_uiHigh;                  //: Unsigned int limits
      float m_fLow, m_fHigh;                       //: Float limits

   };

protected:

   /* Control properties */
   ECortonaResult m_eCortonaResult;          //: GetCortona result
   EUIResult m_eUIResult;                    //: UIDataPath result
   ESimResult m_eSimResult;                  //: SimDataPath result
   unsigned int m_uiAsyncFlags;              //: Asynchronous data flags
   CString m_oWRLPath;                       //: Scene WRL path
   CString m_oPosition;                      //: Viewpoint position
   CString m_oOrientation;                   //: Viewpoint orientation
   CString m_oScale;                         //: Scale factor string

   /* UI vars */
   bool m_bLButtonDown;                      //: Left mouse button down indicator
   bool m_bMouseOver;                        //: Mouse over control indicator
   int m_iUIZone;                            //: Current remote button under mouse (-1 if none)
   unsigned int m_uiFrame;                   //: Current animation frame
   ERunMode m_eRunMode;                      //: Current run mode
   bool m_bRunning;                          //: Animation running indicator
   bool m_bLoop;                             //: Looping play indicator
   bool m_bStep;                             //: Single step indicator
   bool m_bStressColour;                     //: Stress colouring indicator
   unsigned int m_uiUITab;                   //: UI tab marker
   float m_fXScale, m_fYScale, m_fZScale;    //: Scale vars
   unsigned int m_uiAnimSpeed;               //: Animation speed (5 = fast, 1 = slow);
   unsigned int m_uiStartGroup;              //: Start group for UI list box
   bool *m_pbVisibleGroup;                   //: Array of visible groups

   /* Slider vars */
   CSlider m_oSFrame;                        //: Frame counter slider
   CSlider m_oSXScale;                       //: X scale slider
   CSlider m_oSYScale;                       //: Y scale slider
   CSlider m_oSZScale;                       //: Z scale slider
   CSlider m_oSAnimSpeed;                    //: Animation speed slider

   /* Render buffers */
   CBitmap m_oBackBuffer;                    //: Screen back buffer
   CSize m_oBufferSize;                      //: Current back buffer size
   CBitmap m_oUIBitmap;                      //: UI bitmap object
   
   /* Control objects */
   CCortonaControl m_oCortona;               //: Cortona control manager
   CUIDataPath m_oUIData;                    //: Asynchronous UI data file
   CSimDataPath m_oSimData;                  //: Asynchronous simulation data file
   CCortonaUtil *m_poCortonaUtil;            //: Cortona Utility class
   CSceneManager *m_poScene;                 //: Scene manager

//:-----
//: Windows Mappings

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

	//{{AFX_MSG(CVTStrucVisCtl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//{{AFX_DISPATCH(CVTStrucVisCtl)
	afx_msg BSTR GetSimData();
	afx_msg void SetSimData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetUIData();
	afx_msg void SetUIData(LPCTSTR lpszNewValue);
	afx_msg BSTR GetWRLPath();
	afx_msg void SetWRLPath(LPCTSTR lpszNewValue);
	afx_msg BSTR GetPosition();
	afx_msg void SetPosition(LPCTSTR lpszNewValue);
	afx_msg BSTR GetOrientation();
	afx_msg void SetOrientation(LPCTSTR lpszNewValue);
	afx_msg BSTR GetScale();
	afx_msg void SetScale(LPCTSTR lpszNewValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();
   //: Call to display About box

	//{{AFX_EVENT(CVTStrucVisCtl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

public:

	enum {
	//{{AFX_DISP_ID(CVTStrucVisCtl)
   dispidSimData = 1L,
   dispidUIData = 2L,
	dispidWRLPath = 3L,
	dispidPosition = 4L,
	dispidOrientation = 5L,
	dispidScale = 6L,
	//}}AFX_DISP_ID
	};
   //: Dispatch ID's

};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_CONTROL__
