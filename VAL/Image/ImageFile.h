//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFile.h - 21/12/1999 - Warren Moore
//	Abstract image file format header
//
// $Id: ImageFile.h,v 1.1 2000/06/16 21:59:45 waz Exp $
//

#ifndef _IMAGEFILE_
#define _IMAGEFILE_

#pragma once

//#===--- Includes
#include "Defs.h"
#include "Palette.h"

///////////////
// CImageFile

class CImageFile {
public:
	CImageFile() {};
	virtual ~CImageFile() {};

	virtual const char *GetFileExtension() const = 0;
	virtual const char *GetFileTitle() const = 0;
// Load Functions
	virtual FRESULT GetImageType(const char *pFname, IMAGETYPE &eImgType) = 0;
	virtual FRESULT Load(const char *pFname, int &x, int &y, unsigned char *&pData) = 0;
// Save Functions
	virtual bool CheckSaveType(IMAGETYPE eImgType) const = 0;
	virtual void SetSaveType(IMAGETYPE eImgType) = 0;
	virtual void SetPalette(CImagePalette *pPalette) = 0;
	virtual FRESULT Save(const char *pFname, const int x, const int y,
		const unsigned char *pData) const = 0;
};

#endif // _IMAGEFILE_