//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.cpp - 23/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.cpp,v 1.2 2000/07/24 21:10:59 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "RenderContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===--- Defines
#define RC_ID								"BaseRC-23072000"

///////////////////
// CRenderContext

CRenderContext::CRenderContext() {
	m_uWidth = m_uHeight = 0;
	m_uDepth = 0;
	m_bCreated = false;
	m_bEnabled = false;
} // Contructor

CRenderContext::~CRenderContext() {
} // Destructor

const char *CRenderContext::GetID() const {
	return RC_ID;
} // GetID

void CRenderContext::SetOption(int iOption, unsigned int uValue) {
	if (m_bCreated)
		return;
	switch (iOption) {
		case RC_WIDTH:
			m_uWidth = uValue;
			break;
		case RC_HEIGHT:
			m_uHeight = uValue;
			break;
		case RC_DEPTH:
			m_uDepth = uValue;
			break;
		default:
			break;
	}
} // SetOption (Unsigned Int)

void CRenderContext::GetOption(int iOption, unsigned int &uValue) {
	switch (iOption) {
		case RC_WIDTH:
			uValue = m_uWidth;
			break;
		case RC_HEIGHT:
			uValue = m_uHeight;
			break;
		case RC_DEPTH:
			uValue = m_uDepth;
			break;
		default:
			break;
	}
} // GetOption (Unsigned Int)

