//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.cpp
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.cpp,v 1.6 2002/03/25 02:34:54 vap-warren Exp $

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
   m_uiDataRead(0),
   m_bSetup(false) {
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
      m_bSetup = false;
   }

   // How much data came this time?
   DWORD uiArriving = dwSize - m_uiDataRead;
   // Only pump in new data if the scene isn't set up yet
   if (!m_bSetup && (uiArriving > 0)) {
      // Allocate the memory
      unsigned char *pucData = (unsigned char*) new unsigned char[uiArriving];
      if (pucData) {
         // Seek to the correct point in the file
         Seek(m_uiDataRead, CFile::begin);
         // Read in the data
         unsigned int uiRead = Read((void*) pucData, uiArriving);
         if (uiRead == uiArriving) {
            m_bSetup = ((CVTStrucVisCtl*)GetControl())->SceneSetup(pucData, uiArriving);
         }
         // Delete the data
         delete [] pucData;
      }
      // Update read so far
      m_uiDataRead = dwSize;
   }

   // Last call, so signal file loaded
   if (bscfFlag & BSCF_LASTDATANOTIFICATION) {
      ((CVTStrucVisCtl*)GetControl())->SimLoaded();
   }
	
	CDataPathProperty::OnDataAvailable(dwSize, bscfFlag);
}

