//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarPose.h - 21/2/2000 - James Smith
//	Avatar pose class header
//
// $Id: AvatarPose.h,v 1.8 2000/11/22 00:44:11 waz Exp $
//

#ifndef _VAL_AVATARPOSE_
#define _VAL_AVATARPOSE_

#pragma once

class CAvatarPose;

// standard includes
#include <fstream.h>

//libVapour includes
#include "AxisRotation.h"
#include "Vector3D.h"
#include "Wedgie.h"

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

//////////////////////////////////////////////////////////////////
// CAvatarPose
//////////////////////////////////////////////////////////////////

class DLL CAvatarPose {

public:

   // Copy Constructor
   CAvatarPose(const CAvatarPose& apOtherPose);

   // Creates a default pose
   CAvatarPose(int iNumJoints = 0);

	// Loads a pose directly from a file
   CAvatarPose(const char* pszFilename);

	// Loads a pose directly from a file
   CAvatarPose(const char* pszFilename, CWedgie &oWedgie);

	// Loads a pose directly from a stream
   CAvatarPose(ifstream& isInputStream);

   // Destructor
   virtual ~CAvatarPose();

   // Assignment Functions
   void SetRootTranslation(const CVector3D& vecTranslation);
   void SetJointRotation(int iJoint, const CAxisRotation& rotNewAngle);

   // Access Functions
	int GetNumJoints() const;
   CVector3D GetRootTranslation(void);
   CAxisRotation GetJointRotation(int iJoint);

   // Animation Functions
   // Initialises a pose interpolation
   int InitInterpolation(const CAvatarPose& poTarget, bool bAccel = false) const;
   // Calculates 1 frame of a pose interpolation and returns the result.
   CAvatarPose CalcInterpolationFrame(double dAmount) const;
   // Calculates 1 frame of a pose interpolation and stores the result in poResult.
   void CalcInterpolationFrame(double dAmount, CAvatarPose* poResult) const;
   // Cleans up after a pose interpolation
   void EndInterpolation(void) const;

   CAvatarPose InterpolateToZero(double dAmount, bool bAccel = false) const;
   
   // Load functions
	FRESULT Load(const char* pszFilename);
	FRESULT Load(const char *pcFilename, CWedgie &oWedgie);
	FRESULT Load(ifstream& isInputStream);

   // Save functions
	FRESULT Save(const char* pszFilename) const;
	FRESULT Save(ofstream& osOutputStream) const;
   
   // File type information functions
   static const char* GetExtension() {return ("vpo");}
   static const char* GetTitle() {return ("Vapourtech Pose");}

private:

   // Converts a linear 0..1 interpolation coefficient into one with acceleration
   double AddAcceleration(double dAmount) const;

protected:

   long int m_iNumJoints;

   CAxisRotation* m_pJointRotations;
   CVector3D m_vecRootTranslation;

   mutable CQuaternion* m_pSourceQuats;
   mutable CQuaternion* m_pTargetQuats;
   mutable CVector3D* m_pTargetTranslation;
   mutable bool m_bAccel;

   friend class CAvatar;

};

/////////////////////
// Inline Functions

inline CAvatarPose::GetNumJoints() const {
	return m_iNumJoints;
} // GetNumJoints

#endif //_VAL_AVATARPOSE_
