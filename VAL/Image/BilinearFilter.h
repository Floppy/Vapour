//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// BilinearFilter.h - 29/03/2000 - Warren Moore
//	Bilinear filter class declartion
//
// $Id: BilinearFilter.h,v 1.2 2000/06/17 10:42:16 waz Exp $
//

#ifndef _VAL_BILINEARFILTER_
#define _VAL_BILINEARFILTER_

#pragma once

#include "BaseFilter.h"

////////////////////
// CBilinearFilter

class CBilinearFilter : public CBaseFilter {
//#===--- Functions
public:
	CBilinearFilter(double dWidth = 1.0) : CBaseFilter(dWidth) {};
	~CBilinearFilter() {};

	double Filter(double dVal);
};

//#===--- Inline Functions

inline double CBilinearFilter::Filter(double dVal) {
	dVal = fabs(dVal);
	return (dVal < m_dWidth ? m_dWidth - dVal : 0.0);
} // Filter

#endif // _VAL_BILINEARFILTER_

