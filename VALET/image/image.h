//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_IMAGE_
#define _VALET_IMAGE_

////////////////
//! file 		= "VALET/image/image.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid 		= "$Id: image.h,v 1.8 2001/10/27 09:43:05 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Image"
//! example 	= VALET/image/image.test.cpp

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/palette.h"

namespace NVALET {

   enum EImageType {
      IT_UNKNOWN = 0,
      IT_GREY = 1,
      IT_PALETTE = 2,
      IT_RGB = 3,
      IT_RGBA = 4
   };
   //: Image types
   // IT_GREY     = Greyscale             - 1 x 8-bit plane
   // IT_PALETTE  = Palette based         - 1 x 8-bit plane + colour palette
   // IT_RGB      = True colour           - 3 x 8-bit planes
   // IT_RGBA     = True colour + alpha   - 4 x 8-bit planes

   enum EImagePlane {
      IP_GREY = 0,
      IP_RED = 1,
      IP_GREEN = 2,
      IP_BLUE = 3,
      IP_ALPHA = 4
   };      
   //: Image planes
   // Referenced against m_pucPlaneNum
   // IP_GREY     = Plane 0
   // IP_RED      = Plane 0
   // IP_GREEN    = Plane 1
   // IP_BLUE     = Plane 2
   // IP_ALPHA    = Plane 3

   enum EImageResult {
      IR_OK = 0,
      IR_OUT_OF_RANGE,
      IR_INVALID_PARAM,
      IR_INCORRECT_TYPE,
      IR_UNSUPPORTED_TYPE,
      IR_NO_IMAGE,
      IR_NO_PALETTE,
      IR_OUT_OF_MEMORY,
      IR_NO_DATA,
      IR_ERROR
   };
   //: Image result codes
   
   enum EImageFilter {
      IF_BOX = 0,
      IF_BILINEAR
   };
   //: Image scaling filter type
   // Must be implemented, defined in VALET/image/filter.h
   // IF_BOX      = Box filter, blocky pixel resize filter
   // IT_BILINEAR = Bilinear filter, smooth resampling filter

   /*#===--- Pre-declared classes */
   class CImageFile;

   //#===--- CImage
   //: Image Class
   // General image loading and manipulation class
   //!classtodo: Sort out image loading/saving

   class CImage {
   public:

	//:------
   //: Image Creation

      explicit CImage();
      //: Empty constructor
      explicit CImage(EImageType eType);
      //: Empty typed constructor
      explicit CImage(EImageType eType = IT_UNKNOWN, unsigned int uiWidth = 0,
             unsigned int uiHeight = 0);
      //: Allocating constructor
      CImage(const CImage &oCopy);
      //: Copy constructor
      ~CImage();
      //: Destructor

	//:------ 
   //: Manipulation Functions

      EImageType GetType() const {
         return m_eImageType;
      }
      // Returns the current image type
      void GetSize(unsigned int &uiWidth, unsigned int &uiHeight) const {
         uiWidth = m_uiWidth; uiHeight = m_uiHeight;
      }
      // Returns the image size
      EImageResult SetSize(unsigned int uiWidth, unsigned int uiHeight);
      // Change the size of the image (reallocating memory if necessary)
      //!todo: Copy over previous image contents
      void SetClearColour(EImagePlane ePlane, unsigned char ucCol);
      // Sets the clear colour for a plane
      void Clear();
      // Clears the image using the table of plane clear colours
      
	//:------
   //:  Image Importing

      EImageResult ImportRawRGB(unsigned char *pcRaw, bool bWordAlign = false,
                                bool bReversePixels = false, bool bFlip = false);
      // Import raw RGB888 image data
      // Image must already be created with the correct size
      //<B>NOTE: Does not bounds check the raw data</B>
      //!bug: Currently reverse and flip are considered false, even if set otherwise
      //!param: pucRaw = Pointer to start of raw data
      //!param: bWordAlign = Lines are 32-bit word aligned
      //!param: bReversePixels = Data is BGR formatted
      //!param: bFlip = Lines are arranged bottom first

	//:------
   //: Type Conversion Functions

   
	//:------
   //: Scaling Functions


	//:------
   //: Loading and Saving

   protected:

   //:------
   //: Allocation Functions
 
      EImageResult CreateImage();
      // Image memory allocation
      
      EImageResult CreatePlane(unsigned char ucPlane);
      // Allocates an individual image plane
      // m_uiDataSize must be set before this function is called
      //!param: uiPlane = Number of plane to be allocated
      
      void DeleteImage();
      // Deletes image memory and set member vars appropriately
      
      void DeletePlane(unsigned char ucPlane);
      // Deletes an image plane
      //!param: uiPlane = Number of plane to be deleted
      
   protected:

      const static unsigned int m_uiMaxWidth = 1600;     // Maximum image width
      const static unsigned int m_uiMaxHeight = 1200;    // Maximum image height
      const static unsigned int m_uiNumTypes = 5;        // Number of EImageTypeS's 
      const static unsigned int m_uiNumPlanes = 4;       // Number of allocated planes
      const static unsigned int m_uiNumPlaneTypes = 5;   // Number of EImagePlane's
      const static unsigned char m_pucPlaneUse[m_uiNumTypes];
      // Number of planes used for each image type
      const static unsigned char m_pucPlaneNum[m_uiNumPlaneTypes];
      // Index of plane names (in EImagePlane) to plane number
      const static unsigned char m_pucDefClearCol[m_uiNumPlanes];
      // Table of default clear colours

      EImageType m_eImageType;                     // Image type
      unsigned int m_uiWidth, m_uiHeight;          // Dimensions
      unsigned int m_uiDataSize;                   // Size of image data (in 4 byte blocks)
      unsigned int m_uiLineSize;                   // Size of a scan line (in 4 byte blocks)
      unsigned int* m_ppuiData[m_uiNumPlanes];     // Pointer to image data
                                                   // Lines are 32-bit word aligned
      CImagePalette *m_poPalette;                  // Pointer to palette object
      unsigned char m_pucClearCol[m_uiNumPlanes];  // List of clear colours
      
   };

}

#endif // _VALET_IMAGE_

