//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2000 Vapour Technology Ltd.
//
//! docentry  = "Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/UIDataPath.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: UIDataPath.h,v 1.3 2002/04/03 11:14:32 vap-james Exp $"

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

   DWORD m_uiDataRead;              // Data read so far

//#===--- Windows Mappings

public:
	//{{AFX_VIRTUAL(CUIDataPath)
   public:
	protected:
	virtual void OnDataAvailable(DWORD dwSize, DWORD bscfFlag);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUIDataPath)
	//}}AFX_MSG

};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTUCVIS_UIDATAPATH__
