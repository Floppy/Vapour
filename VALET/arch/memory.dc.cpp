//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/arch/memory.dc.cpp"
//! author 		= "Warren Moore"
//! date 		= "15/10/2001"
//! lib 			= libVALETarch
//! rcsid 		= "$Id: memory.dc.cpp,v 1.1 2001/10/16 22:31:17 vap-warren Exp $"

//#===--- Includes
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

//: Dreamcast default new and delete implementations
// sh-elf-g++ 2.95.x does not have any default implementations
// for new and delete, so here are some home rolled ones

void *__builtin_new(int size) {
	return malloc(size);
}

void *__builtin_vec_new(int size) {
	return __builtin_new(size);
}

void __builtin_delete(void *ptr) {
	free(ptr);
}

void __builtin_vec_delete(void *ptr) {
	__builtin_delete(ptr);
}

//: Abort required by sh-elf gcc-lib
// Ok, ok, this is a horrendous hack which will barf warnings
// about a noreturn function returning, but it links ok
// Will need to be implemented properly once asserts are implemented

void abort(void) {
}

#ifdef __cplusplus
}
#endif
