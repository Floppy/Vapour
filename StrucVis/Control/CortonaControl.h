//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaControl.h
// 17/03/2002 - Warren Moore
//
// $Id: CortonaControl.h,v 1.5 2002/03/27 13:10:44 vap-warren Exp $

#ifndef __VTSTRUCVIS_CORTONACONTROL__
#define __VTSTRUCVIS_CORTONACONTROL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Automation interfaces for Cortona
#include "shelley.h"

class CCortonaControl {
//#===--- Construction/Destruction
public:
   CCortonaControl();
   // Constructor must be called with a valid IEngine interface
   ~CCortonaControl();

//#===--- Member Functions

   bool Attach(LPOLEOBJECT pOleObject);
   // Attachs the dispatch driver for the local Cortona control

   bool Attached() const;
   // Returns whether the driver is attached

   void Release();
   // Releases the attached control

   IEngine *GetEngine();
   // Returns a pointer to the controls IEngine interface

   void NavBar(bool bVal);
   // Turns the navigation bar on(1) or off(0)

   void ContextMenu(bool bVal);
   // Turns the context menu on(1) or off(0)

   void Headlight(bool bVal);
   // Sets the headlight 

   void Edit();
   // Enables direct scene modication

   void Freeze(bool bVal);
   // Freezes the Cortona rendering

   void Refresh();
   // Refreshes the Cortona render pipeline

   void Trace(const char *pcText);
   // Outputs a string to the Cortona console

//#===--- Member Variables
protected:
   LPDISPATCH m_pDispatch;          // Raw dispatch interface
   COleDispatchDriver m_oDispatch;  // Dispatch driver for Cortona control
   bool m_bAttached;                // IDispatch interface attached indicator

};

inline bool CCortonaControl::Attached() const {
   return m_bAttached;
}

#endif // __VTSTRUCVIS_CORTONACONTROL__
