//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaUtil.h
// 07/03/2002 - Warren Moore
//
// $Id: CortonaUtil.h,v 1.3 2002/03/20 21:57:19 vap-warren Exp $

#ifndef __CORTONA_UTIL__
#define __CORTONA_UTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

   bool CreateVrmlFromString(const char *pcVrml, INodeObject **ppNode);
   // Create a VRML node from a string
   // Address of node left in *ppNode

   bool AddToScene(INodeObject* pNode);
   // Add a node to the list of root nodes

   bool GetNode(const char *pcName, INodeObject **ppNodeCol);
   // Get a node by it's DEF'd name
   // Address of node left in ppNode

   bool AddRoute(INodeObject *pSrcNode, const char *pcSrcField,
                 INodeObject *pDstNode, const char *pcDstField);
   // Add a route from the src node's field to the destination node's field

   bool GetField(INodeObject *pNode, const char *pcName, CCortonaField **ppoField);
   // Creates a CCortonaField object for the specified field of the supplied node
   // Address of field object left in *ppoField

   bool CreateField(const char *pcType, CCortonaField **ppoField);
   // Create an empty field

//#===--- Member Variables
protected:
   IEngine *m_pEngine;
   // Cortona Engine interface

};

#endif // __CORTONA_UTIL__
