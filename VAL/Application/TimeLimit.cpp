//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.cpp - 07/06/2000 - Warren Moore
//	Time limit validation
//
// $Id: TimeLimit.cpp,v 1.1 2000/06/16 21:57:48 waz Exp $
//

#include "StdAfx.h"

#include "TimeLimit.h"

#include <iostream.h>
#include <fstream.h>
#include "SGAToSims.h"
#include <time.h>

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

	bool bValid = (uYear == 2000) && (uMonth == 6) && (uDay > 6) && (uDay < 22);

	return bValid;
} // Valid
