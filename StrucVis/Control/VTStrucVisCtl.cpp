//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// VTStructVisCtl.cpp
// 05/03/2002 - Warren Moore
//
// $Id: VTStrucVisCtl.cpp,v 1.3 2002/03/20 13:08:26 vap-warren Exp $

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

////////////////////
// CVTStrucVisCtrl

IMPLEMENT_DYNCREATE(CVTStrucVisCtrl, COleControl)

// Message map
BEGIN_MESSAGE_MAP(CVTStrucVisCtrl, COleControl)
	//{{AFX_MSG_MAP(CVTStrucVisCtrl)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()

// Dispatch map
BEGIN_DISPATCH_MAP(CVTStrucVisCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CVTStrucVisCtrl)
	DISP_STOCKPROP_READYSTATE()
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CVTStrucVisCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()

// Event map
BEGIN_EVENT_MAP(CVTStrucVisCtrl, COleControl)
	//{{AFX_EVENT_MAP(CVTStrucVisCtrl)
	EVENT_STOCK_READYSTATECHANGE()
	//}}AFX_EVENT_MAP
END_EVENT_MAP()

// Property pages
// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CVTStrucVisCtrl, 1)
	PROPPAGEID(CVTStrucVisPropPage::guid)
END_PROPPAGEIDS(CVTStrucVisCtrl)


// Initialize class factory and guid
IMPLEMENT_OLECREATE_EX(CVTStrucVisCtrl, "VTStructVis.Control.1",
	0xad64b056, 0xe5e7, 0x4c45, 0x92, 0x43, 0x75, 0x58, 0x2, 0xd7, 0xcb, 0xf7)

// Type library ID and version
IMPLEMENT_OLETYPELIB(CVTStrucVisCtrl, _tlid, _wVerMajor, _wVerMinor)

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

IMPLEMENT_OLECTLTYPE(CVTStrucVisCtrl, IDS_VTSTRUCVIS, _dwVTStrucVisOleMisc)

// Adds or removes system registry entries for CVTStrucVisCtrl
BOOL CVTStrucVisCtrl::CVTStrucVisCtrlFactory::UpdateRegistry(BOOL bRegister) {

   if (bRegister) {
      // Regsiter as a control
      CreateComponentCategory(CATID_Control,
                              L"Controls");
      RegisterCLSIDInCategory(m_clsid, CATID_Control);
      // Register safe for initialising
      CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data");
      RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);
      // Register safe for scripting
      CreateComponentCategory(CATID_SafeForScripting, L"Controls that are safely scriptable");
      RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);
      // Register persists to property bag
//      CreateComponentCategory(CATID_PersistsToPropertyBag,
//                              L"Support initialize via PersistPropertyBag");
//      RegisterCLSIDInCategory(m_clsid, CATID_PersistsToPropertyBag);
      // Register requires data path host
//      CreateComponentCategory(CATID_RequiresDataPathHost,
//                              L"Requires Data Path Host");
//      RegisterCLSIDInReqCategory(m_clsid, CATID_RequiresDataPathHost);
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
			afxRegApartmentThreading,
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
//      UnregisterCLSIDInCategory(m_clsid, CATID_PersistsToPropertyBag);
//      UnregisterCLSIDInReqCategory(m_clsid, CATID_RequiresDataPathHost);
      UnregisterCLSIDInCategory(m_clsid, CATID_InternetAware);
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
   }
}

// Constructor
CVTStrucVisCtrl::CVTStrucVisCtrl() {
	InitializeIIDs(&IID_DVTStrucVis, &IID_DVTStrucVisEvents);

	m_lReadyState = READYSTATE_LOADING;
	// TODO: Call InternalSetReadyState when the readystate changes.

	// TODO: Initialize your control's instance data here.
   m_oDrawText = "Started";
}

// Destructor
CVTStrucVisCtrl::~CVTStrucVisCtrl() {
}

// Drawing function
void CVTStrucVisCtrl::OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid) {
   // Paint the background white
   CBrush bkBrush;
   bkBrush.CreateSolidBrush(RGB(0xff, 0xff, 0xff));
   pdc->FillRect(rcBounds, &bkBrush);
   // Draw the text from the member string
   pdc->SetBkMode(TRANSPARENT);
   pdc->SetTextColor(RGB(0x00, 0x00, 0x00));
   pdc->DrawText(m_oDrawText, -1, CRect(rcBounds), DT_LEFT | DT_WORDBREAK);
}

// Persistence support
void CVTStrucVisCtrl::DoPropExchange(CPropExchange* pPX) {
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}

// Reset control to default state
void CVTStrucVisCtrl::OnResetState() {
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}

// Display an "About" box to the user
void CVTStrucVisCtrl::AboutBox() {
	CDialog dlgAbout(IDD_ABOUTBOX_VTSTRUCVIS);
	dlgAbout.DoModal();
}

// Enable flicker free activation
DWORD CVTStrucVisCtrl::GetControlFlags() {
	return COleControl::GetControlFlags() | noFlickerActivate;
}

/////////////////////
// Message handlers

void CVTStrucVisCtrl::OnLButtonDown(UINT nFlags, CPoint point) {
   // Set the draw string to clicked and repaint
   if (GetCortona()) {
      // Turn on the Nav nar
      m_oControl.NavBar(1);
      m_oDrawText += "Turning on nav bars\n";
   }
   InvalidateControl();
	
	COleControl::OnLButtonDown(nFlags, point);
}

bool CVTStrucVisCtrl::GetCortona() {
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
         m_oDrawText = "";
         LPUNKNOWN pNextControl = NULL;
         bool bNext = true;
         while (bNext && pEnumUnknown->Next(1, &pNextControl, NULL) == S_OK) {
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
                     m_oDrawText += "Found Cortona control\n";
                     if (m_oControl.Attach(pOleObject)) {
                        m_oDrawText += "Attached dispatch driver\n";
                        bNext = false;
                     }
                     else
                        m_oDrawText += "Unable to get dispatch interface\n";
                  }
                  else {
                     LPOLESTR pClsid = NULL;
                     StringFromCLSID(clsid, &pClsid);
                     m_oDrawText += "Found control CLSID: ";
                     m_oDrawText += pClsid;
                     m_oDrawText += "\n";
                  }
               }
               else
                  m_oDrawText += "Found a control without CLSID\n";
               // Release the OLE object
               pOleObject->Release();
            }
            else
               m_oDrawText += "Found control but no OLE object\n";
            pNextControl->Release();
         }
      }
      else
         m_oDrawText = "Failed to enum objects";
      // Release the EnumUnknown interface
      pEnumUnknown->Release();
      // Release the container interface
      pContainer->Release();
   }
   else
      m_oDrawText = "Failed to get container";

   return m_oControl.Attached();
}
