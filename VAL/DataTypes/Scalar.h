//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scalar.h - 10/08/2000 - James Smith
//	Scalar value class header
//
// $Id: Scalar.h,v 1.1 2000/11/21 16:39:16 waz Exp $
//

#ifndef _VAL_SCALAR_
#define _VAL_SCALAR_

#pragma once

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

class DLL CScalar {
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
