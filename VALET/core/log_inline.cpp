//#========---
//#=- VALET
//#=====---
//Vapour Technology All-Purpose Library
//Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_LOG_INLINE_
#define _VALET_LOG_INLINE_

////////////////
//! file 		= "VALET/core/log_inline.cpp"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: log_inline.cpp,v 1.2 2001/10/09 20:44:30 vap-james Exp $"

//#===--- CLog Inline Functions

namespace NValet {
	
	INLINE void CLog::Trace(const char *pcMessage, int iLevel) {
		ASSERT(pcMessage);
		// If we have an id, trace the message
		if (m_iID >= 0)
			g_oLogManager.Trace(m_iID, pcMessage, iLevel);
	} // CLog::Trace

}

#endif // _VALET_LOG_INLINE_
