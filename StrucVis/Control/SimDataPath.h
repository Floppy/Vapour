//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// SimDataPath.h
// 19/03/2002 - Warren Moore
//
// $Id: SimDataPath.h,v 1.1 2002/03/22 16:54:43 vap-warren Exp $

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

//#===--- Member Variables

//#===--- Windows Mappings

public:
	//{{AFX_VIRTUAL(CSimDataPath)
	protected:
	virtual void OnDataAvailable(DWORD dwSize, DWORD bscfFlag);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSimDataPath)
	//}}AFX_MSG

};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_SIMDATAPATH__
