//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// Image.h - 21/12/1999 - Warren Moore
//	Image header
//
// $Id: Image.h,v 1.10 2000/08/07 18:57:47 waz Exp $
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
// Image types
enum IMAGETYPE {
	IT_UNKNOWN,
	IT_MONO,
	IT_GREY,
	IT_PALETTE,
	IT_RGB,
};

// Scaling filter types
enum IMAGEFILTERTYPE {
	IF_BOX,
	IF_BILINEAR,
};

// Image operation results
enum IRESULT {
	I_OK,
	I_OUT_OF_RANGE,
	I_INVALID_PARAM,
	I_INCORRECT_TYPE,
	I_UNSUPPORTED_TYPE,
	I_NO_IMAGE,
	I_NO_PALETTE,
	I_OUT_OF_MEMORY,
	I_NO_DATA,
	I_ERROR,
};

///////////
// CImage

class CImage {
public:
	//#===--- Image Creation
	// Contructor
	CImage(IMAGETYPE eType = IT_UNKNOWN, int iWidth = 0, int iHeight = 0, int iColours = 256);
	// Copy constructor
	CImage(const CImage &oCopy);
	// Scaling copy constructor (e.g. double size if iZoom = 1 : half size if iZoom = -1)
	CImage(const CImage &oCopy, int iZoom);
	// Scaling copy constructor (normal Scale params)
	CImage(const CImage &oCopy, int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);
	// Converting copy constructor
	CImage(const CImage &oCopy, IMAGETYPE eImgType, int iColours = 256);
	// Destructor
	~CImage();

	//#===--- Editing
	// Returns the current image type
	IMAGETYPE GetType() const;
	// Returns the image size
	void GetSize(int &iWidth, int &iHeight) const;
	// Checng the size of the image, reallocating memory if necessary)
	IRESULT SetSize(int iWidth, int iHeight);
	// Sets the image black
	void Clear();
	// Paste an image into the calling image (image offset optional)
	void Paste(const CImage &oCopy, int iXOff = 0, int iYOff = 0);
	// Paste an image into the calling image (transparent colour and image offset optional)
	// If bTrans not set, uColour ignored, otherwise this colour is not drawn (uColour = 0x00RRGGBB)
	void Paste(const CImage &oCopy, float fAlpha, bool bTrans = false, 
						 unsigned int uColour = 0, int iXOff = 0, int iYOff = 0);
	// Vertically flips the image
	void Flip();

	//#===--- Access
	// Returns pointer to image palette object
	const CImagePalette* GetPalette(void) const;
	// Returns pixel value at coords
	unsigned int GetPixel(int iX, int iY) const;
	unsigned int GetPixel(unsigned int uPixel) const;

	//#===--- Conversion 
	// Convert image type (if necessary, default 256 colours for IT_PALETTE)
	IRESULT Convert(IMAGETYPE eType, int iColours = 256);
	// Force an RGB image to the supplied palette
	IRESULT ForceToPalette(CImagePalette &oPalette);
	// Change the image type and disregard the image data (palette colours supplied if necessary)
	IRESULT ForceType(IMAGETYPE eType, int iColours = 256);

	//#===--- Scaling
	// Scales image to specified height (default filter: bilinear)
	IRESULT Scale(int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);

	//#===--- Image Loading and Saving
	// Requires a previously created file filter object
	FRESULT Load(const char *pcFname, CImageFile *poImageFile);
	FRESULT Save(const char *pcFname, CImageFile *poImageFile) const;

	//#===--- Import
	// Import an image from an AME texture structure
	IRESULT ImportAMETexture(stAMETexture &sAME);
	// Import an image from raw data (bounds checking not performed)
	IRESULT ImportRawImage(unsigned char *pcRaw, bool bWordAlign = false,
												 bool bReversePixels = false, bool bFlip = false);

//#===--- Internal Structures
protected:
	typedef struct SContribStruct {
		float *m_pWeights;
		int m_iLeft, m_iRight;

		SContribStruct() {
			m_pWeights = NULL;
			m_iLeft = m_iRight = 0;
		}
	} SContrib;

	typedef struct SContribListStruct {
		SContrib *m_pContrib;
		int m_iWidth, m_iWindow;

		SContribListStruct() {
			m_pContrib = NULL;
			m_iWidth = m_iWindow = 0;
		}
	} SContribList;

//#===--- Internal Functions
protected:
	// Image memory allocation
	IRESULT CreateImage();

	//#===--- Colour Quantisation
	IRESULT CreatePaletteFromRGB(int iColours = 256);

	//#===--- Scaling
	SContribList *AllocateContributions(int iWidth, int iWindow);
	SContribList *CalculateContributions(int iInputWidth, int iOutputWidth, IMAGEFILTERTYPE eFilter);
	void DeleteContributions(SContribList *psList);

	IRESULT ScaleHorizontal(int iWidth, IMAGEFILTERTYPE eFilter);
	IRESULT ScaleHorizontalRGB(int iWidth, IMAGEFILTERTYPE eFilter);

	IRESULT ScaleVertical(int iHeight, IMAGEFILTERTYPE eFilter);
	IRESULT ScaleVerticalRGB(int iHeight, IMAGEFILTERTYPE eFilter);

//#===--- Internal Data
protected:
	IMAGETYPE m_eImageType;
	int m_iWidth, m_iHeight;
	// these variable reference data in 32 bit blocks
	unsigned int m_iDataSize;						// size of image data
	unsigned int m_iLineSize;						// size of a scan line
	unsigned int* m_pData;							// pointer to image data
	CImagePalette *m_poPalette;	// pointer to palette object

//#===--- Friends
	friend class CRCOpenGLBufferWin32;

};

//#===--- Inline Functions

inline IMAGETYPE CImage::GetType() const {
	return m_eImageType;
}

inline void CImage::GetSize(int &iWidth, int &iHeight) const {
	iWidth = m_iWidth;
	iHeight = m_iHeight;
}

inline const CImagePalette* CImage::GetPalette(void) const {
	return m_poPalette;
}

inline unsigned int CImage::GetPixel(int iX, int iY) const {
	unsigned int uPixel = iY*m_iWidth + iX;
	return GetPixel(uPixel);
}

inline unsigned int CImage::GetPixel(unsigned int uPixel) const {
	if (m_eImageType == IT_RGB) {
		unsigned int uValue= 0;
		unsigned char *pPixel = (unsigned char*)m_pData + (uPixel * 3);
		uValue = *((unsigned int*)pPixel) & 0x00FFFFFF;
		return uValue;
	}
	else 
		if ((m_eImageType == IT_PALETTE) || (m_eImageType == IT_GREY))
			return ((unsigned char*)m_pData)[uPixel];
		else
			return 0;
}

#endif // _VAL_IMAGE_

