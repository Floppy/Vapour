//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.cpp - 07/06/2000 - Warren Moore
//	Time limit validation
//
// $Id: TimeLimit.cpp,v 1.2 2000/07/11 17:41:15 waz Exp $
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

	bool bValid = (uYear == 2000) && (uMonth == 7) && (uDay > 10) && (uDay < 25);

	return bValid;
} // Valid
