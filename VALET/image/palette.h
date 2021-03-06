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
//! rcsid 		= "$Id: palette.h,v 1.11 2001/10/24 23:39:26 vap-warren Exp $"
//! userlevel 	= Advanced
//! docentry 	= "VALET.Image.Palette"

//#===--- Includes
#include "VALET/valet.h"

namespace NVALET {

   #define IPL_MAX_ENTRIES			256
   // Max size of palette

   //#===--- CImagePalette
   //: Image Palette Storage and Manipulation

   class CImagePalette {
   public:

      explicit CImagePalette();
      //: Empty Constructor
      // Creates an empty palette object
      
   	explicit CImagePalette(int iSize, unsigned int uColour = 0);
      //: Allocating constructor
      // Create the palette of a specified size filled with a default colour
      //!param: iSize = Size of palette
      //!param: uColour = Default colour palette entries (format 0x00RRGGBB)

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
      // Clears the palette with a default colour (format 0x00RRGGBB)

   //:------
   //: Entry Manipulation

      int AddEntry(unsigned int uColour);
      // Add an entry to the palette (format 0x00RRGGBB)
      //!return: The colour index if added ok, -1 if palette full

      void SetEntry(int iIndex, unsigned int uColour);
      // Set a specific entry with a colour
      //!param: iIndex = Index of palette entry to set
      //!param: uColour = Colour of entry (format 0x00RRGGBB)

      void GetEntry(int iIndex, unsigned int &uColour) const;
      // Get a specific entry (0 if invalid index)
      //!param: iIndex = Index of palette entry to get
      //!param: uColour = Reference to colour in which palette entry is copied

   //:------
   //: Colour Matching

      int MatchColour(unsigned int uColour);
      // Returns a palette index closest matching the supplied colour
      //!param: uColour = Colour to match (format 0x00RRGGBB)
      //!return: Index of nearest matching colour

   protected:

   //#===--- Structures

   	//: Colour matching cache
   	struct SCacheStruct {
         unsigned int m_uColour;
         int m_iIndex;
         int m_iHits;

         SCacheStruct() {
            m_uColour = 0xFF000000;          // Invalid colour = 0xFF000000
            m_iIndex = -1;                   // Invalid index = -1
            m_iHits = -1;                    // Value guaranteed to be initially overwritten = -1
         }
         // Inline constructor

      };

      //: Hash table entries
      struct SHashEntryStruct {
         int m_iIndex;
         unsigned int m_uColour;
         int m_iR, m_iG, m_iB;
      };

      //: Palette hash table
      struct SHashStruct {
         unsigned int m_uEntries;            // Number of entries in this part of the hash
         struct SHashEntryStruct *m_psColour;

         SHashStruct() {
            m_uEntries = 0;
            m_psColour = NULL;
         }
         // Inline constructor

      };

      typedef struct SCacheStruct SCache;             // Cache structure typedef
      typedef struct SHashEntryStruct SHashEntry;     // Hash table entry typedef
      typedef struct SHashStruct SHash;               // Hash table typedef

   //:------
   //: Palette Allocation

      bool AllocatePalette();
      // Allocates the palette memory from m_iSize. Returns false and sets size to 0 if failed

   //:------
   //: Matched colour cache

      void CreateCache();
      // Creates a cache for recently matched values

      int CheckCache(unsigned int uColour);
      // Find the colour in the cache
      //!param: uColour = Colour to match (format 0x00RRGGBB)
      //!return: Returns palette index if found, -1 if not

      void AddCacheEntry(unsigned int uColour, int iIndex);
      // Adds a colour index to the cache

      void DeleteCache();
      // Deletes the cache

   //:------
   //: Palette entry hash table

      void CreateHash();
      // Creates a hash table of the palette entries

      int CheckHash(unsigned int uColour);
      // Find the colour in the hash
      //!param: uColour = Colour to match (format 0x00RRGGBB)
      //!return: Returns palette index if found, -1 if not

      void DeleteHash();
      // Deletes the hash memory

   //:------
   //: Brute force colour matching

      int FindLowest(unsigned int uColour);
      // Finds the entry with the lowest error
      //!param: uColour = Colour to match (format 0x00RRGGBB)

      int m_iSize;                           // Max number of colour entries
      int m_iNextEntry;                      // Internal counter for AddEntry function

      unsigned int *m_puPalette;             // Palette data

      mutable SCache *m_psCache;             // Colour match cache structure

      mutable SHash *m_psHash;               // Colour match hash table

   };

}

#endif // _VALET_IMAGE_PALETTE_
