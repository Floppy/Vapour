//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scalar.h - 10/08/2000 - James Smith
//	Scalar value class header
//
// $Id: Scalar.h,v 1.0 2000/08/18 12:03:32 waz Exp $
//

#ifndef _VAL_SCALAR_
#define _VAL_SCALAR_

#pragma once

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

class DLL_IMP_EXP CScalar {
protected:
	double m_dScalar;
public:
	//Contruction/Destruction
	CScalar();
	CScalar(const CScalar & oScl);
	CScalar(const double & dScl);
	virtual ~CScalar();
	//Operator Functions
	operator double() const;
	operator =(const double dScl);
	//Input Functions
	bool ParseString(const char* strInput, int* used = NULL);
	//Output Functions
	char* ToString(int precision) const;
};

#endif // _VAL_SCALAR_
