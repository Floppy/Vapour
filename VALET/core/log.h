//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_LOG_
#define _VALET_LOG_

////////////////
//! file 		= "VALET/core/log.h"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: log.h,v 1.2 2001/10/09 20:44:30 vap-james Exp $"
//! userlevel 	= Normal
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "../arch/valet.h"
#include "logmanager.h"

#include <string.h>

namespace NValet {
	
	//#===--- Externally linked objects
	extern CLogManager g_oLogManager;
	
	//#===--- CLog
	//: Debug trace logging object
	//	An object created inside functions used for effective logging
	//	NOTE: Do not use inside small, inlinable functions

	class CLog {
	public:
		CLog(const char *pcType, const char *pcFunction);
		//: Constructor
		// Registers the trace object with the specified type
		// and stores the function name
		//!param: pcType = Type under which to be logged
		//!param: pcFunction = Name of function object is instantiated in
	
		~CLog();
		// Destructor

		void Trace(const char *pcMessage, int iLevel = 0);
		//: Trace Function
		// Routes trace messages through to the log manager
		//!param: pcMessage = Log message
		//!param: iLevel = Log level of message

	protected:

		int m_iID;					// Log type ID
		char *m_pcFunction;		// Function name
		
	};

}

//#===--- Inline Functions

#ifdef VALET_INLINE
#include "log_inline.cpp"
#endif // VALET_INLINE

#endif // _VALET_LOG_
