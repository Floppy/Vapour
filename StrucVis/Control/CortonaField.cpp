//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.cpp,v 1.19 2002/04/05 08:56:21 vap-warren Exp $

#include "stdafx.h"

#include "CortonaField.h"

#include <comdef.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// CCortonaField

CCortonaField::CCortonaField(IFieldObject *pField) : m_pField(pField) {
   // Check we have a proper field and get the type
   m_uInterface.m_pMFColor = NULL;
   if (m_pField) {
      m_pField->get_Type(&m_eType);
      // Get the specific interface
      switch (m_eType) {
         case tMFColor:
            m_pField->QueryInterface(IID_IMFColorObject, (void**)&m_uInterface.m_pMFColor);
            break;
         case tMFInt32:
            m_pField->QueryInterface(IID_IMFInt32Object, (void**)&m_uInterface.m_pMFInt32);
            break;
         case tMFString:
            m_pField->QueryInterface(IID_IMFStringObject, (void**)&m_uInterface.m_pMFString);
            break;
         case tMFVec3f:
            m_pField->QueryInterface(IID_IMFVec3fObject, (void**)&m_uInterface.m_pMFVec3f);
            break;
         case tSFBool:
            m_pField->QueryInterface(IID_ISFBoolObject, (void**)&m_uInterface.m_pSFBool);
            break;
         case tSFRotation:
            m_pField->QueryInterface(IID_ISFRotationObject, (void**)&m_uInterface.m_pSFRotation);
            break;
         case tSFVec3f:
            m_pField->QueryInterface(IID_ISFVec3fObject, (void**)&m_uInterface.m_pSFVec3f);
            break;
         default:
            TRACE("Unknown field type\n");
      }
   }
}

CCortonaField::~CCortonaField() {
}

void CCortonaField::Release() {
   // If we have a field, set it free
   if (m_pField) {
      m_pField->Release();
      // Release specific interface
      if (m_uInterface.m_pMFColor) {
         switch (m_eType) {
            case tMFColor:
               m_uInterface.m_pMFColor->Release();
               break;
            case tMFInt32:
               m_uInterface.m_pMFInt32->Release();
               break;
            case tMFString:
               m_uInterface.m_pMFString->Release();
               break;
            case tMFVec3f:
               m_uInterface.m_pMFVec3f->Release();
               break;
            case tSFBool:
               m_uInterface.m_pSFBool->Release();
               break;
            case tSFRotation:
               m_uInterface.m_pSFRotation->Release();
               break;
            case tSFVec3f:
               m_uInterface.m_pSFVec3f->Release();
               break;
            default:
               TRACE("Unknown field type\n");
         }
      }
   }
}

long CCortonaField::GetMFCount() {
   // Make sure we have an MF type
   if (((int)m_eType) < 216)
      return false;

   // Get the IMField interface
   IMFieldObject *pMField = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFieldObject, (void**)&pMField);
   if (FAILED(hResult))
      return false;

   // Get the field count
   long liCount = 0;
   hResult = pMField->get_Count(&liCount);

   // Release the interface
   pMField->Release();

   return liCount;
}

bool CCortonaField::SetMFCount(const long liCount) {
   // Make sure we have an MF type
   if (((int)m_eType) < 216 || (liCount < 0))
      return false;

   // Get the IMField interface
   IMFieldObject *pMField = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFieldObject, (void**)&pMField);
   if (FAILED(hResult))
      return false;

   // Set the count value
   hResult = pMField->put_Count(liCount);

   // Release the interface
   pMField->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFVec3f(const long liIndex, float &fX, float &fY, float &fZ) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFVec3f || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   HRESULT hResult = m_uInterface.m_pMFVec3f->get_Value(liIndex, &sVar);
   // If we got the variant array, get access to the data
   bool bOk = false;
   if (SUCCEEDED(hResult)) {
      VARIANT *pVarArray = NULL;
      hResult = SafeArrayAccessData(sVar.parray, (void**)&pVarArray);
      // If we got access, get each element
      if (SUCCEEDED(hResult)) {
         // Check each type
         bOk  = (pVarArray[0].vt == VT_R4);
         bOk &= (pVarArray[1].vt == VT_R4);
         bOk &= (pVarArray[2].vt == VT_R4);
         if (bOk) {
            // All types checked so get the values
            fX = pVarArray[0].fltVal;
            fY = pVarArray[1].fltVal;
            fZ = pVarArray[2].fltVal;
         }
      }
      SafeArrayUnaccessData(sVar.parray);
   }

   // Clear the variant
   VariantClear(&sVar);

   return bOk;
}

bool CCortonaField::TestMFVec3f() {
   // Experimental function to get entire MFVec3f array using undocumented interface

   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   /*
   // Get the values
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Get the complete value
   hResult = pVariable->get_Contents(&sVarArray);
   // If we got the variant array, get access to the data
   if (SUCCEEDED(hResult)) {
      float *pfArray = NULL;
      hResult = SafeArrayAccessData(sVarArray.parray, (void**)&pfArray);
      // If we got access, get each element
//      if (SUCCEEDED(hResult)) {
//      }
      SafeArrayUnaccessData(sVarArray.parray);
   }

   // Clear the variant
   VariantClear(&sVarArray);
   */

   // Get the IVariable interface
   IVariable *pVariable = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IVariable, (void**)&pVariable);
   if (FAILED(hResult)) 
      return false;

   // Create the safe array
   const int iCount = 6;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_R4, 1, sSABound);

   // Gain access to the values
   float *pfVal = NULL;
   hResult = SafeArrayAccessData(pSA, (void**)&pfVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      pfVal[0] = 0.0f;
      pfVal[1] = 1.0f;
      pfVal[2] = 2.0f;
      pfVal[3] = 3.0f;
      pfVal[4] = 4.0f;
      pfVal[5] = 5.0f;
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      pVariable->Release();
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_R4;
   sVarArray.parray = pSA;

   // Add the value
   hResult = pVariable->put_Contents(&sVarArray);

   // Clear the variant
   VariantClear(&sVarArray);

   // Release the interface
   pVariable->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFVec3f(const long liIndex, const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFVec3f || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fX,
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fY,
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fZ,
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   // Add the value
   hResult = m_uInterface.m_pMFVec3f->put_Value(liIndex, sVarArray);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFVec3f(const float *pfVal, const unsigned int uLen) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFVec3f || uLen == 0)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Loop vars
   float *pfPos = const_cast<float*>(pfVal);
   HRESULT hResult = S_OK;

   // Loop through each value
   for (unsigned int i = 0; i < uLen && SUCCEEDED(hResult); i++) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
      if (SUCCEEDED(hResult)) {
         // Populate the array
         VariantInit(pVal);
         pVal[0].vt = VT_R4;
         pVal[0].fltVal = *pfVal++;
         VariantInit(pVal + 1);
         pVal[1].vt = VT_R4;
         pVal[1].fltVal = *pfVal++;
         VariantInit(pVal + 2);
         pVal[2].vt = VT_R4;
         pVal[2].fltVal = *pfVal++;
         SafeArrayUnaccessData(pSA);
      }
      else {
         SafeArrayDestroy(pSA);
         return false;
      }

      // Initialise the variant
      VARIANT sVarArray;
      VariantInit(&sVarArray);

      // Set the type and value
      sVarArray.vt = VT_ARRAY | VT_VARIANT;
      sVarArray.parray = pSA;

      // Add the value
      hResult = m_uInterface.m_pMFVec3f->put_Value(i, sVarArray);

      // Clear the variant
      VariantClear(&sVarArray);
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFVec3f(const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFVec3f)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fX;
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fY;
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fZ;
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   // Set the optional before param
   VARIANT sVarBefore;
   VariantInit(&sVarBefore);
   sVarBefore.vt = VT_ERROR;
   sVarBefore.scode = DISP_E_PARAMNOTFOUND;

   // Add the value
   hResult = m_uInterface.m_pMFVec3f->Add(&sVarArray, &sVarBefore);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetSFVec3f(float &fX, float &fY, float &fZ) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFVec3f)
      return false;

   // Check the type
   if (m_eType != tSFVec3f)
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   HRESULT hResult = m_uInterface.m_pSFVec3f->get_Value(&sVar);
   // If we got the variant array, get access to the data
   bool bOk = false;
   if (SUCCEEDED(hResult)) {
      VARIANT *pVarArray = NULL;
      hResult = SafeArrayAccessData(sVar.parray, (void**)&pVarArray);
      // If we got access, get each element
      if (SUCCEEDED(hResult)) {
         // Check each type
         bOk  = (pVarArray[0].vt == VT_R4);
         bOk &= (pVarArray[1].vt == VT_R4);
         bOk &= (pVarArray[2].vt == VT_R4);
         if (bOk) {
            // All types checked so get the values
            fX = pVarArray[0].fltVal;
            fY = pVarArray[1].fltVal;
            fZ = pVarArray[2].fltVal;
         }
      }
      SafeArrayUnaccessData(sVar.parray);
   }

   // Clear the variant
   VariantClear(&sVar);

   return bOk;
}

bool CCortonaField::SetSFVec3f(const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFVec3f)
      return false;

   // Check the type
   if (m_eType != tSFVec3f)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fX;
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fY;
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fZ;
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   // Add the value
   hResult = m_uInterface.m_pSFVec3f->put_Value(sVarArray);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFColor(const long liIndex, float &fR, float &fG, float &fB) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFColor || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   HRESULT hResult = m_uInterface.m_pMFColor->get_Value(liIndex, &sVar);
   // If we got the variant array, get access to the data
   bool bOk = false;
   if (SUCCEEDED(hResult)) {
      VARIANT *pVarArray = NULL;
      hResult = SafeArrayAccessData(sVar.parray, (void**)&pVarArray);
      // If we got access, get each element
      if (SUCCEEDED(hResult)) {
         // Check each type
         bOk  = (pVarArray[0].vt == VT_R4);
         bOk &= (pVarArray[1].vt == VT_R4);
         bOk &= (pVarArray[2].vt == VT_R4);
         if (bOk) {
            // All types checked so get the values
            fR = pVarArray[0].fltVal;
            fG = pVarArray[1].fltVal;
            fB = pVarArray[2].fltVal;
         }
      }
      SafeArrayUnaccessData(sVar.parray);
   }

   // Clear the variant
   VariantClear(&sVar);

   return bOk;
}

bool CCortonaField::SetMFColor(const long liIndex, const float fR, const float fG, const float fB) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFColor || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fR;
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fG;
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fB;
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   // Add the value
   hResult = m_uInterface.m_pMFColor->put_Value(liIndex, sVarArray);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFColor(const float *pfVal, const unsigned int uLen) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFColor || uLen == 0)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Loop vars
   float *pfPos = const_cast<float*>(pfVal);
   HRESULT hResult = S_OK;

   // Loop through each value
   for (unsigned int i = 0; i < uLen && SUCCEEDED(hResult); i++) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
      if (SUCCEEDED(hResult)) {
         // Populate the array
         VariantInit(pVal);
         pVal[0].vt = VT_R4;
         pVal[0].fltVal = *pfVal++;
         VariantInit(pVal + 1);
         pVal[1].vt = VT_R4;
         pVal[1].fltVal = *pfVal++;
         VariantInit(pVal + 2);
         pVal[2].vt = VT_R4;
         pVal[2].fltVal = *pfVal++;
         SafeArrayUnaccessData(pSA);
      }
      else {
         SafeArrayDestroy(pSA);
         return false;
      }

      // Initialise the variant
      VARIANT sVarArray;
      VariantInit(&sVarArray);

      // Set the type and value
      sVarArray.vt = VT_ARRAY | VT_VARIANT;
      sVarArray.parray = pSA;

      // Add the value
      hResult = m_uInterface.m_pMFColor->put_Value(i, sVarArray);

      // Clear the variant
      VariantClear(&sVarArray);
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFColor(const float fR, const float fG, const float fB) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFColor)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fR,
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fG;
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fB;
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   // Set the optional before param
   VARIANT sVarBefore;
   VariantInit(&sVarBefore);
   sVarBefore.vt = VT_ERROR;
   sVarBefore.scode = DISP_E_PARAMNOTFOUND;

   // Add the value
   hResult = m_uInterface.m_pMFColor->Add(&sVarArray, &sVarBefore);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetSFBool(bool &bVal) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFBool)
      return false;

   // Check the type
   if (m_eType != tSFBool)
      return false;

   // Get the BOOL value
   BOOL bRet;
   HRESULT hResult = m_uInterface.m_pSFBool->get_Value(&bRet);
   bVal = (bRet == TRUE);

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetSFBool(const bool bVal) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFBool)
      return false;

   // Check the type
   if (m_eType != tSFBool)
      return false;

   // Set the BOOL value
   HRESULT hResult = m_uInterface.m_pSFBool->put_Value(bVal);

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetSFRotation(float &fX, float &fY, float &fZ, float &fAngle) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFRotation)
      return false;

   // Check the type
   if (m_eType != tSFRotation)
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   HRESULT hResult = m_uInterface.m_pSFRotation->get_Value(&sVar);
   // If we got the variant array, get access to the data
   bool bOk = false;
   if (SUCCEEDED(hResult)) {
      VARIANT *pVarArray = NULL;
      hResult = SafeArrayAccessData(sVar.parray, (void**)&pVarArray);
      // If we got access, get each element
      if (SUCCEEDED(hResult)) {
         // Check each type
         bOk  = (pVarArray[0].vt == VT_R4);
         bOk &= (pVarArray[1].vt == VT_R4);
         bOk &= (pVarArray[2].vt == VT_R4);
         bOk &= (pVarArray[3].vt == VT_R4);
         if (bOk) {
            // All types checked so get the values
            fX = pVarArray[0].fltVal;
            fY = pVarArray[1].fltVal;
            fZ = pVarArray[2].fltVal;
            fAngle = pVarArray[3].fltVal;
         }
      }
      SafeArrayUnaccessData(sVar.parray);
   }

   // Clear the variant
   VariantClear(&sVar);

   return bOk;
}

bool CCortonaField::SetSFRotation(const float fX, const float fY, const float fZ, const float fAngle) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pSFRotation)
      return false;

   // Check the type
   if (m_eType != tSFRotation)
      return false;

   // Create the safe array
   const int iCount = 4;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 0;
   pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

   // Gain access to the values
   VARIANT *pVal = NULL;
   HRESULT hResult = SafeArrayAccessData(pSA, (void**)&pVal);
   if (SUCCEEDED(hResult)) {
      // Populate the array
      VariantInit(pVal);
      pVal[0].vt = VT_R4;
      pVal[0].fltVal = fX,
      VariantInit(pVal + 1);
      pVal[1].vt = VT_R4;
      pVal[1].fltVal = fY,
      VariantInit(pVal + 2);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fZ,
      VariantInit(pVal + 3);
      pVal[2].vt = VT_R4;
      pVal[2].fltVal = fAngle,
      SafeArrayUnaccessData(pSA);
   }
   else {
      SafeArrayDestroy(pSA);
      return false;
   }

   // Initialise the variant
   VARIANT sVarArray;
   VariantInit(&sVarArray);

   // Set the type and value
   sVarArray.vt = VT_ARRAY | VT_VARIANT;
   sVarArray.parray = pSA;

   hResult = m_uInterface.m_pSFRotation->put_Value(sVarArray);

   // Clear the variant
   VariantClear(&sVarArray);

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFInt32(const long liIndex, long &liValue) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFInt32 || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Get the values
   HRESULT hResult = m_uInterface.m_pMFInt32->get_Value(liIndex, &liValue);

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFInt32(const long liIndex, const long liValue) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFInt32 || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Set the values
   HRESULT hResult = m_uInterface.m_pMFInt32->put_Value(liIndex, liValue);

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFInt32(const long liValue) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFInt32)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Set the optional before param
   VARIANT sVarBefore;
   VariantInit(&sVarBefore);
   sVarBefore.vt = VT_ERROR;
   sVarBefore.scode = DISP_E_PARAMNOTFOUND;

   // Add the values
   HRESULT hResult = m_uInterface.m_pMFInt32->Add(liValue, &sVarBefore);

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFString(const long liIndex, const char* pcString) {
   // Check the field pointer
   if (!m_pField || !m_uInterface.m_pMFString || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFString)
      return false;

   // Convert to BSTR
   COleVariant oStr(pcString);

   // Put the value
   HRESULT hResult = m_uInterface.m_pMFString->put_Value(liIndex, oStr.bstrVal);

   return SUCCEEDED(hResult);
}

