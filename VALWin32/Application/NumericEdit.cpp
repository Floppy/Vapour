//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// Scalar.cpp - 18/05/2000 - James Smith
//	Scalar value class implementation
//
// $Id: NumericEdit.cpp,v 1.2 2000/11/29 17:28:10 james Exp $
//

// Windows includes/defines
#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Standard includes
#include <math.h>

// Windows includes
#include <afxole.h>

// VAL includes
#include "NumericEdit.h"

/////////////////
// CNumericEdit

CNumericEdit::CNumericEdit(int iNumEntries) {
	SetOptions(iNumEntries);
   return;
} //CNumericEdit(int iNumEntries)

CNumericEdit::~CNumericEdit() {
   return;
} //~CNumericEdit()

//#===--- MFC Message Handling

BEGIN_MESSAGE_MAP(CNumericEdit, CEdit)
	//{{AFX_MSG_MAP(CNumericEdit)
	ON_WM_CHAR()
   ON_MESSAGE(WM_PASTE, OnPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) {
	if (IsValid(nChar)) {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
} //OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)

LRESULT CNumericEdit::OnPaste(WPARAM Wparam, LPARAM LParam) {
	COleDataObject oleObject;
	if (oleObject.AttachClipboard()) {
		if (oleObject.IsDataAvailable(CF_TEXT)) {
			CFile *inputData = oleObject.GetFileData(CF_TEXT);
			if (inputData) {
				CString str;
				LPSTR buffer = str.GetBufferSetLength(inputData->GetLength());
				inputData->Read(buffer,inputData->GetLength());
				delete inputData;
				str.ReleaseBuffer();
				str.FreeExtra();
				for (int i=0; i<str.GetLength(); i++) {
					char cInput = str.GetAt(i);
					int startPos, endPos;
					GetSel(startPos, endPos);
					if (IsValid(cInput)) {
						CString newString(cInput);
						ReplaceSel(newString);
					}
				}
			}
		}
	}
	return 1;
} //OnPaste(WPARAM Wparam, LPARAM LParam)

bool CNumericEdit::IsValid(char cInput) {
	bool bReturn = false;
	int startPos, endPos;
	GetSel(startPos, endPos);
	CString str;
	GetWindowText(str);
	str.Delete(startPos, endPos-startPos);
	char cNext = (startPos < str.GetLength()) ? str.GetAt(startPos) : ' ';
	char cPrevious = (startPos > 0) ? str.GetAt(startPos-1) : ' ';
	if (isdigit(cInput)) bReturn =  true;
	// Allow cut(3), copy(22), paste(24) and delete(8) keypresses through.
	else if ((cInput == 3) || (cInput == 22) || (cInput == 24) || (cInput == 8)) bReturn =  true;
	else if (m_iNumEntries == 1) {
		if ((cInput == '-') && ((startPos == 0) || (cPrevious == 'e') || (cPrevious == 'E')) && (cNext != '-')) {
			bReturn =  true;
		}
		else if (((cInput == 'e') || (cInput =='E')) && isdigit(cPrevious)) {
			CString str;
			GetWindowText(str);
			if ((str.Find('e') < 0) && (str.Find('E') < 0) && (startPos > str.Find('.'))) {
				bReturn =  true;
			}
		}
		else if (cInput == '.') {
			CString str;
			GetWindowText(str);
			if (str.Find('.') < 0) {
				if ((str.Find('e') < 0) && (str.Find('E') < 0)) bReturn = true;
				else if ((startPos <= str.Find('e')) || (startPos <= str.Find('E'))) bReturn = true;
			}
		}
	}
	else {
		if (((cInput == 'e') || (cInput =='E')) && isdigit(cPrevious)) {
			int prevSpace, nextSpace = 0;
			while ((nextSpace < startPos) && (nextSpace != -1)) {
				prevSpace = nextSpace;
				nextSpace = str.Find(' ', nextSpace+1);
			}
			if (nextSpace == -1) {
				nextSpace = str.GetLength();
			}
			CString substr = str.Mid(prevSpace, nextSpace - prevSpace);
			if ((substr.Find('e') < 0) && (substr.Find('E') < 0) && (startPos-prevSpace > str.Find('.'))) {
				bReturn =  true;
			}
		}
		else if (cInput == '.') {
			int prevSpace, nextSpace = 0;
			while ((nextSpace < startPos) && (nextSpace != -1)) {
				prevSpace = nextSpace;
				nextSpace = str.Find(' ', nextSpace+1);
			}
			if (nextSpace == -1) {
				nextSpace = str.GetLength();
			}
			CString substr = str.Mid(prevSpace, nextSpace - prevSpace);
			if (substr.Find('.') < 0) {
				if ((substr.Find('e') < 0) && (substr.Find('E') < 0)) bReturn = true;
				else if ((startPos-prevSpace <= substr.Find('e')) || (startPos-prevSpace <= substr.Find('E'))) bReturn = true;
			}
		}
		if ((cInput == '-') && ((cPrevious == ' ') || (cPrevious == 'e') || (cPrevious == 'E')) && (cNext != '-')) {
			bReturn =  true;
		}
		else if ((cInput == ' ') && (cPrevious != '-') && (cPrevious != '.') && (cPrevious != ' ') && (cPrevious != 'e') && (cPrevious != 'E')) {
			int numSpaces = 0;
			for (int i=0; i<str.GetLength(); i++) {
				if (str.GetAt(i) == ' ') numSpaces++;
			}
			if (numSpaces < (m_iNumEntries-1)) bReturn = true;
			else bReturn = false;
		}
	}
	return bReturn;
} //IsValid(char cInput)

void CNumericEdit::SetOptions(int iNumEntries) {
	m_iNumEntries = iNumEntries;
	return;
} //SetOptions(int iNumEntries)
