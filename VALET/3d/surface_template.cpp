//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/surface_template.cpp"
//! author     = "James Smith"
//! date       = "12/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: surface_template.cpp,v 1.4 2001/10/24 23:18:57 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {
  
   template<class T> 
   CSurface<T>::CSurface() {
      CLog oLog("3d","CSurface::Constructor (default)",LL_OBJECT);
   } //CSurface()
  
   template<class T>
   CSurface<T>::CSurface(int iNumVertices) :
      m_oVertices(iNumVertices)
   {
      CLog oLog("3d","CSurface::Constructor (int)",LL_OBJECT);
   } //CSurface(int iNumVertices)
  
   template<class T>
   CSurface<T>::~CSurface() {
      CLog oLog("3d","CSurface::Destructor",LL_OBJECT);
   } //~CSurface()
  
   template<class T>
   bool CSurface<T>::AddVertices(int iNumVertices) {
      CLog oLog("3d","CSurface::AddVertices",LL_FUNCTION);
      m_oVertices.resize(NumVertices() + iNumVertices);
      return true;
   } //AddVertices(int iNumVertices)
  
   template<class T>
   const T& CSurface<T>::Vertex(int iVertex) const {
      CLog oLog("3d","CSurface::Vertex (const)",LL_FUNCTION);
      return m_oVertices[iVertex];
   }
   
   template<class T>
   T& CSurface<T>::Vertex(int iVertex) {
      CLog oLog("3d","CSurface::Vertex",LL_FUNCTION);
      return m_oVertices[iVertex];
   }
   
   template<class T>
   int CSurface<T>::NumVertices(void) const {
      CLog oLog("3d","CSurface::NumVertices",LL_FUNCTION);
      return m_oVertices.size();
   }
    

}
