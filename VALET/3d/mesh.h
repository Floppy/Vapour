//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_MESH_
#define _VALET_MESH_

////////////////
//! file 		= "VALET/3d/mesh.h"
//! author 		= "James Smith"
//! date 		= "15/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: mesh.h,v 1.1 2001/10/15 16:16:50 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "../arch/valet.h"

namespace NValet {

  //: Polygon Mesh
  // This class describes the topology of a mesh surface. Geometry info is provided by
  // whichever class encapsulated this one.

  class CMesh {

  protected:

  public:    
    
    //:------------------------
    //: Contruction/Destruction

    CMesh();
    //: Default Constructor
    // Creates an empty mesh.
    
    ~CMesh();
    //: Destructor
    
  }; 

}

#endif //_VALET_MESH_
