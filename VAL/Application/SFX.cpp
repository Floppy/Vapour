//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SFX.h - 10/07/2000 - Warren Moore
//	Class for self-management of self-extracting wedgies
//
// $Id: SFX.cpp,v 1.4 2000/07/11 15:07:40 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "SFX.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////
// CSFX

//#===--- Static variables
unsigned char CSFX::m_pcStartID[SFX_START_SIZE] = SFX_START;
unsigned char CSFX::m_pcMagic[4] = { 0xFF, 0xF0, 0xF0, 0x00 };
unsigned int CSFX::m_uWJEPos = 0xFFFFFFFF;

CSFX::CSFX() {
} // Constructor

CSFX::~CSFX() {
} // Destructor

bool CSFX::EndSet() {
	return (m_pcMagic[3] != 0);
} // EndSet

bool CSFX::SetEnd() {
	// Check it hasn't been done already
	if (m_pcMagic[3] != 0)
		return true;
	// Get the app name
	ASSERT(g_poVAL);
	const char *pcAppName = g_poVAL->GetAppName();
	if (!pcAppName)
		return false;
	// Open the application
	fstream oFile;
	oFile.open(pcAppName, ios::in|ios::binary|ios::nocreate|ios::ate);
	if (oFile.fail())
		return false;
	// Get the end of the file
	unsigned int uEnd = oFile.tellg();
	// Seek to the beginning
	oFile.seekg(0, ios::beg);
	// Search for the identifier
	unsigned int uPos = FindPos(oFile);
	oFile.close();
	if (uPos == 0)
		return false;
	// Close the app and reopen writeable

	//#===--- TODO: Get this to open the app while running
	oFile.open(pcAppName, ios::in|ios::out|ios::binary|ios::nocreate, filebuf::sh_write);
	if (oFile.fail()) {
		//#=== TODO: Remove this once file open works ok
		CString strAddress;
		strAddress.Format("Pos address : 0x%08X - End pos : 0x%08X", uPos, uEnd);
		AfxMessageBox(strAddress, MB_OK);

		oFile.close();
		//#===--- TODO: Don't forget to set this to false when corrected
		return true;
	}
	// Seek to the position of the flag
	oFile.seekp(uPos - 1, ios::beg);
	// Write the flag
	oFile.put((char)0xFF);
	// Write the end position
	oFile.write((char*)&uEnd, 4);
	if (oFile.bad()) {
		oFile.close();
		return false;
	}
	oFile.close();

	// Set the object variables
	m_pcMagic[3] = 0xFF;
	m_uWJEPos = uPos;

	return true;
} // SetEnd
	
ifstream *CSFX::GetWedgie() {
	// Check the pos paramters
	if ((m_pcMagic[3] != 0xFF) || (m_uWJEPos == 0xFFFFFFFF))
		return NULL;
	// Get the app name
	ASSERT(g_poVAL);
	const char *pcAppName = g_poVAL->GetAppName();
	if (!pcAppName)
		return false;
	// Open the application
	ifstream *poFile = NULL;
	NEWBEGIN
	poFile = (ifstream*) new ifstream;
	NEWEND("CSFX::GetWedgie - File stream")
	if (!poFile)
		return NULL;
	// Open the file and seek to the wedgie pos
	poFile->open(pcAppName, ios::in|ios::binary|ios::nocreate|ios::ate);
	if (poFile->bad())
		return false;
	poFile->seekg(m_uWJEPos, ios::beg);
	if (poFile->bad()) {
		poFile->close();
		return NULL;
	}
	return poFile;
} // GetData

unsigned int CSFX::FindPos(fstream &oFile) {
	ASSERT((m_pcMagic[3] == 0x00) && (m_uWJEPos == 0xFFFFFFFF));
	// Seek to the beginning
	oFile.seekg(0, ios::beg);
	if (oFile.bad())
		return 0;
	// Params
	unsigned int uPos = 0;
	unsigned char c = 0x00;
	int iCount = 0;
	bool bDone = false;
	// Loop through the file until pos found or end reached
	while (!bDone) {
		c = oFile.get();
		// Look for the first char in the start ID
		if (c == m_pcStartID[0]) {
			// Save the next pos
			uPos = oFile.tellg();
			// Set the found marker
			bool bFound = true;
			// If found, check it against the rest of the string
			iCount = 0;
			while (bFound && (iCount < SFX_START_SIZE - 1)) {
				iCount++;
				c = oFile.get();
				if (c != m_pcStartID[iCount])
					bFound = false;
			}
			// If the string matches, check the magic number
			if (bFound) {
				for (iCount = 0; iCount < 4; iCount++) {
					c = oFile.get();
					if (c != m_pcMagic[iCount])
						bFound = false;
				}
				// If the magic checks out, save the pos and end the search
				if (bFound) {
					uPos = oFile.tellg();
					bDone = true;
				}
			}
			// Otherwise, seek back to the next char and carry on
			else {
				oFile.seekg(uPos, ios::beg);
				uPos = 0;
			}
		}
		// Stop if end of file reached
		if (oFile.eof()) 
			bDone = true;
	}
	return uPos;
} // FindPos
