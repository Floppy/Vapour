//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// NumericEdit.h - 13/01/2000 - Warren Moore
//	Numeric Edit control definition
//
// $Id: NumericEdit.h,v 1.2 2000/11/29 17:28:10 james Exp $
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

class CNumericEdit : public CEdit {
public:
	CNumericEdit(int iNumEntries = 1);

	//{{AFX_VIRTUAL(CNumericEdit)
	//}}AFX_VIRTUAL

	void SetOptions(int iNumEntries = 1);

	virtual ~CNumericEdit();

protected:

	int m_iNumEntries;

	bool IsValid(char cInput);

	//{{AFX_MSG(CNumericEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
   afx_msg LRESULT OnPaste(WPARAM Wparam, LPARAM LParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _VAL_NUMERICEDIT_
