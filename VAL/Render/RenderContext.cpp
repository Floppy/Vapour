//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.cpp - 02/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.cpp,v 1.1 2000/07/24 10:03:06 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "RenderContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////
// CWedgie

CRenderContext::CRenderContext() {
} // Contructor

CRenderContext::~CRenderContext() {
} // Destructor

void CRenderContext::SetOption(int iOption, int iValue) {
} // SetOption (Int)

void CRenderContext::SetOption(int iOption, const char *pcValue) {
} // SetOption (String)

void CRenderContext::GetOption(int iOption, int &iValue) {
} // GetOption (Int)

void CRenderContext::GetOption(int iOption, const char *&pcValue) {
} // GetOption (String)

