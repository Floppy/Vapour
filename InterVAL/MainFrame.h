//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// MainFrame.h - 23/11/2000 - Warren Moore
//	   Derived main frame window
//
// $Id: MainFrame.h,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_MAINFRAME_
#define _INTERVAL_MAINFRAME_

#pragma once

class CMainFrame : public CMDIFrameWnd {
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

//#===--- Diagnostics

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

//#===--- MFC Mapping

	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_MAINFRAME_
