//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/image/palette.cpp"
//! author		= "Warren Moore"
//! date			= "17/10/2001"
//! lib 			= libVALETimage
//! rcsid		= "$Id: palette.cpp,v 1.1 2001/10/17 22:05:33 vap-warren Exp $"

//#===--- Includes
#include "palette.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>

namespace NValet {

   // Cache defines
   #define IPL_CACHE_SIZE           50
   #define IPL_LRU_MAX              5
   // Hash defines
   #define IPL_HASH_BITS            4
   #define IPL_HASH_THRESHHOLD      20
   #define IPL_HASH_MASK            (0x000000FF << (8 - IPL_HASH_BITS)) & 0x000000FF

   //#===--- CImagePalette

   CImagePalette::CImagePalette(int iSize, unsigned int uColour) :
      m_iNextEntry(0),
      m_puPalette(NULL),
      m_psCache(NULL),
      m_psHash(NULL)
   {
      // Set the initial parameters
      m_iSize = (iSize > IPL_MAX_ENTRIES) ? IPL_MAX_ENTRIES : (iSize < 0) ? 0 : iSize;
      uColour &= 0x00FFFFFF;
      // Allocate the palette and set the clear colour
      if (m_iSize) {
         // If allocated, set the default palette entry
         if (AllocatePalette()) {
            int iCount = 0;
            unsigned int *puEntry = m_puPalette;
            while (iCount < m_iSize) {
               *puEntry = uColour;
               iCount++;
               puEntry++;
            }
         }
         else
            m_iSize = 0;
      }
   } // CImagePalette::Constructor

   CImagePalette::CImagePalette(const CImagePalette &oCopy) :
      m_iSize(oCopy.m_iSize),
      m_iNextEntry(oCopy.m_iNextEntry),
      m_puPalette(NULL),
      m_psCache(NULL),
      m_psHash(NULL)
   {
      // Allocate and copy the palette
      if (AllocatePalette())
         memcpy(m_puPalette, oCopy.m_puPalette, m_iSize * sizeof(unsigned int));
   } // CImagePalette::Copy Constructor

   CImagePalette::~CImagePalette() {
      // Delete the palette memory
      delete [] m_puPalette;
      // Delete the cache
      DeleteCache();
      DeleteHash();
   } // CImagePalette::Destructor

   void CImagePalette::Copy(const CImagePalette *poCopy) {
      ASSERT(poCopy);
      // Make sure we have a palette to copy
      if (!poCopy)
         return;
      // Set the palette sizes
      int iOldSize = m_iSize;
      m_iSize = poCopy->m_iSize;
      // If the sizes are different, reallocate the memory
      if (iOldSize != m_iSize)
         // Leave early if out of memory
         if (!AllocatePalette())
            return;
      // Copy the palette over
      m_iNextEntry = poCopy->m_iNextEntry;
      memcpy(m_puPalette, poCopy->m_puPalette, m_iSize * sizeof(unsigned int));
      // Cache out of date, so delete it
      DeleteCache();
      // Same with hash
      DeleteHash();
   } // CImagePalette::Copy

   bool CImagePalette::AllocatePalette() {
      delete [] m_puPalette;
      m_puPalette = NULL;
      // Leave if no palette size set
      if (m_iSize == 0)
         return true;
      m_puPalette = (unsigned int*) new unsigned int[m_iSize];
      // Everything ok?
      if (!m_puPalette) {
         m_iSize = 0;
         return false;
      }
      return true;
   } // CImagePalette::AllocatePalette

   //#===--- Setup

   void CImagePalette::SetSize(int iSize) {
      // Check the size is within limits
      int iOldSize = m_iSize;
      m_iSize = (iSize > IPL_MAX_ENTRIES) ? IPL_MAX_ENTRIES : (iSize < 0) ? 0 : iSize;
      // Allocate the palette memory
      if (iOldSize != m_iSize)
         AllocatePalette();
      // Set the AddEntry counter
      m_iNextEntry = 0;
      // Delete the cache
      DeleteCache();
      DeleteHash();
   } // CImagePalette::SetSize

   int CImagePalette::GetSize() const {
      return m_iSize;
   } // CImagePalette::GetSize

   void CImagePalette::Clear(unsigned int uColour) {
      // Mask the input colour
      uColour &= 0x00FFFFFF;
      // Loop through entries setting clear colour
      int iCount = 0;
      unsigned int *puEntry = m_puPalette;
      while (iCount < m_iSize) {
         *puEntry = uColour;
         iCount++;
         puEntry++;
      }
      // Delete the cache
      DeleteCache();
      DeleteHash();
   } // CImagePalette::Clear

   //#===--- Palette

   int CImagePalette::AddEntry(unsigned int uColour) {
      ASSERT(m_iNextEntry < m_iSize);
      // If not within palette size bail out
      if (m_iNextEntry >= m_iSize)
         return -1;
      // Set the palette entry
      uColour &= 0x00FFFFFF;
      m_puPalette[m_iNextEntry] = uColour;
      return m_iNextEntry++;
   } // CImagePalette::AddEntry

   void CImagePalette::SetEntry(int iIndex, unsigned int uColour) {
      ASSERT(iIndex < m_iSize);
      // If not within palette size bail out
      if (iIndex >= m_iSize) 
         return;
      // Set the palette entry
      uColour &= 0x00FFFFFF;
      m_puPalette[iIndex] = uColour;
   } // CImagePalette::SetEntry

   void CImagePalette::GetEntry(int iIndex, unsigned int &uColour) const {
      ASSERT(iIndex < m_iSize);
      // Get the palette entry if within palette size, 0 otherwise
      uColour = (iIndex < m_iSize) ? m_puPalette[iIndex] : 0;
   } // CImagePalette::GetEntry

   //#===--- Colour Matching

   int CImagePalette::MatchColour(unsigned int uColour) {
      // Create the cache
      if (!m_psCache)
         CreateCache();
      //	if (!m_psHash)
//		CreateHash();
      // Try to find the colour in the cache
      int iIndex = CheckCache(uColour);
      if (iIndex != -1) {
//		static unsigned int uCacheHits = 0;
//		uCacheHits++;
//		TRACE("Cache hit (%6u) - Index: %3d\n", uCacheHits, iIndex);
         return iIndex;
      }
      // Find the nearest match in the hash table
//	iIndex = CheckHash(uColour);
//	if (iIndex != -1) {
//		static unsigned int uHashHits = 0;
//		uHashHits++;
//		TRACE("Hash hit (%6u) - Index: %3d\n", uHashHits, iIndex);
//		AddCacheEntry(uColour, iIndex);
//		return iIndex;
//	}
      // Check over palette entry to find the lowest error
      iIndex = FindLowest(uColour);
      // Must have found one now, so add cache entry
      AddCacheEntry(uColour, iIndex);
//	static unsigned int uSADHits = 0;
//	uSADHits++;
//	TRACE("SAD hit (%6u) - Index: %3d\n", uSADHits, iIndex);
      return iIndex;
   } // CImagePalette::MatchColour

   void CImagePalette::CreateCache() {
      // Return if cache created
      if (m_psCache)
         return;
   	m_psCache = (SCache*) new SCache[IPL_CACHE_SIZE];
   } // CImagePalette::CreateCache

   int CImagePalette::CheckCache(unsigned int uColour) {
      // Return invalid entry if no cache
      if (!m_psCache)
         return -1;
      // Decrement all the entries
      register int iCount = 0;
      SCache *psEntry = m_psCache;
      while (iCount < IPL_CACHE_SIZE) {
         // Decrement value
         psEntry->m_iHits--;
         // Increment the counters
         iCount++;
         psEntry++;
	   }
      // Mask the colour entry
      uColour &= 0x00FFFFFF;
      // Check the cache
      iCount = 0;
      psEntry = m_psCache;
      while (iCount < IPL_CACHE_SIZE) {
         // Have we found it, if so leave early
         if (uColour == psEntry->m_uColour) {
            psEntry->m_iHits = IPL_LRU_MAX;
            return psEntry->m_iIndex;
         }
         // Increment the counters
         iCount++;
         psEntry++;
      }
      // Not found so return -1
      return -1;
   } // CImagePalette::CheckCache

   void CImagePalette::AddCacheEntry(unsigned int uColour, int iIndex) {
      // Return if no cache
      if (!m_psCache)
         return;
      // Mask the colour entry
      uColour &= 0x00FFFFFF;
      // Check the cache
      register int iCount = 0;
      SCache *psEntry = m_psCache;
      int iLowEntry = 0;
      int iLowHits = psEntry->m_iHits;
      while (iCount < IPL_CACHE_SIZE) {
         // Have we found it, set the entry and leave
         if (psEntry->m_iHits < 0) {
            psEntry->m_uColour = uColour;
            psEntry->m_iIndex = iIndex;
            psEntry->m_iHits = IPL_LRU_MAX;
            return;
         }
         // If not found, find the least recently used
         if (psEntry->m_iHits < iLowHits) {
            iLowEntry = iCount;
            iLowHits = psEntry->m_iHits;
         }
         // Increment the counters
         iCount++;
         psEntry++;
      }
      // Not found, so use the lowest found
      psEntry = m_psCache + iLowEntry;
      psEntry->m_uColour = uColour;
      psEntry->m_iIndex = iIndex;
      psEntry->m_iHits = IPL_LRU_MAX;
   } // AddCacheEntry

   void CImagePalette::DeleteCache() {
      delete [] m_psCache;
      m_psCache = NULL;
   } // CImagePalette::DeleteCache

   void CImagePalette::CreateHash() {
      // Return if cache created
      if (m_psHash)
         return;
      // Create the hash table
      int iSize = (int)pow(2, 3 * IPL_HASH_BITS);
      m_psHash = (SHash*) new SHash[iSize];
      if (!m_psHash)
         return;
      // Create the temporary entry count table
      int *piEntries = NULL;
      piEntries = (int*) new int[iSize];
      if (!piEntries) {
         delete [] m_psHash;
         m_psHash = NULL;
         return;
      }
      memset(piEntries, 0, iSize * sizeof(int));
      // Find out how many entries in each
      int iCount = 0;
      unsigned int uColour = 0;
      int iR = 0, iG = 0, iB = 0;
      int iKey = 0, iOffset = 0;
      while (iCount < m_iSize) {
         // Get the palette entry
         uColour = m_puPalette[iCount];
         // Break into components
         iR = (uColour >> 16) & 0x000000FF;
         iG = (uColour >>  8) & 0x000000FF;
         iB = (uColour      ) & 0x000000FF;
         // Calculate the hash key
         iOffset = (IPL_HASH_BITS - 8);
         // Blue component
         if (iOffset)
            iKey = (iOffset > 0) ? (iB & IPL_HASH_MASK) << iOffset : (iB & IPL_HASH_MASK) >> -iOffset;
         else
            iKey = iB & IPL_HASH_MASK;
         // Green component
         iOffset += IPL_HASH_BITS;
         if (iOffset)
            iKey |= (iOffset > 0) ? (iG & IPL_HASH_MASK) << iOffset : (iG & IPL_HASH_MASK) >> -iOffset;
         else
            iKey |= iG & IPL_HASH_MASK;
         // Red component
         iOffset += IPL_HASH_BITS;
         if (iOffset)
            iKey |= (iOffset > 0) ? (iR & IPL_HASH_MASK) << iOffset : (iR & IPL_HASH_MASK) >> -iOffset;
         else
            iKey |= iR & IPL_HASH_MASK;
         // Add a hit to the hash table entry
         piEntries[iKey]++;
         // Step to the next
         iCount++;
      }
      // Allocate the entries
      iCount = 0;
      SHash *psEntry = m_psHash;
      while (iCount < iSize) {
         // Allocate the entry table
         if (piEntries[iCount]) {
            psEntry->m_psColour = (SHashEntry*) new SHashEntry[piEntries[iCount]];
         }
         // Step through
         iCount++;
         psEntry++;
      }
      // Fill the table with the colour entries
      iCount = 0;
      SHashEntry *psColour = NULL;
      while (iCount < m_iSize) {
         // Get the palette entry
         uColour = m_puPalette[iCount];
         // Break into components
         iR = (uColour >> 16) & 0x000000FF;
         iG = (uColour >>  8) & 0x000000FF;
         iB = (uColour      ) & 0x000000FF;
         // Calculate the hash key
         iOffset = (IPL_HASH_BITS - 8);
         // Blue component
         if (iOffset)
            iKey = (iOffset > 0) ? (iB & IPL_HASH_MASK) << iOffset : (iB & IPL_HASH_MASK) >> -iOffset;
         else
            iKey = iB & IPL_HASH_MASK;
         // Green component
         iOffset += IPL_HASH_BITS;
         if (iOffset)
            iKey |= (iOffset > 0) ? (iG & IPL_HASH_MASK) << iOffset : (iG & IPL_HASH_MASK) >> -iOffset;
         else
            iKey |= iG & IPL_HASH_MASK;
         // Red component
         iOffset += IPL_HASH_BITS;
         if (iOffset)
            iKey |= (iOffset > 0) ? (iR & IPL_HASH_MASK) << iOffset : (iR & IPL_HASH_MASK) >> -iOffset;
         else
            iKey |= iR & IPL_HASH_MASK;
         // Add the hash table entry
         if (m_psHash[iKey].m_psColour) {
            // Find the next entry
            unsigned int &uEntry = m_psHash[iKey].m_uEntries;
            // Get the hash entry structure
            psColour = m_psHash[iKey].m_psColour + uEntry;
            // Set the palette info
            psColour->m_iIndex = iCount;
            psColour->m_uColour = uColour;
            psColour->m_iR = iR;
            psColour->m_iG = iG;
            psColour->m_iB = iB;
            // Step to the next entry
            uEntry++;
         }
         // Step to the next
         iCount++;
      }
      // Delete the temporary count table
      delete [] piEntries;
   } // CImagePalette::CreateHash

   int CImagePalette::CheckHash(unsigned int uColour) {
      // Mask the colour
      uColour &= 0x00FFFFFF;
      // Break into components
      int iR = (uColour >> 16) & 0x000000FF;
      int iG = (uColour >>  8) & 0x000000FF;
      int iB = (uColour      ) & 0x000000FF;
      // Calculate the hash key
      int iKey = 0;
      int iOffset = (IPL_HASH_BITS - 8);
      // Blue component
      if (iOffset)
         iKey = (iOffset > 0) ? (iB & IPL_HASH_MASK) << iOffset : (iB & IPL_HASH_MASK) >> -iOffset;
      else
         iKey = iB & IPL_HASH_MASK;
      // Green component
      iOffset += IPL_HASH_BITS;
      if (iOffset)
         iKey |= (iOffset > 0) ? (iG & IPL_HASH_MASK) << iOffset : (iG & IPL_HASH_MASK) >> -iOffset;
      else
         iKey |= iG & IPL_HASH_MASK;
      // Red component
      iOffset += IPL_HASH_BITS;
      if (iOffset)
         iKey |= (iOffset > 0) ? (iR & IPL_HASH_MASK) << iOffset : (iR & IPL_HASH_MASK) >> -iOffset;
      else
         iKey |= iR & IPL_HASH_MASK;
      // Check that hash entry contains colours
      const int iSize = m_psHash[iKey].m_uEntries;
      if (iSize == 0)
         return -1;
      // Match the colour the long way
      register int iCount = 0;
      SHashEntry *psEntry = m_psHash[iKey].m_psColour;
      int iIndex = -1;
      int iLowestError = 8192;		// Huge error
      int iRPal = 0, iGPal = 0, iBPal = 0;
      // Loop through every colour
      while (iCount < iSize) {
         // Check for an exact match
         if (uColour == psEntry->m_uColour)
            return psEntry->m_iIndex;
         // Calculate the individual component errors
         iRPal = abs(iR - psEntry->m_iR);
         iGPal = abs(iG - psEntry->m_iG);
         iBPal = abs(iB - psEntry->m_iB);
         // Sum the errors
         int iError = iRPal + iGPal + iBPal;
         // Record if lowest error
         if (iError < iLowestError) {
            iLowestError = iError;
            iIndex = psEntry->m_iIndex;
         }
         // Carry on through the palette
         iCount++;
         psEntry++;
      }
      // Check the threshhold
      if (iLowestError > IPL_HASH_THRESHHOLD)
         return -1;

      return iIndex;
   } // CImagePalette::CheckHash

   void CImagePalette::DeleteHash() {
      // Check we have a table to delete
      if (!m_psHash)
         return;
      // Go through every hash table entry
      int iSize = (int)pow(2, 3 * IPL_HASH_BITS);
      register int iCount = 0;
      SHash *psEntry = m_psHash;
      while (iCount < iSize) {
         if (psEntry->m_psColour)
            delete [] psEntry[iCount].m_psColour;
   		// Step to the next entry
         iCount++;
      }
      // Delete the table
      delete [] m_psHash;
      m_psHash = NULL;
   } // CImagePalette::DeleteHash

   int CImagePalette::FindLowest(unsigned int uColour) {
      // Mask the colour
      uColour &= 0x00FFFFFF;
      // Split the colour into components
      const int iR = (uColour >> 16) & 0x000000FF;
      const int iG = (uColour >>  8) & 0x000000FF;
      const int iB = (uColour      ) & 0x000000FF;
      // Match the colour the long way
      register int iCount = 0;
      unsigned int *puEntry = m_puPalette;
      unsigned int uPal = 0;
      int iIndex = -1;
      int iLowestError = 8192;		// Huge error
      int iRPal = 0, iGPal = 0, iBPal = 0;
      // Loop through every colour
      while (iCount < m_iSize) {
         // Get the colour
         uPal = *puEntry;
         // Calculate the individual component errors
         iRPal = abs(iR - ((uPal      ) & 0x000000FF));
         iGPal = abs(iG - ((uPal >>  8) & 0x000000FF));
         iBPal = abs(iB - ((uPal >> 16) & 0x000000FF));
         // Sum the errors
         int iError = iRPal + iGPal + iBPal;
         // Record if lowest error
         if (iError < iLowestError) {
            iLowestError = iError;
            iIndex = iCount;
         }
         // Carry on through the palette
         iCount++;
         puEntry++;
      }
      return iIndex;
   } // CImagePalette::FindLowest

}
