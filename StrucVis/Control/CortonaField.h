//=============---
// Cortona Base
//-------------
// Base application in which to develop specific Cortona functionality
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.h
// 12/03/2002 - Warren Moore
//
// $Id: CortonaField.h,v 1.5 2002/03/21 14:32:07 vap-warren Exp $

#ifndef __CORTONAFIELD__
#define __CORTONAFIELD__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Automation interfaces for Cortona
#include "shelley.h"

class CCortonaField {
//#===--- Construction/Destruction
public:
   CCortonaField(IFieldObject *pField);
   // Create from another field

   ~CCortonaField();

//#===--- Member Functions

   FTYPE Type() const;
   // Returns the field type

   void Release();
   // Release the field object

   //--- MFVec3f Functions
   // These access functions could be extensively improved

   bool GetMFVec3f(const long liIndex, float &fX, float &fY, float &fZ);
   // Returns the X, Y, Z values at the specified index

   bool GetMFVec3f();
   // Returns an array of the X, Y, Z values

   bool SetMFVec3f(const long liIndex, const float fX, const float fY, const float fZ);
   // Sets the X, Y, Z values at the specified index

   //--- SFVec3f Functions
   // These access functions could be extensively improved

   bool GetSFVec3f(float &fX, float &fY, float &fZ);
   // Returns the X, Y, Z values

   bool SetSFVec3f(const float fX, const float fY, const float fZ);
   // Sets the X, Y, Z values

   //--- MFColor

   bool GetMFColor(const long liIndex, float &fR, float &fG, float &fB);
   // Gets the RGB colour values

   bool SetMFColor(const long liIndex, const float fR, const float fG, const float fB);
   // Sets the RGB colour values

   //--- SFBool

   bool GetSFBool(bool &bVal);
   // Returns the bool value

   bool SetSFBool(const bool bVal);
   // Sets the bool value

protected:

   IFieldObject *Interface() const;
   // Returns the field object interface

   // Friend class for protected functions
   friend class CCortonaUtil;

//#===--- Member Variables
protected:
   IFieldObject *m_pField;       // The field interface
   FTYPE m_eType;                // The field type

};

//#===--- Inlines

inline FTYPE CCortonaField::Type() const {
   return m_eType;
}

inline IFieldObject *CCortonaField::Interface() const {
   return m_pField;
}

#endif // __CORTONAFIELD__
