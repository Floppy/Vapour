//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.cpp - 23/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.cpp,v 1.6 2000/11/25 11:30:37 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "RenderContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////
// CRenderContext

const char *CRenderContext::m_pcErrorString[] = {
	RC_ERROR_STRINGS
};

CRenderContext::CRenderContext(const CDisplayContext *poDisplay) :
	// No default values
	m_uWidth(0),
	m_uHeight(0),
	m_uNewWidth(0),
	m_uNewHeight(0),
	m_uColourDepth(0),
	m_uZBuffer(0),
	m_fNearPlane(0.0F),
	m_fFarPlane(0.0F),
	m_fViewAngle(0.0F),
	// Initial clear colour black
	m_fBackRed(0.0F),
	m_fBackGreen(0.0F),
	m_fBackBlue(0.0F),
	// Initial projection mode
	m_uProjMode(RCP_PERSPECTIVE),
	// Set the initial context state
	m_bCreated(false),
	m_bEnabled(false),
	// Display context
	m_poDisplay(poDisplay) {
} // Contructor

CRenderContext::~CRenderContext() {
} // Destructor

RCRESULT CRenderContext::SetSize(unsigned int uWidth, unsigned int uHeight) {
	m_uNewWidth = uWidth;
	m_uNewHeight = uHeight;
	return RC_OK;
} // SetSize

RCRESULT CRenderContext::GetSize(unsigned int &uWidth, unsigned int &uHeight) {
	uWidth = m_uWidth;
	uHeight = m_uHeight;
	return RC_OK;
} // GetSize

RCRESULT CRenderContext::SetOption(int iOption, unsigned int uValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_WIDTH:
			m_uWidth = uValue;
			break;
		case RCO_HEIGHT:
			m_uHeight = uValue;
			break;
		case RCO_COL_DEPTH:
			// Allow standard colour depths
			if ((uValue == 8) || (uValue == 16) || (uValue == 24) || (uValue == 32))
				m_uColourDepth = uValue;
			break;
		case RCO_Z_BUF_SIZE:
			// Allow n^2 z buffer sizes
			if ((uValue == 8) || (uValue == 16) || (uValue == 24) || (uValue == 32))
				m_uZBuffer = uValue;
			break;
		case RCO_NEARPLANE:
			m_fNearPlane = (float)uValue;
			break;
		case RCO_FARPLANE:
			m_fFarPlane = (float)uValue;
			break;
		case RCO_VIEWANGLE:
			m_fViewAngle = (float)uValue;
			break;
		case RCO_BACKRED:
			// Assume value between 0 and 255 if int supplied
			m_fBackRed = 255.0F / ((float)(uValue & 0x000000ff));
			break;
		case RCO_BACKGREEN:
			// Assume value between 0 and 255 if int supplied
			m_fBackGreen = 255.0F / ((float)(uValue & 0x000000ff));
			break;
		case RCO_BACKBLUE:
			// Assume value between 0 and 255 if int supplied
			m_fBackBlue = 255.0F / ((float)(uValue & 0x000000ff));
			break;
		default:
			eResult = RC_UNKNOWN_OPTION;
	}
	return eResult;
} // SetOption (Int)

RCRESULT CRenderContext::GetOption(int iOption, unsigned int &uValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_WIDTH:
			uValue = m_uWidth;
			break;
		case RCO_HEIGHT:
			uValue = m_uHeight;
			break;
		case RCO_COL_DEPTH:
			uValue = m_uColourDepth;
			break;
		case RCO_Z_BUF_SIZE:
			uValue = m_uZBuffer;
			break;
		case RCO_BACKRED:
			// Return value between 0 and 255 if int supplied
			uValue = 255 * (unsigned int)m_fBackRed;
			break;
		case RCO_BACKGREEN:
			// Return value between 0 and 255 if int supplied
			uValue = 255 * (unsigned int)m_fBackGreen;
			break;
		case RCO_BACKBLUE:
			// Return value between 0 and 255 if int supplied
			uValue = 255 * (unsigned int)m_fBackBlue;
			break;
		default:
			eResult = RC_UNKNOWN_OPTION;
	}
	return eResult;
} // GetOption (Int)

RCRESULT CRenderContext::SetOption(int iOption, float fValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_COL_DEPTH:
			// Allow standard colour depths
			if ((fValue == 8.0F) || (fValue == 16.0F) || (fValue == 24.0F) || (fValue == 32.0F))
				m_uColourDepth = (unsigned int)fValue;
			break;
		case RCO_Z_BUF_SIZE:
			// Allow n^2 z buffer sizes
			if ((fValue == 8.0F) || (fValue == 16.0F) || (fValue == 24.0F) || (fValue == 32.0F))
				m_uZBuffer = (unsigned int)fValue;
			break;
		case RCO_NEARPLANE:
			m_fNearPlane = fValue;
			break;
		case RCO_FARPLANE:
			m_fFarPlane = fValue;
			break;
		case RCO_VIEWANGLE:
			m_fViewAngle = fValue;
			break;
		case RCO_BACKRED:
			// Clip value between 0 and 1 if float supplied
			fValue = (fValue > 1.0F) ? 1.0F : (fValue < 0.0F) ? 0.0F : fValue;
			m_fBackRed = fValue;
			break;
		case RCO_BACKGREEN:
			// Clip value between 0 and 1 if float supplied
			fValue = (fValue > 1.0F) ? 1.0F : (fValue < 0.0F) ? 0.0F : fValue;
			m_fBackGreen = fValue;
			break;
		case RCO_BACKBLUE:
			// Clip value between 0 and 1 if float supplied
			fValue = (fValue > 1.0F) ? 1.0F : (fValue < 0.0F) ? 0.0F : fValue;
			m_fBackBlue = fValue;
			break;
		default:
			eResult = RC_UNKNOWN_OPTION;
	}
	return eResult;
} // SetOption (Float)

RCRESULT CRenderContext::GetOption(int iOption, float &fValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_NEARPLANE:
			fValue = m_fNearPlane;
			break;
		case RCO_FARPLANE:
			fValue = m_fFarPlane;
			break;
		case RCO_VIEWANGLE:
			fValue = m_fViewAngle;
			break;
		case RCO_BACKRED:
			// Return value between 0 and 1 if float supplied
			fValue = m_fBackRed;
			break;
		case RCO_BACKGREEN:
			// Return value between 0 and 1 if float supplied
			fValue = m_fBackGreen;
			break;
		case RCO_BACKBLUE:
			// Return value between 0 and 1 if float supplied
			fValue = m_fBackBlue;
			break;
		default:
			eResult = RC_UNKNOWN_OPTION;
	}
	return eResult;
} // GetOption (Float)

