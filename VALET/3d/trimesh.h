//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_TRIMESH_
#define _VALET_TRIMESH_

////////////////
//! file 		= "VALET/3d/trimesh.h"
//! author 		= "James Smith"
//! date 		= "10/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: trimesh.h,v 1.1 2001/10/10 12:39:23 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Model Representations"

//#===--- Includes
#include "../arch/valet.h"

namespace NValet {

  //: Triangle Mesh
  // A description of a 3d mesh, containing only triangular elements.
  // The class uses a winged-edge representation to allow easy  and fast 
  // manipulation of the mesh.

  class CTriMesh {

  protected:
    

  public:    
    
    //:------------------------
    //: Contruction/Destruction

    CTriMesh();
    //: Default Constructor
    // Creates an empty model.
    
    ~CTriMesh();
    //: Destructor
    
  }; 

}

#endif //_VALET_TRIMESH_
