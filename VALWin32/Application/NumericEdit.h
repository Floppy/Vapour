//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// NumericEdit.h - 13/01/2000 - Warren Moore
//	Numeric Edit control definition
//
// $Id: NumericEdit.h,v 1.0 2000/08/13 11:02:48 waz Exp $
//

#ifndef _VAL_NUMERICEDIT_
#define _VAL_NUMERICEDIT_

#pragma once

//////////////////
// CNumericEdit
//
// To use this class, create a CEdit control in a dialog, and
// create a member variable of CNumericEdit in the dialog's  class.
// Use this classes SubClassDlgItem() to dynamically subclass
// the CEdit control.

class CNumericEdit : public CEdit
{
// Construction
public:
	CNumericEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumericEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	void setNumEntries(int num);
	virtual ~CNumericEdit();

	// Generated message map functions
protected:
	int iNumEntries;
	bool validChar(char cInput);
	//{{AFX_MSG(CNumericEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg LRESULT OnPaste(WPARAM Wparam, LPARAM LParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // _VAL_NUMERICEDIT_
