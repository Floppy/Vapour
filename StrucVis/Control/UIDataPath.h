//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
// UIDataPath.h
// 19/03/2002 - Warren Moore
//
// $Id: UIDataPath.h,v 1.1 2002/03/23 21:18:35 vap-warren Exp $

#ifndef __VTSTUCVIS_UIDATAPATH__
#define __VTSTUCVIS_UIDATAPATH__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////////////////
// CUIDataPath

class CUIDataPath : public CDataPathProperty {
public:
	CUIDataPath();
	virtual ~CUIDataPath();

//#===--- Member Functions
public:

//#===--- Member Variables
protected:

//#===--- Windows Mappings

public:
	//{{AFX_VIRTUAL(CUIDataPath)
	protected:
	virtual void OnDataAvailable(DWORD dwSize, DWORD bscfFlag);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUIDataPath)
	//}}AFX_MSG

};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_UIDATAPATH__
