//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// ImageFileBMP.cpp - 31/07/2000 - Warren Moore
//	Raw image file implementation
//
// $Id: ImageFileRaw.cpp,v 1.2 2000/07/31 15:42:38 waz Exp $
//

#include "stdafx.h"

#include "ImageFileRaw.h"
#include "ImageFileProxy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////
// CImageFileRaw Proxy

CImageFileProxy<CImageFileRaw> g_oImageProxyRaw;

//////////////////
// CImageFileRaw

//#===--- Store Functions

CImageFileRaw::CImageFileRaw() {
	m_eSaveType = IT_RGB;
	m_pPalette = NULL;
} // Constructor

CImageFileRaw::~CImageFileRaw() {
	if (m_pPalette) 
		delete m_pPalette;
} // Destructor

const char *CImageFileRaw::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension

const char *CImageFileRaw::GetFileTitle() const {
	return GetTitle();
} // GetFileExtension

//#===--- ImageFile Functions

FRESULT CImageFileRaw::GetImageType(const char *pFname, IMAGETYPE &eImgType) {
	eImgType = IT_UNKNOWN;
	return F_OK;
} // GetImageType

FRESULT CImageFileRaw::Load(const char *pFname, int &x, int &y, unsigned char *&pData) {
	// Open the file, don't create if it doesn't exist
	m_oFile.open(pFname, ios::in | ios::binary | ios::nocreate);
	if (m_oFile.fail())
		return F_DOES_NOT_EXIST;

	FRESULT eResult = F_OK;
	// Read in the image data
	unsigned int uLineLength = x;
	if (m_eSaveType == IT_RGB)
		uLineLength *= 3;
	unsigned int uLineSize = uLineLength;
	if (uLineSize % 4)
		uLineSize += (4 - (uLineSize % 4));
	unsigned int uMemSize = uLineSize * y;
	unsigned char *pInputData = NULL;
	NEWBEGIN
	pInputData = new unsigned char[uMemSize];
	NEWEND("CImageFileRaw::Load - New image data")
	unsigned char *pIData = pInputData;
	// Load data if memory okay
	if (pInputData) {
		memset(pInputData, 0, uMemSize);
		register unsigned int uY = y;
		while (uY--) {
			// Read the image
			m_oFile.read(pIData, uLineLength);
			// Check the file read
			if (m_oFile.gcount() != uLineLength) {
				delete [] pInputData;
				eResult = F_FILE_ERROR;
				break;
			}
			// Add the line step
			pIData += uLineSize;
		}
		// Set return parameters
		if (eResult == F_OK) {
			if (pData)
				delete [] pData;
			pData = pInputData;
		}
	}
	else
		eResult = F_OUT_OF_MEMORY;

	// Close the file prior to returning
	m_oFile.close();

	return eResult;
} // Load

bool CImageFileRaw::CheckSaveType(IMAGETYPE eImgType) const {
	return (eImgType == IT_RGB) || (eImgType == IT_PALETTE) || (eImgType == IT_GREY);
} // CheckSaveType

void CImageFileRaw::SetSaveType(IMAGETYPE eImgType) {
	m_eSaveType = eImgType;
} // SetSaveType

void CImageFileRaw::SetPalette(CImagePalette *pPalette) {
	return;
} // SetPalette

FRESULT CImageFileRaw::Save(const char *pFname, const int x, const int y,
														const unsigned char *pData) const {
	// Check for valid image save type
	if ((m_eSaveType != IT_RGB) && (m_eSaveType != IT_PALETTE) && (m_eSaveType != IT_GREY))
		return F_WRONG_IMAGE_TYPE;

	// Open the file, destroy the previous file contents
	ofstream oFile(pFname, ios::out | ios::binary | ios::trunc);
	if (oFile.fail())
		return F_FILE_ERROR;

	// Calculate the image parameters
	unsigned int uWidth = x;
	unsigned int uHeight = y;
	unsigned int uLineLength = uWidth * ((m_eSaveType == IT_PALETTE) ? 1 : 3);
	unsigned int uLineSize = uLineLength;
	if (uLineSize % 4)
		uLineSize += (4 - (uLineSize % 4));
	unsigned char *pOData = (unsigned char*)pData;

	register int uY = uHeight;
	while (uY--) {
		// Write the line
		oFile.write(pOData, uLineLength);
		// Add the line step
		pOData += uLineSize;
	}

	// Close the file prior to returning
	oFile.close();

	return F_OK;
} // Save

