//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 1999 Vapour Technology Ltd.
//
// ImageFileStore.h - 21/12/1999 - Warren Moore
//	Image file format self-registering object store
//
// $Id: ImageFileStore.cpp,v 1.1 2000/06/16 21:59:46 waz Exp $
//

#include "stdafx.h"

#include "ImageFileStore.h"
#include "ImageFileProxy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////
// CImageFileStore

CImageFileStore::~CImageFileStore() {
	if (m_pImageFiles!=NULL) delete m_pImageFiles;
} // Destructor

void CImageFileStore::Init() {
	if (!m_pImageFiles)
		m_pImageFiles = new ImageFileVector;
} // Init

void CImageFileStore::Register(const CImageFileProxyBase *pProxy) {
   if (m_pParentStore) m_pParentStore->Register(pProxy);
   else {
      Init();
	   m_pImageFiles->push_back(pProxy);
   }
   return;
} // Register

void CImageFileStore::Override(CImageFileStore &oImageFileStore) {
   int iNumFilters = GetCount();
   for (int i=0; i<iNumFilters; i++) {
      oImageFileStore.Register(GetAt(i));
   }
   delete m_pImageFiles;
   m_pImageFiles = NULL;
   m_pParentStore = &oImageFileStore;
   return;
} // Override

int CImageFileStore::GetCount() const {
   if (m_pParentStore) return m_pParentStore->GetCount();
   else return (m_pImageFiles) ? m_pImageFiles->size() : 0;
} // GetCount

const CImageFileProxyBase *CImageFileStore::GetAt(int i) const {
	if (m_pParentStore) return m_pParentStore->GetAt(i);
   else return m_pImageFiles->at(i);
} // GetAt

bool CImageFileStore::CheckForExtension(const char *pszExtension) {
	if (m_pParentStore) return m_pParentStore->CheckForExtension(pszExtension);
   else {
	   for (unsigned int i=0; i < m_pImageFiles->size(); i++) 
		   if (stricmp(m_pImageFiles->at(i)->GetExtension(), pszExtension) == 0)
			   return true;

	   return false;
   }
} // CheckForExtension

CImageFile *CImageFileStore::CreateByExtension(const char *pszExtension) {
	if (m_pParentStore) return m_pParentStore->CreateByExtension(pszExtension);
   else {
	   for (unsigned int i=0; i < m_pImageFiles->size(); i++) 
		   if (stricmp(m_pImageFiles->at(i)->GetExtension(), pszExtension) == 0)
			   return m_pImageFiles->at(i)->CreateObject();

	   return NULL;
   }
} // CreateByExtension

