//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_IMAGE_PALETTE_
#define _VALET_IMAGE_PALETTE_

////////////////
//! file 		= "VALET/image/palette.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid 		= "$Id: palette.h,v 1.2 2001/10/17 22:14:17 vap-warren Exp $"
//! userlevel 	= Advanced
//! docentry 	= "VALET.Image.Palette"

//#===--- Includes
#include "VALET/valet.h"

namespace NValet {

   #define IPL_MAX_ENTRIES			256
   // Max size of palette

   //#===--- CImagePalette
   //: Image Palette Storage and Manipulation
   //!classtodo: Doesn't compile yet as colour quantisation not an issue yet

   class CImagePalette {
   public:

   	CImagePalette(int iSize = IPL_MAX_ENTRIES, unsigned int uColour = 0);
      //: Constructor
      // Create the palette of a specified size filled with a default colour

      CImagePalette(const CImagePalette &oCopy);
      //: Copy Constructor

      ~CImagePalette();
      //: Destructor

      void Copy(const CImagePalette *poCopy);
      // Copy the palette into current from a pointer
      //!param: poCopy = Pointer to source palette object

   //:------
   //: Settings

      void SetSize(int iSize);
      // Set the size of palette colours
      //!param: iSize = Number of palette colours

      int GetSize() const;
      // Returns the max numbers of colour within the palette

      void Clear(unsigned int uColour = 0);
      // Clears the palette with a default colour

   //:------
   //: Entry Manipulation

      int AddEntry(unsigned int uColour);
      // Add an entry to the palette (maintains an internal counter, returns the index, -1 if invalid)

      void SetEntry(int iIndex, unsigned int uColour);
      // Set a specific entry with a colour

      void GetEntry(int iIndex, unsigned int &uColour) const;
      // Get a specific entry (0 if invalid index)

   //:------
   //: Colour Matching

      int MatchColour(unsigned int uColour);
      // Returns a palette index closest matching the supplied colour

   protected:

   //:------
   //: Palette Allocation

      bool AllocatePalette();
      // Allocates the palette memory from m_iSize. Returns false and sets size to 0 if failed

   //:------
   //: Matched colour cache

      void CreateCache();
      // Creates a cache for recently matched values

      int CheckCache(unsigned int uColour);
      // Returns a palette index if the colour was cached, -1 of not

      void AddCacheEntry(unsigned int uColour, int iIndex);
      // Adds a colour index to the cache

      void DeleteCache();
      // Deletes the cache

   //:------
   //: Palette entry hash table

      void CreateHash();
      // Creates a hash table of the palette entries

      int CheckHash(unsigned int uColour);
      // Returns palette index if colour found, -1 if not

      void DeleteHash();
      // Deletes the hash memory

      /*#===--- Brute force colour matching */

      int FindLowest(unsigned int uColour);
      // Finds the entry with the lowest error

   	typedef struct SCacheStruct {
         unsigned int m_uColour;
         int m_iIndex;
         int m_iHits;

         SCacheStruct() {
            m_uColour = 0xFF000000;          // Invalid colour
            m_iIndex = -1;                   // Invalid index
            m_iHits = -1;                    // Value guaranteed to be initially overwritten
         }
         // Inline constructor

      } SCache;
   	//: Colour matching cache

      typedef struct SHashEntryStruct {
         int m_iIndex;
         unsigned int m_uColour;
         int m_iR, m_iG, m_iB;
      } SHashEntry;
      //: Hash table entries

      typedef struct SHashStruct {
         unsigned int m_uEntries;            // Number of entries in this part of the hash
         SHashEntry *m_psColour;

         SHashStruct() {
            m_uEntries = 0;
            m_psColour = NULL;
         }
         // Inline constructor

      } SHash;
      //: Palette hash table

   protected:

      int m_iSize;                           // Max number of colour entries
      int m_iNextEntry;                      // Internal counter for AddEntry function

      unsigned int *m_puPalette;             // Palette data

      mutable SCache *m_psCache;             // Colour match cache structure

      mutable SHash *m_psHash;               // Colour match hash table

   };

}

#endif // _VALET_IMAGE_PALETTE_
