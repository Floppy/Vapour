//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 2000 Vapour Technology Ltd.
//
// MFCCmdLine.cpp - 03/12/2000 - Warren Moore
//	  Class to override Windows command line parsing
//
// $Id: MFCCmdLine.cpp,v 1.1 2000/12/04 23:05:15 warren Exp $
//

#include "StdAfx.h"
#include "Wedgie.h"

#include "MFCCmdLine.h"

#include <string.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////
// CMFCCmdLine

CMFCCmdLine::CMFCCmdLine() :
	m_pcInputDir(NULL){
} // Constructor

CMFCCmdLine::~CMFCCmdLine() {
	if (m_pcInputDir) 
		delete m_pcInputDir;
} // Destructor

void CMFCCmdLine::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast) {
	// Only parse one argument
	if (m_pcInputDir)
		return;
	// Create the argument string
	int iLength = strlen(lpszParam);
	if (iLength == 0)
		return;
	NEWBEGIN
	m_pcInputDir = (char*) new char[iLength + 1];
	NEWEND("CMFCCmdLine::ParseParam - Input string")
	if (!m_pcInputDir)
		return;
	strcpy(m_pcInputDir, lpszParam);
	/*
	bool bShowSplash = (m_bShowSplash == TRUE);
	CCommandLineInfo::ParseParam(lpszParam, bFlag, bLast);
	if (stricmp("nosplash", lpszParam) == 0)
		m_bShowSplash = FALSE;
	m_bShowSplash &= (bShowSplash ? TRUE : FALSE);
	*/
} // ParseParam

const char *CMFCCmdLine::GetInputDir() const {
	return m_pcInputDir;
} // GetInputDir
