//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaControl.h
// 17/03/2002 - Warren Moore
//
// $Id: CortonaControl.h,v 1.1 2002/03/20 13:08:26 vap-warren Exp $

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

   IEngine *GetEngine();
   // Returns a pointer to the controls IEngine interface

   void NavBar(short sVal);
   // Turns the nvaigation bar on(1) or off(0)

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
