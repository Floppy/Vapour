//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3d/surface_template.cpp"
//! author 		= "James Smith"
//! date 		= "12/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: surface_template.cpp,v 1.1 2001/10/12 15:21:27 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

namespace NValet {
  
  // Creates a Model with 0 vertices
  template<class T> 
  CSurface<T>::CSurface() :
    m_iNumVertices(0),
    m_pVertices(NULL)
{
  return;
} //CSurface()
  
  // Creates a Model with the specified number of vertices
  template<class T>
  CSurface<T>::CSurface(int iNumVertices) {
    // Initialise number of vertices
    m_iNumVertices = 0;
    // Allocate vertex storage
    m_pVertices = NULL;
    m_pVertices = new T[iNumVertices];
    // Check vertex allocation and store number of vertices if appropriate
    if (m_pVertices != NULL) m_iNumVertices = iNumVertices;
    // Done
    return;
  } //CSurface(int iNumVertices)
  
  template<class T>
  CSurface<T>::~CSurface() {
    if (m_pVertices != NULL) delete [] m_pVertices;
    return;
  } //~CSurface()
  
  // Adds the specified number of vertices
  // Returns true if addition is successful
  template<class T>
  bool CSurface<T>::AddVertices(int iNumVertices) {
    bool bResult = false;
    if (m_iNumVertices == 0) {
      // Allocate vertex storage
      m_pVertices = new T[iNumVertices];
      // Check vertex allocation and store number of vertices if appropriate
      if (m_pVertices != NULL) {
	// Store vertex count
	m_iNumVertices = iNumVertices;
	// Success!
	bResult = true;
      }
    }
    else {
      iNumVertices += m_iNumVertices;
      // Allocate new vertex storage
      T* pVertices = NULL;
      pVertices = new T[iNumVertices];
      // Check vertex allocation and store info if appropriate
      if (pVertices  != NULL) {
	// Copy vertex data
	memcpy(pVertices,m_pVertices,m_iNumVertices*sizeof(T));
	// Delete old array
	delete [] m_pVertices;
	// Store new array
	m_pVertices = pVertices;
	// Store new vertex count
	m_iNumVertices = iNumVertices;
	// Success!
	bResult = true;
      }
    }
    return bResult;
  } //AddVertices(int iNumVertices)
  
}
