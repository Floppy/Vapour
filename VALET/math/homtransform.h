//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MATH_HOMTRANSFORM_
#define _VALET_MATH_HOMTRANSFORM_

////////////////
//! file 		= "VALET/math/homtransform.h"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: homtransform.h,v 1.6 2001/10/24 21:33:26 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

namespace NVALET {
  class CHomTransform;
}

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/vector3d.h"
#include "VALET/quaternion.h"
#include "VALET/axisrotation.h"

namespace NVALET {
    
  //: Homogeneous Transformation Matrix
  class CHomTransform {

  protected:
    
    double m_pdMatrix[12];    
    // The matrix itself, stored row by row.
    
  public:
    
    //:------------------------
    //: Contruction/Destruction

    CHomTransform();
    //: Default constructor
    // Creates an identity matrix
    
    CHomTransform(const CHomTransform& oTransform);
    //: Copy constructor
    //!param: oTransform = the transform to be copied

    explicit CHomTransform(const CVector3D& oTranslation);
    //: Translation constructor
    // Creates an matrix equivalent to a translation
    //!param: oTranslation = the translation to be converted.
    
    explicit CHomTransform(const CQuaternion& oQuat);
    //: Constructor from a quaternion
    // Creates an matrix equivalent to a rotation (represented by a quaternion)
    //!param: oQuat = the quaternion to be converted.
    
    CHomTransform(const CQuaternion& oQuat, const CVector3D& oCentre);
    //: Constructor from a quaternion about a centre
    // Creates an matrix equivalent to a rotation (represented by a quaternion)
    // about an arbitrary centre.
    //!param: oQuat = the quaternion to be converted.
    //!param: oCentre = the centre of rotation.
    
    explicit CHomTransform(const CAxisRotation& oRotation);
    //: Constructor from an axis rotation
    // Creates an matrix equivalent to a rotation (represented by an axis-angle rotation)
    //!param: oRotation = the axis rotation to be converted.
    
    CHomTransform(const CAxisRotation& oRotation, const CVector3D& oCentre);
    //: Constructor from an axis rotation about a centre
    // Creates an matrix equivalent to a rotation (represented by an axis-angle rotation)
    // about an arbitrary centre.
    //!param: oRotation = the axis rotation to be converted.
    //!param: oCentre = the centre of rotation.    

    ~CHomTransform();
    //: Destructor
    
    //:----------
    //: Operators

    CHomTransform& operator=(const CHomTransform& oTransform);
    //: Assignment
    //!param: oTransform = the transform to assign to this one.
    //!param: return = a reference to this transform.
    
    CHomTransform operator*(const CHomTransform& oTransform) const;
    //: Multiplication
    // Multiplies two transforms together. This is the same as doing one after 
    // the other.
    //!param: oTransform = the transform to multiply this one by.
    //!param: return = a new transform equal to this * oTransform
    //!todo: Clarify ordering in documentation. 
    //!todo: Rewrite using loop.
    
    CVector3D operator*(const CVector3D& oVec) const;
    //: Vector transformation
    // Multiplies a vector by the transform, thus applying the transform to the vector
    //!param: oVec = the vector to transform.
    //!param: return = the transformed vector.
    //!todo: Rewrite using loop.
    
    //:-----------------
    //: Access functions

    double Element(const unsigned int iRow, const unsigned int iCol) const;
    //: Single element access
    // Gets a single element from the transform.
    //!param: iRow = the row of the desired element.
    //!param: iCol = the column of the desired element.
    //!param: return = the requested element.

  private:

    void ChangeCentre(const CVector3D& oCentre);
    //: Change the centre of rotation
    // This can only be used on pure rotation matrices.
    // It is used by the rotation about arbitrary centre constructors.
    //!param: oCentre = centre of rotation.
    
  };

}

#endif // _VALET_MATH_HOMTRANSFORM_
