//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

//#===--- Includes
#include "VALET/image.h"
#include "VALET/log.h"

////////////////
//! file 		= "VALET/image/image.cpp"
//! author		= "Warren Moore"
//! date			= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid		= "$Id: image.cpp,v 1.5 2001/10/24 23:39:26 vap-warren Exp $"

namespace NVALET {

   //#===--- CImage

   const unsigned char CImage::m_pucPlaneUse[m_uiNumTypes] = { 0, 1, 1, 3, 4 };
   const unsigned char CImage::m_pucPlaneNum[m_uiNumPlaneTypes] = { 0, 0, 1, 2, 3 };
   const unsigned char CImage::m_pucDefClearCol[m_uiNumPlanes] = { 0, 0, 0, 255 };

   CImage::CImage() :
      m_eImageType(IT_UNKNOWN),
      m_uiWidth(0),
      m_uiHeight(0),
      m_uiDataSize(0),
      m_uiLineSize(0),
      m_poPalette(NULL)
   {
      CLog oLog("image", "CImage::Constructor (Default)", LL_OBJECT);
      // Clear the data planes, and get the default clear colours
      register unsigned int uiCount = m_uiNumPlanes;
      while (uiCount--) {
         m_ppuiData[uiCount] = NULL;
         m_pucClearCol[uiCount] = m_pucDefClearCol[uiCount];         
      }
   } // CImage::Constructor

   CImage::CImage(EImageType eType) :
      m_eImageType(eType),
      m_uiWidth(0),
      m_uiHeight(0),
      m_uiDataSize(0),
      m_uiLineSize(0),
      m_poPalette(NULL)
   {
      CLog oLog("image", "CImage::Constructor (Typed)", LL_OBJECT);
      // Clear the data planes, and get the default clear colours
      register unsigned int uiCount = m_uiNumPlanes;
      while (uiCount--) {
         m_ppuiData[uiCount] = NULL;
         m_pucClearCol[uiCount] = m_pucDefClearCol[uiCount];         
      }
   } // CImage::Constructor

   CImage::CImage(EImageType eType, unsigned int uiWidth, unsigned int uiHeight) :
      m_eImageType(eType),
      m_uiWidth(uiWidth),
      m_uiHeight(uiHeight),
      m_uiDataSize(0),
      m_uiLineSize(0),
      m_poPalette(NULL)
   {
      CLog oLog("image", "CImage::Constructor (Allocating)", LL_OBJECT);
      // Clear the data planes, and get the default clear colours
      register unsigned int uiCount = m_uiNumPlanes;
      while (uiCount--) {
         m_ppuiData[uiCount] = NULL;
         m_pucClearCol[uiCount] = m_pucDefClearCol[uiCount];         
      }
      // Allocate the image memory
	   CreateImage();
   } // CImage::Constructor

   CImage::~CImage() {
      CLog oLog("image", "CImage::Destructor", LL_OBJECT);
      DeleteImage();
   } // CImage::Destructor

   EImageResult CImage::SetSize(unsigned int uiWidth, unsigned int uiHeight) {
      CLog oLog("image", "CImage::SetSize");
      
      // Delete the image
      DeleteImage();
      
      // Set the image size
      m_uiWidth = uiWidth;
      m_uiHeight = uiHeight;

      // Allocate the new image memory
      EImageResult eResult = CreateImage();

      return eResult;
   } // CImage::SetSize

   void CImage::SetClearColour(EImagePlane ePlane, unsigned char ucCol) {
      CLog oLog("image", "CImage::SetClearCol");
      // Set the new clear colour
      m_pucClearCol[m_pucPlaneNum[ePlane]] = ucCol;
   }
   
   void CImage::Clear() {
      CLog oLog("image", "CImage::Clear");
      // Check we have an image
      if (m_eImageType == IT_UNKNOWN || m_uiWidth == 0 || m_uiHeight == 0)
         return;
      // Clear the image
      register unsigned char ucPlane = m_pucPlaneUse[m_eImageType];
      unsigned int uiDataSize = m_uiDataSize << 2;
      while (ucPlane--) {
         memset(m_ppuiData[ucPlane], m_pucClearCol[ucPlane], uiDataSize);
      }
   }
   
   EImageResult CImage::CreateImage() {
      CLog oLog("image", "CImage::CreateImage");
      ASSERT(m_uiDataSize == 0);
      EImageResult eResult = IR_OK;

      // Check the image size
      if (m_eImageType == IT_UNKNOWN || m_uiWidth == 0 || m_uiHeight == 0) {
         oLog.Trace("Error: Called with invalid parameter", LL_ERROR);
         eResult = IR_INVALID_PARAM;
      }
      if (m_uiWidth > m_uiMaxWidth) {
         oLog.Trace("Error: Specified width out of range", LL_ERROR);
         eResult = IR_OUT_OF_RANGE;
      }
      if (m_uiHeight > m_uiMaxHeight) {
         oLog.Trace("Error: Specified height out of range", LL_ERROR);
         eResult = IR_OUT_OF_RANGE;
      }

      // If valid, allocate the memory
      if (eResult == IR_OK) {
         // Calculate the line size
         m_uiLineSize = m_uiWidth >> 2;
         unsigned int uiLeft = m_uiWidth % 4;
         if (uiLeft) 
            m_uiLineSize++;
         // Calculate the data size
         m_uiDataSize = m_uiLineSize * m_uiHeight;
         // Allocate all the bit planes
         if (m_uiDataSize) {
            register char ucPlane = m_pucPlaneUse[m_eImageType];
            while (eResult == IR_OK && ucPlane--) {
               m_ppuiData[ucPlane] = (unsigned int*) new unsigned int[m_uiDataSize];
               if (!m_ppuiData[ucPlane]) {
                  oLog.Trace("Error: Out of memory allocating bit plane", LL_ERROR);
                  eResult = IR_OUT_OF_MEMORY;
               }
            }
         }
      }

      // If palette-based image, allocate the palette
      if (eResult == IR_OK && m_eImageType == IT_PALETTE) {
         m_poPalette = (CImagePalette*) new CImagePalette();
         if (!m_poPalette) {
            oLog.Trace("Error: Out of memory allocating palette", LL_ERROR);
            eResult = IR_OUT_OF_MEMORY;
         }
      }
      
      // If we ran out of memory allocating stuff, clean up allocated memory
      if (eResult == IR_OUT_OF_MEMORY)
         DeleteImage();

      return eResult;
   }

   void CImage::DeleteImage() {
      CLog oLog("image", "CImage::DeleteImage");
      // Set the size variables
      m_uiWidth = 0;
      m_uiHeight = 0;
      m_uiLineSize = 0;
      m_uiDataSize = 0;
      // Delete the image planes
      register unsigned char ucPlane = m_pucPlaneUse[m_eImageType];
      while (ucPlane--) {
         if (m_ppuiData[ucPlane]) {
            delete [] m_ppuiData[ucPlane];
            m_ppuiData[ucPlane] = NULL;
         }
      }
      // Delete the palette
      if (m_poPalette) {
         delete m_poPalette;
         m_poPalette = NULL;
      }
   }

   EImageResult CImage::ImportRawRGB(unsigned char *pcRaw, bool bWordAlign = false,
                                     bool bReversePixels = false, bool bFlip = false) {
      ASSERT(pcRaw);
      CLog oLog("image", "CImage::ImportRawRGB");
      
      // Only works with IT_RGB and IT_RGBA images
      if (m_eImageType != IT_RGB && m_eImageType != IT_RGBA) {
         oLog.Trace("Error: Incorrect image type", LL_ERROR);
         return IR_INCORRECT_TYPE;
      }
      // Check we have an image
      if (m_uiWidth == 0 || m_uiHeight == 0) {
         oLog.Trace("Error: No image allocated to import into", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      // Loop vars
      unsigned char *pucR = (unsigned char*)m_ppuiData[0]; 
      unsigned char *pucG = (unsigned char*)m_ppuiData[1]; 
      unsigned char *pucB = (unsigned char*)m_ppuiData[2];
      unsigned int uiStep = 4 - (m_uiWidth % 4);
      unsigned int uiRawStep = 4 - ((m_uiWidth * 3) % 4);
      register unsigned int uiY = m_uiHeight, uiX = m_uiWidth;
      while (uiY--) {
         // Copy out a line
         uiX = m_uiWidth;
         while (uiX--) {
            *pucR = *pcRaw++;
            *pucG = *pcRaw++;
            *pucB = *pcRaw++;
            pucR++;
            pucG++;
            pucB++;
         }
         // Step the gap at the end of the lines
         if (bWordAlign) {
            pcRaw += uiRawStep;
            pucR += uiStep;
            pucG += uiStep;
            pucB += uiStep;
         }
      }
         
      return IR_OK;
   }

}
