//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// Image.h - 21/12/1999 - Warren Moore
//	Image header
//
// $Id: Image.h,v 1.4 2000/07/10 22:17:00 waz Exp $
//

#ifndef _VAL_IMAGE_
#define _VAL_IMAGE_

#pragma once

//#===--- Includes
#include "VAL.h"
//#include "ImageFile.h"
#include "CAvatar.h"
#include "Palette.h"
#include "BaseFilter.h"

//#===--- Pre-declared classes
class CImageFile;

//#===--- Defines
#define IMG_MAXWIDTH		1600
#define IMG_MAXHEIGHT		1200

//#===--- Data Types
enum IMAGETYPE {
	IT_UNKNOWN,
	IT_MONO,
	IT_GREY,
	IT_PALETTE,
	IT_RGB,
};

enum IMAGEFILTERTYPE {
	IF_BOX,
	IF_BILINEAR,
};

enum IRESULT {
	I_OK,
	I_OUTOFRANGE,
	I_INVALIDPARAM,
	I_INCORRECTTYPE,
	I_UNSUPPORTEDTYPE,
	I_NOIMAGE,
	I_OUTOFMEMORY,
	I_ERROR,
};

///////////
// CImage

class CImage {
//#===--- Member Functions
public:
	CImage(IMAGETYPE eType = IT_UNKNOWN, int iWidth = 0, int iHeight = 0, int iColours = 256);
	CImage(const CImage &oCopy);
	CImage(const CImage &oCopy, int iZoom);
	CImage(const CImage &oCopy, int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);
	CImage(const CImage &oCopy, IMAGETYPE eImgType, int iColours = 256);
	~CImage();

// Editing Functions
	IMAGETYPE GetType() const;
	void GetSize(int &iWidth, int &iHeight) const;
	IRESULT SetSize(int iWidth, int iHeight);
	void Clear();
	void Paste(const CImage &copy, int iXOff = 0, int iYOff = 0); // copy is pasted into this
	void Flip();

// Conversion 
	IRESULT Convert(IMAGETYPE eType, int iColours = 256);

// Scaling
	IRESULT Scale(int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);

// Image Loading and Saving Functions
	FRESULT Load(const char *pFname, CImageFile *pImageFile);
	FRESULT Save(const char *pFname, CImageFile *pImageFile) const;

// Import Functions
	IRESULT ImportAMETexture(stAMETexture &sAME);

//#===--- Internal Typedefs
	typedef struct sContribStruct {
		float *m_pWeights;
		int m_iLeft, m_iRight;

		sContribStruct() {
			m_pWeights = NULL;
			m_iLeft = m_iRight = 0;
		}
	} sContrib;

	typedef struct sContribListStruct {
		sContrib *m_pContrib;
		int m_iWidth, m_iWindow;

		sContribListStruct() {
			m_pContrib = NULL;
			m_iWidth = m_iWindow = 0;
		}
	} sContribList;

//#===--- Internal Functions
protected:
// Image Creation
	IRESULT CreateImage();

// Colour Quantisation
	IRESULT CreatePaletteFromRGB(int iColours = 256);

// Scaling
	sContribList *AllocateContributions(int iWidth, int iWindow);
	sContribList *CalculateContributions(int iInputWidth, int iOutputWidth, IMAGEFILTERTYPE eFilter);
	void DeleteContributions(sContribList *pList);

	IRESULT ScaleHorizontal(int iWidth, IMAGEFILTERTYPE eFilter);
	IRESULT ScaleHorizontalRGB(int iWidth, IMAGEFILTERTYPE eFilter);

	IRESULT ScaleVertical(int iHeight, IMAGEFILTERTYPE eFilter);
	IRESULT ScaleVerticalRGB(int iHeight, IMAGEFILTERTYPE eFilter);

//#===--- Internal Data
protected:
	IMAGETYPE m_eImageType;
	int m_iWidth, m_iHeight;
	// these variable reference data in 32 bit blocks
	UINT m_iDataSize;						// size of image data
	UINT m_iLineSize;						// size of a scan line
	UINT* m_pData;							// pointer to image data
	CImagePalette *m_pPalette;	// pointer to palette object

//#===--- Friends
	friend class CScene;

};

//#===--- Inline Functions

inline IMAGETYPE CImage::GetType() const {
	return m_eImageType;
}

inline void CImage::GetSize(int &iWidth, int &iHeight) const {
	iWidth = m_iWidth;
	iHeight = m_iHeight;
}

#endif // _VAL_IMAGE_

