//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_TRIMODEL_
#define _VALET_3D_TRIMODEL_

////////////////
//! file       = "VALET/3d/trimodel.h"
//! author     = "James Smith"
//! date       = "10/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: trimodel.h,v 1.6 2001/10/24 22:01:43 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/surface.h"
#include "VALET/vector3d.h"
#include "VALET/mesh.h"

#include <vector>
using namespace std;

namespace NVALET {

  //: A triangle-mesh-based 3d model.
  // A description of a 3d mesh, containing only triangular elements.

  class CTriModel : public CSurface<CVector3D> {

  protected:

    vector<CMesh> m_lMeshes;

  public:    
    
    //:-------------------------
    //: Construction/Destruction

    CTriModel();
    //: Default Constructor
    // Creates an empty model.
    
    ~CTriModel();
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

#endif //_VALET_3D_TRIMODEL_
