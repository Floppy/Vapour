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
//! rcsid 		= "$Id: loghandle.dc.cpp,v 1.3 2001/10/17 00:22:47 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include "stdio.h"

//#===--- CLogHandle

namespace NValet {

	CLogHandle::CLogHandle(const char *pcType) {
		ASSERT(pcType);
	} // CLogHandle::CLogHandle

	CLogHandle::~CLogHandle() {
	} // CLogHandle::~CLogHandle

	void CLogHandle::Trace(int iLevel, const char *pcMessage) {
		ASSERT(iLevel >= 0);
		ASSERT(pcMessage);
		printf("%d : %s\n", iLevel, pcMessage);
	} // CLogHandle::Trace
	
}
