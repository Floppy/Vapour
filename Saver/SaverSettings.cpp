//======---
// Saver
//------
// Vapour Technology Screen Saver
// Copyright 2000 Vapour Technology Ltd.
//
// SaverSettings.cpp - 26/11/2000 - Warren Moore
//	  Stores screen saver settings
//
// $Id: SaverSettings.cpp,v 1.2 2000/11/30 11:05:29 warren Exp $
//

#include "VAL.h"
#include "SaverSettings.h"

#include "Registry.h"

#include <math.h>

//#===--- Defines

#define SAVER_KEY (REGSTR_PATH_SETUP"\\Screen Savers")

///////////////////
// CSaverSettings

CSaverSettings::CSaverSettings() : 
	m_hInstance(NULL),
	m_hWND(NULL),
	m_hDC(NULL),
	m_eMode(SA_NONE),
	m_uPassDelay(15),
	m_uStartTime(0),
	m_uTimerID(0),
	m_bDialog(false),
	m_iMoveThresh(50),
	m_bClose(false) {
	m_sPos.x = 0;
	m_sPos.y = 0;
} // Contructor

CSaverSettings::~CSaverSettings() {
} // Destructor

void CSaverSettings::ParseCommandLine(char *pcLine) {
	// TODO: Rewrite this parser (not ours!)
	if (*pcLine == '\"') {
		pcLine++;
		while (*pcLine != 0 && *pcLine != '\"')
			pcLine++;
	}
	else {
		while (*pcLine != 0 && *pcLine != ' ')
			pcLine++;
	}
	if (*pcLine != 0)
		pcLine++;
	while (*pcLine == ' ')
		pcLine++;
	m_hWND = NULL;
	if (*pcLine == 0) {
		m_eMode = SA_CONFIG;
		m_hWND = NULL;
	}
	else {
		if (*pcLine == '-' || *pcLine == '/')
			pcLine++;
		if (*pcLine == 'p' || *pcLine == 'P' || *pcLine == 'l' || *pcLine == 'L') {
			pcLine++;
			while (*pcLine == ' ' || *pcLine == ':')
				pcLine++;
#ifdef _DEBUG
			if ((strcmp(pcLine,"scrprev") == 0) || (strcmp(pcLine,"ScrPrev") == 0) || (strcmp(pcLine,"SCRPREV") == 0))
				m_hWND = CheckForScrprev();
			else
				m_hWND = (HWND)atoi(pcLine);
#else
			m_hWND = (HWND)atoi(pcLine);
#endif // _DEBUG
			m_eMode = SA_PREVIEW;
		}
		else
			if (*pcLine == 's' || *pcLine == 'S') {
				m_eMode = SA_SAVER;
			}
			else
				if (*pcLine == 'c' || *pcLine == 'c') {
					pcLine++;
					while (*pcLine == ' ' || *pcLine == ':')
						pcLine++;
					if (*pcLine == 0)
						m_hWND = GetForegroundWindow();
					else
						m_hWND = (HWND)atoi(pcLine);
					m_eMode = SA_CONFIG;
				}
				else
					if (*pcLine == 'a' || *pcLine == 'A') {
						pcLine++;
						while (*pcLine == ' ' || *pcLine == ':')
							pcLine++;
						m_hWND = (HWND)atoi(pcLine);
						m_eMode = SA_PASSWORD;
					}
				}
} // ParseCommandLine

void CSaverSettings::GetSettings() {
	CRegistry oReg;
	// Open the screen saver key
	if (oReg.OpenKey(CURRENT_USER, SAVER_KEY, true)) {
		m_uPassDelay = oReg.ReadInt("Password Delay", m_uPassDelay);
		m_iMoveThresh = oReg.ReadInt("Mouse Threshold", m_iMoveThresh);
//		m_bMute = (oReg.ReadInt("Mute Sound", m_bMute ? 1 : 0) != 0);
		oReg.CloseKey();
	}

} // GetSettings

void CSaverSettings::StoreSettings() {
} // StoreSettings

void CSaverSettings::StartTimer() {
	// Get the start time and start the timer
	m_uStartTime = GetTickCount();
	m_uTimerID = SetTimer(m_hWND, 0, 100, NULL);
} // StartTimer

bool CSaverSettings::CheckPasswordDelay() {
	return ((GetTickCount() - m_uStartTime) > (1000 * m_uPassDelay));
} // CheckPasswordDelay

void CSaverSettings::StopTimer() {
	if (m_uTimerID != 0)
		KillTimer(m_hWND, m_uTimerID);
	m_uTimerID = 0;
} // StopTimer

void CSaverSettings::DialogActive(bool bActive) {
	if (bActive != m_bDialog)
		SendMessage(m_hWND, WM_SETCURSOR, 0, 0);
	m_bDialog = bActive;
} // DialogActive

void CSaverSettings::SaveCursor() {
	GetCursorPos(&m_sPos);
} // SaveCursor

bool CSaverSettings::MovedEnough() {
	POINT sPos;
	GetCursorPos(&sPos);
	int iXDiff = abs(sPos.x - m_sPos.x);
	int iYDiff = abs(sPos.y - m_sPos.y);
	return ((iXDiff > m_iMoveThresh) || (iYDiff > m_iMoveThresh));
} // MovedEnough

