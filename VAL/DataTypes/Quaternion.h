//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Quaternion.h - 12/04/2000 - James Smith
//	Quaternion class header
//
// $Id: Quaternion.h,v 1.3 2000/11/21 16:38:53 waz Exp $
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

class DLL CQuaternion {

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
