//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CommandLine.h - 01/06/2000 - Warren Moore
//	Command line parsing class 
//
// $Id: CommandLine.h,v 1.2 2000/06/17 10:42:06 waz Exp $
//

#ifndef _VAL_COMMANDLINE_
#define _VAL_COMMANDLINE_

#pragma once

//#===--- Defines

//#===--- Data types

/////////////////
// CCommandLine

class CCommandLine {
//#===--- External functions
public:

// Constructor/destructor
	CCommandLine(int iCount, char **pcData);		// Create with command line params argc and argv
	~CCommandLine();

// Returns the argument string supplied for a search option
// Returns NULL if search option not found
//   application usage : application.exe [-option argument]*
	const char *GetValue(const char *pcOption);

// Returns the value at the specified position
	const char *GetValue(const int iPos);

// Returns whether the option was found
	const bool FindOption(const char *pcOption);

//#===--- Internal Functions
private:

//#===--- Internal Data
private:
	int m_iCount;				// argument count
	char **m_pcData;		// list of arguments
};

#endif // _VAL_COMMANDLINE_
