//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// FolderDialog.cpp - 13/01/2000 - Warren Moore
//	Utility class for Windows directory selection
//
// $Id: FolderDialog.cpp,v 1.1 2000/07/10 21:40:23 waz Exp $
//

#include "stdafx.h"
#include "FolderDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int CALLBACK BrowseDirectoryCallback(HWND hWnd, UINT uMsg,
																						LPARAM lParam, LPARAM lpData) {
// Context was pointer to CFolderDialog instance
	CFolderDialog* pFd = (CFolderDialog*)lpData;

// Let the class handle it
	pFd->CallbackFunction(hWnd, uMsg, lParam);

// Always return 0 as per SHBrowseFolder spec
	return 0;
}

//////////////////
// CFolderDialog

CFolderDialog::CFolderDialog(LPCTSTR lpszFolderName, DWORD dwFlags, 
														 CWnd* pParentWnd, LPCTSTR pszFileFilter) {
// Use the supplied initial folder if non-null.
	if (lpszFolderName == NULL)
		m_strInitialFolderName = _T("");
	else
		m_strInitialFolderName = lpszFolderName;

	memset(&m_bi, '\0', sizeof(BROWSEINFO));

	if (pParentWnd == NULL)
		m_bi.hwndOwner = 0;
	else
		m_bi.hwndOwner = pParentWnd->GetSafeHwnd();

// Fill in the rest of the structure
	m_bi.pidlRoot = NULL;
	m_bi.pszDisplayName = m_szDisplayName;
	m_bi.lpszTitle = _T("Current Selection");
	m_bi.ulFlags = dwFlags | BIF_STATUSTEXT;
	m_bi.lpfn = BrowseDirectoryCallback;
	m_bi.lParam = (LPARAM)this;

// Save the file filter
	if (pszFileFilter == NULL)
		m_strFileFilter = _T("*.*");
	else
		m_strFileFilter = pszFileFilter;
} // Constructor

CFolderDialog::~CFolderDialog() {
} // Destructor

void CFolderDialog::CallbackFunction(HWND hWnd, UINT uMsg,	LPARAM lParam) {
// Save the window handle. The Set* functions need it and they may
// be called by the virtual funtions.
	m_hDialogBox = hWnd;

// Dispatch the two message types to the virtual functions
	switch (uMsg) {
	case BFFM_INITIALIZED:
		OnInitDialog();
		break;
	case BFFM_SELCHANGED:
		OnSelChanged((ITEMIDLIST*) lParam);
		break;
	}
} // CallbackFunction

int CFolderDialog::DoModal() {
	int nReturn = IDOK;

// Initialize the result to the starting folder value
	m_strFinalFolderName = m_strInitialFolderName;

	ITEMIDLIST* piid = NULL;

// Call the shell function
	piid = ::SHBrowseForFolder(&m_bi);

// Process the result
	if (piid && ::SHGetPathFromIDList(piid, m_szPath)) {
		m_strFinalFolderName = m_szPath;
		nReturn = IDOK;
	}
	else {
		nReturn = IDCANCEL;
	}

// Release the ITEMIDLIST if we got one
	if (piid) {
		LPMALLOC lpMalloc;
		VERIFY(::SHGetMalloc(&lpMalloc) == NOERROR);
		lpMalloc->Free(piid);
		lpMalloc->Release();
	}

	return nReturn;
} // DoModal

CString CFolderDialog::GetPathName() const {
	return m_strFinalFolderName;
} // GetPathName

void CFolderDialog::EnableOK(BOOL bEnable) {
	::SendMessage(m_hDialogBox, BFFM_ENABLEOK, 0, (bEnable ? 1 : 0));
} // EnableOK

void CFolderDialog::SetSelection(LPCTSTR pszSelection) {
	::SendMessage(m_hDialogBox, BFFM_SETSELECTION, TRUE, (LPARAM) pszSelection);
} // SetSelection 

void CFolderDialog::SetSelection(ITEMIDLIST* pIdl) {
	::SendMessage(m_hDialogBox, BFFM_SETSELECTION, FALSE, (LPARAM) pIdl);
} // SetSelection 

void CFolderDialog::SetStatusText(LPCTSTR pszStatusText) {
	::SendMessage(m_hDialogBox, BFFM_SETSTATUSTEXT, 0, (LPARAM) pszStatusText);
} // SetStatusText

CString CFolderDialog::ShortName(const CString& strName) {
	CString strShort;
	if (strName.GetLength() <= 35)
		strShort = strName;
	else
		strShort = strName.Left(35) + _T("...");

	return strShort;
} // ShortName

void CFolderDialog::OnInitDialog() {
// Default handing of the init dialog message sets the selection to 
//	the initial folder
	SetSelection(m_strInitialFolderName);
	SetStatusText(ShortName(m_strInitialFolderName));
} // OnInitDialog

void CFolderDialog::OnSelChanged(ITEMIDLIST* pIdl) {
	::SHGetPathFromIDList(pIdl, m_szPath);
	m_strFinalFolderName = m_szPath;
	SetStatusText(ShortName(m_strFinalFolderName));

// Check that the file we're looking for it there
	CFileFind file;
	CString strFullFilter;
	strFullFilter = CString(m_szPath) + _T("\\") + m_strFileFilter;

	if (file.FindFile(strFullFilter)) {
		EnableOK(TRUE);
		file.Close();
	}
	else {
		EnableOK(FALSE);
	}
} // OnSelChanged
