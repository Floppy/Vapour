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
//! rcsid 		= "$Id: skeleton.h,v 1.2 2001/10/18 20:57:32 vap-james Exp $"
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

      CVector3D m_vecTranslation;
      //: The translation of the root joint.

      vector<CJoint> m_lJoints;
      //: A list of joints.

   public:

      //:-------------------------
      //: Construction/Destruction
      
      CSkeleton();
      //: Default constructor

      ~CSkeleton();
      //: Destructor
      
      //:-----------------
      //: Access functions

      const CVector3D& Translation(void) const {return m_vecTranslation;}
      //: Access to the root translation.
      //!param: return = a reference to the translation.

      CVector3D& Translation(void) {return m_vecTranslation;}
      //: Non-const access to the root translation.
      // This can be used to translate the model.
      //!param: return = a reference to the translation.

   };

}

#endif //_VALET_3D_SKELETON_
