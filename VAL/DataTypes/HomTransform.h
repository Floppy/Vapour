//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HomTransform.h - 12/04/2000 - James Smith
//	Homogeneous transformation matrix header
//
// $Id: HomTransform.h,v 1.1 2000/06/16 21:59:18 waz Exp $
//

#ifndef _VAPOUR_HOMTRANSFORM_
#define _VAPOUR_HOMTRANSFORM_

#pragma once

// Class declarations
class CHomTransform;

// libVapour includes
#include "Point3D.h"
#include "Vector3D.h"
#include "AxisRotation.h"
#include "Quaternion.h"

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

class DLL_IMP_EXP CHomTransform {

public:

	// Construction & Destruction functions ///////////////////////////////

	//Default constructor - creates an identity transform
	CHomTransform();

	//Constructor - creates a transform equivalent to a translation
	CHomTransform(const CVector3D& vecTranslation);

   //Constructor - creates a transform from a Quaternion
	CHomTransform(const CQuaternion& oQuat);

   //Constructor - creates a transform equivalent to a rotation about an arbitrary centre
	CHomTransform(const CQuaternion& oQuat, const SPoint3D& pntCentre);

   //Constructor - creates a transform from an Axis rotation
	CHomTransform(const CAxisRotation& rotRotation);
	
   //Constructor - creates a transform equivalent to a rotation about an arbitrary centre.
	CHomTransform(const CAxisRotation& rotRotation, const SPoint3D& pntCentre);

	//Copy constructor
	CHomTransform(const CHomTransform& htTransform);

	// Operator functions /////////////////////////////////////////////////

	//Assignment
	CHomTransform& operator=(const CHomTransform& htTransform);

	//Multiplies two transforms
	CHomTransform operator*(const CHomTransform& htTransform) const;

	//Multiplies a point by the transform
	SPoint3D operator*(const SPoint3D& pntPoint) const;

	// Access functions ///////////////////////////////////////////////////

	//Gets a single element from the transform
	double Item(unsigned iRow, unsigned iCol) const {return m_dMatrix[iRow*4 + iCol];}

protected:

	// The homogeneous transform matrix
	double m_dMatrix[12];

};

#endif // _VAPOUR_HOMTRANSFORM_
