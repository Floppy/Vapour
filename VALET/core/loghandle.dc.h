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
//! rcsid 		= "$Id: loghandle.dc.h,v 1.4 2001/10/17 00:46:00 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "valet.h"

namespace NValet {

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

		char m_pcType[STR_LENGTH];		// Name of log type
		
	};

}
	
#endif // _VALET_LOGHANDLE_DC_
