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
//! rcsid		= "$Id: image.cpp,v 1.7 2001/10/28 16:52:22 vap-warren Exp $"

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

   CImage::CImage(const EImageType eType) :
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

   CImage::CImage(const EImageType eType,
                  const unsigned int uiWidth, const unsigned int uiHeight) :
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

   EImageResult CImage::CreateImage() {
      CLog oLog("image", "CImage::CreateImage");
      ASSERT(m_uiDataSize == 0);
      EImageResult eResult = IR_OK;

      // Check the image size
      if (m_eImageType == IT_UNKNOWN || m_uiWidth == 0 || m_uiHeight == 0) {
         oLog.Trace("Called with invalid parameter", LL_ERROR);
         eResult = IR_INVALID_PARAM;
      }
      if (m_uiWidth > m_uiMaxWidth) {
         oLog.Trace("Specified width out of range", LL_ERROR);
         eResult = IR_OUT_OF_RANGE;
      }
      if (m_uiHeight > m_uiMaxHeight) {
         oLog.Trace("Specified height out of range", LL_ERROR);
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
               eResult = CreatePlane(ucPlane);
            }
         }
      }

      // If palette-based image, allocate the palette
      if (eResult == IR_OK && m_eImageType == IT_PALETTE) {
         m_poPalette = (CImagePalette*) new CImagePalette();
         if (!m_poPalette) {
            oLog.Trace("Out of memory allocating palette", LL_ERROR);
            eResult = IR_OUT_OF_MEMORY;
         }
      }
      
      // If we ran out of memory allocating stuff, clean up allocated memory
      if (eResult != IR_OK)
         DeleteImage();
      else
         Clear();

      return eResult;
   }

   EImageResult CImage::CreatePlane(const unsigned char ucPlane) {
      CLog oLog("image", "CImage::CreatePlane");
      // Check the plane number
      if (ucPlane >= m_uiNumPlanes) {
         oLog.Trace("Invalid plane number set", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      // Allocate all the bit planes
      if (m_uiDataSize) {
         m_ppuiData[ucPlane] = (unsigned int*) new unsigned int[m_uiDataSize];
         if (!m_ppuiData[ucPlane]) {
            oLog.Trace("Out of memory allocating image plane", LL_ERROR);
            return IR_OUT_OF_MEMORY;
         }
      }
      return IR_OK;
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
         DeletePlane(ucPlane);
      }
      // Delete the palette
      if (m_poPalette) {
         delete m_poPalette;
         m_poPalette = NULL;
      }
   }
   
   void CImage::DeletePlane(const unsigned char ucPlane) {
      CLog oLog("image", "CImage::DeletePlane");
      // Check the plane number
      if (ucPlane >= m_uiNumPlanes) {
         oLog.Trace("Invalid plane number set", LL_ERROR);
         return;
      }
      if (m_ppuiData[ucPlane]) {
         delete [] m_ppuiData[ucPlane];
         m_ppuiData[ucPlane] = NULL;
      }
   }

   EImageResult CImage::ChangeType(const EImageType eType) {
      CLog oLog("image", "CImage::ChangeType");
      // Run the call to ChangeType
      EImageResult eResult = ChangeType(eType);
      // Clean up after, if all ok
      if (eResult == IR_OK) {
         // Work out how many planes used and required
         const unsigned char ucPlaneCur = m_pucPlaneUse[m_eImageType];
         const unsigned char ucPlaneNew = m_pucPlaneUse[eType];
         char cPlaneDiff = static_cast(char, ucPlaneNew);
         cPlaneDiff -= static_cast(char, ucPlaneCur);
         // Set all planes not be cleared at end 
         bool pbClearPlane[ucPlaneNew];
         register char cPlane = cPlaneDiff;
         while (cPlane--) {
            pbClearPlane[cPlane] = false;
         }
         // If planes added, set them to be cleared if not modified
         if (cPlane > 0) {
            while (cPlane--) {
               pbClearPlane[ucPlaneCur + cPlane] = true;
            }
         }
         
         //#===--- Type specific conversions
         
         //#===--- End of conversions
         
         // Clear unmodified new planes
         cPlane = static_cast(char, ucPlaneNew);
         while (cPlane--) {
            if (pbClearPlane[cPlane])
               ClearPlane(cPlane);
         }
      }
      return eResult;
   } // CImage::ChangeType
      
   EImageResult CImage::ForceType(const EImageType eType) {
      CLog oLog("image", "CImage::ForceType");
      // Check for the same type
      if (m_eImageType == eType) {
         oLog.Trace("Redundant call as image types identical", LL_WARNING);
         return IR_OK;
      }
      
      // Return early if no data set
      if (!m_uiDataSize) {
         oLog.Trace("Redundant call as no size set", LL_WARNING);
         return IR_OK;
      }
      
      // Bug out if it's a palette based image
      if (m_eImageType == IT_PALETTE || eType == IT_PALETTE) {
         oLog.Trace("IT_PALETTE operations not yet implemented", LL_ERROR);
         return IR_UNSUPPORTED_TYPE;
      }
         
      EImageResult eResult = IR_OK;
      // Work out how many planes used and required
      const unsigned char ucPlaneCur = m_pucPlaneUse[m_eImageType];
      const unsigned char ucPlaneNew = m_pucPlaneUse[eType];
      char cPlaneDiff = static_cast(char, ucPlaneNew);
      cPlaneDiff -= static_cast(char, ucPlaneCur);
      // Remove planes
      if (cPlaneDiff < 0) {
         while (cPlaneDiff < 0) {
            DeletePlane(static_cast(unsigned char, ucPlaneCur + cPlaneDiff));
            cPlaneDiff++;
         }
      }
      // or add them
      else {
         while (eResult == IR_OK && cPlaneDiff--) {
            eResult = CreatePlane(static_cast(unsigned char, ucPlaneCur + cPlaneDiff));
         }
      }
      // If all ok, store the change
      if (eResult == IR_OK)
         m_eImageType = eType;
      
      return eResult;
   } // CImage::ForceType
   
   EImageResult CImage::SetSize(const unsigned int uiWidth, const unsigned int uiHeight) {
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

   void CImage::SetClearColour(const EImagePlane ePlane, const unsigned char ucCol) {
      CLog oLog("image", "CImage::SetClearCol");
      // Set the new clear colour
      m_pucClearCol[m_pucPlaneNum[ePlane]] = ucCol;
   } // CImage::SetClearColour
   
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
   } // CImage::Clear
   
   void CImage::ClearPlane(const unsigned char ucPlane) {
      CLog oLog("image", "CImage::ClearPlane");
      // Check we have an image
      if (m_eImageType == IT_UNKNOWN || m_uiWidth == 0 || m_uiHeight == 0)
         return;
      // Check the plane number
      if (ucPlane >= m_uiNumPlanes) {
         oLog.Trace("Invalid plane number set", LL_ERROR);
         return;
      }
      // Clear the image
      unsigned int uiDataSize = m_uiDataSize << 2;
      memset(m_ppuiData[ucPlane], m_pucClearCol[ucPlane], uiDataSize);
   } // CImage::Clear
   
   EImageResult CImage::CopyPlane(const EImagePlane eDest, const EImagePlane eSrc) {
      CLog oLog("image", "CImage::CopyPlane");
      // Get the plane numbers
      const unsigned char ucDest = m_pucPlaneNum[eDest];
      const unsigned char ucSrc = m_pucPlaneNum[eSrc];
      // Check we have allocated both planes
      if (!(m_uiDataSize && m_ppuiData[ucDest] && m_ppuiData[ucSrc])) {
         oLog.Trace("Specified image plane not allocated", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      // Copy the plane contents
      memcpy(m_ppuiData[ucDest], m_ppuiData[ucSrc], m_uiDataSize << 2);
      return IR_OK;
   } // CImage::CopyPlane
   
   EImageResult CImage::CopyPlane(const EImagePlane eDest, const EImagePlane eSrc,
                                  const CImage &oImg) {
      CLog oLog("image", "CImage::CopyPlane");
      // Get the plane numbers
      const unsigned char ucDest = m_pucPlaneNum[eDest];
      const unsigned char ucSrc = m_pucPlaneNum[eSrc];
      // Check we have allocated both planes
      if (!(m_uiDataSize && m_ppuiData[ucDest])) {
         oLog.Trace("Specified destination image plane not allocated", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      if (!(oImg.m_uiDataSize && oImg.m_ppuiData[ucSrc])) {
         oLog.Trace("Specified source image plane not allocated", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      // Check both the image planes are the same size
      if (m_uiDataSize != oImg.m_uiDataSize) {
         oLog.Trace("Image sizes are different", LL_ERROR);
         return IR_INCOMPATIBLE_IMAGE;
      }
      // Copy the plane contents
      memcpy(m_ppuiData[ucDest], oImg.m_ppuiData[ucSrc], m_uiDataSize << 2);

      return IR_OK;
   } // CImage::CopyPlane
   
   EImageResult CImage::ImportRaw(unsigned char *pcRaw, bool bWordAlign = false,
                                  bool bReversePixels = false, bool bFlip = false) {
      ASSERT(pcRaw);
      CLog oLog("image", "CImage::ImportRaw");
      
      // Image type must be set
      if (m_eImageType == IT_UNKNOWN) {
         oLog.Trace("Image type must be set", LL_ERROR);
         return IR_INCORRECT_TYPE;
      }
      // Check we have an image
      if (m_uiWidth == 0 || m_uiHeight == 0) {
         oLog.Trace("No image allocated to import into", LL_ERROR);
         return IR_INVALID_PARAM;
      }
      // Get the plane pointers used
      unsigned char ucPlaneUse = m_pucPlaneUse[m_eImageType];
      unsigned char *pucPlane[ucPlaneUse];
      register unsigned char ucPlane = ucPlaneUse;
      while (ucPlane--) {
         pucPlane[ucPlane] = (unsigned char*)m_ppuiData[ucPlane];
      }
      // Calculate the line steps
      unsigned int uiStep = (m_uiLineSize << 2) - m_uiWidth;
      unsigned int uiRawStep = ((m_uiWidth * ucPlaneUse) % 4);
      if (uiRawStep)
         uiRawStep = 4 - uiRawStep;
      register unsigned int uiY = m_uiHeight, uiX = m_uiWidth;
      // Loop through each line
      while (uiY--) {
         // Copy out a line
         uiX = m_uiWidth;
         while (uiX--) {
            for (ucPlane = 0; ucPlane < ucPlaneUse; ucPlane++) {
               *pucPlane[ucPlane] = *pcRaw++;
               pucPlane[ucPlane]++;
            }
         }
         // Step the gap at the end of the lines
         if (bWordAlign) {
            pcRaw += uiRawStep;
         }
         // Step the gap at the end of the plane
         ucPlane = ucPlaneUse;
         while (ucPlane--) {
            pucPlane[ucPlane] += uiStep;
         }
      }
         
      return IR_OK;
   } // CImage::ImportRaw

   EImageResult CImage::ImportAlpha(CImage &oImg) {
      CLog oLog("image", "CImage::ImportAlpha");
      // Check we have the right types
      if (m_eImageType != IT_RGB || oImg.m_eImageType != IT_GREY) {
         oLog.Trace("Incorrect image type", LL_ERROR);
         return IR_INCORRECT_TYPE;
      }
      // Change the destination image type
      EImageResult eResult = ForceType(IT_RGBA);
      if (eResult == IR_OK) {
         eResult = CopyPlane(IP_ALPHA, IP_GREY, oImg);
      }
      return eResult;
   } // CImage::ImportAlpha
   
}
