//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarPose.cpp - 21/2/2000 - James Smith
//	Avatar pose class implementation
//
// $Id: AvatarPose.cpp,v 1.3 2000/08/21 17:01:03 waz Exp $
//


#include "stdafx.h"

#include <math.h>

#include "AvatarPose.h"
#include "MathConstants.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////
// CAvatarPose

// Copy Constructor
CAvatarPose::CAvatarPose(const CAvatarPose& apOtherPose) {
   m_iNumJoints = apOtherPose.m_iNumJoints;
   m_pJointRotations = new CAxisRotation[m_iNumJoints];
   m_vecRootTranslation = apOtherPose.m_vecRootTranslation;
   for (int i=0; i<m_iNumJoints; i++) {
      m_pJointRotations[i] = apOtherPose.m_pJointRotations[i];
   }
   m_pSourceQuats = NULL;
   m_pTargetQuats = NULL;
   m_pTargetTranslation = NULL;
   return;
} //CAvatarPose(const CAvatarPose& apOtherPose)

CAvatarPose::CAvatarPose(int iNumJoints) {
   m_iNumJoints = iNumJoints;
   m_pJointRotations = new CAxisRotation[m_iNumJoints];
   m_pSourceQuats = NULL;
   m_pTargetQuats = NULL;
   m_pTargetTranslation = NULL;
   return;
} //CAvatarPose()

// Loads a pose directly from a file
CAvatarPose::CAvatarPose(const char* pszFilename) {
   m_iNumJoints = 0;
   m_pJointRotations = NULL;
   m_pSourceQuats = NULL;
   m_pTargetQuats = NULL;
   m_pTargetTranslation = NULL;
   Load(pszFilename);
   return;
}

// Loads a pose directly from a wedgie
CAvatarPose::CAvatarPose(const char* pszFilename, CWedgie &oWedgie) {
   m_iNumJoints = 0;
   m_pJointRotations = NULL;
   m_pSourceQuats = NULL;
   m_pTargetQuats = NULL;
   m_pTargetTranslation = NULL;
   Load(pszFilename, oWedgie);
   return;
}

// Loads a pose directly from a stream
CAvatarPose::CAvatarPose(ifstream& isInputStream) {
   m_iNumJoints = 0;
   m_pJointRotations = NULL;
   m_pSourceQuats = NULL;
   m_pTargetQuats = NULL;
   m_pTargetTranslation = NULL;
   Load(isInputStream);
   return;
}

CAvatarPose::~CAvatarPose() {
   delete [] m_pJointRotations;
   if (m_pSourceQuats != NULL) delete [] m_pSourceQuats;
   if (m_pTargetQuats != NULL) delete [] m_pTargetQuats;
   if (m_pTargetTranslation != NULL) delete m_pTargetTranslation;
   return;
} //~CAvatarPose()

//////////////////////////////////////////////////////////////////////
// Assignment/Access
//////////////////////////////////////////////////////////////////////

void CAvatarPose::SetRootTranslation(CVector3D& vecTranslation) {
   m_vecRootTranslation = vecTranslation;
   return;
} //SetRootTranslation(CVector3D& vecTranslation)

void CAvatarPose::SetJointRotation(int iJoint, CAxisRotation& rotNewAngle) {
   if ((iJoint < m_iNumJoints) && (iJoint >= 0)) m_pJointRotations[iJoint] = rotNewAngle;
   return;
} //SetJointRotation(unsigned int iJoint, CAxisRotation& rotNewAngle)

CVector3D CAvatarPose::GetRootTranslation(void) {
   return m_vecRootTranslation;
} //GetRootTranslation(void)

CAxisRotation CAvatarPose::GetJointRotation(int iJoint) {
   if ((iJoint < m_iNumJoints) && (iJoint >= 0)) return m_pJointRotations[iJoint];
   else {
      CAxisRotation rotNull;
      return rotNull;
   }
} //GetJointRotation(unsigned int iJoint)

//////////////////////////////////////////////////////////////////////
// Animation Functions
//////////////////////////////////////////////////////////////////////

int CAvatarPose::InitInterpolation(const CAvatarPose& poTarget, bool bAccel) const {
   // Check number of joints
   if (m_iNumJoints != poTarget.m_iNumJoints) return 0;
   // Make sure we're ready to start a new interpolation
   if ((m_pSourceQuats != NULL) || (m_pTargetQuats != NULL) || (m_pTargetTranslation != NULL)) {
      // A previous interpolation was not stopped - so stop it
      EndInterpolation();
   }
   // Store acceleration value
   m_bAccel = bAccel;
   // Store target translation
   m_pTargetTranslation = new CVector3D(poTarget.m_vecRootTranslation);
   // Precalc source quats
   m_pSourceQuats = new CQuaternion[m_iNumJoints];
   for (int i=0; i<m_iNumJoints; i++) {
      m_pSourceQuats[i] = CQuaternion(m_pJointRotations[i]);
   }
   // Precalc target quats
   m_pTargetQuats = new CQuaternion[m_iNumJoints];
   for (i=0; i<m_iNumJoints; i++) {
      m_pTargetQuats[i] = CQuaternion(poTarget.m_pJointRotations[i]);
   }
   // Done
   return 1;
} //InitInterpolation(const CAvatarPose& poTarget) const

CAvatarPose CAvatarPose::CalcInterpolationFrame(double dAmount) const {
   // Create result storage
   CAvatarPose poResult(m_iNumJoints);
   // Use the pointer-based version of this function
   CalcInterpolationFrame(dAmount,&poResult);
   // Done!
   return poResult;
} //CalcInterpolationFrame(double dAmount) const

void CAvatarPose::CalcInterpolationFrame(double dAmount, CAvatarPose* poResult) const {
   // Make sure we're ready to go
   if ((m_pSourceQuats == NULL) || (m_pTargetQuats == NULL) || (poResult == NULL)) return;
   // If we're accelerating, account for it
   if (m_bAccel) dAmount = AddAcceleration(dAmount);
   // Work out inverse of amount
   double dCoeff = 1.0 - dAmount;
   //  Interpolate translation
   poResult->m_vecRootTranslation = (m_vecRootTranslation * dCoeff) + (*m_pTargetTranslation * dAmount);
   // Interpolate joint rotations
   for (int i=0; i<m_iNumJoints; i++) {
      CQuaternion qResult = m_pSourceQuats[i].SlerpTo(m_pTargetQuats[i],dAmount);
      poResult->m_pJointRotations[i] = CAxisRotation(qResult);
   }
   // Done!
   return;
} //CalcInterpolationFrame(double dAmount, CAvatarPose* pResult) const

void CAvatarPose::EndInterpolation(void) const {
   // Dump and reset all stored data
   if (m_pSourceQuats != NULL) {
      delete [] m_pSourceQuats;
      m_pSourceQuats = NULL;
   }
   if (m_pTargetQuats != NULL) {
      delete [] m_pTargetQuats;
      m_pTargetQuats = NULL;
   }
   if (m_pTargetTranslation != NULL) {
      delete m_pTargetTranslation;
      m_pTargetTranslation = NULL;
   }
   // Done
   return;
} //EndInterpolation(void) const

CAvatarPose CAvatarPose::InterpolateToZero(double dAmount, bool bAccel) const {
   // If we're accelerating, account for it   
   if (bAccel) dAmount = AddAcceleration(dAmount);
   // Predeclare a double variable
   double dR;
   // Work out inverse of amount
   double dCoeff = 1.0 - dAmount;
   // Create result storage
   CAvatarPose poResult(m_iNumJoints);
   //  Interpolate translation
   poResult.m_vecRootTranslation = m_vecRootTranslation * dCoeff;
   // Interpolate joint rotations
   for (int i=0; i<m_iNumJoints; i++) {
      dR = m_pJointRotations[i].GetAngle();
      poResult.m_pJointRotations[i].SetAngle(dCoeff*dR);
   }
   // Done!
   return poResult;
} //InterpolateToZero(double dAmount) const

double CAvatarPose::AddAcceleration(double dAmount) const {
	// Simple cosine-based acceleration. This may be a bit much - we shall see...
	return 0.5 - cos(V_PI*dAmount)/2;
} //AddAcceleration(double dAmount) const

//////////////////////////////////////////////////////////////////////
// Loading/Saving
//////////////////////////////////////////////////////////////////////

// Load functions return 1 if successful, 0 if not

int CAvatarPose::Load(const char* pszFilename) {
	int iRetVal = 0;
   ifstream isInputStream(pszFilename);
	if (!isInputStream.fail()) {
		iRetVal = Load(isInputStream);
	}
	isInputStream.close();
	return iRetVal;
} //Load(const char* pszFilename)

bool CAvatarPose::Load(const char *pcFilename, CWedgie &oWedgie) {
	// Find the file in the wedgie, return if failed
	int iHandle = oWedgie.Open(pcFilename);
	if (iHandle == -1)
		return false;
	// Load the pose
   unsigned char pcBuffer[3072];
	unsigned char *pcTemp = pcBuffer + 4;
   oWedgie.Read(iHandle, pcBuffer, 3072);
   if ((pcBuffer[0] == 'V') && (pcBuffer[1] == 'P') && (pcBuffer[2] == 'O')) {
      // Test version number
      long int iNewNumJoints = 0;
      double dX = 0;
      double dY = 0;
      double dZ = 0;
      double dR = 0;
      long int i = 0;
      switch (pcBuffer[3]) {
         case 1:
            // Read number of joints
            iNewNumJoints = *((long int*)pcTemp);
				pcTemp += 4;
            // Allocate memory if necessary
            if (iNewNumJoints != m_iNumJoints) {
               m_iNumJoints = iNewNumJoints;
               if (m_pJointRotations != NULL) delete [] m_pJointRotations;
               m_pJointRotations = new CAxisRotation[m_iNumJoints];
            }
            // Read root translation
            dX = *((double*)pcTemp);
				pcTemp += 8;
            dY = *((double*)pcBuffer);
				pcTemp += 8;
            dZ = *((double*)pcBuffer);
				pcTemp += 8;
            m_vecRootTranslation.FromDouble(dX,dY,dZ);
            // Read joint rotations
            for (i=0; i<m_iNumJoints; i++) {
               dX = *((double*)pcTemp);
					pcTemp += 8;
               dY = *((double*)pcTemp);
					pcTemp += 8;
               dZ = *((double*)pcTemp);
					pcTemp += 8;
               dR = *((double*)pcTemp);
					pcTemp += 8;
               m_pJointRotations[i].FromDouble(dX,dY,dZ,dR);
            }
            break;
			default:
				ASSERT(false);
      }
   }
	// Close the wedgie
	oWedgie.Close(iHandle);
	// Return ok
	return true;
} // Load (from Wedgie)

int CAvatarPose::Load(ifstream& isInputStream) {
	int iRetVal = 0;
   unsigned char buffer[8];
   isInputStream.read(buffer,4);
   if ((buffer[0] == 'V') && (buffer[1] == 'P') && (buffer[2] == 'O')) {
      // Test version number
      long int iNewNumJoints = 0;
      double dX = 0;
      double dY = 0;
      double dZ = 0;
      double dR = 0;
      long int i = 0;
      switch (buffer[3]) {
         case 1:
            // Read number of joints
            isInputStream.read(buffer,4);
            iNewNumJoints = *((long int*)buffer);
            // Allocate memory if necessary
            if (iNewNumJoints != m_iNumJoints) {
               m_iNumJoints = iNewNumJoints;
               if (m_pJointRotations != NULL) delete [] m_pJointRotations;
               m_pJointRotations = new CAxisRotation[m_iNumJoints];
            }
            // Read root translation
            isInputStream.read(buffer,8);
            dX = *((double*)buffer);
            isInputStream.read(buffer,8);
            dY = *((double*)buffer);
            isInputStream.read(buffer,8);
            dZ = *((double*)buffer);
            m_vecRootTranslation.FromDouble(dX,dY,dZ);
            // Read joint rotations
            for (i=0; i<m_iNumJoints; i++) {
               isInputStream.read(buffer,8);
               dX = *((double*)buffer);
               isInputStream.read(buffer,8);
               dY = *((double*)buffer);
               isInputStream.read(buffer,8);
               dZ = *((double*)buffer);
               isInputStream.read(buffer,8);
               dR = *((double*)buffer);
               m_pJointRotations[i].FromDouble(dX,dY,dZ,dR);
            }
            break;
			default:
				ASSERT(false);
      }
   }
	return iRetVal;
} //Load(ifstream& isInputStream)
	
// Save functions return 1 if successful, 0 if not

int CAvatarPose::Save(const char* pszFilename) const {
	int iRetVal = 0;
   ofstream osOutputStream(pszFilename);
	if (osOutputStream.good()) {
		iRetVal = Save(osOutputStream);
	}
	osOutputStream.close();
	return iRetVal;
} //Save(const char* pszFilename)

int CAvatarPose::Save(ofstream& osOutputStream) const {
	int iRetVal = 1;
   // Set binary stream mode
   int iOldMode = osOutputStream.setmode(filebuf::binary);
   // Header
   osOutputStream.write("VPO",3);
   // Version number
   osOutputStream << (unsigned char)1;
   // Number of joints
   osOutputStream.write((const char*)(&m_iNumJoints),sizeof(long unsigned int));
   // Translation
   double dx, dy, dz, dr;
   m_vecRootTranslation.ToDouble(dx,dy,dz);
   osOutputStream.write((char*)(&dx),sizeof(double));
   osOutputStream.write((char*)(&dy),sizeof(double));
   osOutputStream.write((char*)(&dz),sizeof(double));
   // Rotations
   for (long int i=0; i<m_iNumJoints; i++) {
      m_pJointRotations[i].ToDouble(dx,dy,dz,dr);
      osOutputStream.write((char*)(&dx),sizeof(double));
      osOutputStream.write((char*)(&dy),sizeof(double));
      osOutputStream.write((char*)(&dz),sizeof(double));
      osOutputStream.write((char*)(&dr),sizeof(double));
   }
   if (osOutputStream.bad()) iRetVal = 0;
   // Restore old stream mode
   if (iOldMode != -1) osOutputStream.setmode(iOldMode);
   // Return
	return iRetVal;
} //Save(ofstream& osOutputStream)