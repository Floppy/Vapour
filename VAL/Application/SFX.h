//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SFX.h - 10/07/2000 - Warren Moore
//	Class for self-management of self-extracting wedgies
//
// $Id: SFX.h,v 1.3 2000/11/27 20:32:40 waz Exp $
//

#ifndef _VAL_SFX_
#define _VAL_SFX_

#pragma once

//#===--- Includes
#include <fstream.h>

//#===--- Defines
#define SFX_START					"WJESFXSTART"
#define SFX_START_SIZE		sizeof(SFX_START)

//#===--- Data types

/////////
// CSFX

class CSFX {
public:
	CSFX();
	~CSFX();

//#===--- External Functions
	// Returns true if the file has been modified with the file end position
	bool EndSet();
	// Inserts the file end position, if not previously set. Returns true if ok
	bool SetEnd(const char *pcEXEName = NULL);
	
	// Returns an input file stream set to the end of the file, NULL on error
	// The file stream must be deleted after use
	ifstream *GetWedgie();

protected:
//#===--- Internal Functions
	// Return the stream pos of the WJE start pos
	unsigned int FindPos(fstream &oFile);

//#===--- Internal Data
	static unsigned char m_pcStartID[SFX_START_SIZE];
	static unsigned char m_pcMagic[4];
	static unsigned int m_uWJEPos;
};

#endif // _VAL_SFX_
