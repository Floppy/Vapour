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
//! rcsid 		= "$Id: valet_common.h,v 1.6 2001/10/18 01:19:19 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Arch"
//! maindoc		= 1

//#===--- NValet Namespace
//: Library Namespace
// Contains VALET globals

namespace NValet {

	#define STR_LENGTH 1024
	// Default fixed string length

   enum FRESULT {
      F_OK,
      F_DOES_NOT_EXIST,
      F_FILE_ERROR,
      F_WRONG_FILE_TYPE,
      F_WRONG_IMAGE_TYPE,
      F_OUT_OF_MEMORY,
      F_WRONG_CONTEXT,
      F_HASNT_CHANGED,
      F_NO_FILE_NAME,
      F_NO_FILE_EXTENSION,
      F_NO_DATA_TO_SAVE,
      F_DIR_ERROR,
      F_INVALID_OPERATION,
      F_BAD_VERSION,
      F_BAD_DATA,
      F_ERROR,
   };

}

//#===--- Debug settings

#define VDEBUG

#ifdef VDEBUG
#define NDEBUG
#endif

//#===--- Inlines

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
