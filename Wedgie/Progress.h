//=======---
// Wedgie
//-------
// Wedgie Archive Compiler
// Copyright 1999-2000 Vapour Technology Ltd.
//
// Progress.h - 02/12/1999 - Warren Moore
//	  Compilation progress dialog box
//
// $Id: Progress.h,v 1.2 2000/12/03 18:31:36 warren Exp $
//

#ifndef _WEDGIE_PROGRESS_
#define _WEDGIE_PROGRESS_

#pragma once

//////////////
// CProgress

class CProgress : public CDialog {
public:
	CProgress(CWnd* pParent = NULL);

	BOOL Create();

	void SetFileName(const char *pText);
	void SetFileSize(unsigned int iSize);
	void SetFilePos(unsigned int iPos);
	void SetTotalSize(unsigned int iSize);
	void SetTotalPos(unsigned int iPos);

// Dialog Data
	//{{AFX_DATA(CProgress)
	enum { IDD = IDD_PROGRESS };
	CProgressCtrl	m_oTotalBar;
	CProgressCtrl	m_oFileBar;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL
	
	//{{AFX_MSG(CProgress)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	unsigned int m_iFileSize;
	unsigned int m_iTotalSize;

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _WEDGIE_PROGRESS_
