//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATH_EULERROTATION_
#define _VALET_MATH_EULERROTATION_

////////////////
//! file       = "VALET/math/eulerrotation.h"
//! author     = "James Smith"
//! date       = "02/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: eulerrotation.h,v 1.9 2001/10/27 13:06:08 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

namespace NVALET {
   //#===--- Pre-declared classes
   class CEulerRotation;
}
   
//#===--- Includes
#include "VALET/valet.h"
#include "VALET/eulertype.h"
#include "VALET/quaternion.h"
#include "VALET/homtransform.h"

namespace NVALET {
    
   //: Euler Rotation
   // A class representing an euler rotation.
   // An euler rotation represents a 3D rotation as a series of rotations
   // about individual axes.
   class CEulerRotation {
    
   protected:
    
      CVector3D m_oAngles;
      //: The three angles, stored in a 3D vector
    
      CEulerType m_oType;
      //: The type of this rotation 
   
   public:    

   //:------------------------
   //: Contruction/Destruction

      CEulerRotation();
      //: Default constructor
      // Creates a zero rotation of type EU_XYZS.
    
      explicit CEulerRotation(const CEulerType& oType);
      //: Default constructor
      // Creates a zero rotation of type oType.
      //!param: oType = the type of the new object
    
      CEulerRotation(const CVector3D& oAngles, const CEulerType& oType);
      //: Constructor from a vector
      //!param: oAngles = the inidividual angles that make up the rotation.
      //!param: oType = the type of the new object

      CEulerRotation(double dAngle0, double dAngle1, double dAngle2, const CEulerType& oType);
      //: Constructor from a set of doubles
      //!param: dAngle0 = the first angle in the rotation.
      //!param: dAngle1 = the second angle in the rotation.
      //!param: dAngle2 = the third angle in the rotation.
      //!param: oType = the type of the new object
    
      CEulerRotation(const CQuaternion& oQuat, const CEulerType& oType);
      //: Constructor from a Quaternion
      //!param: oQuat = the quaternion to convert
      //!param: oType = the type of the new object
    
      CEulerRotation(const CHomTransform & oTransform, const CEulerType& oType);
      //: Constructor from a Homogeneous Transform
      // Only rotation information from the transform will be preserved.
      //!param: oTransform = the transform to convert
      //!param: oType = the type of the new object
    
      ~CEulerRotation();
      //: Destructor
    
   //:---------------------
   //: Arithmetic functions
    
      CEulerRotation& operator *= (const CEulerRotation& oScale);
      //: Scaling
      // Multiplies each angle by the appropriate component of the passed vector.
      // This can be used to damp rotation in certain directions.
      // The passed rotation must have the same type as this one.
      //!param: oScale = the values to multiply by
      //!param: return = a reference to this rotation
    
      bool Limit(const CEulerRotation& oMax, const CEulerRotation& oMin);
      //: Limiting function
      // The angles are limited to the maximum and minimum values supplied
      // Returns true if the angle was clipped to the limits.
      // The passed rotations must have the same type as this one.
      //!param: oMax = the maximum rotation values..
      //!param: oMin = the minimum rotation values.
      //!param: return = true if the angle was modified.

   //:-----------------
   //: Access functions
    
      CVector3D Angles() const;
      //: Access to the angles
      //!param: return = the angles
    
      CVector3D& Angles();
      //: Non-const access to the angles
      // Allows direct modfication of the angles.
      //!param: return = reference to the angles
    
      CEulerType Type(void) const;
      //: Access to the type of the angle.
      //!param: return = the type of the angle   
    
      CEulerType& Type(void);
      //: Non-const access to the type of the angle.
      // Allows direct modfication of the type.
      //!param: return = reference to the type of the angle
    
  };

}

#endif // _VALET_MATH_EULERROTATION_
