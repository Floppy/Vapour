//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// EulerRotation.h - 12/04/2000 - James Smith
//	Euler rotation class header
//
// $Id: EulerRotation.h,v 1.4 2000/11/21 16:38:31 waz Exp $
//

#ifndef _VAL_EULERROTATION_
#define _VAL_EULERROTATION_

#pragma once

// Class declarations
class CEulerRotation;

// libVapour includes
#include "Quaternion.h"
#include "HomTransform.h"

// defines
#define X_AXIS 0x0
#define Y_AXIS 0x1
#define Z_AXIS 0x2

// DLL import/export definitions
#ifndef DLL
	#ifdef VAL_DLL_EXPORT
		#define DLL __declspec(dllexport)
	#endif
	#ifdef VAL_DLL_IMPORT
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

// Definitions of Euler angle types
// Allows angles to be specified as encoding or ordering
enum DLL EulerType {
	// Type specified as encoding
	XEDS = 0x00,
	XEDR = 0x01,
	XESS = 0x02,
	XESR = 0x03,
	XODS = 0x04,
	XODR = 0x05,
	XOSS = 0x06,
	XOSR = 0x07,
	YEDS = 0x08,
	YEDR = 0x09,
	YESS = 0x0A,
	YESR = 0x0B,
	YODS = 0x0C,
	YODR = 0x0D,
	YOSS = 0x0E,
	YOSR = 0x0F,
	ZEDS = 0x10,
	ZEDR = 0x11,
	ZESS = 0x12,
	ZESR = 0x13,
	ZODS = 0x14,
	ZODR = 0x15,
	ZOSS = 0x16,
	ZOSR = 0x17,
	// Type specified as axis orderings
	XYZs = XEDS,
	ZYXr = XEDR,
	XYXs = XESS,
	XYXr = XESR,
	XZYs = XODS,
	YZXr = XODR,
	XZXs = XOSS,
	XZXr = XOSR,
	YZXs = YEDS,
	XZYr = YEDR,
	YZYs = YESS,
	YZYr = YESR,
	YXZs = YODS,
	ZXYr = YODR,
	YXYs = YOSS,
	YXYr = YOSR,
	ZXYs = ZEDS,
	YXZr = ZEDR,
	ZXZs = ZESS,
	ZXZr = ZESR,
	ZYXs = ZODS,
	XYZr = ZODR,
	ZYZs = ZOSS,
	ZYZr = ZOSR,
};

// Euler Type Structure. This can be used as an EulerType, 
// or the individual properties can eb extracted
union DLL UEulerType {
	EulerType m_eIdentifier;
	struct SEulerTypeProperties {
		bool m_bRotatingFrame : 1;
		bool m_bRepetition : 1;
		bool m_bOddParity : 1;
		unsigned char m_ucInnerAxis : 2;
	} m_sProperties;
};

// Euler Rotation class
class DLL CEulerRotation {

public:

	// Construction & Destruction functions ///////////////////////////////

   // Constructor - creates a zero rotation
   CEulerRotation();

   // Constructor - creates a Euler Rotation from a set of doubles
   CEulerRotation(double dFirst, double dSecond, double dThird, UEulerType uType);

   // Constructor - creates a Euler Rotation from a Homogeneous Transform
   CEulerRotation(const CHomTransform & htTransform, UEulerType uType);

   // Constructor - creates a Euler Rotation from a Quaternion
   CEulerRotation(const CQuaternion& oQuat, UEulerType uType);

   // Destructor
   virtual ~CEulerRotation();

	// Utility functions //////////////////////////////////////////////////

   // Damping function. Multiplies each angle by the appropriate factor.
   void Damp(double dDampFirst, double dDampSecond, double dDampThird);

   // Limiting function. The angles are clipped to the maximum and minimum values supplied
   // Returns true if the angle was clipped to the limits.
   bool Limit(double dMaxFirst, double dMaxSecond, double dMaxThird, double dMinFirst, double dMinSecond, double dMinThird);

	// Access functions ///////////////////////////////////////////////////

   // Returns the angle specified by <iAngleNumber>
   double Angle(int iAngleNumber) const {return m_pAngles[iAngleNumber];}

   // Returns the type of the euler rotation
   UEulerType Type(void) const {return m_uType;}

protected:

   // The three angles
   double m_pAngles[3];
	
   // The type of the euler rotation
   UEulerType m_uType;

};

#endif // _VAL_EULERROTATION_
