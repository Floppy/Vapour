//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaNode.h"
//! author    = "Warren Moore"
//! date      = "10/3/2002"
//! rcsid     = "$Id: CortonaNode.h,v 1.7 2002/04/04 19:55:03 vap-warren Exp $"

#ifndef __VSTRUCVIS_CORTONANODE__
#define __VSTRUCVIS_CORTONANODE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaField.h"

// Automation interfaces for Cortona
#include "shelley.h"

//: Cortona node access
// Wrapper around Cortona node Automation interfaces

class CCortonaNode {
//:-----
//: Construction/Destruction
public:

   CCortonaNode(INodeObject *pNode);
   //: Constructor
   // Create from a node interface
   //!param: pNode - INodeObject interface to Cortona field - must not be null

   ~CCortonaNode();
   //: Destructor
   // Release must be called first for new (not copied) objects

//:-----
//: Node Functions

   void Release();
   //: Release the field object
   // Release the node object

   CCortonaField *GetField(const char *pcName);
   //: Create a field object from the specified name
   //!param: pcName - Pointer to string containing the field name
   //!return: Pointer to field object if found, null if not or failed

   bool AssignEventIn(const char *pcField, const CCortonaField &oValue);
   //: Assign the field to the named field
   // The named field must be an EventIn field
   //!param: pcField - Pointer to string containing the field name
   //!param: oField - Reference to field object
   //!return: true if successful

protected:

   INodeObject *Interface() const;
   //: Get the node object interface
   // Only to be called by friend classes

   friend class CCortonaUtil;
   //: Friend class for protected functions

protected:

   INodeObject *m_pNode;         //: The node interface

};

//#===--- Inlines

inline INodeObject *CCortonaNode::Interface() const {
   return m_pNode;
}

#endif // __VSTRUCVIS_CORTONANODE__
