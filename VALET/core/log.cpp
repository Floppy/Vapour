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
//! rcsid		= "$Id: log.cpp,v 1.4 2001/10/21 14:38:32 vap-warren Exp $"

//#===--- Includes
#include "log.h"

namespace NValet {
	
   //#===--- CLog

	CLog::CLog(const char* pcType, const char* pcFunction, int iLevel) :
		m_iID(-1),
      m_iLevel(iLevel)
   {
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
		g_oLogManager.Trace(m_iID, m_pcFunction, "Entering", m_iLevel);
	} // CLog::CLog

	CLog::~CLog() {
		// Return if we don't have an ID
		if (m_iID < 0)
			return;
		// Trace leaving the function
		g_oLogManager.Trace(m_iID, m_pcFunction, "Leaving", m_iLevel);
		// Delete allocated strings
		if (m_pcFunction)
			delete [] m_pcFunction;
	} // CLog::CLog
	
}

//#===--- Inline Functions (only included if inlines switched off)

#ifndef VALET_INLINE
#include "log_inline.cpp"
#endif // VALET_INLINE
