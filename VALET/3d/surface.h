//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_SURFACE_
#define _VALET_3D_SURFACE_

////////////////
//! file 		= "VALET/3d/surface.h"
//! author 		= "James Smith"
//! date 		= "12/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: surface.h,v 1.7 2001/10/23 22:42:14 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"

#include <vector>
using namespace std;

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
    
    const T& Vertex(int iVertex) const {return m_lVertices[iVertex];}
    //: Access to a particular vertex
    //!param: iVertex = the index of the vertex to access.
    //!param: return = a reference to the requested vertex.

    T& Vertex(int iVertex) {return m_lVertices[iVertex];}
    //: Non-const access to a particular vertex
    // This can be used to set vertex information.
    //!param: iVertex = the index of the vertex to access.
    //!param: return = a reference to the requested vertex.

    int NumVertices(void) const {return m_lVertices.size();}
    //: The number of vertices in the model.
    //!param: return = the number of vertices.
    
  };

}  

#include "surface_template.cpp"

#endif //_VALET_3D_SURFACE_
