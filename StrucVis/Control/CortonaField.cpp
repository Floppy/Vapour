//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.cpp,v 1.4 2002/03/21 22:55:22 vap-warren Exp $

#include "stdafx.h"
#include "CortonaBase.h"

#include "CortonaField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// CCortonaField

CCortonaField::CCortonaField(IFieldObject *pField) : m_pField(pField) {
   // Check we have a proper field and get the type
   if (m_pField)
      m_pField->get_Type(&m_eType);
}

CCortonaField::~CCortonaField() {
}

void CCortonaField::Release() {
   // If we have a field, set it free
   if (m_pField)
      m_pField->Release();
}

bool CCortonaField::GetMFVec3f(const long liIndex, float &fX, float &fY, float &fZ) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Get the IMFVec3f interface
   IMFVec3fObject *pMFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFVec3fObject, (void**)&pMFVec3f);
   if (FAILED(hResult))
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   hResult = pMFVec3f->get_Value(liIndex, &sVar);
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

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return bOk;
}

bool CCortonaField::GetMFVec3f() {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Get the IMFVec3f interface
   IVariable *pMFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IVariable, (void**)&pMFVec3f);
   if (FAILED(hResult))
      return false;

   bool bOk = false;
   // Release the MFVec3f interface
   pMFVec3f->Release();

   return bOk;
}

bool CCortonaField::SetMFVec3f(const long liIndex, const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Get the IMFVec3f interface
   IMFVec3fObject *pMFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFVec3fObject, (void**)&pMFVec3f);
   if (FAILED(hResult))
      return false;

   // Get the X value
   bool bOk = SUCCEEDED(pMFVec3f->put_X(liIndex, fX));
   bOk &= SUCCEEDED(pMFVec3f->put_Y(liIndex, fY));
   bOk &= SUCCEEDED(pMFVec3f->put_Z(liIndex, fZ));

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return bOk;
}

bool CCortonaField::AddMFVec3f(const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Create the safe array
   const int iCount = 3;
   SAFEARRAYBOUND sSABound[1];
   SAFEARRAY *pSA;

   sSABound[0].cElements = iCount;
   sSABound[0].lLbound = 1;
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
   VARIANT sVariant;
   VariantInit(&sVariant);

   // Set the type and value
   sVariant.vt = VT_ARRAY | VT_VARIANT;
   sVariant.parray = pSA;

   // Get the IMFVec3f interface
   IMFVec3fObject *pMFVec3f = NULL;
   hResult = m_pField->QueryInterface(IID_IMFVec3fObject, (void**)&pMFVec3f);
   if (SUCCEEDED(hResult)) {
      // Add the value
      hResult = pMFVec3f->Add(&sVariant, NULL);
      // Release the MFVec3f interface
      pMFVec3f->Release();
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetSFVec3f(float &fX, float &fY, float &fZ) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tSFVec3f)
      return false;

   // Get the IMFVec3f interface
   ISFVec3fObject *pSFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFVec3fObject, (void**)&pSFVec3f);
   if (FAILED(hResult))
      return false;

   // Get the values
   bool bOk = SUCCEEDED(pSFVec3f->get_X(&fX));
   bOk &= SUCCEEDED(pSFVec3f->get_Y(&fY));
   bOk &= SUCCEEDED(pSFVec3f->get_Z(&fZ));

   // Release the MFVec3f interface
   pSFVec3f->Release();

   return bOk;
}

bool CCortonaField::SetSFVec3f(const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tSFVec3f)
      return false;

   // Get the IMFVec3f interface
   ISFVec3fObject *pSFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFVec3fObject, (void**)&pSFVec3f);
   if (FAILED(hResult))
      return false;

   // Get the X value
   bool bOk = SUCCEEDED(pSFVec3f->put_X(fX));
   bOk &= SUCCEEDED(pSFVec3f->put_Y(fY));
   bOk &= SUCCEEDED(pSFVec3f->put_Z(fZ));

   // Release the MFVec3f interface
   pSFVec3f->Release();

   return bOk;
}

bool CCortonaField::GetMFColor(const long liIndex, float &fR, float &fG, float &fB) {
   // Check the field pointer
   if (!m_pField || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Get the IMFVec3f interface
   IMFColorObject *pMFColor = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFColorObject, (void**)&pMFColor);
   if (FAILED(hResult))
      return false;

   // Get the values
   bool bOk = SUCCEEDED(pMFColor->get_Red(liIndex, &fR));
   bOk &= SUCCEEDED(pMFColor->get_Green(liIndex, &fG));
   bOk &= SUCCEEDED(pMFColor->get_Blue(liIndex, &fB));

   // Release the MFVec3f interface
   pMFColor->Release();

   return bOk;
}

bool CCortonaField::SetMFColor(const long liIndex, const float fR, const float fG, const float fB) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Get the IMFVec3f interface
   IMFColorObject *pMFColor = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFColorObject, (void**)&pMFColor);
   if (FAILED(hResult))
      return false;

   // Get the X value
   bool bOk = SUCCEEDED(pMFColor->put_Red(liIndex, fR));
   bOk &= SUCCEEDED(pMFColor->put_Green(liIndex, fG));
   bOk &= SUCCEEDED(pMFColor->put_Blue(liIndex, fB));

   // Release the MFVec3f interface
   pMFColor->Release();

   return bOk;
}

bool CCortonaField::GetSFBool(bool &bVal) {
   // Check the type
   if (m_eType != tSFBool)
      return false;

   // Get the IMFVec3f interface
   ISFBoolObject *pSFBool = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFBoolObject, (void**)&pSFBool);
   if (FAILED(hResult))
      return false;

   // Get the X value
   BOOL bRet;
   hResult = pSFBool->get_Value(&bRet);
   bVal = (bRet == TRUE);

   // Release the MFVec3f interface
   pSFBool->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetSFBool(const bool bVal) {
   // Check the type
   if (m_eType != tSFBool)
      return false;

   // Get the IMFVec3f interface
   ISFBoolObject *pSFBool = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFBoolObject, (void**)&pSFBool);
   if (FAILED(hResult))
      return false;

   // Get the X value
   hResult = pSFBool->put_Value(bVal);

   // Release the MFVec3f interface
   pSFBool->Release();

   return SUCCEEDED(hResult);
}

