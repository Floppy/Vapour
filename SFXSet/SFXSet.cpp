//=======---
// SFXSet
//-------
// Sets the end point of a self extracting executable
// Copyright 2000 Vapour Technology Ltd.
//
// SFXSet.cpp - 27/11/2000 - Warren Moore
//	  Main application source for command-line parsing, and execution
// 
// $Id: SFXSet.cpp,v 1.1 2000/11/27 17:16:00 cvs Exp $
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
		cout << "Usage: sfxset <filename>" << endl;
		return -1;
	}
	// Create the SFX object
	CSFX oSFX;

	bool bSet = false;
	int iCount = 0;
	while (iCount++ < 10) {
		// Try to set the file
		bSet = oSFX.SetEnd(pcAppName);
		// Quit early if set
		if (bSet)
			iCount = 10;
	}
	if (bSet)
		cout << "Successfully set self-extractor markers for " << pcAppName << endl;
	else 
		cout << "Unable to set self-extractor markers for " << pcAppName << endl;

	// Delete the VAL management object
	delete g_poVAL;

	return 0;
}

