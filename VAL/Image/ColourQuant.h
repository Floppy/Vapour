//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourQuant.h - 26/12/1999 - Warren Moore
//	Colour quantiser declaration
//
// $Id: ColourQuant.h,v 1.4 2000/08/07 18:56:23 waz Exp $
//

#ifndef _VAL_COLOURQUANT_
#define _VAL_COLOURQUANT_

#pragma once

#include "ColourOctree.h"
#include "Palette.h"

/////////////////
// CColourQuant

class CColourQuant {
public:
	CColourQuant();
	~CColourQuant();

	//#===--- Setup
	// Clear the quantiser and set the number of colours to quantise to
	void SetSize(int iNum);
	// Returns the current max number of colours in the quantiser
	int GetSize() const;
	// Clear the quantiser
	void Clear();

	//#===--- Palette Functions
	// Returns a pointer to the palette object
	CImagePalette *GetPalette();

	//#===--- Colour
	// Adds a colour to the colour cube (can only be called before GeneratePalette)
	void AddColour(unsigned long uColour);
	// Generate a palette from the current colour cube
	void GeneratePalette();
	// Return a palette index for the specified colour (can only be called after GeneratePalette)
	int MatchColour(unsigned long uColour);

//#===--- Internal Functions
protected:
	//#===--- Colour
	// Reduce the number of colours by one (or more if cannot be avoided)
	// (can only be called before GeneratePalette)
	void ReduceColour();

	//#===--- Recursive Functions
	// Place specified colour within the colour cube
	void PlaceColour(CColourOctree &oOctant);
	// Convert the current colour cube entries to palette indices
	void GenerateColour(CColourOctree &oOctant);
	// Find the nearest palette index for the specified colour
	int FindColour(CColourOctree &oOctant);
	// Find the best octant to remove from the colour cube
	void FindBest(CColourOctree &oOctant, CColourOctree *&poOctant);

//#===--- Internal Data
protected:
	CColourOctree *m_poOctree;					// Colour cube structure
	int m_iColours;											// Number of colours in colour cube
	bool m_bGenerated;									// Palette generated indicator

	unsigned char m_cR, m_cG, m_cB;			// Current specified colour

	CImagePalette m_oPalette;						// Palette object
};

#endif // _VAL_COLOURQUANT_