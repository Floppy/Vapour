//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VAL_IMAGE_FILE_RAW_
#define _VAL_IMAGE_FILE_RAW_

//#===--- Includes
#include "imagefile.h"
//#include "ImageFileStore.h"

#include <fstream.h>

////////////////
//! file 		= "VALET/image/imagefile_raw.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid 		= "$Id: imagefile_raw.h,v 1.1 2001/10/18 11:10:42 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Image.File"
//! example 	= VALET/image/image.test.cpp

namespace NValet {

   //#===--- CImageFileRaw

   class CImageFileRaw : public CImageFile {
   public:

      CImageFileRaw();
      ~CImageFileRaw();

      FRESULT GetImageType(const char *pFname, IMAGETYPE &eImgType);
      FRESULT Load(const char *pFname, int &x, int &y, unsigned char *&pData);
      bool CheckSaveType(IMAGETYPE eImgType) const;
      void SetSaveType(IMAGETYPE eImgType);
      void SetPalette(CImagePalette *pPalette);
      FRESULT Save(const char *pFname, const int x, const int y,
                   const unsigned char *pData) const;

   private:

      fstream m_oFile;
      IMAGETYPE m_eSaveType;
      CImagePalette *m_pPalette;

   };

}

#endif // _VAL_IMAGE_FILE_RAW_
