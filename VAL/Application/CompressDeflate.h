//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CompressDeflate.h - 12/06/2000 - Warren Moore
//  Derived class for Deflate compression algorithm
//
// $Id: CompressDeflate.h,v 1.1 2000/06/16 21:57:44 waz Exp $
//

#ifndef _COMPRESS_DEFLATE_
#define _COMPRESS_DEFLATE_

#pragma once

// Includes
#include "Compress.h"
#include "zlib.h"
#include <queue>

// Defines
#define CP_DEFBLOCKSIZE 8192

/////////////////////
// CCompressDeflate

class CCompressDeflate : public CCompress {
public:
	CCompressDeflate();
	~CCompressDeflate();

// Mandatory functions
	void SetCompressionLevel(int iLevel);
	void WriteToFile(bool bFile, ofstream *pFile = NULL);
	bool Init(bool bCompress = true);
	int Input(unsigned char *pData, int iSize);
	int End();
	int Retrieve(unsigned char *pData, int iSize);

// Specific Functions
	void SetBlockSize(int iSize);

private:
//#=- Internal Functions
	int StoreData(unsigned char *pData, int iSize);

//#=- Internal Structures
	typedef struct Block {
		unsigned char *pData;
		unsigned char *pRest;
		int iSize;
	} sBlock;

//#=- Internal Variables
	bool m_bActive;
	bool m_bCompress;
	bool m_bStreamEnd;
	int m_iLevel;
	int m_iBlockSize;
// File data
	bool m_bFile;
	ofstream *m_pFile;
// Output block
	unsigned char *m_pBlock;
// Zlib Stream Structure
	z_stream m_sZStream;
// Retrievable Memory Queue
	std::queue<sBlock> m_oMemQueue;
	int m_iQueueSize;
};

#endif // _COMPRESS_DEFLATE_