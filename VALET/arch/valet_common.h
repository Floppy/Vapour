//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_
#define _VALET_

////////////////
//! file 		= "VALET/arch/valet_common.h""
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALET
//! rcsid 		= "$Id: valet_common.h,v 1.1 2001/09/26 13:10:08 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Arch"

//#===--- VALET Namespace
//: Library Namespace
// Contains VALET globals

namespace VALET {

	#define STR_LENGTH 1024
	// Default fixed string length
}

//#===--- Inline defines

#define VDEBUG

#ifdef VDEBUG
#define INLINE
#else
#define VALET_INLINE
#define INLINE inline
#endif // VDEBUG

//#===--- Common Defines

#ifndef NULL
#define NULL 0
#endif // NULL

#ifndef ASSERT
#define ASSERT(x)
#endif // ASSERT

#endif // _VALET_
