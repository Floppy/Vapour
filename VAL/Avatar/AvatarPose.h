//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarPose.h - 21/2/2000 - James Smith
//	Avatar pose class header
//
// $Id: AvatarPose.h,v 1.2 2000/06/17 10:42:10 waz Exp $
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

// DLL import/export definitions
#ifndef DLL_IMP_EXP
	#ifdef _EXP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllexport)
	#endif
	#ifdef _IMP_VAPOUR_COMMON_DLL_
		#define DLL_IMP_EXP __declspec(dllimport)
	#endif
	#ifndef DLL_IMP_EXP
		#define DLL_IMP_EXP
	#endif
#endif

//////////////////////////////////////////////////////////////////
// CAvatarPose
//////////////////////////////////////////////////////////////////

class DLL_IMP_EXP CAvatarPose {

public:

	// Copy Constructor
   CAvatarPose(const CAvatarPose& apOtherPose);

   // Creates a default pose
   CAvatarPose(int iNumJoints);

	// Loads a pose directly from a file
   CAvatarPose(const char* pszFilename);

	// Loads a pose directly from a stream
   CAvatarPose(ifstream& isInputStream);

   // Destructor
   virtual ~CAvatarPose();

   // Assignment Functions
   void SetRootTranslation(CVector3D& vecTranslation);
   void SetJointRotation(int iJoint, CAxisRotation& rotNewAngle);

   // Access Functions
   CVector3D GetRootTranslation(void);
   CAxisRotation GetJointRotation(int iJoint);

   // Animation Functions
   int InitInterpolation(const CAvatarPose& poTarget, bool bAccel = false) const;
   CAvatarPose CalcInterpolationFrame(double dAmount) const;
   void EndInterpolation(void) const;
   //CAvatarPose InterpolateTo(const CAvatarPose& poTarget, double dAmount) const;
   CAvatarPose InterpolateToZero(double dAmount, bool bAccel = false) const;
   
   // Load functions return 1 if successful, 0 if not
	int Load(const char* pszFilename);
	int Load(ifstream& isInputStream);

   // Save functions return 1 if successful, 0 if not
	int Save(const char* pszFilename) const;
	int Save(ofstream& osOutputStream) const;
   
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

#endif //_VAL_AVATARPOSE_