//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaUtil.h"
//! author    = "Warren Moore"
//! date      = "07/3/2002"
//! rcsid     = "$Id: CortonaUtil.h,v 1.12 2002/04/04 11:01:34 vap-warren Exp $"

#ifndef __VTSTRUCVIS_CORTONAUTIL__
#define __VTSTRUCVIS_CORTONAUTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaNode.h"
#include "CortonaField.h"

// Automation interfaces for Cortona
#include "shelley.h"

class CCortonaUtil {
//#===--- Construction/Destruction
public:
   CCortonaUtil(IEngine *pEngine);
   // Constructor must be called with a valid IEngine interface
   ~CCortonaUtil();

//#===--- Member Functions

   void Release();
   // Release the engine interface

   CCortonaNode *CreateVrmlFromString(const char *pcVrml);
   // Create a VRML node from a string, returns NULL if failed

   bool AddToScene(const CCortonaNode &oNode);
   // Add a node to the list of root nodes

   CCortonaNode *GetNode(const char *pcName);
   // Get a node by it's DEF'd name, returns NULL if failed

   bool AddRoute(const CCortonaNode &oSrcNode, const char *pcSrcField,
                 const CCortonaNode &oDstNode, const char *pcDstField);
   // Add a route from the src node's field to the destination node's field

   CCortonaField *CreateField(const char *pcType);
   // Create an empty field, returns NULL if failed

   bool Clear(void);
   // Empties the VRML scene

//#===--- Member Variables
protected:
   IEngine *m_pEngine;
   // Cortona Engine interface

};

#endif // __VTSTRUCVIS_CORTONAUTIL__
