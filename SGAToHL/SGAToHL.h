//========---
// SGAToHL
//--------
// SGA1 Avatar to Half-Life model exporter
// Copyright 2000 Vapour Technology Ltd.
//
// SGAToHL.h - 01/06/2000 - Warren Moore
//	Main application function definitions for command-line parsing, 
//  export and progress bar update
//
// $Id: SGAToHL.h,v 1.1 2000/08/01 23:10:30 waz Exp $
//

#ifndef _SGATOHL_
#define _SGATOHL_

#pragma once

//#===--- VAL Includes
#include "CommandLine.h"
#include "SGAToHalfLife.h"

//#===--- Defines
#define VEM_CLASS					CSGAToHalfLife
#define VEM_PROGRESS			"HLSave"
#define VEM_MAXPROCESSES	256

// Main application function
int main(int argc, char **argv);

// Command-line option specification
VARESULT SetOptions(CCommandLine &oCmdLine, VEM_CLASS &oSimsExport);

// An example progress text output function
void Output(const char *pcText);

// An example progress bar updating function
void Progress(double dPercent);

#endif // _SGATOHL_
