//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.Cortona Interface"
//! userlevel =  Normal
//! file      = "Control/CortonaField.h"
//! author    = "Warren Moore"
//! date      = "12/3/2002"
//! rcsid     = "$Id: CortonaField.h,v 1.21 2002/04/24 14:56:33 vap-warren Exp $"

#ifndef __VTSTRUCVIS_CORTONAFIELD__
#define __VTSTRUCVIS_CORTONAFIELD__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Automation interfaces for Cortona
#include "shelley.h"

#define TEST_LOOP 1000
#define TEST_SIZE 1000

//: Cortona field access
// Wrapper around Cortona field Automation interfaces

class CCortonaField {
//:-----
//: Construction/Destruction
public:

   CCortonaField(IEngine *pEngine, IFieldObject *pField);
   //: Constructor
   // Creates a field object from a field interface
   //!param: pEngine - IEngine interface to Cortona engine - must not be null
   //!param: pField - IFieldObject interface to Cortona field - must not be null

   ~CCortonaField();
   //: Destructor
   // Release must be called first for new (not copied) objects

//:-----
//: Field Functions

   void Release();
   //: Release the field object

   FTYPE Type() const;
   //: Get the field type
   // Returns the field type. FTYPE is declared in Shelley.h

   long GetMFCount();
   //: Get the number of MF field values
   //!return: Number of fields in the MF field, 0 if not a suitable type

   bool SetMFCount(const long liCount);
   //: Set the number of MF field values
   // Only suitable for MF field types
   //!return: true if successful

//:-----
//: MFVec3f Functions

   bool GetMFVec3f(const long liIndex, float &fX, float &fY, float &fZ);
   //: Get MFVec3f values
   //!param: liIndex - Index of field value
   //!param: fX - Reference to X value returned
   //!param: fY - Reference to Y value returned
   //!param: fZ - Reference to Z value returned
   //!return: true if successful

   bool TestMFVec3f0();
   //: Test function for profiling

   bool TestMFVec3f1();
   //: Test function for profiling

   bool SetMFVec3f(const long liIndex, const float fX, const float fY, const float fZ);
   //: Set MFVec3f values
   //!todo: This function is an application bottleneck
   //!param: liIndex - Index of field value
   //!param: fX - X value
   //!param: fY - Y value
   //!param: fZ - Z value
   //!return: true if successful

   bool SetMFVec3f(const float *pfVal, const unsigned int uLen);
   //: Set all MFVec3f values
   // Speed up SetMFVec3f by passing an entire array to the function at once
   //!param: pfVal - Pointer to array of X,Y,Z triplets
   //!param: uLen - Numer of colours in array
   //!return: true if successful

   bool AddMFVec3f(const float fX, const float fY, const float fZ);
   //: Add a new value to the end of the field
   //!param: fX - X value
   //!param: fY - Y value
   //!param: fZ - Z value
   //!return: true if successful

//:-----
//: SFVec3f Functions

   bool GetSFVec3f(float &fX, float &fY, float &fZ);
   //: Get SFVec3f values
   //!param: fX - Reference to X value returned
   //!param: fY - Reference to Y value returned
   //!param: fZ - Reference to Z value returned
   //!return: true if successful

   bool SetSFVec3f(const float fX, const float fY, const float fZ);
   //: Set SFVec3f values
   //!param: fX - X value
   //!param: fY - Y value
   //!param: fZ - Z value
   //!return: true if successful

//:-----
//: MFColor Functions

   bool GetMFColor(const long liIndex, float &fR, float &fG, float &fB);
   //: Get MFVec3f values
   //!param: liIndex - Index of field value
   //!param: fR - Reference to red value returned
   //!param: fG - Reference to green value returned
   //!param: fB - Reference to blue value returned
   //!return: true if successful

   bool SetMFColor(const long liIndex, const float fR, const float fG, const float fB);
   //: Set MFColor values
   //!todo: This function is an application bottleneck
   //!param: liIndex - Index of field value
   //!param: fR - Red value
   //!param: fG - Blue value
   //!param: fB - Green value
   //!return: true if successful

   bool SetMFColor(const float *pfVal, const unsigned int uLen);
   //: Set all MFColor values
   // Speed up SetMFColor by passing an entire array to the function at once
   //!param: pfVal - Pointer to array of color triplets
   //!param: uLen - Numer of colours in array
   //!return: true if successful

   bool AddMFColor(const float fR, const float fG, const float fB);
   //: Add a new value to the end of the field
   //!bug: For some unknown reason, it won't use the blue value - Use SetMFCount and SetMFColor instead
   //!param: fR - Red value
   //!param: fG - Green value
   //!param: fB - Blue value
   //!return: true if successful

//:-----
//: SFBool Functions

   bool GetSFBool(bool &bVal);
   //: Get the SFBool value
   //!param: bVal - Reference to bool value returned
   //!return: true if successful

   bool SetSFBool(const bool bVal);
   //: Set the SFBool value
   //!param: bVal - Bool value
   //!return: true if successful

//:-----
//: SFRotation Functions

   bool GetSFRotation(float &fX, float &fY, float &fZ, float &fAngle);
   //: Get the SFRotation
   //!param: fX - Reference to X axis value returned
   //!param: fY - Reference to Y axis value returned
   //!param: fZ - Reference to Z axis value returned
   //!param: fAngle - Reference to rotation angle value returned
   //!return: true if successful

   bool SetSFRotation(const float fX, const float fY, const float fZ, const float fAngle);
   //: Set the SFRotation
   //!param: fX - X axis value
   //!param: fY - Y axis value
   //!param: fZ - Z axis value
   //!param: fAngle - Rotation angle value
   //!return: true if successful

//:-----
//: MFVec3f Functions

   bool GetMFInt32(const long liIndex, long &liValue);
   //: Get the MFInt32 value
   //!param: liIndex - Index of field value
   //!param: liValue - Reference to value returned
   //!return: true if successful

   bool SetMFInt32(const long liIndex, const long liValue);
   // Sets the value at the specified index
   //!param: liIndex - Index of field value
   //!param: liValue - Field value
   //!return: true if successful

   bool SetMFInt32(const long *plVal, const unsigned int uLen);
   //: Set all MFInt32 values
   // Speed up SetMFInt32 by passing an entire array to the function at once,
   //!param: pfVal - Pointer to array of color triplets
   //!param: uLen - Numer of colours in array
   //!return: true if successful

   bool AddMFInt32(const long liValue);
   //: Add a new value to the end of the field
   //!param: liValue - Field value
   //!return: true if successful

//:-----
//: MFString Functions

   bool SetMFString(const long liIndex, const char* pcString);
   //: Set the MFString value
   //!param: liIndex - Index of field value
   //!param: pcString - Pointer to string
   //!return: true if successful

protected:

   IFieldObject *Interface() const;
   //: Get the field object interface
   // Only to be called by friend classes

   friend class CCortonaNode;
   //: Friend class for protected functions

protected:

   IEngine *m_pEngine;           //: The engine interface
   IFieldObject *m_pField;       //: The field interface
   FTYPE m_eType;                //: The field type

   //: Specific field interfaces
   union TUInterface {
      IMFColorObject *m_pMFColor;
      IMFVec3fObject *m_pMFVec3f;
      ISFVec3fObject *m_pSFVec3f;
      IMFInt32Object *m_pMFInt32;
      ISFBoolObject *m_pSFBool;
      ISFRotationObject *m_pSFRotation;
      IMFStringObject *m_pMFString;
   } m_uInterface;               //: The specific field interface

   IMFieldObject *m_pMField;           //: The MF field interface
   IUpdateManager *m_pUpdateManager;   //: Update manager interface for MF fields
   IVariable *m_pVariable;             //: Variable interface for MF fields

   float m_pfTestVals[3 * TEST_SIZE];

};

//#===--- Inlines

inline FTYPE CCortonaField::Type() const {
   return m_eType;
}

inline IFieldObject *CCortonaField::Interface() const {
   return m_pField;
}

#endif // __VTSTRUCVIS_CORTONAFIELD__
