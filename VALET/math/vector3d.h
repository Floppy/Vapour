//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_VECTOR3D_
#define _VALET_VECTOR3D_

////////////////
//! file 		= "VALET/math/vector3d.h"
//! author 		= "James Smith"
//! date 		= "01/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: vector3d.h,v 1.2 2001/10/02 14:35:14 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

// Class declarations
//class CAxisRotation;

//#===--- Includes
#include "../arch/valet.h"

namespace VALET {

	//: 3D Vector
	// A class representing a 3D vector - 3 components, X Y and Z.
	class CVector3D {
	
	protected:
	
		//:-----
		//: Data

		double m_pComponents[3];
		//: Vector components
		// Stored in the order X Y Z

	public:
	
		//:------------------------
		//: Contruction/Destruction

		CVector3D();
		//: Default constructor.
		// Creates a zero vector.
	
		CVector3D(const CVector3D & oVec);
		//: Copy constructor.
		//!param: oVec = vector to be copied.
	
		CVector3D(const double dX, const double dY, const double dZ);
		//: 3-argument constructor.
		//!param: dX = X component.
		//!param: dY = Y component.
		//!param: dZ = Z component.
	
		virtual ~CVector3D();
		//: Destructor
	
		//:---------------------
		//: Arithmetic operators
	
		CVector3D operator *(const double dScalar) const;
		//: Multiplication by a scalar
		// A new vector is returned, whose components are equal to
	 	// the components of this vector multiplied by dScalar.	
		// This is equivalent to a uniform scaling.
		//!param: dScalar = scalar value to multiply by.
		//!param: return = a new vector equal to this * dScalar
	
		CVector3D operator *(const CVector3D & oVec) const;
		//: Multiplication by a vector
		// A new vector is returned, whose components are equal to
	 	// the components of this vector multiplied by the corresponding
		// component of oVec.
		// This is equivalent to a non-uniform scaling.
		//!param: oVec = vector to multiply by.
		//!param: return = a new vector equal to this * oVec
	
		CVector3D& operator *=(const double dScalar);
		//: Multiplication by a scalar / Assignment
		// All components of this vector are multiplied by dScalar.
		// This is equivalent to a uniform scaling.
		//!param: dScalar = scalar value to multiply by.
		//!param: return = a reference to the vector being operated upon.
	
		CVector3D& operator *=(const CVector3D & oVec);
		//: Multiplication by a vector / Assignment
		// All components of this vector are multiplied by the corresponding
		// component of oVec.
		// This is equivalent to a non-uniform scaling.
		//!param: oVec = vector to multiply by.
		//!param: return = a reference to the vector being operated upon.
	
		CVector3D operator /(const double dScalar) const;
		//: Division by a scalar
		// A new vector is returned, whose components are equal to
	 	// the components of this vector divided by dScalar.	
		// This is equivalent to an inverse uniform scaling.
		//!param: dScalar = scalar value to divide by.
		//!param: return = a new vector equal to this / dScalar
	
		CVector3D operator /(const CVector3D & oVec) const;
		//: Division by a vector
		// A new vector is returned, whose components are equal to
	 	// the components of this vector divided by the corresponding
		// component of oVec.
		// This is equivalent to an inverse non-uniform scaling.
		//!param: oVec = vector to divide by.
		//!param: return = a new vector equal to this / oVec
	
		CVector3D& operator /=(const double dScalar);
		//: Divide by a scalar / Assignment
		// All components of this vector are divided by dScalar.
		// This is equivalent to an inverse uniform scaling.
		//!param: dScalar = scalar value to divide by.
		//!param: return = a reference to the vector being operated upon.
	
		CVector3D& operator /=(const CVector3D & oVec);
		//: Division by a vector / Assignment
		// All components of this vector are divided by the corresponding
		// component of oVec.
		// This is equivalent to an inverse non-uniform scaling.
		//!param: oVec = vector to divide by.
		//!param: return = a reference to the vector being operated upon.
	
		CVector3D operator +(const CVector3D & oVec) const;
		//: Addition
		// A new vector is returned, whose components are equal to
	 	// the components of this vector added to the corresponding
		// component of oVec.
		//!param: oVec = vector to add.
		//!param: return = a new vector equal to this + oVec
		
		CVector3D& operator +=(const CVector3D & oVec);
		//: Addition / Assignment
		// All components of this vector are added to the corresponding
		// component of oVec.
		//!param: oVec = vector to add.
		//!param: return = a new vector equal to this + oVec
	
		CVector3D operator -(const CVector3D &oVec) const;
		//: Subtraction
		// A new vector is returned, whose components are equal to
	 	// the components of this vector subtracted from the corresponding
		// component of oVec.
		//!param: oVec = vector to add.
		//!param: return = a new vector equal to this + oVec
	
		CVector3D& operator -=(const CVector3D & oVec);
		//: Subtraction / Assignment
		// All components of this vector are added to the corresponding
		// component of oVec.
		//!param: oVec = vector to add.
		//!param: return = a new vector equal to this + oVec
		
		CVector3D operator -(void) const;
		//: Inverse operator
		// Creates a new vector which is the inverse of this one.
		// All components are inverted.
		// See also Invert(), which changes the vector to it's inverse.
		//!param: return = a new vector equal to -this.
	
		CVector3D& Invert();
		//: Invert this vector.
		// All components are inverted.
		//!param: return = reference to this vector
	
		//:---------------------
		//: Comparison operators
	
		bool operator ==(const CVector3D & oVec) const;
		//: Equality
		// Returns true if all components are identical.
		//!param: oVec = vector to be compared.
		//!param: return = boolean true or false.
	
		bool operator !=(const CVector3D & oVec) const;
		//: Inequality
		// Returns true if any component is different.
		//!param: oVec = vector to be compared.
		//!param: return = boolean true or false.
	
		//:--------------------
		//: Geometric functions
	
		double Dot(const CVector3D & oVec) const;
		//: Dot or Scalar product
		// The dot product is the inverse cos of the angle 
		// between the vectors, multiplied by their lengths.
	 	//!param: oVec = the other vector to be used for the dot product
	 	//!param: return = the dot product of the two vectors.
	
		CVector3D Cross(const CVector3D & oVec) const;
		//: Cross or Vector product
		// The cross product of two vectors is a vector orthogonal
		// to both vectors (i.e. a normal). The length of the cross product
		// represents the angle between the vectors.
	 	//!param: oVec = the other vector to be used for the cross product
	 	//!param: return = the cross product of the two vectors.	
		
		double Length() const;
		//: Length of the vector.
		//!param: return = double value representing the length of the vector.	
	
		CVector3D& SetLength(double dLength);
		//: Set length
		// Rescales the vector to be a certain length.
		//!param: dLength = desired length
		//!param: return = reference to this vector
	
		CVector3D& Normalise();
		//: Normalise this vector.
		// i.e. make the length of the vector equal to 1.
		// Equivalent to calling SetLength(1)
		//!param: return = reference to this vector
	
		//CVector3D Rotate(const CAxisRotation & oRot) const;
		//: Rotate
		// Returns a new vector which is this vector rotated about (0,0,0) by oRot.
		//!param: oRot = axis-angle rotation
		//!param: return = new vector after rotation
	
		//:-----------------
		//: Access functions
	
		double X(void) const;
		//: Access to the X component
		//!param: return = the X component
	
		double& X(void);
		//: Access to the X component
		// Allows direct modfication of the component.
		//!param: return = reference to the X component
	
		double Y(void) const;
		//: Access to the Y component
		//!param: return = the Y component
	
		double& Y(void);
		//: Access to the Y component
		// Allows direct modfication of the component.
		//!param: return = reference to the Y component
	
		double Z(void) const;
		//: Access to the Z component
		//!param: return = the Z component
	
		double& Z(void);
		//: Access to the Z component
		// Allows direct modfication of the component.
		//!param: return = reference to the Z component
	
		void ToDouble(double &dX, double &dY, double &dZ) const;
		//: Get all three components at once.
		//!param: dX = a double to be overwritten with the X component.
		//!param: dY = a double to be overwritten with the Y component.
		//!param: dZ = a double to be overwritten with the Z component.
	
		//:----------------
		//: Input functions
		
		void FromDouble(double dX, double dY, double dZ);
		//: Set the value of the vector
		//!param: dX = the new X component.
		//!param: dY = the new Y component.
		//!param: dZ = the new Z component.
	
		bool ParseString(const char* strInput, int* used = NULL);
		//: Load values from a string
		//!todo: Document this properly - don't have time now, it's late and I can't remember
		//!todo: what the return and parameters mean. Oops!
	
		//:-----------------
		//: Output functions
		
		char* ToString(int iPrecision) const;
		//: Print to a string
		// Writes the components in the order X Y Z to a string.
		//!param: iPrecision = maximum number of digits to print after the decimal point.
		//!param: return = a string with the vector in it. Memory is allocated with malloc(),
		//!param:          so you need to get rid of it using free() once you're done.
		//!todo: Change this so that it returns a C++ string or something, so that
		//!todo: we don't have to worry about freeing up memory that's been allocated.
	
	};
	
}

#endif // _VALET_VECTOR3D_
