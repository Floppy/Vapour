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
//! rcsid 		= "$Id: image.h,v 1.10 2002/02/06 15:45:22 vap-warren Exp $"
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
      IR_INCOMPATIBLE_IMAGE,
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

	//:------
   //: Image Creation

   public:
   
      explicit CImage();
      //: Empty constructor
      
      explicit CImage(const EImageType eType);
      //: Empty typed constructor
      
      explicit CImage(const EImageType eType = IT_UNKNOWN, 
                      const unsigned int uiWidth = 0,
                      const unsigned int uiHeight = 0);
      //: Allocating constructor

      explicit CImage(const CImage &oCopy);
      //: Copy constructor

      ~CImage();
      //: Destructor
      
   //:------
   //: Allocation Functions
 
   protected:

      EImageResult CreateImage();
      //: Image memory allocation
      // This image will take the image width and height, calculate the
      // required memory requirements and allocate the image planes
      
      EImageResult CreatePlane(const EImagePlane ePlane) {
         return CreatePlane(m_pucPlaneNum[ePlane]);
      }
      //: Allocates an individual image plane
      // m_uiDataSize must be set before this function is called
      //!param: ePlane = Name of plane to be allocated
      
      EImageResult CreatePlane(const unsigned char ucPlane);
      //: Allocates an individual image plane
      // m_uiDataSize must be set before this function is called
      //!param: ucPlane = Number of plane to be allocated
      
      void DeleteImage();
      //: Deletes entire image content and set member vars appropriately
      
      void DeletePlane(const EImagePlane ePlane) {
         DeletePlane(m_pucPlaneNum[ePlane]);
      }
      //: Deletes an image plane
      //!param: ePlane = Name of plane to be deleted

      void DeletePlane(const unsigned char ucPlane);
      //: Deletes an image plane
      //!param: uiPlane = Number of plane to be deleted

   //:------ 
   //: Manipulation Functions

   public:
   
      EImageType GetType() const {
         return m_eImageType;
      }
      //: Returns the current image type

      EImageResult ChangeType(const EImageType eType);
      //: Change the type of the current image
      // This will perform all the relevant memory management and manipulations
      // to generate as close a copy of the previous image in the new type.
      // <B>NOTE: If converting to IT_PALETTE, set the number of palette entries first</B>
      //!param: eType = Type of image to convert to
      //!bug: Doesn't handle conversion to IT_PALETTE images yet
      //!bug: Can't convert to IT_GREY from any RGB-based format yet
      
      EImageResult ForceType(const EImageType eType);
      //: Change the type of the current image
      // This will perform all the relevant memory management, but will leave
      // image planes untouched.
      //!param: eType = Type of image to convert to
      //!bug: Doesn't handle conversion to IT_PALETTE images yet
      
      void GetSize(unsigned int &uiWidth, unsigned int &uiHeight) const {
         uiWidth = m_uiWidth; uiHeight = m_uiHeight;
      }
      //: Returns the image size

      EImageResult SetSize(const unsigned int uiWidth, const unsigned int uiHeight);
      //: Change the size of the image
      // Change the size of the image and reallocate memory accordingly.
      // Will either crop the image, or fill in gaps with the specified clear colour.
      //!bug: Doesn't keep the previous image contents

      void SetClearColour(const EImagePlane ePlane, const unsigned char ucCol);
      //: Sets the clear colour for a plane
      //!param: ePlane = Name of plane
      //!param: ucCol = 8-bit clear colour value

      void Clear();
      //: Clears the image
      // Clears all image planes using the table of plane clear colours
      
      void ClearPlane(const EImagePlane ePlane) {
         ClearPlane(m_pucPlaneNum[ePlane]);
      }
      //: Clears a single image plane
      // Clears the specified image plane using the table of plane clear colours
      //!param: ePlane = Name of plane to be cleared
      
      EImageResult CopyPlane(const EImagePlane eDest, const EImagePlane eSrc);
      //: Copies the contents of an image plane to another
      //!param: eDest = Name of destination plane
      //!param: eSrc = Name of source plane
      
      EImageResult CopyPlane(const EImagePlane eDest, const EImagePlane eSrc,
                             const CImage &oImg);
      //: Copies the contents of animage plane to from another image
      //!param: eDest = Name of destination plane
      //!param: eSrc = Name of source plane
      //!param: oImg = Source image
      
   protected:
   
      void ClearPlane(const unsigned char ucPlane);
      //: Clears a single image plane using the table of plane clear colours
      //!param: ucPlane = Number of plane to be cleared
      
	//:------
   //:  Image Importing
   
   public:

      EImageResult ImportRaw(unsigned char *pcRaw, bool bWordAlign = false,
                             bool bReversePixels = false, bool bFlip = false);
      //: Import raw image data
      // Imports raw 8-bit image data into a pre-created image
      // The amount of data read is calculated from the image type and size set
      // Data is considered as palette indices if type is IT_PALETTE
      //<B>NOTE: Does not bounds check the raw data</B>
      //!bug: Currently reverse and flip are considered false, even if set otherwise
      //!param: pucRaw = Pointer to start of raw data
      //!param: bWordAlign = Lines are 32-bit word aligned
      //!param: bReversePixels = Data is BGR formatted
      //!param: bFlip = Lines are arranged bottom first

      EImageResult ImportAlpha(CImage &oImg);
      //: Imports an alpha channel from another image
      // Only works on IT_RGB and IT_RGBA images. 
      // Convert unsupported image type prior to adding with ChangeType or ForceType
      //!param: oImg = Reference to same-sized single channel (IT_GREY) image

	//:------
   //: Type Conversion Functions

	//:------
   //: Scaling Functions

	//:------
   //: Loading and Saving

   //:------
   //: Friends

   public:
   
      friend unsigned int generate_texture_rgb(CImage &oImg);
      friend unsigned int generate_texture_argb(CImage &oImg, bool bFourBit);
      
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

