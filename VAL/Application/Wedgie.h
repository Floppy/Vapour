//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.h - 02/07/2000 - Warren Moore
//	Wedgie file class 
//
// $Id: Wedgie.h,v 1.1 2000/07/02 22:59:51 waz Exp $
//

#ifndef _VAL_WEDGIE_
#define _VAL_WEDGIE_

#pragma once

#include "Defs.h"

#include <iostream.h>
#include <fstream.h>

//#===--- Defines

#define WEDGIE_VER_HIGH			0x01
#define WEDGIE_VER_LOW			0x00
#define READ_BLOCK_SIZE			1024
#define HEADER_SIZE						14

#define WJE_TOTAL						"WJETotal"
#define WJE_FILE						"WJEFile"

//#===--- Data types
enum WJERESULT {
	WJE_OK = 0,
	WJE_ERROR,
}

//////////////////
// CWedgieCreate

class CWedgie {
public:
	CWedgie();
	virtual ~CWedgie();

//#===--- External Functions
	// Open the wedgie (assumes the file stream is already opened correctly)
	// Creates the wedgie if bCreate = true
	// Reads the wedgie data if bCreate = false
	WJERESULT Open(fstream *pFile, bool bCreate = false);
	// Close the wedgie (does not close the file stream)
	WJERESULT Close();

	// Set the base directory - where to read from on create, where to write to on read
	void Directory(const char *pDir);
	// Return the current directory set
	const char *Directory();

	// Return the number of files in the wedgie
	int Files() const;

protected:
//#===--- Internal Structures
	typedef struct sFileDataStruct {
		char *m_pName;
		unsigned int m_iOffset;
		unsigned int m_iOrigSize;
		unsigned int m_iCompSize;

		sFileDataStruct() {
			m_pName = NULL;
			m_iOffset = m_iOrigSize = m_iCompSize = 0;
		}

	} sFileData;

//#===--- Internal Functions

//#===--- Internal Data
	char *m_pBaseDir;					// Base directory
	char *m_pWJEName;					// Wedgie filename
	int m_iFiles;							// Number of files in wedgie

	// Wedgie version number
	const char m_cVerHigh;
	const char m_cVerLow;
};

#endif // _VAL_WEDGIE_
