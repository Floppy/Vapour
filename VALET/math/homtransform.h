//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_HOMTRANSFORM_
#define _VALET_HOMTRANSFORM_

////////////////
//! file 		= "VALET/math/homtransform.h"
//! author 		= "James Smith"
//! date 		= "02/10/2001"
//! lib 		= libVALETmath
//! rcsid 		= "$Id: homtransform.h,v 1.3 2001/10/09 18:32:30 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.Math.Geometry"

namespace NValet {
  class CHomTransform;
}

//#===--- Includes
#include "../arch/valet.h"
#include "vector3d.h"
#include "quaternion.h"
#include "axisrotation.h"

namespace NValet {
    
  //: Homogeneous Transformation Matrix
  class CHomTransform {

  protected:
    
    double m_dMatrix[12];    
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

    explicit CHomTransform(const CVector3D& vecTranslation);
    //: Translation constructor
    // Creates an matrix equivalent to a translation
    //!param: vecTranslation = the translation to be converted.
    
    explicit CHomTransform(const CQuaternion& oQuat);
    //: Constructor from a quaternion
    // Creates an matrix equivalent to a rotation (represented by a quaternion)
    //!param: oQuat = the quaternion to be converted.
    
    CHomTransform(const CQuaternion& oQuat, const CVector3D& vecCentre);
    //: Constructor from a quaternion about a centre
    // Creates an matrix equivalent to a rotation (represented by a quaternion)
    // about an arbitrary centre.
    //!param: oQuat = the quaternion to be converted.
    //!param: vecCentre = the centre of rotation.
    
    explicit CHomTransform(const CAxisRotation& rotRotation);
    //: Constructor from an axis rotation
    // Creates an matrix equivalent to a rotation (represented by an axis-angle rotation)
    //!param: oQuat = the axis rotation to be converted.
    
    CHomTransform(const CAxisRotation& rotRotation, const CVector3D& vecCentre);
    //: Constructor from an axis rotation about a centre
    // Creates an matrix equivalent to a rotation (represented by an axis-angle rotation)
    // about an arbitrary centre.
    //!param: oQuat = the axis rotation to be converted.
    //!param: vecCentre = the centre of rotation.
    
    
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

    double Item(const unsigned int iRow, const unsigned int iCol) const {return m_dMatrix[iRow*4 + iCol];}
    //: Single element access
    // Gets a single element from the transform.
    //!param: iRow = the row of the desired element.
    //!param: iCol = the column of the desired element.
    //!param: return = the requested element.

  private:

    void ChangeCentre(const CVector3D& vecCentre);
    //: Change the centre of rotation
    // This can only be used on pure rotation matrices.
    // It is used by the rotation about arbitrary centre constructors.
    //!param: vecCentre = centre of rotation.
    
  };

}

#endif // _VALET_HOMTRANSFORM_
