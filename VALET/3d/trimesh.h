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
//! rcsid 		= "$Id: trimesh.h,v 1.3 2001/10/12 15:22:17 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "../arch/valet.h"
#include "surface.h"
#include "../math/vector3d.h"

namespace NValet {

  //: Triangle Mesh
  // A description of a 3d mesh, containing only triangular elements.
  // The class uses a winged-edge representation to allow easy  and fast 
  // manipulation of the mesh.

  class CTriMesh : public CSurface<CVector3D> {

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
