//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Wedgie.cpp - 02/07/2000 - Warren Moore
//	Wedgie file class implemenation
//
// $Id: Wedgie.cpp,v 1.1 2000/07/02 22:59:50 waz Exp $
//

#include "StdAfx.h"

#include "Wedgie.h"

////////////
// CWedgie

CWedgie();
virtual ~CWedgie();

WJERESULT Open(fstream *pFile, bool bCreate = false);

WJERESULT Close();

void Directory(const char *pDir);

const char *Directory();

int Files() const;

char *m_pBaseDir;					// Base directory
char *m_pWJEName;					// Wedgie filename
int m_iFiles;							// Number of files in wedgie

