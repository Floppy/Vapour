//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContext.cpp - 23/07/2000 - Warren Moore
//	Base class for render contexts
//
// $Id: RenderContext.cpp,v 1.3 2000/07/30 14:57:56 waz Exp $
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

const char *CRenderContext::m_pcErrorString[] = {
	RC_ERROR_STRINGS
};

CRenderContext::CRenderContext() {
	// No default values
	m_uWidth = m_uHeight = 0;
	m_uDepth = 0;
	m_fNearPlane = m_fFarPlane = m_fViewAngle = 0.0F;
	// Initial clear colour black
	m_fBackRed = m_fBackGreen = m_fBackBlue = 0.0F;
	// Initial projection mode
	m_uProjMode = RCP_PERSPECTIVE;
	// Set the initial context state
	m_bCreated = false;
	m_bEnabled = false;
} // Contructor

CRenderContext::~CRenderContext() {
} // Destructor

const char *CRenderContext::GetID() const {
	return RC_ID;
} // GetID

RCRESULT CRenderContext::SetSize(unsigned int uWidth, unsigned int uHeight) {
	m_uWidth = uWidth;
	m_uHeight = uHeight;
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
		case RCO_DEPTH:
			// Allow standard colour depths
			if ((uValue == 8) || (uValue == 16) || (uValue == 24) || (uValue == 32))
				m_uDepth = uValue;
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
			eResult = RC_NO_OPTION;
	}
	return eResult;
} // SetOption (Int)

RCRESULT CRenderContext::GetOption(int iOption, unsigned int &uValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_DEPTH:
			uValue = m_uDepth;
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
			eResult = RC_NO_OPTION;
	}
	return eResult;
} // GetOption (Int)

RCRESULT CRenderContext::SetOption(int iOption, float fValue) {
	RCRESULT eResult = RC_OK;
	switch (iOption) {
		case RCO_DEPTH:
			// Allow standard colour depths
			if ((fValue == 8.0F) || (fValue == 16.0F) || (fValue == 24.0F) || (fValue == 32.0F))
				m_uDepth = (unsigned int)fValue;
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
			eResult = RC_NO_OPTION;
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
			eResult = RC_NO_OPTION;
	}
	return eResult;
} // GetOption (Float)

