//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.cpp
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.cpp,v 1.5 2002/03/24 21:55:51 vap-warren Exp $

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

CSimDataPath::CSimDataPath() :
   m_uiDataRead(0) {
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
   // First call, so signal file loading
   if (bscfFlag & BSCF_FIRSTDATANOTIFICATION) {
      ((CVTStrucVisCtl*)GetControl())->SimLoading();
      m_uiDataRead = 0;
   }

   // Sort out streaming data
   DWORD uiArriving = dwSize - m_uiDataRead;
   if (uiArriving > 0) {
      // Set it immediately so we can test the interactive UI
      ((CVTStrucVisCtl*)GetControl())->GoInteractive();
      // Update read so far
      m_uiDataRead = dwSize;
   }

   // Last call, so signal file loaded
   if (bscfFlag & BSCF_LASTDATANOTIFICATION) {
      ((CVTStrucVisCtl*)GetControl())->SimLoaded();
   }
	
	CDataPathProperty::OnDataAvailable(dwSize, bscfFlag);
}

