//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Compress.h - 12/06/2000 - Warren Moore
//  Compression base class
//
// $Id: Compress.h,v 1.1 2000/06/16 21:57:44 waz Exp $
//

#ifndef _COMPRESS_
#define _COMPRESS_

#pragma once

// Includes
#include <fstream.h>

//#===--- CCompress

class CCompress {
public:
	CCompress() {};
	virtual ~CCompress() {};

	virtual void SetCompressionLevel(int iLevel) = 0;
	virtual void WriteToFile(bool bFile, ofstream *pFile = NULL) = 0;
	virtual bool Init(bool bCompress = true) = 0;
	virtual int Input(unsigned char *pData, int iSize) = 0;
	virtual int End() = 0;
	virtual int Retrieve(unsigned char *pData, int iSize) = 0;
};

#endif // _COMPRESS_