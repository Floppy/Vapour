//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/axisrotation.test.cpp"
//! author 		= "James Smith"
//! date 		= "1/11/2001"
//! rcsid 		= "$Id: axisrotation.test.cpp,v 1.1 2001/11/01 15:51:18 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/axisrotation.h"
#include "VALET/log.h"

#include <stdlib.h>
#include <math.h>

//#===--- Namespaces
using namespace NVALET;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("math",true,LL_ALL);
   CLog oLog("math","axisrotation.test",LL_FUNCTION);

   bool bError(false);

   // Maximum allowed conversion error
   const double dError = 1e-10;

   for (int i=0; i<10; i++) {

      const double dX = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dY = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dZ = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dA = static_cast(double,rand()) / static_cast(double,RAND_MAX) * M_PI;
      
      // Check axisrotation constructor
      CAxisRotation oRot(dX,dY,dZ,dA);
      if (oRot.Axis() != CVector3D(dX,dY,dZ)) {
         oLog.Trace("Axis() test failed",LL_ERROR);
         bError = true;       
      }
      if (oRot.Angle() != dA) {
         oLog.Trace("Angle() test failed",LL_ERROR);
         bError = true;
      }

      // Check equality operator
      if (!(oRot == oRot)) {
         oLog.Trace("Equality test failed",LL_ERROR);
         bError = true;
      }
      
      // Normalise the rotation.
      oRot.Normalise();

      // Check quaternion conversion
      {
         CQuaternion oQuat(oRot);
         CAxisRotation oQRot(oQuat);
         if ((oRot.Axis() - oQRot.Axis()).Length() > dError) {
            oLog.Trace("Quaternion conversion failed (axis part)",LL_ERROR);
            bError = true;
         }
         if (oRot.Angle() - oQRot.Angle() > dError) {
            oLog.Trace("Quaternion conversion failed (angle part)",LL_ERROR);
            bError = true;
         }
      }

      // Check euler angle conversion
      for (int t=0; t<CEulerType::m_iNumIDs; t++) {
         CEulerRotation oEAngle(oRot,static_cast(CEulerType::EEulerTypeID,t));
         CAxisRotation oERot(oEAngle);
         if ((oRot.Axis() - oERot.Axis()).Length() > dError) {
            oLog.Trace("Euler conversion failed (axis part)",LL_ERROR);
            bError = true;
         }
         if (oRot.Angle() - oERot.Angle() > dError) {
            oLog.Trace("Euler conversion failed (angle part)",LL_ERROR);
            bError = true;
         }
      }

      // Check string output and parsing
      {
         char* pcString = oRot.ToString(9);
         CAxisRotation oSRot;
         if (!oSRot.ParseString(pcString)) {
            oLog.Trace("String parsing failed",LL_ERROR);
            bError = true;
         }
         if ((oRot.Axis() - oSRot.Axis()).Length() > 1e-9) {
            oLog.Trace("String I/O failed (axis part)",LL_ERROR);
            bError = true;
         }
         if (oRot.Angle() - oSRot.Angle() > 1e-9) {
            oLog.Trace("String I/O failed (angle part)",LL_ERROR);
            bError = true;
         }
         free(pcString);
      }
   }
   return bError ? -1 : 0;
}

