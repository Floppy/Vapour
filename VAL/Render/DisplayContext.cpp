//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// DisplayContext.cpp - 23/11/2000 - Warren Moore
//	Base class for display contexts. Used to retrieve information from a display
//
// $Id: DisplayContext.cpp,v 1.1 2000/11/25 11:30:37 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "DisplayContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////
// CDisplayContext

const char *CDisplayContext::m_pcErrorString[] = {
	DC_ERROR_STRINGS
};


CDisplayContext::CDiplayContext() {
} // Contructor

CDisplayContext::~CDisplayContext() {
} // Destructor

DCRESULT CDisplayContext::GetDisplayPointer(const int iValue, void *&pData) const {
	return DC_UNSUPPORTED_VALUE;
} // GetDisplayPointer

