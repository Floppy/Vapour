//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourQuant.cpp - 26/12/1999 - Warren Moore
//	Colour quantiser implementation
//
// $Id: ColourQuant.cpp,v 1.2 2000/06/17 10:42:19 waz Exp $
//

#include "StdAfx.h"

#include "ColourQuant.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CColourQuant::CColourQuant() {
	m_oPalette.SetSize(0);
	m_iColours = 0;
	m_pOctree = NULL;
	m_bGenerated = false;
} // Constructor

CColourQuant::~CColourQuant() {
	if (m_pOctree)
		delete m_pOctree;
} // Destructor

//#===--- Setup
void CColourQuant::SetSize(int iNum) {
	ASSERT(m_pOctree == NULL);
// Set the palette size
	m_oPalette.SetSize(iNum);
// Create the colour octree
	NEWBEGIN
	m_pOctree = new CColourOctree();
	NEWEND("CColourQuant::SetSize - Colour octree")
	if (m_pOctree) {
		m_iColours = 0;
		m_bGenerated = false;
	}
// Clear the lists
	for (int i = 0; i < 8; i++)
		m_oJointList[i].Clear();
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
	if (m_pOctree) {
		delete m_pOctree;
		m_pOctree = NULL;
	}
// Clear the lists
	for (int i = 0; i < 8; i++)
		m_oJointList[i].Clear();
} // Clear

CImagePalette *CColourQuant::GetPalette() {
	return &m_oPalette;
} // GetPalette

//#===--- Colour
void CColourQuant::AddColour(unsigned long uColour) {
	if (m_bGenerated)
		return;

	ASSERT(m_pOctree);
// Add the colour to the colour cube (and reverse pixel format)
	m_cR = (unsigned char)uColour;
	m_cG = (unsigned char)(uColour >> 8);
	m_cB = (unsigned char)(uColour >> 16);
	PlaceColour(*m_pOctree);
} // AddColour

void CColourQuant::CreateLists() {
	if (m_bGenerated)
		return;

	ASSERT(m_pOctree);
// Clear list
	for (unsigned char c = 0; c < 8; c++) 
		m_oJointList[c].Clear();

// Traverse tree
	FindJoint(128, *m_pOctree);
} // CreateLists

void CColourQuant::ClearLists() {
// Clear list
	for (unsigned char c = 0; c < 8; c++) 
		m_oJointList[c].Clear();
} // ClearLists

void CColourQuant::ReduceColour() {
	if (m_bGenerated)
		return;

	unsigned char cLevel = 7;
// Find the lowest joint list with entries
	while (m_oJointList[cLevel].Empty()) {
//		ASSERT(cLevel != 0);
		cLevel--;
	}

// Get the best octant
	int iLowest = 0;
	CColourOctree &oOctant = m_oJointList[cLevel].FindBest(iLowest);
	m_iColours -= (oOctant.NumSet() - 1);

// Remove the best option from the octree
	unsigned char cNode = 255;
	CColourOctree &oParent = oOctant.GetParent(cNode);
	ASSERT(&oParent != m_pOctree);
	if (&oParent != m_pOctree) {
		ASSERT(iLowest > 0);
		oParent.Kill(cNode);
		oParent.SetVal(cNode, iLowest);
	}
} // Reduce

int CColourQuant::MatchColour(unsigned long uColour) {
	if (!m_bGenerated)
		return 0;

	ASSERT(m_pOctree);
// Find the colour in the cube (and reverse pixel format)
	m_cR = (unsigned char)uColour;
	m_cG = (unsigned char)(uColour >> 8);
	m_cB = (unsigned char)(uColour >> 16);
	return FindColour(*m_pOctree);
} // MatchColour

void CColourQuant::GeneratePalette() {
	if (m_bGenerated == true)
		return;

	ASSERT(m_pOctree);
// Generate the palette entries
	GenerateColour(*m_pOctree);

	m_bGenerated = true;
} // GeneratePalette

//#===--- Recursive Functions

void CColourQuant::PlaceColour(CColourOctree &oOctant) {
	unsigned char cNode = oOctant.GetNode(m_cR, m_cG, m_cB);
	unsigned char cSize = oOctant.Size();
// If not at max level, reach the furthest possible depth
	if ((cSize > 1)) {
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
		if (!oOctant.IsSet(cNode)) {
			m_iColours++;
			oOctant.SetVal(cNode, 1);
		// Keep to the set amount of colours
			if (m_iColours > m_oPalette.GetSize()) {
				CreateLists();
				ReduceColour();
				ClearLists();
			}
		}
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
//		ASSERT(oOctant.IsSet(cNode));
		iIndex = oOctant.GetVal(cNode) - 1;
//		ASSERT(iIndex < m_iSize);
	}

	return iIndex;
} // FindColour

void CColourQuant::FindJoint(unsigned char cSize, CColourOctree &oOctant) {
	unsigned char cMask = 0x01;
	for (unsigned char cNode = 0; cNode < 8; cNode++) {
		if (oOctant.Reducible()) {
			AddJoint(cSize, oOctant);
		}
		if (oOctant.NodeActive(cNode)) {
			FindJoint(cSize >> 1, oOctant.GetChild(cNode));
		}
	}
} // FindJoint

//#===--- Joint Functions

void CColourQuant::AddJoint(unsigned char cSize, CColourOctree &oOctant) {
//	ASSERT(cSize > 0);
// Calculate level
	unsigned char cLevel = 7;
	while ((cSize & 0x01) == 0) {
		cLevel--;
		cSize >>= 1;
	}
//	ASSERT(cLevel < 8);
// Add joint!
	m_oJointList[cLevel].AddOctant(oOctant);
} // AddJoint
