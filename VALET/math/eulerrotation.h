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
//! rcsid      = "$Id: eulerrotation.h,v 1.8 2001/10/27 09:40:36 vap-warren Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

namespace NVALET {
   //#===--- Pre-declared classes
   class CEulerRotation;
}
   
//#===--- Includes
#include "VALET/valet.h"
#include "VALET/quaternion.h"
#include "VALET/homtransform.h"

namespace NVALET {
    
   //: Euler Rotation
   // A class representing an euler rotation.
   // An euler rotation represents a 3D rotation as a series of rotations
   // about individual axes.
   class CEulerRotation {
    
   public:
    
      //: Euler angle IDs
      // Definitions of Euler angle types
      // Allows types to be specified in code, either as encoding or ordering
      enum EEulerTypeID {
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
         XYZS = XEDS,
         ZYXR = XEDR,
         XYXS = XESS,
         XYXR = XESR,
         XZYS = XODS,
         YZXR = XODR,
         XZXS = XOSS,
         XZXR = XOSR,
         YZXS = YEDS,
         XZYR = YEDR,
         YZYS = YESS,
         YZYR = YESR,
         YXZS = YODS,
         ZXYR = YODR,
         YXYS = YOSS,
         YXYR = YOSR,
         ZXYS = ZEDS,
         YXZR = ZEDR,
         ZXZS = ZESS,
         ZXZR = ZESR,
         ZYXS = ZODS,
         XYZR = ZODR,
         ZYZS = ZOSS,
         ZYZR = ZOSR
      };

      //: Euler Type structure
      // Describes the type of an euler angle.
      // This can be accessed as a complete EulerTypeID,
      // or the properties can be extracted inidividually.
      union TEulerType {
         EEulerTypeID m_eID;
         struct TProperties  {
            bool m_bRotatingFrame : 1;
            bool m_bRepetition : 1;
            bool m_bOddParity : 1;
            unsigned char m_ucInnerAxis : 2;
         } m_tProperties;
      };

   protected:
    
      CVector3D m_oAngles;
      //: The three angles, stored in a 3D vector
    
      TEulerType m_tType;
      //: The type of this rotation 
   
   public:    

   //:------------------------
   //: Contruction/Destruction

      CEulerRotation();
      //: Default constructor
      // Creates a zero rotation of type XYZs.
    
      explicit CEulerRotation(const TEulerType& tType);
      //: Default constructor
      // Creates a zero rotation of type tType.
      //!param: tType = the type of the new object
    
      CEulerRotation(const CVector3D& oAngles, const TEulerType& tType);
      //: Constructor from a vector
      //!param: oAngles = the inidividual angles that make up the rotation.
      //!param: tType = the type of the new object

      CEulerRotation(double dAngle0, double dAngle1, double dAngle2, const TEulerType& tType);
      //: Constructor from a set of doubles
      //!param: dAngle0 = the first angle in the rotation.
      //!param: dAngle1 = the second angle in the rotation.
      //!param: dAngle2 = the third angle in the rotation.
      //!param: tType = the type of the new object
    
      CEulerRotation(const CQuaternion& oQuat, const TEulerType& tType);
      //: Constructor from a Quaternion
      //!param: oQuat = the quaternion to convert
      //!param: tType = the type of the new object
    
      CEulerRotation(const CHomTransform & oTransform, const TEulerType& tType);
      //: Constructor from a Homogeneous Transform
      // Only rotation information from the transform will be preserved.
      //!param: oTransform = the transform to convert
      //!param: tType = the type of the new object
    
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
    
      TEulerType Type(void) const;
      //: Access to the type of the angle.
      //!param: return = the type of the angle   
    
      TEulerType& Type(void);
      //: Non-const access to the type of the angle.
      // Allows direct modfication of the type.
      //!param: return = reference to the type of the angle
    
  };

}

#endif // _VALET_MATH_EULERROTATION_
