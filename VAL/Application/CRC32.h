//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CRC32.h - 12/06/2000 - Warren Moore
//  32 bit CRC declaration
//
// $Id: CRC32.h,v 1.2 2000/06/17 10:42:07 waz Exp $
//

#ifndef _VAL_CRC32_
#define _VAL_CRC32_

#pragma once

///////////
// CCRC32

class CCRC32 {
public:
	CCRC32();
	~CCRC32();

// External functions
	void CRC(unsigned long lCRC);
	unsigned long CRC() const;
	unsigned long Update(unsigned char *pData, int iSize);

private:
// Internal functions
	void MakeTable();

// Internal variables
	static bool m_bTable;
	static unsigned long m_lTable[256];

	unsigned long m_lCRC;
};

//#=- Inline functions

inline void CCRC32::CRC(unsigned long lCRC) {
	m_lCRC = lCRC;
} // CRC (Set)

inline unsigned long CCRC32::CRC() const {
	return m_lCRC;
} // CRC (Get)

#endif // _VAL_CRC32_