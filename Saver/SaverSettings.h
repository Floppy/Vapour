//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// SaverSettings.h - 26/11/2000 - Warren Moore
//	  Stores screen saver settings
//
// $Id: SaverSettings.h,v 1.1 2000/11/27 17:17:01 cvs Exp $
//

#ifndef _SAVER_SETTINGS_
#define _SAVER_SETTINGS_

#pragma once

#include "VAL.h"

#include "Saver.h"
#include "DisplayContext.h"

//#===--- Data types

enum SAMODE {
	SA_NONE,
	SA_CONFIG,
	SA_PASSWORD,
	SA_PREVIEW,
	SA_SAVER
};

//#===--- Defines

//#===--- External linkage dependencies

///////////////////
// CSaverSettings

class CSaverSettings : public CDisplayContext {
public:
	// Constructor
	CSaverSettings();
	// Destructor
	~CSaverSettings();

//#===--- External Functions

	// Parses the command line arguments and determines operating mode and HWND
	void ParseCommandLine(char *pcLine);
	// Retrieves the saver operating mode
	SAMODE GetMode() const;

	// Sets the application instance
	void SetInstance(HINSTANCE hInstance);
	// Returns the application instance
	HINSTANCE GetInstance() const;

	// Sets the window handle
	void SetHWND(HWND hWND);
	// Returns the specified HWND
	HWND GetHWND() const;

	// Reads global and local settings from the registry
	void GetSettings();
	// Writes local settings to the registry
	void StoreSettings();

	// Starts the windows timer
	void StartTimer();
	// Checks whether the password delay timer has been exceeded
	bool CheckPasswordDelay();
	// Stops the timer
	void StopTimer();

	// Sets whether the dialog is active or not, enabling or disabling the cursor
	void DialogActive(bool bActive);
	// Returns the dialog status
	bool DialogActive() const;

	// Saves the cursor position
	void SaveCursor();
	// Check whether the threshold has been exceeded
	bool MovedEnough();

	// Indicate we have called close, and post close message
	void Close();
	// Returns if close has been called by us yet
	bool Closed() const;

	// Returns a pointer to a display context
	DCRESULT GetDisplayPointer(const int iValue, void *&pData) const;

protected:
//#===--- Internal Structures

//#===--- Internal Functions

//#===--- Internal Data
	// These are required by the screensaver
	HINSTANCE m_hInstance;				// Application instance
	HWND m_hWND;							// Window handle
	HDC m_hDC;								// Window device context
	SAMODE m_eMode;						// Saver operating mode

	unsigned int m_uPassDelay;			// Password delay in seconds
	unsigned int m_uStartTime;			// Time when timer started in ms
	unsigned int m_uTimerID;			// Windows timer ID

	bool m_bDialog;						// Dialog status indicator

	POINT m_sPos;							// Saved cursor position
	int m_iMoveThresh;					// Movement threshold

	bool m_bClose;							// Close called indicator
};

//#===--- Inline Functions

inline SAMODE CSaverSettings::GetMode() const {
	return m_eMode;
} // GetMode

inline DCRESULT CSaverSettings::GetDisplayPointer(const int iValue, void *&pData) const {
	pData = GetDC(m_hWND);
	return DC_OK;
} // GetDisplayPointer

inline void CSaverSettings::SetInstance(HINSTANCE hInstance) {
	m_hInstance = hInstance;
} // SetInstance

inline HINSTANCE CSaverSettings::GetInstance() const {
	return m_hInstance;
} // GetInstance

inline void CSaverSettings::SetHWND(HWND hWND) {
	m_hWND = hWND;
} // GetHWND

inline HWND CSaverSettings::GetHWND() const {
	return m_hWND;
} // GetHWND

inline bool CSaverSettings::DialogActive() const {
	return m_bDialog;
} // DialogActive

inline void CSaverSettings::Close() {
	m_bClose = true;
	PostMessage(m_hWND, WM_CLOSE, 0, 0);
} // Close

inline bool CSaverSettings::Closed() const {
	return m_bClose;
} // Closed

#endif // _SAVER_SETTINGS_
