//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file       = "VALET/3d/heightfield.cpp"
//! author     = "James Smith"
//! date       = "18/10/2001"
//! lib        = libVALET3d
//! rcsid      = "$Id: heightfield.cpp,v 1.5 2001/10/24 23:30:35 vap-james Exp $"
//! userlevel  = Normal
//! docentry   = "VALET.3D.Surface Representations"

#include "VALET/heightfield.h"

// VALET includes
#include "VALET/log.h"

// Standard includes

namespace NVALET {
   
   CHeightField::CHeightField(int iRows, int iCols) : 
      CSurface<float>(iRows*iCols),
      m_iNumRows(iRows),
      m_iNumCols(iCols),
      m_oScale(1,1,1)
   {
      CLog oLog("3d","CHeightField::Constructor(int,int)",LL_OBJECT);
      return;
   } //CHeightField(int iRows, int iCols)
   
   CHeightField::~CHeightField() {
      CLog oLog("3d","CHeightField::Destructor",LL_OBJECT);
   } //~CHeightField()
   
   int CHeightField::Rows(void) const {
      CLog oLog("3d","CHeightField::Rows",LL_FUNCTION);
      return m_iNumRows;
   }
      
   int CHeightField::Cols(void) const {
      CLog oLog("3d","CHeightField::Cols",LL_FUNCTION);
      return m_iNumCols;
   }

   const CVector3D& CHeightField::Scale(void) const {
      CLog oLog("3d","CHeightField::Scale (const)",LL_FUNCTION);
      return m_oScale;
   }

   CVector3D& CHeightField::Scale(void) {
      CLog oLog("3d","CHeightField::Scale",LL_FUNCTION);
      return m_oScale;
   }

   const CMaterial& CHeightField::Material(void) const {
      CLog oLog("3d","CHeightField::Material (const)",LL_FUNCTION);
      return m_oMaterial;
   }

   CMaterial& CHeightField::Material(void) {
      CLog oLog("3d","CHeightField::Material",LL_FUNCTION);
      return m_oMaterial;
   }

   CVector3D CHeightField::Vertex3D(int iRow, int iCol) const {
      CLog oLog("3d","CHeightField::Vertex3D",LL_FUNCTION);
      return CVector3D(iCol, Vertex((iRow * m_iNumCols) + iCol), iRow) * m_oScale;
   } //3DVertex(int iRow, int iCol) const

   const float& CHeightField::Height(int iRow, int iCol) const {
      CLog oLog("3d","CHeightField::Height (const)",LL_FUNCTION);
      return Vertex((iRow * m_iNumCols) + iCol);
   } //Height(int iRow, int iCol) const

   float& CHeightField::Height(int iRow, int iCol) {
      CLog oLog("3d","CHeightField::Height",LL_FUNCTION);
      return Vertex((iRow * m_iNumCols) + iCol);
   } //Height(int iRow, int iCol)

}
