//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctree.h - 26/12/1999 - Warren Moore
//	Octree colour cube declaration
//
// $Id: ColourOctree.h,v 1.1 2000/06/16 21:59:43 waz Exp $
//

#ifndef _COLOUROCTREE_
#define _COLOUROCTREE_

#pragma once

//#===--- Predeclatations
class CColourQuant;

////////////
// COctree

class CColourOctree {
public:
	CColourOctree();
	CColourOctree(const CColourOctree &copy);
	~CColourOctree();

// Child Functions
	bool NodeActive(const unsigned char cNode) const;
	CColourOctree &Spawn(const unsigned char cNode);
	CColourOctree &GetChild(const unsigned char cNode) const;
	CColourOctree &GetParent(unsigned char &cNode) const;
	bool Reducible() const;
	void Kill(const unsigned char cNode);

// Octant Fill Property Functions
	bool IsSet(const unsigned char cNode) const;
	bool AnySet() const;
	void SetVal(const unsigned char cNode, int iVal);
	void IncVal(const unsigned char cNode);
	int GetVal(const unsigned char cNode) const;
	int SumVals() const;
	int SumNodes() const;
	int NumSet() const;

// Diagnostics
	unsigned int GetDepth() const;

//#===--- Internal Data
private:
	unsigned char m_cActive;
	CColourOctree *m_pParent;
	unsigned char m_cPNode;
	CColourOctree *m_pChild[8];

};

//#===--- Inline Functions

inline bool CColourOctree::NodeActive(const unsigned char cNode) const {
	ASSERT(cNode < 8);
	unsigned char iActive = (0x01 << cNode) & m_cActive;
	return (iActive > 0);
} // NodeActive

inline CColourOctree &CColourOctree::GetParent(unsigned char &cNode) const {
	cNode = m_cPNode;
	return *m_pParent;
} // GetParent

#endif // _COLOUROCTREE_