//========---
// SGAToUT
//--------
// SGA1 Avatar to Unreal Tournament model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToUT.h - 01/06/2000 - Warren Moore
//	Main application function definitions for command-line parsing, 
//  export and progress bar update
//
// $Id: SGAToUT.h,v 1.0 2000/08/29 13:51:01 waz Exp $
//

#ifndef _SGATOUT_
#define _SGATOUT_

#pragma once

//#===--- VAL Includes
#include "CommandLine.h"
#include "SGAToUnrealTournament.h"

//#===--- Defines
#define VEM_CLASS					CSGAToUnrealTournament
#define VEM_PROGRESS			"UTSave"
#define VEM_MAXPROCESSES	256

// Main application function
int main(int argc, char **argv);

// Command-line option specification
VARESULT SetOptions(CCommandLine &oCmdLine, VEM_CLASS &oSimsExport);

// An example progress text output function
void Output(const char *pcText);

// An example progress bar updating function
void Progress(double dPercent);

#endif // _SGATOUT_
