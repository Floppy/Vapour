//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Design.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaControl.h"
//! author    = "Warren Moore"
//! date      = "17/3/2002"
//! rcsid     = "$Id: CortonaControl.h,v 1.10 2002/04/25 15:44:03 vap-warren Exp $"

#ifndef __VTSTRUCVIS_CORTONACONTROL__
#define __VTSTRUCVIS_CORTONACONTROL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Automation interfaces for Cortona
#include "shelley.h"

//: Cortona control access
// Wrapper around Cortona Automation interfaces

class CCortonaControl {
//:-----
//: Construction/Destruction
public:

   CCortonaControl();
   //: Constructor

   ~CCortonaControl();
   //: Destructor

//:-----
//: Control Interface Management

   bool Attach(LPOLEOBJECT pOleObject);
   //: Specify a Cortona dispatch driver for the object
   //!param: pOleObject - Pointer to an IOleObject interface for the active Cortona control
   //!return: true if attached ok

   bool Attached() const;
   //: Cortona object attached indicator
   //!return: true if driver attached

   void Release();
   //: Releases the attached driver

//:-----
//: Cortona Functions
// Requires an attached dispatch driver

   IEngine *GetEngine();
   //: Get the Cortona IEngine interface
   //!return: Pointer to the control's IEngine interface, null if not attached

   void NavBar(bool bVal);
   //: Enable the navigation bars

   void ContextMenu(bool bVal);
   //: Enable the context menu (right-mouse click)

   void Headlight(bool bVal);
   //: Enable the headlight

   void Edit();
   //: Enable direct editing of an empty scene

   void Freeze(bool bVal);
   //: Disable the renderer

   void Refresh();
   //: Force one execution of the render pipeline

   void Trace(const char *pcText);
   //: Send output to the Cortona console

protected:
   LPDISPATCH m_pDispatch;          //: Raw dispatch driver interface
   COleDispatchDriver m_oDispatch;  //: Dispatch driver for Cortona control
   bool m_bAttached;                //: Dispatch driver attached indicator

};

inline bool CCortonaControl::Attached() const {
   return m_bAttached;
}

#endif // __VTSTRUCVIS_CORTONACONTROL__
