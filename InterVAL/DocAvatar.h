//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// DocAvatar.h - 23/11/2000 - Warren Moore
//	  InterVAL Avatar document type
//
// $Id: DocAvatar.h,v 1.1 2000/11/25 22:52:26 cvs Exp $
//

#ifndef _INTERVAL_DOCAVATAR_
#define _INTERVAL_DOCAVATAR_

#pragma once

#include "Avatar.h"

class CDocAvatar : public CDocument {
//#===--- External Functions
public:
	virtual ~CDocAvatar();

	// Returns the current avatar pointer
	CAvatar *GetAvatar(bool &bGeneric) const;

//#===--- Internal Functions
protected: // create from serialization only
	CDocAvatar();
	DECLARE_DYNCREATE(CDocAvatar)

//#===--- Internal Data
protected:
	CAvatar *m_poAvatar;									// Loaded avatar
	bool m_bGeneric;											// Generic avatar indicator

//#===--- Diagnostics
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

//#===--- MFC Mapping

	//{{AFX_VIRTUAL(CDocAvatar)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CDocAvatar)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // _INTERVAL_DOCAVATAR_
