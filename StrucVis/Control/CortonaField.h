//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// CortonaField.h
// 12/03/2002 - Warren Moore
//
// $Id: CortonaField.h,v 1.11 2002/03/26 19:21:43 vap-warren Exp $

#ifndef __VTSTRUCVIS_CORTONAFIELD__
#define __VTSTRUCVIS_CORTONAFIELD__

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

   long GetMFCount();
   // Returns the number of fields in the MF object

   bool SetMFCount(const long liCount);
   // Creates the number of fields within the MF object

   void Release();
   // Release the field object

   //--- MFVec3f Functions

   bool GetMFVec3f(const long liIndex, float &fX, float &fY, float &fZ);
   // Returns the X, Y, Z values at the specified index

   bool GetMFVec3f();
   // Returns an array of the X, Y, Z values

   bool SetMFVec3f(const long liIndex, const float fX, const float fY, const float fZ);
   // Sets the X, Y, Z values at the specified index

   bool AddMFVec3f(const float fX, const float fY, const float fZ);
   // Appends the X, Y, Z triplet to the end of the vector list

   //--- SFVec3f Functions

   bool GetSFVec3f(float &fX, float &fY, float &fZ);
   // Returns the X, Y, Z values

   bool SetSFVec3f(const float fX, const float fY, const float fZ);
   // Sets the X, Y, Z values

   //--- MFColor

   bool GetMFColor(const long liIndex, float &fR, float &fG, float &fB);
   // Gets the RGB colour values

   bool SetMFColor(const long liIndex, const float fR, const float fG, const float fB);
   // Sets the RGB colour values

   bool AddMFColor(const float fR, const float fG, const float fB);
   // Appends the RGB colour values
   // NOTE: For some damn reason, it won't use the blue value - Use SetMFCount and SetMFColor instead

   //--- SFBool

   bool GetSFBool(bool &bVal);
   // Returns the bool value

   bool SetSFBool(const bool bVal);
   // Sets the bool value

   //--- SFRotation

   bool GetSFRotation(float &fX, float &fY, float &fZ, float &fAngle);
   // Gets the rotation value

   bool SetSFRotation(const float fX, const float fY, const float fZ, const float fAngle);
   // Sets the rotation value

   //--- MFVec3f Functions

   bool GetMFInt32(const long liIndex, long &liValue);
   // Returns the value at the specified index

   bool SetMFInt32(const long liIndex, const long liValue);
   // Sets the value at the specified index

   bool AddMFInt32(const long liValue);
   // Appends the value to the end of the list

   //--- MFString Functions

   bool SetMFString(const long liIndex, const char* pcString);

protected:

   IFieldObject *Interface() const;
   // Returns the field object interface

   // Friend class for protected functions
   friend class CCortonaNode;

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

#endif // __VTSTRUCVIS_CORTONAFIELD__
