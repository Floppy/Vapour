//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/log.cpp"
//! author		= "Warren Moore"
//! date			= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid		= "$Id: log.cpp,v 1.3 2001/10/17 22:00:54 vap-warren Exp $"

//#===--- Includes
#include "log.h"

namespace NValet {
	
   //#===--- CLog

	CLog::CLog(const char* pcType, const char* pcFunction) :
		m_iID(-1) {
		ASSERT(pcType);
		ASSERT(pcFunction);
		// Skip straight out if not logging
		if (!g_oLogManager.Active())
			return;
		// Get the log ID
		m_iID = g_oLogManager.Active(pcType);
		if (m_iID < 0)
			return;
		// Store the function name
		int iLength = strlen(pcFunction);
		iLength++;
		m_pcFunction = (char*) new char[iLength];
		if (m_pcFunction) {
			strcpy(m_pcFunction, pcFunction);
		}
		else {
			m_iID = -1;
		}
		// Trace the function name
		char pcMessage[STR_LENGTH] = "";
		strcpy(pcMessage, "Entering function ");
		strcat(pcMessage, m_pcFunction);
		g_oLogManager.Trace(m_iID, pcMessage);
	} // CLog::CLog

	CLog::~CLog() {
		// Return if we don't have an ID
		if (m_iID < 0)
			return;
		// Trace leaving the function
		char pcMessage[STR_LENGTH] = "";
		strcpy(pcMessage, "Leaving function ");
		strcat(pcMessage, m_pcFunction);
		g_oLogManager.Trace(m_iID, pcMessage);
		// Delete allocated strings
		if (m_pcFunction)
			delete [] m_pcFunction;
	} // CLog::CLog
	
}

//#===--- Inline Functions (only included if inlines switched off)

#ifndef VALET_INLINE
#include "log_inline.cpp"
#endif // VALET_INLINE
