//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/loghandle.dc.cpp"
//! author 		= "Warren Moore"
//! date 		= "26/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.6 2001/10/24 21:31:39 vap-james Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>

//#===--- CLogHandle

namespace NVALET {

	CLogHandle::CLogHandle(const char *pcType) {
		ASSERT(pcType);
		// Store the log type
		strcpy(m_pcType, pcType);
	} // CLogHandle::CLogHandle

	CLogHandle::~CLogHandle() {
	} // CLogHandle::~CLogHandle

	void CLogHandle::Trace(int iLevel, const char *pcFunction, const char *pcMessage) {
		ASSERT(iLevel >= 0);
		ASSERT(pcMessage);
		printf("(%s) %d : %s - %s\n", m_pcType, iLevel, pcFunction, pcMessage);
	} // CLogHandle::Trace
	
}
