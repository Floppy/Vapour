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
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.4 2001/10/17 00:46:00 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>

//#===--- CLogHandle

namespace NValet {

	CLogHandle::CLogHandle(const char *pcType) {
		ASSERT(pcType);
		// Store the log type
		strcpy(m_pcType, pcType);
	} // CLogHandle::CLogHandle

	CLogHandle::~CLogHandle() {
	} // CLogHandle::~CLogHandle

	void CLogHandle::Trace(int iLevel, const char *pcMessage) {
		ASSERT(iLevel >= 0);
		ASSERT(pcMessage);
		printf("(%s) %d : %s\n", m_pcType, iLevel, pcMessage);
	} // CLogHandle::Trace
	
}
