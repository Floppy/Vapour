//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/trimodel.cpp"
//! author     = "James Smith"
//! date       = "10/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: trimodel.cpp,v 1.4 2001/10/24 23:30:35 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

#include "VALET/trimodel.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {
  
   CTriModel::CTriModel() {
      CLog oLog("3d","CTriModel::Constructor (default)",LL_OBJECT);
   }
  
   CTriModel::~CTriModel() {
      CLog oLog("3d","CTriModel::Destructor",LL_OBJECT);
   }

   int CTriModel::NumMeshes(void) const {
      CLog oLog("3d","CTriModel::NumMeshes",LL_FUNCTION);
      return m_oMeshes.size();
   }

   const CMesh& CTriModel::Mesh(int iMesh) const {
      CLog oLog("3d","CTriModel::Mesh (const)",LL_FUNCTION);
      return m_oMeshes[iMesh];
   }

   CMesh& CTriModel::Mesh(int iMesh) {
      CLog oLog("3d","CTriModel::Mesh",LL_FUNCTION);
      return m_oMeshes[iMesh];
   }
  
}
