//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HeightField.cpp - 16/11/2000 - James Smith
//	Height Field class implementation
//
// $Id: HeightField.cpp,v 1.0 2000/11/21 16:47:15 waz Exp $
//

#include "stdafx.h"

#include "HeightField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////
// Construction/Destruction ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Creates a HeightField with the specified number of rows and columns
CHeightField::CHeightField(int iRows, int iCols) : 
   CModel<float>(iRows*iCols),
   m_iNumRows(iRows),
   m_iNumCols(iCols),
   m_dRowSpacing(1),
   m_dColSpacing(1)
{
   return;
} //CHeightField(int iRows, int iCols)

CHeightField::~CHeightField() {
} //~CHeightField()

///////////////////////////////////////////////////////////////////////
// Loading Functions //////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Sets information for a single vertex
void CHeightField::SetVertex(int iVertexRow, int iVertexCol, float fHeight) {
   // Test limits of height field
   if ((iVertexRow > m_iNumRows) || (iVertexCol > m_iNumCols)) return;
   // If it's OK, carry on
   else m_pVertices[iVertexRow * m_iNumCols + iVertexCol] = fHeight;
} //SetVertex(int iVertexRow, int iVertexCol, float fHeight)

// Sets the spacing information between rows and columns
void CHeightField::SetSpacing(double dRowSpacing, double dColSpacing) {
   m_dRowSpacing = dRowSpacing;
   m_dColSpacing = dColSpacing;
   return;
}

///////////////////////////////////////////////////////////////////////
// Access Functions ///////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Gets the spacing information between rows and columns
void CHeightField::GetSpacing(double& dRowSpacing, double& dColSpacing) {
   dRowSpacing = m_dRowSpacing;
   dColSpacing = m_dColSpacing;
   return;
}

// Access to the vertex normal array
float CHeightField::Vertex(int iVertexRow, int iVertexCol) const {
   // Test limits of height field
   if ((iVertexRow > m_iNumRows) || (iVertexCol > m_iNumCols)) return 0;
   // If it's OK, carry on
   else return m_pVertices[iVertexRow * m_iNumCols + iVertexCol];
} //Vertex(int iVertexRow, int iVertexCol) const

///////////////////////////////////////////////////////////////////////
// Animation Functions ////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// Shift rows - iNumShifts is the number of places each row moves.
// If iNumShifts is 2, row 0 moves to row 2. If iNumShifts is -2, 
// row 3 moves to row 1. Emptied rows are filled with zeroes.
// This is much faster than ShiftCols, as fast memory operations can be used.
void CHeightField::ShiftRows(int iNumShifts) {
   // If we're shifting down
   if (iNumShifts < 0) {
      // Go from the bottom row up
      for (int iDestRow=0; iDestRow<m_iNumRows; iDestRow++) {
         // Calculate source row
         int iSourceRow = iDestRow - iNumShifts;
         // If source data is available
         if (iSourceRow < m_iNumRows) {
            // Move row
            memcpy(m_pVertices+(iDestRow*m_iNumCols), m_pVertices+(iSourceRow*m_iNumCols), m_iNumCols * sizeof(float));
         }
         // Otherwise, if this row has no source data
         else {
            // Zero row
            memset(m_pVertices+(iDestRow*m_iNumCols), 0, m_iNumCols * sizeof(float));
         }
      }
   }
   // Otherwise, we're shifting up
   else {
      // So go from the top row down
      for (int iDestRow=m_iNumRows; iDestRow-->0; ) {
         // Calculate source row
         int iSourceRow = iDestRow - iNumShifts;
         // If source data is available
         if (iSourceRow >= 0) {
            // Move row
            memcpy(m_pVertices+(iDestRow*m_iNumCols), m_pVertices+(iSourceRow*m_iNumCols), m_iNumCols * sizeof(float));
         }
         // Otherwise, if this row has no source data
         else {
            // Zero row
            memset(m_pVertices+(iDestRow*m_iNumCols), 0, m_iNumCols * sizeof(float));
         }
      }
   }
   // Done
   return;
} //ShiftRows(int iNumShifts)

// Shift columns - iNumShifts is the number of places each column moves.
// Behaviour is as for ShiftRows
// This is much slower than ShiftRows, as columns are not contiguous in 
// memory, so fast memory operations cannot be used.
void CHeightField::ShiftCols(int iNumShifts) {
   // If we're shifting down
   if (iNumShifts < 0) {
      // Go from the bottom column up
      for (int iDestCol=0; iDestCol<m_iNumCols; iDestCol++) {
         // Calculate source column
         int iSourceCol = iDestCol - iNumShifts;
         // If source data is available
         if (iSourceCol < m_iNumCols) {
            // Move column
            for (int iRow=0; iRow<m_iNumRows; iRow++) SetVertex(iRow,iDestCol,Vertex(iRow,iSourceCol));
         }
         // Otherwise, if this column has no source data
         else {
            // Zero column
            for (int iRow=0; iRow<m_iNumRows; iRow++) SetVertex(iRow,iDestCol,0);
         }
      }
   }
   // Otherwise, we're shifting up
   else {
      // So go from the top column down
      for (int iDestCol=m_iNumCols; iDestCol-->0; ) {
         // Calculate source column
         int iSourceCol = iDestCol - iNumShifts;
         // If source data is available
         if (iSourceCol >= 0) {
            // Move column
            for (int iRow=0; iRow<m_iNumRows; iRow++) SetVertex(iRow,iDestCol,Vertex(iRow,iSourceCol));
         }
         // Otherwise, if this row has no source data
         else {
            // Zero column
            for (int iRow=0; iRow<m_iNumRows; iRow++) SetVertex(iRow,iDestCol,0);
         }
      }
   }
   // Done
   return;
} //ShiftCols(int iNumShifts)

