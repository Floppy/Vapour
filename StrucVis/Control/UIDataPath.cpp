//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// UIDataPath.cpp
// 19/03/2002 - Warren Moore
//
// $Id: UIDataPath.cpp,v 1.1 2002/03/23 21:18:35 vap-warren Exp $

#include "stdafx.h"
#include "vtstrucvis.h"
#include "UIDataPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////
// CUIDataPath

CUIDataPath::CUIDataPath() {
}

CUIDataPath::~CUIDataPath() {
}

#if 0
BEGIN_MESSAGE_MAP(CUIDataPath, CDataPathProperty)
	//{{AFX_MSG_MAP(CUIDataPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////
// Message Handlers

void CUIDataPath::OnDataAvailable(DWORD dwSize, DWORD bscfFlag) {
   // ... insert here ...
	
	CDataPathProperty::OnDataAvailable(dwSize, bscfFlag);
}

