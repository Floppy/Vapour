//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_EULERROTATION_
#define _VALET_EULERROTATION_

////////////////
//! file 		= "VALET/math/eulerrotation.h"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: eulerrotation.h,v 1.4 2001/10/17 14:44:56 vap-warren Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

namespace NValet {
  class CEulerRotation;
}

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/quaternion.h"
#include "VALET/homtransform.h"

namespace NValet {
    
  //: Euler Rotation
  // A class representing an euler rotation.
  // An euler rotation represents a 3D rotation as a series of rotations
  // about individual axes.
  class CEulerRotation {
    
  public:
    
    //: Euler angle IDs
    // Definitions of Euler angle types
    // Allows types to be specified in code, either as encoding or ordering
    enum EulerTypeID {
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

    //: Euler Type structure
    // Describes the type of an euler angle.
    // This can be accessed as a complete EulerTypeID,
    // or the properties can be extracted inidividually.
    union UEulerType {
      EulerTypeID m_eID;
      struct {
	bool m_bRotatingFrame : 1;
	bool m_bRepetition : 1;
	bool m_bOddParity : 1;
	unsigned char m_ucInnerAxis : 2;
      } m_sProperties;
    };

  protected:
    
    CVector3D m_vecAngles;
    //: The three angles, stored in a 3D vector
    
    UEulerType m_uType;
    //: The type of this rotation 
   
  public:    

    //:------------------------
    //: Contruction/Destruction

    CEulerRotation();
    //: Default constructor
    // Creates a zero rotation of type XYZs.
    
    explicit CEulerRotation(const UEulerType& uType);
    //: Default constructor
    // Creates a zero rotation of type uType.
    //!param: uType = the type of the new object
    
    CEulerRotation(const CVector3D& vecAngles, const UEulerType& uType);
    //: Constructor from a vector
    //!param: vecAngles = the inidividual angles that make up the rotation.
    //!param: uType = the type of the new object
    
    CEulerRotation(const CQuaternion& oQuat, const UEulerType& uType);
    //: Constructor from a Quaternion
    //!param: oQuat = the quaternion to convert
    //!param: uType = the type of the new object
    
    CEulerRotation(const CHomTransform & oTransform, const UEulerType& uType);
    //: Constructor from a Homogeneous Transform
    // Only rotation information from the transform will be preserved.
    //!param: oTransform = the transform to convert
    //!param: uType = the type of the new object
    
    ~CEulerRotation();
    //: Destructor
    
    //:---------------------
    //: Arithmetic functions
    
    CEulerRotation& operator *= (const CVector3D& oVec);
    //: Scaling
    // Multiplies each angle by the appropriate component of the passed vector.
    // This can be used to damp rotation in certain directions.
    //!param: oVec = the vector to multiply by
    //!param: return = a reference to this rotation
    
    bool Limit(const CVector3D& vecMax, const CVector3D& vecMin);
    //: Limiting function
    // The angles are limited to the maximum and minimum values supplied
    // Returns true if the angle was clipped to the limits.
    //!param: vecMax = the maximum values for each component.
    //!param: vecMin = the minimum values for each component.
    //!param: return = true if the angle was modified.

    //:-----------------
    //: Access functions
    
    CVector3D Angles() const {return m_vecAngles;}
    //: Access to the angles
    //!param: return = the angles
    
    CVector3D& Angles(const int iAngleNumber) {return m_vecAngles;}
    //: Non-const access to the angles
    // Allows direct modfication of the angles.
    //!param: return = reference to the angles
    
    UEulerType Type(void) const {return m_uType;}
    //: Access to the type of the angle.
    //!param: return = the type of the angle   
    
    UEulerType& Type(void) {return m_uType;}
    //: Non-const access to the type of the angle.
    // Allows direct modfication of the type.
    //!param: return = reference to the type of the angle
    
  };

}

#endif // _VALET_EULERROTATION_
