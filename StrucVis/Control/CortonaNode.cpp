//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaNode.cpp"
//! author    = "Warren Moore"
//! date      = "10/3/2002"
//! rcsid     = "$Id: CortonaNode.cpp,v 1.6 2002/04/03 15:57:08 vap-warren Exp $"

#include "stdafx.h"

#include "CortonaNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CCortonaNode

CCortonaNode::CCortonaNode(INodeObject *pNode) : m_pNode(pNode) {
}

CCortonaNode::~CCortonaNode() {
}

void CCortonaNode::Release() {
   // If we have a node, set it free
   if (m_pNode)
      m_pNode->Release();
}

CCortonaField *CCortonaNode::GetField(const char *pcName) {
   // Check for pointers
   if (!pcName)
      return NULL;

   // Get the node fields
   IFieldsCollection *pFields = NULL;
   HRESULT hResult = m_pNode->get_Fields(&pFields);
   if (FAILED(hResult))
      return NULL;

   // Get the specified field
   IFieldObject *pFieldObject = NULL;
   COleVariant oFieldName(pcName);
   hResult = pFields->get_Item(oFieldName, &pFieldObject);

   // If ok, create the field object
   CCortonaField *poField = NULL;
   if (SUCCEEDED(hResult)) {
      // Create the field object
      poField = (CCortonaField*) new CCortonaField(pFieldObject);
   }

   // Release the field collection
   pFields->Release();

   return poField;
}

bool CCortonaNode::AssignEventIn(const char *pcField, const CCortonaField &oValue) {
   // Chec the pointers
   if (!m_pNode)
      return false;

   // Get the node fields
   IFieldsCollection *pFields = NULL;
   HRESULT hResult = m_pNode->get_Fields(&pFields);
   if (FAILED(hResult))
      return false;

   // Get the specified field
   IFieldObject *pFieldObject = NULL;
   COleVariant oFieldName(pcField);
   hResult = pFields->get_Item(oFieldName, &pFieldObject);

   // If we got the field, check that it's an EventIn field
   if (SUCCEEDED(hResult)) {
      FCategory eCategory;
      pFieldObject->get_Category(&eCategory);
      if (eCategory == cEventIn) {
         // If it is an EventIn, check the field types are compatible
         FTYPE eType;
         pFieldObject->get_Type(&eType);
         if (eType == oValue.Type()) {
            // All checked, assign the value
            pFieldObject->Assign(oValue.Interface());
         }
      }
      pFieldObject->Release();
   }

   // Release the field collection
   pFields->Release();

   return SUCCEEDED(hResult);
}
