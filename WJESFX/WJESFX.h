//=======---
// WJESFX
//-------
// Avatar Self-Extractor for Games
// Copyright 2000 Vapour Technology Ltd.
//
// WJESFX.h - 11/07/2000 - Warren Moore
//	Application and installation
//
// $Id: WJESFX.h,v 1.3 2000/07/11 19:20:25 waz Exp $
//

#ifndef _WJESFX_
#define _WJESFX_

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"

#include <fstream.h>

// Predeclared classes
class CProgressDlg;

//#===--- Game dependent definitions

#define GAME_SIMS
//#define GAME_HALFLIFE
//#define GAME_UNREALTOURNAMENT

// The Sims
#ifdef GAME_SIMS
#define GAME_NAME					"The Sims"
#define GAME_INSTALL			InstallSims
#endif // GAME_SIMS

// Half Life
#ifdef GAME_HALFLIFE
#define GAME_NAME					"Half Life"
#define GAME_INSTALL			InstallHL
#endif // GAME_HALFLIFE

// Unreal Tournament
#ifdef GAME_UNREALTOURNAMENT
#define GAME_NAME					"Unreal Tournament"
#define GAME_INSTALL			InstallUT
#endif // GAME_UNREALTOURNAMENT

// Check a game has been specified
#ifndef GAME_NAME
#error "Game build not specified"
#endif // GAME_NAME

///////////////
// CWJESFXApp

class CWJESFXApp : public CWinApp {
public:
	CWJESFXApp();

	// Game dependent installation functions
	void InstallSims(CProgressDlg *poDlg);
	void InstallHL(CProgressDlg *poDlg);
	void InstallUT(CProgressDlg *poDlg);

	//{{AFX_VIRTUAL(CWJESFXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CWJESFXApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WJESFX_
