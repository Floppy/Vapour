//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

#ifndef _VALET_LOGMANAGER_NOARCH_
#define _VALET_LOGMANAGER_NOARCH_

////////////////
//! file 		= "VALET/core/logmanager.h"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: logmanager.h,v 1.4 2001/10/17 00:22:47 vap-warren Exp $"
//! userlevel 	= Develop
//! docentry 	= "VALET.Core.Log"
//! example 	= VALET/core/log.test.cpp

//#===--- Includes
#include "valet.h"

namespace NValet {

	//#===--- Predeclared Classes
	class CLogHandle;

	//#===--- CLogManager
	//: Central manager for log output
	// Object that controls the output of CLog objects
	// Should only be one global log manager per application
	//!classtodo: Move arrays to dynamically allocated list? (perf. worries)

	#define LMGR_MAX 10
	/* Maximum number of available log streams*/

	class CLogManager {
	public:
		CLogManager(bool bLog = false);
      //: Constructor
		//!param: bLog = Global logging option

		~CLogManager();
      //: Destructor

		//:------
		//: Log management
		
		void SetLog(const char *pcType, bool bLog, int iLevel = -1);
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

		void Trace(int iID, const char *pcMessage, int iLevel = 0);
		// Outputs to the log
		//!param: iID = Log type ID
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

		bool m_bActive;								// Global debug indicator
		char *m_ppcType[LMGR_MAX];					// Array of log type strings
		bool m_pbActive[LMGR_MAX];					// Array of log type indicators
		int m_piLevel[LMGR_MAX];					// Array of log levels
		CLogHandle *m_poHandle[LMGR_MAX];		// Array of log handles
		
	};

}

//#===--- Inline Functions

#ifdef VALET_INLINE
#include "logmanager_inline.cpp"
#endif // VALET_INLINE

#endif // _VALET_LOGMANAGER_NOARCH_
