//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VAL_IMAGE_FILE_
#define _VAL_IMAGE_FILE_

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/image.h"
#include "VALET/palette.h"

////////////////
//! file 		= "VALET/image/imagefile.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid 		= "$Id: imagefile.h,v 1.1 2001/10/18 11:10:41 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Image.File"
//! example 	= VALET/image/image.test.cpp

namespace NValet {

   //#===--- CImageFile
   //: Base Image Loading/Saving Class

   class CImageFile {
   public:
	
      CImageFile() {};
      virtual ~CImageFile() {};

//      virtual const char *GetFileExtension() const = 0;
//      virtual const char *GetFileTitle() const = 0;

   //:------
   //: Load Functions

   	virtual FRESULT GetImageType(const char *pFname, IMAGETYPE &eImgType) = 0;
   	virtual FRESULT Load(const char *pFname, int &x, int &y, unsigned char *&pData) = 0;

   //:------
   //: Save Functions

      virtual bool CheckSaveType(IMAGETYPE eImgType) const = 0;
      virtual void SetSaveType(IMAGETYPE eImgType) = 0;
      virtual void SetPalette(CImagePalette *pPalette) = 0;
      virtual FRESULT Save(const char *pFname, const int x, const int y,
                           const unsigned char *pData) const = 0;
   };

}

#endif // _VAL_IMAGE_FILE_
