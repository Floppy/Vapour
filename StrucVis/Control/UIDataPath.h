//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/UIDataPath.h"
//! author    = "Warren Moore"
//! date      = "27/3/2002"
//! rcsid     = "$Id: UIDataPath.h,v 1.6 2002/04/04 21:03:37 vap-warren Exp $"

#ifndef __VTSTUCVIS_UIDATAPATH__
#define __VTSTUCVIS_UIDATAPATH__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: User interface data path property
// Management class for asynchronous loading of user interface bitmap

class CUIDataPath : public CDataPathProperty {
public:
	CUIDataPath();
   //: Constructor

	virtual ~CUIDataPath();
   //: Destructor

protected:

   DWORD m_uiDataRead;              //: Data read so far

//:-----
//: Windows Mappings

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
