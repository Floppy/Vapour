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
//! rcsid 		= "$Id: log_inline.cpp,v 1.4 2001/10/24 14:26:26 vap-warren Exp $"

//#===--- CLog Inline Functions

namespace NValet {

#ifdef VDEBUG

	INLINE void CLog::Trace(const char *pcMessage, int iLevel) {
		ASSERT(pcMessage);
		// If we have an id, trace the message
		if (m_iID >= 0)
			g_oLogManager.Trace(m_iID, m_pcFunction, pcMessage, iLevel);
	} // CLog::Trace

#else

   INLINE CLog::CLog(const char* pcType, const char* pcFunction, int iLevel) {}

   INLINE CLog::~CLog() {}

   INLINE void CLog::Trace(const char *pcMessage, int iLevel) {}   

#endif // VDEBUG

}

#endif // _VALET_LOG_INLINE_
