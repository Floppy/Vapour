//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.cpp
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.cpp,v 1.1 2002/03/22 16:54:43 vap-warren Exp $

#include "stdafx.h"
#include "vtstrucvis.h"
#include "SimDataPath.h"

#include "VTStrucVisCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////
// CSimDataPath

CSimDataPath::CSimDataPath() {
}

CSimDataPath::~CSimDataPath() {
}

#if 0
BEGIN_MESSAGE_MAP(CSimDataPath, CDataPathProperty)
	//{{AFX_MSG_MAP(CSimDataPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////
// Message Handlers

void CSimDataPath::OnDataAvailable(DWORD dwSize, DWORD bscfFlag)  {
   // First call
   if (bscfFlag & BSCF_FIRSTDATANOTIFICATION > 0) {
      ((CVTStrucVisCtl*)GetControl())->InitCortona();
   }
	
	CDataPathProperty::OnDataAvailable(dwSize, bscfFlag);
}
