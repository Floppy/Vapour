//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// NumericEdit.h - 13/01/2000 - Warren Moore
//	Numeric Edit control definition
//
// $Id: NumericEdit.h,v 1.1 2000/08/13 11:05:43 waz Exp $
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
	CNumericEdit();

	//{{AFX_VIRTUAL(CNumericEdit)
	//}}AFX_VIRTUAL

	void setNumEntries(int num);
	virtual ~CNumericEdit();

protected:
	int iNumEntries;
	bool validChar(char cInput);
	//{{AFX_MSG(CNumericEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg LRESULT OnPaste(WPARAM Wparam, LPARAM LParam);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _VAL_NUMERICEDIT_
