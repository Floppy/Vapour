//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/arch/global.cpp"
//! author 		= "Warren Moore"
//! date 		= "20/10/2001"
//! lib 			= libVALETarch
//! rcsid 		= "$Id: global.cpp,v 1.1 2001/10/21 14:28:50 vap-warren Exp $"

//#===--- Includes
#include "VALET/valet.h"
#include "VALET/logmanager.h"

namespace NValet {
   
   //#===--- Global variables

   // The debug log manager - only running if VDEBUG defined
#ifdef VDEBUG
#define VALET_LOGGING_ACTIVE true
#else
#define VALET_LOGGING_ACTIVE
#endif
   CLogManager g_oLogManager(VALET_LOGGING_ACTIVE);

}


