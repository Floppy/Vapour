//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VAL_SURFACE_
#define _VAL_SURFACE_

////////////////
//! file 		= "VALET/3d/surface.h"
//! author 		= "James Smith"
//! date 		= "12/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: surface.h,v 1.1 2001/10/12 15:21:27 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "../arch/valet.h"

namespace NValet {
  
  //: A templated base class for a 3D surface
  // The class T is the type of the vertices/control points for the surface.

  template<class T> class CSurface {
    
  public:
    
    //:-------------------------
    //: Construction/Destruction

    CSurface();
    //: Creates an empty surface
    
    CSurface(int iNumVertices);
    //: Creates a surface with the specified number of vertices.
    // All the vertices will be their default value.
    //!param: iNumVertices = the number of vertices to create.
    
    virtual ~CSurface();
    
    //:-------------------------
    //: Other functions

    virtual bool AddVertices(int iNumVertices);
    //: Adds the specified number of vertices
    // Returns true if addition is successful
    // Do not use this too much - it is very expensive.
    
    // Sets information for a single vertex
    void SetVertex(int iVertex, const T& oVertex) {if (iVertex<m_iNumVertices) m_pVertices[iVertex]=oVertex;}
    
    // Provides direct access to the vertex array
    const T* Vertices(void) const {return m_pVertices;}
    
    // The number of vertices in the model.
    int NumVertices(void) const {return m_iNumVertices;}
    
    // Access to a particular vertex
    const T* Vertex(int iVertex) const {return (iVertex<m_iNumVertices) ? m_pVertices + iVertex : NULL;}
    
  protected:
	
   // Geometry
    int m_iNumVertices;
    T* m_pVertices;
    
  };

}  

#include "surface_template.cpp"

#endif //_VALET_SURFACE_
