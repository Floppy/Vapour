//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/logmanager.cpp"
//! author		= "Warren Moore"
//! date			= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid		= "$Id: logmanager.cpp,v 1.5 2001/10/24 21:31:39 vap-james Exp $"

//#===--- Includes
#include "logmanager.h"
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogManager

namespace NVALET {
	
	CLogManager::CLogManager(bool bLog) :
		m_bActive(bLog) {
		// Clear all the debug links
		register unsigned int ui = m_uiMaxLogs;
		while (ui--) {
			m_ppcType[ui] = NULL;
			m_pbActive[ui] = false;
			m_piLevel[ui] = 0;
			m_poHandle[ui] = NULL;
		}
	} // CLogManager::CLogManager

	CLogManager::~CLogManager() {
		// Clear all the allocated type strings
		register unsigned int ui = m_uiMaxLogs;
		while (ui--) {
			if (m_ppcType[ui]) {
				Close(ui);
			}
		}
	} // CLogManager::~CLogManager

	void CLogManager::SetLog(const char *pcType, bool bLog, int iLevel) {
		ASSERT(pcType);
		// Check we're logging at all
		if (!m_bActive)
			return;
		// Get the ID, or create one otherwise
		int iID = CheckType(pcType);
		if (iID < 0)
			return;
		// Store the log activity state
		m_pbActive[iID] = bLog;
		// Update the log level
		if (iLevel >= 0)
			m_piLevel[iID] = iLevel;
	} // CLogManager::SetLog

	int CLogManager::CheckType(const char *pcType) {
		ASSERT(pcType);
		// Loop through each entry
		register int i = m_uiMaxLogs;
		int iFound = -1, iFree = -1;
		while (i--) {
			// Check if we have an entry
			if (m_ppcType[i]) {
				// Check each type
				char const *pcCurrent = pcType;
				char const *pcCheck = m_ppcType[i];
				while (true) {
					// Stop if the chars are different
					if (*pcCurrent != *pcCheck)
						break;
					// Stop if we've reached the end of the strings
					if (*pcCurrent == 0) {
						iFound = i;
						break;
					}
					// Step to the next char
					pcCurrent++;
					pcCheck++;
				}
				// Stop checking if we've found one
				if (iFound >= 0)
					break;
			}
			// Store the current free entry
			else {
				iFree = i;
			}
		}
		// Try to add the new entry
		if ((iFound < 0) && (iFree >= 0))
			iFound = Open(pcType, iFree);
		// Return what we found
		return iFound;
	} // CLogManager::CheckType

	int CLogManager::Open(const char *pcType, int iID) {
		ASSERT(pcType);
		ASSERT((iID >= 0) && (iID < m_uiMaxLogs));
		ASSERT(m_ppcType[iID] == NULL);
		// Create the handle object
		m_poHandle[iID] = (CLogHandle*) new CLogHandle(pcType);
		if (!m_poHandle[iID]) {
			return -1;
		}
		// Find the length of the log type
		int iLength = strlen(pcType);
		iLength++;
		// Allocate the log string
		m_ppcType[iID] = (char*) new char[iLength];
		if (!m_ppcType[iID]) {
			delete m_poHandle[iID];
			m_poHandle[iID] = NULL;
			return -1;
		}
		// Copy it over
		strcpy(m_ppcType[iID], pcType);
		// Return the handle
		return iID;
	} // CLogManager::Open

	void CLogManager::Close(int iID) {
		ASSERT((iID >= 0) && (iID < m_uiMaxLogs));
		ASSERT(m_ppcType[iID] != NULL);
		ASSERT(m_poHandle[iID] != NULL);
		// Delete the allocate memory
		delete [] m_ppcType[iID];
		delete m_poHandle[iID];
		// Reset the vars
		m_ppcType[iID] = NULL;
		m_poHandle[iID] = NULL;
	} // CLogManager::Close

	void CLogManager::Trace(int iID, const char *pcFunction, const char *pcMessage, int iLevel) {
		ASSERT((iID >= 0) && (iID < m_uiMaxLogs));
		ASSERT(m_ppcType[iID]);
		ASSERT(m_poHandle[iID]);
		ASSERT(pcFunction);
		ASSERT(pcMessage);
		// Make sure logging is on
		if (!m_bActive)
			return;
		if (!m_pbActive[iID])
			return;
		if (iLevel >= m_piLevel[iID]) {
			m_poHandle[iID]->Trace(iLevel, pcFunction, pcMessage);
		}
	} // CLogManager::Trace

}

//#===--- Inline Functions (only included if inlines switched off)

#ifndef VALET_INLINE
#include "logmanager_inline.cpp"
#endif // VALET_INLINE
