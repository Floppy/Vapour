//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CGI.cpp - 12/12/2000 - Warren Moore
//	  CGI utility class
//
// $Id: CGI.cpp,v 1.1 2000/12/14 14:11:38 warren Exp $
//

#include "CGI.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#ifndef WIN32
#define stricmp					strcasecmp
#define strnicmp					strncasecmp
#endif // WIN32

/////////
// CCGI

CCGI::CCGI() :
	m_uRequestType(CGI_M_UNKNOWN),
	m_bDebug(false) {
	// Make sure all the pointers are NULL (SPair values set by constructor)
	memset(m_pcValidRef, 0, sizeof(char*) * CGI_MAX_REFERRERS);
} // Constructor

CCGI::~CCGI() {
	// Delete all the referrer strings
	int iCount = 0;
	while ((iCount < CGI_MAX_REFERRERS) && m_pcValidRef[iCount]) {
		delete [] m_pcValidRef[iCount];
		iCount++;
	}
	// Delete all the data pairs
	iCount = 0;
	while ((iCount < CGI_MAX_PAIRS) && m_pcData[iCount].m_pcID) {
		delete [] m_pcData[iCount].m_pcID;
		if (m_pcData[iCount].m_pcValue)
			delete [] m_pcData[iCount].m_pcValue;
		iCount++;
	}
} // Destructor

void CCGI::Init() {
	// Get the request method
	GetRequestMethod();
	// Process usage dependent on request type
	switch(m_uRequestType) {
		case CGI_M_GET:
		case CGI_M_POST:
			ParseQuery();
			break;
		case CGI_M_UNKNOWN:
		case CGI_M_NONE:
		default:
			return;
	}
} // Init (Server only)

void CCGI::Init(int iArgc, char **ppcArgv) {
	// Get the request method
	GetRequestMethod();
	// Process usage dependent on request type
	switch(m_uRequestType) {
		case CGI_M_GET:
		case CGI_M_POST:
			ParseQuery();
			break;
		case CGI_M_NONE:
			ParseCommandLine(iArgc, ppcArgv);
			break;
		case CGI_M_UNKNOWN:
		default:
			return;
	}
} // Init (Command Line)

void CCGI::AddReferrer(const char *pcName) {
	// Check we have a srting
	if (!pcName)
		return;
	// Find a free referrer string
	int iCount = 0;
	while ((iCount < CGI_MAX_REFERRERS) && m_pcValidRef[iCount])
		iCount++;
	if (iCount == CGI_MAX_REFERRERS)
		return;
	// Create and copy the string
	m_pcValidRef[iCount] = (char*) new char[strlen(pcName) + 1];
	if (m_pcValidRef[iCount])
		strcpy(m_pcValidRef[iCount], pcName);
} // AddReferrer

bool CCGI::ValidReferrer() const {
	// All ok if in debug mode
	if (m_bDebug)
		return true;
	// Check all valid referrer names
	char *pcName = getenv("HTTP_REFERER");
	if (!pcName)
		return true;
	int iCount = 0;
	bool bFound = false;
	while ((iCount < CGI_MAX_REFERRERS) && m_pcValidRef[iCount]) {
		if (strnicmp(m_pcValidRef[iCount], pcName, strlen(m_pcValidRef[iCount])) == 0) {
			bFound = true;
			break;
		}
		iCount++;
	}
	return bFound;
} // ValidReferrer

const char *CCGI::GetValue(const char *pcID) const {
	int iCount = 0;
	const char *pcReturn = NULL;
	while ((iCount < CGI_MAX_PAIRS) && m_pcData[iCount].m_pcID) {
		if (stricmp(pcID, m_pcData[iCount].m_pcID) == 0) {
			pcReturn = m_pcData[iCount].m_pcValue;
			break;
		}
		iCount++;
	}
	return pcReturn;
} // GetValue

void CCGI::GetRequestMethod() {
	char *pcMethod = getenv("REQUEST_METHOD");
	// Make sure we have a valid request method, assuming command line execution if not
	if (!pcMethod) {
		m_uRequestType = CGI_M_NONE;
		m_bDebug = true;
		return;
	}
	// Check for each method
	if (stricmp(pcMethod, "GET") == 0)
		m_uRequestType = CGI_M_GET;
	if (stricmp(pcMethod, "POST") == 0)
		m_uRequestType = CGI_M_POST;
} // GetRequestMethod

bool CCGI::CorrectRequest(unsigned int uMethod) const {
	return ((m_uRequestType & uMethod) != 0);
} // CorrectRequest

bool CCGI::Debug() const {
	return m_bDebug;
} // Debug

void CCGI::ParseQuery() {
	// Check we have a valid request type
	if ((m_uRequestType & (CGI_M_UNKNOWN | CGI_M_NONE)) != 0)
		return;
	// Get the query string
	char *pcQuery = NULL;
	char *pcBuffer = NULL;
	char *pcTemp = NULL;
	bool bUrlEncoded = false;
	int iLen = 0;
	switch (m_uRequestType) {
		case CGI_M_GET:
			// Get the query string
			pcQuery = getenv("QUERY_STRING");
			bUrlEncoded = true;
			break;
		case CGI_M_POST:
			// Get the length of the query string
			pcTemp = getenv("CONTENT_LENGTH");
			if (pcTemp) {
				iLen = atoi(pcTemp);
			}
			// If set, get the query string
			if (iLen > 0) {
				pcBuffer = (char*) new char[iLen + 1];
				if (pcBuffer) {
					fread(pcBuffer, 1, iLen, stdin);
					pcBuffer[iLen] = '\0';
					pcQuery = pcBuffer;
				}
			}
			// Check the content type
			if (pcQuery) {
				pcTemp = getenv("CONTENT_TYPE");
				if (pcTemp) {
					if (stricmp(pcTemp, "application/x-www-form-urlencoded") == 0)
						bUrlEncoded = true;
				}
			}
			break;
		default:
			break;
	}
	// Check we now have a query string
	if (!(pcQuery && bUrlEncoded))
		return;
	// Find the first free data pair
	int iCount = 0;
	while ((iCount < CGI_MAX_PAIRS) && m_pcData[iCount].m_pcID)
		iCount++;
	if (iCount == CGI_MAX_PAIRS)
		return;
	// Parse the query string
	char *pcStart = pcQuery;
	char *pcPos = pcStart;
	char *pcDiv = pcPos;
	char *pcData = NULL;
	// Go through changing '+'s to ' 's
	while (*pcPos) {
		if (*pcPos == '+')
			*pcPos = ' ';
		pcPos++;
	}
	pcPos = pcStart;
	// Now parse
	bool bDone = false;
	while (!bDone) {
		// Have we hit the end of a data pair
		if ((*pcPos == '\0') || (*pcPos == '&')) {
			// Process single value
			if (pcStart == pcDiv) {
				int iLen = pcPos - pcStart;
				pcData = (char*) new char[iLen + 1];
				if (pcData) {
					memcpy(pcData, pcStart, iLen);
					pcData[iLen] = '\0';
					DecodeHex(pcData);
					m_pcData[iCount].m_pcID = pcData;
					iCount++;
				}
			}
			// Parse value pair
			else {
				// Copy out ID
				int iLen = pcDiv - pcStart;
				pcData = (char*) new char[iLen + 1];
				if (pcData) {
					memcpy(pcData, pcStart, iLen);
					pcData[iLen] = '\0';
					DecodeHex(pcData);
					m_pcData[iCount].m_pcID = pcData;
				}
				// Copy out value
				iLen = pcPos - pcDiv - 1;
				pcData = (char*) new char[iLen + 1];
				if (pcData) {
					memcpy(pcData, pcDiv + 1, iLen);
					pcData[iLen] = '\0';
					DecodeHex(pcData);
					m_pcData[iCount].m_pcValue = pcData;
					iCount++;
				}
			}
			// Prepare for the next one
			if (*pcPos == '\0')
				bDone = true;
			else {
				pcPos++;
				pcStart = pcDiv = pcPos;
			}
		}
		// Otherwise, just scan through
		else {
			if (*pcPos == '=')
				pcDiv = pcPos;
			pcPos++;
		}
	}
	// Delete allocated memory
	if (pcBuffer)
		delete [] pcBuffer;
} // ParseQuery

void CCGI::ParseCommandLine(int iArgc, char **ppcArgv) {
	// Check we have some arguments
	if (iArgc == 1)
		return;
	// Find the first free data pair
	int iCount = 0;
	while ((iCount < CGI_MAX_PAIRS) && m_pcData[iCount].m_pcID)
		iCount++;
	if (iCount == CGI_MAX_PAIRS)
		return;
	// Parse each argument
	int iArgNum = 1;
	while (iArgNum < iArgc) {
		// Go through string and look for '='
		const char *pcStart = ppcArgv[iArgNum];
		const char *pcPos = pcStart;
		const char *pcDiv = pcPos;
		while (*pcPos) {
			if (*pcPos == '=')
				pcDiv = pcPos;
			pcPos++;
		}
		// Process single value
		if (pcStart == pcDiv) {
			int iLen = strlen(pcStart);
			char *pcData = (char*) new char[iLen + 1];
			if (pcData) {
				strcpy(pcData, pcStart);
				m_pcData[iCount].m_pcID = pcData;
				if (m_bDebug)
					printf("Value: %s\n", pcData);
				iCount++;
			}
		}
		// Parse value pair
		else {
			// Copy out ID
			int iLen = pcDiv - pcStart;
			char *pcData = (char*) new char[iLen + 1];
			if (pcData) {
				memcpy(pcData, pcStart, iLen);
				pcData[iLen] = '\0';
				m_pcData[iCount].m_pcID = pcData;
			}
			// Copy out value
			iLen = pcPos - pcDiv - 1;
			pcData = (char*) new char[iLen + 1];
			if (pcData) {
				memcpy(pcData, pcDiv + 1, iLen);
				pcData[iLen] = '\0';
				m_pcData[iCount].m_pcValue = pcData;
				if (m_bDebug)
					printf ("Value: %s = %s\n", m_pcData[iCount].m_pcID, m_pcData[iCount].m_pcValue);
				iCount++;
			}
		}
		// Next argument
		iArgNum++;
	}
} // ParseCommandLine

char CCGI::GetChar(char *pcIn) {
    char cDigit;
    // NOTE: the hex 0cDF takes care of uppercasing in ASCII
    cDigit = (pcIn[0] >= 'A' ? ((pcIn[0] & 0xDF) - 'A')+10 : (pcIn[0] - '0'));
    cDigit *= 16;
    cDigit += (pcIn[1] >= 'A' ? ((pcIn[1] & 0xDF) - 'A')+10 : (pcIn[1] - '0'));
    return(cDigit);
} // GetChar

void CCGI::DecodeHex(char *pcIn) {
    int i, j;
    for(i = 0, j = 0; pcIn[j]; ++i, ++j) {
        if((pcIn[i] = pcIn[j]) == '%') {
            pcIn[i] = GetChar(&pcIn[j + 1]);
            j += 2;
        }
    }
    pcIn[i] = '\0';
} // DecodeHex
