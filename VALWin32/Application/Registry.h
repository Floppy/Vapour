//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Registry.h - 13/01/2000 - Warren Moore
//	Utility class for registry key management
//
// $Id: Registry.h,v 1.1 2000/07/10 21:40:21 waz Exp $
//

#ifndef _REGISTRY_
#define _REGISTRY_

#pragma once

//#===--- Data Types
enum KEYTYPE {
	LOCAL_MACHINE,
	CURRENT_USER,
};

//////////////
// CRegistry

class CRegistry {
public:
	CRegistry();
	~CRegistry();

//#===--- Individual Key Functions
// The key must be opened firstin either read or write mode.
// The subkeys can then be accessed in the mode specified.
// The Key must then be closed

// Key Management Functions
	bool OpenKey(KEYTYPE eType, const char *pKey, bool bRead);
	void CloseKey();

// Read Functions
	CString ReadString(const char *pSubKey, const char *pDefault);
	DWORD ReadInt(const char *pSubKey, const DWORD uDefault);

// Write Functions
	void WriteString(const char *pSubKey, const char *pString);
	void WriteInt(const char *pSubKey, const DWORD uNum);

//#===--- Monolithic Functions
// Can be called at any time
	CString ReadString(KEYTYPE eType, const char *pKey, const char *pSubKey, const char *pDefault);
	DWORD ReadInt(KEYTYPE eType, const char *pKey, const char *pSubKey, const DWORD uDefault);

	void WriteString(KEYTYPE eType, const char *pKey, const char *pSubKey, const char *pString);
	void WriteInt(KEYTYPE eType, const char *pKey, const char *pSubKey, const DWORD uNum);

//#===--- Internal Data
private:
	bool m_bOpen, m_bRead;
	HKEY m_hRoot, m_hKey;
};

#endif // _REGISTRY_

