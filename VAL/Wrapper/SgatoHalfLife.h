//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToHalfLife.h - 12/06/2000 - Warren Moore
//	SGA Avatar to Half Life converter wrapper 
//
// $Id: SgatoHalfLife.h,v 1.3 2000/07/19 08:49:39 waz Exp $
//

#ifndef _WRAP_SGATOHALFLIFE_
#define _WRAP_SGATOHALFLIFE_

#pragma once

#include "Wrapper.h"

//#===--- Defines

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
	char *m_pcModelname;													// Model filename storage
	char *m_pcPath;																// Model temp directory

	bool m_bVerbose;															// Verbose output indicator

	VARESULT m_eResult;														// Status storage
	static const char *m_pcError[];								// Status/error string table
};

#endif // _WRAP_SGATOHALFLIFE_
