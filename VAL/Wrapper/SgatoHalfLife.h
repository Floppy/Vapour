//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToHalfLife.h - 06/07/2000 - Warren Moore
//	SGA Avatar to Half Life converter wrapper 
//
// $Id: SgatoHalfLife.h,v 1.2 2000/07/11 17:41:38 waz Exp $
//

#ifndef _WRAP_SGATOHALFLIFE_
#define _WRAP_SGATOHALFLIFE_

#pragma once

#include "Wrapper.h"

//#===--- Defines

// Definitions of class options and arguments
// Option IDs
#define SGA_FILENAME			1
#define HL_MODELNAME		2
#define HL_DIRECTORY		3
#define HL_VERBOSE			4
// Option values
#define HL_UNKNOWN			0
// SIMS_VERBOSE
#define HL_TRUE		1
#define HL_FALSE	2

//#===--- Data types

///////////////////
// CSGAToHalfLife

class CSGAToHalfLife {
//#===--- External functions
public:
// Standard constructor/destructor
	CSGAToHalfLife();
	~CSGAToHalfLife();

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
	char *m_pcHLModelname;												// HL filename storage
	char *m_pcHLPath;															// HL model directory

	bool m_bVerbose;															// Verbose output indicator

	VARESULT m_eResult;														// Status storage
	static const char *m_pcError[];								// Status/error string table
};

#endif // _WRAP_SGATOHALFLIFE_
