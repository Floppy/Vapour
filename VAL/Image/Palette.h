//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Palette.h - 19/03/2000 - Warren Moore
//	Palette header
//
// $Id: Palette.h,v 1.1 2000/06/16 21:59:40 waz Exp $
//

#ifndef _PALETTE_
#define _PALETTE_

#pragma once

//#===--- Includes
#include "Defs.h"

/////////////
// CPalette

class CImagePalette {
//#===--- Member Functions
public:
	CImagePalette(int iSize = 256, unsigned long uColour = 0);
	CImagePalette(const CImagePalette &oCopy);
	~CImagePalette();
	void Copy(CImagePalette *pCopy);

// Setup
	void SetSize(int iSize);
	int GetSize() const;
	void Clear(unsigned long uColour = 0);

// Palette
	int AddEntry(unsigned long uColour);
	void SetEntry(int iIndex, unsigned long uColour);
	void GetEntry(int iIndex, unsigned long &uColour);

//#===--- Internal Functions
protected:

//#===--- Internal Data
protected:
	int m_iSize;
	int m_iNextEntry;

	unsigned long m_uPalette[256];
};

//#===--- Inline Functions

inline int CImagePalette::GetSize() const {
	return m_iSize;
} // GetSize

#endif // _PALETTE_

