//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.h
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.h,v 1.1 2002/03/19 01:48:59 vap-warren Exp $

#ifndef __CORTONA_FIELD__
#define __CORTONA_FIELD__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Automation interfaces for Cortona
#include "shelley.h"

class CCortonaField {
//#===--- Construction/Destruction
public:
   CCortonaField(IFieldObject *pField);
   ~CCortonaField();

//#===--- Member Functions

   FTYPE Type() const;
   // Returns the field type

   void Release();
   // Release the field object

   //--- MFVec3f Functions
   // These access functions could be extensively improved

   bool GetMFVec3f_X(long liIndex, float &fX);
   // Returns the X value at the specified index

   bool GetMFVec3f_Y(long liIndex, float &fY);
   // Returns the X value at the specified index

   bool GetMFVec3f_Z(long liIndex, float &fZ);
   // Returns the X value at the specified index

   bool SetMFVec3f_X(long liIndex, const float fX);
   // Sets the X value at the specified index

   bool SetMFVec3f_Y(long liIndex, const float fY);
   // Sets the X value at the specified index

   bool SetMFVec3f_Z(long liIndex, const float fZ);
   // Sets the X value at the specified index

//#===--- Member Variables
protected:
   IFieldObject *m_pField;       // The field interface
   FTYPE m_eType;                // The field type

};

//#===--- Inlines

inline FTYPE CCortonaField::Type() const {
   return m_eType;
}

#endif // __CORTONA_UTIL__
