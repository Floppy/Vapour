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
//! rcsid 		= "$Id: mesh.h,v 1.4 2001/10/15 19:51:38 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "../arch/valet.h"
#include "material.h"

namespace NValet {

  //: Polygon Mesh
  // This class describes the topology of a mesh surface. Geometry info is provided by
  // whichever class encapsulates this one.
  // The topology is stored in a winged-edge representation for fast and easy manipulation.

  class CMesh {

  protected:

    // Predeclare CEdge for the next two classes to use.
    class CEdge;

    //: A face in a winged-edge mesh
    class CFace {
      CEdge* m_pEdge;
      //: The first incident edge on this face
    };
    
    //: A vertex in a winged-edge mesh
    class CVertex {
      int m_iIndex;
      //: Vertex index
      CEdge* m_pEdge;
      //: The first incident edge on this vertex
    };

    //: An edge in a winged-edge mesh
    class CEdge {
      CVertex* m_pStart;
      //: The starting vertex
      CVertex* m_pEnd;
      //: The ending vertex
      CFace*   m_pLeft;
      //: The left face
      CFace*   m_pRight;
      //: The right face
      CEdge*   m_pPrevLeft;
      //: The previous edge in a left traverse
      CEdge*   m_pNextLeft;
      //: The next edge in a left traverse
      CEdge*   m_pPrevRight;
      //: The previous edge in a right traverse
      CEdge*   m_pNextRight;
      //: The next edge in a right traverse
    };

    CMaterial m_oMaterial;
    //: Surface material

  public:
    
    //:------------------------
    //: Contruction/Destruction

    CMesh();
    //: Default Constructor
    // Creates an empty mesh.
    
    ~CMesh();
    //: Destructor

    //:-----------------
    //: Access functions
    
    const CMaterial& Material(void) const {return m_oMaterial;}
    //: Access the surface material
    //!param: return = the surface material of the mesh.
    
  }; 

}

#endif //_VALET_MESH_
