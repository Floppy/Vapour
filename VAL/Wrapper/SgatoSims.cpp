//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToSim.cpp - 12/06/2000 - James Smith
//	SGA Avatar to Sim converter wrapper 
//
// $Id: SgatoSims.cpp,v 1.1 2000/06/16 21:57:47 waz Exp $
//

#include "StdAfx.h"

#include "SGAToSims.h"

#include <iostream.h>
#include "Defs.h"
#include "AvatarFileAME.h"
#include "AvatarFileSims.h"
#include "Avatar.h"
#include "TimeLimit.h"

///////////////
// CSGAToSims

const char *CSGAToSims::m_pcError[] = {
	"Converter status OK",
	"An invalid option has been supplied",
	"Invalid argument for the option specified",
	"A required filename has not been specified",
	"A required option argument was not set",
	"An error occured loading the SGA avatar",
	"An error occured saving the Sims model",
	"Evaluation time expired",
	"Unable to allocate memory block",
	"Error editing a model directory",
	"Unknown error",
};

CSGAToSims::CSGAToSims() {
// Set vars
	m_eResult = VA_OK;
	m_pcSGAFilename = NULL;
	m_pcSimModelname = NULL;
	m_pcSimPath = NULL;
	m_iAge = m_iSex = m_iSkinTone = m_iBuild = SIMS_UNKNOWN;
	m_bVerbose = true;
} // Contructor

CSGAToSims::~CSGAToSims() {
	if (m_pcSGAFilename)
		delete [] m_pcSGAFilename;
	if (m_pcSimModelname)
		delete [] m_pcSimModelname;
	if (m_pcSimPath)
		delete [] m_pcSimPath;
} // Destructor

VARESULT CSGAToSims::SetOption(int iOption, int iArgument) {
	m_eResult = VA_OK;
	switch (iOption) {
		case SIMS_AGE:
			if ((iArgument == SIMS_ADULT) || (iArgument == SIMS_CHILD))
				m_iAge = iArgument;
			else
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case SIMS_SEX:
			if ((iArgument == SIMS_MALE) || (iArgument == SIMS_FEMALE))
				m_iSex = iArgument;
			else
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case SIMS_SKINTONE:
			if ((iArgument == SIMS_LIGHT) || (iArgument == SIMS_MEDIUM) || (iArgument == SIMS_DARK))
				m_iSkinTone = iArgument;
			else
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case SIMS_BUILD:
			if ((iArgument == SIMS_FIT) || (iArgument == SIMS_FAT) || (iArgument == SIMS_SKINNY))
				m_iBuild = iArgument;
			else
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case SIMS_VERBOSE:
			if ((iArgument == SIMS_TRUE) || (iArgument == SIMS_FALSE))
				m_bVerbose = iArgument == SIMS_TRUE;
			else
				m_eResult = VA_INVALID_ARGUMENT;
		default:
			m_eResult = VA_INVALID_OPTION;
	}

	return m_eResult;
} // SetOption

VARESULT CSGAToSims::SetOption(int iOption, const char *pcString) {
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
		case SIMS_MODELNAME:
			if (pcString) {
				iLength = strlen(pcString) + 1;
				pcBuffer = (char*) new char[iLength];
				if (pcBuffer) {
					strcpy(pcBuffer, pcString);
					if (m_pcSimModelname)
						delete [] m_pcSimModelname;
					m_pcSimModelname = pcBuffer;
				}
				else
					m_eResult = VA_OUT_OF_MEMORY;
			}
			else 
				m_eResult = VA_INVALID_ARGUMENT;
			break;
		case SIMS_DIRECTORY:
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
					if (m_pcSimPath)
						delete [] m_pcSimPath;
					m_pcSimPath = pcBuffer;
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

const int CSGAToSims::GetOptionInt(int iOption) {
	int iReturn = SIMS_UNKNOWN;
	switch (iOption) {
		case SIMS_AGE:
			iReturn = m_iAge;
			break;
		case SIMS_SEX:
			iReturn = m_iSex;
			break;
		case SIMS_SKINTONE:
			iReturn = m_iSkinTone;
			break;
		case SIMS_BUILD:
			iReturn = m_iBuild;
			break;
		default:
			m_eResult = VA_INVALID_OPTION;
	}
	return iReturn;
} // GetOptionInt

const char *CSGAToSims::GetOptionString(int iOption) {
	char *pcReturn = NULL;
	switch (iOption) {
		case SGA_FILENAME:
			pcReturn = m_pcSGAFilename;
			break;
		case SIMS_MODELNAME:
			pcReturn = m_pcSimModelname;
			break;
		case SIMS_DIRECTORY:
			pcReturn = m_pcSimPath;
			break;
		default:
			m_eResult = VA_INVALID_OPTION;
	}
	return pcReturn;
} // GetOptionString

VARESULT CSGAToSims::Export() {
/*
// Check for time limit validation
	CTimeLimit oTimeLimit;
	if (!oTimeLimit.Valid()) {
		m_eResult = VA_TIME_EXPIRED;
		return m_eResult;
	}
*/
// Show star-up text
	if (m_bVerbose) {
		cout << "SGA Avatar To The Sims model converter v1.1 (15/06/2000)" << endl;
		cout << "Copyright 2000 Vapour Technology Ltd." << endl << endl;
		cout << "Exclusively licensed to AvatarMe Ltd." << endl << endl;
	}
// Check for correct options
	m_eResult = VA_OK;
	bool bOk = m_pcSGAFilename && m_pcSimModelname;
	if (bOk) {
		bOk = (m_iAge != SIMS_UNKNOWN) && (m_iSex != SIMS_UNKNOWN) &&
					(m_iSkinTone != SIMS_UNKNOWN) && (m_iBuild != SIMS_UNKNOWN);
		if (!bOk)
			m_eResult = VA_MISSING_ARGUMENT;
	}
	else
		m_eResult = VA_MISSING_FILENAME;
// Start model export
	if (bOk) {
		if (m_bVerbose) {
		// Show startup text
			cout << "SGA Avatar    : " << m_pcSGAFilename << endl;
			cout << "Sim Model     : ";
			if (m_pcSimPath)
				cout << m_pcSimPath;
			cout << m_pcSimModelname << endl;
			cout << "Sim Age       : ";
			switch (m_iAge) {
				case SIMS_ADULT: cout << "Adult" << endl; break;
				case SIMS_CHILD: cout << "Child" << endl; break;
			}
			cout << "Sim Sex       : ";
			switch (m_iSex) {
				case SIMS_MALE: cout << "Male" << endl; break;
				case SIMS_FEMALE: cout << "Female" << endl; break;
			}
			cout << "Sim Skin Tone : ";
			switch (m_iSkinTone) {
				case SIMS_LIGHT: cout << "Light" << endl; break;
				case SIMS_MEDIUM: cout << "Medium" << endl; break;
				case SIMS_DARK: cout << "Dark" << endl; break;
			}
			cout << "Sim Build     : ";
			switch (m_iBuild) {
				case SIMS_FIT: cout << "Fit" << endl; break;
				case SIMS_FAT: cout << "Fat" << endl; break;
				case SIMS_SKINNY: cout << "Skinny" << endl; break;
			}
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

	// Allocate the Sims complete filename
		int iLength = strlen(m_pcSimModelname) + 1;
		if (m_pcSimPath)
			iLength += strlen(m_pcSimPath);
		char *pcFilename = NULL;
		NEWBEGIN
		pcFilename = (char*) new char[iLength];
		NEWEND("CSGAToSims::Export - Temporary sim filename")
		if (pcFilename) {
			pcFilename[0] = 0;
		// Generate the Sims complete filename
			if (m_pcSimPath)
				strcpy(pcFilename, m_pcSimPath);
			strcat(pcFilename, m_pcSimModelname);
		// Create the exporter
			CAvatarFileSims oSims;
		// Set the options
			oSims.SetOption(SIMS_AGE, m_iAge);
			oSims.SetOption(SIMS_SEX, m_iSex);
			oSims.SetOption(SIMS_SKINTONE, m_iSkinTone);
			oSims.SetOption(SIMS_BUILD, m_iBuild);
		// Save the Sims model
			if (oSims.Save(pcFilename, poAvatar) == 0)
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

const char *CSGAToSims::GetErrorString() {
	return m_pcError[(int)m_eResult];
} // GetErrorString
