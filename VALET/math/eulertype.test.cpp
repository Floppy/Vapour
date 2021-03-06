//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/eulertype.test.cpp"
//! author 		= "James Smith"
//! date 		= "1/11/2001"
//! rcsid 		= "$Id: eulertype.test.cpp,v 1.2 2001/11/01 14:01:53 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/eulertype.h"
#include "VALET/log.h"

//#===--- Namespaces
using namespace NVALET;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("math",true,LL_ALL);
   CLog oLog("math","eulertype.test",LL_FUNCTION);

   bool bError(false);

   CEulerType oType0(CEulerType::EU_XYZS);
   if (oType0.InnerAxis() != CEulerType::EU_X_AXIS) {
      oLog.Trace("EU_XYZS.InnerAxis() != EU_X_AXIS",LL_ERROR);
      bError = true;  
   }
   if (oType0.OddParity() != false) {
      oLog.Trace("EU_XYZS.OddParity() != false",LL_ERROR);
      bError = true; 
   }
   if (oType0.Repetition() != false) {
      oLog.Trace("EU_XYZS.Repetition() != false",LL_ERROR);
      bError = true; 
   }
   if (oType0.RotatingFrame() != false) {
      oLog.Trace("EU_XYZS.RotatingFrame() != false",LL_ERROR);
      bError = true; 
   }

   CEulerType oType1(CEulerType::EU_ZYXR);
   if (oType1.InnerAxis() != CEulerType::EU_X_AXIS) {
      oLog.Trace("EU_ZYXR.InnerAxis() != EU_X_AXIS",LL_ERROR);
      bError = true; 
   }
   if (oType1.OddParity() != false) {
      oLog.Trace("EU_ZYXR.OddParity() != false",LL_ERROR);
      bError = true; 
   }
   if (oType1.Repetition() != false) {
      oLog.Trace("EU_ZYXR.Repetition() != false",LL_ERROR);
      bError = true; 
   }
   if (oType1.RotatingFrame() != true) {
      oLog.Trace("EU_ZYXR.RotatingFrame() != true",LL_ERROR);
      bError = true; 
   }

   CEulerType oType2(CEulerType::EU_XZXS);
   if (oType2.InnerAxis() != CEulerType::EU_X_AXIS) {
      oLog.Trace("EU_XZXS.InnerAxis() != EU_X_AXIS",LL_ERROR);
      bError = true;
   }
   if (oType2.OddParity() != true) {
      oLog.Trace("EU_XZXS.OddParity() != true",LL_ERROR);
      bError = true; 
   }
   if (oType2.Repetition() != true) {
      oLog.Trace("EU_XZXS.Repetition() != true",LL_ERROR);
      bError = true; 
   }
   if (oType2.RotatingFrame() != false) {
      oLog.Trace("EU_XZXS.RotatingFrame() != false",LL_ERROR);
      bError = true;   
   }

   CEulerType oType3(CEulerType::EU_ZODS);
   if (oType3.InnerAxis() != CEulerType::EU_Z_AXIS) {
      oLog.Trace("EU_ZODS.InnerAxis() != EU_Z_AXIS",LL_ERROR);
      bError = true;
   }
   if (oType3.OddParity() != true) {
      oLog.Trace("EU_ZODS.OddParity() != true",LL_ERROR);
      bError = true; 
   }
   if (oType3.Repetition() != false) {
      oLog.Trace("EU_ZODS.Repetition() != false",LL_ERROR);
      bError = true; 
   }
   if (oType3.RotatingFrame() != false) {
      oLog.Trace("EU_ZODS.RotatingFrame() != false",LL_ERROR);
      bError = true;   
   }


   CEulerType oType4(CEulerType::EU_YESR);
   if (oType4.InnerAxis() != CEulerType::EU_Y_AXIS) {
      oLog.Trace("EU_YESR.InnerAxis() != EU_Y_AXIS",LL_ERROR);
      bError = true;
   }
   if (oType4.OddParity() != false) {
      oLog.Trace("EU_YESR.OddParity() != false",LL_ERROR);
      bError = true; 
   }
   if (oType4.Repetition() != true) {
      oLog.Trace("EU_YESR.Repetition() != true",LL_ERROR);
      bError = true; 
   }
   if (oType4.RotatingFrame() != true) {
      oLog.Trace("EU_YESR.RotatingFrame() != true",LL_ERROR);
      bError = true;   
   }

   return bError ? -1 : 0;
}

