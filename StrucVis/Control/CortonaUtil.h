//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaUtil.h
// 07/03/2002 - Warren Moore
//
// $Id: CortonaUtil.h,v 1.1 2002/03/18 23:01:09 vap-warren Exp $

#ifndef __CORTONABASE_UTIL__
#define __CORTONABASE_UTIL__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

   bool GetNodeByName(const char *pcName, INodeObject **ppNodeCol);
   // Get a node by it's DEF'd name
   // Address of node left in ppNode

   bool AddRoute(INodeObject *pSrcNode, const char *pcSrcField,
                 INodeObject *pDstNode, const char *pcDstField);
   // Add a route from the src node's field to the destination node's field

//#===--- Member Variables
protected:
   IEngine *m_pEngine;
   // Cortona Engine interface

};

#endif // __CORTONABASE_UTIL__
