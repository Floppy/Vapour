//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3d/heightfield.cpp"
//! author 		= "James Smith"
//! date 		= "18/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: heightfield.cpp,v 1.2 2001/10/18 13:25:53 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

#include "VALET/heightfield.h"

// VALET includes

// Standard includes

namespace NValet {
   
   CHeightField::CHeightField(int iRows, int iCols) : 
      CSurface<float>(iRows*iCols),
      m_iNumRows(iRows),
      m_iNumCols(iCols),
      m_vecScale(1,1,1)
   {
      return;
   } //CHeightField(int iRows, int iCols)
   
   CHeightField::~CHeightField() {
   } //~CHeightField()
   
   CVector3D CHeightField::Vertex(int iRow, int iCol) const {
      return CVector3D(iCol, CSurface<float>::Vertex((iRow * m_iNumCols) + iCol), iRow) * m_vecScale;
   } //Vertex(int iRow, int iCol) const

   const float& CHeightField::Height(int iRow, int iCol) const {
      return CSurface<float>::Vertex((iRow * m_iNumCols) + iCol);
   } //Height(int iRow, int iCol) const

   float& CHeightField::Height(int iRow, int iCol) {
      return CSurface<float>::Vertex((iRow * m_iNumCols) + iCol);
   } //Height(int iRow, int iCol)

}
