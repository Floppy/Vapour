//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ColourOctantPos.cpp - 27/12/1999 - Warren Moore
//	Colour cube octree position calculator implementation
//
// $Id: ColourOctantPos.cpp,v 1.1 2000/06/16 21:59:42 waz Exp $
//

#include "stdafx.h"

#include "Defs.h"
#include "ColourOctantPos.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////
// CColourOctantPos

CColourOctantPos::CColourOctantPos() {
	m_cRPos = m_cGPos = m_cBPos = 0;
	m_cSize = 128;
} // Constructor

CColourOctantPos::~CColourOctantPos() {
} // Destructor

//#===--- Child Functions

CColourOctantPos &CColourOctantPos::Spawn(unsigned char cNode) {
	ASSERT(cNode < 8);
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

// Create the new OctantPos
	CColourOctantPos *pOctantPos = NULL;
	NEWBEGIN
	pOctantPos = new CColourOctantPos;
	NEWEND("CColourOctantPos::Spawn - Octant pos object")
// Error Checking
	if (!pOctantPos)
		return *this;

// Set the new variables
	pOctantPos->m_cSize = m_cSize >> 1;
	pOctantPos->m_cRPos = cRPos;
	pOctantPos->m_cGPos = cGPos;
	pOctantPos->m_cBPos = cBPos;

	return *pOctantPos;
} // Spawn

//#===--- Colour Functions

unsigned char CColourOctantPos::GetNode(unsigned char cR, unsigned char cG, unsigned char cB) {
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

void CColourOctantPos::GetColour(unsigned char cNode, unsigned char &cR,
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

void CColourOctantPos::Display() const {
	TRACE("OctantPos Display :-\n");
	TRACE("m_cSize : %4d  m_cRPos : %4d m_cGPos : %4d  m_cBPos : %4d\n",
		m_cSize, m_cRPos, m_cGPos, m_cBPos);
} // Display
