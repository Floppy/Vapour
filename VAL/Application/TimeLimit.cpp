//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.cpp - 07/06/2000 - Warren Moore
//	Time limit validation
//
// $Id: TimeLimit.cpp,v 1.7 2000/10/10 17:50:29 waz Exp $
//

#include "StdAfx.h"

#include "TimeLimit.h"

#include <iostream.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////
// CTimeLimit

unsigned int CTimeLimit::m_uDaysInMonth[12] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 
};

CTimeLimit::CTimeLimit(unsigned int uDay, unsigned int uMonth, unsigned int uYear, unsigned int uLimit) :
	m_uDay(uDay), m_uMonth(uMonth), m_uYear(uYear), m_uLimit(uLimit) {
	// Check a valid month has been specified
	ASSERT((m_uMonth >= 1) && (m_uMonth <= 12));
	// TODO: Check for leap year and change m_uDaysInMonth[2] to match

	// Check a valid day has been set
	ASSERT((m_uDay >= 1) && (m_uDay <= m_uDaysInMonth[m_uMonth - 1]));
} // Contructor

CTimeLimit::~CTimeLimit() {
} // Destructor

bool CTimeLimit::Valid() {
	// Get number of secs since epoch
	time_t iNumSecs;
	time(&iNumSecs);

	// Convert to useful form
	tm *pCurrentTime = gmtime(&iNumSecs);
	unsigned int uYear = pCurrentTime->tm_year + 1900;
	unsigned int uMonth = pCurrentTime->tm_mon + 1;
	unsigned int uDay = pCurrentTime->tm_mday;

	// Check a valid month has been retrieved
	ASSERT((uMonth >= 1) && (uMonth <= 12));
	// Check a valid day has been retrieved
	ASSERT((uDay >= 1) && (uDay <= m_uDaysInMonth[uMonth - 1]));

	// Calculate time limit date
	unsigned int uEndDay = m_uDay + m_uLimit;
	unsigned int uEndMonth = m_uMonth;
	unsigned int uEndYear = m_uYear;
	if (uEndDay > m_uDaysInMonth[m_uMonth - 1]) {
		uEndDay -= m_uDaysInMonth[m_uMonth - 1];
		uEndMonth++;
		if (uEndMonth > 12) {
			uEndMonth = 1;
			uEndYear++;
		}
	}

	// Set validity initially false
	bool bValid = false;
	// Find out if within valid dates
	if ((uYear == m_uYear) && (uMonth == m_uMonth) && (uDay >= m_uDay))
		bValid = true;
	else
		if ((uYear == uEndYear) && (uMonth == uEndMonth) && (uDay <= uEndDay))
			bValid = true;

	// Diagnostic output
	TRACE("CTimeLimit - Start date: %02d/%02d/%04d   End Date: %02d/%02d/%04d   Valid: %s\n",
		m_uDay, m_uMonth, m_uYear, uEndDay, uEndMonth, uEndYear, (bValid ? "Yes" : "No"));

	return bValid;
} // Valid
