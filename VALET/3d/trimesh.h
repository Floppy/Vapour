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
//! rcsid 		= "$Id: trimesh.h,v 1.4 2001/10/15 16:35:39 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "../arch/valet.h"
#include "surface.h"
#include "../math/vector3d.h"
#include "mesh.h"

#include <vector>

namespace NValet {

  //: Triangle Mesh
  // A description of a 3d mesh, containing only triangular elements.
  // The class uses a winged-edge representation to allow easy  and fast 
  // manipulation of the mesh.

  class CTriMesh : public CSurface<CVector3D> {

  protected:

    vector<CMesh> m_lMeshes;

  public:    
    
    //:------------------------
    //: Contruction/Destruction

    CTriMesh();
    //: Default Constructor
    // Creates an empty model.
    
    ~CTriMesh();
    //: Destructor
    
    //:-----------------
    //: Access functions

    int NumMeshes(void) const {return m_lMeshes.size();}
    //: Number of meshes
    //!param: return = how many meshes are in this model?

    const CMesh& Mesh(int iMesh) const {return m_lMeshes[iMesh];}
    //: Access a particular mesh
    //!param: iMesh = which mesh to access?
    //!param: return = constant reference to a mesh.

    CMesh& Mesh(int iMesh) {return m_lMeshes[iMesh];}
    //: Non-const access to a particular mesh
    //!param: iMesh = which mesh to access?
    //!param: return = reference to the mesh.

  }; 

}

#endif //_VALET_TRIMESH_
