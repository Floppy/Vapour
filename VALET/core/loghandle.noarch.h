//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_CORE_LOGHANDLE_NOARCH_
#define _VALET_CORE_LOGHANDLE_NOARCH_

////////////////
//! file 		= "VALET/core/loghandle.noarch.h"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.noarch.h,v 1.8 2001/10/27 00:02:17 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "VALET/valet.h"

#include <fstream.h>

namespace NVALET {

	//#===--- CLogHandle
	//: Debug log handle for files
	// This is the default implementation of the log handle object
	// Uses standard C++ file support

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

      char m_pcType[g_uiStrLength];          // Name of log type
      static ofstream *m_poLogFile;          // File handle for all log output
      static unsigned int m_uiFileCount;     // Reference counter for file
      
	};

}
	
#endif // _VALET_CORE_LOGHANDLE_NOARCH_
