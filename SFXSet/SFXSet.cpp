//=======---
// SFXSet
//-------
// Sets the end point of a self extracting executable
// Copyright 2000 Vapour Technology Ltd.
//
// SFXSet.cpp - 27/11/2000 - Warren Moore
//	  Main application source for command-line parsing, and execution
// 
// $Id: SFXSet.cpp,v 1.2 2000/11/29 15:52:26 warren Exp $
//

#include <iostream.h>
#include <string.h>

// VAL
#include "VALWin32.h"
// Command-line parser object
#include "CommandLine.h"
// Self-extrater manager
#include "SFX.h"


// VAL management object
CVAL *g_poVAL = NULL;

//#===--- Main Function

int main(int argc, char **argv) {
	// Create the VAL management object
	g_poVAL = (CVAL*) new CVALWin32;
	if (!g_poVAL)
		return -1;

	// Write the banner
	cout << "SFXSet v1.0 (" << __DATE__ << ")" << endl;
	cout << "Copyright 2000 Vapour Technology Ltd." << endl << endl;

	// Process command line arguments
	CCommandLine oCmdLine(argc, argv);

	const char *pcAppName = oCmdLine.GetValue(1);
	if (!pcAppName) {
		cout << "Missing required filename" << endl;
		cout << "Usage: sfxset <sfx_file> [<data_file>]" << endl;
		return -1;
	}
	// Create the SFX object
	CSFX oSFX;

	// Try to set the file
	if (oSFX.SetEnd(pcAppName))
		cout << "Successfully set self-extractor markers for " << pcAppName << endl;
	else 
		cout << "Unable to set self-extractor markers for " << pcAppName << endl;

	// Check if we have a data file
	const char *pcDataName = oCmdLine.GetValue(2);
	if (pcDataName) {
		if (oSFX.AttachFile(pcDataName, pcAppName))
			cout << "Successfully attached " << pcDataName << " to " << pcAppName << endl;
		else 
			cout << "Unable to attach " << pcDataName << " to " << pcAppName << endl;
	}

	// Delete the VAL management object
	delete g_poVAL;

	return 0;
}

