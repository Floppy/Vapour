//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_CORE_LOGMANAGER_
#define _VALET_CORE_LOGMANAGER_

////////////////
//! file 		= "VALET/core/logmanager.h"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: logmanager.h,v 1.11 2001/10/24 21:31:39 vap-james Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "VALET/valet.h"

namespace NVALET {

   enum LOG_LEVEL {
      LL_ALL = 0,
      LL_FUNCTION = 1,
      LL_OBJECT = 2,
      LL_BLOCK = 3,
      LL_ERROR = 4,
      LL_CRITICAL = 5
   };
   //: Log level enum
   // LL_ALL         = Complete verbose logging
   // LL_FUNCTION    = Logging of function calls and below
   // LL_OBJECT      = Logging of object creation/destruction and below
   // LL_BLOCK       = Logging of block level operations and below
   //	LL_ERROR       = Logging of errors only
   // LL_CRITICAL    = Logging of fatal errors only
   
	//#===--- Predeclared Classes
	class CLogHandle;

	//#===--- CLogManager
	//: Central manager for log output
	// Object that controls the output of CLog objects
	// Should only be one global log manager per application
   // (automatically include in VALET/valet.h &amp; library VALETarch
	//!classtodo: Move arrays to dynamically allocated lists if it is a performance issue

	class CLogManager {
	public:
		CLogManager(bool bLog = false);
      //: Constructor
		//!param: bLog = Global logging option

		~CLogManager();
      //: Destructor

		//:------
		//: Log management
		
		void SetLog(const char *pcType, bool bLog, int iLevel = LL_ALL);
		// Set whether a log is active, and to what level
		//!param: pcType = Type under which to be logged
		//!param: bLog = Whether log is active or not
		//!param: iLevel = Set which semantic level to log to

		//:------
		//: CLog Requests

		bool Active();
		// Queries whether logging is active in general

		int Active(const char *pcType);
		// Queries whether a log is active, returns -1 if not, otherwise returns log ID
		//!param: pcType = Log type to test

		void Trace(int iID, const char *pcFunction, const char *pcMessage, int iLevel);
		// Outputs to the log
		//!param: iID = Log type ID
		//!param: pcFunction = Function name
		//!param: pcMessage = Log message
		//!param: iLevel = Level of log message

	protected:
		//:------
		//: Utility Functions
	
		int CheckType(const char *pcType);
		// Checks whether a log is active, and if not, tries to create one
		//!param: pcType = Log type to check

		//:------
		//: Log creation

		int Open(const char *pcType, int iID);
		// Opens the log - I/O dependent

		void Close(int iID);
		// Close the log - I/O dependent
      
      const static unsigned int m_uiMaxLogs = 10;     // Maximum number of logs available
      
		bool m_bActive;      								   // Global debug indicator
		char *m_ppcType[m_uiMaxLogs];      					// Array of log type strings
		bool m_pbActive[m_uiMaxLogs];		      			// Array of log type indicators
		int m_piLevel[m_uiMaxLogs];				      	// Array of log levels
		CLogHandle *m_poHandle[m_uiMaxLogs];		      // Array of log handles
		
	};

}

//#===--- Inline Functions

#ifdef VALET_INLINE
#include "VALET/logmanager_inline.cpp"
#endif // VALET_INLINE

#endif // _VALET_CORE_LOGMANAGER_
