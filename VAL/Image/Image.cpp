//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// Image.h - 21/12/1999 - Warren Moore
//	Image implementation
//
// $Id: Image.cpp,v 1.7 2000/07/22 23:23:06 waz Exp $
//

#include "stdafx.h"

#include "Image.h"

#include "ImageFile.h"
#include <fstream.h>
#include "ColourQuant.h"
#include "BaseFilter.h"
#include "BoxFilter.h"
#include "BilinearFilter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////
// CImage

CImage::CImage(IMAGETYPE eType, int iWidth, int iHeight, int iColours) {
// Set the image type
	m_eImageType = eType;
// Set the width and height params
	m_iWidth = iWidth;
	m_iHeight = iHeight;
// Allocate the memory
	m_pData = NULL;
	m_pPalette = NULL;
	IRESULT eResult = CreateImage();
} // Constructor

CImage::CImage(const CImage &oCopy) {
// Set the relative parameters
	m_eImageType = oCopy.m_eImageType;
	m_iWidth = oCopy.m_iWidth;
	m_iHeight = oCopy.m_iHeight;
// Allocate the memory
	IRESULT eResult = CreateImage();
// If image data has been allocated ok, copy over
	if (eResult == I_OK) {
		memcpy(m_pData, oCopy.m_pData, m_iDataSize * 4);
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_pPalette))
			m_pPalette->Copy(oCopy.m_pPalette);
	}
} // Copy Constructor

CImage::CImage(const CImage &oCopy, int iZoom) {
// Set the relative parameters
	m_eImageType = oCopy.m_eImageType;
	m_iWidth = oCopy.m_iWidth;
	m_iHeight = oCopy.m_iHeight;
// Allocate the memory
	IRESULT eResult = CreateImage();
// If image data has been allocated ok, copy over
	if (eResult == I_OK) {
		memcpy(m_pData, oCopy.m_pData, m_iDataSize * 4);
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_pPalette))
			m_pPalette->Copy(oCopy.m_pPalette);
	}
// Scale the copied image
	if (eResult == I_OK) {
		int iWidth = m_iWidth, iHeight = m_iHeight;
		while (iZoom) {
			if (iZoom > 0) {
				iZoom--;
				iWidth <<= 1;
				iHeight <<= 1;
			}
			else {
				iZoom++;
				iWidth >>= 1;
				iHeight >>= 1;
			}
		}
		eResult = Scale(iWidth, iHeight);
	}
// Clean up on errors
	if (eResult != I_OK) {
		m_iWidth = m_iHeight = 0;
		m_iDataSize = m_iLineSize = 0;
		if (m_pData)
			delete [] m_pData;
		m_pData = NULL;
		m_pPalette = NULL;
	}

} // Zoom Copy Constructor

CImage::CImage(const CImage &oCopy, int iWidth, int iHeight, IMAGEFILTERTYPE eFilter) {
// Set the relative parameters
	m_eImageType = oCopy.m_eImageType;
	m_iWidth = oCopy.m_iWidth;
	m_iHeight = oCopy.m_iHeight;
// Allocate the memory
	IRESULT eResult = CreateImage();
// If image data has been allocated ok, copy over
	if (eResult == I_OK) {
		memcpy(m_pData, oCopy.m_pData, m_iDataSize * 4);
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_pPalette))
			m_pPalette->Copy(oCopy.m_pPalette);
	}
// Scale the copied image
	if (eResult == I_OK) {
		eResult = Scale(iWidth, iHeight, eFilter);
	}
// Clean up on errors
	if (eResult != I_OK) {
		m_iWidth = m_iHeight = 0;
		m_iDataSize = m_iLineSize = 0;
		if (m_pData)
			delete [] m_pData;
		m_pData = NULL;
		m_pPalette = NULL;
	}

} // Scale copy constructor

CImage::CImage(const CImage &oCopy, IMAGETYPE eImgType, int iColours) {
// Set the relative parameters
	m_eImageType = oCopy.m_eImageType;
	m_iWidth = oCopy.m_iWidth;
	m_iHeight = oCopy.m_iHeight;
// Allocate the memory
	IRESULT eResult = CreateImage();
// If image data has been allocated ok, copy over
	if (eResult == I_OK) {
		memcpy(m_pData, oCopy.m_pData, m_iDataSize * 4);
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_pPalette))
			m_pPalette->Copy(oCopy.m_pPalette);
	}
// Convert the copied image
	if (eResult == I_OK) {
		eResult = Convert(eImgType, iColours);
	}
// Clean up on errors
	if (eResult != I_OK) {
		m_iWidth = m_iHeight = 0;
		m_iDataSize = m_iLineSize = 0;
		if (m_pData)
			delete [] m_pData;
		m_pData = NULL;
		m_pPalette = NULL;
	}
} // Conversion Copy Constructor

CImage::~CImage() {
// Delete the allocated memory
	if (m_pData)
		delete [] m_pData;
	if (m_pPalette)
		delete m_pPalette;
} // Destructor

//#===--- Image Creation

IRESULT CImage::CreateImage() {
	IRESULT eResult = I_OK;
// Validate image size
	if ((m_iWidth == 0) || (m_iHeight == 0))
		eResult = I_NO_IMAGE;
	if ((m_iWidth < 0) || (m_iWidth > IMG_MAXWIDTH)) {
		m_iWidth = 0;
		eResult = I_OUT_OF_RANGE;
	}
	if ((m_iHeight < 0) || (m_iHeight > IMG_MAXWIDTH)) {
		m_iHeight = 0;
		eResult = I_OUT_OF_RANGE;
	}

// If the size is valid, allocate the image memory
	m_pData = NULL;
	if (eResult == I_OK) {
	// Calculate the line length - word align lines
		switch (m_eImageType) {
		// IT_UNKNOWN - Ignore image properties
			case IT_UNKNOWN:
				m_iLineSize = 0;
				break;
		// IT_MONO - 1 bit per pixel
			case IT_MONO:
				m_iLineSize = m_iWidth / 32;
				if (m_iWidth % 32)
					m_iLineSize++;
				break;
		// IT_GREY - 8 bit greyscale per pixel
		// IT_PALETTE - 8 bits per colour index
			case IT_GREY:
			case IT_PALETTE:
				m_iLineSize = m_iWidth / 4;
				if (m_iWidth % 4)
					m_iLineSize++;
				break;
		// IT_RGB - 24 bits per pixel
			case IT_RGB:
				m_iLineSize = (m_iWidth * 3) / 4;
				if ((m_iWidth * 3) % 4)
					m_iLineSize++;
				break;
		}

	// Set the total data size
		m_iDataSize = m_iLineSize * m_iHeight;
	// If valid, allocate the memory
		if (m_iDataSize) {
			NEWBEGIN
			m_pData = (UINT*) new UINT[m_iDataSize];
			NEWEND("CImage::CreateImage - Image data")
			if (!m_pData)
				eResult = I_OUT_OF_MEMORY;
		}
	}

// If necessary, create the palette
	m_pPalette = NULL;
	if ((m_eImageType == IT_PALETTE) && (eResult == I_OK)) {
		NEWBEGIN
		m_pPalette = (CImagePalette*) new CImagePalette;
		NEWEND("CImage::CreateImage - Palette object")
		if (!m_pPalette)
			eResult = I_OUT_OF_MEMORY;
	}

// If allocation or a size parameter failed, zero everything
	if (eResult != I_OK) {
		m_iWidth = 0;
		m_iHeight = 0;
		m_iDataSize = 0;
		m_iLineSize = 0;
		if (m_pData)
			delete [] m_pData;
		if (m_pPalette)
			delete m_pPalette;
	}

	return eResult;
} // CreateImage

//#===--- Editing Functions

IRESULT CImage::SetSize(int iWidth, int iHeight) {
// Set the image size
	m_iWidth = iWidth;
	m_iHeight = iHeight;

// If previous data exists, delete it
	if (m_pData) {
		delete [] m_pData;
		m_pData = NULL;
	}

// Allocate the new image memory
	IRESULT eResult = CreateImage();

// Copy the contents of the old image across

// TODO

	return eResult;
} // SetSize

void CImage::Clear() {
// Clear the image memory (set to black)
	if (m_pData)
		memset(m_pData, 0, m_iDataSize * 4);
} // Clear

void CImage::Paste(const CImage &copy, int iXOff, int iYOff) {
	if ((m_eImageType != IT_RGB) || (copy.m_eImageType != IT_RGB)) 
		return;
// Make sure we don't go over the sides
	int iClipLeft = 0, iClipRight = 0, iClipTop = 0, iClipBottom = 0;
	if (iXOff < 0)
		iClipLeft = -iXOff;
	if ((iXOff + copy.m_iWidth) > m_iWidth)
		iClipRight = (iXOff + copy.m_iWidth) - m_iWidth;
	if (iYOff < 0)
		iClipTop = -iYOff;
	if ((iYOff + copy.m_iHeight) > m_iHeight)
		iClipBottom = (iYOff + copy.m_iHeight) - m_iHeight;
// Set up the params
	// Input
	const int iInputLine = (copy.m_iWidth - iClipRight) * 3;
	const int iInputHeight = copy.m_iHeight - iClipBottom;
	const int iInputPitch = copy.m_iLineSize * 4;
	unsigned char *pIData = (unsigned char*)copy.m_pData;
	// Output
	const int iOutputPitch = m_iLineSize * 4;
	unsigned char *pOData = (unsigned char*)m_pData;
	pOData += (iXOff + iClipLeft) * 3;
	pOData += iOutputPitch * (iYOff + iClipTop);
// Copy the image
	register int iY = iInputHeight;
	while (iY--) {
		memcpy(pOData, pIData, (size_t)iInputLine);
		pIData += iInputPitch;
		pOData += iOutputPitch;
	}
} // Paste

void CImage::Flip() {
// Save the original image details
	const int iLineSize = m_iLineSize * 4;
	unsigned long *pInputData = (unsigned long*)m_pData;
	unsigned long *pIData = pInputData + (m_iHeight - 1) * m_iLineSize;
// Create the new image
	IRESULT eResult = CreateImage();
// If image allocated ok, begin quantising
	if (eResult == I_OK) {
		unsigned long *pOData = (unsigned long*)m_pData;
	// Enter each colour into the quantiser
		register int iY = m_iHeight;
	// For each line
		while (iY--) {
			memcpy(pOData, pIData, iLineSize);
		// Add the line step
			pIData -= m_iLineSize;
			pOData += m_iLineSize;
		}
	}
// Delete the original image
	delete [] pInputData;
} // Flip

//#===--- Conversion Functions

IRESULT CImage::Convert(IMAGETYPE eType, int iColours) {
// Check input image
	IRESULT eResult = I_OK;
	if ((m_iWidth <= 0) || (m_iHeight <= 0)) 
		eResult = I_NO_IMAGE;

	if (eResult == I_OK) {
// Select the correct conversion method for the current image type
		switch (m_eImageType) {
			case IT_RGB:
			// Convert RGB image to correct type
				switch (eType) {
					case IT_PALETTE:
						eResult = CreatePaletteFromRGB(iColours);
						break;
					default:
						eResult = I_UNSUPPORTED_TYPE;
				}
				break;
			default:
				eResult = I_UNSUPPORTED_TYPE;
		}
	}
	return eResult;
} // Convert

IRESULT CImage::ForceToPalette(const CImagePalette &oPalette) {
	// Check for image
	if ((m_iWidth == 0) || (m_iHeight == 0))
		return I_NO_IMAGE;
	// Check image type
	if (m_eImageType != IT_RGB)
		return I_INCORRECT_TYPE;
	// Check the palette contains colours
	int iSize = oPalette.GetSize();
	if (iSize == 0)
		return I_NO_PALETTE;
	// Save the original image details
	const int iInputLine = m_iLineSize;
	unsigned long *pInputData = (unsigned long*)m_pData;
	unsigned long *pIData = pInputData;
	unsigned long *pIDataPtr = pInputData;
	const unsigned iWidth = m_iWidth / 4;
	const unsigned int iLeft = m_iWidth % 4;
	// Create the new image
	IRESULT eResult = I_OK;
	m_eImageType = IT_PALETTE;
	eResult = CreateImage();
	// If image allocated ok, begin quantising
	if (eResult == I_OK) {
		// Get the palette
		m_pPalette->Copy(&(CImagePalette)oPalette);
		// Create the new image with the correct image indices
		const int iOutputLine = m_iLineSize * 4;
		pIData = pInputData;
		unsigned char *pOData = (unsigned char*)m_pData;
		unsigned char *pODataPtr = pOData;
		register int iY = m_iHeight, iX = iWidth;
		unsigned long uInput, uColour;
		// Retreive every pixel from the original image, and match it in BGR format
		// For each line
		while (iY--) {
			pIDataPtr = pIData;
			pODataPtr = pOData;
			iX = iWidth;
			// For each pixel in the line
			while (iX--) {
				// First pixel
				uColour = *pIDataPtr++;
				*pODataPtr = MatchColour(uColour);
				TRACE("Index %d\n", *pODataPtr);
				pODataPtr++;
				// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = MatchColour(uColour);
				TRACE("Index %d\n", *pODataPtr);
				pODataPtr++;
				// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = MatchColour(uColour);
				TRACE("Index %d\n", *pODataPtr);
				pODataPtr++;
				// Fourth pixel
				uColour = uInput >> 8;
				*pODataPtr = MatchColour(uColour);
				TRACE("Index %d\n", *pODataPtr);
				pODataPtr++;
			}
			// For any remaining pixels
			if (iLeft > 0) {
				// First pixel
				uColour = *pIDataPtr++;
				*pODataPtr = MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 1) {
				// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 2) {
				// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = MatchColour(uColour);
				pODataPtr++;
			}
			// Add the line steps
			pIData += iInputLine;
			pOData += iOutputLine;
		}
	}
	// Delete the original image
	delete [] pInputData;
	return I_OK;
} // ForceToPalette

//#===--- Colour Quantisation

IRESULT CImage::CreatePaletteFromRGB(int iColours) {
	IRESULT eResult = I_OK;
// Create colour quantiser
	CColourQuant oQuant;
	iColours = (iColours < 1) ? 1 : (iColours > 256) ? 256 : iColours;
	oQuant.SetSize(iColours);
// Save the original image details
	const int iInputLine = m_iLineSize;
	unsigned long *pInputData = (unsigned long*)m_pData;
	unsigned long *pIData = pInputData;
	unsigned long *pIDataPtr = pInputData;
	const unsigned iWidth = m_iWidth / 4;
	const unsigned int iLeft = m_iWidth % 4;
// Create the new image
	m_eImageType = IT_PALETTE;
	eResult = CreateImage();
// If image allocated ok, begin quantising
	if (eResult == I_OK) {
	// Enter each colour into the quantiser
		register int iY = m_iHeight, iX = m_iLineSize;
		unsigned long uInput, uColour;
	// For each line
		while (iY--) {
			pIDataPtr = pIData;
			iX = iWidth;
		// For each three words (12 bytes) in the line add the pixel in BGR format
			while (iX--) {
			// First pixel
				uColour = *pIDataPtr++;
				oQuant.AddColour(uColour);
			// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				oQuant.AddColour(uColour);
			// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				oQuant.AddColour(uColour);
			// Fourth pixel
				uColour = uInput >> 8;
				oQuant.AddColour(uColour);
			}
		// For any remaining pixels
			if (iLeft > 0) {
			// First pixel
				uColour = *pIDataPtr++;
				oQuant.AddColour(uColour);
			}
			if (iLeft > 1) {
			// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				oQuant.AddColour(uColour);
			}
			if (iLeft > 2) {
			// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				oQuant.AddColour(uColour);
			}
		// Add the line step
			pIData += iInputLine;
		}
	// Generate the palette
		oQuant.GeneratePalette();
	// Get the palette
		m_pPalette->Copy(oQuant.GetPalette());
	// Create the new image with the correct image indices
		const int iOutputLine = m_iLineSize * 4;
		pIData = pInputData;
		unsigned char *pOData = (unsigned char*)m_pData;
		unsigned char *pODataPtr = pOData;
		iY = m_iHeight;
	// Retreive every pixel from the original image, and match it in BGR format
	// For each line
		while (iY--) {
			pIDataPtr = pIData;
			pODataPtr = pOData;
			iX = iWidth;
		// For each pixel in the line
			while (iX--) {
			// First pixel
				uColour = *pIDataPtr++;
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			// Fourth pixel
				uColour = uInput >> 8;
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			}
		// For any remaining pixels
			if (iLeft > 0) {
			// First pixel
				uColour = *pIDataPtr++;
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 1) {
			// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 2) {
			// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = oQuant.MatchColour(uColour);
				pODataPtr++;
			}
		// Add the line steps
			pIData += iInputLine;
			pOData += iOutputLine;
		}
	}
// Delete the original image
	delete [] pInputData;
	return eResult;
} // CreatePaletteFromRGB

int CImage::MatchColour(unsigned long uColour) {
	// Make sure it's a palettised image
	if (m_eImageType != IT_PALETTE)
		return -1;
	// Match the colour
	int iCount = 0;
	unsigned long uPal = 0;
	int iVal = -1;
	int iLowestError = 8192;	// Huge error
	while (iCount < m_pPalette->GetSize()) {
		// Get the colour
		m_pPalette->GetEntry(iCount, uPal);
		// Calculate the error
		int iR = (uColour >> 16) & 0x000000FF;
		int iG = (uColour >>  8) & 0x000000FF;
		int iB = (uColour      ) & 0x000000FF;
		iR = abs(iR - ((uPal      ) & 0x000000FF));
		iG = abs(iG - ((uPal >>  8) & 0x000000FF));
		iB = abs(iB - ((uPal >> 16) & 0x000000FF));
		int iError = iR + iG + iB;
		// Record if lowest error
		if (iError < iLowestError) {
			iLowestError = iError;
			iVal = iCount;
		}
		// Carry on through the palette
		iCount++;
	}
	ASSERT(iVal != 255);
	return iVal;
} // MatchColour

//#===--- Scaling Functions

IRESULT CImage::Scale(int iWidth, int iHeight, IMAGEFILTERTYPE eFilter) {
	ASSERT(m_iWidth && m_iHeight);
	IRESULT eResult = I_OK;
// Validate the new image size
	if ((iWidth < 0) || (iWidth > IMG_MAXWIDTH))
		iWidth = 0;
	if ((iHeight < 0) || (iHeight > IMG_MAXHEIGHT))
		iHeight = 0;
	if ((iWidth == 0) || (iHeight == 0))
		eResult = I_INVALID_PARAM;

// Begin scaling if okay
	if (eResult == I_OK) {
	// Calculate the differences
		int iWidthDif = iWidth - m_iWidth;
		int iHeightDif = iHeight - m_iHeight;
	// Arrange scaling passes in most efficient order
		if (iWidthDif < iHeightDif) {
			if (iWidthDif != 0) 
				eResult = ScaleHorizontal(iWidth, eFilter);
			if ((eResult == I_OK) && (iHeightDif != 0)) {
				eResult = ScaleVertical(iHeight, eFilter);
			}
		}
		else {
			if (iHeightDif != 0)
				eResult = ScaleVertical(iHeight, eFilter);
			if ((eResult == I_OK) && (iWidthDif != 0)) {
				eResult = ScaleHorizontal(iWidth, eFilter);
			}
		}
	}

	return eResult;
} // Scale

IRESULT CImage::ScaleHorizontal(int iWidth, IMAGEFILTERTYPE eFilter) {
	IRESULT eResult = I_UNSUPPORTED_TYPE;
	if (m_eImageType == IT_RGB)
		eResult = ScaleHorizontalRGB(iWidth, eFilter);
	return eResult;
} // ScaleHorizontal

IRESULT CImage::ScaleVertical(int iHeight, IMAGEFILTERTYPE eFilter) {
	IRESULT eResult = I_UNSUPPORTED_TYPE;
	if (m_eImageType == IT_RGB)
		eResult = ScaleVerticalRGB(iHeight, eFilter);
	return eResult;
} // ScaleVertical

CImage::SContribList *CImage::AllocateContributions(int iWidth, int iWindow) {
// Allocate the contribution list
	SContribList *pList = NULL;
// The list structure
	NEWBEGIN
	pList = (SContribList*) new SContribList;
	NEWEND("CImage::AllocateContributions - Contribution list")
	if (pList) {
		pList->m_iWidth = iWidth;
		pList->m_iWindow = iWindow;
	// The line list
		NEWBEGIN
		pList->m_pContrib = (SContrib*) new SContrib[iWidth];
		NEWEND("CImage::AllocateContributions - Contributions")
		if (pList->m_pContrib) {
			int iCount = 0;
			while (iCount < iWidth) {
			// The line weights
				NEWBEGIN
				pList->m_pContrib[iCount].m_pWeights = (float*) new float[iWindow];
				NEWEND("CImage::AllocateContributions - Contribution weights")
				iCount++;
			}
		// Delete the remaining line widths
			if (iCount < iWidth) {
				while (iCount >= 0) {
					iCount--;
					delete [] pList->m_pContrib[iCount].m_pWeights;
				}
				delete [] pList->m_pContrib;
				delete pList;
			}
		}
	// Delete the list object
		else {
			delete pList;
		}
	}
	return pList;
} // AllocateContributions

CImage::SContribList *CImage::CalculateContributions(int iInputWidth, int iOutputWidth, IMAGEFILTERTYPE eFilter) {
	SContribList *pList = NULL;
// Allocate the filter
	CBaseFilter *pFilter = NULL;
	switch (eFilter) {
		case IF_BOX:
			NEWBEGIN
			pFilter = (CBaseFilter*) new CBoxFilter;
			NEWEND("CImage::CalculateContributions - Image filter")
			break;
		case IF_BILINEAR:
			NEWBEGIN
			pFilter = (CBaseFilter*) new CBilinearFilter;
			NEWEND("CImage::CalculateContributions - Image filter")
			break;
		default:
			ASSERT(false);
	}

// Fill the contribution list
	if (pFilter) {
	// Set the vars
		float fScale = float(iOutputWidth) / float(iInputWidth);
		float fWidth = 0.0F;
		float fFScale = 1.0F;
		const float fFilterWidth = pFilter->Width();
		if (fScale < 1.0) {
		// Minification
			fWidth = fFilterWidth / fScale; 
			fFScale /= fScale; 
		}
		else {
		// Magnification
			fWidth = fFilterWidth; 
    }

	// Window size is the number of sampled pixels
		int iWindow = 2 * ((int)ceil(fWidth) + 1); 

	// Allocate the contributions
		pList = AllocateContributions(iOutputWidth, iWindow); 

		if (pList) {
			for (int i = 0; i < iOutputWidth; i++) {
			// Scan through line of contributions
				float fCenter = float(i) / fScale;   // Reverse mapping

			// Find the significant edge points that affect the pixel
				int iLeft = (int) floor(fCenter - fWidth); 
				if (iLeft < 0) 
					iLeft = 0;
				pList->m_pContrib[i].m_iLeft = iLeft; 

				int iRight = (int) ceil(fCenter + fWidth); 
				if (iRight >= iInputWidth)
					iRight = iInputWidth - 1;
				pList->m_pContrib[i].m_iRight = iRight;

				float fTotalWeight = 0.0;  // Zero sum of weights
				for (int iSrc = iLeft; iSrc <= iRight; iSrc++) {
				// Calculate weights
					float fWeight = (fCenter - float(iSrc));
					fWeight /= fWidth;
					fWeight = pFilter->Filter(fWeight);
					fWeight /= fFScale;
					fTotalWeight += fWeight;
					pList->m_pContrib[i].m_pWeights[iSrc - iLeft] = fWeight;
				}
				ASSERT (fTotalWeight >= 0.0);   // An error in the filter function can cause this 
				if (fTotalWeight > 0.0) {
				// Normalize weight of neighbouring points
					for (iSrc = iLeft; iSrc <= iRight; iSrc++)
						pList->m_pContrib[i].m_pWeights[iSrc - iLeft] /= fTotalWeight;
				} 
			}
		}
		delete pFilter;
	}

	return pList;
} // CalculateContributions

void CImage::DeleteContributions(SContribList *pList) {
	ASSERT(pList);
// Delete the weights
	for (int i = 0; i < pList->m_iWidth; i++) {
		delete [] pList->m_pContrib[i].m_pWeights;
	}
// Delete the list
	delete [] pList->m_pContrib;
// Delete the object
	delete pList;
} // DeleteContributions

IRESULT CImage::ScaleHorizontalRGB(int iWidth, IMAGEFILTERTYPE eFilter) {
// Remember the current image details
	const int iOriginalWidth = m_iWidth;
	const UINT iLineSize = m_iLineSize;
	UINT *pInputData = m_pData;
// Allocate new image
	m_iWidth = iWidth;
	IRESULT eResult = CreateImage();
	if (eResult == I_OK) {
	// Calculate the weights
		SContribList *pList = CalculateContributions(iOriginalWidth, m_iWidth, eFilter);
		if (pList) {
		// Loop vars
			unsigned char *pIData = (unsigned char*)pInputData;
			unsigned char *pIDataPtr = pIData;
			const unsigned int iIPitch = iLineSize * 4;
			unsigned char *pOData = (unsigned char*)m_pData;
			unsigned char *pODataPtr = pOData;
			const unsigned int iOPitch = m_iLineSize * 4;
			unsigned char cR = 0, cG = 0, cB = 0;
			register int iY = m_iHeight, iX = m_iWidth, iPos = 0;
			int iLeft = 0, iRight = 0;
		// Loop through rows
			while (iY--) {
			// Set the image pointers
				pIDataPtr = pIData;
				pODataPtr = pOData;
			// Loop through pixels
				for (iX = 0; iX < m_iWidth; iX++) {
				// Initialise the colour
					cR = cG = cB = 0;
				// Retrieve the boundaries
					iLeft = pList->m_pContrib[iX].m_iLeft;
					iRight = pList->m_pContrib[iX].m_iRight;
				// Add the contributions
					for (iPos = iLeft; iPos <= iRight; iPos++) {
            cR += (unsigned char)(pList->m_pContrib[iX].m_pWeights[iPos - iLeft] * float(pIData[iPos * 3])); 
            cG += (unsigned char)(pList->m_pContrib[iX].m_pWeights[iPos - iLeft] * float(pIData[iPos * 3 + 1])); 
            cB += (unsigned char)(pList->m_pContrib[iX].m_pWeights[iPos - iLeft] * float(pIData[iPos * 3 + 2])); 
					}
				// Write the colour
					*pODataPtr++ = cR;
					*pODataPtr++ = cG;
					*pODataPtr++ = cB;
				}
			// Add the line steps
				pIData += iIPitch;
				pOData += iOPitch;
			}
		// Delete the weights
			DeleteContributions(pList);
		}
	// Delete the old image
		delete [] pInputData;
	}

	return eResult;
} // ScaleHorizontalRGB

IRESULT CImage::ScaleVerticalRGB(int iHeight, IMAGEFILTERTYPE eFilter) {
// Remember the current image details
	const int iOriginalHeight = m_iHeight;
	const UINT iLineSize = m_iLineSize;
	UINT *pInputData = m_pData;
// Allocate new image
	m_iHeight = iHeight;
	IRESULT eResult = CreateImage();
	if (eResult == I_OK) {
	// Calculate the weights
		SContribList *pList = CalculateContributions(iOriginalHeight, m_iHeight, eFilter);
		if (pList) {
		// Loop vars
			unsigned char *pIData = (unsigned char*)pInputData;
			unsigned char *pIDataPtr = pIData;
			const unsigned int iIPitch = iLineSize * 4;
			unsigned char *pOData = (unsigned char*)m_pData;
			unsigned char *pODataPtr = pOData;
			const unsigned int iOPitch = m_iLineSize * 4;
			unsigned char cR = 0, cG = 0, cB = 0;
			register int iY = m_iHeight, iX = m_iWidth, iPos = 0;
			int iLeft = 0, iRight = 0;
		// Loop through columns
			while (iX--) {
			// Set the image pointers
				pIDataPtr = pIData;
				pODataPtr = pOData;
			// Loop through rows
				for (iY = 0; iY < m_iHeight; iY++) {
				// Initialise the colour
					cR = cG = cB = 0;
				// Retrieve the boundaries
					iLeft = pList->m_pContrib[iY].m_iLeft;
					iRight = pList->m_pContrib[iY].m_iRight;
				// Add the contributions
					for (iPos = iLeft; iPos <= iRight; iPos++) {
            cR += (unsigned char)(pList->m_pContrib[iY].m_pWeights[iPos - iLeft] * float(pIData[iPos * iIPitch])); 
            cG += (unsigned char)(pList->m_pContrib[iY].m_pWeights[iPos - iLeft] * float(pIData[iPos * iIPitch + 1])); 
            cB += (unsigned char)(pList->m_pContrib[iY].m_pWeights[iPos - iLeft] * float(pIData[iPos * iIPitch + 2])); 
					}
				// Write the colour
					pODataPtr[0] = cR;
					pODataPtr[1] = cG;
					pODataPtr[2] = cB;
				// Add the line steps
					pIDataPtr += iIPitch;
					pODataPtr += iOPitch;
				}
			// Add the pixel steps
				pIData += 3;
				pOData += 3;
			}
		// Delete the weights
			DeleteContributions(pList);
		}
	// Delete the old image
		delete [] pInputData;
	}

	return eResult;
} // ScaleVerticalRGB

//#===--- Load/Save Functions

FRESULT CImage::Load(const char *pFname, CImageFile *pImageFile) {
// Check the image type
	IMAGETYPE eInputType;
	FRESULT eFResult = pImageFile->GetImageType(pFname, eInputType);
	if (eFResult != F_OK)
		return eFResult;
	m_eImageType = eInputType;

// Delete the original image
	if (m_pData)
		delete [] m_pData;
// Load the image
	unsigned char *pData = NULL;
	eFResult = pImageFile->Load(pFname, m_iWidth, m_iHeight, pData);
	m_pData = (UINT*)pData;

// If image loaded, adjust the other image variables
	if (eFResult == F_OK) {
		UINT iLineLength = m_iWidth;
		switch(m_eImageType) {
			case IT_MONO:
				break;
			case IT_GREY:
				break;
			case IT_PALETTE:
				break;
			case IT_RGB:
				iLineLength *= 3;
				break;
			default:
				ASSERT(false);
		}
		m_iLineSize = iLineLength / 4;
		if (iLineLength % 4)
			m_iLineSize++;
		m_iDataSize = m_iLineSize * m_iHeight;
	}
	else {
		m_iWidth = m_iHeight = 0;
		m_iLineSize = m_iDataSize = 0;
		m_pData = NULL;
	}

	return eFResult;
} // Load

FRESULT CImage::Save(const char *pFname, CImageFile *pImageFile) const {
// Set the image type
	pImageFile->SetSaveType(m_eImageType);

// Check if CImage contains an image
	if ((!m_iWidth) || (!m_iHeight))
		return F_NODATATOSAVE;

// If exists, set palette
	if ((m_eImageType == IT_PALETTE) && m_pPalette)
		pImageFile->SetPalette(m_pPalette);

	FRESULT fResult = pImageFile->Save(pFname, m_iWidth, m_iHeight, (unsigned char*)m_pData);

	return fResult;
} // Save

//#===--- Import

IRESULT CImage::ImportAMETexture(stAMETexture &sAME) {
// Set the new image size
	m_eImageType = IT_RGB;
	IRESULT eIResult = SetSize(sAME.iWidth, sAME.iHeight);
	if (eIResult == I_OK) {
		ASSERT(sAME.ptrRGBData != NULL);
	// Copy the image data
		const int iInputLine = m_iWidth * 3;
		const int iOutputLine = m_iLineSize * 4;
		unsigned char *pIData = sAME.ptrRGBData;
		unsigned char *pOData = (unsigned char*)m_pData;
	// Copy each line
		register int iY = m_iHeight;
		while (iY--) {
		// Copy the line
			memcpy(pOData, pIData, iInputLine);
		// Add the line steps
			pIData += iInputLine;
			pOData += iOutputLine;
		}
	}
	return eIResult;
} // ImportAMETexture


