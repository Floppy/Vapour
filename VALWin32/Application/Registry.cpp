//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Registry.cpp - 13/01/2000 - Warren Moore
//	  Utility class for registry key management
//
// $Id: Registry.cpp,v 1.3 2001/07/11 11:29:30 vap-james Exp $
//

#include "StdAfx.h"
#include "VALWin32.h"

#include "Registry.h"

//////////////
// CRegistry

CRegistry::CRegistry() :
	m_bOpen(false),
	m_bRead(true),
	m_hRoot(NULL),
	m_hKey(NULL) {
} // Constructor

CRegistry::~CRegistry() {
	if (m_bOpen)
		::RegCloseKey(m_hKey);
} // Destructor

bool CRegistry::OpenKey(KEYTYPE eType, const char *pcKey, bool bRead) {
	// Close the key if already open
	if (m_bOpen) {
		CloseKey();
	}

	m_bRead = bRead;
	//#===--- Open the key
	// Set registry root
	switch (eType) {
		case LOCAL_MACHINE:		m_hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:		m_hRoot = HKEY_CURRENT_USER; break;
		case CLASSES_ROOT:		m_hRoot = HKEY_CLASSES_ROOT; break;
	}

	// Open for reading
	if (m_bRead) {
		m_bOpen = (::RegOpenKeyEx(m_hRoot,
										  pcKey, 
										  0L, 
										  KEY_QUERY_VALUE,
										  &m_hKey) == ERROR_SUCCESS);
	}
	// Open for writing (create if doesn't exist)
	else {
		m_bOpen = (::RegCreateKeyEx(m_hRoot, 
											 pcKey, 
											 0L, 
											 NULL,
											 REG_OPTION_NON_VOLATILE,
											 KEY_WRITE,
											 NULL,
											 &m_hKey,
											 NULL) == ERROR_SUCCESS);
	}

	return m_bOpen;
} // OpenKey

void CRegistry::CloseKey() {
	// Check a key is open
	if (!m_bOpen)
		return;

	// Close the key
	::RegCloseKey(m_hKey);
	m_bOpen = false;
} // CloseKey

void CRegistry::ReadString(const char *pcSubKey, const char *pcDefault, char *const pcValue) {
	ASSERT(m_bRead /*&& pcSubKey*/ && pcValue);
	// commented out pcSubKey, because it IS allowed to be NULL for default values.

	if (!m_bOpen) {
		strcat(pcValue, pcDefault);
		return;
	}
	// Set data buffer
	DWORD uSize = STR_SIZE;
	char pcData[STR_SIZE] = "";
	// Read the key
	if (::RegQueryValueEx(m_hKey,
								 pcSubKey,
								 NULL,
								 NULL, 
								 (LPBYTE)pcData,
								 &uSize) == ERROR_SUCCESS) {
		strcat((char*)pcValue, pcData);
	}
	// else set the default
	else {
		if (pcDefault)
			strcat(pcValue, pcDefault);
		else
			pcValue[0] = '\0';
	}
} // ReadString

void CRegistry::WriteString(const char *pcSubKey, const char *pcString) {
	ASSERT(m_bOpen && (!m_bRead) /*&& pcSubKey*/ && pcString); 
	// commented out pcSubKey, because it IS allowed to be NULL for default values.

	// Key data
	DWORD uSize = strlen(pcString);
	// Write the value
	::RegSetValueEx(m_hKey,
						 pcSubKey,
						 NULL,
						 REG_SZ, 
						 (LPBYTE)pcString,
						 uSize);
} // WriteString

DWORD CRegistry::ReadInt(const char *pcSubKey, const DWORD uDefault) {
	ASSERT(m_bRead && pcSubKey);
	if (!m_bOpen)
		return uDefault;
	// Set data buffer
	DWORD uSize = sizeof(DWORD);
	DWORD uNum = 0;
	// Read the key
	if (::RegQueryValueEx(m_hKey,
								 pcSubKey,
								 NULL,
								 NULL, 
								 (LPBYTE)&uNum,
								 &uSize) != ERROR_SUCCESS) {
		uNum = uDefault;
	}

	return uNum;
} // ReadInt

void CRegistry::WriteInt(const char *pcSubKey, const DWORD uNum) {
	ASSERT(m_bOpen && (!m_bRead) && pcSubKey);
	// Key data
	DWORD uSize = sizeof(DWORD);
	// Write the value
	::RegSetValueEx(m_hKey,
						 pcSubKey,
						 NULL,
						 REG_DWORD, 
						 (LPBYTE)&uNum,
						 uSize);
} // WriteInt

//#===--- Monolithic Functions

void CRegistry::ReadString(KEYTYPE eType, const char *pcKey, const char *pcSubKey, 
									const char *pcDefault, char *const pcValue) {
	// Key data
	DWORD uSize = STR_SIZE;
	char pcData[STR_SIZE] = "";
	HKEY hKey = NULL;
	// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
		case CLASSES_ROOT:			hRoot = HKEY_CLASSES_ROOT; break;
	}
// Read key
	// Open the key
	if (::RegOpenKeyEx(hRoot, 
							 pcKey, 
							 0L, 
							 KEY_QUERY_VALUE,
							 &hKey) == ERROR_SUCCESS) {
		// Read the value
		if (::RegQueryValueEx(hKey,
									 pcSubKey,
									 NULL,
									 NULL, 
									 (LPBYTE)pcData,
									 &uSize) == ERROR_SUCCESS) {
			strcat(pcValue, pcData);
		}
		// or set the default
		else {
			if (pcDefault)
				strcat(pcValue, pcDefault);
			else
				pcValue[0] = '\0';
		}
		// Close the key
		::RegCloseKey(hKey);
	}
} // ReadString - Monolithic

void CRegistry::WriteString(KEYTYPE eType, const char *pcKey, const char *pcSubKey, 
									 const char *pcString) {
	ASSERT(pcString);
	// Key data
	DWORD uSize = strlen(pcString);
	HKEY hKey = NULL;
	// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
		case CLASSES_ROOT:			hRoot = HKEY_CLASSES_ROOT; break;
	}
// Read key
	// Open the key
	if (::RegCreateKeyEx(hRoot, 
								pcKey, 
								0L, 
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL) == ERROR_SUCCESS) {
		// Write the value
		::RegSetValueEx(hKey,
							 pcSubKey,
							 NULL,
							 REG_SZ, 
							 (LPBYTE)pcString,
							 uSize);
		// Close the key
		::RegCloseKey(hKey);
	}
} // WriteString - Monolithic

DWORD CRegistry::ReadInt(KEYTYPE eType, const char *pcKey, const char *pcSubKey,
								 const DWORD dDefault) {
	// Key data
	DWORD uSize = sizeof(DWORD);
	HKEY hKey = NULL;
	DWORD uNum = 0;
	// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
		case CLASSES_ROOT:			hRoot = HKEY_CLASSES_ROOT; break;
	}
// Read key
	// Open the key
	if (::RegOpenKeyEx(hRoot, 
							 pcKey, 
							 0L, 
							 KEY_QUERY_VALUE,
							 &hKey) == ERROR_SUCCESS) {
		// Read the value
		if (::RegQueryValueEx(hKey,
									 pcSubKey,
									 NULL,
									 NULL, 
									 (LPBYTE)&uNum,
									 &uSize) != ERROR_SUCCESS) {
		// or set the default
			uNum = dDefault;
		}
		// Close the key
		::RegCloseKey(hKey);
	}
	return uNum;
} // ReadInt - Monolithic

void CRegistry::WriteInt(KEYTYPE eType, const char *pcKey, const char *pcSubKey, 
								 const DWORD uNum) {
	// Key data
	DWORD uSize = sizeof(DWORD);
	HKEY hKey = NULL;
	// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
		case CLASSES_ROOT:			hRoot = HKEY_CLASSES_ROOT; break;
	}
// Read key
	// Open the key
	if (::RegCreateKeyEx(hRoot, 
								pcKey, 
								0L, 
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_WRITE,
								NULL,
								&hKey,
								NULL) == ERROR_SUCCESS) {
		// Write the value
		::RegSetValueEx(hKey,
							 pcSubKey,
							 NULL,
							 REG_DWORD, 
							 (LPBYTE)&uNum,
							 uSize);
		// Close the key
		::RegCloseKey(hKey);
	}
} // WriteInt - Monolithic
