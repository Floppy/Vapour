//=========---
// SGAToSim
//---------
// SGA1 Avatar to The Sims model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToSim.cpp - 01/06/2000 - Warren Moore
//	Main application source for command-line parsing, 
//  export and progress bar updates
//
// $Id: SGAToSim.cpp,v 1.3 2000/07/11 14:21:48 waz Exp $
//

// Pre-compiled header include
#include "stdafx.h"

#include <iostream.h>
#include <direct.h>
#include <process.h>

// VAL
#include "VALWin32.h"
// Executable include
#include "SGAToSim.h"
// Exporter class include
#include "SGAToSims.h"
// Command-line parser object
#include "CommandLine.h"
// Progress bar controller
#include "ProgressControl.h"

// VAL management object
CVAL *g_poVAL = NULL;

// External progress controller
extern CProgressControl g_oProgressControl;

//#===--- Main Function

int main(int argc, char **argv) {
	// Create the VAL management object
	g_poVAL = (CVAL*) new CVALWin32;
	if (!g_poVAL)
		return -1;

	// Create the Sims exporter
	CSGAToSims oSimsExport;

	// Process command line arguments
	CCommandLine oCmdLine(argc, argv);
	SetOptions(oCmdLine, oSimsExport);

	// Check for verbose output
	bool bVerbose = !oCmdLine.FindOption("0");

	// Attach the output handler functions, if verbose output specified
	if (bVerbose) {
		g_oProgressControl.SetTextFunction("AMELoad", &Output);
		g_oProgressControl.SetProgressFunction("AMELoad", &Progress);
		g_oProgressControl.SetTextFunction("SimsSave", &Output);
		g_oProgressControl.SetProgressFunction("SimsSave", &Progress);
	}

	// Export sims model
	int iReturn = 0;
	VARESULT eResult = oSimsExport.Export();

	// Report error status
	switch (eResult) {
		case VA_OK:
			if (bVerbose)
				cout << "The Sims model created successfully." << endl << endl;
			break;
		default:
			cout << "Error encountered : " << oSimsExport.GetErrorString() << endl << endl;
			iReturn = -1;
	}

	// Package the output files into a self-extracting zip
	if (eResult == VA_OK) {
		// Get the model directory
		const char *pcTemp = NULL;
		char pcPath[STR_SIZE] = "";
		pcTemp = oSimsExport.GetOptionString(SIMS_DIRECTORY);
		if (pcTemp)
			strcpy(pcPath, pcTemp);
		// Get the application directory
		const char *pcAppDir = g_poVAL->GetAppDir();
		if (!pcAppDir) {
			cout << "Error encountered : Unable to get application directory" << endl << endl;
			iReturn = -1;
		}
		if (iReturn == 0) {
			// Get the self-extracting zip filename
			pcTemp = oCmdLine.GetValue(2);
			char pcSFXName[STR_SIZE] = "";
			if ((pcTemp[0] != '\\') && (pcTemp[1] != ':'))
				strcpy(pcSFXName, pcAppDir);
			strcat(pcSFXName, pcTemp);
			// Compress the file
			eResult = oSimsExport.Compress(pcPath, pcSFXName);
			if (eResult != VA_OK) {
				cout << "Error encountered : Unable to create self-extracting avatar" << endl << endl;
				iReturn = -1;
			}
			else
				if (bVerbose)
					cout << "Model compressed successfully" << endl << endl;
		}
	}

	// Delete the VAL management object
	delete g_poVAL;

	return iReturn;
}

//#===--- Command-line argument processing

VARESULT SetOptions(CCommandLine &oCmdLine, CSGAToSims &oSimsExport) {
/* Command-line argument parsing

	This functions sets the default Sim properteries, then parses the command-line
	arguments to find overiding values.
	This function returns an error if the required avatar and sim filenames are not supplied
	Once all the Sim properties have been searched for, the properties are passed
	directly to the Sim exporter object
*/

// Set export defaults
	int iAge = SIMS_ADULT;
	int iSex = SIMS_MALE;
	int iSkinTone = SIMS_LIGHT;
	int iBuild = SIMS_FIT;
	int iVerbose = SIMS_TRUE;

// Command line argument vars
	const char *pcTemp = NULL;
	VARESULT eResult = VA_OK;
	bool bOk = true;

	// SGA filename
	if (pcTemp = oCmdLine.GetValue(1)) {
		oSimsExport.SetOption(SGA_FILENAME, pcTemp);
	}
	else
		bOk = false;

	// Check output filename has been supplied
	if ((!bOk) && (!oCmdLine.GetValue(2)))
		bOk = false;

	// Check all filenames have been specified
	if (!bOk) 
		eResult = VA_MISSING_FILENAME;

	// Sims model name
	if (bOk) {
		pcTemp = oCmdLine.GetValue("sim");
		if (pcTemp)
			oSimsExport.SetOption(SIMS_MODELNAME, pcTemp);
		else {
			pcTemp = oCmdLine.GetValue(2);
			char pcName[STR_SIZE] = "";
			int iDir = 0;
			int iExt = strlen(pcTemp);
			int iCount = 0;
		// Strip preceeding path
			while (iCount < (iExt - 1)) {
				if (pcTemp[iCount] == '\\')
					iDir = iCount + 1;
				iCount++;
			}
			iCount = iExt;
		// Strip file extension
			while (iCount > 0) {
				if (pcTemp[iCount] == '.') {
					iExt = iCount;
					iCount = 1;
				}
				iCount--;
			}
		// Copy out the extracted model name
			iCount = iExt - iDir;
			memcpy(pcName, pcTemp + iDir, iCount);
			pcName[iCount] = 0;
		// Set the model name
			oSimsExport.SetOption(SIMS_MODELNAME, pcName);
		}
	}

	// Sims directory name
	if (bOk) {
	// Get the base directory
		pcTemp = oCmdLine.GetValue("path");
	// If none specified, set the working directory
		char pcWorkingDir[STR_SIZE] = "";
		if (!pcTemp) {
			if (GetModuleFileName( NULL, pcWorkingDir, STR_SIZE) == 0)
				return VA_ERROR;
			char *pch;
			pch = (pcWorkingDir + strlen(pcWorkingDir) - 1);
			while( *pch != '\\' )
				pch--;
			*pch = '\0';
			pcTemp = pcWorkingDir;
		}
	// Find a suitable working directory
		char pcPath[STR_SIZE] = "";
		bool bFound = false;
		int iCount = 0;
		while ((!bFound) && (iCount < MAX_PROCESSES)) {
			pcPath[0] = 0;
			if (pcTemp) {
				strcpy(pcPath, pcTemp);
				int iLength = strlen(pcPath);
				if (pcPath[iLength - 1] != '\\')
					strcat(pcPath, "\\");
			}
			sprintf(pcPath, "%s%d", pcPath, iCount);
			int iResult = _mkdir(pcPath);
			if (iResult == -1) {
				// Check for a valid path
				if (errno == ENOENT) {
					eResult = VA_DIRECTORY_ERROR;
					bOk = false;
					bFound = true;
				}
			}
			else
				bFound = true;
			iCount++;
		}
	// Make sure we haven't overun the max number of processes
		if (iCount == MAX_PROCESSES) {
			eResult = VA_DIRECTORY_ERROR;
			bOk = false;
		}
		oSimsExport.SetOption(SIMS_DIRECTORY, pcPath);
	}

	// Set Sim age
	if (bOk && (pcTemp = oCmdLine.GetValue("age"))) {
		if (pcTemp) {
		// Check for adult
			if (stricmp(pcTemp, "adult") == 0)
				iAge = SIMS_ADULT;
		// Check for child
			if (stricmp(pcTemp, "child") == 0)
				iAge = SIMS_CHILD;
		}
	}
// Set option
	oSimsExport.SetOption(SIMS_AGE, iAge);

	// Set Sim sex
	if (bOk && (pcTemp = oCmdLine.GetValue("sex"))) {
		if (pcTemp) {
		// Check for adult
			if (stricmp(pcTemp, "male") == 0)
				iSex = SIMS_MALE;
		// Check for child
			if (stricmp(pcTemp, "female") == 0)
				iSex = SIMS_FEMALE;
		}
	}
// Set option
	oSimsExport.SetOption(SIMS_SEX, iSex);

	// Set Sim skin tone
	if (bOk && (pcTemp = oCmdLine.GetValue("skin"))) {
		if (pcTemp) {
		// Check for light skin
			if (stricmp(pcTemp, "light") == 0)
				iSkinTone = SIMS_LIGHT;
		// Check for medium skin
			if (stricmp(pcTemp, "medium") == 0)
				iSkinTone = SIMS_MEDIUM;
		// Check for dark skin
			if (stricmp(pcTemp, "dark") == 0)
				iSkinTone = SIMS_DARK;
		}
	}
// Set option
	oSimsExport.SetOption(SIMS_SKINTONE, iSkinTone);

// Set Sim build
	if (bOk && (pcTemp = oCmdLine.GetValue("build"))) {
		if (pcTemp) {
		// Check for fit build
			if (stricmp(pcTemp, "fit") == 0)
				iBuild = SIMS_FIT;
		// Check for fat build
			if (stricmp(pcTemp, "fat") == 0)
				iBuild = SIMS_FAT;
		// Check for skinny build
			if (stricmp(pcTemp, "skinny") == 0)
				iBuild = SIMS_SKINNY;
		}
	}
// Set option
	oSimsExport.SetOption(SIMS_BUILD, iBuild);

// Set converter verbose output
	if (bOk && (oCmdLine.FindOption("0")))
		iVerbose = SIMS_FALSE;
	oSimsExport.SetOption(SIMS_VERBOSE, iVerbose);
	return eResult;
} // SetOptions

//#===--- Progress bar updates

void Output(const char *pcText) {
/* Example text status report function

	This function receives text output of the current status of the avatar load and
	Sims save functions. In this example, the string values passed back by the exporter
	modules are echoed directly to standard output.
*/

	if (pcText)
		cout << pcText << endl;
} // Output

void Progress(double dPercent) {
/* Example function description

	The export process is in two parts, 1) loading and processing the avatar and 2)
	processing and saving the avatar in the Sims format. As such, each part returns
	a progress update value from 0.0 to 1.0, once for loading then again for saving.
	This function converts the two ranges to one range from 0.0 to 1.0 by initially
	scaling the input value by 50 (to give a range between 0 and 50). After the load
	has completed (this function has been called between 0.0 and 1.0, and this function
	has outputted integer values between 0 and 50), an offset value of 50 is set.
	The save function calls this function with values between 0.0 and 1.0, and this
	function outputs integer values between 50 and 100.
*/

	static int iOffset = 0;						// Percentage offset - 0 for load, 50 for save
// Calculate an integer percentage (from 0 to 50) from the 
// floating point value given from the load or save modules
	int iPercent = int(dPercent * 50.0) + iOffset;
// Uncomment the following line if you wish to display the percentage progress
/*
	cout << iPercent << " complete" << endl;
*/
// If the load reaches the end (dPercent == 1.0), then set the offset to prepare for the save
	if ((iPercent == 50) && (iOffset == 0))
		iOffset = 50;
} // Progress


