//======---
// Saver
//------
// Vapour Technology Screen Saver
// Copyright 2000 Vapour Technology Ltd.
//
// SaverSettings.cpp - 26/11/2000 - Warren Moore
//	  Stores screen saver settings
//
// $Id: SaverSettings.cpp,v 1.3 2000/12/03 13:24:00 warren Exp $
//

#include "VAL.h"
#include "SaverSettings.h"

#include "Registry.h"

#include <math.h>
#include <fstream.h>

//#===--- Defines

#define SAVER_KEY (REGSTR_PATH_SETUP"\\Screen Savers")

///////////////////
// CSaverSettings

CSaverSettings::CSaverSettings() : 
	m_hInstance(NULL),
	m_hWND(NULL),
	m_hDC(NULL),
	m_eMode(SA_NONE),
	m_uPassDelay(10),
	m_uStartTime(0),
	m_uTimerID(0),
	m_bDialog(false),
	m_iMoveThresh(5),
	m_bDisplaySet(false),
	m_bClose(false) {
	// Set the initial mouse pos
	m_sPos.x = 0;
	m_sPos.y = 0;
	// Set the new display settings
	memset(&m_sNewMode, 0, sizeof(m_sNewMode));
	m_sNewMode.dmSize = sizeof(m_sNewMode);
	m_sNewMode.dmDriverExtra = 0;
	m_sNewMode.dmBitsPerPel = 16;
	m_sNewMode.dmPelsWidth = 640;
	m_sNewMode.dmPelsHeight = 480;
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
	m_uTimerID = SetTimer(m_hWND, 0, 40, NULL);
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

void CSaverSettings::SetDisplay(bool bSaverMode) {
	// If the display is being set to the mode it's currently in, leave immediately
	if (m_bDisplaySet == bSaverMode)
		return;
	// If the display is being set
	if (bSaverMode) {
		// Save the original mode, leaving if we can't
		if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &m_sDevMode) == 0)
			return;
		// Test the new settings
		m_sNewMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		LONG lResult = ChangeDisplaySettings(&m_sNewMode, CDS_TEST);
		// If ok, set the display
		if (lResult == DISP_CHANGE_SUCCESSFUL) {
			lResult = ChangeDisplaySettings(&m_sNewMode, CDS_FULLSCREEN);
			if (lResult == DISP_CHANGE_SUCCESSFUL)
				m_bDisplaySet = true;
		}
		// Save the start time
		m_uDisplayTime = GetTickCount();
	}
	else {
		// Set the display mode back
		m_sDevMode.dmSize = sizeof(m_sDevMode);
		m_sDevMode.dmDriverExtra = 0;
		m_sDevMode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		ChangeDisplaySettings(&m_sDevMode, CDS_RESET);
		m_bDisplaySet = false;
	}
} // SetDisplay

bool CSaverSettings::MouseStable() {
	// Don't bother if the display isn't set
	if (!m_bDisplaySet)
		return true;
	// If it is, check whether enough time has passed
	return ((GetTickCount() - m_uDisplayTime) > 2000);
} // MouseStable

void CSaverSettings::GetSize(int &iWidth, int &iHeight, int &iWidthOffset, int &iHeightOffset) {
	if (!m_bDisplaySet) {
		RECT sRect;
		GetClientRect(m_hWND, &sRect); 
		iWidth = sRect.right;
		iHeight = sRect.bottom;
		iWidthOffset = 0;
		iHeightOffset = 0;
	}
	else {
		iWidth = m_sNewMode.dmPelsWidth;
		iHeight = m_sNewMode.dmPelsHeight;
		iWidthOffset = 0;
		iHeightOffset = m_sDevMode.dmPelsHeight - iHeight;
	}
} // GetSize
