//======---
// Saver
//------
// Vapour Technology Screen Saver
// Copyright 2000 Vapour Technology Ltd.
//
// Saver.h - 26/11/2000 - Warren Moore
//	  Main screen saver Windows source
//
// $Id: Saver.h,v 1.2 2000/11/30 11:03:21 warren Exp $
//

#ifndef _SAVER_WINMAIN_
#define _SAVER_WINMAIN_

#pragma once

//#===--- Windows Setup

#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <regstr.h>

#pragma hdrstop

#include "resource.h"

//#===--- Functions

// Main Loop
int WINAPI WinMain(HINSTANCE h, HINSTANCE ,LPSTR ,int);

// Creates the scene before the saver message loop begins
void CreateScene();

// Cleans up the scene
void DestroyScene();

// Starts the screen saver
void StartSaver();

// Checks the password
bool VerifyPassword();

// Windows 95 call to change the password
void ChangePassword();

// Message loop callback function
LRESULT CALLBACK SaverWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Dialog execute callback function
BOOL CALLBACK ConfigDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Runs the screen saver within the ScrPrev app (Debug build only)
#ifdef _DEBUG
HWND CheckForScrprev();
#endif _DEBUG

#endif // _SAVER_WINMAIN_