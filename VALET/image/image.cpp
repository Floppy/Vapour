//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

//#===--- Includes
#include "image.h"
#include "imagefile.h"
#include "filter.h"
//#include "ImageFile.h"
//#include "ColourQuant.h"

#include <fstream.h>
#include <string.h>

////////////////
//! file 		= "VALET/image/image.cpp"
//! author		= "Warren Moore"
//! date			= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid		= "$Id: image.cpp,v 1.1 2001/10/18 11:10:41 vap-warren Exp $"

namespace NValet {

   //#===--- CImage

   CImage::CImage(IMAGETYPE eType, int iWidth, int iHeight, int iColours) :
      m_eImageType(eType),
      m_iWidth(iWidth),
      m_iHeight(iHeight),
      m_pData(NULL),
      m_poPalette(NULL)
   {
	   CreateImage();
   } // CImage::Constructor

   CImage::CImage(const CImage &oCopy) :
      m_eImageType(oCopy.m_eImageType),
      m_iWidth(oCopy.m_iWidth),
      m_iHeight(oCopy.m_iHeight)
   {
      // Allocate the memory
      IRESULT eResult = CreateImage();
      // If image data has been allocated ok, copy over
      if (eResult == I_OK) {
         memcpy(m_pData, oCopy.m_pData, m_iDataSize * 4);
         if ((m_eImageType == IT_PALETTE) && (oCopy.m_poPalette))
            m_poPalette->Copy(oCopy.m_poPalette);
      }
   } // CImage::Copy Constructor

/*
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
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_poPalette))
			m_poPalette->Copy(oCopy.m_poPalette);
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
		m_poPalette = NULL;
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
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_poPalette))
			m_poPalette->Copy(oCopy.m_poPalette);
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
		m_poPalette = NULL;
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
		if ((m_eImageType == IT_PALETTE) && (oCopy.m_poPalette))
			m_poPalette->Copy(oCopy.m_poPalette);
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
		m_poPalette = NULL;
	}
} // Conversion Copy Constructor
*/

   CImage::~CImage() {
      delete [] m_pData;
      delete m_poPalette;
   } // CImage::Destructor

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
            m_pData = (unsigned int*) new unsigned int[m_iDataSize];
            if (!m_pData)
               eResult = I_OUT_OF_MEMORY;
         }
      }

      // If necessary, create the palette
      m_poPalette = NULL;
      if ((m_eImageType == IT_PALETTE) && (eResult == I_OK)) {
         m_poPalette = (CImagePalette*) new CImagePalette;
         if (!m_poPalette)
            eResult = I_OUT_OF_MEMORY;
      }

      // If allocation or a size parameter failed, zero everything
      if (eResult != I_OK) {
         m_iWidth = 0;
         m_iHeight = 0;
         m_iDataSize = 0;
         m_iLineSize = 0;
         delete [] m_pData;
         m_pData = NULL;
         delete m_poPalette;
         m_poPalette = NULL;
      }

      return eResult;
   } // CImage::CreateImage

   IMAGETYPE CImage::GetType() const {
      return m_eImageType;
   } // CImage::GetType

   void CImage::GetSize(int &iWidth, int &iHeight) const {
      iWidth = m_iWidth;
      iHeight = m_iHeight;
   } // CImage::GetSize

   IRESULT CImage::SetSize(int iWidth, int iHeight) {
      // Set the image size
      m_iWidth = iWidth;
      m_iHeight = iHeight;

      delete [] m_pData;
      m_pData = NULL;

      // Allocate the new image memory
      IRESULT eResult = CreateImage();

      return eResult;
   } // CImage::SetSize

   void CImage::Clear() {
      // Clear the image memory (set to black)
      if (m_pData)
         memset(m_pData, 0, m_iDataSize * 4);
   } // CImage::Clear

/*
void CImage::Paste(const CImage &oCopy, int iXOff, int iYOff) {
	if ((m_eImageType != IT_RGB) || (oCopy.m_eImageType != IT_RGB)) 
		return;
	// Make sure we don't go over the sides
	int iClipLeft = 0, iClipRight = 0, iClipTop = 0, iClipBottom = 0;
	if (iXOff < 0)
		iClipLeft = -iXOff;
	if ((iXOff + oCopy.m_iWidth) > m_iWidth)
		iClipRight = (iXOff + oCopy.m_iWidth) - m_iWidth;
	if (iYOff < 0)
		iClipTop = -iYOff;
	if ((iYOff + oCopy.m_iHeight) > m_iHeight)
		iClipBottom = (iYOff + oCopy.m_iHeight) - m_iHeight;
	// Set up the params
		// Input
	const int iInputLine = (oCopy.m_iWidth - iClipRight) * 3;
	const int iInputHeight = oCopy.m_iHeight - iClipBottom;
	const int iInputPitch = oCopy.m_iLineSize * 4;
	unsigned char *pIData = (unsigned char*)oCopy.m_pData;
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

void CImage::Paste(const CImage &oCopy, float fAlpha, bool bTrans, unsigned int uColour, int iXOff, int iYOff) {
	if ((m_eImageType != IT_RGB) || (oCopy.m_eImageType != IT_RGB)) 
		return;
	// Make sure we don't go over the sides
	int iClipLeft = 0, iClipRight = 0, iClipTop = 0, iClipBottom = 0;
	if (iXOff < 0)
		iClipLeft = -iXOff;
	if ((iXOff + oCopy.m_iWidth) > m_iWidth)
		iClipRight = (iXOff + oCopy.m_iWidth) - m_iWidth;
	if (iYOff < 0)
		iClipTop = -iYOff;
	if ((iYOff + oCopy.m_iHeight) > m_iHeight)
		iClipBottom = (iYOff + oCopy.m_iHeight) - m_iHeight;
	// Set up the input params
	const int iInputWidth = oCopy.m_iWidth - iClipRight;
	const int iInputLine = iInputWidth * 3;
	const int iInputHeight = oCopy.m_iHeight - iClipBottom;
	const int iInputPitch = oCopy.m_iLineSize * 4;
	unsigned char *pIData = (unsigned char*)oCopy.m_pData;
	unsigned char *pIDataPtr = pIData;
	// Set up the output params
	const int iOutputPitch = m_iLineSize * 4;
	unsigned char *pOData = (unsigned char*)m_pData;
	unsigned char *pODataPtr = pOData;
	pOData += (iXOff + iClipLeft) * 3;
	pOData += iOutputPitch * (iYOff + iClipTop);
	// Alpha and transparency params
	const unsigned int uTR = (uColour & 0x00FF0000) >> 16;
	const unsigned int uTG = (uColour & 0x0000FF00) >> 8;
	const unsigned int uTB = (uColour & 0x000000FF);
	if (fAlpha > 1.0F)
		fAlpha = 1.0F;
	if (fAlpha <= 0.0F)
		return;
	const float fOAlpha = 1.0F - fAlpha;
	const bool bAlpha = fAlpha < 1.0F;
	// Loop params
	register int iX = iInputWidth, iY = iInputHeight;
	unsigned int uR, uG, uB;
	// Copy the image
	while (iY--) {
		// Set the line pointers
		pIDataPtr = pIData;
		pODataPtr = pOData;
		iX = iInputWidth;
		// Loop through each pixel
		while (iX--) {
			// Get the input data
			uR = (unsigned int)*pIDataPtr++;
			uG = (unsigned int)*pIDataPtr++;
			uB = (unsigned int)*pIDataPtr++;
			// Check if its a transparent pixel
			if ((!bTrans) || (uR != uTR) || (uB != uTB) || (uG != uTB)) {
				// Sort out the alpha
				if (bAlpha) {
					// Red pixel
					uR = unsigned int((fOAlpha * (float(pODataPtr[0]))) + (fAlpha * (float(uR))));
					if (uR > 255)
						uR = 255;
					// Green pixel
					uG = unsigned int((fOAlpha * (float(pODataPtr[1]))) + (fAlpha * (float(uG))));
					if (uG > 255)
						uG = 255;
					// Red pixel
					uB = unsigned int((fOAlpha * (float(pODataPtr[2]))) + (fAlpha * (float(uB))));
					if (uB > 255)
						uB = 255;
				}
				// Write out the pixels
				pODataPtr[0] = (unsigned char)uR;
				pODataPtr[1] = (unsigned char)uG;
				pODataPtr[2] = (unsigned char)uB;
			}
			// Step to the next pixel
			pODataPtr += 3;
		}
		// Add the line step
		pIData += iInputPitch;
		pOData += iOutputPitch;
	}
} // Paste (Transparenct and alpha)

void CImage::Flip() {
	// Save the original image details
	const int iLineSize = m_iLineSize * 4;
	unsigned int *pInputData = (unsigned int*)m_pData;
	unsigned int *pIData = pInputData + (m_iHeight - 1) * m_iLineSize;
	// Create the new image
	IRESULT eResult = CreateImage();
	// If image allocated ok, begin quantising
	if (eResult == I_OK) {
		unsigned int *pOData = (unsigned int*)m_pData;
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

unsigned int CImage::GetPixel(int iX, int iY) const {
	unsigned int uPixel = iY*m_iWidth + iX;
	return GetPixel(uPixel);
}

unsigned int CImage::GetPixel(unsigned int uPixel) const {
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

	//#===--- Conversion Functions

const CImagePalette* CImage::GetPalette(void) const {
	return m_poPalette;
}

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

IRESULT CImage::ForceToPalette(CImagePalette &oPalette) {
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
	unsigned int *pInputData = (unsigned int*)m_pData;
	unsigned int *pIData = pInputData;
	unsigned int *pIDataPtr = pInputData;
	const unsigned iWidth = m_iWidth / 4;
	const unsigned int iLeft = m_iWidth % 4;
	// Create the new image
	IRESULT eResult = I_OK;
	m_eImageType = IT_PALETTE;
	eResult = CreateImage();
	// If image allocated ok, begin quantising
	if (eResult == I_OK) {
		// Get the palette
		m_poPalette->Copy(&(CImagePalette)oPalette);
		// Create the new image with the correct image indices
		const int iOutputLine = m_iLineSize * 4;
		pIData = pInputData;
		unsigned char *pOData = (unsigned char*)m_pData;
		unsigned char *pODataPtr = pOData;
		register int iY = m_iHeight, iX = iWidth;
		unsigned int uInput, uColour;
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
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
				// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
				// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
				// Fourth pixel
				uColour = uInput >> 8;
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
			}
			// For any remaining pixels
			if (iLeft > 0) {
				// First pixel
				uColour = *pIDataPtr++;
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 1) {
				// Second pixel
				uInput = *pIDataPtr++;
				uColour = (uColour >> 24) | (uInput << 8);
				*pODataPtr = oPalette.MatchColour(uColour);
				pODataPtr++;
			}
			if (iLeft > 2) {
				// Third pixel
				uColour = uInput >> 16;
				uInput = *pIDataPtr++;
				uColour |= (uInput << 16);
				*pODataPtr = oPalette.MatchColour(uColour);
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

IRESULT CImage::ForceType(IMAGETYPE eType, int iColours) {
	// Changing the palette size
	if ((m_eImageType == IT_PALETTE) && (eType == IT_PALETTE) && (iColours != m_poPalette->GetSize())) {
		m_poPalette->SetSize(iColours);
		return I_OK;
	}
	// Get out quick if on change required
	if (eType == m_eImageType)
		return I_OK;
	// Delete any current memory
	if (m_pData) {
		delete [] m_pData;
		m_pData = NULL;
	}
	if (m_poPalette) {
		delete m_poPalette;
		m_poPalette = NULL;
	}
	// Change the type and sort out memory and vars
	m_eImageType = eType;
	IRESULT eResult = CreateImage();
	// Set the palette size, if IT_PALETTE
	if ((m_eImageType == IT_PALETTE) && (eResult == I_OK))
		m_poPalette->SetSize(iColours);
	return eResult;
} // ForceType

//#===--- Colour Quantisation

IRESULT CImage::CreatePaletteFromRGB(int iColours) {
	IRESULT eResult = I_OK;
	// Create colour quantiser
	CColourQuant oQuant;
	iColours = (iColours < 1) ? 1 : (iColours > 256) ? 256 : iColours;
	oQuant.SetSize(iColours);
	// Save the original image details
	const int iInputLine = m_iLineSize;
	unsigned int *pInputData = (unsigned int*)m_pData;
	unsigned int *pIData = pInputData;
	unsigned int *pIDataPtr = pInputData;
	const unsigned iWidth = m_iWidth / 4;
	const unsigned int iLeft = m_iWidth % 4;
	// Create the new image
	m_eImageType = IT_PALETTE;
	eResult = CreateImage();
	// If image allocated ok, begin quantising
	if (eResult == I_OK) {
		// Enter each colour into the quantiser
		register int iY = m_iHeight, iX = m_iLineSize;
		unsigned int uInput, uColour;
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
		m_poPalette->Copy(oQuant.GetPalette());
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
	pList = (SContribList*) new SContribList;
	if (pList) {
		pList->m_iWidth = iWidth;
		pList->m_iWindow = iWindow;
		// The line list
		pList->m_pContrib = (SContrib*) new SContrib[iWidth];
		if (pList->m_pContrib) {
			int iCount = 0;
			while (iCount < iWidth) {
				// The line weights
				pList->m_pContrib[iCount].m_pWeights = (float*) new float[iWindow];
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
			pFilter = (CBaseFilter*) new CBoxFilter;
			break;
		case IF_BILINEAR:
			pFilter = (CBaseFilter*) new CBilinearFilter;
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
				float fCenter = float(i) / fScale;   	// Reverse mapping

				// Find the significant edge points that affect the pixel
				int iLeft = (int) floor(fCenter - fWidth); 
				if (iLeft < 0) 
					iLeft = 0;
				pList->m_pContrib[i].m_iLeft = iLeft; 

				int iRight = (int) ceil(fCenter + fWidth); 
				if (iRight >= iInputWidth)
					iRight = iInputWidth - 1;
				pList->m_pContrib[i].m_iRight = iRight;

				float fTotalWeight = 0.0;  	// Zero sum of weights
				for (int iSrc = iLeft; iSrc <= iRight; iSrc++) {
					// Calculate weights
					float fWeight = (fCenter - float(iSrc));
					fWeight /= fWidth;
					fWeight = pFilter->Filter(fWeight);
					fWeight /= fFScale;
					fTotalWeight += fWeight;
					pList->m_pContrib[i].m_pWeights[iSrc - iLeft] = fWeight;
				}
				ASSERT (fTotalWeight >= 0.0);   	// An error in the filter function can cause this 
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

void CImage::DeleteContributions(SContribList *psList) {
	ASSERT(psList);
	// Delete the weights
	for (int i = 0; i < psList->m_iWidth; i++) {
		delete [] psList->m_pContrib[i].m_pWeights;
	}
	// Delete the list
	delete [] psList->m_pContrib;
	// Delete the object
	delete psList;
} // DeleteContributions

IRESULT CImage::ScaleHorizontalRGB(int iWidth, IMAGEFILTERTYPE eFilter) {
	// Remember the current image details
	const int iOriginalWidth = m_iWidth;
	const unsigned int iLineSize = m_iLineSize;
	unsigned int *pInputData = m_pData;
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
	const unsigned int iLineSize = m_iLineSize;
	unsigned int *pInputData = m_pData;
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
*/
//#===--- Load/Save Functions

FRESULT CImage::Load(const char *pFname, CImageFile *pImageFile) {
	// Check the image type
	IMAGETYPE eInputType = IT_UNKNOWN;
	FRESULT eFResult = pImageFile->GetImageType(pFname, eInputType);
	if (eFResult != F_OK)
		return eFResult;
	if (eInputType != IT_UNKNOWN)
		m_eImageType = eInputType;

	// Delete the original image
	if (m_pData)
		delete [] m_pData;
	// Load the image
	unsigned char *pData = NULL;
	eFResult = pImageFile->Load(pFname, m_iWidth, m_iHeight, pData);
	m_pData = (unsigned int*)pData;

	// If image loaded, adjust the other image variables
	if (eFResult == F_OK) {
		unsigned int iLineLength = m_iWidth;
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
		return F_NO_DATA_TO_SAVE;

	// If exists, set palette
	if ((m_eImageType == IT_PALETTE) && m_poPalette)
		pImageFile->SetPalette(m_poPalette);

	FRESULT fResult = pImageFile->Save(pFname, m_iWidth, m_iHeight, (unsigned char*)m_pData);

	return fResult;
} // Save

/*
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

IRESULT CImage::ImportRawImage(unsigned char *pcRaw, bool bWordAlign, bool bReversePixels, bool bFlip) {
	ASSERT(pcRaw);
	// Make sure we have input data
	if (!pcRaw)
		return I_NO_DATA;
	// Make sure the image is already set up
	if (m_iWidth * m_iHeight == 0)
		return I_NO_IMAGE;
	// Check for supported image types
	if ((m_eImageType != IT_RGB) &&
			(m_eImageType != IT_PALETTE) &&
			(m_eImageType != IT_GREY))
			return I_UNSUPPORTED_TYPE;
	// Calculate input line length
	unsigned int uIWidth = m_iWidth;
	if (m_eImageType == IT_RGB)
		uIWidth *= 3;
	unsigned int uILineStep = uIWidth;
	if (bWordAlign && (uILineStep % 4))
		uILineStep += (4 - (uILineStep % 4));
	// Calculate the image object line step
	unsigned int uOLineStep = m_iLineSize * 4;
	//#===--- Loop vars
	// Input image line pointers
	unsigned char *pcIData = pcRaw;
	if (bFlip) {
		pcIData += uILineStep * (m_iHeight - 1);
		uILineStep = -uILineStep;
	}
	unsigned char *pcIDataPtr = pcIData;
	// Image object line pointers
	unsigned char *pcOData = (unsigned char*)m_pData;
	unsigned char *pcODataPtr = pcOData;
	// Loop vars
	register unsigned int uX = m_iWidth, uY = m_iHeight;
	// Reverse pixels
	if (bReversePixels) {
		while (uY--) {
			// Set the line pointers
			pcIDataPtr = pcIData;
			pcODataPtr = pcOData;
			uX = m_iWidth;
			while (uX--) {
				// Read the pixels out in reverse order
				pcODataPtr[2] = *pcIDataPtr++;
				pcODataPtr[1] = *pcIDataPtr++;
				pcODataPtr[0] = *pcIDataPtr++;
				pcODataPtr += 3;
			}
			// Add the line step
			pcIData += uILineStep;
			pcOData += uOLineStep;
		}
	}
	// Correct order pixels
	else {
		while (uY--) {
			// Read the pixels over
			memcpy(pcOData, pcIData, uIWidth);
			// Add the line step
			pcIData += uILineStep;
			pcOData += uOLineStep;
		}
	}
	return I_OK;
} // ImportRawImage

*/

}
