//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Registry.cpp - 13/01/2000 - Warren Moore
//	Utility class for registry key management
//
// $Id: Registry.cpp,v 1.1 2000/07/10 21:40:22 waz Exp $
//

#include "StdAfx.h"

#include "Registry.h"

//////////////
// CRegistry

CRegistry::CRegistry() {
	m_bOpen = false;
	m_bRead = true;
	m_hRoot = m_hKey = NULL;
} // Constructor

CRegistry::~CRegistry() {
	if (m_bOpen)
		::RegCloseKey(m_hKey);
} // Destructor

bool CRegistry::OpenKey(KEYTYPE eType, const char *pKey, bool bRead) {
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
	}

// Open for reading
	if (m_bRead) {
		m_bOpen = (::RegOpenKeyEx(m_hRoot,
														  pKey, 
														  0L, 
														  KEY_QUERY_VALUE,
														  &m_hKey) == ERROR_SUCCESS);
	}
// Open for writing (create if doesn't exist)
	else {
		m_bOpen = (::RegCreateKeyEx(m_hRoot, 
															  pKey, 
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

CString CRegistry::ReadString(const char *pSubKey, const char *pDefault) {
	ASSERT(m_bRead && pSubKey);
	if (!m_bOpen)
		return pDefault;
// Set data buffer
	DWORD uSize = 256;
	char pData[256];
	pData[0] = '\0';
// Create output string
	CString strKeyInfo("");
// Read the key
	if (::RegQueryValueEx(m_hKey,
			pSubKey,
			NULL,
			NULL, 
			(LPBYTE)pData,
			&uSize) == ERROR_SUCCESS) {
		strKeyInfo = pData;
	}
// else return default
	else {
		if (pDefault)
			strKeyInfo = pDefault;
	}

	return strKeyInfo;
} // ReadString

void CRegistry::WriteString(const char *pSubKey, const char *pString) {
	ASSERT(m_bOpen && (!m_bRead) && pSubKey && pString);
// Key data
	DWORD uSize = strlen(pString);
// Write the value
	::RegSetValueEx(m_hKey,
									pSubKey,
									NULL,
									REG_SZ, 
									(LPBYTE)pString,
									uSize);
} // WriteString

DWORD CRegistry::ReadInt(const char *pSubKey, const DWORD uDefault) {
	ASSERT(m_bRead && pSubKey);
	if (!m_bOpen)
		return uDefault;
// Set data buffer
	DWORD uSize = sizeof(DWORD);
	DWORD uNum = 0;
// Read the key
	if (::RegQueryValueEx(m_hKey,
			pSubKey,
			NULL,
			NULL, 
			(LPBYTE)&uNum,
			&uSize) != ERROR_SUCCESS) {
		uNum = uDefault;
	}

	return uNum;
} // ReadInt

void CRegistry::WriteInt(const char *pSubKey, const DWORD uNum) {
	ASSERT(m_bOpen && (!m_bRead) && pSubKey);
// Key data
	DWORD uSize = sizeof(DWORD);
// Write the value
	::RegSetValueEx(m_hKey,
									pSubKey,
									NULL,
									REG_DWORD, 
									(LPBYTE)&uNum,
									uSize);
} // WriteInt

//#===--- Monolithic Functions

CString CRegistry::ReadString(KEYTYPE eType, const char *pKey, const char *pSubKey, 
														const char *pDefault) {
// Key data
	DWORD uSize = 256;
	char pData[256];
	pData[0] = '\0';
	HKEY hKey = NULL;
	CString strKeyInfo("");
// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
	}
// Read key
	// Open the key
	if (::RegOpenKeyEx(hRoot, 
										 pKey, 
										 0L, 
										 KEY_QUERY_VALUE,
										 &hKey) == ERROR_SUCCESS) {
		// Read the value
		if (::RegQueryValueEx(hKey,
				pSubKey,
				NULL,
				NULL, 
				(LPBYTE)pData,
				&uSize) == ERROR_SUCCESS) {
			strKeyInfo = pData;
		}
		// or set the default
		else {
			if (pDefault)
				strKeyInfo = pDefault;
		}
		// Close the key
		::RegCloseKey(hKey);
	}
	return strKeyInfo;
} // ReadString - Monolithic

void CRegistry::WriteString(KEYTYPE eType, const char *pKey, const char *pSubKey, 
													const char *pString) {
	ASSERT(pString);
// Key data
	DWORD uSize = strlen(pString);
	HKEY hKey = NULL;
// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
	}
// Read key
	// Open the key
	if (::RegCreateKeyEx(hRoot, 
											 pKey, 
											 0L, 
											 NULL,
											 REG_OPTION_NON_VOLATILE,
											 KEY_WRITE,
											 NULL,
											 &hKey,
											 NULL) == ERROR_SUCCESS) {
		// Write the value
		::RegSetValueEx(hKey,
										pSubKey,
										NULL,
										REG_SZ, 
										(LPBYTE)pString,
										uSize);
		// Close the key
		::RegCloseKey(hKey);
	}
} // WriteString - Monolithic

DWORD CRegistry::ReadInt(KEYTYPE eType, const char *pKey, const char *pSubKey, 
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
	}
// Read key
	// Open the key
	if (::RegOpenKeyEx(hRoot, 
										 pKey, 
										 0L, 
										 KEY_QUERY_VALUE,
										 &hKey) == ERROR_SUCCESS) {
		// Read the value
		if (::RegQueryValueEx(hKey,
				pSubKey,
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

void CRegistry::WriteInt(KEYTYPE eType, const char *pKey, const char *pSubKey, 
												 const DWORD uNum) {
// Key data
	DWORD uSize = sizeof(DWORD);
	HKEY hKey = NULL;
// Set registry root
	HKEY hRoot;
	switch (eType) {
		case LOCAL_MACHINE:			hRoot = HKEY_LOCAL_MACHINE; break;
		case CURRENT_USER:			hRoot = HKEY_CURRENT_USER; break;
	}
// Read key
	// Open the key
	if (::RegCreateKeyEx(hRoot, 
											 pKey, 
											 0L, 
											 NULL,
											 REG_OPTION_NON_VOLATILE,
											 KEY_WRITE,
											 NULL,
											 &hKey,
											 NULL) == ERROR_SUCCESS) {
		// Write the value
		::RegSetValueEx(hKey,
										pSubKey,
										NULL,
										REG_DWORD, 
										(LPBYTE)&uNum,
										uSize);
		// Close the key
		::RegCloseKey(hKey);
	}
} // WriteInt - Monolithic

