//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.h - 02/07/2000 - Warren Moore
//	Creation and reading of compressed Wedgie files
//
// $Id: Wedgie.h,v 1.3 2000/07/10 09:04:42 waz Exp $
//

#ifndef _VAL_WEDGIE_
#define _VAL_WEDGIE_

#pragma once

#include "VAL.h"

#include <iostream.h>
#include <fstream.h>

//#===--- Defines

// File ID
#define WJE_ID					"VTWEDGIE"
#define WJE_ID_SIZE			strlen(WJE_ID)

// Version
#define WJE_VER_HIGH		0x01
#define WJE_VER_LOW			0x00
#define WJE_VER_SIZE		2

// Data sizes
#define WJE_POS_SIZE		sizeof(streampos)
#define WJE_PRE_SIZE		4
#define WJE_START_SIZE	(WJE_ID_SIZE + WJE_VER_SIZE)
#define WJE_BLOCK_SIZE	8192

// Progress ID's
#define WJE_TOTAL				"WJETotal"
#define WJE_FILE				"WJEFile"

//#===--- Data types
enum WJERESULT {
	WJE_OK = 0,
	WJE_NO_FILE,
	WJE_NO_DATA,
	WJE_OUT_OF_MEMORY,
	WJE_CREATE_ERROR,
	WJE_FILE_OPEN_ERROR,
	WJE_FILE_READ_ERROR,
	WJE_FILE_WRITE_ERROR,
	WJE_FILE_CREATE_ERROR,
	WJE_WRONG_MODE,
	WJE_INVALID_ID,
	WJE_UNSUPPORTED_VERSION,
	WJE_INVALID_ENTRY,
	WJE_DIR_ERROR,
	WJE_CANNOT_MOVE,
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
	// Directory to be created from or virtual directory to read from
	// Creates the wedgie if bCreate = true
	// Reads the wedgie data if bCreate = false
	// Caution: Position of get and put pointers may have changed after calling
	WJERESULT Open(fstream *poFile, const char *pcDir = NULL, bool bCreate = false, bool bMove = false);
	// Close the wedgie (does not close the file stream)
	WJERESULT Close();

	// Return the current base directory 
	const char *Directory() const;

	// Return the number of files in the wedgie
	unsigned int Files() const;

	// Return the name of the selected file, NULL if invalid
	const char *Filename(unsigned int uEntry);

	// Extracts the selected file. An alternative filename may be provided
	WJERESULT Extract(unsigned int uEntry, const char *pcFilename = NULL);
	// Extract functions for specific version (complete filename created by base extract function)
	WJERESULT Extract1_0(unsigned int uEntry, const char *pcFilename);

protected:
//#===--- Internal Structures
	typedef struct sFileDataStruct {
		char *m_pcName;
		unsigned int m_uOffset;
		unsigned int m_uOrigSize;
		unsigned int m_uCompSize;

		sFileDataStruct() {
			m_pcName = NULL;
			m_uOffset = m_uOrigSize = m_uCompSize = 0;
		}

	} sFileData;

//#===--- Internal Functions
	// Creates the wedgie from the base dir
	WJERESULT Write(bool bMove = false);
	// Reads the contents of the wedgie
	WJERESULT Read();
	// Read functions for specific versions
	WJERESULT Read1_0();

	// Counts the files within the supplied directory
	unsigned int Count(const char *pcDir);

	//#===--- File creation functions

	// Fills the table with the files in the supplied directory
	// File table must have been created
	WJERESULT FillTable(const char *pcDir, int &iIndex);

	// Creates the wedgie file header from the contents of the file table
	WJERESULT StartHeader();

	// Compresses the files in the file table
	WJERESULT ProcessFiles();

	// Updates the wedgie header with the post-compression results
	WJERESULT EndHeader();

	// Removes compressed files and subdirectories
	WJERESULT RemoveFiles(const char *pcDir);

//#===--- Internal Data
	char *m_pcBaseDir;							// Base directory
	int m_iBaseLength;							// Length of base directory string
	fstream *m_poFile;							// Active file stream pointer
	bool m_bCreate;									// Wedgie creation indicator
	unsigned int m_uFiles;					// Number of files in wedgie
	sFileData *m_psTable;						// File data table
	unsigned int m_uTotalSize;			// Total amount of uncompressed data in wedgie
	unsigned int m_uStartPos;				// File pos of the start of the wedgie
	unsigned int m_uOffsetPos;			// Running stream offset for file positions
	unsigned int m_uPreambleSize;		// Size of the wedgie preamble
	unsigned int m_uTOCSize;				// Size of the table of contents

	// Wedgie version number
	const char m_cVerHigh;
	const char m_cVerLow;
	unsigned int m_uCurrentVer;
};

#endif // _VAL_WEDGIE_
