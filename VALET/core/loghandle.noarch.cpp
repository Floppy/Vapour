//#========---
//#=- VALET
//#=====---
// Vapour Technology All-Purpose Library
// Copyright 2001 Vapour Technology Ltd.
//
// Add confidentiality notice

////////////////
//! file 		= "VALET/core/loghandle.noarch.cpp"
//! author 		= "Warren Moore"
//! date 		= "23/09/2001"
//! lib 			= libVALETcore
//! rcsid 		= "$Id: loghandle.noarch.cpp,v 1.6 2001/10/26 12:53:06 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogHandle

namespace NVALET {
   
   CLogHandle::CLogHandle(const char *pcType) : 
      m_poLogFile(NULL) {
      using namespace std;
      ASSERT(pcType);
      // Create the log filename
      char pcName[g_uiStrLength] = "";
      strcpy(pcName, pcType);
      strcat(pcName, ".log");
      // Create the log file
      m_poLogFile = new ofstream(pcName, ios::out|ios::app);
      // TODO: Add time and date
      if (m_poLogFile) {
         *m_poLogFile << "---" << endl;
      }
   } // CLogHandle::CLogHandle
   
   CLogHandle::~CLogHandle() {
      // Delete the log file
      if (m_poLogFile) {
         m_poLogFile->close();
         delete m_poLogFile;
      }
   } // CLogHandle::~CLogHandle
   
   void CLogHandle::Trace(int iLevel, const char *pcFunction, const char *pcMessage) {
      using namespace std;
      ASSERT(iLevel >= 0);
      ASSERT(pcMessage);
      if (!m_poLogFile)
         return;
      *m_poLogFile << iLevel << " : " << pcFunction << " - " << pcMessage << endl;
   } // CLogHandle::Trace
   
}
