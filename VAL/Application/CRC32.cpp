//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CRC32.cpp - 12/06/2000 - Warren Moore
//  Simple 32 bit CRC
//
// $Id: CRC32.cpp,v 1.1 2000/06/16 21:57:45 waz Exp $
//

#include "StdAfx.h"

#include "CRC32.h"

///////////
// CCRC32

//#=- Static variables

bool CCRC32::m_bTable = false;

unsigned long CCRC32::m_lTable[256];

CCRC32::CCRC32() {
// Set the initial CRC
	m_lCRC = 0L;
// If not previously built, make the CRC table
	if (!m_bTable) {
		MakeTable();
	}
} // Constructor

CCRC32::~CCRC32() {
} // Destructor

void CCRC32::MakeTable() {
// See ISO 3309 and ITU-T V.42 for formal spec
	unsigned long l;
	for (int i = 0; i < 256; i++) {
		l = (unsigned long)i;
		for (int j = 0; j < 8; j++) {
			if (l & 1) {
				l = 0xEDB88320L ^ (l >> 1);
			}
			else {
				l >>= 1;
			}
		}
		m_lTable[i] = l;
	}
	m_bTable = true;
} // MakeTable

unsigned long CCRC32::Update(unsigned char *pData, int iSize) {
	unsigned long lCRC = m_lCRC ^ 0xFFFFFFFFL;
	int i;

	for (i = 0; i < iSize; i++) {
		lCRC = m_lTable[(lCRC ^ pData[i]) & 0xFF] ^ (lCRC >> 8); 
	}

	return m_lCRC = lCRC;
} // Update


