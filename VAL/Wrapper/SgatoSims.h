//=========---
// SGAToSim
//---------
// SGA1 Avatar to The Sims model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToSims.h - 01/06/2000 - Warren Moore
//	Sims export class
//
// $Id: SgatoSims.h,v 1.2 2000/06/17 10:42:05 waz Exp $
//

#ifndef _WRAP_SGATOSIMS_
#define _WRAP_SGATOSIMS_

#pragma once

//#===--- Defines

// Definitions of class options and arguments
// Option IDs
#define SIMS_AGE					1
#define SIMS_SEX					2
#define SIMS_SKINTONE			3
#define SIMS_BUILD				4
#define SGA_FILENAME			5
#define SIMS_MODELNAME		6
#define SIMS_DIRECTORY		7
#define SIMS_VERBOSE			8
// Option values
#define SIMS_UNKNOWN			0
// SIMS_AGE
#define SIMS_ADULT  1
#define SIMS_CHILD  2
// SIMS_SEX
#define SIMS_MALE   1
#define SIMS_FEMALE 2
// SIMS_SKIN
#define SIMS_LIGHT  1
#define SIMS_MEDIUM 2
#define SIMS_DARK   3
// SIMS_BUILD
#define SIMS_FIT    1
#define SIMS_FAT    2
#define SIMS_SKINNY 3
// SIMS_VERBOSE
#define SIMS_TRUE		1
#define SIMS_FALSE	2

//#===--- Data types

// Exporter status
enum VARESULT {
	VA_OK = 0,
	VA_INVALID_OPTION = 1,
	VA_INVALID_ARGUMENT = 2,
	VA_MISSING_FILENAME = 3,
	VA_MISSING_ARGUMENT = 4,
	VA_AVATAR_LOAD_ERROR = 5,
	VA_MODEL_SAVE_ERROR = 6,
	VA_TIME_EXPIRED = 7,
	VA_OUT_OF_MEMORY = 8,
	VA_DIRECTORY_ERROR = 9,
	VA_ERROR = 10,
};

///////////////
// CSGAToSims

class CSGAToSims {
//#===--- External functions
public:
// Standard constructor/destructor
	CSGAToSims();
	~CSGAToSims();

// Set export options prior to export execution
	VARESULT SetOption(int iOption, int iArgument);						// Integer options
	VARESULT SetOption(int iOption, const char *pcString);		// String options
	const int GetOptionInt(int iOption);
	const char *GetOptionString(int iOption);

// Once options have been set, call export to generate the Sims model
	VARESULT Export();

// Get string of current status/error of exporter class
	const char *GetErrorString();
	
//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	char *m_pcSGAFilename;												// SGA filename storage
	char *m_pcSimModelname;												// Sims filename storage
	char *m_pcSimPath;														// Sims model directory
	int m_iAge, m_iSex, m_iSkinTone, m_iBuild;		// Sims model properties

	bool m_bVerbose;															// Verbose output indicator

	VARESULT m_eResult;														// Status storage
	static const char *m_pcError[];								// Status/error string table
};

#endif // _WRAP_SGATOSIMS_
