//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/scalar.test.cpp"
//! author 		= "James Smith"
//! date 		= "1/11/2001"
//! rcsid 		= "$Id: scalar.test.cpp,v 1.1 2001/11/02 11:54:27 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/scalar.h"
#include "VALET/axisrotation.h"
#include "VALET/log.h"

#include <stdlib.h>
#include <math.h>

//#===--- Namespaces
using namespace NVALET;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("math",true,LL_ALL);
   CLog oLog("math","scalar.test",LL_FUNCTION);

   bool bError(false);

   for (int i=0; i<10; i++) {

      const double dS = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      
      CScalar oScl(dS);

      // Check string output and parsing
      {
         char* pcString = oScl.ToString(9);
         CScalar oSScl;
         if (!oSScl.ParseString(pcString)) {
            oLog.Trace("String parsing failed",LL_ERROR);
            bError = true;
         }
         if ((oScl.ToDouble() - oSScl.ToDouble()) > 1e-9) {
            oLog.Trace("String I/O failed",LL_ERROR);
            bError = true;
         }
         free(pcString);
      }
   }
   return bError ? -1 : 0;
}

