//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileJPEG.h - 22/12/1999 - Warren Moore
//	JPEG file format image
//
// $Id: ImageFileJPEG.h,v 1.3 2000/06/17 11:40:27 waz Exp $
//

#ifndef _VAL_IMAGEFILEBMP_
#define _VAL_IMAGEFILEBMP_

#pragma once

#include "ImageFileStore.h"

#include <fstream.h>

///////////////////
// CImageFileJPEG

//#===--- Internal Defines
#ifdef _VAL_IMAGEFILEJPEG_INTERNAL_

#define JPEG_QUALITY		85

#endif // _VAL_IMAGEFILEJPEG_INTERNAL_

class CImageFileJPEG : public CImageFile {
public:
	CImageFileJPEG();

// Exported Functions
	const char *GetFileExtension() const;
	const char *GetFileTitle() const;
	FRESULT GetImageType(const char *pFname, IMAGETYPE &eImgType);
	FRESULT Load(const char *pFname, int &x, int &y, unsigned char *&pData);
	bool CheckSaveType(IMAGETYPE eImgType) const;
	void SetSaveType(IMAGETYPE eImgType);
	void SetPalette(CImagePalette *pPalette);
	FRESULT Save(const char *pFname, const int x, const int y,
		const unsigned char *pData) const;

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	IMAGETYPE m_eSaveType;

// ImageFileProxy Functions
public:
	static const char *GetTitle() {
		return ("JPEG Image");
	}

	static const char *GetExtension() {
		return ("jpg");
	}

};

#endif // _VAL_IMAGEFILEJPEG_