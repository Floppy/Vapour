//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/image/image.test.cpp"
//! author 		= "Warren Moore"
//! date 		= "17/10/2001"
//! rcsid 		= "$Id: image.test.cpp,v 1.1 2001/11/02 08:59:21 vap-warren Exp $"

//#===--- Includes
#include "image_test.h"
#include "image.h"

//#===--- Namespaces
using namespace NVALET;
	
//#===--- Defines

//#===--- Test Functions

//#===--- Main function loop

int main(int argc, char *argv[]) {

	// Init
	if (!test_init()) {
		return -1;
	}
	
   // Test Code

	// Shutdown
	test_exit();
	
	return 0;
}

