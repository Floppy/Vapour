//====---
// VAL
//----
// Vapour Technology All-Purpose Library
// Copyright 2000 Vapour Technology Ltd.
//
// RenderContextStore.cpp - 05/10/2000 - Warren Moore
//	Render context selection speciality store implementation
//
// $Id: RenderContextStore.cpp,v 1.0 2000/10/05 20:16:38 waz Exp $
//

#include "stdafx.h"

#include "RenderContextStore.h"
#include "RenderContextProxy.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////
// CRenderContextStore

const double CRenderContextStore::m_dMaxFilterVersion = 2.0;

/////////////////////
// Member Functions

CRenderContextStore::~CRenderContextStore() {
	if (m_pRenderContexts!=NULL) delete m_pRenderContexts;
} // Destructor

void CRenderContextStore::Init() {
	if (!m_pRenderContexts)
		m_pRenderContexts = new RenderContextVector;
} // Init

void CRenderContextStore::Register(const CRenderContextProxyBase* pProxy) {
   if (m_pParentStore) m_pParentStore->Register(pProxy);
   else {
      Init();
      m_pRenderContexts->push_back(pProxy);
   }
   return;
} // Register

void CRenderContextStore::Override(CRenderContextStore &oRenderContextStore) {
   int iNumFilters = GetCount();
   for (int i=0; i<iNumFilters; i++) {
      oRenderContextStore.Register(GetAt(i));
   }
   delete m_pRenderContexts;
   m_pRenderContexts = NULL;
   m_pParentStore = &oRenderContextStore;
   return;
} // Override

int CRenderContextStore::GetCount() const {
   if (m_pParentStore) return m_pParentStore->GetCount();
   else return (m_pRenderContexts) ? m_pRenderContexts->size() : 0;
} // GetCount

const CRenderContextProxyBase* CRenderContextStore::GetAt(int i) const {
   if (m_pParentStore) return m_pParentStore->GetAt(i);
	else return m_pRenderContexts->at(i);
} // GetAt

/*
bool CRenderContextStore::CheckForExtension(const char* pszExtension) {
   if (m_pParentStore) return m_pParentStore->CheckForExtension(pszExtension);
   else {
      for (RenderContextIterator iter = m_pRenderContexts->begin(); iter != m_pRenderContexts->end(); iter++) {
         const CRenderContextProxyBase* pProxy = *iter;
		   if (stricmp(pProxy->GetExtension(), pszExtension) == 0)
			   return true;
      }
	   return false;
   }
} // CheckForExtension

CRenderContext* CRenderContextStore::CreateByExtension(const char* pszExtension) {
   if (m_pParentStore) return m_pParentStore->CreateByExtension(pszExtension);
   else {
      RenderContextIterator iterSelected = NULL;
      double dVersion = 0.0;
      for (RenderContextIterator iter = m_pRenderContexts->begin(); iter != m_pRenderContexts->end(); iter++) {
         const CRenderContextProxyBase* pProxy = *iter;
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
*/

bool CompareFilters(const CRenderContextProxyBase* pFilterOne ,const CRenderContextProxyBase* pFilterTwo) {
   bool bRetVal = true;
   CString strOne = pFilterOne->GetTitle();
   CString strTwo = pFilterTwo->GetTitle();
	/*
   if (strOne == strTwo) {
      // Compare extensions
      strOne = pFilterOne->GetExtension();
      strTwo = pFilterTwo->GetExtension();
      bRetVal = (strOne < strTwo) ? true : false;
   }
   else {
	*/
      // Compare titles
      bRetVal = (strOne < strTwo) ? true : false;
   //}
   return bRetVal;
} //CompareFilters(const CRenderContextProxyBase* pFilterOne ,const CRenderContextProxyBase* pFilterTwo)
