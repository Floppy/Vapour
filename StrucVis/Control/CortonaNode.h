//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaNode.h
// 10/03/2002 - Warren Moore
//
// $Id: CortonaNode.h,v 1.1 2002/03/21 14:32:07 vap-warren Exp $

#ifndef __CORTONANODE__
#define __CORTONANODE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // __CORTONANODE__
