//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.h - 07/06/2000 - Warren Moore
//	Time limit validation class 
//
// $Id: TimeLimit.h,v 1.4 2000/10/10 17:50:29 waz Exp $
//

#ifndef _VAL_TIMELIMIT_
#define _VAL_TIMELIMIT_

#pragma once

//#===--- Defines

//#===--- Data types

///////////////
// CTimeLimit

class CTimeLimit {
//#===--- External functions
public:

// Constructor/destructor
	// Set start time of time limit
	//		1 < uDay < m_uDaysInMonth[uMonth - 1]
	//		1 < uMonth < 12
	CTimeLimit(unsigned int uDay, unsigned int uMonth, unsigned int uYear, unsigned int uDayLimit);		
	~CTimeLimit();

	bool Valid();

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	static unsigned int m_uDaysInMonth[12];

	const unsigned int m_uDay;
	const unsigned int m_uMonth;
	const unsigned int m_uYear;
	const unsigned int m_uLimit;
};

#endif // _VAL_TIMELIMIT_
