//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/3D/quaternion.test.cpp"
//! author 		= "James Smith"
//! date 		= "1/11/2001"
//! rcsid 		= "$Id: quaternion.test.cpp,v 1.1 2001/11/02 16:36:53 vap-james Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/quaternion.h"
#include "VALET/log.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

//#===--- Namespaces
using namespace NVALET;

int main(int argc, char *argv[]) {
 
   g_oLogManager.SetLog("math",true,LL_ALL);
   CLog oLog("math","quaternion.test",LL_FUNCTION);

   bool bError(false);

   // Maximum allowed conversion error
   const double dError = 1e-10;

   for (int i=0; i<10; i++) {

      const double dX = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dY = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dZ = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      const double dS = static_cast(double,rand()) / static_cast(double,RAND_MAX);
      
      // Check quaternion constructor
      CQuaternion oQuat(dS,CVector3D(dX,dY,dZ));

      // Check axisrotation conversion
      {
         CAxisRotation oRot(oQuat);
         CQuaternion oRQuat(oRot);
         if ((oQuat.Vector() - oRQuat.Vector()).Length() > dError) {
            oLog.Trace("Axis Rotation conversion failed (vector part)",LL_ERROR);
            bError = true;
         }
         if (oQuat.Scalar() - oRQuat.Scalar() > dError) {
            oLog.Trace("Axis Rotation conversion failed (scalar part)",LL_ERROR);
            bError = true;
         }
      }

      // Check euler angle conversion
      for (int t=0; t<CEulerType::m_iNumIDs; t++) {
         CEulerRotation oEAngle(oQuat,static_cast(CEulerType::EEulerTypeID,t));
         CQuaternion oEQuat(oEAngle);
         if ((oQuat.Vector() - oEQuat.Vector()).Length() > dError) {
            oLog.Trace("Euler conversion failed (vector part)",LL_ERROR);
            char pcStr[512];
            sprintf(pcStr,"0x%X: %f %f %f != %f %f %f\n",t,oQuat.Vector().X(),oQuat.Vector().Y(),oQuat.Vector().Z(),oEQuat.Vector().X(),oEQuat.Vector().Y(),oEQuat.Vector().Z());
            oLog.Trace(pcStr,LL_ERROR);
            bError = true;
         }
         if (oQuat.Scalar() - oEQuat.Scalar() > dError) {
            oLog.Trace("Euler conversion failed (scalar part)",LL_ERROR);
            char pcStr[512];
            sprintf(pcStr,"0x%X: %f != %f\n",t,oQuat.Scalar(),oEQuat.Scalar());
            oLog.Trace(pcStr,LL_ERROR);
            bError = true;
         }
      }

   }
   return bError ? -1 : 0;
}

