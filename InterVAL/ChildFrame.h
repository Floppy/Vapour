//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// ChildFrame.h - 23/11/2000 - Warren Moore
//	  Derived child frame window
//
// $Id: ChildFrame.h,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_CHILDFRAME_
#define _INTERVAL_CHILDFRAME_

#pragma once

class CChildFrame : public CMDIChildWnd {
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();
	virtual ~CChildFrame();

//#===--- Diagnostics

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//#===--- MFC Mapping

	//{{AFX_VIRTUAL(CChildFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CChildFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_CHILDFRAME_
