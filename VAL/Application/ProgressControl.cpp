//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// ProgressControl.h - 01/06/2000 - Warren Moore
//	Class for the callback of external progress updates functions
//
// $Id: ProgressControl.cpp,v 1.1 2000/06/16 21:57:46 waz Exp $
//

// Pre-compiled header include
#include "stdafx.h"

#include <iostream.h>
#include "ProgressControl.h"

//#===--- Defines

//#===--- Data types

/////////////////////
// CProgressControl

CProgressControl::CProgressControl() {
} // Constructor

CProgressControl::~CProgressControl() {
// Delete the allocate strings
	VecProgress::iterator pControl = m_oProgressTable.begin();
	while (pControl != m_oProgressTable.end()) {
		if (pControl->pcName)
			delete [] pControl->pcName;
		pControl++;
	}
// Clear the vector table
	m_oProgressTable.clear();
} // Destructor

//#===--- Client functions

void CProgressControl::SetTextFunction(const char *pcName, TEXT_FUNC pText) {
	if (pcName) {
	// Check it doesn't already exist
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
	// Add it if not found
		if (!bFound) {
		// Create the string
			int iLength = strlen(pcName) + 1;
			SProgressControl sControl;
			sControl.pcName = (char*) new char[iLength];
			if (sControl.pcName) {
				strcpy(sControl.pcName, pcName);
			}
			sControl.pText = pText;
			m_oProgressTable.push_back(sControl);
		}
	// Otherwise change the function pointer
		else {
			pControl->pText = pText;
		}
	}
} // SetText

void CProgressControl::SetProgressFunction(const char *pcName, HANDLER_FUNC pHandler) {
	if (pcName) {
	// Check it doesn't already exist
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
	// Add it if not found
		if (!bFound) {
		// Create the string
			int iLength = strlen(pcName) + 1;
			SProgressControl sControl;
			sControl.pcName = (char*) new char[iLength];
			if (sControl.pcName) {
				strcpy(sControl.pcName, pcName);
			}
			sControl.pHandler = pHandler;
			m_oProgressTable.push_back(sControl);
		}
	// Otherwise change the function pointer
		else {
			pControl->pHandler = pHandler;
		}
	}
} // SetHandler

//#===--- Server functions
// Text output
void CProgressControl::SetText(const char *pcName, const char *pcText) {
	if (pcName) {
	// Make sure the handler exists
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
		if (bFound && pControl->pText) {
			pControl->pText(pcText);
		}
	}
} // SetText

// Progress output
void CProgressControl::SetMaxProgress(const char *pcName, unsigned int uMax) {
	if (pcName) {
	// Make sure the handler exists
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
		if (bFound) {
			pControl->uMax = uMax;
			pControl->uCount = 0;
		}
	}
} // SetMaxProgress

void CProgressControl::SetProgress(const char *pcName, unsigned int uPercent) {
	if (pcName) {
	// Make sure the handler exists
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
		if (bFound && pControl->pHandler) {
		// Set the current count
			pControl->uCount = (uPercent > pControl->uMax) ? pControl->uMax : uPercent;
		// Calculate the percentage
			double dProgress = double(pControl->uCount) / double(pControl->uMax);
			pControl->pHandler(dProgress);
		}
	}
} // SetProgress

void CProgressControl::Step(const char *pcName) {
	if (pcName) {
	// Make sure the handler exists
		bool bFound = false;
		VecProgress::iterator pControl = m_oProgressTable.begin();
		while ((!bFound) && (pControl != m_oProgressTable.end())) {
			bFound = strcmp(pControl->pcName, pcName) == 0;
			if (!bFound)
				pControl++;
		}
		if (bFound && pControl->pHandler) {
		// Set the current count
			unsigned int uCount = pControl->uCount + 1;
			pControl->uCount = (uCount > pControl->uMax) ? pControl->uMax : uCount;
		// Calculate the percentage
			double dProgress = double(pControl->uCount) / double(pControl->uMax);
			pControl->pHandler(dProgress);
		}
	}
} // Step

