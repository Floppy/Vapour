//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/vector3d.test.cpp"
//! author 		= "James Smith"
//! date 		= "1/11/2001"
//! rcsid 		= "$Id: vector3d.test.cpp,v 1.1 2001/11/01 15:51:18 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/vector3d.h"
#include "VALET/axisrotation.h"
#include "VALET/log.h"

#include <stdlib.h>
#include <math.h>

//#===--- Namespaces
using namespace NVALET;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("math",true,LL_ALL);
   CLog oLog("math","vector3d.test",LL_FUNCTION);

   bool bError(false);

   // Maximum allowed conversion error
   const double dError = 1e-10;

   for (int i=0; i<10; i++) {

      const double dX = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dY = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dZ = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      
      // Check axisrotation constructor
      CVector3D oVec(dX,dY,dZ);

      // Check equality operator
      if (!(oVec == oVec)) {
         oLog.Trace("Equality test failed",LL_ERROR);
         bError = true;
      }

      // Check addition/multiplication      
      if (!(oVec+oVec == oVec*2)) {
         oLog.Trace("Add/Mult test failed",LL_ERROR);
         bError = true;         
      }
      
      // Check subtraction
      if ((oVec-oVec).Length() != 0) {
         oLog.Trace("Subtract test failed",LL_ERROR);
         bError = true;         
      }

      // Check inversion and copy constructor
      CVector3D oInv(oVec);
      oInv.Invert();
      if ((oVec+oInv).Length() != 0) {
         oLog.Trace("Inversion test failed",LL_ERROR);
         bError = true;
      }

      // Check length functions
      double dLength = static_cast(double,rand()) / static_cast(double,RAND_MAX) * 100;
      oInv.SetLength(dLength);
      if ((oInv.Length() - dLength) > dError) {
         oLog.Trace("SetLength test failed",LL_ERROR);
         bError = true;
      }
      oInv.Normalise();
      if ((oInv.Length() - 1) > dError) {
         oLog.Trace("Normalise test failed",LL_ERROR);
         bError = true;
      }

      // Check rotation functions
      CAxisRotation oRot(0,1,0,M_PI*2);
      if ((oVec.Rotate(oRot) - oVec).Length() > dError) {
         oLog.Trace("Rotation test failed",LL_ERROR);
         bError = true;         
      }

      // Check string output and parsing
      {
         char* pcString = oVec.ToString(9);
         CVector3D oSVec;
         if (!oSVec.ParseString(pcString)) {
            oLog.Trace("String parsing failed",LL_ERROR);
            bError = true;
         }
         if ((oVec - oSVec).Length() > 1e-9) {
            oLog.Trace("String I/O failed (axis part)",LL_ERROR);
            bError = true;
         }
         free(pcString);
      }
   }
   return bError ? -1 : 0;
}

