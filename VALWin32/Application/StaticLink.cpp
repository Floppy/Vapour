//====---
// VAL
//----
// Avatar editor and exporter
// Copyright 2000 Vapour Technology Ltd.
//
// StaticLink.cpp - 12/04/2000 - Warren Moore
//	Dialog URL implementation
//
// $Id: StaticLink.cpp,v 1.1 2000/07/10 21:40:23 waz Exp $
//

#include "StdAfx.h"

#include "StaticLink.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////
// CStaticLink

//#===--- Static variables

COLORREF CStaticLink::g_iColourUnvisited = RGB( 68, 68,124);
COLORREF CStaticLink::g_iColourVisited   = RGB( 24, 24, 49);

HCURSOR	CStaticLink::g_hCursorLink = NULL;

IMPLEMENT_DYNAMIC(CStaticLink, CStatic)

BEGIN_MESSAGE_MAP(CStaticLink, CStatic)
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

CStaticLink::CStaticLink(LPCTSTR lpText, BOOL bDeleteOnDestroy) {
	m_strURL = lpText;
	m_iColour = g_iColourUnvisited;
	m_bDeleteOnDestroy = bDeleteOnDestroy;
} // Constructor

UINT CStaticLink::OnNcHitTest(CPoint point) {
	return HTCLIENT;
} // OnNCHitTest

HBRUSH CStaticLink::CtlColor(CDC* pDC, UINT nCtlColor) {
	ASSERT(nCtlColor == CTLCOLOR_STATIC);
	DWORD dwStyle = GetStyle();
	
	HBRUSH hbr = NULL;
	if ((dwStyle & 0xFF) <= SS_RIGHT) {
	// This is a text control so set up font and colors
		if (!(HFONT)m_oFont) {
		// If first time create font
			LOGFONT lf;
			GetFont()->GetObject(sizeof(lf), &lf);
			lf.lfUnderline = TRUE;
			m_oFont.CreateFontIndirect(&lf);
		}

	// Use underline font and visited/unvisited colors
		pDC->SelectObject(&m_oFont);
		pDC->SetTextColor(m_iColour);
		pDC->SetBkMode(TRANSPARENT);

	// Return hollow brush to preserve parent background color
		hbr = (HBRUSH)::GetStockObject(HOLLOW_BRUSH);
	}
	return hbr;
} // CtlColor

void CStaticLink::OnLButtonDown(UINT nFlags, CPoint point) {
// Get the URL text
	!m_strURL.IsEmpty() || m_strURL.LoadString(GetDlgCtrlID()) || (GetWindowText(m_strURL),1);
// Die if no text available
	if (m_strURL.IsEmpty())
		return;

// Open the URL
	HINSTANCE h = ShellExecute(0, _T("open"), (LPCTSTR)m_strURL, 0, 0, SW_SHOWNORMAL);
	if ((UINT)h > 32) {									// success!
		m_iColour = g_iColourVisited;			// change color
		Invalidate();											// repaint 
	}
	else {
		MessageBeep(0);										// unable to execute file!
		TRACE("CStaticLink - unable to navigate link %s\n", (LPCTSTR)m_strURL);
	}
} // OnLButtonDown

BOOL CStaticLink::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {
// If cursor not set
	if (g_hCursorLink == NULL) {
		static bTriedOnce = FALSE;
	// Look for it inside winhlp32
		if (!bTriedOnce) {
			CString windir;
			GetWindowsDirectory(windir.GetBuffer(MAX_PATH), MAX_PATH);
			windir.ReleaseBuffer();
			windir += _T("\\winhlp32.exe");
			HMODULE hModule = LoadLibrary(windir);
			if (hModule) {
				g_hCursorLink = CopyCursor(::LoadCursor(hModule, MAKEINTRESOURCE(106)));
			}
			FreeLibrary(hModule);
			bTriedOnce = TRUE;
		}
	}
// If found now, set it
	if (g_hCursorLink) {
		::SetCursor(g_hCursorLink);
		return TRUE;
	}
	return FALSE;
} // OnSetCursor

void CStaticLink::PostNcDestroy() {
	if (m_bDeleteOnDestroy)
		delete this;
} // PostNcDestroy
