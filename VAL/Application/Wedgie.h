//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.h - 02/07/2000 - Warren Moore
//	Creation and reading of compressed Wedgie files
//
// $Id: Wedgie.h,v 1.2 2000/07/02 23:22:36 waz Exp $
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
};

//////////////////
// CWedgieCreate

class CWedgie {
public:
	CWedgie();
	virtual ~CWedgie();

//#===--- External Functions
	// Open the wedgie (assumes the file stream is already opened correctly)
	// Require the base directory to be created from or extracted to
	// Creates the wedgie if bCreate = true
	// Reads the wedgie data if bCreate = false
	WJERESULT Open(fstream *pFile, const char *pDir = NULL, bool bCreate = false);
	// Close the wedgie (does not close the file stream)
	WJERESULT Close();

	// Return the current base directory 
	const char *Directory() const;
	// Return the current wedgie name
	const char *Name() const;

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
	// Count the files within the base directory
	int Count();

//#===--- Internal Data
	char *m_pcBaseDir;					// Base directory
	char *m_pcWJEName;					// Wedgie filename
	fstream *m_poFile;					// Active file stream pointer
	bool m_bCreate;							// Wedgie creation indicator
	int m_iFiles;								// Number of files in wedgie

	// Wedgie version number
	const char m_cVerHigh;
	const char m_cVerLow;
};

#endif // _VAL_WEDGIE_
