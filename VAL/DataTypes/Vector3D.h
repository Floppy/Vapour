//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Vector3d.h - 18/05/2000 - James Smith
//	3D vector class header
//
// $Id: Vector3D.h,v 1.4 2000/07/31 17:40:21 waz Exp $
//

#ifndef _VAL_VECTOR3D_
#define _VAL_VECTOR3D_

#pragma once

// libVapour includes
#include "Point3D.h"

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

// Class declarations
class CAxisRotation;

class DLL_IMP_EXP CVector3D {

protected:

	double m_pdVector[3];

public:

	//Contruction/Destruction
	CVector3D();
	CVector3D(const SPoint3D & pPoint);
	CVector3D(const CVector3D & oVec);
	CVector3D(const double & dX, const double & dY, const double & dZ);
	virtual ~CVector3D();

	//Operator Functions
	CVector3D operator *(const double & dScalar) const;
	CVector3D operator *(const CVector3D & oVec) const;
	CVector3D& operator *=(const double & dScalar);
	CVector3D& operator *=(const CVector3D & oVec);
	CVector3D operator +(const SPoint3D & oPoint) const;
	CVector3D operator +(const CVector3D & oVec) const;
	CVector3D& operator +=(const CVector3D & oVec);
	CVector3D operator -(const CVector3D &oVec) const;
	CVector3D& operator -=(const CVector3D & oVec);
	CVector3D operator -(void) const;
	CVector3D operator /(const double & dScalar) const;
	CVector3D operator /(const CVector3D & oVec) const;
	CVector3D& operator /=(const double & dScalar);
	CVector3D& operator /=(const CVector3D & oVec);
	bool operator ==(const CVector3D & oVec) const;
	bool operator !=(const CVector3D & oVec) const;

   //Geometric Functions
	double Dot(const CVector3D & oVec) const;
	CVector3D Cross(const CVector3D & oVec) const;
	double Length() const;
	CVector3D Normalise() const;
	CVector3D Rotate(const CAxisRotation & oRot) const;

	//Input Functions
	void FromDouble(double dX, double dY, double dZ);
	bool ParseString(const char* strInput, int* used = NULL);

	//Output Functions
	char* ToString(int precision) const;
	void ToPoint3D(SPoint3D& sPoint) const;
	void ToDouble(double &dX, double &dY, double &dZ) const;
   double X(void) const;
   double Y(void) const;
   double Z(void) const;
};

#endif // _VAL_VECTOR3D_
