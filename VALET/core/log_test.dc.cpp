//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/log_test.dc.cpp"
//! author 		= "Warren Moore"
//! date 		= "15/10/2001"
//! rcsid 		= "$Id: log_test.dc.cpp,v 1.2 2001/10/24 21:31:38 vap-james Exp $"

//#===--- Includes
#include "log_test.h"

#include <stdio.h>

//#===--- Namespaces
using namespace NVALET;
	
bool test_init() {
	// Set up the DC and dcload overrides
	kos_init_all(ALL_ENABLE, ROMDISK_NONE);
	// no result for setup, so return true
	return true;
} // test_init

void test_exit() {
	// Close down the DC and dctool so we don't have to reboot the console!
	kos_shutdown_all();
} // test_exit

bool test_confirm(const char *pcLog, int iLevel, const char *pcMessage) {
	//!todo: Actually implement test confirmation
	return true;
} // test_confirm

