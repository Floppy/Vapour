//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaControl.cpp
// 17/03/2002 - Warren Moore
//
// $Id: CortonaControl.cpp,v 1.1 2002/03/20 13:08:26 vap-warren Exp $

#include "stdafx.h"
#include "VTStrucVis.h"

#include "CortonaControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Include the Cortona IID's
#include "shelley_i.c"

// For OLE conversion macros
#include "afxpriv.h"

////////////////////
// CCortonaControl

CCortonaControl::CCortonaControl() :
   m_bAttached(false),
   m_pDispatch(NULL) {
}

CCortonaControl::~CCortonaControl() {
   // Detach the dispatch driver if attached
   if (m_bAttached) {
      m_oDispatch.DetachDispatch();
      m_pDispatch->Release();
      m_bAttached = false;
   }
}

bool CCortonaControl::Attach(LPOLEOBJECT pOleObject) {
   // Check we have an object
   if (!pOleObject)
      return false;
   // Check we haven't already called this
   if (m_bAttached)
      return true;

   // Create the dispatch interface
   HRESULT hResult = pOleObject->QueryInterface(IID_IDispatch, (LPVOID*)&m_pDispatch);

   if (SUCCEEDED(hResult)) {
      // Attach the dispatch driver
      m_oDispatch.AttachDispatch(m_pDispatch, FALSE);
      m_bAttached = true;
   }

   return SUCCEEDED(hResult);
}

IEngine *CCortonaControl::GetEngine() {
   // Check we have a dispatch interface
   if (!m_bAttached)
      return NULL;

   IEngine *pEngineDisp = NULL;
   IEngine *pEngine = NULL;

   DISPID dwDispID;
   USES_CONVERSION;
   // Use Automation to get the Engine property
   LPCOLESTR lpOleStr = T2OLE("Engine");
   if (SUCCEEDED(m_pDispatch->GetIDsOfNames(IID_NULL,
                                          (LPOLESTR*)&lpOleStr,
                                          1,
                                          0,
                                          &dwDispID))) {
      // If we've found the property, get the value
      m_oDispatch.GetProperty(dwDispID, VT_DISPATCH, &pEngineDisp);
      // Now query for the IEngine interface
      if (pEngine) {
         HRESULT hResult = pEngineDisp->QueryInterface(IID_IEngine, (void**)&pEngine);
         pEngineDisp->Release();
         if (FAILED(hResult))
            pEngine = NULL;
      }
   }

   return pEngine;
}

void CCortonaControl::NavBar(short sVal) {
   // Check we have a dispatch interface
   if (!m_bAttached)
      return;

   DISPID dwDispID;
   USES_CONVERSION;
   // Use Automation to set the Navigation bars
   LPCOLESTR lpOleStr = T2OLE("NavigationBar");
   if (SUCCEEDED(m_pDispatch->GetIDsOfNames(IID_NULL,
                                          (LPOLESTR*)&lpOleStr,
                                          1,
                                          0,
                                          &dwDispID))) {
      static BYTE parms[] = VTS_I2;
      m_oDispatch.InvokeHelper(dwDispID, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, sVal);
   }
}

