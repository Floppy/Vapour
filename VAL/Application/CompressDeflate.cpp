//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CompressDeflate.cpp - 12/06/2000 - Warren Moore
//	Compress implementation for Deflate algorithm using zlib
//
// $Id: CompressDeflate.cpp,v 1.1 2000/06/16 21:57:44 waz Exp $
//

#include "stdafx.h"
#include "CompressDeflate.h"

/* ZLib Error Codes
	 0: Z_OK
	 1: Z_STREAM_END
	 2: Z_NEED_DICT
	-1: Z_ERRNO
	-2: Z_STREAM_ERROR
	-3: Z_DATA_ERROR
	-4: Z_MEM_ERROR
	-5: Z_BUF_ERROR
	-6: Z_VERSION_ERROR
*/

/////////////////////
// CCompressDeflate

CCompressDeflate::CCompressDeflate() {
	m_bActive = false;
	m_bCompress = true;
	m_bStreamEnd = false;
	m_iLevel = Z_DEFAULT_COMPRESSION;
	m_iBlockSize = CP_DEFBLOCKSIZE;
	m_pBlock = NULL;
	m_iQueueSize = 0;
	m_bFile = false;
	m_pFile = NULL;
// Set up z_stream constants 
	m_sZStream.zalloc = (alloc_func)0;
	m_sZStream.zfree = (free_func)0;
	m_sZStream.opaque = (voidpf)0;
} // Constructor

CCompressDeflate::~CCompressDeflate() {
// If data left, free the queue
	while (m_oMemQueue.size()) {
		sBlock &sOut = m_oMemQueue.front();
		delete [] sOut.pData;
		m_oMemQueue.pop();
	}
} // Destructor

void CCompressDeflate::SetCompressionLevel(int iLevel) {
// Check compression level
	if ((iLevel >= 0) && (iLevel <= 9)) {
		m_iLevel = iLevel;
	}
} // SetCompressionLevel

void CCompressDeflate::WriteToFile(bool bFile, ofstream *pFile) {
	if (!m_bActive) {
		m_bFile = false;
		if (bFile && pFile) {
			m_bFile = true;
			m_pFile = pFile;
		}
	}
} // WriteToFile

bool CCompressDeflate::Init(bool bCompress) {
	bool bActive = false;
	if (!m_bActive) {
	// Clear the memory queue
		while (m_oMemQueue.size()) {
			sBlock &sOut = m_oMemQueue.front();
			delete [] sOut.pData;
			m_oMemQueue.pop();
		}
	// Clear the stream end marker
		m_bStreamEnd = false;
	// Clear the stream structure
		memset(&m_sZStream, 0, sizeof(m_sZStream));
	// Allocate retrieve block
		if (m_pBlock = (unsigned char*)new unsigned char[m_iBlockSize]) {
		// Compress
			if (bCompress) {
			// Initialise zlib
				memset(&m_sZStream, 0, sizeof(m_sZStream));
				m_sZStream.next_out = m_pBlock;
				m_sZStream.avail_out = m_iBlockSize;
				bActive = deflateInit(&m_sZStream, m_iLevel) == Z_OK;
			}
		// Decompress
			else {
			// Initialise zlib
				m_sZStream.next_in = Z_NULL;
				m_sZStream.avail_in = 0;
				m_sZStream.next_out = m_pBlock;
				m_sZStream.avail_out = m_iBlockSize;
				bActive = inflateInit(&m_sZStream) == Z_OK;
			}
		// Set the active marker
			m_bActive = bActive;
		}
	}
	m_bCompress = bCompress;
	return bActive;
} // Init

int CCompressDeflate::Input(unsigned char *pData, int iSize) {
// Check we can do anything?
	if (!m_bActive) {
		return 0;
	}

// Check we have input data
	if (iSize > 0) {
	// Set up z stream
		m_sZStream.next_in = pData;
		m_sZStream.avail_in = iSize;
	// Input the data
		int iZResult;
		bool bDone = true;
		do {
		// Compress
			if (m_bCompress) {
				iZResult = deflate(&m_sZStream, Z_NO_FLUSH);
			}
		// Decompress
			else {
				if (!m_bStreamEnd) {
					iZResult = inflate(&m_sZStream, Z_NO_FLUSH);
				}
				m_bStreamEnd = (iZResult == Z_STREAM_END);
			}
		// Go again if we still have data to input, or an error has occured
			bDone = (m_sZStream.avail_in == 0);
		// If data output, save the memory to the output queue
			if (m_sZStream.avail_out < m_iBlockSize) {
				StoreData(m_pBlock, m_iBlockSize - m_sZStream.avail_out);
				m_sZStream.next_out = m_pBlock;
				m_sZStream.avail_out = m_iBlockSize;
			// There may be more data to output, so go again
				bDone &= false;
			}
		// If no data to output, stop
			else {
				bDone &= true;
			}
		} while (!bDone);
	}
	return m_iQueueSize;
} // Compress

int CCompressDeflate::End() {
	if (!m_bActive) {
		return 0;
	}

	int iZResult;
	m_sZStream.next_in = NULL;
	m_sZStream.avail_in = 0;
	bool bDone = false;
	do {
	// End the deflate stream
		if (m_bCompress) {
			iZResult = deflate(&m_sZStream, Z_FINISH);
		}
		else {
			iZResult = m_bStreamEnd ? Z_STREAM_END : inflate(&m_sZStream, Z_FINISH);
		}
	// If data output, save the memory to the output queue
		if (((iZResult == Z_OK) || (iZResult == Z_STREAM_END)) && (m_sZStream.avail_out < m_iBlockSize)) {
			StoreData(m_pBlock, m_iBlockSize - m_sZStream.avail_out);
		}
		bDone = (iZResult == Z_STREAM_END) || (iZResult != Z_OK);
	} while (!bDone);
// End the stream
	iZResult = m_bCompress ? deflateEnd(&m_sZStream) : inflateEnd(&m_sZStream);
// Free the block data
	if (m_pBlock) {
		delete [] m_pBlock;
	}
// Mark as inactive
	m_bActive = false;
	return m_iQueueSize;
} // End

int CCompressDeflate::Retrieve(unsigned char *pData, int iSize) {
	if (m_iQueueSize && iSize) {
		do {
			sBlock &sOut = m_oMemQueue.front();
		// If memory block fits within free mem, copy lot and delete
			if (iSize >= sOut.iSize) {
			// Copy the data out
				memcpy(pData, sOut.pRest, sOut.iSize);
				iSize -= sOut.iSize;
				m_iQueueSize -= sOut.iSize;
				pData += sOut.iSize;
			// Delete the memory block
				delete [] sOut.pData;
				m_oMemQueue.pop();
			}
		// If memory block larger than free, mark whats been copied out
			else {
			// Copy the data out
				memcpy(pData, sOut.pRest, iSize);
			// Set the unread pointer in the block
				sOut.iSize -= iSize;
				m_iQueueSize -= iSize;
				sOut.pRest += iSize;
				iSize = 0;
			}
		} while ((iSize > 0) && (m_oMemQueue.size() > 0));

	}
	return m_iQueueSize;
} // Retrieve

void CCompressDeflate::SetBlockSize(int iSize) {
	if ((!m_bActive) && (iSize > 0)) {
		m_iBlockSize = iSize;
	}
} // SetBlockSize

int CCompressDeflate::StoreData(unsigned char *pData, int iSize) {
	int iRetrieve = 0;
// Write to the memory queue
	if (!m_bFile) {
		sBlock sOut;
		sOut.iSize = iSize;
		if (sOut.pData = (unsigned char*) new unsigned char[iSize]) {
		// Add the output data to the memory queue
			m_iQueueSize += iSize;
			memcpy(sOut.pData, m_pBlock, iSize);
			sOut.pRest = sOut.pData;
			m_oMemQueue.push(sOut);
		}
		iRetrieve = m_iQueueSize;
	}
// Write to file
	else {
		m_pFile->write((const signed char*)pData, iSize);
	}
	return iRetrieve;
} // StoreData

