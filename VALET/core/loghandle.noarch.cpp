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
//! rcsid 		= "$Id: loghandle.noarch.cpp,v 1.7 2001/10/27 00:02:17 vap-warren Exp $"

//#===--- Includes
#include "loghandle.h"

#include <stdio.h>
#include <string.h>

//#===--- CLogHandle

namespace NVALET {
   
   ofstream * CLogHandle::m_poLogFile = NULL;
   unsigned int CLogHandle::m_uiFileCount = 0;
   
   CLogHandle::CLogHandle(const char *pcType) {
      ASSERT(pcType);
		// Store the log type
		strcpy(m_pcType, pcType);
      // Check to see if the log file's been open
      if (!m_poLogFile) {
         // Open the log file
         m_poLogFile = new ofstream("test.log", ios::out|ios::app);
         if (m_poLogFile) {
            *m_poLogFile << "---" << endl;
         }
      }
      // Update the count
      m_uiFileCount++;
   } // CLogHandle::CLogHandle
   
   CLogHandle::~CLogHandle() {
      // Decrement the count
      m_uiFileCount--;
      // Delete the log file
      if (m_uiFileCount == 0 && m_poLogFile) {
         m_poLogFile->close();
         delete m_poLogFile;
      }
   } // CLogHandle::~CLogHandle
   
   void CLogHandle::Trace(int iLevel, const char *pcFunction, const char *pcMessage) {
      ASSERT(iLevel >= 0);
      ASSERT(pcMessage);
      if (!m_poLogFile)
         return;
      // Write the log message
      *m_poLogFile << "(" << m_pcType << ") " << iLevel << " : ";
      *m_poLogFile << pcFunction << " - " << pcMessage << endl;
   } // CLogHandle::Trace
   
}
