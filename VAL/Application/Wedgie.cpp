//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.cpp - 02/07/2000 - Warren Moore
//	Creation and reading of compressed Wedgie files
//
// $Id: Wedgie.cpp,v 1.2 2000/07/02 23:22:36 waz Exp $
//

#include "StdAfx.h"

#include "Wedgie.h"

#include <string.h>
#include "CompressDeflate.h"
#include "ProgressControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CProgressControl g_oProgressControl;

////////////
// CWedgie

CWedgie::CWedgie() : m_cVerHigh(WEDGIE_VER_HIGH), m_cVerLow(WEDGIE_VER_LOW) {
	m_pcBaseDir = NULL;
	m_pcWJEName = NULL;
	m_poFile = NULL;
	m_bCreate = false;
	m_iFiles = 0;
} // Constructor

CWedgie::~CWedgie() {
} // Destructor

WJERESULT CWedgie::Open(fstream *pFile, const char *pDir, bool bCreate) {
	// If a wedgie is open, close it ignoring the return
	if (m_poFile)
		Close();

	// Count the file
	return WJE_OK;
} // Open

WJERESULT CWedgie::Close() {
	return WJE_OK;
} // Close

const char *CWedgie::Directory() const {
	return m_pcBaseDir;
} // Directory

const char *CWedgie::Name() const {
	return m_pcWJEName;
} // Name

int CWedgie::Files() const {
	return m_iFiles;
} // Files

int CWedgie::Count() {
	return 0;
} // Count
