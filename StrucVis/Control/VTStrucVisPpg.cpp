//===================================---
// VT Structure Visualisation Control
//-----------------------------------
// ActiveX Control for structure visualisation
// Copyright 2002 Vapour Technology Ltd.
//
//! docentry  = "VTStrucVis.Visualisation Control.ActiveX Control"
//! userlevel =  Normal
//! file      = "Control/VTStrucPpg.cpp"
//! author    = "Warren Moore"
//! date      = "5/3/2002"
//! rcsid     = "$Id: VTStrucVisPpg.cpp,v 1.4 2002/04/04 11:01:39 vap-warren Exp $"

#include "stdafx.h"
#include "VTStrucVis.h"
#include "VTStrucVisPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CVTStrucVisPropPage, COlePropertyPage)

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CVTStrucVisPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CVTStrucVisPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CVTStrucVisPropPage, "VTStructVis.PropPage.1",
	0xf3b5114f, 0x1633, 0x443d, 0x8f, 0x8, 0xa8, 0xff, 0xe, 0xb9, 0x3a, 0x72)

/////////////////////////////////////////////////////////////////////////////
// CVTStrucVisPropPage::CVTStrucVisPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CVTStrucVisPropPage

BOOL CVTStrucVisPropPage::CVTStrucVisPropPageFactory::UpdateRegistry(BOOL bRegister) {
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_VTSTRUCVIS_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CVTStrucVisPropPage::CVTStrucVisPropPage - Constructor

CVTStrucVisPropPage::CVTStrucVisPropPage() :
	COlePropertyPage(IDD, IDS_VTSTRUCVIS_PPG_CAPTION) {
	//{{AFX_DATA_INIT(CVTStrucVisPropPage)
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
// CVTStrucVisPropPage::DoDataExchange - Moves data between page and properties

void CVTStrucVisPropPage::DoDataExchange(CDataExchange* pDX) {
	//{{AFX_DATA_MAP(CVTStrucVisPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}

/////////////////////////////////////////////////////////////////////////////
// CVTStrucVisPropPage message handlers
