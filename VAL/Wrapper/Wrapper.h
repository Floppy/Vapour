//=========---
// SGAToSim
//---------
// SGA1 Avatar to The Sims model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// Wrapper.h - 06/06/2000 - Warren Moore
//	Global definitions for avatar converter wrappers
//
// $Id: Wrapper.h,v 1.4 2000/07/21 17:13:19 waz Exp $
//

#ifndef _WRAP_
#define _WRAP_

#pragma once

//#===--- Defines

// Definitions of standard class options and arguments
// for Vapour Exported Models

// Option IDs
#define VEM_SGANAME				1
#define VEM_MODELNAME			2
#define VEM_DIRECTORY			3
#define VEM_VERBOSE				4
// Option values
#define VEM_UNKNOWN				0
#define VEM_TRUE					1
#define VEM_FALSE					2

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
	VA_WJE_MISSING = 10,
	VA_WJE_ERROR = 11,
	VA_SFX_ERROR = 12,
	VA_COMPRESS_ERROR = 13,
	VA_ERROR = 14,
};

// Exporter error strings
#define WRAP_ERROR_STRINGS \
	"Converter status OK", \
	"An invalid option has been supplied", \
	"Invalid argument for the option specified", \
	"A required filename has not been specified", \
	"A required option argument was not set", \
	"An error occured loading the SGA avatar", \
	"An error occured saving the exported model", \
	"Evaluation time expired", \
	"Unable to allocate memory block", \
	"Error editing a model directory", \
	"Error WJE containing self-extractor missing", \
	"Error opening WJE file", \
	"Error opening self-extracting installer", \
	"Error compressing avatar", \
	"Unknown error",

#endif // _WRAP_
