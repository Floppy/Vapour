//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/VTStrucVisCtl.cpp"
//! author    = "Warren Moore"
//! date      = "5/3/2002"
//! rcsid     = "$Id: VTStrucVisCtl.cpp,v 1.26 2002/04/03 15:57:12 vap-warren Exp $"

#include "stdafx.h"
#include "VTStrucVis.h"
#include "VTStrucVisCtl.h"
#include "VTStrucVisPpg.h"

#include "category.h"
#include "util.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////
// CVTStrucVisCtl::CSlider

CVTStrucVisCtl::CSlider::CSlider() :
   m_uiX(0),
   m_uiY(0),
   m_uiW(0),
   m_uiH(0),
   m_uiSteps(0),
   m_fPos(0.0f),
   m_iLow(0),
   m_iHigh(0),
   m_uiLow(0),
   m_uiHigh(0),
   m_fLow(0.0f),
   m_fHigh(0.0f)
{
}

void CVTStrucVisCtl::CSlider::SetPos(const unsigned int uiX, const unsigned int uiY) {
   m_uiX = uiX;
   m_uiY = uiY;
}

void CVTStrucVisCtl::CSlider::SetSize(const unsigned int uiW, const unsigned int uiH) {
   m_uiW = uiW;
   m_uiH = uiH;
}

void CVTStrucVisCtl::CSlider::SetLimitI(const int iLow, const int iHigh, const int iVal) {
   if (iHigh > iLow) {
      m_iLow = iLow;
      m_iHigh = iHigh;
      m_uiSteps = iHigh - iLow;
      // Set the initial value
      const int iLVal = (iVal < iLow ? iLow : (iVal > iHigh ? iHigh : iVal));
      m_fPos = (float)(iLVal - iLow);
      m_fPos /= (float)(iHigh - iLow);
   }
}

void CVTStrucVisCtl::CSlider::SetLimitUI(const unsigned uiLow,
                                         const unsigned int uiHigh,
                                         const unsigned int uiVal) {
   if (uiHigh > uiLow) {
      m_uiLow = uiLow;
      m_uiHigh = uiHigh;
      m_uiSteps = uiHigh - uiLow;
      // Set the initial value
      const unsigned int uiLVal = (uiVal < uiLow ? uiLow : (uiVal > uiHigh ? uiHigh : uiVal));
      m_fPos = (float)(uiLVal - uiLow);
      m_fPos /= (float)(uiHigh - uiLow);
   }
}

void CVTStrucVisCtl::CSlider::SetLimitF(const float fLow, const float fHigh, const float fVal) {
   if (fHigh > fLow) {
      m_fLow = fLow;
      m_fHigh = fHigh;
      // Set the initial value
      const float fLVal = (fVal < fLow ? fLow : (fVal > fHigh ? fHigh : fVal));
      m_fPos = fLVal - fLow;
      m_fPos /= fHigh - fLow;
   }
}

void CVTStrucVisCtl::CSlider::SetSteps(const unsigned int uiSteps) {
   m_uiSteps = uiSteps;
}

bool CVTStrucVisCtl::CSlider::GetPosition(const CPoint oPoint) {
   bool bFound = false;
   // Is the point in the box?
   if ((oPoint.x >= m_uiX) && (oPoint.y >= m_uiY) &&
      (oPoint.x < m_uiX + m_uiW) && (oPoint.y < m_uiY + m_uiH)) {
      const float fX = (float)(oPoint.x - m_uiX);
      const float fSize = ((float)m_uiW / (float)m_uiSteps);
      const float fHalfSize = fSize / 2.0f;
      const float fPos = fX / fSize;
      // Go through the steps
      for (unsigned int i = 0; i <= m_uiSteps && !bFound; i++) {
         const float fStep = (float)i * fSize;
         if ((fX >= fStep - fHalfSize) && (fX < fStep + fHalfSize)) {
            m_fPos = fStep / (float)m_uiW;
            bFound = true;
         }
      }
   }
   return bFound;
}

float CVTStrucVisCtl::CSlider::GetPosition() const {
   return m_fPos;
}

int CVTStrucVisCtl::CSlider::GetInt() {
   const int fStep = (float)(m_iHigh - m_iLow);
   const float fVal = (fStep * m_fPos) + (float)m_uiLow;
   double dInt = 0;
   return (int)(modf((double)fVal, &dInt) >= 0.5 ? ceil((double)fVal) : floor((double)fVal));
}

unsigned int CVTStrucVisCtl::CSlider::GetUnsignedInt() {
   const int fStep = (float)(m_uiHigh - m_uiLow);
   const float fVal = (fStep * m_fPos) + (float)m_uiLow;
   double dInt = 0;
   return (unsigned int)(modf((double)fVal, &dInt) >= 0.5 ? ceil((double)fVal) : floor((double)fVal));
}

float CVTStrucVisCtl::CSlider::GetFloat() {
   const float fStep = m_fHigh - m_fLow;
   return (fStep * m_fPos) + m_fLow;
}

///////////////////
// CVTStrucVisCtl

const int CVTStrucVisCtl::m_piAnimSpeed[5] = {
   5000,
   2000,
   1000,
   500,
   30
};

IMPLEMENT_DYNCREATE(CVTStrucVisCtl, COleControl)

// Message map
BEGIN_MESSAGE_MAP(CVTStrucVisCtl, COleControl)
	//{{AFX_MSG_MAP(CVTStrucVisCtl)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// Dispatch map
BEGIN_DISPATCH_MAP(CVTStrucVisCtl, COleControl)
	//{{AFX_DISPATCH_MAP(CVTStrucVisCtl)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "SimData", GetSimData, SetSimData, VT_BSTR)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "UIData", GetUIData, SetUIData, VT_BSTR)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "WRLPath", GetWRLPath, SetWRLPath, VT_BSTR)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "Position", GetPosition, SetPosition, VT_BSTR)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "Orientation", GetOrientation, SetOrientation, VT_BSTR)
	DISP_PROPERTY_EX(CVTStrucVisCtl, "Scale", GetScale, SetScale, VT_BSTR)
	DISP_STOCKFUNC_REFRESH()
	DISP_STOCKPROP_READYSTATE()
	DISP_STOCKPROP_BACKCOLOR()
	DISP_STOCKPROP_FORECOLOR()
	DISP_STOCKPROP_CAPTION()
	DISP_STOCKPROP_ENABLED()
	DISP_STOCKPROP_APPEARANCE()
	DISP_STOCKPROP_BORDERSTYLE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CVTStrucVisCtl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Event map
BEGIN_EVENT_MAP(CVTStrucVisCtl, COleControl)
	//{{AFX_EVENT_MAP(CVTStrucVisCtl)
	EVENT_STOCK_READYSTATECHANGE()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

// Property pages
// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVTStrucVisCtl, 2)
	PROPPAGEID(CVTStrucVisPropPage::guid)
   PROPPAGEID(CLSID_CColorPropPage)
END_PROPPAGEIDS(CVTStrucVisCtl)


// Initialize class factory and guid
IMPLEMENT_OLECREATE_EX(CVTStrucVisCtl, "VTStructVis.Control.1",
	0xad64b056, 0xe5e7, 0x4c45, 0x92, 0x43, 0x75, 0x58, 0x2, 0xd7, 0xcb, 0xf7)

// Type library ID and version
IMPLEMENT_OLETYPELIB(CVTStrucVisCtl, _tlid, _wVerMajor, _wVerMinor)

// Interface IDs
const IID BASED_CODE IID_DVTStrucVis =
		{ 0xc9ae69bb, 0x64d8, 0x46cb, { 0xb4, 0xf5, 0x49, 0x48, 0x3d, 0x21, 0x17, 0x44 } };
const IID BASED_CODE IID_DVTStrucVisEvents =
		{ 0xec14f800, 0x845f, 0x4b98, { 0x99, 0x5, 0xd1, 0xd1, 0xa1, 0xd1, 0x26, 0x7f } };


// Control type information
static const DWORD BASED_CODE _dwVTStrucVisOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CVTStrucVisCtl, IDS_VTSTRUCVIS, _dwVTStrucVisOleMisc)

// Adds or removes system registry entries for CVTStrucVisCtl
BOOL CVTStrucVisCtl::CVTStrucVisCtlFactory::UpdateRegistry(BOOL bRegister) {

   if (bRegister) {
      // Regsiter as a control
      CreateComponentCategory(CATID_Control,
                              L"Controls");
      RegisterCLSIDInCategory(m_clsid, CATID_Control);
      // Register safe for initialising
      CreateComponentCategory(CATID_SafeForInitializing,
                              L"Controls safely initializable from persistent data");
      RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);
      // Register safe for scripting
      CreateComponentCategory(CATID_SafeForScripting,
                              L"Controls that are safely scriptable");
      RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);
      // Register supports initialise via property bag
      CreateComponentCategory(CATID_PersistsToPropertyBag,
                              L"Support initialize via PersistPropertyBag");
      RegisterCLSIDInCategory(m_clsid, CATID_PersistsToPropertyBag);
      // Register requires data path host
      CreateComponentCategory(CATID_RequiresDataPathHost,
                              L"Requires Data Path Host");
      RegisterCLSIDInReqCategory(m_clsid, CATID_RequiresDataPathHost);
      // Register Internet aware
      CreateComponentCategory(CATID_InternetAware,
                              L"Internet-Aware");
      RegisterCLSIDInCategory(m_clsid, CATID_InternetAware);
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_VTSTRUCVIS,
			IDB_VTSTRUCVIS,
			afxRegInsertable | afxRegApartmentThreading,
			_dwVTStrucVisOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
   }
   else {
      // Unregister the categories
      UnregisterCLSIDInCategory(m_clsid, CATID_Control);
      UnregisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);
      UnregisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);
      UnregisterCLSIDInCategory(m_clsid, CATID_PersistsToPropertyBag);
      UnregisterCLSIDInReqCategory(m_clsid, CATID_RequiresDataPathHost);
      UnregisterCLSIDInCategory(m_clsid, CATID_InternetAware);
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
   }
}

// Constructor
CVTStrucVisCtl::CVTStrucVisCtl() :
   m_poCortonaUtil(NULL),
   m_poScene(NULL),
   m_eCortonaResult(CR_UNKNOWN),
   m_eUIResult(UI_UNKNOWN),
   m_eSimResult(SD_UNKNOWN),
   m_uiAsyncFlags(AD_EMPTY),
   m_bLButtonDown(false),
   m_bMouseOver(false),
   m_iUIZone(-1),
   m_oBufferSize(0, 0),
   m_uiFrame(0),
   m_eRunMode(RM_PAUSE),
   m_bLoop(false),
   m_bStep(false),
   m_bStressColour(false),
   m_uiUITab(0),
   m_bRunning(false),
   m_fXScale(SF_MIN), 
   m_fYScale(SF_MIN), 
   m_fZScale(SF_MIN),
   m_uiAnimSpeed(5),
   m_uiStartGroup(0),
   m_pbVisibleGroup(NULL) {

   InitializeIIDs(&IID_DVTStrucVis, &IID_DVTStrucVisEvents);

   // Set the initial state to loading
	m_lReadyState = READYSTATE_LOADING;

   // Set the data path controls
   m_oSimData.SetControl(this);
   m_oUIData.SetControl(this);

   // Set the initial window size
   SetInitialSize(180, 250);

   // Frame slider
   m_oSFrame.SetPos(0, 40);
   m_oSFrame.SetSize(180, 10);

   // X scale slider
   m_oSXScale.SetPos(0, 185);
   m_oSXScale.SetSize(180, 10);
   m_oSXScale.SetLimitF(SF_MIN, SF_MAX, m_fXScale);
   m_oSXScale.SetSteps(SF_STEPS);

   // Y scale slider
   m_oSYScale.SetPos(0, 210);
   m_oSYScale.SetSize(180, 10);
   m_oSYScale.SetLimitF(SF_MIN, SF_MAX, m_fYScale);
   m_oSYScale.SetSteps(SF_STEPS);

   // Z scale slider
   m_oSZScale.SetPos(0, 235);
   m_oSZScale.SetSize(180, 10);
   m_oSZScale.SetLimitF(SF_MIN, SF_MAX, m_fZScale);
   m_oSZScale.SetSteps(SF_STEPS);

   // Animation speed slider
   m_oSAnimSpeed.SetPos(0, 185);
   m_oSAnimSpeed.SetSize(180, 10);
   m_oSAnimSpeed.SetLimitUI(1, 5, 5);
}

// Destructor
CVTStrucVisCtl::~CVTStrucVisCtl() {
   ExitCortona();
}

/////////////////////
// Member Functions

bool CVTStrucVisCtl::InitCortona() {
   // Initialise Cortona
   if (!m_oCortona.Attached()) {
      m_eCortonaResult = CR_UNKNOWN;
      if (GetCortona()) {
         // Found the control, so initialise it
         m_oCortona.NavBar(false);
         m_oCortona.ContextMenu(true);
         m_oCortona.Headlight(true);
         m_oCortona.Edit();
         m_oCortona.Refresh();

         // Create the utility class
         m_poCortonaUtil = (CCortonaUtil*) new CCortonaUtil(m_oCortona.GetEngine());

         // Create the scene manager
         m_poScene = (CSceneManager*) new CSceneManager(m_poCortonaUtil);

         // Set the WRL path
         m_poScene->SetBaseURL(m_oWRLPath);

         // Set the viewpoint params
         float pfPosition[3] = {0.0f, 0.0f, 0.0f};
         float pfOrientation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
         if ((ParseFloat(m_oPosition, 3, pfPosition) == 3) &&
             (ParseFloat(m_oOrientation, 4, pfOrientation) == 4)) {
            m_poScene->SetViewpoint(pfPosition, pfOrientation);
         }

         // Set scale factor
         float pfScale[3] = {0.0f, 0.0f, 0.0f};
         if ((ParseFloat(m_oScale, 3, pfScale) == 3)) {
            // Store values within limits
            m_fXScale = (pfScale[0] > SF_MAX ? SF_MAX : (pfScale[0] < SF_MIN ? SF_MIN : pfScale[0]));
            m_fYScale = (pfScale[1] > SF_MAX ? SF_MAX : (pfScale[1] < SF_MIN ? SF_MIN : pfScale[1]));
            m_fZScale = (pfScale[2] > SF_MAX ? SF_MAX : (pfScale[2] < SF_MIN ? SF_MIN : pfScale[2]));
            // Update the sliders
            m_oSXScale.SetLimitF(SF_MIN, SF_MAX, m_fXScale);
            m_oSYScale.SetLimitF(SF_MIN, SF_MAX, m_fYScale);
            m_oSZScale.SetLimitF(SF_MIN, SF_MAX, m_fZScale);
         }
         m_poScene->SetScaleFactor(m_fXScale, m_fYScale, m_fZScale);

         // Set colour scheme
         m_poScene->SetColourScheme(COLOUR_GROUP);
      }
   }

   // Repaint
   InvalidateControl();

   return (m_eCortonaResult == CR_OK);
}

void CVTStrucVisCtl::ExitCortona() {
   // Delete the scene manager
   if (m_poScene) {
      delete m_poScene;
      m_poScene = NULL;
   }
   // Delete the Cortona utility class
   if (m_poCortonaUtil) {
      delete m_poCortonaUtil;
      m_poCortonaUtil = NULL;
   }
   // Release Cortona
   m_eCortonaResult = CR_UNKNOWN;
   if (m_oCortona.Attached())
      m_oCortona.Release();
}

bool CVTStrucVisCtl::GetCortona() {
   m_eCortonaResult = CR_UNKNOWN;
   LPOLECONTAINER pContainer = NULL;
   // Get a pointer to the IOleItemContainer interface
   HRESULT hResult = GetClientSite()->GetContainer(&pContainer);
   if (SUCCEEDED(hResult)) {
      // Set the types of objects to enum
      DWORD dwFlags = OLECONTF_ONLYIFRUNNING |
                      OLECONTF_EMBEDDINGS |
                      OLECONTF_ONLYUSER;
      LPENUMUNKNOWN pEnumUnknown = NULL;
      // Enumerate away!
      hResult = pContainer->EnumObjects(dwFlags, &pEnumUnknown);
      if (SUCCEEDED(hResult)) {
         // What have we got?
         LPUNKNOWN pNextControl = NULL;
         bool bNext = true;
         while (bNext && SUCCEEDED(pEnumUnknown->Next(1, &pNextControl, NULL)) && pNextControl) {
            // Get the OLE object interface
            LPOLEOBJECT pOleObject = NULL;
            hResult = pNextControl->QueryInterface(IID_IOleObject, (LPVOID*)&pOleObject);
            bNext = SUCCEEDED(hResult);
            if (bNext) {
               // Get the CLSID
               CLSID clsid;
               hResult = pOleObject->GetUserClassID(&clsid);
               if (SUCCEEDED(hResult)) {
                  // Check for Cortona
            		static CLSID const IID_CortonaControl
			            = { 0x86a88967, 0x7a20, 0x11d2, { 0x8e, 0xda, 0x0, 0x60, 0x8, 0x18, 0xed, 0xb1 } };
                  if (clsid == IID_CortonaControl) {
                     // Get the Cortona Engine interface
                     if (m_oCortona.Attach(pOleObject)) {
                        TRACE("Found Cortona control\n");
                        m_eCortonaResult = CR_OK;
                        bNext = false;
                     }
                  }
#ifdef _DEBUG
                  else {
                     // Trace the found controls
                     LPOLESTR pClsid = NULL;
                     StringFromCLSID(clsid, &pClsid);
                     TRACE("Found control CLSID: %s\n", pClsid);
                  }
#endif // _DEBUG
               }
               // Release the OLE object
               pOleObject->Release();
            }
            pNextControl->Release();
         }
         // Release the EnumUnknown interface
         pEnumUnknown->Release();
      }
      else
         m_eCortonaResult = CR_NOENUMOBJECTS;
      // Release the container interface
      pContainer->Release();
   }
   else
      m_eCortonaResult = CR_NOCONTAINER;

   // If no other errors encountered, but we don't have a control, it can't be there
   bool bFound = m_oCortona.Attached();
   if (m_eCortonaResult == CR_OK && !bFound)
      m_eCortonaResult = CR_NOCONTROL;

   return bFound;
}

void CVTStrucVisCtl::DrawPlaceholder(CDC *pDC, const CRect &rcBounds, bool bRun) {
   // Set the background white
   CBrush oBrush;
   oBrush.CreateSolidBrush(TranslateColor(RGB(0xFF, 0xFF, 0xFF)));
   pDC->FillRect(rcBounds, &oBrush);

   // Set the draw height
   CRect oTextRect(rcBounds);

   // Draw the icon
   HICON hIcon = AfxGetApp()->LoadIcon(IDI_ABOUTDLL);
   if (hIcon) {
      pDC->DrawIcon(CPoint((rcBounds.Width() / 2) - 16, 0), hIcon);
      oTextRect.SetRect(oTextRect.left, oTextRect.top + 32, oTextRect.right, oTextRect.bottom);
   }

   // Store the old font and select the stock font
   pDC->SetBkMode(TRANSPARENT);
   CFont *pOldFont = SelectStockFont(pDC);
   // Set the text colour on the enabled state
   pDC->SetTextColor(TranslateColor(GetEnabled() ? RGB(0x00, 0x00, 0x00): RGB(0xC0, 0xC0, 0xC0)));

   // Display the title text
   const char pcTitle[] = "Vapour Technology\nArup Visualisation Control\n";
   pDC->DrawText(pcTitle, oTextRect, DT_CENTER | DT_WORDBREAK);

   // Calculate the remaining text room
   int iTextHeight = pDC->DrawText(pcTitle, oTextRect, DT_CENTER | DT_WORDBREAK);
   oTextRect.SetRect(oTextRect.left, oTextRect.top + iTextHeight, oTextRect.right, oTextRect.bottom);

   if (bRun) {
      //#===--- Asynchronous data status
      CString oStr = "";
      // If the UI data is in error
      if ((m_eUIResult == UI_UNKNOWN) || (m_eUIResult == UI_OK)) {
         // Is the UI loading?
         if ((m_uiAsyncFlags & AD_UILOADING) > 0)
            oStr += "UI data loading\n";
         else
            // Is the UI loaded?
            if ((m_uiAsyncFlags & AD_UILOADED) > 0)
               oStr += "UI data loaded\n";
      }

      // If the sim data is in error
      if ((m_eSimResult == SD_UNKNOWN) || (m_eSimResult == SD_OK)) {
         // Is the simulation data loading?
         if ((m_uiAsyncFlags & AD_SIMLOADING) > 0)
            oStr += "Simulation data loading\n";
         else
            // Is the simulation data loaded?
            if ((m_uiAsyncFlags & AD_SIMLOADED) > 0)
               oStr += "Simulation data loaded\n";
      }

      // Display the status text
      pDC->DrawText(oStr, oTextRect, DT_CENTER | DT_WORDBREAK);

      // Calculate the remaining text room
      iTextHeight = pDC->DrawText(oStr, oTextRect, DT_CENTER | DT_WORDBREAK);
      oTextRect.SetRect(oTextRect.left, oTextRect.top + iTextHeight, oTextRect.right, oTextRect.bottom);

      //#===--- Errors
      // Check the Cortona result
      oStr = "";
      switch (m_eCortonaResult) {
         case CR_NOCONTAINER:
            oStr = "Unable to query container\n";
            break;
         case CR_NOENUMOBJECTS:
            oStr = "Unable to query other containers\n";
            break;
         case CR_NOCONTROL:
            oStr = "No Cortona control found\n";
            break;
         case CR_OK:
            break;
         case CR_UNKNOWN:
         default:
            oStr = "Cortona status unknown\n";
      }

      // Check to see if the UI data param set
      if (m_oUIData.GetPath() == "")
         oStr += "UIData param not set\n";
      else
         // Data set but is it loading?
         if ((m_uiAsyncFlags & AD_UIACTIVE) == 0)
            oStr += "Error loading UI data\n";
         else
            // Has an error condition been reached?
            switch (m_eUIResult) {
               case UI_NOTSET:
                  oStr += "UIData param not set\n";
                  break;
               case UI_NOTLOADED:
                  oStr += "UI data not loaded\n";
                  break;
               case UI_DATAERROR:
                  oStr += "Error loading UI data\n";
                  break;
               case UI_NOTBITMAP:
                  oStr += "UI data not a Windows bitmap\n";
                  break;
               case UI_WRONGDEPTH:
                  oStr += "UI data not 24 bits per pixel\n";
                  break;
               case UI_WRONGTYPE:
                  oStr += "Wrong UI data image type\n";
                  break;
               case UI_WRONGFORMAT:
                  oStr += "Wrong UI data image format\n";
                  break;
               // Result OK or unknown yet
               case UI_UNKNOWN:
               case UI_OK:
                  break;
               // Unknown result
               default:
                  oStr += "Unknown UI data error\n";
                  break;
            }

      // Check to see if the simulation data param set
      if (m_oSimData.GetPath() == "")
         oStr += "SimData param not set\n";
      else
         // Data set but is it loading?
         if ((m_uiAsyncFlags & AD_SIMACTIVE) == 0)
            oStr += "Error loading simulation data\n";
         else
            // Has an error condition been reached?
            switch (m_eSimResult) {
               // Setup not found and all data in
               case SD_ERROR:
                  oStr += "No simulation data found\n";
                  break;
               // Result OK or unknown yet
               case SD_UNKNOWN:
               case SD_OK:
                  break;
               // Unknown result
               default:
                  oStr += "Unknown simulation data error\n";
                  break;
            }

      // Check to see that the WRL path is set
      if (m_oWRLPath == "")
         oStr += "WRL path not set\n";

      // Display the error text
      pDC->SetTextColor(TranslateColor(GetEnabled() ? RGB(0xFF, 0x00, 0x00) : RGB(0xFF, 0xC0, 0xC0)));
      pDC->DrawText(oStr, oTextRect, DT_CENTER | DT_WORDBREAK);
   }
   // Display the caption text
   else {
      pDC->DrawText(InternalGetText(), oTextRect, DT_CENTER | DT_WORDBREAK);
   }
   // Restore the old font
   pDC->SelectObject(pOldFont);
}

void CVTStrucVisCtl::DrawUI(CDC *pDC, const CRect &rcBounds, bool bRun) {
   // If we haven't started it already, start the timer
   if (!m_bRunning) {
      SetTimer(TI_ANIMATE, m_piAnimSpeed[m_uiAnimSpeed], NULL);
      m_bRunning = true;
   }

   // Create compatible DCs for the remote and the back buffer
	CDC oDCRemote, oDCBuffer;
	oDCRemote.CreateCompatibleDC(pDC);
   oDCBuffer.CreateCompatibleDC(pDC);
   // Check if the render size has changed
   if (m_oBufferSize != rcBounds.Size()) {
      m_oBufferSize = rcBounds.Size();
      // Create the new back buffer
      m_oBackBuffer.DeleteObject();
      m_oBackBuffer.CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
   }
   // Initialise the buffers with bitmaps
   CBitmap *poRemoteBitmap = oDCRemote.SelectObject(&m_oUIBitmap);
   CBitmap *poBufferBitmap = oDCBuffer.SelectObject(&m_oBackBuffer);
   // Set the background to the background colour
   CBrush oBrush;
   oBrush.CreateSolidBrush(TranslateColor(GetBackColor()));
   oDCBuffer.FillRect(rcBounds, &oBrush);

   // Copy the base remote
	oDCBuffer.BitBlt(0, 0, 180, 170, &oDCRemote, 0, 0, SRCCOPY);

   //#===--- Highlight buttons
   // Loop indicator
   if (m_bLoop)
      oDCBuffer.BitBlt(120, 80, 60, 30, &oDCRemote, 300, 80, SRCCOPY);

   // Step indicator
   if (m_bStep)
      oDCBuffer.BitBlt(120, 110, 60, 30, &oDCRemote, 300, 110, SRCCOPY);

   // Colour indicator
   if (m_bStressColour)
      oDCBuffer.BitBlt(0, 140, 180, 15, &oDCRemote, 180, 140, SRCCOPY);

   // Tab top
   if (m_uiUITab < 3)
      oDCBuffer.BitBlt(m_uiUITab * 60, 155, 60, 15, &oDCRemote, 180 + m_uiUITab * 60, 155, SRCCOPY);

   // If we're running and enabled
   if (bRun && GetEnabled()) {

      // Mouse over button
      if (m_bLButtonDown && (m_iUIZone >= 0)) {
         const int iX = (m_iUIZone % 3) * 60;
         const int iY = (m_iUIZone / 3) * 30 + 50;
      	oDCBuffer.BitBlt(iX, iY, 60, 30, &oDCRemote, 180 + iX, iY, SRCCOPY);
      }

      // Current run mode
      int iRunZone = -1;
      switch (m_eRunMode) {
         case RM_PLAY:
            iRunZone = 1;
            break;
         case RM_PLAYREV:
            iRunZone = 0;
            break;
         case RM_PAUSE:
            iRunZone = 2;
            break;
         case RM_REWIND:
            iRunZone = 3;
            break;
         case RM_FASTFORWARD:
            iRunZone = 4;
            break;
         default:
            iRunZone = -1;
      }
      if (iRunZone >= 0) {
         const int iX = (iRunZone % 3) * 60;
         const int iY = (iRunZone / 3) * 30 + 50;
      	oDCBuffer.BitBlt(iX, iY, 60, 30, &oDCRemote, 180 + iX, iY, SRCCOPY);
      }
   }

   //#===--- Frame count bar
   unsigned int uiFrames = m_poScene->NumFrames();
   if (uiFrames > 0) {
      const int iStep = 180 / (uiFrames - 1);
      // Copy the bar
      oDCBuffer.BitBlt(0, 40, iStep * m_oSFrame.GetUnsignedInt(), 10, &oDCRemote, 180, 40, SRCCOPY);
   }

   //#===--- Tabs
   // Visibility Tab
   if (m_uiUITab == 0) {
      // Get the number of groups
      unsigned int uiGroups = m_poScene->NumGroups();
      // Get the size of the remain screen
      CRect oScreen;
      GetClientRect(oScreen);
      unsigned int uiMaxGroups = (oScreen.bottom - 270) / 10;
      // Set the extent
      unsigned int uiViewGroups = uiGroups - m_uiStartGroup;
      if (uiViewGroups > uiMaxGroups)
         uiViewGroups = uiMaxGroups;
      // Paint the group holders
      unsigned int uiOffset = 0;
      for (unsigned int i = 0; i < uiViewGroups; i++) {
         // Move to the last one
         if (i + 1 == uiViewGroups)
            uiOffset += 15;
         // Paint the group holder
         oDCBuffer.BitBlt(0, 170 + i * 15,
                          180, 15,
                          &oDCRemote,
                          (m_pbVisibleGroup[i + m_uiStartGroup] ? 180 : 0), 270 + uiOffset,
                          SRCCOPY);
         // Next off the first one
         if (i == 0)
            uiOffset += 15;
      }
   }

   // Scale Tab
   if (m_uiUITab == 1) {
      // Back
      oDCBuffer.BitBlt(0, 170, 180, 75, &oDCRemote, 0, 170, SRCCOPY);
      // X Scale
      int iVal = (int)(180.0f * m_oSXScale.GetPosition());
      oDCBuffer.BitBlt(0, 185, iVal, 10, &oDCRemote, 180, 185, SRCCOPY);
      // Y Scale
      iVal = (int)(180.0f * m_oSYScale.GetPosition());
      oDCBuffer.BitBlt(0, 210, iVal, 10, &oDCRemote, 180, 210, SRCCOPY);
      // Z Scale
      iVal = (int)(180.0f * m_oSZScale.GetPosition());
      oDCBuffer.BitBlt(0, 235, iVal, 10, &oDCRemote, 180, 235, SRCCOPY);
   }

   // Animate Tab
   if (m_uiUITab == 2) {
      oDCBuffer.BitBlt(0, 170, 180, 25, &oDCRemote, 0, 245, SRCCOPY);
      // Anim speed
      int iVal = (int)(180.0f * m_oSAnimSpeed.GetPosition());
      oDCBuffer.BitBlt(0, 185, iVal, 10, &oDCRemote, 180, 185, SRCCOPY);
   }

   //#===--- Frame counter
   // Set the the DC
   oDCBuffer.SetMapMode(MM_ANISOTROPIC);
   oDCBuffer.SetWindowExt(1440, 1440);
   oDCBuffer.SetViewportExt(oDCBuffer.GetDeviceCaps(LOGPIXELSX), oDCBuffer.GetDeviceCaps(LOGPIXELSY));
   // Set the text map mode
   oDCBuffer.SetBkMode(TRANSPARENT);
   oDCBuffer.SetTextColor(TranslateColor(GetForeColor()));
   // Create and select the font
   CFont oCountFont;
   const int iCountPoints = 20;
   oCountFont.CreateFont(iCountPoints * 20,
                         0,
                         0,
                         0,
                         400,
                         FALSE,
                         FALSE,
                         0,
                         ANSI_CHARSET,
                         OUT_DEFAULT_PRECIS,
                         CLIP_DEFAULT_PRECIS,
                         DEFAULT_QUALITY,
                         DEFAULT_PITCH | FF_SWISS,
                         "Arial");
   CFont *poBufferFont = oDCBuffer.SelectObject(&oCountFont);
   // Create the font text
   CString oText("");
   oText.Format("%d/%d", m_oSFrame.GetUnsignedInt() + 1, uiFrames);
   // Set the coords
   TEXTMETRIC sTM;
   oDCBuffer.GetTextMetrics(&sTM);
   CSize oTextSize = oDCBuffer.GetTextExtent(oText);
   oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
   oDCBuffer.LPtoDP(&oTextSize);
   CSize oDevSize(173 - oTextSize.cx, 7);
   oDCBuffer.DPtoLP(&oDevSize);
   // Output the font
   oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);

   // Tab Text
   CFont oTabFont;
   const int iTabPoints = 10;
   oTabFont.CreateFont(iTabPoints * 20,
                       0,
                       0,
                       0,
                       400,
                       FALSE,
                       FALSE,
                       0,
                       ANSI_CHARSET,
                       OUT_DEFAULT_PRECIS,
                       CLIP_DEFAULT_PRECIS,
                       DEFAULT_QUALITY,
                       DEFAULT_PITCH | FF_SWISS,
                       "Arial");
   oDCBuffer.SelectObject(oTabFont);

   // Visibility tab
   if (m_uiUITab == 0) {
      // Get the number of groups
      unsigned int uiGroups = m_poScene->NumGroups();
      // Get the size of the remain screen
      CRect oScreen;
      GetClientRect(oScreen);
      unsigned int uiMaxGroups = (oScreen.bottom - 270) / 10;
      // Set the extent
      unsigned int uiViewGroups = uiGroups - m_uiStartGroup;
      if (uiViewGroups > uiMaxGroups)
         uiViewGroups = uiMaxGroups;
      // Go through each group
      for (unsigned int i = 0; i < uiViewGroups; i++) {
         // Set the text
         unsigned int uiGroupNum = i + m_uiStartGroup;
         TElementType eType = m_poScene->GroupType(uiGroupNum + 1);
         unsigned int uiElements = m_poScene->NumElements(uiGroupNum + 1);
         oText.Format("%d: %s (%d)", uiGroupNum + 1, (eType == ELEMENT_SLAB ? "Slab" : "Beam"), uiElements);
         // Set the coords
         oDCBuffer.GetTextMetrics(&sTM);
         oTextSize = oDCBuffer.GetTextExtent(oText);
         oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
         oDCBuffer.LPtoDP(&oTextSize);
         oDevSize.cx = 13;
         oDevSize.cy = 171 + i * 15;
         oDCBuffer.DPtoLP(&oDevSize);
         // Output the font
         oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);
      }
   }

   // Scale Tab
   if (m_uiUITab == 1) {
      // X Scale
      oText.Format("%.3f", m_oSXScale.GetFloat());
      // Set the coords
      oDCBuffer.GetTextMetrics(&sTM);
      oTextSize = oDCBuffer.GetTextExtent(oText);
      oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
      oDCBuffer.LPtoDP(&oTextSize);
      oDevSize.cx = 176 - oTextSize.cx;
      oDevSize.cy = 171;
      oDCBuffer.DPtoLP(&oDevSize);
      // Output the font
      oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);
      // Y Scale
      oText.Format("%.3f", m_oSYScale.GetFloat());
      // Set the coords
      oDCBuffer.GetTextMetrics(&sTM);
      oTextSize = oDCBuffer.GetTextExtent(oText);
      oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
      oDCBuffer.LPtoDP(&oTextSize);
      oDevSize.cx = 176 - oTextSize.cx;
      oDevSize.cy = 196;
      oDCBuffer.DPtoLP(&oDevSize);
      // Output the font
      oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);
      // Z Scale
      oText.Format("%.3f", m_oSZScale.GetFloat());
      // Set the coords
      oDCBuffer.GetTextMetrics(&sTM);
      oTextSize = oDCBuffer.GetTextExtent(oText);
      oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
      oDCBuffer.LPtoDP(&oTextSize);
      oDevSize.cx = 176 - oTextSize.cx;
      oDevSize.cy = 221;
      oDCBuffer.DPtoLP(&oDevSize);
      // Output the font
      oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);
   }

   // Animation Tab
   if (m_uiUITab == 2) {
      // Animation speed
      switch (m_oSAnimSpeed.GetUnsignedInt()) {
         case 1:
            oText = "Slowest";
            break;
         case 2:
            oText = "Slow";
            break;
         case 3:
            oText = "Medium";
            break;
         case 4:
            oText = "Fast";
            break;
         case 5:
         default:
            oText = "Fastest";
            break;
      }
      // Set the coords
      oDCBuffer.GetTextMetrics(&sTM);
      oTextSize = oDCBuffer.GetTextExtent(oText);
      oTextSize.cy = sTM.tmHeight + sTM.tmExternalLeading;
      oDCBuffer.LPtoDP(&oTextSize);
      oDevSize.cx = 124;
      oDevSize.cy = 171;
      oDCBuffer.DPtoLP(&oDevSize);
      // Output the font
      oDCBuffer.TextOut(oDevSize.cx, oDevSize.cy, oText);
   }

   // Restore the DC mapping mode
   oDCBuffer.SetMapMode(MM_TEXT);

   // Copy over the back buffer
   pDC->BitBlt(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(), &oDCBuffer, 0, 0, SRCCOPY);

   // Restore the saved objects
   oDCRemote.SelectObject(poRemoteBitmap);
   oDCBuffer.SelectObject(poBufferBitmap);
   oDCBuffer.SelectObject(poBufferFont);
}

bool CVTStrucVisCtl::LoadBitmap() {
   // Check the data object
   if (m_oUIData.GetPath() == "") {
      m_eUIResult = UI_NOTSET;
      return false;
   }
   if ((m_uiAsyncFlags & AD_UILOADED) == 0) {
      m_eUIResult = UI_NOTLOADED;
      return false;
   }
   // Try loading the object
   m_eUIResult = UI_UNKNOWN;
   // Seek to the beginning of the file
   m_oUIData.Seek(0L, CFile::begin);
   // Check the bitmap file header
	BITMAPFILEHEADER bmfh;
   unsigned int uiRead = m_oUIData.Read((void*)&bmfh, sizeof(bmfh));
   // File error checking
   if (uiRead != sizeof(bmfh)) {
      m_eUIResult = UI_DATAERROR;
      return false;
   }
   // Check bitmap type
   if (bmfh.bfType != 0x4d42) { // 'BM'
		m_eUIResult = UI_NOTBITMAP;
      return false;
   }
   if ((bmfh.bfReserved1 != 0) || (bmfh.bfReserved2 != 0)) {
		m_eUIResult = UI_NOTBITMAP;
      return false;
   }
   // Read in the rest of the file
   unsigned int uiLength = m_oUIData.GetLength() - sizeof(bmfh);
   unsigned char *pucData = (unsigned char*) new unsigned char[uiLength];
   if (!pucData) {
      m_eUIResult = UI_DATAERROR;
      return false;
   }
   uiRead = m_oUIData.Read((void*)pucData, uiLength);
   // Get the bitmap structures
   BITMAPINFOHEADER &sBMIHeader = *(LPBITMAPINFOHEADER)pucData;
	BITMAPINFO &sBMInfo = *(LPBITMAPINFO)pucData;
   // Get resolution & orientation
	unsigned int uiWidth = sBMIHeader.biWidth;
	unsigned int uiHeight = abs(sBMIHeader.biHeight);
	bool bTopDown = (sBMIHeader.biHeight < 0);
   // Check the colour depth = 1 bit planes, 24 bpp
   unsigned int uiPlanes = sBMIHeader.biPlanes;
   unsigned int uiBPP = sBMIHeader.biBitCount;
   if ((uiPlanes != 1) || (uiBPP != 24)) {
      m_eUIResult = UI_WRONGDEPTH;
      return false;
   }
   // No compression
   if (sBMIHeader.biCompression != BI_RGB) {
      m_eUIResult = UI_WRONGTYPE;
      return false;
   }
   // No colour table, must be a packed bitmap
   if ((sBMIHeader.biClrUsed != 0) || (sBMIHeader.biClrImportant != 0)) {
      m_eUIResult = UI_WRONGFORMAT;
      return false;
   }
   // Set the data pointer
   LPVOID lpDIBBits = (LPVOID)sBMInfo.bmiColors;
   // Create the dib
   CClientDC oDC(NULL);
	HBITMAP hBmp = CreateDIBitmap(oDC.GetSafeHdc(),    // Device context
                                 &sBMIHeader,         // Pointer to bitmap size and format data 
                                 CBM_INIT,            // Initialization flag 
                                 lpDIBBits,           // Pointer to initialization data 
                                 &sBMInfo,            // Pointer to bitmap colour format data 
                                 DIB_RGB_COLORS);     // Colour data usage
   // Check we have a bitmap
   if (hBmp) {
      m_eUIResult = UI_OK;
      // Attach the bitmap
   	m_oUIBitmap.Attach(hBmp);
   }
   else 
      m_eUIResult = UI_DATAERROR;
   // Delete the loaded file
   delete [] pucData;

	return true;
}

bool CVTStrucVisCtl::FrameControl(unsigned int &uiFrame) {
   uiFrame = m_uiFrame;
   // Check we have a scene manager present
   if (!m_poScene)
      return false;
   // Get the max number of frames
   unsigned int uiFrames = m_poScene->NumFrames();
   // Set the fast forward/ rewind step
   const unsigned int uiStep = 5;
   // Store the current frame and state
   const ERunMode eRunMode = m_eRunMode;
   // Update dependent on the run mode
   switch (m_eRunMode) {
      // PLAY
      case RM_PLAY:
         if (uiFrame + 1 < uiFrames)
            uiFrame++;
         else
            if (m_bLoop)
               uiFrame = 0;
            else
               m_eRunMode = RM_PAUSE;
         if (m_bStep)
            m_eRunMode = RM_PAUSE;
         break;
      // PLAY REVERSE
      case RM_PLAYREV:
         if (uiFrame > 0)
            uiFrame--;
         else
            if (m_bLoop)
               uiFrame = NextFrame(uiFrames - 1);
            else
               m_eRunMode = RM_PAUSE;
         if (m_bStep)
            m_eRunMode = RM_PAUSE;
         break;
      // REWIND
      case RM_REWIND:
         if (m_bStep) {
            m_eRunMode = RM_PAUSE;
            break;
         }
         if (uiFrame >= uiStep)
            uiFrame -= uiStep;
         else
            if (m_bLoop)
               uiFrame = NextFrame(uiFrames - 1);
            else {
               uiFrame = 0;
               m_eRunMode = RM_PAUSE;
            }
         break;
      // FAST FORWARD
      case RM_FASTFORWARD:
         if (m_bStep) {
            m_eRunMode = RM_PAUSE;
            break;
         }
         if (uiFrame + uiStep < uiFrames)
            uiFrame = NextFrame(uiFrame + uiStep);
         else
            if (m_bLoop)
               uiFrame = 0;
            else
               m_eRunMode = RM_PAUSE;
         break;
      // NONE
      default:
         break;
   }
   // Changed if the frame number or the run mode has change
   return ((uiFrame != m_uiFrame) || (eRunMode != m_eRunMode));
}

unsigned int CVTStrucVisCtl::NextFrame(const unsigned int uiFrame) {
   // Set the vars
   unsigned int uiAvailable = uiFrame;
   unsigned int uiSeek = 0, uiLength = 0;
   bool bFound = false;
   while ((uiAvailable != m_uiFrame) && !bFound) {
      m_poScene->FrameInfo(uiAvailable, uiSeek, uiLength);
      if (m_oSimData.Available(uiSeek, uiLength))
         bFound = true;
      else
         uiAvailable += (uiAvailable > m_uiFrame) ? -1 : 1;
   }
   return uiAvailable;
}

void CVTStrucVisCtl::ButtonControl(unsigned int &uiFrame) {
   uiFrame = m_uiFrame;
   // Check we have a scene manager present
   if (!m_poScene)
      return;
   // Update dependent on the position of the mouse
   switch (m_iUIZone) {
      // PLAY
      case 1:
         m_eRunMode = RM_PLAY;
         break;
      // PLAY REVERSE
      case 0:
         m_eRunMode = RM_PLAYREV;
         break;
      // PAUSE
      case 2:
         m_eRunMode = RM_PAUSE;
         break;
      // REWIND
      case 3:
         m_eRunMode = RM_REWIND;
         break;
      // FAST FORWARD
      case 4:
         m_eRunMode = RM_FASTFORWARD;
         break;
      // LOOP
      case 5:
         m_bLoop = !m_bLoop;
         break;
      // GO TO START
      case 6:
         m_eRunMode = RM_PAUSE;
         uiFrame = 0;
         break;
      // GO TO END
      case 7:
         m_eRunMode = RM_PAUSE;
         uiFrame = NextFrame(m_poScene->NumFrames() - 1);
         break;
      // STEP
      case 8:
         m_bStep = !m_bStep;
         break;
      // Unknown
      default:
         break;
   }
}

bool CVTStrucVisCtl::Interactive() {
   // In interactive state
   bool bOk = (GetReadyState() == READYSTATE_INTERACTIVE) || (GetReadyState() == READYSTATE_COMPLETE);
   // UI loaded
   bOk &= (m_eUIResult == UI_OK);
   // Sim data valid
   bOk &= (m_eSimResult == SD_OK);
   // WRL Path set
   bOk &= (m_oWRLPath != "");
   // Scene object created
   bOk &= (m_poScene != NULL);

   return bOk;
}

bool CVTStrucVisCtl::Running() {
   // Check ambient modes
   return (AmbientUserMode() && !AmbientUIDead());
}

bool CVTStrucVisCtl::CheckSliders(const CPoint oPoint) {
   // Don't bother if the mouse isn't down
   if (!m_bLButtonDown)
      return false;

   bool bModified = false;

   // Check the frame slider
   bModified |= m_oSFrame.GetPosition(oPoint);

   // Tabs
   switch (m_uiUITab) {
      // Scale tab
      case 1:
         bModified |= m_oSXScale.GetPosition(oPoint);
         bModified |= m_oSYScale.GetPosition(oPoint);
         bModified |= m_oSZScale.GetPosition(oPoint);
         break;
      // Animation tab
      case 2:
         bModified |= m_oSAnimSpeed.GetPosition(oPoint);
         break;
      default:
         break;
   }

   return bModified;
}

bool CVTStrucVisCtl::UpdateSliders(unsigned int &uiFrame) {
   bool bModified = false;

   // Check the frame
   uiFrame = m_oSFrame.GetUnsignedInt();
   if (uiFrame != m_uiFrame) {
      // Check the new frame is available
      unsigned int uiSeek = 0, uiLength = 0;
      m_poScene->FrameInfo(uiFrame, uiSeek, uiLength);
      // It's available
      if (uiLength > 0 && m_oSimData.Available(uiSeek, uiLength)) {
         bModified = true;
      }
      // Don't go there!
      else
         uiFrame = m_uiFrame;
   }

   // Scale Tab
   if (m_uiUITab == 1) {
      bool bScaleModified = false;
      float fVal = 0.0f;
      // X Scale
      fVal = m_oSXScale.GetFloat();
      if (fVal != m_fXScale) {
         m_fXScale = fVal;
         bScaleModified = true;
      }
      // Y Scale
      fVal = m_oSYScale.GetFloat();
      if (fVal != m_fYScale) {
         m_fYScale = fVal;
         bScaleModified = true;
      }
      // Z Scale
      fVal = m_oSZScale.GetFloat();
      if (fVal != m_fZScale) {
         m_fZScale = fVal;
         bScaleModified = true;
      }
      // Update the scale factor
      if (bScaleModified) {
         Update(true);
         m_poScene->SetScaleFactor(m_fXScale, m_fYScale, m_fZScale);
         Update(false);
         bModified = true;
      }
   }

   // Animation tab
   if (m_uiUITab == 2) {
      m_uiAnimSpeed = m_oSAnimSpeed.GetUnsignedInt();
      // Stop the timer, if it's running
      if (m_bRunning) {
         KillTimer(TI_ANIMATE);
      }
      // Restart the timer with the new animation speed
      SetTimer(TI_ANIMATE, m_piAnimSpeed[m_uiAnimSpeed], NULL);
      m_bRunning = true;
   } 

   return bModified;
}

void CVTStrucVisCtl::Update(bool bStart) {
   // Make sure we have a control
   if (!m_oCortona.Attached())
      return;
   // Freeze
   m_oCortona.Freeze(bStart);
}

void CVTStrucVisCtl::UILoading() {
   // Set the async data flags
   m_uiAsyncFlags &= AD_UIMASK;
   m_uiAsyncFlags |= AD_UILOADING;
   // Set the UI status unknown
   m_eUIResult = UI_UNKNOWN;
   // Refresh the control
   InvalidateControl();
}

void CVTStrucVisCtl::UILoaded() {
   // Set the async data flags
   m_uiAsyncFlags &= AD_UIMASK;
   m_uiAsyncFlags |= AD_UILOADED;
   // Try to get the bitmap
   LoadBitmap();
   // Refresh the control
   InvalidateControl();
}

void CVTStrucVisCtl::SimLoading() {
   // Set the async data flags
   m_uiAsyncFlags &= AD_SIMMASK;
   m_uiAsyncFlags |= AD_SIMLOADING;
   // Refresh the control
   InvalidateControl();
}

void CVTStrucVisCtl::SimLoaded() {
   // Set the async data flags
   m_uiAsyncFlags &= AD_SIMMASK;
   m_uiAsyncFlags |= AD_SIMLOADED;
   // Check the ready state
   if ((m_uiAsyncFlags & AD_UILOADED) > 0)
      InternalSetReadyState(READYSTATE_COMPLETE);
   // Check to see if setup has completed
   if (m_eSimResult != SD_OK)
      m_eSimResult = SD_ERROR;
   // Refresh the control
   InvalidateControl();
}

bool CVTStrucVisCtl::SceneSetup(const unsigned char *pucData, const unsigned int uiLength) {
   // Check we have a scene manager present
   if (!m_poScene)
      return false;
   // Can we go interactive yet?
   bool bDone = m_poScene->Setup(pucData, uiLength);
   if (bDone) {
      m_eSimResult = SD_OK;
      GoInteractive();
      // Create the visible group list
      ASSERT(!m_pbVisibleGroup);
      unsigned int uiGroups = m_poScene->NumGroups();
      m_pbVisibleGroup = (bool*) new bool[uiGroups];
      if (m_pbVisibleGroup) {
         for (int i = 0; i < uiGroups; i++)
            m_pbVisibleGroup[i] = true;
      }
   }
   return bDone;
}

bool CVTStrucVisCtl::ShowFrame(const unsigned int uiFrame) {
   // Check we have a scene manager present
   if (!m_poScene)
      return false;
   // Show the current frame
   unsigned int uiSeek, uiLength;
   // Only get the frame when setup is complete
   if (m_eSimResult == SD_OK) {
      // Get the frame info
      m_poScene->FrameInfo(uiFrame, uiSeek, uiLength);
      // Display or queue the frame
      if (uiLength > 0) {
         // Get the data
         return m_oSimData.ShowFrame(uiFrame, uiSeek, uiLength);
      }
   }

   return false;
}

void CVTStrucVisCtl::ShowFrame(const unsigned int uiFrame,
                               const unsigned char *pucData,
                               const unsigned int uiLength) {
   // Check we have a scene manager present
   if (!m_poScene)
      return;
   // Set the current frame
   m_uiFrame = uiFrame;
   // Set the frame count slider
   m_oSFrame.SetLimitUI(0, m_poScene->NumFrames() - 1, m_uiFrame);
   // Update the display
   Update(true);
   m_poScene->ShowFrame(pucData, uiLength);
   Update(false);
}

void CVTStrucVisCtl::GoInteractive() {
   // Set the control interactive if we have all the properties available
   InternalSetReadyState(READYSTATE_INTERACTIVE);
   // Set the frame count slider
   m_oSFrame.SetLimitUI(0, m_poScene->NumFrames() - 1, m_uiFrame);
   // Refresh the control
   InvalidateControl();
}

/////////////////////
// Message handlers

void CVTStrucVisCtl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) {
   // If we're in run mode, the UI is loaded and the ready state is marked interactive
   // render the full-on interface
   bool bRun = Running();
   if (bRun && Interactive()) {
      // Draw the full-on interface
      DrawUI(pdc, rcBounds, bRun);
   }
   else {
      // Draw the placeholder
      DrawPlaceholder(pdc, rcBounds, bRun);
   }
}

void CVTStrucVisCtl::OnDrawMetafile(CDC* pDC, const CRect& rcBounds) {
   // Just do something simple
   DrawPlaceholder(pDC, rcBounds, false);
	
	COleControl::OnDrawMetafile(pDC, rcBounds);
}

void CVTStrucVisCtl::DoPropExchange(CPropExchange* pPX) {
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

   // Sort out the WRL path first
   PX_String(pPX, _T("WRLPath"), m_oWRLPath, "");

   // Get the initial position and orientation
   PX_String(pPX, _T("Position"), m_oPosition, "");
   PX_String(pPX, _T("Orientation"), m_oOrientation, "");

   // Get the scale factor
   PX_String(pPX, _T("Scale"), m_oScale, "");

   // Reset the async data flags if loading
   if (pPX->IsLoading()) {
      // Indicate that the synchronous data has been loaded
      InternalSetReadyState(READYSTATE_LOADED);

      // Set the asynchronous data flags
      m_uiAsyncFlags = AD_EMPTY;
      m_eUIResult = UI_UNKNOWN;
      m_eSimResult = SD_UNKNOWN;

      // Init Cortona
      InitCortona();

      // Reset the animation
      if (m_bRunning) {
         KillTimer(TI_ANIMATE);
         m_bRunning = false;
      }
      m_eRunMode = RM_PAUSE;
      m_uiFrame = 0;
      ShowFrame(0);
      m_uiStartGroup = 0;
      if (m_pbVisibleGroup) {
         delete [] m_pbVisibleGroup;
         m_pbVisibleGroup = NULL;
      }
   }

   // Start the asynchronous downloads
   PX_DataPath(pPX, _T("UIData"), m_oUIData);
   PX_DataPath(pPX, _T("SimData"), m_oSimData);
}

void CVTStrucVisCtl::OnResetState() {
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

   // Set the ready state back to LOADED
   InternalSetReadyState(READYSTATE_LOADED);

   // Reset the data result
   m_uiAsyncFlags = AD_EMPTY;
   m_eUIResult = UI_UNKNOWN;
   m_eSimResult = SD_UNKNOWN;

   // Reset the animation
   if (m_bRunning) {
      KillTimer(TI_ANIMATE);
      m_bRunning = false;
   }
   m_eRunMode = RM_PAUSE;
   m_uiFrame = 0;
   m_uiStartGroup = 0;
   if (m_pbVisibleGroup) {
      delete [] m_pbVisibleGroup;
      m_pbVisibleGroup = NULL;
   }

   // Restart Cortona
   ExitCortona();
   InitCortona();

   // Load the initial frame
   ShowFrame(0);
}

void CVTStrucVisCtl::AboutBox() {
	CDialog dlgAbout(IDD_ABOUTBOX_VTSTRUCVIS);
	dlgAbout.DoModal();
}

DWORD CVTStrucVisCtl::GetControlFlags() {
	return COleControl::GetControlFlags() | noFlickerActivate;
}

void CVTStrucVisCtl::OnAmbientPropertyChange(DISPID dispid) {
   // Check here to see if we've changed operating mode
	
	COleControl::OnAmbientPropertyChange(dispid);
}

void CVTStrucVisCtl::OnLButtonDown(UINT nFlags, CPoint point) {
   m_bLButtonDown = true;

   // Check the sliders
   CheckSliders(point);

   // Mouse may be in a different zone, so update anyway
   InvalidateControl();

   COleControl::OnLButtonDown(nFlags, point);
}

void CVTStrucVisCtl::OnLButtonUp(UINT nFlags, CPoint point) {
   m_bLButtonDown = false;
	
   // If we have a scene and we're interactive and running, process the event
   if (Running() && Interactive()) {
      // Process the controls
      unsigned int uiFrame = m_uiFrame;
      ButtonControl(uiFrame);
      if (uiFrame != m_uiFrame)
         ShowFrame(uiFrame);

      // Check for colouring change
      if ((point.y >= 140) && (point.y < 155)) {
         m_bStressColour = !m_bStressColour;
         m_poScene->SetColourScheme(m_bStressColour ? COLOUR_STRESS : COLOUR_GROUP);
      }

      // Check for the tab change
      if ((point.y >= 155) && (point.y < 170)) {
         m_uiUITab = point.x / 60;
      }

      // Check the sliders
      CheckSliders(point);
      uiFrame = m_uiFrame;
      // Update if the frame has changed
      UpdateSliders(uiFrame);
      if (uiFrame != m_uiFrame)
         ShowFrame(uiFrame);

      // Check the group list, if visibility tab set
      if (m_uiUITab == 0) {
         unsigned int uiGroups = m_poScene->NumGroups();
         // Get the size of the remain screen
         CRect oScreen;
         GetClientRect(oScreen);
         unsigned int uiMaxGroups = (oScreen.bottom - 270) / 10;
         // Set the extent
         unsigned int uiViewGroups = uiGroups - m_uiStartGroup;
         if (uiViewGroups > uiMaxGroups)
            uiViewGroups = uiMaxGroups;
         // Go through each group
         for (unsigned int i = 0; i < uiViewGroups; i++) {
            unsigned int uiGroup = i + m_uiStartGroup; 
            int iYLow = 172 + i * 15;
            int iYHigh = iYLow + 10;
            if ((point.x >= 2) && (point.x < 10) && (point.y >= iYLow) && (point.y < iYHigh)) {
               m_pbVisibleGroup[uiGroup] = !m_pbVisibleGroup[uiGroup];
               m_poScene->SetVisibility(uiGroup + 1, m_pbVisibleGroup[uiGroup]);
            }
         }
         // Check for the down arrow
         int fDown = (uiViewGroups * 15) + 170;
         if ((point.x >= 170) && (point.x < 180) && (point.y >= (fDown - 10)) && (point.y < fDown)) {
            if ((uiGroups > uiMaxGroups) && (uiMaxGroups + m_uiStartGroup < uiGroups))
               m_uiStartGroup++;
         }
         // Check for the up arrow
         if ((point.x >= 170) && (point.x < 180) && (point.y >= 170) && (point.y < 180)) {
            if ((uiGroups > uiMaxGroups) && (m_uiStartGroup > 1))
               m_uiStartGroup--;
         }
      }
   }

   // Operational zone may have changed, so update anyway
   InvalidateControl();

	COleControl::OnLButtonUp(nFlags, point);
}

void CVTStrucVisCtl::OnMouseMove(UINT nFlags, CPoint point) {
   // If we're running, react to the movement
   if (Running() && Interactive()) {
      // Check the mouse state
      if ((nFlags & MK_LBUTTON) > 0)
         m_bLButtonDown = true;

      // Is the mouse in a zone?
      const int iCol = point.x / 60;
      const int iRow = point.y - 50;
      int iUIZone = (iRow >= 0 ? (iRow / 30) * 3 + iCol : -1);
      iUIZone = (iUIZone > 8 ? -1 : iUIZone);
      bool bModified = (iUIZone != m_iUIZone);
      // Update the zone
      m_iUIZone = iUIZone;

      // Check the sliders
      bModified |= CheckSliders(point);

      // Repaint if necessary
      if (bModified)
         InvalidateControl();
   }

   // Check if the mouse is over the control
   if (!m_bMouseOver) {
      m_bMouseOver = true;
      // Set a timer to check for the mouse every 1/10 of a second
      SetTimer(TI_MOUSEOVER, 100, NULL);
   }

	COleControl::OnMouseMove(nFlags, point);
}

void CVTStrucVisCtl::OnTimer(UINT nIDEvent) {
   // Mouse over timer
   if (nIDEvent == TI_MOUSEOVER) {
      // Where is the mouse?
      CPoint oPos(GetMessagePos());
      ScreenToClient(&oPos);

      // Get the bounds of the control (just the client area)
      CRect oRect;
      GetClientRect(oRect);

      // Check the mouse is inside the control
      if (!oRect.PtInRect(oPos)) {
         // Stop looking if out of control
         m_bMouseOver = FALSE;
         KillTimer(TI_MOUSEOVER);
         // Forget about mouse states
         m_bLButtonDown = false;
         // Redraw the control
         InvalidateControl();
         // Check the frame for updates
         unsigned int uiFrame = m_uiFrame;
         UpdateSliders(uiFrame);
         if (uiFrame != m_uiFrame)
            ShowFrame(uiFrame);
      }
   }

   // Animate event
   if (nIDEvent == TI_ANIMATE) {
      if (Running() && Interactive()) {
         // Update the frame number if we're not still waiting on the last
         unsigned int uiFrame = m_uiFrame;
         if (!m_oSimData.Waiting() || m_eRunMode == RM_PLAYREV || m_eRunMode == RM_REWIND)
            FrameControl(uiFrame);
         // Update the frame if it's been modified
         if (uiFrame != m_uiFrame) {
            // Show the frame
            ShowFrame(uiFrame);
         }
         // Redraw the control
         InvalidateControl();
      }
   }

	COleControl::OnTimer(nIDEvent);
}

BSTR CVTStrucVisCtl::GetUIData() {
	CString strResult = m_oUIData.GetPath();

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetUIData(LPCTSTR lpszNewValue) {
   // Reset the UI data flags
   m_uiAsyncFlags &= AD_UIMASK;
   m_eUIResult = UI_UNKNOWN;
   // Load the new data
   Load(lpszNewValue, m_oUIData);
   // Notify a property browser
   BoundPropertyChanged(dispidUIData);
   // Mark the properties modified
   SetModifiedFlag();
}

BSTR CVTStrucVisCtl::GetSimData() {
	CString strResult = m_oSimData.GetPath();

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetSimData(LPCTSTR lpszNewValue) {
   // Reset the simulatio data flags
   m_uiAsyncFlags &= AD_SIMMASK;
   m_eSimResult = SD_UNKNOWN;
   // Load the new data
   Load(lpszNewValue, m_oSimData);
   // Notify a property browser
   BoundPropertyChanged(dispidSimData);
   // Mark the properties modified
   SetModifiedFlag();
}

BSTR CVTStrucVisCtl::GetWRLPath() {
	CString strResult = m_oWRLPath;

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetWRLPath(LPCTSTR lpszNewValue) {
   // Save the value
   m_oWRLPath = lpszNewValue;
   // Notify a property browser
   BoundPropertyChanged(dispidWRLPath);
   // Mark the properties modified
	SetModifiedFlag();
}

BSTR CVTStrucVisCtl::GetPosition() {
	CString strResult = m_oPosition;

   // Check for the scene
   if (m_poScene) {
      m_oPosition.Format("%s", m_poScene->GetCurrentPosition());
      strResult = m_oPosition;
      // Now delete the temp string
      delete [] m_poScene;
   }

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetPosition(LPCTSTR lpszNewValue) {
   CString oPos(lpszNewValue);

   // Check the position
   float pfPosition[3] = {0.0f, 0.0f, 0.0f};
   if (ParseFloat(oPos, 3, pfPosition) == 3) { 
      // String checks out, so save it
      m_oPosition = oPos;
      // Mark the porperty as modified
      BoundPropertyChanged(dispidPosition);
      // Mark the properties modified
      SetModifiedFlag();
      // If we have a scene, get the orientation and set the viewpoint
      float pfOrientation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
      if (m_poScene && (ParseFloat(m_oOrientation, 4, pfOrientation) == 4)) {
         m_poScene->SetViewpoint(pfPosition, pfOrientation);
      }
   }
}

BSTR CVTStrucVisCtl::GetOrientation() {
	CString strResult = m_oOrientation;

   // Check for the scene
   if (m_poScene) {
      m_oOrientation.Format("%s", m_poScene->GetCurrentOrientation());
      strResult = m_oOrientation;
      // Now delete the temp string
      delete [] m_poScene;
   }

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetOrientation(LPCTSTR lpszNewValue) {
   CString oOrientation(lpszNewValue);

   // Check the position
   float pfOrientation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
   if (ParseFloat(oOrientation, 4, pfOrientation) == 4) { 
      // String checks out, so save it
      m_oOrientation = oOrientation;
      // Mark the porperty as modified
      BoundPropertyChanged(dispidOrientation);
      // Mark the properties modified
      SetModifiedFlag();
      float pfPosition[3] = {0.0f, 0.0f, 0.0f};
      // If we have a scene, get the orientation and set the viewpoint
      if (m_poScene && (ParseFloat(m_oPosition, 3, pfPosition) == 3)) {
         m_poScene->SetViewpoint(pfPosition, pfOrientation);
      }
   }
}

BSTR CVTStrucVisCtl::GetScale() {
	CString strResult = m_oScale;

   // Get the scale format
   m_oScale.Format("%.3f, %.3f, %.3f", m_fXScale, m_fYScale, m_fZScale);
   strResult = m_oScale;

	return strResult.AllocSysString();
}

void CVTStrucVisCtl::SetScale(LPCTSTR lpszNewValue) {
   // Do nothing yet
}

