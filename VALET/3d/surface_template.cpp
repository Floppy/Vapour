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
//! rcsid 		= "$Id: surface_template.cpp,v 1.2 2001/10/15 16:06:23 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

namespace NValet {
  
  template<class T> 
  CSurface<T>::CSurface() {
  } //CSurface()
  
  template<class T>
  CSurface<T>::CSurface(int iNumVertices) :
    m_lVertices(iNumVertices)
  {
  } //CSurface(int iNumVertices)
  
  template<class T>
  CSurface<T>::~CSurface() {
  } //~CSurface()
  
  template<class T>
  bool CSurface<T>::AddVertices(int iNumVertices) {
    m_lVertices.resize(NumVertices() + iNumVertices);
    return true;
  } //AddVertices(int iNumVertices)
  
}
