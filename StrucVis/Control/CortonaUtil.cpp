//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaUtil.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaUtil.cpp,v 1.8 2002/03/24 23:00:08 vap-james Exp $

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

CCortonaNode *CCortonaUtil::CreateVrmlFromString(const char *pcVRML) {
   // Check the pointers
   if (!m_pEngine || !pcVRML)
      return NULL;

   // Create the VRML string
   INodeObject *pNodeObject = NULL;
   COleVariant oVRML(pcVRML);
   HRESULT hResult = m_pEngine->CreateNodeFromString(oVRML.bstrVal, &pNodeObject);

   // If ok, create the node object
   CCortonaNode *poNode = NULL;
   if (SUCCEEDED(hResult)) {
      poNode = (CCortonaNode*) new CCortonaNode(pNodeObject);
   }

   return poNode;
}

bool CCortonaUtil::AddToScene(const CCortonaNode &oNode) {
   // Check the pointers
   if (!m_pEngine)
      return false;

   // Get root nodes collection
   INodesCollection* pRoots;
   if (FAILED(m_pEngine->get_RootNodes(&pRoots)))
      return false;

   // Create an empty variant
   VARIANT varDef;
   VariantInit(&varDef);
   varDef.vt = VT_ERROR;
   varDef.lVal = 0;

   // Add the given node to the root nodes collection
   HRESULT hResult = pRoots->Add(oNode.Interface(), &varDef);
   pRoots->Release();
    
   return SUCCEEDED(hResult);
}

CCortonaNode *CCortonaUtil::GetNode(const char *pcName) {
   // Check the pointers
   if (!m_pEngine || !pcName)
      return NULL;

   // Get all the named nodes
   INodesCollection* pNodes;
   HRESULT hResult = m_pEngine->get_Nodes(&pNodes);
   if (FAILED(hResult))
      return NULL;

   // Get the item by the name
   INodeObject *pNodeObject = NULL;
   COleVariant oNodeName(pcName);
   hResult = pNodes->get_Item(oNodeName, &pNodeObject);

   // If ok, create the node object
   CCortonaNode *poNode = NULL;
   if (SUCCEEDED(hResult)) {
      poNode = (CCortonaNode*) new CCortonaNode(pNodeObject);
   }

   return poNode;
}

bool CCortonaUtil::AddRoute(const CCortonaNode &oSrcNode, const char *pcSrcField,
                            const CCortonaNode &oDstNode, const char *pcDstField) {
   // Check the pointers
   if (!m_pEngine || !pcSrcField || !pcDstField)
      return false;

   // Create the field name strings
   COleVariant oSrcField(pcSrcField);
   COleVariant oDstField(pcDstField);

   HRESULT hResult = m_pEngine->AddRoute(oSrcNode.Interface(),
                                         oSrcField.bstrVal,
                                         oDstNode.Interface(),
                                         oDstField.bstrVal);

   return SUCCEEDED(hResult);
}

CCortonaField *CCortonaUtil::CreateField(const char *pcType) {
   // Check for pointers
   if (!m_pEngine || !pcType)
      return NULL;

   // Create the field
   IFieldObject *pFieldObject = NULL;
   COleVariant oFieldType(pcType);
   HRESULT hResult = m_pEngine->CreateField(oFieldType.bstrVal, &pFieldObject);

   // If ok, create the field object
   CCortonaField *poField = NULL;
   if (SUCCEEDED(hResult)) {
      poField = (CCortonaField*) new CCortonaField(pFieldObject);
   }

   return poField;
}

bool CCortonaUtil::Clear(void) {
   // Check for pointers
   if (!m_pEngine)
      return false;

   // Get root nodes collection
   INodesCollection* pRoots;
   if (FAILED(m_pEngine->get_RootNodes(&pRoots)))
      return false;

   // Clear the root nodes collection
   pRoots->Clear();
   pRoots->Release();

   return true;
}