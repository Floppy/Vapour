//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourQuant.h - 26/12/1999 - Warren Moore
//	Colour quantiser declaration
//
// $Id: ColourQuant.h,v 1.1 2000/06/16 21:59:44 waz Exp $
//

#ifndef _COLOURQUANT_
#define _COLOURQUANT_

#pragma once

#include "ColourOctantPos.h"
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
	void PlaceColour(CColourOctantPos &oPos, CColourOctree &oOctant);
	void GenerateColour(CColourOctantPos &oPos, CColourOctree &oOctant);
	int FindColour(CColourOctantPos &oPos, CColourOctree &oOctant);
	void FindJoint(unsigned char cSize, CColourOctantPos &oPos, CColourOctree &oOctant);

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

#endif // _COLOURQUANT_