//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Quaternion.h - 12/04/2000 - James Smith
//	Quaternion class header
//
// $Id: Quaternion.h,v 1.2 2000/06/17 10:42:13 waz Exp $
//

#ifndef _VAL_QUATERNION_
#define _VAL_QUATERNION_

#pragma once

class CQuaternion;

// libVapour includes
#include "Vector3D.h"
#include "AxisRotation.h"
#include "EulerRotation.h"
#include "HomTransform.h"

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

class DLL_IMP_EXP CQuaternion {

protected:

	// The scalar part of the quaternion
	double m_dScalar;

	// The vector part of the quaternion
	CVector3D m_oVector;

public:
	//Construction/Destruction
	CQuaternion();
	CQuaternion(const CQuaternion &oQuat);
	CQuaternion(const CAxisRotation &oRot);
	CQuaternion(const CEulerRotation &oRot);
	CQuaternion(const double &dScalar, const CVector3D & oVector);
	virtual ~CQuaternion();
	//Operator Functions
   CQuaternion& operator=(const CQuaternion& oQuat);
	CQuaternion operator*(const CQuaternion & oQuat) const;
	CQuaternion operator/(const double & dScalar) const;
   //Quaternion Functions
   CQuaternion SlerpTo(const CQuaternion& oQuat, double dAmount) const;
	//Geometric Functions
	CQuaternion Conjugate() const;
	double Norm() const;
	CQuaternion Inverse() const;
	//Output Functions
	double GetScalar(void) const;
	CVector3D GetVector(void) const;
};

#endif // _VAL_QUATERNION_
