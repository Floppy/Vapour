//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATH_SCALAR_
#define _VALET_MATH_SCALAR_

////////////////
//! file 		= "VALET/math/scalar.h"
//! author 		= "James Smith"
//! date 		= "01/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: scalar.h,v 1.5 2001/10/24 19:57:53 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

//#===--- Includes
#include "VALET/valet.h"

namespace NValet {

  //: Scalar value
  // A class representing a double-precision number.
  // This class is mainly designed to provide controllable string parsing and printing for doubles.  
  class CScalar {

  protected:

    double m_dScalar;
    //: The value of the scalar

  public:

    //:-------------------------
    //: Construction/Destruction

    CScalar();
    //: Default constructor.
    // Creates a scalar with a value of 0.

    CScalar(const CScalar& oScl);
    //: Copy constructor.
    //!param: oScl = scalar to be copied.

    CScalar(const double dScl);
    //: Constructor from double.
    //!param: dScl = the value of the new object.

    ~CScalar();
    //: Destructor

    //:----------
    //: Operators

    CScalar& operator=(const double dScl);
    //: Assignment from a double.
    //!param: dScl = the new value of the object.

    //:----------------
    //: Input functions

    bool ParseString(const char* pcInput, int* piUsed = NULL);
    //: Load values from a string
    //!todo: Document this properly - don't have time now, I can't remember
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
    
    double ToDouble(void) const;
    //: Convert to double.
    //!param: return = the value of the scalar.
    
  };
  
}

#endif // _VALET_MATH_SCALAR_
