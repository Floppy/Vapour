//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_JOINT_
#define _VALET_3D_JOINT_

////////////////
//! file       = "VALET/3d/joint.h"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: joint.h,v 1.5 2001/10/24 23:03:48 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Animation"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/vector3d.h"
#include "VALET/eulerrotation.h"
#include "VALET/homtransform.h"
#include "VALET/axisrotation.h"

#include <vector>
using namespace std;

namespace NVALET {

   //: A Joint in a skeleton
   
   class CJoint {

   protected:

      CVector3D m_oCentre;
      //: The joint centre
      
      CEulerRotation m_oMinRotation;
      //: Minimum values for the joint angles.

      CEulerRotation m_oMaxRotation;
      //: Maximum values for the joint angles.

      CEulerRotation m_oDamping;
      //: Damping values for the joint angles.

      typedef vector<CJoint>::iterator JointPtr;
      //: A pointer/iterator to a joint.

      JointPtr m_pParent;
      //: Parent joint.

      vector<JointPtr> m_oChildren;
      //: Child joints.

      vector<int> m_oVertices;
      //: A list of affected vertices.

      vector<int> m_oFaces;
      //: A list of affected faces.

      CAxisRotation m_oRotation;
      //: The current joint rotation.

      CHomTransform m_oTransform;
      //: The transform below this point in the skeleton.
      // This is the complete transform as far as (and including) this joint.

      bool m_bDirtyAngle;
      //: Has the angle changed recently?

      bool m_bDirtyShape;
      //: Has the shape changed recently?

   public:

      //:-------------------------
      //: Construction/Destruction

      CJoint(CVector3D oCentre, JointPtr pParent = JointPtr());
      //: Default constructor
      //!param: oCentre = the joint's centre of rotation.
      //!param: pParent = a pointer/iterator to the joint's parent.

      ~CJoint();
      //: Destructor

      //:-----------------
      //: Access functions      

      const CVector3D& Centre(void) const;
      //: Access the joint centre.
      //!param: return = the joint's centre of rotation.

      const CEulerRotation& MinRotation(void) const;
      //: Access the minimum rotation values.
      //!param: return = the minimum rotation values of the joint

      CEulerRotation& MinRotation(void);
      //: Non-const access to the minimum rotation values.
      // This can be used to change the values.
      //!param: return = the minimum rotation values of the joint

      const CEulerRotation& MaxRotation(void) const;
      //: Access the maximum rotation values.
      //!param: return = the maximum rotation values of the joint

      CEulerRotation& MaxRotation(void);
      //: Non-const access to the maximum rotation values.
      // This can be used to change the values.
      //!param: return = the maximum rotation values of the joint

      const CEulerRotation& Damping(void) const;
      //: Access the rotation damping values.
      //!param: return = the scale factor on the rotations.

      CEulerRotation& Damping(void);
      //: Non-const access to the rotation damping values.
      // This can be used to change the values.
      //!param: return = the scale factor on the rotations.
      
      const JointPtr Parent(void) const;
      //: Get the parent
      //!param: return = a pointer/iterator to the parent joint.

      int NumChildren(void) const;
      //: How many children does this joint have?
      //!param: return = the number of children this joint has.

      const JointPtr Child(int iChild) const;
      //: Access an individual child joint.
      //!param: iChild = the index of the desired child joint - should be < NumChildren()
      //!param: return = a pointer/iterator to the requested child.

      //:--------------------
      //: Animation functions

      const CAxisRotation& Rotate(const CAxisRotation& oRot, bool bLimit = true, bool bDamp = true);
      //: Rotate the joint relative to the current value.
      // If joint limiting is disabled, damping is automatically disabled.
      //!param: oRot = the new rotation - this is added to the current rotation.      
      //!param: bClip = enable joint limits.
      //!param: bDamp = enable rotation damping.
      //!param: return = the new value of the rotation, after limiting and damping have been applied.
      //!bug: There may be a rotation multiplication in the wrong order in here.
      //!todo: Improve this function - it's not very good.

      const CAxisRotation& RotateAbs(const CAxisRotation& oRot, bool bLimit = true, bool bDamp = true);
      //: Set the absolute rotation of the joint.
      // If joint limiting is disabled, damping is automatically disabled.
      //!param: oRot = the new rotation - the old rotation is lost.
      //!param: bClip = enable joint limits.
      //!param: bDamp = enable rotation damping.
      //!param: return = the new value of the rotation, after limiting and damping have been applied.
      //!todo: Improve this function - it's not very good.
            
   };

}

#endif //_VALET_3D_JOINT_
