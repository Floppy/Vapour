//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scene.cpp - 06/10/2000 - Warren Moore
//	Base class for scene rendering classes
//
// $Id: Scene.cpp,v 1.1 2000/12/02 07:39:54 warren Exp $
//

#include "StdAfx.h"

#include "VAL.h"
#include "Scene.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////
// CScene

const char *CScene::m_pcErrorString[] = {
	SC_ERROR_STRINGS
};

CScene::CScene(CDisplayContext *poDisplay) :
	m_poDisplay(poDisplay),
	m_poRC(NULL) {
} // Contructor

CScene::~CScene() {
} // Destructor

