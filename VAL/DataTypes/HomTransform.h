//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// HomTransform.h - 12/04/2000 - James Smith
//	Homogeneous transformation matrix header
//
// $Id: HomTransform.h,v 1.3 2000/11/21 16:38:38 waz Exp $
//

#ifndef _VAL_HOMTRANSFORM_
#define _VAL_HOMTRANSFORM_

#pragma once

// Class declarations
class CHomTransform;

// libVapour includes
#include "Point3D.h"
#include "Vector3D.h"
#include "AxisRotation.h"
#include "Quaternion.h"

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

class DLL CHomTransform {

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

#endif // _VAL_HOMTRANSFORM_
