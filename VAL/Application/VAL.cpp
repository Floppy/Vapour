//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// VAL.cpp - 03/07/2000 - Warren Moore
//	VAL management object for OS independent functions
//
// $Id: VAL.cpp,v 1.3 2000/07/19 08:52:03 waz Exp $
//

#include "StdAfx.h"

#include "VAL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////
// CVAL

CVAL::CVAL() {
	NEWBEGIN
	m_poProgressCtrl = (CProgressControl*) new CProgressControl;
	NEWEND("CVAL::CVAL - Progress control object")
} // Constructor

CVAL::~CVAL() {
	if (m_poProgressCtrl)
		delete m_poProgressCtrl;
} // Destructor

void CVAL::SetProgressTextFunction(const char *pcName, TEXT_FUNC pText) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->SetTextFunction(pcName, pText);
} // SetProgressTextFunction

void CVAL::SetProgressFunction(const char *pcName, HANDLER_FUNC pHandler) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->SetProgressFunction(pcName, pHandler);
} // SetProgressFunction

void CVAL::SetProgressText(const char *pcName, const char *pcText) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->SetText(pcName, pcText);
} // SetProgressText

void CVAL::SetProgressMax(const char *pcName, unsigned int uMax) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->SetMaxProgress(pcName, uMax);
} // SetProgressMax

void CVAL::SetProgressPos(const char *pcName, unsigned int uPercent) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->SetProgress(pcName, uPercent);
} // SetProgressPos

void CVAL::StepProgress(const char *pcName) {
	if (m_poProgressCtrl)
		m_poProgressCtrl->Step(pcName);
} // StepProgress
