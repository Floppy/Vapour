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
//! rcsid 		= "$Id: valet_common.h,v 1.13 2001/10/27 15:40:01 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Arch"
//! maindoc		= 1

//#===--- NVALET Namespace
//: Library Namespace
// Contains VALET global definitions

namespace NVALET {

   /*#===--- Constants */
   const unsigned int g_uiStrLength = 1024;	    // Default fixed string length

   enum EFileResult {
      FR_OK = 0,
      FR_DOES_NOT_EXIST,
      FR_FILE_ERROR,
      FR_WRONG_FILE_TYPE,
      FR_WRONG_IMAGE_TYPE,
      FR_OUT_OF_MEMORY,
      FR_WRONG_CONTEXT,
      FR_HASNT_CHANGED,
      FR_NO_FILE_NAME,
      FR_NO_FILE_EXTENSION,
      FR_NO_DATA_TO_SAVE,
      FR_DIR_ERROR,
      FR_INVALID_OPERATION,
      FR_BAD_VERSION,
      FR_BAD_DATA,
      FR_ERROR
   };
   //: File Results

   /*#===--- Logging */
   class CLogManager;
   extern CLogManager g_oLogManager;
   
}

/*#===--- Inlines */

#ifdef VDEBUG
#define INLINE
#else
#define VALET_INLINE
#define INLINE inline
#endif /* VDEBUG */

/*#===--- C++ Style casts */
#ifdef VALET_CXX_CASTS_SUPPORTED

#define const_cast(x, y)            const_cast<x>(y)
#define static_cast(x, y)           static_cast<x>(y)
#define dynamic_cast(x, y)          dynamic_cast<x>(y)
#define reinterpret_cast(x, y)      reinterpret_cast<x>(y)

#else

#define const_cast(x, y)            ((x)y)
#define static_cast(x, y)           ((x)y)
#define dynamic_cast(x, y)          ((x)y)
#define reinterpret_cast(x, y)      ((x)y)

#endif /* VALET_CXX_CASTS_SUPPORTED */

/*#===--- Common Defines */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef ASSERT
#define ASSERT(x) ((void)0)
#endif /* ASSERT */

#endif /* _VALET_ */
