//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaNode.h
// 10/03/2002 - Warren Moore
//
// $Id: CortonaNode.h,v 1.3 2002/03/24 13:51:59 vap-warren Exp $

#ifndef __VSTRUCVIS_CORTONANODE__
#define __VSTRUCVIS_CORTONANODE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CortonaField.h"

// Automation interfaces for Cortona
#include "shelley.h"

class CCortonaNode {
//#===--- Construction/Destruction
public:
   CCortonaNode(INodeObject *pNode);
   // Create from another node

   ~CCortonaNode();

//#===--- Member Functions

   void Release();
   // Release the node object

   CCortonaField *GetField(const char *pcName);
   // Creates a field object for the named field, returns NULL if failed

   bool AssignEventIn(const char *pcField, const CCortonaField &oValue);
   // Assigns the supplied field value to the named field

protected:

   INodeObject *Interface() const;
   // Returns the node object interface

   // Friend class for protected functions
   friend class CCortonaUtil;

//#===--- Member Variables

protected:
   INodeObject *m_pNode;         // The node interface

};

//#===--- Inlines

inline INodeObject *CCortonaNode::Interface() const {
   return m_pNode;
}

#endif // __VSTRUCVIS_CORTONANODE__
