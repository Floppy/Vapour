//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/SimDataPath.cpp"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: SimDataPath.cpp,v 1.13 2002/04/04 11:01:36 vap-warren Exp $"

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
   m_bSetup(false),
   m_uiFrame(0),
   m_uiFrameSeek(0),
   m_uiFrameLength(0) {
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
      // Reset the vars
      m_uiDataRead = 0;
      m_bSetup = false;
      m_uiFrameSeek = m_uiFrameLength = 0;
   }

   // How much data came this time?
   unsigned int uiArriving = dwSize - m_uiDataRead;

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
   }

   // Are we waiting for a frame to load?
   if (m_bSetup && (m_uiFrameLength > 0) && (dwSize >= m_uiFrameSeek + m_uiFrameLength)) {
      // Allocate the memory
      unsigned char *pucData = (unsigned char*) new unsigned char[m_uiFrameLength];
      if (pucData) {
         // Seek to the correct point in the file
         Seek(m_uiFrameSeek, CFile::begin);
         // Read in the data
         unsigned int uiRead = Read((void*) pucData, m_uiFrameLength);
         if (uiRead == m_uiFrameLength) {
            ((CVTStrucVisCtl*)GetControl())->ShowFrame(m_uiFrame, pucData, m_uiFrameLength);
         }
         // Delete the data
         delete [] pucData;
         // Reset the frame info
         m_uiFrameSeek = m_uiFrameLength = 0;
      }
   }

   // Last call, so signal file loaded
   if (bscfFlag & BSCF_LASTDATANOTIFICATION) {
      ((CVTStrucVisCtl*)GetControl())->SimLoaded();
   }
	
   // Update read so far
   m_uiDataRead = dwSize;

   CDataPathProperty::OnDataAvailable(dwSize, bscfFlag);
}

bool CSimDataPath::ShowFrame(const unsigned int uiFrame, 
                             const unsigned int uiSeek,
                             const unsigned int uiLength) {
   bool bDefer = true;
   // Reset the referred frame info
   m_uiFrameSeek = m_uiFrameLength = 0;
   // Do we have enough data?
   if (m_uiDataRead >= uiSeek + uiLength) {
      // Allocate the memory
      unsigned char *pucData = (unsigned char*) new unsigned char[uiLength];
      if (pucData) {
         // Seek to the correct point in the file
         Seek(uiSeek, CFile::begin);
         // Read in the data
         unsigned int uiRead = Read((void*) pucData, uiLength);
         if (uiRead == uiLength) {
            ((CVTStrucVisCtl*)GetControl())->ShowFrame(uiFrame, pucData, uiLength);
            bDefer = false;
         }
         // Delete the data
         delete [] pucData;
      }
   }
   // Otherwise defer until it's arrived
   else {
      m_uiFrame = uiFrame;
      m_uiFrameSeek = uiSeek;
      m_uiFrameLength = uiLength;
   }
   return bDefer;
}

bool CSimDataPath::Available(const unsigned int uiSeek, const unsigned int uiLength) const {
   return (m_uiDataRead >= uiSeek + uiLength);
}

