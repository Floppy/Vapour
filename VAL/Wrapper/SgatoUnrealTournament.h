//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToUnrealTournament.h - 06/07/2000 - Warren Moore
//	SGA Avatar to Unreal Tournament converter wrapper 
//
// $Id: SgatoUnrealTournament.h,v 1.2 2000/07/16 12:28:05 waz Exp $
//

#ifndef _WRAP_SGATOUNREALTOURNAMENT_
#define _WRAP_SGATOUNREALTOURNAMENT_

#pragma once

#include "Wrapper.h"

//#===--- Defines

// Definitions of class options and arguments
// Option IDs
#define SGA_FILENAME			1
#define UT_MODELNAME		2
#define UT_DIRECTORY		3
#define UT_VERBOSE			4
// Option values
#define UT_UNKNOWN			0
// SIMS_VERBOSE
#define UT_TRUE		1
#define UT_FALSE	2

// App defines
#define SFX_NAME				"utsfx.wje"
#define VER_DATE				"v1.0 (11/07/2000)"

//#===--- Data types

///////////////////////////
// CSGAToUnrealTournament

class CSGAToUnrealTournament {
//#===--- External functions
public:
// Standard constructor/destructor
	CSGAToUnrealTournament();
	~CSGAToUnrealTournament();

// Set export options prior to export execution
	VARESULT SetOption(int iOption, int iArgument);						// Integer options
	VARESULT SetOption(int iOption, const char *pcString);		// String options
	const int GetOptionInt(int iOption);
	const char *GetOptionString(int iOption);

// Once options have been set, call export to generate the Sims model
	VARESULT Export();

// Once the model has been exported, compress it into a wedgie
	VARESULT Compress(const char *pcDir, const char *pcSFXName);

// Get string of current status/error of exporter class
	const char *GetErrorString();
	
//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	char *m_pcSGAFilename;												// SGA filename storage
	char *m_pcModelname;													// Filename storage
	char *m_pcPath;																// Model directory

	bool m_bVerbose;															// Verbose output indicator

	VARESULT m_eResult;														// Status storage
	static const char *m_pcError[];								// Status/error string table
};

#endif // _WRAP_SGATOUNREALTOURNAMENT_
