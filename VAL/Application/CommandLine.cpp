//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// CommandLine.cpp - 01/06/2000 - Warren Moore
//	Command line parsing class 
//
// $Id: CommandLine.cpp,v 1.1 2000/06/16 21:57:49 waz Exp $
//

#include "StdAfx.h"

#include "CommandLine.h"

#include "Defs.h"
#include <string.h>

/////////////////
// CCommandLine

CCommandLine::CCommandLine(int iCount, char **pcData) {
	m_iCount = iCount;
	m_pcData = pcData;
} // Contructor

CCommandLine::~CCommandLine() {
} // Destructor

const char *CCommandLine::GetValue(const char *pcOption) {
	char *pcValue = NULL;
	if (pcOption && (m_iCount > 2)) {
		int iCount = 1;
		while ((!pcValue) && (iCount < m_iCount - 1)) {
			if ((m_pcData[iCount][0] == '-') && (stricmp(m_pcData[iCount] + 1, pcOption) == 0)) {
				pcValue = m_pcData[iCount + 1];
			}
			iCount++;
		}
	}
	return pcValue;
} // GetValue (Option)

const char *CCommandLine::GetValue(const int iPos) {
	if (iPos >= m_iCount)
		return NULL;
	return m_pcData[iPos];
} // GetValue (Position)

const bool CCommandLine::FindOption(const char *pcOption) {
	bool bFound = false;
	if (pcOption && (m_iCount > 1)) {
		int iCount = 1;
		while ((!bFound) && (iCount < m_iCount)) {
			if ((m_pcData[iCount][0] == '-') && (stricmp(m_pcData[iCount] + 1, pcOption) == 0)) {
				bFound = true;
			}
			iCount++;
		}
	}
	return bFound;
} // FindOption