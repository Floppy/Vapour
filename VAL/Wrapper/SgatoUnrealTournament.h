//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToUnrealTournament.h - 12/06/2000 - Warren Moore
//	SGA Avatar to Unreal Tournament converter wrapper 
//
// $Id: SgatoUnrealTournament.h,v 1.4 2000/08/29 13:49:30 waz Exp $
//

#ifndef _WRAP_SGATOUNREALTOURNAMENT_
#define _WRAP_SGATOUNREALTOURNAMENT_

#pragma once

#include "Wrapper.h"

//#===--- Defines

// Definitions of class options and arguments
// Option IDs
#define UNREAL_SEX			   5
// Option values
// UNREAL_SEX
#define UNREAL_MALE		      1
#define UNREAL_FEMALE			2

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
	VARESULT SetOption(int iOption, int iArgument);					// Integer options
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
	char *m_pcModelname;													// Model filename storage
	char *m_pcPath;														// Model temp directory

	int m_iSex;																// Model properties

	bool m_bVerbose;														// Verbose output indicator

	VARESULT m_eResult;													// Status storage
	static const char *m_pcError[];									// Status/error string table
};

#endif // _WRAP_SGATOUNREALTOURNAMENT_
