//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.cpp - 02/07/2000 - Warren Moore
//	Creation and reading of compressed Wedgie files
//
// $Id: Wedgie.cpp,v 1.5 2000/07/11 14:20:02 waz Exp $
//

#include "StdAfx.h"

#include "Wedgie.h"

#include <string.h>
#include "CompressDeflate.h"
#include "ProgressControl.h"
#include <direct.h>
#include <errno.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgressControl g_oProgressControl;

////////////
// CWedgie

CWedgie::CWedgie() : m_cVerHigh(WJE_VER_HIGH), m_cVerLow(WJE_VER_LOW) {
	// TODO: Remove this hack and add proper handle for different pos sizes
	ASSERT(WJE_POS_SIZE == 4);
	ASSERT(sizeof(unsigned int) == 4);
	m_pcBaseDir = NULL;
	m_iBaseLength = 0;
	m_poFile = NULL;
	m_bCreate = false;
	m_uFiles = 0;
	m_psTable = NULL;
	m_uCurrentVer = 0;
} // Constructor

CWedgie::~CWedgie() {
	// Close the wedgie if one open
	if (m_poFile)
		Close();
} // Destructor

WJERESULT CWedgie::Open(fstream *poFile, const char *pcDir, bool bCreate, bool bMove) {
	// If a wedgie is open, close it ignoring the return
	if (m_poFile)
		Close();

	// Check a file stream was given
	if (!poFile)
		return WJE_NO_FILE;

	//#===--- Set the wedgie vars
	// Set the base directory
	if (pcDir && (strlen(pcDir) > 0)) {
		// Get the string length and add trailing slash
		int iLength = strlen(pcDir) + 1;
		bool bTrail = true;			// trailing slash present
		bool bBase = false;			// base directory required
		if (pcDir[iLength - 2] != '\\') {
			iLength++;
			bTrail = false;
		}
		// If not a specific dir, add the base dir
		const char *pcAppDir = g_poVAL->GetAppDir();
		if (pcAppDir && (iLength > 2) && (pcDir[1] != ':') && (pcDir[0] != '\\')) {
			iLength += strlen(pcAppDir);
			bBase = true;
		}
		// Allocate the string
		NEWBEGIN
		m_pcBaseDir = (char*) new char[iLength];
		NEWEND("CWedgie::Open - Base directory name")
		if (m_pcBaseDir) {
			// Terminate the fresh string
			m_pcBaseDir[0] = '\0';
			// If needed, add the base dir
			if (bBase)
				strcpy(m_pcBaseDir, pcAppDir);
			// Copy in the supplied directory
			strcat(m_pcBaseDir, pcDir);
			// If needed, add the trailing slash
			if (!bTrail)
				strcat(m_pcBaseDir, "\\");
		}
		else {
			Close();
			return WJE_OUT_OF_MEMORY;
		}
		// Set the directory length
		m_iBaseLength = strlen(m_pcBaseDir);
	}
	else {
		NEWBEGIN
		m_pcBaseDir = (char*) new char[1];
		NEWEND("CWedgie::Open - Base directory name")
		if (m_pcBaseDir)
			m_pcBaseDir[0] = '\0';
		else {
			Close();
			return WJE_OUT_OF_MEMORY;
		}
	}

	// Everythin ok, so set the file stream as this indicates everything well
	m_poFile = poFile;
	m_bCreate = bCreate;

	// Either create the wedgie from the base dir, or read the wedgie file contents
	WJERESULT eResult = WJE_OK;
	if (m_bCreate) {
		eResult = Write(bMove);
	}
	else {
		eResult = Read();
	}
	if (eResult != WJE_OK)
		Close();

	return eResult;
} // Open

WJERESULT CWedgie::Close() {
	// Delete the data structures and reset the vars
	if (m_pcBaseDir)
		delete [] m_pcBaseDir;
	if (m_psTable) {
		ASSERT(m_uFiles);
		for (unsigned int i = 0; i < m_uFiles; i++) {
			if (m_psTable[i].m_pcName)
				delete m_psTable[i].m_pcName;
		}
		delete [] m_psTable;
	}
	m_pcBaseDir = NULL;
	m_iBaseLength = 0;
	m_poFile = NULL;
	m_bCreate = false;
	m_uFiles = 0;
	m_psTable = NULL;
	m_uCurrentVer = 0;

	return WJE_OK;
} // Close

const char *CWedgie::Directory() const {
	return m_pcBaseDir;
} // Directory

unsigned int CWedgie::Files() const {
	return m_uFiles;
} // Files

const char *CWedgie::Filename(unsigned int uEntry) {
	return (uEntry < m_uFiles) ? m_psTable[uEntry].m_pcName : NULL;
} // Filename

WJERESULT CWedgie::Extract(unsigned int uEntry, const char *pcFilename) {
	ASSERT(m_poFile && (!m_bCreate));
	// Check it's a valid entry
	if (uEntry >= m_uFiles)
		return WJE_INVALID_ENTRY;

	// Create the base directory name
	sFileData &sEntry = m_psTable[uEntry];
	char pcName[STR_SIZE] = "";
	strcpy(pcName, m_pcBaseDir);
	// Alternative name supplied
	if (pcFilename) {
		// Specific dir supplied
		if ((strlen(pcFilename) > 0) && (pcFilename[0] == '\\'))
			strcpy(pcName, pcFilename);
		else 
			// Specific drive and dir supplied
			if ((strlen(pcFilename) > 1) && (pcFilename[1] == ':'))
				strcpy(pcName, pcFilename);
			else
				// Relative name supplied
				strcat(pcName, pcFilename);
	}
	else
		// No alternative name supplied
		strcat(pcName, sEntry.m_pcName);
	// Seek to the beginning of the file
	m_poFile->seekg(m_uStartPos, ios::beg);
	m_poFile->seekg(sEntry.m_uOffset, ios::cur);
	if (m_poFile->bad())
		return WJE_FILE_READ_ERROR;
	// Create the directories, necessary
	char pcDir[STR_SIZE];
	int iLength = strlen(pcName);
	int iCount = 0;
	if ((iLength > 0) && (pcName[0] == '\\'))
		iCount = 1;
	else
		if ((iLength > 2) && (pcName[1] == ':'))
			iCount = 3;
	bool bMore = true;
	while (bMore) {
		// Find the next directory
		while ((pcName[iCount] != '\\') && (pcName[iCount] != '\0'))
			iCount++;
		// If a dir is found, copy it out then create it
		if (pcName[iCount] == '\\') {
			// Copy the directory name out
			memcpy(pcDir, pcName, iCount);
			pcDir[iCount] = '\0';
			// Create the directory
			int iResult = _mkdir(pcDir);
			if ((iResult == -1) && (errno != EEXIST)) {
				return WJE_DIR_ERROR;
			}
			// Set the counter to look for the next dir
			iCount++;
		}
		// otherwise stop
		else
			bMore = false;
	}
	// Select the specific extraction function
	switch (m_uCurrentVer) {
		case 0x0100:
			return Extract1_0(uEntry, pcName);
			break;
		default:
			return WJE_UNSUPPORTED_VERSION;
	}
} // Extract

WJERESULT CWedgie::Extract1_0(unsigned int uEntry, const char *pcFilename) {

	ASSERT(pcFilename);
	// Open the file
	ofstream oFile;
	oFile.open(pcFilename, ios::out|ios::binary|ios::trunc);
	if (oFile.bad())
		return WJE_FILE_CREATE_ERROR;

	// Open the decompressor
	CCompressDeflate oDeflate;
	oDeflate.WriteToFile(true, &oFile);
	oDeflate.Init(false);

	// Decompress the file
	sFileData &sEntry = m_psTable[uEntry];
	unsigned char pcBuffer[WJE_BLOCK_SIZE];
	unsigned int uCount = 0;
	unsigned int uBlock = 0;
	while (uCount < sEntry.m_uCompSize) {
		// Read the data
		uBlock = (sEntry.m_uCompSize - uCount > WJE_BLOCK_SIZE) ? 
			WJE_BLOCK_SIZE : sEntry.m_uCompSize - uCount;
		m_poFile->read(pcBuffer, uBlock);
		int iRead = m_poFile->gcount();
		uCount += iRead;
		// Pass the data to the compressor
		oDeflate.Input(pcBuffer, iRead);
		// Check all is well
		if (m_poFile->bad())
			uCount = sEntry.m_uCompSize;
	}

	// Close the file and tidy up
	oDeflate.End();
	oFile.close();

	return WJE_OK;
} // Extract1_0

WJERESULT CWedgie::Write(bool bMove) {
	ASSERT(m_poFile);
	ASSERT(m_bCreate);
	// Count the number of files within the base dir
	m_uFiles = Count(m_pcBaseDir);
	if (m_uFiles == 0)
		return WJE_NO_DATA;

	// Allocate the file table
	NEWBEGIN
	m_psTable = (sFileData*)new sFileData[m_uFiles];
	NEWEND("CWedgie::Write - File table")
	if (!m_psTable)
		return WJE_OUT_OF_MEMORY;

	// Generate file list
	m_uTotalSize = 0;
	int iNext = 0;
	WJERESULT eResult = WJE_OK;
	eResult = FillTable(m_pcBaseDir, iNext);

	// Write the header
	if (eResult == WJE_OK)
		eResult = StartHeader();

	// Process the files
	if (eResult == WJE_OK)
		eResult = ProcessFiles();

	// Update the header
	if (eResult == WJE_OK)
		eResult = EndHeader();

	// If specified, remove the files and directories
	if ((eResult == WJE_OK) && bMove)
		eResult = RemoveFiles(m_pcBaseDir);

	// Tidy up if an error occured
	if (eResult != WJE_OK)
		Close();

	return eResult;
} // Write

WJERESULT CWedgie::Read() {
	ASSERT(m_poFile);
	ASSERT(!m_bCreate);
	// Save the beginning of the stream
	if (m_poFile->bad())
		return WJE_FILE_READ_ERROR;
	m_uStartPos = m_poFile->tellg();
	// Validate the file ID
	char pcID[sizeof(WJE_ID)];
	pcID[WJE_ID_SIZE] = '\0';
	m_poFile->read(pcID, WJE_ID_SIZE);
	if (strcmp(WJE_ID, pcID) != 0)
		return WJE_INVALID_ID;
	// Get the version
	m_uCurrentVer = 0;
	m_poFile->read((char*)&m_uCurrentVer + 1, 1);
	m_poFile->read((char*)&m_uCurrentVer    , 1);
	// Get the preamble size
	m_poFile->read((char*)&m_uPreambleSize, WJE_PRE_SIZE);
	if (m_uPreambleSize == 0)
		return WJE_NO_DATA;
	// Select the correct version reader
	switch (m_uCurrentVer) {
		case 0x0100:
			return Read1_0();
		default:
			return WJE_UNSUPPORTED_VERSION;
	}
	return WJE_OK;
} // Read

WJERESULT CWedgie::Read1_0() {
	ASSERT(m_uPreambleSize);
	// Read the number of files
	m_poFile->read((char*)&m_uFiles, WJE_POS_SIZE);
	if (m_uFiles == 0)
		return WJE_NO_DATA;
	// Read the total amount of data
	m_poFile->read((char*)&m_uTotalSize, WJE_POS_SIZE);
	// Read the toc size
	m_poFile->read((char*)&m_uTOCSize, WJE_POS_SIZE);
	// Allocate the file data table
	NEWBEGIN
	m_psTable = (sFileData*)new sFileData[m_uFiles];
	NEWEND("CWedgie::Read1_0 - File table")
	if (!m_psTable)
		return WJE_OUT_OF_MEMORY;
	// Read the toc data
	unsigned int uCount = 0;
	char pcName[STR_SIZE] = "";
	while (uCount < m_uFiles) {
		// Read the filename
		m_poFile->getline(pcName, STR_SIZE, '\0');
		if (strlen(pcName) > 0) {
			NEWBEGIN
			m_psTable[uCount].m_pcName = (char*) new char[m_poFile->gcount()];
			NEWEND("CWedgie::Read1_0 - File table entry name string")
			if (!m_psTable[uCount].m_pcName)
				return WJE_OUT_OF_MEMORY;
			strcpy(m_psTable[uCount].m_pcName, pcName);
		}
		// Read the offset
		m_poFile->read((char*)&m_psTable[uCount].m_uOffset, WJE_POS_SIZE);
		// Read the original size
		m_poFile->read((char*)&m_psTable[uCount].m_uOrigSize, WJE_POS_SIZE);
		// Read the compressed size
		m_poFile->read((char*)&m_psTable[uCount].m_uCompSize, WJE_POS_SIZE);
		// Check we're ok
		if (m_poFile->bad())
			return WJE_FILE_READ_ERROR;
		uCount++;
	}
	return WJE_OK;
} // Read1_0

unsigned int CWedgie::Count(const char *pcDir) {
	unsigned int uCount = 0;
	CString strSearch;
	strSearch.Format("%s*.*", pcDir);
	CFileFind oFind;
	bool bFound = oFind.FindFile((LPCSTR)strSearch) == TRUE;
	while (bFound) {
		bFound = oFind.FindNextFile() == TRUE;
		if (!oFind.IsDots()) {
			if (oFind.IsDirectory()) {
				// Process subdirectories
				strSearch.Format("%s%s\\", pcDir, oFind.GetFileName());
				uCount += Count(strSearch);
			}
			else {
				// Count another file
				uCount++;
			}
		}
	}
	return uCount;
} // Count

WJERESULT CWedgie::FillTable(const char *pcDir, int &iIndex) {
	// Check the file is being created
	if (!m_bCreate)
		return WJE_WRONG_MODE;

	// Check we have data to compress
	if (m_uFiles == 0)
		return WJE_NO_DATA;

	// Begin obtaining file info
	WJERESULT eResult = WJE_OK;
	CString strSearch;
	strSearch.Format("%s*.*", pcDir);
	CFileFind oFind;
	bool bFound = oFind.FindFile((LPCSTR)strSearch) == TRUE;
	while (bFound) {
		bFound = oFind.FindNextFile() == TRUE;
		if (!oFind.IsDots()) {
			if (oFind.IsDirectory()) {
				// Process subdirectories
				strSearch.Format("%s%s\\", pcDir, oFind.GetFileName());
				eResult = FillTable(strSearch, iIndex);
			}
			else {
				// Process another file
				if (iIndex >= m_uFiles) {
					eResult = WJE_CREATE_ERROR;
					bFound = false;
				}
				// Get the file name
				CString strName = oFind.GetFilePath();
				// File size
				ifstream oFile(strName, ios::in|ios::binary|ios::ate|ios::nocreate);
				if (!oFile.bad()) {
					m_psTable[iIndex].m_uOrigSize = oFile.tellg();
					m_uTotalSize += m_psTable[iIndex].m_uOrigSize;
					oFile.close();
					// Add name to list
					strName = strName.Right(strName.GetLength() - m_iBaseLength);
					int iLength = strName.GetLength();
					NEWBEGIN
					m_psTable[iIndex].m_pcName = (char*)new char[iLength + 1];
					NEWEND("CWedgie::FillTable - File list entry")
					if (m_psTable[iIndex].m_pcName) {
						strcpy(m_psTable[iIndex].m_pcName, strName);
					}
					else {
						eResult = WJE_OUT_OF_MEMORY;
						bFound = false;
					}
					// Inc next pointer
					iIndex++;
				}
				else {
					eResult = WJE_FILE_OPEN_ERROR;
					bFound = false;
				}
			}
		}
	}
	return eResult;
} // FillTable

WJERESULT CWedgie::StartHeader() {
	// Check the file is being created
	if (!m_bCreate)
		return WJE_WRONG_MODE;

	// Check we have data to compress
	if (m_uFiles == 0)
		return WJE_NO_DATA;

	// Set initial vars
	int i = 0;
	m_uStartPos = 0;
	m_uOffsetPos = 0;
	m_uPreambleSize = 0;
	m_uTOCSize = 0;
	// Save the beginning of the stream
	if (m_poFile->bad()) 
		return WJE_FILE_WRITE_ERROR;
	m_uStartPos = m_poFile->tellp();
	// Write the file identifier
	m_poFile->write(WJE_ID, WJE_ID_SIZE);
	// Write the version
	m_poFile->write(&m_cVerHigh, 1);
	m_poFile->write(&m_cVerLow, 1);
	// Save space for the preamble size
	for (i = 0; i < WJE_PRE_SIZE; i++)
		m_poFile->put('\0');
	// Write the number of files
	m_poFile->write((char*)&m_uFiles, WJE_POS_SIZE);
	// Save space for the total amount of data
	for (i = 0; i < WJE_POS_SIZE; i++)
		m_poFile->put('\0');
	// Calculate the preamble size
	if (m_poFile->bad()) 
		return WJE_FILE_WRITE_ERROR;
	m_uPreambleSize = m_poFile->tellp() - m_uStartPos - WJE_START_SIZE - WJE_PRE_SIZE;
	// Save space for the toc size
	for (i = 0; i < WJE_POS_SIZE; i++)
		m_poFile->put('\0');
	// Write the file data
	for (unsigned int uCount = 0; uCount< m_uFiles; uCount++) {
		// Filename
		m_poFile->write(m_psTable[uCount].m_pcName, strlen(m_psTable[uCount].m_pcName) + 1);
		// Stream offset
		for (i = 0; i < WJE_POS_SIZE; i++)
			m_poFile->put('\0');
		// Original size
		m_poFile->write((char*)&(m_psTable[uCount].m_uOrigSize), WJE_POS_SIZE);
		// Compressed size
		for (i = 0; i < WJE_POS_SIZE; i++)
			m_poFile->put('\0');
		// Error checking
		if (m_poFile->bad()) 
			return WJE_FILE_WRITE_ERROR;
	}
	// Calculate the toc size
	m_uOffsetPos = m_poFile->tellp() - m_uStartPos;
	m_uTOCSize = m_uOffsetPos - m_uPreambleSize - WJE_START_SIZE - WJE_PRE_SIZE - WJE_POS_SIZE;

	return WJE_OK;
} // StartHeader

WJERESULT CWedgie::ProcessFiles() {
	WJERESULT eResult = WJE_OK;
	unsigned char pcBuffer[WJE_BLOCK_SIZE];

// Process each file in time
	unsigned int uSize = 0;
	unsigned int uTotal = 0;
	g_oProgressControl.SetMaxProgress(WJE_TOTAL, m_uTotalSize);
	for (unsigned int uCount = 0; uCount < m_uFiles; uCount++) {
		// Set the dialog info
		uSize = 0;
		sFileData &sEntry = m_psTable[uCount];
		g_oProgressControl.SetText(WJE_FILE, sEntry.m_pcName); 
		g_oProgressControl.SetProgress(WJE_FILE, uSize);
		g_oProgressControl.SetMaxProgress(WJE_FILE, sEntry.m_uOrigSize);
		// Set the file offset
		sEntry.m_uOffset = m_uOffsetPos;
		// Create the path name
		CString strPath;
		strPath.Format("%s%s", m_pcBaseDir, sEntry.m_pcName);
		// Open the file
		ifstream oFile;
		oFile.open(strPath, ios::in|ios::binary|ios::nocreate);
		if (oFile.good()) {
		// Set the compressor
			CCompressDeflate oDeflate;
			oDeflate.WriteToFile(true, (ofstream*)m_poFile);
			oDeflate.Init(true);
			unsigned int uBlock = 0;
			while (uSize < sEntry.m_uOrigSize) {
				// Read the data
				uBlock = (sEntry.m_uOrigSize - uSize > WJE_BLOCK_SIZE) ? 
					WJE_BLOCK_SIZE : sEntry.m_uOrigSize - uSize;
				// Read the data
				oFile.read(pcBuffer, uBlock);
				int iRead = oFile.gcount();
				// Pass the data to the compressor
				oDeflate.Input(pcBuffer, iRead);

				uSize += iRead;
				uTotal += iRead;
				g_oProgressControl.SetProgress(WJE_FILE, uSize);
				g_oProgressControl.SetProgress(WJE_TOTAL, uTotal);
			}
		// Finish it all off
			oDeflate.End();
			oFile.close();
		}
		else {
			eResult = WJE_FILE_READ_ERROR;
			uCount = m_uFiles;
		}
	// Calculate the new offset
		unsigned int uOffset = m_poFile->tellp() - m_uStartPos;
		sEntry.m_uCompSize = uOffset - m_uOffsetPos;
		m_uOffsetPos = uOffset;
	}
	return eResult;
} // ProcessHeader

WJERESULT CWedgie::EndHeader() {
	// Seek to the preamble size
	m_poFile->seekp(m_uStartPos + WJE_START_SIZE, ios::beg);
	// Write the new preamble size
	m_poFile->write((char*)&m_uPreambleSize, WJE_PRE_SIZE);
	if (m_poFile->bad())
		return WJE_FILE_WRITE_ERROR;
	// Skip the number of files
	m_poFile->seekp(WJE_POS_SIZE, ios::cur);
	// Write the total data size
	m_poFile->write((char*)&m_uTotalSize, WJE_POS_SIZE);
	// Write the toc size
	m_poFile->write((char*)&m_uTOCSize, WJE_POS_SIZE);
	if (m_poFile->bad())
		return WJE_FILE_WRITE_ERROR;
	// Write the updated toc file data
	unsigned int uCount = 0;
	while (uCount < m_uFiles) {
		m_poFile->seekp(strlen(m_psTable[uCount].m_pcName) + 1, ios::cur);
		m_poFile->write((char*)&(m_psTable[uCount].m_uOffset), WJE_POS_SIZE);
		m_poFile->seekp(WJE_POS_SIZE, ios::cur);
		m_poFile->write((char*)&(m_psTable[uCount].m_uCompSize), WJE_POS_SIZE);
		if (m_poFile->bad())
			return WJE_FILE_WRITE_ERROR;
		uCount++;
	}
	return WJE_OK;
} // EndHeader

WJERESULT CWedgie::RemoveFiles(const char *pcDir) {
	WJERESULT eResult = WJE_OK;
	CString strSearch;
	strSearch.Format("%s*.*", pcDir);
	CFileFind oFind;
	bool bFound = oFind.FindFile((LPCSTR)strSearch) == TRUE;
	while (bFound) {
		bFound = oFind.FindNextFile() == TRUE;
		if (!oFind.IsDots()) {
			if (oFind.IsDirectory()) {
				// Process subdirectories
				strSearch.Format("%s%s\\", pcDir, oFind.GetFileName());
				eResult = RemoveFiles(strSearch);
				if (eResult != WJE_OK)
					// Fail on error
					bFound = false;
			}
			else {
				// Delete the file
				int iResult = remove(oFind.GetFilePath());
				if ((iResult == -1) && (errno != EACCES) && (errno != ENOENT)) {
					eResult = WJE_CANNOT_MOVE;
					bFound = false;
				}
			}
		}
	}
	int iResult = _rmdir(pcDir);
	if ((iResult == -1) && (errno != ENOTEMPTY) && (errno != ENOENT)) {
		eResult = WJE_CANNOT_MOVE;
		bFound = false;
	}
	// Delete the initial directory
	return eResult;
} // RemoveFiles