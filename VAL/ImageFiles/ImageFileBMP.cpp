//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileBMP.cpp - 21/12/1999 - Warren Moore
//	BMP file format image implementation
//
// $Id: ImageFileBMP.cpp,v 1.1 2000/06/16 22:00:33 waz Exp $
//

#include "stdafx.h"

#include "ImageFileBMP.h"
#include "ImageFileProxy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////
// CImageFileBMP Proxy

CImageFileProxy<CImageFileBMP> g_oImageProxyBMP;

//////////////////
// CImageFileBMP

// Store Functions

CImageFileBMP::CImageFileBMP() {
	m_eSaveType = IT_RGB;
	m_pPalette = NULL;
} // Constructor

CImageFileBMP::~CImageFileBMP() {
	if (m_pPalette) 
		delete m_pPalette;
} // Destructor

const char *CImageFileBMP::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension

const char *CImageFileBMP::GetFileTitle() const {
	return GetTitle();
} // GetFileExtension

// ImageFile Functions

FRESULT CImageFileBMP::LoadHeaders() {
// Check the bitmap file header
	BITMAPFILEHEADER bmfh;
	m_iFile.read((char*)&bmfh, sizeof(bmfh));
// File error checking
	if (m_iFile.bad())
		return F_FILEERROR;
// Check bitmap type
	if (bmfh.bfType != 0x4d42) // 'BM'
		return F_WRONGFILETYPE;
	if ((bmfh.bfReserved1 != 0) || (bmfh.bfReserved2 != 0))
		return F_WRONGFILETYPE;

// Check the bitmap info header
	BITMAPINFOHEADER bmih;
	m_iFile.read((char*)&bmih, sizeof(bmih));
// File error checking
	if (m_iFile.bad())
		return F_FILEERROR;
// Get resolution & orientation
	m_iWidth = bmih.biWidth;
	m_iHeight = abs(bmih.biHeight);
	m_bTopDown = (bmih.biHeight < 0);

// Check the colour depth = 1 bit planes, 24 bpp
	if ((bmih.biPlanes != 1) || (bmih.biBitCount != 24)) 
		return F_WRONGIMAGETYPE;
// No compression
	if (bmih.biCompression != BI_RGB)
		return F_WRONGIMAGETYPE;
// No colour table, must be a packed bitmap
	if ((bmih.biClrUsed != 0) || (bmih.biClrImportant != 0))
		return F_WRONGIMAGETYPE;

// Set the read pointer to the beginning of the data
	m_iFile.seekg(bmfh.bfOffBits, ios::beg);
// File error checking
	if (m_iFile.bad())
		return F_FILEERROR;

	return F_OK;
} // LoadHeader

FRESULT CImageFileBMP::GetImageType(const char *pFname, IMAGETYPE &eImgType) {
// Open the file, don't create if it doesn't exist
	m_iFile.open(pFname, ios::in | ios::binary | ios::nocreate);
	if (!m_iFile)
		return F_DOESNOTEXIST;

// Check the bitmap headers for image type
	FRESULT eFResult = F_OK;
	if ((eFResult = LoadHeaders()) == F_OK)
		eImgType = IT_RGB;

// Close the file
	m_iFile.close();

	return eFResult;
} // GetImageType

FRESULT CImageFileBMP::Load(const char *pFname, int &x, int &y, unsigned char *&pData) {
// Open the file, don't create if it doesn't exist
	m_iFile.open(pFname, ios::in | ios::binary | ios::nocreate);
	if (!m_iFile)
		return F_DOESNOTEXIST;

// Check the bitmap headers
	FRESULT fResult = F_OK;
	fResult = LoadHeaders();
	if (fResult == F_OK) {
	// Read in the image data
		UINT iLineLength = m_iWidth * 3;
		UINT iLineSize = iLineLength;
		if (iLineSize % 4)
			iLineSize += (4 - (iLineSize % 4));
		UINT iPad = iLineSize - iLineLength;
		UINT iMemSize = iLineSize * m_iHeight;
		unsigned char *pInputData = NULL;
		NEWBEGIN
		pInputData = new unsigned char[iMemSize];
		NEWEND("CImageFileBMP::Load - New image data")
		unsigned char *pIData = pInputData;
		unsigned char *pIDataPtr = pIData;
		if (!m_bTopDown)
			pIData += iLineSize * (m_iHeight - 1);
		int iPitch = (m_bTopDown ? iLineSize : -iLineSize);
		unsigned char *pTemp = NULL;
		NEWBEGIN
		pTemp = (unsigned char*) new unsigned char[iLineSize];
		NEWEND("CImageFileBMP::Load - Temporary line buffer")
		unsigned char *pTempPtr = pTemp;

	// Load data if memory okay
		if (pInputData && pTemp) {
			memset(pInputData, 0, iMemSize);
			register int iY = m_iHeight, iX = m_iWidth;
			while (iY--) {
				pIDataPtr = pIData;
				pTempPtr = pTemp;
				m_iFile.read(pTempPtr, iLineSize);
			// Reverse the pixel format
				iX = m_iWidth;
				while (iX--) {
					pIDataPtr[0] = pTempPtr[2];
					pIDataPtr[1] = pTempPtr[1];
					pIDataPtr[2] = pTempPtr[0];
					pIDataPtr += 3;
					pTempPtr += 3;
				}
			// Add the line step
				pIData += iPitch;
			}
			delete [] pTemp;
		// Set return parameters
			if (pData)
				delete [] pData;
			pData = pInputData;
			x = m_iWidth;
			y = m_iHeight;
		}
		else {
			if (pInputData)
				delete [] pInputData;
			if (pTemp)
				delete [] pTemp;
			fResult = F_OUTOFMEMORY;
		}
	}

// Close the file prior to returning
	m_iFile.close();

//	return fResult;
	return fResult;
} // Load

bool CImageFileBMP::CheckSaveType(IMAGETYPE eImgType) const {
	return (eImgType == IT_RGB) || (eImgType == IT_PALETTE);
} // CheckSaveType

void CImageFileBMP::SetSaveType(IMAGETYPE eImgType) {
	m_eSaveType = eImgType;
} // SetSaveType

void CImageFileBMP::SetPalette(CImagePalette *pPalette) {
	if (m_eSaveType != IT_PALETTE)
		return;
// If not there, create the palette
	if (!m_pPalette) {
		NEWBEGIN
		m_pPalette = (CImagePalette*) new CImagePalette;
		NEWEND("CImageFileBMP::SetPalette - Palette object")
	}
// If there, copy the palette
	if (m_pPalette && pPalette) {
		m_pPalette->Copy(pPalette);
	}
} // SetPalette

FRESULT CImageFileBMP::Save(const char *pFname, const int x, const int y,
														 const unsigned char *pData) const {
// Check for valid image save type
	if ((m_eSaveType != IT_RGB) && (m_eSaveType != IT_PALETTE))
		return F_WRONGIMAGETYPE;

// Open the file, destroy the previous file contents
	ofstream oFile(pFname, ios::out | ios::binary | ios::trunc);
	if (!oFile)
		return F_FILEERROR;

// Calculate the image parameters
	UINT iWidth = x;
	UINT iHeight = y;
	UINT iLineLength = iWidth * ((m_eSaveType == IT_PALETTE) ? 1 : 3);
	UINT iLineSize = iLineLength;
	if (iLineSize % 4)
		iLineSize += (4 - (iLineSize % 4));
	UINT iPad = iLineSize - iLineLength;
	UINT iMemSize = iLineSize * iHeight;
	unsigned char *pOData = (unsigned char*)pData;
	pOData += iLineSize * (iHeight - 1);
	unsigned char *pODataPtr = pOData;
	int iPitch = -iLineSize;

// Set the header structure values
	BITMAPFILEHEADER bmfh;
	BITMAPINFOHEADER bmih;

// File header
	bmfh.bfType = 0x4d42;
	bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	if (m_eSaveType == IT_PALETTE)
		bmfh.bfOffBits += 1024;
	bmfh.bfSize = bmfh.bfOffBits + iMemSize;

// Info header
	bmih.biSize = sizeof(bmih);
	bmih.biWidth = iWidth;
	bmih.biHeight = iHeight;
	bmih.biPlanes = 1;
	bmih.biBitCount = (m_eSaveType == IT_PALETTE) ? 8 : 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = iMemSize;
	bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 5120;
	bmih.biClrUsed = bmih.biClrImportant = 0;

// Write the file header
	oFile.write((unsigned char*)&bmfh, sizeof(bmfh));
// File error checking
	if (m_iFile.bad()) {
		oFile.close();
		return F_FILEERROR;
	}
// Write the info header
	oFile.write((unsigned char*)&bmih, sizeof(bmih));
// File error checking
	if (m_iFile.bad()) {
		oFile.close();
		return F_FILEERROR;
	}

// Write if the palette (if necessary);
	if (m_pPalette && (m_eSaveType == IT_PALETTE)) {
		for (int i = 0; i < 256; i++) {
			unsigned long uColour;
			m_pPalette->GetEntry(i, uColour);
			oFile.write((unsigned char*)&uColour, 4);
		}
	}

// Write the data bottom-up
	register int iY = iHeight, iX = iWidth;
	if (m_eSaveType == IT_RGB) {
		while (iY--) {
			pODataPtr = pOData;
			iX = iWidth;
			while (iX--) {
			// Write the pixels in reverse order
				oFile.write(pODataPtr + 2, 1);
				oFile.write(pODataPtr + 1, 1);
				oFile.write(pODataPtr    , 1);
				pODataPtr += 3;
			}
		// Write the padding
			oFile.write("\0\0\0", iPad);
		// Add the line step
			pOData += iPitch;
		}
	}
	else {
		while (iY--) {
		// Write the line
			oFile.write(pODataPtr, iLineSize);
		// Add the line step
			pODataPtr += iPitch;
		}
	}

// Close the file prior to returning
	oFile.close();

	return F_OK;
} // Save

