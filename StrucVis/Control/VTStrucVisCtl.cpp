//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.cpp,v 1.15 2002/03/25 15:57:03 vap-warren Exp $

#include "stdafx.h"
#include "VTStrucVis.h"
#include "VTStrucVisCtl.h"
#include "VTStrucVisPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////
// Category Support Functions

// Require these for proper Internet aware controls
const CATID CATID_SafeForScripting =
   {0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const CATID CATID_SafeForInitializing =
   {0x7dd95802,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

// Creates a category entry
HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription) {
   ICatRegister *pcr = NULL;
   HRESULT hResult = S_OK;

   // Create an instance of the category manager
   hResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_ICatRegister,
                              (void**)&pcr);
   if (FAILED(hResult))
      return hResult;

   CATEGORYINFO catinfo;
   catinfo.catid = catid;
   // English locale ID in hex
   catinfo.lcid = 0x0409;

   int len = wcslen(catDescription);
   wcsncpy(catinfo.szDescription, catDescription, len);
   catinfo.szDescription[len] = '\0';

   hResult = pcr->RegisterCategories(1, &catinfo);
   pcr->Release();

   return hResult;
}

// Registers that the category entry is implemented
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid) {
   ICatRegister *pcr = NULL;
   HRESULT hResult = S_OK;

   // Create an instance of the category manager
   hResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_ICatRegister,
                              (void**)&pcr);
   if (SUCCEEDED(hResult)) {
      CATID rgcatid[1];
      rgcatid[0] = catid;
      hResult = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
   }

   if (pcr != NULL)
      pcr->Release();

   return hResult;
}

// Unregisters the implemented category
HRESULT UnregisterCLSIDInCategory(REFCLSID clsid, CATID catid) {
   ICatRegister *pcr = NULL;
   HRESULT hResult = S_OK;

   // Create an instance of the category manager
   hResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_ICatRegister,
                              (void**)&pcr);
   if (SUCCEEDED(hResult)) {
      CATID rgcatid[1];
      rgcatid[0] = catid;
      hResult = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
   }

   if (pcr != NULL)
      pcr->Release();

   return hResult;
}

// Registers that the category is required
HRESULT RegisterCLSIDInReqCategory(REFCLSID clsid, CATID catid) {
   ICatRegister *pcr = NULL;
   HRESULT hResult = S_OK;

   // Create an instance of the category manager
   hResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_ICatRegister,
                              (void**)&pcr);
   if (SUCCEEDED(hResult)) {
      CATID rgcatid[1];
      rgcatid[0] = catid;
      hResult = pcr->RegisterClassReqCategories(clsid, 1, rgcatid);
   }

   if (pcr != NULL)
      pcr->Release();

   return hResult;
}

// Unregisters the required category
HRESULT UnregisterCLSIDInReqCategory(REFCLSID clsid, CATID catid) {
   ICatRegister *pcr = NULL;
   HRESULT hResult = S_OK;

   // Create an instance of the category manager
   hResult = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                              NULL,
                              CLSCTX_INPROC_SERVER,
                              IID_ICatRegister,
                              (void**)&pcr);
   if (SUCCEEDED(hResult)) {
      CATID rgcatid[1];
      rgcatid[0] = catid;
      hResult = pcr->UnRegisterClassReqCategories(clsid, 1, rgcatid);
   }

   if (pcr != NULL)
      pcr->Release();

   return hResult;
}

///////////////////
// CVTStrucVisCtl

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
   m_bDirty(true),
   m_eRunMode(RM_PLAY),
   m_bLoop(false),
   m_bRunning(false) {

   InitializeIIDs(&IID_DVTStrucVis, &IID_DVTStrucVisEvents);

   // Set the initial state to loading
	m_lReadyState = READYSTATE_LOADING;

   // Set the data path controls
   m_oSimData.SetControl(this);
   m_oUIData.SetControl(this);

   // Set the initial window size
   SetInitialSize(180, 250);
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
         m_oCortona.Trace("Turned off the nav bar\n");
         m_oCortona.ContextMenu(true);
         m_oCortona.Trace("Turned off the context menu\n");
         m_oCortona.Headlight(true);
         m_oCortona.Trace("Turned on the headlight\n");
         m_oCortona.Edit();
         m_oCortona.Trace("Prepared the engine for direct editing\n");
         m_oCortona.Refresh();

         // Create the utility class
         m_poCortonaUtil = (CCortonaUtil*) new CCortonaUtil(m_oCortona.GetEngine());

         // Create the scene manager
         m_poScene = (CSceneManager*) new CSceneManager(m_poCortonaUtil);

         // Set the WRL path
         m_poScene->SetBaseURL(m_oWRLPath);

         // Setup viewpoint location
         float pfPosition[3] = {5.0f, 5.0f, 5.0f};
         float pfRotation[4] = {-0.59f, 0.77f, 0.24f, 0.99f};
         m_poScene->SetViewpoint(pfPosition, pfRotation);

         // Set scale factor
         m_poScene->SetScaleFactor(1, 1, 1);

         // Set colour scheme
         m_poScene->SetColourScheme(GROUP);
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
         while (bNext && SUCCEEDED(pEnumUnknown->Next(1, &pNextControl, NULL))) {
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
      pDC->SetTextColor(TranslateColor(GetEnabled() ? RGB(0xFF, 0x00, 0x00): RGB(0xFF, 0xC0, 0xC0)));
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
   // If we haven't started it already, run the animation
   if (!m_bRunning) {
      // Set the animation vars
      m_uiFrame = 0;
      m_eRunMode = RM_PLAY;
      SetTimer(TI_ANIMATE, 30, NULL);
      m_bRunning = true;
      // Show the first frame
      ShowFrame(0);
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
   CBitmap *pRemoteBitmap = oDCRemote.SelectObject(&m_oUIBitmap);
   CBitmap *pBufferBitmap = oDCBuffer.SelectObject(&m_oBackBuffer);
   // Set the background to the background colour
   CBrush oBrush;
   oBrush.CreateSolidBrush(TranslateColor(GetBackColor()));
   oDCBuffer.FillRect(rcBounds, &oBrush);

   // Get the bitmap params
	BITMAP sBitmap;
	m_oUIBitmap.GetBitmap(&sBitmap);
   const int iWidth = sBitmap.bmWidth / 2;
   // Copy the base remote
	oDCBuffer.BitBlt(0, 0, iWidth, sBitmap.bmHeight, &oDCRemote, 0, 0, SRCCOPY);

   //#===--- Highlight buttons
   // Loop indicator
   if (m_bLoop)
      oDCBuffer.BitBlt(120, 80, 60, 30, &oDCRemote, 300, 80, SRCCOPY);

   // If we're running and enabled
   if (bRun && GetEnabled()) {

      // Mouse over button
      if (m_bLButtonDown && (m_iUIZone >= 0)) {
         const int iX = (m_iUIZone % 3) * 60;
         const int iY = (m_iUIZone / 3) * 30 + 50;
      	oDCBuffer.BitBlt(iX, iY, 60, 30, &oDCRemote, iWidth + iX, iY, SRCCOPY);
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
      	oDCBuffer.BitBlt(iX, iY, 60, 30, &oDCRemote, iWidth + iX, iY, SRCCOPY);
      }
   }

   //#===--- Frame count

   // Copy over the back buffer
   pDC->BitBlt(rcBounds.left, rcBounds.top, rcBounds.Width(), rcBounds.Height(), &oDCBuffer, 0, 0, SRCCOPY); 
   // Restore the saved objects
   oDCRemote.SelectObject(pRemoteBitmap);
   oDCBuffer.SelectObject(pBufferBitmap);
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

void CVTStrucVisCtl::FrameControl() {
   // Check we have a scene manager present
   if (!m_poScene)
      return;
   // Get the max number of frames
   unsigned int uiFrames = m_poScene->NumFrames();
   // Set the fast forward/ rewind step
   const unsigned int uiStep = 2;
   // Store the current frame
   const unsigned int uiCurrent = m_uiFrame;
   // Update dependent on the run mode
   switch (m_eRunMode) {
      // PLAY
      case RM_PLAY:
         if (m_uiFrame + 1 < uiFrames)
            m_uiFrame++;
         else
            if (m_bLoop)
               m_uiFrame = 0;
            else
               m_eRunMode = RM_PAUSE;
         break;
      // PLAY REVERSE
      case RM_PLAYREV:
         if (m_uiFrame > 0)
            m_uiFrame--;
         else
            if (m_bLoop)
               m_uiFrame = uiFrames - 1;
            else
               m_eRunMode = RM_PAUSE;
         break;
      // REWIND
      case RM_REWIND:
         if (m_uiFrame > uiStep)
            m_uiFrame -= uiStep;
         else
            if (m_bLoop)
               m_uiFrame = uiFrames - 1;
            else
               m_eRunMode = RM_PAUSE;
         break;
      // FAST FORWARD
      case RM_FASTFORWARD:
         if (m_uiFrame + uiStep < uiFrames)
            m_uiFrame += uiStep;
         else
            if (m_bLoop)
               m_uiFrame = 0;
            else
               m_eRunMode = RM_PAUSE;
         break;
      // NONE
      default:
         break;
   }
   // Check to see if the frame number is modified
   if (uiCurrent != m_uiFrame)
      m_bDirty = true;
}

void CVTStrucVisCtl::UIControl() {
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
         m_bLoop = false;
         m_uiFrame = 0;
         m_bDirty = true;
         break;
      // GO TO END
      case 7:
         m_eRunMode = RM_PAUSE;
         m_bLoop = false;
         m_uiFrame = m_poScene->NumFrames() - 1;
         m_bDirty = true;
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

bool CVTStrucVisCtl::SceneSetup(const unsigned char *pucData, unsigned int uiLength) {
   // Check we have a scene manager present
   if (!m_poScene)
      return false;
   // Can we go interactive yet?
   bool bDone = m_poScene->Setup(pucData, uiLength);
   if (bDone) {
      m_eSimResult = SD_OK;
      GoInteractive();
   }
   return bDone;
}

void CVTStrucVisCtl::ShowFrame(unsigned int uiFrame) {
   // Check we have a scene manager present
   if (!m_poScene)
      return;
   // Show the current frame
   unsigned int uiSeek, uiLength;
   m_poScene->FrameInfo(m_uiFrame, uiSeek, uiLength);
   if (uiLength > 0)
      m_oSimData.ShowFrame(uiSeek, uiLength);
}

void CVTStrucVisCtl::ShowFrame(const unsigned char *pucData, unsigned int uiLength) {
   // Check we have a scene manager present
   if (!m_poScene)
      return;
   // Pass the data through
   m_poScene->ShowFrame(pucData, uiLength);
}

void CVTStrucVisCtl::GoInteractive() {
   // Set the control interactive if we have all the properties available
   InternalSetReadyState(READYSTATE_INTERACTIVE);
   // Refresh the control
   InvalidateControl();
}

/////////////////////
// Message handlers

void CVTStrucVisCtl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) {
   // Check our operating mode
   bool bRun = AmbientUserMode() && !AmbientUIDead();
   // If we're in run mode, the UI is loaded and the ready state is marked interactive
   // render the full-on interface
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
      m_eRunMode = RM_PLAY;
      m_uiFrame = 0;
      m_bDirty = true;
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
   m_eRunMode = RM_PLAY;
   m_uiFrame = 0;
   m_bDirty = true;

   // Restart Cortona
   ExitCortona();
   InitCortona();
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
   InvalidateControl();

   COleControl::OnLButtonDown(nFlags, point);
}

void CVTStrucVisCtl::OnLButtonUp(UINT nFlags, CPoint point) {
   m_bLButtonDown = false;
	
   // Check our operating mode
   bool bRun = AmbientUserMode() && !AmbientUIDead();
   bool bInteractive = (GetReadyState() == READYSTATE_INTERACTIVE) || (GetReadyState() == READYSTATE_COMPLETE);
   // If we have a scene and we're interactive and running, process the event
   if (bRun && Interactive())
      UIControl();
   // Refresh the display
   InvalidateControl();

	COleControl::OnLButtonUp(nFlags, point);
}

void CVTStrucVisCtl::OnMouseMove(UINT nFlags, CPoint point) {
   // Work out where the mouse is
   const int iCol = point.x / 60;
   const int iRow = (point.y - 50) / 30;
   int iUIZone = iRow * 3 + iCol;
   iUIZone = (iUIZone < 0 ? -1 : (iUIZone > 7 ? -1 : iUIZone));
   // Update if the value has changed
   if (iUIZone != m_iUIZone)
      InvalidateControl();
   // Check if the mouse is over the control
   if (!m_bMouseOver) {
      m_bMouseOver = true;
      // Set a timer to check for the mouse every 1/10 of a second
      SetTimer(TI_MOUSEOVER, 100, NULL);
   }
   m_iUIZone = iUIZone;

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
      }
   }

   // Animate event
   if (nIDEvent == TI_ANIMATE) {
      bool bInteractive = (GetReadyState() == READYSTATE_INTERACTIVE) || (GetReadyState() == READYSTATE_COMPLETE);
      if (m_poScene && bInteractive) {
         // Update the frame number
         FrameControl();
         if (m_bDirty) {
            // Show the frame
            ShowFrame(m_uiFrame);
            // Show updated
            m_bDirty = false;
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

	SetModifiedFlag();
}
