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
// $Id: SGAToSim.h,v 1.2 2000/07/14 19:57:25 waz Exp $
//

#ifndef _SGATOSIM_
#define _SGATOSIM_

#pragma once

//#===--- VAL Includes
#include "CommandLine.h"
#include "SGAToSims.h"

//#===--- Defines
#define MAX_PROCESSES	256

// Main application function
int main(int argc, char **argv);

// Command-line option specification
VARESULT SetOptions(CCommandLine &oCmdLine, CSGAToSims &oSimsExport);

// An example progress text output function
void Output(const char *pcText);

// An example progress bar updating function
void Progress(double dPercent);

#endif // _SGATOSIM_
