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
//! rcsid 		= "$Id: surface.h,v 1.4 2001/10/17 14:44:53 vap-warren Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"

#include <vector>

namespace NValet {
  
  //: A templated base class for a 3D surface
  // The class T is the type of the vertices/control points for the surface.

  template<class T> class CSurface {
    
  protected:
	
    vector<T> m_lVertices;
    //: The vertices themselves
    
  public:
    
    //:-------------------------
    //: Construction/Destruction

    CSurface();
    //: Creates an empty surface
    // i.e. 0 vertices.
   
    CSurface(int iNumVertices);
    //: Creates a surface with the specified number of vertices.
    // All the vertices will be their default value.
    //!param: iNumVertices = the number of vertices to create.
    
    virtual ~CSurface();
    
    //:-------------------------
    //: Other functions

    virtual bool AddVertices(int iNumVertices);
    //: Adds the specified number of vertices
    // Returns true if addition is successful. The new vertices will be set to their default value.
    //!param: iNumVertices = the number of vertices to add.
    //!param: return = true if succesful.
    
    void SetVertex(int iVertex, const T& oValue) {m_lVertices[iVertex] = oValue;}
    //: Sets information for a single vertex
    //!param: iVertex = the vertex to set.
    //!param: oValue = the value to set the vertex to.
    
    const T* Vertex(int iVertex) const {return m_lVertices[iVertex];}
    //: Access to a particular vertex
    //!param: iVertex = the vertex to access.
    //!param: return = the requested vertex.

    int NumVertices(void) const {return m_lVertices.size();}
    //: The number of vertices in the model.
    //!param: return = the number of vertices.
    
  };

}  

#include "surface_template.cpp"

#endif //_VALET_SURFACE_
