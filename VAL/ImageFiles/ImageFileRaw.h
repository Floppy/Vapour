//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// ImageFileBMP.h - 31/07/2000 - Warren Moore
//	Raw bitmap file format image
//
// $Id: ImageFileRaw.h,v 1.2 2000/07/31 15:42:39 waz Exp $
//

#ifndef _VAL_IMAGEFILERAW_
#define _VAL_IMAGEFILERAW_

#pragma once

#include "ImageFileStore.h"

#include <fstream.h>

//////////////////
// CImageFileRaw

class CImageFileRaw : public CImageFile {
public:
	CImageFileRaw();
	~CImageFileRaw();

	//#===--- Exported Functions
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

	//#===--- Internal Data
private:
	fstream m_oFile;
	IMAGETYPE m_eSaveType;
	CImagePalette *m_pPalette;

	//#===--- ImageFileProxy Functions
public:
	static const char *GetTitle() {
		return ("Raw Image");
	}

	static const char *GetExtension() {
		return ("raw");
	}

};

#endif // _VAL_IMAGEFILERAW_