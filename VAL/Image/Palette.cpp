//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Palette.cpp - 19/03/2000 - Warren Moore
//	Palette implementation
//
// $Id: Palette.cpp,v 1.3 2000/07/11 16:09:39 waz Exp $
//

#include "stdafx.h"

#include "Palette.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////
// CImagePalette

CImagePalette::CImagePalette(int iSize, unsigned long uColour) {
// Set the initial parameters
	m_iSize = (iSize > 256) ? 256 : (iSize < 0) ? 0 : iSize;
	uColour &= 0x00FFFFFF;
	m_iNextEntry = 0;
// Clear the palette
	int i = 256;
	while (i) {
		m_uPalette[--i] = uColour;
	}
} // Constructor

CImagePalette::CImagePalette(const CImagePalette &oCopy) {
// Copy it all over
	m_iSize = oCopy.m_iSize;
	m_iNextEntry = oCopy.m_iNextEntry;
	memcpy(m_uPalette, oCopy.m_uPalette, 256 * sizeof(unsigned long));
} // Copy Constructor

CImagePalette::~CImagePalette() {
} // Destructor

void CImagePalette::Copy(CImagePalette *pCopy) {
	ASSERT(pCopy);
// Copy it all over
	m_iSize = pCopy->m_iSize;
	m_iNextEntry = pCopy->m_iNextEntry;
	memcpy(m_uPalette, pCopy->m_uPalette, m_iSize * sizeof(unsigned long));
} // Copy Constructor

//#===--- Setup

void CImagePalette::SetSize(int iSize) {
	m_iSize = (iSize > 256) ? 256 : (iSize < 0) ? 0 : iSize;
	m_iNextEntry = 0;	
} // SetSize

void CImagePalette::Clear(unsigned long uColour) {
	uColour &= 0x00FFFFFF;
	int i = 256;
	while (i) {
		m_uPalette[--i] = uColour;
	}
} // Clear

//#===--- Palette

int CImagePalette::AddEntry(unsigned long uColour) {
	ASSERT(m_iNextEntry < m_iSize);
	uColour &= 0x00FFFFFF;
	m_uPalette[m_iNextEntry] = uColour;
	int iIndex = m_iNextEntry++;
	return iIndex;
} // AddEntry

void CImagePalette::SetEntry(int iIndex, unsigned long uColour) {
	ASSERT(iIndex < m_iSize);
	uColour &= 0x00FFFFFF;
	m_uPalette[iIndex] = uColour;
} // SetEntry

void CImagePalette::GetEntry(int iIndex, unsigned long &uColour) const {
	ASSERT(iIndex < m_iSize);
	uColour = m_uPalette[iIndex];
} // GetEntry

