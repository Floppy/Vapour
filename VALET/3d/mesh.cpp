//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/mesh.cpp"
//! author     = "James Smith"
//! date       = "10/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: mesh.cpp,v 1.4 2001/10/24 23:30:35 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

#include "VALET/mesh.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {
  
   CMesh::CMesh() {
      CLog oLog("3d","CMesh::Constructor (default)",LL_OBJECT);
   }
  
   CMesh::~CMesh() {
      CLog oLog("3d","CMesh::Destructor",LL_OBJECT);
   }
  
   const CMaterial& CMesh::Material(void) const {
      CLog oLog("3d","CMesh::Material",LL_FUNCTION);
      return m_oMaterial;
   }
      

}
