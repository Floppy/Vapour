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
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.7 2001/10/27 11:26:57 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"
#include "logmanager.h"

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
		printf("(%s) %s : %s - %s\n", m_pcType, g_pcLogLevel[iLevel], pcFunction, pcMessage);
	} // CLogHandle::Trace
	
}
