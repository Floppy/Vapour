//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_JOINT_
#define _VALET_3D_JOINT_

////////////////
//! file 		= "VALET/3d/joint.h"
//! author 		= "James Smith"
//! date 		= "18/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: joint.h,v 1.1 2001/10/18 15:45:16 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Animation"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/vector3d.h"
#include "VALET/homtransform.h"
#include "VALET/axisrotation.h"

#include <vector>

namespace NValet {

   //: A Joint in a skeleton

   class CJoint {

   protected:

      CVector3D m_vecCentre;
      //: The joint centre
      
      CVector3D m_vecMinRotation;
      //: Minimum values for the joint angles.

      CVector3D m_vecMaxRotation;
      //: Maximum values for the joint angles.

      CVector3D m_vecDamping;
      //: Damping values for the joint angles.

      typedef vector<CJoint>::iterator JointPtr;
      //: A pointer/iterator to a joint.

      JointPtr m_pParent;
      //: Parent joint.

      vector<JointPtr> m_pChildren;
      //: Child joints.

      vector<int> m_pVertices;
      //: A list of affected vertices.

      vector<int> m_pFaces;
      //: A list of affected faces.
      
      CHomTransform m_htTransform;
      //: The transform below this point in the skeleton.
      // This is the complete transform as far as (and including) this joint.

      bool m_bDirtyAngle;
      //: Has the angle changed recently?

      bool m_bDirtyShape;
      //: Has the shape changed recently?

   public:

      //:-------------------------
      //: Construction/Destruction
      
      CJoint(CVector3D vecCentre, JointPtr pParent = NULL);
      //: Default constructor
      //!param: vecCentre = the joint's centre of rotation.
      //!param: pParent = a pointer/iterator to the joint's parent.

      ~CJoint();
      //: Destructor
      
   };

}

#endif //_VALET_3D_JOINT_
