//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_CORE_LOG_TEST_
#define _VALET_CORE_LOG_TEST_

////////////////
//! file 		= "VALET/core/log_test.h"
//! author 		= "Warren Moore"
//! date 		= "15/10/2001"
//! rcsid 		= "$Id: log_test.h,v 1.4 2001/10/21 14:38:33 vap-warren Exp $"
//! userlevel 	= Develop

//#===--- Includes
#include "VALET/valet.h"

// This file defines the arch specific functions for testing the log output
// Each implementation of the log_test.arch.cpp must contain these functions

//: Initialisation
// Sets up any hardware specific issues for testing debug output
//!return: bool - true if init runs ok
bool test_init();

//: Shutdown
// Shuts down any implementation specific stuff
void test_exit();

//: Log output confirmation
// Check that a log actually contains the supplied log message in a suitable format
//!return: bool - true if the message is found in the log at the correct level
bool test_confirm(const char *pcLog, int iLevel, const char *pcMessage);

#endif // _VALET_CORE_LOG_TEST_
