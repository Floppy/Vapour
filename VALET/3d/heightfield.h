//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_3D_HEIGHTFIELD_
#define _VALET_3D_HEIGHTFIELD_

////////////////
//! file 		= "VALET/3d/heightfield.h"
//! author 		= "James Smith"
//! date 		= "10/10/2001"
//! lib 		= libVALET3d
//! rcsid 		= "$Id: heightfield.h,v 1.3 2001/10/18 13:25:53 vap-james Exp $"
//! userlevel 	        = Normal
//! docentry 	        = "VALET.3D.Surface Representations"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/surface.h"
#include "VALET/vector3d.h"
#include "VALET/material.h"

namespace NValet {

   //: A Heightfield, or elevation grid.
   // The HeightField is defined as a set of heights rows*cols, with spacings as
   // appropriate in X and Z. The HeightField is considered to be arranged as follows
   // (though may not necessarily be rendered this way). The element (0,0) is at the 
   // point (0,0,0). Rows proceed in the +Z direction, and columns in the +X direction. 
   // The default row-column spacing is 1. This can be changed using the scaling vector.

   class CHeightField : public CSurface<float> {
      
   protected:
      
      int m_iNumRows;
      //: Number of rows (Z)
      
      int m_iNumCols;
      //: Number of columns (X)
      
      CVector3D m_vecScale;
      //: Scale factor
      
      CMaterial m_oMaterial;
      //: The material of the heightfield
      
   public:    
      
      //:-------------------------
      //: Construction/Destruction
      
      CHeightField(int iRows, int iCols);
      //: Creates a HeightField with the specified number of rows and columns
      //!param: iRows = the number of rows.
      //!param: iCols = the number of columns
      
      ~CHeightField();
      //: Destructor
      
      //:-----------------
      //: Access functions
      
      int Rows(void) const {return m_iNumRows;}
      //: How many rows?
      //!param: return = the number of rows (in the Z direction).
      
      int Cols(void) const {return m_iNumCols;}
      //: How many columns?
      //!param: return = the number of columns (in the X direction).

      const CVector3D& Scale(void) const {return m_vecScale;}
      //: Access to the scale factor
      //!param: return = a vector representing the scale factor of the heightfield.

      CVector3D& Scale(void) {return m_vecScale;}
      //: Non-const access to the scale factor
      // This can be used to set the scale factor.
      //!param: return = a reference to a vector representing the scale factor.

      const CMaterial& Material(void) const {return m_oMaterial;}
      //: Access the surface material
      //!param: return = the surface material of the height field.

      CMaterial& Material(void) {return m_oMaterial;}
      //: Non-const access to the surface material
      // This can be used to set the material.
      //!param: return = the surface material of the height field.

      CVector3D Vertex(int iRow, int iCol) const;
      //: Access a 3D vertex.
      // This functions calculates a 3D vertex position for the specified vertex.
      //!param: iRow = the row of the vertex.
      //!param: iCol = the column of the vertex.
      //!param: return = a 3d vertex position for the specified vertex.

      const float& Height(int iRow, int iCol) const;
      //: Access an individual height value.
      //!param: iRow = the row of the vertex.
      //!param: iCol = the column of the vertex.
      //!param: return = the height value.

      float& Height(int iRow, int iCol);
      //: Non-const access to an individual height value.
      // This can be use to set the height.
      //!param: iRow = the row of the vertex.
      //!param: iCol = the column of the vertex.
      //!param: return = a reference to the height value.
      
   };

}

#endif //_VALET_3D_HEIGHTFIELD_
