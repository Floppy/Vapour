//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 2000 Vapour Technology Ltd.
//
// MFCCmdLine.h - 03/12/2000 - Warren Moore
//	  Class to override Windows command line parsing
//
// $Id: MFCCmdLine.h,v 1.1 2000/12/04 23:05:15 warren Exp $
//

#ifndef _WEDGIE_MFCCMDLINE_
#define _WEDGIE_MFCCMDLINE_

#pragma once

////////////////
// CMFCCmdLine

class CMFCCmdLine : public CCommandLineInfo {
public:
	//#===--- External Functions
	CMFCCmdLine();
	virtual ~CMFCCmdLine();

	// Overridden command line parser functions
	virtual void ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast);

	//#===--- Data retrieval
	// Returns the directory name passed
	const char *GetInputDir() const;

protected:
	//#===--- Internal data
	char *m_pcInputDir;
};

#endif // _WEDGIE_MFCCMDLINE_
