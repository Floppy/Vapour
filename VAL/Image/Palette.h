//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Palette.h - 19/03/2000 - Warren Moore
//	Palette header
//
// $Id: Palette.h,v 1.6 2000/08/07 18:58:33 waz Exp $
//

#ifndef _VAL_PALETTE_
#define _VAL_PALETTE_

#pragma once

//#===--- Includes
#include "VAL.h"

//#===--- Defines
#define IPL_CACHE_SIZE			50
#define IPL_LRU_MAX					10

/////////////
// CPalette

class CImagePalette {
public:
	//#===--- Constructions
	// Create the palette of a specified size filled with a default colour (max 256)
	CImagePalette(int iSize = 256, unsigned int uColour = 0);
	CImagePalette(const CImagePalette &oCopy);
	~CImagePalette();
	// Copy the palette into current from a pointer
	void Copy(CImagePalette *pCopy);

	//#===--- Settings
	// Set the size of colours within the palette
	void SetSize(int iSize);
	// Returns the max numbers of colour within the palette
	int GetSize() const;
	// Clears the palette with a default colour
	void Clear(unsigned int uColour = 0);

	//#===--- Entry Manipulation
	// Add an entry to the palette (maintains an internal counter, returns the index, -1 if invalid)
	int AddEntry(unsigned int uColour);
	// Set a specific entry with a colour
	void SetEntry(int iIndex, unsigned int uColour);
	// Get a specific entry (0 if invalid index)
	void GetEntry(int iIndex, unsigned int &uColour) const;

	//#===--- Colour Matching
	// Returns a palette index closest matching the supplied colour
	int MatchColour(unsigned int uColour);

//#===--- Internal Functions
protected:
	//#===--- Matched colour cache
	// Creates a cache for recently matched values
	void CreateCache();
	// Returns a palette index if the colour was cached, -1 of not
	int CheckCache(unsigned int uColour);
	// Adds a colour index to the cache
	void AddCacheEntry(unsigned int uColour, int iIndex);
	// Deletes the cache
	void DeleteCache();

	//#===--- Palette entry hash table
	// Creates a hash table of the palette entries

//#===--- Internal Structures
	// Colour matching cache
	typedef struct SCacheStruct {
		unsigned int m_uColour;
		int m_iIndex;
		int m_iHits;

		SCacheStruct() {						// Inline constructor
			m_uColour = 0xFF000000;		// Invalid colour
			m_iIndex = -1;						// Invalid index
			m_iHits = -1;							// Value guaranteed to be initially overwritten
		}
	} SCache;

	// Palette hash table
	typedef struct SHashStruct {
		unsigned int m_uEntries;
		int *m_piIndex;

		SHashStruct() {							// Inline constructor
			m_uEntries = 0;
			m_piIndex = NULL;
		}
	} SHash;

//#===--- Internal Data
protected:
	int m_iSize;												// Max number of colour entries
	int m_iNextEntry;										// Internal counter for AddEntry function

	unsigned int m_uPalette[256];				// Palette data

	SCache *m_psCache;									// Colour match cache structure
};

//#===--- Inline Functions

inline int CImagePalette::GetSize() const {
	return m_iSize;
} // GetSize

#endif // _VAL_PALETTE_

