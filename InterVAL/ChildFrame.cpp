//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// ChildFrame.cpp - 23/11/2000 - Warren Moore
//	  Derived child frame window
//
// $Id: ChildFrame.cpp,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#include "StdAfx.h"
#include "InterVAL.h"

#include "ChildFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////
// CChildFrame

CChildFrame::CChildFrame() {
} // Constructor

CChildFrame::~CChildFrame() {
} // Destructor

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs) {
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	if (!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
} // PreCreateWindow

//#===--- MFC Mapping

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//#===--- Diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const {
	CMDIChildWnd::AssertValid();
} // AssertValid

void CChildFrame::Dump(CDumpContext& dc) const {
	CMDIChildWnd::Dump(dc);
} // Dump

#endif //_DEBUG
