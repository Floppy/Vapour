//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.cpp,v 1.9 2002/03/24 13:43:49 vap-warren Exp $

#include "stdafx.h"
#include "CortonaBase.h"

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
   if (SUCCEEDED(hResult)) {
      // Release the MFVec3f interface
      pMFVec3f->Release();
   }

   return SUCCEEDED(hResult);
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
   if (FAILED(hResult)) {
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
      hResult = pMFVec3f->put_Value(liIndex, sVarArray);

      /*
      // Get the X value
      bool bOk = SUCCEEDED(pMFVec3f->put_X(liIndex, fX));
      bOk &= SUCCEEDED(pMFVec3f->put_Y(liIndex, fY));
      bOk &= SUCCEEDED(pMFVec3f->put_Z(liIndex, fZ));
      */

      // Release the MFVec3f interface
      pMFVec3f->Release();
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFVec3f(const float fX, const float fY, const float fZ) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFVec3f)
      return false;

   // Get the IMFVec3f interface
   IMFVec3fObject *pMFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFVec3fObject, (void**)&pMFVec3f);
   if (SUCCEEDED(hResult)) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      hResult = SafeArrayAccessData(pSA, (void**)&pVal);
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
      hResult = pMFVec3f->Add(&sVarArray, &sVarBefore);

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
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   hResult = pSFVec3f->get_Value(&sVar);
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

   /*
   // Get the values
   bool bOk = SUCCEEDED(pSFVec3f->get_X(&fX));
   bOk &= SUCCEEDED(pSFVec3f->get_Y(&fY));
   bOk &= SUCCEEDED(pSFVec3f->get_Z(&fZ));
   */

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

   // Get the ISFVec3f interface
   ISFVec3fObject *pSFVec3f = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFVec3fObject, (void**)&pSFVec3f);
   if (SUCCEEDED(hResult)) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      hResult = SafeArrayAccessData(pSA, (void**)&pVal);
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
      hResult = pSFVec3f->put_Value(sVarArray);

      /*
      // Set the X value
      bool bOk = SUCCEEDED(pSFVec3f->put_X(fX));
      bOk &= SUCCEEDED(pSFVec3f->put_Y(fY));
      bOk &= SUCCEEDED(pSFVec3f->put_Z(fZ));
      */

      // Release the MFVec3f interface
      pSFVec3f->Release();
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFColor(const long liIndex, float &fR, float &fG, float &fB) {
   // Check the field pointer
   if (!m_pField || liIndex < 0)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Get the IMFColor interface
   IMFColorObject *pMFColor = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFColorObject, (void**)&pMFColor);
   if (FAILED(hResult))
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   hResult = pMFColor->get_Value(liIndex, &sVar);
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

   /*
   // Get the values
   bool bOk = SUCCEEDED(pMFColor->get_Red(liIndex, &fR));
   bOk &= SUCCEEDED(pMFColor->get_Green(liIndex, &fG));
   bOk &= SUCCEEDED(pMFColor->get_Blue(liIndex, &fB));
   */

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
   if (SUCCEEDED(hResult)) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      hResult = SafeArrayAccessData(pSA, (void**)&pVal);
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
      hResult = pMFColor->put_Value(liIndex, sVarArray);

      /*
      // Get the X value
      bool bOk = SUCCEEDED(pMFColor->put_Red(liIndex, fR));
      bOk &= SUCCEEDED(pMFColor->put_Green(liIndex, fG));
      bOk &= SUCCEEDED(pMFColor->put_Blue(liIndex, fB));
      */

      // Release the MFVec3f interface
      pMFColor->Release();
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFColor(const float fR, const float fG, const float fB) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFColor)
      return false;

   // Get the IMFVec3f interface
   IMFVec3fObject *pMFColor = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFColorObject, (void**)&pMFColor);
   if (SUCCEEDED(hResult)) {
      // Create the safe array
      const int iCount = 3;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      hResult = SafeArrayAccessData(pSA, (void**)&pVal);
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
      hResult = pMFColor->Add(&sVarArray, &sVarBefore);

      // Release the MFVec3f interface
      pMFColor->Release();
   }

   return SUCCEEDED(hResult);
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

bool CCortonaField::GetSFRotation(float &fX, float &fY, float &fZ, float &fAngle) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tSFRotation)
      return false;

   // Get the ISFRotation interface
   ISFRotationObject *pSFRotation = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFRotationObject, (void**)&pSFRotation);
   if (FAILED(hResult))
      return false;

   // Get the values
   VARIANT sVar;
   VariantInit(&sVar);

   // Get the complete value
   hResult = pSFRotation->get_Value(&sVar);
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

   /*
   // Get the values
   bool bOk = SUCCEEDED(pSFRotation->get_X(&fX));
   bOk &= SUCCEEDED(pSFRotation->get_Y(&fY));
   bOk &= SUCCEEDED(pSFRotation->get_Z(&fZ));
   bOk &= SUCCEEDED(pSFRotation->get_Angle(&fAngle));
   */

   // Release the MFVec3f interface
   pSFRotation->Release();

   return bOk;
}

bool CCortonaField::SetSFRotation(const float fX, const float fY, const float fZ, const float fAngle) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tSFRotation)
      return false;

   // Get the ISFRotation interface
   ISFRotationObject *pSFRotation = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_ISFRotationObject, (void**)&pSFRotation);
   if (SUCCEEDED(hResult)) {
      // Create the safe array
      const int iCount = 4;
      SAFEARRAYBOUND sSABound[1];
      SAFEARRAY *pSA;

      sSABound[0].cElements = iCount;
      sSABound[0].lLbound = 0;
      pSA = SafeArrayCreate(VT_VARIANT, 1, sSABound);

      // Gain access to the values
      VARIANT *pVal = NULL;
      hResult = SafeArrayAccessData(pSA, (void**)&pVal);
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

      hResult = pSFRotation->put_Value(sVarArray);
      /*
      // Get the X value
      bool bOk = SUCCEEDED(pSFRotation->put_X(fX));
      bOk &= SUCCEEDED(pSFRotation->put_Y(fY));
      bOk &= SUCCEEDED(pSFRotation->put_Z(fZ));
      bOk &= SUCCEEDED(pSFRotation->put_Angle(fAngle));
      */

      // Release the SFRotation interface
      pSFRotation->Release();
   }

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFInt32(const long liIndex, long &liValue) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Get the IMFVec3f interface
   IMFInt32Object *pMFInt32 = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFInt32Object, (void**)&pMFInt32);
   if (FAILED(hResult))
      return false;

   // Get the values
   hResult = pMFInt32->get_Value(liIndex, &liValue);

   // Release the MFVec3f interface
   pMFInt32->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFInt32(const long liIndex, const long liValue) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Get the IMFVec3f interface
   IMFInt32Object *pMFInt32 = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFInt32Object, (void**)&pMFInt32);
   if (FAILED(hResult))
      return false;

   // Get the values
   hResult = pMFInt32->put_Value(liIndex, liValue);

   // Release the MFVec3f interface
   pMFInt32->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::AddMFInt32(const long liValue) {
   // Check the field pointer
   if (!m_pField)
      return false;

   // Check the type
   if (m_eType != tMFInt32)
      return false;

   // Get the IMFVec3f interface
   IMFInt32Object *pMFInt32 = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFInt32Object, (void**)&pMFInt32);
   if (FAILED(hResult))
      return false;

   // Set the optional before param
   VARIANT sVarBefore;
   VariantInit(&sVarBefore);
   sVarBefore.vt = VT_ERROR;
   sVarBefore.scode = DISP_E_PARAMNOTFOUND;

   // Get the values
   hResult = pMFInt32->Add(liValue, &sVarBefore);

   // Release the MFVec3f interface
   pMFInt32->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFString(const long liIndex, const char* pcString) {
   // Check the field pointer
   if (!m_pField || liIndex < 1)
      return false;

   // Check the type
   if (m_eType != tMFString)
      return false;

   // Get the IMFString interface
   IMFStringObject *pMFString = NULL;
   HRESULT hResult = m_pField->QueryInterface(IID_IMFStringObject, (void**)&pMFString);
   if (FAILED(hResult))
      return false;

   // Convert to BSTR
   COleVariant oStr(pcString);

   // Put the value
   hResult = pMFString->put_Value(liIndex, oStr.bstrVal);

   // Release the MFString interface
   pMFString->Release();

   return SUCCEEDED(hResult);
}

