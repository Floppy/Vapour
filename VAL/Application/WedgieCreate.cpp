///////////////////////////////
// Wedgie - WJE file compiler
//
// Copyright 2000 Vapour Technology
// 12/04/2000 - Warren Moore
// 
// TOC.cpp
//

#include "stdafx.h"

#include "WedgieCreate.h"

#include <string.h>
#include "CompressDeflate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// CWedgieCreate

CWedgieCreate::CWedgieCreate() : m_cVerHigh(WEDGIE_VER_HIGH), m_cVerLow(WEDGIE_VER_LOW) {
	m_strBase = "";
	m_strName = "";
	m_iBaseLength = 0;
	m_iFiles = 0;
	m_iTotalSize = 0;
	m_iNext = 0;
	m_pFile = NULL;
	m_iOffset = 0;
} // Constructor

CWedgieCreate::~CWedgieCreate() {
	DeleteFileList();
} // Destructor

void CWedgieCreate::SetBaseName(const char *pBase) {
	int iCount = strlen(pBase);
	if (iCount) {
		while (pBase[--iCount] != '\\') {}
		m_strBase = pBase;
		m_strName = m_strBase.Right(strlen(pBase) - iCount - 1);
		m_strBase = m_strBase.Left(iCount) + "\\";
		m_iBaseLength = strlen(pBase) + 1;
		CString strDir;
		strDir.Format("%s%s\\", m_strBase, m_strName);
		m_iFiles = Count(strDir);
	}
} // SetBaseName

CString CWedgieCreate::GetName() {
	return m_strName;
} // GetName

int CWedgieCreate::GetFiles() {
	return m_iFiles;
} // GetFiles

int CWedgieCreate::Count(CString strDir) {
	int iCount = 0;
	CString strSearch = strDir;
	strSearch += "*.*";
	CFileFind oFind;
	bool bFound = oFind.FindFile((LPCSTR)strSearch) == TRUE;
	while (bFound) {
		bFound = oFind.FindNextFile() == TRUE;
		if (!oFind.IsDots()) {
			if (oFind.IsDirectory()) {
				strSearch = strDir + oFind.GetFileName() + "\\";
				iCount += Count(strSearch);
			}
			else {
				iCount++;
			}
		}
	}
	return iCount;
} // Count

FRESULT CWedgieCreate::Generate(CProgress &oDlg) {
	ASSERT(m_iFiles > 0);
// Allocate TOC
	FRESULT eResult = F_OK;
	NEWBEGIN
	m_pFile = (sFileData*)new sFileData[m_iFiles];
	NEWEND("CWedgieCreate::Generate - File list")
// Generate file list
	if (m_pFile) {
		m_iTotalSize = 0;
		m_iNext = 0;
		CString strDir;
		strDir.Format("%s%s\\", m_strBase, m_strName);
		eResult = GenerateTOC(strDir);
	}
	else {
		eResult = F_OUTOFMEMORY;
	}

// Open the wedgie
	ofstream oFile;
	if (eResult == F_OK) {
		CString strName;
		strName.Format("%s%s.wje", m_strBase, m_strName);
		oFile.open(strName, ios::out|ios::binary|ios::trunc);
		if (oFile.bad())
			eResult = F_FILEERROR;
	}

// Write the header
	if (eResult == F_OK) {
		eResult = WriteHeader(oFile);
	}

// Process the files
	if (eResult == F_OK) {
		eResult = ProcessFiles(oFile, oDlg);
	}

// Update the header
	if (eResult == F_OK) {
		eResult = UpdateHeader(oFile);
	}

// Close the wedgie
	oFile.close();

	return eResult;
} //Generate

FRESULT CWedgieCreate::GenerateTOC(CString strDir) {
	FRESULT eResult = F_OK;
	CString strSearch = strDir;
	strSearch += "*.*";
	CFileFind oFind;
	bool bFound = oFind.FindFile((LPCSTR)strSearch) == TRUE;
	while (bFound && (eResult == F_OK)) {
		bFound = oFind.FindNextFile() == TRUE;
		if (!oFind.IsDots()) {
			if (oFind.IsDirectory()) {
				strSearch = strDir + oFind.GetFileName() + "\\";
				eResult = GenerateTOC(strSearch);
			}
			else {
				ASSERT(m_iNext < m_iFiles);
			// Get the file data
				CString strName = oFind.GetFilePath();
			// File size
				ifstream oFile(strName, ios::in|ios::binary|ios::ate|ios::nocreate);
				if (oFile.good()) {
					m_pFile[m_iNext].m_iOrigSize = oFile.tellg();
					m_iTotalSize += m_pFile[m_iNext].m_iOrigSize;
					oFile.close();
				// Add name to list
					strName = strName.Right(strName.GetLength() - m_iBaseLength);
					int iLength = strName.GetLength();
					NEWBEGIN
					m_pFile[m_iNext].m_pName = (char*)new char[iLength + 1];
					NEWEND("CWedgieCreate::GenerateTOC - File list entry")
					if (m_pFile[m_iNext].m_pName) {
						strcpy(m_pFile[m_iNext].m_pName, strName);
					}
					else {
						eResult = F_OUTOFMEMORY;
					}
				// Inc next pointer
					m_iNext++;
				}
				else {
					eResult = F_FILEERROR;
				}
			}
		}
	}
	return F_OK;
} // GenerateTOC

void CWedgieCreate::DeleteFileList() {
	if (m_pFile) {
		ASSERT(m_iFiles);
		for (int i = 0; i < m_iFiles; i++) {
			if (m_pFile[i].m_pName)
				delete [] m_pFile[i].m_pName;
		}
		delete [] m_pFile;
	}
} // DeleteFileList

FRESULT CWedgieCreate::WriteHeader(ofstream &oFile) {
	FRESULT eResult = F_OK;

// Write the file identifier
	oFile.write("VAWEDGIE", 8);
// Write the version
	oFile.write(&m_cVerHigh, 1);
	oFile.write(&m_cVerLow, 1);
// Write the number of files
	oFile.write((char*)&m_iFiles, 4);

//#===--- Don't forget to increase HEADER_SIZE if anything is added

	m_iOffset = HEADER_SIZE;
// Write the file data
	for (int i = 0; i < m_iFiles; i++) {
		oFile.write(m_pFile[i].m_pName, strlen(m_pFile[i].m_pName) + 1);
		oFile.write("\0\0\0\0", 4);
		oFile.write((char*)&(m_pFile[i].m_iOrigSize), 4);
		oFile.write("\0\0\0\0", 4);
		m_iOffset += strlen(m_pFile[i].m_pName) + 13;
	}

	return eResult;
} // WriteHeader

FRESULT CWedgieCreate::ProcessFiles(ofstream &oFile, CProgress &oDlg) {
	FRESULT eResult = F_OK;
	unsigned char pBuffer[READ_BLOCK_SIZE];

// Process each file in time
	unsigned int iPos = 0;
	unsigned long iTotal = 0;
	oDlg.SetTotalSize(m_iTotalSize);
	for (int i = 0; i < m_iFiles; i++) {
	// Set the dialog info
		iPos = 0;
		oDlg.SetFileName(m_pFile[i].m_pName);
		oDlg.SetFileSize(m_pFile[i].m_iOrigSize);
		oDlg.SetFilePos(iPos);
	// Set the file offset
		m_pFile[i].m_iOffset = m_iOffset;
	// Create the path name
		CString strPath;
		strPath.Format("%s%s\\%s", m_strBase, m_strName, m_pFile[i].m_pName);
		TRACE("%s\n", strPath);
	// Open the file
		ifstream oInFile;
		oInFile.open(strPath, ios::in|ios::binary|ios::nocreate);
		if (oInFile.good()) {
		// Set the compressor
			CCompressDeflate oDeflate;
			oDeflate.WriteToFile(true, &oFile);
			oDeflate.Init(true);
			while (!oInFile.eof()) {
			// Read the data
				oInFile.read(pBuffer, READ_BLOCK_SIZE);
				int iRead = oInFile.gcount();
			// Pass the data to the compressor
				oDeflate.Input(pBuffer, iRead);
				iPos += iRead;
				iTotal += iRead;
				oDlg.SetFilePos(iPos);
				oDlg.SetTotalPos(iTotal);
			}
		// Finish it all off
			oDeflate.End();
			oInFile.close();
		}
		else {
			eResult = F_FILEERROR;
			i = m_iFiles;
		}
	// Caculate the new offset
		unsigned int iOffset = oFile.tellp();
		m_pFile[i].m_iCompSize = iOffset - m_iOffset;
		m_iOffset = iOffset;
	}
	return eResult;
} // ProcessFiles

FRESULT CWedgieCreate::UpdateHeader(ofstream &oFile) {
	FRESULT eResult = F_OK;

// Seek to the beginning of the file
	oFile.seekp(HEADER_SIZE, ios::beg);
	int iCount = 0;
	while (iCount < m_iFiles) {
		oFile.seekp(strlen(m_pFile[iCount].m_pName) + 1, ios::cur);
		oFile.write((char*)&(m_pFile[iCount].m_iOffset), 4);
		oFile.seekp(4, ios::cur);
		oFile.write((char*)&(m_pFile[iCount].m_iCompSize), 4);
		iCount++;
	}

	return eResult;
} // UpdateHeader

