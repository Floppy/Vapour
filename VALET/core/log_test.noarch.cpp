//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/log_test.noarch.cpp"
//! author 		= "Warren Moore"
//! date 		= "15/10/2001"
//! rcsid 		= "$Id: log_test.noarch.cpp,v 1.1 2001/10/17 00:22:48 vap-warren Exp $"

//#===--- Includes
#include "log_test.h"

#include <stdio.h>

//#===--- Namespaces
using namespace NValet;
	
bool test_init() {
	// Don't need to set anything up
	return true;
} // test_init

void test_exit() {
	// Don't need to shut anything down
} // test_exit

bool test_confirm(const char *pcLog, int iLevel, const char *pcMessage) {
	//!todo: Actually implement test confirmation
	return true;
} // test_confirm

