//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.h
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.h,v 1.5 2002/03/25 13:15:58 vap-warren Exp $

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

   void ShowFrame(unsigned int uiSeek, unsigned int uiLength);
   // Passes the loaded data into the control, or defers until all the data is in

//#===--- Member Variables
protected:

   unsigned int m_uiDataRead;                         // Data read so far
   bool m_bSetup;                                     // Scene setup complete indicator
   unsigned int m_uiFrameSeek, m_uiFrameLength;       // Data to be read for next frame

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

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_SIMDATAPATH__
