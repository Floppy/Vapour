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
//! rcsid 		= "$Id: memory.dc.cpp,v 1.2 2001/10/27 15:41:18 vap-warren Exp $"

//#===--- Includes
#include "VALET/valet.h"

using namespace NVALET;

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
// Calls arch_exit and does not return
void abort(void) {
   dbglog(DBG_CRITICAL, "Program aborted...\n");
   arch_exit();
}

//: Assert failed output function
// Actual macros declared in assert.h, this simply formats output then aborts
void __assert(const char *pcFile, int iLine, const char *pcData) {
   char pcStr[g_uiStrLength] = "";
   sprintf(pcStr, "ASSERTION FAILED: in '%s' line (%d) : %s\n", pcFile, iLine, pcData);
   dbglog(DBG_CRITICAL, pcStr);
   abort();
}

#ifdef __cplusplus
}
#endif
