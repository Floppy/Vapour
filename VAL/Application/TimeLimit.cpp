//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// TimeLimit.cpp - 07/06/2000 - Warren Moore
//	Time limit validation
//
// $Id: TimeLimit.cpp,v 1.5 2000/08/01 23:07:58 waz Exp $
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

	bool bValid = (uYear == 2000) && (uMonth == 8) && (uDay >= 1) && (uDay < 15);

	return bValid;
} // Valid
