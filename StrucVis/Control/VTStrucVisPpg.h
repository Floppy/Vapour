//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/VTStrucPpg.h"
//! author    = "Warren Moore"
//! date      = "5/3/2002"
//! rcsid     = "$Id: VTStrucVisPpg.h,v 1.5 2002/04/04 21:03:38 vap-warren Exp $"

#ifndef __VTSTRUCTVIS_PROPPAGE__
#define __VTSTRUCTVIS_PROPPAGE__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//: Property page dialog

class CVTStrucVisPropPage : public COlePropertyPage {
//:-----
//: Construction/Destruction

	DECLARE_DYNCREATE(CVTStrucVisPropPage)
	DECLARE_OLECREATE_EX(CVTStrucVisPropPage)
public:

	CVTStrucVisPropPage();
   //: Dynamic construction

//:-----
//: Windows Mappings

	//{{AFX_DATA(CVTStrucVisPropPage)
	enum { IDD = IDD_PROPPAGE_VTSTRUCVIS };
	//}}AFX_DATA

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	//{{AFX_MSG(CVTStrucVisPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}

#endif // __VTSTRUCTVIS_PROPPAGE__
