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
//! rcsid 		= "$Id: log.test.cpp,v 1.6 2001/10/27 00:01:40 vap-warren Exp $"

//#===--- Includes
#include "log_test.h"
#include "logmanager.h"
#include "log.h"

#include <stdio.h>

//#===--- Namespaces
using namespace NVALET;
	
//#===--- Defines
const static int g_uiMaxDepth = 5;
const static int g_uiMaxLoop = 2;

//#===--- Test Functions

void LoopTest() {
	CLog oLog("loop", "LoopTest");
   // Call the trace function several times
	register int iCount = g_uiMaxLoop;
	while (iCount--) {
		oLog.Trace("Trace called");
	}
} // AddOne

void StartTest(int iCurDepth) {
   CLog oLog("start", "StartTest");
   if (iCurDepth < g_uiMaxDepth) {
      oLog.Trace("Calling StartTest at increased depth", LL_BLOCK);
      StartTest(iCurDepth + 1);
   }
   LoopTest();
}

//#===--- Main function loop

int main(int argc, char *argv[]) {

	// Init
	if (!test_init()) {
		return -1;
	}
	
	// Set the log attributes
	NVALET::g_oLogManager.SetLog("loop", true, LL_ALL);
	NVALET::g_oLogManager.SetLog("start", true, LL_ALL);

	// Start the loop test
	StartTest(0);

	bool bOk = true;
	// Confirm the log results output

	// Shutdown
	test_exit();
	
	return bOk ? 0 : -1;
}

