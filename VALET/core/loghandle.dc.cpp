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
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.1 2001/09/26 13:10:10 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogHandle

namespace VALET {

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
