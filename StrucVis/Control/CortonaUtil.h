//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Design.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaUtil.h"
//! author    = "Warren Moore"
//! date      = "07/3/2002"
//! rcsid     = "$Id: CortonaUtil.h,v 1.14 2002/04/25 15:44:04 vap-warren Exp $"

#ifndef __VTSTRUCVIS_CORTONAUTIL__
#define __VTSTRUCVIS_CORTONAUTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaNode.h"
#include "CortonaField.h"

// Automation interfaces for Cortona
#include "shelley.h"

//: Cortona utility class
// Data manager for Cortona scene variables

class CCortonaUtil {
//:-----
//: Construction/Destruction
public:

   CCortonaUtil(IEngine *pEngine);
   //: Constructor
   // Creates the utility class from the Automation engine interface
   //!param: pEngine - Pointer to IEngine interface - must not be null

   ~CCortonaUtil();
   //: Destructor
   // Releases the engine interface

//:-----
//: Management Functions

   void Release();
   //: Release the engine interface

   CCortonaNode *CreateVrmlFromString(const char *pcVrml);
   //: Creates a VRML node from the specified string
   //!return: Pointer to node object, null if failed

   bool AddToScene(const CCortonaNode &oNode);
   //: Add node to the scene
   // Adds to the list of root nodes
   //!return: true if successful

   CCortonaNode *GetNode(const char *pcName);
   //: Get a node by it's DEF'd name
   //!return: Pointer to node object, null if failed

   bool AddRoute(const CCortonaNode &oSrcNode, const char *pcSrcField,
                 const CCortonaNode &oDstNode, const char *pcDstField);
   //: Add a route
   // Add a route from the source node's specified field to the 
   // destination node's specified field
   //!param: oSrcNode - Reference to source node object
   //!param: pcSrcField - Pointer to string containing source field name
   //!param: oDstNode - Reference to destination node object
   //!param: pcDstField - Pointer to string containing destination field name
   //!return: true if successful

   CCortonaField *CreateField(const char *pcType);
   //: Create an empty field
   //!return: Pointer to new field object, null if failed

   bool Clear(void);
   //: Clear the VRML scene
   //!return: true if successful

protected:

   IEngine *m_pEngine;        //: Cortona engine Automation interface

};

#endif // __VTSTRUCVIS_CORTONAUTIL__
