//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// WedgieCreate.h - 12/04/2000 - Warren Moore
//	Wedgie creation class 
//
// $Id: WedgieCreate.h,v 1.3 2000/06/17 12:13:17 waz Exp $
//

#ifndef _VAL_WEDGIECREATE_
#define _VAL_WEDGIECREATE_

#pragma once

#include "Defs.h"

#include <iostream.h>
#include <fstream.h>

//#===--- Defines

#define WEDGIE_VER_HIGH			0x01
#define WEDGIE_VER_LOW			0x00
#define READ_BLOCK_SIZE			1024
#define HEADER_SIZE						14

#define WJE_TOTAL						"WJECreateTotal"
#define WJE_FILE						"WJECreateFile"

//////////////////
// CWedgieCreate

class CWedgieCreate {
public:
	CWedgieCreate();
	virtual ~CWedgieCreate();

//#===--- External Functions
	void SetBaseName(const char *pBase);
	CString GetName();
	int GetFiles();
	FRESULT Generate();

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
	int Count(CString strDir);
	FRESULT GenerateTOC(CString strDir);
	void DeleteFileList();
	FRESULT WriteHeader(ofstream &oFile);
	FRESULT ProcessFiles(ofstream &oFile);
	FRESULT UpdateHeader(ofstream &oFile);

//#===--- Internal Data
	CString m_strBase;
	CString m_strName;
	int m_iBaseLength;
	int m_iFiles;
	unsigned long m_iTotalSize;
	int m_iNext;
	sFileData *m_pFile;
	streampos m_iOffset;

	const char m_cVerHigh;
	const char m_cVerLow;
};

#endif // _VAL_WEDGIECREATE_
