//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_NOARCH_
#define _VALET_NOARCH_

////////////////
//! file 		= "VALET/arch/valet.noarch.h""
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALET
//! rcsid 		= "$Id: valet.noarch.h,v 1.5 2001/10/27 15:41:19 vap-warren Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Arch"

/*#===--- Supports C++ style casts */
#define VALET_CXX_CASTS_SUPPORTED

/*#===--- Includes */
#include "VALET/valet_common.h"

/*#===--- Assert */

#ifdef ASSERT
#undef ASSERT
#endif

#define ASSERT(x)    assert(x)

#include <assert.h>

#endif /* _VALET_NOARCH_ */
