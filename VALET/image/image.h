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
//! rcsid 		= "$Id: image.h,v 1.1 2001/10/18 11:10:41 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Image"
//! example 	= VALET/image/image.test.cpp

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/palette.h"
//#include "CAvatar.h"

namespace NValet {

   //#===--- Pre-declared classes
   class CImageFile;

   //#===--- Defines
   #define IMG_MAXWIDTH       1600
   #define IMG_MAXHEIGHT      1200
   // Maximum image size

   enum IMAGETYPE {
      IT_UNKNOWN,
      IT_MONO,
      IT_GREY,
      IT_PALETTE,
      IT_RGB,
   };
   // Image types

   enum IMAGEFILTERTYPE {
      IF_BOX,
      IF_BILINEAR,
   };
   // Scaling filter types

   enum IRESULT {
      I_OK,
      I_OUT_OF_RANGE,
      I_INVALID_PARAM,
      I_INCORRECT_TYPE,
      I_UNSUPPORTED_TYPE,
      I_NO_IMAGE,
      I_NO_PALETTE,
      I_OUT_OF_MEMORY,
      I_NO_DATA,
      I_ERROR,
   };
   // Image operation results

   //#===--- CImage
   //: Image Class
   // General image loading and manipulation class
   //!classtodo: Sort out image loading/saving

   class CImage {
   public:

	//:------
   //: Image Creation

      CImage(IMAGETYPE eType = IT_UNKNOWN, int iWidth = 0, int iHeight = 0, int iColours = 256);
      //: Contructor
      CImage(const CImage &oCopy);
      //: Copy constructor
      CImage(const CImage &oCopy, int iZoom);
      // Scaling copy constructor (e.g. double size if iZoom = 1 : half size if iZoom = -1)
      CImage(const CImage &oCopy, int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);
      // Scaling copy constructor (normal Scale params)
      CImage(const CImage &oCopy, IMAGETYPE eImgType, int iColours = 256);
      // Converting copy constructor
      ~CImage();
      //: Destructor

	//:------ 
   //: Manipulation Functions

      IMAGETYPE GetType() const;
      // Returns the current image type
      void GetSize(int &iWidth, int &iHeight) const;
      // Returns the image size
      IRESULT SetSize(int iWidth, int iHeight);
      // Change the size of the image (reallocating memory if necessary)
      //!todo: Copy over previous image contents
      void Clear();
      // Sets the image black
      void Paste(const CImage &oCopy, int iXOff = 0, int iYOff = 0);
      // Paste an image into the calling image (image offset optional)
      void Paste(const CImage &oCopy, float fAlpha, bool bTrans = false, 
                 unsigned int uColour = 0, int iXOff = 0, int iYOff = 0);
      // Paste an image into the calling image (transparent colour and image offset optional)
      // If bTrans not set, uColour ignored, otherwise this colour is not drawn (uColour = 0x00RRGGBB)
      void Flip();
      // Vertically flips the image
      unsigned int GetPixel(int iX, int iY) const;
      // Get pixel at coord
      unsigned int GetPixel(unsigned int uPixel) const;
      // Get pixel at coord

	//:------
   //: Type Conversion Functions
 
      const CImagePalette* GetPalette(void) const;
      // Returns pointer to image palette object
      IRESULT Convert(IMAGETYPE eType, int iColours = 256);
      // Convert image type (if necessary, default 256 colours for IT_PALETTE)
      IRESULT ForceToPalette(CImagePalette &oPalette);
      // Force an RGB image to the supplied palette
      IRESULT ForceType(IMAGETYPE eType, int iColours = 256);
      // Change the image type and disregard the image data (palette colours supplied if necessary)

	//:------
   //: Scaling Functions

      IRESULT Scale(int iWidth, int iHeight, IMAGEFILTERTYPE eFilter = IF_BILINEAR);
      // Scales image to specified height (default filter: bilinear)

	//:------
   //: Loading and Saving

      FRESULT Load(const char *pcFname, CImageFile *poImageFile);
      // Requires a previously created file filter object
      FRESULT Save(const char *pcFname, CImageFile *poImageFile) const;
      // Requires a previously created file filter object

	//:------
   //:  Image Importing

      IRESULT ImportRawImage(unsigned char *pcRaw, bool bWordAlign = false,
                             bool bReversePixels = false, bool bFlip = false);
	// Import an image from raw data (bounds checking not performed)
//	IRESULT ImportAMETexture(stAMETexture &sAME);
	// Import an image from an AME texture structure

   protected:

      //: Image Filter Precalculated Weights
      struct SContribStruct {
         float *m_pWeights;
         int m_iLeft, m_iRight;

         SContribStruct() {
            m_pWeights = NULL;
            m_iLeft = m_iRight = 0;
         }
         // Inline constructor

      };
      typedef struct SContribStruct SContrib;            // Filter weights typedef

      //: List of Filter Weights
   	struct SContribListStruct {
         SContrib *m_pContrib;
         int m_iWidth, m_iWindow;

         SContribListStruct() {
            m_pContrib = NULL;
            m_iWidth = m_iWindow = 0;
         }
      };
      typedef struct SContribListStruct SContribList;    // Weight List typedef

   //:------
   //: Internal Functions
 
      IRESULT CreateImage();
      // Image memory allocation

      IRESULT CreatePaletteFromRGB(int iColours = 256);
      // Colour quantisation

      SContribList *AllocateContributions(int iWidth, int iWindow);
      SContribList *CalculateContributions(int iInputWidth, int iOutputWidth, IMAGEFILTERTYPE eFilter);
      void DeleteContributions(SContribList *psList);

      IRESULT ScaleHorizontal(int iWidth, IMAGEFILTERTYPE eFilter);
      IRESULT ScaleHorizontalRGB(int iWidth, IMAGEFILTERTYPE eFilter);

      IRESULT ScaleVertical(int iHeight, IMAGEFILTERTYPE eFilter);
      IRESULT ScaleVerticalRGB(int iHeight, IMAGEFILTERTYPE eFilter);

   protected:

      IMAGETYPE m_eImageType;             // Image type
      int m_iWidth, m_iHeight;            // Dimensions
      unsigned int m_iDataSize;           // Size of image data (in 4 byte blocks)
      unsigned int m_iLineSize;           // Size of a scan line (in 4 byte blocks)
      unsigned int* m_pData;              // Pointer to image data
      CImagePalette *m_poPalette;         // pointer to palette object

   //#===--- Friends
//      friend class CRCOpenGLBufferWin32;
//      friend class CRCOpenGLWin32;

   };

}

#endif // _VALET_IMAGE_

