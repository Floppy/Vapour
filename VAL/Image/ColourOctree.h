//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctree.h - 26/12/1999 - Warren Moore
//	Octree colour cube declaration
//
// $Id: ColourOctree.h,v 1.5 2000/08/07 18:58:12 waz Exp $
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
	// Returns the sum of all values in this and all child octants (uses value cache)
	int SumVals();
	// Returns the number of values set
	int NumSet() const;
	// Returns the number of values set in this and all child octants (uses value cache);
	int SumSet();

	//#===--- Colour Functions
	// Returns the current size of the octant (1 - 128)
	unsigned char Size() const;
	// Returns the node that is closest to the specified colour
	unsigned char GetNode(unsigned char cR, unsigned char cG, unsigned char cB);
	// Returns the colour of the specified node within the colour cube
	void GetColour(unsigned char cNode, unsigned char &cR, unsigned char &cG, unsigned char &cB);

//#===--- Internal Functions
private:
	// Called when a recursive status functions is called so that sums are cached to speed up
	// all subsequent calls
	void SetCache();
	// Called when a tree modifying function is called to clear cache
	void ClearCache();
	// Recursive function to clear cache values
	void ClearCacheValues();
	// Recursive function to find the top of the tree from an arbitrary position
	void FindTop();

//#===--- Internal Data
private:
	unsigned char m_cActive;
	CColourOctree *m_poParent;
	unsigned char m_cPNode;
	CColourOctree *m_poChild[8];

	unsigned char m_cRPos, m_cGPos, m_cBPos;
	unsigned char m_cSize;
	unsigned int m_iTotal;
	unsigned char m_cColours;

	bool m_bValCache;
	bool m_bSetCache;
	int m_iValSum;
	int m_iSetSum;
	bool m_bCacheClear;
	bool *m_pbCacheClear;
};

//#===--- Inline Functions

inline bool CColourOctree::NodeActive(const unsigned char cNode) const {
	ASSERT(cNode < 8);
	unsigned char iActive = (0x01 << cNode) & m_cActive;
	return (iActive > 0);
} // NodeActive

inline CColourOctree &CColourOctree::GetParent(unsigned char &cNode) const {
	cNode = m_cPNode;
	return *m_poParent;
} // GetParent

inline unsigned char CColourOctree::Size() const {
	return m_cSize;
} // Size

#endif // _VAL_COLOUROCTREE_