//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// StaticLink.h - 12/04/2000 - Warren Moore
//	Dialog URL header
//
// $Id: StaticLink.h,v 1.1 2000/07/10 21:40:24 waz Exp $
//

#ifndef _STATICLINK_
#define _STATICLINK_

#pragma once

////////////////
// CStaticLink

class CStaticLink : public CStatic {
public:
	DECLARE_DYNAMIC(CStaticLink)
	CStaticLink(LPCTSTR lpText = NULL, BOOL bDeleteOnDestroy=FALSE);
	~CStaticLink() { }

// Text - if not set will be retreived from dialog
	CString	m_strURL;
	COLORREF m_iColour;

// Default colours
	static COLORREF g_iColourUnvisited;
	static COLORREF g_iColourVisited;

// Cursor for links - can be set or defautl retreived
	static HCURSOR g_hCursorLink;

protected:
// Font object
	CFont m_oFont;
// Delete when window destroyed indicator
	BOOL m_bDeleteOnDestroy;	

	virtual void PostNcDestroy();

//#===--- Message handlers
	DECLARE_MESSAGE_MAP()
	afx_msg UINT	OnNcHitTest(CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

#endif _STATICLINK_
