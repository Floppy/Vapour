//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_AXISROTATION_
#define _VALET_AXISROTATION_

////////////////
//! file 		= "VALET/math/axisrotation.h"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: axisrotation.h,v 1.2 2001/10/02 22:15:57 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

namespace VALET {
	class CAxisRotation;
}

//#===--- Includes
#include "../arch/valet.h"
#include "vector3d.h"
#include "quaternion.h"
#include "eulerrotation.h"

namespace VALET {

	//: 3D Axis-Angle rotation
	// A class representing a rotation about an arbitrary axis.

	class CAxisRotation {

	protected:

		double m_dAngle;
	 	//: The angle to rotate (in radians)
	
		CVector3D m_oAxis;
		//: The axis about which to rotate

	public:

		//:------------------------
		//: Contruction/Destruction

		CAxisRotation();
		//: Default constructor
		// Creates a null (zero) rotation

		CAxisRotation(const CAxisRotation & oRot);
		//: Copy constructor
		//!param: oRot = rotation to be copied.

		CAxisRotation(const double dX, const double dY, const double dZ, const double dAngle);
		//: Constructor from individual double values
		//!param: dX = the X component of the axis.
		//!param: dY = the Y component of the axis.
		//!param: dZ = the Z component of the axis.
		//!param: dAngle = the angle to rotate.

		CAxisRotation(const CVector3D & oAxis, const double dAngle);
		//: Constructor from an axis and an angle
		//!param: oAxis = the axis to rotate around.
		//!param: dAngle = the angle to rotate by.

		explicit CAxisRotation(const CQuaternion & oQuat);
		//: Constructor from a quaternion
		//!param: oQuat = the quaternion to convert from.

		explicit CAxisRotation(const CEulerRotation & oRot);
		//: Constructor from an Euler rotation
		//!param: oRot = the euler rotation to convert from.

		//:----------
		//: Operators

		CAxisRotation& operator=(const CAxisRotation& oRot);
		//: Assignment
		//!param: oRot = the rotation to be assigned to this one.

 		bool operator ==(const CAxisRotation& oRot) const;
		//: Equality
		// Returns true if all components are identical.
		//!param: oRot = rotation to be compared.
		//!param: return = boolean true or false.

		//:--------------------
		//: Geometric functions

		CAxisRotation MergeInside(const CAxisRotation & oRot) const;
		//: Multiplies two rotations together
		//!todo: Remove this function and create a single multiply function.

		CAxisRotation MergeOutside(const CAxisRotation & oRot) const;
		//: Multiplies two rotations together - in the opposite order
		//!todo: Remove this function and create a single multiply function.

		CAxisRotation& Normalise(void);
		//: Normalise the rotation vector
		// i.e. make it's length 1.

		//:-----------------
		//: Access functions

 		CVector3D Axis(void) const {return m_oAxis;}
		//: Access to the rotation axis

 		CVector3D& Axis(void) {return m_oAxis;}
		//: Non-const access to the rotation axis
		// This can be used to modify the rotation axis

		double Angle(void) const {return m_dAngle;}
		//: Access to the rotation angle
		
		double& Angle(void) {return m_dAngle;}
		//: Non-const access to the rotation angle
		// This can be used to modify the rotation angle

		void ToDouble(double& dX, double& dY, double& dZ, double& dAngle) const;
		//: Get all four components at once.
		//!param: dX = a double to be overwritten with the X component of the axis.
		//!param: dY = a double to be overwritten with the Y component of the axis.
		//!param: dZ = a double to be overwritten with the Z component of the axis.
		//!param: dAngle = a double to be overwritten with the angle.

		//:----------------
		//: Input functions
	
 		void FromDouble(double dX, double dY, double dZ, double dAngle);
		//: Set the value of the rotation
		//!param: dX = the new X component.
		//!param: dY = the new Y component.
		//!param: dZ = the new Z component.
		//!param: dAngle = the new angle

		void SetAngle(double dAngle);
		//: Set the value of the angle
		//!param: dAngle = the new angle

		bool ParseString(const char* strInput, int* used = NULL);
		//: Load values from a string
		//!todo: Document this properly - don't have time now, and I can't remember
		//!todo: what the return and parameters mean. Oops!

		//:-----------------
		//: Output functions

		char* ToString(int precision) const;
		//: Print to a string
		// Writes the components in the order X Y Z R to a string.
		//!param: iPrecision = maximum number of digits to print after the decimal point.
		//!param: return = a string with the rotation in it. Memory is allocated with malloc(),
		//!param:          so you need to get rid of it using free() once you're done.
		//!todo: Change this so that it returns a C++ string or something, so that
		//!todo: we don't have to worry about freeing up memory that's been allocated.

	};
}


#endif // _VALET_AXISROTATION_
