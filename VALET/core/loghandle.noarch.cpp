//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/loghandle.noarch.cpp"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.noarch.cpp,v 1.1 2001/09/26 13:10:10 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogHandle

namespace VALET {

	CLogHandle::CLogHandle(const char *pcType) : 
		m_poLogFile(NULL) {
		ASSERT(pcType);
		// Create the log filename
		char pcName[STR_LENGTH] = "";
		strcpy(pcName, pcType);
		strcat(pcName, ".log");
		// Create the log file
		m_poLogFile = new ofstream(pcName, ios::out|ios::app);
		// TODO: Add time and date
		if (m_poLogFile) {
			*m_poLogFile << "---" << endl;
		}
	} // CLogHandle::CLogHandle

	CLogHandle::~CLogHandle() {
		// Delete the log file
		if (m_poLogFile) {
			m_poLogFile->close();
			delete m_poLogFile;
		}
	} // CLogHandle::~CLogHandle

	void CLogHandle::Trace(int iLevel, const char *pcMessage) {
		ASSERT(iLevel >= 0);
		ASSERT(pcMessage);
		if (!m_poLogFile)
			return;
		*m_poLogFile << iLevel << " : " << pcMessage << endl;
	} // CLogHandle::Trace
	
}
