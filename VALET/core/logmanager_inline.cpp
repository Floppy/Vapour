//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_LOGMANAGER_INLINE_
#define _VALET_LOGMANAGER_INLINE_

////////////////
//! file 		= "VALET/core/logmanager_inline.h"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: logmanager_inline.cpp,v 1.1 2001/09/26 13:10:11 vap-warren Exp $"

//#===--- CLogManager Inline Functions

namespace VALET {

	INLINE bool CLogManager::Active() {
		return m_bActive;
	} // CLogManager::Active

	INLINE int CLogManager::Active(const char *pcType) {
		ASSERT(pcType);
		if (!m_bActive)
			return -1;
		return CheckType(pcType);
	} // CLogManager::Active(type string)

}

#endif // _VALET_LOGMANAGER_INLINE_
