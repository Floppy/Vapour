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
//! rcsid      = "$Id: trimodel.h,v 1.8 2001/11/04 15:48:41 vap-warren Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/surface.h"
#include "VALET/vector3d.h"
#include "VALET/mesh.h"

#include <vector>

namespace NVALET {

   using namespace std;
   
   //: A triangle-mesh-based 3d model.
   // A description of a 3d mesh, containing only triangular elements.

   class CTriModel : public CSurface<CVector3D> {

   protected:

      vector<CMesh> m_oMeshes;

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

      int NumMeshes(void) const;
      //: Number of meshes
      //!param: return = how many meshes are in this model?

      const CMesh& Mesh(int iMesh) const;
      //: Access a particular mesh
      //!param: iMesh = which mesh to access?
      //!param: return = constant reference to a mesh.

      CMesh& Mesh(int iMesh);
      //: Non-const access to a particular mesh
      //!param: iMesh = which mesh to access?
      //!param: return = reference to the mesh.

   }; 

}

#endif //_VALET_3D_TRIMODEL_
