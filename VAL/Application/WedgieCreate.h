///////////////////////////////
// Wedgie - WJE file compiler
//
// Copyright 2000 Vapour Technology
// 12/04/2000 - Warren Moore
// 
// TOC.h
//

#ifndef _VAL_WEDGIECREATE_
#define _VAL_WEDGIECREATE_

#pragma once

#include <iostream.h>
#include <fstream.h>
#include "Progress.h"

//#===--- Defines

#define WEDGIE_VER_HIGH			0x01
#define WEDGIE_VER_LOW			0x00
#define READ_BLOCK_SIZE			1024
#define HEADER_SIZE						14

//////////////////
// CWedgieCreate

class CWedgieCreate {
public:
	CWedgieCreate();
	virtual ~CWedgieCreate();

	void SetBaseName(const char *pBase);
	CString GetName();
	int GetFiles();
	FRESULT Generate(CProgress &oDlg);

protected:
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

	int Count(CString strDir);
	FRESULT GenerateTOC(CString strDir);
	void DeleteFileList();
	FRESULT WriteHeader(ofstream &oFile);
	FRESULT ProcessFiles(ofstream &oFile, CProgress &oDlg);
	FRESULT UpdateHeader(ofstream &oFile);

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
