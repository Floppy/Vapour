//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.h
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.h,v 1.3 2002/03/24 01:55:46 vap-warren Exp $

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

//#===--- Member Variables
protected:

   DWORD m_uiDataRead;              // Data read so far

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
