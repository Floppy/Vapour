//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// EulerRotation.cpp - 12/04/2000 - James Smith
//	Euler rotation class implementation
//
// $Id: EulerRotation.cpp,v 1.2 2000/08/21 23:07:50 waz Exp $
//

// Windows includes/defines
#include "stdafx.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Standard includes
#include <math.h>
#include <float.h>

// libVapour includes
#include "EulerRotation.h"
#include "Quaternion.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEulerRotation::CEulerRotation() {
	m_pAngles[0] = 0;
	m_pAngles[1] = 0;
	m_pAngles[2] = 0;
	m_uType.m_eIdentifier = XYZs;
} //CEulerRotation()

CEulerRotation::CEulerRotation(double dFirst, double dSecond, double dThird, UEulerType uType) {
	m_pAngles[0] = dFirst;
	m_pAngles[1] = dSecond;
	m_pAngles[2] = dThird;
	m_uType = uType;
} //CEulerRotation(double dFirst, double dSecond, double dThird, UEulerType uType)

CEulerRotation::CEulerRotation(const CHomTransform & htTransform, UEulerType uType) {
	m_uType = uType;
	int NextAngle[] = {Y_AXIS, Z_AXIS, X_AXIS, Y_AXIS};
	int i = uType.m_sProperties.m_ucInnerAxis;	
	int j = NextAngle[i + uType.m_sProperties.m_bOddParity];
	int k = NextAngle[i + 1 - uType.m_sProperties.m_bOddParity];	
	if (uType.m_sProperties.m_bRepetition) {
		double sy = sqrt(htTransform.Item(i,j)*htTransform.Item(i,j) + htTransform.Item(i,k)*htTransform.Item(i,k));
		if (sy > 16*FLT_EPSILON) {
			m_pAngles[0] = atan2(htTransform.Item(i,j),htTransform.Item(i,k));
			m_pAngles[1] = atan2(sy, htTransform.Item(i,i));
			m_pAngles[2] = atan2(htTransform.Item(j,i), -htTransform.Item(k,i));
		}
		else {
			m_pAngles[0] = atan2(-htTransform.Item(j,k), htTransform.Item(j,j));
			m_pAngles[1] = atan2(sy, htTransform.Item(i,i));
			m_pAngles[2] = 0;
		}
	}
	else {
		double cy = sqrt(htTransform.Item(i,i)*htTransform.Item(i,i) + htTransform.Item(j,i)*htTransform.Item(j,i));
		if (cy > 16*FLT_EPSILON) {
			m_pAngles[0] = atan2(htTransform.Item(k,j), htTransform.Item(k,k));
			m_pAngles[1] = atan2(-htTransform.Item(k,i), cy);
			m_pAngles[2] = atan2(htTransform.Item(j,i), htTransform.Item(i,i));
		}
		else {
			m_pAngles[0] = atan2(-htTransform.Item(j,k), htTransform.Item(j,j));
			m_pAngles[1] = atan2(-htTransform.Item(k,i), cy);
			m_pAngles[2] = 0;
		}
	}
	if (uType.m_sProperties.m_bOddParity) {
		m_pAngles[0] = -m_pAngles[0];
		m_pAngles[1] = -m_pAngles[1];
		m_pAngles[2] = -m_pAngles[2];
	}
	if (uType.m_sProperties.m_bRotatingFrame) {
		double dTemp = m_pAngles[0];
		m_pAngles[0] = m_pAngles[2];
		m_pAngles[2] = dTemp;
	}
} //CEulerRotation(const CHomTransform & htTransform, UEulerType uType)

CEulerRotation::CEulerRotation(const CQuaternion& oQuat, UEulerType uType) {
   CHomTransform htTransform(oQuat);
	m_uType = uType;
	int NextAngle[] = {Y_AXIS, Z_AXIS, X_AXIS, Y_AXIS};
	int i = uType.m_sProperties.m_ucInnerAxis;	
	int j = NextAngle[i + uType.m_sProperties.m_bOddParity];
	int k = NextAngle[i + 1 - uType.m_sProperties.m_bOddParity];	
	if (uType.m_sProperties.m_bRepetition) {
		double sy = sqrt(htTransform.Item(i,j)*htTransform.Item(i,j) + htTransform.Item(i,k)*htTransform.Item(i,k));
		if (sy > 16*FLT_EPSILON) {
			m_pAngles[0] = atan2(htTransform.Item(i,j),htTransform.Item(i,k));
			m_pAngles[1] = atan2(sy, htTransform.Item(i,i));
			m_pAngles[2] = atan2(htTransform.Item(j,i), -htTransform.Item(k,i));
		}
		else {
			m_pAngles[0] = atan2(-htTransform.Item(j,k), htTransform.Item(j,j));
			m_pAngles[1] = atan2(sy, htTransform.Item(i,i));
			m_pAngles[2] = 0;
		}
	}
	else {
		double cy = sqrt(htTransform.Item(i,i)*htTransform.Item(i,i) + htTransform.Item(j,i)*htTransform.Item(j,i));
		if (cy > 16*FLT_EPSILON) {
			m_pAngles[0] = atan2(htTransform.Item(k,j), htTransform.Item(k,k));
			m_pAngles[1] = atan2(-htTransform.Item(k,i), cy);
			m_pAngles[2] = atan2(htTransform.Item(j,i), htTransform.Item(i,i));
		}
		else {
			m_pAngles[0] = atan2(-htTransform.Item(j,k), htTransform.Item(j,j));
			m_pAngles[1] = atan2(-htTransform.Item(k,i), cy);
			m_pAngles[2] = 0;
		}
	}
	if (uType.m_sProperties.m_bOddParity) {
		m_pAngles[0] = -m_pAngles[0];
		m_pAngles[1] = -m_pAngles[1];
		m_pAngles[2] = -m_pAngles[2];
	}
	if (uType.m_sProperties.m_bRotatingFrame) {
		double dTemp = m_pAngles[0];
		m_pAngles[0] = m_pAngles[2];
		m_pAngles[2] = dTemp;
	}
} //CEulerRotation(const CQuaternion& oQuat, UEulerType uType)

CEulerRotation::~CEulerRotation() {
} //~CEulerRotation()

//////////////////////////////////////////////////////////////////////
// Utility Functions
//////////////////////////////////////////////////////////////////////

void CEulerRotation::Damp(double dDampFirst, double dDampSecond, double dDampThird) {
   m_pAngles[0] *= dDampFirst;
   m_pAngles[1] *= dDampSecond;
   m_pAngles[2] *= dDampThird;
   return;
} //Damp(double dDampFirst, double dDampSecond, double dDampThird)

bool CEulerRotation::Limit(double dMaxFirst, double dMaxSecond, double dMaxThird, double dMinFirst, double dMinSecond, double dMinThird) {
	bool bReturn = false;
	if (m_pAngles[0] > dMaxFirst) {
		m_pAngles[0] = dMaxFirst; 
		bReturn = true;
	}
	else if (m_pAngles[0] < dMinFirst) {
		m_pAngles[0] = dMinFirst;
		bReturn = true;
	}
	if (m_pAngles[1] > dMaxSecond) {
		m_pAngles[1] = dMaxSecond;
		bReturn = true;
	}
	else if (m_pAngles[1] < dMinSecond) {
		m_pAngles[1] = dMinSecond;
		bReturn = true;
	}
	if (m_pAngles[2] > dMaxThird) {
		m_pAngles[2] = dMaxThird;
		bReturn = true;
	}
	else if (m_pAngles[2] < dMinThird) {
		m_pAngles[2] = dMinThird;
		bReturn = true;
	}
	return bReturn;
} //Limit(double dMaxFirst, double dMaxSecond, double dMaxThird, double dMinFirst, double dMinSecond, double dMinThird)
