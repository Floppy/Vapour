//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.cpp - 07/06/2000 - Warren Moore
//	Time limit validation
//
// $Id: TimeLimit.cpp,v 1.6 2000/08/29 13:49:45 waz Exp $
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

CTimeLimit::CTimeLimit() {
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

	bool bValid = (uYear == 2000);
	if (uMonth == 8)
		bValid &= (uDay >= 28);
	if (uMonth == 9)
		bValid &= (uDay <= 12);

	return bValid;
} // Valid
