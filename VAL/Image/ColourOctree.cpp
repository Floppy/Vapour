//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctree.cpp - 26/12/1999 - Warren Moore
//	Octree colour cube implementation
//
// $Id: ColourOctree.cpp,v 1.5 2000/08/07 18:58:12 waz Exp $
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
	m_poParent = NULL;
	m_cPNode = 0;
	memset(m_poChild, 0, 8 * sizeof(CColourOctree*));
	m_cRPos = m_cGPos = m_cBPos = 0;
	m_cSize = 128;
	m_iTotal = 0;
	m_cColours = 0;

	m_bValCache = false;
	m_bSetCache = false;
	m_bCacheClear = true;
	m_pbCacheClear = &m_bCacheClear;
} // Constructor

CColourOctree::CColourOctree(const CColourOctree &oCopy) {
	// Copy the active children data
	m_cActive = oCopy.m_cActive;
	m_poParent = oCopy.m_poParent;
	m_cPNode = oCopy.m_cPNode;
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		// If children are active, create copies of them
		if (m_cActive & cMask) {
			CColourOctree *poTemp = NULL;
			NEWBEGIN
			poTemp = (CColourOctree*) new CColourOctree(*oCopy.m_poChild[cNode]);
			NEWEND("CColourOctree::CColourOctree(copy) - Child octant")
			// If memory allocated okay, copy the child pointer
			if (poTemp)
				m_poChild[cNode] = poTemp;
			// Set the child empty and uncoloured, if not
			else
				m_poChild[cNode] = (CColourOctree*)0;
		}
		// ... otherwise, copy the colour data
		else 
			m_poChild[cNode] = oCopy.m_poChild[cNode];
		cMask <<= 1;
	}
	// Copy the other variables
	m_cRPos = oCopy.m_cRPos;
	m_cGPos = oCopy.m_cGPos;
	m_cBPos = oCopy.m_cBPos;
	m_cSize = oCopy.m_cSize;
	m_iTotal = oCopy.m_iTotal;
	m_cColours = oCopy.m_cColours;
	m_bValCache = oCopy.m_bValCache;
	m_bSetCache = oCopy.m_bSetCache;
	m_iValSum = oCopy.m_iValSum;
	m_iSetSum = oCopy.m_iSetSum;
	m_bCacheClear = oCopy.m_bCacheClear;
	m_pbCacheClear = oCopy.m_pbCacheClear;
} // Copy Constructor

CColourOctree::~CColourOctree() {
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		// If children are active, delete them
		if (m_cActive & cMask)
			delete m_poChild[cNode];
		cMask <<= 1;
	}
} // Destructor

	//#===--- Child Functions

CColourOctree &CColourOctree::Spawn(const unsigned char cNode) {
	unsigned char cMask = 0x01 << cNode;
	// Check if the node contains a value, to set whether cache should be cleared after spawn
	bool bClearCache = IsSet(cNode);
	// Check if the node is already active
	if (!(cMask & m_cActive)) {
		// If not, create a new octant with settings provided
		CColourOctree *poTemp = NULL;
		NEWBEGIN
		poTemp = (CColourOctree*) new CColourOctree();
		NEWEND("CColourOctree::Spawn - Child octant")
		// Error checking
		if (!poTemp)
			return *this;
		// Set node parent details
		poTemp->m_poParent = this;
		poTemp->m_cPNode = cNode;
		// Set child details
		m_poChild[cNode] = poTemp;
		m_cActive |= cMask;
		poTemp->m_bValCache = false;
		poTemp->m_bSetCache = false;
		poTemp->m_pbCacheClear = m_pbCacheClear;
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
		poTemp->m_cSize = m_cSize >> 1;
		poTemp->m_cRPos = cRPos;
		poTemp->m_cGPos = cGPos;
		poTemp->m_cBPos = cBPos;
	}
	// Clear the cache
	if (bClearCache)
		ClearCache();
	// Return the new octant (as a reference)
	return *m_poChild[cNode];
} // Spawn

CColourOctree &CColourOctree::GetChild(const unsigned char cNode) const {
	// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return *(CColourOctree*)m_poChild[cNode];

	// Show debug error message if inactive node
	TRACE("Octree GetChild accessing inactive node (%d)\n", cNode);
	return *(CColourOctree*)this;
} // GetChild

void CColourOctree::Kill(const unsigned char cNode) {
	unsigned char cMask = 0x01 << cNode;
	// If children are active, delete them
	if (cMask & m_cActive) {
		delete m_poChild[cNode];
		m_poChild[cNode] = (CColourOctree*)0;
		m_cActive &= ~cMask;
	}
	// Clear the cache
	ClearCache();
} // Kill

void CColourOctree::Flatten() {
	// Delete the octree's children and save the values
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		if (cMask & m_cActive) {
			// Save the child's values
			int iVal = m_poChild[cNode]->SumVals();
			// Delete the child
			delete m_poChild[cNode];
			m_poChild[cNode] = NULL;
			m_cActive &= ~cMask;
			// Set the value
			SetVal(cNode, iVal);
		}
		cMask <<= 1;
	}
	m_cActive = 0;
	// Clear the cache
	ClearCache();
} // Flatten

//#===--- Octant Value Functions

bool CColourOctree::IsSet(const unsigned char cNode) const {
	// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return false;
	return (((int)m_poChild[cNode]) > 0);
} // IsSet

bool CColourOctree::AnySet() const {
	return (m_iTotal > 0);
} // AnySet

void CColourOctree::SetVal(const unsigned char cNode, int iVal) {
	// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (!(cMask & m_cActive)) {
		// Get the current value
		int iCur = ((int)m_poChild[cNode]);
		// Add or remove a child if necessary
		if ((iCur == 0) && (iVal != 0))
			m_cColours++;
		if ((iCur != 0) && (iVal == 0))
			m_cColours--;
		// Make changes to the total value
		m_iTotal += (iVal - iCur);
		// Set the node the value specified
		m_poChild[cNode] = (CColourOctree*)iVal;
	}
	// Clear the cache
	ClearCache();
} // SetVal

void CColourOctree::IncVal(const unsigned char cNode) {
	// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (!(cMask & m_cActive)) {
		int iVal = ((int)m_poChild[cNode]) + 1;
		if (iVal == 1)
			m_cColours++;
		m_poChild[cNode] = ((CColourOctree*)iVal);
		m_iTotal++;
	}
	// Clear the cache
	ClearCache();
} // IncVal

int CColourOctree::GetVal(const unsigned char cNode) const {
	// Check if the child is active
	unsigned char cMask = 0x01 << cNode;
	if (cMask & m_cActive)
		return 0;
	return ((int)m_poChild[cNode]);
} // GetVal

int CColourOctree::GetTotal() const {
	return m_iTotal;
} // GetTotal

int CColourOctree::SumVals() {
	// Check for a cache value
	if (m_bValCache)
		return m_iValSum;
	// Initial count zero
	int iCount = 0;
	unsigned char cMask = 0x01;
	// Count children and octants
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		if (cMask & m_cActive)
			iCount += m_poChild[cNode]->SumVals();
		cMask <<= 1;
	}
	iCount += m_iTotal;
	// Set the cache value
	*m_pbCacheClear = false;
	m_bValCache = true;
	m_iValSum = iCount;
	// Return the sum
	return iCount;
} // SumVals

int CColourOctree::NumSet() const {
	return m_cColours;
} // NumSet

int CColourOctree::SumSet() {
	// Check for a cache value
	if (m_bSetCache)
		return m_iSetSum;
	// Initial count zero
	int iCount = 0;
	unsigned char cMask = 0x01;
	// Count children and octants
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		if (cMask & m_cActive)
			iCount += m_poChild[cNode]->SumSet();
		cMask <<= 1;
	}
	iCount += m_cColours;
	// Set the cache value
	*m_pbCacheClear = false;
	m_bSetCache = true;
	m_iSetSum = iCount;
	// Return the sum
	return iCount;
} // SumSet

//#===--- Colour Functions

unsigned char CColourOctree::GetNode(unsigned char cR, unsigned char cG, unsigned char cB) {
	unsigned char cNode = 255;

	unsigned char cRPos = m_cRPos + m_cSize;
	unsigned char cGPos = m_cGPos + m_cSize;
	unsigned char cBPos = m_cBPos + m_cSize;

	if (cR >= cRPos)
		if (cG >= cGPos)
			if (cB >= cBPos)
				cNode = 7;
			else
				cNode = 5;
		else
			if (cB >= cBPos)
				cNode = 3;
			else
				cNode = 1;
	else
		if (cG >= cGPos)
			if (cB >= cBPos)
				cNode = 6;
			else
				cNode = 4;
		else
			if (cB >= cBPos)
				cNode = 2;
			else
				cNode = 0;

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

/*
	// Find centre of node
	unsigned char cMid = m_cSize >> 1;
	cR = cRPos + cMid;
	cG = cGPos + cMid;
	cB = cBPos + cMid;
*/
	cR = cRPos;
	cG = cGPos;
	cB = cBPos;
} // GetColour

void CColourOctree::ClearCache() {
	// Return if the cache is still clear
	if (*m_pbCacheClear)
		return;
	// Only start clearing the cache from the top
	if (m_poParent != NULL)
		m_poParent->ClearCache();
	else {
		ClearCacheValues();
		*m_pbCacheClear = true;
	}
} // ClearCache

void CColourOctree::ClearCacheValues() {
	// Clear the cache flags
	m_bValCache = false;
	m_bSetCache = false;
	// Clear the children too
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		if (cMask & m_cActive)
			m_poChild[cNode]->ClearCacheValues();
		cMask <<= 1;
	}
} // ClearCacheValues

