//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_LOGHANDLE_DC_
#define _VALET_LOGHANDLE_DC_

////////////////
//! file 		= "VALET/core/loghandle.dc.h"
//! author 		= "Warren Moore"
//! date 		= "26/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.dc.h,v 1.1 2001/09/26 13:10:10 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "../arch/valet.h"

#include <fstream.h>

namespace VALET {

	//#===--- CLogHandle
	//: Debug log handle for files
	// This is the serial implementation of the log handle object
	// using the DC serial cable

	class CLogHandle {
	public:
		CLogHandle(const char *pcType);
		// Constructor
		//!param: pcType = Type of log stream
		
		~CLogHandle();
		// Destructor

		void Trace(int iLevel, const char *pcMessage);
		// Log output
		//!param: iLevel = Level of log message
		//!param: pcMessage = Log message string

	protected:

	};

}
	
#endif // _VALET_LOGHANDLE_DC_
