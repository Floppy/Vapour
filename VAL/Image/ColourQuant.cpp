//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourQuant.cpp - 26/12/1999 - Warren Moore
//	Colour quantiser implementation
//
// $Id: ColourQuant.cpp,v 1.3 2000/08/06 19:21:47 waz Exp $
//

#include "StdAfx.h"

#include "ColourQuant.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////
// CColourQuant

CColourQuant::CColourQuant() {
	m_oPalette.SetSize(0);
	m_iColours = 0;
	m_poOctree = NULL;
	m_bGenerated = false;
} // Constructor

CColourQuant::~CColourQuant() {
	if (m_poOctree)
		delete m_poOctree;
} // Destructor

//#===--- Setup

void CColourQuant::SetSize(int iNum) {
	// Clear the quantiser
	if (m_poOctree)
		Clear();
	// Set the palette size
	m_oPalette.SetSize(iNum);
	// Create the colour octree
	NEWBEGIN
	m_poOctree = new CColourOctree();
	NEWEND("CColourQuant::SetSize - Colour octree")
	if (m_poOctree) {
		m_iColours = 0;
		m_bGenerated = false;
	}
} // SetSize

int CColourQuant::GetSize() const {
	return m_oPalette.GetSize();
} // GetSize

void CColourQuant::Clear() {
	// Clear the palette
	m_oPalette.Clear();
	// Set the params
	m_iColours = 0;
	m_bGenerated = false;
	// Delete the colour octree
	if (m_poOctree) {
		delete m_poOctree;
		m_poOctree = NULL;
	}
} // Clear

CImagePalette *CColourQuant::GetPalette() {
	return &m_oPalette;
} // GetPalette

//#===--- Colour
void CColourQuant::AddColour(unsigned long uColour) {
	if (m_bGenerated)
		return;

	ASSERT(m_poOctree);
	// Add the colour to the colour cube (and reverse pixel format)
	m_cR = (unsigned char)uColour;
	m_cG = (unsigned char)(uColour >> 8);
	m_cB = (unsigned char)(uColour >> 16);
	PlaceColour(*m_poOctree);
	// Keep to the set amount of colours
	if (m_iColours > m_oPalette.GetSize())
		ReduceColour();
} // AddColour

void CColourQuant::ReduceColour() {
	if (m_bGenerated)
		return;

	// Find the most suitable colour to remove
	CColourOctree *poBest = m_poOctree;
	FindBest(*m_poOctree, poBest);
	ASSERT(poBest != m_poOctree);

	// Set the correct number of colours left
	m_iColours -= (poBest->SumSet() - 1);

	// Get the octants parent
	unsigned char cNode = 255; // An invalid node number
	CColourOctree &oParent = poBest->GetParent(cNode);
	// Find the number of hits to set
	int iHits = poBest->SumVals();
	oParent.Kill(cNode);
	oParent.SetVal(cNode, iHits);
} // Reduce

int CColourQuant::MatchColour(unsigned long uColour) {
	if (!m_bGenerated)
		return 0;

	ASSERT(m_poOctree);
	// Find the colour in the cube (and reverse pixel format)
	m_cR = (unsigned char)uColour;
	m_cG = (unsigned char)(uColour >> 8);
	m_cB = (unsigned char)(uColour >> 16);
	return FindColour(*m_poOctree);
} // MatchColour

void CColourQuant::GeneratePalette() {
	if (m_bGenerated == true)
		return;

	ASSERT(m_poOctree);
	// Generate the palette entries
	GenerateColour(*m_poOctree);

	m_bGenerated = true;
} // GeneratePalette

//#===--- Recursive Functions

void CColourQuant::PlaceColour(CColourOctree &oOctant) {
	unsigned char cNode = oOctant.GetNode(m_cR, m_cG, m_cB);
	unsigned char cSize = oOctant.Size();
	// Try to reach the furthest possible depth, unless colours already set
	if (cSize > 1) {
		// If the child isn't active, inc value (if set), or spawn new child
		if (!oOctant.NodeActive(cNode)) {
			if (!oOctant.IsSet(cNode)) {
				// Spawn a new octant and follow it down
				oOctant.Spawn(cNode);
				PlaceColour(oOctant.GetChild(cNode));
			}
		}
		// If the child is active, follow it down
		else {
			PlaceColour(oOctant.GetChild(cNode));
		}
	}
	// If at bottom of tree, add a hit
	else {
		// Add a colour if first hit on new segment
		if (!oOctant.IsSet(cNode))
			m_iColours++;
		// Increment the colour value
		oOctant.IncVal(cNode);
	}
} // PlaceColour

void CColourQuant::GenerateColour(CColourOctree &oOctant) {
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		// Follow the octree down
		if (oOctant.NodeActive(cNode)) {
			GenerateColour(oOctant.GetChild(cNode));
		}
		// Otherwise find the colour of the octant, and set the palette index
		else {
			if (oOctant.IsSet(cNode)) {
				unsigned char cR, cG, cB;
				oOctant.GetColour(cNode, cR, cG, cB);
				unsigned long uColour = (cR << 16) | (cG << 8) | cB;
				int iEntry = m_oPalette.AddEntry(uColour);
				oOctant.SetVal(cNode, iEntry + 1);
			}
		}
	}
} // GenerateColour

int CColourQuant::FindColour(CColourOctree &oOctant) {
	unsigned char cNode = oOctant.GetNode(m_cR, m_cG, m_cB);
	int iIndex = 0;

	// Follow the octree down
	if (oOctant.NodeActive(cNode)) {
		iIndex = FindColour(oOctant.GetChild(cNode));
	}
	// Otherwise get the palette index entry
	else {
		iIndex = oOctant.GetVal(cNode) - 1;
	}

	return iIndex;
} // FindColour

void CColourQuant::FindBest(CColourOctree &oOctant, CColourOctree *&poBest) {
	int iCurSet = oOctant.SumSet();
	// Is the octant reducible?
	if (iCurSet > 1) {
		int iCurSize = oOctant.Size();
		int iBestSize = poBest->Size();
		// Only check octants of the same size or smaller
		if (iCurSize <= iBestSize)
			// If this octant is smaller, it qualifies
			if (iCurSize < iBestSize)
				poBest = &oOctant;
			else {
				// Only check octants with same number of hits or less
				int iBestSet = poBest->SumSet();
				if (iCurSet <= iBestSet) {
					// If this octant has less colours, it qualifies
					if (iCurSet < iBestSet)
						poBest = &oOctant;
					else
						// This octant qualifies if it has less hits
						if (oOctant.SumVals() < poBest->SumVals())
							poBest = &oOctant;
				}
			}
	}

	unsigned char cNode = 0;
	while (cNode < 8) {	
		// Follow the octree down
		if (oOctant.NodeActive(cNode))
			FindBest(oOctant.GetChild(cNode), poBest);
		cNode++;
	}
} // FindBest

