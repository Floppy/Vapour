//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// BaseFilter.h - 29/03/2000 - Warren Moore
//	Base filter class declartion
//
// $Id: BaseFilter.h,v 1.1 2000/06/16 22:00:16 waz Exp $
//

#ifndef _BASEFILTER_
#define _BASEFILTER_

#pragma once

////////////////
// CBaseFilter

class CBaseFilter {
//#===--- Functions
public:
	CBaseFilter(double dWidth = 0) : m_dWidth(dWidth) {};
	virtual ~CBaseFilter() {};

	void Width(double dWidth);
	double Width() const;

	virtual double Filter(double dVal) = 0;

//#===--- Internal Data
protected:
	double m_dWidth;
};

//#===--- Inline Functions

inline void CBaseFilter::Width(double dWidth) {
	m_dWidth = dWidth;
} // Width (Get)

inline double CBaseFilter::Width() const {
	return m_dWidth;
} // Width (Get)

#endif // _BASEFILTER_
