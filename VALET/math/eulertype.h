//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATH_EULERTYPE_
#define _VALET_MATH_EULERTYPE_

////////////////
//! file       = "VALET/math/eulertype.h"
//! author     = "James Smith"
//! date       = "27/10/2001"
//! lib        = libVALETmath
//! rcsid      = "$Id: eulertype.h,v 1.1 2001/10/27 13:06:09 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.Math.Geometry"

//#===--- Includes
#include "VALET/valet.h"

namespace NVALET {
    
   //: Euler Rotataion Type
   // A class representing the type of an euler rotation.
   // An euler rotation represents a 3D rotation as a series of rotations
   // about individual axes.
   // The type determines in which order these rotations are applied, and about
   // which axes.
   class CEulerType {
    
   public:
    
      //: Euler angle IDs
      // Definitions of Euler angle types
      // Allows types to be specified in code, either as encoding or ordering
      // The encoding is as follows. There are four letters in the encoding.
      // The first is the inner axis of the type. The second states whether the 
      // type is odd(O) or even(E) parity. The third is for repetition - if the first
      // and last axes are different(D) or the same(S). The last is the frame, whether 
      // static(S) or rotating(R).
      // The ordered values are a list of three axes, X Y and Z, followed by a static or
      // rotating frame identifier.
      enum EEulerTypeID {
         // Type specified as encoding
         EU_XEDS = 0x00,
         EU_XEDR = 0x01,
         EU_XESS = 0x02,
         EU_XESR = 0x03,
         EU_XODS = 0x04,
         EU_XODR = 0x05,
         EU_XOSS = 0x06,
         EU_XOSR = 0x07,
         EU_YEDS = 0x08,
         EU_YEDR = 0x09,
         EU_YESS = 0x0A,
         EU_YESR = 0x0B,
         EU_YODS = 0x0C,
         EU_YODR = 0x0D,
         EU_YOSS = 0x0E,
         EU_YOSR = 0x0F,
         EU_ZEDS = 0x10,
         EU_ZEDR = 0x11,
         EU_ZESS = 0x12,
         EU_ZESR = 0x13,
         EU_ZODS = 0x14,
         EU_ZODR = 0x15,
         EU_ZOSS = 0x16,
         EU_ZOSR = 0x17,
         // Type specified as axis orderings
         EU_XYZS = EU_XEDS,
         EU_ZYXR = EU_XEDR,
         EU_XYXS = EU_XESS,
         EU_XYXR = EU_XESR,
         EU_XZYS = EU_XODS,
         EU_YZXR = EU_XODR,
         EU_XZXS = EU_XOSS,
         EU_XZXR = EU_XOSR,
         EU_YZXS = EU_YEDS,
         EU_XZYR = EU_YEDR,
         EU_YZYS = EU_YESS,
         EU_YZYR = EU_YESR,
         EU_YXZS = EU_YODS,
         EU_ZXYR = EU_YODR,
         EU_YXYS = EU_YOSS,
         EU_YXYR = EU_YOSR,
         EU_ZXYS = EU_ZEDS,
         EU_YXZR = EU_ZEDR,
         EU_ZXZS = EU_ZESS,
         EU_ZXZR = EU_ZESR,
         EU_ZYXS = EU_ZODS,
         EU_XYZR = EU_ZODR,
         EU_ZYZS = EU_ZOSS,
         EU_ZYZR = EU_ZOSR
      };
      
      //: Axis IDs for Euler Angles
      enum EEulerAxis {
         EU_X_AXIS = 0,
         EU_Y_AXIS = 1,
         EU_Z_AXIS = 2
      };

   protected:
    
      EEulerAxis m_eInnerAxis;
      //: The inner axis of the rotation

      bool m_bRotatingFrame;
      //: Rotating or Static frame?

      bool m_bRepetition;
      //: Axis repetition?

      bool m_bOddParity;
      //: Odd parity?


   public:    

   //:------------------------
   //: Contruction/Destruction

      CEulerType(EEulerTypeID eID = EU_XYZS);
      //: Constructor from type ID
    
      ~CEulerType();
      //: Destructor

   //:---------------------
   //: Operator functions

      bool operator==(const CEulerType& oType);
      //: Equality

      bool operator!=(const CEulerType& oType);
      //: Inequality

   //:---------------------
   //: Access functions

      EEulerAxis InnerAxis(void) const {return m_eInnerAxis;}
      //: The inner axis.

      bool RotatingFrame(void) const {return m_bRotatingFrame;}
      //: Does the type use a rotating frame?
      //!param: return = true => rotating frame
      //!param:          false => static frame

      bool Repetition(void) const {return m_bRepetition;}
      //: Does the type have repetition?

      bool OddParity(void) const {return m_bOddParity;}
      //: Does the type have odd parity?

  };

}

#endif // _VALET_MATH_EULERTYPE_
