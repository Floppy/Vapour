//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/image/image_test.dc.cpp"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! rcsid 		= "$Id: image_test.dc.cpp,v 1.1 2001/11/02 08:59:21 vap-warren Exp $"

//#===--- Includes
#include "image_test.h"

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

