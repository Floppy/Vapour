//=========---
// InterVAL
//---------
// Vapour Technology VAL Development Platform
// Copyright 2000 Vapour Technology Ltd.
//
// DocAvatar.cpp - 23/11/2000 - Warren Moore
//	  InterVAL Avatar document type
//
// $Id: DocAvatar.cpp,v 1.1.1.1 2000/11/25 22:52:26 cvs Exp $
//

#include "StdAfx.h"
#include "InterVAL.h"

#include "DocAvatar.h"

#include "AvatarFileStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#===---- Externally linked avatar file specialty store
extern CAvatarFileStore g_oAvatarFileStore;

///////////////
// CDocAvatar

CDocAvatar::CDocAvatar() :
	m_poAvatar(NULL),
	m_bGeneric(true) {
} // Constructor

CDocAvatar::~CDocAvatar() {
} // Destructor

CAvatar *CDocAvatar::GetAvatar(bool &bGeneric) const {
	bGeneric = m_bGeneric;
	return m_poAvatar;
} // GetAvatar

void CDocAvatar::DeleteContents() {
	// Delete the avatar if it exists
	if (m_poAvatar) {
		delete m_poAvatar;
		m_poAvatar = NULL;
		m_bGeneric = true;
	}
	
	CDocument::DeleteContents();
} // DeleteContents

BOOL CDocAvatar::OnNewDocument() {
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Make sure we don't have a live avatar
	ASSERT(m_poAvatar == NULL);

	// Load the generic avatar
	CAvatarFile *poFilter = g_oAvatarFileStore.CreateByExtension("generic");
	if (poFilter) {
		m_poAvatar = poFilter->Load(NULL);
		m_bGeneric = true;
		delete poFilter;
	}

	return TRUE;
} // OnNewDocument

BOOL CDocAvatar::OnOpenDocument(LPCTSTR lpszPathName) {
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// Get the extension
	char *pcTemp = (char*)lpszPathName, *pcExt = NULL;
	if (pcTemp) {
		do {
			if (*pcTemp == '.')
				pcExt = pcTemp + 1;
		} while (*pcTemp++ != '\0');
	}

	// Create the avatar filter 
	CAvatarFile *poFilter = NULL;
	if (pcExt) {
		poFilter = g_oAvatarFileStore.CreateByExtension(pcExt);
	}

	// Load the avatar
	if (poFilter) {
		m_poAvatar = poFilter->Load(lpszPathName);
		m_bGeneric = (m_poAvatar == NULL);
		delete poFilter;
	}

	return (m_poAvatar != NULL);
} // OnOpenDocument

void CDocAvatar::Serialize(CArchive& ar) {
	/* Do nothing
	if (ar.IsStoring()) {
	}
	else {
	}
	*/
} // Serialize

//#===--- MFC Mapping

IMPLEMENT_DYNCREATE(CDocAvatar, CDocument)

BEGIN_MESSAGE_MAP(CDocAvatar, CDocument)
	//{{AFX_MSG_MAP(CDocAvatar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//#===--- diagnostics

#ifdef _DEBUG
void CDocAvatar::AssertValid() const {
	CDocument::AssertValid();
} // AssertValid

void CDocAvatar::Dump(CDumpContext& dc) const {
	CDocument::Dump(dc);
} // Dump

#endif //_DEBUG


