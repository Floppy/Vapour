//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.h
// 07/03/2002 - Warren Moore
//
// $Id: CortonaField.h,v 1.2 2002/03/19 23:34:31 vap-james Exp $

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
   // Returns the Y value at the specified index

   bool GetMFVec3f_Z(long liIndex, float &fZ);
   // Returns the Z value at the specified index

   bool SetMFVec3f_X(long liIndex, const float fX);
   // Sets the X value at the specified index

   bool SetMFVec3f_Y(long liIndex, const float fY);
   // Sets the Y value at the specified index

   bool SetMFVec3f_Z(long liIndex, const float fZ);
   // Sets the Z value at the specified index

   //--- SFVec3f Functions
   // These access functions could be extensively improved

   bool GetSFVec3f_X(float &fX);
   // Returns the X value

   bool GetSFVec3f_Y(float &fY);
   // Returns the Y value

   bool GetSFVec3f_Z(float &fZ);
   // Returns the Z value

   bool SetSFVec3f_X(const float fX);
   // Sets the X value

   bool SetSFVec3f_Y(const float fY);
   // Sets the Y value

   bool SetSFVec3f_Z(const float fZ);
   // Sets the Z value

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
