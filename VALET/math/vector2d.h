//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATH_VECTOR2D_
#define _VALET_MATH_VECTOR2D_

////////////////
//! file       = "VALET/math/vector2d.h"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: vector2d.h,v 1.2 2001/10/23 16:01:53 vap-warren Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

namespace NValet {
   class CAxisRotation;
}

//#===--- Includes
#include "VALET/valet.h"

namespace NValet {

   //: 2D Vector
   // A class representing a 2D vector - 2 components, X and Y.

   class CVector2D {
	
   protected:
	
      double m_pComponents[2];
      //: Vector components
      // Stored in the order X Y

   public:
	
      //:-------------------------
      //: Construction/Destruction

      CVector2D();
      //: Default constructor.
      // Creates a zero vector.
	
      CVector2D(const CVector2D & oVec);
      //: Copy constructor.
      //!param: oVec = vector to be copied.
	
      CVector2D(const double dX, const double dY);
      //: 2-argument constructor.
      //!param: dX = X component.
      //!param: dY = Y component.
	
      virtual ~CVector2D();
      //: Destructor
	
      //:---------------------
      //: Arithmetic operators
	
      CVector2D operator *(const double dScalar) const;
      //: Multiplication by a scalar
      // A new vector is returned, whose components are equal to
      // the components of this vector multiplied by dScalar.	
      // This is equivalent to a uniform scaling.
      //!param: dScalar = scalar value to multiply by.
      //!param: return = a new vector equal to this * dScalar
	
      CVector2D operator *(const CVector2D & oVec) const;
      //: Multiplication by a vector
      // A new vector is returned, whose components are equal to
      // the components of this vector multiplied by the corresponding
      // component of oVec.
      // This is equivalent to a non-uniform scaling.
      //!param: oVec = vector to multiply by.
      //!param: return = a new vector equal to this * oVec
	
      CVector2D& operator *=(const double dScalar);
      //: Multiplication by a scalar / Assignment
      // All components of this vector are multiplied by dScalar.
      // This is equivalent to a uniform scaling.
      //!param: dScalar = scalar value to multiply by.
      //!param: return = a reference to the vector being operated upon.
	
      CVector2D& operator *=(const CVector2D & oVec);
      //: Multiplication by a vector / Assignment
      // All components of this vector are multiplied by the corresponding
      // component of oVec.
      // This is equivalent to a non-uniform scaling.
      //!param: oVec = vector to multiply by.
      //!param: return = a reference to the vector being operated upon.
	
      CVector2D operator /(const double dScalar) const;
      //: Division by a scalar
      // A new vector is returned, whose components are equal to
      // the components of this vector divided by dScalar.	
      // This is equivalent to an inverse uniform scaling.
      //!param: dScalar = scalar value to divide by.
      //!param: return = a new vector equal to this / dScalar
	
      CVector2D operator /(const CVector2D & oVec) const;
      //: Division by a vector
      // A new vector is returned, whose components are equal to
      // the components of this vector divided by the corresponding
      // component of oVec.
      // This is equivalent to an inverse non-uniform scaling.
      //!param: oVec = vector to divide by.
      //!param: return = a new vector equal to this / oVec
	
      CVector2D& operator /=(const double dScalar);
      //: Divide by a scalar / Assignment
      // All components of this vector are divided by dScalar.
      // This is equivalent to an inverse uniform scaling.
      //!param: dScalar = scalar value to divide by.
      //!param: return = a reference to the vector being operated upon.
	
      CVector2D& operator /=(const CVector2D & oVec);
      //: Division by a vector / Assignment
      // All components of this vector are divided by the corresponding
      // component of oVec.
      // This is equivalent to an inverse non-uniform scaling.
      //!param: oVec = vector to divide by.
      //!param: return = a reference to the vector being operated upon.
	
      CVector2D operator +(const CVector2D & oVec) const;
      //: Addition
      // A new vector is returned, whose components are equal to
      // the components of this vector added to the corresponding
      // component of oVec.
      //!param: oVec = vector to add.
      //!param: return = a new vector equal to this + oVec
		
      CVector2D& operator +=(const CVector2D & oVec);
      //: Addition / Assignment
      // All components of this vector are added to the corresponding
      // component of oVec.
      //!param: oVec = vector to add.
      //!param: return = a new vector equal to this + oVec
	
      CVector2D operator -(const CVector2D &oVec) const;
      //: Subtraction
      // A new vector is returned, whose components are equal to
      // the components of this vector subtracted from the corresponding
      // component of oVec.
      //!param: oVec = vector to add.
      //!param: return = a new vector equal to this + oVec
	
      CVector2D& operator -=(const CVector2D & oVec);
      //: Subtraction / Assignment
      // All components of this vector are added to the corresponding
      // component of oVec.
      //!param: oVec = vector to add.
      //!param: return = a new vector equal to this + oVec
		
      CVector2D operator -(void) const;
      //: Inverse operator
      // Creates a new vector which is the inverse of this one.
      // All components are inverted.
      // See also Invert(), which changes the vector to it's inverse.
      //!param: return = a new vector equal to -this.
	
      CVector2D& Invert();
      //: Invert this vector.
      // All components are inverted.
      //!param: return = reference to this vector
	
      //:---------------------
      //: Comparison operators
	
      bool operator ==(const CVector2D & oVec) const;
      //: Equality
      // Returns true if all components are identical.
      //!param: oVec = vector to be compared.
      //!param: return = boolean true or false.
	
      bool operator !=(const CVector2D & oVec) const;
      //: Inequality
      // Returns true if any component is different.
      //!param: oVec = vector to be compared.
      //!param: return = boolean true or false.
	
      //:--------------------
      //: Geometric functions
	
      double Dot(const CVector2D & oVec) const;
      //: Dot or Scalar product
      // The dot product is the inverse cos of the angle 
      // between the vectors, multiplied by their lengths.
      //!param: oVec = the other vector to be used for the dot product
      //!param: return = the dot product of the two vectors.

      double Length() const;
      //: Length of the vector.
      //!param: return = double value representing the length of the vector.	
	
      CVector2D& SetLength(double dLength);
      //: Set length
      // Rescales the vector to be a certain length.
      //!param: dLength = desired length
      //!param: return = reference to this vector
	
      CVector2D& Normalise();
      //: Normalise this vector.
      // i.e. make the length of the vector equal to 1.
      // Equivalent to calling SetLength(1)
      //!param: return = reference to this vector
	
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
	
      void ToDouble(double &dX, double &dY) const;
      //: Get all three components at once.
      //!param: dX = a double to be overwritten with the X component.
      //!param: dY = a double to be overwritten with the Y component.
	
      //:----------------
      //: Input functions
		
      void FromDouble(double dX, double dY);
      //: Set the value of the vector
      //!param: dX = the new X component.
      //!param: dY = the new Y component.
	
      bool ParseString(const char* strInput, int* used = NULL);
      //: Load values from a string
      //!todo: Document this properly - don't have time now, I can't remember
      //!todo: what the return and parameters mean. Oops!
      //!todo: Rewrite this to use CScalar::ParseString()
	
      //:-----------------
      //: Output functions
		
      char* ToString(int iPrecision) const;
      //: Print to a string
      // Writes the components in the order X Y to a string.
      //!param: iPrecision = maximum number of digits to print after the decimal point.
      //!param: return = a string with the vector in it. Memory is allocated with malloc(),
      //!param:          so you need to get rid of it using free() once you're done.
      //!todo: Change this so that it returns a C++ string or something, so that
      //!todo: we don't have to worry about freeing up memory that's been allocated.
	
   };
	
}

#endif // _VALET_MATH_VECTOR2D_
