//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_SKELETON_
#define _VALET_3D_SKELETON_

////////////////
//! file 		= "VALET/3d/skeleton.h"
//! author 		= "James Smith"
//! date 		= "18/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: skeleton.h,v 1.1 2001/10/18 15:45:17 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Animation"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/joint.h"

#include <vector>

namespace NValet {

   //: A Skeleton
   // Basically a collection of joints.

   class CSkeleton {

   protected:

      vector<CJoint> m_lJoints;
      //: A list of joints.

   public:

      //:-------------------------
      //: Construction/Destruction
      
      CSkeleton();
      //: Default constructor

      ~CSkeleton();
      //: Destructor
      
   };

}

#endif //_VALET_3D_SKELETON_
