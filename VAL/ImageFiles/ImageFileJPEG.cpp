//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileJPEG.cpp - 21/12/1999 - Warren Moore
//	JPEG file format image implementation
//
// $Id: ImageFileJPEG.cpp,v 1.1 2000/06/16 22:00:32 waz Exp $
//

#include "stdafx.h"

#define _IMAGEFILEJPEG_INTERNAL_
#include "ImageFileJPEG.h"
#include "ImageFileProxy.h"

#include <stdio.h>
#include "jpeglib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

///////////////////
// CImageFileJPEG

CImageFileProxy<CImageFileJPEG> g_ImageProxyJPEG;

// Store Functions

CImageFileJPEG::CImageFileJPEG() {
	m_eSaveType = IT_RGB;
} // Constructor

const char *CImageFileJPEG::GetFileExtension() const {
	return GetExtension();
} // GetFileExtension

const char *CImageFileJPEG::GetFileTitle() const {
	return GetTitle();
} // GetFileExtension

// ImageFile Functions

FRESULT CImageFileJPEG::GetImageType(const char *pFname, IMAGETYPE &eImgType) {
// Open the file, don't create if it doesn't exist
	FILE *pFile =	fopen(pFname, "rb");
	if (!pFile)
		return F_DOESNOTEXIST;

// jpeg lib structures
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

// Set the error routines
  cinfo.err = jpeg_std_error(&jerr);

// Initialise decompression object
  jpeg_create_decompress(&cinfo);

// Set the input source
  jpeg_stdio_src(&cinfo, pFile);

// Read the image header
  jpeg_read_header(&cinfo, TRUE);

	FRESULT eFResult = F_OK;
// Calculate image type
	if (cinfo.num_components == 1) 
		eImgType = IT_GREY;
	else
		if (cinfo.num_components == 3)
			eImgType = IT_RGB;
		else
			eFResult = F_WRONGIMAGETYPE;

// Destroy the decompression object
  jpeg_destroy_decompress(&cinfo);

// Close 
	fclose(pFile);

	return eFResult;
} // GetImageType

FRESULT CImageFileJPEG::Load(const char *pFname, int &x, int &y, unsigned char *&pData) {
// Check the file exists
	FILE *pFile = fopen(pFname, "rb");
	if (!pFile)
		return F_DOESNOTEXIST;

// jpeg lib structures
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

// Set up the error routines
  cinfo.err = jpeg_std_error(&jerr);
// Init the decompression object
  jpeg_create_decompress(&cinfo);

// Set the data source
  jpeg_stdio_src(&cinfo, pFile);

// Read the jpeg header
  (void)jpeg_read_header(&cinfo, TRUE);

// Calculate image type
	IMAGETYPE eImgType = IT_UNKNOWN;
	if (cinfo.num_components == 1) 
		eImgType = IT_GREY;
	else
		if (cinfo.num_components == 3)
			eImgType = IT_RGB;

// Start the decompression
	(void)jpeg_start_decompress(&cinfo);
	
	FRESULT eFResult = F_OK;
// Allocate image memory
  unsigned int iWidth = cinfo.output_width;
  unsigned int iLineLength = cinfo.output_width*cinfo.output_components;
	unsigned int iLineSize = iLineLength;
	if (iLineSize % 4 > 0) {
		iLineSize += 4 - (iLineSize % 4);
	}
	unsigned int iHeight = cinfo.output_height;
	unsigned int iDataSize = iLineSize * iHeight;
	unsigned char *pOImageData = NULL;
	NEWBEGIN
	pOImageData = new unsigned char[iDataSize];
	NEWEND("CImageFileJPEG::Load - Image data")
// Read the scan lines
	if (pOImageData) {
	// Allocate libjpeg scanline buffer
	  unsigned char **pBuffer = (*cinfo.mem->alloc_sarray)
	   ((j_common_ptr)&cinfo, JPOOL_IMAGE, iLineLength, 1);
	// Read in the data
		unsigned char *pOData = pOImageData;
	// Copy each line
		while (cinfo.output_scanline < iHeight) {
		// Read the scanline
			(void)jpeg_read_scanlines(&cinfo, pBuffer, 1);
			memcpy(pOData, *pBuffer, iLineLength);
			pOData += iLineSize;
		}
	// Set the image params
		pData = pOImageData;
		x = iWidth;
		y = iHeight;
	} 
	else {
		eFResult = F_OUTOFMEMORY;
	}

// Finish decompression
  (void)jpeg_finish_decompress(&cinfo);

// Destroy the compressor (release memory)
	jpeg_destroy_decompress(&cinfo);
// Check jerr.num_warnings, just in case

// Close the file
	fclose(pFile);

	return eFResult;
} // Load

bool CImageFileJPEG::CheckSaveType(IMAGETYPE eImgType) const {
	return ((eImgType == IT_RGB) || (eImgType == IT_GREY));
} // CheckSaveType

void CImageFileJPEG::SetSaveType(IMAGETYPE eImgType) {
	m_eSaveType = eImgType;
} // SetSaveType

void CImageFileJPEG::SetPalette(CImagePalette *pPalette) {
	return;
} // SetPalette

FRESULT CImageFileJPEG::Save(const char *pFname, const int x, const int y,
														 const unsigned char *pData) const {
// Check the save type
	if ((m_eSaveType != IT_GREY) && (m_eSaveType != IT_RGB)) 
		return F_WRONGIMAGETYPE;

// Open the save type
	FILE *pFile = fopen(pFname, "wb");
	if (!pFile)
		return F_FILEERROR;

// jpeg lib structures
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

// Set the error routines
  cinfo.err = jpeg_std_error(&jerr);
// Initialise JPEG compression object
  jpeg_create_compress(&cinfo);

// Set output file
  jpeg_stdio_dest(&cinfo, pFile);

// Set image properties
  cinfo.image_width = x;
  cinfo.image_height = y;
  cinfo.input_components = m_eSaveType == IT_RGB ? 3 : 1;
	cinfo.in_color_space = m_eSaveType == IT_RGB ? JCS_RGB : JCS_GRAYSCALE;

// Set defaults :o)
  jpeg_set_defaults(&cinfo);

// Set compression quality
  int iQuality = JPEG_QUALITY;
  jpeg_set_quality(&cinfo, iQuality, TRUE);

// Start compression
  jpeg_start_compress(&cinfo, TRUE);

  unsigned int iWidth = x;
  unsigned int iLineLength = iWidth * (m_eSaveType == IT_RGB ? 3 : 1);
	unsigned int iLineSize = iLineLength;
	if (iLineSize % 4 > 0) {
		iLineSize += 4 - (iLineSize % 4);
	}
// Write scanline
	unsigned char *pDataPtr = (unsigned char*)pData;
	while (cinfo.next_scanline < cinfo.image_height) {
		(void)jpeg_write_scanlines(&cinfo, &pDataPtr, 1);
		pDataPtr += iLineSize;
	}

// Finish compression
  jpeg_finish_compress(&cinfo);

// Delete compression object (release memory)
  jpeg_destroy_compress(&cinfo);

// Close file
	fclose(pFile);

	return F_OK;
} // Save

