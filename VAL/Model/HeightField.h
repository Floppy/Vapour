//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HeightField.cpp - 16/11/2000 - James Smith
//	Height Field class declaration
//
// $Id: HeightField.h,v 1.0 2000/11/21 16:47:16 waz Exp $
//

#ifndef _VAL_HEIGHTFIELD_
#define _VAL_HEIGHTFIELD_

#pragma once

// VAL includes
#include "Val.h"
#include "Model.h"
#include "SurfaceMaterial.h"

// DLL import/export definitions
#ifndef DLL
	#ifdef VAL_DLL_EXPORT
		#define DLL __declspec(dllexport)
	#endif
	#ifdef VAL_DLL_IMPORT
		#define DLL __declspec(dllimport)
	#endif
	#ifndef DLL
		#define DLL
	#endif
#endif

// The HeightField is defined as a set of heights rows*cols, with spacings as
// appropriate in X and Z. The HeightField is considered to be arranged as follows
// (though may not necessarily be rendered this way). The element (0,0) is at the 
// point (0,x,0). Rows proceed in the +Z direction, and columns in the +X direction. 
// Thus a 5*5 grid with spacing of 1 in each direction will be 4*4 units in size.
// Internally, vertices are stored in order offset(row,col) = (row * m_iNumCols) + col

class DLL CHeightField : public CModel<float> {

public:

   ///////////////////////////////////////////////////////////////////////
   // Construction/Destruction ///////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Creates a HeightField with the specified number of rows and columns
	CHeightField(int iRows, int iCols);

   virtual ~CHeightField();

   ///////////////////////////////////////////////////////////////////////
   // Loading Functions //////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // Sets information for a single vertex
   void SetVertex(int iVertexRow, int iVertexCol, float fHeight);

   // Sets the spacing information between rows and columns
   void SetSpacing(double dRowSpacing, double dColSpacing);

   ///////////////////////////////////////////////////////////////////////
   // Access Functions ///////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////
   
   // How many rows?
   int NumRows(void) const {return m_iNumRows;}

   // How many columns?
   int NumCols(void) const {return m_iNumCols;}

   // Gets the spacing information between rows and columns
   void GetSpacing(double& dRowSpacing, double& dColSpacing);

	// Access to the vertex array
   float Vertex(int iVertexRow, int iVertexCol) const;

   // Access to the material
   CSurfaceMaterial* Material(void);

   ///////////////////////////////////////////////////////////////////////
   // Animation Functions ////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////

   // Shift rows - iNumShifts is the number of places each row moves.
   // If iNumShifts is 2, row 0 moves to row 2. If iNumShifts is -2, 
   // row 3 moves to row 1. Emptied rows are filled with zeroes.
   // This is much faster than ShiftCols, as fast memory operations can be used.
   void ShiftRows(int iNumShifts);

   // Shift columns - iNumShifts is the number of places each column moves.
   // Behaviour is as for ShiftRows
   // This is much slower than ShiftRows, as columns are not contiguous in 
   // memory, so fast memory operations cannot be used.
   void ShiftCols(int iNumShifts);

protected:

   // Number of rows
   int m_iNumRows;

   // Number of columns
   int m_iNumCols;

   // Spacing between rows
   double m_dRowSpacing;
      
   // Spacing between columns
   double m_dColSpacing;

   // The material of the heightfield
   CSurfaceMaterial m_oMaterial;

};

#endif //_VAL_HEIGHTFIELD_
