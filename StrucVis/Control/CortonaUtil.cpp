//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaUtil.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaUtil.cpp,v 1.2 2002/03/19 01:49:00 vap-warren Exp $

#include "stdafx.h"
#include "CortonaBase.h"

#include "CortonaUtil.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// To link in all the GUIDs
#include "shelley_i.c"

/////////////////
// CCortonaUtil

CCortonaUtil::CCortonaUtil(IEngine *pEngine) : m_pEngine(pEngine) {
}

CCortonaUtil::~CCortonaUtil() {
}

bool CCortonaUtil::CreateVrmlFromString(const char *pcVRML, INodeObject **ppNode) {
   // Check the pointers
   if (!m_pEngine || !pcVRML || !ppNode)
      return false;

   // Create the VRML string
   COleVariant oVRML(pcVRML);
   HRESULT hResult = m_pEngine->CreateNodeFromString(oVRML.bstrVal, ppNode);

   return SUCCEEDED(hResult);
}

bool CCortonaUtil::AddToScene(INodeObject *pNode) {
   if (!m_pEngine || !pNode)
      return false;

   // We get root nodes collection
   INodesCollection* pRoots;
   if (FAILED(m_pEngine->get_RootNodes(&pRoots)))
      return false;

   // NOTE: you can pass the next VARIANT if an operand in method is optional
   VARIANT varDef;
   VariantInit(&varDef);
   varDef.vt = VT_ERROR;
   varDef.lVal = 0;

   // Add the given node to the root nodes collection
   HRESULT hResult = pRoots->Add(pNode, &varDef);
   pRoots->Release();
    
   return SUCCEEDED(hResult);
}

bool CCortonaUtil::GetNode(const char *pcName, INodeObject **ppNode) {
   // Check the pointers
   if (!m_pEngine || !pcName)
      return false;

   // Recipients for the 
   INodesCollection* pNodes;

   // Get all the named nodes
   HRESULT hResult = m_pEngine->get_Nodes(&pNodes);
   if (FAILED(hResult))
      return false;

   // Get the item by the name
   COleVariant oNodeName(pcName);
   hResult = pNodes->get_Item(oNodeName, ppNode);

   return SUCCEEDED(hResult);
}

bool CCortonaUtil::AddRoute(INodeObject *pSrcNode, const char *pcSrcField,
                            INodeObject *pDstNode, const char *pcDstField) {
   // Check the pointers
   if (!pSrcNode || !pcSrcField || !pDstNode || !pcDstField)
      return false;

   // Create the field name strings
   COleVariant oSrcField(pcSrcField);
   COleVariant oDstField(pcDstField);

   HRESULT hResult = m_pEngine->AddRoute(pSrcNode, oSrcField.bstrVal, pDstNode, oDstField.bstrVal);

   return SUCCEEDED(hResult);
}

bool CCortonaUtil::GetField(INodeObject *pNode, const char *pcName, CCortonaField **ppoField) {
   // Check for pointers
   if (!m_pEngine || !pNode || !pcName)
      return false;

   // Get the node fields
   IFieldsCollection *pFields = NULL;
   HRESULT hResult = pNode->get_Fields(&pFields);
   if (FAILED(hResult))
      return false;

   // Get the specified field
   IFieldObject *pFieldObject = NULL;
   COleVariant oFieldName(pcName);
   hResult = pFields->get_Item(oFieldName, &pFieldObject);
   if (SUCCEEDED(hResult)) {
      // Create the field object
      *ppoField = (CCortonaField*) new CCortonaField(pFieldObject);
   }

   // Release the field collection
   pFields->Release();
   return SUCCEEDED(hResult);
}

