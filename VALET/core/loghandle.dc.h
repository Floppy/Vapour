//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_CORE_LOGHANDLE_DC_
#define _VALET_CORE_LOGHANDLE_DC_

////////////////
//! file 		= "VALET/core/loghandle.dc.h"
//! author 		= "Warren Moore"
//! date 		= "26/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.dc.h,v 1.7 2001/10/21 14:38:33 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "VALET/valet.h"

namespace NValet {

	//#===--- CLogHandle
	//: Debug log handle for files
	// This is the serial implementation of the log handle object
	// using the DC serial cable and dc-tool

	class CLogHandle {
	public:

		CLogHandle(const char *pcType);
		// Constructor
		//!param: pcType = Type of log stream
		
		~CLogHandle();
		// Destructor

		void Trace(int iLevel, const char *pcFunction, const char *pcMessage);
		// Log output
		//!param: iLevel = Level of log message
		//!param: pcFunction = Function name
		//!param: pcMessage = Log message string

	protected:

		char m_pcType[m_uiStrLength];		// Name of log type
		
	};

}
	
#endif // _VALET_CORE_LOGHANDLE_DC_
