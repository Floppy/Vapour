//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// FolderDialog.h - 13/01/2000 - Warren Moore
//	Utility class for Windows directory selection
//
// $Id: FolderDialog.h,v 1.1 2000/07/10 21:40:22 waz Exp $
//

#ifndef _FOLDERDIALOG_
#define _FOLDERDIALOG_

#pragma once

#include <shlobj.h>

//////////////////
// CFolderDialog

class CFolderDialog {
public:
	CFolderDialog(	LPCTSTR lpszFolderName = NULL, 
					DWORD dwFlags = NULL/*BIF_RETURNONLYFSDIRS*/, 
					CWnd* pParentWnd = NULL,
					LPCTSTR pszFileFilter = NULL);
	virtual ~CFolderDialog();
	virtual int DoModal();
	CString GetPathName() const;

protected:
	virtual void OnInitDialog();
	virtual void OnSelChanged(ITEMIDLIST* pIdl);
	virtual void CallbackFunction(HWND hWnd, UINT uMsg,	LPARAM lParam);

	void EnableOK(BOOL bEnable = TRUE);
	void SetSelection(LPCTSTR pszSelection);
	void SetSelection(ITEMIDLIST* pIdl);
	void SetStatusText(LPCTSTR pszStatusText);
	CString ShortName(const CString& strName);

public:
	BROWSEINFO m_bi;

//#===--- Internal Data
protected:
	CString m_strInitialFolderName;
	CString m_strFinalFolderName;

	TCHAR m_szDisplayName[MAX_PATH];
	TCHAR m_szPath[MAX_PATH];

	HWND m_hDialogBox;

	CString m_strFileFilter;

//#===--- Friend Functions
friend static int CALLBACK BrowseDirectoryCallback(
				HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

};

#endif // _FOLDERDIALOG_
