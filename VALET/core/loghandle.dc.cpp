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
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.2 2001/10/09 20:44:29 vap-james Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogHandle

namespace NValet {

	CLogHandle::CLogHandle(const char *pcType) : 
		m_poLogFile(NULL) {
		ASSERT(pcType);
	} // CLogHandle::CLogHandle

	CLogHandle::~CLogHandle() {
	} // CLogHandle::~CLogHandle

	void CLogHandle::Trace(int iLevel, const char *pcMessage) {
		ASSERT(iLevel >= 0);
		ASSERT(pcMessage);
	} // CLogHandle::Trace
	
}
