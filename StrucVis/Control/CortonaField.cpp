//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.cpp
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.cpp,v 1.1 2002/03/19 01:48:59 vap-warren Exp $

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

bool CCortonaField::GetMFVec3f_X(long liIndex, float &fX) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->get_X(liIndex, &fX);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFVec3f_Y(long liIndex, float &fY) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->get_Y(liIndex, &fY);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::GetMFVec3f_Z(long liIndex, float &fZ) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->get_Z(liIndex, &fZ);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFVec3f_X(long liIndex, const float fX) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->put_X(liIndex, fX);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFVec3f_Y(long liIndex, const float fY) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->put_Y(liIndex, fY);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

bool CCortonaField::SetMFVec3f_Z(long liIndex, const float fZ) {
   // Check the field pointer
   if (!m_pField)
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
   hResult = pMFVec3f->put_Z(liIndex, fZ);

   // Release the MFVec3f interface
   pMFVec3f->Release();

   return SUCCEEDED(hResult);
}

