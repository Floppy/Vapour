//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.h
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.h,v 1.7 2002/03/27 11:45:14 vap-warren Exp $

#ifndef __VTSTUCVIS_SIMDATAPATH__
#define __VTSTUCVIS_SIMDATAPATH__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////
// CSimDataPath

class CSimDataPath : public CDataPathProperty {
public:
	CSimDataPath();
	virtual ~CSimDataPath();

//#===--- Member Functions
public:

   bool ShowFrame(const unsigned int uiFrame,
                  const unsigned int uiSeek,
                  const unsigned int uiLength);
   // Passes the loaded data into the control, or defers until all the data is in
   // Returns false if it has to defer

   bool Available(const unsigned int uiSeek, const unsigned int uiLength) const;
   // Returns true if the data is available immediately

   bool Waiting() const;
   // Returns true if not setup or waiting on a deferred frame

//#===--- Member Variables
protected:

   unsigned int m_uiDataRead;                                     // Data read so far
   bool m_bSetup;                                                 // Scene setup complete indicator
   unsigned int m_uiFrame, m_uiFrameSeek, m_uiFrameLength;        // Data to be read for next frame

//#===--- Windows Mappings

public:
	//{{AFX_VIRTUAL(CSimDataPath)
   public:
	protected:
	virtual void OnDataAvailable(DWORD dwSize, DWORD bscfFlag);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSimDataPath)
	//}}AFX_MSG

};

inline bool CSimDataPath::Waiting() const {
   return (!m_bSetup || (m_uiFrameLength > 0));
}

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_SIMDATAPATH__
