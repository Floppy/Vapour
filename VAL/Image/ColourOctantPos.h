//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// OctantPos.h - 27/12/1999 - Warren Moore
//	Octree position calculator declaration
//
// $Id: ColourOctantPos.h,v 1.1 2000/06/16 21:59:42 waz Exp $
//

#ifndef _COLOUROCTANTPOS_
#define _COLOUROCTANTPOS_

#pragma once

/////////////////////
// CColourOctantPos

class CColourOctantPos {
public:
// Constructor
	CColourOctantPos();
	~CColourOctantPos();

// Child Functions
	CColourOctantPos &Spawn(unsigned char cNode);

// Colour Functions
	unsigned char GetNode(unsigned char cR, unsigned char cG, unsigned char cB);
	void GetColour(unsigned char cNode, unsigned char &cR, unsigned char &cG, unsigned char &cB);

// Output Functions
	unsigned char Size() const;

// Diagnostic Functions
	void Display() const;

//#===--- Internal Data
private:
	unsigned char m_cRPos, m_cGPos, m_cBPos;
	unsigned char m_cSize;
};

//#===--- Inline Functions
inline unsigned char CColourOctantPos::Size() const {
	return m_cSize;
} // Level (Get)

#endif // _COLOUROCTANTPOS_