//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Palette.cpp - 19/03/2000 - Warren Moore
//	Palette implementation
//
// $Id: Palette.cpp,v 1.4 2000/08/07 18:58:33 waz Exp $
//

#include "stdafx.h"

#include "Palette.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CImagePalette

CImagePalette::CImagePalette(int iSize, unsigned int uColour) {
	// Set the initial parameters
	m_iSize = (iSize > 256) ? 256 : (iSize < 0) ? 0 : iSize;
	uColour &= 0x00FFFFFF;
	m_iNextEntry = 0;
	// Clear the palette
	int i = 256;
	while (i) {
		m_uPalette[--i] = uColour;
	}
	m_psCache = NULL;
} // Constructor

CImagePalette::CImagePalette(const CImagePalette &oCopy) {
	// Copy it all over
	m_iSize = oCopy.m_iSize;
	m_iNextEntry = oCopy.m_iNextEntry;
	memcpy(m_uPalette, oCopy.m_uPalette, m_iSize * sizeof(unsigned int));
	m_psCache = NULL;
} // Copy Constructor

CImagePalette::~CImagePalette() {
	DeleteCache();
} // Destructor

void CImagePalette::Copy(CImagePalette *pCopy) {
	ASSERT(pCopy);
	// Copy it all over
	m_iSize = pCopy->m_iSize;
	m_iNextEntry = pCopy->m_iNextEntry;
	memcpy(m_uPalette, pCopy->m_uPalette, m_iSize * sizeof(unsigned int));
} // Copy Constructor

//#===--- Setup

void CImagePalette::SetSize(int iSize) {
	// Check the size is within limits (0 - 256)
	m_iSize = (iSize > 256) ? 256 : (iSize < 0) ? 0 : iSize;
	// Set the AddEntry counter
	m_iNextEntry = 0;
	// Delete the cache
	DeleteCache();
} // SetSize

void CImagePalette::Clear(unsigned int uColour) {
	// Mask the input colour
	uColour &= 0x00FFFFFF;
	// Loop through entries setting clear colour
	int i = 256;
	while (i) {
		m_uPalette[--i] = uColour;
	}
	// Delete the cache
	DeleteCache();
} // Clear

//#===--- Palette

int CImagePalette::AddEntry(unsigned int uColour) {
	ASSERT(m_iNextEntry < m_iSize);
	// If not within palette size bail out
	if (m_iNextEntry >= m_iSize)
		return -1;
	// Set the palette entry
	uColour &= 0x00FFFFFF;
	m_uPalette[m_iNextEntry] = uColour;
	return m_iNextEntry++;
} // AddEntry

void CImagePalette::SetEntry(int iIndex, unsigned int uColour) {
	ASSERT(iIndex < m_iSize);
	// If not within palette size bail out
	if (iIndex >= m_iSize) 
		return;
	// Set the palette entry
	uColour &= 0x00FFFFFF;
	m_uPalette[iIndex] = uColour;
} // SetEntry

void CImagePalette::GetEntry(int iIndex, unsigned int &uColour) const {
	ASSERT(iIndex < m_iSize);
	// Get the palette entry if within palette size, 0 otherwise
	uColour = (iIndex < m_iSize) ? m_uPalette[iIndex] : 0;
} // GetEntry

int CImagePalette::MatchColour(unsigned int uColour) {
	// Create the cache (does nothing if already create)
	CreateCache();
	// Try to find the colour in the cache
	int iVal = CheckCache(uColour);
	if (iVal != -1)
		return iVal;
	// Split the colour into components
	const int iR = (uColour >> 16) & 0x000000FF;
	const int iG = (uColour >>  8) & 0x000000FF;
	const int iB = (uColour      ) & 0x000000FF;
	// Match the colour the long way
	register int iCount = 0;
	unsigned int uPal = 0;
	int iLowestError = 8192;		// Huge error
	int iRPal = 0, iGPal = 0, iBPal = 0;
	// Loop through every colour
	while (iCount < m_iSize) {
		// Get the colour
		uPal = m_uPalette[iCount];
		// Calculate the individual component errors
		iRPal = abs(iR - ((uPal      ) & 0x000000FF));
		iGPal = abs(iG - ((uPal >>  8) & 0x000000FF));
		iBPal = abs(iB - ((uPal >> 16) & 0x000000FF));
		// Sum the errors
		int iError = iRPal + iGPal + iBPal;
		// Record if lowest error
		if (iError < iLowestError) {
			iLowestError = iError;
			iVal = iCount;
		}
		// Carry on through the palette
		iCount++;
	}
	// Add the cache entry
	AddCacheEntry(uColour, iVal);
	return iVal;
} // MatchColour

void CImagePalette::CreateCache() {
	// Return if cache created
	if (m_psCache)
		return;
	// Create the new
	NEWBEGIN
	m_psCache = (SCache*) new SCache[IPL_CACHE_SIZE];
	NEWEND("CImagePalette::CreateCache - Palette cache memory")
} // CreateCache

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
} // CheckCache

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
	if (m_psCache) {
		delete [] m_psCache;
		m_psCache = NULL;
	}
} // DeleteCache

