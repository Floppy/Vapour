//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// VALWin32.cpp - 03/07/2000 - Warren Moore
//	VAL management object for Windows
//
// $Id: VALWin32.cpp,v 1.1 2000/07/10 21:40:24 waz Exp $
//

#include "StdAfx.h"

#include "VALWin32.h"

#include <string.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////
// CVALWin32

CVALWin32::CVALWin32(){
	// Set the app strings initially empty
	m_pcAppDir = NULL;
	m_pcAppName = NULL;
	// Get the app name
	char pcAppName[STR_SIZE] = "";
	if (GetModuleFileName(NULL, pcAppName, STR_SIZE) != 0) {
		// Allocate the app name string
		NEWBEGIN
		m_pcAppName = (char*) new char[strlen(pcAppName) + 1];
		NEWEND("CVALWin32::CVALWin32 - App name string")
		if (m_pcAppName) {
			// Set the string and strip the directory off
			strcpy(m_pcAppName, pcAppName);
			int iCount = strlen(m_pcAppName);
			while ((iCount > 0) && (m_pcAppName[iCount - 1] != '\\'))
				iCount--;
			if (iCount) {
				// Allocate the dir string
				NEWBEGIN
				m_pcAppDir = (char*) new char[iCount + 1];
				NEWEND("CVALWin32::CVALWin32 - App directory string")
				// Copy the directory string out
				if (m_pcAppDir) {
					memcpy(m_pcAppDir, m_pcAppName, iCount);
					m_pcAppDir[iCount] = '\0';
				}
			}
		}
	}
} // Constructor

CVALWin32::~CVALWin32() {
	if (m_pcAppName)
		delete [] m_pcAppName;
	if (m_pcAppDir)
		delete [] m_pcAppDir;
} // Destructor

const char *CVALWin32::GetAppDir() {
	return m_pcAppDir;
} // GetAppDir

const char *CVALWin32::GetAppName() {
	return m_pcAppName;
} // GetAppName

