//=========---
// SGAToSim
//---------
// SGA1 Avatar to The Sims model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToSim.h - 01/06/2000 - Warren Moore
//	Main application function definitions for command-line parsing, 
//  export and progress bar update
//
// $Id: SGAToSim.h,v 1.3 2000/07/19 08:52:29 waz Exp $
//

#ifndef _SGATOSIM_
#define _SGATOSIM_

#pragma once

//#===--- VAL Includes
#include "CommandLine.h"
#include "SGAToSims.h"

//#===--- Defines
#define VEM_CLASS					CSGAToSims
#define VEM_PROGRESS			"SimsSave"
#define VEM_MAXPROCESSES	256

// Main application function
int main(int argc, char **argv);

// Command-line option specification
VARESULT SetOptions(CCommandLine &oCmdLine, VEM_CLASS &oSimsExport);

// An example progress text output function
void Output(const char *pcText);

// An example progress bar updating function
void Progress(double dPercent);

#endif // _SGATOSIM_
