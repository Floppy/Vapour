//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_IMAGE_TEST_
#define _VALET_IMAGE_TEST_

////////////////
//! file 		= "VALET/image/image_test.h"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! rcsid 		= "$Id: image_test.h,v 1.1 2001/11/02 08:59:21 vap-warren Exp $"
//! userlevel 	= Develop

//#===--- Includes
#include "VALET/valet.h"

// This file defines the arch specific functions for testing the image functions
// Each implementation of the image_test.arch.cpp must contain these functions

//: Initialisation
// Sets up any hardware specific issues for testing debug output
//!return: bool - true if init runs ok
bool test_init();

//: Shutdown
// Shuts down any implementation specific stuff
void test_exit();

#endif // _VALET_IMAGE_TEST_
