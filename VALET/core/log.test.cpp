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
//! rcsid 		= "$Id: log.test.cpp,v 1.4 2001/10/21 14:38:32 vap-warren Exp $"

//#===--- Includes
#include "log_test.h"
#include "logmanager.h"
#include "log.h"

#include <stdio.h>

//#===--- Namespaces
using namespace NValet;
	
//#===--- Defines
#define LOOP_MAX 5

//#===--- Test Functions

void AddOne(int x) {
	CLog oLog("test", "AddOne");
	register int i = LOOP_MAX;
	while (i--) {
		oLog.Trace("Adding one", 1);
		x++;
	}
} // AddOne

//#===--- Main function loop

int main(int argc, char *argv[]) {

	// Init
	if (!test_init()) {
		return -1;
	}
	
	// Set the log attributes
	NValet::g_oLogManager.SetLog("test", true, 0);

	// Start the loop test
	int x = 0;
	AddOne(x);

	bool bOk = true;
	// Confirm the log results output

	// Shutdown
	test_exit();
	
	return bOk ? 0 : -1;
}

