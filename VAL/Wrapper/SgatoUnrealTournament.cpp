//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToUnrealTournament.cpp - 06/07/2000 - Warren Moore
//	SGA Avatar to Unreal Tournament converter wrapper 
//
// $Id: SgatoUnrealTournament.cpp,v 1.1 2000/07/15 16:48:09 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "SGAToHalfLife.h"

#include <iostream.h>
#include "AvatarFileAME.h"
#include "AvatarFileUnrealTournament.h"
#include "Avatar.h"
#include "TimeLimit.h"
#include "Wedgie.h"

///////////////////////////
// CSGAToUnrealTournament

const char *CSGAToUnrealTournament::m_pcError[] = {
	WRAP_ERROR_STRINGS
};

CSGAToUnrealTournament::CSGAToUnrealTournament() {
// Set vars
	m_eResult = VA_OK;
	m_pcSGAFilename = NULL;
	m_pcUTModelname = NULL;
	m_pcUTPath = NULL;
	m_bVerbose = true;
} // Contructor

CSGAToUnrealTournament::~CSGAToUnrealTournament() {
	if (m_pcSGAFilename)
		delete [] m_pcSGAFilename;
	if (m_pcUTModelname)
		delete [] m_pcUTModelname;
	if (m_pcUTPath)
		delete [] m_pcUTPath;
} // Destructor

VARESULT CSGAToUnrealTournament::SetOption(int iOption, int iArgument) {
	m_eResult = VA_OK;
	switch (iOption) {
		case UT_VERBOSE:
			if ((iArgument == UT_TRUE) || (iArgument == UT_FALSE))
				m_bVerbose = iArgument == UT_TRUE;
			else
				m_eResult = VA_INVALID_ARGUMENT;
		default:
			m_eResult = VA_INVALID_OPTION;
	}

	return m_eResult;
} // SetOption

VARESULT CSGAToUnrealTournament::SetOption(int iOption, const char *pcString) {
	m_eResult = VA_OK;
	int iLength = 0;
	char *pcBuffer = NULL;
	switch (iOption) {
		case SGA_FILENAME:
			if (pcString) {
				iLength = strlen(pcString) + 1;
				pcBuffer = (char*) new char[iLength];
				if (pcBuffer) {
					strcpy(pcBuffer, pcString);
					if (m_pcSGAFilename)
						delete [] m_pcSGAFilename;
					m_pcSGAFilename = pcBuffer;
				}
				else
					m_eResult = VA_OUT_OF_MEMORY;
			}
			else 
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case UT_MODELNAME:
			if (pcString) {
				iLength = strlen(pcString) + 1;
				pcBuffer = (char*) new char[iLength];
				if (pcBuffer) {
					strcpy(pcBuffer, pcString);
					if (m_pcUTModelname)
						delete [] m_pcUTModelname;
					m_pcUTModelname = pcBuffer;
				}
				else
					m_eResult = VA_OUT_OF_MEMORY;
			}
			else 
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case UT_DIRECTORY:
			if (pcString) {
				iLength = strlen(pcString) + 1;
				bool bAdd = false;
				if (pcString[iLength - 1] != '\\') {
					iLength++;
					bAdd = true;
				}
				pcBuffer = (char*) new char[iLength];
				if (pcBuffer) {
					strcpy(pcBuffer, pcString);
					if (bAdd)
						strcat(pcBuffer, "\\");
					if (m_pcUTPath)
						delete [] m_pcUTPath;
					m_pcUTPath = pcBuffer;
				}
				else
					m_eResult = VA_OUT_OF_MEMORY;
			}
			else 
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		default:
			m_eResult = VA_INVALID_OPTION;
	}

	return m_eResult;
} // SetOption

const int CSGAToUnrealTournament::GetOptionInt(int iOption) {
	int iReturn = UT_UNKNOWN;
	switch (iOption) {
		case UT_VERBOSE:
			iReturn = m_bVerbose ? UT_TRUE : UT_FALSE;
		default:
			m_eResult = VA_INVALID_OPTION;
	}
	return iReturn;
} // GetOptionInt

const char *CSGAToUnrealTournament::GetOptionString(int iOption) {
	char *pcReturn = NULL;
	switch (iOption) {
		case SGA_FILENAME:
			pcReturn = m_pcSGAFilename;
			break;
		case UT_MODELNAME:
			pcReturn = m_pcUTModelname;
			break;
		case UT_DIRECTORY:
			pcReturn = m_pcUTPath;
			break;
		default:
			m_eResult = VA_INVALID_OPTION;
	}
	return pcReturn;
} // GetOptionString

VARESULT CSGAToUnrealTournament::Export() {
// Show star-up text
	if (m_bVerbose) {
		cout << "SGA Avatar To Half Life model converter v1.0 (11/07/2000)" << endl;
		cout << "Copyright 2000 Vapour Technology Ltd." << endl << endl;
		cout << "Exclusively licensed to AvatarMe Ltd." << endl << endl;
	}
// Check for time limit validation
	cout << "Time limited evaluation version" << endl << endl;
	CTimeLimit oTimeLimit;
	if (!oTimeLimit.Valid()) {
		m_eResult = VA_TIME_EXPIRED;
		return m_eResult;
	}
// Check for correct options
	m_eResult = VA_OK;
	bool bOk = m_pcSGAFilename && m_pcUTModelname;
	if (!bOk)
		m_eResult = VA_MISSING_FILENAME;
// Start model export
	if (bOk) {
		if (m_bVerbose) {
		// Show startup text
			cout << "SGA Avatar      : " << m_pcSGAFilename << endl;
			cout << "Half Life Model : ";
			if (m_pcUTPath)
				cout << m_pcUTPath;
			cout << m_pcUTModelname << endl;
			cout << endl;
			cout << "Starting model export..." << endl;
		}

	// Load the SGA avatar
		CAvatarFileAME oAME;
		CAvatar *poAvatar = oAME.Load(m_pcSGAFilename);
	// Check the avatar
		if (!poAvatar) {
			m_eResult = VA_AVATAR_LOAD_ERROR;
			return m_eResult;
		}

	// Allocate the complete filename
		int iLength = strlen(m_pcUTModelname) + 1;
		if (m_pcUTPath)
			iLength += strlen(m_pcUTPath);
		char *pcFilename = NULL;
		NEWBEGIN
		pcFilename = (char*) new char[iLength];
		NEWEND("CSGAToUnrealTournament::Export - Temporary model filename")
		if (pcFilename) {
			pcFilename[0] = 0;
		// Generate the complete filename
			if (m_pcUTPath)
				strcpy(pcFilename, m_pcUTPath);
			strcat(pcFilename, m_pcUTModelname);
		// Create the exporter
			CAvatarFileHalflife oHL;
		// Set the options

		// Save the Sims model
			if (oHL.Save(pcFilename, poAvatar) == 0)
				m_eResult = VA_MODEL_SAVE_ERROR;
		}
		else
			m_eResult = VA_OUT_OF_MEMORY;
	// Clean up
		if (pcFilename)
			delete [] pcFilename;
		if (poAvatar)
			delete poAvatar;
	}
	return m_eResult;
} // Export

VARESULT CSGAToUnrealTournament::Compress(const char *pcDir, const char *pcSFXName) {
	ASSERT(g_poVAL);
	// Check we have complete string names
	if ((!pcDir) || (!pcSFXName))
		return VA_SFX_ERROR;
	// Get the sfx wedgie name
	const char *pcAppDir = g_poVAL->GetAppDir();
	char pcWJEName[STR_SIZE] = "";
	if (pcAppDir)
		strcpy(pcWJEName, pcAppDir);
	strcat(pcWJEName, "hlsfx.wje");
	// Open the sfx wedgie
	fstream oWJEFile;
	oWJEFile.open(pcWJEName, ios::in|ios::binary|ios::nocreate);
	if (oWJEFile.fail())
		return VA_SFX_ERROR;
	// Decompress the sfx
	CWedgie oWJE;
	if (oWJE.Open(&oWJEFile, "") != WJE_OK) {
		oWJEFile.close();
		return VA_SFX_ERROR;
	}
	if (oWJE.Files() != 1) {
		oWJEFile.close();
		return VA_SFX_ERROR;
	}
	if (oWJE.Extract("hlsfx.exe", pcSFXName) != WJE_OK) {
		oWJEFile.close();
		return VA_SFX_ERROR;
	}
	oWJE.Close();
	oWJEFile.close();
	// Open the sfx application
	fstream oSFXFile;
	oSFXFile.open(pcSFXName, ios::out|ios::binary|ios::nocreate|ios::ate);
	if (oSFXFile.fail()) {
		oSFXFile.close();
		return VA_SFX_ERROR;
	}
	// Create the sfx from the model directory
	if (oWJE.Open(&oSFXFile, pcDir, true, true) != WJE_OK) {
		oWJEFile.close();
		return VA_SFX_ERROR;
	}
	oWJE.Close();
	oSFXFile.close();

	return VA_OK;
} // Compress

const char *CSGAToUnrealTournament::GetErrorString() {
	return m_pcError[(int)m_eResult];
} // GetErrorString
