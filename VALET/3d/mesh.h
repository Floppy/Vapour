//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_MESH_
#define _VALET_3D_MESH_

////////////////
//! file 		= "VALET/3d/mesh.h"
//! author 		= "James Smith"
//! date 		= "15/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: mesh.h,v 1.11 2001/10/23 22:42:14 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/material.h"
#include "VALET/vector3d.h"

#include <vector>
#include <map>
using namespace std;

namespace NValet {

   //: Polygon Mesh
   // This class describes the topology of a mesh surface. Geometry info is provided by
   // whichever class encapsulates this one.
   // The topology is stored in a winged-edge representation for fast and easy manipulation.
   
   class CMesh {
      
   protected:
      
      // Predeclare private classes
      class CWEEdge;
      class CWEFace;
      class CWEVertex;
      
      // Iterator typedefs
      typedef vector<CWEEdge>::iterator        EdgeIter;
      typedef vector<CWEFace>::iterator        FaceIter;
      typedef vector<CVector3D>::iterator      VecIter;
      typedef map<VecIter,CWEVertex>::iterator VertIter;
      
      //: A face in a winged-edge mesh
      class CWEFace {
         EdgeIter m_pEdge;
         //: The first incident edge on this face
      };
      
      //: A vertex in a winged-edge mesh
      class CWEVertex {
         EdgeIter m_pEdge;
         //: The first incident edge on this vertex
      };
      
      //: An edge in a winged-edge mesh
      class CWEEdge {
         VertIter m_pStart;
         //: The starting vertex
         VertIter m_pEnd;
         //: The ending vertex
         FaceIter m_pLeft;
         //: The left face
         FaceIter m_pRight;
         //: The right face
         EdgeIter m_pPrevLeft;
         //: The previous edge in a left traverse
         EdgeIter m_pNextLeft;
         //: The next edge in a left traverse
         EdgeIter m_pPrevRight;
         //: The previous edge in a right traverse
         EdgeIter m_pNextRight;
         //: The next edge in a right traverse
      };
      
      vector<CWEEdge> m_oEdgeList;
      //: The edge list.
      
      vector<CWEFace> m_oFaceList;
      //: The face list.
      
      map<VecIter,CWEVertex> m_oVertexList;
      //: The vertex list.
      // This is a map - the first element (the key) is an iterator into a vector of CVector3ds.
      // The second is a CWEVertex data structure.
      
      CMaterial m_oMaterial;
      //: Surface material
      
   public:
      
      //:-------------------------
      //: Construction/Destruction
      
      CMesh();
      //: Default Constructor
      // Creates an empty mesh.
      
      ~CMesh();
      //: Destructor
      
      //:-----------------------
      //: Manipulation functions

      bool AddTriFace(VecIter pV1, VecIter pV2, VecIter pV3);
      //: Adds a triangular face.
      

      //:----------------
      //: Query functions

      vector<VecIter> AdjVertices(VecIter pVertex);
      //: Returns the vertices adjoining this vertex.
      //!param: pVertex = The vertex to query
      //!param: return = a vector of the adjoining vertices.

      vector<VecIter> AdjVertices(FaceIter pFace);
      //: Returns the vertices sharing this face.
      //!param: pFace = The face to query
      //!param: return = a vector of the adjoining vertices.

      vector<FaceIter> AdjFaces(VecIter pVertex);
      //: Returns the faces adjoining this vertex.
      //!param: pVertex = The vertex to query
      //!param: return = a vector of the adjoining faces.      

      vector<FaceIter> AdjFaces(FaceIter pFace);
      //: Returns the faces adjoining this face.
      //!param: pFace = The face to query
      //!param: return = a vector of the adjoining faces.

      //:-----------------
      //: Access functions
      
      const CMaterial& Material(void) const {return m_oMaterial;}
      //: Access the surface material
      //!param: return = the surface material of the mesh.
      
   }; 
   
}

#endif //_VALET_3D_MESH_
