//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_QUATERNION_
#define _VALET_QUATERNION_

////////////////
//! file 		= "VALET/math/quaternion.h"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: quaternion.h,v 1.3 2001/10/02 22:13:15 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

namespace VALET {
	class CQuaternion;
}

//#===--- Includes
#include "../arch/valet.h"
#include "vector3d.h"
#include "axisrotation.h"
#include "eulerrotation.h"

namespace VALET {

	//: Quaternion
	// A class representing a quaternion, a representation of a rotation.
	// The rotation is represented by a point on the surface of a 4D sphere.

	class CQuaternion {
	
	protected:

		double m_dScalar;
		//: The scalar part

		CVector3D m_oVector;
		//: The vector part

	public:
		//:------------------------
		//: Contruction/Destruction

		CQuaternion();
		//: Default constructor

		CQuaternion(const CQuaternion &oQuat);
		//: Copy constructor
		//!param: oQuat = the quaternion to be copied.

		CQuaternion(const double dScalar, const CVector3D & oVector);
		//: Constructor from vector/scalar
		//!param: dScalar = the scalar part.
		//!param: dVector = the vector part.

		explicit CQuaternion(const CAxisRotation &oRot);
		//: Constructor from axis-angle rotation

		explicit CQuaternion(const CEulerRotation &oRot);
		//: Constructor from euler rotation

		~CQuaternion();
		//: Destructor

		//:---------------------
		//: Arithmetic operators

		CQuaternion& operator=(const CQuaternion& oQuat);
		//: Assignment operator
		//!param: oQuat = the quaternion to be assigned to this one.

		CQuaternion operator*(const CQuaternion & oQuat) const;		
		//: Quaternion multiplication
		//!param: oQuat = the quaternion to multiply this one by.
		//!param: return = a new quaternion equal to this * oQuat.

		CQuaternion operator/(const double dScalar) const;
		//: Division
		// This operator divides all components of the quaternion by the argument.
		//!param: dScalar = the value to divide by.
		//!param: return = a new quaternion equal to this / dScalar.

		//:--------------------
		//: Geometric functions

		CQuaternion Conjugate() const;
		//: Conjugation
		// Return the conjugate of this quaternion.
		//!param: return = the conjugate.
		//!todo: Explain what this means better in the documentation.

		double Norm() const;
		//: Norm
		// Return the norm of this quaternion.
		//!param: return = the norm.
		//!todo: Explain what this means better in the documentation.

		CQuaternion Inverse() const;
		//: Inversion
		// Calculate the inverse of this quaternion
		//!param: return = a new quaternion equal to the inverse of this one.

		//:--------------
		//: Interpolation

		CQuaternion SlerpTo(const CQuaternion& oQuat, double dAmount) const;
		//: <B>S</B>pherical <B>L</B>inear int<B>ERP</B>olation
		// Calculate a new quaternion that is an interpolation of this one and oQuat.		
		//!param: oQuat = the target quaternion.
		//!param: dAmount = value between 0..1 inclusive. 
		//!param:           this.SlerpTo(oQuat,0) == this; 
                //!param:           this.SlerpTo(oQuat,1) == oQuat;

		//:-----------------
		//: Access functions

		double Scalar(void) const;
		//: Access to scalar part
		
		double& Scalar(void);
		//: Non-const access to scalar part
		// This can be used to modify the scalar part.

		CVector3D Vector(void) const;
		//: Access to vector part

		CVector3D& Vector(void);
		//: Non-const access to vector part
		// This can be used to modify the vector part.
	};
}

#endif // _VALET_QUATERNION_
