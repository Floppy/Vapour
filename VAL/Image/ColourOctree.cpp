//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctree.cpp - 26/12/1999 - Warren Moore
//	Octree colour cube implementation
//
// $Id: ColourOctree.cpp,v 1.3 2000/07/10 09:15:39 waz Exp $
//

#include "stdafx.h"

#include "VAL.h"
#include "ColourOctree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CColourOctree

CColourOctree::CColourOctree() {
// No children active
	m_cActive = 0;
	m_pParent = NULL;
	m_cPNode = 0;
	for (int i=0; i<8; i++) {
	// Set all values to zero
		m_pChild[i] = (CColourOctree*)0;
	}
	m_cRPos = m_cGPos = m_cBPos = 0;
	m_cSize = 128;
} // Constructor

CColourOctree::CColourOctree(const CColourOctree &copy) {
// Copy the active children data
	m_cActive = copy.m_cActive;
	m_pParent = copy.m_pParent;
	m_cPNode = copy.m_cPNode;
	unsigned char cMask = 0x01;
	for (unsigned char i=0; i<8; i++) {
	// If children are active, create copies of them
		if (m_cActive & cMask) {
			CColourOctree *pTemp = NULL;
			NEWBEGIN
			pTemp = (CColourOctree*) new CColourOctree(*copy.m_pChild[i]);
			NEWEND("CColourOctree::CColourOctree(copy) - Child octant")
		// If memory allocated okay, copy the child pointer
			if (pTemp)
				m_pChild[i] = pTemp;
		// Set the child empty and uncoloured, if not
			else
				m_pChild[i] = (CColourOctree*)0;
		}
	// ... otherwise, copy the colour data
		else 
			m_pChild[i] = copy.m_pChild[i];
		cMask <<= 1;
	}
	m_cRPos = copy.m_cRPos;
	m_cGPos = copy.m_cGPos;
	m_cBPos = copy.m_cBPos;
	m_cSize = copy.m_cSize;
} // Copy Constructor

CColourOctree::~CColourOctree() {
	unsigned char cMask = 0x01;
	for (unsigned char i=0; i<8; i++) {
	// If children are active, delete them
		if (m_cActive & cMask)
			delete m_pChild[i];
		cMask <<= 1;
	}
} // Destructor

//#===--- Child Functions

CColourOctree &CColourOctree::Spawn(const unsigned char cNode) {
//	ASSERT(cNode < 8);
	unsigned char cMask = 0x01 << cNode;
// Check if the node is already active
	if (!(cMask & m_cActive)) {
	// If not, create a new octant with settings provided
		CColourOctree *pTemp = NULL;
		NEWBEGIN
		pTemp = (CColourOctree*) new CColourOctree();
		NEWEND("CColourOctree::Spawn - Child octant")
	// Error checking
		if (!pTemp)
			return *this;
	// Set node parent details
		pTemp->m_pParent = this;
		pTemp->m_cPNode = cNode;
	// Set child details
		m_pChild[cNode] = pTemp;
		m_cActive |= cMask;
	// Calculate new size and offsets for the node
		unsigned int cRPos = m_cRPos, cGPos = m_cGPos, cBPos = m_cBPos;
		switch (cNode) {
			case 0:
				break;
			case 1:
				cRPos += m_cSize; break;
			case 2:
																						cBPos += m_cSize; break;
			case 3:
				cRPos += m_cSize;                   cBPos += m_cSize; break;
			case 4:
													cGPos += m_cSize; break;
			case 5:
				cRPos += m_cSize; cGPos += m_cSize; break;
			case 6:
													cGPos += m_cSize; cBPos += m_cSize; break;
			case 7:
				cRPos += m_cSize; cGPos += m_cSize; cBPos += m_cSize; break;
		}
	// Set the new variables
		pTemp->m_cSize = m_cSize >> 1;
		pTemp->m_cRPos = cRPos;
		pTemp->m_cGPos = cGPos;
		pTemp->m_cBPos = cBPos;
	}
// Return the new octant (as a reference)
	return *m_pChild[cNode];
} // Spawn

CColourOctree &CColourOctree::GetChild(const unsigned char cNode) const {
//	ASSERT(cNode < 8);
// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return *(CColourOctree*)m_pChild[cNode];

// Show debug error message if inactive node
	TRACE("Octree GetChild accessing inactive node (%d)\n", cNode);
	return *(CColourOctree*)this;
} // GetChild

bool CColourOctree::Reducible() const {
	bool bReducible = false;
	bool bSingle = false;
	bool bBuffer = false;
	register unsigned char cPos = 0;
	while ((!bReducible) && (cPos < 8)) {
		bBuffer = (((int)m_pChild[cPos]) > 0);
		bReducible = bSingle && bBuffer;
		bSingle = bSingle || bBuffer;
		cPos++;
	}
	return bReducible;
} // Reducible

void CColourOctree::Kill(const unsigned char cNode) {
//	ASSERT(cNode < 8);
// Delete the octree's children
	unsigned char cMask = 0x01 << cNode;
// If children are active, delete them
	if (cMask & m_cActive) {
		delete m_pChild[cNode];
		m_pChild[cNode] = (CColourOctree*)0;
		m_cActive &= ~cMask;
	}
} // KillChildren

//#===--- Octant Value Functions

bool CColourOctree::IsSet(const unsigned char cNode) const {
//	ASSERT(cNode < 8);
// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return false;
	return (((int)m_pChild[cNode]) > 0);
} // IsSet

bool CColourOctree::AnySet() const {
// Initial count zero
	bool bSet = false;
	unsigned char cMask = 0x01;
// Count children and octants
	for (unsigned char i=0; i<8; i++) {
		if (!(cMask & m_cActive))
			if (((int)m_pChild[i]) > 0)
				bSet = true;
		cMask <<= 1;
	}
	return bSet;
} // AnySet

void CColourOctree::SetVal(const unsigned char cNode, int iVal) {
//	ASSERT(cNode < 8);
// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (!(cMask & m_cActive))
		m_pChild[cNode] = (CColourOctree*)iVal;
} // SetVal

void CColourOctree::IncVal(const unsigned char cNode) {
//	ASSERT(cNode < 8);
// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (!(cMask & m_cActive)) {
		int iVal = ((int)m_pChild[cNode]) + 1;
		m_pChild[cNode] = ((CColourOctree*)iVal);
	}
} // IncVal

int CColourOctree::GetVal(const unsigned char cNode) const {
//	ASSERT(cNode < 8);
// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return -1;
	return ((int)m_pChild[cNode]);
} // GetVal

int CColourOctree::SumVals() const {
// Initial count zero
	int iCount = 0;
	unsigned char cMask = 0x01;
// Count children and octants
	for (unsigned char i=0; i<8; i++) {
		if (cMask & m_cActive)
			iCount += m_pChild[i]->SumVals();
		else
			iCount += ((int)m_pChild[i]);
		cMask <<= 1;
	}
	return iCount;
} // SumVals

int CColourOctree::SumNodes() const {
// Initial count zero
	int iCount = 0;
	unsigned char cMask = 0x01;
// Count children and octants
	for (unsigned char i=0; i<8; i++) {
		if (cMask & m_cActive)
			iCount += m_pChild[i]->SumVals();
		else
			if (((int)m_pChild[i]) > 0) 
				iCount++;
		cMask <<= 1;
	}
	return iCount;
} // SumNodes

int CColourOctree::NumSet() const {
	int iChildren = 0;
	unsigned char cMask = 0x01;
	for (unsigned char c = 0; c < 8; c++) {
		if (cMask & m_cActive)
			iChildren += m_pChild[c]->NumSet();
		else
			if (((int)m_pChild[c]) > 0)
				iChildren++;
		cMask <<= 1;
	}
	return iChildren;
} // NumSet

//#===--- Colour Functions

unsigned char CColourOctree::GetNode(unsigned char cR, unsigned char cG, unsigned char cB) {
	unsigned char cNode = 255;
//	ASSERT((cR >= m_cRPos) && (cR < (m_cRPos + (m_cSize << 1))));
//	ASSERT((cG >= m_cGPos) && (cG < (m_cGPos + (m_cSize << 1))));
//	ASSERT((cB >= m_cBPos) && (cB < (m_cBPos + (m_cSize << 1))));

	unsigned char cRPos = m_cRPos + m_cSize;
	unsigned char cGPos = m_cGPos + m_cSize;
	unsigned char cBPos = m_cBPos + m_cSize;

	if (cR >= cRPos) {
		if (cG >= cGPos) {
			if (cB >= cBPos) {
				cNode = 7;
			}
			else {
				cNode = 5;
			}
		}
		else {
			if (cB >= cBPos) {
				cNode = 3;
			}
			else {
				cNode = 1;
			}
		}
	}
	else {
		if (cG >= cGPos) {
			if (cB >= cBPos) {
				cNode = 6;
			}
			else {
				cNode = 4;
			}
		}
		else {
			if (cB >= cBPos) {
				cNode = 2;
			}
			else {
				cNode = 0;
			}
		}
	}

	return cNode;
} // GetNode

void CColourOctree::GetColour(unsigned char cNode, unsigned char &cR,
																 unsigned char &cG, unsigned char &cB) {
// Calculate new size and offsets for the node
	unsigned int cRPos = m_cRPos, cGPos = m_cGPos, cBPos = m_cBPos;

	switch (cNode) {
		case 0:
			break;
		case 1:
			cRPos += m_cSize; break;
		case 2:
			                                    cBPos += m_cSize; break;
		case 3:
			cRPos += m_cSize;                   cBPos += m_cSize; break;
		case 4:
			                  cGPos += m_cSize; break;
		case 5:
			cRPos += m_cSize; cGPos += m_cSize; break;
		case 6:
			                  cGPos += m_cSize; cBPos += m_cSize; break;
		case 7:
			cRPos += m_cSize; cGPos += m_cSize; cBPos += m_cSize; break;
	}

// Find centre of node
	unsigned char cMid = m_cSize >> 1;
	cR = cRPos + cMid;
	cG = cGPos + cMid;
	cB = cBPos + cMid;
} // GetColour

//#===--- Diagnostic Functions

unsigned int CColourOctree::GetDepth() const {
// Initial depth zero
	unsigned int iDepth = 0;
	if (m_cActive) {
	// If there are children, increase depth by 1
		iDepth = 1;
		unsigned int iChildDepth = 0;
	// Traverse children
		unsigned char cMask = 0x01;
		for (unsigned char i=0; i<8; i++) {
			if (cMask & m_cActive) {
			// Find the deepest child
				unsigned int iCurrentDepth = m_pChild[i]->GetDepth();
				if (iCurrentDepth > iChildDepth)
					iChildDepth = iCurrentDepth;
			}
			cMask <<= 1;
		}
	// Add the deepest child to the current depth
		iDepth += iChildDepth;
	}
	return iDepth;
} // GetDepth

