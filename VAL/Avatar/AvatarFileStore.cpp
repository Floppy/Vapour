//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// AvatarFileStore.cpp - James Smith
//	Avatar import/export filter speciality store implementation
//
// $Id: AvatarFileStore.cpp,v 1.1 2000/06/16 21:58:16 waz Exp $
//

#include "stdafx.h"

#include "AvatarFileStore.h"
#include "AvatarFileProxy.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////
// CAvatarFileStore

const double CAvatarFileStore::m_dMaxFilterVersion = 2.0;

/////////////////////
// Member Functions

CAvatarFileStore::~CAvatarFileStore() {
	if (m_pAvatarFiles!=NULL) delete m_pAvatarFiles;
} // Destructor

void CAvatarFileStore::Init() {
	if (!m_pAvatarFiles)
		m_pAvatarFiles = new AvatarFileVector;
} // Init

void CAvatarFileStore::Register(const CAvatarFileProxyBase* pProxy) {
   if (m_pParentStore) m_pParentStore->Register(pProxy);
   else {
      Init();
      m_pAvatarFiles->push_back(pProxy);
   }
   return;
} // Register

void CAvatarFileStore::Override(CAvatarFileStore &oAvatarFileStore) {
   int iNumFilters = GetCount();
   for (int i=0; i<iNumFilters; i++) {
      oAvatarFileStore.Register(GetAt(i));
   }
   delete m_pAvatarFiles;
   m_pAvatarFiles = NULL;
   m_pParentStore = &oAvatarFileStore;
   return;
} // Override

int CAvatarFileStore::GetCount() const {
   if (m_pParentStore) return m_pParentStore->GetCount();
   else return (m_pAvatarFiles) ? m_pAvatarFiles->size() : 0;
} // GetCount

const CAvatarFileProxyBase* CAvatarFileStore::GetAt(int i) const {
   if (m_pParentStore) return m_pParentStore->GetAt(i);
	else return m_pAvatarFiles->at(i);
} // GetAt

bool CAvatarFileStore::CheckForExtension(const char* pszExtension) {
   if (m_pParentStore) return m_pParentStore->CheckForExtension(pszExtension);
   else {
      for (AvatarFileIterator iter = m_pAvatarFiles->begin(); iter != m_pAvatarFiles->end(); iter++) {
         const CAvatarFileProxyBase* pProxy = *iter;
		   if (stricmp(pProxy->GetExtension(), pszExtension) == 0)
			   return true;
      }
	   return false;
   }
} // CheckForExtension

CAvatarFile* CAvatarFileStore::CreateByExtension(const char* pszExtension) {
   if (m_pParentStore) return m_pParentStore->CreateByExtension(pszExtension);
   else {
      AvatarFileIterator iterSelected = NULL;
      double dVersion = 0.0;
      for (AvatarFileIterator iter = m_pAvatarFiles->begin(); iter != m_pAvatarFiles->end(); iter++) {
         const CAvatarFileProxyBase* pProxy = *iter;
         if ((stricmp(pProxy->GetExtension(), pszExtension) == 0)
             && (pProxy->GetVersion() < m_dMaxFilterVersion)
             && (pProxy->GetVersion() > dVersion)) {
            dVersion = pProxy->GetVersion();
   		   iterSelected = iter;
         }
      }
      if (iterSelected) return (*iterSelected)->CreateObject();
      else return NULL;
   }
} // CreateByExtension

bool CompareFilters(const CAvatarFileProxyBase* pFilterOne ,const CAvatarFileProxyBase* pFilterTwo) {
   bool bRetVal = true;
   CString strOne = pFilterOne->GetTitle();
   CString strTwo = pFilterTwo->GetTitle();
   if (strOne == strTwo) {
      // Compare extensions
      strOne = pFilterOne->GetExtension();
      strTwo = pFilterTwo->GetExtension();
      bRetVal = (strOne < strTwo) ? true : false;
   }
   else {
      // Compare titles
      bRetVal = (strOne < strTwo) ? true : false;
   }
   return bRetVal;
} //CompareFilters(const CAvatarFileProxyBase* pFilterOne ,const CAvatarFileProxyBase* pFilterTwo)