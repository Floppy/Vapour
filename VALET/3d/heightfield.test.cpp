//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/heightfield.test.cpp"
//! author 		= "James Smith"
//! date 		= "18/10/2001"
//! rcsid 		= "$Id: heightfield.test.cpp,v 1.5 2001/10/24 19:55:44 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/heightfield.h"
#include "VALET/log.h"

#include <stdio.h>
#include <math.h>

//#===--- Namespaces
using namespace NValet;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("3d",true,LL_ALL);
   g_oLogManager.SetLog("math",true,LL_ALL);

   bool bError = false;

   // Create a small heightfield
   int iNumRows = 5;
   int iNumCols = 5;
   CHeightField oHF(iNumRows,iNumCols);
   
   // Fill with random data
   for (int r=0; r<iNumRows; r++) {
      for (int c=0; c<iNumRows; c++) {
         oHF.Height(r,c) = rand();
      }
   }

   // Check access functions
   for (int r=0; r<iNumRows; r++) {
      for (int c=0; c<iNumRows; c++) {
         if (oHF.Height(r,c) != oHF.Vertex(r*iNumRows + c)) bError = true;
      }
   }
   
   return bError ? -1 : 0;

}

