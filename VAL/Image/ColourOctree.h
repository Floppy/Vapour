//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctree.h - 26/12/1999 - Warren Moore
//	Octree colour cube declaration
//
// $Id: ColourOctree.h,v 1.4 2000/08/06 19:21:48 waz Exp $
//

#ifndef _VAL_COLOUROCTREE_
#define _VAL_COLOUROCTREE_

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

	//#===--- Child Functions
	// Returns true if the node points to a child
	bool NodeActive(const unsigned char cNode) const;
	// Spawns a child at the specified node - returns reference to child
	CColourOctree &Spawn(const unsigned char cNode);
	// Returns a reference to the child at the specified node
	CColourOctree &GetChild(const unsigned char cNode) const;
	// Returns reference to the parent of the octant (NULL if this is root), parent-child node in cNode
	CColourOctree &GetParent(unsigned char &cNode) const;
	// Deletes the specified node
	void Kill(const unsigned char cNode);
	// Flattens all the child to this node
	void Flatten();

	//#===--- Octant Fill Property Functions
	// Returns whether the node contains a value
	bool IsSet(const unsigned char cNode) const;
	// Returns whether any node contains a value
	bool AnySet() const;
	// Sets the value of the specified node
	void SetVal(const unsigned char cNode, int iVal);
	// Increments the value of the specified node
	void IncVal(const unsigned char cNode);
	// Returns the value at the specified node
	int GetVal(const unsigned char cNode) const;
	// Returns the sum of all the values in this octant
	int GetTotal() const;
	// Returns the sum of all values in this and all child octants
	int SumVals() const;
	// Returns the number of values set
	int NumSet() const;
	// Returns the number of values set in this and all child octants
	int SumSet() const;

	//#===--- Colour Functions
	// Returns the current size of the octant (1 - 128)
	unsigned char Size() const;
	// Returns the node that is closest to the specified colour
	unsigned char GetNode(unsigned char cR, unsigned char cG, unsigned char cB);
	// Returns the colour of the specified node within the colour cube
	void GetColour(unsigned char cNode, unsigned char &cR, unsigned char &cG, unsigned char &cB);

	//#===--- Internal Data
private:
	unsigned char m_cActive;
	CColourOctree *m_pParent;
	unsigned char m_cPNode;
	CColourOctree *m_pChild[8];

	unsigned char m_cRPos, m_cGPos, m_cBPos;
	unsigned char m_cSize;
	unsigned int m_iTotal;
	unsigned char m_cColours;
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

inline unsigned char CColourOctree::Size() const {
	return m_cSize;
} // Size

#endif // _VAL_COLOUROCTREE_