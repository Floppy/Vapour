//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Palette.h - 19/03/2000 - Warren Moore
//	Palette header
//
// $Id: Palette.h,v 1.5 2000/07/30 20:56:56 waz Exp $
//

#ifndef _VAL_PALETTE_
#define _VAL_PALETTE_

#pragma once

//#===--- Includes
#include "VAL.h"

/////////////
// CPalette

class CImagePalette {
public:
	//#===--- Constructions
	CImagePalette(int iSize = 256, unsigned long uColour = 0);
	CImagePalette(const CImagePalette &oCopy);
	~CImagePalette();
	void Copy(CImagePalette *pCopy);

	//#===--- Settings
	void SetSize(int iSize);
	int GetSize() const;
	void Clear(unsigned long uColour = 0);

	//#===--- Entry Manipulation
	int AddEntry(unsigned long uColour);
	void SetEntry(int iIndex, unsigned long uColour);
	void GetEntry(int iIndex, unsigned long &uColour) const;

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

#endif // _VAL_PALETTE_

