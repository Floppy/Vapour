//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/log.test.cpp"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: log.test.cpp,v 1.2 2001/10/09 20:44:30 vap-james Exp $"

//#===--- Includes
#include "../arch/valet.h"
#include "logmanager.h"
#include "log.h"

#include <stdio.h>

//#===--- Namespaces
using namespace NValet;
	
//#===--- Defines
#define LOOP_MAX 1000

//#===--- Globals
// Instance of the global log manager (located in namespace NValet)
NValet::CLogManager NValet::g_oLogManager(true);

//#===--- Test Functions

void AddOne(int &x) {
	CLog oLog("test", "AddOne");
	register int i = LOOP_MAX;
	while (i--) {
		oLog.Trace("Adding one", 1);
		x++;
	}
} // AddOne

//#===--- Main function loop

int main(int argc, char *argv[]) {

	// Set the log attributes
	NValet::g_oLogManager.SetLog("test", true, 0);

	// Start the loop test
	printf("Starting test...\n");
	int x = 0;
	AddOne(x);

	// Don't cause 'make test' to fail the test
	return 0;
}

