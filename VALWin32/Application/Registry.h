//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Registry.h - 13/01/2000 - Warren Moore
//	  Utility class for registry key management
//
// $Id: Registry.h,v 1.2 2000/11/27 20:39:20 waz Exp $
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
// The key must be opened first in either read or write mode.
// The subkeys can then be accessed in the mode specified.
// The Key must then be closed

// Key Management Functions
	bool OpenKey(KEYTYPE eType, const char *pcKey, bool bRead);
	void CloseKey();

// Read Functions
	void ReadString(const char *pcSubKey, const char *pcDefault, char *const pcValue);
	DWORD ReadInt(const char *pcSubKey, const DWORD uDefault);

// Write Functions
	void WriteString(const char *pcSubKey, const char *pcString);
	void WriteInt(const char *pcSubKey, const DWORD uNum);

//#===--- Monolithic Functions
// Can be called at any time
	void ReadString(KEYTYPE eType, const char *pcKey, const char *pcSubKey, const char *pcDefault, char *const pcValue);
	DWORD ReadInt(KEYTYPE eType, const char *pcKey, const char *pcSubKey, const DWORD uDefault);

	void WriteString(KEYTYPE eType, const char *pcKey, const char *pcSubKey, const char *pcString);
	void WriteInt(KEYTYPE eType, const char *pcKey, const char *pcSubKey, const DWORD uNum);

//#===--- Internal Data
private:
	bool m_bOpen, m_bRead;
	HKEY m_hRoot, m_hKey;
};

#endif // _REGISTRY_

