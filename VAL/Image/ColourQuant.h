//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourQuant.h - 26/12/1999 - Warren Moore
//	Colour quantiser declaration
//
// $Id: ColourQuant.h,v 1.2 2000/06/17 10:42:21 waz Exp $
//

#ifndef _VAL_COLOURQUANT_
#define _VAL_COLOURQUANT_

#pragma once

#include "ColourOctree.h"
#include "JointList.h"
#include <vector>
#include "Palette.h"

class CColourQuant {
//#===--- Functions
public:
	CColourQuant();
	~CColourQuant();

// Setup
	void SetSize(int iNum);
	int GetSize() const;
	void Clear();

	CImagePalette *GetPalette();

// Colour
	void AddColour(unsigned long uColour);
	void CreateLists();
	void ClearLists();
	void ReduceColour();
	int MatchColour(unsigned long uColour);
	void GeneratePalette();

//#===--- Internal Functions
protected:
// Recursive Functions
	void PlaceColour(CColourOctree &oOctant);
	void GenerateColour(CColourOctree &oOctant);
	int FindColour(CColourOctree &oOctant);
	void FindJoint(unsigned char cSize, CColourOctree &oOctant);

// Joint Functions
	void AddJoint(unsigned char cSize, CColourOctree &oOctant);

//#===--- Internal Data
protected:
	CColourOctree *m_pOctree;
	int m_iColours;
	bool m_bGenerated;

	unsigned char m_cR, m_cG, m_cB;
	CJointList m_oJointList[8];

	CImagePalette m_oPalette;
};

#endif // _VAL_COLOURQUANT_